/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_wdog.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct brite_wdog g_wdog;
struct brite_wdog *p_wdog = &g_wdog;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/
static void wdog_set_lock(E_WDOG_LOCK lock);

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//  Function:  wdog_int_handler(void *ptr)
//
//  Parameters:
//		ptr [in]	: pointer
//
//  Return:
//		none
//		
//  Description:
//     Wdog interrupt hadler.
//
/*****************************************************************************/
#if 0
void wdog_int_handler(void *ptr)
{
	
}
#endif

/*****************************************************************************/
//  Function:  void wdog_wr_reg(uint32_t reg_ofst, uint32_t val)
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
void wdog_wr_reg(uint32_t reg_ofst, uint32_t val)
{
	__raw_writel(val, p_wdog->base + reg_ofst);
}

/*****************************************************************************/
//  Function:  uint32_t wdog_rd_reg(uint32_t reg_ofst)
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
uint32_t wdog_rd_reg(uint32_t reg_ofst)
{
	return __raw_readl(p_wdog->base + reg_ofst);
}

/*****************************************************************************/
//  Function:  BOOL wdog_verify_periph_id(void)
//
//  Parameters:
//		none
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
BOOL wdog_verify_periph_id(void)
{
	uint8_t i, id[4];

	// read the peripheral id from the register
	for (i = 0; i < 4; i++)
	{
		id[i] = wdog_rd_reg(WDOG_PERIPHID0_OFST + (i<<2));
		id[i] &= WDOG_PERIPHID_MASK;
	}

	// verify the validity of the peripheral id
	if ((id[0] == WDOG_PERIPHID0) && 
		(id[1] == WDOG_PERIPHID1) && 
		(id[2] == WDOG_PERIPHID2) && 
		(id[3] == WDOG_PERIPHID3)) 
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************/
//  Function:  BOOL wdog_verify_pcell_id(void)
//
//  Parameters:
//		none
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
BOOL wdog_verify_pcell_id(void)
{
	uint8_t i, id[4];

	// read the primecell id from the register
	for (i = 0; i < 4; i++)
	{
		id[i] = wdog_rd_reg(WDOG_PCELLID0_OFST + (i<<2));
		id[i] &= WDOG_PCELLID_MASK;
	}

	// verify the validity of the primecell id
	if ((id[0] == WDOG_PCELLID0) && 
		(id[1] == WDOG_PCELLID1) && 
		(id[2] == WDOG_PCELLID2) && 
		(id[3] == WDOG_PCELLID3)) 
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************/
//  Function:  void wdog_set_load(uint32_t val)
//
//  Parameters:
//		val		: load value
//
//  Return:
//		none
//
//  Description:
//       Set wdog load value.
//
/*****************************************************************************/
void wdog_set_load(uint32_t val)
{
	wdog_wr_reg(WDOG_LOAD_OFST, val);
}

/*****************************************************************************/
//  Function:  uint32_t wdog_get_value(void)
//
//  Parameters:
//		none
//
//  Return:
//		The current value for the watchdog counter 
//		
//  Description:
//      Get the current counter of WDT .
//
/*****************************************************************************/
uint32_t wdog_get_value(void)
{
	return wdog_rd_reg(WDOG_VALUE_OFST);
}


/*****************************************************************************/
//  Function:  void wdog_set_ctrl(E_WDOG_INTEN inten, E_WDOG_RESEN resen)
//
//  Parameters:
//		inten		: wdog interrupt enable
//
//		resen		: wdog reset output enable
//
//  Return:
//		none
//
//  Description:
//       Set wdog control mode.
//
/*****************************************************************************/
void wdog_set_ctrl(E_WDOG_INTEN inten, E_WDOG_RESEN resen)
{
	uint32_t reg_val = wdog_rd_reg(WDOG_CONTROL_OFST);

	// set inten
	reg_val &= ~WDT_INTEN_MASK;
	reg_val |= WDT_INTEN_VAL(inten);
	
	// set resen
	reg_val &= ~WDOG_RESEN_MASK;
	reg_val |= WDOG_RESEN_VAL(resen);

	wdog_wr_reg(WDOG_CONTROL_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void wdog_clr_int(void)
//
//  Parameters:
//		none
//
//  Return:
//		none
//
//  Description:
//       Clear the wdog interrupt and reload the counter from WdogLoad .
//
/*****************************************************************************/
void wdog_clr_int(void)
{
	// A write of any value to this location clears the watchdog interrupt, and reloads the 
	// counter from the value in WdogLoad.
	wdog_wr_reg(WDOG_INTCLR_OFST, 0x12345678);
}

/*****************************************************************************/
//  Function:  uint32_t wdog_get_ris(void)
//
//  Parameters:
//		none
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
uint32_t wdog_get_ris(void)
{
	uint32_t reg_val = wdog_rd_reg(WDOG_RIS_OFST);
	
	reg_val &= WDOG_INT_ST_MASK;
	
	return reg_val;
}

/*****************************************************************************/
//  Function:  uint32_t wdog_get_mis(void)
//
//  Parameters:
//		none
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
uint32_t wdog_get_mis(void)
{
	uint32_t reg_val = wdog_rd_reg(WDOG_MIS_OFST);
	
	reg_val &= WDOG_INT_ST_MASK;
	
	return reg_val;
}

/*****************************************************************************/
//  Function:  void wdog_set_lock(E_WDOG_LOCK lock)
//
//  Parameters:
//		lock	: lock control
//
//  Return:
//		none
//
//  Description:
//       wdog lock control.
//
/*****************************************************************************/
static void wdog_set_lock(E_WDOG_LOCK lock)
{
	uint32_t reg_val;

	// set the lock control
	wdog_wr_reg(WDOG_LOCK_OFST, lock);

	// check the lock status
	while (1)
	{
		reg_val = wdog_rd_reg(WDOG_LOCK_OFST);
		if (lock == WDOG_WR_EN)
		{
			if ((reg_val & WDOG_WR_EN_ST_MASK) != WDOG_WR_EN_ST_MASK) 
			{
				return;
			}
		}
		else
		{
			if ((reg_val & WDOG_WR_EN_ST_MASK) == WDOG_WR_EN_ST_MASK) 
			{
				return;
			}
		}
	}
}

/*****************************************************************************/
//  Function:  void wdog_start(void)
//
//  Parameters:
//		none
//
//  Return:
//		none
//		
//  Description:
//      WDOG start.
//
/*****************************************************************************/
void wdog_start(void)
{
	wdog_wr_reg(WDOG_LOAD_OFST, p_wdog->load_val);
	
	wdog_clr_int();
	wdog_set_ctrl(WDOG_INTEN_EN, WDOG_RES_EN);
}

/*****************************************************************************/
//  Function:  void wdog_stop(void)
//
//  Parameters:
//		none
//
//  Return:
//		none
//		
//  Description:
//      WDOG stop.
//
/*****************************************************************************/
void wdog_stop(void)
{
	wdog_set_ctrl(WDOG_INTEN_DIS, WDOG_RES_DIS);
	wdog_clr_int();
}

/*****************************************************************************/
//  Function:  void wdog_restart(void)
//
//  Parameters:
//		none
//
//  Return:
//		none
//		
//  Description:
//      WDOG restart.
//
/*****************************************************************************/
void wdog_restart(void)
{
	//info("cur_val: 0x%x\n", wdog_get_value());
	
	// Reloads the counter from the value in WdogLoad 
	wdog_wr_reg(WDOG_LOAD_OFST, p_wdog->load_val);
	
	// when the interrupt is enabled, and was previously disabled.
	wdog_stop();
	wdog_start();

	//info("load_val: 0x%x, cur_val: 0x%x\n", reg_val, wdog_get_value());
}

/*****************************************************************************/
//  Function:  void wdog_init(uint32_t base, uint32_t pclk, uint32_t load_val)
//
//  Parameters:
//		base		: wdog base address
//
//		pclk		: wdog input clock
//
//		load_val	: wdog load value(ms)
//
//  Return:
//		none
//
//  Description:
//       Initialize the base and input clock of wdog
//
/*****************************************************************************/
void wdog_init(uint32_t base, uint32_t pclk, uint32_t load_val)
{
	mem_set8(p_wdog, 0, sizeof(struct brite_wdog));
	
	p_wdog->base = base;
	p_wdog->pclk = pclk;
	p_wdog->freq = pclk;
	p_wdog->load_val = ((p_wdog->pclk / 1000) * load_val ) / 2;
	
	wdog_set_lock(WDOG_WR_EN);
	wdog_wr_reg(WDOG_LOAD_OFST, p_wdog->load_val);
	wdog_set_ctrl(WDOG_INTEN_DIS, WDOG_RES_DIS);
}

