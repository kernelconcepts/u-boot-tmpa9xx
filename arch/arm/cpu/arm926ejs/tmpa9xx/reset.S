/*
 *  Copyright (c) 2010	Thomas Haase (Thomas.Haase@web.de)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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

.align	5
.globl reset_cpu
reset_cpu:
	ldr	r0, WDOGLOCK	/* Enbale write to WD registers */
	ldr	r1, =0x1acce551
	str	r1, [r0]
        
	ldr	r0, WDOGLOAD	/* Set counter to minimum value */
        mov	r1, $0x01
	str	r1, [r0]

	ldr	r0, WDOGCONTROL /* Enable WD Reset Enable WD Counter */
        mov	r1, $0x03
	str	r1, [r0]

	nop
	nop
	nop
	nop
reset_cpu_loop:
	b	reset_cpu_loop

WDOGLOAD:
	.word	 0xf0010000
WDOGCONTROL:
	.word	 0xf0010008
WDOGLOCK:
	.word	 0xf0010c00
