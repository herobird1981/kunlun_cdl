/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_dma.h"
#include "cdl_spi.h"
#include "cdl_i2s.h"
#include "cdl_i2c.h"
#include "musb_core.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct brite_dma g_dma[NUM_DMA_PORTS];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//  Function:  dma_int_handler(void *ptr)
//
//  Parameters:
//		ptr [in]	: pointer
//
//  Return:
//		none
//		
//  Description:
//     DMA interrupt hadler.
//
/*****************************************************************************/
void dma_int_handler(void *ptr)
{
	
}


/*****************************************************************************/
//  Function:  void dma_wr_reg(E_DMA_PORT port, uint32_t reg_ofst, uint32_t val)
//
//  Parameters:
//		port			: dma port
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
void dma_wr_reg(E_DMA_PORT port, uint32_t reg_ofst, uint32_t val)
{
	__raw_writel(val, g_dma[port].base + reg_ofst);
}

/*****************************************************************************/
//  Function:  uint32_t dma_rd_reg(E_DMA_PORT port, uint32_t reg_ofst)
//
//  Parameters:
//		port			: dma port
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
uint32_t dma_rd_reg(E_DMA_PORT port, uint32_t reg_ofst)
{
	return __raw_readl(g_dma[port].base + reg_ofst);
}

/*****************************************************************************/
//  Function:  void dma_rd_id(E_DMA_PORT port, uint32_t *p_id)
//
//  Parameters:
//		port			: dma port
//
//		p_id [out]	: dma id[2*32bits]
//
//  Return:
//		none
//		
//  Description:
//      Read the ID of a dma controller.
//
/*****************************************************************************/
void dma_rd_id(E_DMA_PORT port, uint32_t *p_id)
{
	p_id[0] = dma_rd_reg(port, DMAC_IDREG_OFST);
	p_id[1] = dma_rd_reg(port, (DMAC_IDREG_OFST + 4));
}

/*****************************************************************************/
//  Function:  void dma_rd_ver(E_DMA_PORT port, uint32_t *p_ver)
//
//  Parameters:
//		port			: dma port
//
//		p_ver [out]	: dma version[32bits]
//
//  Return:
//		none
//		
//  Description:
//      Read the version of a dma controller.
//
/*****************************************************************************/
void dma_rd_ver(E_DMA_PORT port, uint32_t *p_ver)
{
	*p_ver = dma_rd_reg(port, DMAC_COMPVERREG_OFST);
}

