/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_i2c.h"
#include "cdl_dma.h"
#include "sys_timer.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define I2C_TIMEOUT (300) //ms

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct dw_i2c i2c_dev[NUM_I2C_PORTS];
struct dw_i2c *p_i2c_dev[NUM_I2C_PORTS] = {&i2c_dev[0], &i2c_dev[1]};

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//  Function:  i2c0_int_handler(void *ptr)
//
//  Parameters:
//		ptr [in]	: pointer
//
//  Return:
//		none
//		
//  Description:
//     I2C0 interrupt hadler.
//
/*****************************************************************************/
void i2c0_int_handler(void *ptr)
{
	
}

/*****************************************************************************/
//  Function:  i2c1_int_handler(void *ptr)
//
//  Parameters:
//		ptr [in]	: pointer
//
//  Return:
//		none
//		
//  Description:
//     I2C1 interrupt hadler.
//
/*****************************************************************************/
void i2c1_int_handler(void *ptr)
{
	
}

/*****************************************************************************/
//  Function:  void dw_i2c_writel(struct dw_i2c *p_priv, uint32_t val, uint32_t reg_ofst)
//
//  Parameters:
//		p_priv	[in] 	: structure pointer
//	
//		val			: the value to write 
//
//		reg_ofst		: the offset of a register
//
//  Return:
//		none
//		
//  Description:
//      Write a value to a register.
//
/*****************************************************************************/
void dw_i2c_writel(struct dw_i2c *p_priv, uint32_t val, uint32_t reg_ofst)
{
	__raw_writel(val, (uint32_t)p_priv->regs + reg_ofst);
}

/*****************************************************************************/
//  Function:  uint32_t dw_i2c_readl(struct dw_i2c * p_priv, uint32_t reg_ofst)
//
//  Parameters:
//		p_priv	[in] 	: structure pointer
//
//		reg_ofst		: the offset of a register
//
//  Return:
//		The value from a register
//		
//  Description:
//      Read a value from a register.
//
/*****************************************************************************/
uint32_t dw_i2c_readl(struct dw_i2c *p_priv, uint32_t reg_ofst)
{
	return __raw_readl((uint32_t)p_priv->regs + reg_ofst);
}

/*****************************************************************************/
//  Function:  static void i2c_ctrl(struct dw_i2c *p_priv, E_I2C_ENABLE ctrl)
//
//  Parameters:
//		p_priv	[in] 	: structure pointer
//
//		ctrl			: i2c enable control 
//
//  Return:
//		none
//		
//  Description:
//      Controls whether the DW_apb_i2c is enabled.
//
/*****************************************************************************/
static void i2c_ctrl(struct dw_i2c *p_priv, E_I2C_ENABLE ctrl)
{
	uint32_t reg_val;
	
	reg_val = dw_i2c_readl(p_priv, I2C_ENABLE_OFST);
	reg_val &= ~I2C_ENABLE_MASK;
	reg_val |= I2C_ENABLE_VAL(ctrl);
	dw_i2c_writel(p_priv, reg_val, I2C_ENABLE_OFST);
}

/*****************************************************************************/
//	Function:  static void i2c_set_speed_mode(struct dw_i2c *p_priv, E_I2C_SPEED_MODE mode)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		mode 		: i2c speed mode
//
//	Return:
//		none
//		
//	Description:
//		Set the i2c speed mode (standard, high, fast)
//
/*****************************************************************************/
static void i2c_set_speed_mode(struct dw_i2c *p_priv, E_I2C_SPEED_MODE mode)
{
	uint32_t reg_val;
	uint32_t hcnt, lcnt;
	int32_t micro_fix_val;
	
	// disable i2c
	i2c_ctrl(p_priv, I2C_CTRL_DIS);

	// set speed
	switch (mode) 
	{			
		case I2C_SPEED_HIGH_MODE:
#if 0
				hcnt = (p_priv->pclk/1000000 * MIN_HS_SCL_HIGHTIME) / NANO_TO_MICRO;
				dw_i2c_writel(p_priv, hcnt, I2C_HS_SCL_HCNT_OFST);
				
				lcnt = (p_priv->pclk/1000000 * (294- MIN_HS_SCL_HIGHTIME)) / NANO_TO_MICRO - 43; // rise tim
				dw_i2c_writel(p_priv, lcnt, I2C_HS_SCL_LCNT_OFST);
#else
			{
				int cycle_clk_cnt = p_priv->pclk / p_priv->speed;
				hcnt = cycle_clk_cnt/3;
				dw_i2c_writel(p_priv, hcnt, I2C_HS_SCL_HCNT_OFST);
				
				lcnt = cycle_clk_cnt - hcnt; // rise tim
				dw_i2c_writel(p_priv, lcnt, I2C_HS_SCL_LCNT_OFST);
			}
#endif
			break;
		case I2C_SPEED_STANDARD_MODE:
				hcnt = (p_priv->pclk/1000000 * MIN_SS_SCL_HIGHTIME) / NANO_TO_MICRO;
				dw_i2c_writel(p_priv, hcnt, I2C_SS_SCL_HCNT_OFST);

				micro_fix_val = ((uint32_t)p_priv->regs == I2C0_BASE) ? (-28) : (-32);
				lcnt = (p_priv->pclk/1000000 * (10000 - MIN_SS_SCL_HIGHTIME)) / NANO_TO_MICRO + micro_fix_val; // rise tim
				dw_i2c_writel(p_priv, lcnt, I2C_SS_SCL_LCNT_OFST);

			break;
		case I2C_SPEED_FAST_MODE:
		default:
				hcnt = (p_priv->pclk/1000000 * MIN_FS_SCL_HIGHTIME) / NANO_TO_MICRO;
				dw_i2c_writel(p_priv, hcnt, I2C_FS_SCL_HCNT_OFST);

				micro_fix_val = ((uint32_t)p_priv->regs == I2C0_BASE) ? (-28) : (-36);
				lcnt = (p_priv->pclk/1000000 * (2500- MIN_FS_SCL_HIGHTIME)) / NANO_TO_MICRO + micro_fix_val; // rise tim
				dw_i2c_writel(p_priv, lcnt, I2C_FS_SCL_LCNT_OFST);

			break;
	}

	info("hcnt: %d, lcnt: %d\n", hcnt, lcnt);

	// set speed mode
	reg_val = dw_i2c_readl(p_priv, I2C_CON_OFST);
	reg_val &= ~I2C_SPEED_MASK;
	reg_val |= I2C_SPEED_VAL(mode);
	dw_i2c_writel(p_priv, reg_val, I2C_CON_OFST);

	// set sda setup and hold time
	info("sda hold : 0x%x\n", dw_i2c_readl(p_priv, I2C_SDA_HOLD_OFST));
	info("sda setup : 0x%x\n", dw_i2c_readl(p_priv, I2C_SDA_SETUP_OFST));

	// enable i2c
	i2c_ctrl(p_priv, I2C_CTRL_EN);
}

