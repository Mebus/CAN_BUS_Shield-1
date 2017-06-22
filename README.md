# CAN BUS Shield
---

## Fork Details
This fork of Seeed-Studios CAN shield library abopts little enhancements to interface 3rd party boards. Current changes:

### Use TX/RX-interrupt-pins as GPIO

The TX and RC interrupt pins of a MCP2515 can be used as generic GPIO. This changes add the capability to control the pins using the following commands:

```c
// Set RX0BF as GPIO
mcp2515_modifyRegister(MCP_BFPCTRL, B0BFM | B0BFE  | B0BFS, B0BFE);

//Turn RX0BF on
mcp2515_modifyRegister( MCP_BFPCTRL, B0BFS, 0);

//Turn RX0BF off
mcp2515_modifyRegister( MCP_BFPCTRL, B0BFS, 1);
```

Same goes for B**1**BF…

You can - for example - connect RX0BF to the Rs-pin of an PCA82C250 and use this GPIO to shutdown the transceiver on TX-only- or polling nodes saving additional power.

### Support for 4MHz-Modules

Based on the work of [Cory J. Fowler](https://github.com/coryjfowler/MCP_CAN_lib/) the code was extended to also support modules using a 4MHz crystal. All configuration values where recalculated to archive minimal deviation. These values are just calculated and not thoroughly tested, but you do use prototypes before hooking anything up to important communication systems, right?

---
## The CAN-Bus
CAN-BUS is a common industrial bus because of its long travel distance, medium communication speed and high reliability. It is commonly found on modern machine tools and as an automotive diagnostic bus.

## This Library
This library can be used to interface an Arduino compatible AVR-board to most CAN modules based on a MCP2515.

# Installation:

Download the ZIP and use arduinos "Add Zip"-function to install it. Keep in mind to delete other versions of this library if you used it or similar forks in the past.

# Usage:

You'll have a new option in Sketch -> Include Library -> CAN_BUS_Shield.  There are some examplex included.

## 1. Set CS-Pin, Mode, Clock and Baudrate
This function is used to initialize the baudrate of the CAN Bus system.

The following modes are supported:

    #define MCP_STDEXT   0                                                  // Standard and Extended
    #define MCP_ANY      3                                                  // Disables Masks and Filters

Due to a bug in the MCP2515 silicon it is not possible to reliably match only standard or only extended IDs. If you need to filter this way you have to implement it in software. In doubt use MCP_ANY.

---

The following CAN board clocks are supported:

    #define MCP_20MHZ    0
    #define MCP_16MHZ    1
    #define MCP_8MHZ     2
    #define MCP_4MHZ     3

Most boards use 16MHz, just look at the crystal soldered to your module

---

The following baudrates are available:

	#define CAN_5KBPS    1 (Not on 20MHz)
	#define CAN_10KBPS   2
	#define CAN_20KBPS   3
	#define CAN_25KBPS   4 
	#define CAN_31K25BPS 5
	#define CAN_33KBPS   6 (33.333kBps)
	#define CAN_40KBPS   7
	#define CAN_50KBPS   8
	#define CAN_80KBPS   9
	#define CAN_83K3BPS  10 (83.333kBps)
	#define CAN_95KBPS   11 (95.238kBps)
	#define CAN_100KBPS  12
	#define CAN_125KBPS  13
	#define CAN_200KBPS  14
	#define CAN_250KBPS  15
	#define CAN_500KBPS  16 (Not on 4MHz)
	#define CAN_666kbps  17 (666.666kBps, Not on 4MHz)
	#define CAN_1000KBPS 18 (Not on 4MHz and 8MHz)

You are free to choose any rate you want as long as all senders and receivers are configured equal. Some rates may not be available with certain crystals. Faster rates enable you to transfer more data, slower rates are more reliable and allow to transfer over a greater distance.

---

The initialization might look this way:

```C
include <SPI.h>
#include "mcp_can.h"

MCP_CAN CAN(10); //Use Hardware SPI and Pin 10 for CS

void setup()
{
    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN init failed");
        delay(100);
    }
    Serial.println("CAN init OK");
}

```

##2. Set Receive Mask and Filter

There are 2 receive mask registers and 5 filter registers on the controller chip. Using these you can filter incoming messages on the CAN controller avoiding to clutter your arduino. They are useful especially in a large network consisting of numerous nodes.


    init_Mask(unsigned char num, unsigned char ext, unsigned char ulData);
    init_Filt(unsigned char num, unsigned char ext, unsigned char ulData);

**num** represents which register to use. You can use 0 or 1 for mask sand 0 to 5 for filters.
**ext** represents the status of the frame. 0 means it's a mask or filter for a standard address. 1 means it's for a extended address.
**ulData** represents the content of the mask of filter.

## 3. Check Receive
The MCP2515 can operate in either a polled mode, where the software checks for a received frame, or using additional pins to signal that a frame has been received or transmit completed.  Use the following function to poll for received frames.

    byte MCP_CAN::checkReceive(void);

The function will return 1 if a frame was received and 0 if there are no frames available.

## 4. Get CAN ID

When data arrives, you can use the following function to get the CAN ID of the message.

    unsigned long MCP_CAN::getCanId(void);

## 5. Send Data

    CAN.sendMsgBuf(unsigned long id, byte ext, byte len, byte *buf);

**id** represents the CAN ID
**ext** represents the type of the frame. '0' means standard frame. '1' means extended frame.
**len** represents the number of bytes in buf.
**buf** is the content of this message.

```C  
unsigned char stmp[8] = {0, 1, 2, 3, 4, 5, 6, 7};

CAN.sendMsgBuf(0x00, 0, 8, stmp); //send the 8-byte-message 'stmp' to the bus and tell other devices this is a standard frame from 0x00.
```

## 6. Receive Data

The following function is used to receive data:

    CAN.readMsgBuf(byte *len, byte *buf);

Under the condition that masks and filters have been set, this function will only return frames that meet the requirements of those masks and filters.

**len** represents the number of bytes in buf.
**buf** is where the data is stored.

If you also want to process the CAN-ID it can be returned as well:

    byte MCP_CAN::readMsgBufID(unsigned long *ID, byte *ext, byte *len, byte buf[])
    
**ID** represents the CAN-ID.
**ext** represents type of the frame. '0' means standard frame. '1' means extended frame.
**len** represents the number of bytes in buf.
**buf** is where the data is stored.

## 7. Check additional flags

When a frame was received you may check whether it was remote request and whether it was an extended (29bit) frame.

    CAN.isRemoteRequest();
    CAN.isExtendedFrame();

**return value** is '0' for a negative response and '1' for a positive

----

# Authors

This software was written by loovee ([luweicong@seeed.cc](luweicong@seeed.cc "luweicong@seeed.cc")) for seeed studio
and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.
The original code is located at https://github.com/Seeed-Studio/CAN_BUS_Shield

The original code for supporting boards with crystals other than 16MHz as well as several code cleanups where written by Cory J. Fowler
You can find his code at https://github.com/coryjfowler/MCP_CAN_lib

This fork was created by Florian Knodt for use with aSysBus. The code is located at https://github.com/adlerweb/CAN_BUS_Shield

Additional Contributors:
  * Latonita
  * Woodward1
  * Mehtajaghvi
  * BykeBlast
  * TheRo0T
  * Tsipizic
  * ralfEdmund
  * Nathancheek
  * BlueAndi
  * Btetz
  * Hurvajs
  * xboxpro1
