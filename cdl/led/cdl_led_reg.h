/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_LED_REG_H__
#define __CDL_LED_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define LED_SLAVE_ADDR	(0x46>>1)

// register layout
#define LED_RSTCTR_OFST				0x00			// 1 bit, W, reset register 
#define LED_RGB_OE_OFST				0x01			// 6 bits, W, LED enable register
#define LED_FIXBRIT_LED_0_7_OFST(port)	(0x02 + port)
#define LED_FIXBRIT_LED_8_15_OFST(port)	(0x0C + (port - 8))


// offset: 0x00, reset fegister 
typedef enum
{
	LED_POWERON		= 0,
	LED_SHUTDOWN	= 1
}E_LED_RST_MODE;

// offset: 0x01, led enable register 
typedef enum
{
	LED_RGB0_OE_SHIFT 		= 0,	// [0], Write, RGB0 enable(LED0->LED2)
									// 0 = close
									// 1 = open

	LED_RGB1_OE_SHIFT 		= 1,	// [0], Write, RGB0 enable(LED3->LED5)
									// 0 = close
									// 1 = open

	LED_RGB67_OE_SHIFT 		= 2,	// [0], Write, RGB0 enable(LED6->LED7)
									// 0 = close
									// 1 = open

	LED_RGB89_OE_SHIFT 		= 3,	// [0], Write, RGB0 enable(LED8->LED9)
									// 0 = close
									// 1 = open

	LED_RGB2_OE_SHIFT 		= 4,	// [0], Write, RGB0 enable(LED10->LED12)
									// 0 = close
									// 1 = open

	LED_RGB3_OE_SHIFT 		= 5,	// [0], Write, RGB0 enable(LED13->LED15)
									// 0 = close
									// 1 = open
									
	LED_RGB9_OE_SHIFT		= 6,	// [7:6], reserved
}E_LED_OE_SHIFT;

#define LED_RGB_OE_MASK	0x3F

typedef enum
{
	LED_RGB_0_2_OE		=	(1<<LED_RGB0_OE_SHIFT),
	LED_RGB_3_5_OE 		=	(1<<LED_RGB1_OE_SHIFT),
	LED_RGB_6_7_OE 		=	(1<<LED_RGB67_OE_SHIFT),
	LED_RGB_8_9_OE 		=	(1<<LED_RGB89_OE_SHIFT),
	LED_RGB_10_12_OE 	=	(1<<LED_RGB2_OE_SHIFT),
	LED_RGB_13_15_OE 	= 	(1<<LED_RGB3_OE_SHIFT),
	LED_RGB_0_15_OE		= 	0x3F
}E_LED_OE;

// offset: 0x02, fix brightness Register
#define LED_FIX_BRIT_MASK	0x7F

// 0x00: ²»ÁÁ
// 0x7F: ×îÁÁ
#define LED_BRIT_VAL(v)	(v&LED_FIX_BRIT_MASK)

#endif

