/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __TEST_CONFIG_H__
#define __TEST_CONFIG_H__
#include "cdl_config.h"
#include "hw_maps.h"
#include "hw_interrupt.h"
#include "cdl_dma.h"
#include "sys_timer.h"
#include "interrupt.h"
#include "cdl_sys.h"
#include "cdl_wdog.h"
#include "cdl_pll.h"
#include "cdl_uart.h"
#include "cmdline.h"
#include "console.h"
#include "cdl_vad.h"
#include "test_utils.h" 

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// console uart select
#define CONSOLE_BASE			UART0_BASE

// include test device list
#define CONFIG_ADC_TEST			1
#define CONFIG_AVCODEC_TEST		1
#define CONFIG_DAC_TEST			1
#define CONFIG_DMAC_TEST		1
#define CONFIG_GPIO_TEST		1
#define CONFIG_I2C_TEST			1
#define CONFIG_I2S_TEST			1
#define CONFIG_PLL_TEST			1
#define CONFIG_QSPI_TEST		1	
#define CONFIG_SPI_TEST			1
#define CONFIG_SYS_TEST			1
#define CONFIG_TIMER_TEST		1
#define CONFIG_UART_TEST		1
#define CONFIG_USB_TEST			1
#define CONFIG_VAD_TEST			1
#define CONFIG_WDOG_TEST		1
#define CONFIG_DMA_TEST			1
#define CONFIG_USB_TEST			1
#define CONFIG_MEM_TEST			1
#define CONFIG_PTC_TEST			1

#define DHRYSTONE_TEST			0

#define UART_BOOT_TEST			0
#define SPI_BOOT_TEST			1
#define I2C_BOOT_TEST			0

#define CONFIG_LED_TEST			1
#define CONFIG_AC108_TEST		1


/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/

#endif

