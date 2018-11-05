/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_SPI_H__
#define __CDL_SPI_H__
#include "test_config.h"
#include "cdl_spi_reg.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// SPI FIFO depth by HW
#define SPI_FIFO_DEPTH	16

// cs active/inactive
typedef enum
{
	SPI_CS_DIS 	= 1,	// cs inactive
	SPI_CS_EN 	= 0		// cs active
}E_SPI_CS_CTRL;

// SPI mode
// CPOL: The sclk polarity when the spi is idle.
//		= 0, low level
//		= 1, high level
//
// CPHA: The sclk edge when sample.
//		= 0, sample at the first edge
//		= 1, sample at the second edge
typedef enum
{
	SPI_MODE0 = 0,	// CPOL= 0, CPHA = 0
	SPI_MODE1 = 1,	// CPOL= 0, CPHA = 1
	SPI_MODE2 = 2,	// CPOL= 1, CPHA = 0
	SPI_MODE3 = 3	// CPOL= 1, CPHA = 1
}E_SPI_MODE;

// SPI tranfer flags
typedef enum
{
	SPI_XFER_BEGIN 	= (1<<0),							// Assert CS before transfer
	SPI_XFER_END 	= (1<<1),							// Deassert CS after transfer
	SPI_XFER_ONCE 	= SPI_XFER_BEGIN | SPI_XFER_END,	// Assert/Deassert CS after transfer
}E_SPI_XFER_FLAG;

// RX/TX timerout
#define RX_TIMEOUT			1000	// timeout in ms
#define TX_TIMEOUT			1000	// timeout in ms

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
struct brite_spi {
	uint32_t port;
	uint32_t base;
	uint32_t pclk;			// IP input frequency
	uint32_t clk;			// IP working frequency
	
	E_SPI_FRF type;			// frame format(motorrola/ti/national)
	E_SPI_MS ms;			// master/slave
	E_SPI_MODE mode;		// polarity and phase
	E_DT_MODE dt_mode;		// data transfer mode(cpu/dma)
	E_SPI_SSE sync_mode;	// sync/async mode 
	uint8_t ds;				// data size(width)
	uint8_t step;			// 1/2 byte aligned

	// tx/rx data transfer
	uint32_t len;			// byte aligned
	uint8_t *tx;
	uint8_t *tx_end;
	uint8_t *rx;
	uint8_t *rx_end;

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
};

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern struct brite_spi g_spi[];

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void spi_int_handler(void *ptr);

void spi_cs_ctrl(E_SPI_PORT port, E_SPI_CS_CTRL ctrl);

void spi_wr_reg(E_SPI_PORT port, uint32_t reg_ofst, uint32_t val);

uint32_t spi_rd_reg(E_SPI_PORT port, uint32_t reg_ofst);

uint32_t spi_get_mis(E_SPI_PORT port);

void spi_clr_int(E_SPI_PORT port, uint32_t mask);

void spi_empty_fifo(E_SPI_PORT port);

BOOL spi_verify_periph_id(E_SPI_PORT port);

BOOL spi_verify_pcell_id(E_SPI_PORT port);

void spi_init(E_SPI_PORT port, uint32_t clk, E_SPI_FRF type, E_SPI_MS ms, 
					E_SPI_MODE mode, uint32_t ds, E_DT_MODE dt_mode);

int spi_xfer(E_SPI_PORT port, uint8_t *p_out, uint8_t *p_in, uint32_t len, E_SPI_XFER_FLAG flag);

int spi_xfer_tx_withoutDMA(E_SPI_PORT port, const uint8_t *p_out, uint32_t len, uint32_t timeout);


#endif

