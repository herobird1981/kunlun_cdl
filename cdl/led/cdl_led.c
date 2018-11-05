/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_i2c.h"
#include "cdl_led.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//	Function:  void led_init(void)
//
//	Parameters:
//		none
//
//	Return:
//		none
//
//	Description:
//		Initialization i2c master.
//
/*****************************************************************************/
void led_init(void)
{
	E_I2C_PORT port = I2C1;
	struct dw_i2c *priv = p_i2c_dev[port];
	
	i2c_test_prepare(port);

	// Set i2c to master mode
	i2c_init_master(priv, 100000, I2C_7BIT_ADDR);	
}

/*****************************************************************************/
//	Function:  int led_reset(E_LED_RST_MODE mode)
//
//	Parameters:
//		mode	: reset mode
//
//	Return:
//		0		: ok
//		other	: fail
//
//	Description:
//		led reset..
//
/*****************************************************************************/
int led_reset(E_LED_RST_MODE mode)
{
	int ret_val;
	E_I2C_PORT port = I2C1;
	struct dw_i2c *priv = p_i2c_dev[port];
	uint8_t buf[1] = {mode};
	
	ret_val = i2c_write(priv, LED_SLAVE_ADDR, LED_RSTCTR_OFST, 1, buf, 1);

	return ret_val;
}

/*****************************************************************************/
//	Function:  int led_rgb_oe(E_LED_OE led_oe)
//
//	Parameters:
//		led_oe	: led output enable bits
//
//	Return:
//		0		: ok
//		other	: fail
//
//	Description:
//		led output enable.
//
/*****************************************************************************/
int led_rgb_oe(E_LED_OE led_oe)
{
	int ret_val;
	E_I2C_PORT port = I2C1;
	struct dw_i2c *priv = p_i2c_dev[port];
	uint8_t buf[1] = {led_oe};
	
	ret_val = i2c_write(priv, LED_SLAVE_ADDR, LED_RGB_OE_OFST, 1, buf, 1);

	return ret_val;
}

/*****************************************************************************/
//	Function:  int led_fix_brightness(E_LED_PORT port, uint8_t bright_val)
//
//	Parameters:
//		port		: led port
//
//		bright_val: brightness
//					= 0x00, the min  light
//					= 0x7F, the max light
//	Return:
//		0		: ok
//		other	: fail
//
//	Description:
//		set the led brightness.
//
/*****************************************************************************/
int led_fix_brightness(E_LED_PORT port, uint8_t bright_val)
{
	int ret_val;
	struct dw_i2c *priv = p_i2c_dev[I2C1];
	uint8_t buf[1] = {bright_val};
	uint8_t reg_addr = (port < LED_8) ? LED_FIXBRIT_LED_0_7_OFST(port) : LED_FIXBRIT_LED_8_15_OFST(port);
	
	ret_val = i2c_write(priv, LED_SLAVE_ADDR, reg_addr, 1, buf, 1);

	return ret_val;
}

