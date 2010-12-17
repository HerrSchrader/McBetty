/*
    crt.s - startup code
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
.global main

.global _etext
.global _data
.global _edata
.global __bss_start
.global __bss_end__
.global _stack

/* Stack Sizes */
.set  UND_STACK_SIZE,	0x00000004	/* stack for "undefined instruction" interrupts is 4 bytes  	*/
.set  ABT_STACK_SIZE,	0x00000004	/* stack for "abort" interrupts is 4 bytes                  	*/
.set  FIQ_STACK_SIZE,	0x00000104	/* stack for "FIQ" interrupts  is 4 bytes         		*/
.set  IRQ_STACK_SIZE,	0X000003F0	/* stack for "IRQ" normal interrupts is 4 bytes    		*/
.set  SVC_STACK_SIZE,	0x00000004	/* stack for "SVC" supervisor mode is 4 bytes  			*/

/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs */
.set  MODE_USR,		0x10		/* Normal User Mode 						*/
.set  MODE_FIQ,		0x11		/* FIQ Processing Fast Interrupts Mode 				*/
.set  MODE_IRQ,		0x12		/* IRQ Processing Standard Interrupts Mode 			*/
.set  MODE_SVC,		0x13		/* Supervisor Processing Software Interrupts Mode 		*/
.set  MODE_ABT,		0x17		/* Abort Processing memory Faults Mode 				*/
.set  MODE_UND,		0x1B		/* Undefined Processing Undefined Instructions Mode 		*/
.set  MODE_SYS,		0x1F		/* System Running Priviledged Operating System Tasks  Mode	*/
.set  I_BIT,		0x80		/* when I bit is set, IRQ is disabled (program status registers) */
.set  F_BIT,		0x40		/* when F bit is set, FIQ is disabled (program status registers) */


.text
.arm

.global	Reset_Handler
.global _startup

.section .text.fastcode

.global ramvectors
.func   ramvectors
ramvectors:
	ldr	PC, Ram_Reset_Addr
	ldr	PC, Ram_Undef_Addr
	ldr	PC, Ram_SWI_Addr
	ldr	PC, Ram_PAbt_Addr
	ldr	PC, Ram_DAbt_Addr
	nop				/* Reserved Vector (holds Philips ISP checksum) */
	ldr	PC, do_vic_addr		/* Route IRQ to VIC */
	ldr	PC, Ram_FIQ_Addr

Ram_Reset_Addr:	.word   Reset_Handler	/* defined in this module below  */
Ram_Undef_Addr:	.word   UNDEF_Routine	/* defined in main.c  */
Ram_SWI_Addr:	.word   SWI_Routine	/* defined in main.c  */
Ram_PAbt_Addr:	.word   UNDEF_Routine	/* defined in main.c  */
Ram_DAbt_Addr:	.word   UNDEF_Routine	/* defined in main.c  */
do_vic_addr:	.word   do_vic	
Ram_FIQ_Addr:	.word   do_fiq	/* defined in main.c  */
		.word   0		/* rounds vectors to 64 bytes total  */
.endfunc

.global do_vic
.func do_vic
do_vic:

	stmfd	sp!, { r0-r5, r12, lr }	/* save work regs & spsr on stack */
	ldr	r4, =VICVectAddr	/* get the ISR address from VIC */
	ldr	r5, [r4]
	mov	lr, pc			/* set return to common exit of ISR */
	bx	r5			/* go handle the interrupt */
/*	mov	pc, r5			/* go handle the interrupt */
	str	lr, [r4]		/* update VICVectAddr */
	ldmfd	sp!, { r0-r5, r12, lr }	/* restore work regs and spsr_irq */
	subs	pc, lr, #0x4		/* return, restoring CPSR from SPSR */
/*
stmfd sp!, { lr }
mrs lr, spsr
stmfd sp!, { r4-r5, lr }

ldr r4, =VICVectAddr
ldr r5, [r4]

msr cpsr_c, #0x13

stmfd sp!, { r0-r3, r12, lr }
mov lr, pc
bx r5
ldmfd sp!, { r0-r3, r12, lr }

msr cpsr_c, #0x92

str lr, [r4]

ldmfd sp!, { r4-r5, lr }
msr spsr_cxsf, lr
ldmfd sp!, { lr }

subs pc, lr, #0x4
*/

.endfunc

.global do_fiq
.func do_fiq
do_fiq:

	stmfd	sp!, { r0-r7, lr }
	ldr	r8, =FIQ_Routine
	mov	lr, pc
	bx	r8
	ldmfd	sp!, { r0-r7, lr }
	subs	pc, lr, #0x4
.endfunc	


.section .text

.func   _startup
_startup:

# Exception Vectors

_vectors:
		ldr	PC, Reset_Addr

Reset_Addr:     .word   Reset_Handler

# Reset Handler

Reset_Handler:  

