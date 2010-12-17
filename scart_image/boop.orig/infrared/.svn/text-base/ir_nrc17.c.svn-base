/*
    ir_nrc17.c - nokia remote control 17 encoder
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
#include "ir_nrc17.h"
#include "encoders.h"
#include "codes.h"

#define NRC17_IDLE	0x00
#define NRC17_PRE_A	0x01
#define NRC17_PRE_B	0x02
#define NRC17_START_A	0x03
#define NRC17_START_B	0x04
#define NRC17_BIT_A	0x05
#define NRC17_BIT_B	0x06
#define NRC17_WAIT	0x07

#define NRC17_BONTIME	19
#define NRC17_BOFFTIME	19
#define NRC17_PTIME	95
#define NRC17_GTIME	156
#define NRC17_WTIME	38

#define NRC17_STARTSTOP_DBOX2	0xFEFF
#define NRC17_STARTSTOP_TP720	0x01FF

const struct irModule NRC17_Module =
{ 
	NRC17_Encode,
	NRC17_Send,
	NRC17_Repeat,
	NRC17_Stop,
	80,	// carrier normal mode
	96,	// carrier turbo mode
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

extern char sysInfo;

void __attribute__ ((section(".text.fastcode"))) NRC17_Encode(void) 
{
	switch(ir_state)
	{
		case NRC17_IDLE:
			mod_enable = 0;
			break;

		case NRC17_PRE_A:
		case NRC17_START_A:
			cycles = NRC17_BONTIME;	// 0.5 ms
			mod_enable = 1;
			ir_state++;
			break;

		case NRC17_PRE_B:
		case NRC17_START_B:
			if(ir_state == NRC17_PRE_B)
				cycles = NRC17_PTIME;	// 2,5 ms
			else
				cycles = NRC17_BOFFTIME;	// 0.5 ms
			mod_enable = 0;
			ir_state++;
			break;

		case NRC17_BIT_A:
			if(ir_cmd & 0x0001)
				mod_enable = 1;
			else
				mod_enable = 0;
			ir_cmd >>= 1;
			ir_state++;
			break;

		case NRC17_BIT_B:
			if(mod_enable == 1)
				mod_enable = 0;
			else
				mod_enable = 1;
			ir_bit++;
			if(ir_bit == ir_numbits)
				ir_state++;
			else
				ir_state--;
			break;

		case NRC17_WAIT:
			mod_enable = 0;
			ir_wait++;
			cycles = NRC17_BONTIME;		// 1 ms
			if(ir_wait >= ir_pause)
			{
				ir_bit = 0;
				ir_wait = 0;
				ir_pause = NRC17_PTIME;
				ir_state = NRC17_IDLE;
			}
			break;

	}
}

void NRC17_CopyMap(unsigned char xtra, unsigned short *map)
{
	int x;
	for(x=0;x<42;x++)
	{
		keyMap[x] = map[x];
	}
}

void NRC17_LoadMap(unsigned char map)
{	
	switch(map)
	{
		case 1:		// tp720 - tv, 9 bit data
			ir_startstop = NRC17_STARTSTOP_TP720;
			ir_numbits = 9;
			break;
		default:	// nokia dbox2, 16 bit data
			ir_startstop = NRC17_STARTSTOP_DBOX2;
			ir_numbits = 16;
			break;
	}

}

void NRC17_Init(unsigned char map)
{
	if(map < NRC17.num_tables)
	{
		ir_state	= NRC17_IDLE;
		ir_cmd	= 0x0000;
		ir_actcmd	= 0x0000;
		ir_bit	= 0x00;
		ir_numbits	= 16;
		ir_wait	= 0;
		ir_pause	= NRC17_BONTIME;
		copyMapS((unsigned short*)NRC17.table[map].codes);
		NRC17_LoadMap(map);
		setIR(NRC17_Module);
	}
}

void NRC17_Send(unsigned long cmd)
{
	ir_actcmd = cmd;
	if(ir_actcmd != 0x0000)
	{
		setIRspeed(NRC17_Module);
		if(ir_state == NRC17_IDLE)
		{
			ir_pause = NRC17_WTIME;
			ir_cmd = ir_startstop;
			ir_state++;
			runIR();
		}
	}
}

void NRC17_Repeat(void)
{
	if(ir_actcmd != 0x0000)
	{
		if(ir_state == NRC17_IDLE)
		{
			ir_pause = NRC17_GTIME;
			ir_cmd = ir_actcmd;
			ir_state++;
		}
	}
}

void NRC17_Stop(void)
{
	if(ir_actcmd != 0x0000)
	{
retry:
		if(ir_state == NRC17_IDLE)
		{
			ir_cmd = ir_startstop;
			ir_state++;
		}
		else
			goto retry;
	
waitend:
		if(ir_state != NRC17_IDLE)
			goto waitend;
	}
	ir_actcmd = 0x0000;
	stopIR();
}	

