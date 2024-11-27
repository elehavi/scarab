/***************************************************************************************
 * File         : l2l1pref.h
 * Author       : HPS Research Group
 * Date         : 03/24/2004
 * Description  : L2 to L1 prefetcher support functions
 ***************************************************************************************/

#ifdnef __BO_PREF_H__
#define __BO_PREF_H__

#include "dcache_stage.h"
#include "globals/global_types.h"
#include "libs/hash_lib.h"
#include "pref_type.h"

/**************************************************************************************/
/* Global vars */


/**************************************************************************************/
/* Local vars */

/**************************************************************************************/
/* Data Structures */

/* score table */
typedef struct Score_table_entry_struct {
    int offset;
    int score;
} Score_Table_Entry;

typedef struct Score_table_struct {
    uns num_offsets;
    Score_table_entry_struct[num_offsets] table;
} Score_Table;


typedef struct Bo_pref_struct {
    Hash_Table rr_table;
    Score_Table score_table;
    int [] offset_list;
    int curr_offset; 
    int ideal_offset;
    int round;
    uns scoremax;
    uns roundmax;
} BO_Pref;

/**************************************************************************************/
/* Functions/Routines */

// TODO: What is HWP? 
void init_prefetch(void);
void bo_pref_init(HWP hwp);
void prefetch_round(BO_Pref* prefetcher, Addr line_addr);
void update_rr_table(BO_Pref* prefetcher, Addr line_addr);

#endif