/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __TEST_GPIO_H__
#define __TEST_GPIO_H__
#include "test_config.h"
#include "cdl_gpio.h"

#if (CONFIG_GPIO_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern cmdline_entry gpio_test_menu[];

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
int gpio_test_prepare(int sel);

#else
#define gpio_test_menu NULL
#define gpio_test_prepare NULL
#endif

#endif
