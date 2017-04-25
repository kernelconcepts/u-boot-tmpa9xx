/*
 * (C) Copyright 2009, 2010, 2017
 * Kernel Concepts <www.kernelconcepts.de>
 * Florian Boor (florian.boor@kernelconcepts.de)
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
#include <nand.h>
#include <asm/arch/tmpa9xx.h>
#include <asm/arch/tmpa9xx_nand.h>

#define ECCBUF_SIZE 256
#define DMA_TIMEOUT  0xFFFF
#define NAND_DMA_TRANSFER_SIZE 512

#define MAX_WAIT_FOR_HW		100000

/* id marker for multiple acess methods for single flash */
static int id = 0;

struct tmpa9xx_nand_timing {
	unsigned int splw;
	unsigned int sphw;
	unsigned int splr;
	unsigned int sphr;
};

struct tmpa9xx_nand_private {
	unsigned int dma;
	unsigned int mlc;
        unsigned int page_size;
        unsigned int spare_size;
	unsigned int column;
	unsigned int page_addr;
        unsigned int rndout;
	struct tmpa9xx_nand_timing timing;
	unsigned int needs_onchip_ecc;
};

static unsigned int onchip_ecc_enabled = 0;

/* Nand Control */
struct tmpa9xx_nand_private tmpa9xx_nand[2] = {
	{	
		.dma	= 1,
		.timing =
		{
			.splw = 0x2,
			.sphw = 0x2,
			.splr = 0x2,
			.sphr = 0x2,
		},
		.needs_onchip_ecc = 0,
	},
	{	
		.dma	= 1,
		.timing =
		{
			.splw = 0x2,
			.sphw = 0x2,
			.splr = 0x2,
			.sphr = 0x2,
		},
		.needs_onchip_ecc = 0,
	},
};

static struct nand_ecclayout nand_oob_hamming_512 = {
	.eccbytes = 6,
	.eccpos =  {8, 9, 10, 13, 14, 15},
	.oobfree = { {2, 6} }
};

static struct nand_ecclayout nand_oob_hamming_2048 = {
	.eccbytes = 24,
	.eccpos   =  { 8,  9, 10, 13, 14, 15, 24, 25,
                      26, 29, 30, 31, 40, 41, 42, 45,
                      46, 47, 56, 57, 58, 61, 62, 63},
	.oobfree  = {{ 2, 6},
                     {16, 8},
                     {32, 8},
                     {48, 8},
                     {11, 2},
                     {27, 2},
                     {43, 2},
                     {59, 2}}
};

