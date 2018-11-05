/*----------------------------------------------------------------------------------------------------------*/
/*                                                                                               											 		 */
/*           Copyright (C) 2016 Brite Semiconductor Co., Ltd. All rights reserved.                									 */
/*                                                                                                													 */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __TEST_MEM_H__
#define __TEST_MEM_H__
#include "test_config.h"

#if (CONFIG_MEM_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern cmdline_entry mem_test_menu[];

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
int mem_test_prepare(int port);
int mem_test_cleanup(int port);

#else
#define mem_test_menu 		NULL
#define mem_test_prepare 	NULL
#define mem_test_cleanup 	NULL
#endif

#endif

