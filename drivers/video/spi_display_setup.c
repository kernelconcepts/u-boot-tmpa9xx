/*
 * (C) Copyright 2011
 * Thomas Haase (Thomas.Haase@web.de)
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

#define CONFIG_SPI_BUS     1
#define CONFIG_SPI_CS      0
#define MAX_HERTZ          200000000
#define SPI_MODE           SPI_MODE_3

#define MAX_SEQUENCE       50
#define MAX_VALUE          30

#include <spi.h>
#include <config.h>
#include <common.h>
#include <asm/arch/tmpa9xx.h>
#include <spi.h>

int bus;

#ifdef DISPLAY_32WVF0HZ1
#define TRANSMIT_BITS 16
static int display_setup[MAX_SEQUENCE][MAX_VALUE] =
	{ {0x20,                                              -1},
          {0x3a,0x70,                                         -1},
          {0xb1,0x06,0x1e,0x0c,                               -1},
          {0xb2,0x10,0xc8,                                    -1},
          {0xb3,0x00,                                         -1},
          {0xb4,0x04,                                         -1},
          {0xb5,0x01,0x20,0x20,0x00,0x00,                     -1},
          {0xb6,0x01,0x18,0x02,0x40,0x10,0x40,                -1},
          {0xc3,0x02,0x04,0x03,0x03,0x03,                     -1},
          {0xc4,0x12,0x22,0x10,0x0c,0x03,0x6e,                -1},
          {0xc5,0x76,                                         -1},
          {0xf9,0x40,                                         -1},
          {0xc6,0x27,0x50,                                    -1},
          {0xd0,0x00,0x44,0x44,0x16,0x09,0x03,0x61,0x16,0x03, -1},
          {0xd1,0x00,0x44,0x44,0x16,0x09,0x03,0x61,0x16,0x03, -1},
          {0xd2,0x00,0x44,0x44,0x16,0x09,0x03,0x61,0x16,0x03, -1},
          {0xd3,0x00,0x44,0x44,0x16,0x09,0x03,0x61,0x16,0x03, -1},
          {0xd4,0x00,0x44,0x44,0x16,0x09,0x03,0x61,0x16,0x03, -1},
          {0xd5,0x00,0x44,0x44,0x16,0x09,0x03,0x61,0x16,0x03, -1},
          {0x29,                                              -1},
          {0x11,                                              -1},
          {  -1                                                 },
        };

#endif /* 32WVFOH */

#ifdef DISPLAY_35WVF0HZ2
#define TRANSMIT_BITS 16
static int display_setup[MAX_SEQUENCE][MAX_VALUE] =
	{ {0x20,                                              -1},
          {0x3a,0x70,                                         -1},
          {0xb1,0x06,0x1e,0x0c,                               -1},
          {0xb2,0x10,0xc8,                                    -1},
          {0xb3,0x00,                                         -1},
          {0xb4,0x04,                                         -1},
          {0xb5,0x01,0x20,0x20,0x00,0x00,                     -1},
          {0xb6,0x01,0x18,0x02,0x40,0x10,0x40,                -1},
          {0xc3,0x02,0x04,0x03,0x03,0x03,                     -1},
          {0xc4,0x12,0x22,0x10,0x0c,0x03,0x6e,                -1},
          {0xc5,0x76,                                         -1},
          {0xf9,0x40,                                         -1},
          {0xc6,0x27,0x50,                                    -1},
          {0xd0,0x00,0x44,0x44,0x16,0x00,0x03,0x61,0x16,0x03, -1},
          {0xd1,0x00,0x44,0x44,0x16,0x00,0x03,0x61,0x16,0x03, -1},
          {0xd2,0x00,0x44,0x44,0x16,0x00,0x03,0x61,0x16,0x03, -1},
          {0xd3,0x00,0x44,0x44,0x16,0x00,0x03,0x61,0x16,0x03, -1},
          {0xd4,0x00,0x44,0x44,0x16,0x00,0x03,0x61,0x16,0x03, -1},
          {0xd5,0x00,0x44,0x44,0x16,0x00,0x03,0x61,0x16,0x03, -1},
          {0x29,                                              -1},
          {0x11,                                              -1},
          {  -1                                                 },
        };

