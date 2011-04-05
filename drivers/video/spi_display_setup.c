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

#define MAX_SEQUENCE       30
#define MAX_VALUE          10

#include <spi.h>
#include <config.h>

#ifdef LG4573_480x800_RGB262k_Mode
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

#endif /* LG4573_480x800_RGB262k_Mode */

static struct spi_slave *slave;

int setup_spi_display(void)
{
        int i=0;
	int ret=0;

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
        	unsigned char transfer[2];
                unsigned char dummy[2];
                int j=1;
                
        	transfer[0]=0x70;
                transfer[1]=display_setup[i][0] & 0xff;
                
                ret |= spi_xfer(slave, 16, transfer, dummy, SPI_XFER_BEGIN | SPI_XFER_END);

                while (display_setup[i][j]!=-1)
                {
               		transfer[0]=0x72;
                	transfer[1]=display_setup[i][j] & 0xff;
	                ret |= spi_xfer(slave, 16, transfer, dummy, SPI_XFER_BEGIN | SPI_XFER_END);
                        j++;
                }
                i++;
        }
        
	spi_release_bus(slave);

	return ret;
}
