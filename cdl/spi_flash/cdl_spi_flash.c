/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_spi_flash.h"


/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct spi_flash g_sf;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/


/*****************************************************************************/
//  Function:  int sf_set_controller(E_SPI_PORT port, spi_xfer_fn xfer)
//
//  Parameters:
//		port		: spi port
//
//		xfer		: spi transfer function
//
//  Return:
//		none
//
//  Description:
//       set spi flash stucture.
//
/*****************************************************************************/
void sf_set_controller(E_SPI_PORT port, spi_xfer_fn xfer)
{
	struct spi_flash *p_sf = &g_sf;
	
	mem_set8(p_sf, 0, sizeof(struct spi_flash));
	p_sf->port = port;
	p_sf->xfer = xfer;
	p_sf->time_out = SPI_FLASH_TIME_OUT;
}

/*****************************************************************************/
//  Function:  static int sf_read_write(uint8_t *p_cmd, uint32_t cmd_len, uint8_t *p_out, 
//								uint8_t *data_in, uint32_t data_len)
//
//  Parameters:
//		p_cmd [out]	: command buffer
//
//		cmd_len		: command length
//
//		p_out [out]	: tx buffer
//
//		p_in    [in]	: rx buffer
//
//		data_len		:  transfer data length(byte aligned)
//
//		flag			: transmit flag
//
//  Return:
//		0		: transfer ok
//		others	: transfer fail
//
//  Description:
//       spi flash read and write data, whih timing is compatible with nor flash.
//
/*****************************************************************************/
static int sf_read_write(uint8_t *p_cmd, uint32_t cmd_len, uint8_t *p_out, 
							uint8_t *p_in, uint32_t data_len)
{
	struct spi_flash *p_sf = &g_sf;
	int ret_val;
	E_SPI_XFER_FLAG flag = (data_len == 0) ? SPI_XFER_ONCE : SPI_XFER_BEGIN;

	//info("begin to wr, flags:%x\n", flags);

	// transfer command
	ret_val = p_sf->xfer(p_sf->port, p_cmd, NULL, cmd_len, flag);
	if (ret_val) 
	{
		debug("SF: Failed to send command (%zu bytes): %d\n", cmd_len, ret_val);
		return ret_val;
	} 

	// transfer and receive data
	if (data_len != 0) 
	{
		//info("begin to read\n");
		
		flag = SPI_XFER_ONCE;
		ret_val = p_sf->xfer(p_sf->port, p_out, p_in, data_len, flag);
		if (ret_val)
		{
	  		debug("SF: Failed to transfer %zu bytes of data: %d\n", data_len, ret_val);
		}	
	}

	return ret_val;
}

/*****************************************************************************/
//  Function:  static int sf_cmd_read(uint8_t *p_cmd, uint32_t cmd_len, uint8_t *p_in, uint32_t data_len)
//
//  Parameters:
//		p_cmd [out]	: command buffer
//
//		cmd_len		: command length
//
//		p_in    [in]	: rx buffer
//
//		data_len		:  transfer data length(byte aligned)
//
//  Return:
//		0		: transfer ok
//		others	: transfer fail
//
//  Description:
//       Issue read command and read data from the flash.
//
/*****************************************************************************/
static int sf_cmd_read(uint8_t *p_cmd, uint32_t cmd_len, uint8_t *p_in, uint32_t data_len)
{
	return sf_read_write(p_cmd, cmd_len, NULL, p_in, data_len);
}

/*****************************************************************************/
//  Function:  static int sf_cmd_write(uint8_t *p_cmd, uint32_t cmd_len, uint8_t *p_out, uint32_t data_len)
//
//  Parameters:
//		p_cmd [out]	: command buffer
//
//		cmd_len		: command length
//
//		p_out [out]	: tx buffer
//
//		data_len		:  transfer data length(byte aligned)
//
//  Return:
//		0		: transfer ok
//		others	: transfer fail
//
//  Description:
//       Issue write command and write data to the flash.
//
/*****************************************************************************/
static int sf_cmd_write(uint8_t *p_cmd, uint32_t cmd_len, uint8_t *p_out, uint32_t data_len)
{
	return sf_read_write(p_cmd, cmd_len, p_out, NULL, data_len);
}