/*****************************************************************************/
//	Function:  static void i2c_set_bus_speed(struct dw_i2c *p_priv, E_I2C_SPEED speed)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		speed 		: i2c speed mode
//
//	Return:
//		none
//		
//	Description:
//		Set the i2c speed.
//
/*****************************************************************************/
static void i2c_set_bus_speed(struct dw_i2c *p_priv, E_I2C_SPEED speed)
{
	E_I2C_SPEED_MODE mode;
	
	if (speed >= I2C_SPEED_HIGH) 
	{
		mode = I2C_SPEED_HIGH_MODE;
	} 
	else if (speed >= I2C_SPEED_FAST)
	{
		mode = I2C_SPEED_FAST_MODE;
	} 
	else
	{
		mode = I2C_SPEED_STANDARD_MODE;
	}
	p_priv->speed = speed;

	i2c_set_speed_mode(p_priv, mode);
	
}

/*****************************************************************************/
//	Function:  static void i2c_set_slave_addr(struct dw_i2c *p_priv, uint32_t dev)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		dev 		: target i2c address
//
//	Return:
//		none
//		
//	Description:
//		Sets the target slave address.
//
/*****************************************************************************/
static void i2c_set_slave_addr(struct dw_i2c *p_priv, uint32_t dev)
{
	// disable i2c
	i2c_ctrl(p_priv, I2C_CTRL_DIS);

	dev &= 0x3FF;
	dw_i2c_writel(p_priv, dev, I2C_TAR_OFST);

	// enable i2c
	i2c_ctrl(p_priv, I2C_CTRL_EN);
}

/*****************************************************************************/
//	Function:  static void i2c_flush_rx_fifo(struct dw_i2c *p_priv)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//	Return:
//		none
//		
//	Description:
//		Flush the i2c RX FIFO if it is not empty.
//
/*****************************************************************************/
static void i2c_flush_rx_fifo(struct dw_i2c *p_priv)
{
	uint32_t reg_val;
	unsigned long long start = get_timer(0);
	
	while (1) 
	{
		reg_val = dw_i2c_readl(p_priv, I2C_STATUS_OFST);
		if ((reg_val & I2C_RFNE_MASK) != I2C_RFNE_MASK)
		{
			break;
		}
		
		dw_i2c_readl(p_priv, I2C_DATA_CMD_OFST);
		
		if(get_timer(start) > I2C_TIMEOUT) 
		{
			info("i2c flush rxfifo timeout\n");
			break;
		}
	}
}

/*****************************************************************************/
//	Function:  static void i2c_chk_bus(struct dw_i2c *p_priv)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//	Return:
//		none
//		
//	Description:
//		Check the bus until it is ready.
//
/*****************************************************************************/
static int i2c_chk_bus(struct dw_i2c *p_priv)
{
	uint32_t reg_val;
	unsigned long long start_time_bb = get_timer(0);
	u32 raw_stat;
	int ret = 0;

	// check the bus until it's ready.
	while (1) 
	{
		reg_val = dw_i2c_readl(p_priv, I2C_STATUS_OFST);
		if (((reg_val & I2C_MST_ACTIVITY_MASK) != I2C_MST_ACTIVITY_MASK) && 
			(reg_val & I2C_TFE_MASK))
		{
			break;
		}

		// Evaluate timeout
		if (get_timer(start_time_bb) > (unsigned long)(I2C_BYTE_TO_BB)) 
		{
			debug("i2c_chk_bus: Timeout!\n");
			ret = 1;
			break;
		}
	}
	
	// check tx abort, and clear it if exists,consider write fail,and we abort write
	raw_stat = dw_i2c_readl(p_priv, I2C_RAW_INTR_STAT_OFST);
	if (raw_stat & I2C_INT_ST_TX_ABRT_MASK) 
	{
		dw_i2c_readl(p_priv, I2C_CLR_TX_ABRT_OFST);
	}
	
	return ret;
}

