/*
    rf.c - 
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

#include "lpc2220.h"
#include "rf.h"
#include "cc1100.h"
#include "timerfuncs.h"
#include "irq.h"
#include "cc1100.h"
#include "lcd.h"
#include "fonty.h"
#include "rtc.h"
#include "global.h"

unsigned char RFbuf[64];
unsigned int WORrx_cb;
volatile unsigned int RFstatus;
unsigned char cc1100Addr;

const struct RFsettings_ default_RFsettings =
{
	0x01,	//address
	0x01	//channel
};

void switch_to_idle() {
	cc1100_strobe(SIDLE);
	while (cc1100_read1(0x35 | READ | BURST) != 01);
}

void RF_init (void) {
	cc1100_init();
	RFstatus = 0;
	cc1100Addr = conf[0x09];
}

void draw_ant(unsigned char x, unsigned char y, unsigned char m) {

	draw_vline(x+3,y,7,3,m);
	draw_pixel(x,y,3,m);
	draw_pixel(x+1,y+1,3,m);
	draw_pixel(x+2,y+2,3,m);
	draw_pixel(x+6,y,3,m);
	draw_pixel(x+5,y+1,3,m);
	draw_pixel(x+4,y+2,3,m);
	
}

void enableWOR(void) {
	
	cc1100_strobe(SIDLE);
	RFstatus |= WORenabled;
	cc1100_strobe(SWOR);
	
}

/*void enableRX(void) {

	unsigned char tmp;
	tmp = cc1100_strobe(SNOP);
	if (!(tmp & 0xf0)) {
		cc1100_strobe(SFRX);
		cc1100_strobe(SRX);
		RFstatus |= RXenabled;
	}
	else {
		cc1100_strobe(SIDLE);
		cc1100_strobe(SPWD);
		RFstatus &= ~RXenabled;
	}
}*/


void waitTX(void)
{
	unsigned char status;
	unsigned char x;
	
	x=1;
	while (x) {
		status = cc1100_strobe(SNOP);
		switch ( 0xf0 & status) {
			case 0x70:
					cc1100_strobe(SFTX);
			case  0x10:
			case  0x00:
				if (RFstatus & WORsend) {
					switch_to_idle();
				}
				else if (RFstatus & WORenabled)
					cc1100_strobe(SWOR);
				else if (RFstatus & WORrxon)
					cc1100_strobe(SRX);
				else if (RFstatus & RXenabled)
					cc1100_strobe(SRX);
				else {
					switch_to_idle();
				}					
				x=0;
				break;
		}	
	}
}

void RF_send(unsigned char* b) {
	
	VICIntEnClr = INT_EINT0;
	if(RFstatus & WORenabled) {
		switch_to_idle();
	}
	cc1100_write(TX_fifo | BURST,b,b[0]+1);
	cc1100_strobe(STX);
	waitTX();
	VICIntEnable = INT_EINT0;
}

void RF_getfifo(unsigned char* b) {

	cc1100_read(RX_fifo, b,1);
	cc1100_read(RX_fifo, &b[1],b[0]);
	
}

void WORsend_timeout(void) {
	RFstatus &= ~WORsend;
}

void RF_sendWOR(unsigned char addr) {

	unsigned char b[2];
	unsigned int WOR_cb;
	
	b[0]=0x01;
	b[1]=addr;
	
	VICIntEnClr = INT_EINT0;
	switch_to_idle();
	RFstatus |= WORsend;
	cc1100_write1(0x18,conf[0x18] & 0xCF);
	
	cc1100_strobe(SCAL);
	waitTX();
	WOR_cb = addTimerCB(WORsend_timeout, 101);
	startCB(WOR_cb);
	
	while (RFstatus & WORsend) {
		cc1100_write(TX_fifo | BURST,b,2);
		cc1100_strobe(STX);
		waitTX();
	}
	
	stopCB(WOR_cb);
	removeTimerCB(WOR_cb);
	
	cc1100_write1(0x18,conf[0x18]);
	waitTX();
	VICIntEnable = INT_EINT0;	
}

void WORrx_timeout(void) {
	stopCB(WORrx_cb);
	removeTimerCB(WORrx_cb);
	RFstatus &= ~WORrxon;
	draw_ant(100,0,DRAW_ERASE);
	cc1100_write1(0x16,conf[0x16]);
	RFstatus |= WORenabled;
	
	cc1100_strobe(SIDLE);
	cc1100_strobe(SWOR);
}

void switch_WOR_RX(void) {
	if (RFstatus & WORrxon)
		setCBIntervall(WORrx_cb,WORrx_time);
	else {
		WORrx_cb = addTimerCB(WORrx_timeout, WORrx_time);
		startCB(WORrx_cb);
		RFstatus |= WORrxon;
		RFstatus &= ~WORenabled;
		draw_ant(100,0,DRAW_PUT);
		cc1100_write1(0x16,0x07);
	}
}

void cc1100IRQ (void) {
		
	if (cc1100_read1(0x3B | READ | BURST) > 0) {
		RF_getfifo(RFbuf);
		cc1100_strobe(SIDLE);
		cc1100_strobe(SFRX);
				
		struct cc1100frame_ *RXframe;
		RXframe = (struct cc1100frame_ *)&RFbuf[0];
		
		if ((RXframe->len == 1) && ((RXframe->destAddr == cc1100Addr)  || (RXframe->destAddr == 0x00))) {
			switch_WOR_RX();
		}
		
		if(RFstatus & WORenabled)
			cc1100_strobe(SWOR);
		else
			cc1100_strobe(SRX);
			
		if (RXframe->len > 3) {
			setCBIntervall(WORrx_cb,WORrx_time);
			switch ( RXframe->packetType) {
				case 0x01:
					draw_block(0,50,128,10,3,DRAW_ERASE);
					draw_string(0, 50, (char*)&RFbuf[headerLen], 3, DRAW_PUT);
				break;
				case 0x02:
					if (RXframe->data[0] == 0x01) {
						RXframe->destAddr = RXframe->srcAddr;
						RXframe->srcAddr = cc1100Addr;
						RXframe->data[0] = 0x02;
						RXframe->len = 0x04;
						switch_to_idle();
						RF_send(RFbuf);
					}
					else if (RFbuf[headerLen] == 0x02) {
						RFstatus |= Received;
					}
				break;
				case 0x03:
					if ((RXframe->data[0] == 0x01) && (timeInfo & time_accurate)) {
						send_time(RXframe->srcAddr);
					}
					else if (RFbuf[headerLen] == 0x02) {
						memcpy((unsigned char *)&time,(unsigned char *)&RXframe->data[1],3);
						memcpy((unsigned char *)&date,(unsigned char *)&RXframe->data[4],4);
						timeInfo |=0x02;
						VICSoftInt = INT_EINT2;
					}
			}
		}
	}
	
	waitTX();
	EXTINT = 0x01;
}

void startcc1100IRQ(void) {
	
	PINSEL1 |= 1;
	EXTMODE |= 1;
	EXTPOLAR |= 0;	
	EXTWAKE |= 1;
	EXTINT |= 1;
	
	VICVectAddr2 = (unsigned long)&(cc1100IRQ);
	VICVectCntl2 = VIC_SLOT_EN | INT_SRC_EINT0;
	VICIntEnable = INT_EINT0;
}

	
