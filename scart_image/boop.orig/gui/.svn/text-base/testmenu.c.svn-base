/*
    testmenu.c - testmenu for menuing system
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

#include "menu.h"
#include "lcd.h"
#include "fonty.h"
#include "keyboard.h"
#include "backlight.h"
#include "adc.h"
#include "rf.h"
#include "flash.h"
#include "ir_raw.h"
#include "ir_capture.h"
#include "ir_selector.h"
#include "infrared.h"
#include "sid.h"
#include "timerfuncs.h"
#include "sound.h"
#include "lpc2220.h"
#include "controls.h"
#include "global.h"
//#include "sidfiles.h"

extern char sysInfo;

extern volatile unsigned int keys[2];
//extern unsigned int keys[2];
extern volatile unsigned char bat_state;

unsigned int testdata1, testdata2;

#define MAX_SOFTMENU	16

void test_circle(void)
{
	lcd_fill(0);
	draw_disc(20, 20, 18, 3, DRAW_PUT);
	draw_disc(20, 20, 14, 3, DRAW_XOR);

	draw_qdisc(60, 20, 18, 0, 3, DRAW_PUT);
	draw_qdisc(60, 40, 18, 1, 3, DRAW_PUT);
	draw_qdisc(60, 60, 18, 2, 3, DRAW_PUT);
	draw_qdisc(60, 80, 18, 3, 3, DRAW_PUT);

	draw_qcircle(100, 20, 18, 0, 3, DRAW_PUT);
	draw_qcircle(100, 40, 18, 1, 3, DRAW_PUT);
	draw_qcircle(100, 60, 18, 2, 3, DRAW_PUT);
	draw_qcircle(100, 80, 18, 3, 3, DRAW_PUT);


	draw_circle(30,120,20,3,DRAW_PUT);

	waitKeyUpDown();
}

void test_frame(void)
{
	lcd_fill(0);
	
	draw_frame(0,0,40,40,4,0,3,DRAW_PUT);
	draw_frame(42,0,40,40,3,16,3,DRAW_PUT);
	draw_frame(84,0,40,40,7,3,3,DRAW_PUT);


	draw_frame(0,42,40,40,4,7,3,DRAW_PUT);
	draw_frame(42,42,40,40,4,8,3,DRAW_PUT);
	draw_frame(84,42,40,40,1,7,3,DRAW_PUT);

	waitKeyUpDown();
}

void test_frame2(void)
{

	unsigned char t, r;
	t = 1;
	r = 0;
again:
	lcd_fill(0);

	draw_string(0, 152, "t:", LCD_COLOR_B, DRAW_PUT);
	draw_numU(12,152,t,0,LCD_COLOR_B,DRAW_PUT);
	draw_string(64, 152, "r:", LCD_COLOR_B, DRAW_PUT);
	draw_numU(76,152,r,0,LCD_COLOR_B,DRAW_PUT);
	
	draw_frame(0,0,128,128,t,r,3,DRAW_PUT);

	while(!KEY_Exit) 
	{
		waitKeyUpDown();
		if(KEY_Vplus)
		{
			t++;
			if(t>64)
				t = 1;
			goto again;
		}
		if(KEY_Vminus)
		{
			if(t==1)
				t = 64;
			else
				t--;
			goto again;
		}
		if(KEY_Pplus)
		{
			r++;
			if(r>63)
				r = 0;
			goto again;
		}
		if(KEY_Pminus)
		{
			if(r==0)
				r = 63;
			else
				r--;
			goto again;
		}
	}
}

void draw_fonts(void)
{
	lcd_fill(0);
	set_font(SMALLFONT);
	draw_string(0, 10, "Small Font", 3, DRAW_PUT);
	set_font(DOUBLEFONT);
	draw_string(0, 20, "Double Font", 3, DRAW_PUT);
	
	set_font(BOLDFONT);
	draw_string(0, 38, "Bold Font", 3, DRAW_PUT);
	set_font(BOLDDOUBLEFONT);
	draw_string(0, 48, "B.Dbl. Font", 3, DRAW_PUT);
	
	set_font(SMOOTHFONT);
	draw_string(0, 70, "Smooth Font", 3, DRAW_PUT);
	set_font(BOLDFONT);
	waitKeyUpDown();
}

void test_logo(void)
{
	lcd_fill(0);
	draw_logo();
	waitKeyUpDown();
}

void fp1(void)
{
	unsigned char x,y;
	lcd_fill(0);
	set_font(SMALLFONT);

	for(x=0;x<16;x++)
	{
		for(y=0;y<16;y++)
		{
			draw_char(x*8,(y*8)+16,x*16+y,3,DRAW_PUT);
		}
	}
	waitKeyUpDown();
}

extern volatile unsigned char bl_val;

void set_bl(void)
{
	lcd_fill(0);
	set_font(SMALLFONT);

	draw_string(0, 20, "Set Backlight:", 3, DRAW_PUT);
	draw_string(0, 30, "Prog/Vol+: brighter", 3, DRAW_PUT);
	draw_string(0, 40, "Prog/Vol-: darker", 3, DRAW_PUT);
	draw_numU(0, 64, bl_val, 0, 3, DRAW_PUT);
	
	while(!KEY_Exit)
	{
		waitKeyUpDown();
		bl_val &= 0x3F;
		if(KEY_Pplus || KEY_Pminus || KEY_Vplus || KEY_Vminus)
		{	
			draw_block(0,64,32,8,3,DRAW_ERASE);
			if(KEY_Pplus)
				bl_val++;
			if(KEY_Pminus)
				bl_val--;
			if(KEY_Vplus)
				bl_val+=10;
			if(KEY_Vminus)
				bl_val-=10;
			draw_numU(0, 64, bl_val, 0, 3, DRAW_PUT);
		}
	}

}

/*
void softfunc1(void)
{
	set_font(BOLDFONT);
	draw_block(0, 80, 128, 20, 3, DRAW_ERASE);
	draw_string(0, 80, "softfunc1 called", 3, DRAW_PUT);
while(!KEY_Exit) {};
}
*/

