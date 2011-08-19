/*
 * (C) Copyright 2009,2010
 * Thomas Haase (Thomas.Haase@web.de)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.         See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef __CONFIG_H
#define __CONFIG_H

/* Tonga2 Board */
#define TONGA2
#define MACH_TYPE MACH_TYPE_TONGA
#define CONFIG_SOC_TMPA900

/* Memory Info */
#define PHYS_SDRAM_1_SIZE                   0x04000000     /* 64MB */

/* Flash usage */
#define USE_NAND

/* Ethernet */
#define USE_SMCS91XX

/* MTD Definitions */
#define MTDIDS_DEFAULT                      "nand0=tmpa9xx-nand"

/* Enviroment vaiables */
#define CONFIG_BOOTCOMMAND                  "nand erase.chip;nand bad;dynpart;nand write 0x40060000 u-boot;nand write 0x40700000 application"


/* Include generic part */
#include "tmpa9xx-generic.h"

#undef  CONFIG_NAND_DYNPART_PARTITIONING

#define CONFIG_NAND_DYNPART_PARTITIONING						\
	unsigned int dynpart_size[] = {							\
	    CONFIG_SYS_ENV_OFFSET,      /* before is (size of) u-boot */		\
	    CONFIG_SYS_ENV_SECT_SIZE,   /* one page for the u-boot environment */	\
	    CONFIG_SPLASH_SIZE,								\
	    0xa00000,									\
	    0 };									\
	char *dynpart_names[] = {							\
	    "u-boot",									\
	    "u-boot_env",								\
	    "splash",									\
	    "application",								\
	     NULL };

#endif
