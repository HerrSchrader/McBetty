/*
    lpc2220.h - register definitions
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

#ifndef LPC2220_H
#define LPC2220_H

/* External Memory Controller (EMC) */
#define BCFG0          (*((volatile unsigned long *) 0xFFE00000))
#define BCFG1          (*((volatile unsigned long *) 0xFFE00004))
#define BCFG2          (*((volatile unsigned long *) 0xFFE00008))
#define BCFG3          (*((volatile unsigned long *) 0xFFE0000C))

/* Vectored Interrupt Controller (VIC) */
#define VICIRQStatus   (*((volatile unsigned long *) 0xFFFFF000))
#define VICFIQStatus   (*((volatile unsigned long *) 0xFFFFF004))
#define VICRawIntr     (*((volatile unsigned long *) 0xFFFFF008))
#define VICIntSelect   (*((volatile unsigned long *) 0xFFFFF00C))
#define VICIntEnable   (*((volatile unsigned long *) 0xFFFFF010))
#define VICIntEnClr    (*((volatile unsigned long *) 0xFFFFF014))
#define VICSoftInt     (*((volatile unsigned long *) 0xFFFFF018))
#define VICSoftIntClr  (*((volatile unsigned long *) 0xFFFFF01C))
#define VICProtection  (*((volatile unsigned long *) 0xFFFFF020))
#define VICVectAddr    (*((volatile unsigned long *) 0xFFFFF030))
#define VICDefVectAddr (*((volatile unsigned long *) 0xFFFFF034))
#define VICVectAddr0   (*((volatile unsigned long *) 0xFFFFF100))
#define VICVectAddr1   (*((volatile unsigned long *) 0xFFFFF104))
#define VICVectAddr2   (*((volatile unsigned long *) 0xFFFFF108))
#define VICVectAddr3   (*((volatile unsigned long *) 0xFFFFF10C))
#define VICVectAddr4   (*((volatile unsigned long *) 0xFFFFF110))
#define VICVectAddr5   (*((volatile unsigned long *) 0xFFFFF114))
#define VICVectAddr6   (*((volatile unsigned long *) 0xFFFFF118))
#define VICVectAddr7   (*((volatile unsigned long *) 0xFFFFF11C))
#define VICVectAddr8   (*((volatile unsigned long *) 0xFFFFF120))
#define VICVectAddr9   (*((volatile unsigned long *) 0xFFFFF124))
#define VICVectAddr10  (*((volatile unsigned long *) 0xFFFFF128))
#define VICVectAddr11  (*((volatile unsigned long *) 0xFFFFF12C))
#define VICVectAddr12  (*((volatile unsigned long *) 0xFFFFF130))
#define VICVectAddr13  (*((volatile unsigned long *) 0xFFFFF134))
#define VICVectAddr14  (*((volatile unsigned long *) 0xFFFFF138))
#define VICVectAddr15  (*((volatile unsigned long *) 0xFFFFF13C))
#define VICVectCntl0   (*((volatile unsigned long *) 0xFFFFF200))
#define VICVectCntl1   (*((volatile unsigned long *) 0xFFFFF204))
#define VICVectCntl2   (*((volatile unsigned long *) 0xFFFFF208))
#define VICVectCntl3   (*((volatile unsigned long *) 0xFFFFF20C))
#define VICVectCntl4   (*((volatile unsigned long *) 0xFFFFF210))
#define VICVectCntl5   (*((volatile unsigned long *) 0xFFFFF214))
#define VICVectCntl6   (*((volatile unsigned long *) 0xFFFFF218))
#define VICVectCntl7   (*((volatile unsigned long *) 0xFFFFF21C))
#define VICVectCntl8   (*((volatile unsigned long *) 0xFFFFF220))
#define VICVectCntl9   (*((volatile unsigned long *) 0xFFFFF224))
#define VICVectCntl10  (*((volatile unsigned long *) 0xFFFFF228))
#define VICVectCntl11  (*((volatile unsigned long *) 0xFFFFF22C))
#define VICVectCntl12  (*((volatile unsigned long *) 0xFFFFF230))
#define VICVectCntl13  (*((volatile unsigned long *) 0xFFFFF234))
#define VICVectCntl14  (*((volatile unsigned long *) 0xFFFFF238))
#define VICVectCntl15  (*((volatile unsigned long *) 0xFFFFF23C))

#define SCS       (*((volatile unsigned char *) 0xE01FC1A0))
#define FIOPIN0         (*((volatile unsigned long *) 0x3FFFC014))
#define FIOSET0         (*((volatile unsigned long *) 0x3FFFC018))
#define FIODIR0         (*((volatile unsigned long *) 0x3FFFC000))
#define FIOCLR0         (*((volatile unsigned long *) 0x3FFFC01C))
#define FIOMASK0        (*((volatile unsigned long *) 0x3FFFC010))

