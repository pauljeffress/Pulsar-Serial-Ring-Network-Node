/*
 * setupSerial.cpp
 */

#include "global_vars.h"        // My main header file for this project itself

void setupSerial() {
  // NOTE: Because the FNIC <> HOST connection setup depends on each HOST, SerialHost is not setup here
  //       unless the FNIC<>HOST connection is SRNP, in which case there is config in the #ifdef below.
  //       See setupHostSpecific() for all other SerialHost configuration.
  
  // MUST KEEP SerialHost.begin(115200) being done right here.  All SerialXXX.begin() commands must be above
  // pinPeripheral commands or else weird things happen like data gets pushed out wrong physical port!!!!!
  #ifdef HOST_CON_IS_SRNP
    #ifdef IAMNODE1
    SerialHost.begin(115200);   // FNIC<>HOST is hardware serial in use, so go full speed.
    #endif
    #ifdef IAMNODE3
    SerialHost.begin(38400);   // FNIC<>HOST is software serial in use, so go slower speed.
    #endif
  #endif
  // Initialise Left & Right Serial ports
  SerialLeft.begin(115200); 
  SerialRight.begin(115200); 
  
  // Reassign pins on the internal SAMD pinmux, to connect to my SERCOMs. They may have defaulted to other peripherals.
  // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(10, PIO_SERCOM);    // the 'PIO_SERCOM' should be 'PIO_SERCOM_ALT' if we are trying to use the 'alternate' pins for this.
  pinPeripheral(11, PIO_SERCOM);    // same as above comment.
  // Assign pins 5 & 22 SERCOM functionality
  pinPeripheral(5, PIO_SERCOM); // the 'PIO_SERCOM' should be 'PIO_SERCOM_ALT' if we are trying to use the 'alternate' pins for this.
  pinPeripheral(22, PIO_SERCOM); 
  // Assign pins 15 & 16 SERCOM functionality
  pinPeripheral(15, PIO_SERCOM_ALT); // the 'PIO_SERCOM' should be 'PIO_SERCOM_ALT' if we are trying to use the 'alternate' pins for this.
  pinPeripheral(16, PIO_SERCOM_ALT);

  // Initialise Left & Right SerialTransfer drivers
  STdriverLeft.begin(SerialLeft);
  STdriverRight.begin(SerialRight);

  #ifdef HOST_CON_IS_SRNP 
    // DO NOT put SerialHost.begin(115200) in here, all SerialXXX.begin() commands must be abov ethe above
    // pinPeripheral commands or else weird things happen like data gets pushed out wrong physical port!!!!!
    STdriverHost.begin(SerialHost);
  #endif 
}
