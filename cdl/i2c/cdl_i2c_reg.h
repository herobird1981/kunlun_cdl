/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_I2C_REG_H__
#define __CDL_I2C_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// register layout
#define I2C_CON_OFST						0x00 	// 20 bits, R/W, I2C Control
#define I2C_TAR_OFST						0x04 	// 12,13,14 or 16bits, R/W, I2C Target Address
#define I2C_SAR_OFST						0x08 	// 10 bits, R/W, I2C Slave Address
#define I2C_HS_MADDR_OFST					0x0C 	// 3 bits, R/W, I2C HS Master Mode Code Address
#define I2C_DATA_CMD_OFST					0x10 	// Refer to description, R/W, I2C Rx/Tx Data Buffer and Command

#define I2C_SS_SCL_HCNT_OFST				0x14 	// 16 bits, R/W, Standard speed I2C Clock SCL High Count
#define I2C_SS_SCL_LCNT_OFST				0x18 	// 16 bits, R/W, Standard speed I2C Clock SCL Low Count
#define I2C_FS_SCL_HCNT_OFST				0x1C 	// 16 bits, R/W, Fast Mode and Fast Mode Plus I2C Clock SCL High Count
#define I2C_FS_SCL_LCNT_OFST				0x20 	// 16 bits, R/W, Fast Mode and Fast Mode Plus I2C Clock SCL Low Count
#define I2C_HS_SCL_HCNT_OFST				0x24 	// 16 bits, R/W, High speed I2C Clock SCL High Count
#define I2C_HS_SCL_LCNT_OFST				0x28 	// 16 bits, R/W, High speed I2C Clock SCL Low Count

#define I2C_INTR_STAT_OFST					0x2C 	// 15 bits, R,     I2C Interrupt Status
#define I2C_INTR_MASK_OFST					0x30 	// 15 bits, R/W, I2C Interrupt Mask
#define I2C_RAW_INTR_STAT_OFST				0x34 	// 15 bits, R,     I2C Raw Interrupt Status

#define I2C_RX_TL_OFST						0x38 	// 8 bits, R/W, I2C Receive FIFO Threshold
#define I2C_TX_TL_OFST						0x3C 	// 8 bits, R/W, I2C Transmit FIFO Threshold

#define I2C_CLR_INTR_OFST					0x40 	// 1 bit, R,      Clear Combined and Individual Interrupts
#define I2C_CLR_RX_UNDER_OFST				0x44 	// 1 bit, R,      Clear RX_UNDER Interrupt
#define I2C_CLR_RX_OVER_OFST				0x48 	// 1 bit, R,      Clear RX_OVER Interrupt
#define I2C_CLR_TX_OVER_OFST				0x4C 	// 1 bit, R,      Clear TX_OVER Interrupt
#define I2C_CLR_RD_REQ_OFST					0x50 	// 1 bit, R,      Clear RD_REQ Interrup
#define I2C_CLR_TX_ABRT_OFST				0x54 	// 1 bit, R,      Clear TX_ABRT Interrupt
#define I2C_CLR_RX_DONE_OFST				0x58 	// 1 bit, R,      Clear RX_DONE Interrupt
#define I2C_CLR_ACTIVITY_OFST				0x5C 	// 1 bit, R,      Clear ACTIVITY Interrupt
#define I2C_CLR_STOP_DET_OFST				0x60 	// 1 bit, R,      Clear STOP_DET Interrupt
#define I2C_CLR_START_DET_OFST				0x64 	// 1 bit, R,      Clear START_DET Interrupt
#define I2C_CLR_GEN_CALL_OFST				0x68 	// 1 bit, R,      Clear GEN_CALL Interrupt
#define I2C_ENABLE_OFST						0x6C 	// Refer to description, R/W, I2C Enable

#define I2C_STATUS_OFST						0x70 	// Refer to description, R, I2C Status register
#define I2C_TXFLR_OFST						0x74 	// TX_ABW+1, R, Transmit FIFO Level Register
#define I2C_RXFLR_OFST						0x78 	// RX_ABW+1, R, Receive FIFO Level Register
#define I2C_SDA_HOLD_OFST					0x7C 	// 24 bit, R/W, SDA hold time length register
#define I2C_TX_ABRT_SOURCE_OFST				0x80 	// 32 bit, R, I2C Transmit Abort Status Register
#define I2C_SLV_DATA_NACK_ONLY_OFST			0x84 	// 1 bit, R/W, Generate SLV_DATA_NACK Register
#define I2C_DMA_CR_OFST						0x88 	// 2 bit, R/W, DMA Control Register for transmit and receive handshaking interface

#define I2C_DMA_TDLR_OFST					0x8C 	// TX_ABW, R/W, DMA Transmit Data Level
#define I2C_DMA_RDLR_OFST					0x90 	// RX_ABW, R/W, DMA Receive Data Leve
#define I2C_SDA_SETUP_OFST					0x94 	// 8 bit, R/W, I2C SDA Setup Register
#define I2C_ACK_GENERAL_CALL_OFST			0x98 	// 1 bit, R/W, I2C ACK General Call Register
#define I2C_ENABLE_STATUS_OFST				0x9C 	// 3 bit, R,     I2C Enable Status Register
#define I2C_FS_SPKLEN_OFST					0xA0 	// 8 bit, R/W, ISS and FS spike suppression limit
#define I2C_HS_SPKLEN_OFST					0xA4 	// 8 bit, R/W, HS spike suppression limit
#define I2C_CLR_RESTART_DET_OFST			0xA8 	// 1 bit, R,  Clear RESTART_DET Interrupt
#define I2C_COMP_PARAM_1_OFST				0xF4 	// 32 bit, R,  Component Parameter Register
#define I2C_COMP_VERSION_OFST				0xF8 	// 32 bit, R,  Component Version ID
#define I2C_COMP_TYPE_OFST					0xFC 	// 32 bit, R,  DesignWare Component Type Register

