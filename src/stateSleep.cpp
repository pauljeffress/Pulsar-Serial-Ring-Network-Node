/*
 * stateSleep()
 * 
 * Do pre sleep stuff then put CPU to sleep.  After waking, go to state WAKEUP.
 * 
 */

#include "global_vars.h"

void stateSleep(){
    
    // TODO - code this function.

    
    state = WAKEUP;  // always goto that state after this one. 
}