/* Setup a stack for each mode - note that this only sets up a usable stack
   for User mode.   Also each mode is setup with interrupts initially disabled. */
	
	ldr	r0, =_stack_end
	msr	CPSR_c, #MODE_UND|I_BIT|F_BIT 	/* Undefined Instruction Mode  */
	mov	sp, r0

	sub	r0, r0, #UND_STACK_SIZE
	msr	CPSR_c, #MODE_ABT|I_BIT|F_BIT 	/* Abort Mode */
	mov	sp, r0

	sub	r0, r0, #ABT_STACK_SIZE
	msr	CPSR_c, #MODE_FIQ|I_BIT|F_BIT 	/* FIQ Mode */
	mov	sp, r0	

	sub	r0, r0, #FIQ_STACK_SIZE
	msr	CPSR_c, #MODE_IRQ|I_BIT|F_BIT 	/* IRQ Mode */
	mov	sp, r0

	sub	r0, r0, #IRQ_STACK_SIZE
	msr	CPSR_c, #MODE_SVC|I_BIT|F_BIT 	/* Supervisor Mode */
	mov	sp, r0

	sub	r0, r0, #SVC_STACK_SIZE
	msr	CPSR_c, #MODE_SYS|I_BIT|F_BIT 	/* User Mode */
	mov	sp, r0

/* Setup Pins and Memory */
	ldr	r0,=BCFG0
				/*            W B                R              */
				/*            P S                B              */
				/*        B W E E                L              */
				/*  AT|MW|M|P|R|R|reserved|WST2 |E|WST1 |r|IDCY */
				/*  --+--+-+-+-+-+--------+-----+-------+-+---- */
				/*  3 | 2| | | |2|   2   1|   1 | | 0   |0| 0   */
				/*  1 | 8| | | |4|   0   6|   2 | | 8   |4| 0   */
				/*  --+--+-+-+-+-+--------+-----+-------+-+---- */
/*	ldr	r1, =0x10000400 /*  00|01|0|0|0|0|00000000|00000|1|00000|0|0000 16bit, rble, 3wst - 10 mhz*/
/*	ldr	r1, =0x10000420 /*  00|01|0|0|0|0|00000000|00000|1|00001|0|0000 16bit, rble, 4wst - 30 mhz*/
	ldr	r1, =0x100004A0 /*  00|01|0|0|0|0|00000000|00000|1|00101|0|0000 16bit, rble, 6wst - 60 mhz*/

	str	r1,[r0]		/* set bcfg0 (flash) */
	str	r1,[r0,#0x08]	/* set bcfg2 (flash) */

	ldr	r0, =BCFG1
/*	ldr	r1, =0x00000422 /*  00|00|0|0|0|0|00000000|00000|1|00001|0|0010  8 bit, 3 sram wst, rble, 5 wst 3 idcy*/
	ldr	r1, =0x00000C42 /*  00|00|0|0|0|0|00000000|00001|1|00010|0|0010  8 bit, 3 sram wst, rble, 5 wst 3 idcy*/
	str	r1, [r0]	/* set bcfg1 (lcd) */

	ldr	r0, =PINSEL0
	ldr	r1, =0x00008005
	str	r1, [r0]

	ldr	r0, =PINSEL1
	ldr	r1, =0x00000000  /* gpio 0.30 */
	str	r1, [r0]

	ldr	r0, =PINSEL2
	ldr	r1, =0x0de049d4
	str	r1, [r0]

	ldr	r0, = IO2SET
	ldr	r1, =0x1FC0000
	str	r1, [r0]
	str	r1, [r0,#0x04]

	ldr	r0, = IO0DIR
	ldr	r1, =0x002018D0
	str	r1, [r0]

/* Setup PLL */
	ldr	r0, =0xe01fc000
	ldr	r2, =0xaa
	ldr	r3, =0x55
	ldr	r1, =0x03
	str	r1, [r0,#0x80]		/* set pllcon to 0x03 = pll enable (0x01) + pll connect (0x02) */
	ldr	r1, =0x0
	str	r1, [r0,#0x100]		/* set vbpdiv to 0x00 = 1/4th; 0x01 = 1/1, 0x02 = 1/2*/
/*	ldr	r1, =0x42 */
	ldr	r1, =0x45
	str	r1,[r0,#0x84]		/* set pllcfg to 0x42 = psel = 10 (4) = msel= 00010 (3) = 240 mhz Fcco*/
	str	r2, [r0,#0x8c]
	str	r3, [r0,#0x8c]

/* Copy .fastcode & .data section (Copy from ROM to RAM) */
	ldr	R0, =__fastcode_load	/*_etext*/
	ldr	r3, entry_mask		/* this and the next instruction are an workaround  */
	and	r0, r0, r3		/* for some ugly bug in winarm to force msb to 0x80 */
	ldr	R1, =__fastcode_start	/*_data*/
	ldr	R2, =_edata
1:
	cmp	r1,r2
	ldmltia	r0!,{r3}
	stmltia	r1!,{r3}
	blt	1b

/* Clear .bss section (Zero init)  */
	mov	R0, #0
	ldr	R1, =_bss_start
	ldr	R2, =_bss_end
2:
	cmp	R1, R2
	strlo	R0, [R1], #4
	blo	2b

	ldr	r0, =MEMMAP
	ldr	r1, =0x02		/* irq vectors in ram */
	str	r1, [r0]

	mov	r0,#0
	mov	r1,r0
	mov	r2,r0
	mov	fp,r0
	mov	r7,r0
	ldr	r10,=main
	mov	lr,pc
	bx	r10

.endfunc

entry_mask:     .word   0x8FFFFFFF	/* defined in this module below  */

.end
