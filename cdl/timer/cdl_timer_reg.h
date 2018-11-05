/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_TIMER_REG_H__
#define __CDL_TIMER_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define TIMER_PORT_SIZE	0x20

// register layout
#define TIMER_LOAD_OFST			0x000 	// Read/Write, 32 bit, Load value for Timer1
#define TIMER_VALUE_OFST		0x004	// Read,          32 bit, The current value for Timer1
#define TIMER_CONTROL_OFST		0x008	// Read/Write,  8 bit,  Timer 1 control register
#define TIMER_INTCLR_OFST		0x00C	// Write Only,  -----,  Timer 1 interrupt clear
#define TIMER_RIS_OFST			0x010	// Read Only,    1 bit,  Timer 1 raw interrupt status
#define TIMER_MIS_OFST			0x014	// Read Only,    1 bit, Timer 1 masked interrupt status
#define TIMER_BGLOAD_OFST		0x018	// Read/Write, 32 bit, Background load value for Timer 1

// Integration
#define TIMER_ITCR_OFST			0xF00	// Read/Write, 1 bit, Integration test control register
#define TIMER_ITOP_OFST			0xF04	// Write,         2 bit, Integration test output set register

// Peripheral0/1/2/3
#define TIMER_PERIPHID0_OFST	0xFE0	// Read,         8 bit, Peripheral ID register bits 7:0
#define TIMER_PERIPHID1_OFST	0xFE4	// Read,         8 bit, Peripheral ID register bits 15:8
#define TIMER_PERIPHID2_OFST	0xFE8	// Read,         8 bit, Peripheral ID register bits 23:16
#define TIMER_PERIPHID3_OFST	0xFEC	// Read,         8 bit, Peripheral ID register bits 31:24

// PrimeCell0/1/2/3
#define TIMER_PCELLID0_OFST		0xFF0	// Read,         8 bit, PrimeCell ID register bits 7:0
#define TIMER_PCELLID1_OFST		0xFF4	// Read,         8 bit, PrimeCell ID register bits 15:8
#define TIMER_PCELLID2_OFST		0xFF8	// Read,         8 bit, PrimeCell ID register bits 23:16
#define TIMER_PCELLID3_OFST		0xFFC	// Read,         8 bit, PrimeCell ID register bits 31:24

// offset: 0x008, Timer 1 control register
typedef enum
{
	TIMER_ONE_SHORT_COUNT_SHIFT = 0,	// [0], Read/write, Selects one-shot or wrapping counter mode:
										// 0 = wrapping mode (default)
										// 1 = one-shot mode

	TIMER_SIZE_SHIFT 			= 1,	// [1], Read/write, Selects 16/32 bit counter operation:
										// 0 = 16-bit counter (default)
										// 1 = 32-bit counter

	TIMER_PRE_SHIFT 			= 2,	// [3:2], Read/write, Prescale bits:
										// 00 = 0 stages of prescale, clock is divided by 1 (default)
										// 01 = 4 stages of prescale, clock is divided by 16
										// 10 = 8 stages of prescale, clock is divided by 256
										// 11 = Undefined, do not use.

	TIMER_REVD0_SHIFT			= 4,	// [4], Reserved bit, do not modify, and ignore on read
	
	TIMER_INT_EN_SHIFT 			= 5,	// [5], Read/write, Interrupt Enable bit:
										// 0 = Timer Interrupt disabled
										// 1 = Timer Interrupt enabled (default)	

	TIMER_MODE_SHIFT 			= 6,	// [6], Read/write, Mode bit:
										// 0 = Timer is in free-running mode (default)
										// 1 = Timer is in periodic mode.	

	TIMER_EN_SHIFT 				= 7,	// [7], Read/write, Enable bit:
										// 0 = Timer disabled (default)
										// 1 = Timer enabled.
										
	TIMER_REVD1_SHIFT			= 8		// [31:8], Reserved, read undefined, must read as zeros
}E_TIMER_CONTROL_SHIFT;

// [0], timer one shot count mask and value
#define TIMER_ONE_SHORT_COUNT_MASK		(1<<TIMER_ONE_SHORT_COUNT_SHIFT)
#define TIMER_ONE_SHORT_COUNT_VAL(val)	((val)<<TIMER_ONE_SHORT_COUNT_SHIFT)

