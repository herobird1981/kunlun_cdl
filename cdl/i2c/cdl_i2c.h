/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_I2C_H__
#define __CDL_I2C_H__
#include "cdl_config.h"
#include "cdl_i2c_reg.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// i2c FIFO size
#define I2C_FIFO_SIZE 32

// i2c slave default address
#define I2C_SLAVE_DEFAULT_ADDR 0x50

// Worst case timeout for 1 byte is kept as 2ms
#define I2C_BYTE_TO 	50
#define I2C_STOPDET_TO	50
#define I2C_BYTE_TO_BB	(I2C_BYTE_TO * 16)

// Speed Selection
#define NANO_TO_MICRO 	1000
#define NANO_TO_S 		1000000000

// High and low times in different speed modes (in ns)
#define MIN_SS_SCL_HIGHTIME	4000
#define MIN_SS_SCL_LOWTIME	4700

// accoring to spec, the FS HIGH TIME should be 600, but for enlarge the marge, we change it as 700
#define MIN_FS_SCL_HIGHTIME	700 //600
#define MIN_FS_SCL_LOWTIME	1300
#define MIN_HS_SCL_HIGHTIME	60
#define MIN_HS_SCL_LOWTIME	160

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
struct dw_i2c {
	uint32_t regs;
	unsigned long pclk;
	int speed;
	int master_addr_mode; 	// 1 for 10 bit mode ; 0  7 bit mode

	int role; 				// master or slave 0: master 1:slave

	int dma_mode;
	int dmac_idx;
	int dma_ch_tx;
	int dma_ch_rx;
	int dma_req_tx;
	int dma_req_rx;

	int port;
	int int_vector;

	int slave_addr;
	int slave_addr_mode;
	int slave_reg_width; 	// regs width:1,2,3,4 bytes
	int max_len;
	int xfer_len;

	int slave_state; 		// slave state: 0 wait regs offset; 1 data phase
	int slave_tx_ptr;
	int slave_rx_ptr;
	int slave_rx_len;
	unsigned char slave_reg_off[4]; // store regs offset
};

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern struct dw_i2c *p_i2c_dev[];

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
extern E_DMA_ST dma_i2c_tx(struct dw_i2c *p_priv, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

extern E_DMA_ST dma_i2c_rx(struct dw_i2c *p_priv, E_DMA_CH ch, E_CH_PRIOR prior, uint32_t addr, uint32_t len, E_ST_POLLING st_polling);

void i2c0_int_handler(void *ptr);

void i2c1_int_handler(void *ptr);

void dw_i2c_writel(struct dw_i2c *p_priv, uint32_t val, uint32_t reg);

uint32_t dw_i2c_readl(struct dw_i2c *p_priv, uint32_t reg);

void i2c_init(struct dw_i2c *p_priv, uint32_t base, uint32_t pclk, uint32_t dma_mode);

// master mode
void i2c_init_master(struct dw_i2c *p_priv, E_I2C_SPEED speed, E_I2C_ADDR_MODE addr_mode);

int i2c_read(struct dw_i2c *p_priv, uint32_t dev, uint32_t addr, uint32_t alen, uint8_t *p_data, uint32_t dlen);

int i2c_write(struct dw_i2c *p_priv, uint32_t dev, uint32_t addr, uint32_t alen, uint8_t *p_data, uint32_t dlen);

// slave mode
void i2c_init_slave(struct dw_i2c *p_priv, uint32_t dev, uint32_t addr_mode, uint32_t reg_width, uint32_t max_len);

int32_t i2c_slave_transfer_rx(struct dw_i2c *p_priv, uint8_t *p_data);

int32_t i2c_slave_transfer_tx(struct dw_i2c *p_priv, uint8_t *p_data, uint32_t len);

uint32_t i2c_get_int_status(struct dw_i2c *p_priv);

uint32_t i2c_get_raw_int_status(struct dw_i2c *p_priv);

void i2c_set_int_mask(struct dw_i2c *p_priv, uint32_t mask);

void i2c_clear_int(struct dw_i2c *p_priv, uint32_t ofst);

void i2c_clear_all_int(struct dw_i2c *p_priv);

uint32_t i2c_get_tx_abrt_source(struct dw_i2c *p_priv);

void dw_i2c_tx_abort_recovery(struct dw_i2c* priv);

int i2c_master_tx_slave_rx(struct dw_i2c *p_master, struct dw_i2c *p_slave,
		uint32_t slave_dev, uint32_t addr, uint32_t alen, 
		uint8_t *p_tx_data, uint8_t *p_rx_data, uint32_t dlen);

int i2c_master_rx_slave_tx(struct dw_i2c *p_master, struct dw_i2c *p_slave,
		uint32_t slave_dev, uint32_t addr, uint32_t alen, 
		uint8_t *p_rx_data, uint8_t *p_tx_data, uint32_t dlen);
#endif

