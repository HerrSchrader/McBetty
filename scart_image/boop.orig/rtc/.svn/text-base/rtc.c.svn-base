/*
    rtc.c - 
    Copyright (C) 2008  <telekatz@gmx.de>  

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
#include "rtc.h"
#include "i2c.h"
#include "lcd.h"
#include "fonty.h"
#include "irq.h"
#include "adc.h"
#include "keyboard.h"
#include "rf.h"
#include "global.h"

volatile struct time_ time;
volatile struct date_ date;
volatile unsigned char timeInfo;

const unsigned char rtc_init[3] = {0x00, 0x08, 0x01};
const unsigned char rtc_timer_init[3] = {0x0e, 0x82, 0x01};
const unsigned long default_time[1] = {0x00000000};
const unsigned char default_date[4] = {0x01, 0x02, 0x01, 0x08};
const unsigned char months[36] = "JANFEBMRZAPRMAIJUNJULAUGSEPOKTNOVDEZ";
const unsigned char days[14] = "SOMODIMIDOFRSA";

extern volatile unsigned int keys[2];

void initRTC(void) {
	
	unsigned char x;
	unsigned char err;
	const unsigned char rtc_init[3] = {0x00, 0x08, 0x01};
	const unsigned char rtc_timer_init[3] = {0x0e, 0x82, 0x01};
	const unsigned long default_time[1] = {0x00000000};
	const unsigned char default_date[4] = {0x01, 0x02, 0x01, 0x08};
	
	do {
		err = 0;
		PINSEL0 |= (1<<4) | (1<<6);
		I2SCLL = 38;	
		I2SCLH = 38;
		I2CONSET = 0x40;
		
		i2c_start(0xA2 | 0);
		err = i2c_write((unsigned char*) rtc_init,3);
		i2c_stop();
		
		i2c_start(0xA2 | 0);
		err |= i2c_write((unsigned char*) rtc_timer_init,3);
		i2c_stop();
		
		i2c_start(0xA2 | 0);
		i2c_write1(offset_time);
		i2c_start(0xA2 | 1);
		x = i2c_read(0) & 0x80;
		i2c_stop();
		if (x) {
			setTime((struct time_*) default_time);
			setDate((struct date_*) default_date);
		}
	} while(err);
}

void getTime(struct time_ *t) {
	
	i2c_start(0xA2 | 0);
	i2c_write1(offset_time);
	i2c_start(0xA2 | 1);
	
	t->second = i2c_read(1) & 0x7F;
	t->minute = i2c_read(1) & 0x7F;
	t->hour   = i2c_read(0) & 0x3F;
	
	i2c_stop();
}

void getDate(struct date_ *d) {
	
	i2c_start(0xA2 | 0);
	i2c_write1(offset_date);
	i2c_start(0xA2 | 1);
	
	d->day     = i2c_read(1) & 0x3F;
	d->weekday = i2c_read(1) & 0x07;
	d->month   = i2c_read(1) & 0x1F;
	d->year    = i2c_read(0);
	
	i2c_stop();
}

void setTime(struct time_ *t) {
	
	i2c_start(0xA2 | 0);
	i2c_write1(offset_time);
	i2c_write((unsigned char *)t,3);
	i2c_stop();
}

void setDate(struct date_ *d) {
	
	i2c_start(0xA2 | 0);
	i2c_write1(offset_date);
	i2c_write((unsigned char *)d,4);
	i2c_stop();
}

void printTime(unsigned char x, unsigned char y, struct time_* t) {

	unsigned char f;
	
	f=activefont;
	set_font(SMALLFONT);
	
	draw_block(x,y,42,7,3,DRAW_ERASE);
	draw_hexC(x+30,y,t->second, 3, DRAW_PUT);
	draw_hexC(x+15,y,t->minute, 3, DRAW_PUT);
	draw_hexC(x+0,y,t->hour, 3, DRAW_PUT);
	draw_char(x+12,y,':', 3, DRAW_PUT);
	draw_char(x+27,y,':', 3, DRAW_PUT);
	set_font(f);
}


void printDate(unsigned char x, unsigned char y, struct date_* d, unsigned char format) {

	unsigned char f;
	char s[16];
	
	f=activefont;
	set_font(SMALLFONT);
	
	switch(format) {
		case 1:
			draw_block(x,y,80,7,3,DRAW_ERASE);
			s[0] = days[d->weekday * 2];
			s[1] = days[(d->weekday * 2) + 1];
			s[2] = ',';
			s[3] = ' ';
			s[4] = ((d->day & 0xf0)>>4) + 0x30;
			s[5] = ((d->day & 0x0f)) + 0x30;
			s[6] = '.';
			s[7] = ' ';	
			s[8] = months[(d->month - 1) * 3];
			s[9] = months[((d->month - 1) * 3)+1];
			s[10] = months[((d->month - 1) * 3)+2];
			s[11] = ' ';
			s[12] = '2';
			s[13] = '0';
			s[14] = ((d->year & 0xf0)>>4) + 0x30;
			s[15] = ((d->year & 0x0f)) + 0x30;
			s[16] = 0;
			draw_string(x, y, s, 3, DRAW_PUT);
			break;
		default:
			draw_block(x,y,57,7,3,DRAW_ERASE);
			
			draw_char(x+0,y,days[d->weekday * 2], 3, DRAW_PUT);
			draw_char(x+6,y,days[(d->weekday * 2) + 1], 3, DRAW_PUT);
			draw_hexC(x+15,y,d->day, 3, DRAW_PUT);
			draw_char(x+27,y,'.', 3, DRAW_PUT);
			draw_hexC(x+30,y,d->month, 3, DRAW_PUT);
			draw_char(x+42,y,'.', 3, DRAW_PUT);
			draw_hexC(x+45,y,d->year, 3, DRAW_PUT);
	}
		
	set_font(f);
}

void rtcIRQ (void) {
	
	EXTINT = (1<<2);
	if (timeInfo & setnewtime) {
		setTime((struct time_ *)&time);
		setDate((struct date_ *)&date);
		timeInfo &= ~setnewtime;
		timeInfo |= time_accurate;
	}
	else {
		getTime((struct time_ *)&time);
		getDate((struct date_ *)&date);
	}
	i2c_start(0xA2 | 0);
	i2c_write1(0x01);
	i2c_write1(0x01);
	i2c_stop();
	
	if (!(PLLSTAT & (1<<8))) {
		if ((time.second & 0x0f) == 9)
			calcBat();
		if ((time.second & 0x0f) == 0)
			showBat();
	}
	timeInfo |= timechanged;
	VICSoftIntClr = INT_EINT2;
}

void startRtcIRQ(void) {
	
	PINSEL0 |= (1<<31);	
	EXTWAKE |= (1<<2);
	EXTINT |= (1<<2);
	
	VICVectAddr3 = (unsigned long)&(rtcIRQ);
	VICVectCntl3 = VIC_SLOT_EN | INT_SRC_EINT2;
	VICIntEnable = INT_EINT2;
}

void RTCtest(void) {

	unsigned char dat[16];
	unsigned char x;
	unsigned char y;
	
	i2c_start(0xA2 | 0);
	i2c_write1(0);
	
	i2c_start(0xA2 | 1);
	for (x=0;x<16;x++) {
		dat[x] = i2c_read(x!=15);
	}
	i2c_stop();
	
	draw_block(0,60,128,40,3,DRAW_ERASE);
	for (y=0;y<4;y++) {
		for (x=0;x<4;x++) {
			draw_hexC(0 + 20 * x,60 + y * 10,dat[x + y * 4], 3, DRAW_PUT);
		}
	}
}

void calcBCD(unsigned char* value, char a, char max, char min) {
	
	signed char x;
	unsigned char y;
	
	x = (((*value & 0xf0)>>4) * 10 + (*value & 0x0f)) + a;
	if (x > max)
		x -= (1 + max - min);
	if (x < min)
		x += (1 + max - min);
	y = x % 10;
	y |= ((x-y) / 10)<<4;
	*value = y;
}

unsigned char checkBCD(unsigned char value, char max, char min) {
	
	signed char x;
	unsigned char y;
	
	x = (((value & 0xf0)>>4) * 10 + (value & 0x0f));
	if (x > max)
		x = max;
	if (x < min)
		x = min;
	y = x % 10;
	y |= ((x-y) / 10)<<4;
	return(y);
}

void send_time(unsigned char dest) {
	unsigned char TXbuf[12];
	struct cc1100frame_ *TXframe;
	TXframe = (struct cc1100frame_ *)&TXbuf[0];
	
	TXframe->packetType = 0x03;
	TXframe->destAddr = dest;
	TXframe->srcAddr = cc1100Addr;
	TXframe->data[0] = 0x02;
	memcpy((unsigned char *)&TXbuf[headerLen+1],(unsigned char *)&time,3);
	memcpy((unsigned char *)&TXbuf[headerLen+4],(unsigned char *)&date,4);
	TXframe->len = 11;					
	RF_send(TXbuf);
	
}

void change_clock(void) {

	struct time_ time1;
	struct date_ date1;
	unsigned char cursor;
	const char cpos[7] = {15, 30, 45, 75, 90, 105};
	signed char x;
	signed char v;
	
	lcd_fill(0);
	
	time1 = time;
	date1 = date;
	cursor = 0;
	
	set_font(BOLDFONT);
	draw_string(0, 0, "Set time/date", 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);
	
	set_font(SMALLFONT);
	draw_string(0, 40, "left/right:", 3, DRAW_PUT);
	draw_string(60, 40, "move cursor", 3, DRAW_PUT);
	draw_string(0, 50, "up/down:", 3, DRAW_PUT);
	draw_string(60, 50, "change value", 3, DRAW_PUT);
	draw_string(0, 60, "0-9:", 3, DRAW_PUT);
	draw_string(60, 60, "enter value", 3, DRAW_PUT);
	draw_string(0, 70, "OK:", 3, DRAW_PUT);
	draw_string(60, 70, "apply changes", 3, DRAW_PUT);
	draw_string(0, 80, "EXIT:", 3, DRAW_PUT);
	draw_string(60, 80, "abort", 3, DRAW_PUT);
	
	waitKeyUp();
	
	while(!(KEY_Exit | KEY_OK)) {
		draw_block(4,99,120,9,3,DRAW_ERASE);
		
		printTime(80,100,(struct time_ *)&time1);
		printDate(5,100,(struct date_ *)&date1,0);
			
		draw_block(4 + cpos[cursor],99,13,9,3,DRAW_XOR);
		
		waitKeyUpDown();
		if(KEY_Left || KEY_Right) {
			cursor += 6;
			if (KEY_Left)
				cursor--;
			else
				cursor++;
			cursor %= 6;
		}
		
		x = getNumKeyValue();
		if (x >= 0) {
			v= x<<4;
			draw_block(4+cpos[cursor],99,13,9,3,DRAW_ERASE);
			draw_char(5+cpos[cursor],100, x + 0x30,3, DRAW_PUT);
			draw_hline(11+cpos[cursor],107,5,3, DRAW_PUT);
			waitKeyUpDown();
			x = getNumKeyValue();
			if (x >= 0) {
				v |= x;
				switch (cursor++) {
					case 0:
						date1.day = checkBCD(v,31,1);
						break;
					case 1:
						date1.month = checkBCD(v,12,1);
						break;
					case 2:
						date1.year = checkBCD(v,99,0);
						break;
					case 3:
						time1.hour = checkBCD(v,23,0);
						break;
					case 4:
						time1.minute = checkBCD(v,59,0);
						break;
					case 5:
						time1.second = checkBCD(v,59,0);
						break;
				}
				cursor %= 6;
			}
		}
		
		if(KEY_Down || KEY_Up) {	
			if (KEY_Up)
				x=1;
			else
				x=-1;
			switch (cursor) {
				case 0:
					calcBCD((unsigned char*)&date1.day, x, 31, 1);
					break;
				case 1:
					calcBCD((unsigned char*)&date1.month, x, 12, 1);
					break;
				case 2:
					calcBCD((unsigned char*)&date1.year, x, 99, 0);
					break;
				case 3:
					calcBCD((unsigned char*)&time1.hour, x, 23, 0);
					break;
				case 4:
					calcBCD((unsigned char*)&time1.minute, x, 59, 0);
					break;
				case 5:
					calcBCD((unsigned char*)&time1.second, x, 59, 0);
					break;
			}
		}
		calcweekday(&date1);
	}
	if (KEY_OK) {
		switch (date1.month) {
			case 0x02:
				x = (((date1.year & 0xf0)>>4) * 10 + (date1.year & 0x0f)) & 0x03; 
				if (x && (date1.day > 0x28))
					date1.day = 0x28;
				else if (!x && (date1.day > 0x29))
					date1.day = 0x29;
				break;
			case 0x04:
			case 0x06:
			case 0x09:
			case 0x11:
				if (date1.day > 0x30)
					date1.day = 0x30;
		}
		calcweekday(&date1);
		VICIntEnClr = INT_EINT2;
		time = time1;
		date = date1;
		RF_sendWOR(0);
		send_time(0);	
		setTime(&time1);
		setDate(&date1);
		timeInfo |= time_accurate;
		VICIntEnable = INT_EINT2;
	}
}

/* Globale zur Verfügung stehenden Variablen:
//Variablen für die Zeit:
u08 dayofweek = 1, day = 1, month = 1;
u16 year = 2004;
u08 isschaltjahr;
*/

