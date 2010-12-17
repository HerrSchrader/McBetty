/*
    rf.h - 
    Copyright (C) 2008  

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RF_H
#define RF_H

#define RXenabled		0x80
#define WORenabled		0x40
#define WORsend			0x20
#define WORrxon			0x08
#define Received		0x01

#define WORrx_time		200

struct cc1100frame_ {
	unsigned char len;	
	unsigned char destAddr;
	unsigned char srcAddr;	
	unsigned char packetType;
	unsigned char data[];
};
#define headerLen		0x04

struct RFsettings_ {
	unsigned char address;	
	unsigned char channel;
};

const struct RFsettings_ default_RFsettings;
unsigned char RFbuf[64];

unsigned char cc1100Addr;
volatile unsigned int RFstatus;

void startcc1100IRQ(void);
void cc1100IRQ (void);
void RF_init (void);
void RF_getfifo(unsigned char* b);
void switch_WOR_RX(void);
void enableWOR(void);
//void enableRX(void);
void RF_send(unsigned char* b);
void RF_sendWOR(unsigned char addr);
void waitTX(void);

#endif