/*****************************************************************************/
//	Function:  static int i2c_xfer_init(struct dw_i2c *p_priv, uint32_t dev, uint32_t addr, uint32_t alen)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		dev			: i2c slave address
//
//		addr			: address to read or write
//
//		alen			: address size(n bytes)
//	Return:
//		0 : transfer ok
//	
//		1 : transfer fail
//
//	Description:
//		Transfer address.
//
/*****************************************************************************/
static int i2c_xfer_init(struct dw_i2c *p_priv, uint32_t dev, uint32_t addr, uint32_t alen)
{	
	if (i2c_chk_bus(p_priv)) 
	{
		info("bus busy\n");
		return 1;
	}

	i2c_set_slave_addr(p_priv, dev);
	
	while (alen) 
	{
		alen--;
		
		// high byte address going out first
		dw_i2c_writel(p_priv, (addr >> (alen * 8)) & 0xFF, I2C_DATA_CMD_OFST);
	}
	
	return 0;
}

/*****************************************************************************/
//	Function:  static int i2c_xfer_finish(struct dw_i2c *p_priv)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//	Return:
//		0 : transfer ok
//	
//		1 : transfer fail
//
//	Description:
//		Transfer process finish.
//
/*****************************************************************************/
static int i2c_xfer_finish(struct dw_i2c *p_priv)
{
	uint32_t reg_val;
	u64 start_stop_det = get_timer(0);

	while (1) 
	{
		reg_val = dw_i2c_readl(p_priv, I2C_RAW_INTR_STAT_OFST);
		if (reg_val & I2C_INT_ST_STOP_DET_MASK)
		{
			dw_i2c_readl(p_priv, I2C_CLR_STOP_DET_OFST);
			break;
		} 
		else if (get_timer(start_stop_det) > I2C_STOPDET_TO)
		{
			//debug("i2c_xfer_finish timout\n");
			sys_sw_rst(I2CS_RST);
			sys_sw_rst(I2CM_RST);
			break;
		}
	}

	if (i2c_chk_bus(p_priv)) 
	{
		// maybe send fail, we should abort to pervent something left in tx fifo
		reg_val = dw_i2c_readl(p_priv, I2C_ENABLE_OFST);
		reg_val |= I2C_ABORT_MASK;
		dw_i2c_writel(p_priv, reg_val, I2C_ENABLE_OFST);
		
		debug("Timed out waiting for bus,abort it\n");
	}

	i2c_flush_rx_fifo(p_priv);

	return 0;
}

/*****************************************************************************/
//  Function:  static void i2c_dma_ctrl(struct dw_i2c *p_priv, E_IC_DMA_CR rmda_ctrl, E_IC_DMA_CR tmda_ctrl)
//
//  Parameters:
//		p_priv	[in] 	: structure pointer
//
//		rmda_ctrl		: i2c receive dma control 
//
//		tmda_ctrl		: i2c transmit dma control
//
//  Return:
//		none
//		
//  Description:
//      Configure the receice/transmit dma.
//
/*****************************************************************************/
static void i2c_dma_ctrl(struct dw_i2c *p_priv, E_IC_DMA_CR rmda_ctrl, E_IC_DMA_CR tmda_ctrl)
{
	uint32_t reg_val;
	
	reg_val = dw_i2c_readl(p_priv, I2C_DMA_CR_OFST);
	reg_val &= ~I2C_RDMAE_MASK;
	reg_val |= I2C_RDMAE_VAL(rmda_ctrl);
	
	reg_val &= ~I2C_TDMAE_MASK;
	reg_val |= I2C_TDMAE_VAL(tmda_ctrl);
	dw_i2c_writel(p_priv, reg_val, I2C_DMA_CR_OFST);
}

/*****************************************************************************/
//	Function:  static int i2c_dma_transfer(struct dw_i2c *p_priv, uint8_t *p_data, uint32_t len)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		p_data	[in]	: data buffer to transfer
//
//		len			: data transfer size
//
//	Return:
//		0 : transfer ok
//	
//		1 : transfer fail
//
//	Description:
//		Transfer data with DMA mode.
//
/*****************************************************************************/
static int i2c_dma_transfer(struct dw_i2c *p_priv, uint8_t *p_data, uint32_t len)
{
	dma_i2c_tx(p_priv, DMA_CH0, CH_PRIOR_0, (uint32_t)p_data, len, ST_POLLING);
	
	return 0;
}

/*****************************************************************************/
//	Function:  static int i2c_dma_receive(struct dw_i2c *p_priv, uint8_t *p_data, int len)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		p_data	[in]	: data buffer to transfer
//
//		len			: data transfer length(byte aligned)
//
//	Return:
//		0 : transfer ok
//	
//		1 : transfer fail
//
//	Description:
//		Transfer and receive data with DMA mode.
//
/*****************************************************************************/
static int i2c_dma_receive(struct dw_i2c *p_priv, uint8_t *p_data, int len)
{
	dma_i2c_rx(p_priv, DMA_CH1, CH_PRIOR_0, (uint32_t)p_data, len, ST_POLLING);
	
	return 0;
}

