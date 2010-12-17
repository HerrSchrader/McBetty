/*
    backlight.c - lcd backlight control
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
#include "timerfuncs.h"
#include "backlight.h"

extern volatile unsigned char bl_val;

unsigned char bl_max;
unsigned int bl_cb;
unsigned char direction;

unsigned char bl_speed;
unsigned int bl_timeout;

void autoBL(void)
{
	switch(direction)
	{
		case 0:
			if(bl_val < bl_max)
			{
				bl_val++;
			}
			else
			{
				direction = 1;
				setCBIntervall(bl_cb, bl_timeout);
			}
			break;

		case 1:
			direction = 2;
			setCBIntervall(bl_cb, bl_speed);
			break;

		case 2:
			if(bl_val > 0x00)
			{
				bl_val--;
			}
			else
			{
				direction = 0;
				stopCB(bl_cb);
			}
			break;
	}
}

void initBacklight(void)
{
	direction = 1;
	bl_speed = 2;
	bl_timeout = 5000;
	bl_max = 63;
	bl_cb = addTimerCB(autoBL, bl_timeout);
	startCB(bl_cb);
}

void setBacklight(unsigned char level)
{
	if(level == BL_AUTO)
	{
		stopCB(bl_cb);
		direction = 0;
		setCBIntervall(bl_cb, bl_speed);
		startCB(bl_cb);
	}
	else if(level < BL_AUTO)
	{
		stopCB(bl_cb);
		bl_val = level;
		direction = 1;
		setCBIntervall(bl_cb, bl_timeout);
		startCB(bl_cb);
	}
}

void setBLSpeed(unsigned char s)
{
	if(s > 0)
		bl_speed = s;
}

void setBLTimeout(unsigned int t)
{
	if(t > 0)
		bl_timeout = t;
}
