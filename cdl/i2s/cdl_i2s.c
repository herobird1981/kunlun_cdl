/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_i2s.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct brite_i2s g_i2s[NUM_I2S_PORTS];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//  Function:  static inline void i2s_write_reg(uint32_t base, uint32_t reg_ofst, uint32_t val)
//
//  Parameters:
//		base			: base address
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
static inline void i2s_write_reg(uint32_t base, uint32_t reg_ofst, uint32_t val)
{
	writel(val, base + reg_ofst);
}

/*****************************************************************************/
//  Function:  static inline u32 i2s_read_reg(uint32_t base, uint32_t reg_ofst)
//
//  Parameters:
//		base			: base address
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
static inline u32 i2s_read_reg(uint32_t base, uint32_t reg_ofst)
{
	return readl(base + reg_ofst);
}

/*****************************************************************************/
//  Function:  static inline void i2s_disable_channels(struct brite_i2s *priv)
//
//  Parameters:
//		priv	: device pointer 
//
//  Return:
//		none
//		
//  Description:
//       Disalbe all channels.
//
/*****************************************************************************/
static inline void i2s_disable_channels(struct brite_i2s *priv)
{
	u32 ch = 0;

	if ((priv->capability & DWC_I2S_PLAY) && (priv->stream_dir == SNDRV_PCM_STREAM_PLAYBACK)) 
	{		
		for (ch = 0; ch < priv->tx_max_channels; ch++)
		{
			// Transmit channel disable
			i2s_write_reg(priv->base, I2S_TER_OFST(ch), I2S_DIS);
		}	
	} 
	else if ((priv->capability & DWC_I2S_RECORD) && (priv->stream_dir == SNDRV_PCM_STREAM_CAPTURE)) 
	{
		for (ch = 0; ch < priv->rx_max_channels; ch++)
		{
			// Receive channel disable
			i2s_write_reg(priv->base, I2S_RER_OFST(ch), I2S_DIS);
		}
	} 
	else 
	{
		debug("i2s_disable_channels: unsupport\n");
	}
}

/*****************************************************************************/
//  Function:  static inline void i2s_clear_irqs(struct brite_i2s *priv)
//
//  Parameters:
//		priv	: device pointer 
//
//  Return:
//		none
//		
//  Description:
//       Clear the interrupt bit of each channel.
//
/*****************************************************************************/
static inline void i2s_clear_irqs(struct brite_i2s *priv)
{
	u32 ch = 0;

	if (priv->capability & DWC_I2S_PLAY) 
	{
		for (ch = 0; ch < priv->ch_num; ch++)
		{
			// Read this bit to clear the TX FIFO Data Overrun interrupt.
			i2s_read_reg(priv->base, I2S_TOR_OFST(priv->hw_channel_map[ch]));
		}
			
	} 
	else if(priv->capability & DWC_I2S_RECORD) 
	{
		for (ch = 0; ch < priv->ch_num; ch++)
		{
			// Read this bit to clear the RX FIFO Data Overrun interrupt.
			i2s_read_reg(priv->base, I2S_ROR_OFST(priv->hw_channel_map[ch]));
		}
	} 
	else 
	{
		debug("i2s_clear_irqs: unsupport\n");
	}
}

/*****************************************************************************/
//  Function:  static void i2s_start(struct brite_i2s *priv)
//
//  Parameters:
//		priv	: device pointer 
//
//  Return:
//		none
//		
//  Description:
//       Start  the i2s channels.
//
/*****************************************************************************/
static void i2s_start(struct brite_i2s *priv)
{
	u32 i, ch, irq;

	i2s_clear_irqs(priv);

	i2s_write_reg(priv->base, I2S_IER_OFST, I2S_EN);
	
	if (priv->capability & DWC_I2S_PLAY) 
	{
		for (i = 0; i < priv->ch_num; i++) 
		{
			ch = priv->hw_channel_map[i];
			
			i2s_write_reg(priv->base, I2S_ITER_OFST, I2S_EN);
			i2s_write_reg(priv->base, I2S_TFCR_OFST(ch), priv->tx_fifo_empty_trig);

			irq = i2s_read_reg(priv->base, I2S_IMR_OFST(ch));
			irq &= ~I2S_IMR_TXFE_MASK;
			i2s_write_reg(priv->base, I2S_IMR_OFST(ch), irq);
		}

		//info("tx_fifo_empty_trig: %x\n", priv->tx_fifo_empty_trig);
		//info("IMR: %x\n", i2s_read_reg(priv->base, I2S_IMR_OFST(ch)));
	} 
	else if (priv->capability & DWC_I2S_RECORD) 
	{
		for (i = 0; i < priv->ch_num; i++) 
		{
			ch = priv->hw_channel_map[i];
			
			i2s_write_reg(priv->base, I2S_IRER_OFST, I2S_EN);
			i2s_write_reg(priv->base, I2S_RFCR_OFST(ch), priv->rx_fifo_avail_trig);

			irq = i2s_read_reg(priv->base, I2S_IMR_OFST(ch));
			irq &= ~I2S_IMR_RXDA_MASK;
			i2s_write_reg(priv->base, I2S_IMR_OFST(ch), irq);

			//info("rx_fifo_avail_trig: %x\n", priv->rx_fifo_avail_trig);
			//info("IMR: %x\n", i2s_read_reg(priv->base, I2S_IMR_OFST(ch)));
		}
	} 
	else 
	{
		debug("i2s_start: unsupport\n");
	}
}

