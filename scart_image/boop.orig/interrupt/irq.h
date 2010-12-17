/*
    irq.h - irq core functions
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

#ifndef IRQ_H
#define IRQ_H

#define 	INT_WDT		0x00000001
#define		INT_CORE0	0x00000004
#define		INT_CORE1	0x00000008
#define		INT_TIMER0	0x00000010
#define		INT_TIMER1	0x00000020
#define		INT_UART0	0x00000040
#define		INT_UART1	0x00000080
#define		INT_PWM		0x00000100
#define		INT_I2C		0x00000200
#define		INT_SPI0	0x00000400
#define		INT_SPI1	0x00000800
#define		INT_PLL		0x00001000
#define		INT_RTC		0x00002000
#define		INT_EINT0	0x00004000
#define		INT_EINT1	0x00008000
#define		INT_EINT2	0x00010000
#define		INT_EINT3	0x00020000
#define		INT_ADC		0x00040000

#define		INT_SRC_WDT	0
#define		INT_SRC_CORE0	2
#define		INT_SRC_CORE1	3
#define		INT_SRC_TIMER0	4
#define		INT_SRC_TIMER1	5
#define		INT_SRC_UART0	6
#define		INT_SRC_UART1	7
#define		INT_SRC_PWM	8
#define		INT_SRC_I2C	9
#define		INT_SRC_SPI0	10
#define		INT_SRC_SPI1	11
#define		INT_SRC_PLL	12
#define		INT_SRC_RTC	13
#define		INT_SRC_EINT0	14
#define		INT_SRC_EINT1	15
#define		INT_SRC_EINT2	16
#define		INT_SRC_EINT3	17
#define		INT_SRC_ADC	18

#define		VIC_SLOT_EN	0x00000020

void __attribute__ ((section(".text.fastcode"))) FIQ_Routine (void);
void SWI_Routine (void)   __attribute__ ((interrupt("SWI")));
void UNDEF_Routine (void) __attribute__ ((interrupt("UNDEF")));


unsigned enableIRQ(void);
unsigned disableIRQ(void);
unsigned restoreIRQ(unsigned oldCPSR);

#endif