#define I2C_SCL_STUCK_AT_LOW_TIMEOUT_OFST	0xAC 	// 32 bit, R/W, I2C SCL stuck at low timeout register
#define I2C_SDA_STUCK_AT_LOW_TIMEOUT_OFST	0xB0 	// 32 bit, R/W, I2C SDA Stuck at Low Timeout
#define I2C_CLR_SCL_STUCK_DET_OFST			0xB4 	// 1 bit, R, Clear SCL Stuck at Low Detect Interrupt Registe
#define I2C_DEVICE_ID_OFST					0xB8 	// 24 bit, R,  I2C Device ID
#define I2C_UFM_SCL_HCNT_OFST				0x14 	// 16 bit, R/W, Ultra-Fast mode I2C Clock High Count Register
#define I2C_UFM_SCL_LCNT_OFST				0x18 	// 16 bit, R/W, Ultra-Fast mode I2C Clock Low Count Register
#define I2C_UFM_TBUF_CNT_OFST				0x1C 	// 16 bit, R/W, Ultra-Fast mode TBuf Idle Count Register
#define I2C_UFM_SPKLEN_OFST					0xA0 	// 8 bit, R/W, I2C Ultra-Fast mode Spike suppression Register
#define I2C_SMBUS_CLOCK_LOW_SEXT_OFST		0xBC 	// 32 bit, R/W, SMBUS Slave Clock Extend Timeout Register
#define I2C_SMBUS_CLOCK_LOW_MEXT_OFST		0xC0 	// 32 bit, R/W, SMBUS Master extend clock Timeout Register
#define I2C_SMBUS_THIGH_MAX_IDLE_COUNT_OFST	0xC4 	// 16 bit, R/W, SMBus Thigh MAX Bus-Idle count Register
#define I2C_SMBUS_INTR_STAT_OFST			0xC8 	// 32 bit, R,  2C SMBUS Interrupt Status Register
#define I2C_SMBUS_INTR_MASK_OFST			0xCC 	// 32 bit, R/W, I2C Interrupt Mask Register
#define I2C_IC_SMBUS_INTR_RAW_STATUS_OFST	0xD0 	// 32 bit, R,  I2C SMBUS Raw Interrupt Status Register
#define I2C_CLR_SMBUS_INTR_OFST				0xD4 	// 32 bit, W,  Clear SMBUS Interrupt Register
#define I2C_OPTIONAL_SAR_OFST				0xD8 	// 7 bit, R/W, I2C Optional Slave Address Register
#define I2C_SMBUS_UDID_LSB_OFST				0xDC 	// 32 bit, R/W, SMBUS ARP UDID LSB Register

