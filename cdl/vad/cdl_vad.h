/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_VAD_H__
#define __CDL_VAD_H__
#include "test_config.h"
#include "cdl_vad_reg.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
struct brite_vad {
	uint32_t base;	// IP base address
};


/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern struct brite_vad g_vad;

extern volatile int8_t g_spint_cnt;
extern volatile int8_t g_slint_cnt;

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void vad_wr_reg(uint32_t reg_ofst, uint32_t val);

uint32_t vad_rd_reg(uint32_t reg_ofst);

void vad_sw_ctrl(E_VAD_SW_MODE mode);

void vad_set_stop_dly(E_VAD_STOP_DLY dly);

void vad_set_start_wrap_addr(uint32_t st_addr, uint32_t size);

void vad_switch_xmem(E_VAD_MEM_SW_MODE mode);

void vad_normal_to_idle_init(E_VAD_LR_SEL lr_sel, E_VAD_STOP_DLY stop_dly);

void vad_spint_handle(void *ptr); 

void vad_slint_handle(void *ptr); 

void vad_init(void);


#endif