/* Pin Connect Block */
#define PINSEL0        (*((volatile unsigned long *) 0xE002C000))
#define PINSEL1        (*((volatile unsigned long *) 0xE002C004))
#define PINSEL2        (*((volatile unsigned long *) 0xE002C014))

/* General Purpose Input/Output (GPIO) */
#define IOPIN0         (*((volatile unsigned long *) 0xE0028000))
#define IOSET0         (*((volatile unsigned long *) 0xE0028004))
#define IODIR0         (*((volatile unsigned long *) 0xE0028008))
#define IOCLR0         (*((volatile unsigned long *) 0xE002800C))
#define IOPIN1         (*((volatile unsigned long *) 0xE0028010))
#define IOSET1         (*((volatile unsigned long *) 0xE0028014))
#define IODIR1         (*((volatile unsigned long *) 0xE0028018))
#define IOCLR1         (*((volatile unsigned long *) 0xE002801C))
#define IOPIN2         (*((volatile unsigned long *) 0xE0028020))
#define IOSET2         (*((volatile unsigned long *) 0xE0028024))
#define IODIR2         (*((volatile unsigned long *) 0xE0028028))
#define IOCLR2         (*((volatile unsigned long *) 0xE002802C))
#define IOPIN3         (*((volatile unsigned long *) 0xE0028030))
#define IOSET3         (*((volatile unsigned long *) 0xE0028034))
#define IODIR3         (*((volatile unsigned long *) 0xE0028038))
#define IOCLR3         (*((volatile unsigned long *) 0xE002803C))

/* Phase Locked Loop (PLL) */
#define PLLCON         (*((volatile unsigned char *) 0xE01FC080))
#define PLLCFG         (*((volatile unsigned char *) 0xE01FC084))
#define PLLSTAT        (*((volatile unsigned short*) 0xE01FC088))
#define PLLFEED        (*((volatile unsigned char *) 0xE01FC08C))

/* VPB Divider */
#define VPBDIV         (*((volatile unsigned char *) 0xE01FC100))

/* Power Control */
#define PCON           (*((volatile unsigned char *) 0xE01FC0C0))
#define PCONP          (*((volatile unsigned long *) 0xE01FC0C4))

/* External Interrupts */
#define EXTINT         (*((volatile unsigned char *) 0xE01FC140))
#define EXTWAKE        (*((volatile unsigned char *) 0xE01FC144))
#define EXTMODE        (*((volatile unsigned char *) 0xE01FC148))
#define EXTPOLAR       (*((volatile unsigned char *) 0xE01FC14C))

/* Timer 0 */
#define T0IR           (*((volatile unsigned long *) 0xE0004000))
#define T0TCR          (*((volatile unsigned long *) 0xE0004004))
#define T0TC           (*((volatile unsigned long *) 0xE0004008))
#define T0PR           (*((volatile unsigned long *) 0xE000400C))
#define T0PC           (*((volatile unsigned long *) 0xE0004010))
#define T0MCR          (*((volatile unsigned long *) 0xE0004014))
#define T0MR0          (*((volatile unsigned long *) 0xE0004018))
#define T0MR1          (*((volatile unsigned long *) 0xE000401C))
#define T0MR2          (*((volatile unsigned long *) 0xE0004020))
#define T0MR3          (*((volatile unsigned long *) 0xE0004024))
#define T0CCR          (*((volatile unsigned long *) 0xE0004028))
#define T0CR0          (*((volatile unsigned long *) 0xE000402C))
#define T0CR1          (*((volatile unsigned long *) 0xE0004030))
#define T0CR2          (*((volatile unsigned long *) 0xE0004034))
#define T0CR3          (*((volatile unsigned long *) 0xE0004038))
#define T0EMR          (*((volatile unsigned long *) 0xE000403C))

#define T0CTCR		(*((volatile unsigned long *) 0xE0004070))

/* Timer 1 */
#define T1IR           (*((volatile unsigned long *) 0xE0008000))
#define T1TCR          (*((volatile unsigned long *) 0xE0008004))
#define T1TC           (*((volatile unsigned long *) 0xE0008008))
#define T1PR           (*((volatile unsigned long *) 0xE000800C))
#define T1PC           (*((volatile unsigned long *) 0xE0008010))
#define T1MCR          (*((volatile unsigned long *) 0xE0008014))
#define T1MR0          (*((volatile unsigned long *) 0xE0008018))
#define T1MR1          (*((volatile unsigned long *) 0xE000801C))
#define T1MR2          (*((volatile unsigned long *) 0xE0008020))
#define T1MR3          (*((volatile unsigned long *) 0xE0008024))
#define T1CCR          (*((volatile unsigned long *) 0xE0008028))
#define T1CR0          (*((volatile unsigned long *) 0xE000802C))
#define T1CR1          (*((volatile unsigned long *) 0xE0008030))
#define T1CR2          (*((volatile unsigned long *) 0xE0008034))
#define T1CR3          (*((volatile unsigned long *) 0xE0008038))
#define T1EMR          (*((volatile unsigned long *) 0xE000803C))

