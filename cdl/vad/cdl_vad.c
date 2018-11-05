/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_vad.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct brite_vad g_vad;

// interrtupt
volatile int8_t g_spint_cnt = 0;
volatile int8_t g_slint_cnt = 0;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//  Function:  void vad_wr_reg(uint32_t reg_ofst, uint32_t val)
//
//  Parameters:
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
void vad_wr_reg(uint32_t reg_ofst, uint32_t val)
{
	__raw_writel(val, (g_vad.base + reg_ofst));
}

/*****************************************************************************/
//  Function:  uint32_t vad_rd_reg(uint32_t reg_ofst)
//
//  Parameters:
//		reg_ofst		: the offset of a register
//
//  Return:
//		The value from a register
//		
//  Description:
//      Read a value from a register.
//
/*****************************************************************************/
uint32_t vad_rd_reg(uint32_t reg_ofst)
{
	return __raw_readl(g_vad.base + reg_ofst);
}


/*****************************************************************************/
//	Function:  static void vad_lr_ctrl(E_VAD_LR_SEL lr_sel, E_VAD_HL16BIT_SEL hl_16bits)
//
//	Parameters:
//		lr_sel	: left/right mode
//
//		hl_16bits	: high/low 16bits
//
//	Return:
//		none
//		
//	Description:
//		control left/right channel and high/low 16 bits.
//
/*****************************************************************************/
static void vad_lr_ctrl(E_VAD_LR_SEL lr_sel, E_VAD_HL16BIT_SEL hl_16bits)
{	
	vad_wr_reg(VAD_LR_SEL_OFST, lr_sel);

	if (lr_sel == LR_SEL_LEFT)
	{
		vad_wr_reg(VAD_LEFT_WD_OFST, hl_16bits);
	}
	else
	{
		vad_wr_reg(VAD_RIGHT_WD_OFST, hl_16bits);
	}
}