/*****************************************************************************/
//  Function:  static int sf_read_st(uint8_t *p_st)
//
//  Parameters:
//
//		p_st [in]	: status buffer
//
//  Return:
//		0		: transfer ok
//		others	: transfer fail
//
//  Description:
//       Read status from the flash.
//
/*****************************************************************************/
static int sf_read_st(uint8_t *p_st)
{
	int ret_val;
	uint8_t cmd = CMD_READ_STATUS;

	ret_val = sf_cmd_read(&cmd, 1, p_st, 1);
	if (ret_val < 0) 
	{
		debug("SF: fail to read status register\n");
	}

	return ret_val;
}

static int sf_winbond_read_st_1(uint8_t *p_st)
{
	int ret_val;
	uint8_t cmd = CMD_READ_WINBOND_STATUS1;

	ret_val = sf_cmd_read(&cmd, 1, p_st, 1);
	if (ret_val < 0) 
	{
		debug("SF: fail to read status register\n");
	}

	return ret_val;
}

static int sf_mxic_read_st_1(uint8_t *p_st)
{
	int ret_val;
	uint8_t cmd = NOR_CMD_MXIC_RDCR;

	ret_val = sf_cmd_read(&cmd, 1, p_st, 1);
	if (ret_val < 0) 
	{
		debug("SF: fail to read status register\n");
	}

	return ret_val;
}


/*****************************************************************************/
//  Function:  static int sf_chk_st(void)
//
//  Parameters:
//
//		none
//
//  Return:
//		0		: transfer ok
//		others	: transfer fail
//
//  Description:
//       Check status until it is ready.
//
/*****************************************************************************/
static int sf_chk_st(void)
{
	struct spi_flash *p_sf = &g_sf;
	int ret_val;
	unsigned long long timebase;
	uint8_t st;

	timebase = get_timer(0);

	do {
		ret_val = sf_read_st(&st);
		if (ret_val < 0)
		{
			return ret_val;
		}
		
		if ((st & QSPI_NOR_ST_WIP_MASK) == 0)
		{
			return 0;
		}

	} while (get_timer(timebase) < p_sf->time_out);

	info("SF: sf_chk_st timeout!\n");

	return -1;
}

/*****************************************************************************/
//  Function:  static inline int sf_cmd_write_enable(void)
//
//  Parameters:
//
//		none
//
//  Return:
//		0		: transfer ok
//		others	: transfer fail
//
//  Description:
//       Issue write enable command to the flash.
//
/*****************************************************************************/
static int sf_cmd_write_enable(void)
{
	int ret_val;
	uint8_t cmd = CMD_WRITE_ENABLE;

	ret_val = sf_cmd_read(&cmd, 1, NULL, 0);
	if (ret_val < 0) 
	{
		debug("SF: fail to read status register\n");
	}

	return ret_val;
}

/*****************************************************************************/
//  Function:  uint8_t qspi_nor_reset(void)
//
//  Parameters:
//		none
//
//  Return:
//		E_QSPI_ST
//
//  Description:
//      Reset the device.
//
/*****************************************************************************/
uint8_t sf_reset(void)
{
	int ret_val;
	uint8_t cmd[5];
	
	// issue write enable command
	ret_val = sf_cmd_write_enable();
	if (ret_val) 
	{
		debug("SF: enabling write failed\n");
		return ret_val;
	}
	
	// reset enable
	cmd[0] = CMD_NOR_RSTEN;
	ret_val = sf_cmd_write(cmd, 1, NULL, 0);
	if (ret_val) 
	{
		return ret_val;
	}

	// reset
	cmd[0] = CMD_NOR_RST;
	ret_val = sf_cmd_write(cmd, 1, NULL, 0);
	if (ret_val) 
	{
		return ret_val;
	}	

	sf_chk_st();

	return ret_val;
}

