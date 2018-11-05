/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_ptc.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct brite_ptc g_ptc;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//  Function:  void ptc_wr_reg(E_PTC_PORT port, uint32_t reg_ofst, uint32_t val)
//
//  Parameters:
//		port			: ptc port
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
void ptc_wr_reg(E_PTC_PORT port, uint32_t reg_ofst, uint32_t val)
{
	__raw_writel(val, g_ptc.base + (port * PTC_PORT_SIZE) + reg_ofst);
}

/*****************************************************************************/
//  Function:  uint32_t ptc_rd_reg(E_PTC_PORT port, uint32_t reg_ofst)
//
//  Parameters:
//		port			: ptc port
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
uint32_t ptc_rd_reg(E_PTC_PORT port, uint32_t reg_ofst)
{
	return __raw_readl(g_ptc.base + (port * PTC_PORT_SIZE) + reg_ofst);
}

/*****************************************************************************/
//  Function:  void ptc_start(E_PTC_PORT port)
//
//  Parameters:
//		port			: ptc port
//
//  Return:
//		none
//		
//  Description:
//      PTC start.
//
/*****************************************************************************/
void ptc_start(E_PTC_PORT port)
{
	uint32_t reg_val = ptc_rd_reg(port, PTC_CTRL_OFST);

	reg_val |= PTC_EN_MASK;
	ptc_wr_reg(port, PTC_CTRL_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void ptc_stop(E_PTC_PORT port)
//
//  Parameters:
//		port			: ptc port
//
//  Return:
//		none
//		
//  Description:
//      PTC stop.
//
/*****************************************************************************/
void ptc_stop(E_PTC_PORT port)
{
	uint32_t reg_val = ptc_rd_reg(port, PTC_CTRL_OFST);

	reg_val &= ~PTC_EN_MASK;
	ptc_wr_reg(port, PTC_CTRL_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void ptc_init(uint32_t base, uint32_t pclk)
//
//  Parameters:
//		base		: wdog base address
//
//		pclk		: wdog input clock
//
//  Return:
//		none
//
//  Description:
//       Initialize the base and input clock of ptc
//
/*****************************************************************************/
void ptc_init(uint32_t base, uint32_t pclk)
{
	struct brite_ptc *priv = &g_ptc;
	
	mem_set8(priv, 0, sizeof(struct brite_ptc));
	
	priv->base = base;
	priv->pclk = pclk;
}

/*****************************************************************************/
//  Function:  void ptc_pwm_init(E_PTC_PORT port, uint32_t hrc, uint32_t lrc)
//
//  Parameters:
//		port			: ptc port
//
//		hrc		: the number of clock cycles after reset of the RPTC_CNTR when PWM output should go high
//
//		lrc		: the number of clock cycles after reset of the RPTC_CNTR when PWM output should go low
//
//  Return:
//		none
//
//  Description:
//       PWM mode initialize.
//
/*****************************************************************************/
void ptc_pwm_init(E_PTC_PORT port, uint32_t hrc, uint32_t lrc)
{
	uint32_t reg_val;
	
	// set hrc
	ptc_wr_reg(port, PTC_HRC_OFST, hrc);

	// set lrc
	ptc_wr_reg(port, PTC_LRC_OFST, lrc);

	// set control
	//reg_val =  SPI_OE_MASK | SPI_CNTRRST_MASK;
	reg_val =  SPI_OE_MASK;
	ptc_wr_reg(port, PTC_CTRL_OFST, reg_val);
}


