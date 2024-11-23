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

/* Struct RR table*/

/* Offset list*/

/* score table */
// typedef struct Score_table_entry_struct {
//     int offset;
//     int score;
// } Score_Table_Entry;

// typedef struct Score_table_struct {
//     uns num_offsets;

// } Score_Table;

//TODO: separately defined data structures?

/**************************************************************************************/
/* Functions/Routines */

// TODO: What is HWP? 
void init_prefetch(void);
void bo_pref_init(void /* HWP* hwp */);
void learning_phase(Hash_Table* rr_table, Hash_Table* score_table, bool hit);


#endif