void draw_itile(unsigned char x, unsigned char y, unsigned char type)
{
again:
	if(type < 0xF0)
	{
		if(type & 0x10)
			draw_block(x+1,	y+1,	2,	2,	3,	DRAW_XOR);
		if(type & 0x20)
			draw_block(x+5,	y+1,	2,	2,	3,	DRAW_XOR);
		if(type & 0x40)
			draw_block(x+1,	y+5,	2,	2,	3,	DRAW_XOR);
		if(type & 0x80)
			draw_block(x+5,	y+5,	2,	2,	3,	DRAW_XOR);
	}

	type <<= 4;

	if(type)
	{
		x += 8;
		goto again;
	}
}

const char itilemap[7][7] = 
{
	{ 0x00, 0x00, 0x00, 0x6C, 0x90, 0x00, 0x00 },
	{ 0x00, 0x00, 0x66, 0x6C, 0x99, 0x90, 0x00 },
	{ 0x00, 0x06, 0x66, 0x6C, 0x99, 0x99, 0x00 },
	{ 0x00, 0x66, 0x66, 0x6C, 0x99, 0x99, 0x90 },
	{ 0x00, 0x66, 0x66, 0x6C, 0x99, 0x99, 0x90 },
	{ 0x06, 0x66, 0x66, 0x6C, 0x99, 0x99, 0x99 },
	{ 0x0A, 0xAA, 0xAA, 0xA0, 0x55, 0x55, 0x55 }
}; 

void illusion(void)
{
	int x,y,z;

	lcd_fill(0);
	z = 1;
	for(x=0;x<15;x++)
	{
		for(y=0;y<15;y++)
		{
			if(z++ & 0x01)
			{
				draw_block(2+(x*8), 16+(y*8), 8, 8, 3, DRAW_PUT);
			}
		}
	}

	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			draw_itile(2+(x*16), 24+(y*8), itilemap[y][x]);
		}
	}

	for(y=0;y<6;y++)
	{
		for(x=0;x<7;x++)
		{
			draw_itile(2+(x*16), 80+(y*8), ~itilemap[5-y][x]);
		}
	}


	waitKeyUpDown();
}

void test_bl(void)
{
	setBacklight(BL_AUTO);
}

void bat_dbg(void)
{
	bat_state ^= BAT_DEBUG;
}

void tkey(void)
{
	while(1)
	{
		waitKeyUpDown();
		setBacklight(BL_AUTO);
		lcd_fill(0);
		set_font(SMALLFONT);
		draw_hexW(0, 16, keys[0], 3, DRAW_PUT);
		draw_hexW(0, 24, keys[1], 3, DRAW_PUT);
	}
}

