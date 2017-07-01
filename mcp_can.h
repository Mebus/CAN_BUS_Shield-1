/*
  mcp_can.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee (loovee@seeed.cc)
  2014-1-16

  Contributor:

  Cory J. Fowler
  Latonita
  Woodward1
  Mehtajaghvi
  BykeBlast
  TheRo0T
  Tsipizic
  ralfEdmund
  Nathancheek
  BlueAndi
  Adlerweb
  Btetz
  Hurvajs

  The MIT License (MIT)

  Copyright (c) 2013 Seeed Technology Inc.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
#ifndef _MCP2515_H_
#define _MCP2515_H_

#include "mcp_can_dfs.h"

#define MAX_CHAR_IN_MESSAGE 8

class MCP_CAN
{
    private:

    byte   ext_flg;                         // Identifier Type
                                            // Extended (29 bit) or Standard (11 bit)
    unsigned long  can_id;                  // CAN ID
    byte   dta_len;                         // Data Length Code
    byte   dta[MAX_CHAR_IN_MESSAGE];        // Data array
    byte   rtr;                             // Remote request flag
    byte   filhit;                          // The number of the filter that matched the message
    byte   SPICS;                           // Chip Select pin number
    byte   mcpMode;                         // Mode to return to after configurations are performed

/*
*  mcp2515 driver function
*/

private:

    void mcp2515_reset(void);                                   // reset mcp2515

    byte mcp2515_readRegister(const byte address);              // read mcp2515's register

    void mcp2515_readRegisterS(const byte address,              // Read MCP2515 successive registers
	                           byte values[],
                               const byte n);
    void mcp2515_setRegister(const byte address,                // Set MCP2515 register
                             const byte value);

    void mcp2515_setRegisterS(const byte address,               // Set MCP2515 successive registers
                              const byte values[],
                              const byte n);

    void mcp2515_initCANBuffers(void);

    void mcp2515_modifyRegister(const byte address,             // Set specific bit(s) of a register
                                const byte mask,
                                const byte data);

    byte mcp2515_readStatus(void);                              // Read MCP2515 Status
    byte mcp2515_setCANCTRL_Mode(const byte newmode);           // Set mode
    byte mcp2515_configRate(const byte canSpeed,                // Set baudrate
                             const byte canClock);
    byte mcp2515_init(const byte canIDMode,                     // Initialize Controller
                       const byte canSpeed,
                       const byte canClock);

    void mcp2515_write_id(const byte mcp_addr,                  // Write CAN Mask or Filter
                           const byte ext,
                           const unsigned long id);

    void mcp2515_read_id(const byte mcp_addr,                   // Read CAN ID
                          byte* ext,
                          unsigned long* id);

    void mcp2515_write_canMsg(const byte buffer_sidh_addr, bool rtrBit);        // Write CAN message
    void mcp2515_read_canMsg(const byte buffer_sidh_addr);      // Read CAN message
    void mcp2515_start_transmit(const byte mcp_addr);           // Start transmission
    byte mcp2515_getNextFreeTXBuf(byte *txbuf_n);               // Find empty transmit buffer

// CAN operator function

    byte setMsg(unsigned long id, byte ext, byte len, byte rtr, byte *pData);   // Set message
    byte setMsg(unsigned long id, byte ext, byte len, byte *pData);             // Set message
    byte clearMsg();                                             // Clear all message to zero
    byte readMsg();                                              // Read message
    byte sendMsg(bool rtrBit);                                   // Send message

public:
    MCP_CAN(byte _CS);
    byte begin(byte idmodeset, byte speedset, byte clockset);    // Initilize controller prameters
    byte begin(byte speedset);                                   // Initilize controller prameters
    byte init_Mask(byte num, byte ext, unsigned long ulData);    // Initilize Mask(s)
    byte init_Mask(byte num, unsigned long ulData);              // Initilize Mask(s)
    byte init_Filt(byte num, byte ext, unsigned long ulData);    // Initilize Filter(s)
    byte init_Filt(byte num, unsigned long ulData);              // Initilize Filter(s)
    byte setMode(byte opMode);                                   // Set operational mode
    byte sendMsgBuf(unsigned long id, byte ext, byte rtr, byte len, byte *buf);     // Send message to transmit buffer
    byte sendMsgBuf(unsigned long id, byte len, byte *buf);                         // Send message to transmit buffer
    byte sendMsgBuf(unsigned long id, byte ext, byte len, byte *buf);               // Send message to transmit buffer
    byte readMsgBuf(byte *len, byte *buf);                       // Read message from receive buffer
    byte readMsgBufID(unsigned long *ID, byte *len, byte *buf);  // Read message with ID from receive buffer
    byte readMsgBufID(unsigned long *ID, byte *ext, byte *len, byte *buf);          // Read message with ID from receive buffer
    byte checkReceive(void);                                     // Check for received data
    byte checkError(void);                                       // Check for errors
    byte getError(void);                                         // Check for errors
    byte errorCountRX(void);                                     // Get error count
    byte errorCountTX(void);                                     // Get error count
    byte enOneShotTX(void);                                      // Enable one-shot transmission
    byte disOneShotTX(void);                                     // Disable one-shot transmission
    unsigned long getCanId(void);                                // get can id when receive
    byte isRemoteRequest(void);                                  // get RR flag when receive
    byte isExtendedFrame(void);                                  // did we recieve 29bit frame?

    bool pinMode(const byte pin, const byte mode);               // switch supported pins between HiZ, interrupt, output or input
    bool digitalWrite(const byte pin, const byte mode);          // write HIGH or LOW to RX0BF/RX1BF
    byte digitalRead(const byte pin);                            // read HIGH or LOW from supported pins
};

#endif
/*********************************************************************************************************
 *  END FILE
 *********************************************************************************************************/