unsigned char isschaltjahr(unsigned int year) {

	if ( (((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0) )
		return 1;
	else
		return 0;
}

void calcweekday(struct date_* d) {

	unsigned int tagdesjahres;
	unsigned int jahrdiff;
	unsigned int tempwochentag;
	unsigned char day;
	unsigned char month;
	unsigned char year;
	
	day = (d->day >> 4)*10 + (d->day & 0x0f);
	month = (d->month >> 4)*10 + (d->month & 0x0f);
	year = 2000+(d->year >> 4)*10 + (d->year & 0x0f);
	
	tagdesjahres = 31*(month-1);
	if (month > 2) {
		tagdesjahres = tagdesjahres -3 +isschaltjahr(year);
	}
	if (month > 4) tagdesjahres--;
	if (month > 6) tagdesjahres--;
	if (month > 9) tagdesjahres--;
	if (month > 11) tagdesjahres--;
	tagdesjahres += day;
	
	jahrdiff = year -2000;
	tempwochentag = jahrdiff + (jahrdiff-1) / 4 - (jahrdiff-1) / 100 + (jahrdiff-1) / 400 + tagdesjahres;
	if (jahrdiff > 0)
		tempwochentag++;
	tempwochentag = (tempwochentag % 7)+3;
	if (tempwochentag > 6)
		tempwochentag -= 7;
	d->weekday =  tempwochentag;
}
