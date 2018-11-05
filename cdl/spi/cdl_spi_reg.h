/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_SPI_REG_H__
#define __CDL_SPI_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define SPI_PORT_SIZE	0x20

// register layout 
#define SPI_SSPCR0_OFST			0x000	// Read/Write, 16 bit, Control register 0
#define SPI_SSPCR1_OFST			0x004	// Read/Write, 4 bit,  Control register 1
#define SPI_SSPDR_OFST			0x008	// Read/Write, 16 bit, Receive FIFO (read) and transmit FIFO data register (write)
#define SPI_SSPSR_OFST			0x00C	// Read,         5 bit, Status register
#define SPI_SSPCPSR_OFST		0x010	// Read/Write, 8 bit, Clock prescale register
#define SPI_SSPIMSC_OFST 		0x014	// Read/Write, 4 bit, Interrupt mask set and clear register
#define SPI_SSPRIS_OFST			0x018	// Read,         4 bit, Raw interrupt status register
#define SPI_SSPMIS_OFST			0x01C	// Read,         4 bit, Masked interrupt status register
#define SPI_SSPICR_OFST			0x020	// Read,         4 bit, Interrupt clear register
#define SPI_SSPDMACR_OFST 		0x024	// Read/Write, 2 bit, DMA control register

#define SPI_REVD0_OFST			0x028	// reserved [0x28, 0x7C]
#define SPI_TEST_OFST			0x080	// reserved for test[0x80, 0x8C]
#define SPI_REVD1_OFST			0x090	// reserved[0x90, 0x0FCC]
#define SPI_REVD2_OFST			0xFD0	// reserved for future expansion[0xFD0, 0xFDC]

// Peripheral0/1/2/3
#define SPI_PERIPHID0_OFST		0xFE0	// Read,         8 bit, Peripheral ID register bits 7:0
#define SPI_PERIPHID1_OFST		0xFE4	// Read,         8 bit, Peripheral ID register bits 15:8
#define SPI_PERIPHID2_OFST		0xFE8	// Read,         8 bit, Peripheral ID register bits 23:16
#define SPI_PERIPHID3_OFST		0xFEC	// Read,         8 bit, Peripheral ID register bits 31:24

// PrimeCell0/1/2/3
#define SPI_PCELLID0_OFST		0xFF0	// Read,         8 bit, PrimeCell ID register bits 7:0
#define SPI_PCELLID1_OFST		0xFF4	// Read,         8 bit, PrimeCell ID register bits 15:8
#define SPI_PCELLID2_OFST		0xFF8	// Read,         8 bit, PrimeCell ID register bits 23:16
#define SPI_PCELLID3_OFST		0xFFC	// Read,         8 bit, PrimeCell ID register bits 31:24

// offset: 0x000, Control register 0
typedef enum
{
	SPI_DSS_SHIFT 			= 0,	// [3:0], Read/Write, Data Size Select
									// 0000 = Reserved, undefined operation
									// 0001 = Reserved, undefined operation
									// 0010 = Reserved, undefined operation
									// 0011 = 4-bit data
									// 0100 = 5-bit data
									// 0101 = 6-bit data
									// 0110 = 7-bit data
									// 0111 = 8-bit data
									// 1000 = 9-bit data
									// 1001 = 10-bit data
									// 1010 = 11-bit data
									// 1011 = 12-bit data
									// 1100 = 13-bit data
									// 1101 = 14-bit data
									// 1110 = 15-bit data
									// 1111 = 16-bit data

	SPI_FRF_SHIFT 			= 4,	// [5:4], Read/Write, Frame format:
									// 00 = Motorola SPI frame format
									// 01 = TI synchronous serial fram format
									// 10 = National Microwire frame format
									// 11 = Reserved, undefined operation
										
	SPI_SPO_SHIFT 			= 6,	// [6], Read/Write, SSPCLKOUT polarity(applicabe to Motorola SPI frame format only)
	
	SPI_SPH_SHIFT 			= 7,	// [7], Read/Write, SSPCLKOUT phase(applicabe to Motorola SPI frame format only)
										
	SPI_SCR_SHIFT			= 8		// [15:8], Read/Write, Serail clock rate. The value SCR is used to generate the transmit and receive bit rate of the PrimeCell SSP.
									// Fsspclk /(CPSDVR*(1+SCR))
}E_SPI_CR0_SHIFT;