/*****************************************************************************/
//  Function:  static void i2s_stop(struct brite_i2s *priv)
//
//  Parameters:
//		priv	: device pointer 
//
//  Return:
//		none
//		
//  Description:
//       Stop  the i2s channels.
//
/*****************************************************************************/
void i2s_stop(struct brite_i2s *priv)
{
	u32 ch = 0, irq;

	i2s_clear_irqs(priv);
	
	if (priv->capability & DWC_I2S_PLAY) 
	{
		i2s_write_reg(priv->base, I2S_ITER_OFST, I2S_DIS);

		for (ch = 0; ch < priv->ch_num; ch++) 
		{
			irq = i2s_read_reg(priv->base, I2S_IMR_OFST(priv->hw_channel_map[ch]));
			i2s_write_reg(priv->base, I2S_IMR_OFST(priv->hw_channel_map[ch]), irq | 0x30);
		}
	} 
	else if (priv->capability & DWC_I2S_RECORD) 
	{
		i2s_write_reg(priv->base, I2S_IRER_OFST, I2S_DIS);

		for (ch = 0; ch < priv->ch_num; ch++) 
		{
			irq = i2s_read_reg(priv->base, I2S_IMR_OFST(priv->hw_channel_map[ch]));
			i2s_write_reg(priv->base, I2S_IMR_OFST(priv->hw_channel_map[ch]), irq | 0x03);
		}
	} 
	else 
	{
		debug("i2s_stop: unsupport\n");
	}

	if ((priv->capability & DW_I2S_MASTER) && (priv->role == WORK_AS_MASTER))
	{
		i2s_write_reg(priv->base, I2S_CER_OFST, I2S_DIS);
	}

	i2s_write_reg(priv->base, I2S_IER_OFST, I2S_DIS);
}

/*****************************************************************************/
//  Function:  static int i2s_set_hw_params(struct brite_i2s *priv)
//
//  Parameters:
//		priv	: device pointer 
//
//  Return:
//		none
//		
//  Description:
//       Set  the i2s parameter of the channels.
//
/*****************************************************************************/
static int i2s_set_hw_params(struct brite_i2s *priv)
{
	u32 ccr, xfer_resolution, i;

	priv->bits_per_sample = priv->sample_format;
	switch (priv->sample_format) 
	{
		case SNDRV_PCM_FORMAT_S16_LE:
				ccr = I2S_WSS_VAL(I2S_WSS_16_CLK_CYCLE);
				xfer_resolution = I2S_WSS_16BIT_RES;
			break;
		case SNDRV_PCM_FORMAT_S24_LE:
				ccr = I2S_WSS_VAL(I2S_WSS_24_CLK_CYCLE);
				xfer_resolution = I2S_WSS_24BIT_RES;
			break;
		case SNDRV_PCM_FORMAT_S32_LE:
				ccr = I2S_WSS_VAL(I2S_WSS_32_CLK_CYCLE);
				xfer_resolution = I2S_WSS_32BIT_RES;
			break;
		default:
				debug("designware-i2s: unsuppted PCM fmt");
			return -EINVAL;
	}

	switch (priv->ch_num*2) 
	{
		case EIGHT_CHANNEL_SUPPORT:
		case SIX_CHANNEL_SUPPORT:
		case FOUR_CHANNEL_SUPPORT:
		case TWO_CHANNEL_SUPPORT:
			break;
		default:
				debug("channel not supported\n");
			return -EINVAL;
	}

	i2s_disable_channels(priv);
	
	for (i = 0; i < priv->ch_num; i++) 
	{
		int ch = priv->hw_channel_map[i];
		
		if (priv->capability & DWC_I2S_PLAY) 
		{
			i2s_write_reg(priv->base, I2S_TCR_OFST(ch), xfer_resolution);
			i2s_write_reg(priv->base, I2S_TFF_OFST(ch), 1);
			i2s_write_reg(priv->base, I2S_TER_OFST(ch), I2S_EN);
		} 
		else if (priv->capability & DWC_I2S_RECORD) 
		{
			i2s_write_reg(priv->base, I2S_RCR_OFST(ch), xfer_resolution);
			i2s_write_reg(priv->base, I2S_RFF_OFST(ch), 1);
			i2s_write_reg(priv->base, I2S_RER_OFST(ch), I2S_EN);
		} 
		else 
		{
			debug("i2s_set_hw_params: unsupport\n");
		}
	}

	i2s_write_reg(priv->base, I2S_CCR_OFST, ccr);

	return 0;
}

/*****************************************************************************/
//  Function:  static void i2s_fifo_reset(struct brite_i2s *priv)
//
//  Parameters:
//		priv	: device pointer 
//
//  Return:
//		none
//		
//  Description:
//       i2s fifo reset.
//
/*****************************************************************************/
static void i2s_fifo_reset(struct brite_i2s *priv)
{
	if ((priv->capability & DWC_I2S_PLAY) && (priv->stream_dir == SNDRV_PCM_STREAM_PLAYBACK))
	{
		// Transmitter FIFO Reset
		i2s_write_reg(priv->base, I2S_TXFFR_OFST, 1);
	}
	else if ((priv->capability & DWC_I2S_RECORD) && (priv->stream_dir == SNDRV_PCM_STREAM_CAPTURE))
	{
		// Receiver FIFO Reset
		i2s_write_reg(priv->base, I2S_RXFFR_OFST, 1);
	}
	else
	{
		debug("i2s_fifo_reset: unsupport\n");
	}
}

