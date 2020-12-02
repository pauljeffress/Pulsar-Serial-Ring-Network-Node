/*
 * stateWakeup()
 * 
 * Do post sleep stuff then  go to next state
 * 
 */

#include "global_vars.h"

void stateWakeup(){
    

    // TODO - code this function.

    state = CHECKONNETWORK;  // always goto that state after this one. 
}