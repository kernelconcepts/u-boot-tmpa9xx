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

#ifndef	_TMPA9X0_HEAD_
#define	_TMPA9X0_HEAD_

#ifndef	__ASSEMBLY__
#define	__REG__(x)		(*((volatile u32 *)(x)))
#else
#define	__REG__(x)		(x)
#endif

/* interrupt controller registers */
#define	INT_BASE_ADDRESS	(0xF4000000)
#define	VICIRQSTATUS        __REG__(INT_BASE_ADDRESS + 0x0000)
#define	VICFIQSTATUS        __REG__(INT_BASE_ADDRESS + 0x0004)
#define	VICRAWINTR          __REG__(INT_BASE_ADDRESS + 0x0008)
#define	VICINTSELECT        __REG__(INT_BASE_ADDRESS + 0x000C)
#define	VICINTENABLE        __REG__(INT_BASE_ADDRESS + 0x0010)
#define	VICINTENCLEAR       __REG__(INT_BASE_ADDRESS + 0x0014)
#define	VICSOFTINT          __REG__(INT_BASE_ADDRESS + 0x0018)
#define	VICSOFTINTCLEAR     __REG__(INT_BASE_ADDRESS + 0x001C)
#define	VICPROTECTION       __REG__(INT_BASE_ADDRESS + 0x0020)
#define	VIC_WPRIORITYMASK   __REG__(INT_BASE_ADDRESS + 0x0024)

/* GPIO registers */
#define	IO_1_GPIO_BASE		(0xF0800000)
#define	IO_1_BASE		(IO_1_GPIO_BASE)
#define	GPIO_A_BASE		(IO_1_GPIO_BASE + 0x0000)
#define	GPIO_B_BASE		(IO_1_GPIO_BASE + 0x1000)
#define	GPIO_C_BASE		(IO_1_GPIO_BASE + 0x2000)
#define	GPIO_D_BASE		(IO_1_GPIO_BASE + 0x3000)
#define	GPIO__BASE		(IO_1_GPIO_BASE + 0x4000)
#define	GPIO_F_BASE		(IO_1_GPIO_BASE + 0x5000)
#define	GPIO_G_BASE		(IO_1_GPIO_BASE + 0x6000)
#define	GPIO_H_BASE		(IO_1_GPIO_BASE + 0x7000)
#define	GPIO_J_BASE		(IO_1_GPIO_BASE + 0x8000)
#define	GPIO_K_BASE		(IO_1_GPIO_BASE + 0x9000)
#define	GPIO_L_BASE		(IO_1_GPIO_BASE + 0xA000)
#define	GPIO_M_BASE		(IO_1_GPIO_BASE + 0xB000)
#define	GPIO_N_BASE		(IO_1_GPIO_BASE + 0xC000)
#define	GPIO_P_BASE		(IO_1_GPIO_BASE + 0xD000)
#define	GPIO_R_BASE		(IO_1_GPIO_BASE + 0xE000)
#define	GPIO_T_BASE		(IO_1_GPIO_BASE + 0xF000)
/* Different Ports for TMPA900 and TMPA910  on same address */
#define	GPIO_U_BASE		(IO_1_GPIO_BASE + 0x4000)
#define	GPIO_V_BASE		(IO_1_GPIO_BASE + 0x7000)

#define	GPIOADATA		__REG__(GPIO_A_BASE + 0x3FC)
#define	GPIOADIR		__REG__(GPIO_A_BASE + 0x400)
#define	GPIOAFR1		__REG__(GPIO_A_BASE + 0x424)
#define	GPIOAFR2		__REG__(GPIO_A_BASE + 0x428)
#define	GPIOAIS			__REG__(GPIO_A_BASE + 0x804)
#define	GPIOAIBE		__REG__(GPIO_A_BASE + 0x808)
#define	GPIOAIEV		__REG__(GPIO_A_BASE + 0x80C)
#define	GPIOAIE			__REG__(GPIO_A_BASE + 0x810)
#define	GPIOARIS		__REG__(GPIO_A_BASE + 0x814)
#define	GPIOAMIS		__REG__(GPIO_A_BASE + 0x818)
#define	GPIOAIC			__REG__(GPIO_A_BASE + 0x81C)

