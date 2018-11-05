/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_WDOG_REG_H__
#define __CDL_WDOG_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// register layout
#define WDOG_LOAD_OFST		0x000 	// Read/Write, 32 bit, Watchdog load register
#define WDOG_VALUE_OFST		0x004	// Read,         32 bit, The current value for the watchdog counter  
#define WDOG_CONTROL_OFST	0x008	// Read/Write,  2 bit,  Watchdog control register
#define WDOG_INTCLR_OFST	0x00C	// Write Only,  -----,  Clears the watchdog interrupt
#define WDOG_RIS_OFST		0x010	// Read Only,    1 bit,  Watchdog raw interrupt status
#define WDOG_MIS_OFST		0x014	// Read Only,    1 bit, Watchdog masked interrupt status
#define WDOG_LOCK_OFST		0xC00	// Read/Write, 32 bit, Watchdog lock register

// Integration
#define WDOG_ITCR_OFST		0xF00	// Read/Write,   1 bit, ntegration test control register
#define WDOG_ITOP_OFST		0xF04	// Write Only,    2 bit, Integration test output set register

// Peripheral0/1/2/3
#define WDOG_PERIPHID0_OFST	0xFE0	// Read Only, 8 bit, Peripheral ID register bits 7:0
#define WDOG_PERIPHID1_OFST	0xFE4	// Read Only, 8 bit, Peripheral ID register bits 15:8
#define WDOG_PERIPHID2_OFST	0xFE8	// Read Only, 8 bit, Peripheral ID register bits 23:16
#define WDOG_PERIPHID3_OFST	0xFEC	// Read Only, 8 bit, Peripheral ID register bits 31:24

// PCellID0/1/2/3
#define WDOG_PCELLID0_OFST	0xFF0	// Read Only, 8 bit, PrimeCell ID register bits 7:0
#define WDOG_PCELLID1_OFST	0xFF4	// Read Only, 8 bit, PrimeCell ID register bits 15:8
#define WDOG_PCELLID2_OFST	0xFF8	// Read Only, 8 bit, PrimeCell ID register bits 23:16
#define WDOG_PCELLID3_OFST	0xFFC	// Read Only, 8 bit, PrimeCell ID register bits 31:24

// offset: 0x008, Watchdog control register
typedef enum
{
	WDOG_INTEN_SHIFT  			= 0,	// [0], Read/write,   Enable the interrupt event (WDOGINT). Set HIGH to enable the counter and the interrupt, and 
										// set LOW to disable the counter and interrupt. Reloads the counter from the value in WdogLoad 
										// when the interrupt is enabled, and was previously disabled.

	WDOG_RESEN_SHIFT 			= 1,	// [1], Read/write,   Enable Watchdog reset output (WDOGRES). 
										// Acts as a mask for the reset output. Set HIGH to enable the reset, and LOW to disable the reset.
										
	WDOG_CTRL_REVD_SHIFT		= 2		// [31:2], RO, Reserved, read undefined, must read as zeros.
}E_WDOG_CONTROL_SHIFT;

//[0],  INTEN
#define WDT_INTEN_MASK		(1<<WDOG_INTEN_SHIFT)
#define WDT_INTEN_VAL(val)	((val)<<WDOG_INTEN_SHIFT)

typedef enum
{
	WDOG_INTEN_DIS	= 0,
	WDOG_INTEN_EN	= 1
}E_WDOG_INTEN;

// [1], RESEN mask and value
#define WDOG_RESEN_MASK		(1<<WDOG_RESEN_SHIFT)
#define WDOG_RESEN_VAL(val)	((val)<<WDOG_RESEN_SHIFT)

typedef enum
{
	WDOG_RES_DIS	= 0,
	WDOG_RES_EN		= 1
}E_WDOG_RESEN;

// offset: 0x010, Watchdog raw interrupt status
// offset: 0x014, Watchdog masked interrupt status
typedef enum
{
	WDOG_INT_ST_SHIFT 		= 0,	// [0], Read, Enabled interrupt status from the counter
										
	WDOG_INT_ST_REVD_SHIFT	= 1		// [31:1], -, Reserved, read undefined, must read as zeros.
}E_WDOG_RAW_INT_SHIFT;

// INT mask
#define WDOG_INT_ST_MASK	(1<<WDOG_INT_ST_SHIFT)

// offset: 0xC00, Watchdog lock register	
typedef enum
{
	WDOG_WR_EN_ST_SHIFT  	= 0,	// [0], Read, 0 = write access to all other registers is enabled (default)
									//               1 = write access to all other registers is disabled
										
	WDOG_WR_EN_SHIFT		= 0		// [31:0], Write, Enable write access to all other registers by writing 0x1ACCE551. 
									// Disable write access by writing any other value.
}E_WDOG_LOCK_SHIFT;

// write enable status mask and value
#define WDOG_WR_EN_ST_MASK		(1<<WDOG_WR_EN_ST_SHIFT)
#define WDOG_WR_EN_ST_VAL(val)	((val)<<WDOG_WR_EN_ST_SHIFT)

// write enable/disable
typedef enum
{
	WDOG_WR_DIS	= 0x12345678,
	WDOG_WR_EN	= 0x1ACCE551
}E_WDOG_LOCK;

// 0xFE0 to 0xFEC, Peripheral identification registers, WdogPeriphID0-3
#define WDOG_PERIPHID_MASK	0xFF

#define WDOG_PERIPHID0 		0x05
#define WDOG_PERIPHID1 		0x18
#define WDOG_PERIPHID2 		0x04
#define WDOG_PERIPHID3 		0x00

// 0xFF0 to 0xFFC, PrimeCell identification registers, WdogPCellID0-3
#define WDOG_PCELLID_MASK	0xFF

#define WDOG_PCELLID0		0x0D
#define WDOG_PCELLID1		0xF0
#define WDOG_PCELLID2		0x05
#define WDOG_PCELLID3		0xB1

#endif

