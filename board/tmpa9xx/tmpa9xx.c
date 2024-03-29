/*
 * (C) Copyright 2009,2010
 * Kernel Concepts <www.kernelconcepts.de>
 * Florian Boor (florian.boor@kernelconcepts.de)
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

#include <common.h>
#include <netdev.h>
#include "asm/arch-tmpa9xx/tmpa9xx.h"

DECLARE_GLOBAL_DATA_PTR;

/*
 * Miscelaneous platform dependent initialisations
 */
static void init_tmpa9xx(void)
{
#if defined CONFIG_DM9000_BASE
	SMC_SET_CYCLES_3 = 0x0004afaa;
	SMC_SET_OPMODE_3 = 0x00000002;
	SMC_DIRECT_CMD_3 = 0x00c00000;
#endif
#if defined CONFIG_SMC911X_BASE
/* Tonga2 SD Ram Version */        
	SMC_SET_CYCLES_3 = 0x0004afaa;
	SMC_SET_OPMODE_3 = 0x00000001;
	SMC_DIRECT_CMD_3 = 0x00c00000;
/* Tonga2 DDR Ram Version */        
	SMC_SET_CYCLES_5 = 0x0004afaa;
	SMC_SET_OPMODE_5 = 0x00000001;
	SMC_DIRECT_CMD_5 = 0x00c00000;
#endif

	// port J,K are LCDC
	GPIOJFR1 = 0xff;
	GPIOJFR2 = 0x00;
	GPIOKFR1 = 0xff;
	GPIOKFR2 = 0x00;

/*
	# Active INTH 
	# Configure it as active low, level sensitive
	# INTH is PR2 -> bit2 in the registers
	# DIR, FR1 and FR2 are default 0 -> ok for INTH
	# GPIORIS (Port Interrupt Selection Register (Level and Edge))
	# [2] PR2IS R/W 0y0 Port R interrupt sensitivity register
	#                   0: Edge-sensitive
	#                   1: Level-sensitive
	# Default is edge sensitive but
	# we want level sensitive
*/
	GPIORIS |= (1<<2);
/*
	# GPIORIE (Port Interrupt Enable Register) Address = (0xF080_E000) +(0x0810)
	#   [2]         PR2IE           R/W       0y0       Port R interrupt enable register
	#                                                   0: Disable
	#                                                   1: Enable
*/
	GPIORIE |= (1<<2);
	
#ifndef TOPAS_910
	// ports U,V are NANDC
	GPIOUFR1 = 0xff;
	GPIOUFR2 = 0x00;
	GPIOVFR1 = 0x7f;
	GPIOVFR2 = 0x00;
#endif
}


int board_eth_init(bd_t *bis) {
#if defined CONFIG_DM9000_BASE
	return dm9000_initialize(bis);
#elif defined CONFIG_SMC911X_BASE
 	return smc911x_initialize(0, CONFIG_SMC911X_BASE);
#else
        return 0;
#endif
}

int board_init (void)
{
	init_tmpa9xx();
	/* memory and cpu-speed are setup before relocation */
	/* so we do _nothing_ here */

	gd->bd->bi_arch_number = MACH_TYPE;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = LINUX_BOOT_PARAM_ADDR;
	
        gd->flags = GD_FLG_RELOC | GD_FLG_SILENT;
	return 0;
}

int board_late_init(void)
{
	setenv("stdout", "serial");
	setenv("stderr", "serial");
	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size  = PHYS_SDRAM_1_SIZE;  
	// gd->ram_size =  PHYS_SDRAM_1_SIZE; // already done in dram_init()
}

int dram_init (void)
{
        //dram_init_banksize(); // disable because gd->bd not yet initialized
	gd->ram_size =  PHYS_SDRAM_1_SIZE; // added because board.c need it
        return 0;
}

#ifdef CONFIG_NAND_DYNPART

/* Little bit tricky - partitioning is defined inside board file */
CONFIG_NAND_DYNPART_PARTITIONING

#endif