// offset: 0x00, I2C Control
typedef enum
{
	I2C_MASTER_MODE_SHIFT								= 0,	// [18], R/W, This bit controls whether the DW_apb_i2c master is enabled. 
																// 0: master disabled 
																// 1: master enabled
																// Reset value: IC_MASTER_MODE configuration parameter
																// NOTE: Software should ensure that if this bit is written with 
																// ��1,�� then bit 6 should also be written with a ��1��
	
	I2C_SPEED_SHIFT										= 1,	// [2:1], R/W, These bits control at which speed the DW_apb_i2c operates. 
																// Hardware protects against illegal values being programmed 
																// by software. register These bits must be programmed 
																// appropriately for slave mode also, as it is used to capture 
																// correct value of spike filter as per the speed mode.
																// This register should be programmed only with a value in the 
																// range of 1 to IC_MAX_SPEED_MODE; otherwise, hardware 
																// updates this register with the value of 
																// IC_MAX_SPEED_MODE.
																// �� 1: standard mode (0 to 100 Kb/s)  
																// �� 2: fast mode (��  400 Kb/s) or fast mode plus (��  1000 Kb/s)
																// �� 3: high speed mode (�� 3.4 Mb/s)
																// NOTE: This field is not applicable in Ultra-Fast speed mode (IC_ULTRA_FAST_MODE=1)
																// Reset value: IC_MAX_SPEED_MODE configuration

	I2C_10BITADDR_SLAVE_SHIFT							= 3,	// [3],   R/W, When acting as a slave, this bit controls whether the 
																// DW_apb_i2c responds to 7- or 10-bit addresses.
																// 0: 7-bit addressing. The DW_apb_i2c ignores transactions 
																// that involve 10-bit addressing; for 7-bit addressing, only the 
																// lower 7 bits of the IC_SAR register are compared.
																// 1: 10-bit addressing. The DW_apb_i2c responds to only 
																// 10-bit addressing transfers that match the full 10 bits of the 
																// IC_SAR register.
																// Reset value: IC_10BITADDR_SLAVE configuration parameter
	
	I2C_10BITADDR_MASTER_SHIFT							= 4,	// [4],   R/W, If the I2C_DYNAMIC_TAR_UPDATE configuration parameter 
																// is set to ��No�� (0), this bit is named IC_10BITADDR_MASTER 
																// and controls whether the DW_apb_i2c starts its transfers in 7- 
																// or 10-bit addressing mode when acting as a master.
																// If I2C_DYNAMIC_TAR_UPDATE is set to ��Yes�� (1), the 
																// function of this bit is handled by bit 12 of IC_TAR register, and 
																// becomes a read-only copy called 
																// IC_10BITADDR_MASTER_rd_only.
																// 0: 7-bit addressing 
																// 1: 10-bit addressing
																// Dependencies: If I2C_DYNAMIC_TAR_UPDATE = 1, then 
																// this bit is read-only. If I2C_DYNAMIC_TAR_UPDATE = 0, 
																// then this bit can be read or write.
																// Reset value: IC_10BITADDR_MASTER configuration parameter
	
	I2C_RESTART_EN_SHIFT								= 5,	// [5],   R/W, Determines whether RESTART conditions may be sent when 
																// acting as a master. Some older slaves do not support 
																// handling RESTART conditions; however, RESTART 
																// conditions are used in several DW_apb_i2c operations.
																// 0: disable
																// 1: enable
																// When the RESTART is disabled, the DW_apb_i2c master is 
																// incapable of performing the following functions:
																// �� Sending a START BYTE
																// �� Performing any high-speed mode operation
																// �� Performing direction changes in combined format mode
																// �� Performing a read operation with a 10-bit address
																// By replacing RESTART condition followed by a STOP and a 
																// subsequent START condition, split operations are broken 
																// down into multiple DW_apb_i2c transfers. If the above 
																// operations are performed, it will result in setting bit 6 
																// (TX_ABRT) of the IC_RAW_INTR_STAT register.
																// Reset value: IC_RESTART_EN configuration parameter
	
	I2C_SLAVE_DISABLE_SHIFT								= 6,	// [6],   R/W, This bit controls whether I2C has its slave disabled, which 
																// means once the presetn signal is applied, then this bit takes 
																// on the value of the configuration parameter 
																// IC_SLAVE_DISABLE. You have the choice of having the 
																// slave enabled or disabled after reset is applied, which means 
																// software does not have to configure the slave. By default, the 
																// slave is always enabled (in reset state as well). If you need to 
																// disable it after reset, set this bit to 1.
																// If this bit is set (slave is disabled), DW_apb_i2c functions only 
																// as a master and does not perform any action that requires a slave.
																// 0: slave is enabled 
																// 1: slave is disabled
																// Reset value: IC_SLAVE_DISABLE configuration parameter
																// NOTE: Software should ensure that if this bit is written with 
																//��0,�� then bit 0 should also be written with a ��0��.
	
	I2C_STOP_DET_IFADDRESSED_SHIFT						= 7,	// [7],  R/W, In slave mode:
																// 1��b1 �C issues the STOP_DET interrupt only when it is addressed.
																// 1��b0 �C issues the STOP_DET irrespective of whether it's addressed or not.
																// Dependencies: This register bit value is  applicable in the 
																// slave mode only (MASTER_MODE = 1��b0)
																// Reset value: 1��b0
																// NOTE: During a general call address, this slave does not 
																// issue the STOP_DET interrupt if 
																// STOP_DET_IF_ADDRESSED = 1��b1, even if the slave 
																// responds to the general call address by generating ACK.
																// The STOP_DET interrupt is generated only when the 
																// transmitted address matches the slave address (SAR)
	
	I2C_TX_EMPTY_CTRL_SHIFT								= 8,	// [8],  R/W, This bit controls the generation of the TX_EMPTY interrupt, 
																// as described in the IC_RAW_INTR_STAT register.
																// Reset value: 0x0
	
	I2C_RX_FIFO_FULL_HLD_CTRL_SHIFT						= 9,	// [9],   R    ,   This bit controls whether DW_apb_i2c should hold the bus 
																// when the Rx FIFO is physically full to its 
																// RX_BUFFER_DEPTH, as described in the 
																// IC_RX_FULL_HLD_BUS_EN parameter.
																// Dependencies: This register bit value is applicable only 
																// when the IC_RX_FULL_HLD_BUS_EN configuration 
																// parameter is set to 1. If IC_RX_FULL_HLD_BUS_EN  = 0, 
																// then this bit is read-only. If IC_RX_FULL_HLD_BUS_EN = 1
																// then this bit can be read or write. 
																// This field is not applicable in Ultra-Fast speed mode (IC_ULTRA_FAST_MODE=1)
																// Reset value: 0x0
	
	I2C_STOP_DET_IF_MASTER_ACTIVE_SHIFT					= 10,	// [18], R/W, In Master mode
																// �� 1��b1: Issues the STOP_DET interrupt only when the master is active
																// �� 1��b0: Issues the STOP_DET irrespective of whether the master is active
																// Reset value: 1��b0
																// Dependencies: This Register bit value is applicable only 
																// when IC_STOP_DET_IF_MASTER_ACTIVE=1.
																// This field is not applicable in Ultra-Fast speed mode (IC_ULTRA_FAST_MODE=1)
	
	I2C_BUS_CLEAR_FEATURE_CTRL_SHIFT					= 11,	// [11], R/W, n Master Mode:
																// �� 1'b1: Bus Clear Feature is enabled
																// �� 1'b0: Bus Clear Feature is disabled
																// In Slave Mode, this register bit is not applicable.
																// Reset Value: 1'b0
																// Dependencies: This register bit value is applicable only 
																// when IC_BUS_CLEAR_FEATURE=1.
																// This field is not applicable in Ultra-Fast speed mode 
																// (IC_ULTRA_FAST_MODE=1)
	
	I2C_CON_REVD0_SHIFT									= 12,	// [15:12], R/W, Reserved
	
	I2C_OPTIONAL_SAR_CTRL_SHIFT							= 16,	// [16], R/W, Enables the usage of IC_OPTIONAL_SAR register.
																// If IC_OPTIONAL_SAR =1, IC_OPTIONAL_SAR value is 
																// used as additional slave address. User must program a valid 
																// address in IC_OPTIONAL_SAR before writing 1 to this field.
																// If IC_OPTIONAL_SAR =0, IC_OPTIONAL_SAR value is not 
																// used as additional slave address. In this mode only one I2C 
																// slave address is used.
																// Dependencies: This register bit is valid only if configuration 
																// parameter IC_OPTIONAL_SAR is set to 1
																// Reset Value: IC_OPTIONAL_SAR_DEFAULT
	
	I2C_SMBUS_SLAVE_QUICK_CMD_EN_SHIFT					= 17,	// [17], R/W, If this bit is set to 1, DW_apb_i2c slave only receives Quick 
																// commands in SMBus Mode.
																// If this bit is set to 0, DW_apb_i2c slave receives all bus 
																// protocols but not Quick commands.
																// This bit is applicable only in slave mode. 
																// Dependencies: This register bit is applicable only when the 
																// IC_SMBUS configuration parameter is set to 1.
																// Reset Value: 0x0
		
	I2C_SMBUS_ARP_EN_SHIFT								= 18,	// [18], R/W, This bit controls whether DW_apb_i2c should enable Address 
																// Resolution Logic in SMBus Mode. The Slave mode will 
																// decode the Address Resolution Protocol commands and 
																// respond to it. The DW_apb_i2c slave also includes the 
																// generation/validity of PEC byte for Address Resolution 
																// Protocol commands.
																// This bit is applicable only in Slave mode.
																// Dependencies: This register bit is applicable only when the 
																// IC_SMBUS_ARP configuration parameter is set to 1.
																// Reset Value: 0x0
	
	I2C_CON_SMBUS_PERSISTANT_SLV_ADDR_EN_SHIFT			= 19,	// [19], -, This bit controls to enable DW_apb_i2c slave as persistent or 
																// non-persistent slave. 
																// If the slave is non-PSA then DW_apb_i2c slave device clears 
																// the Address valid flag for both General and Directed Reset 
																// ARP command else the address valid flag will always set to 1.
																// Dependencies: This register bit is applicable only when the 
																// IC_SMBUS_ARP configuration parameter is set to 1.
																// This bit is applicable only in Slave mode.
																// Reset Value: IC_PERSISTANT_SLV_ADDR_DEFAULT
	
	I2C_CON_REVD1_SHIFT									= 20	// [31:20], N/A, Reserved
}E_I2C_CON_SHIFT;

// [0], I2C master mode
#define I2C_MASTER_MODE_MASK		(1<<I2C_MASTER_MODE_SHIFT)
#define I2C_MASTER_MODE_VAL(val)	((val)<<I2C_MASTER_MODE_SHIFT)

typedef enum
{
	I2C_SLAVE_MODE	= 0,
	I2C_MASTER_MODE	= 1
}E_I2C_MASTER_MODE;

// [2:1], I2C speed
#define I2C_SPEED_MASK		(3<<I2C_SPEED_SHIFT)
#define I2C_SPEED_VAL(val)	((val)<<I2C_SPEED_SHIFT)

typedef enum
{
	I2C_SPEED_STANDARD_MODE	= 1,	// standard mode (0 to 100 Kb/s)  
	I2C_SPEED_FAST_MODE		= 2,	// fast mode (��  400 Kb/s) or fast mode plus (��  1000 Kb/s)
	I2C_SPEED_HIGH_MODE		= 3		// high speed mode: HS (400pf) 1.666Mb/s, HS (100pf) ( 3.333 Mb/s)
}E_I2C_SPEED_MODE;