/*****************************************************************************/
//  Function:  static int i2s_config_from_hw(struct brite_i2s *priv)
//
//  Parameters:
//		priv	: device pointer 
//
//  Return:
//		none
//		
//  Description:
//       i2s configure upon the hw parameter.
//
/*****************************************************************************/
static int i2s_config_from_hw(struct brite_i2s *priv)
{
	//  Read component parameter registers to extract  the I2S block's configuration.
	u32 comp1 = i2s_read_reg(priv->base, I2S_COMP_PARAM_1_OFST);
	u32 comp2 = i2s_read_reg(priv->base, I2S_COMP_PARAM_2_OFST);
	u32 fifo_depth = 1 << (1 + COMP1_FIFO_DEPTH_GLOBAL(comp1));
	u32 idx;

	printf("dw_i2s_config_from_hw\n");

	idx = COMP1_APB_DATA_WIDTH(comp1);
	if (idx >= ARRAY_SIZE(bus_widths))
	{
		return -EINVAL;
	}
	
	priv->fifo_depth = fifo_depth;
	printf("fifo_depth: 0x%x\n", fifo_depth);
	
	if (COMP1_TX_ENABLED(comp1)) 
	{
		info("port%d:0x%x: play supported\n", priv->port, priv->base);

		priv->capability |= DWC_I2S_PLAY;
		
		priv->tx_max_channels = COMP1_TX_CHANNELS(comp1) + 1;
		if (priv->ch_num > priv->tx_max_channels)
		{
			priv->ch_num = priv->tx_max_channels;
		}
		
		priv->tx_fifo_empty_trig = priv->fifo_depth / 2;
		
		idx = COMP1_TX_WORDSIZE_0(comp1);
		priv->tx_fifo_width = fifo_width[idx];

		info("tx_fifo_empty_trig: 0x%x\n", priv->tx_fifo_empty_trig);
		info("tx_max_channels: 0x%x\n", priv->tx_max_channels);
	}

	if (COMP1_RX_ENABLED(comp1)) 
	{
		debug("port%d:0x%x: record supported\n", priv->port, priv->base);

		priv->capability |= DWC_I2S_RECORD;
		
		priv->rx_max_channels = COMP1_RX_CHANNELS(comp1) + 1;
		if (priv->ch_num > priv->rx_max_channels)
		{
			priv->ch_num = priv->rx_max_channels;
		}
		
		priv->rx_fifo_avail_trig = priv->fifo_depth / 2;
		
		idx = COMP2_RX_WORDSIZE_0(comp2);
		priv->rx_fifo_width = fifo_width[idx];

		info("rx_fifo_avail_trig: 0x%x\n", priv->rx_fifo_avail_trig);
		info("rx_max_channels: 0x%x\n", priv->rx_max_channels);
	}

	if (COMP1_MODE_EN(comp1)) 
	{
		debug("port%d: i2s master mode supported\n", priv->port);
		priv->capability |= DW_I2S_MASTER;
	} 
	else 
	{
		debug("port%d: i2s slave mode supported\n", priv->port);
		priv->capability |= DW_I2S_SLAVE;
	}

	return 0;
}

/*****************************************************************************/
//  Function:  int i2s_hw_init_tx(struct brite_i2s *priv)
//
//  Parameters:
//		priv	: device pointer 
//
//  Return:
//		none
//		
//  Description:
//       i2s tx initialize.
//
/*****************************************************************************/
int i2s_hw_init_tx(struct brite_i2s *priv)
{
	int ret;

	if (!(priv->capability & DWC_I2S_PLAY))
	{
		return 0;
	}
	
	priv->stream_dir = SNDRV_PCM_STREAM_PLAYBACK;

	// init from user
	ret = i2s_set_hw_params(priv);
	if (ret != 0) 
	{
		debug("dw_i2s_tx_init:set playback hw params failed\n");
		return ret;
	}

	// disable i2s transmit and flush the tx fifo
	i2s_fifo_reset(priv);
	
	return ret;
}

/*****************************************************************************/
//  Function:  int i2s_hw_init_rx(struct brite_i2s *priv)
//
//  Parameters:
//		priv	: device pointer 
//
//  Return:
//		none
//		
//  Description:
//       i2s rx initialize.
//
/*****************************************************************************/
int i2s_hw_init_rx(struct brite_i2s *priv)
{
	int ret;

	if (!(priv->capability & DWC_I2S_RECORD))
	{
		return 0;
	}

	priv->stream_dir = SNDRV_PCM_STREAM_CAPTURE;

	ret = i2s_set_hw_params(priv);
	if (ret != 0) 
	{
		debug("dw_i2s_tx_init:set playback hw params failed\n");
		return ret;
	}

	// disable i2s receiver and flush the rx fifo
	i2s_fifo_reset(priv);
	
	return ret;
}

