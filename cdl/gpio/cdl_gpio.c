/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_gpio.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct brite_gpio g_gpio[NUM_GPIO_PORTS];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/
static void gpio_set_is(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IS is);

static void gpio_set_ibe(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IBE ibe);

static void gpio_set_iev(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IEV iev);

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//  Function:  gpio_int_handler(void *ptr)
//
//  Parameters:
//		ptr [in]	: pointer
//
//  Return:
//		none
//		
//  Description:
//     GPIO interrupt hadler.
//
/*****************************************************************************/
#if 0
void gpio_int_handler(void *ptr)
{
	
}
#endif

/*****************************************************************************/
//  Function:  void gpio_wr_reg(E_GPIO_PORT port, uint32_t reg_ofst, uint32_t val)
//
//  Parameters:
//		port			: gpio port
//
//		reg_ofst		: the offset of a register
//
//		val			: the value to write 
//
//  Return:
//		none
//		
//  Description:
//      Write the value to a register.
//
/*****************************************************************************/
void gpio_wr_reg(E_GPIO_PORT port, uint32_t reg_ofst, uint32_t val)
{
	__raw_writel(val, (g_gpio[port].base + reg_ofst));
}

/*****************************************************************************/
//  Function:  uint32_t gpio_rd_reg(E_GPIO_PORT port, uint32_t reg_ofst)
//
//  Parameters:
//		port			: gpio port
//
//		reg_ofst		: the offset of a register
//
//  Return:
//		The value from a register
//		
//  Description:
//      Read the value from a register.
//
/*****************************************************************************/
uint32_t gpio_rd_reg(E_GPIO_PORT port, uint32_t reg_ofst)
{
	return __raw_readl(g_gpio[port].base + reg_ofst);
}

/*****************************************************************************/
//  Function:  BOOL gpio_verify_periph_id(E_GPIO_PORT port)
//
//  Parameters:
//		port		: gpio port
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
BOOL gpio_verify_periph_id(E_GPIO_PORT port)
{
	uint8_t i, id[4];

	// read the peripheral id from the register
	for (i = 0; i < 4; i++)
	{
		id[i] = gpio_rd_reg(port, (GPIO_PERIPHID0_OFST + (i<<2)));
		id[i] &= GPIO_PERIPHID_MASK;
	}

	// verify the validity of the peripheral id
	if ((id[0] == GPIO_PERIPHID0) && 
		(id[1] == GPIO_PERIPHID1) && 
		(id[2] == GPIO_PERIPHID2) && 
		(id[3] == GPIO_PERIPHID3)) 
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************/
//  Function:  BOOL gpio_verify_pcell_id(E_GPIO_PORT port)
//
//  Parameters:
//		port		: gpio port
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
BOOL gpio_verify_pcell_id(E_GPIO_PORT port)
{
	uint8_t i, id[4];

	// read the primecell id from the register
	for (i = 0; i < 4; i++)
	{
		id[i] = gpio_rd_reg(port, (GPIO_PCELLID0_OFST + (i<<2)));
		id[i] &= GPIO_PCELLID_MASK;
	}

	// verify the validity of the primecell id
	if ((id[0] == GPIO_PCELLID0) && 
		(id[1] == GPIO_PCELLID1) && 
		(id[2] == GPIO_PCELLID2) && 
		(id[3] == GPIO_PCELLID3)) 
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************/
//  Function:  void gpio_set_output(E_GPIO_PORT port, E_PIN_PORT pin, E_PIN_LEVEL level)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//		level		: output level(high/low) on a pin
//
//  Return:
//		none
//
//  Description:
//      Set pin level(high/low).
//
/*****************************************************************************/
void gpio_set_output(E_GPIO_PORT port, E_PIN_PORT pin, E_PIN_LEVEL level)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_DATA_OFST);

	reg_val &= ~(1 << pin);
	
	if (level) 
	{
		reg_val |= (1 << pin);
	}
	
	gpio_wr_reg(port, GPIO_DATA_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  E_PIN_LEVEL gpio_get_output(E_PIN_PORT pin)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//  Return:
//		pin level(high/low)
//
//  Description:
//       Get pin level(high/low).
//
/*****************************************************************************/
E_PIN_LEVEL gpio_get_output(E_GPIO_PORT port, E_PIN_PORT pin)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_DATA_OFST);
	
	reg_val = (reg_val >> pin) & 0x1;
	
	return reg_val;
}

