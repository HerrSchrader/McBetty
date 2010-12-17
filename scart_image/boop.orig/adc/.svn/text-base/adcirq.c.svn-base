/*
    adcirq.c - adc iinterrupt handler
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

#include "irq.h"
#include "adcirq.h"
#include "adc.h"
#include "lpc2220.h"

volatile unsigned int bat_adc;
volatile unsigned int bmax, bmin;

void __attribute__ ((section(".text.fastcode"))) adcIRQ (void)
{
	ADINTEN = 0;
	bat_adc = ADDR;
	bat_adc >>= 6;
	bat_adc &= 0x03FF;
	if(bmax < bat_adc)
		bmax = bat_adc;
	if(bmin > bat_adc)
		bmin = bat_adc;
	ADCR &=~(1UL<<21);
}