static struct nand_ecclayout nand_oob_rs_2048 = {
	.eccbytes = 40,
	.eccpos   =  { 6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
                      22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                      38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
                      54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
	.oobfree  = {{ 2, 4},
                     {16, 6},
                     {32, 6},
                     {48, 6}}
};

void tmpa9xx_nand_dma_read(unsigned char *buf, unsigned short size)
{
	DMACConfiguration = 0x00000001;		// DMAC Enable ON
	DMACC5SrcAddr=(unsigned int)&NDFDTR;	// Source address
	DMACC5DestAddr=(unsigned int)buf;	// Destination address
	DMACC5Control = 0x88489000+(size/4);
	DMACC5Configuration=0x00009009; 	//FlowCntrl:peripheral to Memory,ITC=1;DMA=EN
}

void tmpa9xx_nand_dma_write(const unsigned char *buf, unsigned short size)
{
	DMACConfiguration = 0x00000001;		// DMAC Enable ON
	DMACC5SrcAddr=(unsigned int)buf;	// Source address
	DMACC5DestAddr=(unsigned int)&NDFDTR;	// Destination address
	DMACC5Control = 0x84489000+(size/4);
	DMACC5Configuration=0x00008901;		//FlowCntrl:Memory to peripheral,ITC=1;DMA=EN
}

static int tmpa9xx_nand_wait_dma_complete(unsigned int timeout)
{
	unsigned int val, count = 0;
	do {
		val = DMACIntTCStatus;
		count++;
	}while (((val & NAND_DMAC_STATUS) != NAND_DMAC_STATUS) && count < timeout);	// NDRB=READY wait

	if (count == timeout && ((val & NAND_DMAC_STATUS) != NAND_DMAC_STATUS))
		return -1;
		
	val  = DMACIntTCClear;
	val |= NAND_DMAC_CLEAR;
	DMACIntTCClear = val;

	return 0;
}

static void tmpa9xx_nand_set_ecctype(unsigned int mlc)
{
	if (mlc)
		NDFMCR1 |= NDFMCR1_ECCS;
	else
		NDFMCR1 &= ~NDFMCR1_ECCS;
}

static void tmpa9xx_nand_start_autoload(unsigned int read)
{
	unsigned int val;

	val = NDFMCR1;
	if (read)
		val &= ~NDFMCR1_SELAL;	/*Set it to be read */
	else
		val |= NDFMCR1_SELAL;	/*Set it to be written */

	/* start autoload */
	val |= NDFMCR1_ALS;
	NDFMCR1 = val;
}

static void tmpa9xx_nand_start_ecc(unsigned int read, unsigned int mlc)
{
	/* 28.11.2016 (sae): Take care of datasheet annotation page 327:
	   Before reading ECC registers, be sure to set NDFMCR0<ECCE> to 0 ...
	   After that, i have seen ecc handling in function. Without that, just sometimes.
	   Debug messages showed, that ECCE was on, everytime.
	*/
/*	NDFMCR0 &= ~NDFMCR0_ECCE;

	if (mlc && read == 0) {
		NDFMCR0 |= NDFMCR0_RSEDN;
	}
*/
	if (read)
		NDFMCR0 |= NDFMCR0_ECCE |  NDFMCR0_ECCRST;
	else
		NDFMCR0 |= NDFMCR0_ECCE |  NDFMCR0_ECCRST | /*NDFMCR0_WE*/NDFMCR0_RSEDN;
}

static void tmpa9xx_nand_set_rw_mode(unsigned int read)
{
	if (read)
		NDFMCR0 &= ~NDFMCR0_WE;
	else
		NDFMCR0 |= NDFMCR0_WE;
}

static int tmpa9xx_nand_dev_ready(struct mtd_info *mtd)
{
       return !(NDFMCR0 & NDFMCR0_BUSY);
}

static int tmpa9xx_nand_als_ready(struct mtd_info *mtd)
{
       return !(NDFMCR1 & NDFMCR1_ALS);
}

static void tmpa9xx_nand_select_chip(struct mtd_info *mtd, int chip)
{
	unsigned int val;

	switch (chip)
	{
	case 0:
		val =  NDFMCR0;
		val |= NDFMCR0_CE0;	/*Enable  chip0 */
		val &= ~NDFMCR0_CE1;    /*Disable chip1 */
		NDFMCR0 = val;
		break;
		
	case 1:
		val = NDFMCR0;
		val |= NDFMCR0_CE1;	/*Enable  chip0 */
		val &= ~NDFMCR0_CE0;    /*Disable chip1 */
		NDFMCR0 = val;
		break;
		
	case -1:
		val = NDFMCR0;
		val &= ~(NDFMCR0_CE1 | NDFMCR0_CE0); 
		NDFMCR0 = val;
		break;
	
	default:
		printf("tmpa9xx_nand_select_chip - trying to enable chip %d - that is not possible\n",chip);
		break;
	}					
}


static u_char tmpa9xx_nand_read_byte(struct mtd_info *mtd)
{
	tmpa9xx_nand_set_rw_mode(1);	
	return NDFDTR;
}

static void tmpa9xx_nand_set_cmd(unsigned int cmd)
{
	/* Begin command latch cycle */
	NDFMCR0 |= NDFMCR0_CLE |NDFMCR0_WE;
	/* Write out the command to the device. */
	NDFDTR = cmd;
	/* End command latch cycle */
	NDFMCR0 &= ~(NDFMCR0_CLE |NDFMCR0_WE);
}

static void tmpa9xx_nand_set_addr(unsigned int column, unsigned int page_addr)
{
	NDFMCR0 |= NDFMCR0_ALE |NDFMCR0_WE;

	/* Serially input address */
	if (column != -1) {
		/* Adjust columns for 16 bit buswidth */
		NDFDTR = column & 0xff;
		NDFDTR = column >> 8;
	}
	if (page_addr != -1) {
		NDFDTR = (unsigned char) (page_addr & 0xff);
		NDFDTR = (unsigned char) ((page_addr >> 8) & 0xff);
		NDFDTR = (unsigned char) ((page_addr >> 16) & 0xff);
	}
	/* Latch in address */
	NDFMCR0 &= ~(NDFMCR0_ALE |NDFMCR0_WE);
}

static int tmpa9xx_wait_nand_dev_ready(void)
{
	int count = 0;


	while ( ((NDFMCR0 & NDFMCR0_BUSY)==NDFMCR0_BUSY) && count < MAX_WAIT_FOR_HW)
        	count++;

	if (count>= MAX_WAIT_FOR_HW)
        {
        	printk(KERN_ERR " tmpa9xx_wait_nand_dev_ready timeout\n");
                return 0;
	}
        return 1;
}


/* Switch Micron on-chip ECC function */
static void tmpa9xx_nand_set_feature_ecc(int enable)
{
	unsigned char buf[4] = { 0x00, 0x00, 0x00, 0x00 };
	int i;

	if (enable) 
		buf[0] = 0x08;

	/* write parameters */
	tmpa9xx_nand_set_cmd(NAND_CMD_SET_FEATURES);
	tmpa9xx_nand_set_addr(-1, 0x90);
	tmpa9xx_nand_set_rw_mode(0);	/* Set controller to write mode */
	tmpa9xx_wait_nand_dev_ready();

	for (i = 0; i < 4; i++)
		NDFDTR = buf[i];
	tmpa9xx_wait_nand_dev_ready();

	/* check parameters */
	tmpa9xx_nand_set_cmd(NAND_CMD_GET_FEATURES);
	tmpa9xx_nand_set_addr(-1, 0x90);
	tmpa9xx_nand_set_rw_mode(1);	/* Set controller to read mode */
	tmpa9xx_wait_nand_dev_ready();

	for (i = 0; i < 4; i++)
		buf[i] = tmpa9xx_nand_read_byte(NULL);
	printk ("\nMicron ECC feature: %s\n", (buf[0] == 0x08) ? "enabled" : "disabled" );
}

static void tmpa9xx_nand_check_enable_feature_ecc(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	struct tmpa9xx_nand_private * priv= (struct tmpa9xx_nand_private *)this->priv;
	
	if (priv->needs_onchip_ecc) {
		if (!onchip_ecc_enabled) {
			printf("NAND ECC enable\n");
			tmpa9xx_nand_set_feature_ecc(1);
			onchip_ecc_enabled = 1;
		}
	} else {
		if (onchip_ecc_enabled) {
			printf("NAND ECC disable\n");
			tmpa9xx_nand_set_feature_ecc(0);
			onchip_ecc_enabled = 0;
		}
	}
}


static void tmpa9xx_nand_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	struct tmpa9xx_nand_private * priv= (struct tmpa9xx_nand_private *)this->priv;
	