typedef enum
{
	I2C_SPEED_STANDARD	= 100000,	// standard mode (0 to 100 Kb/s)  
	I2C_SPEED_FAST		= 400000,	// fast mode (��  400 Kb/s) or fast mode plus (��  1000 Kb/s)
	I2C_SPEED_HIGH		= 1600000	// high speed mode ( 1.666Mb/s or 3.333 Mb/s)
}E_I2C_SPEED;

// [3], 10 bit slave address
#define I2C_10BITADDR_SLAVE_MASK		(1<<I2C_10BITADDR_SLAVE_SHIFT)
#define I2C_10BITADDR_SLAVE_VAL(val)	((val)<<I2C_10BITADDR_SLAVE_SHIFT)

// [4], 10 bit master address
#define I2C_10BITADDR_MASTER_MASK	(1<<I2C_10BITADDR_MASTER_SHIFT)
#define I2C_10BITADDR_MASTER_VAL	((val)<<I2C_10BITADDR_MASTER_SHIFT)

typedef enum
{
	I2C_7BIT_ADDR	= 0,	// 0: 7-bit addressing
	I2C_10BIT_ADDR	= 1		// 1: 10-bit addressing
}E_I2C_ADDR_MODE;

// [5], resart enable
#define I2C_RESTART_EN_MASK 	(1<<I2C_RESTART_EN_SHIFT)

// [6], salve disable
#define I2C_SLAVE_DISABLE_MASK 	(1<<I2C_SLAVE_DISABLE_SHIFT)

// offset: 0x10, I2C Rx/Tx Data Buffer and Command
typedef enum
{
	I2C_DAT_SHIFT				= 0,	// [7:0], R/W, This register contains the data to be transmitted or received on the I2C bus. 
										// If you are writing to this register and want to perform a read, bits 7:0 (DAT) 
										// are ignored by the DW_apb_i2c. However, when you read this register, 
										// these bits return the value of data received on the DW_apb_i2c interface. 
										// Reset value: 0x0
	
	I2C_CMD_SHIFT				= 8,	// [8], W, This bit controls whether a read or a write is performed. This bit does not 
										// control the direction when the DW_apb_i2c acts as a slave. It controls only 
										// the direction when it acts as a master.
										// �� 1 = Read
										// �� 0 = Write
										// When a command is entered in the TX FIFO, this bit distinguishes the write 
										// and read commands. In slave-receiver mode, this bit is a ��don��t care�� 
										// because writes to this register are not required. In slave-transmitter mode, a 
										// ��0�� indicates that the data in IC_DATA_CMD is to be transmitted.
										// When programming this bit, you should remember the following: attempting 
										// to perform a read operation after a General Call command has been sent 
										// results in a TX_ABRT interrupt (bit 6 of the IC_RAW_INTR_STAT register), 
										// unless bit 11 (SPECIAL) in the IC_TAR register has been cleared.
										// If a ��1�� is written to this bit after receiving a RD_REQ interrupt, then a 
										// TX_ABRT interrupt occurs.
										// Dependencies: This field is not applicable in Ultra-Fast speed mode ( 
										// IC_ULTRA_FAST_MODE=1)
										// Reset value: 0x0
	
	I2C_STOP_SHIFT				= 9,	// [9], W, This bit controls whether a STOP is issued after the byte is sent or received. 
										// This bit is available only if IC_EMPTYFIFO_HOLD_MASTER_EN is configured to 1.
										// �� 1 �C STOP is issued after this byte, regardless of whether or not the 
										// Tx FIFO is empty. If the Tx FIFO is not empty, the master immediately 
										// tries to start a new transfer by issuing a START and arbitrating for the bus.
										// �� 0 �C STOP is not issued after this byte, regardless of whether or not the 
										// Tx FIFO is empty. If the Tx FIFO is not empty, the master continues the 
										// current transfer by sending/receiving data bytes according to the value of 
										// the CMD bit. If the Tx FIFO is empty, the master holds the SCL line low 
										// and stalls the bus until a new command is available in the Tx FIFO.
	
	I2C_RESTART_SHIFT			= 10,	// [10], W, This bit controls whether a RESTART is issued before the byte is sent or 
										// received. This bit is available only if IC_EMPTYFIFO_HOLD_MASTER_EN 
										// is configured to 1.
										// �� 1 �C If IC_RESTART_EN is 1, a RESTART is issued before the data is 
										// sent/received (according to the value of CMD), regardless of whether or 
										// not the transfer direction is changing from the previous command; if 
										// IC_RESTART_EN is 0, a STOP followed by a START is issued instead.
										// �� 0 �C If IC_RESTART_EN is 1, a RESTART is issued only if the transfer 
										// direction is changing from the previous command; if IC_RESTART_EN 
										// is 0, a STOP followed by a START is issued instead.
	
	I2C_FIRST_DATA_BYTE_SHIFT	= 11,	// [11], R, Indicates the first data byte received after the address phase for receive 
										// transfer in Master receiver or Slave receiver mode.
										// Reset value: 0x0
										// Dependencies: This Register bit value is  applicable only when 
										// FIRST_DATA_BYTE_STATUS=1.
										// Note: In case of APB_DATA_WIDTH=8:
										// 1. You must perform two APB Reads to IC_DATA_CMD to get status on 11 bit.
										// 2. To read the 11 bit, you must perform the first data byte read [7:0] (offset 
										// 0x10)  and then perform the second read[15:8](offset 0x11) to know the 
										// status of 11 bit (whether the data received in previous read is a first data byte).
										// 3. The 11th bit  is an optional read field. You can ignore 2nd byte read [15:8] 
										// (offset 0x11) if not interested in the FIRST_DATA_BYTE status.

	I2C_DATA_CMD_REVD1_SHIFT	= 12	// [31:12], N/A, Reserved
}E_DATA_CMD_SHIFT;

typedef enum
{
	I2C_CMD					= 1<<I2C_CMD_SHIFT,
	I2C_STOP				= 1<<I2C_STOP_SHIFT,
	I2C_RESTART				= 1<<I2C_RESTART_SHIFT,
	I2C_FIRST_DATA_BYTE_ 	= 1<<I2C_FIRST_DATA_BYTE_SHIFT
}E_DATA_CMD;

