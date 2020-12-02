/*
 * stateInitmyself()
 * 
 * Reset everything to a known initial state.
 * 
 * Gets called for two reasons;
 * 1. the MCU has been powered up or RESET and setup() calls this function.
 * OR
 * 2. while running, my code thinks something may be astray and wants to get back to a known initial state.
 * 
 */

#include "global_vars.h"

void stateInitmyself(){
    Serial.println("stateInitmyself() START");


    //zero out the seen packets data store
    for(int row=0;row<SEENPACKETSMAXROW;row++)
        for(int col=0;col<SEENPACKETSMAXCOL;col++)
            seenPackets[row][col] = 0;
    
    // prep an empty sendPacketLeft
    sendPacketLeft = emptyPacket();
    // prep an empty receivePacketLeft
    receivePacketLeft = emptyPacket();
    // prep an empty sendPacketRight
    sendPacketRight = emptyPacket();
    // prep an empty receivePacketRight
    receivePacketRight = emptyPacket();

    #ifdef HOST_CON_IS_SRNP             // if the HOST<>FNIC connection is going to run SerialTransfer and SRNP then do this
      // prep an empty sendPacketHost
      sendPacketHost = sendPacketLeft;
      // prep an empty receivePacketHost
      receivePacketHost = receivePacketLeft;
    #endif

    #ifdef HOST_CON_IS_I2C
      // Initialize I2C communications as Master
      Wire.begin();
      sendBlob = emptyI2CBlob();    // zero out this struct ready for use
      receiveBlob = emptyI2CBlob(); // zero out this struct ready for use 
      pinMode(HOST2FNIC, OUTPUT);
      pinMode(FNIC2HOST, INPUT);
      digitalWrite(HOST2FNIC, LOW);
    #endif

    setupPins();    // Initiaise any GPIOs
  
    setupSerial();  // Initialise the custom Serial ports etc
  
    setupRTC();     // Initialise the RTC

    setupTimers();  // Initialise any timers

    setupHostSpecific(); // do any custom setup for this particular FNIC's HOST.

    // TODO - insert code to create (or queue for delivery) a locally timestamped (regardless of clock correctness)
    //        log entry to send to the FDR. Its important to know if this state was executed for some reason 
    //        other than at Power On RESET.

    
    state = CHECKONNETWORK;  // as this is called once in setup() I actually need to set the next state here otherwise
                  // I was finding it was running a 2nd time the first time loop() executed.
}   // END - statInitMyself()