	tmpa9xx_nand_check_enable_feature_ecc(mtd);

	if ((len==NAND_DMA_TRANSFER_SIZE) && (priv->dma==1))
        {
		tmpa9xx_nand_set_cmd(NAND_CMD_READ0);
		tmpa9xx_nand_dma_read(buf,len);
		tmpa9xx_nand_start_autoload(1);
		tmpa9xx_nand_set_addr(priv->column,priv->page_addr);
		tmpa9xx_nand_set_cmd(NAND_CMD_READSTART);
		tmpa9xx_nand_set_rw_mode(1);
		if ( tmpa9xx_nand_wait_dma_complete(DMA_TIMEOUT)) {
			printf("ERROR: read page :0x%x, column:0x%x time out\n", priv->page_addr, priv->column);
			return;
		}
		while(!tmpa9xx_nand_als_ready(mtd));
                priv->column+=len;
	}
        else
        {
	       	int i;
                if (priv->rndout==0)
                {
	 		tmpa9xx_nand_set_cmd(NAND_CMD_READ0);
			tmpa9xx_nand_set_addr(priv->column,priv->page_addr);
			tmpa9xx_nand_set_cmd(NAND_CMD_READSTART);
                }
                else
                {
	 		tmpa9xx_nand_set_cmd(NAND_CMD_RNDOUT);
			tmpa9xx_nand_set_addr(priv->column,-1);
			tmpa9xx_nand_set_cmd(NAND_CMD_RNDOUTSTART);
                }
		tmpa9xx_nand_set_rw_mode(1);
		while(!tmpa9xx_nand_dev_ready(mtd));
		for (i=0;i < len;i++)
			*(buf+i)=tmpa9xx_nand_read_byte(mtd);
                priv->column+=len;
	}       

}

