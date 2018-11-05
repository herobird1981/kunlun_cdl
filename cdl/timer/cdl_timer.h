/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_TIMER_H__
#define __CDL_TIMER_H__
#include "test_config.h"
#include "cdl_timer_reg.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
struct brite_timer{
	uint32_t base;
	uint32_t pclk;
	uint32_t freq;
	int port;
};

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern struct brite_timer g_timer[];

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void timer_int_handler(void *ptr);

void timer_wr_reg(E_TIMER_PORT port, uint32_t reg_ofst, uint32_t val);

uint32_t timer_rd_reg(E_TIMER_PORT port, uint32_t reg_ofst);

BOOL timer_verify_periph_id(E_TIMER_PORT port);

BOOL timer_verify_pcell_id(E_TIMER_PORT port);

void timer_set_load(E_TIMER_PORT port, uint32_t val);

uint32_t timer_get_value(E_TIMER_PORT port);

void timer_set_ctrl(E_TIMER_PORT port, E_TIMER_SHORTCNT short_cnt, E_TIMER_SIZE size, E_TIMER_DIV div,
						 E_TIMER_INT_EN int_en, E_TIMER_MODE mode, E_TIMER_EN timer_en);

void timer_enable_ctrl(E_TIMER_PORT port, E_TIMER_EN timer_en);

void timer_clr_int(E_TIMER_PORT port);

uint32_t timer_get_ris(E_TIMER_PORT port);

uint32_t timer_get_mis(E_TIMER_PORT port);

void timer_set_bgload(E_TIMER_PORT port, uint32_t val);

void timer_start(E_TIMER_PORT port);

void timer_stop(E_TIMER_PORT port);

void timer_init(E_TIMER_PORT port, uint32_t base, uint32_t pclk);

#endif

