/*
    rtc.h - 
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

#ifndef RTC_H
#define RTC_H

struct time_ {
	unsigned char second;	
	unsigned char minute;
	unsigned char hour;	
};
#define offset_time	0x02

struct date_ {
	unsigned char day;	
	unsigned char weekday;
	unsigned char month;
	unsigned char year;
};
#define offset_date	0x05

volatile struct time_ time;
volatile struct date_ date;
volatile unsigned char timeInfo;
#define timechanged  	0x01
#define setnewtime		0x02
#define time_accurate	0x04

void initRTC(void);
void getTime(struct time_ *t);
void getDate(struct date_ *d);
void setTime(struct time_ *t);
void setDate(struct date_ *d);
void printTime(unsigned char x, unsigned char y, struct time_* t);
void printDate(unsigned char x, unsigned char y, struct date_* d, unsigned char format);
void rtcIRQ (void);
void startRtcIRQ(void);
void change_clock(void);
void calcweekday(struct date_* d);
void send_time(unsigned char dest);

#endif