// offset: 0x2C, I2C Interrupt Status
// offset: 0x30, I2C Interrupt Mask
// offset: 0x34, I2C Raw Interrupt Status
typedef enum
{
	I2C_INT_ST_RX_UNDER_SHIFT		= 0,	// [0], R, Set if the processor attempts to read the receive buffer when it is empty by 
											// reading from the IC_DATA_CMD register. If the module is disabled 
											// (IC_ENABLE[0]=0), this bit keeps its level until the master or slave state 
											// machines go into idle, and when ic_en goes to 0, this interrupt is cleared.
											// Reset value: 0x0
												
	I2C_INT_ST_RX_OVER_SHIFT		= 1,	// [1],  R, Set if the receive buffer is completely filled to IC_RX_BUFFER_DEPTH and 
											// an additional byte is received from an external I2C device. The DW_apb_i2c 
											// acknowledges this, but any data bytes received after the FIFO is full are lost. If 
											// the module is disabled (IC_ENABLE[0]=0), this bit keeps its level until the 
											// master or slave state machines go into idle, and when ic_en goes to 0, this interrupt is cleared.
											// NOTE: If the configuration parameter IC_RX_FULL_HLD_BUS_EN is enabled 
											// and bit 9 of the IC_CON register (RX_FIFO_FULL_HLD_CTRL) is 
											// programmed to HIGH, then the RX_OVER interrupt never occurs, because the 
											// Rx FIFO never overflows.
											// Reset value: 0x0

	I2C_INT_ST_RX_FULL_SHIFT		= 2,	// [2],  R, Set when the receive buffer reaches or goes above the RX_TL threshold in the 
											// IC_RX_TL register. It is automatically cleared by hardware when buffer level 
											// goes below the threshold. If the module is disabled (IC_ENABLE[0]=0), the 
											// RX FIFO is flushed and held in reset; therefore the RX FIFO is not full. So this 
											// bit is cleared once IC_ENABLE[0] is set to 0, regardless of the activity that continues.
											// Reset value: 0x0
												
	I2C_INT_ST_TX_OVER_SHIFT		= 3,	// [3], R, et during transmit if the transmit buffer is filled to IC_TX_BUFFER_DEPTH 
											// and the processor attempts to issue another I2C command by writing to the 
											// IC_DATA_CMD register. When the module is disabled, this bit keeps its level 
											// until the master or slave state machines go into idle, and when ic_en goes to 
											// 0, this interrupt is cleared.
											// Reset value: 0x0
	
	I2C_INT_ST_TX_EMPTY_SHIFT		= 4,	// [4],  R, The behavior of the TX_EMPTY interrupt status differs based on the 
											// TX_EMPTY_CTRL selection in the IC_CON register.
											// �� When TX_EMPTY_CTRL = 0:
											// This bit is set to 1 when the transmit buffer is at or below the threshold 
											// value set in the IC_TX_TL register.
											// �� When TX_EMPTY_CTRL = 1:
											// This bit is set to 1 when the transmit buffer is at or below the threshold 
											// value set in the IC_TX_TL register and the transmission of the 
											// address/data from the internal shift register for the most recently popped 
											// command is completed.
											// It is automatically cleared by hardware when the buffer level goes above the 
											// threshold. When IC_ENABLE[0] is set to 0, the TX FIFO is flushed and held in 
											// reset. There the TX FIFO looks like it has no data within it, so this bit is set 
											// to 1, provided there is activity in the master or slave state machines. When 
											// there is no longer any activity, then with ic_en=0, this bit is set to 0.
											// Reset value: 0x0
	
	I2C_INT_ST_RD_REQ_SHIFT			= 5,	// [5]  , R, This bit is set to 1 when DW_apb_i2c is acting as a slave and another I2C 
											// master is attempting to read data from DW_apb_i2c. The DW_apb_i2c holds the I2C
											// bus in a wait state (SCL=0) until this interrupt is serviced, which means 
											// that the slave has been addressed by a remote master that is asking for data 
											// to be transferred. The processor must respond to this interrupt and then write 
											// the requested data to the IC_DATA_CMD register. This bit is set to 0 just after 
											// the processor reads the IC_CLR_RD_REQ register.
											// Dependencies: This field is not applicable in Ultra-Fast speed mode (IC_ULTRA_FAST_MODE=1)
											// Reset value: 0x0
	
	I2C_INT_ST_TX_ABRT_SHIFT		= 6,	// [6], R, This bit indicates if DW_apb_i2c, as an I2C transmitter, is unable to complete 
											// the intended actions on the contents of the transmit FIFO. 
											// This situation can occur both as an I2C master or an I2C slave, and is referred to as a ��transmit abort��.
											// When this bit is set to 1, the IC_TX_ABRT_SOURCE register indicates the 
											// reason why the transmit abort takes places.
											// NOTE: The DW_apb_i2c flushes/resets/empties  only the TX_FIFO whenever 
											// there is a transmit abort caused by any of the events tracked by the 
											// IC_TX_ABRT_SOURCE register. The Tx FIFO remains in this flushed state 
											// until the register IC_CLR_TX_ABRT is read. Once this read is performed, the 
											// Tx FIFO is then ready to accept more data bytes from the APB interface. RX 
											// FIFO is flushed because of TX_ABRT is controlled by the coreConsultant 
											// parameter  IC_AVOID_RX_FIFO_FLUSH_ON_TX_ABRT.
											// Reset value: 0x0
	
	I2C_INT_ST_RX_DONE_SHIFT		= 7,	// [7], R, When the DW_apb_i2c is acting as a slave-transmitter, this bit is set to 1 if the 
											// master does not acknowledge a transmitted byte. This occurs on the last byte 
											// of the transmission, indicating that the transmission is done.
											// Dependencies: This field is not applicable in Ultra-Fast speed mode (IC_ULTRA_FAST_MODE=1)
											// Reset value: 0x0
	
	I2C_INT_ST_ACTIVITY_SHIFT		= 8,	// [8], R, This bit captures DW_apb_i2c activity and stays set until it is cleared. There 
											// are four ways to clear it: 
											// �� Disabling the DW_apb_i2c
											// �� Reading the IC_CLR_ACTIVITY register
											// �� Reading the IC_CLR_INTR register
											// �� System reset
											// Once this bit is set, it stays set unless one of the four methods is used to clear 
											// it. Even if the DW_apb_i2c module is idle, this bit remains set until cleared, 
											// indicating that there was activity on the bus.
											// Reset value: 0x0
	
	I2C_INT_ST_STOP_DET_SHIFT		= 9,	// [9], R, Indicates whether a STOP condition has occurred on the I2C interface 
											// regardless of whether DW_apb_i2c is operating in slave or master mode.
											// In Slave Mode:
											// �� If IC_CON[7]=1'b1  (STOP_DET_IFADDRESSED), the STOP_DET 
											// interrupt is generated only if the slave is addressed.
											// Note: During a general call address, this slave does not issue a 
											// STOP_DET interrupt if STOP_DET_IF_ADDRESSED=1'b1, even if the 
											// slave responds to the general call address by generating ACK. The 
											// STOP_DET interrupt is generated only when the transmitted address 
											// matches the slave address (SAR).
											// �� If IC_CON[7]=1'b0  (STOP_DET_IFADDRESSED), the STOP_DET 
											// interrupt is issued irrespective of whether it is being addressed.
											// In Master Mode:
											// �� If IC_CON[10]=1��b1  (STOP_DET_IF_MASTER_ACTIVE), the STOP_DET 
											// interrupt is issued only if the master is active. 
											// �� If IC_CON[10]=1��b0  (STOP_DET_IFADDRESSED), the STOP_DET 
											// interrupt is issued irrespective of whether the master is active.
											// Reset value: 0x0

	I2C_INT_ST_START_DET_SHIFT		= 10,	// [10], R, Indicates whether a START or RESTART condition has occurred on the I2C 
											// interface regardless of whether DW_apb_i2c is operating in slave or master mode.
											// Reset value: 0x0
	
	I2C_INT_ST_SGEN_CALL_SHIFT		= 11,	// [11], R, Set only when a General Call address is received and it is acknowledged. It 
											// stays set until it is cleared either by disabling DW_apb_i2c or when the CPU 
											// reads bit 0 of the IC_CLR_GEN_CALL register. DW_apb_i2c stores the 
											// received data in the Rx buffer.
											// Reset value: 0x0
	
	I2C_INT_ST_RESTART_DET_SHIFT	= 12,	// [12], R, Indicates whether a RESTART condition has occurred on the I2C interface 
											// when DW_apb_i2c is operating in slave mode and the slave is the addressed slave.
											// Enabled only when IC_SLV_RESTART_DET_EN = 1
											// NOTE: However, in high-speed mode or during a START BYTE transfer, the 
											// RESTART comes before the address field as per the I2C protocol. In this case, 
											// the slave is not the addressed slave when the RESTART is issued, therefore 
											// DW_apb_i2c does not generate the RESTART_DET interrupt.
											// Reset value: 0x0
	
	I2C_INT_ST_MST_ON_HOLD_SHIFT	= 13,	// [13], Indicates whether a master is holding the bus and the Tx FIFO is empty. 
											// Enabled only when I2C_DYNAMIC_TAR_UPDATE = 1 and 
											// IC_EMPTYFIFO_HOLD_MASTER_EN = 1
											// Reset value: 0X0
	
	I2C_INT_ST_SCL_STUCK_AT_LOW_SHIFT	= 14,	// [14], R, Indicates whether the SCL Line is stuck at low for the 
											// IC_SCL_STUCK_LOW_TIMOUT number of ic_clk periods.
											// Enabled only when IC_BUS_CLEAR_FEATURE = 1
											// Dependencies: This field is not applicable in Ultra-Fast speed mode (IC_ULTRA_FAST_MODE=1)
											// Reset Value: 0x0
	
	I2C_INT_ST_REVD1_SHIFT			= 15	// [31:15], N/A, Reserved
} E_IC_RAW_INTR_STAT_SHIFT;

