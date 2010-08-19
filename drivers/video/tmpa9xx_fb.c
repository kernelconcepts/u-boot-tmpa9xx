/*
 * (C) Copyright 2009,2010
 * Kernel Concepts <www.kernelconcepts.de>
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
 
#include <stdarg.h>
#include <malloc.h>
#include <bmp_layout.h>
#include <common.h>
#include <stdio_dev.h>
#include <asm/arch/tmpa9xx.h>

static unsigned char *framebuffer;

#ifdef TONGA2
static unsigned long videoparams[4]={0x19211e4c,0x10040cef,0x013f380d,0x00010828};
static unsigned int display_bpp=16;
static unsigned int display_width=640;
static unsigned int display_height=480;
#else
static unsigned long videoparams[4]={0x0707074c,0x020204ef,0x013f200e,0x0001082A};
static unsigned int display_bpp=32;
static unsigned int display_width=320;
static unsigned int display_height=240;
#endif

static unsigned int hatoi(unsigned char *p)
{
	unsigned int res = 0;

	while (1) {
		switch (*p) {
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			res |= (*p - 'a' + 10);
			break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			res |= (*p - 'A' + 10);
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			res |= (*p - '0');
			break;
		default:
		return res;
		}
		p++;
		if (*p == 0) {
			break;
		}
		res <<= 4;
	}

	return res;
}

int drv_video_init(void)
{
	int error;
	struct stdio_dev videodev;
	char *params;
        char *paramererpos=NULL;
	unsigned long fbmem_size;
        
	params = getenv ("videoparams");
	paramererpos=strstr(params,"tmpa9xxfb");
        
        if (paramererpos)
        {
        	unsigned char i;
	        unsigned char nr_params;
                unsigned char parameter[10];
                
        	nr_params=(strlen(paramererpos)-9)/9;
                for (i=0;i<nr_params;i++)
                {
                	memset(parameter,0,sizeof(parameter));
                        memcpy(parameter,(unsigned char*) ( (unsigned long)paramererpos+9*(i+1)+1),8);
                        videoparams[i]=hatoi(parameter);
                }
                display_width = (((videoparams[0]>>2)&0x3f)+1)*16;
                display_height=	(videoparams[1]&0x3ff)+1;
                display_bpp   = 1<<((videoparams[3]>>1)&0x07);
        }
        else
        	printf("No videoparams environment variable found - using default\n");
        
        fbmem_size = display_width * display_height * display_bpp / 8;
	framebuffer = malloc(fbmem_size);
	if (framebuffer == NULL) {
		printf("Failed to alloc FB memory\n");
		return -1;
	}

	LCD_TIMING_0 = videoparams[0];
	LCD_TIMING_1 = videoparams[1];
	LCD_TIMING_2 = videoparams[2];
	LCD_TIMING_3 = 0;
	LCDUPBASE    = (unsigned long)framebuffer;
	LCDLPBASE    = 0;
	LCDIMSC	     = 0;
	LCD_CONTROL  = videoparams[3];

	memset(framebuffer,0x00, fbmem_size);

#ifdef TONGA2
	/* Configure Pins and reset LCD */
        GPIOMDIR=3;
        GPIOMFR1=0;

	/* Reset */
	GPIOMDATA=0;
        
    	udelay(1000);
	GPIOMDATA|=(1<<0);

	/* Enable */
	GPIOMDATA|=(1<<1);

	/* Light */
	GPIOCDATA=0;
#endif

	if (display_bpp==16)
        {
		LCDCOP_STN64CR |= LCDCOP_STN64CR_G64_8bit;
		GPIOJFR2 = 0x00;
		GPIOJFR1 = 0xFF;
		GPIOKFR2 = 0x00;
		GPIOKFR1 = 0xFF;
		PMCCTL &= ~PMCCTL_PMCPWE;
		PMCWV1 |= PMCWV1_PMCCTLV;
		udelay(200);
        }

	memset(&videodev, 0, sizeof(videodev));
	strcpy(videodev.name, "video");
	videodev.ext = DEV_EXT_VIDEO;		/* Video extensions */
	videodev.flags = DEV_FLAGS_SYSTEM;	/* No Output */
	videodev.putc = NULL;			/* 'putc' function */
	videodev.puts = NULL;			/* 'puts' function */

	error = stdio_register(&videodev);

	return (error == 0) ? 1 : error;
}