void test_RF(void) {
	
	unsigned char xmit[16];
	unsigned char destAddr;
	
	lcd_fill(0);
	destAddr = 0x01;
	
	set_font(BOLDFONT);
	draw_string(0, 0, "RF test", 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);
	draw_string(1, 15, "Destination address", 3, DRAW_PUT);
	set_font(SMALLFONT);
	
	draw_string(0, 60, "TV/Power Key:", 3, DRAW_PUT);
	draw_string(0, 69, "send something", 3, DRAW_PUT);
	draw_string(0, 86, "OK Key: Ping", 3, DRAW_PUT);

	
	draw_numU(1, 25, destAddr, 0, 3, DRAW_PUT);
	
	while(KEY_Exit) {};
	while(!KEY_Exit)
	{
		waitKeyUpDown();
		
		
		draw_numU(1, 25, destAddr, 0, 3, DRAW_XOR);
		if(KEY_Left) {
			destAddr--;
		}
		if(KEY_Right) {
			destAddr++;
		}
		draw_numU(1, 25, destAddr, 0, 3, DRAW_XOR);
		
		
		if(KEY_OK)
		{
			draw_block(0,40,128,10,3,DRAW_ERASE);
			draw_string(0, 40, "send ping", 3, DRAW_PUT);
			RF_sendWOR(destAddr);
			xmit[0] = 4;
			xmit[1] = destAddr;
			xmit[2] = cc1100Addr;
			xmit[3] = 2;
			xmit[4] = 1;
			switch_WOR_RX();
			RFstatus &= ~Received;
			RF_send(xmit);
			while ((RFstatus & WORrxon) && !(RFstatus & Received)) ;
			draw_block(0,40,128,10,3,DRAW_ERASE);
			if ((RFstatus & Received) && (RFbuf[03] == 0x02) && (RFbuf[04] == 0x02)) {
				draw_string(0, 40, "ping received", 3, DRAW_PUT);
			}
			else {
				draw_string(0, 40, "ping timeout", 3, DRAW_PUT);
			}
			
		}
		else if(KEY_Power)
		{
			RF_sendWOR(destAddr);
			xmit[0] = 6;
			xmit[1] = destAddr;
			xmit[2] = cc1100Addr;
			xmit[3] = 1;
			xmit[4] = 'P';
			xmit[5] = 'W';
			xmit[6] = 0x00;
			RF_send(xmit);
		}
		else if(KEY_TV)
		{	
			RF_sendWOR(destAddr);
			xmit[0] = 6;
			xmit[1] = destAddr;
			xmit[2] = cc1100Addr;
			xmit[3] = 1;
			xmit[4] = 'T';
			xmit[5] = 'V';
			xmit[6] = 0x00;
			RF_send(xmit);
		}
	}

}


void test_irrec(void) {
	
	struct RAWcmd_ test_cmd;
	
	test_cmd.tval = 0;
	
	lcd_fill(0);

	set_font(BOLDFONT);
	draw_string(0, 0, "IR receive test", 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);
	set_font(SMALLFONT);
	
	draw_string(0, 20, "OK Key:", 3, DRAW_PUT);
	draw_string(0, 29, "receive IR command", 3, DRAW_PUT);
	
	draw_string(0, 40, "Mute Key:", 3, DRAW_PUT);
	draw_string(0, 49, "send received command", 3, DRAW_PUT);
	
	while(KEY_Exit) {};
	while(!KEY_Exit)
	{
		waitKeyUpDown();
		if(KEY_OK)
		{
			getIR_test(&test_cmd);
		}
		if(KEY_Mute)
		{
			RAW_Send((unsigned long)&test_cmd);
			while(KEY_Mute)
				RAW_Repeat();
			RAW_Stop();
		}
	}
	setEncoder(irDevTab.device[irDevTab.active].encoder, irDevTab.device[irDevTab.active].set);
}

void erase_flash1(void)
{	int x;

	x = eraseFlash(1);
	
}

