/*
    ir_rc5.c - phillips rc5 protocoll encoder
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
#include "ir_rc5.h"
#include "encoders.h"
#include "codes.h"

const struct irModule RC5_Module =
{
	RC5_Encode,
	RC5_Send,
	RC5_Repeat,
	RC5_Stop,
	84,	// carrier normal mode
	101,	// carrier turbo mode
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

extern char sysInfo;

#define RC5_IDLE	0x00
#define RC5_START_A	0x01
#define RC5_START_B	0x02
#define RC5_BIT_A	0x03
#define RC5_BIT_B	0x04
#define RC5_WAIT	0x05


#define RC5_BITTIME	 32
#define RC5_WAITTIME	100

void __attribute__ ((section(".text.fastcode"))) RC5_Encode (void)
{
	switch(ir_state)
	{
		case RC5_IDLE:
			mod_enable = 0;
			break;

		case RC5_START_A:
			mod_enable = 0;
			ir_state++;
			break;

		case RC5_START_B:
			mod_enable = 1;
			ir_state++;
			break;

		case RC5_BIT_A:
			if(ir_cmd & 0x1000)
				mod_enable = 0;
			else
				mod_enable = 1;
			ir_cmd <<= 1;
			ir_state++;
			break;

		case RC5_BIT_B:
			if(mod_enable == 0)
				mod_enable = 1;
			else
				mod_enable = 0;
			ir_bit++;
			if(ir_bit == 13)
				ir_state++;
			else
				ir_state--;
			break;

		case RC5_WAIT:
			mod_enable = 0;
			ir_wait++;
			if(ir_wait >= RC5_WAITTIME)
			{
				ir_bit = 0;
				ir_wait = 0;
				ir_state = RC5_IDLE;
			}
			break;
	}
}

void RC5_Init(unsigned char map)
{
	if(map < RC5.num_tables)
	{
		ir_state	= RC5_IDLE;
		ir_cmd		= 0x0000;
		ir_actcmd	= 0x0000;
		ir_bit		= 0x00;
		ir_wait	= 0;
		copyMapS((unsigned short*)RC5.table[map].codes);
		setIR(RC5_Module);
		cycles = RC5_BITTIME;
	}
}

void RC5_Send(unsigned long cmd)
{
	ir_actcmd = cmd;
	setIRspeed(RC5_Module);
	if((ir_actcmd != 0x0000) && (ir_actcmd != 0x1000))
	{
		//toggle bit sollte nicht über dir Konfiguration gesetzt werden
		if(ir_toggle & 0x01)
			ir_actcmd &= 0xF7FF;
		else
			ir_actcmd |= 0x0800;
		RC5_Repeat();
	}
}

void RC5_Repeat(void)
{
	if((ir_actcmd != 0x0000) && (ir_actcmd != 0x1000))
	{
		if(ir_state == RC5_IDLE)
		{
			ir_cmd = ir_actcmd;
			ir_state++;
			runIR();
		}
	}
}

void RC5_Stop(void)
{
	if((ir_actcmd != 0x0000) && (ir_actcmd != 0x1000))
	{
waitend:
		if(ir_state != RC5_IDLE)
			goto waitend;
	}

	ir_toggle++;
	ir_actcmd = 0x0000;
	stopIR();
}


