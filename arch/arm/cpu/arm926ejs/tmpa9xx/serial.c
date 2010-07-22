/*
 * (C) Copyright 2009,2010
 * Kernel Concepts <www.kernelconcepts.de>
 * Florian Boor (florian.boor@kernelconcepts.de)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <common.h>
#include <asm/arch/tmpa9xx.h>
#include <asm/arch/tmpa9xx_serial.h>

DECLARE_GLOBAL_DATA_PTR;

#define	UART_CLK		96000000

/* We use UART1 as console UART */
void serial_setbrg(void)
{
	unsigned int brdi = 0, brdf = 0;

	/* configure UART1 pins */
	/* GPIOTFR1 = 0x00000030; */

	/* configure UART0 pins */
	GPIONFR1 = 0x000000fd; 
	GPIONFR2 = 0x00000002; 
	
	/* we just support 38400 and 115200 now and assume UART_CLK is 96Mhz */
	if (gd->baudrate == 38400) {
		brdi = 157;
		brdf = 4;
	} else if (gd->baudrate == 115200) {
		brdi = 52;
		brdf = 5;
	} else {
		hang();
	}

	/* Disable UART first */
	UART0CR = 0x0;	

	UART0IBRD = brdi;	/* set baudrate */
	UART0FBRD = brdf;

	/* 8bit, none FIFO mode, 1 stop bit, No parity, No break */
	UART0LCR_H = 0x60;

	/* CTS disable, RTS disable
	 * UART receive enable.
	 * UART transmit enable.
	 * Loopback disable.
	 * SIR disable.
	 * UART enable.
	 */
	UART0CR = 0x301;	
}

int serial_init(void)
{
	serial_setbrg();
	return 0;
}

void serial_putc(const char c)
{
	while ((UART0FR & UART_FR_TXFE) == 0);
	UART0DR = (unsigned long)(c & 0xff);

	if (c == '\n')
		serial_putc('\r');
}

void serial_puts(const char *s)
{
	while (*s) {
		serial_putc(*s++);
	}
}

int serial_getc(void)
{
	while ((UART0FR & UART_FR_RXFE) != 0);

	return UART0DR & 0xff;
}

int serial_tstc(void)
{
	return (!(UART0FR & UART_FR_RXFE));
}