/*****************************************************************************/
//  Function:  uint8_t sf_wr_st(uint8_t *p_st)
//
//  Parameters:
//		p_st 	: status
//
//  Return:
//		0		: transfer ok
//		others	: transfer fail
//
//  Description:
//      Write status to the flash.
//
/*****************************************************************************/
uint8_t sf_wr_st(uint8_t *p_st)
{
	struct spi_flash *p_sf = &g_sf;
	int ret_val;
	uint8_t cmd[5];
	uint8_t cmd_len = (p_sf->manu_id == MANU_ID_ISSI) ? 2 : 3;
	
	// issue write enable command
	ret_val = sf_cmd_write_enable();
	if (ret_val) 
	{
		debug("SF: enabling write failed\n");
		return ret_val;
	}
	
	// issue write command and data
	cmd[0] = CMD_WRITE_STATUS;
	cmd[1] = p_st[0];
	cmd[2] = p_st[1];
	ret_val = sf_cmd_write(cmd, cmd_len, NULL, 0);
	if (ret_val) 
	{
		debug("SF: write cmd failed\n");
		return ret_val;
	}

	sf_chk_st();

	return ret_val;
}

uint8_t sf_blk_unprotect(void)
{
	struct spi_flash *p_sf = &g_sf;
	uint8_t st[2] = {0, 0};

	// read status_0
	sf_read_st(&st[0]);

	// read status_1
	switch (p_sf->manu_id)
	{
		case MANU_ID_MXIC:
				sf_mxic_read_st_1(&st[1]);
			break;
		case MANU_ID_WINBOND:
				sf_winbond_read_st_1(&st[1]);
			break;
		case MANU_ID_ISSI:
				// dummy
			break;
		default:
			break;
	}

	info("st 0, 1: 0x%x, 0x%x\n", st[0], st[1]);

	// write status 0 and 1
	sf_wr_st(st);

	return 0;
}



