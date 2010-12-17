/*
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


#include "lpc2220.h"
#include "serial.h"

extern const char hval[16];

/* flush serial input queue. returns 0 on success or negative error
 * number otherwise
 */
int serial_flush_input(void)
{
	volatile unsigned int tmp;

	/* keep on reading as long as the receiver is not empty */
	while(RX_DATA(U0LSR)) {
		tmp = GET_CHAR(U0RBR);
	}

	return 0;
}


/* flush output queue. returns 0 on success or negative error number
 * otherwise
 */
int serial_flush_output(void)
{
	/* wait until the transmitter is no longer busy */
	while(TX_READY(U0LSR)==0) {
	}

	return 0;
}


void serial_setbrg (unsigned int baudrate)
{

	/* get correct divisor */
    /*for PCLK 15 Mhz */
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
	}
	U0LCR = 0x03;

	serial_flush_output();
	serial_flush_input();
}


/*
 * Initialise the serial port with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 *
 */
int serial_init (void)
{
//	PINSEL0=0x05;			// pin select -> tx, rx
	serial_setbrg (115200);
//	serial_setbrg (57600);
	U0FCR = 0x07;			// enable fifo
	return (0);
}


/*
 * Output a single byte to the serial port.
 */
void serial_putc (const char c)
{
	/* wait for room in the transmit FIFO */
	while(TX_READY(U0LSR)==0);

	PUT_CHAR(U0THR,c);
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */
int serial_tstc (void)
{
	return (RX_DATA(U0LSR));
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */
int serial_getc (void)
{
	int rv;

	for(;;) {
		rv = serial_tstc();

		if(rv > 0)
			return (GET_CHAR(U0RBR));
	}
}

void
serial_puts (const char *s)
{
	while (*s != 0) {
		serial_putc (*s++);
	}
}

void putHexC(const unsigned char v)
{
	serial_putc(hval[v>>4]);
	serial_putc(hval[v & 0x0F]);
}

void putHexS(const unsigned short v)
{
	putHexC(v >> 8);
	putHexC(v & 0xFF);
}

void putHexW(const unsigned long v)
{
	putHexS(v >> 16);
	putHexS(v & 0xFFFF);
}
