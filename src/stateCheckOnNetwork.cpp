/*
 * stateCheckOnNetwork()
 * 
 * Check if there are any packets that have arrived on either LEFt or RIGHT interface.
 * 
 * 
 */

#include "global_vars.h"

void stateCheckOnNetwork(){
    
    bool actionPacket = false;    // assuming no action required unless proven otherwise.
    
    //Serial.println("Checking for LEFT/RIGHT Packets");

    // check if we have received a packet from Left interface
    if(STdriverLeft.available())
    {
        Serial.println("YES something available from LEFT");
        STdriverLeft.rxObj(receivePacketLeft);

        recLeft = true;       // flag GLOBAL that we have received a packet from our HOST.
        actionPacket = true;  // flag LOCALLY in this funbction to ensure we select the correct next STATE at end of this function.

        printPacket(receivePacketLeft);
    }  
    else 
    {
        //Serial.println("NO nothing available from LEFT");
    }

    // check if we have received a packet from Right interface
    if(STdriverRight.available())
    {
        Serial.println("YES something available from RIGHT");
        STdriverRight.rxObj(receivePacketRight);

        recRight = true;       // flag GLOBAL that we have received a packet from our Left interface.
        actionPacket = true;  // flag LOCALLY in this function to ensure we select the correct next STATE at end of this function.

        printPacket(receivePacketRight);
    }  
    else
    {
        //Serial.println("NO nothing available from RIGHT");
    }


    state = CHECKONHOST;  // always goto that state after this one. 

}   // END - stateCheckOnNetwork()