int video_display_bitmap (ulong bmp_image, int x, int y)
{
	bmp_image_t *bmp = (bmp_image_t *) bmp_image;
	uchar *bmap;
	unsigned int bmp_width, bmp_height, bmp_bpp;
	unsigned long compression;
	unsigned char *dest;
        int i,j;
#ifdef CONFIG_VIDEO_BMP_GZIP
	unsigned char *dst = NULL;
	ulong len;
#endif

	if (!((bmp->header.signature[0] == 'B') &&
	      (bmp->header.signature[1] == 'M'))) {

#ifdef CONFIG_VIDEO_BMP_GZIP
		/*
		 * Could be a gzipped bmp image, try to decrompress...
		 */
		len = CONFIG_SYS_VIDEO_LOGO_MAX_SIZE;
		dst = malloc(CONFIG_SYS_VIDEO_LOGO_MAX_SIZE);
		if (dst == NULL) {
			printf("Error: malloc in gunzip failed!\n");
			return(1);
		}
		if (gunzip(dst, CONFIG_SYS_VIDEO_LOGO_MAX_SIZE, (uchar *)bmp_image, &len) != 0) {
			printf ("Error: no valid bmp or bmp.gz image at %lx\n", bmp_image);
			free(dst);
			return 1;
		}
		if (len == CONFIG_SYS_VIDEO_LOGO_MAX_SIZE) {
			printf("Image could be truncated (increase CONFIG_SYS_VIDEO_LOGO_MAX_SIZE)!\n");
		}

		/*
		 * Set addr to decompressed image
		 */
		bmp = (bmp_image_t *)dst;

		if (!((bmp->header.signature[0] == 'B') &&
		      (bmp->header.signature[1] == 'M'))) {
			printf ("Error: no valid bmp.gz image at %lx\n", bmp_image);
			free(dst);
			return 1;
		}
#else
		printf ("Error: no valid bmp image at %lx\n", bmp_image);
		return 1;
#endif /* CONFIG_VIDEO_BMP_GZIP */
	}

	bmp_width   = le32_to_cpu (bmp->header.width);
	bmp_height  = le32_to_cpu (bmp->header.height);
	bmp_bpp     = le16_to_cpu (bmp->header.bit_count);
	compression = le32_to_cpu (bmp->header.compression);

	if ((compression != BMP_BI_RGB) && (compression != BMP_BI_BITFIELDS))
         {
		printf ("Error: compression type %ld not supported\n",compression);
#ifdef CONFIG_VIDEO_BMP_GZIP
		if (dst)
			free(dst);
#endif
		return 1;
	}

	if ((bmp_width != display_width) || 
            (bmp_height!= display_height) ||
            (bmp_bpp   != display_bpp))
        {
        	printf("Splash Scrren doesn't fit to display!\n");
                printf("Display: Width:%u Height: %u BPP: %u\n",display_width,display_height,display_bpp);
                printf("Splash:  Width:%u Height: %u BPP: %u\n",bmp_width,bmp_height,bmp_bpp);
        }

	bmap = (uchar *) bmp + le32_to_cpu (bmp->header.data_offset);
        dest = framebuffer;

	for(i=display_height-1;i>=0;i--)
        {
	        switch(display_bpp)
        	{
	        case 32:
        		for(j=0;j<display_width*display_bpp/8;j+=4)
	                {
                        	unsigned long pos=(unsigned long)bmap+i*display_width*display_bpp/8+j;
                        
        	        	*dest++=*(unsigned char*)(pos+2);
                		*dest++=*(unsigned char*)(pos+1);
                		*dest++=*(unsigned char*)(pos+0);
	                	*dest++=0;
                	}
	                break;
        	case 16:
        		for(j=0;j<display_width*display_bpp/8;j+=2)
	                {
                        	unsigned long pos=(unsigned long)bmap+i*display_width*display_bpp/8+j;
                                unsigned int val = (((unsigned int)(*(unsigned char*)(pos+0)))<<0)
					         | (((unsigned int)(*(unsigned char*)(pos+1)))<<8);
                                
                                
				unsigned int pixel = (( val & 0xf800 ) >> 11 )    // red from msb to lsb
                                                   |  ( val & 0x07e0 )            // keep green at current bit pos
                                                   | (( val & 0x001f ) << 11 );   // blue from lsb to msb
                                                   
                                *dest++=(unsigned char)(pixel&0xff);
                        	*dest++=(unsigned char)(pixel>>8);
                        
                	}
	                break;
        	default:
        		printf("Not supported\n");
                }
        }

#ifdef CONFIG_VIDEO_BMP_GZIP
	if (dst) {
		free(dst);
	}
#endif

	LCD_CONTROL |=1; /* Switch on display */

	return (0);
}
