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

#ifndef __CONFIG_H
#define __CONFIG_H

/* Topasa 910 Evaluation Board */
#define TOPAS_910
#define MACH_TYPE MACH_TYPE_TOPAS910

#undef  CONFIG_SYS_NO_FLASH		
#define CONFIG_SYS_CONSOLE_INFO_QUIET

/* SoC Configuration */
#define CONFIG_ARM926EJS				/* arm926ejs CPU */
#define CONFIG_SYS_HZ			32000
#define CONFIG_SOC_TMPA910

/* Memory Info */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1			0x40000000
#define PHYS_SDRAM_1_SIZE		0x04000000	/* 64MB */

/* Serial Driver info: UART0 for console  */
#define	CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE      	{ 115200 }

/* Ethernet:  external DM9000 */
#define CONFIG_DRIVER_DM9000        	1
#define CONFIG_DM9000_BASE          	0x60000000
#define CONFIG_DM9000_USE_16BIT     	1
#define DM9000_IO                   	CONFIG_DM9000_BASE
#define DM9000_DATA		    	(CONFIG_DM9000_BASE+0x1000)
#define CONFIG_NET_MULTI

#define CONFIG_IPADDR		    	192.168.1.2
#define CONFIG_SERVERIP		    	192.168.1.1

/* NAND */
#define CONFIG_SYS_NAND_USE_FLASH_BBT
#define CONFIG_SYS_NAND_PAGE_2K
#define CONFIG_SYS_NAND_LARGEPAGE
#define CONFIG_SYS_NAND_BASE_LIST	{ 0x00000000, }
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_MAX_CHIPS	1

/* NOR */
#define CONFIG_ENV_IS_IN_FLASH
#undef  CONFIG_ENV_IS_IN_NAND
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_SYS_MAX_FLASH_BANKS	1		/* max number of flash banks */
#define CONFIG_SYS_FLASH_SECT_SZ	0x20000		/* 128KB sect size Spansion Flash */
#define PHYS_FLASH_1			0x20000000	/* CS0 Base address 	 */
#define PHYS_FLASH_SIZE			0x2000000	/* Flash size 32MB 	 */
#define CONFIG_SYS_FLASH_BASE		PHYS_FLASH_1	/* Flash Base for U-Boot */

#define CONFIG_SYS_MAX_FLASH_SECT	(PHYS_FLASH_SIZE/CONFIG_SYS_FLASH_SECT_SZ)
#define CONFIG_SYS_ENV_SECT_SIZE	CONFIG_SYS_FLASH_SECT_SZ	/* Env sector Size */
#define CONFIG_SYS_ENV_OFFSET		(CONFIG_SYS_FLASH_SECT_SZ*3)
#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE+CONFIG_SYS_ENV_OFFSET)
#define	CONFIG_ENV_SIZE			CONFIG_SYS_FLASH_SECT_SZ

/* U-Boot command configuration */
#include <config_cmd_default.h>

#undef CONFIG_CMD_BDI
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_SETGETDCR

#define CONFIG_CMD_JFFS2
#define	CONFIG_CMD_ENV
#define CONFIG_CMD_FLASH
#define CONFIG_CMD_ASKENV
#define CONFIG_CMD_PING
#define CONFIG_CMD_SAVES
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_NAND
#define CONFIG_CMD_UBI
#define CONFIG_RBTREE
#define CONFIG_CMD_DHCP

#define CONFIG_CRC32_VERIFY
#define CONFIG_MX_CYCLIC

/* U-Boot general configuration */
#undef CONFIG_USE_IRQ					/* No IRQ/FIQ in U-Boot */
#define CONFIG_SYS_PROMPT		"U-Boot> "	/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE		1024		/* Console I/O Buffer Size  */
#define CONFIG_SYS_PBSIZE				/* Print buffer size */ \
					(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		16		/* max number of command args */
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	">"
#define CONFIG_SYS_LONGHELP

#define CONFIG_BOOTDELAY		1
#define CONFIG_BOOTCOMMAND		"bootm 0x20800000"
#define CONFIG_BOOTARGS 		"mtdparts=physmap-flash.0:512k(bootloader)ro,2048k(kernel),-(root) " \
					"console=ttyS0,115200n8 root=/dev/mtdblock2 rootfstype=jffs2"

#define CONFIG_EXTRA_ENV_SETTINGS	"update_kernel=dhcp uImage-topas910; nand erase 0x80000 0x300000; nand write ${fileaddr} 0x80000 0x300000\0" \
					"update_rootfs=dhcp rootfs-topas910; nand erase 0x380000; nand write ${fileaddr} 0x380000 ${filesize}\0"

#define CONFIG_CMDLINE_EDITING
#define CONFIG_VERSION_VARIABLE
#define CONFIG_TIMESTAMP

#define CONFIG_NET_RETRY_COUNT 10

/* U-Boot memory configuration */

#define	CONFIG_STACKSIZE		(64 * 1024)
#define	CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 512*1024)
#define CONFIG_SYS_GBL_DATA_SIZE	128		/* for initial data */
#define CONFIG_SYS_MEMTEST_START	0x04100000
#define CONFIG_SYS_MEMTEST_END		0x04F00000

/* Linux interfacing */
#define CONFIG_CMDLINE_TAG
#define	LINUX_BOOT_PARAM_ADDR		0x40000100
#define CONFIG_SETUP_MEMORY_TAGS
#define	CONFIG_SYS_CBSIZE		1024
#define	CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define	CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define	CONFIG_SYS_LOAD_ADDR		0x40600000

#endif