/* Pulse Width Modulator (PWM) */
#define PWMIR          (*((volatile unsigned long *) 0xE0014000))
#define PWMTCR         (*((volatile unsigned long *) 0xE0014004))
#define PWMTC          (*((volatile unsigned long *) 0xE0014008))
#define PWMPR          (*((volatile unsigned long *) 0xE001400C))
#define PWMPC          (*((volatile unsigned long *) 0xE0014010))
#define PWMMCR         (*((volatile unsigned long *) 0xE0014014))
#define PWMMR0         (*((volatile unsigned long *) 0xE0014018))
#define PWMMR1         (*((volatile unsigned long *) 0xE001401C))
#define PWMMR2         (*((volatile unsigned long *) 0xE0014020))
#define PWMMR3         (*((volatile unsigned long *) 0xE0014024))
#define PWMMR4         (*((volatile unsigned long *) 0xE0014040))
#define PWMMR5         (*((volatile unsigned long *) 0xE0014044))
#define PWMMR6         (*((volatile unsigned long *) 0xE0014048))
#define PWMCCR         (*((volatile unsigned long *) 0xE0014028))
#define PWMCR0         (*((volatile unsigned long *) 0xE001402C))
#define PWMCR1         (*((volatile unsigned long *) 0xE0014030))
#define PWMCR2         (*((volatile unsigned long *) 0xE0014034))
#define PWMCR3         (*((volatile unsigned long *) 0xE0014038))
#define PWMEMR         (*((volatile unsigned long *) 0xE001403C))
#define PWMPCR         (*((volatile unsigned long *) 0xE001404C))
#define PWMLER         (*((volatile unsigned long *) 0xE0014050))

/* Universal Asynchronous Receiver Transmitter 0 (UART0) */
#define U0RBR          (*((volatile unsigned char *) 0xE000C000))
#define U0THR          (*((volatile unsigned char *) 0xE000C000))
#define U0IER          (*((volatile unsigned char *) 0xE000C004))
#define U0IIR          (*((volatile unsigned char *) 0xE000C008))
#define U0FCR          (*((volatile unsigned char *) 0xE000C008))
#define U0LCR          (*((volatile unsigned char *) 0xE000C00C))
#define U0MCR          (*((volatile unsigned char *) 0xE000C010))
#define U0LSR          (*((volatile unsigned char *) 0xE000C014))
#define U0MSR          (*((volatile unsigned char *) 0xE000C018))
#define U0SCR          (*((volatile unsigned char *) 0xE000C01C))
#define U0FDR          (*((volatile unsigned char *) 0xE000C028))
#define U0DLL          (*((volatile unsigned char *) 0xE000C000))
#define U0DLM          (*((volatile unsigned char *) 0xE000C004))

/* Universal Asynchronous Receiver Transmitter 1 (UART1) */
#define U1RBR          (*((volatile unsigned char *) 0xE0010000))
#define U1THR          (*((volatile unsigned char *) 0xE0010000))
#define U1IER          (*((volatile unsigned char *) 0xE0010004))
#define U1IIR          (*((volatile unsigned char *) 0xE0010008))
#define U1FCR          (*((volatile unsigned char *) 0xE0010008))
#define U1LCR          (*((volatile unsigned char *) 0xE001000C))
#define U1MCR          (*((volatile unsigned char *) 0xE0010010))
#define U1LSR          (*((volatile unsigned char *) 0xE0010014))
#define U1MSR          (*((volatile unsigned char *) 0xE0010018))
#define U1SCR          (*((volatile unsigned char *) 0xE001001C))
#define U1DLL          (*((volatile unsigned char *) 0xE0010000))
#define U1DLM          (*((volatile unsigned char *) 0xE0010004))

/* I2C Interface */
#define I2CONSET       (*((volatile unsigned char *) 0xE001C000))
#define I2STAT         (*((volatile unsigned char *) 0xE001C004))
#define I2DAT          (*((volatile unsigned char *) 0xE001C008))
#define I2ADR          (*((volatile unsigned char *) 0xE001C00C))
#define I2SCLH         (*((volatile unsigned short*) 0xE001C010))
#define I2SCLL         (*((volatile unsigned short*) 0xE001C014))
#define I2CONCLR       (*((volatile unsigned char *) 0xE001C018))

