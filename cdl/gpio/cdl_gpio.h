/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_GPIO_H__
#define __CDL_GPIO_H__
#include "cdl_config.h"
#include "cdl_gpio_reg.h"
#include "cdl_sys.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// interrupt type
typedef enum
{
	GPIO_INT_EDGE_FALLING	= 0,
	GPIO_INT_LEVEL_LOW		= 1,	
	GPIO_INT_EDGE_RISING	= 2,
	GPIO_INT_LEVEL_HIGH		= 3,
	GPIO_INT_EDGE_BOTH		= 4
}E_GPIO_IT;

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
struct brite_gpio {
	uint32_t base;
	uint32_t pclk;
	int port;
};

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern struct brite_gpio g_gpio[];

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void gpio_int_handler(void *ptr);

void gpio_wr_reg(E_GPIO_PORT port, uint32_t reg_ofst, uint32_t val);

uint32_t gpio_rd_reg(E_GPIO_PORT port, uint32_t reg_ofst);

BOOL gpio_verify_periph_id(E_GPIO_PORT port);

BOOL gpio_verify_pcell_id(E_GPIO_PORT port);

void gpio_set_output(E_GPIO_PORT port, E_PIN_PORT pin, E_PIN_LEVEL level);

E_PIN_LEVEL gpio_get_output(E_GPIO_PORT port, E_PIN_PORT pin);

void gpio_set_dir(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_DIR dir);

void gpio_set_ctrl(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_CTRL ctrl);

E_PIN_LEVEL gpio_get_input(E_GPIO_PORT port, E_PIN_PORT pin);

void gpio_set_int_type(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IS is, E_GPIO_IT it);

void gpio_set_ie(E_GPIO_PORT port, E_PIN_PORT pin, E_GPIO_IE ie);

uint32_t gpio_get_ris(E_GPIO_PORT port, E_PIN_PORT pin);

uint32_t gpio_get_mis(E_GPIO_PORT port, E_PIN_PORT pin);

void gpio_set_ic(E_GPIO_PORT port, E_PIN_PORT pin);

void gpio_init(E_GPIO_PORT port, uint32_t base);

#endif

