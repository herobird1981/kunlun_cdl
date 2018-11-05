/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_i2c.h"
#include "cdl_ac108.h"

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
//	Function:  void ac108_init(void)
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
void ac108_init(void)
{
	E_I2C_PORT port = I2C1;
	struct dw_i2c *priv = p_i2c_dev[port];
	
	i2c_test_prepare(port);

	// Set i2c to master mode
	i2c_init_master(priv, 100000, I2C_7BIT_ADDR);	

	// ac108 hw reset
	sys_external_pad_output(0, 0);
	udelay(10000);
	sys_external_pad_output(0, 1);
	udelay(10000);
	
	// ac108 cp_en
	sys_external_pad_output(1, 1);
}

/*****************************************************************************/
//	Function:  static int ac108_write(E_AC108_PORT port, uint8_t reg, uint8_t val)
//
//	Parameters:
//		port		: ac108 port
//
//		reg		: the register of ac108
//
//		data		: the data to write to the register
//
//	Return:
//		0		: ok
//		other	: fail
//
//	Description:
//		write the value to the register of a port.
//
/*****************************************************************************/
static int ac108_write(E_AC108_PORT port, uint8_t reg, uint8_t data)
{
	int ret_val;
	struct dw_i2c *priv = p_i2c_dev[I2C1];
	uint32_t dev = (port == AC108_0) ? AC108_0_SLAVE_ADDR : AC108_1_SLAVE_ADDR;
	
	ret_val = i2c_write(priv, dev, reg, 1, &data, 1);

	return ret_val;
}

/*****************************************************************************/
//	Function:  static int ac108_read(E_AC108_PORT port, uint8_t reg, uint8_t *p_data)
//
//	Parameters:
//		port		: ac108 port
//
//		reg		: the register of ac108
//
//		p_data	: the value to read from the register
//
//	Return:
//		0		: ok
//		other	: fail
//
//	Description:
//		read the value from the register of a port.
//
/*****************************************************************************/
static int ac108_read(E_AC108_PORT port, uint8_t reg, uint8_t *p_data)
{
	int ret_val;
	struct dw_i2c *priv = p_i2c_dev[I2C1];
	uint32_t dev = (port == AC108_0) ? AC108_0_SLAVE_ADDR : AC108_1_SLAVE_ADDR;
	
	ret_val = i2c_read(priv, dev, reg, 1, p_data, 1);

	return ret_val;
}


int ac108_hw_init(E_AC108_PORT port)
{
	uint8_t data;
	
	ac108_read(port, CHIP_AUDIO_RST, &data);
	info("data: 0x%x\n", data);
		
	// chip soft reset
	ac108_write(port, CHIP_AUDIO_RST, 0x12);	// 0x00=0x12: reset all registers to their default state	

	ac108_read(port, CHIP_AUDIO_RST, &data);
	info("data: 0x%x\n", data);	

	return 0;
}