/*****************************************************************************/
//  Function:  void vad_sw_ctrl(E_VAD_SW_MODE mode)
//
//  Parameters:
//		mode	: sw mode
//
//  Return:
//		none
//		
//  Description:
//      VAD SW control to vad or adc mode.
//
/*****************************************************************************/
void vad_sw_ctrl(E_VAD_SW_MODE mode)
{
	uint32_t reg_val = vad_rd_reg(VAD_SW_OFST);

	reg_val &= ~(VAD_ENA_MASK | ADC_ENA_MASK);
	
	if (mode == VAD_SW_MODE_VAD)
	{
		reg_val = VAD_ENA_MASK;
	}
	else if (mode == VAD_SW_MODE_ADC)
	{
		reg_val = ADC_ENA_MASK;
	}
	else if (mode == VAD_SW_MODE_BOTH)
	{	
		reg_val = ADC_ENA_MASK | VAD_ENA_MASK;
	}
	else
	{
		// none
	}
	
	vad_wr_reg(VAD_SW_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void vad_set_stop_dly(E_VAD_STOP_DLY dly)
//
//  Parameters:
//		dly	: delay for n sample
//
//  Return:
//		none
//		
//  Description:
//      delay n sample.
//
/*****************************************************************************/
void vad_set_stop_dly(E_VAD_STOP_DLY dly)
{
	vad_wr_reg(VAD_STOP_DELAY_OFST, dly);
}

/*****************************************************************************/
//  Function:  void vad_set_start_wrap_addr(uint32_t st_addr, uint32_t size)
//
//  Parameters:
//		st_addr	: vad memory start address(1 indicate 8 bytes)
//
//		size		: xmem size(1 indicate 8 bytes)
//
//  Return:
//		none
//		
//  Description:
//      set start and wrap address.
//
/*****************************************************************************/
void vad_set_start_wrap_addr(uint32_t st_addr, uint32_t size)
{		
	vad_wr_reg(VAD_ADDR_START_OFST, (st_addr & 0x1FFF));
	vad_wr_reg(VAD_ADDR_WRAP_OFST, ((st_addr + size) & 0x3FFF));
}

/*****************************************************************************/
//  Function:  void vad_switch_xmem(E_VAD_MEM_SW_MODE mode)
//
//  Parameters:
//		mode	: switch xmem mode
//
//  Return:
//		none
//		
//  Description:
//      switch smem to vad or axi.
//
/*****************************************************************************/
void vad_switch_xmem(E_VAD_MEM_SW_MODE mode)
{
	vad_wr_reg(VAD_MEM_SW_OFST, mode);
}

/*****************************************************************************/
//  Function:  void vad_switch_xmem(E_VAD_MEM_SW_MODE mode)
//
//  Parameters:
//		mode	: switch xmem mode
//
//  Return:
//		none
//		
//  Description:
//      switch smem to vad or axi.
//
/*****************************************************************************/
void vad_normal_to_idle_init(E_VAD_LR_SEL lr_sel, E_VAD_STOP_DLY stop_dly)
{
	// configure vad(normal -> idle)

    // 1. DSP 配置CLK_GEN中的寄存器，关闭XMEM的CLK_EN
    _DISABLE_CLOCK_vad_mem_clk_129_

	// 2. DSP 配置CLK_GEN中的寄存器，打开I2S_VAD的CLK_EN
	_ENABLE_CLOCK_i2svad_pclk_129_

	// 3. DSP 配置RST_GEN中的寄存器，置位i2svad_sresetn持续为1'b0
    _ASSERT_RESET_i2svad_sresetn_129_

	// 4. DSP配置SYSCON的寄存器XMEM_SW=1'b1; 将XMEM的数据通路切换到I2S_VAD
    vad_switch_xmem(SWITCH_XMEM_TO_VAD);

	// 5. DSP配置CLK_GEN寄存器XMEM的clock与I2S_VAD的BCLK同频
    _SWITCH_CLOCK_vad_mem_clk_129_SOURCE_i2sadc_bclk

	// 6. DSP 配置CLK_GEN中的寄存器，打开XMEM的CLK_EN
    _ENABLE_CLOCK_vad_mem_clk_129_

	// 7. DSP 配置RST_GEN中的寄存器，释放i2svad_sresetn为1'b1
    _CLEAR_RESET_i2svad_sresetn_129_

	// 8. DSP 配置I2S_VAD中的寄存器LR_SEL选择声道源
	vad_lr_ctrl(lr_sel, VAD_SEL_H16BITS);

	// 9. DSP 配置I2S_VAD中的寄存器STOPDLY设定采样enable的延时量
	vad_set_stop_dly(stop_dly);

	// 10.  DSP配置I2S_VAD的寄存器，设定数据缓冲到XMEM的起始地址
	// 11.  DSP配置I2S_VAD的寄存器，设定数据缓冲到XMEM的回绕地址
	vad_set_start_wrap_addr(0x0, 0x2000);

	// 12.  DSP配置VAD的寄存器VAD_SW中bit[0]: VAD_ena = 1'b1, 启动I2S_VAD中的VAD功能和XMEM的push功能（both）
	// 12.  DSP配置VAD的寄存器VAD_SW中bit[1]: ADC_ena = 1'b0, 关闭I2S_ADC的数据采集通路╠isable the Gating of LRCK of I2S_ADC）
	vad_sw_ctrl(VAD_SW_MODE_VAD);

	// 13.  DSP配置CLK_GEN，完成idle模式下所有其他模块的时钟配置。
	// 14.  DSP配置SYS_CON的IDEL_EN； PMU关闭system的主电源 
	//		（DSP和主系统进入Idel模式，持续进行人声判别和数据采集、循环存储）
	// TBD
}

void vad_spint_handle(void *ptr) 
{
	uint32_t st;

	info("\n%s\n", __func__);

	st = vad_rd_reg(VAD_SPINT_OFST);
	if (st != 1) 
	{
		info("vad_spint st fail, st: %x\n", st);
		return;
	}

	g_spint_cnt++;

	// clear the status
	vad_wr_reg(VAD_SPINT_CLR_OFST, 1);
}

void vad_slint_handle(void *ptr) 
{
	uint32_t st;

	info("\n%s\n", __func__);
	
	st = vad_rd_reg(VAD_SLINT_OFST);
	if (st != 1) 
	{
		info("vad_slint st fail, st: %x\n", st);
		return;
	}

	g_slint_cnt++;
	
	// clear the status
	vad_wr_reg(VAD_SLINT_CLR_OFST, 1);
}

/*****************************************************************************/
//  Function:  void vad_init(void)
//
//  Parameters:
//		none
//
//  Return:
//		none
//		
//  Description:
//      VAD initialize.
//
/*****************************************************************************/
void vad_init(void)
{
	g_vad.base = VAD_BASE;
}

