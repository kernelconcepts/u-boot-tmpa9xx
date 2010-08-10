/*
 * (C) Copyright 2009,2010
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
	struct tmpa9xx_nand_timing timing;
};

/* Nand Control */
struct tmpa9xx_nand_private tmpa9xx_nand = {
	.dma	= 1,
	.timing =
	{
		.splw = 0x2,
		.sphw = 0x2,
		.splr = 0x2,
		.sphr = 0x2,
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

static void tmpa9xx_nand_start_ecc(unsigned int read)
{
	if (read)
		NDFMCR0 |= NDFMCR0_ECCE |  NDFMCR0_ECCRST;
	else
		NDFMCR0 |= NDFMCR0_ECCE |  NDFMCR0_ECCRST | NDFMCR0_RSEDN;
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

static void tmpa9xx_nand_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	struct tmpa9xx_nand_private * priv= (struct tmpa9xx_nand_private *)this->priv;

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
 		tmpa9xx_nand_set_cmd(NAND_CMD_READ0);
		tmpa9xx_nand_set_addr(priv->column,priv->page_addr);
		tmpa9xx_nand_set_cmd(NAND_CMD_READSTART);
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

static void tmpa9xx_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
	tmpa9xx_nand_start_ecc(mode == NAND_ECC_READ);
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

static int tmpa9xx_nand_part_correctdata_hamming(struct mtd_info *mtd, u_char *dat, u_char *read_ecc, u_char *calc_ecc)
{
	struct nand_chip *this = mtd->priv;
	u_int32_t ecc_nand = read_ecc[0] | (read_ecc[1] << 8) | (read_ecc[2] << 16);
	u_int32_t ecc_calc = calc_ecc[0] | (calc_ecc[1] << 8) | (calc_ecc[2] << 16);
	u_int32_t diff     = ecc_calc ^ ecc_nand;
	
	if (diff) {
		if ((((diff >> 12) ^ diff) & 0xfff) == 0xfff) {
			/* Correctable error */
			if ((diff >> (12 + 3)) < this->ecc.size) {
				uint8_t find_bit = 1 << ((diff >> 12) & 7);
				uint32_t find_byte = diff >> (12 + 3);

				dat[find_byte] ^= find_bit;
				printf("Correcting single bit ECC error at offset: %d, bit: %d\n", find_byte, find_bit);
				return 1;
			} else {
				printf("ECC UNCORRECTED_ERROR 1\n");
				return -1;
			}
		} else if (!(diff & (diff - 1))) {
			/* Single bit ECC error in the ECC itself,
			   nothing to fix */
			printf("Single bit ECC error in ECC.\n");
			return 1;
		} else {
			/* Uncorrectable error */
			//printf("ECC UNCORRECTED_ERROR 2\n");
			return -1;
		}
	}
	return(0);
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

	switch (command)
	{
	case NAND_CMD_READ0:
                priv->column=column;
                priv->page_addr=page_addr;
		break;
	
	case NAND_CMD_PAGEPROG:
		tmpa9xx_nand_set_cmd(NAND_CMD_PAGEPROG);
		break;

	case NAND_CMD_READOOB:
		priv->column=priv->page_size;
                priv->page_addr=page_addr;
		break;

	case NAND_CMD_ERASE1:
		tmpa9xx_nand_set_cmd(NAND_CMD_ERASE1);
		tmpa9xx_nand_set_addr(-1,page_addr);
		break;

	case NAND_CMD_STATUS:
		tmpa9xx_nand_set_cmd(NAND_CMD_STATUS);
		break;

	case NAND_CMD_SEQIN:
                priv->column=column;
                priv->page_addr=page_addr;
		break;

	case NAND_CMD_READID:
		tmpa9xx_nand_set_cmd(NAND_CMD_READID);
		NDFMCR0 |= NDFMCR0_ALE |NDFMCR0_WE;
		NDFDTR = 0x00;
		/* Latch in address */
		NDFMCR0 &= ~(NDFMCR0_ALE |NDFMCR0_WE);
		break;

	case NAND_CMD_ERASE2:
		tmpa9xx_nand_set_cmd(NAND_CMD_ERASE2);
		break;

	case NAND_CMD_RESET:
		tmpa9xx_nand_set_cmd(NAND_CMD_RESET);
		break;

	case NAND_CMD_READSTART:
		tmpa9xx_nand_set_cmd(NAND_CMD_READSTART);
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

	case NAND_CMD_RNDOUT:
		printf("NAND_CMD_RNDOUT unsupported\n");
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
}	

int board_nand_init(struct nand_chip *nand)
{	
	struct tmpa9xx_nand_private * priv;
	priv = &tmpa9xx_nand;

       	nand->priv = priv;
	
	nand->IO_ADDR_R     = (void  __iomem *)(&NDFDTR);
	nand->IO_ADDR_W     = (void  __iomem *)(&NDFDTR);
	
	nand->chip_delay    = 0;

	/* Control Functions */
	nand->select_chip   = tmpa9xx_nand_select_chip;
	nand->cmdfunc       = tmpa9xx_nand_command;
	nand->dev_ready     = tmpa9xx_nand_dev_ready;
		
	/* Data access functions */
	nand->read_byte     = tmpa9xx_nand_read_byte;
	nand->read_buf      = tmpa9xx_nand_read_buf;
	nand->write_buf     = tmpa9xx_nand_write_buf;
	
	/* ECC Mode */
	nand->ecc.mode      = NAND_ECC_HW;

 	tmpa9xx_nand_select_chip(NULL,0);
 	tmpa9xx_nand_set_timing(priv);
	tmpa9xx_nand_get_internal_structure(priv);
        
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
	NDFMCR0 =0x0;
	NDFMCR1 =0x0;

	return 0;
}