// [3:0], Data Size Select
#define SPI_DSS_MASK		(0xF<<SPI_DSS_SHIFT)
#define SPI_DSS_VAL(val)	((val)<<SPI_DSS_SHIFT)

typedef enum
{
	SPI_DSS_4	= 3,	//  4-bit data 
	SPI_DSS_5	= 4,	//  5-bit data 
	SPI_DSS_6	= 5,	//  6-bit data
	SPI_DSS_7	= 6,	//  7-bit data 
	SPI_DSS_8	= 7,	//  8-bit data 
	SPI_DSS_9	= 8,	//  9-bit data 
	SPI_DSS_10	= 9,	//  10-bit data
	SPI_DSS_11	= 0x0A,	//  11-bit data 	
	SPI_DSS_12	= 0x0B,	//  12-bit data 
	SPI_DSS_13	= 0x0C,	//  13-bit data 
	SPI_DSS_14	= 0x0D,	//  14-bit data
	SPI_DSS_15	= 0x0E,	//  15-bit data 
	SPI_DSS_16	= 0x0F	//  16-bit data 	
} E_SPI_DSS;

// bit[5:4], frame format
#define SPI_FRF_MASK		(0x3<<SPI_FRF_SHIFT)
#define SPI_FRF_VAL(val)	((val)<<SPI_FRF_SHIFT)

typedef enum
{
	SPI_FRF_MOTORROLA	= 0, // Motorola SPI frame format 
	SPI_FRF_TI			= 1, // TI synchronous serial frame format  
	SPI_FRF_NATIONAL	= 2, // National Microwire frame format 
	SPI_FRF_REVD		= 3	 // Reserved,  undefined operation 
} E_SPI_FRF;

// bit[6], SSPCLKOUT polarity
#define SPI_SPO_MASK		(1<<SPI_SPO_SHIFT)
#define SPI_SPO_VAL(val)	((val)<<SPI_SPO_SHIFT)

typedef enum
{
	SPI_SPO_0 = 0,
	SPI_SPO_1 = 1
} E_SPI_SPO;

// bit[7], SSPCLKOUT phase
#define SPI_SPH_MASK		(1<<SPI_SPH_SHIFT)
#define SPI_SPH_VAL(val)	((val)<<SPI_SPH_SHIFT)

typedef enum
{
	SPI_SPH_0 = 0,
	SPI_SPH_1 = 1
} E_SPI_SPH;

// bit [15:8], serial clock rate
#define SPI_SCR_MASK		(0xFF<<SPI_SCR_SHIFT)
#define SPI_SCR_VAL(val)	((val)<<SPI_SCR_SHIFT)

// offset: 0x004, Control register 1
typedef enum
{
	SPI_LBM_SHIFT				= 0,	// [0], Loop back mode
										// 0 = Normal serial port operation enabled
										// 1 = Output of transmit serial shifter is connected to input of receive serail shifter internall.
										
	SPI_SSE_SHIFT 				= 1,	// [1], Read/write, Synchronous serial port enable
										// 0 = SSP operation disabled
										// 1 = SSP operation enabled	

	SPI_MS_SHIFT 				= 2,	// [2], Read/write, Master or slave select. This bit can be modified only when the PrimeCell SSP is disabled(SSE=0)
										// 0 = device cofigured as master(default)
										// 1 = device configured as slave

	SPI_SOD_SHIFT 				= 3,	// [3], Read/Write, Slave-mode output disable.
										// 0: SSP can drive the SSPTXD output in slave mode
										// 1: SSP must not drive the SSPTXD output in slave mode
										
	SPI_CR1_REVD1_SHIFT			= 4		// [15:4], Reserved, read undefined, must be written as 0
}E_SPI_CR1_SHIFT;

// [0], Loop back mode mask and value
#define SPI_LBM_MASK		(1<<SPI_LBM_SHIFT)
#define SPI_LBM_VAL(val)	((val)<<SPI_LBM_SHIFT)

