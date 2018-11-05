/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_spi.h"
#include "cdl_sys.h"
#include "cdl_gpio.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct brite_spi g_spi[NUM_SPI_PORTS];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/

/*****************************************************************************/
//  Function:  void spi_int_handler(void *ptr)
//
//  Parameters:
//		ptr [in]	: pointer
//
//  Return:
//		none
//		
//  Description:
//     SPI interrupt hadler.
//
/*****************************************************************************/
void spi_int_handler(void *ptr)
{
	uint32_t int_val = spi_rd_reg(SPI0, SPI_SSPMIS_OFST);

	if (int_val != 0) 
	{
		// write 1 to clear
		spi_wr_reg(SPI0, SPI_SSPICR_OFST, int_val);

	}	
}

/*****************************************************************************/
//  Function:  void spi_wr_reg(E_SPI_PORT port, uint32_t reg_ofst, uint32_t val)
//
//  Parameters:
//		port			: spi port
//
//		reg_ofst		: the offset of a register
//
//		val			: the value to write 
//
//  Return:
//		none
//		
//  Description:
//      Write a value to a register.
//
/*****************************************************************************/
void spi_wr_reg(E_SPI_PORT port, uint32_t reg_ofst, uint32_t val)
{
	__raw_writel(val, (g_spi[port].base + reg_ofst));
}

/*****************************************************************************/
//  Function:  uint32_t spi_rd_reg(E_SPI_PORT port, uint32_t reg_ofst)
//
//  Parameters:
//		port			: spi port
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
uint32_t spi_rd_reg(E_SPI_PORT port, uint32_t reg_ofst)
{
	return __raw_readl(g_spi[port].base + reg_ofst);
}

