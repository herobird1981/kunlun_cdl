/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_I2S_REG_H__
#define __CDL_I2S_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define I2S_PORT_SIZE	0x20
#define I2S_CH_SIZE		0x40

// register layout 
#define I2S_IER_OFST			0x000 	// 1   bits, R/W, DW_apb_i2s Enable Register
#define I2S_IRER_OFST			0x004 	// 1   bits, R/W, I2S Receiver Block Enable Register
#define I2S_ITER_OFST			0x008 	// 1   bits, R/W, I2S Transmitter Block Enable Register
#define I2S_CER_OFST			0x00C 	// 1   bits, R/W, Clock Enable Register
#define I2S_CCR_OFST			0x010 	// 5   bits, R/W, Clock Configuration Register
#define I2S_RXFFR_OFST			0x014 	// 1   bits,    W, Receiver Block FIFO Register
#define I2S_TXFFR_OFST			0x018 	// 1   bits,    W, Transmitter Block FIFO Register

// left/right 0/1/2
#define I2S_LRBR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x020) 	// 32  bits,    R, Left Receive Buffer 0
#define I2S_LTHR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x020) 	// 32  bits,    W, Left Transmit Holding Register 0
#define I2S_RRBR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x024) 	// 32  bits,    R, Right Receive Buffer 0
#define I2S_RTHR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x024) 	// 32  bits,    W, Right Transmit Holding Register 0
#define I2S_RER_OFST(ch)		((I2S_CH_SIZE * ch) + 0x028) 	// 1   bits, R/W, Receive Enable Register 0
#define I2S_TER_OFST(ch)		((I2S_CH_SIZE * ch) + 0x02C) 	// 1   bits, R/W, Transmit Enable Register 0
#define I2S_RCR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x030) 	// 3   bits, R/W, Receive Configuration Register 0
#define I2S_TCR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x034) 	// 3   bits, R/W, Transmit Configuration Register 0
#define I2S_ISR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x038) 	// 6   bits,     R, Interrupt Status Register 0
#define I2S_IMR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x03C) 	// 6   bits, R/W, Interrupt Mask Register 0
#define I2S_ROR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x040) 	// 1   bits,     R, Receive Overrun Register 0
#define I2S_TOR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x044) 	// 1   bits,    R, Transmit Overrun Register 0
#define I2S_RFCR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x048) 	// 4   bits, R/W, Receive FIFO Configuration Register 0
#define I2S_TFCR_OFST(ch)		((I2S_CH_SIZE * ch) + 0x04C) 	// 4   bits, R/W, Transmit FIFO Configuration Register 0
#define I2S_RFF_OFST(ch)		((I2S_CH_SIZE * ch) + 0x050) 	// 1   bits,    W, Receive FIFO Flush 0
#define I2S_TFF_OFST(ch)		((I2S_CH_SIZE * ch) + 0x054) 	// 1   bits,    W, Transmit FIFO Flush 0

// dma
#define I2S_RXDMA_OFST			0x1C0 	// X   bits,    R, Receiver Block DMA Register
#define I2S_RRXDMA_OFST			0x1C4 	// 1   bits,    W, Reset Receiver Block DMA Register
#define I2S_TXDMA_OFST			0x1C8 	// X   bits,    W, Transmitter Block DMA Register
#define I2S_RTXDMA_OFST			0x1CC 	// 1   bits,    W, Reset Transmitter Block DMA Register
#define I2S_COMP_PARAM_2_OFST	0x1F0 	// 32 bits,     R, Component Parameter 2 Register
#define I2S_COMP_PARAM_1_OFST	0x1F4 	// 32 bits,     R, Component Parameter 1 Register
#define I2S_COMP_VERSION_OFST	0x1F8 	// 32 bits,     R, Component Version ID
#define I2S_COMP_TYPE_OFST		0x1FC 	// 32 bits,     R, DesignWare Component Type