/*****************************************************************************/
//	Function:  int i2c_read(struct dw_i2c *p_priv, uint32_t dev, uint32_t addr, uint32_t alen, uint8_t *p_data, uint32_t dlen)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		dev			: i2c slave address
//
//		addr			: address to read or write
//
//		alen			: address length(byte aligned)
//
//		p_data	[in]	: data buffer to read
//
//		dlen			: data transfer length(byte aligned)
//
//	Return:
//		0 : transfer ok
//	
//		1 : transfer fail
//
//	Description:
//		Read data from the i2c memory.
//
/*****************************************************************************/
int i2c_read(struct dw_i2c *p_priv, uint32_t dev, uint32_t addr, uint32_t alen, uint8_t *p_data, uint32_t dlen)
{
	unsigned long long start_time_rx;
	int ret_val = 0;

	if (i2c_xfer_init(p_priv, dev, addr, alen)) 
	{
		return 1;
	}

	if (p_priv->dma_mode) 
	{
		ret_val = i2c_dma_receive(p_priv, p_data, dlen);
		if (ret_val) 
		{
			debug("Timed out. i2c dma read err\n");
			ret_val = 1;
		}
		
		dw_i2c_writel(p_priv, I2C_CMD|I2C_STOP, I2C_DATA_CMD_OFST);
	} 
	else 
	{
		while (dlen) 
		{
			start_time_rx = get_timer(0);
			
			if (dlen == 1) 
			{
	  			dw_i2c_writel(p_priv, I2C_CMD | I2C_STOP, I2C_DATA_CMD_OFST);
			} 
			else 
			{
				dw_i2c_writel(p_priv, I2C_CMD, I2C_DATA_CMD_OFST);
			}
			
			while (!(dw_i2c_readl(p_priv, I2C_STATUS_OFST) & I2C_RFNE_MASK)) 
			{
				if (get_timer(start_time_rx) > I2C_BYTE_TO) 
				{
					debug("Timed out. i2c read err\n");
					ret_val = 1;
					
					goto end;
				}
			}
			
			*p_data++ = (uint8_t)dw_i2c_readl(p_priv, I2C_DATA_CMD_OFST);
			
			dlen--;
		}
	}

end:
	i2c_xfer_finish(p_priv);
	
	return ret_val;
}

/*****************************************************************************/
//	Function:  int i2c_write(struct dw_i2c *p_priv, uint32_t dev, uint32_t addr, uint32_t alen, uint8_t *p_data, uint32_t dlen)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		dev			: i2c slave address
//
//		addr			: address to read or write
//
//		alen			: address length(byte aligned)
//
//		p_data	[in]	: data buffer to read
//
//		dlen			: data transfer length(byte aligned)
//
//	Return:
//		0 : transfer ok
//	
//		1 : transfer fail
//
//	Description:
//		Write data to the i2c memory.
//
/*****************************************************************************/
int i2c_write(struct dw_i2c *p_priv, uint32_t dev, uint32_t addr, uint32_t alen, uint8_t *p_data, uint32_t dlen)
{
	int nb = dlen;
	unsigned long long start_time_tx;
	int ret_val = 0;

	if (i2c_xfer_init(p_priv, dev, addr, alen)) 
	{
		return 1;
	}

	if (p_priv->dma_mode) 
	{
		ret_val = i2c_dma_transfer(p_priv, p_data, (dlen - 1));
		if (ret_val) 
		{
			debug("Timed out. i2c write err\n");
			ret_val = 1;
		}
		
		dw_i2c_writel(p_priv, (p_data[dlen - 1] | I2C_STOP), I2C_DATA_CMD_OFST);
	} 
	else 
	{			
		start_time_tx = get_timer(0);
		while (dlen) 
		{
			if (dw_i2c_readl(p_priv, I2C_STATUS_OFST) & I2C_TFNF_MASK) 
			{
				if (--dlen == 0) 
				{
	  				dw_i2c_writel(p_priv, (*p_data | I2C_STOP), I2C_DATA_CMD_OFST);
				} 
				else 
				{
	  			  	dw_i2c_writel(p_priv, *p_data, I2C_DATA_CMD_OFST);
				}
				
				p_data++;
				start_time_tx = get_timer(0);
			} 
			else if (get_timer(start_time_tx) > (nb * I2C_BYTE_TO)) 
			{
				debug("Timed out. i2c write err\n");
				ret_val = 1;
				
				break;
			}
		}
	}

	i2c_xfer_finish(p_priv);
	
	return ret_val;
}

