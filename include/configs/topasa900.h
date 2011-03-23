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

/* TopasA 900 Evaluation Board */
#define TOPASA_900
#define MACH_TYPE MACH_TYPE_TOPASA900
#define CONFIG_SOC_TMPA900

/* Memory Info */
#define PHYS_SDRAM_1_SIZE                   0x04000000     /* 64MB */

/* Flash usage */
#define USE_NAND
#define USE_NOR
#define BOOT_NOR

/* Ethernet */
#define USE_DM9000

/* MTD Definitions */
#define MTDIDS_DEFAULT                      "nor0=physmap-flash.0,nand0=tmpa9xx-nand"
#define MTDPARTS_DEFAULT                    "mtdparts=physmap-flash.0:0x00080000(u-boot),0x00020000(u-boot_env),0x00300000(splash),0x00300000(kernel),-(rootfs);tmpa9xx-nand:-(storage)"

/* Enviroment vaiables */
#define CONFIG_BOOTCOMMAND                  "run rootfs_base;run bootargs_base; bootm 0x203a0000"

#define CONFIG_EXTRA_ENV_SETTINGS           "update_kernel_dhcp=dhcp uImage-topasa900; erase nor0,3; protect off nor0,3; cp.b ${fileaddr} 0x203a0000 ${filesize}; protect on nor0,3\0" \
                                            "update_rootfs_dhcp=dhcp rootfs          ; erase nor0,4; protect off nor0,4; cp.b ${fileaddr} 0x206a0000 ${filesize}; protect on nor0,4\0" \
                                            "update_kernel_tftp=tftp uImage-topasa900; erase nor0,3; protect off nor0,3; cp.b ${fileaddr} 0x203a0000 ${filesize}; protect on nor0,3\0" \
                                            "update_rootfs_tftp=tftp rootfs          ; erase nor0,4; protect off nor0,4; cp.b ${fileaddr} 0x206a0000 ${filesize}; protect on nor0,4\0" \
                                            "videoparams=video=tmpa9xxfb:0x0707074c:0x020204ef:0x013f200e:0x0001082A\0" \
                                            "mtdparts=" MTDPARTS_DEFAULT "\0"			\
                                            "mtdids=" MTDIDS_DEFAULT "\0" \
                                            "rootfs_jffs2=root=/dev/mtdblock4 rootfstype=jffs2\0" \
                                            "rootfs_ubifs=ubi.mtd=4 root=ubi0:rootfs rootfstype=ubifs\0" \
                                            "rootfs_base=setenv rootfs ${rootfs_jffs2}\0"\
                                            "bootargs_base=setenv bootargs console=ttyS0,115200n8 consoleblank=0 ${rootfs} ${mtdparts}\0" \
                                            "verify=n\0"

/* Include generic part */
#include "tmpa9xx-generic.h"

#endif
