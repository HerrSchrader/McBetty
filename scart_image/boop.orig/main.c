/*
    main.c - main program routines
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

//typedef volatile unsigned int *reg32_t;

#include "lpc2220.h"


#include "lcd.h"
#include "irq.h"
#include "keyboard.h"
#include "lcd.h"
#include "fonty.h"
#include "sid.h"
#include "sound.h"
//#include "sounds.h"
//#include "sound3.h"
#include "infrared.h"
#include "codes.h"
#include "encoders.h"
#include "ir_selector.h"
#include "serial.h"
#include "flash.h"
#include "rf.h"
#include "menu.h"
#include "testmenu.h"
#include "mainmenu.h"
#include "timerfuncs.h"
#include "global.h"
#include "adc.h"
#include "backlight.h"
#include "rtc.h"
#include "i2c.h"
#include "bfs.h"
#include "settingsmenu.h"
#include "ir_raw.h"
#include "ir_capture.h"
#include "controls.h"

unsigned char sysInfo;

extern volatile unsigned int keys[2];
//extern unsigned int keys[2];

extern volatile char timeout;

extern ir_sfn	irSend;
extern ir_fn	irRepeat;
extern ir_fn	irStop;

const char hval[16] = "0123456789ABCDEF";

unsigned int oldkeys[2];
unsigned int key_state;

extern unsigned char act_encoder, act_set;

extern unsigned char bl_val;

void setSpeed(unsigned char sp)
{
	disableIRQ();
	switch(sp)
	{
		case SPEED_60:		// turbo speed (60 mhz)
			BCFG0	= 0x100005EF;
			PLLCFG	= 0x45;		// 0100.0101	= PSEL=10=4	MSEL=00101=6 = 240mhz Fcco
			PLLFEED = 0xAA;
			PLLFEED = 0x55;

			VPBDIV = 0x00;

			BCFG0	= 0x100004A0;
			BCFG2	= 0x100004A0;
			BCFG1	= 0x00000C21;
			sysInfo |= 0x80;
			break;

		case SPEED_30:	// normal speed (30 mhz)
		default:
			BCFG0	= 0x100005EF;
			PLLCFG	= 0x42;		// 0100.0010	= PSEL=10=4	MSEL=00010=3
			PLLFEED = 0xAA;
			PLLFEED = 0x55;

			VPBDIV = 0x02;

			BCFG0	= 0x10000420;
			BCFG2	= 0x10000420;
			BCFG1	= 0x00000400;
			sysInfo &= 0x7F;
			break;
	}
	enableIRQ();
}

void drawMainscreen(void) {
	
	set_font(SMALLFONT);
	lcd_fill(0);
	draw_logo();
	
	draw_block(0, 0, 99, 9, 3, DRAW_ERASE);
	draw_string(0,0,getSetName(irDevTab.device[irDevTab.active].encoder, irDevTab.device[irDevTab.active].set), 3, DRAW_PUT);
	
	printTime(86,152,(struct time_ *)&time);
	printDate(0,152,(struct date_ *)&date,0);
	
	showBat();
}


unsigned char i;
unsigned short xx;

void ask_for_time(unsigned char dest) {
	unsigned char xmit[5];
	RF_sendWOR(dest);
	xmit[0] = 4;
	xmit[1] = dest;
	xmit[2] = cc1100Addr;
	xmit[3] = 3;
	xmit[4] = 1;
	switch_WOR_RX();
	RF_send(xmit);
}

int main(void)
{
	sysInfo = 0;
	
	SCS |= 0x01;
	FIODIR0 |= ((1<<21) | (1<<4) | (1<<11) | (1<<6) | (1<<23) | (1<<19) | (1<<17));
	FIODIR0 |= (1<<12);
	FIOSET0 |= (1<<12);
	xx = 0x00;

	setSpeed(SPEED_30);

	lcd_init(0);
	serial_init();

	startTimerIRQ();
	startADC();

	initKeys();

	initSound();
	startSoundIRQ();

	initIR();
	startIrIRQ();

    RF_init();
    startcc1100IRQ();
	enableWOR();
	
	initRTC();
	startRtcIRQ();
	
	enableIRQ();

	testmenu_init();
	init_menu();
	
	initBacklight();

	oldkeys[0] = keys[0];
	oldkeys[1] = keys[0];

	key_state = KEY_IDLE;

	set_font(BOLDFONT);
	
	BFS_Mount();
	load_RC_setting();
	load_RF_setting();
	load_setting();
	
	{
		struct RAWset_ RAWset;
        unsigned char x;
        unsigned long RAWcmdbase;
        
        RAWcmdbase = FLASH1_BASE +(secaddr[0]<<1);
        x=memcmp((void*)RAWcmdbase,"RC01",4);
		
        if(!x) {
               
				memcpy(&RAWset,(void *)RAWcmdbase,sizeof(struct RAWset_));
				RAWset.name[7] = 0;
				BFS_SaveFile(BFS_ID_RAWslot0, sizeof(struct RAWset_), (unsigned char*) &RAWset);
				eraseSector(1,0);
        }
	}
	
	if (EncIsValid(irDevTab.device[irDevTab.active].encoder, irDevTab.device[irDevTab.active].set)) {
		setEncoder(irDevTab.device[irDevTab.active].encoder, irDevTab.device[irDevTab.active].set);
	}	
	
	drawMainscreen();
	ask_for_time(0);
	
						
/*
	playSound((unsigned char*)sound1_data, sound1_len);
	waitSound();
	playSound((unsigned char*)sound2_data, sound2_len);
*/

	while (1)
	{
		if(keys[0] != oldkeys[0] || keys[1] != oldkeys[1])
		{
			oldkeys[0] = keys[0];
			oldkeys[1] = keys[1];
			sysInfo |= 0x40;
		}

		switch(key_state)
		{
			case KEY_IDLE:
				if(sysInfo & 0x40)
				{
					sysInfo &= 0xBF;
					if(KEY_Betty)
					{
						setBacklight(BL_AUTO);
						menu_exec(&mainMenu);
						if (EncIsValid(irDevTab.device[irDevTab.active].encoder, irDevTab.device[irDevTab.active].set)) {
							setEncoder(irDevTab.device[irDevTab.active].encoder, irDevTab.device[irDevTab.active].set);
						}
						drawMainscreen();
					}
					else if(KEY_2)
					{
						//setSpeed(SPEED_30);
					}
					else if(KEY_3)
					{
						//setSpeed(SPEED_60);
					}
					else  if(KEY_A || KEY_B || KEY_C || KEY_D)
					{
						unsigned char x;
						//playSound((unsigned char*)sound3_data, sound3_len);
						x=0;
						if (KEY_B) x=1;
						if (KEY_C) x=2;
						if (KEY_D) x=3;
						
						if (EncIsValid(irDevTab.device[x].encoder, irDevTab.device[x].set)) {
							setBacklight(BL_AUTO);
							irDevTab.active = x;
							setEncoder(irDevTab.device[x].encoder, irDevTab.device[x].set);
							drawMainscreen();
						}	
					}
/*					else if(KEY_B)
					{
					
					}
					else if(KEY_C)
					{	
//						playSound((unsigned char*)sound1_data, sound1_len);
					}
					else if(KEY_D)
					{	
//						playSound((unsigned char*)sound2_data, sound2_len);
					}*/
					if((keys[0] != 0) || (keys[1] != 0))
						key_state = KEY_PRESS;
				}
				break;
			case KEY_PRESS:
				irSend(getCode());
				key_state = KEY_HOLD;
//				autorepeat = 0;
				break;
			case KEY_HOLD:
//				if(autorepeat >= AUTO_TIMEOUT)
					irRepeat();

				if(keys[0] == 0 && keys[1] == 0)
					key_state = KEY_RELEASE;

				break;
			case KEY_RELEASE:
				irStop();
				key_state = KEY_IDLE;
				break;
		}

		if(serial_tstc() > 0)
		{
			i = serial_getc();
			if(i=='.')
			{
				serial_puts("HELO");
			}
			else if(i=='0')
			{
				setBacklight(0x00);	// pwm value
			}
			else if(i=='1')
			{
				setBacklight(0x1F);	// pwm value
			}
			else if(i=='2')
			{
				setBacklight(0x3F);	// pwm value
			}
			else
				serial_putc(i);
		}
		
		if ((bl_val == 0) && (key_state == KEY_IDLE) && !((FIOPIN0 & (1<<30)) == 0) && (((RFstatus & (WORrxon | RXenabled)) == 0))) {
			EXTINT = 0x08;
			PCON = 0x02;
			PLLFEED = 0xAA;
			PLLFEED = 0x55;
		}
		
		if (timeInfo & timechanged) {
			printTime(86,152,(struct time_ *)&time);
			printDate(0,152,(struct date_ *)&date,0);
			
			timeInfo &= ~timechanged;
		}
		
	}
	return 0;
}


