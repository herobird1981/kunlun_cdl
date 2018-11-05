/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__
#include "test_config.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define TEST_BUFFER_SIZE	(1024 * 1200)
#define TEST_BUF_SIZE		TEST_BUFFER_SIZE
#define TEST_BUFFER_MARGIN	512

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern uint8_t test_txrx_buff[];
extern uint8_t *p_test_tx_buf;
extern uint8_t *p_test_rx_buf;

extern uint8_t *test_send_buff;
extern uint8_t *test_recv_buff;


/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void mem_set_random(void *p_dst, uint32_t size);

int mem_check_mask(uint8_t *p_dst, uint8_t *p_src, uint32_t len, uint32_t dw);

void print_in_line(uint8_t *p_name, uint8_t *p_buf, uint32_t len) ;

void print_hex(const char* what, const unsigned long group, const void *p1, const unsigned long len);

void print_in_line_u16(uint8_t *p_name, uint16_t *p_buf, uint32_t len); 

void print_in_line_u32(uint8_t *p_name, uint32_t *p_buf, uint32_t len); 

void print_in_line_reg32(uint8_t *p_name, uint32_t *p_buf, uint32_t len); 

int i2s_mem_check_mask(uint8_t *p_dst, uint8_t *p_src, uint32_t len, uint32_t dw);


#endif