#endif /* DISPLAY_35WVF0HZ2 */

#ifdef DISPLAY_37WVF0HZ1
#define TRANSMIT_BITS 16
static int display_setup[MAX_SEQUENCE][MAX_VALUE] =
	{ {0x20,                                              -1},
          {0x3a,0x70,                                         -1},
          {0xb1,0x06,0x1e,0x0c,                               -1},
          {0xb2,0x10,0xc8,                                    -1},
          {0xb3,0x00,                                         -1},
          {0xb4,0x04,                                         -1},
          {0xb5,0x01,0x20,0x20,0x00,0x00,                     -1},
          {0xb6,0x01,0x18,0x02,0x40,0x10,0x40,                -1},
          {0xc3,0x02,0x04,0x03,0x03,0x03,                     -1},
          {0xc4,0x12,0x22,0x10,0x0c,0x03,0x6e,                -1},
          {0xc5,0x76,                                         -1},
          {0xf9,0x40,                                         -1},
          {0xc6,0x27,0x50,                                    -1},
          {0xd0,0x10,0x72,0x44,0x10,0x06,0x03,0x60,0x16,0x02, -1},
          {0xd1,0x10,0x72,0x44,0x10,0x06,0x03,0x60,0x16,0x02, -1},
          {0xd2,0x10,0x72,0x44,0x10,0x06,0x03,0x60,0x16,0x02, -1},
          {0xd3,0x10,0x72,0x44,0x10,0x06,0x03,0x60,0x16,0x02, -1},
          {0xd4,0x10,0x72,0x44,0x10,0x06,0x03,0x60,0x16,0x02, -1},
          {0xd5,0x10,0x72,0x44,0x10,0x06,0x03,0x60,0x16,0x02, -1},
          {0x29,                                              -1},
          {0x11,                                              -1},
          {  -1                                                 },
        };

#endif /* DISPLAY_37WVF0HZ1 */

#ifdef DISPLAY_43WVF3AZ0
#define TRANSMIT_BITS 16
static int display_setup[MAX_SEQUENCE][MAX_VALUE] =
	{ {0x20,                                              -1},
          {0x3a,0x70,                                         -1},
          {0xb1,0x06,0x1e,0x0c,                               -1},
          {0xb2,0x10,0xc8,                                    -1},
          {0xb3,0x00,                                         -1},
          {0xb4,0x04,                                         -1},
          {0xb5,0x01,0x20,0x20,0x00,0x00,                     -1},
          {0xb6,0x01,0x18,0x02,0x40,0x10,0x40,                -1},
          {0xc3,0x02,0x04,0x03,0x03,0x03,                     -1},
          {0xc4,0x12,0x22,0x10,0x0c,0x03,0x6e,                -1},
          {0xc5,0x76,                                         -1},
          {0xf9,0x40,                                         -1},
          {0xc6,0x27,0x50,                                    -1},
          {0xd0,0x01,0x17,0x73,0x02,0x00,0x00,0x00,0x00,0x02, -1},
          {0xd1,0x01,0x17,0x73,0x02,0x00,0x00,0x00,0x00,0x02, -1},
          {0xd2,0x01,0x17,0x73,0x02,0x00,0x00,0x00,0x00,0x02, -1},
          {0xd3,0x01,0x17,0x73,0x02,0x00,0x00,0x00,0x00,0x02, -1},
          {0xd4,0x01,0x17,0x73,0x02,0x00,0x00,0x00,0x00,0x02, -1},
          {0xd5,0x01,0x17,0x73,0x02,0x00,0x00,0x00,0x00,0x02, -1},
          {0x29,                                              -1},
          {0x11,                                              -1},
          {  -1                                                 },
        };

#endif /* DISPLAY_43WVF3AZ0 */