/*****************************************************************************/
//  Function:  void i2s_start_transfer(struct brite_i2s *priv, void *data, uint32_t len)
//
//  Parameters:
//		priv	: device pointer 
//
//		data	: data buffer
//
//		len	: data length(byte aligned)
//
//  Return:
//		none
//		
//  Description:
//      i2s start to transfer.
//
/*****************************************************************************/
void i2s_start_transfer(struct brite_i2s *priv, void *data, uint32_t len)
{
	for (int i = 0; i < 3; i++)
	{
		priv->chnl_buf_cur[i] = (unsigned char *)data;
	}

	if ((priv->capability & DWC_I2S_PLAY) && (priv->stream_dir == SNDRV_PCM_STREAM_PLAYBACK)) 
	{
		//info("tx play and playback\n");
		
		priv->tx_buf = data;
		priv->tx_len = len / priv->data_step;
		priv->tx_actual = 0;
		priv->tx_ch_done = 0;
	} 
	else if ((priv->capability & DWC_I2S_RECORD) && (priv->stream_dir == SNDRV_PCM_STREAM_CAPTURE)) 
	{
		//info("rx record and capture\n");
		
		priv->rx_buf = data;
		priv->rx_len = len / priv->data_step;
		priv->rx_actual = 0;
		priv->rx_ch_done = 0;
	}
	
	i2s_start(priv);
}

/*****************************************************************************/
//  Function:  __inline uint32_t read_num_bytes_as_int(void *p, uint32_t n)
//
//  Parameters:
//		p	: data buffer
//
//		n	: byte number(2/3/4)
//
//  Return:
//		none
//		
//  Description:
//      read 2/3/4 byes as integer.
//
/*****************************************************************************/
__inline uint32_t read_num_bytes_as_int(void *p, uint32_t n)
{
	uint8_t *p1 = p;
	uint32_t ret = 0;
	uint32_t i;

	if (n > 4) 
	{
		debug("%s can't read more than 4 bytes\n", __func__);
		return ~0UL;
	}

	if (n == 2)
	{
		ret = *(uint16_t *) p;
	} 
	else if (n == 4)
	{
		ret = *(uint32_t *) p;
	}
	else
	{
		for (i = 0; i < n; i++) 
		{
			ret |= ((uint32_t)*(p1+i)) << (8*i);
		}	
	}
	
	return ret;
}

/*****************************************************************************/
//  Function:  __inline uint32_t write_int_as_num_bytes(void *p, unsigned int val, int n)
//
//  Parameters:
//		p	: data buffer
//
//		val	: integer value
//
//		n	: byte number(2/3/4)
//
//  Return:
//		none
//		
//  Description:
//      write 2/3/4 byes as integer.
//
/*****************************************************************************/
__inline uint32_t write_int_as_num_bytes(void *p, uint32_t val, int n) 
{
	uint8_t *p1 = p;
	uint8_t *pval = (unsigned char *)&val;
	uint8_t i;

	if (n > 4) 
	{
		debug("%s can't read more than 4 bytes\n", __func__);
		return ~0UL;
	}

	if (n == 2) 
	{
		*(uint16_t *)p1 = *(uint16_t *)pval;
	} 
	else if (n == 4) 
	{
		*(uint32_t *)p1 = *(uint32_t *)pval;
	}
	else
	{
		for (i = 0; i < n; i++) 
		{
			*(p1+i) = *(pval+i);
		}	
	}
	
	return 0;
}

/*****************************************************************************/
//  Function:  __inline void i2s_channel_tx_data(struct brite_i2s *priv, E_I2S_CH ch)
//
//  Parameters:
//		priv	: device pointer 
//
//		ch	: i2s channel
//
//  Return:
//		none
//		
//  Description:
//      write 2/3/4 byes as integer.
//
/*****************************************************************************/
__inline void i2s_channel_tx_data(struct brite_i2s *priv, E_I2S_CH ch)
{
	uint32_t remain_len, fifo_cnt;
	
	remain_len = priv->tx_len - priv->tx_actual;
	if (remain_len <= 1) 
	{
		priv->tx_ch_done |= 1<<ch;
		
		u32 irq = i2s_read_reg(priv->base, I2S_IMR_OFST(ch));
		i2s_write_reg(priv->base, I2S_IMR_OFST(ch), (irq | 0x30));
		
		//debug("tx data ch%d done\n", ch);

		return;
	}

	fifo_cnt = (remain_len >= priv->tx_burst_size) ? priv->tx_burst_size : remain_len;
	//info("ch: %d, tx fifo count %d, remain_len: %d\n", ch, fifo_cnt, remain_len);

	int data_step = priv->data_step;
	uint32_t d0, d1;
	
	for (int i = 0; i < fifo_cnt; i += 2) 
	{
		d0 = read_num_bytes_as_int(priv->chnl_buf_cur[ch], data_step);
		d1 = read_num_bytes_as_int(priv->chnl_buf_cur[ch] + data_step, data_step);
		
		i2s_write_reg(priv->base, I2S_LTHR_OFST(ch), d0);
		i2s_write_reg(priv->base, I2S_RTHR_OFST(ch), d1);

		priv->chnl_buf_cur[ch] += priv->chnl_data_step;
		priv->tx_actual += 2;

		if (priv == &g_i2s[I2S_1])
		{
			// i2s1_enable
			_SET_SYSCON_REG_i2s1_control_i2s1_enable_346(1) 	
		}	

		//info("tx ch: %x, i: %d, %x, %x\n", ch, i, d0, d1);
	}	
}

static int i2s_dma_tx(struct brite_i2s *priv)
{
	if (priv->tx_ch_done == priv->ch_mask) 
	{
		return 0;
	}

	//info("i2s_dma_tx\n");
	dma_i2s_tx(priv->port, priv->dma_ch_tx, CH_PRIOR_7, (uint32_t)priv->tx_buf, (priv->tx_len*priv->data_step), ST_NO_POLLING);
	
	priv->tx_actual = priv->tx_len;
	priv->tx_ch_done = priv->ch_mask;

	if (priv == &g_i2s[I2S_1])
	{
		// i2s1_enable
		_SET_SYSCON_REG_i2s1_control_i2s1_enable_346(1) 						
	}
	
    return 0;
}

