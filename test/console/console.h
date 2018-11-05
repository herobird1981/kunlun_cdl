/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include "test_config.h"
#include "cdl_uart.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#if CONFIG_DSP0
#define CONSOLE_UART_PORT	UART0
#else
#define CONSOLE_UART_PORT	UART1
#endif

#define printf(...)	console_printf(__VA_ARGS__)

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern E_UART_PORT g_console_port;

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
//void console_init(E_UART_PORT port, E_UART_BD bd, E_UART_DLS dls, E_UART_STOP stop, E_UART_EPS parity);

void console_reinit_bd(uint32_t pclk);

int console_tstc(void);

void console_flush(void);

uint8_t console_getc(void);

//void console_puts(const uint8_t *s);

int console_gets(uint8_t *buf, uint32_t len);

int console_printf(const char* fmt, ...);

int console_check_char(uint8_t check);

#endif