#ifdef DISPLAY_35HVF0H
#define TRANSMIT_BITS 8
static int display_setup[MAX_SEQUENCE][MAX_VALUE] =
	{ {0xB0,0x00,                                                                                                -1},
          {0xC0,0x03,0x3B,0x00,0x03,0x04,0x01,0x00,0xBA,                                                             -1},
          {0xC1,0x00,0x11,0x04,0x0A,                                                                                 -1},
          {0xC4,0x11,0x01,0x53,0x05,                                                                                 -1},
          {0xC6,0x02,                                                                                                -1},
          {0xC8,0x09,0x0A,0x0B,0x58,0x05,0x05,0x04,0x08,0x00,0x32,0x08,0x0C,0x11,0xE9,0x0F,0x0C,0x13,0x05,0x32,0x11, -1},
          {0x2A,0x00,0x00,0x01,0x3F,                                                                                 -1},
          {0x2B,0x00,0x00,0x01,0xDF,                                                                                 -1},
          {0x34,                                                                                                     -1},
          {0x3A,0x66,                                                                                                -1},
          {0xB3,0x02,0x00,0x00,0x00,                                                                                 -1},
          {0xB4,0x00,                                                                                                -1},
          {0x11,                                                                                                     -1},
          {  -2, 120,                                                                                                -1},
          {0xD0,0x07,0x07,0x19,0x66,                                                                                 -1},
          {0xD1,0x00,0x26,0x0E,                                                                                      -1},
          {0xD2,0x03,0x24,0x00,                                                                                      -1},
          {0xB0,0x00,                                                                                                -1},
          {0xB3,0x02,0x00,0x00,0x00,                                                                                 -1},
          {0x36,0x00,                                                                                                -1},
          {0x2A,0x00,0x00,0x01,0x3F,                                                                                 -1},
          {0x2B,0x00,0x00,0x01,0xDF,                                                                                 -1},
          {0xB4,0x11,                                                                                                -1},
          {  -2,   5,                                                                                                -1},
          {0x2C,                                                                                                     -1},
          {  -2,   5,                                                                                                -1},
          {0x29,                                                                                                     -1},
          {0x2C,                                                                                                     -1},
          {  -1                                                                                                        },
        };

#endif /* DISPLAY_35HVF0H */

#ifdef DISPLAY_30WQF0H
#define TRANSMIT_BITS 24
static int display_setup[MAX_SEQUENCE][MAX_VALUE] =
	{ {0x0000,        -1},
          {0x0000,        -1},
          {0x0000,        -1},
          {0x0000,        -1},
          {0x0400,0x6200, -1},
          {0x0401,0x0000, -1},
          {0x0404,0x0000, -1},
          {0x0008,0x0404, -1},
          {0x0300,0x0700, -1},
          {0x0301,0x570C, -1},
          {0x0302,0x0C05, -1},
          {0x0303,0x0405, -1},
          {0x0304,0x0123, -1},
          {0x0305,0x0504, -1},
          {0x0306,0x550C, -1},
          {0x0307,0x0C07, -1},
          {0x0308,0x0007, -1},
          {0x0309,0x3210, -1},
          {0x0020,0x0015, -1},
          {0x0021,0x0501, -1},
          {0x0022,0x0403, -1},
          {0x0023,0x0002, -1},
          {0x000B,0x0000, -1},
          {0x000C,0x0110, -1},
          {0x000F,0x0002, -1},
          {0x0100,0x0610, -1},
          {0x0101,0x0247, -1},
          {0x0102,0xD180, -1},
          {0x0103,0x0F00, -1},
          {0x0001,0x0100, -1},
          {0x0002,0x0100, -1},
          {0x0003,0x1030, -1},
          {0x0007,0x0000, -1},
          {0x0210,0x0000, -1},
          {0x0211,0x00EF, -1},
          {0x0212,0x0000, -1},
          {0x0213,0x018F, -1},
          {0x0102,0xD1B0, -1},
          {    -2,   150, -1}, 
          {0x0014,0x0076, -1},
          {0x0200,0x0000, -1},
          {0x0201,0x0000, -1},
          {0x0202,        -1},       
          {0x0007,0x0100, -1},
          {    -2,   150, -1},
          {0x0202,        -1},       
          {    -2,   150, -1},
          {    -1,          },
        };

