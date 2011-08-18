/*
 * (C) Copyright 2010
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
#ifndef __GENERIC_CONFIG_H
#define __GENERIC_CONFIG_H

#define CONFIG_SYS_CONSOLE_INFO_QUIET

/* SoC Configuration */
#define CONFIG_ARM926EJS
#define CONFIG_SYS_HZ                       32000

/* Memory Info */
#define CONFIG_NR_DRAM_BANKS                1
#define PHYS_SDRAM_1                        0x40000000


#define CONFIG_SYS_SDRAM_BASE               PHYS_SDRAM_1
#define CONFIG_SYS_INIT_RAM_SIZE            0x1000
#define CONFIG_SYS_INIT_SP_ADDR             (CONFIG_SYS_SDRAM_BASE + \
                                             CONFIG_SYS_INIT_RAM_SIZE - \
                                             GENERATED_GBL_DATA_SIZE)

/* Serial Driver info: UART0 for console  */
#define CONFIG_BAUDRATE                     115200
#define CONFIG_SYS_BAUDRATE_TABLE           { 115200 }

/* Ethernet */
#ifdef USE_DM9000
/* Ethernet:  external DM9000 */
#define CONFIG_DRIVER_DM9000                1
#define CONFIG_DM9000_BASE                  0x60000000
#define CONFIG_DM9000_USE_16BIT             1
#define DM9000_IO                           CONFIG_DM9000_BASE
#define DM9000_DATA                         (CONFIG_DM9000_BASE+0x1000)
#define CONFIG_NET_MULTI

#define CONFIG_IPADDR                       192.168.1.2
#define CONFIG_SERVERIP                     192.168.1.1
#endif /* USE_DM9000 */

#ifdef USE_SMCS91XX
/* Ethernet:  external SMCS91XX */
#define CONFIG_SMC911X
#define CONFIG_SMC911X_BASE                 0x60000000
#define CONFIG_SMC911X_32_BIT
#define CONFIG_NET_MULTI

#define CONFIG_IPADDR                       192.168.1.2
#define CONFIG_SERVERIP                     192.168.1.1
#define CONFIG_ETHADDR                      00:11:22:44:55:66 
/* Enable overwriting MAC Address */
#define CONFIG_ENV_OVERWRITE
//#define CONFIG_OVERWRITE_ETHADDR_ONCE
#endif /* USE_SMCS91XX */


/* General */
#define CONFIG_SYS_ENV_OFFSET               0x80000        /* 384 kB for u-boot */

#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#define CONFIG_SYS_ENV_SECT_SIZE            0x20000
#define CONFIG_ENV_SIZE                     CONFIG_SYS_ENV_SECT_SIZE

/* NAND */
#ifdef USE_NAND
/* In case of NAND boot - autoboot doing this */
#define CONFIG_SKIP_LOWLEVEL_INIT        
#define CONFIG_SKIP_RELOCATE_UBOOT
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_OFFSET_OOB
#define CONFIG_NAND_DYNPART
#define CFG_NAND_DYNPART_MTD_KERNEL_NAME    "tmpa9xx-nand"
#define CONFIG_SYS_NAND_USE_FLASH_BBT
#define CONFIG_SYS_NAND_BASE_LIST           {0x00000000,}
#define CONFIG_SYS_MAX_NAND_DEVICE          1
#define CONFIG_SYS_NAND_MAX_CHIPS           1
#define CFG_MAX_NAND_DEVICE                 CONFIG_SYS_NAND_MAX_CHIPS
#define CONFIG_CMD_NAND

#define CONFIG_NAND_SIZE    0x10000000 /* 256 MB NAND Flash */
#define CONFIG_KERNEL_SIZE  0x00300000	/* keep some spare for further kernel development */
#define CONFIG_SPLASH_SIZE  0x00300000	/* enought for 1024x768x32 */

#define CONFIG_NAND_DYNPART_PARTITIONING						\
	unsigned int dynpart_size[] = {							\
	    CONFIG_SYS_ENV_OFFSET,      /* before is (size of) u-boot */		\
	    CONFIG_SYS_ENV_SECT_SIZE,   /* one page for the u-boot environment */	\
	    CONFIG_SPLASH_SIZE,								\
	    CONFIG_KERNEL_SIZE,								\
	    CONFIG_NAND_SIZE-								\
	    CONFIG_SYS_ENV_OFFSET-							\
	    CONFIG_SYS_ENV_SECT_SIZE-							\
	    CONFIG_SPLASH_SIZE-								\
	    CONFIG_KERNEL_SIZE,         /* rest of nand */				\
	    0 };									\
	char *dynpart_names[] = {							\
	    "u-boot",									\
	    "u-boot_env",								\
	    "splash",									\
	    "kernel",									\
	    "rootfs",									\
	     NULL };

