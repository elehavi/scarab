/***************************************************************************************
 * File         : bo_pref.h
 * Author       : Ella Lehavi and Rushali Ghosh
 * Date         : Fall Quarter 2024
 * Description  : Best-Offset Prefetcher
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


/**************************************************************************************/
/* Data Structures */




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
void bo_dl0_miss(Addr lineAddr, Addr loadPC);
void bo_dl0_pref_hit(Addr lineAddr, Addr loadPC);
void prefetch_round(Pref_BO* prefetcher, Addr line_addr);
Flag update_rr_table(Mem_Req* req);

#endif