// mask
#define I2C_INT_ST_RX_UNDER_MASK			(1<<I2C_INT_ST_RX_UNDER_SHIFT)
#define I2C_INT_ST_RX_OVER_MASK				(1<<I2C_INT_ST_RX_OVER_SHIFT)
#define I2C_INT_ST_RX_FULL_MASK				(1<<I2C_INT_ST_RX_FULL_SHIFT)
#define I2C_INT_ST_TX_OVER_MASK				(1<<I2C_INT_ST_TX_OVER_SHIFT)
#define I2C_INT_ST_TX_EMPTY_MASK			(1<<I2C_INT_ST_TX_EMPTY_SHIFT)
#define I2C_INT_ST_RD_REQ_MASK				(1<<I2C_INT_ST_RD_REQ_SHIFT)
#define I2C_INT_ST_TX_ABRT_MASK				(1<<I2C_INT_ST_TX_ABRT_SHIFT)
#define I2C_INT_ST_RX_DONE_MASK				(1<<I2C_INT_ST_RX_DONE_SHIFT)
#define I2C_INT_ST_ACTIVITY_MASK			(1<<I2C_INT_ST_ACTIVITY_SHIFT)
#define I2C_INT_ST_STOP_DET_MASK			(1<<I2C_INT_ST_STOP_DET_SHIFT)
#define I2C_INT_ST_START_DET_MASK			(1<<I2C_INT_ST_START_DET_SHIFT)
#define I2C_INT_ST_SGEN_CALL_MASK			(1<<I2C_INT_ST_SGEN_CALL_SHIFT)
#define I2C_INT_ST_RESTART_DET_MASK			(1<<I2C_INT_ST_RESTART_DET_SHIFT)
#define I2C_INT_ST_MST_ON_HOLD_MASK			(1<<I2C_INT_ST_MST_ON_HOLD_SHIFT)
#define I2C_INT_ST_SCL_STUCK_AT_LOW_MASK	(1<<I2C_INT_ST_SCL_STUCK_AT_LOW_SHIFT)

