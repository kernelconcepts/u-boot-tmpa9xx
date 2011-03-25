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

#include <common.h>
#include <exports.h>
//#include "simple_io32.h"

struct pin_link
{
	uint32_t out_reg;
	uint32_t out_offset;
	char out_name;
	uint32_t in_reg;
	uint32_t in_offset;
	char in_name;
};

#define PORTA_BASE  			0xF0800000
#define PORTA_GPIODATA			(PORTA_BASE + 0x03fc)
/********/
#define PORTB_BASE  			0xF0801000
#define PORTB_GPIODATA			(PORTB_BASE + 0x03fc)
/********/
#define PORTC_BASE  			0xF0802000
#define PORTC_GPIODATA 			(PORTC_BASE + 0x03fc)
#define PORTC_GPIODIR 			(PORTC_BASE + 0x0400)
#define PORTC_GPIOFR1 			(PORTC_BASE + 0x0424)
#define PORTC_GPIOFR2 			(PORTC_BASE + 0x0428)
/********/
#define PORTD_BASE  			0xF0803000
#define PORTD_GPIODATA 			(PORTD_BASE + 0x03fc)
#define PORTD_GPIOFR1 			(PORTD_BASE + 0x0424)
#define PORTD_GPIOFR2 			(PORTD_BASE + 0x0428)
/********/
#define PORTF_BASE  			0xF0805000
#define PORTF_GPIODATA			(PORTF_BASE + 0x03fc)
#define PORTF_GPIODIR			(PORTF_BASE + 0x0400)
#define PORTF_GPIOFR1     		(PORTF_BASE + 0x0424)
/********/
#define PORTG_BASE  			0xF0806000
#define PORTG_GPIODATA 			(PORTG_BASE + 0x03fc)
#define PORTG_GPIODIR	 		(PORTG_BASE + 0x0400)
#define PORTG_GPIOFR1	 		(PORTG_BASE + 0x0424)
/********/
#define PORTL_BASE        		0xF080A000 
#define PORTL_GPIODATA    		(0xF080A000 + 0x03fc)
#define PORTL_GPIODIR     		(0xF080A000 + 0x0400)
#define PORTL_GPIOFR1     		(0xF080A000 + 0x0424)
#define PORTL_GPIOFR2     		(0xF080A000 + 0x0428)
/********/
#define PORTM_BASE        		0xF080B000
#define PORTM_GPIODATA    		(PORTM_BASE + 0x03fc)
#define PORTM_GPIODIR     		(PORTM_BASE + 0x0400)
#define PORTM_GPIOFR1     		(PORTM_BASE + 0x0424)
/********/
#define PORTT_BASE  			0xF080f000
#define PORTT_GPIODATA  		(PORTT_BASE + 0x03fc)
#define PORTT_GPIODIR   		(PORTT_BASE + 0x0400)
#define PORTT_GPIOFR1	  		(PORTT_BASE + 0x0424)


#define out32(__ofs,__val)  		{ (* ( (volatile uint32_t *) (__ofs)) ) = (uint32_t)  (__val); }
#define in32(__ofs)         		( * ( (volatile uint32_t *) (__ofs) ) )

struct pin_link pin_list[] = 
{
	// Port B -> Port A
	{ PORTB_GPIODATA, 0, 'B', PORTA_GPIODATA, 0, 'A'},
	{ PORTB_GPIODATA, 1, 'B', PORTA_GPIODATA, 1, 'A'},
	{ PORTB_GPIODATA, 2, 'B', PORTA_GPIODATA, 2, 'A'},
	{ PORTB_GPIODATA, 3, 'B', PORTA_GPIODATA, 3, 'A'},

	// Port G[0-3] -> Port D[4->7]
	{ PORTG_GPIODATA, 0, 'G', PORTD_GPIODATA, 4, 'D'},
	{ PORTG_GPIODATA, 1, 'G', PORTD_GPIODATA, 5, 'D'},
	{ PORTG_GPIODATA, 2, 'G', PORTD_GPIODATA, 6, 'D'},
	{ PORTG_GPIODATA, 3, 'G', PORTD_GPIODATA, 7, 'D'},

	// Port G[4] -> Port G[6]
	{ PORTG_GPIODATA, 4, 'G', PORTG_GPIODATA, 6, 'G'},

	// Port G[5] -> Port G[7]
	{ PORTG_GPIODATA, 5, 'G', PORTG_GPIODATA, 7, 'G'},

	// Port T[2-5] -> Port T[0-1]F[6-7]
	{ PORTT_GPIODATA, 5, 'T', PORTT_GPIODATA, 1, 'T'},
	{ PORTT_GPIODATA, 4, 'T', PORTT_GPIODATA, 0, 'T'},
	{ PORTT_GPIODATA, 3, 'T', PORTF_GPIODATA, 7, 'F'},
	{ PORTT_GPIODATA, 2, 'T', PORTF_GPIODATA, 6, 'F'},

	// Port C[2-4] -> Port D[1-3]
	//{ PORTC_GPIODATA, 2, 'C', PORTD_GPIODATA, 1, 'D'},
	{ PORTC_GPIODATA, 3, 'C', PORTD_GPIODATA, 2, 'D'},
	{ PORTC_GPIODATA, 4, 'C', PORTD_GPIODATA, 3, 'D'},

