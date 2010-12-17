/*
 * more changes (c) 2010 H. Raap
 * minor changes (c) 2007 ch.klippel
 *
 * (C) Copyright 2004
 * DAVE Srl
 * http://www.dave-tech.it
 * http://www.wawnet.biz
 * mailto:info@wawnet.biz
 *
 * (C) Copyright 2002-2004
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * Copyright (C) 1999 2000 2001 Erik Mouw (J.A.K.Mouw@its.tudelft.nl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* These routines are only used for debugging. 
	NOTE We can comment them out in release version, but better leave them in.
*/


#include "lpc2220.h"
#include "kernel.h"
#include "fonty.h"

#define	USRRxData      	(1 << 0)
#define	USRTxHoldEmpty 	(1 << 6)

#define TX_READY(s)    	((s) & USRTxHoldEmpty)
#define RX_DATA(s)     	((s) & USRRxData)

#define PUT_CHAR(p,c)  	(p= (unsigned )(c))


/* This global variable is set to TRUE (<> 0) if we want debugging output. 
	Is initialized in serial_init to 0. NOTE currently initialized as 1
*/
int fDebug;


/* 
	The serial port is currently only used for output of debugging messages.
	Debugging should minimally disturb the normal operation of the program.
	So we avoid waiting in debugging at all costs.
	Therefore the sematics are that whenever the debugging output would block
	(UART in use by other thread, message buffer full etc.)
	we simply output as much as possible and return.
	
	Simple API:
	We only use one function (debug_out) to send things to the serial port.
	debug_out is non-blocking, so we do not need a semaphore.
	Bytes are copied to an internal buffer and output via a separate thread.
*/


/* 
	Flush serial input queue. 
*/
static int 
serial_flush_input(void) {
	/* Setting the FIFO RX reset bit. */
	U0FCR = U0FCR | (1<<1);
	return 0;
}


/* flush output queue.
 */
static int
serial_flush_output(void) {
	/* wait until the transmitter is no longer busy */
	while(TX_READY(U0LSR)==0) {
	}

	return 0;
}

/* Set the desired baudrate, assuming a PCLK of 15 MHz */
static void 
serial_setbrg (unsigned int baudrate) {
	/* Allow access to divisor latch register */
	U0LCR = 0x83;

	switch(baudrate) {

	case 75:
		U0DLL = 5;
		U0DLM = 195;
		U0FDR = (1 << 4) + 3;
		break;

	case 150:
		U0DLL = 5;
		U0DLM = 195;
		U0FDR = (1 << 4) + 1;
		break;

	case 300:
		U0DLL = 5;
		U0DLM = 195;
		U0FDR = (1 << 4) + 0;
		break;

	case 600:
		U0DLL = 2;
		U0DLM = 78;
		U0FDR = (4 << 4) + 1;
		break;

	case 1200:
		U0DLL = 1;
		U0DLM = 39;
		U0FDR = (4 << 4) + 1;
		break;

	case 2400:
		U0DLL = 186;
		U0DLM = 0;
		U0FDR = (10 << 4) + 11;
		break;

	case 4800:
		U0DLL = 93;
		U0DLM = 0;
		U0FDR = (10 << 4) + 11;
		break;

	case 9600:
		U0DLL = 71;
		U0DLM = 0;
		U0FDR = (8 << 4) + 3;
		break;

	case 14400:
		U0DLL = 31;
		U0DLM = 0;
		U0FDR = (10 << 4) + 11;
		break;

	case 19200:
		U0DLL = 38;
		U0DLM = 0;
		U0FDR = (14 << 4) + 4;
		break;

	case 28800:
		U0DLL = 24;
		U0DLM = 0;
		U0FDR = (14 << 4) + 5;
		break;

	case 38400:
		U0DLL = 19;
		U0DLM = 0;
		U0FDR = (14 << 4) + 4;
		break;

	case 57600:
		U0DLL = 12;
		U0DLM = 0;
		U0FDR = (14 << 4) + 5;
		break;

	case 115200:
		U0DLL = 6;
		U0DLM = 0;
		U0FDR = (14 << 4) + 5;
		break;

	case 230400:
		U0DLL = 3;
		U0DLM = 0;
		U0FDR = (14 << 4) + 5;
		break;
	};
	
	/* Access to FIFO */
	U0LCR = 0x03;

	serial_flush_output();
	serial_flush_input();
}


#define BUF_NUM 1024
static char out_buf[BUF_NUM];
static int buf_first;
static int buf_last;

static char 
get_from_buf(){
	char val = out_buf[buf_first++];
	if (buf_first >= BUF_NUM)
		   buf_first = 0;
	return val;
};

static int 
put_in_buf(char c){
	int new_last = buf_last+1;
	if (new_last >= BUF_NUM)
		new_last=0;
	if (new_last == buf_first)
		return 0;				// Buffer is full !
	out_buf[buf_last]=c;
	buf_last = new_last;
	return 1;
};

/* Output a string to serial output buffer 
	Returns 0 if string does not fit in buffer.
*/
static int
serial_puts (const char *s){
	while (*s != 0) {
		if (0 == put_in_buf (*s++))
			return 0;
	};
	return 1;
}


/* ### Serial Ouput Thread ### 
	NOTE could be done as an interrupt 
*/
PT_THREAD (serial_out(struct pt *pt)){
	PT_BEGIN(pt);
	while(1){
		PT_WAIT_UNTIL(pt, (buf_first != buf_last) && TX_READY(U0LSR));
		PUT_CHAR(U0THR, get_from_buf());
	};
	PT_END(pt);
};


/*
 * Initialise the serial port (UART0) with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 *
 * To be called once before using the serial port
 */
int 
serial_init (int baudrate){
	
	/* Select UART0 function for P0.0 and P0.1 */
	PINSEL0 = (PINSEL0 & (~0x0f)) | 0x05;
	
	serial_setbrg (baudrate);		// set given baudrate
	U0FCR = 0x07;				// enable fifo
	
	fDebug = 1;
	buf_first=0;					// empty the output buffer
	buf_last=buf_first;
	return (0);
}

/* A single byte is output as 2 hex digits. */
static int
putHexC(unsigned char v){
	char num_chars[3];
	
	get_hex_digits(v, num_chars);
	num_chars[2] = 0;
	return serial_puts(num_chars);
}

/* A 2 byte word is output as 4 hex digits. */
static int 
putHexS(unsigned short v){
	if (0== putHexC(v >> 8))
		return 0;
	return putHexC(v & 0xFF);
}

/* A 4 byte word is output as 8 hex digits. */
static int 
putHexW(unsigned long v){
	if (0 == putHexS(v >> 16))
		return 0;
	return putHexS(v & 0xFFFF);
}


/* A string followed by an integer is output on serial port (followed by newline) 
	We check the global variable fDebug if we should do any debug output at all
*/
void 
debug_out(char *s, unsigned int v){
	
	if (!fDebug) return;
	
	if (0 == serial_puts(s))
		return;
	if (v < (1<<8)){
		if (0 == putHexC(v))
			return;
	} else {
		if ( v < (1<<16) ) {
			if (0 == putHexS(v))
				return;
		} else { 
			if (0 == putHexW(v))
				return;
		};
	};
	put_in_buf('\n');
};