#endif /* DISPLAY_30WQF0H */

#ifdef DISPLAY_NEC_NL2432HC22
#define TRANSMIT_BITS 32
static int display_setup[MAX_SEQUENCE][MAX_VALUE] =
	{ 
          {0x03, 0x01, -1},
          {0x01, 0x00, -1},
          {0x64, 0x0F, -1},
          {0x65, 0x3F, -1},
          {0x66, 0x3F, -1},
          {0x67, 0x00, -1},
          {0x68, 0x00, -1},
          {0x69, 0x30, -1},
          {0x6A, 0x04, -1},
          {0x6B, 0x37, -1},
          {0x6C, 0x17, -1},
          {0x6D, 0x00, -1},
          {0x6E, 0x40, -1},
          {0x6F, 0x30, -1},
          {0x70, 0x04, -1},
          {0x71, 0x37, -1},
          {0x72, 0x17, -1},
          {0x73, 0x00, -1},
          {0x74, 0x40, -1},
          {0x02, 0x40, -1},
          {0x4B, 0x04, -1},
          {0x4C, 0x01, -1},
          {0x4D, 0x01, -1},
          {0x50, 0x00, -1},
          {0x51, 0x00, -1},
          {0x52, 0x2E, -1},
          {0x53, 0xC4, -1},
          {0x56, 0x15, -1},
          {0x57, 0xED, -1},
          {0x5F, 0x3F, -1},
          {0x60, 0x22, -1},
          {0x19, 0x76, -1},
          {0x1A, 0x54, -1},
          {0x1B, 0x67, -1},
          {0x1C, 0x60, -1},
          {0x1D, 0x04, -1},
          {0x1E, 0x1C, -1},
          {0x1F, 0xA9, -1},
          {0x20, 0x00, -1},
          {0x21, 0x20, -1},
          {0x18, 0x77, -1},
          {  -2,20000, -1},
          {0x00, 0x00, -1},
          {  -1,         },
        };

#endif /* DISPLAY_NEC_NL2432HC22 */

static void display_reset(int reset)
{
	if (reset==1)			/* Signal is high active */
	        GPIOCDATA |=  (1<<4);
	else                
	        GPIOCDATA &= ~(1<<4);

}

static struct spi_slave *slave;

/*
 * The following are used to control the SPI chip selects for the SPI command.
 */

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
	        GPIOTDIR = 0x07;
        	GPIOTFR1 = 0x00;
	        GPIOTFR2 = 0x00;
		break;
	case 1:
		GPIOLDATA= 0x00;
		GPIOLDIR = 0x03;
        	GPIOLFR1 = 0x00;
        	GPIOLFR2 = 0x00;
                break;
	default:
            	printf("Invalid bus selected for activate\n");
                break;
	}

	udelay(1);
        GPIOCDATA |= (1<<2);

}

