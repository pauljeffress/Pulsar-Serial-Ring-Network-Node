/*
 * setupHostSpecific() 
 * 
 * do any custom setup for a particular FNIC's HOST.
 * 
 */

#include "global_vars.h"

void setupHostSpecific(){

#ifdef IAMNODE1 // PJ Arduino HOST
    // This code is custom for Node 1

#endif  // NODE1

#ifdef IAMNODE2 // FDR HOST
    // This code is custom for Node 2
    //SerialHost.begin(115200);  MUST GO IN setupSerial(), check notes there.  DO NOT DO IT HERE!

    // Log a message to the FDR announcing that I, the FNIC, have just initialised and executed this function.
    SerialHost.println("Hello FDR, I am your FNIC and I am running setupHostSpecific()");

#endif  // NODE2

#ifdef IAMNODE3
    // This code is custom for Node 3

#endif  // NODE3

#ifdef IAMNODE4
    // This code is custom for Node 4

#endif  // NODE4

}