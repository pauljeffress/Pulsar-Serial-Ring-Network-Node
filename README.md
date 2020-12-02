Serial Ring Network Node doco
By Paul Jeffress

Custom Serial Ports
===================
The custom defined Serial ports to give the Feather enough for this project.
Note: These Serial ports are Layer 1 of my network stack.
This work is entirely based on https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/creating-a-new-serial
Se also my "Feather_M0_extra_UARTs_multiple.ino" where I initially tested out generating the ports and seeing them work.
I create three additional UARTs on a single Feather M0;
* Leave the original Serial for the existing Arduino IDE Programming and Serial Monitor functions
* One to connect the FNIC to the Boat Arduino it serves.
* Two to form the Ring and connect to adjacent nodes.
So this means the Feather will need to have four active UARTs simultaneously.

SerialLeft
----------
Use SERCOM1 to generate port 'SerialLeft'
with TX on D10 and RX on D11
SERCOM1.0 (non alt) > D11 (i.e RX)
SERCOM1.2 (non alt) > D10 (i.e.TX)

SerialHost
-----------
Use SERCOM2 to generate port 'SerialHost'
with TX on D22/MISO and RX on D5
SERCOM2.3 (non alt) > D5 (i.e. RX)
SERCOM2.0 (non alt) > D22/MISO (i.e. TX)

SerialRight
----------
Use SERCOM4 to generate port 'SerialRight'.   Note SERCOM4 is usually used by Arduino to create SPI port
with TX on A2/D16 and RX on A1/D15
SERCOM4.0 (alt) > A2/D16 (i.e. RX)   << note I am using PAD 1 here, whereas in my other examples I've used PAD0. 
SERCOM4.1 (alt) > A1/D15 (i.e. TX)     


SerialTransfer Library
======================
GitHub: https://github.com/PowerBroker2/SerialTransfer
I'm using SerialTransfer to create two (one for Left interface and one for Right interface) simple framed
serial interfaces.  
Note: This Frame level protocol is Layer 2 in my network stack.

01111110 00000000 11111111 00000000 00000000 00000000 ... 00000000 10000001
|      | |      | |      | |      | |      | |      | | | |      | |______|__Stop byte (0x81)
|      | |      | |      | |      | |      | |      | | | |______|___________8-bit CRC
|      | |      | |      | |      | |      | |      | |_|____________________Rest of payload
|      | |      | |      | |      | |      | |______|________________________2nd payload byte
|      | |      | |      | |      | |______|_________________________________1st payload byte
|      | |      | |      | |______|__________________________________________# of payload bytes
|      | |      | |______|___________________________________________________COBS Overhead byte
|      | |______|____________________________________________________________Packet ID (0 by default) (0x00)
|______|_____________________________________________________________________Start byte (constant) (0x7E)


Serial Ring Network
===================
This project.
Note: This Packet level protocol is Layer 3 in my network stack.