typedef enum
{
	SPI_NORMAL_MODE 	= 0,
	SPI_LOOP_BACK_MODE 	= 1
}E_SPI_LBM;

// [1], Synchronous serial port enable mask and value
#define SPI_SSE_MASK		(1<<SPI_SSE_SHIFT)
#define SPI_SSE_VAL(val)	((val)<<SPI_SSE_SHIFT)

typedef enum
{
	SPI_ASYNC_MODE 	= 0,
	SPI_SYNC_MODE 	= 1
}E_SPI_SSE;

// [2], Master or slave select mask and value
#define SPI_MS_MASK		(1<<SPI_MS_SHIFT)
#define SPI_MS_VAL(val)	((val)<<SPI_MS_SHIFT)

typedef enum
{
	SPI_MASTER 	= 0,
	SPI_SLAVE 	= 1
}E_SPI_MS;

// [3], Slave-mode output disable mask and value
#define SPI_SOD_MASK		(1<<SPI_SOD_SHIFT)
#define SPI_SOD_VAL(val)	((val)<<SPI_SOD_SHIFT)

typedef enum
{
	SPI_SOD_EN 	= 0,
	SPI_SOD_DIS = 1
}E_SPI_SOD;

// offset: 0x00C, Status register
typedef enum
{
	SPI_ST_TFE_SHIFT 		= 0,	// [0], Read, Transmit FIFO empty flag(read-only)
									// 0 = Transmit FIFO is not empty
									// 1 = Transmit FIFO is empty
									
	SPI_ST_TNF_SHIFT 		= 1,	// [1], Read, Transmit FIFO not full(read-only)
									// 0 = Transmit FIFO is full
									// 1 = Transmit FIFO is not full

	SPI_ST_RNE_SHIFT 		= 2,	// [2], Read, Receive FIFO not empty flag(read-only)
									// 0 = Receive FIFO is empty
									// 1 = Receive FIFO is not empty
									
	SPI_ST_RFF_SHIFT 		= 3,	// [3], Receive FIFO full(read-only)
									// 0 = Receive FIFO is not full
									// 1 = Receive FIFO is full
									
	SPI_ST_BSY_SHIFT 		= 4,	// [4], Read, PrimeCell SSP busy flag(read-only)
									// 0 = SSP is idle
									// 1 = SSP is currently transmitting and/or receiving a frame or the transmit FIFO is not empty.
										
	SPI_SR_REVD1_SHIFT		= 5		// [15:5], Reserved, read undefined, must be written as 0
}E_SPI_SR_SHIFT;

// status mask
#define SPI_ST_MASK			0x1F
#define SPI_TFE_MASK		(1<<SPI_ST_TFE_SHIFT)
#define SPI_TNF_MASK		(1<<SPI_ST_TNF_SHIFT)
#define SPI_RNE_MASK		(1<<SPI_ST_RNE_SHIFT)
#define SPI_RFF_MASK		(1<<SPI_ST_RFF_SHIFT)
#define SPI_BSY_MASK		(1<<SPI_ST_BSY_SHIFT)

// offset: 0x010, Clock prescale register
typedef enum
{									
	SPI_CPSDVSR_SHIFT 		= 0,	// [7:0], Read, Clock prescale divisor. Must be an even number from 2 to 254, depending on 
									// the frequency of SSPCLK. The least significent bit always returns zero on read.
									// 0 = SSP is idle
									// 1 = SSP is currently transmitting and/or receiving a frame or the transmit FIFO is not empty.
										
	SPI_CPSR_REVD1_SHIFT	= 8		// [15:8], Reserved, read undefined, must be written as 0
}E_SPI_CPSR_SHIFT;

// [7:0], mask and value
#define SPI_CPSDVSR_MASK		(0xFF<<SPI_CPSDVSR_SHIFT)
#define SPI_CPSDVSR_VAL(val)	((val)<<SPI_CPSDVSR_SHIFT)