typedef enum
{
	TIMER_WRAPPING_MODE = 0, // wrapping mode, default
	TIMER_ONE_SHOT_MODE = 1  // one-shot mode
}E_TIMER_SHORTCNT;

// [1], Selects 16/32 bit counter operation mask and value
#define TIMER_SIZE_MASK		(1<<TIMER_SIZE_SHIFT)
#define TIMER_SIZE_VAL(val)	((val)<<TIMER_SIZE_SHIFT)

typedef enum
{
	TIMER_SIZE_BITS16 = 0, // 0 = 16-bit counter (default)
	TIMER_SIZE_BITS32 = 1  // 1 = 32-bit counter
}E_TIMER_SIZE;

// [3:2], timer prescale bits mask and value
#define TIMER_PRE_DIV_MASK		(3<<TIMER_PRE_SHIFT)
#define TIMER_PRE_DIV_VAL(val)	((val)<<TIMER_PRE_SHIFT)

typedef enum
{
	TIMER_DIV_1 	= 0,	// 00 = 0 stages of prescale, clock is divided by 1 (default)
	TIMER_DIV_16 	= 1, 	// 01 = 4 stages of prescale, clock is divided by 16
	TIMER_DIV_256 	= 2  	// 10 = 8 stages of prescale, clock is divided by 256
}E_TIMER_DIV;

// [5], timer interrupt enable mask and value
#define TIMER_INT_EN_MASK		(1<<TIMER_INT_EN_SHIFT)
#define TIMER_INT_EN_VAL(val)	((val)<<TIMER_INT_EN_SHIFT)

typedef enum
{
	TIMER_INT_DIS = 0, 	// Timer Interrupt disabled
	TIMER_INT_EN  = 1  	// Timer Interrupt enabled, default.
}E_TIMER_INT_EN;

// [6], timer mode mask and value
#define TIMER_MODE_MASK		(1<<TIMER_MODE_SHIFT)
#define TIMER_MODE_VAL(val)	((val)<<TIMER_MODE_SHIFT)

typedef enum
{
	TIMER_FREE_MODE 	= 0, // Timer is in free-running mode, default
	TIMER_PERIODIC_MODE	= 1  // Timer is in periodic mode
}E_TIMER_MODE;

// [7], timer enable bit mask and value
#define TIMER_EN_MASK		(1<<TIMER_EN_SHIFT)
#define TIMER_EN_VAL(val)	((val)<<TIMER_EN_SHIFT)

typedef enum
{
	TIMER_DIS = 0, 		// Timer disabled (default)
	TIMER_EN  = 1  		// Timer enabled
}E_TIMER_EN;

// offset: 0x010, Timer 1 raw interrupt status
typedef enum
{
	TIMER_RAW_INT_ST_SHIFT 			= 0,	// [0], Read, Raw interrupt status from the counter
										
	TIMER_RAW_INT_ST_REVD_SHIFT		= 1		// [31:1], Reserved, Reserved, read undefined, must read as zeros
}E_TIMER_RAW_INT_ST_SHIFT;

// timer raw int status mask
#define TIMER_RAW_INT_ST_MASK		(1<<TIMER_RAW_INT_ST_SHIFT)

// offset: 0x014, Timer 1 masked interrupt status
typedef enum
{
	TIMER_INT_ST_SHIFT 			= 0,	// [0], Read, Enabled interrupt status from the counter
										
	TIMER_INT_ST_REVD_SHIFT		= 1		// [31:1], Reserved, read undefined, must read as zeros
}E_TIMER_INT_ST_SHIFT;

// timer masked int status mask
#define TIMER_INT_ST_MASK		(1<<TIMER_INT_ST_SHIFT)

// 0xFE0 to 0xFEC, Peripheral identification registers, TimerPeriphID0-3
#define TIMER_PERIPHID_MASK	0xFF

#define TIMER_PERIPHID0 	0x04
#define TIMER_PERIPHID1 	0x18
#define TIMER_PERIPHID2 	0x04
#define TIMER_PERIPHID3 	0x00

//  0xFF0 to 0xFFC, PrimeCell identification registers, TimerPCellID0-3
#define TIMER_PCELLID_MASK	0xFF

#define TIMER_PCELLID0		0x0D
#define TIMER_PCELLID1		0xF0
#define TIMER_PCELLID2		0x05
#define TIMER_PCELLID3		0xB1

#endif