void test_sid(void) {
	unsigned char temp;
	
	lcd_fill(0);

	set_font(BOLDFONT);
	draw_string(0, 0, "SID", 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);
	set_font(SMALLFONT);
	
	draw_string(0, 20, "A B C D", 3, DRAW_PUT);
	draw_string(0, 29, "set envelope", 3, DRAW_PUT);
	
	draw_string(0, 45, "OK", 3, DRAW_PUT);
	draw_string(0, 54, "play a song", 3, DRAW_PUT);
	
	draw_string(0, 70, "1-7", 3, DRAW_PUT);
	draw_string(0, 79, "play a note", 3, DRAW_PUT);
	
	draw_string(0, 95, "color keys", 3, DRAW_PUT);
	draw_string(0, 104, "set waveform", 3, DRAW_PUT);
	
	sysInfo |= 0x20;
	SID.noise = 0xaa;
	playstate = 0x00;
	playtone[0] = 0;
	playtone[1] = 0;
	playtone[2] = 0;
	temp = 0;
	PWMPR = 0;
	switchSound(SOUND_ON);
	SID.flags |= SIDenable;
	SIDsetwave(0,TRIANGLE);
	SIDsetadsr(0,8,8,10,8);
	
	while(KEY_Exit) {};
	do 	{
		waitKeyUp();
		if (temp) {
			SID.reg[REG_Control] &= ~GATE;
			temp = 0;
		}
		waitKeyDown();
		if(KEY_1) {
			SIDsetfrq(0,4455);
			SID.reg[REG_Control] |= GATE;
			temp =1;
		}
		else if(KEY_2) {
			SIDsetfrq(0,5001);
			SID.reg[REG_Control] |= GATE;
			temp =1;
		}
		else if(KEY_3) {
			SIDsetfrq(0,5613);
			SID.reg[REG_Control] |= GATE;
			temp =1;
		}
		else if(KEY_4) {
			SIDsetfrq(0,5947);
			SID.reg[REG_Control] |= GATE;
			temp =1;
		}
		else if(KEY_5) {
			SIDsetfrq(0,6676);
			SID.reg[REG_Control] |= GATE;
			temp =1;
		}
		else if(KEY_6) {
			SIDsetfrq(0,7493);
			SID.reg[REG_Control] |= GATE;
			temp =1;
		}
		else if(KEY_7) {
			SIDsetfrq(0,8411);
			SID.reg[REG_Control] |= GATE;
			temp =1;
		}
		else if (KEY_A) {
			SIDsetadsr(0,0,10,7,5);
		}
		else if (KEY_B) {
			SIDsetadsr(0,0,10,0,10);
		}
		else if (KEY_C) {
			SIDsetadsr(0,8,8,10,6);
		}
		else if (KEY_D) {
			SIDsetadsr(0,0,0,15,1);
		}
		else if (KEY_Red) {
			SIDsetwave(0,TRIANGLE);
		}
		else if (KEY_Green) {
			SIDsetwave(0,SAWTOOTH);
		}
		else if (KEY_Yellow) {
			SID.reg[REG_PWhi] = 0x08;
			SIDsetwave(0,RECTANGLE);
		}
		else if (KEY_Blue) {
			SIDsetwave(0,NOISE);
		}
		else if (KEY_OK) {
			if (playstate == 0) {
				playstate = 1;
				
				SIDsetadsr(1,0,5,0,5);
				SIDsetwave(1,SAWTOOTH);
				SID.reg[7+REG_PWhi] = 0x06;
				tonelen[1] = 0;
				playtone[1] = (unsigned char*)&song3[0];
				
				SIDsetadsr(2,0,9,0,9);
				SIDsetwave(2,RECTANGLE);
				SID.reg[14+REG_PWhi] = 0x08;
				tonelen[2] = 0;
				playtone[2] = (unsigned char*)&song2[0];
				
				playtone_cb = addTimerCB(SIDplaytone, 4);
				startCB(playtone_cb);
			}
		}
	} while (!KEY_Exit);
	sysInfo &= ~0x20;
	switchSound(SOUND_OFF);
	SID.flags &= ~SIDenable;
	PWMPR = 7;
}

void test_click(FORM* form, CONTROL* control) {
	unsigned char x;
	x=msgbox(45,BC_OKOnly | BC_DefaultButton1,control->caption);
	form_draw(form);
}

void test_numbox(FORM* form, CONTROL* control) {
	unsigned char x;
	x=msgbox(30,BC_OKOnly | BC_DefaultButton1,"numbox click");
	form_draw(form);
}

void test_numchange(FORM* form, CONTROL* control) {
	unsigned short x;
	x=((NUMBOX*)control)->value;
	if(x==50) {
		x=msgbox(30,BC_OKOnly | BC_DefaultButton1,"numbox =50");
		form_draw(form);
	}
}

void formload(FORM* form) {
	unsigned char x;
	x=msgbox(50,BC_OKOnly | BC_DefaultButton1,"Form open");
}

void formclose(FORM* form) {
	unsigned char x;
	x=msgbox(50,BC_OKOnly | BC_DefaultButton1,"Form close");
}

