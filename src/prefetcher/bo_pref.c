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
#include "memory/memory.param.h"
#include "prefetcher//bo_pref.h"
// #include "prefetcher//pref_stride.param.h"
#include "prefetcher/pref.param.h"
#include "prefetcher/pref_common.h"
#include "statistics.h"

/**************************************************************************************/
/* Global Variables */

extern Memory*       mem;
extern Dcache_Stage* dc;
static Cache*        l1_cache;

/***************************************************************************************/
/* Local Prototypes */

int ROUNDMAX;
int SCOREMAX;
int i; // loop variable
int curr_offset; 
int ideal_offset;
int round;

Hash_Table rr_table;  
Hash_Table score_table;
static const int* const offset_list[] = {1, 2, 3, 4, 5, 6, 8, 9, 
                                        10, 12, 15, 16, 18, 20, 24, 
                                        25, 27, 30, 32, 36, 40, 45,
                                        48, 50, 54, 60, 64, 72, 75,
                                        80, 81, 90, 96, 100, 108, 120,
                                        125, 128, 135, 144, 150, 160, 
                                        162, 180, 192, 200, 216, 225,
                                        240, 243, 250, 256 };


/*********************************************************************/

void init_prefetch(void) {
    // TODO: what goes here?
    //  maybe hwp stuff...
}

void bo_pref_init(void) {
    // TODO: initialize data structures
    // rr table
    init_hash_table(&rr_table, "RR Table", 
                    256 /*TODO: how many buckets?*/, 
                    4 /*TODO: what data size?*/);
    // score table
    init_hash_table(&score_table, "Score Table", 51, sizeof(int) );



    ROUNDMAX = 1;
    SCOREMAX = 15;
    current_offset = 1;
}


/*********************************************************************/


/*TODO: prefetch the actual data.*/
 // How/should it b integrated into learning phase?
 // New_mem_req API lets you request memory. Always provide callback fcn!
 // check if prefetcher is on

/*TODO: fcn for hit and miss that call learning phase*/

void handle_round(BO_Pref* prefetcher, Addr line_addr) {
    /*
    INPUTS: Address being accessed, offset tested, current ideal offset, RR table, Score table
    OUTPUTS: 
    */
    if (round == 0) {
        // set values in score table to 0
    } 
    // X is accessed address.
    // d is offset being tested (offset)
    // D is curr_offset
    // We've accessed line X. check for X-d+D in RR table
    // if it hits in RR table, score +=1
    // if score max, break out of both loops.
    round++;
    if (round >= ROUNDMAX) {
        // learning phase complete. select highest offset and set rounds to 0
    } else if (/*most recent score is scoremax*/) {
        // select that offset. end learning phase and set round to 0
    }

}

void update_rr_table(BO_Pref* prefetcher, Addr line_addr) {
    // TODO: update RR table whenever a prefetch request is completed
    // Convert the address to its tag.
    // add it to the rr table
}
 