#define	GPIOBDATA		__REG__(GPIO_B_BASE + 0x3FC)
#define	GPIOBDIR		__REG__(GPIO_B_BASE + 0x400)
#define GPIOBFR1		__REG__(GPIO_B_BASE + 0x424)
#define	GPIOBFR2		__REG__(GPIO_B_BASE + 0x428)
#define	GPIOBODE		__REG__(GPIO_B_BASE + 0xC00)

#define	GPIOCDATA		__REG__(GPIO_C_BASE + 0x3FC)
#define	GPIOCDIR		__REG__(GPIO_C_BASE + 0x400)
#define	GPIOCFR1		__REG__(GPIO_C_BASE + 0x424)
#define	GPIOCFR2		__REG__(GPIO_C_BASE + 0x428)
#define	GPIOCIS			__REG__(GPIO_C_BASE + 0x804)
#define	GPIOCIBE		__REG__(GPIO_C_BASE + 0x808)
#define	GPIOCIEV		__REG__(GPIO_C_BASE + 0x80C)
#define	GPIOCIE			__REG__(GPIO_C_BASE + 0x810)
#define	GPIOCRIS		__REG__(GPIO_C_BASE + 0x814)
#define	GPIOCMIS		__REG__(GPIO_C_BASE + 0x818)
#define	GPIOCIC			__REG__(GPIO_C_BASE + 0x81C)
#define	GPIOCODE		__REG__(GPIO_C_BASE + 0xC00)

#define	GPIODDATA		__REG__(GPIO_D_BASE + 0x3FC)
#define	GPIODDIR		__REG__(GPIO_D_BASE + 0x400)
#define	GPIODFR1		__REG__(GPIO_D_BASE + 0x424)
#define	GPIODFR2		__REG__(GPIO_D_BASE + 0x428)
#define	GPIODIS			__REG__(GPIO_D_BASE + 0x804)
#define	GPIODIBE		__REG__(GPIO_D_BASE + 0x808)
#define	GPIODIEV		__REG__(GPIO_D_BASE + 0x80C)
#define	GPIODIE			__REG__(GPIO_D_BASE + 0x810)
#define	GPIODRIS		__REG__(GPIO_D_BASE + 0x814)
#define	GPIODMIS		__REG__(GPIO_D_BASE + 0x818)
#define	GPIODIC			__REG__(GPIO_D_BASE + 0x81C)

#define	GPIOEDATA		__REG__(GPIO_E_BASE + 0x3FC)
#define GPIOEDIR		__REG__(GPIO_E_BASE + 0x400)
#define GPIOEFR1		__REG__(GPIO_E_BASE + 0x424)
#define GPIOEFR2		__REG__(GPIO_E_BASE + 0x428)
#define GPIOFDATA		__REG__(GPIO_F_BASE + 0x000)
#define GPIOFDIR		__REG__(GPIO_F_BASE + 0x400)
#define GPIOFFR1		__REG__(GPIO_F_BASE + 0x424)
#define GPIOFFR2		__REG__(GPIO_F_BASE + 0x428)
#define GPIOFIS			__REG__(GPIO_F_BASE + 0x804) 
#define GPIOFIBE		__REG__(GPIO_F_BASE + 0x808) 
#define GPIOFIEV		__REG__(GPIO_F_BASE + 0x80C) 
#define GPIOFIE			__REG__(GPIO_F_BASE + 0x810) 
#define GPIOFRIS		__REG__(GPIO_F_BASE + 0x814) 
#define GPIOFMIS		__REG__(GPIO_F_BASE + 0x818) 
#define GPIOFIC			__REG__(GPIO_F_BASE + 0x81C) 
#define GPIOFODE		__REG__(GPIO_F_BASE + 0xC00) 

#define GPIOGDATA		__REG__(GPIO_G_BASE + 0x3FC) 
#define GPIOGDIR		__REG__(GPIO_G_BASE + 0x400) 
#define GPIOGFR1		__REG__(GPIO_G_BASE + 0x424) 
#define GPIOGFR2		__REG__(GPIO_G_BASE + 0x428) 

#define GPIOHDATA		__REG__(GPIO_H_BASE + 0x3FC) 
#define GPIOHDIR		__REG__(GPIO_H_BASE + 0x400)    
#define GPIOHFR1		__REG__(GPIO_H_BASE + 0x424)    
#define GPIOHFR2		__REG__(GPIO_H_BASE + 0x428)    