/*****************************************************************************/
//  Function:  int sf_probe(void)
//
//  Parameters:
//
//		none
//
//  Return:
//		0		: transfer ok
//		others	: transfer fail
//
//  Description:
//       spi flash probe.
//
/*****************************************************************************/
int sf_probe(void)
{
	struct spi_flash *p_sf = &g_sf;
	uint8_t cmd = CMD_READ_ID;
	uint8_t idcode[5];
	int ret_val;

	sf_reset();

	ret_val = sf_cmd_read(&cmd, 1,  idcode, sizeof(idcode));
	if (ret_val) 
	{
		info("SF: Failed to get idcodes\n");
		return ret_val;
	}

	info("id: %x %x %x %x %x\n", idcode[0], idcode[1], idcode[2], idcode[3], idcode[4]);

	// identify spi nand and fill operation parameters
	if (idcode[0] == 0xC8) 
	{
		if ((idcode[1] == 0x40) && (idcode[2] == 0x15))
		{
			info("detect spi flash: GD25Q16B\n");	
		}
		
		p_sf->pages_per_sec = 16;
		p_sf->secs_per_blk = 16;
		p_sf->blks_per_chip = 32;	
		p_sf->page_size = 256;
		p_sf->sec_size = p_sf->page_size * p_sf->pages_per_sec;
		p_sf->blk_size = p_sf->sec_size * p_sf->secs_per_blk;
		p_sf->cap = p_sf->blk_size * p_sf->blks_per_chip; 

		p_sf->erase_cmd = CMD_ERASE_4K;
		p_sf->sio_rd_cmd = CMD_READ_ARRAY_FAST;
		p_sf->sio_pp_cmd = CMD_PAGE_PROGRAM;

		p_sf->dummy_byte = 1;
	}  
	else if (idcode[0] == MANU_ID_WINBOND) 
	{
		if ((idcode[1] == 0x40) && (idcode[2] == 0x15))
		{
			info("detect spi flash: W25Q16D\n");	
		}

		p_sf->manu_id = MANU_ID_WINBOND;
		p_sf->pages_per_sec = 16;
		p_sf->secs_per_blk = 16;
		p_sf->blks_per_chip = 32;	
		p_sf->page_size = 256;
		p_sf->sec_size = p_sf->page_size * p_sf->pages_per_sec;
		p_sf->blk_size = p_sf->sec_size * p_sf->secs_per_blk;
		p_sf->cap = p_sf->blk_size * p_sf->blks_per_chip; 

		p_sf->erase_cmd = CMD_ERASE_4K;
		p_sf->sio_rd_cmd = CMD_READ_ARRAY_FAST;
		p_sf->sio_pp_cmd = CMD_PAGE_PROGRAM;

		p_sf->dummy_byte = 1;
	}	
	else if (idcode[0] == MANU_ID_ISSI) 
	{
		if ((idcode[1] == 0x60) && (idcode[2] == 0x15))
		{
			info("detect spi flash: IS25LP016D\n");
		}
		else if ((idcode[1] == 0x70) && (idcode[2] == 0x15))
		{
			info("detect spi flash: IS25WP016D\n");
		}

		p_sf->manu_id = MANU_ID_ISSI;
		p_sf->pages_per_sec = 16;
		p_sf->secs_per_blk = 16;
		p_sf->blks_per_chip = 32;	
		p_sf->page_size = 256;
		p_sf->sec_size = p_sf->page_size * p_sf->pages_per_sec;
		p_sf->blk_size = p_sf->sec_size * p_sf->secs_per_blk;
		p_sf->cap = p_sf->blk_size * p_sf->blks_per_chip; 

		p_sf->erase_cmd = CMD_ERASE_4K;
		p_sf->sio_rd_cmd = CMD_READ_ARRAY_FAST;
		p_sf->sio_pp_cmd = CMD_PAGE_PROGRAM;

		p_sf->dummy_byte = 1;
	}	
	else if (idcode[0] == MANU_ID_MXIC) 
	{
		p_sf->manu_id = MANU_ID_MXIC;
		
		if ((idcode[1] == 0x23) && (idcode[2] == 0x15))
		{
			info("detect spi flash: MX25V1635F\n");
			
			p_sf->pages_per_sec = 16;
			p_sf->secs_per_blk = 16;
			p_sf->blks_per_chip = 32;	
			p_sf->page_size = 256;
			p_sf->sec_size = p_sf->page_size * p_sf->pages_per_sec;
			p_sf->blk_size = p_sf->sec_size * p_sf->secs_per_blk;
			p_sf->cap = p_sf->blk_size * p_sf->blks_per_chip; 

			p_sf->erase_cmd = CMD_ERASE_4K;
			p_sf->sio_rd_cmd = CMD_READ_ARRAY_FAST;
			p_sf->sio_pp_cmd = CMD_PAGE_PROGRAM;

			p_sf->dummy_byte = 1;
		}
		else if ((idcode[1] == 0x23) && (idcode[2] == 0x14))
		{
			info("detect spi flash: MX25V8035F\n");
		
			p_sf->pages_per_sec = 16;
			p_sf->secs_per_blk = 16;
			p_sf->blks_per_chip = 16;	
			p_sf->page_size = 256;
			p_sf->sec_size = p_sf->page_size * p_sf->pages_per_sec;
			p_sf->blk_size = p_sf->sec_size * p_sf->secs_per_blk;
			p_sf->cap = p_sf->blk_size * p_sf->blks_per_chip; 

			p_sf->erase_cmd = CMD_ERASE_4K;
			p_sf->sio_rd_cmd = CMD_READ_ARRAY_FAST;
			p_sf->sio_pp_cmd = CMD_PAGE_PROGRAM;

			p_sf->dummy_byte = 1;		
		}
	}	
	else 
	{
	  	info("unknow spi flash: %x %x %x\n", idcode[0], idcode[1], idcode[2]);
		ret_val = 1;
	}
	
	if (!ret_val) 
	{
		info("flash chip is spi nor flash, total size %d M bytes\n", p_sf->cap/(1024*1024));
		info("page size=%d \n", p_sf->page_size);
		info("block size=%d \n", p_sf->blk_size);
		info("sector size=%d \n", p_sf->sec_size);
		info("\n");
	}

	// unportect blocks
	sf_blk_unprotect();
	
	return ret_val;
}

