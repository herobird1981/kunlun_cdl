/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __HW_INTERRUPT_H__
#define __HW_INTERRUPT_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define INT_DMAC            XCHAL_EXTINT0_NUM
#define INT_GPIO            XCHAL_EXTINT1_NUM
#define INT_I2CS            XCHAL_EXTINT2_NUM
#define INT_I2CM            XCHAL_EXTINT3_NUM
#define INT_SPI             XCHAL_EXTINT4_NUM
#define INT_TIMER           XCHAL_EXTINT5_NUM
#define INT_UART0           XCHAL_EXTINT6_NUM
#define INT_UART1           XCHAL_EXTINT7_NUM
#define INT_USB             XCHAL_EXTINT8_NUM
#define INT_USB_DMA         INT_USB
#define INT_WDOG            XCHAL_EXTINT9_NUM
#define INT_I2S_VAD_DA      XCHAL_EXTINT10_NUM
#define INT_I2S_VAD_OR      XCHAL_EXTINT11_NUM
#define INT_I2S_VAD_SP      XCHAL_EXTINT12_NUM
#define INT_I2S_VAD_SL      XCHAL_EXTINT13_NUM
#define INT_I2S_ADC_DA_0    XCHAL_EXTINT14_NUM
#define INT_I2S_ADC_DA_1    XCHAL_EXTINT15_NUM
#define INT_I2S_ADC_DA_2    XCHAL_EXTINT16_NUM
#define INT_I2S_ADC_OR_0    XCHAL_EXTINT17_NUM
#define INT_I2S_ADC_OR_1    XCHAL_EXTINT18_NUM
#define INT_I2S_ADC_OR_2    XCHAL_EXTINT19_NUM
#define INT_I2S_DAC_EMP_0   XCHAL_EXTINT20_NUM
#define INT_I2S_DAC_OR_0    XCHAL_EXTINT21_NUM
#define INT_I2S1_EMP_0      XCHAL_EXTINT22_NUM
#define INT_I2S1_OR_0       XCHAL_EXTINT23_NUM
#define INT_CODEC_IRQ       XCHAL_EXTINT24_NUM
#define INT_CODEC_IRQ_WT    XCHAL_EXTINT25_NUM

#define INT_MAX		25
#endif

