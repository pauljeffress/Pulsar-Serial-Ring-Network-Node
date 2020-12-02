/*
 * stateAwake()
 */

#include "global_vars.h"

void stateAwake(){
    // ensure each of the main tasks that need to happen in stateAwake() get an equal chance of running and
    //   they run in sequential order. Without this approach, something like checking for and actioning packets
    //   could take all processor time at the exclussion of other tasks like checking on timers etc.
  switch (awaketask)
  {
    case 0:     // Go straight to CHECKFORPACKETS state to see if there are any received packets.
      state = CHECKFORPACKETS;
      break;
    
    case 1:     // Go straight to CHECKONHOST state to see if there is any received data from our host.
      state = CHECKONHOST;
      break; 

    case 2:     // Is it time to get to HOUSEKEEPING state?
      //Serial.println("Shall we housekeep?");
      if (hasHousekeepingTimerExpired()) state = HOUSEKEEPING;
      break;  

    case 3:     // Can we go into PRESLEEP state?
      //Serial.println("Shall we sleep?");
      if (canWeSleep()) state = PRESLEEP;
      break; 

    default:  // this should not be executed, if it is, something in the awaketask counter went amis
      // write a suitable error message to the Flight Data Recorder
      Serial.println("OH no, we have fallen through to the Switch Default in stateAwake()");
      break;    
  }
    
  // increment to next task when we next run stateAwake().
  awaketask++;  
  if (awaketask > 3) awaketask=0; 


  // at this point this execution of the AWAKE state is finished and the main state machine in loop() will go to next state.
}   // END - stateAwake()