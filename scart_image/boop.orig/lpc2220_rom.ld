/*
    lpc2220.ld - linker script, flash based
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

/* identify the Entry Point  */

ENTRY(_startup)

/* specify the LPC2106 memory areas  */

MEMORY 
{
	ram  	: ORIGIN = 0x40000000, LENGTH = 64K-0x400	/* free RAM area	*/
	flash	: ORIGIN = 0x80000000, LENGTH = 1M	/* FLASH ROM		*/	
}

/* define a global symbol _stack_end  */

/* _stack_end = 0x4000F9DC; */
_stack_end = 0x40010000;

/* now define the output sections  */

SECTIONS 
{
	. = 0;			/* set location counter to address zero  */
	
	startup :
	{
		*(.startup)
	} >flash		/* the startup code goes into FLASH */
	. = ALIGN(4);		/* advance location counter to the next 32-bit boundary */

	.text :			/* collect all sections that should go into FLASH after startup  */ 
	{
		*(.text)	/* all .text sections (code)  */
		*(.rodata)	/* all .rodata sections (constants, strings, etc.)  */
		*(.rodata*)	/* all .rodata* sections (constants, strings, etc.)  */
		*(.glue_7)	/* all .glue_7 sections  (no idea what these are) */
		*(.glue_7t)	/* all .glue_7t sections (no idea what these are) */
		_etext = .;	/* define a global symbol _etext just after the last code byte */
	} >flash		/* put all the above into FLASH */
	. = ALIGN(4);		/* advance location counter to the next 32-bit boundary */

	.fastcode :
	{
		__fastcode_load = LOADADDR(.fastcode);
		__fastcode_start = .;
		/* functions with __attribute__ ((section (".text.fastcode")))*/
		*(.text.fastcode)
		*(.text.ramvectors)
		*(.text.do_vic)
		*(.text.do_fiq)
		*(.text.timerIRQ)
		*(.text.soundIRQ)
		*(.text.FIQ_Routine)
		*(.text.keyScanner)
		*(.text.adcIRQ)
		*(.text.irIRQ)
		*(.text.defIR)
		*(.text.NRC17_Encode)
		*(.text.ITT_Encode)
		*(.text.RC5_Encode)
		*(.text.RCA_Encode)
		*(.text.RCMM_Encode)
		*(.text.REC80_Encode)
		*(.text.RECS80_Encode)
		*(.text.SIRC_Encode)
		*(.text.SPACEENC_Encode)
		*(.text.cc1100IRQ)
		*(.text._evaluate)
		*(.text._push_state)
		*(.text._mcpy)
		*(.text._do_rcu)
		*(.text._do_rcuS)
		*(.text._draw_block)
		*(.text._draw_hline)
		*(.text._draw_vline)

		. = ALIGN (4);
		__fastcode_end = .;
	} >ram AT>flash
	

	.data :			/* collect all initialized .data sections that go into RAM  */ 
	{
		_data = .;	/* create a global symbol marking the start of the .data section  */
		*(.data)	/* all .data sections  */
		_edata = .;	/* define a global symbol marking the end of the .data section  */
	} >ram AT >flash	/* put all the above into RAM (but load the LMA copy into FLASH) */
	. = ALIGN(4);		/* advance location counter to the next 32-bit boundary */

	.bss :			/* collect all uninitialized .bss sections that go into RAM  */
	{
		_bss_start = .;	/* define a global symbol marking the start of the .bss section */
		*(.bss)		/* all .bss sections  */
	} >ram			/* put all the above in RAM (it will be cleared in the startup code */
	
	. = ALIGN(4);		/* advance location counter to the next 32-bit boundary */
	_bss_end = . ;		/* define a global symbol marking the end of the .bss section */
}
_end = .;			/* define a global symbol marking the end of application RAM */

PROVIDE( MEMMAP		= 0xE01FC040 );
PROVIDE( BCFG0 		= 0xFFE00000 );
PROVIDE( BCFG1		= 0xFFE00004 );
PROVIDE( BCFG2		= 0xFFE00008 );
PROVIDE( PINSEL0	= 0xE002C000 );
PROVIDE( PINSEL1	= 0xE002C004 );
PROVIDE( PINSEL2	= 0xE002C014 );
PROVIDE( IO0PIN		= 0xE0028000 );
PROVIDE( IO0DIR		= 0xE0028008 );
PROVIDE( IO2SET		= 0xE0028024 );
PROVIDE( SCS		= 0xE01FC1A0 );
PROVIDE( FIODIR0	= 0x3FFFC000 );
PROVIDE( VICVectAddr	= 0xFFFFF030 );