static void tmpa9xx_nand_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	struct tmpa9xx_nand_private * priv= (struct tmpa9xx_nand_private *)this->priv;
        static int former_column;

	tmpa9xx_nand_check_enable_feature_ecc(mtd);

	if (priv->column==0 || (former_column!=2048 && len==64))
        {
		tmpa9xx_nand_set_cmd(NAND_CMD_SEQIN);
		tmpa9xx_nand_set_addr(priv->column,priv->page_addr);
		tmpa9xx_nand_set_rw_mode(0);
        }       

	if ((len==NAND_DMA_TRANSFER_SIZE) && (priv->dma==1))
        {
		tmpa9xx_nand_dma_write(buf, len);
		tmpa9xx_nand_start_autoload(0);
		if ( tmpa9xx_nand_wait_dma_complete(DMA_TIMEOUT)) {
			printf("ERROR: write page :0x%x, column:0x%x time out\n", priv->page_addr, priv->column);
			return;
		}
		while(!tmpa9xx_nand_als_ready(mtd));
                priv->column+=len;
	}
        else
        {
        	int i;
		for (i=0;i < len;i++)
			NDFDTR=buf[i];
		while(!tmpa9xx_nand_dev_ready(mtd));
                priv->column+=len;
	}
        
        former_column = priv->column;       
}

static int tmpa9xx_nand_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	struct tmpa9xx_nand_private *priv =
		(struct tmpa9xx_nand_private *)chip->priv;
	int i;

	tmpa9xx_nand_set_cmd(NAND_CMD_READ0);
	tmpa9xx_nand_set_addr(priv->column,priv->page_addr);
	tmpa9xx_nand_set_cmd(NAND_CMD_READSTART);

	tmpa9xx_nand_set_rw_mode(1);
	while(!tmpa9xx_nand_dev_ready(mtd));
	priv->column += len;

	for (i = 0; i < len; i++) {
		uint8_t x = NDFDTR;
		if (buf[i] != x) {
			printf("ERROR: verify mismatch @ page addr %u\n", priv->page_addr);
			return -1;
		}
	}

	return 0;
}

static void tmpa9xx_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
	tmpa9xx_nand_start_ecc(mode == NAND_ECC_READ, 0);
}

static int  tmpa9xx_nand_calculate_ecc(struct mtd_info *mtd, const unsigned char *dat, unsigned char *ecc_code)
{
	struct nand_chip *this = mtd->priv;	
	struct tmpa9xx_nand_private * priv= (struct tmpa9xx_nand_private *)this->priv;
	unsigned int ecc_val = 0;
	unsigned char *buf = ecc_code;

	if (!priv->mlc) {
	//SLC, Hamming ECC data
		ecc_val = NDECCRD1;
		*buf++ = (unsigned char)(ecc_val&0xff);
		*buf++ = (unsigned char)(ecc_val>>8);	
		*buf++ = (unsigned char)(ecc_val>>16);	
		ecc_val =  NDECCRD0;
		*buf++ = (unsigned char)(ecc_val&0xff);
		*buf++ = (unsigned char)(ecc_val>>8);		
		*buf++ = (unsigned char)(ecc_val>>16);
	}
	else
	{
	//MLC, Reed solomn ECC data
		ecc_val = NDECCRD0;
		*buf++ = (unsigned char)(ecc_val>>8);	
		*buf++ = (unsigned char)(ecc_val&0xff);
		*buf++ = (unsigned char)(ecc_val>>24);	
		*buf++ = (unsigned char)(ecc_val>>16);	

		ecc_val = NDECCRD1;
		*buf++ = (unsigned char)(ecc_val>>8);
		*buf++ = (unsigned char)(ecc_val&0xff);
		*buf++ = (unsigned char)(ecc_val>>24);
		*buf++ = (unsigned char)(ecc_val>>16);	

		ecc_val = NDECCRD2;
		*buf++ = (unsigned char)(ecc_val>>8);
		*buf++ = (unsigned char)(ecc_val&0xff);
	}
	
	return 0;
}

