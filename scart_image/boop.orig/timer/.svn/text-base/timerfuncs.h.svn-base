/*
    timerfuncs.c - timer control functions
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

#ifndef TIMERFUNCS_H
#define TIMERFUNCS_H

#include "timerirq.h"

void startTimerIRQ(void);
unsigned int addTimerCB(cb_fn cb, unsigned short intervall);
unsigned int removeTimerCB(unsigned int cb);
void startCB(unsigned int cb);
void stopCB(unsigned int cb);
void setCBIntervall(unsigned int cb, unsigned short intervall);
void wait5ms(void);
#endif