#define GPIOJDATA		__REG__(GPIO_J_BASE + 0x3FC)  
#define GPIOJDIR		__REG__(GPIO_J_BASE + 0x400)  
#define GPIOJFR1		__REG__(GPIO_J_BASE + 0x424)  
#define GPIOJFR2		__REG__(GPIO_J_BASE + 0x428)

#define GPIOKDATA		__REG__(GPIO_K_BASE + 0x3FC)
#define GPIOKDIR		__REG__(GPIO_K_BASE + 0x400)
#define GPIOKFR1		__REG__(GPIO_K_BASE + 0x424)
#define GPIOKFR2		__REG__(GPIO_K_BASE + 0x428)

#define GPIOLDATA		__REG__(GPIO_L_BASE + 0x3FC)
#define GPIOLDIR		__REG__(GPIO_L_BASE + 0x400)
#define GPIOLFR1		__REG__(GPIO_L_BASE + 0x424)
#define GPIOLFR2		__REG__(GPIO_L_BASE + 0x428)

#define GPIOMDATA		__REG__(GPIO_M_BASE + 0x3FC)
#define GPIOMDIR		__REG__(GPIO_M_BASE + 0x400)
#define GPIOMFR1		__REG__(GPIO_M_BASE + 0x424)
#define GPIOMFR2		__REG__(GPIO_M_BASE + 0x428)

#define GPIONDATA		__REG__(GPIO_N_BASE + 0x3FC)
#define GPIONDIR		__REG__(GPIO_N_BASE + 0x400)
#define GPIONFR1		__REG__(GPIO_N_BASE + 0x424)
#define GPIONFR2		__REG__(GPIO_N_BASE + 0x428)
#define GPIONIS			__REG__(GPIO_N_BASE + 0x804)
#define GPIONIBE		__REG__(GPIO_N_BASE + 0x808)
#define GPIONIEV		__REG__(GPIO_N_BASE + 0x80C)
#define GPIONIE			__REG__(GPIO_N_BASE + 0x810)
#define GPIONRIS		__REG__(GPIO_N_BASE + 0x814)
#define GPIONMIS		__REG__(GPIO_N_BASE + 0x818)
#define GPIONIC			__REG__(GPIO_N_BASE + 0x81C)

#define GPIOPDATA		__REG__(GPIO_P_BASE + 0x3FC)
#define GPIOPDIR		__REG__(GPIO_P_BASE + 0x400)
#define GPIOPFR1		__REG__(GPIO_P_BASE + 0x424)
#define GPIOPFR2		__REG__(GPIO_P_BASE + 0x428)
#define GPIOPIS			__REG__(GPIO_P_BASE + 0x804)
#define GPIOPIBE		__REG__(GPIO_P_BASE + 0x808)
#define GPIOPIEV		__REG__(GPIO_P_BASE + 0x80C)
#define GPIOPIE			__REG__(GPIO_P_BASE + 0x810)
#define GPIOPRIS		__REG__(GPIO_P_BASE + 0x814)
#define GPIOPMIS		__REG__(GPIO_P_BASE + 0x818)
#define GPIOPIC			__REG__(GPIO_P_BASE + 0x81C)

#define GPIORDATA		__REG__(GPIO_R_BASE + 0x3FC)
#define GPIORDIR		__REG__(GPIO_R_BASE + 0x400)
#define GPIORFR1		__REG__(GPIO_R_BASE + 0x424)
#define GPIORFR2		__REG__(GPIO_R_BASE + 0x428)
#define GPIORIS			__REG__(GPIO_R_BASE + 0x804)
#define GPIORIBE		__REG__(GPIO_R_BASE + 0x808)
#define GPIORIEV		__REG__(GPIO_R_BASE + 0x80C)
#define GPIORIE			__REG__(GPIO_R_BASE + 0x810)
#define GPIORRIS		__REG__(GPIO_R_BASE + 0x814)
#define GPIORMIS		__REG__(GPIO_R_BASE + 0x818)
#define GPIORIC			__REG__(GPIO_R_BASE + 0x81C)

