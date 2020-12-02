/*
 * stateCheckOnHost()
 * 
 * Check if the Host needs attention (usually it would be to send a packet).
 * 
 * Do whatever method I need to for this particular HOST (note they may all interact
 * with thier FNIC slightly differently) to see if it has/wants to give this FNIC data to TX on the network.
 * 
 * If it does, then get that data and place it in the "receivePacketLeft" standard global buffer, set some global flags etc
 * and then go to state ACTIONPACKETS to send it onto the network or whatever else needs action.
 * 
 */

#include "global_vars.h"

void stateCheckOnHost(){
    
    bool actionPacket = false;    // assuming no action required unless proven otherwise.

#ifdef IAMNODE1
  // This code is custom for Node 1. It is specific for the method of communication between HOST NODE 1 and FNIC 1
  // which in this case is pretty straight forward as HOST NODE 1 is speaking straight SRNP Packets over 
  // SerialTransfer to FNIC 1.
  if(STdriverHost.available())
  {
    Serial.println("YES something available from HOST");
    STdriverHost.rxObj(receivePacketHost);

    recHost = true;       // flag GLOBAL that we have received a packet from our HOST.
    actionPacket = true;  // flag LOCALLY in this funbction to ensure we select the correct next STATE at end of this function.

    printPacket(receivePacketHost);
  }  
  else 
  {
      //Serial.println("NO nothing available from HOST"); 
  }

#endif  // NODE1

#ifdef IAMNODE2
    // This code is custom for Node 2

#endif  // NODE2

#ifdef IAMNODE3     // Global Tracker SatCom
  // This code is custom for Node 3. It is specific for the method of communication between HOST NODE 3 and FNIC 3
  // which in this case is pretty straight forward as HOST NODE 3 is speaking straight SRNP Packets over 
  // SerialTransfer to FNIC 3.
  if(STdriverHost.available())
  {
    Serial.println("YES something available from HOST");
    STdriverHost.rxObj(receivePacketHost);

    recHost = true;       // flag GLOBAL that we have received a packet from our HOST.
    actionPacket = true;  // flag LOCALLY in this funbction to ensure we select the correct next STATE at end of this function.

    printPacket(receivePacketHost);
  }  
  else 
  {
      //Serial.println("NO nothing available from HOST"); 
  }

#endif  // NODE3

#ifdef IAMNODE4
    // This code is custom for Node 4
    #ifdef HOST_CON_IS_I2C    
        bool stayInI2CFnic = true;  // prime the flag so we execute I2CFnic at least once.
        //TODO - maybe put a counter in the below while condition so it can execute a max of say 20 times...to help breakout of any problems?
        while (stayInI2CFnic)
        {
            stayInI2CFnic = I2CFnic();  // run the I2C FNIC statemachine
        }

        if(I2CfnicReceiveSuccess)    // we received a packet from HOST over I2C
        {
            Serial.println("YES something available from HOST");

            receivePacketHost = emptyPacket();  // zero it out

            // set fields we already know
            receivePacketHost.ver = SRNPV1;
            receivePacketHost.src = MY_ADDRESS;

            // copy data from HOSTs I2C packet to an SRNP packet ready for FNIC main code to send.
            receivePacketHost.dest = receiveBlob.dest;
            receivePacketHost.packetType = receiveBlob.packetType;
            receivePacketHost.dataLen = receiveBlob.dataLen;
            for (int i=0;i<receiveBlob.dataLen;i++) receivePacketHost.data[i] = receiveBlob.data[i];

            recHost = true;       // flag GLOBAL that we have received a packet from our HOST.
            actionPacket = true;  // flag LOCALLY in this funbction to ensure we select the correct next STATE at end of this function.    

            printPacket(receivePacketHost);
        }  
        else 
        {
            //Serial.println("NO nothing available from HOST"); 
        }
    #endif  // HOST_CON_IS_I2C

#endif  // NODE4


    if (actionPacket) state = ACTIONPACKETS;
    else state = AWAKE;

}   // END - stateCheckOnHost()