// offset: 0x014, Interrupt mask set and clear register
// offset: 0x018, Raw interrupt status register
// offset: 0x01C, Masked interrupt status register
typedef enum
{
	SPI_IMSC_RORIM_SHIFT 	= 0,	// [0], Read/Write, Receive overrun interrupt mask
									// 0 = Rx FIFO written to full condition interrupt is masked
									// 1 = Rx FIFO written to full condition interrupt is not masked

	SPI_IMSC_RTIM_SHIFT 	= 1,	// [1], Read/Write, Receive timeout interrupt mask
									// 0 = Rx FIFO not empty and no read prior to timeout period interrupt is masked
									// 1 = Rx FIFO not empty and no read prior to timeout period interrupt is not masked
									
	SPI_IMSC_RXIM_SHIFT 	= 2,	// [2], Read/Write, Receive FIFO interrupt mask
									// 0 = Rx FIFO half full or less condition interrupt is masked
									// 1 = Rx FIFO half full or less condition interrupt is not masked
									
	SPI_IMSC_TXIM_SHIFT 	= 3,	// [3], Read/Write, Transmit FIFO interrupt mask
									// 0 = Tx FIFO half empty or less condition interrupt is masked
									// 1 = Tx FIFO half empty or less condition interrupt is not masked
										
	SPI_IMSC_REVD1_SHIFT	= 4		// [15:4], Reserved, read undefined, must be written as 0
}E_SPI_IMSC_SHIFT;

// mask
#define SPI_INT_ROR_MASK	(1<<SPI_IMSC_RORIM_SHIFT)
#define SPI_INT_RT_MASK		(1<<SPI_IMSC_RTIM_SHIFT)
#define SPI_INT_RX_MASK		(1<<SPI_IMSC_RXIM_SHIFT)
#define SPI_INT_TX_MASK		(1<<SPI_IMSC_TXIM_SHIFT)

// offset: 0x020, Interrupt clear register
typedef enum
{
	SPI_RORIC_SHIFT 	= 0,	// [0], Write, Clear the SSPRORINTR interrupt
									
	SPI_RTIC_SHIFT 		= 1,	// [1], Write, Clear the SSPRTINTR interrupt
										
	SPI_ICR_REVD1_SHIFT	= 2		// [15:2], Reserved, read undefined, do not modify
}E_SPI_ICR_SHIFT;

// mask
#define SPI_RORIC_MASK	(1<<SPI_RORIC_SHIFT)
#define SPI_RTIC_MASK	(1<<SPI_RTIC_SHIFT)

// offset: 0x024, DMA control register
typedef enum
{
	SPI_RXDMAE_SHIFT 		= 0,	// [0], Write/Read, If this bit is set to 1, DMA for the receive FIFO is enabled
									
	SPI_TXDMAE_SHIFT 		= 1,	// [1], Write/Read, If this bit is set to 1, DMA for the transmit FIFO is enabled
										
	SPI_DMACR_REVD1_SHIFT	= 2		// [15:2], Reserved, read undefined, do not modify
}E_SPI_DMACR_SHIFT;

// mask and value
#define SPI_RXDMAE_MASK		(1<<SPI_RXDMAE_SHIFT)
#define SPI_RXDMAE_VAL(val)	((val)<<SPI_RXDMAE_SHIFT)

#define SPI_TXDMAE_MASK		(1<<SPI_TXDMAE_SHIFT)
#define SPI_TXDMAE_VAL(val)	((val)<<SPI_TXDMAE_SHIFT)

typedef enum
{
	SPI_DMA_DIS	= 0,
	SPI_DMA_EN = 1
}E_SPI_DMACR;

// 0xFE0 to 0xFEC, Peripheral identification registers, TimerPeriphID0-3
#define SPI_PERIPHID_MASK	0xFF

#define SPI_PERIPHID0 		0x22
#define SPI_PERIPHID1 		0x10
#define SPI_PERIPHID2 		0x04
#define SPI_PERIPHID3 		0x00

//  0xFF0 to 0xFFC, PrimeCell identification registers, TimerPCellID0-3
#define SPI_PCELLID_MASK	0xFF

#define SPI_PCELLID0		0x0D
#define SPI_PCELLID1		0xF0
#define SPI_PCELLID2		0x05
#define SPI_PCELLID3		0xB1

#endif