#define GPIOTDATA		__REG__(GPIO_T_BASE + 0x3FC)
#define GPIOTDIR		__REG__(GPIO_T_BASE + 0x400)
#define GPIOTFR1		__REG__(GPIO_T_BASE + 0x424)
#define GPIOTFR2		__REG__(GPIO_T_BASE + 0x428)

#define GPIOUDATA		__REG__(GPIO_U_BASE + 0x3FC)
#define GPIOUDIR		__REG__(GPIO_U_BASE + 0x400)
#define GPIOUFR1		__REG__(GPIO_U_BASE + 0x424)
#define GPIOUFR2		__REG__(GPIO_U_BASE + 0x428)

#define GPIOVDATA		__REG__(GPIO_V_BASE + 0x3FC)
#define GPIOVDIR		__REG__(GPIO_V_BASE + 0x400)
#define GPIOVFR1		__REG__(GPIO_V_BASE + 0x424)
#define GPIOVFR2		__REG__(GPIO_V_BASE + 0x428)

/* PLL registers */
#define	PLL_BASE_ADDRESS	0xF0050000
#define	SYSCR0			__REG__(PLL_BASE_ADDRESS + 0x000)
#define	SYSCR1			__REG__(PLL_BASE_ADDRESS + 0x004)
#define	SYSCR2			__REG__(PLL_BASE_ADDRESS + 0x008)
#define	SYSCR3			__REG__(PLL_BASE_ADDRESS + 0x00C)
#define	SYSCR4			__REG__(PLL_BASE_ADDRESS + 0x010)
#define	SYSCR5			__REG__(PLL_BASE_ADDRESS + 0x014)
#define	SYSCR6			__REG__(PLL_BASE_ADDRESS + 0x018)
#define	SYSCR7			__REG__(PLL_BASE_ADDRESS + 0x01C)
#define	SYSCR8			__REG__(PLL_BASE_ADDRESS + 0x020)
#define	CLKCR4			__REG__(PLL_BASE_ADDRESS + 0x050)
#define	CLKCR5			__REG__(PLL_BASE_ADDRESS + 0x054)

#define	TIME4_BASE_ADDRESS	(0xF0042000)
#define	TIME4_LD		__REG__(TIME4_BASE_ADDRESS + 0x0000)
#define	TIME4_DATA		__REG__(TIME4_BASE_ADDRESS + 0x0004)
#define	TIME4_CONTROL	__REG__(TIME4_BASE_ADDRESS + 0x0008)
#define	TIME4_INT_CLR	__REG__(TIME4_BASE_ADDRESS + 0x000C)
#define	TIME4_MODE		__REG__(TIME4_BASE_ADDRESS + 0x001C)
#define	TIME4_CAPEN		__REG__(TIME4_BASE_ADDRESS + 0x0060)
#define	TIME4_CMPEN		__REG__(TIME4_BASE_ADDRESS + 0x00E0)

#define	UART0_BASE_ADDRESS	(0xF2000000)
#define	UART0DR			__REG__(UART0_BASE_ADDRESS + 0x000)
#define	UART0RSR		__REG__(UART0_BASE_ADDRESS + 0x004)
#define	UART0ECR		__REG__(UART0_BASE_ADDRESS + 0x004)
#define	UART0FR			__REG__(UART0_BASE_ADDRESS + 0x018)
#define	UART0ILPR		__REG__(UART0_BASE_ADDRESS + 0x020)
#define	UART0IBRD		__REG__(UART0_BASE_ADDRESS + 0x024)
#define	UART0FBRD		__REG__(UART0_BASE_ADDRESS + 0x028)
#define	UART0LCR_H		__REG__(UART0_BASE_ADDRESS + 0x02c)
#define	UART0CR			__REG__(UART0_BASE_ADDRESS + 0x030)
#define	UART0IFLS		__REG__(UART0_BASE_ADDRESS + 0x034)
#define	UART0IMSC		__REG__(UART0_BASE_ADDRESS + 0x038)
#define	UART0RIS		__REG__(UART0_BASE_ADDRESS + 0x03c)
#define	UART0MIS		__REG__(UART0_BASE_ADDRESS + 0x040)
#define	UART0ICR		__REG__(UART0_BASE_ADDRESS + 0x044)
#define	UART0DMACR		__REG__(UART0_BASE_ADDRESS + 0x048)
#define	UART0PeriphID0		__REG__(UART0_BASE_ADDRESS + 0xfe0)
#define	UART0PeriphID1		__REG__(UART0_BASE_ADDRESS + 0xfe4)
#define	UART0PeriphID2		__REG__(UART0_BASE_ADDRESS + 0xfe8)
#define	UART0PeriphID3		__REG__(UART0_BASE_ADDRESS + 0xfec)
#define	UART0PCellID0		__REG__(UART0_BASE_ADDRESS + 0xff0)
#define	UART0PCellID1		__REG__(UART0_BASE_ADDRESS + 0xff4)
#define	UART0PCellID2		__REG__(UART0_BASE_ADDRESS + 0xff8)
#define	UART0PCellID3		__REG__(UART0_BASE_ADDRESS + 0xffc)