/*****************************************************************************/
//  Function:  void dma_enable(E_DMA_PORT port, E_DMAC_EN dma_en)
//
//  Parameters:
//		port		: dma port
//
//		dma_en	: dma enable/disable
//
//  Return:
//		none
//		
//  Description:
//      DMA enable/disable control.
//
/*****************************************************************************/
void dma_enable(E_DMA_PORT port, E_DMAC_EN dma_en)
{
	uint32_t reg_val = dma_rd_reg(port, DMAC_CFGREG_OFST);

	reg_val &= ~DMAC_EN_MASK;
	reg_val |= DMAC_EN_VAL(dma_en);
	
	dma_wr_reg(port, DMAC_CFGREG_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void dma_int_enable(E_DMA_PORT port, E_DMAC_INT_EN int_en)
//
//  Parameters:
//		port		: dma port
//
//		int_en	: dma interrupt enable/disable
//
//  Return:
//		none
//		
//  Description:
//      DMA interrupt enable/disable control.
//
/*****************************************************************************/
void dma_int_enable(E_DMA_PORT port, E_DMAC_INT_EN int_en)
{
	uint32_t reg_val = dma_rd_reg(port, DMAC_CFGREG_OFST);

	reg_val &= ~DMAC_INT_EN_MASK;
	reg_val |= DMAC_INT_EN_VAL(int_en);
	
	dma_wr_reg(port, DMAC_CFGREG_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void dma_ch_abort(E_DMA_PORT port, E_DMA_CH ch, E_DMAC_CH_ABORT abort)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel of a DMA
//
//		abort	: dma abort enable/disable
//
//  Return:
//		none
//		
//  Description:
//      Software abort the channel of a DMA.
//
/*****************************************************************************/
void dma_ch_abort(E_DMA_PORT port, E_DMA_CH ch, E_DMAC_CH_ABORT abort)
{
	uint32_t reg_val = dma_rd_reg(port, (DMAC_CHENREG_OFST + 4));

	// abort write enable
	reg_val &= ~DMAC_CH_ABORT_WE_MASK(ch);
	reg_val |= DMAC_CH_ABORT_WE_VAL(ch, DMAC_CH_ABORT_WE_EN);

	// abort
	reg_val &= ~DMAC_CH_ABORT_MASK(ch);
	reg_val |= DMAC_CH_ABORT_VAL(ch, abort);	
	
	dma_wr_reg(port, (DMAC_CHENREG_OFST + 4), reg_val);
}

/*****************************************************************************/
//  Function:  void dma_ch_susp(E_DMA_PORT port, E_DMA_CH ch, E_DMAC_CH_SUSP susp)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel of a DMA
//
//		susp		: channel suspend enable/disable
//
//  Return:
//		none
//		
//  Description:
//      Suspend the channel of a DMA.
//
/*****************************************************************************/
void dma_ch_susp(E_DMA_PORT port, E_DMA_CH ch, E_DMAC_CH_SUSP susp)
{
	uint32_t reg_val = dma_rd_reg(port, DMAC_CHENREG_OFST);

	// suspend write enable
	reg_val &= ~DMAC_CH_SUSP_WE_MASK(ch);
	reg_val |= DMAC_CH_SUSP_WE_VAL(ch, DMAC_CH_SUSP_WE_EN);

	// suspend
	reg_val &= ~DMAC_CH_SUSP_MASK(ch);
	reg_val |= DMAC_CH_SUSP_VAL(ch, susp);	
	
	dma_wr_reg(port, DMAC_CHENREG_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void dma_ch_enable(E_DMA_PORT port, E_DMA_CH ch, E_DMAC_CH_EN ch_en)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel of a DMA
//
//		ch_en	: channel enable/disable
//
//  Return:
//		none
//		
//  Description:
//      Suspend the channel of a DMA.
//
/*****************************************************************************/
void dma_ch_enable(E_DMA_PORT port, E_DMA_CH ch, E_DMAC_CH_EN ch_en)
{
	uint32_t reg_val = dma_rd_reg(port, DMAC_CHENREG_OFST);

	// dma write enable
	reg_val &= ~DMAC_CH_EN_WE_MASK(ch);
	reg_val |= DMAC_CH_EN_WE_VAL(ch, DMAC_CH_EN_WE_EN);

	// dma enable
	reg_val &= ~DMAC_CH_EN_MASK(ch);
	reg_val |= DMAC_CH_EN_VAL(ch, ch_en);
	
	dma_wr_reg(port, DMAC_CHENREG_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  uint32_t dma_rd_int_st(E_DMA_PORT port)
//
//  Parameters:
//		port		: dma port
//
//  Return:
//		interrupt status
//		
//  Description:
//      Read the interrupt status of a DMA.
//
/*****************************************************************************/
uint32_t dma_rd_int_st(E_DMA_PORT port)
{
	return dma_rd_reg(port, DMAC_INTSTATUSREG_OFST);
}

/*****************************************************************************/
//  Function:  void dma_clr_com_int_st(E_DMA_PORT port, uint32_t mask)
//
//  Parameters:
//		port		: dma port
//
//  Return:
//		none
//		
//  Description:
//       Writing 1 to specific field clears the corresponding field in DMAC Common register 
//  Interrupt Status Register (DMAC_CommonReg_IntStatusReg). 
//
/*****************************************************************************/
void dma_clr_com_int_st(E_DMA_PORT port, uint32_t mask)
{
	dma_wr_reg(port, DMAC_COMMONREG_INTCLEARREG_OFST, mask);
}

/*****************************************************************************/
//  Function:  void dma_en_com_int(E_DMA_PORT port, uint32_t mask)
//
//  Parameters:
//		port		: dma port
//
//  Return:
//		none
//		
//  Description:
//        Writing 1 to specific field enables the corresponding interrupt status generation in 
//  DMAC Common register Interrupt Status Register (DMAC_CommonReg_IntStatusReg). 
//
/*****************************************************************************/
void dma_en_com_int(E_DMA_PORT port, uint32_t mask)
{
	dma_wr_reg(port, DMAC_COMMONREG_INTSTATUS_ENABLEREG_OFST, mask);
}

/*****************************************************************************/
//  Function:  void dma_en_com_intsignal(E_DMA_PORT port, uint32_t mask)
//
//  Parameters:
//		port		: dma port
//
//  Return:
//		none
//		
//  Description:
//        Writing 1 to specific field will propagate the corresponding interrupt status in DMAC 
// Common register Interrupt Status Register (DMAC_CommonReg_IntStatusReg) to generate an port 
// level interrupt.
//
/*****************************************************************************/
void dma_en_com_intsignal(E_DMA_PORT port, uint32_t mask)
{
	dma_wr_reg(port, DMAC_COMMONREG_INTSIGNAL_ENABLEREG_OFST, mask);
}

/*****************************************************************************/
//  Function:  uint32_t dma_rd_com_int_st(E_DMA_PORT port)
//
//  Parameters:
//		port		: dma port
//
//  Return:
//		none
//		
//  Description:
//      This Register captures Slave interface access errors.
//
/*****************************************************************************/
uint32_t dma_rd_com_int_st(E_DMA_PORT port)
{
	return dma_rd_reg(port, DMAC_COMMONREG_INTSTATUSREG_OFST);
}

/*****************************************************************************/
//  Function:  void dma_en_com_intsignal(E_DMA_PORT port, uint32 mask)
//
//  Parameters:
//		port		: dma port
//
//  Return:
//		none
//		
//  Description:
//        Software DMA reset all the modules except the slave bus interface module and clears this bit to 0.
//
/*****************************************************************************/
void dma_reset(E_DMA_PORT port)
{
	dma_wr_reg(port, DMAC_RESETREG_OFST, DMAC_RST_MASK);
}

/*****************************************************************************/
//  Function: void dma_clr_ch_int_st(E_DMA_PORT port, E_DMA_CH ch, uint32_t mask)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel
//
//		mask	: mask value
//
//  Return:
//		none
//		
//  Description:
//        Writing 1 to specific field will clear the corresponding field in Channelx Interrupt Status 
// Register(CHx_IntStatusReg).
//
/*****************************************************************************/
void dma_clr_ch_int_st(E_DMA_PORT port, E_DMA_CH ch, uint32_t mask)
{
	dma_wr_reg(port, CHx_INTCLEARREG_OFST(ch), mask);
}

/*****************************************************************************/
//  Function:  void dma_en_com_int(E_DMA_PORT port, uint32_t mask)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel
//
//		mask	: mask value
//
//  Return:
//		none
//		
//  Description:
//        Writing 1 to specific field enables the corresponding interrupt status generation in 
// Channelx Interrupt Status Register(CH1_IntStatusReg).
//
/*****************************************************************************/
void dma_en_ch_int(E_DMA_PORT port, E_DMA_CH ch, uint32_t mask)
{
	dma_wr_reg(port, CHx_INTSTATUS_ENABLEREG_OFST(ch), mask);
}

/*****************************************************************************/
//  Function:  void dma_en_ch_intsignal(E_DMA_PORT port, E_DMA_CH ch, uint32_t mask)
//
//  Parameters:
//		port		: dma port
//
//  Return:
//		port		: dma port
//
//		ch		: channel
//
//		mask	: mask value
//		
//  Description:
//         This register contains fields that are used to enable the generation of port level interrupt 
// at the channel level.
//
/*****************************************************************************/
void dma_en_ch_intsignal(E_DMA_PORT port, E_DMA_CH ch, uint32_t mask)
{
	dma_wr_reg(port, CHx_INTSIGNAL_ENABLEREG_OFST(ch), mask);
}

/*****************************************************************************/
//  Function:  uint32_t dma_rd_com_int_st(E_DMA_PORT port)
//
//  Parameters:
//		port		: dma port
//
//  Return:
//		port		: dma port
//
//		ch		: channel
//		
//  Description:
//      Channelx Interrupt Status Register captures the Channelx specific interrupts.
//
/*****************************************************************************/
uint32_t dma_rd_ch_int_st(E_DMA_PORT port, E_DMA_CH ch)
{
	return dma_rd_reg(port, CHx_INTSTATUS_OFST(ch));
}

/*****************************************************************************/
//  Function:  void dma_set_ch_sw_hs_src(E_DMA_PORT port, E_DMA_CH ch)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel
//
//  Return:
//		none
//		
//  Description:
//        Set software handshake source register.
//
/*****************************************************************************/
void dma_set_ch_sw_hs_src(E_DMA_PORT port, E_DMA_CH ch)
{
	uint32_t reg_val = DMA_CHX_SWHSSRC_VAL(0, 0, 0);
	
	dma_wr_reg(port, CHx_SWHSSRCREG_OFST(ch), reg_val);
}

/*****************************************************************************/
//  Function:  void dma_set_ch_sw_hs_dst(E_DMA_PORT port, E_DMA_CH ch)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel
//
//  Return:
//		none
//		
//  Description:
//        Set software handshake destination register.
//
/*****************************************************************************/
void dma_set_ch_sw_hs_dst(E_DMA_PORT port, E_DMA_CH ch)
{
	uint32_t reg_val = DMA_CHX_SWHSDST_VAL(0, 0, 0);
	
	dma_wr_reg(port, CHx_SWHSDSTREG_OFST(ch), reg_val);
}

/*****************************************************************************/
//  Function:  uint32_t dma_rd_ch_st(E_DMA_PORT port, E_DMA_CH ch)
//
//  Parameters:
//		port		: dma port
//
//  Return:
//		port		: dma port
//
//		ch		: channel
//		
//  Description:
//       Channelx Status Register contains fields that indicate the status of DMA transfers for Channelx. 
//
/*****************************************************************************/
uint32_t dma_rd_ch_st(E_DMA_PORT port, E_DMA_CH ch)
{
	return dma_rd_reg(port, CHx_STATUSREG_OFST(ch));
}

/*****************************************************************************/
//  Function:  void dma_init(E_DMA_PORT port)
//
//  Parameters:
//		port		: dma port
//
//  Return:
//		port		: dma port
//
//		ch		: channel
//		
//  Description:
//       Channelx Status Register contains fields that indicate the status of DMA transfers for Channelx. 
//
/*****************************************************************************/
void dma_init(E_DMA_PORT port)
{
	struct brite_dma *priv = &g_dma[port];

	if (port == DMA0)
	{
		priv->base = DMAC_BASE;
		priv->int_vector = INT_DMAC;
	}
	else
	{	
		info("hw not defined\n");
		return;
	}
	
	// disable the interrupt of the controller
	dma_int_enable(port, DMAC_INT_DIS);

	// enable the dma controller
	dma_enable(port, DMAC_EN);

	// clear the interrupt
	dma_clr_com_int_st(port, 0xFFFFFFFF);

	// interrupt enable configure
	dma_int_enable(port, DMAC_INT_EN);	
}

/*****************************************************************************/
//  Function:  static E_DMA_ST dma_ch_select(E_DMA_PORT port, E_DMA_CH ch)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel
//  Return:
//		dma status
//
//  Description:
//	Check channel is idle,if not disable and clear the pending interrupt
//
// The bit 'DMAC_ChEnReg.CH1_EN' is automatically cleared 
// by hardware to disable the channel after the last AMBA 
// transfer of the DMA transfer to the destination has 
// completed. Software can therefore poll this bit to determine 
// when this channel is free for a new DMA transfer
//
/*****************************************************************************/
static E_DMA_ST dma_ch_select(E_DMA_PORT port, E_DMA_CH ch)
{
	uint32_t reg_val;
	int timeout = 500; // ms
	u64 start = get_timer(0);
	
	while (1) 
	{
		reg_val = dma_rd_reg(port, DMAC_CHENREG_OFST);
		if (!(reg_val & DMAC_CH_EN_MASK(ch)))
		{
			break;
		}
			
		//info("warning:ch %d still active, force stop it\n", ch);
		dma_ch_enable(port, ch, DMAC_CH_EN_DIS);
		
		if (get_timer(start) > timeout) 
		{
			//info("force dma stop error\n");
			return DMA_ST_TIME_OUT;
		}
	}

	// clear all interrupt status
	dma_clr_ch_int_st(port, ch, 0xFFFFFFFF);

	return DMA_ST_DONE;
}

/*****************************************************************************/
//  Function:  void dma_ch_config_src(E_DMA_PORT port, E_DMA_CH ch, E_DMA_WIDTH width, E_DMA_MSIZE msize, 
//	 						E_DMA_ADDR addr_inc, uint8_t dev_per, E_HS_SEL hs_sel, E_TT_FC tt_fc)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel
//
//		width	: data width
//
//		msize	: one burst transfer length
//
//		addr_inc	: address increment type
//		
//		dev_per	: a hardware handshaking interface
//		
//		hs_sel	: software or hardware handshaking select
//
//		tt_fc		: transfer type and flow control
//
//  Return:
//		none
//		
//  Description:
//       Configure the source parameter of a channel.
//
/*****************************************************************************/
void dma_ch_config_src(E_DMA_PORT port, E_DMA_CH ch, E_DMA_WIDTH width, E_DMA_MSIZE msize, 
	 						E_DMA_ADDR addr_inc, uint8_t dev_per, E_HS_SEL hs_sel, E_TT_FC tt_fc)
{
	struct brite_dma *priv = &g_dma[port];

	// source
	priv->ch_cfg[ch].src_width = width;
	priv->ch_cfg[ch].src_msize = msize;
	priv->ch_cfg[ch].src_addr_inc = addr_inc;
	priv->ch_cfg[ch].src_per = dev_per;
	priv->ch_cfg[ch].src_hs_sel = hs_sel;

	// common
	priv->ch_cfg[ch].tt_fc = tt_fc;
}

/*****************************************************************************/
//  Function:  void dma_ch_config_dst(E_DMA_PORT port, E_DMA_CH ch, E_DMA_WIDTH width, E_DMA_MSIZE msize,
//	 							E_DMA_ADDR addr_inc, uint8_t dev_per, E_HS_SEL hs_sel, E_TT_FC tt_fc)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel
//
//		width	: data width
//
//		msize	: one burst transfer length
//
//		addr_inc	: address increment type
//		
//		dev_per	: a hardware handshaking interface
//		
//		hs_sel	: software or hardware handshaking select
//
//		tt_fc		: transfer type and flow control
//
//  Return:
//		none
//		
//  Description:
//       Configure the destination parameter of a channel.
//
/*****************************************************************************/
void dma_ch_config_dst(E_DMA_PORT port, E_DMA_CH ch, E_DMA_WIDTH width, E_DMA_MSIZE msize,
	 						E_DMA_ADDR addr_inc, uint8_t dev_per, E_HS_SEL hs_sel, E_TT_FC tt_fc)
{
	struct brite_dma *priv = &g_dma[port];

	// destination
	priv->ch_cfg[ch].dst_width = width;
	priv->ch_cfg[ch].dst_msize = msize;	
	priv->ch_cfg[ch].dst_addr_inc = addr_inc;
	priv->ch_cfg[ch].dst_per = dev_per;
	priv->ch_cfg[ch].dst_hs_sel = hs_sel;

	// common
	priv->ch_cfg[ch].tt_fc = tt_fc;
}

/*****************************************************************************/
//  Function:  void dma_ch_config_hw(E_DMA_PORT port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t dst_addr, 
//							uint32_t src_addr, uint32_t len, E_DMAC_INT_EN int_en)
//
//  Parameters:
//		port		: dma port
//
//		ch		: channel
//	
//		prior		: priority
//
//		dst_addr	: destination address
//
//		src_addr	: source address
//
//		len		: transfer length(aligned by byte)
//		
//		int_en	: interrupt enable/disable
//
//  Return:
//		none
//		
//  Description:
//       Configure the destination parameter of a channel.
//
/*****************************************************************************/
void dma_ch_config_hw(E_DMA_PORT port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t dst_addr, 
							uint32_t src_addr, uint32_t len, E_DMAC_INT_EN int_en)
{
	struct brite_dma *priv = &g_dma[port];
	uint32_t reg_val;
	uint32_t block_ts;
	uint32_t mask = 0;

	// clear all interrupt status
	dma_clr_ch_int_st(port, ch, 0xFFFFFFFF);

	// interrupt mask configure
	mask = (int_en == DMAC_INT_EN) ? DMAC_INTST_TRANS_DONE_MASK : 0;
	dma_en_ch_int(port, ch, mask);	
	dma_en_ch_intsignal(port, ch, mask);	
	
	// initialize sar and dar
	dma_wr_reg(port, CHX_SAR_OFST(ch), src_addr);
	dma_wr_reg(port, CHX_DAR_OFST(ch), dst_addr);

	// set block transfer size
	block_ts = (len/(1<<priv->ch_cfg[ch].src_width)) - 1;
	dma_wr_reg(port, CHX_BLOCK_TS_OFST(ch), block_ts);

#if 1
	// set CHx_CTL
	reg_val = DMA_CHX_CTL_L32BITS_VAL(priv->ch_cfg[ch].dst_msize, 
									  priv->ch_cfg[ch].src_msize, 
									  priv->ch_cfg[ch].dst_width, 
									  priv->ch_cfg[ch].src_width, 
									  priv->ch_cfg[ch].dst_addr_inc, 
									  priv->ch_cfg[ch].src_addr_inc, 
									  MS0, 
									  MS0);
	dma_wr_reg(port, CHX_CTL_OFST(ch), reg_val);

	//info("CHX_CTL_OFST low : 0x%x\n", reg_val);

	// The burst length must be 2/4/8/16
	// The burst length couldn't across the 4KB boundary
	// AXI3 support ARLEN/AWLEN from 1 to 16, burst length = AxLEN[3:0] + 1
	reg_val = DMA_CHX_CTL_H32BITS_VAL(0, 0);
	dma_wr_reg(port, (CHX_CTL_OFST(ch) + 4), reg_val);

	//info("CHX_CTL_OFST high : 0x%x\n", reg_val);

	//set CHx_CFG
	reg_val = DMA_CHX_CFG_L32BITS_VAL(MB_TYPE_CONTIGUOUS, MB_TYPE_CONTIGUOUS);
	dma_wr_reg(port, CHX_CFG_OFST(ch), reg_val);

	//info("CHX_CFG_OFST low : 0x%x\n", reg_val);

	reg_val = DMA_CHX_CFG_H32BITS_VAL(prior, 
									  priv->ch_cfg[ch].dst_per, 
									  priv->ch_cfg[ch].src_per, 
									  priv->ch_cfg[ch].dst_hs_sel, 
									  priv->ch_cfg[ch].src_hs_sel, 
									  priv->ch_cfg[ch].tt_fc);
	dma_wr_reg(port, (CHX_CFG_OFST(ch) + 4), reg_val);

	//info("CHX_CFG_OFST high : 0x%x\n", reg_val);
#endif

#if 0
	// i2s1 tx
	dma_wr_reg(port, CHX_CTL_OFST(ch), 0x85340);
	dma_wr_reg(port, (CHX_CTL_OFST(ch) + 4), 0);

	dma_wr_reg(port, CHX_CFG_OFST(ch), 0);
	dma_wr_reg(port, (CHX_CFG_OFST(ch) + 4), 0x5001);
#endif

#if 0
	// tx
	dma_wr_reg(port, CHX_CTL_OFST(ch), 0x88040);
	dma_wr_reg(port, (CHX_CTL_OFST(ch) + 4), 0);

	dma_wr_reg(port, CHX_CFG_OFST(ch), 0);
	dma_wr_reg(port, (CHX_CFG_OFST(ch) + 4), 0xc001);
#endif

#if 0
	// rx
	//dma_wr_reg(port, CHX_CTL_OFST(ch), 0x88010);
	//dma_wr_reg(port, (CHX_CTL_OFST(ch) + 4), 0);

	//dma_wr_reg(port, CHX_CFG_OFST(ch), 0);
	//dma_wr_reg(port, (CHX_CFG_OFST(ch) + 4), 0x682);
#endif

	dma_en_ch_int(port, ch, DMAC_DMA_TFR_DONE_INTSTAT_MASK);

	// enable the channel to start dma
	dma_ch_enable(port, ch, DMAC_CH_EN_EN);

}


E_DMA_ST dma_i2s_tx_is_done(E_DMA_PORT dma_port, int i2s_port)
{
	uint32_t reg_val;
	
	reg_val = dma_rd_ch_int_st(dma_port, g_i2s[i2s_port].dma_ch_tx);
	dma_clr_ch_int_st(dma_port, g_i2s[i2s_port].dma_ch_tx, DMAC_DMA_TFR_DONE_INTSTAT_MASK);
	//info("\ndma_rd_ch_int_st: 0x%x\n", reg_val);
	return !!(reg_val & DMAC_DMA_TFR_DONE_INTSTAT_MASK);
}


/*****************************************************************************/
//	Function:  E_DMA_ST dma_polling_st(E_DMA_PORT port, E_DMA_CH ch)
//
//	Parameters:
//		port		: dma port
//
//		ch		: channel
//
//	Return:
//		dma status.
//		
//	Description:
//		 Polling channel status until it complete or timeout.
//
/*****************************************************************************/
E_DMA_ST dma_polling_st(E_DMA_PORT port, E_DMA_CH ch)
{
	uint32_t reg_val;
	int timeout = 500; 			// ms
	u64 start = get_timer(0);
	
	while (1) 
	{
		reg_val = dma_rd_ch_int_st(port, ch);
		//info("\ndma_rd_ch_int_st: 0x%x\n", reg_val);
		if (reg_val & DMAC_DMA_TFR_DONE_INTSTAT_MASK)
		{
			//info("\nch:0x%x, st: 0x%x\n", ch, reg_val);
			
			// should check transfer done and no err
			if (reg_val & DMAC_INTST_TRANS_ERR_MASK)
			{
				return DMA_ST_FAIL;
			}
			else
			{
				return DMA_ST_DONE;
			}
		}

		if (get_timer(start) > timeout) 
		{
			info("ch:0x%x force dma stop error\n", ch);
			return DMA_ST_TIME_OUT;
		}		
	}

	return DMA_ST_DONE;
}

/*****************************************************************************/
//	Function:  E_DMA_ST dma_m2m(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t dst_addr, uint32_t src_addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		ch		: channel
//
//		dst_addr	: destination address
//
//		src_addr	: source address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from memory to memory.
//
/*****************************************************************************/
E_DMA_ST dma_m2m(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t dst_addr, uint32_t src_addr, uint32_t len, E_ST_POLLING st_polling)
{
	E_DMA_PORT port = DMA0;
	
	dma_ch_select(port, ch);
	
	dma_ch_config_src(port, ch, DMA_WIDTH_BITS_64, DATA_ITEMS_16, ADDR_INC, 0, HS_SEL_SW, MEM_TO_MEM_DMAC);

	dma_ch_config_dst(port, ch, DMA_WIDTH_BITS_64, DATA_ITEMS_16, ADDR_INC, 0, HS_SEL_SW, MEM_TO_MEM_DMAC);	

	dma_ch_config_hw(port, ch, prior, dst_addr, src_addr, len, DMAC_INT_EN);	

	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(port, ch);
	}

	return DMA_ST_DONE;
}

/*****************************************************************************/
//	Function:  E_DMA_ST dma_uart_tx(E_UART_PORT port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		port		: uart port
//
//		ch		: dma channel
//
//		prior		: dma priority
//
//		addr		: memory address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from memory to uart tx fifo.
//
/*****************************************************************************/
E_DMA_ST dma_uart_tx(E_UART_PORT port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
{
	E_DMA_PORT dma_port = DMA0;
	uint32_t dst_addr = g_uart[port].base + UART_THR_OFST;
	uint8_t dev_per = (port == UART0) ? DMA_REQ_UART0_TX : DMA_REQ_UART1_TX;
	
	dma_ch_select(dma_port, ch);
	
	dma_ch_config_src(dma_port, ch, DMA_WIDTH_BITS_8, DATA_ITEM_1, ADDR_INC, dev_per, HS_SEL_HW, MEM_TO_PER_DMAC);

	dma_ch_config_dst(dma_port, ch, DMA_WIDTH_BITS_8, DATA_ITEM_1, ADDR_FIXED, dev_per, HS_SEL_HW, MEM_TO_PER_DMAC);	

	dma_ch_config_hw(dma_port, ch, prior, dst_addr, addr, len, DMAC_INT_DIS);	

	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(dma_port, ch);
	}

	return DMA_ST_DONE;
}

/*****************************************************************************/
//	Function:  E_DMA_ST dma_uart_rx(E_UART_PORT port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		port		: uart port
//
//		ch		: dma channel
//
//		prior		: dma priority
//
//		addr		: memory address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from uart rx fifo to memory.
//
/*****************************************************************************/
E_DMA_ST dma_uart_rx(E_UART_PORT port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
{
	E_DMA_PORT dma_port = DMA0;
	uint32_t src_addr = g_uart[port].base + UART_RBR_OFST;
	uint8_t dev_per = (port == UART0) ? DMA_REQ_UART0_RX : DMA_REQ_UART1_RX;
	
	dma_ch_select(dma_port, ch);

	dma_ch_config_src(dma_port, ch, DMA_WIDTH_BITS_8, DATA_ITEMS_4, ADDR_FIXED, dev_per, HS_SEL_HW, PER_TO_MEM_DMAC);

	dma_ch_config_dst(dma_port, ch, DMA_WIDTH_BITS_32, DATA_ITEM_1, ADDR_INC, dev_per, HS_SEL_HW, PER_TO_MEM_DMAC);		

	dma_ch_config_hw(dma_port, ch, prior, addr, src_addr, len, DMAC_INT_DIS);

	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(dma_port, ch);
	}

	return DMA_ST_DONE;
}

/*****************************************************************************/
//	Function:  dma_spi_tx(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		ch		: dma channel
//
//		prior		: dma priority
//
//		addr		: memory address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from memory to spi tx fifo.
//
/*****************************************************************************/
E_DMA_ST dma_spi_tx(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
{
	E_DMA_PORT dma_port = DMA0;
	struct brite_spi *priv = &g_spi[SPI0];
	uint32_t dst_addr = priv->base + SPI_SSPDR_OFST;
	uint8_t dev_per = DMA_REQ_SPI_TX;
	E_DMA_WIDTH width;
	
	dma_ch_select(dma_port, ch);

	// configure data width upon the spi ds
	width = (priv->ds <= 8) ? DMA_WIDTH_BITS_8 : DMA_WIDTH_BITS_16;
	
	dma_ch_config_src(dma_port, ch, width, DATA_ITEM_1, ADDR_INC, dev_per, HS_SEL_HW, MEM_TO_PER_DMAC);
	
	dma_ch_config_dst(dma_port, ch, width, DATA_ITEM_1, ADDR_FIXED, dev_per, HS_SEL_HW, MEM_TO_PER_DMAC);

	dma_ch_config_hw(dma_port, ch, prior, dst_addr, addr, len, DMAC_INT_DIS);	

	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(dma_port, ch);
	}

	return DMA_ST_DONE;
}

/*****************************************************************************/
//	Function:  E_DMA_ST dma_spi_rx(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		ch		: dma channel
//
//		prior		: dma priority
//
//		addr		: memory address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from spi rx fifo to memory.
//
/*****************************************************************************/
E_DMA_ST dma_spi_rx(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
{
	E_DMA_PORT dma_port = DMA0;
	struct brite_spi *priv = &g_spi[SPI0];
	uint32_t src_addr = priv->base + SPI_SSPDR_OFST;
	uint8_t dev_per = DMA_REQ_SPI_RX;
	E_DMA_WIDTH width;
	
	dma_ch_select(dma_port, ch);

	// configure data width upon the spi ds
	width = (priv->ds <= 8) ? DMA_WIDTH_BITS_8 : DMA_WIDTH_BITS_16;

	dma_ch_config_src(dma_port, ch, width, DATA_ITEMS_4, ADDR_FIXED, dev_per, HS_SEL_HW, PER_TO_MEM_DMAC);
	
	dma_ch_config_dst(dma_port, ch, width, DATA_ITEMS_4, ADDR_INC, dev_per, HS_SEL_HW, PER_TO_MEM_DMAC);

	dma_ch_config_hw(dma_port, ch, prior, addr, src_addr, len, DMAC_INT_DIS);	

	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(dma_port, ch);
	}

	return DMA_ST_DONE;
}

/*****************************************************************************/
//	Function:  E_DMA_ST dma_i2s_tx(E_I2S_PORT i2s_port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		i2s_port	: i2s port
//
//		ch		: dma channel
//
//		prior		: dma priority
//
//		addr		: memory address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from memory to i2s_dac /i2s1 tx fifo.
//
/*****************************************************************************/
E_DMA_ST dma_i2s_tx(E_I2S_PORT i2s_port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
{
	struct brite_i2s *p_i2s = &g_i2s[i2s_port];
	E_DMA_PORT dma_port = DMA0;
	uint32_t dst_addr = p_i2s->base + I2S_TXDMA_OFST;
	uint8_t dev_per = (i2s_port == I2S_DAC) ? DMA_REQ_I2S_DAC : DMA_REQ_I2S1;
	E_DMA_WIDTH src_width, dst_width;
	E_DMA_MSIZE src_msize, dst_msize;

	if (p_i2s->sample_format == SNDRV_PCM_FORMAT_S16_LE)
	{
		src_width = DMA_WIDTH_BITS_32;
		src_msize = DATA_ITEMS_4;
	
		dst_width = DMA_WIDTH_BITS_16;
		dst_msize = DATA_ITEMS_8;
	}
	else
	{
		src_width = DMA_WIDTH_BITS_64;
		src_msize = DATA_ITEMS_4;		
		
		dst_width = DMA_WIDTH_BITS_32;
		dst_msize = DATA_ITEMS_8;	
	}

	dma_ch_select(dma_port, ch);
	
	dma_ch_config_src(dma_port, ch, src_width, src_msize, ADDR_INC, dev_per, HS_SEL_HW, MEM_TO_PER_DMAC);
	
	dma_ch_config_dst(dma_port, ch, dst_width, dst_msize, ADDR_FIXED, dev_per, HS_SEL_HW, MEM_TO_PER_DMAC);

	dma_ch_config_hw(dma_port, ch, prior, dst_addr, addr, len, DMAC_INT_EN);

	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(dma_port, ch);
	}

	return DMA_ST_DONE;
}

/*****************************************************************************/
//	Function:  E_DMA_ST dma_i2s_tx(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		ch		: dma channel
//
//		prior		: dma priority
//
//		addr		: memory address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from i2s_adc rx fifo to memory.
//
/*****************************************************************************/
E_DMA_ST dma_i2s_rx(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
{
	struct brite_i2s *p_i2s = &g_i2s[I2S_ADC];
	E_DMA_PORT dma_port = DMA0;
	uint32_t src_addr = p_i2s->base + I2S_RXDMA_OFST;
	uint8_t dev_per = DMA_REQ_I2S_ADC;
	E_DMA_WIDTH src_width, dst_width;
	E_DMA_MSIZE src_msize, dst_msize;

	if (p_i2s->sample_format == SNDRV_PCM_FORMAT_S16_LE)
	{
		src_width = DMA_WIDTH_BITS_16;
		src_msize = DATA_ITEMS_8;

		dst_width = DMA_WIDTH_BITS_32;
		dst_msize = DATA_ITEMS_4;
	}
	else
	{
		src_width = DMA_WIDTH_BITS_32;
		src_msize = DATA_ITEMS_8;

		dst_width = DMA_WIDTH_BITS_64;
		dst_msize = DATA_ITEMS_4;		
	}

	dma_ch_select(dma_port, ch);
	
	dma_ch_config_src(dma_port, ch, src_width, src_msize, ADDR_FIXED, dev_per, HS_SEL_HW, PER_TO_MEM_DMAC);

	dma_ch_config_dst(dma_port, ch, dst_width, dst_msize, ADDR_INC, dev_per, HS_SEL_HW, PER_TO_MEM_DMAC);	

	dma_ch_config_hw(dma_port, ch, prior, addr, src_addr, len, DMAC_INT_DIS);	

	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(dma_port, ch);
	}

	return DMA_ST_DONE;
}


/*****************************************************************************/
//	Function:  E_DMA_ST dma_i2c_tx(struct dw_i2c *p_priv, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		p_priv	: i2c pointer
//
//		ch		: dma channel
//
//		prior		: dma priority
//
//		addr		: memory address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from memory to i2c tx fifo.
//
/*****************************************************************************/
E_DMA_ST dma_i2c_tx(struct dw_i2c *p_priv, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
{
	E_DMA_PORT dma_port = DMA0;
	uint32_t dst_addr = p_priv->regs + I2C_DATA_CMD_OFST;
	uint8_t dev_per = (p_priv->regs == I2C0_BASE) ? DMA_REQ_I2C_S_TX : DMA_REQ_I2C_M_TX;
	
	dma_ch_select(dma_port, ch);
	
	dma_ch_config_src(dma_port, ch, DMA_WIDTH_BITS_8, DATA_ITEM_1, ADDR_INC, dev_per, HS_SEL_HW, MEM_TO_PER_DMAC);

	dma_ch_config_dst(dma_port, ch, DMA_WIDTH_BITS_8, DATA_ITEM_1, ADDR_FIXED, dev_per, HS_SEL_HW, MEM_TO_PER_DMAC);	

	dma_ch_config_hw(dma_port, ch, prior, dst_addr, addr, len, DMAC_INT_DIS);	

	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(dma_port, ch);
	}

	return DMA_ST_DONE;
}

/*****************************************************************************/
//	Function:  E_DMA_ST dma_i2c_rx(struct dw_i2c *p_priv, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		p_priv	: i2c pointer
//
//		ch		: dma channel
//
//		prior		: dma priority
//
//		addr		: memory address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from i2c rx fifo to memory.
//
/*****************************************************************************/
E_DMA_ST dma_i2c_rx(struct dw_i2c *p_priv, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
{
	E_DMA_PORT dma_port = DMA0;
	uint32_t src_addr = p_priv->regs + I2C_DATA_CMD_OFST;
	uint8_t dev_per = (p_priv->regs == I2C0_BASE) ? DMA_REQ_I2C_S_RX : DMA_REQ_I2C_M_RX;
	
	dma_ch_select(dma_port, ch);
	
	dma_ch_config_src(dma_port, ch, DMA_WIDTH_BITS_8, DATA_ITEMS_4, ADDR_FIXED, dev_per, HS_SEL_HW, PER_TO_MEM_DMAC);

	dma_ch_config_dst(dma_port, ch, DMA_WIDTH_BITS_32, DATA_ITEM_1, ADDR_INC, dev_per, HS_SEL_HW, PER_TO_MEM_DMAC);	

	dma_ch_config_hw(dma_port, ch, prior, addr, src_addr, len, DMAC_INT_DIS);

	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(dma_port, ch);
	}

	return DMA_ST_DONE;
}

/*****************************************************************************/
//	Function:  dma_usb_tx(struct musb_hw_ep *hw_ep, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		ch		: dma channel
//
//		prior		: dma priority
//
//		addr		: memory address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from memory to usb ep tx fifo.
//
/*****************************************************************************/
E_DMA_ST dma_usb_tx(struct musb_hw_ep *hw_ep, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
{
	E_DMA_PORT dma_port = DMA0;
	uint32_t dst_addr = (uint32_t)hw_ep->fifo;
	uint8_t dev_per = hw_ep->epnum - 1;

    //info("dma_usb_tx:ep%d, src 0x%x fifo_reg:0x%x per %d\n", hw_ep->epnum, addr, dst_addr, dev_per);

	dma_ch_select(dma_port, ch);
	dma_ch_config_src(dma_port, ch, DMA_WIDTH_BITS_64, DATA_ITEMS_32, ADDR_INC, dev_per, HS_SEL_HW, MEM_TO_PER_DMAC);
	dma_ch_config_dst(dma_port, ch, DMA_WIDTH_BITS_8, DATA_ITEMS_256, ADDR_FIXED, dev_per, HS_SEL_HW, MEM_TO_PER_DMAC);
	dma_ch_config_hw(dma_port, ch, prior, dst_addr, addr, len, DMAC_INT_DIS);	
	switch (hw_ep->epnum) {
	case 1:
		_SET_SYSCON_REG_dma_req_control_ep1_switch_331(1);
		break;
	case 2:
		_SET_SYSCON_REG_dma_req_control_ep2_switch_332(1);
		break;
	case 3:
		_SET_SYSCON_REG_dma_req_control_ep3_switch_333(1);
		break;
	default:
		info("unsupport usb ep dma\n");
		break;
	}

	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(dma_port, ch);
	}

	return DMA_ST_DONE;
}

/*****************************************************************************/
//	Function:  E_DMA_ST dma_usb_rx(struct musb_hw_ep *hw_ep, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
//
//	Parameters:
//		ch		: dma channel
//
//		prior		: dma priority
//
//		addr		: memory address
//
//		len 		: transfer length(byte aligned)
//					the max lenght should be less than DMA_MAX_BURST_SIZE
//
//		st_polling	: polling status flag
//
//	Return:
//		dma status.
//		
//	Description:
//		 DMA transfer data from usb ep rx fifo to memory.
//
/*****************************************************************************/
E_DMA_ST dma_usb_rx(struct musb_hw_ep *hw_ep, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling)
{
	E_DMA_PORT dma_port = DMA0;
	uint32_t src_addr = (uint32_t)hw_ep->fifo;
	uint8_t dev_per = hw_ep->epnum - 1;

    //info("dma_usb_rx:ep%d, dest 0x%x fifo_reg:0x%x per %d\n", hw_ep->epnum, addr, src_addr, dev_per);

	dma_ch_select(dma_port, ch);
	dma_ch_config_src(dma_port, ch, DMA_WIDTH_BITS_8, DATA_ITEMS_256, ADDR_FIXED, dev_per, HS_SEL_HW, PER_TO_MEM_DMAC);
	dma_ch_config_dst(dma_port, ch, DMA_WIDTH_BITS_64, DATA_ITEMS_64, ADDR_INC, dev_per, HS_SEL_HW, PER_TO_MEM_DMAC);
	dma_ch_config_hw(dma_port, ch, prior, addr, src_addr, len, DMAC_INT_DIS);	
	switch (hw_ep->epnum) {
	case 1:
		_SET_SYSCON_REG_dma_req_control_ep1_switch_331(0);
		break;
	case 2:
		_SET_SYSCON_REG_dma_req_control_ep2_switch_332(0);
		break;
	case 3:
		_SET_SYSCON_REG_dma_req_control_ep3_switch_333(0);
		break;
	default:
		info("unsupport usb ep dma\n");
		break;
	}


	if (st_polling == ST_POLLING)
	{
		return dma_polling_st(dma_port, ch);
	}

	return DMA_ST_DONE;
}
