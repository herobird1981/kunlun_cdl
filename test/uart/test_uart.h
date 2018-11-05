/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __TEST_UART_H__
#define __TEST_UART_H__
#include "test_config.h"

#if (CONFIG_UART_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern cmdline_entry uart_test_menu[];

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
int uart_test_prepare(int port);

#else
#define uart_test_menu 			NULL
#define uart_test_prepare 		NULL
#endif

#endif
