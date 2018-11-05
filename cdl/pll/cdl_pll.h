/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_PLL_H__
#define __CDL_PLL_H__
#include "clk_gen_ctrl_macro.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
#if CONFIG_ASIC_VERSION
// TBD
#else
// FPGA clock configuration
#define DSP_PLL1_CLK	50000000	
#define DSP_PLL2_CLK	29500000	
#define DSP_PLL3_CLK	1000000	
#define DSP_PLL4_CLK	1000000	
#define DSP_PLL5_CLK	48000000	// usb used only
#endif

// AXI
// DSP0/1
// internal DMA
// sram
// rom

// AHB

// APB0
#define I2S_ADC_PCLK	DSP_PLL3_CLK
#define I2S_DAC_PCLK	DSP_PLL3_CLK
#define I2S1_PCLK		DSP_PLL3_CLK // mclk sue
#define I2S1_VAD_PCLK	DSP_PLL3_CLK
#define SPI_PCLK		DSP_PLL1_CLK
#define I2C0_PCLK		DSP_PLL1_CLK
#define I2C1_PCLK		DSP_PLL1_CLK
#define ADDC_UCP_PCLK	DSP_PLL1_CLK	// TBD

// APB1
#define UART0_PCLK		DSP_PLL2_CLK
#define UART1_PCLK		DSP_PLL2_CLK
#define PTC_PCLK		DSP_PLL1_CLK
#define WDT_PCLK		DSP_PLL1_CLK
#define TIMER_PCLK		DSP_PLL1_CLK

#endif

