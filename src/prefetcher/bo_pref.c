/***************************************************************************************
 * File         : bo_pref.c
 * Author       : Ella Lehavi and Rushali Ghosh
 * Date         : November 2024
 * Description  : Best Offset Prefetcher (https://ieeexplore.ieee.org/document/7446087)
 ***************************************************************************************/

#include "debug/debug_macros.h"
#include "debug/debug_print.h"
#include "globals/global_defs.h"
#include "globals/global_types.h"
#include "globals/global_vars.h"

#include "globals/assert.h"
#include "globals/utils.h"
#include "op.h"

#include "core.param.h"
#include "debug/debug.param.h"
#include "general.param.h"
#include "libs/hash_lib.h"
#include "libs/list_lib.h"
#include "memory/memory.h"
#include "memory/memory.param.h"
#include "memory/mem_req.h"
#include "prefetcher/bo_pref.h"
#include "prefetcher/pref_common.h"
#include "prefetcher/pref.param.h" //TODO: our own file instead?
#include "dcache_stage.h"




/***************************************************************************************/
/* Local Prototypes */
#define DEBUG(proc_id, args...) _DEBUG(proc_id, DEBUG_DCACHE_STAGE, ##args)
Pref_BO* bo;


static const int offset_list[] = {1, 2, 3, 4, 5, 6, 8, 9,
                                        10, 12, 15, 16, 18, 20, 24,
                                        25, 27, 30, 32, 36, 40, 45,
                                        48, 50, 54, 60, 64, 72, 75,
                                        80, 81, 90, 96, 100, 108, 120,
                                        125, 128, 135, 144, 150, 160,
                                        162, 180, 192, 200, 216, 225,
                                        240, 243, 250, 256 };


/*********************************************************************/

void init_prefetch_bo(HWP* hwp) {
    STAT_EVENT(0, BEST_OFFSET_COUNT);
    if(!PREF_BEST_OFFSET_ON) {
        return;}
    
    bo = (Pref_BO*)calloc(1, sizeof(Pref_BO));
    bo_pref_init(hwp, bo);
    /*
    how 2 check w gdb...
    - b bo_pref.c:line#
    - run with parameters (run --pref_best_offset_on)
    - print parameter (p PREF_BEST_OFFSET_ON)
    */
}

void bo_pref_init(HWP* hwp, Pref_BO* bo_prefetcher) {

    /*initialize hwp info*/
    bo_prefetcher->hwp_info = hwp->hwp_info;
    bo_prefetcher->hwp_info->enabled = TRUE;
    


    // rr table
    init_hash_table(&bo_prefetcher->rr_table, "RR Table",
                    256 /*TODO: how many buckets?*/,
                    4 /*TODO: what data size?*/);

    // score table
    //    bo_prefetcher->score_table = (uns*)malloc(51*sizeof(uns));

    for (int i = 0; i < 51; i++){
        bo_prefetcher->score_table[i] = 0;
    }
    for (int i = 0; i < 51; i++){
        bo_prefetcher->offset_list[i] = offset_list[i];
    }
    //bo_prefetcher->offset_list = offset_list;

    //TODO: malloc for these?
    bo_prefetcher->roundmax = BEST_OFFSET_ROUNDMAX;
    bo_prefetcher->scoremax = BEST_OFFSET_SCOREMAX;
    bo_prefetcher->curr_offset = 1;
    bo_prefetcher->round = 0;
    bo_prefetcher->badscore = 0;
}


/*********************************************************************/


void bo_dl0_miss(Addr lineAddr, Addr loadPC) {
    prefetch_round(bo, lineAddr);
}

void bo_dl0_pref_hit(Addr lineAddr, Addr loadPC) {
    prefetch_round(bo, lineAddr);
}



void prefetch_round(Pref_BO* prefetcher, Addr line_addr) {
    /*
    INPUTS: Address being accessed , offset tested, current ideal offset, RR table, Score table
    OUTPUTS:
    */
   uns big_d = prefetcher->curr_offset;
   uns d = 0;
   int success = 0;
   int i = 0;
   int offset_bits;
   int line_size = DCACHE_LINE_SIZE;
   int num_offsets = 51;

   STAT_EVENT(0, BEST_OFFSET_COUNT);

   /*STEP 1: prefetch memory at X+D (line_addr + big_d)*/
   //shift big_d by num offset bits
    while (line_size > 1)
    {
        line_size = line_size >> 1;
        offset_bits++;
    }
    // calculate the address by adding the current offset shifted by offset bits 2 the line address
    Addr pf_addr = line_addr + (big_d<<offset_bits);
    success = new_mem_req(MRT_DPRF, 0, pf_addr, DCACHE_LINE_SIZE,
                            0, NULL, update_rr_table, unique_count, 0);


    /* STEP 2: Handle one round in algorithm.*/
    if(prefetcher->round == 0) {
        //  set values in score table to 0
        for(i = 0; i < 51; i++) {
            prefetcher->score_table[i] = 0;
        }
    }

    for(i=0;i<num_offsets;i++) {
        // We've accessed line X. check for X-d in RR table
        d = prefetcher->offset_list[i];
        int addr_xd = line_addr - d;
        if(hash_table_access(&prefetcher->rr_table, addr_xd)) {
            prefetcher->score_table[i]++;
            if(prefetcher->score_table[i] >= prefetcher->scoremax){
                prefetcher->round = 0;
                prefetcher->curr_offset = prefetcher->offset_list[i];
                return;
            }
        }
    }

    // if it hits in RR table, score +=1
    // if score max, break out of both loops.
    prefetcher->round++;
    if (prefetcher->round >= prefetcher->roundmax) {
        // learning phase complete. select highest offset and set rounds to 0
        uns highest_score_idx = 0;
        for (int i = 0; i < 51; i++){
            if(prefetcher->score_table[i] > prefetcher->score_table[highest_score_idx]) {
                highest_score_idx = i;
                // TODO: If time, badscore?
            }
        }
        prefetcher->curr_offset = prefetcher->offset_list[highest_score_idx];
        prefetcher->round = 0;
    }
    return;
}

Flag update_rr_table(Mem_Req* req) {
    //Flag* return_flag = (Flag*)malloc(sizeof(Flag*));
    Flag return_flag = TRUE;
    Flag new_entry = FALSE;
    void* got;

    dcache_fill_line(req); // TODO dcache_fill_line ?
    got = hash_table_access_create(&bo->rr_table, req->addr, &new_entry);
    if (got==NULL){
            return_flag = FALSE;}
    return return_flag;
    

}

/* TODO: check
    - DCACHE miss/hit
    - IPC
*/