// offset: 0x000, DW_apb_i2s Enable Register
typedef enum
{
	I2S_IEN_SHIFT 		= 0,	// [0], DW_apb_i2s enable. A disable on this bit overrides any other block 
								// or channel enables and flushes all FIFOs.
								// For more information about how this register affects the other 
								// DW_apb_i2s blocks, refer to "DW_apb_i2s Enable¡± on page 36.
								// 1: enable DW_apb_i2s
								// 0: disable DW_apb_i2s
								// Reset: 0
										
	I2S_IER_REVD_SHIFT	= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_IER_SHIFT;

// [0], IEN mask and value
#define I2S_IEN_MASK		(1<<I2S_IEN_SHIFT)
#define I2S_IEN_VAL(val)	((val)<<I2S_IEN_SHIFT)

typedef enum
{
	I2S_DIS = 0,
	I2S_EN 	= 1
}E_I2S_CTRL;

// offset: 0x004, I2S Receiver Block Enable Register
typedef enum
{
	I2S_RXEN_SHIFT 		= 0,	// [0], Receiver block enable. A disable on this bit overrides any individual receive 
								// channel enables.
								// 1: enable receiver
								// 0: disable receiver
								// For more information about the receiver block, refer to ¡°DW_apb_i2s as 
								// Receiver¡± on page 43.
								// Dependencies: I2S_RECEIVER_BLOCK = 1
								// Reset: 0
										
	I2S_IRER_REVD_SHIFT	= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_IRER_SHIFT;

// [0], RXEN mask and value
#define I2S_RXEN_MASK		(1<<I2S_RXEN_SHIFT)
#define I2S_RXEN_VAL(val)	((val)<<I2S_RXEN_SHIFT)

// offset: 0x008, I2S Transmitter Block Enable Register
typedef enum
{
	I2S_TXEN_SHIFT 		= 0,	// [0], R/W, Transmitter block enable. A disable on this bit overrides any individual transmit 
								// channel enables.
								// 1: enable transmitter
								// 0: disable transmitter
								// For more information about the transmitter block, refer to ¡°DW_apb_i2s as Transmitter¡± 
								// on page 37.
								// Dependencies: I2S_TRANSMITTER_BLOCK = 1
								// Reset: 0
										
	I2S_ITER_REVD_SHIFT	= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_ITER_SHIFT;

// [0], TXEN mask and value
#define I2S_TXEN_MASK		(1<<I2S_TXEN_SHIFT)
#define I2S_TXEN_VAL(val)	((val)<<I2S_TXEN_SHIFT)

// offset: 0x00C, Clock Enable Register
typedef enum
{
	I2S_CLKEN_SHIFT 	= 0,	// [0], R/W, Clock generation enable/disable. This bit enables/disables the clock generation 
								// signals when DW_apb_i2s is a master: sclk_en, ws_out, and sclk_gate.
								// 1: enable
								// 0: disable
								// For more information about clock generation, refer to ¡°Clock Generation (Master 
								// Mode)¡± on page 48.
								// Dependencies: This register is only relevant when the component is configured 
								// to be a master (I2S_MODE_EN = 1).
								// Reset: 0
										
	I2S_CER_REVD_SHIFT	= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_CER_SHIFT;

// [0], CLKEN mask and value
#define I2S_CLKEN_MASK		(1<<I2S_CLKEN_SHIFT)
#define I2S_CLKEN_VAL(val)	((val)<<I2S_CLKEN_SHIFT)

// offset: 0x010, Clock Configuration Register
typedef enum
{
	I2S_SCLKG_SHIFT 	= 0,	// [2:0], R/W, These bits are used to program the gating of sclk:
								// 0: No clock gating
								// 1: Gate after 12 clock cycles 
								// 2: Gate after 16 clock cycles 
								// 3: Gate after 20 clock cycles 
								// 4: Gate after 24 clock cycles 
								// The programmed gating value should be greater than or equal to the largest 
								// configured/programmed audio resolution to prevent the truncating of RX/TX data.
								// The I2S Clock Generation block must be disabled (CER[0] = 0) prior to any changes in this value.
								// Dependencies: This register is only relevant when component is configured to be a 
								// master (I2S_MODE_EN = 1).
								// Reset: I2S_SCLK_GATE configuration parameter

	I2S_WSS_SHIFT 		= 3,	// [4:3], R/W, These bits are used to program the number of sclk cycles for which the word select 
								// line (ws_out) stays in the left or right sample mode:
								// 0: 16 clock cycles
								// 1: 24 clock cycles 
								// 2: 32 clock cycles
								// The I2S Clock Generation block must be disabled (CER[0] = 0) prior to any changes in this value.
								// Dependencies: This register is only relevant when component is configured to be a 
								// master (I2S_MODE_EN = 1).
								// Reset: I2S_WS_LENGTH configuration parameter
										
	I2S_CCR_REVD_SHIFT	= 5		// [31:5], N/A, Reserved and read as zero.
}E_I2S_CCR_SHIFT;

// [2:0], SCLKG mask and value
#define I2S_SCLKG_MASK		(0x7<<I2S_SCLKG_SHIFT)
#define I2S_SCLKG_VAL(val)	((val)<<I2S_SCLKG_SHIFT)

typedef enum
{
	I2S_GATE_AFTER_NO_CLK_CYCLE = 0,
	I2S_GATE_AFTER_12_CLK_CYCLE = 1,
	I2S_GATE_AFTER_16_CLK_CYCLE = 2,
	I2S_GATE_AFTER_20_CLK_CYCLE = 3,
	I2S_GATE_AFTER_24_CLK_CYCLE = 4
}E_I2S_SCLKG;

// [4:3], WSS mask and value
#define I2S_WSS_MASK		(0x3<<I2S_WSS_SHIFT)
#define I2S_WSS_VAL(val)	((val)<<I2S_WSS_SHIFT)

typedef enum
{
	I2S_WSS_16_CLK_CYCLE = 0,
	I2S_WSS_24_CLK_CYCLE = 1,
	I2S_WSS_32_CLK_CYCLE = 2
}E_I2S_WSS;

// offset: 0x014, Receiver Block FIFO Reset Register
typedef enum
{
	I2S_RXFFR_SHIFT 		= 0,	// [0], W, Receiver FIFO Reset. Writing a 1 to this register flushes all the RX FIFOs (this is 
									// a self clearing bit).
									// Receiver Block must be disabled prior to writing this bit.
									// Dependencies: I2S_RECEIVER_BLOCK = 1
									// Reset: 0
										
	I2S_RXFFR_REVD_SHIFT	= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_RXFFR_SHIFT;

// [0], RXFFR mask and value
#define I2S_RXFFR_MASK		(1<<I2S_RXFFR_SHIFT)
#define I2S_RXFFR_VAL(val)	((val)<<I2S_RXFFR_SHIFT)

// offset: 0x018, Transmitter Block FIFO Reset Register
typedef enum
{
	I2S_TXFFR_SHIFT 		= 0,	// [0], W, Transmitter FIFO Reset. Writing a 1 to this register flushes all the TX FIFOs 
									// (this is a self clearing bit). The Transmitter Block must be disabled prior to 
									// writing this bit.
									// Dependencies: I2S_TRANSMITTER_BLOCK = 1
									// Reset: 0
										
	I2S_TXFFR_REVD_SHIFT	= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_TXFFR_SHIFT;

// [0], TXFFR mask and value
#define I2S_TXFFR_MASK		(1<<I2S_TXFFR_SHIFT)
#define I2S_TXFFR_VAL(val)	((val)<<I2S_TXFFR_SHIFT)

// offset: 0x020, Left Receive Buffer Register
// LRBR0: 0x020 
// LRBR1: 0x060 
// LRBR2: 0x0A0 
// LRBR3: 0x0E0
typedef enum
{
	I2S_LRBRX_SHIFT 		= 0,	// [I2S_RX_WORDSIZE_x - 1:0], R, The left stereo data received serially from the receive 
									// channel input (sdix) is read through this register. 
									// If the RX FIFO is full and the two-stage read 
									// operation (for instance, a read from LRBRx followed 
									// by a read from RRBRx) is not performed before the 
									// start of the next stereo pair, then the new data is lost 
									// and an overrun interrupt occurs. (Data already in the 
									// RX FIFO is preserved.)
									// Dependencies: I2S_RX_CHANNELS > x, where x is 
									// the number of receive channel.
									// NOTE: Before reading this register again, the right 
									// stereo data MUST be read from RRBRx, or the 
									// status/interrupts will not be valid.
									// Reset: 0x0
										
	I2S_LRBRX_REVD_SHIFT	= 1		// [31:I2S_RX_WORDSIZE_x], N/A, Reserved and read as zero.
}E_I2S_LRBRX_SHIFT;

// offset: 0x020, Left Transmit Holding Register
// LTHR0: 0x020 
// LTHR1: 0x060 
// LTHR2: 0x0A0 
// LTHR3: 0x0E0
typedef enum
{
	I2S_LTHRX_SHIFT 		= 0,	// [I2S_TX_WORDSIZE_x - 1:0], W, The left stereo data to be transmitted serially through the 
									// transmit channel output (sdox) is written through this register. 
									// Writing is a two-stage process: 
									// (1) A write to this register passes the left stereo sample to 
									// the transmitter.
									// (2) This MUST be followed by writing the right stereo sample 
									// to the RTHRx register.
									// Data should only be written to the FIFO when it is not full. 
									// Any attempt to write to a full FIFO results in that data being 
									// lost and an overrun interrupt being generated.
									// Dependencies: I2S_TX_CHANNELS > x, where x is the 
									// number of transmit channel.
									// Reset: 0x0
										
	I2S_LTHRX_REVD_SHIFT	= 1		// [31:I2S_TX_WORDSIZE_x], N/A, Reserved and read as zero.
}E_I2S_LTHRX_SHIFT;

// offset: 0x024,  Right Transmit Holding Register
// RTHR0: 0x024 
// RTHR1: 0x064 
// RTHR2: 0x0A4 
// RTHR3: 0x0E4
typedef enum
{
	I2S_TX_SHIFT 		= 0,	// [I2S_TX_WORDSIZE_x - 1:0], W, The right stereo data to be transmitted serially through the 
								// transmit channel output (sdox) is written through this 
								// register. Writing is a two-stage process: 
								// (1) A left stereo sample MUST first be written to the LTHRx 
								// register.
								// (2) A write to this register passes the right stereo sample to 
								// the transmitter.
								// Data should only be written to the FIFO when it is not full. 
								// Any attempt to write to a full FIFO results in that data being 
								// lost and an overrun interrupt being generated.
								// Dependencies: I2S_TX_CHANNELS > x, where x is the 
								// number of transmit channel.
								// Reset: 0x0
										
	I2S_RTHRX_REVD_SHIFT	= 1		// [31:I2S_TX_WORDSIZE_x], N/A, Reserved and read as zero.
}E_I2S_RTHRX_SHIFT;

// offset: 0x028,  Receive Enable Register
// RER0: 0x028 
// RER1: 0x068 
// RER2: 0x0A8 
// RER3: 0x0E8
typedef enum
{
	I2S_RXCHENX_SHIFT 	= 0,	// [0], R/W, Receive channel enable. This bit enables/disables a receive channel, 
								// independently of all other channels.
								// On enable, the channel begins receiving on the next left stereo cycle.
								// A global disable of DW_apb_i2s (IER[0] = 0) or the Receiver block (IRER[0] = 
								// 0) overrides this value.
								// 1: Enable
								// 0: Disable
								// Dependencies: I2S_RX_CHANNELS > x, where x is the number of receive channel.
								// Reset: 1
									
	I2S_RERX_REVD_SHIFT	= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_RERX_SHIFT;

// offset: 0x02C,   Transmit Enable Register
// RER0: 0x02C 
// RER1: 0x06C 
// RER2: 0x0AC 
// RER3: 0x0EC
typedef enum
{
	I2S_TXCHENX_SHIFT 	= 0,	// [0], R/W, Transmit channel enable. This bit enables/disables a transmit channel, 
								// independently of all other channels.
								// On enable, the channel begins transmitting on the next left stereo cycle.
								// A global disable of DW_apb_i2s (IER[0] = 0) or Transmitter block (ITER[0] = 
								// 0) overrides this value.
								// 0: Disable
								// 1: Enable
								// Dependencies: I2S_TX_CHANNELS > x, where x is the number of transmit channel.
								// Reset: 1

	I2S_TERX_REVD_SHIFT	= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_TERX_SHIFT;

// offset: 0x034, Receive Configuration Register
// RCR0: 0x034 
// RCR1: 0x074 
// RCR2: 0x0B4 
// RCR3: 0x0F4
typedef enum
{
	I2S_WLEN_SHIFT 		= 0,	// [2: 0], R/W, These bits are used to program the desired data resolution of the receiver 
								// and enables the LSB of the incoming left (or right) word to be placed in the 
								// LSB of the LRBRx (or RRBRx) register.
								// 000 = Ignore word length 
								// 001 = 12 bit resolution 
								// 010 = 16 bit resolution 
								// 011 = 20 bit resolution 
								// 100 = 24 bit resolution 
								// 101 = 32 bit resolution
								// Programmed data resolution must be less than or equal to 
								// I2S_RX_WORDSIZE_x. If the selected resolution is greater than the 
								// I2S_RX_WORDSIZE_x, the receive channel defaults back to 
								// I2S_RX_WORDSIZE_RESET_x.
								// The channel must be disabled prior to any changes in this value  (RERx[0] = 0).
								// Dependencies: I2S_RX_CHANNELS > x, where x is the number of receive channel.
								// Reset: I2S_RX_WORDSIZE_RESET_x, where x is the number of receive channel

	I2S_RCRX_REVD_SHIFT	= 3		// [31:3], N/A, Reserved and read as zero.
}E_I2S_RCRX_SHIFT;

// WLEN mask and value
#define I2S_WLEN_SHIFT		(0x7<<I2S_WLEN_SHIFT)
#define I2S_WLEN_VAL(val)	((val)<<I2S_WLEN_SHIFT)

typedef enum
{
	I2S_WLEN_IGNORE   = 0,
	I2S_WSS_12BIT_RES = 1,
	I2S_WSS_16BIT_RES = 2,
	I2S_WSS_20BIT_RES = 3,
	I2S_WSS_24BIT_RES = 4,
	I2S_WSS_32BIT_RES = 5
}E_I2S_WLEN;

// offset: 0x038,  Interrupt Status Register
// ISR0: 0x038 
// ISR1: 0x078 
// ISR2: 0x0B8 
// ISR3: 0x0F8
// offset: 0x03C, Interrupt Mask Register
// IMR0: 0x03C 
// IMR1: 0x07C 
// IMR2: 0x0BC 
// IMR3: 0x0FC
typedef enum
{
	I2S_ST_RXDA_SHIFT 		= 0,	// [0], R, Status of Receive Data Available interrupt. RX FIFO data available.
									// 1: trigger level reached 
									// 0: trigger level not reached
									// Dependencies: I2S_RX_CHANNELS > x, where x is the number of RX channel.
									// Reset: 0
	I2S_ST_RXFO_SHIFT 		= 1,	// [1], R, Status of Data Overrun interrupt for the RX channel. Incoming data lost due to a full RX FIFO.
									// 0: RX FIFO write valid 
									// 1: RX FIFO write overrun
									// Dependencies: I2S_RX_CHANNELS > x, where x is the number of RX channel.
									// Reset: 0

	I2S_ST_REVD0_SHIFT 		= 2,	// [3:2], N/A, Reserved and read as zero.

	I2S_ST_TXFE_SHIFT 		= 4,	// [4], R, Status of Transmit Empty Trigger interrupt. TX FIFO is empty.
									// 1: trigger level reached 
									// 0: trigger level not reached
									// Dependencies: I2S_TX_CHANNELS > x, where x is the number of TX channel.
									// Reset: 1

	I2S_ST_TXFO_SHIFT 		= 5,	// [5], R, Status of Data Overrun interrupt for the TX channel. Attempt to write to full TX FIFO.
									// 0: TX FIFO write valid 
									// 1: TX FIFO write overrun
									// Dependencies: I2S_TX_CHANNELS > x, where x is the number of transmit channel.
									// Reset: 0

	I2S_ISRX_REVD_SHIFT		= 6		// [31:6], N/A, Reserved and read as zero.
}E_I2S_ISRX_SHIFT;

// ISR mask
#define I2S_ST_RXDA_MASK	(1<<I2S_ST_RXDA_SHIFT)
#define I2S_ST_RXFO_MASK	(1<<I2S_ST_RXFO_SHIFT)
#define I2S_ST_TXFE_MASK	(1<<I2S_ST_TXFE_SHIFT)
#define I2S_ST_TXFO_MASK	(1<<I2S_ST_TXFO_SHIFT)

// IMR mask
#define I2S_IMR_RXDA_MASK	(1<<I2S_ST_RXDA_SHIFT)
#define I2S_IMR_RXFO_MASK	(1<<I2S_ST_RXFO_SHIFT)
#define I2S_IMR_TXFE_MASK	(1<<I2S_ST_TXFE_SHIFT)
#define I2S_IMR_TXFO_MASK	(1<<I2S_ST_TXFO_SHIFT)

// offset: 0x040  Receive Overrun Register
// ROR0: 0x040 
// ROR1: 0x080 
// ROR2: 0x0C0 
// ROR3: 0x100
typedef enum
{
	I2S_RXCHO_SHIFT 		= 0,	// [0], R, Read this bit to clear the RX FIFO Data Overrun interrupt.
									// 0: RX FIFO write valid 
									// 1: RX FIFO write overrun
									// Dependencies: I2S_RX_CHANNELS > x, where x is the number of receive channel.
									// Reset: 0

	I2S_RORX_REVD_SHIFT		= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_RORX_SHIFT;

// mask
#define I2S_RXCHO_MASK	(1<<I2S_RXCHO_SHIFT)

// offset: 0x044  Receive Overrun Register
// TOR0: 0x044 
// TOR1: 0x084 
// TOR2: 0x0C4 
// TOR3: 0x104
typedef enum
{
	I2S_TXCHO_SHIFT 		= 0,	// [0], R, Read this bit to clear the TX FIFO Data Overrun interrupt.
									// 0: TX FIFO write valid 
									// 1: TX FIFO write overrun
									// Dependencies: I2S_TX_CHANNELS > x, where x is the number of transmit channel.
									// Reset: 0

	I2S_TORX_REVD_SHIFT		= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_TORX_SHIFT;

// mask
#define I2S_TXCHO_MASK	(1<<I2S_TXCHO_SHIFT)

// offset: 0x048,  Receive FIFO Configuration Register
// RFCR0: 0x048 
// RFCR1: 0x088 
// RFCR2: 0x0C8 
// RFCR3: 0x108
// offset: 0x04C,  Transmit FIFO Configuration Register
// TFCR0: 0x04C 
// TFCR1: 0x08C 
// TFCR2: 0x0CC 
// TFCR3: 0x10C
typedef enum
{
	I2S_RXCHDT_SHIFT 		= 0,	// [3:0], R/W, These bits program the trigger level in the RX FIFO at which the Received 
									// Data Available interrupt is generated. 
									// Trigger Level = Programmed Value + 1  
									// (for example, 1 to I2S_RX_FIFO_DEPTH_x)
									// Valid RXCHDT values: 0 to (I2S_RX_FIFO_x - 1)
									// If an illegal value is programmed, these bits saturate to  
									// (I2S_RX_FIFO_x ¨C 1). 
									// The channel must be disabled prior to any changes in this value (that is, 
									// RERx[0] = 0).
									// Dependencies: I2S_RX_CHANNELS > x, where x is the number of receive 
									// channel.
									// Reset: I2S_RX_FIFO_THRE_x, where x is the number of receive channel.

	I2S_RFCRX_REVD_SHIFT	= 4		// [31:4], N/A, Reserved and read as zero.
}E_I2S_RFCRX_SHIFT;

// RFCR mask
#define I2S_RXCHDT_MASK	(0xF<<I2S_RXCHDT_SHIFT)

// TFCR mask
#define I2S_TXCHDT_MASK	(0xF<<I2S_RXCHDT_SHIFT)

// offset: 0x050,  Receive FIFO Flush Register
// RFF0: 0x050 
// RFF1: 0x090 
// RFF2: 0x0D0 
// RFF3: 0x110
// offset: 0x054,  Transmit FIFO Flush Register
// TFF0: 0x054 
// TFF1: 0x094 
// TFF2: 0x0D4 
// TFF3: 0x114
typedef enum
{
	I2S_RXCHFR_SHIFT 	= 0,	// [0], W, Receive Channel FIFO Reset. Writing a 1 to this register flushes an individual 
								// RX FIFO. (This is a self clearing bit.)
								// RX channel or block must be disabled prior to writing to this bit.
								// Dependencies: I2S_RX_CHANNELS > x, where x is the number of receive channel.
								// Reset: 0

	I2S_RFFX_REVD_SHIFT	= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_RFFX_SHIFT;

// RXCHFR mask
#define I2S_RXCHFR_MASK	(1<<I2S_RXCHFR_SHIFT)

// TXCHFR mask
#define I2S_TXCHFR_MASK	(1<<I2S_RXCHFR_SHIFT)

// offset: 0x1C0,  Receiver Block DMA Register
// offset: 0x1C8,  Transmitter Block DMA Register
typedef enum
{
	I2S_RXDMA_SHIFT 		= 0,	// [APB_DATA_WIDTH ¨C 1:0], R, Receiver Block DMA Register. Used to cycle repeatedly 
									// through the enabled receive channels (from lowest 
									// numbered to highest), reading stereo data pairs.
									// Dependencies: I2S_RECEIVER_BLOCK = 1
									// Reset: 0x00

	I2S_RXDMA_REVD_SHIFT	= 1		// [31:APB_DATA_WIDTH], N/A, Reserved and read as zero.
}E_I2S_RXDMA_SHIFT;

// offset: 0x1C4,  Reset Receiver Block DMA Register
// offset: 0x1CC,  Reset Transmitter Block DMA Register
typedef enum
{
	I2S_RRXDMA_SHIFT 		= 0,	// [0], W, Reset Receiver Block DMA Register. Writing a 1 to this self-clearing register 
									// resets the RXDMA register mid-cycle to point to the lowest enabled Receive channel. 
									// NOTE: Writing to this register has no effect if the component is performing a 
									// stereo pair read (such as, when left stereo data has been read but not right 
									// stereo data).
									// Dependencies: I2S_RECEIVER_BLOCK = 1 AND I2S_RX_CHANNELS > 1
									// Reset: 0x00

	I2S_RRXDMA_REVD_SHIFT	= 1		// [31:1], N/A, Reserved and read as zero.
}E_I2S_RRXDMA_SHIFT;

// RRXDMA mask
#define I2S_RRXDMA_MASK		(1<<I2S_RRXDMA_SHIFT)

// RTXDMA mask
#define I2S_RTXDMA_MASK		(1<<I2S_RRXDMA_SHIFT)

// offset: 0x1F0,   Component Parameter Register 2
typedef enum
{
	I2S_RX_WORDSIZE_0_SHIFT 		= 0,	// [2:0], R, 
											// 0x2 = 20 bit resolution 
											// 0x3 = 24 bit resolution 
											// 0x4 = 32 bit resolution 
											// 0x5 - 0x7 = reserved

	I2S_RX_WORDSIZE_1_SHIFT 		= 3,	// [5:3], R, 
											// 0x0 = 12 bit resolution 
											// 0x1 = 16 bit resolution 

	I2S_COMP_PARAM_2_REVD_SHIFT		= 6,	// [6], N/A, Reserved and read as zero.

	I2S_RX_WORDSIZE_2_SHIFT 		= 7,	// [9:7], R,  
											// 0x2 = 20 bit resolution 
											// 0x3 = 24 bit resolution 
											// 0x4 = 32 bit resolution 
											// 0x5 - 0x7 = reserved	

	I2S_RX_WORDSIZE_3_SHIFT 		= 10,	// [12:10], R,  
											// 0x0 = 12 bit resolution 
											// 0x1 = 16 bit resolution 
	
	I2S_COMP_PARAM_2_REVD1_SHIFT	= 13	// [31:13], N/A, Reserved and read as zero.
}E_I2S_COMP_PARAM_2_SHIFT;

// RX WORDSIZE_0 mask and value
#define I2S_RX_WORDSIZE_0_MASK		(0x7<<I2S_RX_WORDSIZE_0_SHIFT)
#define I2S_RX_WORDSIZE_0_VAL(val)	((val)<<I2S_RX_WORDSIZE_0_SHIFT)

// RX WORDSIZE_1 mask and value
#define I2S_RX_WORDSIZE_1_MASK		(0x7<<I2S_RX_WORDSIZE_1_SHIFT)
#define I2S_RX_WORDSIZE_1_VAL(val)	((val)<<I2S_RX_WORDSIZE_1_SHIFT)

// RX WORDSIZE_2 mask and value
#define I2S_RX_WORDSIZE_2_MASK		(0x7<<I2S_RX_WORDSIZE_2_SHIFT)
#define I2S_RX_WORDSIZE_2_VAL(val)	((val)<<I2S_RX_WORDSIZE_2_SHIFT)

// RX WORDSIZE_3 mask and value
#define I2S_RX_WORDSIZE_3_MASK		(0x7<<I2S_RX_WORDSIZE_3_SHIFT)
#define I2S_RX_WORDSIZE_3_VAL(val)	((val)<<I2S_RX_WORDSIZE_3_SHIFT)

// RX/TX wordsize resolution
typedef enum
{
	I2S_WORDSIZE_12BIT_RES = 0,
	I2S_WORDSIZE_16BIT_RES = 1,
	I2S_WORDSIZE_20BIT_RES = 2,
	I2S_WORDSIZE_24BIT_RES = 3,
	I2S_WORDSIZE_32BIT_RES = 4
}E_I2S_WORDSIZE_RES;

// offset: 0x1F4,  Component Parameter Register 1
typedef enum
{
	I2S_APB_DATA_WIDTH_SHIFT 		= 0,	// [1:0], R, 
											// 0x0 = 8 bits 
											// 0x1 = 16 bits 
											// 0x2 = 32 bits 
											// 0x3 = reserved
											
	I2S_FIFO_DEPTH_GLOBAL_SHIFT 	= 2,	// [3:2], R, 
											// 0x0 if I2S_FIFO_DEPTH_GLOBAL = 2 
											// 0x1 if I2S_FIFO_DEPTH_GLOBAL = 4 
											// 0x2 if I2S_FIFO_DEPTH_GLOBAL = 8 
											// 0x3 if I2S_FIFO_DEPTH_GLOBAL = 16

	I2S_MODE_EN_SHIFT				= 4,	// [4], R

	I2S_TRANSMITTER_BLOCK_SHIFT 	= 5,	// [5], R,  

	I2S_RECEIVER_BLOCK_SHIFT 		= 6,	// [6], R,  
											// 0x0 = FALSE 
											// 0x1 = TRUE

	I2S_RX_CHANNELS_SHIFT			= 7,	// [8:7], R
	I2S_TX_CHANNELS_SHIFT			= 9,	// [10:9], R
											// 0X0 = 1 channel
											// 0X1 = 2 channel
											// 0X2 = 3 channel
											// 0X3 = 4 channel
											
	I2S_COMP_PARAM_1_REVD1_SHIFT	= 11,	// [15:11], Reserved
	
	I2S_TX_WORDSIZE_0_SHIFT			= 16,	// [18:16], R, 
	I2S_TX_WORDSIZE_1_SHIFT			= 19,	// [21:19], R,
	I2S_TX_WORDSIZE_2_SHIFT			= 22,	// [24:22], R,
	I2S_TX_WORDSIZE_3_SHIFT			= 25,	// [27:25], R,
											// 0x0 = 12 bit resolution 
											// 0x1 = 16 bit resolution 
											// 0x2 = 20 bit resolution 
											// 0x3 = 24 bit resolution 
											// 0x4 = 32 bit resolution 
											// 0x5 - 0x7 = reserved
	
	I2S_COMP_PARAM_1_REVD2_SHIFT	= 28	// [31:28], N/A, Reserved and read as zero.
}E_I2S_COMP_PARAM_1_SHIFT;

// [1:0], data width mask and value
#define I2S_APB_DATA_WIDTH_MASK 	(3<<I2S_APB_DATA_WIDTH_SHIFT)
#define I2S_APB_DATA_WIDTH_VAL(val) ((val)<<I2S_APB_DATA_WIDTH_SHIFT)

// RX/TX wordsize resolution
typedef enum
{
	I2S_DW_8BITS 	= 0,
	I2S_DW_16BITS 	= 1,
	I2S_DW_32BITS 	= 2,
	I2S_DW_REVD 	= 3
}E_I2S_DATA_WIDTH;

// [3:2], fifo depth mask and value
#define I2S_FIFO_DEPTH_GLOBAL_SHIFT		(3<<I2S_FIFO_DEPTH_GLOBAL_SHIFT)
#define I2S_FIFO_DEPTH_GLOBAL_VAL(val)	((val)<<I2S_FIFO_DEPTH_GLOBAL_SHIFT)

// fifo width
typedef enum
{
	I2S_FIFO_DEPTH_2 	= 0,
	I2S_FIFO_DEPTH_4 	= 1,
	I2S_FIFO_DEPTH_8 	= 2,
	I2S_FIFO_DEPTH_16 	= 3
}E_I2S_FIFO_DEPTH;

// [4], mode en mask
#define I2S_MODE_EN_MASK			(1<<I2S_MODE_EN_SHIFT)

// [5], transmitter block mask
// [6], receiver block mask
#define I2S_TRANSMITTER_BLOCK_MASK	(1<<I2S_TRANSMITTER_BLOCK_SHIFT)
#define I2S_RECEIVER_BLOCK_MASK		(1<<I2S_RECEIVER_BLOCK_SHIFT)

// [8:7], rx channels mask
// [10:9], tx channels mask
#define I2S_RX_CHANNELS_MASK		(3<<I2S_RX_CHANNELS_SHIFT)
#define I2S_TX_CHANNELS_MASK		(3<<I2S_TX_CHANNELS_SHIFT)

// fifo width
typedef enum
{
	I2S_CH_1 	= 0,
	I2S_CH_2 	= 1,
	I2S_CH_3 	= 2,
	I2S_CH_4 	= 3
}E_I2S_CHANNEL;

// [18:16], tx_wordsize_0 mask and value
#define I2S_TX_WORDSIZE_0_MASK		(0x7<<I2S_TX_WORDSIZE_0_SHIFT)
#define I2S_TX_WORDSIZE_0_VAL(val)	((val)<<I2S_TX_WORDSIZE_0_SHIFT)

// [21:19], tx_wordsize_1 mask and value
#define I2S_TX_WORDSIZE_1_MASK		(0x7<<I2S_TX_WORDSIZE_1_SHIFT)
#define I2S_TX_WORDSIZE_1_VAL(val)	((val)<<I2S_TX_WORDSIZE_1_SHIFT)

// [24:22], tx_wordsize_2 mask and value
#define I2S_TX_WORDSIZE_2_MASK		(0x7<<I2S_TX_WORDSIZE_2_SHIFT)
#define I2S_TX_WORDSIZE_2_VAL(val)	((val)<<I2S_TX_WORDSIZE_2_SHIFT)

// [27:25], tx_wordsize_3 mask and value
#define I2S_TX_WORDSIZE_3_MASK		(0x7<<I2S_TX_WORDSIZE_3_SHIFT)
#define I2S_TX_WORDSIZE_3_VAL(val)	((val)<<I2S_TX_WORDSIZE_3_SHIFT)

// offset: 0x1FC,   I2S Component Type Register
#define I2S_TYPE		0x445701A0

// Component parameter register fields - define the I2S block's configuration.
#define	COMP1_TX_WORDSIZE_3(r)	(((r) & GENMASK(27, 25)) >> 25)
#define	COMP1_TX_WORDSIZE_2(r)	(((r) & GENMASK(24, 22)) >> 22)
#define	COMP1_TX_WORDSIZE_1(r)	(((r) & GENMASK(21, 19)) >> 19)
#define	COMP1_TX_WORDSIZE_0(r)	(((r) & GENMASK(18, 16)) >> 16)
#define	COMP1_TX_CHANNELS(r)	(((r) & GENMASK(10, 9)) >> 9)
#define	COMP1_RX_CHANNELS(r)	(((r) & GENMASK(8, 7)) >> 7)
#define	COMP1_RX_ENABLED(r)	(((r) & BIT(6)) >> 6)
#define	COMP1_TX_ENABLED(r)	(((r) & BIT(5)) >> 5)
#define	COMP1_MODE_EN(r)	(((r) & BIT(4)) >> 4)
#define	COMP1_FIFO_DEPTH_GLOBAL(r)	(((r) & GENMASK(3, 2)) >> 2)
#define	COMP1_APB_DATA_WIDTH(r)	(((r) & GENMASK(1, 0)) >> 0)

#define	COMP2_RX_WORDSIZE_3(r)	(((r) & GENMASK(12, 10)) >> 10)
#define	COMP2_RX_WORDSIZE_2(r)	(((r) & GENMASK(9, 7)) >> 7)
#define	COMP2_RX_WORDSIZE_1(r)	(((r) & GENMASK(5, 3)) >> 3)
#define	COMP2_RX_WORDSIZE_0(r)	(((r) & GENMASK(2, 0)) >> 0)

#endif

