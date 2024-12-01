/***************************************************************************************
 * File         : l2l1pref.h
 * Author       : HPS Research Group
 * Date         : 03/24/2004
 * Description  : L2 to L1 prefetcher support functions
 ***************************************************************************************/

#ifndef __BO_PREF_H__
#define __BO_PREF_H__

#include "dcache_stage.h"
#include "globals/global_types.h"
#include "libs/hash_lib.h"
#include "prefetcher/pref_type.h"
#include "pref_common.h"
#include "memory/mem_req.h"

typedef unsigned uns;
/**************************************************************************************/
/* Global vars */


/**************************************************************************************/
/* Local vars */

/**************************************************************************************/
/* Data Structures */

/* score table */
// typedef struct Score_table_entry_struct {
//     int offset;
//     int score;
// } Score_Table_Entry;

// typedef struct Score_table_struct {
//     uns num_offsets;
//     Score_table_entry_struct[num_offsets] table;
// } Score_Table;


typedef struct Bo_pref_struct {
    HWP_Info* hwp_info;
    Hash_Table rr_table;
    uns score_table[51];
    int offset_list[51];
    int curr_offset;
    int round;
    uns scoremax;
    uns roundmax;
    uns badscore;
} Pref_BO;

/**************************************************************************************/
/* Functions/Routines */

void init_prefetch_bo(HWP* hwp);
void bo_pref_init(HWP* hwp, Pref_BO* pref);
void bo_ul1_miss(uns8 proc_id, Addr lineAddr, Addr loadPC, uns32 global_hist);
void bo_ul1_pref_hit(uns8 proc_id, Addr lineAddr, Addr loadPC, uns32 global_hist);
void prefetch_round(Pref_BO* prefetcher, Addr line_addr, uns proc_id);
Flag update_rr_table(Mem_Req* req);

#endif