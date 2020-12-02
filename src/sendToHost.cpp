/*
 * sendToHost.cpp
 * 
 * Called by stateActionPackets() when there is a packet that needs to be 
 * sent up to the HOST.  Now because there are a number of ways an FNIC may 
 * connected to its HOST and it can be custom per FNIC<>HOST combo, we need
 * to break out to here and pass the data to the HOST in the way it accepts.
 * 
 * 
 */

#include "global_vars.h"

void sendToHost(SrnpPacket pac){
    

#ifdef IAMNODE1
  // This code is custom for Node 1. 
  // It is specific for the method of communication between HOST NODE 1 and FNIC 1
  // which in this case is pretty straight forward as HOST<>FNIC are speaking straight SRNP Packets over 
  // SerialTransfer to FNIC 1.
  STdriverHost.sendDatum(pac);
#endif  // NODE1

#ifdef IAMNODE2
    // This code is custom for Node 2

    // First check its a UNICAST to me packet AND its pacetType is PT_DATA_FDR_ENTRY, else don't pass it to Host.
    if ((pac.dest == MY_ADDRESS) && (pac.packetType == PT_DATA_FDR_ENTRY)){        
        Serial.println("sendToHost() NODE2 executing");
        SerialHost.print("FDR ENTRY dest:"); SerialHost.print(pac.dest);
        SerialHost.print(" src:"); SerialHost.print(pac.src);
        SerialHost.print(" magicnum:"); SerialHost.print(pac.magicnum);
        SerialHost.print(" dataLen:"); SerialHost.print(pac.dataLen);
        SerialHost.print(" data(HEX):"); 
            for (int i=0;i<SRNP_DATA_MAX_SIZE;i++){
                SerialHost.print(" 0x");
                SerialHost.print(pac.data[i],HEX);
            }
        SerialHost.print(" data:"); 
            for (int i=0;i<SRNP_DATA_MAX_SIZE;i++){
                SerialHost.print((char)pac.data[i]);
            }
        SerialHost.println();
    }
    
#endif  // NODE2

#ifdef IAMNODE3     // Global Tracker SatCom
  // This code is custom for Node 3. 
  // It is specific for the method of communication between HOST NODE 3 and FNIC 3
  // which in this case is pretty straight forward as HOST<>FNIC are speaking straight SRNP Packets over 
  // SerialTransfer to FNIC 3.

    // First check its a UNICAST to me packet AND its packetType is something the AGT is interested in, else don't pass it to Host.
    if ((pac.dest == MY_ADDRESS) && (pac.packetType == PT_DATA_AGT_SEND)){        
        Serial.println("sendToHost() NODE3 executing");
        
        // ensure the HOST is listening (it is using SoftwareSerial so need to be sure it's ready to receive)
        HOSTNOTIFYPINhigh();

        // TODO - has the HOST responded with its PIN to say its ready?

        // TODO - insert code to send packet up to AGTHOST

        HOSTNOTIFYPINlow(); // let HOST know we don't need it to be ready to receive anymore.
    }





  STdriverHost.sendDatum(pac);

#endif  // NODE3

#ifdef IAMNODE4
    // This code is custom for Node 4

#endif  // NODE4


}   // END - sendToHost()