#define	UART1_BASE_ADDRESS	(0xF2001000)
#define	UART1DR			__REG__(UART1_BASE_ADDRESS + 0x000)
#define	UART1RSR		__REG__(UART1_BASE_ADDRESS + 0x004)
#define	UART1ECR		__REG__(UART1_BASE_ADDRESS + 0x004)
#define	UART1FR			__REG__(UART1_BASE_ADDRESS + 0x018)
#define	UART1ILPR		__REG__(UART1_BASE_ADDRESS + 0x020)
#define	UART1IBRD		__REG__(UART1_BASE_ADDRESS + 0x024)
#define	UART1FBRD		__REG__(UART1_BASE_ADDRESS + 0x028)
#define	UART1LCR_H		__REG__(UART1_BASE_ADDRESS + 0x02c)
#define	UART1CR			__REG__(UART1_BASE_ADDRESS + 0x030)
#define	UART1IFLS		__REG__(UART1_BASE_ADDRESS + 0x034)
#define	UART1IMSC		__REG__(UART1_BASE_ADDRESS + 0x038)
#define	UART1RIS		__REG__(UART1_BASE_ADDRESS + 0x03c)
#define	UART1MIS		__REG__(UART1_BASE_ADDRESS + 0x040)
#define	UART1ICR		__REG__(UART1_BASE_ADDRESS + 0x044)
#define	UART1DMACR		__REG__(UART1_BASE_ADDRESS + 0x048)
#define	UART1PeriphID0		__REG__(UART1_BASE_ADDRESS + 0xfe0)
#define	UART1PeriphID1		__REG__(UART1_BASE_ADDRESS + 0xfe4)
#define	UART1PeriphID2		__REG__(UART1_BASE_ADDRESS + 0xfe8)
#define	UART1PeriphID3		__REG__(UART1_BASE_ADDRESS + 0xfec)
#define	UART1PCellID0		__REG__(UART1_BASE_ADDRESS + 0xff0)
#define	UART1PCellID1		__REG__(UART1_BASE_ADDRESS + 0xff4)
#define	UART1PCellID2		__REG__(UART1_BASE_ADDRESS + 0xff8)
#define	UART1PCellID3		__REG__(UART1_BASE_ADDRESS + 0xffc)

#define	NAND_BASE_ADDRESS	(0xF2010000)
#define	NDFMCR0			__REG__(NAND_BASE_ADDRESS + 0x0)
#define	NDFMCR1			__REG__(NAND_BASE_ADDRESS + 0x4)
#define	NDFMCR2			__REG__(NAND_BASE_ADDRESS + 0x8)
#define	NDFINTC			__REG__(NAND_BASE_ADDRESS + 0xc)
#define	NDFDTR			__REG__(NAND_BASE_ADDRESS + 0x10)
#define	NDECCRD0		__REG__(NAND_BASE_ADDRESS + 0x20)
#define	NDECCRD1		__REG__(NAND_BASE_ADDRESS + 0x24)
#define	NDECCRD2		__REG__(NAND_BASE_ADDRESS + 0x28)
#define	NDRSCA0			__REG__(NAND_BASE_ADDRESS + 0x30)
#define	NDRSCD0			__REG__(NAND_BASE_ADDRESS + 0x34)
#define	NDRSCA1			__REG__(NAND_BASE_ADDRESS + 0x38)
#define	NDRSCD1			__REG__(NAND_BASE_ADDRESS + 0x3C)
#define	NDRSCA2			__REG__(NAND_BASE_ADDRESS + 0x40)
#define	NDRSCD2			__REG__(NAND_BASE_ADDRESS + 0x44)
#define	NDRSCA3			__REG__(NAND_BASE_ADDRESS + 0x48)
#define	NDRSCD3			__REG__(NAND_BASE_ADDRESS + 0x4C)