/*****************************************************************************/
//	Function:  static void i2c_init_hw(struct dw_i2c *p_priv, E_I2C_SPEED speed)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		speed		: i2c speed mode
//
//	Return:
//		none
//
//	Description:
//		Initialization function.
//
/*****************************************************************************/
static void i2c_init_hw(struct dw_i2c *p_priv, E_I2C_SPEED speed)
{
	uint32_t reg_val = 0;
	int i;

	// disable i2c
	i2c_ctrl(p_priv, I2C_CTRL_DIS);	

	// mask and clear all interrupt
	dw_i2c_writel(p_priv, 0, I2C_INTR_MASK_OFST);
	for (i = 0; i < 11; i++) 
	{
		dw_i2c_readl(p_priv, I2C_CLR_INTR_OFST + (i * 4));
	}

	// config master/slave mode
	if (p_priv->role == I2C_MASTER_MODE) 
	{
		reg_val = I2C_SLAVE_DISABLE_MASK 				| 
					I2C_SPEED_VAL(I2C_SPEED_FAST_MODE) 	| 
					I2C_MASTER_MODE_MASK 				| 
					I2C_RESTART_EN_MASK;
		if (p_priv->master_addr_mode == I2C_10BIT_ADDR)
		{
			reg_val |= I2C_10BITADDR_MASTER_MASK;
		}
		
		dw_i2c_writel(p_priv, reg_val, I2C_CON_OFST);
	} 
	else 
	{
		// set slave addr
		dw_i2c_writel(p_priv, p_priv->slave_addr, I2C_SAR_OFST);

		if (p_priv->slave_addr_mode == I2C_10BIT_ADDR) 
		{
			reg_val |= I2C_10BITADDR_SLAVE_MASK;
		}
		
		reg_val |= I2C_SPEED_VAL(I2C_SPEED_FAST_MODE);
		dw_i2c_writel(p_priv, reg_val, I2C_CON_OFST);
	}
	
    debug("regs:0x%x, I2C_CON_OFST: 0x%08x\n", p_priv->regs, dw_i2c_readl(p_priv, I2C_CON_OFST));

	// set dma 
	i2c_dma_ctrl(p_priv, I2C_DMA_EN, I2C_DMA_EN);

	// set rx/tx FIFO threshold
	dw_i2c_writel(p_priv, 16, I2C_DMA_TDLR_OFST);
	dw_i2c_writel(p_priv, 3, I2C_DMA_RDLR_OFST);

	if (p_priv->role == I2C_MASTER_MODE)
	{
		i2c_set_bus_speed(p_priv, speed);
	}
	
	dw_i2c_writel(p_priv, 0, I2C_INTR_MASK_OFST);

	// Enable i2c
	reg_val = dw_i2c_readl(p_priv, I2C_ENABLE_OFST);
	reg_val |= I2C_ENABLE_MASK;
	reg_val &= ~I2C_TX_CMD_BLOCK_MASK;
	dw_i2c_writel(p_priv, reg_val, I2C_ENABLE_OFST);
}

/*****************************************************************************/
//	Function:  void i2c_init(struct dw_i2c *p_priv, uint32_t base, uint32_t pclk, uint32_t dma_mode)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		base			: the base address of the i2c controller
//
//		pclk			: the input clock of the i2c controller
//
//		dma_mode	: dma mode
//
//	Return:
//		none
//
//	Description:
//		Initialization function.
//
/*****************************************************************************/
void i2c_init(struct dw_i2c *p_priv, uint32_t base, uint32_t pclk, uint32_t dma_mode)
{
	if ((uint32_t)p_priv->regs == I2C0_BASE)
	{
		sys_sw_rst(I2CS_RST);
	}
	else
	{
		sys_sw_rst(I2CM_RST);	
	}
		
	p_priv->regs = base;
	p_priv->pclk = pclk;
	p_priv->dma_mode = dma_mode;
	p_priv->role = I2C_MASTER_MODE;

	// slave mode default para
	p_priv->slave_addr = I2C_SLAVE_DEFAULT_ADDR;
	p_priv->slave_addr_mode = I2C_7BIT_ADDR;
}

/*****************************************************************************/
//	Function:  void i2c_init_master(struct dw_i2c *p_priv, E_I2C_SPEED speed, E_I2C_ADDR_MODE addr_mode)
//
//	Parameters:
//		p_priv	[in]	: structure pointer
//
//		speed		: i2c speed mode
//
//		addr_mode	: address mode
//
//	Return:
//		none
//
//	Description:
//		Initialization i2c master mode.
//
/*****************************************************************************/
void i2c_init_master(struct dw_i2c *p_priv, E_I2C_SPEED speed, E_I2C_ADDR_MODE addr_mode)
{
	p_priv->role = I2C_MASTER_MODE;
	p_priv->master_addr_mode = addr_mode;
	i2c_init_hw(p_priv, speed);
}


void i2c_init_slave(struct dw_i2c *p_priv, uint32_t dev, uint32_t addr_mode, uint32_t reg_width, uint32_t max_len)
{
	p_priv->role = I2C_SLAVE_MODE;
	p_priv->slave_addr = dev;
	p_priv->slave_addr_mode = addr_mode;
	p_priv->slave_reg_width = reg_width;
	p_priv->max_len = max_len;

	mem_set8(p_priv->slave_reg_off, 0, 4);
	p_priv->slave_state = 0;
	p_priv->slave_tx_ptr = 0;
	p_priv->slave_rx_ptr = 0;

	i2c_init_hw(p_priv, 0);
}

/*****************************************************************************/
//	Function: int32_t i2c_slave_recv_data(struct dw_i2c *p_priv, uint8_t *p_data, uint32_t ofst, uint32_t max)
//
//	Parameters:
//		p_priv	[in]		: structure pointer
//
//		p_data	[out]	: buffer to receive data
//
//		ofst				: the offset in the receive buffer
//
//		max				: the max data buffer size
//
//	Return:
//		0 : no receive data
//
//		1 : reached max request len
//
//	Description:
//		Receive data from the i2c.
//
/*****************************************************************************/
int32_t i2c_slave_recv_data(struct dw_i2c *p_priv, uint8_t *p_data, uint32_t ofst, uint32_t max)
{
	uint32_t reg_val;

	do {
		reg_val = dw_i2c_readl(p_priv, I2C_STATUS_OFST);
		
		if (reg_val & I2C_RFNE_MASK) 
		{
			if (p_priv->slave_rx_ptr + ofst < max) 
			{
				p_data[ofst + p_priv->slave_rx_ptr] = (uint8_t)dw_i2c_readl(p_priv, I2C_DATA_CMD_OFST);
				p_priv->slave_rx_ptr++;
			} 
			else 
			{
				return 1;
			}
		}
	} while (reg_val & I2C_RFNE_MASK);
	
	if (p_priv->slave_rx_ptr + ofst < max)
	{
		return 0;
	}
	
	return 0;
}

