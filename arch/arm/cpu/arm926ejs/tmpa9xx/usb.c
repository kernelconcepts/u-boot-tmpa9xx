/*
 * (C) Copyright 2006
 * DENX Software Engineering <mk@denx.de>
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

#if defined(CONFIG_USB_OHCI_NEW) && \
    defined(CONFIG_SYS_USB_OHCI_CPU_INIT) && \
    defined(CONFIG_SOC_TMPA900)

#include <asm/arch/tmpa9xx.h>

int usb_cpu_init(void)
{
        /* prepare Port C for USB Host */
        GPIOCDATA  =  (0xcf);
        GPIOCDIR  &= ~(0xc0);
        GPIOCFR1  &= ~(0xc0);
        GPIOCFR2  |=  (0xc0);
        GPIOCODE  &= ~(0xc0);
        GPIOCIE   &= ~(0xc0);

        /* Enable USB Host Controller Clock Domain */
        CLKCR5    |= (1<<4);

        /* Set appropriate clock seting for USB in SYSCR8.
           For USB device, 24Mhz directly from quartz: [5:4]  11 / 0x3
           For USB host  , 48Mhz from F PPL / 4      : [3:0] 100 / 0x4 */
        SYSCR8    |= ((0x3<<4)|(0x4<<0));

        /* Enable overcurrent */
        USBHOST_OHCI_HcBCR0     = 0;

	return 0;
}

int usb_cpu_stop(void)
{

        /* reset Port C to default */
        GPIOCDATA  = 0x00;
        GPIOCDIR   = 0x00;
        GPIOCFR1   = 0x00;
        GPIOCFR2   = 0x00;
        GPIOCODE   = 0x00;
        GPIOCIE    = 0x00;

        /* Disable USB Host Controller Clock Domain */
        CLKCR5    &= ~(1<<4);

        /* De-Set appropriate clock seting for USB in SYSCR8.
           For USB device, 24Mhz directly from quartz: [5:4]  11 / 0x3
           For USB host  , 48Mhz from F PPL / 4      : [3:0] 100 / 0x4 */
        SYSCR8    &= ~((0x3<<4)|(0x4<<0));

	return 0;
}

int usb_cpu_init_fail(void)
{
	return usb_cpu_stop();
}

#endif 

/* defined(CONFIG_USB_OHCI_NEW) && \
   defined(CONFIG_SYS_USB_OHCI_CPU_INIT) && \
   defined(CONFIG_S3C24X0) */
