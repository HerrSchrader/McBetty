/*
    ir_rec80.c - nec rec80 and derivates ir encoder
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
#include "ir_rec80.h"
#include "encoders.h"
#include "codes.h"

const struct irModule REC80_Module =
{ 
	REC80_Encode,
	REC80_Send,
	REC80_Repeat,
	REC80_Stop,
	94,	// carrier normal mode
	113,	// carrier turbo mode
	1,	// carrier-on
	4,	// carrier-off
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

#define REC80_IDLE	0x00
#define REC80_HEAD_A	0x01
#define REC80_HEAD_B	0x02
#define REC80_PRE_A	0x03
#define REC80_PRE_B	0x04
#define REC80_BIT_A	0x05
#define REC80_BIT_B	0x06
#define REC80_TRAIL	0x07
#define REC80_WAIT	0x08

#define REC80_HEADER_1	128
#define REC80_HEADER_0	51
#define REC80_BONTIME	13
#define REC80_BITTIME_0	13
#define REC80_BITTIME_1	38
#define REC80_WAITTIME	187

void __attribute__ ((section(".text.fastcode"))) REC80_Encode (void)
{
	switch(ir_state)
	{
		case REC80_IDLE:
			mod_enable = 0;
			break;

		case REC80_HEAD_B:
			mod_enable = 0;
			cycles = REC80_HEADER_0;
			ir_cmd = ir_preamble;
			ir_bit = 0;
			ir_state++;
			break;

		case REC80_HEAD_A:
		case REC80_PRE_A:
		case REC80_BIT_A:
		case REC80_TRAIL:
			mod_enable = 1;
			if(ir_state == REC80_HEAD_A)
				cycles = REC80_HEADER_1;
			else
				cycles = REC80_BONTIME;
			ir_state++;
			break;

		case REC80_PRE_B:
		case REC80_BIT_B:
			mod_enable = 0;
			if(ir_cmd & 0x01)
				cycles = REC80_BITTIME_1;
			else
				cycles = REC80_BITTIME_0;
			ir_cmd >>= 1;
			ir_bit++;
			if(ir_bit == ir_numbits)
			{
				if(ir_state == REC80_PRE_B)
				{
					ir_bit = 0;
					ir_cmd = ir_actcmd;
				}
				ir_state++;
			}
			else
				ir_state--;
			break;

		case REC80_WAIT:
			mod_enable = 0;
			ir_wait++;
			cycles = REC80_BONTIME;		// 1 ms
			if(ir_wait >= ir_pause)
			{
				ir_bit = 0;
				ir_wait = 0;
				ir_state = REC80_IDLE;
			}
			break;
	}
}

void REC80_LoadMap(unsigned char map)
{
	switch(map)
	{
		default:
			ir_preamble	= 0x00A02002;
			copyMapI((unsigned int*)REC80.table[map].codes);
			break;
	}
}

void REC80_Init(unsigned char map)
{
	if(map < REC80.num_tables)
	{
		ir_cmd	= 0x00000000;
		ir_actcmd	= 0x00000000;
		ir_bit	= 0;
		ir_numbits	= 24;
		ir_wait	= 0;
		ir_pause	= REC80_WAITTIME;
		ir_state	= REC80_IDLE;
		REC80_LoadMap(map);
		setIR(REC80_Module);
	}
}

void REC80_Send(unsigned long cmd)
{
	ir_actcmd = cmd;

	if(ir_actcmd & 0x80000000)
		ir_numbits = 24;
	else
		ir_numbits = 0;

 	ir_actcmd &= 0x0FFFFFFF;
	
	setIRspeed(REC80_Module);
	REC80_Repeat();
}

void REC80_Repeat(void)
{
	if(ir_actcmd != 0x00000000)
	{
		if(ir_state == REC80_IDLE)
		{
			ir_state++;
			runIR();
		}
	}
}

void REC80_Stop(void)
{
	if(ir_actcmd != 0x00000000)
	{
waitend:
		if(ir_state != REC80_IDLE)
			goto waitend;
	}
	ir_actcmd = 0x00000000;
	stopIR();
}