/*****************************************************************************/
//	Function: int32_t i2c_slave_transfer(struct dw_i2c *p_priv, uint8_t *p_data)
//
//	Parameters:
//		p_priv	[in]		: structure pointer
//
//		p_data	[out]	: buffer to transfer data
//
//	Return:
//		0 : no receive data
//
//		1 : reached max request len
//
//	Description:
//		i2c slave device transfer data.
//
/*****************************************************************************/
#define MZX_BUFFER_SIZE (128*1024)
int32_t i2c_slave_transfer_rx(struct dw_i2c *p_priv, uint8_t *p_data)
{
	uint32_t reg_val;
	uint32_t raw_stat;
	int ret = 0;

	// master write request
	while (1)
	{
		reg_val = dw_i2c_readl(p_priv, I2C_STATUS_OFST);
		if (reg_val & I2C_RFNE_MASK)
		{
			if (p_priv->slave_rx_ptr < MZX_BUFFER_SIZE)
			{
				p_priv->slave_rx_len++;
				p_data[p_priv->slave_rx_ptr++] = (u8)dw_i2c_readl(p_priv, I2C_DATA_CMD_OFST);
				//debug("Slave read %d: @0x%02x\n", p_data[p_priv->slave_rx_ptr-1], p_priv->slave_rx_ptr-1);
			}
			else
			{
				// have read all data,dummy read to clear fifo
				dw_i2c_readl(p_priv, I2C_DATA_CMD_OFST);
				debug("Slave read dummy\n");
			}
		}
		else
		{
			break;
		}
	}

	// master read request
	raw_stat = dw_i2c_readl(p_priv, I2C_RAW_INTR_STAT_OFST);
	if (raw_stat & I2C_INT_ST_STOP_DET_MASK)
	{
		p_priv->slave_tx_ptr = 0;
		p_priv->slave_rx_ptr = 0;
		dw_i2c_readl(p_priv, I2C_CLR_STOP_DET_OFST);
		ret = 1;
	}

	return ret;
}

int32_t i2c_slave_transfer_tx(struct dw_i2c *p_priv, uint8_t *p_data, uint32_t len)
{
	u64 timer_start;
	uint32_t reg_val;
	uint32_t raw_stat;
	unsigned char val;
	int i;
	int ret = 0;

	// master read request
	timer_start = get_timer(0);
	raw_stat = dw_i2c_readl(p_priv, I2C_RAW_INTR_STAT_OFST);
	while (!((raw_stat = dw_i2c_readl(p_priv, I2C_RAW_INTR_STAT_OFST)) & I2C_INT_ST_RD_REQ_MASK) &&
			get_timer(timer_start) < I2C_BYTE_TO_BB)
		{}

	if ((raw_stat = dw_i2c_readl(p_priv, I2C_RAW_INTR_STAT_OFST)) & I2C_INT_ST_RD_REQ_MASK) 
	{
		// READ clear RD_REQ
		dw_i2c_readl(p_priv, I2C_CLR_RD_REQ_OFST);
		
		// reg off not enough,something wrong for the protocol
		if (raw_stat & I2C_INT_ST_START_DET_MASK) 
		{
			p_priv->slave_tx_ptr = 0;
			dw_i2c_readl(p_priv, I2C_CLR_START_DET_OFST);
		}

		// tx fifo not empty when request read,clr the interrupt
		if (raw_stat & I2C_INT_ST_TX_ABRT_MASK) 
		{
			dw_i2c_readl(p_priv, I2C_CLR_TX_ABRT_OFST);
		}

		for (i = 0; i < len; i++) 
		{
			// write until fifo full or master stop
			reg_val = dw_i2c_readl(p_priv, I2C_STATUS_OFST);
			if (reg_val & I2C_TFNF_MASK) 
			{
				val = (p_priv->slave_tx_ptr < MZX_BUFFER_SIZE) ?  p_data[p_priv->slave_tx_ptr++] : 0;
				
				//debug("Slave write:0x%02x @0x%x\n", val, p_priv->slave_tx_ptr);
				dw_i2c_writel(p_priv, val, I2C_DATA_CMD_OFST);
			}
		}
	} 
	else 
	{
		ret = 1;
		printf("slave wait rx req timeout.\n");
	}

	if (raw_stat & I2C_INT_ST_STOP_DET_MASK) 
	{
		p_priv->slave_tx_ptr = 0;
		p_priv->slave_rx_ptr = 0;
		dw_i2c_readl(p_priv, I2C_CLR_STOP_DET_OFST);
	}

	return ret;
}