static int i2s_dma_rx(struct brite_i2s *priv)
{
	if (priv->rx_ch_done == priv->ch_mask) 
	{
		return 0;
	}

	//info("i2s_dma_rx\n");
	dma_i2s_rx(priv->dma_ch_rx, CH_PRIOR_7, (uint32_t)priv->rx_buf, (priv->rx_len*priv->data_step), ST_NO_POLLING);
		
	priv->rx_actual = priv->rx_len;
	priv->rx_ch_done = priv->ch_mask;

    return 0;
}

__inline void i2s_channel_rx_data(struct brite_i2s *priv, E_I2S_CH ch)
{
	uint32_t fifo_cnt, remain_len;

	//debug("rx_len:%d, rx_actual:%d\n", priv->rx_len, priv->rx_actual);
	
	remain_len = priv->rx_len - priv->rx_actual;
    if (remain_len <= 1) 
	{
		priv->rx_ch_done |= 1<<ch;
		
		u32 irq = i2s_read_reg(priv->base, I2S_IMR_OFST(ch));
		i2s_write_reg(priv->base, I2S_IMR_OFST(ch), irq | 0x03);
		
		debug("rx data ch%d done\n", ch);

		return;
    }
	
	fifo_cnt = (remain_len >= priv->rx_burst_size) ? priv->rx_burst_size : remain_len;
	//printf("rx fifo count %d\n", fifo_cnt);
	//printf("ch: %d, rx fifo count %d, remain_len: %d\n", ch, fifo_cnt, remain_len);

	int data_step = priv->data_step;
	uint32_t r0, r1;
	
	for (int i = 0; i < fifo_cnt; i += 2) 
	{
		r0 = i2s_read_reg(priv->base, I2S_LRBR_OFST(ch));
		r1 = i2s_read_reg(priv->base, I2S_RRBR_OFST(ch));
		
		write_int_as_num_bytes(priv->chnl_buf_cur[ch], r0, data_step);
		write_int_as_num_bytes(priv->chnl_buf_cur[ch] + data_step, r1, data_step);
		
		priv->chnl_buf_cur[ch] += priv->chnl_data_step;
		priv->rx_actual += 2;
	}
}

__inline void i2s_channel_irq_rx(struct brite_i2s *priv)
{
	uint32_t int_isr;
	uint32_t i, ch;

	for (i = 0; i < priv->ch_num; i++) 
	{
		ch = priv->hw_channel_map[i];
		
		//debug("rx_irq:ch%d\n", ch);
		
		if (priv->rx_ch_done & (1<<ch))
		{
			continue;
		}
		
		int_isr = i2s_read_reg(priv->base, I2S_ISR_OFST(ch));
		if (int_isr & I2S_ST_RXDA_MASK) 
		{
			//debug("RX data interrupt, rx data\n");
			i2s_channel_rx_data(priv, ch);
		}
		
		if (int_isr & I2S_ST_RXFO_MASK) 
		{
			//debug("RX OV\n");
			i2s_read_reg(priv->base, I2S_ROR_OFST(ch));
		}
	}
}


__inline void i2s_channel_irq_tx(struct brite_i2s *priv)
{
	uint32_t int_isr;
	E_I2S_CH ch = I2S_CH0;

	//debug("tx_irq:ch%d 0x%x/0x%x\n", chnum, i2s_read_reg(priv->base, I2S_ISR_OFST(chnum)), i2s_read_reg(priv->base, I2S_IMR_OFST(chnum)));
	if (priv->tx_ch_done & (1<<ch)) 
	{
		return;
	}

	int_isr = i2s_read_reg(priv->base, I2S_ISR_OFST(ch));
	if (int_isr & I2S_ST_TXFO_MASK) 
	{
		//debug("TX Data Overrun interrupt, read reg clear\n");
		debug("TX OV\n");
		i2s_read_reg(priv->base, I2S_TOR_OFST(ch));
	}

	if (int_isr & I2S_ST_TXFE_MASK) 
	{
		//debug("TX fifo empty interrupt, tx data\n");
		i2s_channel_tx_data(priv, ch);
	}	
}


void i2s_tx_interrupt_handler(void *ptr)
{
	struct brite_i2s *p_i2s = ptr;

	if (!p_i2s) 
	{
		info("i2s tx interrupt handler err: p_i2s null\n");
		return;
	}
	
	if ((p_i2s->capability & DWC_I2S_PLAY) && 
		(p_i2s->ch_mask != p_i2s->tx_ch_done))
	{
		i2s_channel_irq_tx(p_i2s);
	}	
}

void i2s_rx_interrupt_handler(void *ptr)
{
	struct brite_i2s *p_i2s = ptr;

	if (!p_i2s) 
	{
		info("i2s rx interrupt handler err: p_i2s null\n");
		return;
	}

	if ((p_i2s->capability & DWC_I2S_RECORD) && 
		(p_i2s->ch_mask != p_i2s->rx_ch_done))
	{
		i2s_channel_irq_rx(p_i2s);
	}
}


