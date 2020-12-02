/*
 * stateHousekeeping()
 * 
 * Miscellaneous odd jobs that need to happen from time to time when the node is awake.
 * 
 */

#include "global_vars.h"

void stateHousekeeping(){
    

    state = SHOULDISLEEP;  // always goto that state after this one. 
}