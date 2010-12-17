/*
    ir_recs80.c - space modulated protocoll encoder
    Copyright (C) 2007  Ch. Klippel <ck@mamalala.net>

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
#include "infrared.h"
#include "ir_recs80.h"
#include "encoders.h"
#include "codes.h"

const struct irModule RECS80_Module =
{ 
	RECS80_Encode,
	RECS80_Send,
	RECS80_Repeat,
	RECS80_Stop,
	80,	// carrier 
	96,	// carrier 
	1,	// carrier-on
	4	// carrier-off
};

extern volatile unsigned char mod_enable;
extern volatile unsigned int cycles;
extern volatile unsigned long keyMap[42];

extern unsigned int	ir_cmd;
extern unsigned int	ir_actcmd;
extern unsigned int	ir_startstop;
extern unsigned int	ir_state;
extern unsigned int	ir_bit;
extern unsigned int	ir_numbits;
extern unsigned int	ir_wait;
extern unsigned int	ir_pause;
extern unsigned int	ir_leadtime;
extern unsigned int	ir_bittime0;
extern unsigned int	ir_bittime1;
extern unsigned int	ir_toggle;
extern unsigned int	ir_len;
extern unsigned int	ir_trail;
extern unsigned int	ir_preamble;

extern char sysInfo;

#define RECS80_IDLE	0x00
#define RECS80_BIT_A	0x01
#define RECS80_BIT_B	0x02
#define RECS80_STOP	0x03
#define RECS80_WAIT	0x04

#define RECS80_NUMBITS	 11
#define RECS80_BITTIME	  7
#define RECS80_BIT0	 27*RECS80_BITTIME
#define RECS80_BIT1	 41*RECS80_BITTIME
#define RECS80_WAITTIME	 10

void __attribute__ ((section(".text.fastcode"))) RECS80_Encode (void)
{
	switch(ir_state)
	{
		case RECS80_IDLE:
			mod_enable = 0;
			break;

		case RECS80_BIT_A:
		case RECS80_STOP:
			cycles = RECS80_BITTIME;
			mod_enable = 1;
			ir_state++;
			break;

		case RECS80_BIT_B:
			mod_enable = 0;
			if(ir_cmd & 0x0400)
				cycles = RECS80_BIT1;
			else
				cycles = RECS80_BIT0;
			ir_cmd <<= 1;
			ir_bit++;
			if(ir_bit == RECS80_NUMBITS)
				ir_state++;
			else
				ir_state--;
			break;

		case RECS80_WAIT:
			mod_enable = 0;
			cycles = RECS80_BIT0;
			ir_wait++;
			if(ir_wait >= RECS80_WAITTIME)
			{
				ir_bit = 0;
				ir_wait = 0;
				ir_state = RECS80_IDLE;
			}
			break;
	}
}

void RECS80_Init(unsigned char map)
{
	if(map < RECS80.num_tables)
	{
		ir_state		= RECS80_IDLE;
		ir_cmd		= 0x0000;
		ir_actcmd		= 0x0000;
		ir_bit		= 0x00;
		ir_wait		= 0;
		ir_toggle		= 1;
		copyMapS((unsigned short*)RECS80.table[map].codes);
		setIR(RECS80_Module);
		cycles = RECS80_BITTIME;
	}
}

void RECS80_Send(unsigned long cmd)
{
	ir_actcmd = cmd;

	if(ir_toggle & 0x01)
		ir_actcmd |= 0x0200;
	setIRspeed(RECS80_Module);
	RECS80_Repeat();
}

void RECS80_Repeat(void)
{
	if(ir_actcmd != 0x0000)
	{
		if(ir_state == RECS80_IDLE)
		{
			ir_cmd = ir_actcmd;
			ir_state++;
			runIR();
		}
	}
}

void RECS80_Stop(void)
{
	if(ir_actcmd != 0x00000000)
	{
waitend:
		if(ir_state != RECS80_IDLE)
			goto waitend;
	}
	ir_toggle++;
	stopIR();
}
