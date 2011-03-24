/*
 * (C) Copyright 2009,2010
 * Kernel Concepts <www.kernelconcepts.de>
 * Florian Boor (florian.boor@kernelconcepts.de)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <asm/arch/tmpa9xx.h>

.globl	lowlevel_init
lowlevel_init:
	/* All interrupt as IRQ */
	mov	r1, #0
	ldr	r0, =VICINTSELECT
	str	r1, [r0]

	/* Disable all interrupt */
	mvn	r1, #0
	ldr	r0, = VICINTENCLEAR
	str	r1, [r0]	
#if 1
	ldr	r1, =SYSCR2
	ldr	r0, [r1]
	ands	r0, r0, #01
	beq     setup
	/* autoboot already initialized PLL and DDR for us.
	 * Needn't change it here.
	 */
#endif

	mov	pc, lr

setup:
	/* system PLL setting */
	ldr	r0, SYSCR0SET
	ldr	r1, =SYSCR0
	str	r0, [r1]

	ldr	r0, SYSCR1SET
	ldr	r1, =SYSCR1
	str	r0, [r1]

	mov	r0, #0
	ldr	r1, =SYSCR2
	str	r0, [r1]

	mov	r0, #7
	ldr	r1, =SYSCR3
	str	r0, [r1]

	ldr	r0, SYSCR4SET
	ldr	r1, =SYSCR4
	str	r0, [r1]

	/* PLL setting OK */
	ldr	r0, SYSCR3SET
	ldr	r1, =SYSCR3
	str	r0, [r1]

pll_lockup:
	/* Wait for PLL lockup counter end */
	ldr	r1, =SYSCR2
	ldr	r0, [r1]
	ands	r0, r0, #01
	beq	pll_lockup

	/* set Fpll */	
	ldr	r0, SYSCR2SET
	ldr	r1, =SYSCR2
	str	r0, [r1]
/* wait for PLL to settle */
	nop
	nop
	nop
	nop
	nop

	ldr	r0, CLKCR5SET
	ldr	r1, =CLKCR5
	str	r0, [r1]

	/*------------------------------------------------------*
	 * configuration for DDR, NOR Flash, Ethernet
	 *------------------------------------------------------*/