// offset: 0x6C, I2C Enable
typedef enum
{
	I2C_ENABLE_SHIFT							= 0,	// [0], R/W, Controls whether the DW_apb_i2c is enabled. 
														// �� 0: Disables DW_apb_i2c (TX and RX FIFOs are held in an erased state)
														// �� 1: Enables DW_apb_i2c
														// Software can disable DW_apb_i2c while it is active. However, it is important 
														// that care be taken to ensure that DW_apb_i2c is disabled properly. A 
														// recommended procedure is described in ��Disabling DW_apb_i2c�� on page 62.
														// When DW_apb_i2c is disabled, the following occurs:
														// �� The TX FIFO and RX FIFO get flushed.
														// �� Status bits in the IC_INTR_STAT register are still active until DW_apb_i2c goes into IDLE state.
														// If the module is transmitting, it stops as well as deletes the contents of the 
														// transmit buffer after the current transfer is complete. If the module is receiving, 
														// the DW_apb_i2c stops the current transfer at the end of the current byte and 
														// does not acknowledge the transfer.
														// In systems with asynchronous pclk and ic_clk when IC_CLK_TYPE 
														// parameter set to asynchronous (1), there is a two ic_clk delay when enabling 
														// or disabling the DW_apb_i2c.
														// For a detailed description on how to disable DW_apb_i2c, refer to ��Disabling 
														// DW_apb_i2c�� on page 62.
														// Reset value: 0x0
	
	I2C_ABORT_SHIFT								= 1,	// [1], R/W, When set, the controller initiates the transfer abort.
														// �� 0: ABORT not initiated or ABORT done
														// �� 1: ABORT operation in progress
														// The software can abort the I2C transfer in master mode by setting this bit. The 
														// software can set this bit only when ENABLE is already set; otherwise, the 
														// controller ignores any write to ABORT bit. The software cannot clear the 
														// ABORT bit once set. In response to an ABORT, the controller issues a STOP 
														// and flushes the Tx FIFO after completing the current transfer, then sets the 
														// TX_ABORT interrupt after the abort operation. The ABORT bit is cleared 
														// automatically after the abort operation.
														// For a detailed description on how to abort I2C transfers, refer to ��Aborting I2C 
														// Transfers�� on page 63.
														// Reset value: 0x0
	
	I2C_TX_CMD_BLOCK_SHIFT						= 2,	// [2], R/W, In Master mode
														// �� 1��b1: Blocks the transmission of data on I2C bus even if Tx FIFO has data to transmit.
														// �� 1��b0: The transmission of data starts on I2C bus automatically, as soon as 
														// the first data is available in the Tx FIFO.
														// Reset value: IC_TX_CMD_BLOCK_DEFAULT 
														// Dependencies: This Register bit value is  applicable only when 
														// IC_TX_CMD_BLOCK =1.
														// Note: To block the execution of Master commands, set the TX_CMD_BLOCK 
														// bit only when Tx FIFO is empty (IC_STATUS[2]=1) and the master is in the 
														// Idle state (IC_STATUS[5] == 0). Any further commands put in the Tx FIFO are 
														// not executed until TX_CMD_BLOCK bit is unset
	
	I2C_SDA_STUCK_RECOVERY_ENABLE_SHIFT			= 3,	// [3], R/W, If SDA is stuck at low indicated through the TX_ABORT interrupt 
														// (IC_TX_ABRT_SOURCE[17]), then this bit is used as a control knob to initiate 
														// the SDA Recovery Mechanism (that is, send at most 9 SCL clocks and STOP 
														// to release the SDA line) and then this bit gets auto clear.
														// This bit is enabled only when IC_BUS_CLEAR_FEATURE = 1.
														// Dependencies: This field is not applicable in Ultra-Fast speed mode ( 
														// IC_ULTRA_FAST_MODE=1)
														// Reset Value: 0x0
	
	I2C_ENABLE_REVD0_SHIFT						= 4,	// [15:4], N/A, Reserved
	
	I2C_SMBUS_CLK_RESET_SHIFT					= 16,	// [16], R/W, This bit is used in SMBus Host mode to initiate the SMBus Master Clock 
														// Reset. This bit should be enabled only when Master is in idle. Whenever this 
														// bit is enabled, the SMBCLK is held low for the IC_SCL_STUCK_TIMEOUT 
														// ic_clk cycles to reset the SMBus Slave devices.
														// Dependencies: This Register bit value is applicable only when
														// IC_SMBUS=1
														// Reset Value: 0x0
	
	I2C_SMBUS_SUSPEND_EN_SHIFT					= 17,	// [17], R/W, The SMBUS_SUSPEND_EN register bit is used to control assertion and de-
														// assertion of SMBSUS signal.
														// 0: De-assert SMBSUS signal
														// 1: Assert SMBSUS signal
														// Dependencies: This Register bit value is applicable only when 
														// IC_SMBUS_SUSPEND_ALERT=1
														// Reset Value: 0x0
		
	I2C_SMBUS_ALERT_EN_SHIFT					= 18,	// [18], R/W, The SMBUS_ALERT_CTRL register bit is used to control assertion of 
														// SMBALERT signal.
														// 1: Assert SMBALERT signal
														// This register bit is auto-cleared after detection of Acknowledgement from 
														// master for Alert Response address.
														// Dependencies: This Register bit value is applicable only when 
														// IC_SMBUS_SUSPEND_ALERT=1 
														// Reset Value: 0x0
	
	I2C_ENABLE_REVD1_SHIFT						= 19	// [31:19], N/A, Reserved
}E_I2C_ENABLE_SHIFT;

// [0], I2C enable
#define I2C_ENABLE_MASK		(1<<I2C_ENABLE_SHIFT)
#define I2C_ENABLE_VAL(val)	((val)<<I2C_ENABLE_SHIFT)

typedef enum
{
	I2C_CTRL_DIS	= 0,
	I2C_CTRL_EN		= 1
}E_I2C_ENABLE;

// [1], I2C abort
#define I2C_ABORT_MASK		(1<<I2C_ABORT_SHIFT)
#define I2C_ABORT_VAL(val)	((val)<<I2C_ABORT_SHIFT)

// [2], TX cmd block
#define I2C_TX_CMD_BLOCK_MASK	(1<<I2C_TX_CMD_BLOCK_SHIFT)




