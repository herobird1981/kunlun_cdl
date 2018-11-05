/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_GPIO_REG_H__
#define __CDL_GPIO_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define GPIO_PORT_SIZE 0x1000

// register layout
#define GPIO_DATA_OFST			0x3FC 	// Read/Write, 8 bit, PrimeCell GPIO data register
#define GPIO_DIR_OFST			0x400	// Read/Write, 8 bit, PrimeCell GPIO data direction register
#define GPIO_IS_OFST			0x404	// Read/Write,  8 bit, PrimeCell GPIO interrupt sense register
#define GPIO_IBE_OFST			0x408	// Read/Write,  8 bit, PrimeCell GPIO interrupt both edges register
#define GPIO_IEV_OFST			0x40C	// Read/Write,  8 bit, PrimeCell GPIO interrupt event register
#define GPIO_IE_OFST			0x410	// Read/Write,  8 bit, PrimeCell GPIO interrupt mask register
#define GPIO_RIS_OFST			0x414	// Read,          8 bit, PrimeCell GPIO raw interrupt status register
#define GPIO_MIS_OFST			0x418	// Read,          8 bit, PrimeCell GPIO masked interrupt status register
#define GPIO_IC_OFST			0x41C	// Write,          8 bit, PrimeCell GPIO interrupt clear register
#define GPIO_AFSEL_OFST			0x420	// Read/Write,  8 bit, PrimeCell GPIO mode control select register

// Peripheral0/1/2/3
#define GPIO_PERIPHID0_OFST		0xFE0	// Read,         8 bit, Peripheral ID register bits 7:0
#define GPIO_PERIPHID1_OFST		0xFE4	// Read,         8 bit, Peripheral ID register bits 15:8
#define GPIO_PERIPHID2_OFST		0xFE8	// Read,         8 bit, Peripheral ID register bits 23:16
#define GPIO_PERIPHID3_OFST		0xFEC	// Read,         8 bit, Peripheral ID register bits 31:24

// PrimeCell0/1/2/3
#define GPIO_PCELLID0_OFST		0xFF0	// Read,         8 bit, PrimeCell ID register bits 7:0
#define GPIO_PCELLID1_OFST		0xFF4	// Read,         8 bit, PrimeCell ID register bits 15:8
#define GPIO_PCELLID2_OFST		0xFF8	// Read,         8 bit, PrimeCell ID register bits 23:16
#define GPIO_PCELLID3_OFST		0xFFC	// Read,         8 bit, PrimeCell ID register bits 31:24

// offset: 0x000, rimeCell GPIO data register
typedef enum
{
	LEVEL_LOW	= 0,
	LEVEL_HIGH	= 1	 
}E_PIN_LEVEL;

// offset: 0x400, PrimeCell GPIO data direction register
typedef enum
{
	GPIO_INPUT		= 0, // Cleaning a bit configures the pin to be input 
	GPIO_OUTPUT		= 1	 // Bits set to HIGH in the GPIODIR configure corresponding pins to be an output 
}E_GPIO_DIR;

// offset: 0x404, PrimeCell GPIO interrupt sense register
typedef enum
{
	GPIO_IS_EDGE	= 0, // Cleaning a bit configures the pin to detect edges 
	GPIO_IS_LEVEL	= 1	 // Bits set to HIGH in the GPIOIS configure corresponding pins to detect level 
}E_GPIO_IS;

// offset: 0x408, PrimeCell GPIO interrupt both edges register
typedef enum
{
	GPIO_IBE_EDGE_SINGLE	= 0, // interrupt generation event is controlled by GPIOIEV 
	GPIO_IBE_EDGE_BOTH		= 1	 // both edge on corresponding pin trigger an interrupt
}E_GPIO_IBE;

// offset: 0x40C, PrimeCell GPIO interrupt event register
typedef enum
{
	GPIO_IEV_EDGE_FALLING	= 0,
	GPIO_IEV_LEVEL_LOW		= 0,	
	GPIO_IEV_EDGE_RISING	= 1,
	GPIO_IEV_LEVEL_HIGH		= 1
}E_GPIO_IEV;

// offset: 0x410, PrimeCell GPIO interrupt mask register
typedef enum
{
	GPIO_IE_DIS	= 0,
	GPIO_IE_EN	= 1
}E_GPIO_IE;

// offset: 0x420, PrimeCell GPIO mode control select register
typedef enum
{
	GPIO_CTRL_SW	= 0,	// enable software control mode
	GPIO_CTRL_HW	= 1		// enable hardware control mode
} E_GPIO_CTRL;

// 0xFE0 to 0xFEC, Peripheral identification registers, GPIOPeriphID0-3
#define GPIO_PERIPHID_MASK	0xFF

#define GPIO_PERIPHID0 		0x61
#define GPIO_PERIPHID1 		0x10
#define GPIO_PERIPHID2 		0x04
#define GPIO_PERIPHID3 		0x00

// 0xFF0 to 0xFFC, PrimeCell identification registers, GPIOPCellID0-3
#define GPIO_PCELLID_MASK	0xFF

#define GPIO_PCELLID0		0x0D
#define GPIO_PCELLID1		0xF0
#define GPIO_PCELLID2		0x05
#define GPIO_PCELLID3		0xB1

#endif

