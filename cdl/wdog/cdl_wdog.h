/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_WDOG_H__
#define __CDL_WDOG_H__
#include "cdl_config.h"
#include "cdl_wdog_reg.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define WDOG_TIME_OUT	0xFFFFFFFF

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
struct brite_wdog {
	uint32_t base;
	uint32_t pclk;
	uint32_t load_val;

	uint32_t freq;
};

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern struct brite_wdog *p_wdog;

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
//void wdog_int_handler(void *ptr);

void wdog_wr_reg(uint32_t reg_ofst, uint32_t val);

uint32_t wdog_rd_reg(uint32_t reg_ofst);

BOOL wdog_verify_periph_id(void);

BOOL wdog_verify_pcell_id(void);

void wdog_set_load(uint32_t val);

uint32_t wdog_get_value(void);

void wdog_set_ctrl(E_WDOG_INTEN inten, E_WDOG_RESEN resen);

void wdog_clr_int(void);

uint32_t wdog_get_ris(void);

uint32_t wdog_get_mis(void);

void wdog_start(void);

void wdog_stop(void);

void wdog_restart(void);

void wdog_init(uint32_t base, uint32_t pclk, uint32_t load_val);

#endif