/* SPI0 (Serial Peripheral Interface 0) */
#define S0SPCR         (*((volatile unsigned char *) 0xE0020000))
#define S0SPSR         (*((volatile unsigned char *) 0xE0020004))
#define S0SPDR         (*((volatile unsigned char *) 0xE0020008))
#define S0SPCCR        (*((volatile unsigned char *) 0xE002000C))
#define S0SPTCR        (*((volatile unsigned char *) 0xE0020010))
#define S0SPTSR        (*((volatile unsigned char *) 0xE0020014))
#define S0SPTOR        (*((volatile unsigned char *) 0xE0020018))
#define S0SPINT        (*((volatile unsigned char *) 0xE002001C))

/* SPI1 (Serial Peripheral Interface 1) */
#define S1SPCR         (*((volatile unsigned char *) 0xE0030000))
#define S1SPSR         (*((volatile unsigned char *) 0xE0030004))
#define S1SPDR         (*((volatile unsigned char *) 0xE0030008))
#define S1SPCCR        (*((volatile unsigned char *) 0xE003000C))
#define S1SPTCR        (*((volatile unsigned char *) 0xE0030010))
#define S1SPTSR        (*((volatile unsigned char *) 0xE0030014))
#define S1SPTOR        (*((volatile unsigned char *) 0xE0030018))
#define S1SPINT        (*((volatile unsigned char *) 0xE003001C))

/* Real Time Clock */
#define ILR            (*((volatile unsigned char *) 0xE0024000))
#define CTC            (*((volatile unsigned short*) 0xE0024004))
#define CCR            (*((volatile unsigned char *) 0xE0024008))
#define CIIR           (*((volatile unsigned char *) 0xE002400C))
#define AMR            (*((volatile unsigned char *) 0xE0024010))
#define CTIME0         (*((volatile unsigned long *) 0xE0024014))
#define CTIME1         (*((volatile unsigned long *) 0xE0024018))
#define CTIME2         (*((volatile unsigned long *) 0xE002401C))
#define SEC            (*((volatile unsigned char *) 0xE0024020))
#define MIN            (*((volatile unsigned char *) 0xE0024024))
#define HOUR           (*((volatile unsigned char *) 0xE0024028))
#define DOM            (*((volatile unsigned char *) 0xE002402C))
#define DOW            (*((volatile unsigned char *) 0xE0024030))
#define DOY            (*((volatile unsigned short*) 0xE0024034))
#define MONTH          (*((volatile unsigned char *) 0xE0024038))
#define YEAR           (*((volatile unsigned short*) 0xE002403C))
#define ALSEC          (*((volatile unsigned char *) 0xE0024060))
#define ALMIN          (*((volatile unsigned char *) 0xE0024064))
#define ALHOUR         (*((volatile unsigned char *) 0xE0024068))
#define ALDOM          (*((volatile unsigned char *) 0xE002406C))
#define ALDOW          (*((volatile unsigned char *) 0xE0024070))
#define ALDOY          (*((volatile unsigned short*) 0xE0024074))
#define ALMON          (*((volatile unsigned char *) 0xE0024078))
#define ALYEAR         (*((volatile unsigned short*) 0xE002407C))
#define PREINT         (*((volatile unsigned short*) 0xE0024080))
#define PREFRAC        (*((volatile unsigned short*) 0xE0024084))

/* A/D Converter */
#define ADCR           (*((volatile unsigned long *) 0xE0034000))
#define ADDR           (*((volatile unsigned long *) 0xE0034004))
#define ADINTEN        (*((volatile unsigned long *) 0xE003400C))
#define ADSTAT         (*((volatile unsigned long *) 0xE0034030))

/* Watchdog */
#define WDMOD          (*((volatile unsigned char *) 0xE0000000))
#define WDTC           (*((volatile unsigned long *) 0xE0000004))
#define WDFEED         (*((volatile unsigned char *) 0xE0000008))
#define WDTV           (*((volatile unsigned long *) 0xE000000C))

/* Memory Mapping control register */
#define MEMMAP         (*((volatile unsigned long *) 0xE01FC040))

/* SSP control register */
#define SSPCR0         (*((volatile unsigned long *) 0xE005C000))
#define SSPCR1         (*((volatile unsigned char *) 0xE005C004))
#define SSPDR          (*((volatile unsigned char *) 0xE005C008))
#define SSPSR          (*((volatile unsigned char *) 0xE005C00C))
#define SSPCPSR        (*((volatile unsigned char *) 0xE005C010))


#endif /* __ASM_ARCH_HARDWARE_H */