/**
 * @brief init, clear and start the controller
 * @param [port] input, I2S port
 * @param [clk] input, specify the I2S clock rate in Hz
 * @param [type] input, I2S data transfer format
 * @param [wss] input,
 * @param [wlen] input,
 * @param [clk_en] input,
 * @param [dt_mode] input, data transfer by cpu/dma mode
 * @param [role] input, master or slave; should be 0(master) or 1(slave)
 * @param [pi_mode] input, i2s interrupt request process mode, polling or interrupt
 * @param [stream_dir] input, i2s data direction, playback or capture
 * @param [sample_format] input, sampling format (S16_LE, S24_LE, S32_LE)
 * @param [sample_rate] input, sampling frequency (8Khz, 16Khz, 32Khz, 44Khz, 48Khz)
 * @param [bits_per_sample] input, bits per sample (8/16/24/32 bit)
 * @param [channels] input, number of channel (2/4/6/8)
 *
 * @return void
 */
void i2s_init(E_I2S_PORT port, uint32_t clk, E_I2S_TYPE type, E_I2S_WSS wss, 
					E_I2S_WLEN wlen, E_I2S_CTRL clk_en, E_DT_MODE dt_mode,
					E_PI_MODE pi_mode, E_I2S_PCM_FORMAT sample_format,
					int ch0_en, int ch1_en, int ch2_en)
{	
	struct brite_i2s *p_i2s = &g_i2s[port];
	int i, ret = 0;

	// init from user command line
	p_i2s->clk = clk;
	p_i2s->type = type;
	p_i2s->wss = wss;
	p_i2s->wlen = wlen;	
	p_i2s->clk_en = clk_en;
	p_i2s->dt_mode = dt_mode;
	p_i2s->pi_mode = pi_mode;
	p_i2s->sample_format = sample_format;
	p_i2s->data_step = (p_i2s->sample_format == SNDRV_PCM_FORMAT_S16_LE) ? 4 : 4;
	//p_i2s->data_step = sample_format/8;
	p_i2s->ch_num = 0;
	p_i2s->ch_mask = 0;

	if (ch0_en) 
	{
		p_i2s->hw_channel_map[p_i2s->ch_num++] = 0;
		p_i2s->ch_mask |= 0x1;
	}

	if (ch1_en) 
	{
		p_i2s->hw_channel_map[p_i2s->ch_num++] = 1;
		p_i2s->ch_mask |= 0x2;
	}

	if (ch2_en) 
	{
		p_i2s->hw_channel_map[p_i2s->ch_num++] = 2;
		p_i2s->ch_mask |= 0x4;
	}

	info("\nchannel_mask: %x, ch_num: %x\n", p_i2s->ch_mask, p_i2s->ch_num);

	// init from hw support
	ret = i2s_config_from_hw(p_i2s);
	if (ret != 0) 
	{
		debug("i2s_init:configure from hw support failed\n");
		return;
	}
	
	p_i2s->tx_burst_size = p_i2s->fifo_depth - p_i2s->tx_fifo_empty_trig;
	p_i2s->rx_burst_size = p_i2s->rx_fifo_avail_trig;
	printf("i2s burst tx %d rx %d\n", p_i2s->tx_burst_size, p_i2s->rx_burst_size);

	if (p_i2s->pi_mode == PI_INT) 
	{
		switch (port) 
		{
			case I2S_ADC:
					p_i2s->int_line[0] = INT_I2S_ADC_DA_0;
					p_i2s->int_line[1] = INT_I2S_ADC_OR_0;
					p_i2s->int_line[2] = INT_I2S_ADC_DA_1;
					p_i2s->int_line[3] = INT_I2S_ADC_OR_1;
					p_i2s->int_line[4] = INT_I2S_ADC_DA_2;
					p_i2s->int_line[5] = INT_I2S_ADC_OR_2;
				break;
			case I2S_DAC:
					p_i2s->int_line[0] = INT_I2S_DAC_EMP_0;
					p_i2s->int_line[1] = INT_I2S_DAC_OR_0;
				break;
			case I2S_1:
					p_i2s->int_line[0] = INT_I2S1_EMP_0;
					p_i2s->int_line[1] = INT_I2S1_OR_0;
				break;
			case I2S_VAD:
					p_i2s->int_line[0] = INT_I2S_VAD_SP;
					p_i2s->int_line[1] = INT_I2S_VAD_SL;
				break;
			default:
					debug("unsupport i2s port\n");
				break;
		}
		
		// register int handler and enable it
		if (port != I2S_VAD)
		{
			for (i = 0; i < p_i2s->ch_num; i++) 
			{
				int ch = p_i2s->hw_channel_map[i];
				if (p_i2s->capability & DWC_I2S_PLAY) 
				{
					irq_generic_install_handler(p_i2s->int_line[ch*2], i2s_tx_interrupt_handler, p_i2s); 
					irq_generic_config_int(p_i2s->int_line[ch*2], IRQ_TYPE_LEVEL_HIGH);
					irq_generic_enable_int(p_i2s->int_line[ch*2]);
			
					irq_generic_install_handler(p_i2s->int_line[ch*2+1], i2s_tx_interrupt_handler, p_i2s); 
					irq_generic_config_int(p_i2s->int_line[ch*2+1], IRQ_TYPE_LEVEL_HIGH);
					irq_generic_enable_int(p_i2s->int_line[ch*2+1]);
			
					info("TX I2S%d: base=0x%x int_vector=(%d, %d)\n", p_i2s->port, p_i2s->base, 
							p_i2s->int_line[ch*2], p_i2s->int_line[ch*2+1]);
				} 
				else 
				{
					irq_generic_install_handler(p_i2s->int_line[ch*2], i2s_rx_interrupt_handler, p_i2s); 
					irq_generic_config_int(p_i2s->int_line[ch*2], IRQ_TYPE_LEVEL_HIGH);
					irq_generic_enable_int(p_i2s->int_line[ch*2]);
					
					irq_generic_install_handler(p_i2s->int_line[ch*2+1], i2s_rx_interrupt_handler, p_i2s); 
					irq_generic_config_int(p_i2s->int_line[ch*2+1], IRQ_TYPE_LEVEL_HIGH);
					irq_generic_enable_int(p_i2s->int_line[ch*2+1]);
			
					info("RX I2S%d: base=0x%x int_vector=(%d, %d)\n", p_i2s->port, p_i2s->base, 
							p_i2s->int_line[ch*2], p_i2s->int_line[ch*2+1]);
				}
			}			
		}
		else
		{
			irq_generic_install_handler(p_i2s->int_line[0], vad_spint_handle, p_i2s); 
			irq_generic_config_int(p_i2s->int_line[0], IRQ_TYPE_LEVEL_HIGH);
			irq_generic_enable_int(p_i2s->int_line[0]);
			
			irq_generic_install_handler(p_i2s->int_line[1], vad_slint_handle, p_i2s); 
			irq_generic_config_int(p_i2s->int_line[1], IRQ_TYPE_LEVEL_HIGH);
			irq_generic_enable_int(p_i2s->int_line[1]);	

			info("RX I2S%d: base=0x%x int_vector=(%d, %d)\n", p_i2s->port, p_i2s->base, 
							p_i2s->int_line[0], p_i2s->int_line[1]);
		}

		_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);
	}

	i2s_hw_init_tx(p_i2s);
	i2s_hw_init_rx(p_i2s);

	p_i2s->chnl_data_step = p_i2s->ch_num * p_i2s->data_step * 2;
}