/*****************************************************************************/
//	Function: uint32_t i2c_get_int_status(struct dw_i2c *p_priv)
//
//	Parameters:
//		p_priv	[in]		: structure pointer
//
//	Return:
//		i2c interrupt status
//
//	Description:
//		Get the i2c interrupt status.
//
/*****************************************************************************/
uint32_t i2c_get_int_status(struct dw_i2c *p_priv)
{
	return dw_i2c_readl(p_priv, I2C_INTR_STAT_OFST);
}

/*****************************************************************************/
//	Function: uint32_t i2c_get_raw_int_status(struct dw_i2c *p_priv)
//
//	Parameters:
//		p_priv	[in]		: structure pointer
//
//	Return:
//		i2c raw interrupt status
//
//	Description:
//		Get the i2c raw interrupt status.
//
/*****************************************************************************/
uint32_t i2c_get_raw_int_status(struct dw_i2c *p_priv)
{
	return dw_i2c_readl(p_priv, I2C_RAW_INTR_STAT_OFST);
}

/*****************************************************************************/
//	Function: void i2c_set_int_mask(struct dw_i2c *p_priv, uint32_t mask)
//
//	Parameters:
//		p_priv	[in]		: structure pointer
//
//		mask			: mask value
//
//	Return:
//		none
//
//	Description:
//		Mask the i2c nterrupt source.
//
/*****************************************************************************/
void i2c_set_int_mask(struct dw_i2c *p_priv, uint32_t mask)
{
	dw_i2c_writel(p_priv, mask, I2C_INTR_MASK_OFST);
}

/*****************************************************************************/
//	Function: void i2c_clear_int(struct dw_i2c *p_priv, uint32_t ofst)
//
//	Parameters:
//		p_priv	[in]		: structure pointer
//
//		ofst				: register offset
//
//	Return:
//		none
//
//	Description:
//		Clear a i2c nterrupt status.
//
/*****************************************************************************/
void i2c_clear_int(struct dw_i2c *p_priv, uint32_t ofst)
{
	dw_i2c_readl(p_priv, ofst);
}

/*****************************************************************************/
//	Function: void i2c_clear_all_int(struct dw_i2c *p_priv)
//
//	Parameters:
//		p_priv	[in]		: structure pointer
//
//	Return:
//		none
//
//	Description:
//		Clear all i2c nterrupt status.
//
/*****************************************************************************/
void i2c_clear_all_int(struct dw_i2c *p_priv)
{
	uint32_t i;
	
	for (i = 0; i < 11; i++) 
	{
		dw_i2c_readl(p_priv, I2C_CLR_INTR_OFST + (i * 4));
	}
}

/*****************************************************************************/
//	Function: uint32_t i2c_get_tx_abrt_source(struct dw_i2c *p_priv)
//
//	Parameters:
//		p_priv	[in]		: structure pointer
//
//	Return:
//		i2c transmit abort status
//
//	Description:
//		Get i2c transmit abort status.
//
/*****************************************************************************/
uint32_t i2c_get_tx_abrt_source(struct dw_i2c *p_priv)
{
	return dw_i2c_readl(p_priv, I2C_TX_ABRT_SOURCE_OFST);
}

/*****************************************************************************/
//	Function: void i2c_tx_abort_recovery(struct dw_i2c *p_priv)
//
//	Parameters:
//		p_priv	[in]		: structure pointer
//
//	Return:
//		none
//
//	Description:
//		i2c tx abort recovery
//
/*****************************************************************************/
void i2c_tx_abort_recovery(struct dw_i2c *p_priv)
{
	uint32_t reg_val;
	//struct dw_dmac* p_dma_rx = &dma_dev_list[p_priv->dmac_idx];
	//struct dw_dmac* p_dma_tx = &dma_dev_list[p_priv->dmac_idx];
	
	//dw_dmac_channel_abort(p_dma_rx, p_priv->dma_ch_rx);
	//dw_dmac_channel_abort(p_dma_tx, p_priv->dma_ch_tx);

	reg_val = dw_i2c_readl(p_priv, I2C_DMA_CR_OFST);
	reg_val &= ~(1<<1);
	dw_i2c_writel(p_priv, reg_val, I2C_DMA_CR_OFST);

	reg_val = dw_i2c_readl(p_priv, I2C_ENABLE_OFST);
	reg_val |= I2C_ABORT_MASK;
	dw_i2c_writel(p_priv, reg_val, I2C_ENABLE_OFST);	
	mdelay(50);
    //while(!(dw_i2c_readl(priv,  I2C_RAW_INTR_STAT_OFST ) & I2C_INT_ST_TX_ABRT_MASK));
	reg_val = dw_i2c_readl(p_priv, I2C_TX_ABRT_SOURCE_OFST);
	dw_i2c_readl(p_priv, I2C_CLR_TX_ABRT_OFST);

    //reg_val = dw_i2c_readl(p_priv, I2C_ENABLE_OFST);
	//reg_val &= ~I2C_ABORT_MASK;
	//dw_i2c_writel(p_priv, reg_val, I2C_ENABLE_OFST);	
}