#define BIT7        0x80
#define BIT6        0x40
#define BIT5        0x20
#define BIT4        0x10
#define BIT3        0x08
#define BIT2        0x04
#define BIT1        0x02
#define BIT0        0x01
#define BIT1BIT0    0x03
#define BIT23       0x00800000L
#define MASK_CPS    0x3f
#define CORRECTABLE 0x00555554L

static int  tmpa9xx_nand_part_correctdata_rs(unsigned char *data, unsigned char *eccdata, unsigned char ecc1, unsigned char ecc2, unsigned char ecc3)
{
	/*FIXME should be U31, U16 and U8 */
	unsigned int l; /* Working to check d */
	unsigned int d; /* Result of comparison */
	unsigned short i; /* For counting */
	unsigned char d1,d2,d3; /* Result of comparison */
	unsigned char a; /* Working for add */
	unsigned char add; /* Byte address of cor. DATA */
	unsigned char b; /* Working for bit */
	unsigned char bit; /* Bit address of cor. DATA */
	

	d1=ecc1^eccdata[1]; d2=ecc2^eccdata[0]; /* Compare LP's */
	d3=ecc3^eccdata[2]; /* Comapre CP's */
	d=((unsigned int)d1<<16) /* Result of comparison */
	    +((unsigned int)d2<<8)
	        +(unsigned int)d3;
	if (d==0) 
		return(0); /* If No error, return */
	if (((d^(d>>1))&CORRECTABLE)==CORRECTABLE) { /* If correctable */
		l=BIT23;
		add=0; /* Clear parameter */
		a=BIT7;
		for(i=0; i<8; ++i) { /* Checking 8 bit */
			if ((d&l)!=0) 
				add|=a; /* Make byte address from LP's */
			l>>=2; a>>=1; /* Right Shift */
		}
		bit=0; /* Clear parameter */
		b=BIT2;
		for (i = 0; i < 3; ++i) { /* Checking 3 bit */
		    if ((d&l)!=0) 
				bit|=b; /* Make bit address from CP's */
		    l>>=2; b>>=1; /* Right shift */
		}
		b=BIT0;
		data[add]^=(b<<bit); /* Put corrected data */
		return(1);
	}
	i=0; /* Clear count */
	d&=0x00ffffffL; /* Masking */
	while (d)	{ /* If d=0 finish counting */
		if (d&BIT0) 
			++i; /* Count number of 1 bit */
		d>>=1; /* Right shift */
	}
	if (i==1)	{ /* If ECC error */
		eccdata[1]=ecc1; 
		eccdata[0]=ecc2; /* Put right ECC code */
		eccdata[2]=ecc3;
		return(2);
	}
	return(3); /* Uncorrectable error */
}

#define ECC_DEBUG(...)

