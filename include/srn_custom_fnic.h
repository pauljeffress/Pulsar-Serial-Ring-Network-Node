/*
 * srn_custom_fnic.h
 * 
 * This is for the defines that need to be changed per SRN Node. 
 * e.g. Node address. Also any specifics about how each indicvidual FNIC interfaces with thier HOST.
 * 
 */

#ifndef SRN_CUSTOM_FNIC_H
#define SRN_CUSTOM_FNIC_H

// uncomment only one of these at a time. Uncomment the define for the node you are compiling for.
// Note: this define affects code here and in "sendToHost.cpp", "stateCheckOnHost.cpp" & "setupHostSpecific.cpp"
//#define IAMNODE1        // PJ Arduino        
//#define IAMNODE2        // Flight Data Recorder
//#define IAMNODE3        // Global Tracker SatCom
#define IAMNODE4

// As each Host may use a different method to communicate iwth its FNIC, we may need to 
// prep some host specific defines etc here, specific to a NODE/FNIC pairing.
#ifdef IAMNODE1
    // This code is custom for the FNIC connected to Node 1
    #define MY_ADDRESS          1    // Serial Ring Network Protocol config - CHANGE ALL FOR EACH NODE!
    // Define HOST <> FNIC connection for this particular FNIC/HOST pair.
    #define HOST_CON_IS_SRNP    // We are using native SRNP over SerialTransfer 
#endif  // END - NODE1

#ifdef IAMNODE2     // Flight Data Recorder
    // This code is custom for the FNIC connected to Node 2
    #define MY_ADDRESS          2    // Serial Ring Network Protocol config - CHANGE ALL FOR EACH NODE!
    // Define HOST <> FNIC connection for this particular FNIC/HOST pair.
    

#endif  // END - NODE2

#ifdef IAMNODE3     // Global Tracker SatCom
    // This code is custom for the FNIC connected to Node 3
    #define MY_ADDRESS          3    // Serial Ring Network Protocol config - CHANGE ALL FOR EACH NODE!
    // Define HOST <> FNIC connection for this particular FNIC/HOST pair.
    #define HOST_CON_IS_SRNP    // We are using native SRNP over SerialTransfer 
#endif  // END - NODE3

#ifdef IAMNODE4
    // This code is custom for the FNIC connected to Node 4
    #define MY_ADDRESS          4    // Serial Ring Network Protocol config - CHANGE ALL FOR EACH NODE!
#endif  // END - NODE4

// The below was what i got up to when I was thinking of using I2C for FNIC<>HOST comms for the AGT.  I decided 
// not to do that eventually, but thought it might be handy to keep this code in case I need it in future.
// #ifdef IAMNODE4
//     // This code is custom for the FNIC connected to Node 4
//     #define MY_ADDRESS          4    // Serial Ring Network Protocol config - CHANGE ALL FOR EACH NODE!
//     // Define HOST <> FNIC connection for this particular FNIC/HOST pair.
//     #define HOST_CON_IS_SRNP    // We are using native SRNP over SerialTransfer 
//     // OR
//     //#define HOST_CON_IS_I2C // set this if the connection between the HOST and FNIC is I2C
//     #ifdef HOST_CON_IS_I2C
//         // Define Slave I2C Address
//         #define I2C_SLAVE_ADDR 9    // this will be the I2C address of this FNIC (the slave)
//         // Define Slave answer size
//         #define ACKNAKSIZE 3    // three chars "ACK" or "NAK"
//         // GPIO signalling pins to assist I2C
//         #define HOST2FNIC 23  // MOSI/D23
//         #define FNIC2HOST 6        
//     #endif
// #endif  // END - NODE4


#endif  // END - srn_custom_fnic.h