	// Port M[1-3] -> Port L[1-3]
	{ PORTM_GPIODATA, 1, 'M', PORTL_GPIODATA, 1, 'L'},
	{ PORTM_GPIODATA, 2, 'M', PORTL_GPIODATA, 2, 'L'},
	{ PORTM_GPIODATA, 3, 'M', PORTL_GPIODATA, 3, 'L'},

	// Port M[0] -> Port D[0]
	{ PORTM_GPIODATA, 0, 'M', PORTD_GPIODATA, 0, 'D'},

	// Port M[0] -> Port L[0]
	{ PORTM_GPIODATA, 0, 'M', PORTL_GPIODATA, 0, 'L'},

};

static int ping_test(void)
{
        int ret;

	/* PING TEST */
	
	run_command("setenv ipaddr   192.168.1.2",0);
	run_command("setenv serverip 192.168.1.1",0);

        ret=run_command("ping 192.168.1.1",0);
        if (ret==1)
	        printf("\nPing test successful\n");
	else
       	{
	        printf("\nPing test failed\n");
                return -1;
        }

	return 0;

}

static int display_test(void)
{
	int ret;

	/* DISPLAY TEST */
        
	run_command("setenv videoparams 'video=tmpa9xxfb:28050a74:0808290f:01df000b:00010828'",0);

        ret=run_command("nand read 0x41000000 splash 0x3000",0);
        if (ret==0)
       	{
	        printf("\nReading Testpicture from NAND failed\n");
                return -1;
        }
        
        ret=run_command("bmp display 0x41000000",0);

        if (ret==1)
	        printf("\nDisplay test successful\n");
	else
       	{
	        printf("\nDisplay test failed\n");
                return -1;
        }

	return 0;
}

static void _configure_gpio(void)
{
	// PB [0-3] as output
	// output only, no config reg

	// PA [0-3] as input
	// input only, no config reg

	// PG [0-3][4-5] as output | PG [6-7] as input
	out32(PORTG_GPIODIR, 0x3f);
	out32(PORTG_GPIOFR1, 0x00);

	// PD
	// input only, no config reg
	out32(PORTD_GPIOFR1, 0x00);
	out32(PORTD_GPIOFR2, 0x00);

	// PM [0-3] as output. only [0-3] valid
	out32(PORTM_GPIODIR, 0x0f);
	out32(PORTM_GPIOFR1, 0x00);


	// PL [0-3] as input. only [0-3] valid
	out32(PORTL_GPIODIR, 0x00);
	out32(PORTL_GPIOFR1, 0x00);
	out32(PORTL_GPIOFR2, 0x00);

	// PC [2-4] as output. only [2-4][6-7] valid
	printf("Configure port C PORTC_GPIODIR (0x%8x) <- 0x%x\n", PORTC_GPIODIR, 0x1c);
	out32(PORTC_GPIODIR, 0x1c);
	out32(PORTC_GPIOFR1, 0x00);
	out32(PORTC_GPIOFR2, 0xC0);

	// PT [2-5] as output.
	out32(PORTT_GPIODIR, 0x3c);
	out32(PORTT_GPIOFR1, 0x00);

	// PF [6-7] as input. only [6-7] valid
	out32(PORTF_GPIODIR, 0x00);
	out32(PORTF_GPIOFR1, 0x00);
}

static int gpio_test(void)
{
	int i;
	uint32_t input;
	uint32_t reg;
	uint32_t out_mask;
	uint32_t output;
	int tab_size;
	struct pin_link *pin_link;

printf("GPIO Test\n");

	_configure_gpio();

	tab_size = sizeof(pin_list) / sizeof(struct pin_link) ;

	for(i=0; i < tab_size; i++)
	{
		pin_link = &pin_list[i];
		
		printf("test P%c%d VS P%c%d\n",
			pin_link->out_name, pin_link->out_offset,
			pin_link->in_name,  pin_link->in_offset);

		out_mask = 1<<pin_link->out_offset;
		reg      = in32(pin_link->out_reg);

		// clear the bit
		output   = reg & ~out_mask;

		out32(pin_link->out_reg, output);

		reg   = in32(pin_link->in_reg);
		input = (reg >> pin_link->in_offset) & 0x1;
		if (input)
		{
			printf("in reg 0x%x | out 0x%x\n", in32(pin_link->in_reg), output );
			printf("P%c%d 0 != P%c%d %d\n",
				pin_link->out_name, pin_link->out_offset,
				pin_link->in_name, pin_link->in_offset, input);

			return -1;
		}

		reg      = in32(pin_link->out_reg);

		// set the bit
		output   = reg | out_mask;

		out32(pin_link->out_reg, output);

		reg = in32(pin_link->in_reg);
		input = (reg >> pin_link->in_offset) & 0x1;
		if (input != 1)
		{
			printf("P%c%d 0 != P%c%d %d\n",
				pin_link->out_name, pin_link->out_offset,
				pin_link->in_name, pin_link->in_offset, input);

			return -1;
		}
	}

	return 0;
}


int hw_test (void)
{
        int ret;

	printf ("Tonga2 HW test routine\n");

	ret=ping_test();
        if (ret)
        	return -1;

	ret=display_test();
        if (ret)
        	return -1;

	ret=gpio_test();
        if (ret)
        	return -1;

	printf("All tests successful!!\n\n");


	printf ("Hit any key to exit ... \n");

	while (!tstc());
	(void) getc();

	do_reset();                      
	return (0);
}



