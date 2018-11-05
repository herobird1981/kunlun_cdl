/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_SPI_FLASH_H__
#define __CDL_SPI_FLASH_H__
#include "test_config.h"
#include "cdl_spi.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// manufacture ID
#define MANU_ID_MXIC		0xC2
#define MANU_ID_WINBOND		0xEF
#define MANU_ID_ISSI		0x9D

// Erase commands
#define CMD_ERASE_4K			0x20
#define CMD_ERASE_32K			0x52
#define CMD_ERASE_CHIP			0xc7
#define CMD_ERASE_64K			0xd8

#define CMD_NOR_RSTEN			0x66	// Reset enable
#define	CMD_NOR_RST				0x99	// Reset memory

// Write commands
#define CMD_WRITE_STATUS		0x01
#define CMD_PAGE_PROGRAM		0x02
#define CMD_WRITE_DISABLE		0x04
#define CMD_WRITE_ENABLE		0x06
#define CMD_QUAD_PAGE_PROGRAM	0x32
#define CMD_WRITE_EVCR			0x61

// Read commands
#define CMD_READ_ARRAY_SLOW		0x03
#define CMD_READ_ARRAY_FAST		0x0b
#define CMD_READ_DUAL_OUTPUT_FAST	0x3b
#define CMD_READ_DUAL_IO_FAST		0xbb
#define CMD_READ_QUAD_OUTPUT_FAST	0x6b
#define CMD_READ_QUAD_IO_FAST	0xeb
#define CMD_READ_ID			0x9f
#define CMD_READ_STATUS		0x05
#define CMD_READ_WINBOND_STATUS1		0x35
#define CMD_READ_CONFIG		0x35
#define CMD_FLAG_STATUS		0x70
#define CMD_READ_EVCR			0x65

// register/setting command
#define NOR_CMD_MXIC_RDCR		0x15	// Read configuration register

// ISSI serial nor flash command
#define NOR_CMD_ISSR_RDFR			0x48	// Read Function register 
#define NOR_CMD_ISSR_WRFR			0x42	// Write Function register 


// default timeout value
#define SPI_FLASH_TIME_OUT	(5 * 100)	

// status definition
typedef enum
{
	QSPI_NOR_ST_WIP_SHIFT  	= 0,	// bit [0],The Write in Progress (WIP) bit, a volatile bit, indicates whether the device is busy in program/erase/write 
									// status register progress. 
									// 1: Busy,  0: Ready 
							
	QSPI_NOR_ST_WEL_SHIFT 	= 1,	// bit [1], Write enable latch
								 	// The Write Enable Latch (WEL) bit is a volatile bit that is set to "1" by the WREN instruction. WEL needs to be
									// set to ¡°1¡± before the device can accept program and erase instructions, otherwise the program and erase instructions
									// are ignored. WEL automatically clears to ¡°0¡± when a program or erase operation completes. To ensure that both WIP
									// and WEL are ¡°0¡± and the device is ready for the next program or erase operation, it is recommended that WIP be
									// confirmed to be ¡°0¡± before checking that WEL is also ¡°0¡±. If a program or erase instruction is applied to a protected
									// memory area, the instruction will be ignored and WEL will clear to ¡°0¡±.
	
	QSPI_NOR_ST_BPX_SHIFT 	= 2,	// bit [2, 5], The Block Protect (BP3, BP2, BP1, BP0) bits, non-volatile bits, indicate the protected area.
									

	QSPI_NOR_ST_QE_SHIFT  	= 6,	// bit [6], Quad Enable
									// 0: not quad enable
									// 1: quad enable
	
	QSPI_NOR_ST_SRWD_SHIFT  = 7		// bit [7], Status register write protect
									// 0: status register write enabled
									// 1: status register write disable
}E_QSPI_NOR_ST_SHIFT;

#define QSPI_NOR_ST_WIP_MASK	(1<<QSPI_NOR_ST_WIP_SHIFT)

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
// spi transfer definition
typedef int (*spi_xfer_fn)(E_SPI_PORT, uint8_t *, uint8_t *, uint32_t, E_SPI_XFER_FLAG);

// spi flash structure
struct spi_flash {
	E_SPI_PORT port;
	spi_xfer_fn xfer;

	uint8_t manu_id;				// manufacture vendor	
									//	= QSPI_MANU_ID_MXIC
									//	= QSPI_MANU_ID_WINBOND
									//	= QSPI_MANU_ID_ISSI
	uint8_t revd0[3];

	uint16_t page_size;		// page size

	uint16_t sec_size;		// sector size

	uint32_t blk_size;		// block size

	uint16_t pages_per_sec;	// the page number per sector

	uint16_t secs_per_blk;	// the sector number per block

	uint32_t blks_per_chip;	// the block number per chip	
	
	uint32_t cap;			// flash memory density(byte aligned)
	
	int dummy_byte; 		// dummy bytes after read command

	// read command	
	uint8_t sio_rd_cmd;

	// write command
	uint8_t sio_pp_cmd;	

	// erase command
	uint8_t erase_cmd;

	// timeout value(2 ms)
	uint32_t time_out;
};

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
void sf_set_controller(E_SPI_PORT port, spi_xfer_fn xfer);

int sf_probe(void);

int sf_erase_sec(uint32_t addr, uint32_t len);

int sf_read(uint32_t addr, uint8_t *p_in, uint32_t len);

int sf_write(uint32_t addr, uint8_t *p_out, uint32_t len);

#endif