static int tmpa9xx_nand_part_correctdata_hamming_256(struct mtd_info *mtd,
						 u_char *dat,
						 u_char *read_ecc,
						 u_char *calc_ecc)
{
	u_int32_t ecc_nand = read_ecc[0] | (read_ecc[1] << 8) | (read_ecc[2] << 16);
	u_int32_t ecc_calc = calc_ecc[0] | (calc_ecc[1] << 8) | (calc_ecc[2] << 16);
	u_int32_t bits_set;
	uint8_t line;
	uint8_t column;
	uint8_t byte;
	int i;

	/* this algorithm follows "3.11.4.2 Error Correction Methods" from the spec */

	/* step 2 */
	u_int32_t diff = ecc_calc ^ ecc_nand;

	ECC_DEBUG("%s(): read_ecc %02x %02x %02x\n", __func__, read_ecc[0], read_ecc[1], read_ecc[2]);
	ECC_DEBUG("%s(): calc_ecc %02x %02x %02x\n", __func__, calc_ecc[0], calc_ecc[1], calc_ecc[2]);
	ECC_DEBUG("%s(): diff 0x%08x\n", __func__, diff);

	/* step 3 */
	if (!diff)
		return 0;

	/* http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan */
	for (bits_set = 0; diff; bits_set++)
		diff &= diff - 1; /* clear the least significant bit set */

	/* step 4 */
	if (bits_set == 1)
		return 1;

	/* step 5 */
	diff = ecc_calc ^ ecc_nand;
	for (i = 0; i < 12; i++, diff >>= 2) {
		ECC_DEBUG("%s(): diff 0x%08x, bits %02x\n", __func__, diff, diff & 0x3);
		if (i == 8)
			continue;
		if ((diff & 0x3) == 0x1 || (diff & 0x3) == 0x2)
			continue;
		ECC_DEBUG("%s(): uncorrectable error\n", __func__);
		return -1;
	}

	/* step 6: the ecc error is correctable */
	diff = ecc_calc ^ ecc_nand;
	for (i = 0, line = 0; i < 8; i++, diff >>= 2)
		if ((diff & 0x3) == 0x2)
			line |= (0x1 << i);

	diff >>= 2;
	for (i = 0, column = 0; i < 3; i++, diff >>= 2)
		if ((diff & 0x3) == 0x2)
			column |= (0x1 << i);

	byte = *(dat + line);
	ECC_DEBUG("%s(): single bit ECC error detected in line 0x%02x, column 0x%02x\n", __func__, line, column);
	ECC_DEBUG("%s(): old byte is 0x%02x\n", __func__, byte);

	if (byte & (1 << column))
		byte &= ~(1 << column);
	else
		byte |= (1 << column);

	*(dat + line) = byte;
	ECC_DEBUG("%s(): new byte is 0x%02x\n", __func__, byte);

	return 1;
}

static int tmpa9xx_nand_part_correctdata_hamming(struct mtd_info *mtd,
						 u_char * dat,
						 u_char * read_ecc,
						 u_char * calc_ecc)
{
	register struct nand_chip *this = mtd->priv;
	struct tmpa9xx_nand_private * priv= (struct tmpa9xx_nand_private *)this->priv;
	int ret1;
	int ret2;

	ret1 = tmpa9xx_nand_part_correctdata_hamming_256(mtd, dat+256, read_ecc,   calc_ecc);
	ret2 = tmpa9xx_nand_part_correctdata_hamming_256(mtd, dat,     read_ecc+3, calc_ecc+3);

	if (onchip_ecc_enabled)
		return 0;

	/* all went fine */
	if (!ret1 && !ret2)
		return 0;

	/* check for uncorrectable error in one of the blocks */
	if (ret1 < 0)
		return ret1;
	if (ret2 < 0)
		return ret2;

	/* report all corrected errors */
	return ret1 + ret2;
}

static int tmpa9xx_nand_correct_data(struct mtd_info *mtd, u_char *data, u_char *read_ecc, u_char *calc_ecc)
{
	struct nand_chip *this = mtd->priv;
        struct tmpa9xx_nand_private * priv= (struct tmpa9xx_nand_private *)this->priv;

	if (priv->mlc)
	{
		unsigned int size;
		unsigned char ecc1, ecc2, ecc3;
		unsigned int ret, ret_tmp;

		size = 10;

		ret_tmp = 0;
		while (size > 0)
		{
			ecc2 = *calc_ecc++;
			ecc1 = *calc_ecc++;
			ecc3 = *calc_ecc++;
			ret = tmpa9xx_nand_part_correctdata_rs(data, read_ecc, ecc1, ecc2, ecc3);
			if(ret > ret_tmp)
				ret_tmp = ret;
			
			size -= ECCBUF_SIZE;
			data += ECCBUF_SIZE;
			read_ecc += 3;	
		}
		return (ret_tmp != 0 && ret_tmp != 1)? -1 : 0;
	}	
	else
		return tmpa9xx_nand_part_correctdata_hamming(mtd,data,read_ecc,calc_ecc);
	
	return 0; /* Fix compiler warning */
}

