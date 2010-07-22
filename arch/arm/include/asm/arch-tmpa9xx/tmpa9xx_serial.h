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

#ifndef	__TMPA9X0_SERIAL_H__
#define	__TMPA9X0_SERIAL_H__

/* UARTDR */
#define	UART_DR_OE		(1 << 11)
#define	UART_DR_BE		(1 << 10)
#define	UART_DR_PE		(1 << 9)
#define	UART_DR_FE		(1 << 8)

/* UARTRSR */
#define	UART_RSR_OE		(1 << 3)
#define	UART_RSR_BE		(1 << 2)
#define	UART_RSR_PE		(1 << 1)
#define	UART_RSR_FE		(1 << 0)

/* UARTFR */
#define	UART_FR_RI		(1 << 8)
#define	UART_FR_TXFE		(1 << 7)
#define	UART_FR_RXFF		(1 << 6)
#define	UART_FR_TXFF		(1 << 5)
#define	UART_FR_RXFE		(1 << 4)
#define	UART_FR_BUSY		(1 << 3)
#define	UART_FR_DCD		(1 << 2)
#define	UART_FR_DSR		(1 << 1)
#define	UART_FR_CTS		(1 << 0)

/* UARTILPR */

/* UARTLCR */
#define	UART_LCR_SPS		(1 << 7)
#define	UART_LCR_WLEN5		(0x0)
#define	UART_LCR_WLEN6		(1 << 5)
#define	UART_LCR_WLEN7		(2 << 5)
#define	UART_LCR_WLEN8		(3 << 5)
#define	UART_LCR_FEN		(1 << 4)
#define	UART_LCR_STOP		(1 << 3)
#define	UART_LCR_EPAR		(1 << 2)
#define	UART_LCR_PARITY		(1 << 1)
#define	UART_LCD_SBC		(1 << 0)

/* UARTCR */
#define	UART_CR_CTS_EN		(1 << 15)
#define	UART_CR_RTS_EN		(1 << 14)
#define	UART_CR_OUT2		(1 << 13)
#define	UART_CR_OUT1		(1 << 12)
#define	UART_CR_RTS		(1 << 11)
#define	UART_CR_DTR		(1 << 10)
#define	UART_CR_RXE		(1 << 9)
#define	UART_CR_TXE		(1 << 8)
#define	UART_CR_LBE		(1 << 7)
#define	UART_CR_SIRLP		(1 << 2)
#define	UART_CR_SIREN		(1 << 1)
#define	UART_CR_UARTEN		(1 << 0)

/* UART_INT_MASK */
#define	UART_IMSC_OEIM		(1 << 10)
#define	UART_IMSC_BEIM		(1 << 9)
#define	UART_IMSC_PEIM		(1 << 8)
#define	UART_IMSC_FEIM		(1 << 7)
#define	UART_IMSC_RTIM		(1 << 6)
#define	UART_IMSC_TXIM		(1 << 5)
#define	UART_IMSC_RXIM		(1 << 4)
#define	UART_IMSC_DSRMIM	(1 << 3)
#define	UART_IMSC_DCDMIM	(1 << 2)
#define	UART_IMSC_CTSMIM	(1 << 1)
#define	UART_IMSC_RIMIM		(1 << 0)

/* UART interrupt status */
#define	UART_MIS_OE		(1 << 10)
#define	UART_MIS_BE		(1 << 9)
#define	UART_MIS_PE		(1 << 8)
#define	UART_MIS_FE		(1 << 7)
#define	UART_MIS_RT		(1 << 6)
#define	UART_MIS_TX		(1 << 5)
#define	UART_MIS_RX		(1 << 4)
#define	UART_MIS_DSR		(1 << 3)
#define	UART_MIS_DCD		(1 << 2)
#define	UART_MIS_CTS		(1 << 1)
#define	UART_MIS_RI		(1 << 0)

/* UART DMA CR */
#define	UART_DMACR_DMA_ERR	(1 << 2)
#define	UART_DMACR_TXDMAE	(1 << 1)
#define	UART_DMACR_RSDMAE	(1 << 0)

#endif