#define	DMA_BASE_ADDRESS	(0xF4100000)
#define	DMACConfiguration	__REG__(DMA_BASE_ADDRESS + 0x30)
#define	DMACIntTCStatus		__REG__(DMA_BASE_ADDRESS + 0x4)
#define	DMACIntTCClear		__REG__(DMA_BASE_ADDRESS + 0x8)
#define	DMACC5SrcAddr		__REG__(DMA_BASE_ADDRESS + 0x1a0)
#define	DMACC5DestAddr		__REG__(DMA_BASE_ADDRESS + 0x1a4)
#define	DMACC5Control		__REG__(DMA_BASE_ADDRESS + 0x1ac)
#define	DMACC5Configuration	__REG__(DMA_BASE_ADDRESS + 0x1b0)

/* SDR_MPMC0 0xf430_1000*/
#define SMC_MPMC0_BASE          (0xf4301000)
#define	SMC_MEMC_STATUS_3       __REG__(SMC_MPMC0_BASE + 0x000)
#define	SMC_MEMIF_CFG_3         __REG__(SMC_MPMC0_BASE + 0x004)
#define	SMC_DIRECT_CMD_3        __REG__(SMC_MPMC0_BASE + 0x010)
#define	SMC_SET_CYCLES_3        __REG__(SMC_MPMC0_BASE + 0x014)
#define	SMC_SET_OPMODE_3        __REG__(SMC_MPMC0_BASE + 0x018)
#define	SMC_SRAM_CYCLES_0_3     __REG__(SMC_MPMC0_BASE + 0x100)
#define	SMC_SRAM_CYCLES_1_3     __REG__(SMC_MPMC0_BASE + 0x120)
#define	SMC_SRAM_CYCLES_2_3     __REG__(SMC_MPMC0_BASE + 0x140)
#define	SMC_SRAM_CYCLES_3_3     __REG__(SMC_MPMC0_BASE + 0x160)
#define	SMC_OPMODE0_0_3         __REG__(SMC_MPMC0_BASE + 0x104)
#define	SMC_OPMODE0_1_3         __REG__(SMC_MPMC0_BASE + 0x124)
#define	SMC_OPMODE0_2_3         __REG__(SMC_MPMC0_BASE + 0x144)
#define	SMC_OPMODE0_3_3         __REG__(SMC_MPMC0_BASE + 0x164)

/* SDR_MPMC1 0xf431_1000*/
#define SMC_MPMC1_BASE          (0xf4311000)
#define	SMC_MEMC_STATUS_5       __REG__(SMC_MPMC1_BASE + 0x000)
#define	SMC_MEMIF_CFG_5         __REG__(SMC_MPMC1_BASE + 0x004)
#define	SMC_DIRECT_CMD_5        __REG__(SMC_MPMC1_BASE + 0x010)
#define	SMC_SET_CYCLES_5        __REG__(SMC_MPMC1_BASE + 0x014)
#define	SMC_SET_OPMODE_5        __REG__(SMC_MPMC1_BASE + 0x018)
#define	SMC_SRAM_CYCLES_0_5     __REG__(SMC_MPMC1_BASE + 0x100)
#define	SMC_SRAM_CYCLES_1_5     __REG__(SMC_MPMC1_BASE + 0x120)
#define	SMC_SRAM_CYCLES_2_5     __REG__(SMC_MPMC1_BASE + 0x140)
#define	SMC_SRAM_CYCLES_3_5     __REG__(SMC_MPMC1_BASE + 0x160)
#define	SMC_OPMODE0_0_5         __REG__(SMC_MPMC1_BASE + 0x104)
#define	SMC_OPMODE0_1_5         __REG__(SMC_MPMC1_BASE + 0x124)
#define	SMC_OPMODE0_2_5         __REG__(SMC_MPMC1_BASE + 0x144)
#define	SMC_OPMODE0_3_5         __REG__(SMC_MPMC1_BASE + 0x164)

