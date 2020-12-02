/*
 * srnp.h
 * 
 * 
 * 
 */

#ifndef SRNP_H
#define SRNP_H

#define SRNP_HEADER_SIZE
#define SRNP_DATA_MAX_SIZE   32 //Max number of bytes in SRNP DATA field. 

#define SRNP_I2CBLOB_DATA_MAX_SIZE   24 //Max number of bytes in I2C Blob data field. (not actually SRNP...it;s just the HOST<>FNIC I2C)
                                        // keeping in mind I2C can tx/rx ,ax 32bytes at a time, HW limit I believe

// SRNP Version Byte
#define SRNPV1  1

// SRNP Address Byte
#define BROADCAST 255

// SRNP Packet Type Byte
#define PT_NULL                 0
//   Host Data Types
//     Simple types
#define PT_DATA_SINGLE_FLOAT    1
#define PT_DATA_SINGLE_INT32    2
#define PT_DATA_SINGLE_BOOL     3
#define PT_DATA_SINGLE_STRING   4

//     Complex types
#define PT_DATA_FDR_ENTRY         20    
#define PT_DATA_NINE_DOF          21
#define PT_DATA_CURRENT_COURSE    22
#define PT_DATA_CURRENT_LOCATION  23
#define PT_DATA_PREVIOUS_LOCATION 24
#define PT_DATA_TIME              25
#define PT_DATA_WAYPOINT_LOCATION 26
#define PT_DATA_COURSE            27
#define PT_DATA_AGT_SEND          28



//   Host Command/Response Types
//     Commands
#define PT_CMD_DISABLE_PROP 40
#define PT_CMD_ENABLE_PROP  41
#define PT_CMD_TIME_REQ     42
#define PT_CMD_LOCATION_REQ 43
//     Responses
#define PT_RSP_TIME_RSP     52
#define PT_CMD_LOCATION_RSP 53


// SRNP Copied Byte
#define SRNPNOTCOPIED   0
#define SRNPCOPIED      1


// SRNP Packet Struct
struct SrnpPacket {
  uint8_t ver;                    // SRNP version number. see SRNPV define
  uint8_t dest;                   // SRNP Destination Address - can be a number for Unicast packet or BROADCAST for Broadcast packet.
  uint8_t src;                    // SRNP Source Address
  uint8_t magicnum;               // SRNP Magic Number - just a rolling counter to help identify identical packets sent both ways around the ring.
  uint8_t packetType;             // SRNP Packet Type 
  uint8_t copied;                 // SRNP Copied - Marked when a Unicast packet has been read/copied by the intended Destination Node.
  uint8_t dataLen;                // SRNP Data Len - Amount of bytes populated in the 'data' field.
  byte data[SRNP_DATA_MAX_SIZE];  // SRNP Data - Byte array, content and structure defined by packetType.
};

// SRNP Packet Struct
struct BufferedSrnpPacket {
  uint8_t sourceInt;              // Track which interface the packet was received on.
  uint8_t ver;                    // SRNP version number. see SRNPV define
  uint8_t dest;                   // SRNP Destination Address - can be a number for Unicast packet or BROADCAST for Broadcast packet.
  uint8_t src;                    // SRNP Source Address
  uint8_t magicnum;               // SRNP Magic Number - just a rolling counter to help identify identical packets sent both ways around the ring.
  uint8_t packetType;             // SRNP Packet Type 
  uint8_t copied;                 // SRNP Copied - Marked when a Unicast packet has been read/copied by the intended Destination Node.
  uint8_t dataLen;                // SRNP Data Len - Amount of bytes populated in the 'data' field.
  byte data[SRNP_DATA_MAX_SIZE];  // SRNP Data - Byte array, content and structure defined by packetType.
};

// HOST<>FNIC via I2C Packet Struct
struct SrnpI2CBlob {
  uint8_t dest;                   // SRNP Destination Address - can be a number for Unicast packet or BROADCAST for Broadcast packet.
  uint8_t packetType;             // SRNP Packet Type 
  uint8_t dataLen;                // SRNPI2C Data Len - Amount of bytes populated in the 'data' field.
  byte data[SRNP_I2CBLOB_DATA_MAX_SIZE];  // SRNP Data - Byte array, content and structure defined by packetType.
};



#endif