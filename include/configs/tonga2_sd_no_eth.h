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

/* Tonga2 Board */
#define TONGA2_SD_NO_ETH
#define MACH_TYPE MACH_TYPE_TONGA

#define CONFIG_SKIP_LOWLEVEL_INIT	
#define CONFIG_SKIP_RELOCATE_UBOOT

#define CONFIG_SYS_CONSOLE_INFO_QUIET

/* SoC Configuration */
#define CONFIG_ARM926EJS				/* arm926ejs CPU */
#define CONFIG_SYS_HZ			32000
#define CONFIG_SOC_TMPA900

/* Memory Info */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1			0x40000000
#define PHYS_SDRAM_1_SIZE		0x02000000	/* 32MB */

/* Serial Driver info: UART0 for console  */
#define	CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE      	{ 115200 }

/* NAND */
#define CONFIG_ENV_OFFSET_OOB
#define CONFIG_NAND_DYNPART
#define MTDIDS_DEFAULT			 "nand0=tmpa9x0-nand"
#define CFG_NAND_DYNPART_MTD_KERNEL_NAME "tmpa9x0-nand"


#define CONFIG_SYS_NO_FLASH
#define	CONFIG_ENV_IS_IN_NAND
#define	CONFIG_SYS_ENV_SECT_SIZE	0x20000

#define CONFIG_SYS_NAND_USE_FLASH_BBT
#define CONFIG_SYS_NAND_PAGE_2K
#define CONFIG_SYS_NAND_LARGEPAGE

#define CONFIG_SYS_NAND_BASE_LIST	{ 0x00000000, }
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_MAX_CHIPS	1
#define CFG_MAX_NAND_DEVICE		CONFIG_SYS_NAND_MAX_CHIPS
#define	CONFIG_ENV_SIZE			CONFIG_SYS_ENV_SECT_SIZE
#define CONFIG_ENV_OFFSET               (CONFIG_ENV_SIZE*4)

/* U-Boot command configuration */
#include <config_cmd_default.h>

#undef CONFIG_CMD_BDI
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_SETGETDCR
#undef CONFIG_CMD_NET

#define CONFIG_CMD_JFFS2
#define CONFIG_JFFS2_NAND
#define CONFIG_JFFS2_DEV		"tmpa9x0-nand"
#define	CONFIG_CMD_ENV
#define CONFIG_CMD_SAVES
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_NAND
#define CONFIG_CMD_UBI
#define CONFIG_RBTREE
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_BMP
#define CONFIG_CRC32_VERIFY
#define CONFIG_MX_CYCLIC

/* U-Boot general configuration */
#undef CONFIG_USE_IRQ					/* No IRQ/FIQ in U-Boot */
#define CONFIG_SYS_PROMPT		"Tonga2_sd_no_eth>"	/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE		1024		/* Console I/O Buffer Size  */
#define CONFIG_SYS_PBSIZE				/* Print buffer size */ \
					(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		16		/* max number of command args */
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	">"
#define CONFIG_SYS_LONGHELP

#define CONFIG_PREBOOT                  "run setup"
#define CONFIG_BOOTDELAY		1
#define CONFIG_BOOTCOMMAND		"run rootfs_base;run bootargs_base; nboot kernel; bootm"

#define CONFIG_EXTRA_ENV_SETTINGS      	"rootfs_jffs2=root=/dev/mtdblock4 rootfstype=jffs2\0" \
                                    	"rootfs_ubifs=ubi.mtd=4 root=ubi0:rootfs rootfstype=ubifs\0" \
                                    	"rootfs_base=setenv rootfs ${rootfs_jffs2}\0"\
                                        "videoparams=video=tmpa9xxfb:28050a74:0808290f:01df000b:00010828\0" \
                                    	"bootargs_base=setenv bootargs console=ttyS0,115200n8 ${rootfs} ${mtdparts} ${videoparams}\0" \
                                        "setup=" \
                                        "if test -n mtdparts; then " \
                                        	"nand bad;"\
                                                "dynpart;"\
                                                "nand env.oob set u-boot_env;"\
                                                "setenv setup;"\
                                                "setenv preboot 'nand read 0x43000000 splash;bmp display 0x43000000';"\
                                                "saveenv;"\
                                                "reset;"\
                                        "fi\0"

#define CONFIG_CMDLINE_EDITING
#define CONFIG_VERSION_VARIABLE
#define CONFIG_TIMESTAMP

#define CONFIG_NET_RETRY_COUNT 10
#define CONFIG_SPLASH_SCREEN
#define CONFIG_VIDEO
#define CONFIG_VIDEO_BMP_GZIP
#define CONFIG_SYS_VIDEO_LOGO_MAX_SIZE  0x1800000
#define CONFIG_VIDEO_TMPA9XX
/* U-Boot memory configuration */

#define	CONFIG_STACKSIZE		(64 * 1024)
//#define	CONFIG_SYS_MALLOC_LEN		(CONFIG_SYS_VIDEO_LOGO_MAX_SIZE+CONFIG_ENV_SIZE + 512*1024)
#define CONFIG_SYS_MALLOC_LEN		(4 << 20)	/* Reserve 4 MB for malloc */
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
