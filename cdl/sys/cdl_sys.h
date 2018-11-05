/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_SYS_H__
#define __CDL_SYS_H__
#include "cdl_types.h"
#include "pad_share_ctrl_macro.h"
#include "rst_gen_ctrl_macro.h"
#include "sys_con_macro.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// DSP core Number
typedef enum
{
	DSP_CORE0           = 0,
	DSP_CORE1           = 1,
	NUM_DSP_CORE_PORTS  = 2
}E_DSP_CORE_PORT;

// UART Port Number
typedef enum
{
	UART0           = 0,
	UART1           = 1,
	NUM_UART_PORTS  = 2
}E_UART_PORT;

// I2C Port Number
typedef enum
{
	I2C0           = 0, // slave
	I2C1           = 1,	// master
	NUM_I2C_PORTS  = 2
}E_I2C_PORT;

// I2S Port Number
typedef enum
{
	I2S_ADC			= 0,
	I2S_DAC			= 1,
	I2S_1			= 2,
	I2S_VAD			= 3,
	NUM_I2S_PORTS	= 4
}E_I2S_PORT;

// UART Port Number
typedef enum
{
	USB0           = 0,
	USB1           = 1,
	NUM_USB_PORTS  = 2
}E_USB_PORT;

// SPI Port Number
typedef enum
{
	SPI0           = 0,
	NUM_SPI_PORTS  = 1
}E_SPI_PORT;

// Timer Port Number
typedef enum
{
	TIMER0           = 0,
	TIMER1           = 1,
	NUM_TIMER_PORTS  = 2
}E_TIMER_PORT;

// GPIO Port Number
typedef enum
{
	GPIO0           = 0,	
	NUM_GPIO_PORTS  = 1
}E_GPIO_PORT;

// GPIO Pin Number
typedef enum
{
	PIN_0           = 0,
	PIN_1           = 1,
	PIN_2           = 2,
	PIN_3           = 3,
	PIN_4           = 4,
	PIN_5           = 5,
	PIN_6           = 6,
	PIN_7           = 7,
	NUM_PIN_PORTS  	= 8
}E_PIN_PORT;

// Software reset
typedef enum
{
	SYS_A_RST		= 0,
	SYS_H_RST		= 1,
	SYS_P_RST		= 2,
	SYS_AON_RST		= 3,
	DSP0_B_RST		= 4,
	DSP0_D_RST		= 5,
	DSP1_B_RST		= 6,
	DSP1_D_RST		= 7,
	DMAC_RST		= 8,
	GPIO_RST		= 9,
	I2CS_RST		= 10,
	I2CM_RST		= 11,
	SPI_RST			= 12,
	TIMER_RST		= 13,
	UART0_RST		= 14,
	UART1_RST		= 15,
	USB_RST			= 16,
	I2SADC_RST		= 17,
	I2SDAC_RST		= 18,
	I2S1_RST		= 19,
	I2SVAD_RST		= 20,
	ACDCUCP_RST		= 21,
	WDT_RST			= 22,
	NRST_RST		= 23,
	PTC_RST			= 24,
	NUM_RST_PORTS	= 25
}E_SW_RST_PORT;

// pinmux function
typedef enum
{
	PINMUX_FUNC0		= 0,
	PINMUX_FUNC1		= 1,
	PINMUX_FUNC2		= 2,
	PINMUX_FUNC3		= 3,
	PINMUX_FUNC4		= 4,
	PINMUX_FUNC5		= 5,
	PINMUX_FUNC6		= 6,
	PINMUX_FUNC7		= 7,	
	NUM_PINMUX_FUNCS	= 8
}E_PINMUX_FUNC;

// pin pull down enable/disable
typedef enum
{
	PIN_PD_DIS		= 0,
	PIN_PD_EN		= 1
}E_PIN_REN;

// I2S_ADC mode
typedef enum
{
	I2S_ADC_DATA_FROM_CODEC	= 0,
	I2S_ADC_DATA_FROM_PAD	= 1,
}E_I2S_ADC_DATA_MODE;

// I2S_DAC data mode
typedef enum
{
	I2S_DAC_DATA_TO_CODEC	= 0,
	I2S_DAC_DATA_TO_PAD		= 1,
}E_I2S_DAC_DATA_MODE;

// I2S_DAC mode
typedef enum
{
	I2S_DAC_SRCCLK_PLL	= 0,
	I2S_DAC_SRCCLK_PAD	= 1,
}E_I2S_DAC_SRCCLK_MODE;

// I2S_mode
typedef enum
{
	I2S_MASTER	= 0,
	I2S_SLAVE	= 1,
}E_I2S_MS_MODE;


/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void sys_peripheral_cache_disable(void);

//software reset 
void sys_sw_rst(E_SW_RST_PORT port);

// pin mux
void pad_tck_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_tms_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_tdo_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_tdi_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_trst_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_spiclk_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_spicsn_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_spirxd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_spitxd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_uart0rxd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_uart0txd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_i2csscl_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_i2cssda_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_gpio0_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_gpio1_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_pwm0_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_pwm1_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_pwm2_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_dmic_clk11_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_dmic_clk12_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_dmic_in1_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_dmic_clk21_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_dmic_clk22_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_dmic_in2_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_i2smic_sbclk_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_i2smic_slr0_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_i2smic_ssdata_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_dopwmlp_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_dopwmln_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_dopwmrp_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_dopwmrn_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

void pad_i2s2_mclk_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren);

// clock configure
void i2s_adc_clk_congig(uint8_t div, E_I2S_ADC_DATA_MODE mode);

void i2s1_master_clk_config(uint8_t div, uint8_t type, uint8_t sample_format);

void i2s1_slave_clk_config(uint8_t div, uint8_t type, uint8_t sample_format);

void i2s_dac_clk_config(uint8_t div, E_I2S_MS_MODE mode, E_I2S_DAC_DATA_MODE dac_mode);

void usb_clk_config(void);

// syscon configuration
uint32_t boot_strap(void);

void sys_dsp0_config(void);

void sys_dsp1_config(void);

void sys_external_pad_output(uint8_t pin, uint8_t v);

#endif