/*****************************************************************************/
//  Function:  BOOL spi_verify_periph_id(E_SPI_PORT port)
//
//  Parameters:
//		port		: spi port
//
//  Return:
//		FALSE	: verify fail
//		
//		TRUE	: verify ok
//
//  Description:
//      Read and verify the peripheral id.
//
/*****************************************************************************/
BOOL spi_verify_periph_id(E_SPI_PORT port)
{
	uint8_t i, id[4];

	// read the peripheral id from the register
	for (i = 0; i < 4; i++)
	{
		id[i] = spi_rd_reg(port, (SPI_PERIPHID0_OFST + (i<<2)));
		id[i] &= SPI_PERIPHID_MASK;
	}

	// verify the validity of the peripheral id
	if ((id[0] == SPI_PERIPHID0) && 
		(id[1] == SPI_PERIPHID1) && 
		(id[2] == SPI_PERIPHID2) && 
		(id[3] == SPI_PERIPHID3)) 
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************/
//  Function:  BOOL spi_verify_pcell_id(E_SPI_PORT port)
//
//  Parameters:
//		port		: spi port
//
//  Return:
//		FALSE	: verify fail
//		
//		TRUE	: verify ok
//
//  Description:
//      Read and verify the primecell id.
//
/*****************************************************************************/
BOOL spi_verify_pcell_id(E_SPI_PORT port)
{
	uint8_t i, id[4];

	// read the primecell id from the register
	for (i = 0; i < 4; i++)
	{
		id[i] = spi_rd_reg(port, (SPI_PCELLID0_OFST + (i<<2)));
		id[i] &= SPI_PCELLID_MASK;
	}

	// verify the validity of the primecell id
	if ((id[0] == SPI_PCELLID0) && 
		(id[1] == SPI_PCELLID1) && 
		(id[2] == SPI_PCELLID2) && 
		(id[3] == SPI_PCELLID3)) 
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************/
//  Function:  void spi_set_cr0(E_SPI_PORT port, E_SPI_SPH sph, E_SPI_SPO spo, E_SPI_FRF frf, E_SPI_DSS dss)
//
//  Parameters:
//		port		: spi port
//
//		sph		: SSPCLKOUT phase(applicabe to Motorola SPI frame format only)
//
//		spo		: SSPCLKOUT polarity(applicabe to Motorola SPI frame format only)
//
//		frf		: frame format:
//
//		dss		: data size select
//
//  Return:
//		none
//
//  Description:
//       Set spi control 0(clock rate, phase, polarity, frame format, data size select).
//
/*****************************************************************************/
void spi_set_cr0(E_SPI_PORT port, E_SPI_SPH sph, E_SPI_SPO spo, E_SPI_FRF frf, E_SPI_DSS dss)
{
	uint32_t reg_val = spi_rd_reg(port, SPI_SSPCR0_OFST);

	// set sph
	reg_val &= ~SPI_SPH_MASK;
	reg_val |= SPI_SPH_VAL(sph);

	// set spo
	reg_val &= ~SPI_SPO_MASK;
	reg_val |= SPI_SPO_VAL(spo);
	
	// set frf
	reg_val &= ~SPI_FRF_MASK;
	reg_val |= SPI_FRF_VAL(frf);

	// set frame format
	reg_val &= ~SPI_DSS_MASK;
	reg_val |= SPI_DSS_VAL(dss);
	
	spi_wr_reg(port, SPI_SSPCR0_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void spi_set_cr1(E_SPI_PORT port, E_SPI_MS ms, E_SPI_SSE sse)
//
//  Parameters:
//		port		: spi port
//
//		ms		: master or slave mode select
//
//		sse		: synchronous serial port enable
//
//  Return:
//		none
//
//  Description:
//       Set spi control 1(mast select, sync mode).
//
/*****************************************************************************/
void spi_set_cr1(E_SPI_PORT port, E_SPI_MS ms, E_SPI_SSE sse)
{
	uint32_t reg_val = spi_rd_reg(port, SPI_SSPCR1_OFST);

	// set master or slave mode
	reg_val &= ~SPI_MS_MASK;
	reg_val |= SPI_MS_VAL(ms);

	// set syn/async mode
	reg_val &= ~SPI_SSE_MASK;
	reg_val |= SPI_SSE_VAL(sse);	
	
	spi_wr_reg(port, SPI_SSPCR1_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void spi_set_sod(E_SPI_PORT port, E_SPI_SOD sod)
//
//  Parameters:
//		port		: spi port
//
//		sod		: slave-mode output disable
//
//  Return:
//		none
//
//  Description:
//       Set spi slave output enable/disable.
//
/*****************************************************************************/
void spi_set_sod(E_SPI_PORT port, E_SPI_SOD sod)
{
	uint32_t reg_val = spi_rd_reg(port, SPI_SSPCR1_OFST);
	
	// set slave mode output
	reg_val &= ~SPI_SOD_MASK;
	reg_val |= SPI_SOD_VAL(sod);

	spi_wr_reg(port, SPI_SSPCR1_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void spi_set_lbm(E_SPI_PORT port, E_SPI_LBM lbm)
//
//  Parameters:
//		port		: spi port
//
//		lbm		: loop back mode
//
//  Return:
//		none
//
//  Description:
//       Set spi loop back mode.
//
/*****************************************************************************/
void spi_set_lbm(E_SPI_PORT port, E_SPI_LBM lbm)
{
	uint32_t reg_val = spi_rd_reg(port, SPI_SSPCR1_OFST);
	
	// set slave mode output
	reg_val &= ~SPI_LBM_MASK;
	reg_val |= SPI_LBM_VAL(lbm);

	spi_wr_reg(port, SPI_SSPCR1_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  uint32_t spi_get_st(E_SPI_PORT port)
//
//  Parameters:
//		port	: spi port
//
//  Return:
//		The FIFO fill status and the PrimeCell SSP busy status
//
//  Description:
//       Get the FIFO fill status and the PrimeCell SSP busy status
//
/*****************************************************************************/
uint32_t spi_get_st(E_SPI_PORT port)
{
	uint32_t reg_val = spi_rd_reg(port, SPI_SSPSR_OFST);
	
	reg_val &= SPI_ST_MASK;
	
	return reg_val;
}

/*****************************************************************************/
//  Function:  void spi_set_div(E_SPI_PORT port, uint32_t div)
//
//  Parameters:
//		port	: spi port
//
//		div	: divisor
//
//  Return:
//		none
//
//  Description:
//       Set the divison factor.
//
/*****************************************************************************/
void spi_set_div(E_SPI_PORT port, uint32_t div)
{
	spi_wr_reg(port, SPI_SSPCPSR_OFST, div);
}

/*****************************************************************************/
//  Function:  uint32_t spi_get_ris(E_SPI_PORT port)
//
//  Parameters:
//		port	: spi port
//
//  Return:
//		The raw interrupt status
//
//  Description:
//       Get the raw interrupt status.
//
/*****************************************************************************/
uint32_t spi_get_ris(E_SPI_PORT port)
{
	uint32_t reg_val = spi_rd_reg(port, SPI_SSPRIS_OFST);
	
	return reg_val;
}

/*****************************************************************************/
//  Function:  uint32_t spi_get_mis(E_SPI_PORT port)
//
//  Parameters:
//		port	: spi port
//
//  Return:
//		The pin masked interrupt status
//
//  Description:
//       Get the masked interrupt status.
//
/*****************************************************************************/
uint32_t spi_get_mis(E_SPI_PORT port)
{
	uint32_t reg_val = spi_rd_reg(port, SPI_SSPMIS_OFST);
		
	return reg_val;
}

/*****************************************************************************/
//  Function:  void spi_clr_int(E_SPI_PORT port, uint32_t mask)
//
//  Parameters:
//		port		: spi port
//
//		mask	: interrupt mask
//					= SPI_RORIC_MASK
//					= SPI_RTIC_MASK
//  Return:
//		none
//
//  Description:
//       Clear the interrupt output from the counter.
//
/*****************************************************************************/
void spi_clr_int(E_SPI_PORT port, uint32_t mask)
{
	// write 1 to clear
	spi_wr_reg(port, SPI_SSPICR_OFST, mask);
}

/*****************************************************************************/
//  Function:  void spi_set_dma_ctrl(E_SPI_PORT port, E_SPI_DMACR tx_dma, E_SPI_DMACR rx_dma)
//
//  Parameters:
//		port		: spi port
//
//		tx_dma	: tx dma enable
//
//		rx_dma	: rx dma enable
//  Return:
//		none
//
//  Description:
//       Enable/Disable rx or tx dma.
//
/*****************************************************************************/
void spi_set_dma_ctrl(E_SPI_PORT port, E_SPI_DMACR tx_dma, E_SPI_DMACR rx_dma)
{
	uint32_t reg_val = SPI_RXDMAE_VAL(rx_dma) | SPI_TXDMAE_VAL(tx_dma);
	
	// write 1 to clear
	spi_wr_reg(port, SPI_SSPDMACR_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void spi_set_speed(E_SPI_PORT port, uint32_t clk)
//
//  Parameters:
//		port		: spi port
//
//		clk		: specify the SPI clock rate in Hz. 
//
//  Return:
//		none
//		
//  Description:
//		Set SPI speed(HZ).
//
/*****************************************************************************/
void spi_set_speed(E_SPI_PORT port, uint32_t clk)
{
	uint32_t m, n;
	uint32_t reg_val;

	if (clk > g_spi[port].pclk)
	{
		m = 0;
		n = 2;
	}
	else
	{
		if ((g_spi[port].pclk / clk) > 254)
		{
			m = (g_spi[port].pclk / clk) / 254;
			n = g_spi[port].pclk / (clk * (m + 1));
		}
		else
		{
			m = 0;
			n = g_spi[port].pclk / clk;
		}
	}

	// set serial clock rate
	reg_val = spi_rd_reg(port, SPI_SSPCR0_OFST);
	reg_val &= ~SPI_SCR_MASK;
	reg_val |= SPI_SCR_VAL(m);
	spi_wr_reg(port, SPI_SSPCR0_OFST, reg_val);

	// set clock prescale(must be an even number from 2 to 254)
	reg_val = n & SPI_CPSDVSR_MASK;
	spi_wr_reg(port, SPI_SSPCPSR_OFST, reg_val);
}

/*****************************************************************************/
//  Function: void spi_set_dss(E_SPI_PORT port, E_SPI_DSS dss)
//
//  Parameters:
//		port		: spi port
//
//		dss		: data size select
//
//  Return:
//		none
//		
//  Description:
//      SPI set dss.
//
/*****************************************************************************/
void spi_set_dss(E_SPI_PORT port, E_SPI_DSS dss)
{	
	uint32_t reg_val = spi_rd_reg(port, SPI_SSPCR0_OFST);

	// set frame format
	reg_val &= ~SPI_DSS_MASK;
	reg_val |= SPI_DSS_VAL(dss);
	
	spi_wr_reg(port, SPI_SSPCR0_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  static void spi_empty_fifo(E_SPI_PORT port) 
//
//  Parameters:
//		port	: spi port
//
//  Return:
//		none
//
//  Description:
//       Empty the fifo.
//
/*****************************************************************************/
void spi_empty_fifo(E_SPI_PORT port) 
{
	uint32_t reg_val;
	u64 start = get_timer(0);

	while (1)
	{
		reg_val = spi_rd_reg(port, SPI_SSPSR_OFST);
		if (reg_val & SPI_RNE_MASK)
		{	
			// dummy read data to clear the FIFO
			spi_rd_reg(port, SPI_SSPDR_OFST);

			if (get_timer(start) > RX_TIMEOUT)
			{
				info("SPI FIFO EMPTY FAIL\n");
				break;
			}
		}
		else
		{
			break;
		}
	}
}


/*****************************************************************************/
//  Function:  void spi_cs_ctrl(E_SPI_PORT port, E_SPI_CS_CTRL ctrl)
//
//  Parameters:
//		port	: spi port
//
//		ctrl	: cs pin control
//
//  Return:
//		none
//
//  Description:
//       Control CS.
//
/*****************************************************************************/
void spi_cs_ctrl(E_SPI_PORT port, E_SPI_CS_CTRL ctrl)
{
	E_GPIO_PORT gpio_port = GPIO0;
	E_PIN_PORT pin = PIN_3;
		
	// initialize gpio pin
	gpio_set_ctrl(gpio_port, pin, GPIO_CTRL_SW);
	gpio_set_ie(gpio_port, pin, GPIO_IE_DIS);
	gpio_set_ic(gpio_port, pin); 
	gpio_set_dir(gpio_port, pin, GPIO_OUTPUT);
	
	// output level on the pin
	gpio_set_output(gpio_port, pin, ctrl);
}

/*****************************************************************************/
//  Function:  void spi_init(E_SPI_PORT port, uint32_t clk, E_SPI_FRF type, E_SPI_MS ms, 
//					E_SPI_MODE mode, uint32_t ds, E_DT_MODE dt_mode)
//
//  Parameters:
//		port			: spi port
//
//		clk			:  specify the SPI clock rate in Hz. 
//
//		type			: frame format
//
//		ms			: master or slave mode select
//
//		mode		: spi mode
//
//		ds			: data size
//
//		dt_mode		: data transfer by cpu/dma mode
//
//  Return:
//		none
//
//  Description:
//       Restart the controller, disable all interrupts, clean rx fifo.
//
/*****************************************************************************/
void spi_init(E_SPI_PORT port, uint32_t clk, E_SPI_FRF type, E_SPI_MS ms, 
					E_SPI_MODE mode, uint32_t ds, E_DT_MODE dt_mode)
{	
	struct brite_spi *priv = &g_spi[port];
	E_SPI_SPH sph; 
	E_SPI_SPO spo;	

	sys_sw_rst(SPI_RST);

	spi_verify_periph_id(port);

	spi_verify_pcell_id(port);

	// initialize the parameter
	priv->clk = clk;
	priv->type = type;
	priv->ms = ms;
	priv->mode = mode;
	priv->ds = ds;	
	priv->dt_mode = dt_mode;
	priv->port = port;
	// set sync mode
	priv->sync_mode = SPI_SYNC_MODE;
		
	// disable chip
	spi_cs_ctrl(port, SPI_CS_DIS);

	// disable all interrupt
	spi_wr_reg(port, SPI_SSPIMSC_OFST, 0);

	// clear all interrupt
	spi_clr_int(port, (SPI_RORIC_MASK|SPI_RTIC_MASK));

	// software control cs, can't use ti mode, it force low and one high pulse before valid data
	// TI mode or normal mode
	// gctl |= (priv->type ? SPI_TI_MODE : 0);

	// calculate step(transfer out/in data should orgnized according this)
	// 4-8   bit: one transfer one byte (only 4-8bit valid)
	// 9-16 bit: one transfer two bytes (only 9-16bit valid)
	// 17-32bit: one transfer four bytes(only 17-32bit valid)
	if ((priv->ds >= 4) && (priv->ds <= 8)) 
	{
		priv->step = 1;
	} 
	else if ((priv->ds >= 9) && (priv->ds <= 16)) 
	{
		priv->step = 2;
	} 
	else 
	{
		priv->step = 4;
	}

	// set baud rate
	spi_set_speed(port, priv->clk);
	
	// initialize cr0
	sph = priv->mode & 1; 
	spo = (priv->mode>>1) & 1;	
	spi_set_cr0(port, sph, spo, priv->type, (priv->ds-1));
	
	// initialize cr1
	spi_set_cr1(port, priv->ms, priv->sync_mode);
	spi_set_sod(port, SPI_SOD_EN);	
	spi_set_lbm(port, SPI_NORMAL_MODE);	
	spi_set_dma_ctrl(port, SPI_DMA_EN, SPI_DMA_EN);

	// clear the empty fifo
	spi_empty_fifo(port);
}

/*****************************************************************************/
//	Function:  static int spi_tx_cpu_poll(E_SPI_PORT port)
//
//	Parameters:
//		port		: spi port
//
//	Return:
//		0		: transfer ok
//
//		others	: transfer fail
//
//	Description:
//		 Transmit data with cpu poling mode.
//
/*****************************************************************************/
static int spi_tx_cpu_poll(E_SPI_PORT port)
{
	struct brite_spi *priv = &g_spi[port];
	uint32_t reg_val;
	uint32_t txw = 0;
		
	// Wait for tx fifo not full
	reg_val = spi_rd_reg(port, SPI_SSPSR_OFST);
	if (!(reg_val & SPI_TNF_MASK)) 
	{
		return 0;
	}

	while ((priv->tx < priv->tx_end) && (reg_val & SPI_TNF_MASK)) 
	{		
		// Set the tx word if the transfer's original "tx" is not null
		// check original "tx" buffer is not null
		if ((uint32_t)priv->tx_end > priv->len)
		{
			if (priv->step == 1) 
			{
				txw = *(uint8_t *)(priv->tx);
			} 
			else if (priv->step == 2) 
			{
				txw = *(uint16_t *)(priv->tx);
			} 
			else 
			{
				txw = *(uint32_t *)(priv->tx);
			}
		}
		
		spi_wr_reg(port, SPI_SSPDR_OFST, txw);

		priv->tx += priv->step;
		
		reg_val = spi_rd_reg(port, SPI_SSPSR_OFST);
	}
	
	return 0;
}

/*****************************************************************************/
//	Function:  static int spi_tx_cpu_int(E_SPI_PORT port)
//
//	Parameters:
//		port		: spi port
//
//	Return:
//		0		: transfer ok
//
//		others	: transfer fail
//
//	Description:
//		 Transmit data with cpu interrupt mode.
//
/*****************************************************************************/
static int spi_tx_cpu_int(E_SPI_PORT port)
{
	return 0;
}


/*****************************************************************************/
//	Function:  static int spi_tx_dma_poll(E_SPI_PORT port)
//
//	Parameters:
//		port		: spi port
//
//	Return:
//		0		: transfer ok
//
//		others	: transfer fail
//
//	Description:
//		 Transmit data with dma poling mode.
//
/*****************************************************************************/
static int spi_tx_dma_poll(E_SPI_PORT port)
{
	struct brite_spi *priv = &g_spi[port];
	
	if (DMA_ST_DONE != dma_spi_tx(DMA_CH0, CH_PRIOR_0, (uint32_t)priv->tx, priv->len, ST_POLLING))
	{
		return 1;
	}
	
	return 0;
}

/*****************************************************************************/
//	Function:  static int spi_tx_dma_int(E_SPI_PORT port)
//
//	Parameters:
//		port		: spi port
//
//	Return:
//		0		: transfer ok
//
//		others	: transfer fail
//
//	Description:
//		 Transmit data with dma interrupt mode.
//
/*****************************************************************************/
static int spi_tx_dma_int(E_SPI_PORT port)
{
	return 0;
}


/*****************************************************************************/
//	Function:  static int spi_rx_cpu_poll(E_SPI_PORT port)
//
//	Parameters:
//		port		: spi port
//
//	Return:
//		0		: transfer ok
//
//		others	: transfer fail
//
//	Description:
//		 Receive data with cpu poling mode.
//
// check status, we must continue the loop when SPI_RXAVL
// (receive fifo have data);but only check RXAVL may loss
// data, because tx/rx may slow than this loop so when we
// check here, tx fifo not complete send, RXAVL may not
// ready now
/*****************************************************************************/
static int spi_rx_cpu_poll(E_SPI_PORT port)
{
	struct brite_spi *priv = &g_spi[port];
	uint32_t reg_val;
	u64 start = get_timer(0);
	uint32_t rxw = 0;

	// Wait for rx data to be ready
	reg_val = spi_rd_reg(port, SPI_SSPSR_OFST);
	while (!(reg_val & SPI_RNE_MASK)) 
	{
		if (priv->ms == SPI_MASTER) 
		{ 
			// master
			if (get_timer(start) > RX_TIMEOUT) 
			{
				return -ETIMEDOUT;
			}
		}
		
		reg_val = spi_rd_reg(port, SPI_SSPSR_OFST);
	}

	while ((priv->rx < priv->rx_end) && (reg_val & SPI_RNE_MASK)) 
	{
		rxw = spi_rd_reg(port, SPI_SSPDR_OFST);

		// check original "rx" buffer is not null
		if ((uint32_t)priv->rx_end > priv->len)
		{
			if (priv->step == 1) 
			{
				*(uint8_t *)(priv->rx) = rxw;
			} 
			else if (priv->step == 2) 
			{
				*(uint16_t *)(priv->rx) = rxw;
			} 
			else 
			{
				*(uint32_t *)(priv->rx) = rxw;
			}
		}
		
		priv->rx += priv->step;

		// check status
		reg_val = spi_rd_reg(port, SPI_SSPSR_OFST);
	}

	return 0;
}

/*****************************************************************************/
//	Function:  static int spi_rx_cpu_int(E_SPI_PORT port)
//
//	Parameters:
//		port		: spi port
//
//	Return:
//		0		: transfer ok
//
//		others	: transfer fail
//
//	Description:
//		 Receive data with cpu interrupt mode.
//
/*****************************************************************************/
static int spi_rx_cpu_int(E_SPI_PORT port)
{
	return 0;
}

/*****************************************************************************/
//	Function:  static int spi_rx_dma_int(E_SPI_PORT port)
//
//	Parameters:
//		port		: spi port
//
//	Return:
//		0		: transfer ok
//
//		others	: transfer fail
//
//	Description:
//		 Receive data with dma interrupt mode.
//
/*****************************************************************************/
static int spi_rx_dma_int(E_SPI_PORT port)
{
	return 0;
}


/*****************************************************************************/
//  Function:  static int spi_xfer_cpu(E_SPI_PORT port)
//
//  Parameters:
//		port		: spi port
//
//  Return:
//		0		: transfer ok
//
//		others	: transfer fail
//
//  Description:
//       Transfer data with cpu poling/interrupt mode.
//
/*****************************************************************************/
static int spi_xfer_cpu(E_SPI_PORT port)
{
	struct brite_spi *priv = &g_spi[port];
	int ret = 0;

	if (priv->int_en)
	{
		while (priv->rx_end > priv->rx) 
		{
			spi_tx_cpu_int(port);
		
			ret = spi_rx_cpu_int(port);
			if (ret < 0) 
			{
				return ret;
			}
		}		
	}
	else
	{
		while (priv->rx_end > priv->rx) 
		{
			spi_tx_cpu_poll(port);

			ret = spi_rx_cpu_poll(port);
			if (ret < 0) 
			{
				return ret;
			}		
		}		
	}
	
	return 0;
}

/*****************************************************************************/
//  Function:  static int spi_xfer_dma(E_SPI_PORT port)
//
//  Parameters:
//		port			: spi port
//
//  Return:
//		0		: transfer ok
//
//		others	: transfer fail
//
//  Description:
//       Transfer data with dma poling/interrupt mode.
//
/*****************************************************************************/
static int spi_xfer_dma(E_SPI_PORT port)
{
	struct brite_spi *priv = &g_spi[port];
	int ret = 0;

	if (priv->int_en)
	{
		while (priv->rx_end > priv->rx) 
		{
			spi_tx_dma_int(port);
		
			ret = spi_rx_dma_int(port);
			if (ret < 0) 
			{
				return ret;
			}
		}		
	}
	else
	{
		if (DMA_ST_DONE != dma_spi_tx(DMA_CH0, CH_PRIOR_0, (uint32_t)priv->tx, priv->len, ST_NO_POLLING))
		{
			return 1;
		}
	
		if (DMA_ST_DONE != dma_spi_rx(DMA_CH1, CH_PRIOR_0, (uint32_t)priv->rx, priv->len, ST_NO_POLLING))
		{
			return 1;
		}
	
		dma_polling_st(DMA0, DMA_CH0);
		dma_polling_st(DMA0, DMA_CH1);
	}
	
	return 0;
}

/*****************************************************************************/
//  Function:  int spi_xfer(E_SPI_PORT port, uint8_t *p_out, uint8_t *p_in, uint32_t len, E_SPI_XFER_FLAG flag)
//
//  Parameters:
//		port			: spi port
//
//		p_out [out]	: tx buffer
//
//		p_in [in]		: rx buffer
//
//		len			:  transfer data length(byte aligned)
//
//		flag			: transmit flag
//
//  Return:
//		0		: transfer ok
//		others	: transfer fail
//
//  Description:
//       Transfer data with cpu/dma polling/interrupt mode.
//
/*****************************************************************************/
int spi_xfer(E_SPI_PORT port, uint8_t *p_out, uint8_t *p_in, uint32_t len, E_SPI_XFER_FLAG flag)
{
	struct brite_spi *priv = &g_spi[port];
	int ret = 0;

	// initialize data transfer parameter
	priv->len = len;
	priv->tx = p_out;
	priv->tx_end = priv->tx + priv->len;
	priv->rx = p_in;
	priv->rx_end = priv->rx + priv->len;
	
	// start transfer in a polling loop
	if (priv->dt_mode == DT_DMA) 
	{
		ret = spi_xfer_dma(port);
	} 
	else 
	{
		ret = spi_xfer_cpu(port);
	}

	return ret;
}

int spi_xfer_tx_withoutDMA(E_SPI_PORT port, const uint8_t *p_out, uint32_t len, uint32_t timeout)
{
	int i=0;
	//u64 start = get_timer(0);
	uint32_t reg_val;	

	spi_cs_ctrl(port, SPI_CS_EN);
	//while (i < len || j < len) 
	while (i < len) 
	{
		reg_val = spi_rd_reg(port, SPI_SSPSR_OFST);
		//if (get_timer(start) > timeout) 
		{
		//	spi_cs_ctrl(port, SPI_CS_DIS);
		//	return -1;
		}

		if(reg_val & SPI_TNF_MASK)
		{
			spi_wr_reg(port, SPI_SSPDR_OFST, p_out[i++]);			
		}
		//if(reg_val & SPI_RNE_MASK)
		{
		//	spi_rd_reg(port, SPI_SSPDR_OFST);
		//	j++;
		}		
	}
	
	//wait for idle
	while(spi_rd_reg(port, SPI_SSPSR_OFST) & SPI_BSY_MASK);	

	spi_cs_ctrl(port, SPI_CS_DIS);

	spi_empty_fifo(port);
	
	return 0;
}


