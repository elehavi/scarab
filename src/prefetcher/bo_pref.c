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

void learning_phase(Hash_Table* rr_table, Hash_Table* score_table, bool hit) {
    int scoremax_reached;
    int optimal_offset;
    // TODO: set values in score table to 0. memset?
    // Check for prefetch miss/hit
    // TODO: update RR table whenever a prefetch request is made
    for (i = 0; i < ROUNDMAX; i++) {
        // TODO: is each round 1 access
        // for each offset
            // test prefetching from the offset
            // if it hits in RR table, score +=1
            // if score max, break out of both loops.
            if (score == SCOREMAX) {
                scoremax_reached = 1;
                optimal_offset = offset;
                break;
                // TODO: could we return?
            }
        if (scoremax_reached) {
            break;
        }  

    }

    // TODO: set best offset to the prefetch offset
    current_offset = optimal_offset;
    // TODO: tie?
    /* Learning phase ends when:
    one of the scores equals SCOREMAX, 
    the number of rounds equals ROUNDMAX (a fixed parameter).*/
}