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

#define NAND_SIZE    0x10000000 /* 256 MB NAND Flash */
#define KERNEL_SIZE  0x00300000	/* keep some spare for further kernel development */
#define SPLASH_SIZE  0x00300000	/* enought for 1024x768x32 */

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
        gd->ram_size =  PHYS_SDRAM_1_SIZE;
}

int dram_init (void)
{
	dram_init_banksize();
	return 0;
}

#ifdef CONFIG_NAND_DYNPART
unsigned int dynpart_size[] = {
    CONFIG_SYS_ENV_OFFSET,      /* before is (size of) u-boot */

    CONFIG_SYS_ENV_SECT_SIZE,   /* one page for the u-boot environment */

    SPLASH_SIZE,

    KERNEL_SIZE,
    
    NAND_SIZE-
    CONFIG_SYS_ENV_OFFSET-
    CONFIG_SYS_ENV_SECT_SIZE-
    SPLASH_SIZE-
    KERNEL_SIZE,                /* rest of nand */
    
    0 };
    
char *dynpart_names[] = {
    "u-boot",
    "u-boot_env",
    "splash",
    "kernel",
    "rootfs",
     NULL };
#endif

/*
 * The following are used to control the SPI chip selects for the SPI command.
 */
#if defined(CONFIG_SOFT_SPI)

#include <spi.h>

int bus;

int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return ((bus==0)||(bus==1));
}

void spi_cs_activate(struct spi_slave *slave)
{

	bus=slave->bus;

	switch (bus)
        {
	case 0:
    		GPIOTDATA= 0x00;
	        GPIOTDIR = 0x0f;
        	GPIOTFR1 = 0x00;
	        GPIOTFR2 = 0x00;
		break;
	case 1:
		GPIOLDATA= 0x00;
		GPIOLDIR = 0x0f;
        	GPIOLFR1 = 0x00;
        	GPIOLFR2 = 0x00;
                break;
	default:
            	printf("Invalid bus selected for activate\n");
                break;
	}
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	switch (bus)
        {
	case 0:
    		GPIOTDATA= 0x00;
	        GPIOTDIR = 0x00;
        	GPIOTFR1 = 0x00;
	        GPIOTFR2 = 0x00;
		break;
	case 1:
		GPIOLDATA= 0x00;
		GPIOLDIR = 0x00;
        	GPIOLFR1 = 0x00;
        	GPIOLFR2 = 0x00;
                break;
	default:
            	printf("Invalid bus selected for deactivate\n");
                break;
	}
}

void tmpa_spi_bb_sda(int bit)
{
	switch (bus)
        {
	case 0:
		if(bit)
			GPIOTDATA |=  (1<<2);
		else
			GPIOTDATA &= ~(1<<2);
                break;
    	case 1:
		if(bit)
			GPIOLDATA |=  (1<<2);
		else
			GPIOLDATA &= ~(1<<2);
                break;
        default:
        	printf("Invalid bus selected for sda\n");
                break;
	}
}        

void tmpa_spi_bb_scl(int bit)
{
	switch (bus)
        {
	case 0:
		if(bit)
			GPIOTDATA |=  (1<<1);
		else
			GPIOTDATA &= ~(1<<1);
                break;
    	case 1:
        
		if(bit)
			GPIOLDATA |=  (1<<1);
		else
			GPIOLDATA &= ~(1<<1);
                break;
        default:
        	printf("Invalid bus selected for scl\n");
                break;
	}
}

unsigned char tmpa_spi_bb_read(void)
{
	switch (bus)
        {
        case 0:
		return ((GPIOTDATA & (1<<3))!=0);
        	break;
        case 1:
		return ((GPIOLDATA & (1<<3))!=0);
                break;
        default:
        	printf("Invalid bus selected for read\n");
                return -1;
                break;
	}
}
#endif