void i2s_exit(E_I2S_PORT port)
{
	struct brite_i2s *p_i2s = &g_i2s[port];

	if (p_i2s->pi_mode == PI_INT) 
	{
		int i;

		// disable interrupt
		for (i = 0; i < p_i2s->ch_num; i++) 
		{
			int ch = p_i2s->hw_channel_map[i];
			irq_generic_disable_int(p_i2s->int_line[ch*2]);
			irq_generic_remove_handler(p_i2s->int_line[ch*2],NULL,NULL);

			irq_generic_disable_int(p_i2s->int_line[ch*2+1]);
			irq_generic_remove_handler(p_i2s->int_line[ch*2+1],NULL,NULL);
		}
		
		//_SET_SYSCON_REG_interrupt_control_interrupt_en_227(0);
	}
}

/**
 * @brief I2S data transfer for playback
 * @param [priv] input, Driver instance data
 * @param [data] input, data buffer
 * @param [data_size] input, data length in bytes
 *
 * @return directly if failed
 */
int i2s_xfer_tx(E_I2S_PORT port, void *data, uint32_t len)
{
	struct brite_i2s *priv = &g_i2s[port];

	int data_step_tx = priv->data_step;
	int ret = 0;

	if (!(priv->capability & DWC_I2S_PLAY))
	{
		return ret;
	}
	
	priv->stream_dir = SNDRV_PCM_STREAM_PLAYBACK;

	if (len < data_step_tx * priv->ch_num * 2) 
	{
		debug("i2s_xfer: Invalid data size\n");
		return -1;
	}

	i2s_start_transfer(priv, data, len);
	
	while (priv->ch_mask != priv->tx_ch_done) 
	{
		if (priv->pi_mode == PI_POLLING)
		{
			if (priv->dt_mode == DT_CPU)
			{
				i2s_channel_irq_tx(priv);
			}
			else
			{
				i2s_dma_tx(priv);
			}
		}
		else
		{}		
	}

	if ((priv->pi_mode == PI_POLLING) && (priv->dt_mode == DT_DMA))
	{
		dma_polling_st(DMA0, priv->dma_ch_tx);
	}

	i2s_stop(priv);
	
	//debug("i2s_xfer_tx: transfer %d done!\n", priv->tx_actual * priv->data_step);
	
	return ret;
}


int i2s_xfer_tx_async(E_I2S_PORT port, void *data, uint32_t len)
{
	struct brite_i2s *priv = &g_i2s[port];

	int data_step_tx = priv->data_step;
	int ret = 0;

	if (!(priv->capability & DWC_I2S_PLAY))
	{
		return ret;
	}
	
	priv->stream_dir = SNDRV_PCM_STREAM_PLAYBACK;

	if (len < data_step_tx * priv->ch_num * 2) 
	{
		debug("i2s_xfer: Invalid data size\n");
		return -1;
	}

	i2s_start_transfer(priv, data, len);
	while (priv->ch_mask != priv->tx_ch_done) 
	{
		if (priv->pi_mode == PI_POLLING)
		{
			if (priv->dt_mode == DT_CPU)
			{
				i2s_channel_irq_tx(priv);
			}
			else
			{
				i2s_dma_tx(priv);
			}
		}
		else
		{}		
	}

#if 0
	if ((priv->pi_mode == PI_POLLING) && (priv->dt_mode == DT_DMA))
	{
		dma_polling_st(DMA0, priv->dma_ch_tx);
	}

	//i2s_stop(priv);
#endif
	//debug("i2s_xfer_tx: transfer %d done!\n", priv->tx_actual * priv->data_step);
	
	return ret;
}


