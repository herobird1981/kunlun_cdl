/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_DMA_H__
#define __CDL_DMA_H__
#include "test_config.h"
#include "cdl_dma_reg.h"
#include "cdl_sys.h"

struct musb_hw_ep;

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define DMAC_TIMEOUT	1200

#define DMA_MAX_BURST_SIZE	4096

// DMA Port Number
typedef enum {
	DMA0 = 0,	
	NUM_DMA_PORTS   = 1
} E_DMA_PORT;

// DMA Channel Number
typedef enum {
	DMA_CH0 = 0,
	DMA_CH1 = 1,
	DMA_CH2 = 2,
	DMA_CH3 = 3,
	DMA_CH4 = 4,
	DMA_CH5 = 5,
	DMA_CH6 = 6,
	DMA_CH7 = 7,
	NUM_DMA_CHANNELS = 8
} E_DMA_CH;

// DMA status
typedef enum {
	DMA_ST_DONE		= 0,
	DMA_ST_FAIL		= 1,
	DMA_ST_TIME_OUT	= 2
} E_DMA_ST;

// status polling
typedef enum
{
	ST_NO_POLLING	= 0,
	ST_POLLING		= 1
}E_ST_POLLING;


typedef enum {
	MEM_TO_MEM = 0,
	MEM_TO_PERI,
	PERI_TO_MEM,
	PERI_TO_PERI
}FLOW_CONTROL;

typedef enum {
	BYTE_WIDTH = 0,
	HWORD_WIDTH,
	WORD_WIDTH,
	TWO_WORD_WIDTH,
	FOUR_WORD_WIDTH,
	EIGHT_WORD_WIDTH
}TRANSFRE_WIDTH;

typedef enum {
	INCREASE = 0,
	DECREASE,
	FIXED
}ADDRESS_UPDATE;

typedef enum {
	BURST_1 = 0,
	BURST_4,
	BURST_8,
	BURST_16,
	BURST_32,
	BURST_64,
	BURST_128,
	BURST_256
}BURST_SIZE;

// peripheral hw handshake
#define DMA_REQ_USB_EP1		0
#define DMA_REQ_USB_EP2		1
#define DMA_REQ_USB_EP3		2	
#define DMA_REQ_I2S_ADC		3	// done
#define DMA_REQ_I2S_DAC		4	// done
#define DMA_REQ_I2S1		5	// done
#define DMA_REQ_SPI_TX		6	// done
#define DMA_REQ_SPI_RX		7	// done
#define DMA_REQ_I2C_S_TX	8	// done
#define DMA_REQ_I2C_S_RX	9	// done
#define DMA_REQ_I2C_M_TX	10	// done
#define DMA_REQ_I2C_M_RX	11	// done
#define DMA_REQ_UART0_TX	12	// done
#define DMA_REQ_UART0_RX	13	// done
#define DMA_REQ_UART1_TX	14	// done
#define DMA_REQ_UART1_RX	15	// done

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
struct dma_ch_config {
	// source
	E_DMA_WIDTH src_width;
	E_DMA_MSIZE src_msize;
	E_DMA_ADDR src_addr_inc;
	uint8_t src_per;			// a hardware handshaking interface
	E_HS_SEL src_hs_sel; 

	// destination
	E_DMA_WIDTH dst_width;
	E_DMA_MSIZE dst_msize;
	E_DMA_ADDR dst_addr_inc;
	uint8_t dst_per;			// a hardware handshaking interface
	E_HS_SEL dst_hs_sel; 

	// common
	E_TT_FC tt_fc;
};

struct brite_dma{
	uint32_t base;
	int int_vector;
	int port;

	// channel
	struct dma_ch_config ch_cfg[8];
};

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern struct brite_dma g_dma[];

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void dma_int_handler(void *ptr);

void dma_wr_reg(E_DMA_PORT port, uint32_t reg_ofst, uint32_t val);

uint32_t dma_rd_reg(E_DMA_PORT port, uint32_t reg_ofst);

void dma_rd_id(E_DMA_PORT port, uint32_t *p_id);

void dma_rd_ver(E_DMA_PORT port, uint32_t *p_ver);

void dma_int_enable(E_DMA_PORT port, E_DMAC_INT_EN int_en);

uint32_t dma_rd_int_st(E_DMA_PORT port);

void dma_clr_com_int_st(E_DMA_PORT port, uint32_t mask);

void dma_en_com_int(E_DMA_PORT port, uint32_t mask);

void dma_en_com_intsignal(E_DMA_PORT port, uint32_t mask);

uint32_t dma_rd_com_int_st(E_DMA_PORT port);

void dma_clr_ch_int_st(E_DMA_PORT port, E_DMA_CH ch, uint32_t mask);

void dma_en_ch_int(E_DMA_PORT port, E_DMA_CH ch, uint32_t mask);

void dma_en_ch_intsignal(E_DMA_PORT port, E_DMA_CH ch, uint32_t mask);

uint32_t dma_rd_ch_int_st(E_DMA_PORT port, E_DMA_CH ch);

void dma_init(E_DMA_PORT port);

E_DMA_ST dma_polling_st(E_DMA_PORT port, E_DMA_CH ch);

E_DMA_ST dma_m2m(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t dst_addr, uint32_t src_addr, uint32_t len, E_ST_POLLING st_polling);

E_DMA_ST dma_uart_tx(E_UART_PORT port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

E_DMA_ST dma_uart_rx(E_UART_PORT port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

E_DMA_ST dma_spi_tx(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

E_DMA_ST dma_spi_rx(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

E_DMA_ST dma_i2s_tx(E_I2S_PORT i2s_port, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

E_DMA_ST dma_i2s_rx(E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

//E_DMA_ST dma_i2c_tx(struct dw_i2c *p_priv, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

//E_DMA_ST dma_i2c_rx(struct dw_i2c *p_priv, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

E_DMA_ST dma_usb_tx(struct musb_hw_ep *hw_ep, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

E_DMA_ST dma_usb_rx(struct musb_hw_ep *hw_ep, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

#endif

