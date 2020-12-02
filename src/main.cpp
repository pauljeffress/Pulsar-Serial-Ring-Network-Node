/*
 * main.cpp
 */

#include "global_vars.h"        // My main header file for this project itself


/**********************************/
/* Global Variables               */
/**********************************/
/*
 * Create new Serial ports
 */
// Create SerialLeft
Uart SerialLeft (&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler()
{
  SerialLeft.IrqHandler();
}
// Create SerialHost
Uart SerialHost (&sercom2, 5, 22, SERCOM_RX_PAD_3, UART_TX_PAD_0);
void SERCOM2_Handler()
{
  SerialHost.IrqHandler();
}
// Create SerialRight
Uart SerialRight (&sercom4, 15, 16, SERCOM_RX_PAD_1, UART_TX_PAD_0);
void SERCOM4_Handler()
{
  SerialRight.IrqHandler();
}

// SerialTransfer initialisations
SerialTransfer STdriverLeft;
SerialTransfer STdriverRight;
#ifdef HOST_CON_IS_SRNP             // if the HOST<>FNIC connection is going to run SerialTransfer and SRNP then do this
     SerialTransfer STdriverHost;
#endif 

#ifdef HOST_CON_IS_I2C    
     SrnpI2CBlob sendBlob, receiveBlob;  // buffers for data sent/received to/from FNIC
    enum I2Cstatemachine i2cstate=HOST_IDLE; // Track the states of my main state machine. Start in IDLE state.
    // I2C Global flags 
    bool I2ChostTransmitSuccess = false; // indicates if HOST data was successfully transmitted to and received by FNIC
    bool I2ChostWantsToTransmit = false; // Main HOST functions set this when they have populated the sendBlob and want it sent.
    bool I2CfnicReceiveSuccess = false; // indicates FNIC data was successfully received.

    int I2CnumBytesReceived = 0; // used to tell how many bytes came in over I2C

#endif 

// Global single packet buffers
SrnpPacket sendPacketLeft, receivePacketLeft;
SrnpPacket sendPacketRight, receivePacketRight;
SrnpPacket sendPacketHost, receivePacketHost;     // recall that we insert data into/out of these even if HOST<>FNIC is custom.

// Global multi packet ring buffers
RingBuf<struct BufferedSrnpPacket, RINGBUFFERSIZE> bufferOutLeft;
RingBuf<struct BufferedSrnpPacket, RINGBUFFERSIZE> bufferOutRight;
RingBuf<struct BufferedSrnpPacket, RINGBUFFERSIZE> bufferOutHost;

// State machine vars
enum statemachine state=INITMYSELF; // Track the states of my main state machine. Start in INITMYSELF state.


//RTCZero rtc; // Create an RTC object

// Global flags to mark when an SRNP packet has been received from any of the three possible interfaces.
bool recHost = false;
bool recLeft = false;
bool recRight = false;

// Global flags to track packets we have put onto the network vs seeing those same packets come around the ring and removing them off the ring.
uint32_t mineTXLeft, mineRXRight;
uint32_t mineTXRight, mineRXLeft;

// Global datastore to track duplicate packets (same packet will usually come both ways around the ring)
// each row will represent one seen packet, it will contail src, dst, magicnum
// the index will keep track of where to write the next record, we will use it as a "last X packets" store.
uint8_t seenPackets[SEENPACKETSMAXROW][SEENPACKETSMAXCOL];
uint8_t seenPacketsIndex = 0;


/**********************************/
/*  setup()                       */
/**********************************/
void setup() {
  // Initialise IDE Serial Monitor port
  Serial.begin(9600);
  delay(3000);  // wait for serial to be ready so we don't miss first bits of output to Serial Monitor
  Serial.println("Starting Sketch NOW!!!");

  ledInitFlasher();

  stateInitmyself();  // Execute state INITMYSELF once here in setup()

  Serial.println("setup() finishing");
} // END - setup()


/**********************************/
/*  loop()                        */
/**********************************/
void loop() {
  //Serial.println("loop() top");
  switch (state)
  {
    case INITMYSELF:
      //Serial.println("INITMYSELF Starting");
      stateInitmyself();
      //Serial.println("INITMYSELF Finishing");
      // CHECKONNETWORK is always next state
      break; 

    case CHECKONNETWORK: // 
      //Serial.println("CHECKONNETWORK Starting");
      stateCheckOnNetwork();
      //Serial.println("CHECKONNETWORK Finishing");
      // CHECKONHOST is always next state
      break; 

    case CHECKONHOST: // 
      //Serial.println("CHECKONHOST Starting");
      stateCheckOnHost();
      //Serial.println("CHECKONHOST Finishing");
      // ACTIONPACKETS is always next state
      break;      

    case ACTIONPACKETS:
      //Serial.println("ACTIONPACKETS Starting");
      stateActionPackets();
      // HOUSEKEEPING is always next state
      break;

    case HOUSEKEEPING:
      //Serial.println("HOUSEKEEPING Starting");
      stateHousekeeping();
      //Serial.println("HOUSEKEEPING Finishing");
      // SHOULDISLEEP is always next state
      break;  

    case SHOULDISLEEP:
      //Serial.println("SHOULDISLEEP Starting");
      stateShouldISleep();
      //Serial.println("SHOULDISLEEP Finishing");
      // Next state is determined dynamically within this state.
      break; 

    case SLEEP:
      //Serial.println("SLEEP Starting");
      stateSleep();
      //Serial.println("SLEEP Finishing");
      // WAKEUP is always next state
      break; 

    case WAKEUP:
      //Serial.println("WAKEUP Starting");
      stateWakeup();
      //Serial.println("WAKEUP Finishing");
      // CHECKONNETWORK is always next state
      break; 


    default:  // this should not be executed, if it is, something in the statemachine went amis
      // write a suitable error message to the Flight Data Recorder
      Serial.println("OH no, we have fallen through to the master Switch Default in loop()");
      state = INITMYSELF;  // return the state machine to the safest state.
      break;    
  }
  
  //Serial.println("loop() delaying");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  //Serial.println("loop() bottom");
} // END - loop()