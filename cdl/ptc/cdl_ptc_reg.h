/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_PTC_REG_H__
#define __CDL_PTC_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define PTC_PORT_SIZE	0x10

// register layout
#define PTC_CNTR_OFST			0x000 	// R/W, 32 bit, main ptc counter register
#define PTC_HRC_OFST			0x004 	// R/W, 32 bit, PTC HI Reference/Capture register
#define PTC_LRC_OFST			0x008 	// R/W, 32 bit, PTC LO Reference/Capture register
#define PTC_CTRL_OFST			0x00C 	// R/W, 9 bit, Control register

// offset: 0x000, Control register 0
typedef enum
{
	PTC_EN_SHIFT 			= 0,	// [0], R/W, EN 
									// When set, RPTC_CNTR can be incremented. 

	SPI_ECLK_SHIFT 			= 1,	// [1], Read/Write, ECLK
									// When set, ptc_ecgt signal is used to increment RPTC_CNTR. 
									// When cleared, system clock is used instead. 
										
	SPI_NEC_SHIFT 			= 2,	// [2], Read/Write, NEC 
									// When set, ptc_ecgt increments on negative edge and gates on low period. 
									// When cleared, ptc_ecgt increments on positive edge and 
									// gates on high period. 
									// This bit has effect only on ¡®gating¡¯ function of ptc_ecgt 
									// when RPTC_CTRL[ECLK] bit is cleared. 
	
	SPI_OE_SHIFT 			= 3,	// [3], Read/Write, OE
									// Inverted value of this bit is reflected on the ptc_oen signal. 
									// It is used to enable PWM output driver. 
										
	SPI_SINGLE_SHIFT		= 4,	// [4], Read/Write, When set, RPTC_CNTR is not incremented anymore after 
									// it reaches value equal to the RPTC_LRC value. 
									// When cleared, RPTC_CNTR is restarted after it reaches 
									// value in the RPTC_LCR register. 

	SPI_INTE_SHIFT 			= 5,	// [5], Read/Write, When set, PTC asserts an interrupt when RPTC_CNTR 
									// value is equal to the value of RPTC_LRC or RPTC_HRC.  
									// When cleared, interrupts are maske

	SPI_INT_SHIFT 			= 6,	// [6], Read/Write,	INT
									// When read, this bit represents pending interrupt. When it is 
									// set, an interrupt is pending. 
									// When this bit is written with ¡®1¡¯, interrupt request is cleared. 

	SPI_CNTRRST_SHIFT 		= 7,	// [7], Read/Write,	CNTRRST
									// When set, RPTC_CNTR is under reset. 
									// When cleared, normal operation of the counter is allowed.

	SPI_CAPTE_SHIFT 		= 8,	// [8], Read/Write,	CAPTE
									// When set, ptc_capt signal can be used to capture 
									// RPTC_CNTR into RPTC_LRC or RPTC_HRC registers. 
									// Into which reference/capture register capture occurs 
									// depends on edge of the ptc_capt signal. 
									// When cleared, capture function is masked. 	
}E_PTC_CTRL_SHIFT;

#define PTC_EN_MASK			(1<<PTC_EN_SHIFT)
#define SPI_ECLK_MASK		(1<<SPI_ECLK_SHIFT)
#define SPI_NEC_MASK		(1<<SPI_NEC_SHIFT)
#define SPI_OE_MASK			(1<<SPI_OE_SHIFT)
#define SPI_SINGLE_MASK		(1<<SPI_SINGLE_SHIFT)
#define SPI_INTE_MASK		(1<<SPI_INTE_SHIFT)
#define SPI_INT_MASK		(1<<SPI_INT_SHIFT)
#define SPI_CNTRRST_MASK	(1<<SPI_CNTRRST_SHIFT)
#define SPI_CAPTE_MASK		(1<<SPI_CAPTE_SHIFT)

#endif