/*****************************************************************************/
//  Function:  void gpio_set_dir(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_DIR dir)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//		dir		: pin direction
//
//  Return:
//		none
//
//  Description:
//       Set pin directionl(input/output).
//
/*****************************************************************************/
void gpio_set_dir(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_DIR dir)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_DIR_OFST);

	reg_val &= ~(1 << pin);
	
	if (dir) 
	{
		reg_val |= (1 << pin);
	}
	
	gpio_wr_reg(port, GPIO_DIR_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void gpio_set_ctrl(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_CTRL ctrl)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//		ctrl		: gpio control mode
//
//  Return:
//		none
//
//  Description:
//       Set gpio control mode(hw/sw).
//
/*****************************************************************************/
void gpio_set_ctrl(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_CTRL ctrl)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_AFSEL_OFST);

	reg_val &= ~(1 << pin);
	
	if (ctrl) 
	{
		reg_val |= (1 << pin);
	}
	
	gpio_wr_reg(port, GPIO_AFSEL_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  E_PIN_LEVEL gpio_get_input(E_GPIO_PORT port, E_PIN_PORT pin)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//  Return:
//		none
//
//  Description:
//       Get gpio pin level(low/high).
//
/*****************************************************************************/
E_PIN_LEVEL gpio_get_input(E_GPIO_PORT port, E_PIN_PORT pin)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_DATA_OFST);
	
	reg_val = (reg_val >> pin) & 0x1;
	
	return reg_val;
}

/*****************************************************************************/
//  Function:  static void gpio_set_is(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IS is)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//		is		: interrupt sense(edge/level)
//
//  Return:
//		none
//
//  Description:
//      Set pin interrutp sense.
//
/*****************************************************************************/
static void gpio_set_is(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IS is)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_IS_OFST);

	reg_val &= ~(1 << pin);
	
	if (is) 
	{
		reg_val |= (1 << pin);
	}
	
	gpio_wr_reg(port, GPIO_IS_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  static void gpio_set_ibe(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IBE ibe)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//		ibe		: interrupt both edges
//
//  Return:
//		none
//
//  Description:
//      Set pin interrutp both edge.
//
/*****************************************************************************/
static void gpio_set_ibe(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IBE ibe)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_IBE_OFST);

	reg_val &= ~(1 << pin);
	
	if (ibe) 
	{
		reg_val |= (1 << pin);
	}
	
	gpio_wr_reg(port, GPIO_IBE_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  static void gpio_set_iev(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IEV iev)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//		iev		: interrupt event(high/low/rising edge/falling edge)
//
//  Return:
//		none
//
//  Description:
//      Set pin interrutp event.
//
/*****************************************************************************/
static void gpio_set_iev(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IEV iev)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_IEV_OFST);

	reg_val &= ~(1 << pin);
	
	if (iev) 
	{
		reg_val |= (1 << pin);
	}
	
	gpio_wr_reg(port, GPIO_IEV_OFST, reg_val);
}