static void tmpa9xx_nand_set_timing(struct tmpa9xx_nand_private *priv)
{
	NDFMCR2 = ((priv->timing.splw<<12)|(priv->timing.sphw<<8)|(priv->timing.splr<<4)|(priv->timing.sphr<<0));
}

static void tmpa9xx_nand_command (struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
	register struct nand_chip *this = mtd->priv;
	struct tmpa9xx_nand_private * priv= (struct tmpa9xx_nand_private *)this->priv;

	tmpa9xx_nand_check_enable_feature_ecc(mtd);

	switch (command)
	{
	case NAND_CMD_READ0:
        	priv->rndout=0;
                priv->column=column;
                priv->page_addr=page_addr;
		break;
	
	case NAND_CMD_PAGEPROG:
        	priv->rndout=0;
		tmpa9xx_nand_set_cmd(NAND_CMD_PAGEPROG);
		break;

	case NAND_CMD_READOOB:
        priv->rndout=0;
		priv->column=priv->page_size;
        priv->page_addr=page_addr;
		break;

	case NAND_CMD_ERASE1:
        priv->rndout=0;
        priv->page_addr=page_addr;
		tmpa9xx_nand_set_cmd(NAND_CMD_ERASE1);
		tmpa9xx_nand_set_addr(-1,page_addr);
		break;

	case NAND_CMD_STATUS:
        	priv->rndout=0;
		tmpa9xx_nand_set_cmd(NAND_CMD_STATUS);
		break;

	case NAND_CMD_SEQIN:
        	priv->rndout=0;
                priv->column=column;
                priv->page_addr=page_addr;
		break;

	case NAND_CMD_READID:
        	priv->rndout=0;
		tmpa9xx_nand_set_cmd(NAND_CMD_READID);
		NDFMCR0 |= NDFMCR0_ALE |NDFMCR0_WE;
		NDFDTR = 0x00;
		/* Latch in address */
		NDFMCR0 &= ~(NDFMCR0_ALE |NDFMCR0_WE);
		break;

	case NAND_CMD_ERASE2:
        	priv->rndout=0;
		tmpa9xx_nand_set_cmd(NAND_CMD_ERASE2);
		break;

	case NAND_CMD_RESET:
        	priv->rndout=0;
		tmpa9xx_nand_set_cmd(NAND_CMD_RESET);
		break;

	case NAND_CMD_READSTART:
        	priv->rndout=0;
		tmpa9xx_nand_set_cmd(NAND_CMD_READSTART);
		break;
                
	case NAND_CMD_RNDOUT:
        	priv->rndout=1;
                priv->column=column;
		break;

	case NAND_CMD_RNDOUTSTART:
		printf("NAND_CMD_RNDOUTSTART unsupported\n");
		break;

	case NAND_CMD_CACHEDPROG:
		printf("NAND_CMD_CACHEDPROG unsupported\n");
		break;

	case NAND_CMD_READ1:
		printf("NAND_CMD_READ1 unsupported\n");
		break;

	case NAND_CMD_RNDIN:
		printf("NAND_CMD_RNDIN unsupported\n");
		break;

	case NAND_CMD_STATUS_MULTI:
		printf("NAND_CMD_STATUS_MULTI unsupported\n");
		break;

	default:
		printf ("Unknown command code:%x\n",command);
		break;	
	}

	while(!tmpa9xx_nand_dev_ready(mtd));
}