void spi_cs_deactivate(struct spi_slave *slave)
{
	bus=slave->bus;

        GPIOCDATA &= ~(1<<2);
        udelay(1);

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
			GPIOLDATA |=  (1<<0);
		else
			GPIOLDATA &= ~(1<<0);
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
        
		if(!bit)
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

#ifndef DISPLAY_35HVF0H
int setup_spi_display(void)
{
        int i=0;
	int ret=0;
        
        /* Setup port for RESET & CS of Display */
	GPIOCDATA=0x10;
        GPIOCDIR =0x14;
        GPIOCFR1 =0x00;
        GPIOCFR2 =0x00;

        display_reset(1);	/* Put display to reset */
        udelay(10*1000);	/* Wait 10ms */
        display_reset(0);	/* release reset */
        udelay(20*1000);	/* Wait 20ms */


	if (!slave) {
		/* FIXME: Verify the max SCK rate */
		slave = spi_setup_slave(CONFIG_SPI_BUS,CONFIG_SPI_CS,MAX_HERTZ,SPI_MODE);
		if (!slave)
			return -1;
	}

	if (spi_claim_bus(slave))
		return -1;


	while (display_setup[i][0]!=-1)
        {
        
        	unsigned char transfer[4];
                unsigned char dummy[4];
                int j=1;

                if (display_setup[i][0]==-2)
                {
               		udelay(1000*display_setup[i][j]);
		}
                else
		{
	                memset (transfer,0,4);
        	        memset (dummy,0,4);
	        	transfer[0]=0x70;
			if (TRANSMIT_BITS==16)
		                transfer[1]=display_setup[i][0] & 0xff;
			else if (TRANSMIT_BITS==24)
        	        {
		                transfer[1]=(display_setup[i][0] >> 8) & 0xff;
		                transfer[2]=(display_setup[i][0] >> 0) & 0xff;
			}
                        else // TRANSMIT_BITS==32
                        {
		        	transfer[0]=0x00;
		                transfer[1]=display_setup[i][0] & 0xff;
		        	transfer[2]=0x01;
		                transfer[3]=display_setup[i][1] & 0xff;
                                j=2;
                        }
	                ret |= spi_xfer(slave, TRANSMIT_BITS, transfer, dummy, SPI_XFER_BEGIN | SPI_XFER_END);

	                while (display_setup[i][j]!=-1)
	                {
	               		transfer[0]=0x72;
				if (TRANSMIT_BITS==16)
			                transfer[1]=display_setup[i][j] & 0xff;
				else
		                {
			                transfer[1]=(display_setup[i][j] >> 8) & 0xff;
        			        transfer[2]=(display_setup[i][j] >> 0) & 0xff;
				}                                                        
		                ret |= spi_xfer(slave, TRANSMIT_BITS, transfer, dummy, SPI_XFER_BEGIN | SPI_XFER_END);
       		                j++;
	                }
	        }
       	        i++;
	}
        
	spi_release_bus(slave);

	return ret;
}
#else
int setup_spi_display(void)
{
        int i=0;
	int ret=0;
        
        /* Setup port for RESET & CS of Display */
	GPIOCDATA=0x10;
        GPIOCDIR =0x14;
        GPIOCFR1 =0x00;
        GPIOCFR2 =0x00;

        display_reset(1);	/* Put display to reset */
        udelay(10*1000);	/* Wait 10ms */
        display_reset(0);	/* release reset */
        udelay(20*1000);	/* Wait 20ms */


	if (!slave) {
		/* FIXME: Verify the max SCK rate */
		slave = spi_setup_slave(CONFIG_SPI_BUS,CONFIG_SPI_CS,MAX_HERTZ,SPI_MODE);
		if (!slave)
			return -1;
	}

	if (spi_claim_bus(slave))
		return -1;


	while (display_setup[i][0]!=-1)
        {
        
        	unsigned char transfer[4];
                unsigned char dummy[4];
                int j=1;

                if (display_setup[i][0]==-2)
                {
               		udelay(1000*display_setup[i][j]);
		}
                else
		{
	                memset (transfer,0,4);
        	        memset (dummy,0,4);
                
	                transfer[0]=display_setup[i][0] & 0xff;
			GPIOLDIR = 0x03;
                        GPIOLDATA= 0x02;
                        udelay(10);
		        GPIOCDATA |= (1<<2);
                        udelay(10);
                        GPIOLDATA= 0x00;
                        udelay(10);
                        GPIOLDATA= 0x02;

	                ret |= spi_xfer(slave, 8, transfer, dummy, SPI_XFER_BEGIN | SPI_XFER_END);

	                while (display_setup[i][j]!=-1)
	                {
				GPIOLDIR = 0x03;
        	                GPIOLDATA= 0x03;
                	        udelay(10);
		        	GPIOCDATA |= (1<<2);
	                        udelay(10);
        	                GPIOLDATA= 0x01;
                	        udelay(10);
                        	GPIOLDATA= 0x03;

		                transfer[0]=display_setup[i][j] & 0xff;
		                ret |= spi_xfer(slave, 8, transfer, dummy, SPI_XFER_BEGIN | SPI_XFER_END);
       		                j++;
	                }
	        }
       	        i++;
	}
        
	spi_release_bus(slave);

	return ret;
}
#endif //