// offset: 0x70, I2C Status register
typedef enum
{
	I2C_ACTIVITY_SHIFT						= 0,	// [0], R, I2C Activity Status. 
													// Reset value: 0x0
													
	I2C_TFNF_SHIFT							= 1,	// [1], R, Transmit FIFO Not Full. Set when the transmit FIFO contains one or 
													// more empty locations, and is cleared when the FIFO is full.
													// �� 0: Transmit FIFO is full
													// �� 1: Transmit FIFO is not full
													// Reset value: 0x1
													
	I2C_TFE_SHIFT							= 2,	// [2], R, Transmit FIFO Completely Empty. When the transmit FIFO is 
													// completely empty, this bit is set. When it contains one or more valid 
													// entries, this bit is cleared. This bit field does not request an interrupt.
													// �� 0: Transmit FIFO is not empty
													// �� 1: Transmit FIFO is empty
													// Reset value: 0x1
	
	I2C_RFNE_SHIFT							= 3,	// [3], R, Receive FIFO Not Empty. This bit is set when the receive FIFO 
													// contains one or more entries; it is cleared when the receive FIFO is empty. 
													// �� 0: Receive FIFO is empty
													// �� 1: Receive FIFO is not empty
													// Reset value: 0x0
	
	I2C_RFF_SHIFT							= 4,	// [4], R, Receive FIFO Completely Full. When the receive FIFO is 
													// completely full, this bit is set. When the receive FIFO contains one 
													// or more empty location, this bit is cleared.
													// �� 0: Receive FIFO is not full
													// �� 1: Receive FIFO is full
													// Reset value: 0x0 

	I2C_MST_ACTIVITY_SHIFT					= 5,	// [5], R, Master FSM Activity Status. When the Master Finite State Machine 
													// (FSM) is not in the IDLE state, this bit is set.
													// �� 0: Master FSM is in IDLE state so the Master part of 
													// DW_apb_i2c is not Active
													// �� 1: Master FSM is not in IDLE state so the Master part of 
													// DW_apb_i2c is Active
													// NOTE: IC_STATUS[0]��that is, ACTIVITY bit��is the OR of 
													// SLV_ACTIVITY and MST_ACTIVITY bits.
													// Reset value: 0x0 
	
	I2C_SLV_ACTIVITY_SHIFT					= 6,	// [6], R, Slave FSM Activity Status. When the Slave Finite State Machine 
													// (FSM) is not in the IDLE state, this bit is set.
													// �� 0: Slave FSM is in IDLE state so the Slave part of DW_apb_i2c is not Active
													// �� 1: Slave FSM is not in IDLE state so the Slave part of DW_apb_i2c is Active
													// Reset value: 0x0 
	
	I2C_MST_HOLD_TX_FIFO_EMPTY_SHIFT		= 7,	// [7], R, If the IC_EMPTYFIFO_HOLD_MASTER_EN parameter is set to 1, 
													// the DW_apb_i2c master stalls the write transfer when Tx FIFO is 
													// empty, and the the last byte does not have the Stop bit set.
													// This bit indicates the BUS hold when the master holds the bus 
													// because of the Tx FIFO being empty, and the the previous 
													// transferred command does not have the Stop bit set. (This  kind of 
													// Bus hold is applicable if IC_EMPTYFIFO_HOLD_MASTER_EN is 
													// set to 1).
													// Reset value: 0x0
													// Dependencies: This Register bit value is  applicable only when
													// IC_STAT_FOR_CLK_STRETCH=1
	
	I2C_MST_HOLD_RX_FIFO_FULL_SHIFT			= 8,	// [8], R, This bit indicates the BUS Hold in Master mode due to Rx FIFO is 
													// Full and additional byte has been received (This  kind of Bus hold is 
													// applicable if IC_RX_FULL_HLD_BUS_EN is set to 1).
													// Reset value: 0x0
													// Dependencies: This Register bit value is  applicable only when
													// IC_STAT_FOR_CLK_STRETCH=1
	
	I2C_SLV_HOLD_TX_FIFO_EMPTY_SHIFT		= 9,	// [9], R, This bit indicates the BUS Hold in Slave mode for the Read request 
													// when the Tx FIFO is empty. The Bus is in hold until the Tx FIFO has 
													// data to Transmit for the read request.
													// Reset value: 0x0
													// Dependencies: This Register bit value is applicable only when 
													// IC_STAT_FOR_CLK_STRETCH=1.
	
	I2C_SLV_HOLD_RX_FIFO_FULL_SHIFT			= 10,	// [10], R, This bit indicates the BUS Hold in Slave mode due to the Rx FIFO 
													// being Full and an additional byte being received (this  kind of Bus 
													// hold is applicable if IC_RX_FULL_HLD_BUS_EN is set to 1).
													// Reset value: 0x0
													// Dependencies: This Register bit value is  applicable only when 
													// IC_STAT_FOR_CLK_STRETCH=1.
	
	I2C_SDA_STUCK_NOT_RECOVERED_SHIFT		= 11,	// [11], R, This bit indicates that an SDA stuck at low is not recovered after the 
													// recovery mechanism.
													// This bit is enabled only when IC_BUS_CLEAR_FEATURE = 1.
													// Reset Value: 0x0
	
	I2C_ST_REVD0_SHIFT						= 12,	// [15:12], N/A, Reserved
	
	I2C_SMBUS_QUICK_CMD_BIT_SHIFT			= 16,	// [16], R, This bit indicates the R/W bit of the Quick command received. This 
													// bit will be cleared after the user has read this bit.
													// Dependencies: Enabled only when IC_SMBUS=1 is set to 1.
													// Reset Value: 0x0
		
	I2C_SMBUS_SLAVE_ADDR_VALID_SHIFT		= 17,	// [17], R, This bit indicates whether the SMBus Slave address (ic_sar[6:0]) is 
													// valid or not.
													// Dependencies: Enabled only when IC_SMBUS_ARP=1 is set to 1.
													// Reset Value: 0x0
	
	I2C_SMBUS_SLAVE_ADDR_RESOLVED_SHIFT		= 18,	// [18], R, This bit indicates whether the SMBus Slave address (ic_sar[6:0]) is 
													// Resolved by ARP Master.
													// Dependencies: Enabled only when IC_SMBUS_ARP=1 is set to 1.
													// Reset Value: 0x0
	
	I2C_SMBUS_SUSPEND_STATUS_SHIFT			= 19,	// [19], R, This bit indicates whether the status of the input signal is 
													// ic_smbus_sus_in_n. This signal is asserted when the SMBus 
													// Suspend signal is asserted by the SMBus Host.
													// Dependencies: Enabled only when 
													// IC_SMBUS_SUSPEND_ALERT=1 is set to 1.
													// Reset Value: 0x0

	I2C_SMBUS_ALERT_STATUS_SHIFT			= 20,	// [20], R, This bit indicates whether the status of the input signal is 
													// ic_smbus_alert_in_n. This signal is asserted when the SMBus Alert 
													// signal is asserted by the SMBus Device.
													// Dependencies: Enabled only when 
													// IC_SMBUS_SUSPEND_ALERT=1 is set to 1.
													// Reset Value: 0x0 

	I2C_ST_REVD1_SHIFT						= 19	// [31:19], N/A, Reserved
}E_I2C_STATUS_SHIFT;

// mask
#define I2C_ACTIVITY_MASK		(1<<I2C_ACTIVITY_SHIFT)
#define I2C_TFNF_MASK			(1<<I2C_TFNF_SHIFT)
#define I2C_TFE_MASK			(1<<I2C_TFE_SHIFT)
#define I2C_RFNE_MASK			(1<<I2C_RFNE_SHIFT)
#define I2C_RFF_MASK			(1<<I2C_RFF_SHIFT)
#define I2C_MST_ACTIVITY_MASK	(1<<I2C_MST_ACTIVITY_SHIFT)
#define I2C_SLV_ACTIVITY_MASK	(1<<I2C_SLV_ACTIVITY_SHIFT)

// offset: 0x88, DMA Control Register for transmit and receive handshaking interface
typedef enum
{
	I2C_RDMAE_SHIFT			= 0,	// [0], R/W,  Receive DMA Enable. This bit enables/disables the receive FIFO DMA channel.
									//  0 = Receive DMA disabled
									//  1 = Receive DMA enabled 
									// Reset value: 0x0
													
	I2C_TDMAE_SHIFT			= 1,	// [1], R/W, TTransmit DMA Enable. This bit enables/disables the transmit FIFO DMA channel.
									//  0 = Transmit DMA disabled
									//  1 = Transmit DMA enabled
									// Reset value: 0x0 
													
	I2C_DMA_CR_REVD_SHIFT	= 2	// [31:2], N/A, Reserved
}E_IC_DMA_CR_SHIFT;

// receive dma mask and value
#define I2C_RDMAE_MASK		(1<<I2C_RDMAE_SHIFT)
#define I2C_RDMAE_VAL(val)	((val)<<I2C_RDMAE_SHIFT)

// transmit dma mask and value
#define I2C_TDMAE_MASK		(1<<I2C_TDMAE_SHIFT)
#define I2C_TDMAE_VAL(val)	((val)<<I2C_TDMAE_SHIFT)

typedef enum
{
	I2C_DMA_DIS	= 0,
	I2C_DMA_EN	= 1
}E_IC_DMA_CR;



#endif