static void tmpa9xx_nand_get_internal_structure(struct tmpa9xx_nand_private *priv)
{
	int i;
	unsigned char id_code[4];

	tmpa9xx_nand_set_cmd(NAND_CMD_RESET);
	while(!tmpa9xx_nand_dev_ready(NULL));
	tmpa9xx_nand_set_cmd(NAND_CMD_READID);
	while(!tmpa9xx_nand_dev_ready(NULL));
	NDFMCR0 |= NDFMCR0_ALE |NDFMCR0_WE;
	NDFDTR = 0x00;
	/* Latch in address */
	NDFMCR0 &= ~(NDFMCR0_ALE |NDFMCR0_WE);
	while(!tmpa9xx_nand_dev_ready(NULL));

	NDFMCR0 &= ~NDFMCR0_WE;

	for (i=0;i<4;i++)
		id_code[i]=NDFDTR;
	
        if (id_code[1]==0x0c)
        	priv->mlc=1;
                
        if ( (id_code[3]&0x03) == 0x1)
        {
        	priv->page_size =2048;
                priv->spare_size=  64;
        }
        else
        {
        	priv->page_size = 512;
                priv->spare_size=  16;
        }

		/* check for new Micron flash, Tonga only! */
        if ((id_code[0] == 0x2c) && !id) {
        	priv->needs_onchip_ecc = 1;
			printf("Micron flash mode set\n");
		}
}	

int board_nand_init(struct nand_chip *nand)
{	
	struct tmpa9xx_nand_private * priv;
	priv = &tmpa9xx_nand[id];

	nand->priv = priv;
	
	nand->IO_ADDR_R     = (void  __iomem *)(&NDFDTR);
	nand->IO_ADDR_W     = (void  __iomem *)(&NDFDTR);
	
	nand->chip_delay    = 100;
#ifdef CONFIG_SYS_NAND_USE_FLASH_BBT
	nand->options	   |= NAND_USE_FLASH_BBT;
#endif
	nand->options	   |= NAND_NO_SUBPAGE_WRITE;
 
 	/* Control Functions */
	nand->select_chip   = tmpa9xx_nand_select_chip;
	nand->cmdfunc       = tmpa9xx_nand_command;
	nand->dev_ready     = tmpa9xx_nand_dev_ready;
		
	/* Data access functions */
	nand->read_byte     = tmpa9xx_nand_read_byte;
	nand->read_buf      = tmpa9xx_nand_read_buf;
	nand->write_buf     = tmpa9xx_nand_write_buf;
	nand->verify_buf    = tmpa9xx_nand_verify_buf;
	
	/* ECC Mode */
	nand->ecc.mode      = NAND_ECC_HW;

 	tmpa9xx_nand_select_chip(NULL,0);
 	tmpa9xx_nand_set_timing(priv);
	tmpa9xx_nand_get_internal_structure(priv);

	if (priv->needs_onchip_ecc)
		priv->dma = 0;
       
	if (priv->dma==1)
	{
		nand->ecc.mode      = NAND_ECC_HW;
		nand->ecc.calculate = tmpa9xx_nand_calculate_ecc;
		nand->ecc.hwctl     = tmpa9xx_nand_enable_hwecc;
		nand->ecc.correct   = tmpa9xx_nand_correct_data;
		nand->ecc.size      = 512;
	
		if (priv->mlc) /* MLC identifier */
		{
			tmpa9xx_nand_set_ecctype(1);
			nand->ecc.layout	= &nand_oob_rs_2048;
			nand->ecc.bytes 	= 10;
		}
		else
		{
			tmpa9xx_nand_set_ecctype(0);
			nand->ecc.bytes = 6;
			if (priv->page_size==2048)
			{
				nand->ecc.layout    = &nand_oob_hamming_2048;
			}
			else
			{
				nand->ecc.layout    = &nand_oob_hamming_512;
			}
		}
	}
	else
	{
		nand->ecc.mode      = NAND_ECC_SOFT;
	}

	/* special handling of micron flash */
	if (priv->needs_onchip_ecc) {
		nand->ecc.mode = NAND_ECC_NONE;
		tmpa9xx_nand_set_feature_ecc(1);
		onchip_ecc_enabled = 1;
	}

	NDFMCR0 = 0x0;
	NDFMCR1 = 0x0;

	/* increment id after successful initialisation */
	id++;

	return 0;
}