/*****************************************************************************/
//	Function:  void gpio_set_int_type(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IS is, E_GPIO_IT it)
//
//	Parameters:
//		port	: gpio port
//
//		pin 	: gpio pin
//
//		is	: interrupt sense(edge/level)
//
//		it	: interrupt type(high/low/rising edge/falling edge/both edge)
//
//	Return:
//		none
//
//	Description:
//		Set pin interrupt type.
//
/*****************************************************************************/
void gpio_set_int_type(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IS is, E_GPIO_IT it)
{
	E_GPIO_IEV iev;

	// set interrupt sense
	gpio_set_is(port, pin, is);

	if (it == GPIO_INT_EDGE_BOTH)
	{
		// set interrupt both edge enable
		gpio_set_ibe(port, pin, GPIO_IBE_EDGE_BOTH);		
	}
	else
	{
		// set interrupt both edge disable
		gpio_set_ibe(port, pin, GPIO_IBE_EDGE_SINGLE);

		// set interrupt event
		if      (it == GPIO_INT_EDGE_FALLING)	iev = GPIO_IEV_EDGE_FALLING;
		else if (it == GPIO_INT_LEVEL_LOW)		iev = GPIO_IEV_LEVEL_LOW;
		else if (it == GPIO_INT_EDGE_RISING)	iev = GPIO_IEV_EDGE_RISING;
		else 									iev = GPIO_IEV_LEVEL_HIGH;
		gpio_set_iev(port, pin, iev);
	}
}

/*****************************************************************************/
//  Function:  void gpio_set_ie(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IE ie)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//		ie		: interrupt enable/disable
//
//  Return:
//		none
//
//  Description:
//      Set pin interrutp enable/disable.
//
/*****************************************************************************/
void gpio_set_ie(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IE ie)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_IE_OFST);

	reg_val &= ~(1 << pin);
	
	if (ie) 
	{
		reg_val |= (1 << pin);
	}
	
	gpio_wr_reg(port, GPIO_IE_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  uint32_t gpio_get_ris(E_GPIO_PORT port, E_PIN_PORT pin)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//  Return:
//		the pin raw interrupt status
// 			= 0, no masked interrupt
//			= 1, masked interrupt
//
//  Description:
//       Get the raw interrupt status of a gpio pin.
//
/*****************************************************************************/
uint32_t gpio_get_ris(E_GPIO_PORT port, E_PIN_PORT pin)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_RIS_OFST);
	
	reg_val = (reg_val >> pin) & 0x1;
	
	return reg_val;
}

/*****************************************************************************/
//  Function:  uint32_t gpio_get_mis(E_GPIO_PORT port, E_PIN_PORT pin)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//  Return:
//		the pin masked interrupt status
// 			= 0, no raw interrupt
//			= 1, raw interrupt
//
//  Description:
//       Get the masked interrupt status of a gpio pin.
//
/*****************************************************************************/
uint32_t gpio_get_mis(E_GPIO_PORT port, E_PIN_PORT pin)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_MIS_OFST);
	
	reg_val = (reg_val >> pin) & 0x1;
	
	return reg_val;
}

/*****************************************************************************/
//  Function:  void gpio_set_ic(E_GPIO_PORT port, E_PIN_PORT pin)
//
//  Parameters:
//		port		: gpio port
//
//		pin		: gpio pin
//
//  Return:
//		none
//
//  Description:
//       Clear the corresponding edge detection interrupt status.
//	    only effect for edge-sensitive interrupt
//
/*****************************************************************************/
void gpio_set_ic(E_GPIO_PORT port, E_PIN_PORT pin)
{
	uint32_t reg_val = gpio_rd_reg(port, GPIO_IC_OFST);

	reg_val &= ~(1 << pin);
	reg_val |= (1 << pin);
	
	gpio_wr_reg(port, GPIO_IC_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  void gpio_init(E_GPIO_PORT port, uint32_t base)
//
//  Parameters:
//		port		: gpio port
//
//		base		: gpio base address
//
//  Return:
//		none
//
//  Description:
//       Initialize the base and input clock of gpio
//
/*****************************************************************************/
void gpio_init(E_GPIO_PORT port, uint32_t base)
{
	mem_set8(&g_gpio[port], 0, sizeof(struct brite_gpio));

	g_gpio[port].base = base;
	g_gpio[port].port = port;
}

