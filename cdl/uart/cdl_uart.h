/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_UART_H__
#define __CDL_UART_H__
#include "test_config.h"
#include "cdl_uart_reg.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// baud rate setting
typedef enum
{
	UART_BD_110		= 110,
	UART_BD_300		= 300,
	UART_BD_4800	= 4800,
	UART_BD_7200	= 7200,
	UART_BD_9600	= 9600,
	UART_BD_19200	= 19200,
	UART_BD_38400	= 38400,
	UART_BD_57600	= 57600,
	UART_BD_115200	= 115200
} E_UART_BD;

#define UART_FIFO_DEPTH  64

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
struct brite_uart {
	E_UART_PORT port;

	uint32_t base;	// IP base address
	uint32_t pclk;	// IP input frequency
	
	E_UART_BD bd;
	uint8_t data_bits;	// 5/6/7/8
	E_UART_DLS dls;
	E_UART_STOP stop;
	E_UART_EPS parity;

	uint8_t flow_ctl;		// 0: no flow control
							// 1: flow control
							
	uint32_t dt_mode;		// data transfer mode(cpu/dma)

	uint32_t len_per_xfer;
	uint32_t tx_dma_burst_size;
	uint32_t rx_dma_burst_size;	
	
	// dma parameter
	uint8_t dma_burst_len;	// dma burst length
	uint8_t dma_port;		// dma port no.
	uint8_t dma_tx_ch;		// tx channel of a dmac
	uint8_t dma_rx_ch;		// rx channel of a dmac
	uint8_t dma_tx_per;		// tx hardware handshaking interface 
	uint8_t dma_rx_per;		// rx hardware handshaking interface 

	// interrupt
	uint8_t int_en;		// 0: interrupt disable, 1: interrupt enable
	uint32_t int_vec;	// interrupt vector

	uint8_t line;
	uint32_t timeout;
};


/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern struct brite_uart g_uart[];

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void uart0_int_handler(void *ptr);

void uart1_int_handler(void *ptr);

void uart_wr_reg(E_UART_PORT port, uint32_t reg_ofst, uint32_t val);

uint32_t uart_rd_reg(E_UART_PORT port, uint32_t reg_ofst);

void uart_int_ctrl(E_UART_PORT port, uint32_t mask);

void uart_set_bd(E_UART_PORT port);

void uart_init(E_UART_PORT port);

void uart_flush(E_UART_PORT port);

void uart_putc(E_UART_PORT port, uint8_t ch);

uint8_t uart_getc(E_UART_PORT port);

int uart_tstc(E_UART_PORT port);

void uart_empty_fifo(E_UART_PORT port);

uint32_t uart_write(E_UART_PORT port, const uint8_t *p_buf, uint32_t len);

uint32_t uart_read(E_UART_PORT port, uint8_t *p_buf, uint32_t len);

uint32_t uart_rx_non_block(E_UART_PORT port, uint8_t *p_buf, uint32_t len, uint32_t timeout);

void uart_set_rx_fifo_thres(E_UART_PORT port, E_UART_RT_FIFO thres);

void uart_set_tx_fifo_thres(E_UART_PORT port, E_UART_TET_FIFO thres);

#endif