/* 	set PMCDRV register
	mww 0xf0020260 0x00000071 
*/
	LDR	r1,PMC_ADDR
	MOV	r0,#71
	STR	r0,[r1,#0x260]

/* START SDRAM INIT */

	LDR	r1,DMC_ADDR

// mww 0xf4300014 0x00000006
	MOV	r0,#0x06
	STR	r0,[r1,#0x14]
// mww 0xf4300018 0x00000000
	MOV	r0,#0x00
	STR	r0,[r1,#0x18]
// mww 0xf430001C 0x00000002
	MOV	r0,#2
	STR	r0,[r1,#0x1c]
// mww 0xf4300020 0x00000007
	MOV	r0,#7
	STR	r0,[r1,#0x20]
// mww 0xf4300024 0x0000000A
	MOV	r0,#0xa
	STR	r0,[r1,#0x24]
// mww 0xf4300028 0x00000013
	MOV	r0,#0x13
	STR	r0,[r1,#0x28]
// mww 0xf430002C 0x0000010A
	LDR	r0,DMC_DATA0
	STR	r0,[r1,#0x2c]
// mww 0xf4300030 0x00000013
	MOV	r0,#0x13
	STR	r0,[r1,#0x30]
// mww 0xf4300034 0x00000002
	MOV	r0,#2
	STR	r0,[r1,#0x34]
// mww 0xf4300038 0x00000002
	MOV	r0,#2
	STR	r0,[r1,#0x38]
// mww 0xf430003C 0x00000001
	MOV	r0,#1
	STR	r0,[r1,#0x3c]
// mww 0xf4300040 0x0000000A
	MOV	r0,#0x0a
	STR	r0,[r1,#0x40]
// mww 0xf4300044 0x0000000B
	MOV	r0,#0xb
	STR	r0,[r1,#0x44]
// mww 0xf4300048 0x00000014
	MOV	r0,#0x14
	STR	r0,[r1,#0x48]
// mww 0xf430000C 0x00008011
	LDR	r0,DMC_DATA1
	STR	r0,[r1,#0xc]
// mww 0xf4300304 0x00000011
	MOV	r0,#0x11
	STR	r0,[r1, #0x304]
// mww 0xf4300010 0x00000A60
	LDR	r0,DMC_DATA5
	STR	r0,[r1,#0x10]

//	LDR	r0,DMC_DATA3
//	STR	r0,[r1,#0x204]
//	STR	r0,[r1,#0x208]
//	STR	r0,[r1,#0x20c]

// mww 0xf4300200 0x000140FC
	LDR	r0,DMC_DATA2
	STR	r0,[r1,#0x200]
// mww 0xf4300008 0x000C0000
	MOV	r0,#0xc0000
	STR	r0,[r1,#8]
// mww 0xf4300008 0x00000000
	MOV	r0,#0
	STR	r0,[r1,#8]
// mww 0xf4300008 0x00040000
// mww 0xf4300008 0x00040000
	MOV	r0,#0x40000
	STR	r0,[r1,#8]
	STR	r0,[r1,#8]
	STR	r0,[r1,#8]
	STR	r0,[r1,#8]
	STR	r0,[r1,#8]
// mww 0xf4300008 0x00080031
	LDR	r0,DMC_DATA4
	STR	r0,[r1,#8]

// set up arbiter
	MOV	r0,#0xfd
	STR	r0,[r1,#0x100]	//	dmc_id_0_cfg_3 = 0x05;

	MOV	r0,#0xfd
	STR	r0,[r1,#0x104]	//	dmc_id_1_cfg_3 = 0x05;

	MOV	r0,#0x3
	STR	r0,[r1,#0x108]	//	dmc_id_2_cfg_3 = 0x0b;

	MOV	r0,#0xfd
	STR	r0,[r1,#0x10c]	//	dmc_id_3_cfg_3 = 0x05;


// mww 0xf4300004 0x00000000
	MOV	r0,#0
	STR	r0,[r1,#4]
/* END of SDRAM INIT */

/* Set SMC_TIMEOUT 1 */
	LDR	r1,SMC_TIMEOUT_ADDR
	MOV	r0,#0x01
	STR	r0,[r1,#0x00]

	mov	pc, lr


.ltorg

PMC_ADDR:
		.word	0xf0020000

SMC_TIMEOUT_ADDR:
		.word	0xf00a0050
DMC_ADDR:
		.word	0xf4300000
DMC_DATA0:
		.word	0x10a
DMC_DATA2:
		.word	0x140fc
DMC_DATA3:
		.word	0xff00
DMC_DATA5:
		.word	0x0a60

#if   defined BURST1
DMC_DATA1:
		.word	0x00011
DMC_DATA4:
		.word	0x80030
#elif defined BURST2
DMC_DATA1:
		.word	0x08011
DMC_DATA4:
		.word	0x80031
#elif defined BURST4
DMC_DATA1:
		.word	0x10011
DMC_DATA4:
		.word	0x80032
#elif defined BURST8
DMC_DATA1:
		.word	0x18011
DMC_DATA4:
		.word	0x80033
#else
#error "Unknown burst setting"
#endif

/* BIT1: DRVOSC_H, BIT3: EXTIN_H, BIT5: XEN, BIT6,7: USBCLKSEL */
SYSCR0SET:	.word		(0x62)

/* BIT0, BIT1 and BIT2 clock gear select */
SYSCR1SET:	.word		(0x00)

/* BIT0: LUPFLAG, BIT1: FCSEL, BIT7: LUPTEST */
SYSCR2SET:	.word		(0x02)
SYSCR3SET:	.word		(0x87)
SYSCR4SET:	.word		(0x65)
CLKCR5SET:	.word		(0x40)
BOARD_LED_REG:	.word		(0xF08013FC)