/**
 * @brief I2S data transfer for capture
 * @param [priv] input, Driver instance data
 * @param [data] input, data buffer
 * @param [data_size] input, data length in bytes
 *
 * @return directly if failed
 */
int i2s_xfer_rx(E_I2S_PORT port, void *data, uint32_t len)
{
	struct brite_i2s *priv = &g_i2s[port];

	int data_step = priv->data_step;
	int ret = 0;

	if (!(priv->capability & DWC_I2S_RECORD))
	{
		return ret;
	}
	
	priv->stream_dir = SNDRV_PCM_STREAM_CAPTURE;

	if (len < data_step * priv->ch_num * 2) 
	{
		debug("i2s_xfer: Invalid data size\n");
		return -1; /* invalid pcm data size */
	}

	i2s_start_transfer(priv, data, len);
	while (priv->ch_mask != priv->rx_ch_done) 
	{
		if (priv->dt_mode == DT_CPU)
		{
			i2s_channel_irq_rx(priv);
		}
		else 
		{
			//i2s_dma_rx(priv);
		}	
	}

	i2s_stop(priv);
	
	debug("i2s_xfer_rx: transfer %d done!\n", priv->rx_actual * priv->data_step);
	
	return ret;
}

/**
 * @brief I2S data transfer for capture and playback
 * @param [priv] input, Driver instance data
 * @param [data] input, data buffer
 * @param [data_size] input, data length in bytes
 *
 * @return directly if failed
 */
int i2s_xfer(E_I2S_PORT port_tx, E_I2S_PORT port_rx, void *data_tx, void *data_rx, uint32_t len)
{
	struct brite_i2s *p_i2s_tx = &g_i2s[port_tx];
	struct brite_i2s *p_i2s_rx = &g_i2s[port_rx];
	int data_step_tx = p_i2s_tx->data_step;
	int ret = 0;

	if (!(p_i2s_tx->capability & DWC_I2S_PLAY) || !(p_i2s_rx->capability & DWC_I2S_RECORD))
	{
		return ret;
	}

	p_i2s_tx->stream_dir = SNDRV_PCM_STREAM_PLAYBACK;
	p_i2s_rx->stream_dir = SNDRV_PCM_STREAM_CAPTURE;

	if (len < (data_step_tx * p_i2s_tx->ch_num * 2)) 
	{
		debug("i2s_xfer: Invalid data size\n");
		return -1;
	}

	i2s_start_transfer(p_i2s_rx, data_rx, len + 512);
	i2s_start_transfer(p_i2s_tx, data_tx, len);

	while ((p_i2s_tx->ch_mask != p_i2s_tx->tx_ch_done) ||
			(p_i2s_rx->ch_mask != p_i2s_rx->rx_ch_done)) 
	{
		if (p_i2s_tx->ch_mask != p_i2s_tx->tx_ch_done) 
		{
			if (p_i2s_tx->pi_mode == PI_POLLING)
			{
				if (p_i2s_tx->dt_mode == DT_CPU)
				{
					i2s_channel_irq_tx(p_i2s_tx);
				}
				else
				{
					i2s_dma_tx(p_i2s_tx);
					//i2s_channel_irq_tx(p_i2s_tx);
				}
			}
			else
			{}
		}

		if (p_i2s_rx->ch_mask != p_i2s_rx->rx_ch_done) 
		{
			if (p_i2s_rx->pi_mode == PI_POLLING)
			{
				if (p_i2s_rx->dt_mode == DT_CPU)
				{
					i2s_channel_irq_rx(p_i2s_rx);
				}
				else
				{
					i2s_dma_rx(p_i2s_rx);
					//i2s_channel_irq_rx(p_i2s_rx);
				}
			}
			else
			{}
		}
	}

	if ((p_i2s_tx->pi_mode == PI_POLLING) && (p_i2s_tx->dt_mode == DT_DMA))
	{
		dma_polling_st(DMA0, p_i2s_rx->dma_ch_rx);
		dma_polling_st(DMA0, p_i2s_tx->dma_ch_tx);
	}
	
	i2s_stop(p_i2s_tx);
	i2s_stop(p_i2s_rx);

	debug("i2s_xfer: done!\n");
	
	return ret;
}

int i2s_vad_xfer(E_I2S_PORT port_tx, E_I2S_PORT port_rx, void *data_tx, void *data_rx, uint32_t len)
{
	struct brite_i2s *p_i2s_tx = &g_i2s[port_tx];
	struct brite_i2s *p_i2s_rx = &g_i2s[port_rx];
	int data_step_tx = p_i2s_tx->data_step;
	int ret = 0;

	if (!(p_i2s_tx->capability & DWC_I2S_PLAY) || !(p_i2s_rx->capability & DWC_I2S_RECORD))
	{
		return ret;
	}

	p_i2s_tx->stream_dir = SNDRV_PCM_STREAM_PLAYBACK;
	p_i2s_rx->stream_dir = SNDRV_PCM_STREAM_CAPTURE;

	if (len < (data_step_tx * p_i2s_tx->ch_num * 2)) 
	{
		debug("i2s_xfer: Invalid data size\n");
		return -1;
	}

	i2s_start_transfer(p_i2s_rx, data_rx, len);
	i2s_start_transfer(p_i2s_tx, data_tx, len);

	i2s_dma_rx(p_i2s_rx);
	i2s_dma_tx(p_i2s_tx);
	
	return ret;
}