#endif /* USE_NAND */

/* Reconfigure some stuff if booting from NOR is selected */
#ifdef BOOT_NOR
#undef  CONFIG_SKIP_LOWLEVEL_INIT        
#undef  CONFIG_SKIP_RELOCATE_UBOOT
#undef  CONFIG_ENV_IS_IN_NAND
#undef  CONFIG_ENV_OFFSET_OOB
#undef  CONFIG_NAND_DYNPART
#define CONFIG_ENV_IS_IN_FLASH
#define CONFIG_SYS_TEXT_BASE                0x20000000
#else
#if( PHYS_SDRAM_1_SIZE == 0x04000000)
#define CONFIG_SYS_TEXT_BASE                0x43F00000
#else
#define CONFIG_SYS_TEXT_BASE                0x41F00000
#endif

#endif

#ifdef USE_NOR
/* NOR */
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_FLASH_CFI_MTD
#define CONFIG_SYS_MAX_FLASH_BANKS          1              /* max number of flash banks */
#define CONFIG_SYS_FLASH_BASE               0x20000000     /* CS0 Base */
#define CONFIG_SYS_MAX_FLASH_SECT           512
#define CONFIG_ENV_ADDR                     (CONFIG_SYS_FLASH_BASE+CONFIG_SYS_ENV_OFFSET)
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE

#define CONFIG_CMD_FLASH
#else
#define CONFIG_SYS_NO_FLASH
#endif /* USE_NOR */


#ifdef USE_SPI
/* SPI */
#define CONFIG_CMD_SPI
#define CONFIG_SPI
#define CONFIG_SOFT_SPI
#define SPI_READ                            tmpa_spi_bb_read()
#define SPI_SDA(bit)                        tmpa_spi_bb_sda(bit)
#define SPI_SCL(bit)                        tmpa_spi_bb_scl(bit)
#define SPI_DELAY                           udelay(10);
#endif /* USE_SPI */

/* U-Boot general configuration */
#define CONFIG_BOOTDELAY                    1
#undef  CONFIG_USE_IRQ                                     /* No IRQ/FIQ in U-Boot */
#define CONFIG_SYS_PROMPT                   "U-Boot> "     /* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE                   1024           /* Console I/O Buffer Size  */
#define CONFIG_SYS_PBSIZE                                  /* Print buffer size */ \
                                            (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS                  16             /* max number of command args */
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2          ">"
#define CONFIG_SYS_LONGHELP

#define CONFIG_CMDLINE_EDITING
#define CONFIG_TIMESTAMP

#define CONFIG_NET_RETRY_COUNT              10
#define CONFIG_SPLASH_SCREEN
#define CONFIG_VIDEO
#define CONFIG_VIDEO_BMP_GZIP
#define CONFIG_SYS_VIDEO_LOGO_MAX_SIZE      0x400000 
#define CONFIG_VIDEO_TMPA9XX
#define CONFIG_RBTREE

/* U-Boot command configuration */
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_UBI
#define CONFIG_CMD_BMP
#define CONFIG_CMD_MEMORY
#define CONFIG_CMD_RUN
#define CONFIG_CMD_BOOTD

#if defined USE_DM9000 || defined USE_SMCS91XX
#define CONFIG_CMD_PING
#define CONFIG_CMD_NET
#define CONFIG_CMD_DHCP
#endif /* defined USE_DM9000 || defined USE_SMCS91XX */

#if defined USE_NAND || defined USE_NOR
#define CONFIG_CMD_SAVEENV
#define CONFIG_CMD_ENV
#endif /* defined USE_NAND || defined USE_NOR */

/* U-Boot memory configuration */
#define CONFIG_STACKSIZE                    (64 * 1024)
#define CONFIG_SYS_MALLOC_LEN               (2*CONFIG_SYS_VIDEO_LOGO_MAX_SIZE+CONFIG_ENV_SIZE + 512*1024)
#define CONFIG_SYS_GBL_DATA_SIZE            128            /* for initial data */
#define CONFIG_SYS_MEMTEST_START            0x04100000
#define CONFIG_SYS_MEMTEST_END              0x04F00000

/* Linux interfacing */
#define CONFIG_CMDLINE_TAG
#define LINUX_BOOT_PARAM_ADDR               0x40000100
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_SYS_BARGSIZE                 CONFIG_SYS_CBSIZE
#define CONFIG_SYS_LOAD_ADDR                0x40600000

#endif
