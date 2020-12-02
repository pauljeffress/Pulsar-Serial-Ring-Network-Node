/*
 * otherFunctionsI2C.cpp 
 * 
 * various supporting functions for the I2C HOST<>FNIC
 * 
 */

#include "global_vars.h"

#ifdef HOST_CON_IS_I2C

SrnpI2CBlob emptyI2CBlob(){
    // Prep and return a zeroed Blob
    SrnpI2CBlob blob;
    blob.dest = 0;
    blob.packetType = 0;
    blob.dataLen = 0;
    for (int i=0;i<SRNP_I2CBLOB_DATA_MAX_SIZE;i++) blob.data[i] = 0;
return blob;   
}

void printI2CBlob(SrnpI2CBlob blob){
    Serial.println("=====================");
    Serial.print("dest       :"); Serial.println(blob.dest);
    Serial.print("PacketType :"); Serial.println(blob.packetType);
    Serial.print("dataLen    :"); Serial.println(blob.dataLen);
    Serial.print("data       : 0x"); 
    for (int i=0;i<SRNP_I2CBLOB_DATA_MAX_SIZE;i++){
        Serial.print(blob.data[i],HEX);
        Serial.print(" 0x");
    }
    Serial.println();
    Serial.println("=====================");
    Serial.println();
}

void HOST2FNIChigh(){
  digitalWrite(HOST2FNIC, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
}

void HOST2FNIClow(){
  digitalWrite(HOST2FNIC, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

#endif
