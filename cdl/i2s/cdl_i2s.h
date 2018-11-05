/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_I2S_H__
#define __CDL_I2S_H__
#include "test_config.h"
#include "cdl_i2s_reg.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// I2S data transfer format
typedef enum
{
	I2S_STANDARD 		= 0,
	I2S_LEFT_JUSTIFIED	= 1,
	I2S_RITHT_JUSTIFIED	= 2,
	I2S_TDM_POL0		= 3,
	I2S_TDM_POL1		= 4
}E_I2S_TYPE;

// I2S channel
typedef enum
{
	I2S_CH0		= 0,
	I2S_CH1		= 1,
	I2S_CH2		= 2,
	NUM_I2S_CHS	= 3
}E_I2S_CH;

/*add by Sunny*/
typedef enum {
	WORK_AS_MASTER = 0,
	WORK_AS_SLAVE,
	ROLE_LAST = WORK_AS_SLAVE
} E_I2S_ROLE;

typedef enum {
	SNDRV_PCM_STREAM_PLAYBACK = 0,
	SNDRV_PCM_STREAM_CAPTURE,
	SNDRV_PCM_STREAM_LAST = SNDRV_PCM_STREAM_CAPTURE
} E_I2S_PCM_STREAM;

typedef enum {
	SNDRV_PCM_FORMAT_S16_LE = 16,
	SNDRV_PCM_FORMAT_S24_LE = 24,
	SNDRV_PCM_FORMAT_S32_LE = 32,
	SNDRV_PCM_FORMAT_LAST = SNDRV_PCM_FORMAT_S32_LE
} E_I2S_PCM_FORMAT;

/**
 * enum dma_slave_buswidth - defines bus width of the DMA slave
 * device, source or target buses
 */
enum dma_slave_buswidth {
	DMA_SLAVE_BUSWIDTH_UNDEFINED = 0,
	DMA_SLAVE_BUSWIDTH_1_BYTE = 1,
	DMA_SLAVE_BUSWIDTH_2_BYTES = 2,
	DMA_SLAVE_BUSWIDTH_4_BYTES = 4
};

/* Number of entries in WORDSIZE and DATA_WIDTH parameter registers */
#define	COMP_MAX_WORDSIZE	(1 << 3)
#define	COMP_MAX_DATA_WIDTH	(1 << 2)

#define MAX_CHANNEL_NUM		6
#define MIN_CHANNEL_NUM		2

#define TWO_CHANNEL_SUPPORT     2  /* up to 2.0 */
#define FOUR_CHANNEL_SUPPORT    4  /* up to 3.1 */
#define SIX_CHANNEL_SUPPORT     6  /* up to 5.1 */
#define EIGHT_CHANNEL_SUPPORT   8  /* up to 7.1 */

#define DWC_I2S_PLAY	(1 << 0)
#define DWC_I2S_RECORD	(1 << 1)
#define DW_I2S_SLAVE	(1 << 2)
#define DW_I2S_MASTER	(1 << 3)

/*
 * DAI hardware clock masters.
 *
 * This is wrt the codec, the inverse is true for the interface
 * i.e. if the codec is clk and FRM master then the interface is
 * clk and frame slave.
 */
#define SND_SOC_DAIFMT_CBM_CFM		(1 << 12) /* codec clk & FRM master */
#define SND_SOC_DAIFMT_CBS_CFM		(2 << 12) /* codec clk slave & FRM master */
#define SND_SOC_DAIFMT_CBM_CFS		(3 << 12) /* codec clk master & frame slave */
#define SND_SOC_DAIFMT_CBS_CFS		(4 << 12) /* codec clk & FRM slave */

#define SND_SOC_DAIFMT_FORMAT_MASK	0x000f
#define SND_SOC_DAIFMT_CLOCK_MASK	0x00f0
#define SND_SOC_DAIFMT_INV_MASK		0x0f00
#define SND_SOC_DAIFMT_MASTER_MASK	0xf000

/*
 * The following tables allow a direct lookup of various parameters
 * defined in the I2S block's configuration in terms of sound system
 * parameters.  Each table is sized to the number of entries possible
 * according to the number of configuration bits describing an I2S
 * block parameter.
 */

