/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_VAD_REG_H__
#define __CDL_VAD_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// register layout
#define VAD_LEFT_MARGIN_OFST				0x000		// 5 bits, RW, left margin 
#define VAD_RIGHT_MARGIN_OFST				0x004		// 5 bits, RW, right margin
#define VAD_N_LOW_CONTINUE_FRAMES_OFST		0x008		// 5 bits, RW, n low continue frames 
#define VAD_N_LOW_SEEK_FRAMES_OFST			0x00C		// 5 bits, RW, n low seek frames
#define VAD_N_HIGH_CONTINUE_FRAMES_OFST		0x010		// 5 bits, RW, n high continue frames	
#define VAD_N_HIGH_SEEK_FRAMES_OFST			0x014		// 5 bits, RW, n high seek frames
#define VAD_N_SPEECH_LOW_HIGH_FRAMES_OFST	0x018		// 5 bits, RW, n speech low high frames
#define VAD_N_SPEECH_LOW_SEEK_FRAMES_OFST	0x01C		// 5 bits, RW, n speech low seek frames
#define VAD_MEAN_SIL_FRAMES_OFST			0x020		// 5 bits, RW, mean sil frames
#define VAD_N_ALPHA_OFST					0x024		// 12 bits, RW, nalpha
#define VAD_N_BETA_OFST						0x028		// 12 bits, RW, nbeta
#define VAD_FIFO_DEPTH_OFST					0x02C		// 6 bits, RW, fifo_depth
#define VAD_LR_SEL_OFST						0x040		// 1 bit, RW, lr_sel
#define VAD_SW_OFST							0x044		// 2 bits, RW, alias name
#define VAD_LEFT_WD_OFST					0x048		// 1 bit, RW, left wd
#define VAD_RIGHT_WD_OFST					0x04C		// 1 bit, RW, right wd
#define VAD_STOP_DELAY_OFST					0x050		// 2 bit, RW, stop delay
#define VAD_ADDR_START_OFST					0x054		// 13 bits, RW, addr start, vad memory start address,align with 64bit
#define VAD_ADDR_WRAP_OFST					0x058		// 14 bits, RW, addr wrap, vad memory highest address for Push, align with 64bit, (addr_wrap-1) is the max physical address 
#define VAD_MEM_SW_OFST						0x05C		// 1 bit, RW, vad mem sw
#define VAD_SPINT_CLR_OFST					0x060		// 1 bit, RW, vad spint clr
#define VAD_SPINT_EN_OFST					0x064		// 1 bit, RW, vad spint en
#define VAD_SLINT_CLR_OFST					0x068		// 1 bit, RW, vad slint clr
#define VAD_SLINT_EN_OFST					0x06C		// 1 bit, RW, vad slint en
#define VAD_RAW_SPINT_OFST					0x070		// 1 bit, RW, raw vad spint
#define VAD_RAW_SLINT_OFST					0x074		// 1 bit, RW, raw vad slint
#define VAD_SPINT_OFST						0x078		// 1 bit, RW, vad spint 
#define VAD_SLINT_OFST						0x07C		// 1 bit, RW, vad slint
#define VAD_XMEM_ADDR_OFST					0x080		// 13 bit, RW, xmem addr

// offset: 0x40, lr_sel
typedef enum
{
	LR_SEL_LEFT		= 0,
	LR_SEL_RIGHT	= 1
}E_VAD_LR_SEL;

// offset: 0x44, vad_sw register
typedef enum
{
	VAD_ENA_SHIFT		= 0,	// [0], RW
								
	ADC_ENA_SHIFT		= 1,	// [1], RW
										
	VAD_SW_REVD_SHIFT	= 2		// [31:2], Reserved
}E_VAD_SW_SHIFT;

#define VAD_ENA_MASK	(1<<VAD_ENA_SHIFT)
#define ADC_ENA_MASK	(1<<ADC_ENA_SHIFT)

// offset: 0x44, vad_sw
typedef enum
{
	VAD_SW_MODE_NONE	= 0,
	VAD_SW_MODE_VAD		= 1, 
	VAD_SW_MODE_ADC		= 2,
	VAD_SW_MODE_BOTH	= 3
}E_VAD_SW_MODE;

// offset: 0x48, left_wd
// offset: 0x4c, right_wd
typedef enum
{
	VAD_SEL_L16BITS	= 0,
	VAD_SEL_H16BITS	= 1
}E_VAD_HL16BIT_SEL;

// offset: 0x50, stop_dly
typedef enum
{
	STOP_DLY_0_SAMPLE	= 0,
	STOP_DLY_1_SAMPLE	= 1,
	STOP_DLY_2_SAMPLE	= 2,
	STOP_DLY_3_SAMPLE	= 3
}E_VAD_STOP_DLY;

// offset: 0x5C, vad_mem_sw
typedef enum
{
	SWITCH_XMEM_TO_AXI	= 0,
	SWITCH_XMEM_TO_VAD	= 1
}E_VAD_MEM_SW_MODE;

// offset: 0x64, vad_spint_en
// offset: 0x6c, vad_slint_en
typedef enum
{
	VAD_INT_DIS	= 0,
	VAD_INT_EN	= 1
}E_VAD_INT_EN;

#endif