/*****************************************************************************/
//  Function:  int sf_erase_sec(uint32_t addr, uint32_t len)
//
//  Parameters:
//
//		addr	: spi flash address which is not include the base address
//
//		len	: erase length(byte aligned)
//
//  Return:
//		0		: erase ok
//		others	: erase fail
//
//  Description:
//       spi flash erase sector.
//
/*****************************************************************************/
int sf_erase_sec(uint32_t addr, uint32_t len)
{
	struct spi_flash *p_sf = &g_sf;
	int ret_val = 0;
	uint8_t cmd[4];

	// Consistency checking
	if (addr + len > p_sf->cap) 
	{
		info("ERROR: attempting erase past flash size (%#x)\n", p_sf->cap);
		return 1;
	}
	
	if ((addr % p_sf->sec_size) || (len % p_sf->sec_size) || (len == 0)) 
	{
		debug("SF: Erase addr/length not multiple of erase sector size addr/psf->sec_size/len = 0x%x/0x%x/0x%x\n", addr, p_sf->sec_size, len);
		return -1;
	}

	while (len) 
	{
		cmd[0] = p_sf->erase_cmd;
		cmd[1] = (addr>>16)&0xFF;
		cmd[2] = (addr>>8) &0xFF;
		cmd[3] = (addr>>0) &0xFF;
		
		debug("SF: erase %2x %2x %2x %2x (%x)\n", cmd[0], cmd[1], cmd[2], cmd[3], addr);

		ret_val = sf_cmd_write_enable();
		if (ret_val) 
		{
			debug("SF: enabling write failed\n");
			return ret_val;
		}

		ret_val = sf_cmd_write(cmd, 4, NULL, 0);
		if (ret_val) 
		{
			debug("SF: write cmd failed\n");
			return ret_val;
		}

		ret_val = sf_chk_st();
		if (ret_val) 
		{
			debug("SF: erase timed out\n");
			return ret_val;
		}

		addr += p_sf->sec_size;
		len -= p_sf->sec_size;
	}
	
	return 0;
}

/*****************************************************************************/
//  Function:  int sf_read(uint32_t addr, uint8_t *p_in, uint32_t len)
//
//  Parameters:
//
//		addr	: spi flash address which is not include the base address
//
//		p_in [in]	: rx buffer
//
//		len	: rx length(byte aligned)
//
//  Return:
//		0		: read ok
//		others	: read fail
//
//  Description:
//       Read data from the flash..
//
/*****************************************************************************/
int sf_read(uint32_t addr, uint8_t *p_in, uint32_t len)
{
	struct spi_flash *p_sf = &g_sf;
	int ret_val = 0;
	uint8_t cmd[5];

	cmd[0] = p_sf->sio_rd_cmd;
	cmd[1] = (addr>>16)&0xFF;
	cmd[2] = (addr>>8) &0xFF;
	cmd[3] = (addr>>0) &0xFF;	
	cmd[4] = 0;					// dummy	
	ret_val = sf_cmd_read(cmd, (4 + p_sf->dummy_byte), p_in, len);
	if (ret_val) 
	{
		info("SF: read failed\n");
	}

	return ret_val;
}

/*****************************************************************************/
//  Function:  int sf_write(uint32_t addr, uint8_t *p_out, uint32_t len)
//
//  Parameters:
//
//		addr			: spi flash address which is not include the base address
//
//		p_out [out]	: tx buffer
//
//		len			: tx length(byte aligned)
//
//  Return:
//		0		: write ok
//		others	: write fail
//
//  Description:
//       Write data to the flash.
//
/*****************************************************************************/
int sf_write(uint32_t addr, uint8_t *p_out, uint32_t len)
{
	struct spi_flash *p_sf = &g_sf;
	int ret_val = 0;
	uint8_t cmd[5];
	uint32_t data_len;
	uint8_t *p_buf = p_out;

	while (len) 
	{
		data_len = min(len, p_sf->page_size);

		// issue write enable command
		ret_val = sf_cmd_write_enable();
		if (ret_val) 
		{
			debug("SF: enabling write failed\n");
			return ret_val;
		}

		// issue write command and data
		cmd[0] = p_sf->sio_pp_cmd;
		cmd[1] = (addr>>16)&0xFF;
		cmd[2] = (addr>>8) &0xFF;
		cmd[3] = (addr>>0) &0xFF;
		ret_val = sf_cmd_write(cmd, 4, p_buf, data_len);
		if (ret_val) 
		{
			debug("SF: write cmd failed\n");
			return ret_val;
		}

		// polling the satus
		ret_val = sf_chk_st();
		if (ret_val < 0) 
		{
			debug("SF: write time out\n");
			return ret_val;
		}
		
		addr += data_len;
		p_buf += data_len;
		len -= data_len;

		info("addr:0x%x, len:0x%x, data_len:0x%x\n", addr, len, data_len);
	}

	return 0;
}