int i2c_master_tx_slave_rx(struct dw_i2c *p_master, struct dw_i2c *p_slave, 	uint32_t dev, uint32_t addr, uint32_t alen,  
								uint8_t *p_tx, uint8_t *p_rx, uint32_t dlen)
{
	int nb = dlen;
	unsigned long long start_time_tx;
	int ret_val = 0;
	int xfer_len = 0;
	uint32_t data_cmd;
	uint32_t reg_val;
	uint8_t *p_tx_tmp = p_tx; 
	uint8_t *p_rx_tmp = p_rx;

	if (i2c_xfer_init(p_master, dev, addr, alen)) 
	{
		return 1;
	}

	if (p_master->dma_mode) 
	{
		dlen--;
		while (dlen)
		{
			xfer_len = min(dlen, 32);

			// tx
			i2c_dma_transfer(p_master, p_tx_tmp, xfer_len);

			// rx
			i2c_dma_receive(p_slave, p_rx_tmp, xfer_len);

			dlen -= xfer_len;
			p_tx_tmp += xfer_len;
			p_rx_tmp += xfer_len;
		}

		// the last data
		// tx
		dw_i2c_writel(p_master, (*p_tx_tmp | I2C_STOP), I2C_DATA_CMD_OFST);

		// rx
		while (1)
		{
			reg_val = dw_i2c_readl(p_slave, I2C_STATUS_OFST);
			if (reg_val & I2C_RFNE_MASK)
			{
				*p_rx_tmp = (u8)dw_i2c_readl(p_slave, I2C_DATA_CMD_OFST);
				break;
			}
		}	
	} 
	else 
	{
		start_time_tx = get_timer(0);
		while (dlen || (p_slave->slave_rx_len < nb)) 
		{
			// tx
			xfer_len = min(dlen, 16);
			while (xfer_len) 
			{
				if (dw_i2c_readl(p_master, I2C_STATUS_OFST) & I2C_TFNF_MASK) 
				{
					data_cmd = (dlen == 1) ? (*p_tx | I2C_STOP) : *p_tx;
					dw_i2c_writel(p_master, data_cmd, I2C_DATA_CMD_OFST);
					
					dlen--;
					p_tx++;
					xfer_len--;
					start_time_tx = get_timer(0);
				}
				else if (get_timer(start_time_tx) > (nb * I2C_BYTE_TO)) 
				{
					debug("Timed out. i2c write err\n");
					ret_val = 1;
					break;
				}
			}

			// rx
			if (i2c_slave_transfer_rx(p_slave, p_rx)) 
			{
				if(p_slave->slave_rx_len < nb)
				{
					ret_val = 1;
				}
				
				goto done;
			}
		}
	}

done:
	i2c_xfer_finish(p_master);

	info("i2c_master_tx_slave_rx\n");

	return ret_val;
}

int i2c_master_rx_slave_tx(struct dw_i2c *p_master, struct dw_i2c *p_slave, 	uint32_t dev, uint32_t addr, uint32_t alen, 
								uint8_t *p_rx, uint8_t *p_tx, uint32_t dlen)
{
	unsigned long long start_time_rx;
	int ret_val = 0;
	int xfer_len = 0;
	uint32_t data_cmd;
	uint8_t *p_tx_tmp = p_tx; 
	uint8_t *p_rx_tmp = p_rx;	
	
	if (i2c_xfer_init(p_master, dev, addr, alen)) 
	{
		return 1;
	}
	
	dw_i2c_readl(p_slave, I2C_CLR_STOP_DET_OFST);

	if (p_master->dma_mode) 
	{
		dw_i2c_writel(p_master, I2C_CMD, I2C_DATA_CMD_OFST);
		
		while (dlen)
		{
			xfer_len = min(dlen, (I2C_FIFO_SIZE/2));

			// tx
			i2c_dma_transfer(p_slave, p_tx_tmp, xfer_len);

			// master issue clock
			data_cmd = (xfer_len == 1) ? (I2C_CMD | I2C_STOP) : I2C_CMD; 
			for (int i = 0; i < xfer_len; i++) 
			{
				dw_i2c_writel(p_master, data_cmd, I2C_DATA_CMD_OFST);
			}
			
			// rx
			i2c_dma_receive(p_master, p_rx_tmp, xfer_len);

			dlen -= xfer_len;
			p_tx_tmp += xfer_len;
			p_rx_tmp += xfer_len;
		}
	} 
	else 	
	{
		dw_i2c_writel(p_master, I2C_CMD, I2C_DATA_CMD_OFST);
		
		while (dlen) 
		{
			xfer_len = min(dlen, I2C_FIFO_SIZE);
			
			// slave fill tx fifo
			if (i2c_slave_transfer_tx(p_slave, p_tx, xfer_len))
			{
				return 1;
			}

			data_cmd = (xfer_len == 1) ? (I2C_CMD | I2C_STOP) : I2C_CMD; 

			// master rx fifo
			for (int i = 0; i < xfer_len; i++) 
			{
				start_time_rx = get_timer(0);

				dw_i2c_writel(p_master, data_cmd, I2C_DATA_CMD_OFST);

				while (!(dw_i2c_readl(p_master, I2C_STATUS_OFST) & I2C_RFNE_MASK) &&
						get_timer(start_time_rx) < I2C_BYTE_TO)
				{}

				if (dw_i2c_readl(p_master, I2C_STATUS_OFST) & I2C_RFNE_MASK) 
				{
					*p_rx++ = (unsigned char)dw_i2c_readl(p_master, I2C_DATA_CMD_OFST);
				} 
				else 
				{
					debug("Timed out. i2c read err\n");
					ret_val = 1;

					goto done;
				}
			}

			dlen -= xfer_len;
		}
	}

done:
	i2c_xfer_finish(p_master);

	info("i2c_master_rx_slave_tx\n");
	
	return ret_val;
}

