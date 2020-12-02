/*
 * global_vars.h
 * 
 * Overall header file for this project
 * 
 */

#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

/* types for global vars */
#include <Arduino.h>  // Placed here by PlatformIO and also required before "wiring_private.h" (required for SERCOM code)
#include "wiring_private.h"     // for pinPeripheral() function (required for SERCOM code)
#include <Wire.h> // PlatformIO needed me to explicitly include this before SerialTransfer.h ALSO used by HOST<>FNIC I2C
#include <SPI.h>  // PlatformIO needed me to explicitly include this before SerialTransfer.h
#include <SerialTransfer.h> // Note: This library will complain about SPDR in the SPITransfer.cpp/h files at compile time.
                            //       Its a known problem, search my Evernotes. The solution as I am not using SPI under
                            //       SerialTransfer is to rename two of the source files in the SerilTransfer library so
                            //       they are not compiled.  The two files are;
                            //       * SPITransfer.h and SPITransfer.cpp
                            //       they live in .pio/libdeps/..../SerialTransfer/src/
//#include <RTCZero.h>  // RTC library that operates the SAMD21 built in RTC as opposed to an external I2C RTC.
#include <RingBuf.h>    // For my packet Ring Buffers

#include <srnp.h>        
#include <srn_custom_fnic.h>  // custom config that gets changed for each node. 

// define GPIO pins for the Notify and Listen wires that go between this Node and it's Left, Right and Host peers. 
#define LEFTNOTIFYPIN 9
#define LEFTLISTENPIN 12
#define HOSTNOTIFYPIN 23
#define HOSTLISTENPIN 6
#define RIGHTNOTIFYPIN 17
#define RIGHTLISTENPIN 14

#define SEENPACKETSMAXROW 10
#define SEENPACKETSMAXCOL 3

// used in packet ring buffer struct to track sourceInt
#define LEFTINT     1
#define RIGHTINT    2
#define HOSTINT     3
// number of elements in the packet ring buffers
#define RINGBUFFERSIZE  10

/* define any enums */
enum statemachine { INITMYSELF, CHECKONNETWORK, CHECKONHOST, ACTIONPACKETS, HOUSEKEEPING, SHOULDISLEEP, SLEEP, WAKEUP };
enum I2Cstatemachine { HOST_IDLE, HOST_TRANSMIT, HOST_RECEIVE, HOST_RECEIVE_ACTION};


/* extern my global vars */
extern Uart SerialLeft;
extern Uart SerialRight;
extern Uart SerialHost; 
extern SerialTransfer STdriverLeft;
extern SerialTransfer STdriverRight;
extern SrnpPacket sendPacketLeft, receivePacketLeft;
extern SrnpPacket sendPacketRight, receivePacketRight;
extern SrnpPacket sendPacketHost, receivePacketHost;
#ifdef HOST_CON_IS_SRNP 
    extern SerialTransfer STdriverHost;
#endif 
#ifdef HOST_CON_IS_I2C    
    extern SrnpI2CBlob sendBlob, receiveBlob;
    extern enum I2Cstatemachine I2Cstate; 
    extern bool I2ChostTransmitSuccess; 
    extern bool I2ChostWantsToTransmit; 
    extern bool I2CfnicReceiveSuccess; 
    extern int I2CnumBytesReceived;
#endif 
extern enum statemachine state;
//extern RTCZero rtc;
extern bool recHost;
extern bool recLeft;
extern bool recRight;
extern uint32_t mineTXLeft, mineRXRight;
extern uint32_t mineTXRight, mineRXLeft;
extern uint8_t seenPackets[SEENPACKETSMAXROW][SEENPACKETSMAXCOL];
extern uint8_t seenPacketsIndex;

/* function pre defines */
void setupSerial();
void setupRTC();
void setupTimers();
void setupPins();
void stateInitmyself();
void stateHousekeeping();
void stateCheckOnHost();
void stateCheckOnNetwork();
void stateActionPackets();
void stateShouldISleep();
void stateSleep();
void stateWakeup();
bool seenBefore(uint8_t s,uint8_t d,uint8_t m);
void addSeenBefore(uint8_t s,uint8_t d,uint8_t m);
SrnpPacket emptyPacket();
void printPacket(SrnpPacket pac);
void sendToHost(SrnpPacket pac);
void ledInitFlasher();
void setupHostSpecific();
void HOSTNOTIFYPINhigh();
void HOSTNOTIFYPINlow();
#ifdef HOST_CON_IS_I2C
    SrnpI2CBlob emptyI2CBlob();
    void printI2CBlob(SrnpI2CBlob blob);
    void HOST2FNIChigh();
    void HOST2FNIClow();
    bool I2CFnic();
#endif

#endif   