void test_controls(void) {
	
	char txtbuffer[20];
	const LABEL label1		= {BC_Label,0,15,40,10,SMALLFONT,0,"label1",test_click};
	const LABEL label2		= {BC_Labelnotab,60,60,40,9,BOLDFONT,0,"label2",0};
	const LABEL label3		= {BC_Labelnotab,0,110,40,9,BOLDFONT,0,"Numbox:",0};
	const BUTTON button1	= {BC_Button,0,30,40,15,SMALLFONT,0,"Button1",test_click};
	const BUTTON button2	= {BC_Button,0,50,40,16,SMALLFONT,0,"Button2",0};
	const BUTTON button3	= {BC_Button,0,70,40,16,SMALLFONT,0,"Button3",test_click};
	const BUTTON button4	= {BC_Button | BC_Disabled,0,90,40,16,SMALLFONT,0,"Button4",test_click};
	NUMBOX numbox1			= {BC_Numbox,0,120,20,9,SMALLFONT,1,test_numchange,test_numbox,1,500};
	TXTBOX txtbox1			= {BC_Txtbox,0,135,120,10,SMALLFONT,20,0,txtbuffer,test_click};
	
	CONTROL* controls[9] = {	(CONTROL*)&label1,
								(CONTROL*)&label2,
								(CONTROL*)&label3,
								(CONTROL*)&button1,
								(CONTROL*)&button2,
								(CONTROL*)&button3,
								(CONTROL*)&button4,
								(CONTROL*)&numbox1,
								(CONTROL*)&txtbox1};
	
	FORM form  = {"Controls","footer\tTab\tTab",formload,formclose,0,controls,3,9};
	memcpy(txtbuffer,"Textbox",8);
	
	form_exec(&form);
	
}

/*
struct MENU_ENTRY softMenuEntries[16];
struct MENU softMenu;
*/

const struct MENU_ENTRY testMenuEntries[] =
{
	//{ test_circle,	0,			FUNCENTRY,	"circle",	"draw circles"	},
	{ test_controls,	0,			FUNCENTRY,	"controls",	"control elements"	},
	{ test_frame,	0,			FUNCENTRY,	"frame",	"draw frame"	},
	{ test_frame2,	0,			FUNCENTRY,	"frame2",	"draw frame2"	},
	{ draw_fonts,	0,			FUNCENTRY,	"fonts",	"draw fonts"	},
	{ test_logo,	0,			FUNCENTRY,	"logo",		"draw logo"	},
	{ fp1,		0,			FUNCENTRY,	"fp1",		"draw charset"	},
	{ illusion,	0,			FUNCENTRY,	"illusion",	"optical illusion"	},
	{ test_bl,	0,			FUNCENTRY,	"auto-bl",	"test auto backlight"	},
	{ bat_dbg,	0,			FUNCENTRY,	"bat debug",	"show/hide bat values"	},
	{ tkey,		0,			FUNCENTRY,	"keytest",	"test keypad"	},
	{ test_RF,	0,			FUNCENTRY,	"RF test",	"test RF"	},
	{ erase_flash1,	0,			FUNCENTRY,	"Erase Flash1",	"Erase Flash1"	},
	{ test_irrec,	0,			FUNCENTRY,	"receive IR",	"receive IR"	},
	{ test_sid,	0,			FUNCENTRY,	"SID",	"play music"	},
//	{ 0,	 	(void*)&softMenu,	MENUENTRY,	"Soft-Test",	"access soft"	}
};

const struct MENU testMenu =
{
	14, (MENU_ENTRY*)&testMenuEntries, "Test Menu" 
};

void testmenu_init(void)
{
	testdata1 = 0;
	testdata2 = 0;
/*	
	for(i=0;i<16;i++)
	{
		softMenuEntries[i].exec = 0;
		softMenuEntries[i].data = 0;
		softMenuEntries[i].type = 0;
		softMenuEntries[i].name = 0;
		softMenuEntries[i].help = 0;
	}

	softMenuEntries[0].exec = softfunc1;
	softMenuEntries[0].data = 0;
	softMenuEntries[0].type = FUNCENTRY;
	softMenuEntries[0].name = "softfunc test";
	softMenuEntries[0].help = "run softfunc test";


	softMenu.name		= "Soft Menu";
	softMenu.num_entries	= 1;
	softMenu.entry		= softMenuEntries;
*/
}