/* SMC Timeout Register */
#define	SMC_TIMEOUT             __REG__(0Xf00a0000 + 0x050)
    

/* I2S : 0xf2040000   */
#define I2S_BASE                0xF2040000
#define I2STCON                 __REG__(I2S_BASE + 0x000)
#define I2STSLVON               __REG__(I2S_BASE + 0x004)
#define I2STFCLR                __REG__(I2S_BASE + 0x008)
#define I2STMS                  __REG__(I2S_BASE + 0x00C)
#define I2STMCON                __REG__(I2S_BASE + 0x010)
#define I2STMSTP                __REG__(I2S_BASE + 0x014)
#define I2STDMA1                __REG__(I2S_BASE + 0x018)
#define I2SRCON                 __REG__(I2S_BASE + 0x020)
#define I2SRSLVON               __REG__(I2S_BASE + 0x024)
#define I2SFRFCLR               __REG__(I2S_BASE + 0x028)
#define I2SRMS                  __REG__(I2S_BASE + 0x02C)
#define I2SRMCON                __REG__(I2S_BASE + 0x030)
#define I2SRMSTP                __REG__(I2S_BASE + 0x034)
#define I2SRDMA1                __REG__(I2S_BASE + 0x038)
#define I2SCOMMON               __REG__(I2S_BASE + 0x044) 
#define I2STST                  __REG__(I2S_BASE + 0x048)
#define I2SRST                  __REG__(I2S_BASE + 0x04C)
#define I2SINT                  __REG__(I2S_BASE + 0x050)
#define I2SINTMSK               __REG__(I2S_BASE + 0x054)
#define I2STDAT                 __REG__(I2S_BASE + 0x1000)
#define I2SRDAT                 __REG__(I2S_BASE + 0x2000)
#define I2STDAT_ADR            (I2S_BASE + 0x1000)
#define I2SRDAT_ADR            (I2S_BASE + 0x2000)

/* USB OHCI */
#define USBHOST_BASE          	0xF4500000
#define USBHOST_OHCI_HcBCR0   	__REG__(USBHOST_BASE + 0x080)

/* LCDC */
#define LCDC_BASE		0xF4200000
#define LCD_TIMING_0            __REG__(LCDC_BASE + 0x000)
#define LCD_TIMING_1            __REG__(LCDC_BASE + 0x004)
#define LCD_TIMING_2            __REG__(LCDC_BASE + 0x008)
#define LCD_TIMING_3            __REG__(LCDC_BASE + 0x00C)
#define LCDUPBASE               __REG__(LCDC_BASE + 0x010)
#define LCDLPBASE               __REG__(LCDC_BASE + 0x014)
#define LCDIMSC                 __REG__(LCDC_BASE + 0x018)
#define LCD_CONTROL             __REG__(LCDC_BASE + 0x01C)
#define LCDRIS                  __REG__(LCDC_BASE + 0x020)
#define LCDMIS                  __REG__(LCDC_BASE + 0x024)
#define LCDICR                  __REG__(LCDC_BASE + 0x028)
#define LCDUPCURR               __REG__(LCDC_BASE + 0x02C)
#define LCDLPCURR               __REG__(LCDC_BASE + 0x030)
#define LCD_PALETTE(x)          __REG__(LCDC_BASE + 0x200 + x*4)

/* LCD Controller Option Function */
#define LCDCOP_BASE 0xf00b0000
#define LCDCOP_STN64CR             __REG__(LCDCOP_BASE + 0x000)
#define LCDCOP_STN64CR_G64_8bit    (1 << 1)

/* Power Management Controller */
#define	PMC_BASE_ADDRESS	(0xF0020000)
#define PMCDRV			__REG__(PMC_BASE_ADDRESS + 0x0260)
#define PMCCTL			__REG__(PMC_BASE_ADDRESS + 0x0300)
#define PMCWV1			__REG__(PMC_BASE_ADDRESS + 0x0400)
#define PMCRES			__REG__(PMC_BASE_ADDRESS + 0x041C)

#define PMCCTL_PMCPWE  (1 << 6)
#define PMCWV1_PMCCTLV (1 << 5)


#endif
