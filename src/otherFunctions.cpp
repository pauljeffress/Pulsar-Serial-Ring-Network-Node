/*
 * otherFunctions.cpp 
 * 
 * various supporting functions
 * 
 */

#include "global_vars.h"

bool seenBefore(uint8_t s,uint8_t d,uint8_t m){
    bool match = false;
    for(int row=0;row<SEENPACKETSMAXROW;row++){
        if ((seenPackets[row][0] == s) && (seenPackets[row][1] == d) && (seenPackets[row][2] == m)) // i.e. row match
            match = true;
    }
return match;   // did we find a match in the table?
}

void addSeenBefore(uint8_t s,uint8_t d,uint8_t m){
    seenPackets[seenPacketsIndex][0] = s;
    seenPackets[seenPacketsIndex][1] = d;
    seenPackets[seenPacketsIndex][2] = m;
    
    seenPacketsIndex++; // increment index ready and pointing to next row that should be populated.
    if (seenPacketsIndex == SEENPACKETSMAXROW) seenPacketsIndex = 0;    // wrap the index when its reached end of table.
}

SrnpPacket emptyPacket(){
    // Prep and return a zeroed packet
    SrnpPacket pac;
    pac.ver = SRNPV1;
    pac.dest = 0;
    pac.src = 0;
    pac.magicnum = 0;
    pac.packetType = 0;
    pac.copied = SRNPNOTCOPIED;
    pac.dataLen = 0;
    for (int i=0;i<SRNP_DATA_MAX_SIZE;i++) pac.data[i] = 0;
return pac;   
}

void printPacket(SrnpPacket pac){
    Serial.println("=====================");
    Serial.print("millis=");Serial.println(millis());
    Serial.println("=====================");
    Serial.print("ver        :"); Serial.println(pac.ver);
    Serial.print("dest       :"); Serial.println(pac.dest);
    Serial.print("src        :"); Serial.println(pac.src);
    Serial.print("magicnum   :"); Serial.println(pac.magicnum);
    Serial.print("PacketType :"); Serial.println(pac.packetType);
    Serial.print("copied     :"); Serial.println(pac.copied);
    Serial.print("dataLen    :"); Serial.println(pac.dataLen);
    Serial.print("data       :"); 
    for (int i=0;i<SRNP_DATA_MAX_SIZE;i++){
        Serial.print(" 0x");
        Serial.print(pac.data[i],HEX);
    }
    Serial.println();
    Serial.println("=====================");
    Serial.println();
}

void ledInitFlasher(){
    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, LOW);

    for (int i=0;i<3;i++){
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
    }

}

void HOSTNOTIFYPINhigh(){
  digitalWrite(HOSTNOTIFYPIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
}

void HOSTNOTIFYPINlow(){
  digitalWrite(HOSTNOTIFYPIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

