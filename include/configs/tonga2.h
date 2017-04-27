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
#define CONFIG_CMD_SETEXPR
#define CONFIG_PREBOOT                      "run setup"
#define CONFIG_BOOTCOMMAND                  "run rootfs_base;run bootargs_base; nboot kernel; bootm"

#define CONFIG_EXTRA_ENV_SETTINGS           "update_kernel_dhcp=dhcp uImage-tonga2; nand erase.part kernel; nand write ${fileaddr} kernel            \0" \
                                            "update_rootfs_dhcp=dhcp rootfs       ; nand erase.part rootfs; nand write ${fileaddr} rootfs ${filesize}\0" \
                                            "update_kernel_tftp=tftp uImage-tonga2; nand erase.part kernel; nand write ${fileaddr} kernel            \0" \
                                            "update_rootfs_tftp=tftp rootfs       ; nand erase.part rootfs; nand write ${fileaddr} rootfs ${filesize}\0" \
                                            "update_uboot_dhcp=dhcp u-boot_nand_tonga2.bin; nand device 1;nand erase 0 0x80000; nand write ${fileaddr} 0 ${filesize}\0" \
                                            "update_uboot_tftp=tftp u-boot_nand_tonga2.bin; nand device 1;nand erase 0 0x80000; nand write ${fileaddr} 0 ${filesize}\0" \
                                            "update_flash_tftp=tftp flashimage_tonga2.bin; setexpr ts $filesize - 0x80000; nand erase 0x80000 0x800000; nand erase.part rootfs; nand write 0x40680000 0x80000 $ts ; nand device 1;nand erase 0 0x80000; nand write ${loadaddr} 0 0x80000;\0" \
                                            "update_flash_dhcp=dhcp flashimage_tonga2.bin; setexpr ts $filesize - 0x80000; nand erase 0x80000 0x800000; nand erase.part rootfs; nand write 0x40680000 0x80000 $ts ; nand device 1;nand erase 0 0x80000; nand write ${loadaddr} 0 0x80000;\0" \
                                            "rootfs_jffs2=root=/dev/mtdblock4 rootfstype=jffs2\0" \
                                            "rootfs_ubifs=ubi.mtd=4 root=ubi0:rootfs rootfstype=ubifs\0" \
                                            "rootfs_base=setenv rootfs ${rootfs_jffs2}\0"\
                                            "videoparams=video=tmpa9xxfb:19211e4c:10040cef:013f380d:00010828\0" \
                                            "bootargs_base=setenv bootargs console=ttyS0,115200n8 consoleblank=0 ${rootfs} ${mtdparts} ${videoparams} ethaddr=${ethaddr}\0" \
                                            "verify=n\0" \
                                            "setup=" \
                                            "if test -n mtdparts; then " \
                                               "nand bad;"\
                                               "dynpart;"\
                                               "setenv setup;"\
                                               "setenv preboot;"\
                                               "saveenv;"\
                                               "mtdparts;"\
                                            "fi\0"
//                                             "nand env.oob set u-boot_env;"
 /* Include generic part */
#include "tmpa9xx-generic.h"

#endif
