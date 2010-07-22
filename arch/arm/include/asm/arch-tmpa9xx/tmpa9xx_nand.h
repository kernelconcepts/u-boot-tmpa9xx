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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _TMPA9X0_NAND_H
#define _TMPA9X0_NAND_H

#define	NDFMCR0_ECCRST	(0x1 << 0)
#define	NDFMCR0_BUSY	(0x1 << 1)
#define	NDFMCR0_ECCE	(0x1 << 2)
#define	NDFMCR0_CE1	(0x1 << 3)
#define	NDFMCR0_CE0	(0x1 << 4)
#define	NDFMCR0_CLE	(0x1 << 5)
#define	NDFMCR0_ALE	(0x1 << 6)
#define	NDFMCR0_WE	(0x1 << 7)
#define	NDFMCR0_RSEDN	(0x1 << 10)

#define	NDFMCR1_ECCS	(0x1 << 1)
#define	NDFMCR1_SELAL	(0x1 << 9)
#define	NDFMCR1_ALS	(0x1 << 8)

#define	NAND_DMAC_STATUS	(0x1 << 5)
#define	NAND_DMAC_CLEAR		(0x1 << 5)

#endif