/* Maximum bit resolution of a channel - not uniformly spaced */
static const u32 fifo_width[COMP_MAX_WORDSIZE] = {
	12, 16, 20, 24, 32, 0, 0, 0
};

/* Width of (DMA) bus */
static const u32 bus_widths[COMP_MAX_DATA_WIDTH] = {
	DMA_SLAVE_BUSWIDTH_1_BYTE,
	DMA_SLAVE_BUSWIDTH_2_BYTES,
	DMA_SLAVE_BUSWIDTH_4_BYTES,
	DMA_SLAVE_BUSWIDTH_UNDEFINED
};

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
struct brite_i2s{
	E_I2S_PORT port;
	uint32_t base;
	uint32_t pclk;		// IP input frequency
	uint32_t clk;		// IP working frequency
	
	E_I2S_TYPE type;	// frame format
	uint8_t wss;		// the number of sclk cycles for which the word select
	uint8_t wlen;		// the desired data resolution of the receiver/transmiter
	E_I2S_CTRL clk_en;	// clock generation
	E_DT_MODE dt_mode;	// data transfer mode(cpu/dma)

	// dma parameter
	uint8_t dma_burst_len;
	uint8_t dma_core_idx;
	uint8_t dma_ch_tx;
	uint8_t dma_ch_rx;
	uint8_t dma_req_tx;
	uint8_t dma_req_rx;
	int dma_irq_en;
		
	// interrupt
	uint8_t int_en;		// 0: interrupt disable, 1: interrupt enable
	uint32_t int_vec;	// interrupt vector

	unsigned int capability;
	E_I2S_ROLE role; 				// master or slave; 0: master 1:slave
	E_PI_MODE pi_mode;  			// polling or interrupt mode
	E_I2S_PCM_STREAM stream_dir; 	// stream (direction) 
	E_I2S_PCM_FORMAT sample_format; // sampling format (S16_LE, S24_LE, S32_LE)
	unsigned int sample_rate;		// sampling frequency (8Khz, 16Khz, 32Khz, 44Khz, 48Khz)
	unsigned int bits_per_sample;	// bits per sample (16/24/32 bit)
	unsigned int data_step;
	unsigned int ch_num;			// number of channel
	unsigned int ch_mask;

	// from ip configure
	unsigned int fifo_depth;
	unsigned int tx_max_channels;
	unsigned int rx_max_channels;
	unsigned int tx_fifo_width;
	unsigned int rx_fifo_width;
	unsigned int tx_fifo_empty_trig;
	unsigned int rx_fifo_avail_trig;
	unsigned int tx_burst_size;
	unsigned int rx_burst_size;

	int hw_channel_map[4];

	unsigned int int_line[6];
	void *tx_buf;
	unsigned int tx_len;
	unsigned int tx_actual;
	int tx_ch_idx[4];
	volatile unsigned int tx_ch_done;

	void *rx_buf;
	unsigned int rx_len;
	unsigned int rx_actual;
	int rx_ch_idx[4];
    volatile unsigned int rx_ch_done;

	unsigned char *chnl_buf_cur[4];
	unsigned int chnl_data_step;
};

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern struct brite_i2s g_i2s[];

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void i2s_init(E_I2S_PORT port, uint32_t clk, E_I2S_TYPE type, E_I2S_WSS wss, 
					E_I2S_WLEN wlen, E_I2S_CTRL clk_en, E_DT_MODE dt_mode,
					E_PI_MODE pi_mode, E_I2S_PCM_FORMAT sample_format,
					int ch0_en, int ch1_en, int ch2_en);

void i2s_start_transfer(struct brite_i2s *priv, void *data, uint32_t len);

void i2s_exit(E_I2S_PORT port);

int i2s_xfer_tx(E_I2S_PORT port, void *data, uint32_t len);

int i2s_xfer_rx(E_I2S_PORT port, void *data, uint32_t len);

int i2s_xfer_tx_async(E_I2S_PORT port, void *data, uint32_t len);

int i2s_xfer(E_I2S_PORT port_tx, E_I2S_PORT port_rx, void *data_tx, void *data_rx, uint32_t len);

int i2s_vad_xfer(E_I2S_PORT port_tx, E_I2S_PORT port_rx, void *data_tx, void *data_rx, uint32_t len);

uint32_t write_int_as_num_bytes(void *p, unsigned int val, int n);

#endif

