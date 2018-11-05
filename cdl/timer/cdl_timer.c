/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_timer.h"
#include "cdl_sys.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct brite_timer g_timer[NUM_TIMER_PORTS];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//  Function:  timer_int_handler(void *ptr)
//
//  Parameters:
//		ptr [in]	: pointer
//
//  Return:
//		none
//		
//  Description:
//     Timer interrupt hadler.
//
/*****************************************************************************/
#if 0
void timer_int_handler(void *ptr)
{
	
}
#endif

/*****************************************************************************/
//  Function:  void timer_wr_reg(E_TIMER_PORT port, uint32_t reg_ofst, uint32_t val)
//
//  Parameters:
//		port			: timer port
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
void timer_wr_reg(E_TIMER_PORT port, uint32_t reg_ofst, uint32_t val)
{
	__raw_writel(val, (g_timer[port].base + reg_ofst));
}

/*****************************************************************************/
//  Function:  uint32_t timer_rd_reg(E_TIMER_PORT port, uint32_t reg_ofst)
//
//  Parameters:
//		port			: timer port
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
uint32_t timer_rd_reg(E_TIMER_PORT port, uint32_t reg_ofst)
{
	return __raw_readl(g_timer[port].base + reg_ofst);
}

/*****************************************************************************/
//  Function:  BOOL timer_verify_periph_id(E_TIMER_PORT port)
//
//  Parameters:
//		port		: timer port
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
BOOL timer_verify_periph_id(E_TIMER_PORT port)
{
	uint8_t i, id[4];

	// read the peripheral id from the register
	for (i = 0; i < 4; i++)
	{
		id[i] = timer_rd_reg(port, (TIMER_PERIPHID0_OFST + (i<<2)));
		id[i] &= TIMER_PERIPHID_MASK;
	}

	// verify the validity of the peripheral id
	if ((id[0] == TIMER_PERIPHID0) && 
		(id[1] == TIMER_PERIPHID1) && 
		(id[2] == TIMER_PERIPHID2) && 
		(id[3] == TIMER_PERIPHID3)) 
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************/
//  Function:  BOOL timer_verify_pcell_id(E_TIMER_PORT port)
//
//  Parameters:
//		port		: timer port
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
BOOL timer_verify_pcell_id(E_TIMER_PORT port)
{
	uint8_t i, id[4];

	// read the primecell id from the register
	for (i = 0; i < 4; i++)
	{
		id[i] = timer_rd_reg(port, (TIMER_PCELLID0_OFST + (i<<2)));
		id[i] &= TIMER_PCELLID_MASK;
	}

	// verify the validity of the primecell id
	if ((id[0] == TIMER_PCELLID0) && 
		(id[1] == TIMER_PCELLID1) && 
		(id[2] == TIMER_PCELLID2) && 
		(id[3] == TIMER_PCELLID3)) 
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************/
//  Function:  void wdog_set_load(uint32_t val)
//
//  Parameters:
//		port		: timer port
//
//		val		: load value
//
//  Return:
//		none
//
//  Description:
//       Set timer load value.
// When this register is written to directly, the current count is immediately reset to the 
// new value at the next rising edge of TIMCLK which is enabled by TIMCLKEN.
// The value in this register is also overwritten if the TimerXBGLoad register is written 
// to, but the current count is not immediately affected.
//
/*****************************************************************************/
void timer_set_load(E_TIMER_PORT port, uint32_t val)
{
	timer_wr_reg(port, TIMER_LOAD_OFST, val);
}

/*****************************************************************************/
//  Function:  uint32_t timer_get_value(E_TIMER_PORT port)
//
//  Parameters:
//		port		: timer port
//
//  Return:
//		The current value of the timer counter 
//		
//  Description:
//      Get the current counter of timer .
//
/*****************************************************************************/
uint32_t timer_get_value(E_TIMER_PORT port)
{
	return timer_rd_reg(port, TIMER_VALUE_OFST);
}


/*****************************************************************************/
//  Function:  void timer_set_ctrl(E_TIMER_PORT port, E_TIMER_SHORTCNT short_cnt, E_TIMER_SIZE size, E_TIMER_DIV div,
//						 E_TIMER_INT_EN int_en, E_TIMER_MODE mode, E_TIMER_EN timer_en)
//
//  Parameters:
//		port		: timer port
//
//		short_cnt	: selects one-shot or wrapping counter mode
//
//		size		: selects 16/32 bit counter operation
//
//		div		: prescale
//
//		int_en	: interrupt Enable
//
//		mode	: mode	
//
//		timer_en	: timer enable
//
//  Return:
//		none
//
//  Description:
//       Set timer control mode.
//
/*****************************************************************************/
void timer_set_ctrl(E_TIMER_PORT port, E_TIMER_SHORTCNT short_cnt, E_TIMER_SIZE size, E_TIMER_DIV div,
						 E_TIMER_INT_EN int_en, E_TIMER_MODE mode, E_TIMER_EN timer_en)
{
	uint32_t reg_val;

	reg_val = TIMER_ONE_SHORT_COUNT_VAL(short_cnt) 	|
				TIMER_SIZE_VAL(size)				|
				TIMER_PRE_DIV_VAL(div)				|
				TIMER_INT_EN_VAL(int_en)			|
				TIMER_MODE_VAL(mode)				|
				TIMER_EN_VAL(timer_en);

	timer_wr_reg(port, TIMER_CONTROL_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void timer_enable_ctrl(E_TIMER_PORT port, E_TIMER_EN timer_en)
//
//  Parameters:
//		port		: timer port
//
//		timer_en	: timer enable
//
//  Return:
//		none
//
//  Description:
//       Set timer enable/disable.
//
/*****************************************************************************/
void timer_enable_ctrl(E_TIMER_PORT port, E_TIMER_EN timer_en)
{
	uint32_t reg_val = timer_rd_reg(port, TIMER_CONTROL_OFST);
	
	// set enable control
	reg_val &= ~TIMER_EN_MASK;
	reg_val |= TIMER_EN_VAL(timer_en);

	timer_wr_reg(port, TIMER_CONTROL_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void timer_clr_int(E_TIMER_PORT port)
//
//  Parameters:
//		port		: timer port
//
//  Return:
//		none
//
//  Description:
//       Clear the interrupt output from the counter.
//
/*****************************************************************************/
void timer_clr_int(E_TIMER_PORT port)
{
	// Any write to this register will clear the interrupt output from the counter.
	timer_wr_reg(port, TIMER_INTCLR_OFST, 0x12345678);
}

/*****************************************************************************/
//  Function:  uint32_t timer_get_ris(E_TIMER_PORT port)
//
//  Parameters:
//		port	: timer port
//
//  Return:
//		The raw interrupt status
// 			= 0, no raw interrupt
//			= 1, raw interrupt
//
//  Description:
//       Get the raw interrupt status.
//
/*****************************************************************************/
uint32_t timer_get_ris(E_TIMER_PORT port)
{
	uint32_t reg_val = timer_rd_reg(port, TIMER_RIS_OFST);
	
	reg_val &= TIMER_INT_ST_MASK;
	
	return reg_val;
}

/*****************************************************************************/
//  Function:  uint32_t timer_get_mis(E_TIMER_PORT port)
//
//  Parameters:
//		port	: timer port
//
//  Return:
//		The pin masked interrupt status
// 			= 0, no masked interrupt
//			= 1, masked interrupt
//
//  Description:
//       Get the masked interrupt status.
//
/*****************************************************************************/
uint32_t timer_get_mis(E_TIMER_PORT port)
{
	uint32_t reg_val = timer_rd_reg(port, TIMER_MIS_OFST);
	
	reg_val &= TIMER_INT_ST_MASK;
	
	return reg_val;
}

/*****************************************************************************/
//  Function:  void timer_set_bgload(E_TIMER_PORT port, uint32_t val)
//
//  Parameters:
//		port		: timer port
//
//		val		: load value
//
//  Return:
//		none
//
//  Description:
//       Set timer bgload value.
//  This register provides an alternative method of accessing the TimerXLoad register. The 
//  difference is that writes to TimerXBGLoad will not cause the counter immediately to 
//  restart from the new value.
//
/*****************************************************************************/
void timer_set_bgload(E_TIMER_PORT port, uint32_t val)
{
	timer_wr_reg(port, TIMER_BGLOAD_OFST, val);
}

/*****************************************************************************/
//  Function:  void timer_start(E_TIMER_PORT port)
//
//  Parameters:
//		port		: timer port
//
//  Return:
//		none
//		
//  Description:
//      timer start.
//
/*****************************************************************************/
void timer_start(E_TIMER_PORT port)
{
	uint32_t reg_val = timer_rd_reg(port, TIMER_CONTROL_OFST);
	
	// set enable control
	reg_val &= ~TIMER_EN_MASK;
	reg_val |= TIMER_EN_VAL(TIMER_EN);

	timer_wr_reg(port, TIMER_CONTROL_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void timer_stop(E_TIMER_PORT port)
//
//  Parameters:
//		port		: timer port
//
//  Return:
//		none
//		
//  Description:
//      timer stop.
//
/*****************************************************************************/
void timer_stop(E_TIMER_PORT port)
{
	uint32_t reg_val = timer_rd_reg(port, TIMER_CONTROL_OFST);
	
	// set enable control
	reg_val &= ~TIMER_EN_MASK;
	reg_val |= TIMER_EN_VAL(TIMER_DIS);

	timer_wr_reg(port, TIMER_CONTROL_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void timer_init(E_TIMER_PORT port, uint32_t base, uint32_t pclk)
//
//  Parameters:
//		port		: timer port
//
//		base		: timer base address
//
//		pclk		: timer input clock
//
//  Return:
//		none
//
//  Description:
//       Initialize the base and input clock of timer.
//
/*****************************************************************************/
void timer_init(E_TIMER_PORT port, uint32_t base, uint32_t pclk)
{
	mem_set8(&g_timer[port], 0, sizeof(struct brite_timer));
	
	g_timer[port].base = base;
	g_timer[port].pclk = pclk;	
	g_timer[port].port = port;

	timer_set_ctrl(port, TIMER_WRAPPING_MODE, TIMER_SIZE_BITS32, TIMER_DIV_16,
						 TIMER_INT_DIS, TIMER_FREE_MODE, TIMER_DIS);	
}

