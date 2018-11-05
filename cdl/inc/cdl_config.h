/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_CONFIG_H__
#define __CDL_CONFIG_H__
#include <stdlib.h>
#include "cdl_types.h"
#include "memop.h"
#include "cdl_errno.h"
#include "cdl_irq.h"
#include "memleak.h"
#include "hw_maps.h"
#include "rand.h"
#include "cdl_dma.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define DEBUG_OUTPUT

#ifdef 	DEBUG_OUTPUT
#define debug(...)	console_printf(__VA_ARGS__)
#else
#define debug(...)
#endif

#define info(...)	console_printf(__VA_ARGS__)

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
// data transfer by cpu/dma mode
typedef enum
{
	DT_CPU = 0,
	DT_DMA = 1
}E_DT_MODE;

// polling/interrupt mode
typedef enum
{
	PI_POLLING	= 0,
	PI_INT		= 1
}E_PI_MODE;


/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/

extern void* memset(void*, int, unsigned int);

#endif

