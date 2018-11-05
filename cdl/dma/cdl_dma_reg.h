/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_DMA_REG_H__
#define __CDL_DMA_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
//#define DMA_CH_SIZE	0x20

// register layout

// DW_axi_dmac common register address block Registers.
#define DMAC_IDREG_OFST							0x000 	// DMAC ID Register contains a 64-bit value that is hardwired and read back by a read to the DW_axi_dmac...
#define DMAC_COMPVERREG_OFST					0x008	// This register contains a 64-bit value that is hardwired and read back by a read to the DW_axi_dmac...
#define DMAC_CFGREG_OFST						0x010	// This register is used to enable the DW_axi_dmac, which must be done before any channel activity...
#define DMAC_CHENREG_OFST						0x018	// This is DW_axi_dmac Channel Enable Register. If software wants to set up a new channel, it can read...
#define DMAC_INTSTATUSREG_OFST					0x030	// DMAC Interrupt Status Register captures the combined channel interrupt for each channel and Combined...
#define DMAC_COMMONREG_INTCLEARREG_OFST			0x038	// Writing 1 to specific field clears the corresponding field in DMAC Common register Interrupt Status...
#define DMAC_COMMONREG_INTSTATUS_ENABLEREG_OFST	0x040	// Writing 1 to specific field enables the corresponding interrupt status generation in DMAC Common...
#define DMAC_COMMONREG_INTSIGNAL_ENABLEREG_OFST	0x048	// Writing 1 to specific field will propagate the corresponding interrupt status in DMAC Common register...
#define DMAC_COMMONREG_INTSTATUSREG_OFST		0x050	// This Register captures Slave interface access errors.  - Decode Error.  - Write to read only register....
#define DMAC_RESETREG_OFST						0x058	// This register is used to initiate the Software Reset to DW_axi_dmac

// DW_axi_dmac Channel 1 register address block Registers
#define CHX_SAR_OFST(ch)						(0x100 + (ch * 0x100)) // The starting source address is programmed by software before the DMA channel is enabled, or by an...
#define CHX_DAR_OFST(ch)						(0x108 + (ch * 0x100)) // The starting destination address is programmed by the software before the DMA channel is enabled,...
#define CHX_BLOCK_TS_OFST(ch)					(0x110 + (ch * 0x100)) // When DW_axi_dmac is the flow controller, the DMAC uses this register before the channel is enabled...
#define CHX_CTL_OFST(ch)						(0x118 + (ch * 0x100)) // This register contains fields that control the DMA transfer. This register should be programmed...
#define CHX_CFG_OFST(ch)						(0x120 + (ch * 0x100)) // This register contains fields that configure the DMA transfer. This register should be programmed...
#define CHx_LLP_OFST(ch)						(0x128 + (ch * 0x100)) // This is the Linked List Pointer register. This register must be programmed to point to the first...
#define CHx_STATUSREG_OFST(ch)					(0x130 + (ch * 0x100)) // Channelx Status Register contains fields that indicate the status of DMA transfers for...
#define CHx_SWHSSRCREG_OFST(ch)					(0x138 + (ch * 0x100)) // Channelx Software handshake Source Register
#define CHx_SWHSDSTREG_OFST(ch)					(0x140 + (ch * 0x100)) // Channelx Software handshake Destination Register
#define CHx_BLK_TFR_RESUMEREQREG_OFST(ch)		(0x148 + (ch * 0x100)) // Channelx Block Transfer Resume Request Register. This register is used during Linked List or Shadow...
#define CHx_AXI_IDREG_OFST(ch)					(0x150 + (ch * 0x100)) // Channelx AXI ID Register. This register is allowed to be updated only when the channel is disabled,...
#define CHx_AXI_QOSREG_OFST(ch)					(0x158 + (ch * 0x100)) // Channelx AXI QOS Register. This register is allowed to be updated only when the channel is disabled,
#define CHx_SSTA_OFST(ch)						(0x160 + (ch * 0x100)) // Channelx Source Status Register. After each block transfer completes, hardware can retrieve the...
#define CHx_DSTAT_OFST(ch)						(0x168 + (ch * 0x100)) // Channelx Destination Status Register. After each block transfer completes, hardware can retrieve..
#define CHx_SSTATAR_OFST(ch)					(0x170 + (ch * 0x100)) // Channelx Source Status Fetch Register. After completion of each block transfer, hardware can retrieve...
#define CHx_DSTATAR_OFST(ch)					(0x178 + (ch * 0x100)) // Channelx Destination Status Fetch Register. After completion of each block transfer, hardware can...
#define CHx_INTSTATUS_ENABLEREG_OFST(ch)		(0x180 + (ch * 0x100)) // Writing 1 to specific field enables the corresponding interrupt status generation in Channelx Interrupt...
#define CHx_INTSTATUS_OFST(ch)					(0x188 + (ch * 0x100)) // Channelx Interrupt Status Register captures the Channelx specific interrupts
#define CHx_INTSIGNAL_ENABLEREG_OFST(ch)		(0x190 + (ch * 0x100)) // This register contains fields that are used to enable the generation of port level interrupt at..
#define CHx_INTCLEARREG_OFST(ch)				(0x198 + (ch * 0x100)) // Writing 1 to specific field will clear the corresponding field in Channelx Interrupt Status...

// offset: 0x008, This register contains a 64-bit value that is hardwired and read back by a read to the DW_axi_dmac...
typedef enum
{
	DMAC_COMPVER_SHIFT  		= 0,	// [31:0],  R, DMAC Component Version Number.
										
	RSVD_DMAC_COMPVERREG_SHIFT	= 32	// [63:32], R, DMAC_COMPVERREG Reserved bits - Read Only
}E_DMAC_COMPVERREG_SHIFT;

// offset: 0x010, This register is used to enable the DW_axi_dmac, which must be done before any 
// channel activity can begin. This register also contains global interrupt enable bit. 
typedef enum
{
	DMAC_EN_SHIFT  			= 0,	// [0],  R/W, This bit is used to enable the DW_axi_dmac.
									// ¡ö 0: DW_axi_dmac disabled
									// ¡ö 1: DW_axi_dmac enabled
									// NOTE: If this bit DMAC_EN bit is cleared while any channel 
									// is still active, then this bit still returns 1 to indicate that there 
									// are channels still active until DW_axi_dmac hardware has 
									// terminated all activity on all channels, at which point this bit 
									// returns zero (0).
									// Values: 
									// ¡ö 0x1 (ENABLED): DW_axi_dmac is enabled
									// ¡ö 0x0 (DISABLED): DW_axi_dmac is disabled
									// Exists: Always
										
	DMAC_INT_EN_SHIFT		= 1,	// [1], R/W, This bit is used to globally enable the interrupt generation.
									// ¡ö 0: DW_axi_dmac Interrupts are disabled
									// ¡ö 1: DW_axi_dmac Interrupt logic is enabled.
									// Values: 
									// ¡ö 0x1 (ENABLED): DW_axi_dmac Interrupts are enabled
									// ¡ö 0x0 (DISABLED): DW_axi_dmac Interrupts are disabled
									// Exists: Always

	DMAC_CFGREG_RSVD_SHIFT	= 2		// [63:2], R, DMAC_CFGREG Reserved bits - Read Only
									// Exists: Always
}E_DMAC_CFGREG_SHIFT;

// DMAC_EN mask and value
#define DMAC_EN_MASK		(1<<DMAC_EN_SHIFT)
#define DMAC_EN_VAL(val)	((val)<<DMAC_EN_SHIFT)

typedef enum
{
	DMAC_DIS	= 0,
	DMAC_EN		= 1
}E_DMAC_EN;

// DMAC_INT_EN mask and value
#define DMAC_INT_EN_MASK		(1<<DMAC_INT_EN_SHIFT)
#define DMAC_INT_EN_VAL(val)	((val)<<DMAC_INT_EN_SHIFT)

typedef enum
{
	DMAC_INT_DIS	= 0,
	DMAC_INT_EN		= 1
}E_DMAC_INT_EN;

// offset: 0x018, This is DW_axi_dmac Channel Enable Register. If software wants to set up a new channel, it can read...
typedef enum
{
	DMAC_CH1_EN_SHIFT  		= 0,	// [0],  R/W, This bit is used to enable the DW_axi_dmac Channel-1.
									// ¡ö 0: DW_axi_dmac Channel-1 is disabled
									// ¡ö 1: DW_axi_dmac Channel-1 is enabled
									// The bit 'DMAC_ChEnReg.CH1_EN' is automatically cleared 
									// by hardware to disable the channel after the last AMBA 
									// transfer of the DMA transfer to the destination has 
									// completed. Software can therefore poll this bit to determine 
									// when this channel is free for a new DMA transfer.
									// Values: 
									// ¡ö 0x1 (ENABLE_CH1): DW_axi_dmac: Channel-1 is  enabled
									// ¡ö 0x0 (DISABLE_CH1): DW_axi_dmac: Channel-1 is disabled
									// Exists: DMAX_NUM_CHANNELS > 0
									// Volatile: true
	DMAC_CH2_EN_SHIFT		= 1,	// [1], R/W, This bit is used to enable the DW_axi_dmac Channel-2.
	DMAC_CH3_EN_SHIFT		= 2,	// [2], R/W, This bit is used to enable the DW_axi_dmac Channel-3.
	DMAC_CH4_EN_SHIFT		= 3,	// [3], R/W, This bit is used to enable the DW_axi_dmac Channel-4.
	DMAC_CH5_EN_SHIFT		= 4,	// [4], R/W, This bit is used to enable the DW_axi_dmac Channel-5.
	DMAC_CH6_EN_SHIFT		= 5,	// [5], R/W, This bit is used to enable the DW_axi_dmac Channel-6.
	DMAC_CH7_EN_SHIFT		= 6,	// [6], R/W, This bit is used to enable the DW_axi_dmac Channel-7.
	DMAC_CH8_EN_SHIFT		= 7,	// [7], R/W, This bit is used to enable the DW_axi_dmac Channel-8.

	
	DMAC_CH1_EN_WE_SHIFT	= 8,	// [8], W,     DW_axi_dmac Channel-1 Enable Write Enable bit.
									// Read back value of this register bit is always '0'.
									// Values: 
									// ¡ö 0x1 (ENABLE_WR_CH1_EN): Enable Write to CH1_EN bit
									// ¡ö 0x0 (DISABLE_WR_CH1_EN): Disable Write to respective CH1_EN bit
									// Exists: DMAX_NUM_CHANNELS > 0
									// Volatile: true
	DMAC_CH2_EN_WE_SHIFT	= 9,	// [9],  W,     DW_axi_dmac Channel-2 Enable Write Enable bit.
	DMAC_CH3_EN_WE_SHIFT	= 10,	// [10], W,     DW_axi_dmac Channel-3 Enable Write Enable bit.
	DMAC_CH4_EN_WE_SHIFT	= 11,	// [11], W,     DW_axi_dmac Channel-4 Enable Write Enable bit.
	DMAC_CH5_EN_WE_SHIFT	= 12,	// [12], W,     DW_axi_dmac Channel-5 Enable Write Enable bit.
	DMAC_CH6_EN_WE_SHIFT	= 13,	// [13], W,     DW_axi_dmac Channel-6 Enable Write Enable bit.
	DMAC_CH7_EN_WE_SHIFT	= 14,	// [14], W,     DW_axi_dmac Channel-7 Enable Write Enable bit.
	DMAC_CH8_EN_WE_SHIFT	= 15,	// [15], W,     DW_axi_dmac Channel-8 Enable Write Enable bit.


	DMAC_CH1_SUSP_SHIFT		= 16,	// [16], R/W, Channel-1 Suspend Request.
									// Software sets this bit to 1 to request channel suspend. If this 
									// bit is set to 1, DW_axi_dmac suspends all DMA data 
									// transfers from the source gracefully until this bit is cleared. 
									// There is no guarantee that the current dma transaction will 
									// complete. This bit can also be used in conjunction with 
									// CH1_Status.CH_SUSPENDED to cleanly disable the 
									// channel without losing any data. In this case, software first 
									// sets CH1_SUSP bit to 1 and polls 
									// CH1_Status.CH_SUSPENDED till it is set to 1. Software can 
									// then clear CH1_EN bit to 0 to disable the channel.
									// ¡ö 0: No Channel Suspend Request. 
									// ¡ö 1: Request for Channel Suspend.
									// Software can clear CH1_SUSP bit to 0, after DW_axi_dmac 
									// sets CH1_Status.CH_SUSPENDED bit to 1, to exit the 
									// channel suspend mode.
									// Note: CH_SUSP is cleared when channel is disabled.
									// Values: 
									// ¡ö 0x1 (ENABLE_CH1_SUSP): Request to Suspended Channel-1
									// ¡ö 0x0 (DISABLE_CH1_SUSP): No Channel Suspend 
									// Request
									// Exists: DMAX_NUM_CHANNELS > 0
									// Volatile: true
	DMAC_CH2_SUSP_SHIFT		= 17,	// [17], R/W, Channel-2 Suspend Request.
	DMAC_CH3_SUSP_SHIFT		= 18,	// [18], R/W, Channel-3 Suspend Request.
	DMAC_CH4_SUSP_SHIFT		= 19,	// [19], R/W, Channel-4 Suspend Request.
	DMAC_CH5_SUSP_SHIFT		= 20,	// [20], R/W, Channel-5 Suspend Request.
	DMAC_CH6_SUSP_SHIFT		= 21,	// [21], R/W, Channel-6 Suspend Request.
	DMAC_CH7_SUSP_SHIFT		= 22,	// [22], R/W, Channel-7 Suspend Request.
	DMAC_CH8_SUSP_SHIFT		= 23,	// [23], R/W, Channel-8 Suspend Request.

	DMAC_CH1_SUSP_WE_SHIFT	= 24,	// [24], W,   This bit is used as a write enable to the Channel-1 Suspend 
									// bit. The read back value of this register bit is always 0.
									// Values: 
									// ¡ö 0x1 (ENABLE_WR_CH1_SUSP): Enable  Write to respective CH1_SUSP bit
									// ¡ö 0x0 (DISABLE_WR_CH1_SUSP): Disable Write to CH${ch_num}_SUSP bit
									// Exists: DMAX_NUM_CHANNELS > 0
									// Volatile: true
	DMAC_CH2_SUSP_WE_SHIFT	= 25,	// [25], W,   This bit is used as a write enable to the Channel-2 Suspend 
	DMAC_CH3_SUSP_WE_SHIFT	= 26,	// [26], W,   This bit is used as a write enable to the Channel-3 Suspend 
	DMAC_CH4_SUSP_WE_SHIFT	= 27,	// [27], W,   This bit is used as a write enable to the Channel-4 Suspend 
	DMAC_CH5_SUSP_WE_SHIFT	= 28,	// [28], W,   This bit is used as a write enable to the Channel-5 Suspend 
	DMAC_CH6_SUSP_WE_SHIFT	= 29,	// [29], W,   This bit is used as a write enable to the Channel-6 Suspend 
	DMAC_CH7_SUSP_WE_SHIFT	= 30,	// [30], W,   This bit is used as a write enable to the Channel-7 Suspend 
	DMAC_CH8_SUSP_WE_SHIFT	= 31,	// [31], W,   This bit is used as a write enable to the Channel-8 Suspend 


	DMAC_CH1_ABORT_SHIFT	= 32,	// [32], Varies, Channel-1 Abort Request.
									// Software sets this bit to 1 to request channel abort. If this bit 
									// is set to 1, DW_axi_dmac disables the channel immediately. 
									// Aborting the channel might result in AXI Protocol violation as 
									// DW_axi_dmac does not make sure that all AXI transfers 
									// initiated on the master interface are completed.Aborting the 
									// channel is not recommended and should be used only in 
									// situations where a particular channel hangs due to no 
									// response from the corresponding AXI slave interface and 
									// software wants to disable the channel without resetting the 
									// entire DW_axi_dmac. It is recommended to try channel 
									// disabling first and then only opt for channel aborting.
									// ¡ö 0: No Channel Abort Request. 
									// ¡ö 1: Request for Channel Abort.
									// DW_axi_dmac clears this bit to 0 once the channel is 
									// aborted (when it sets CH1_Status.CH_ABORTED bit to 1).
									// Values: 
									// ¡ö 0x1 (ENABLE_CH1_ABORT): Request for Channel-1 Abort
									// ¡ö 0x0 (DISABLE_CH1_ABORT): No Request for Channel-1 Abort
									// Exists: DMAX_NUM_CHANNELS > 0
									// Volatile: true
									// Memory Access: {(DMAX_CH_ABORT_EN == 1) ? "read-write" : "read-only"}
	DMAC_CH2_ABORT_SHIFT	= 33,	// [33], Varies, Channel-2 Abort Request.
	DMAC_CH3_ABORT_SHIFT	= 34,	// [34], Varies, Channel-3 Abort Request.
	DMAC_CH4_ABORT_SHIFT	= 35,	// [35], Varies, Channel-4 Abort Request.
	DMAC_CH5_ABORT_SHIFT	= 36,	// [36], Varies, Channel-5 Abort Request.
	DMAC_CH6_ABORT_SHIFT	= 37,	// [37], Varies, Channel-6 Abort Request.
	DMAC_CH7_ABORT_SHIFT	= 38,	// [38], Varies, Channel-7 Abort Request.
	DMAC_CH8_ABORT_SHIFT	= 39,	// [39], Varies, Channel-8 Abort Request.

	DMAC_CH1_ABORT_WE_SHIFT	= 40,	// [40], Varies, This bit is used to write enable the Channel-1 Abort bit.
									// The read back value of this register bit is always 0.
									// Values: 
									// ¡ö 0x1 (ENABLE_WR_CH1_ABORT): Enable  Write to CH1_ABORT bit
									// ¡ö 0x0 (DISABLE_WR_CH1_ABORT): Disable Write to CH1_ABORT bit
									// Exists: DMAX_NUM_CHANNELS > 0
									// Volatile: true
									// Memory Access: {(DMAX_CH_ABORT_EN == 1) ? "write-only" : "read-only"}
	DMAC_CH2_ABORT_WE_SHIFT	= 41,	// [41], Varies, This bit is used to write enable the Channel-2 Abort bit.
	DMAC_CH3_ABORT_WE_SHIFT	= 42,	// [42], Varies, This bit is used to write enable the Channel-3 Abort bit.
	DMAC_CH4_ABORT_WE_SHIFT	= 43,	// [43], Varies, This bit is used to write enable the Channel-4 Abort bit.
	DMAC_CH5_ABORT_WE_SHIFT	= 44,	// [44], Varies, This bit is used to write enable the Channel-5 Abort bit.
	DMAC_CH6_ABORT_WE_SHIFT	= 45,	// [45], Varies, This bit is used to write enable the Channel-6 Abort bit.
	DMAC_CH7_ABORT_WE_SHIFT	= 46,	// [46], Varies, This bit is used to write enable the Channel-7 Abort bit.
	DMAC_CH8_ABORT_WE_SHIFT	= 47,	// [47], Varies, This bit is used to write enable the Channel-8 Abort bit.
	
	DMAC_CHENREG_REVD_SHIFT	= 48	// [63:48], R, DMAC_CHENREG Reserved bits - Read Only
									// Exists: Always
									// Volatile: true
}E_DMAC_CHENREG_SHIFT;

// CH1_EN mask and value
#define DMAC_CH_EN_MASK(ch)		(1<<(ch + DMAC_CH1_EN_SHIFT))
#define DMAC_CH_EN_VAL(ch, val)	((val)<<(ch + DMAC_CH1_EN_SHIFT))

typedef enum
{
	DMAC_CH_EN_DIS	= 0,
	DMAC_CH_EN_EN	= 1
}E_DMAC_CH_EN;

// CH1_EN_WE mask and value
#define DMAC_CH_EN_WE_MASK(ch)		(1<<(ch + DMAC_CH1_EN_WE_SHIFT))
#define DMAC_CH_EN_WE_VAL(ch, val)	((val)<<(ch + DMAC_CH1_EN_WE_SHIFT))

typedef enum
{
	DMAC_CH_EN_WE_DIS	= 0,
	DMAC_CH_EN_WE_EN	= 1
}E_DMAC_CH1_EN_WE;

// CH1_SUSP mask and value
#define DMAC_CH_SUSP_MASK(ch)		(1<<(ch + DMAC_CH1_SUSP_SHIFT))
#define DMAC_CH_SUSP_VAL(ch, val)	((val)<<(ch + DMAC_CH1_SUSP_SHIFT))

typedef enum
{
	DMAC_CH_SUSP_DIS	= 0,
	DMAC_CH_SUSP_EN		= 1
}E_DMAC_CH_SUSP;

// CH1_SUSP_WE mask and value
#define DMAC_CH_SUSP_WE_MASK(ch)		(1<<(ch + DMAC_CH1_SUSP_WE_SHIFT))
#define DMAC_CH_SUSP_WE_VAL(ch, val)	((val)<<(ch + DMAC_CH1_SUSP_WE_SHIFT))

typedef enum
{
	DMAC_CH_SUSP_WE_DIS	= 0,
	DMAC_CH_SUSP_WE_EN	= 1
}E_DMAC_CH_SUSP_WE;

// CH1_ABORTE mask and value
#define DMAC_CH_ABORT_MASK(ch)		(1<<(ch + DMAC_CH1_ABORT_SHIFT - 32))
#define DMAC_CH_ABORT_VAL(ch, val)	((val)<<(ch + DMAC_CH1_ABORT_SHIFT - 32))

typedef enum
{
	DMAC_CH_ABORT_DIS	= 0,
	DMAC_CH_ABORT_EN	= 1
}E_DMAC_CH_ABORT;

// CH1_ABORTE_WE mask and value
#define DMAC_CH_ABORT_WE_MASK(ch)		(1<<(ch + DMAC_CH1_ABORT_WE_SHIFT - 32))
#define DMAC_CH_ABORT_WE_VAL(ch, val)	((val)<<(ch + DMAC_CH1_ABORT_WE_SHIFT - 32))

typedef enum
{
	DMAC_CH_ABORT_WE_DIS	= 0,
	DMAC_CH_ABORT_WE_EN		= 1
}E_DMAC_CH_ABORT_WE;

// offset: 0x030, DMAC Interrupt Status Register captures the combined channel interrupt for each channel and Combined...
typedef enum
{
	DMAC_CH1_INTSTAT_SHIFT  		= 0,	// [0],  R, Channel 1 Interrupt Status Bit.
											// Values: 
											// ¡ö 0x1 (ACTIVE): Channel 1 Interrupt is Active
											// ¡ö 0x0 (INACTIVE): Channel 1 Interrupt is Inactive
											// Exists: Always
											// Volatile: true
	DMAC_CH2_INTSTAT_SHIFT  		= 1,	// [1],  R, Channel 2 Interrupt Status Bit.
	DMAC_CH3_INTSTAT_SHIFT  		= 2,	// [2],  R, Channel 3 Interrupt Status Bit.
	DMAC_CH4_INTSTAT_SHIFT  		= 3,	// [3],  R, Channel 4 Interrupt Status Bit.
	DMAC_CH5_INTSTAT_SHIFT  		= 4,	// [4],  R, Channel 5 Interrupt Status Bit.
	DMAC_CH6_INTSTAT_SHIFT  		= 5,	// [5],  R, Channel 6 Interrupt Status Bit.
	DMAC_CH7_INTSTAT_SHIFT  		= 6,	// [6],  R, Channel 7 Interrupt Status Bit.
	DMAC_INTSTATUSREG_REVD0_SHIFT  	= 7,	// [7],  R, Channel 8 Interrupt Status Bit.
										
	DMAC_COMPVERREG_REVD0_SHIFT		= 8,	// [15:8], -, Reserved Field: Yes

	DMAC_COMMONREG_INTSTAT_SHIFT	= 16,	// [16], R, Common Register Interrupt Status Bit.
											// Values: 
											// ¡ö 0x1 (ACTIVE): Common Register Interrupt is Active
											// ¡ö 0x0 (INACTIVE): Common Register Interrupt is Inactive
											// Exists: Always
											// Volatile: true

	DMAC_COMPVERREG_REVD1_SHIFT		= 17	// [63:17], R, DMAC Interrupt Status Register (bits 63to17) Reserved bits - 
											// Read Only
											// Exists: Always
											// Volatile: true
}E_DMAC_INTSTATUSREG_SHIFT;

// CH1_INTSTAT mask
#define DMAC_CH_INT_STAT_MASK(ch)		(1<<(ch + DMAC_CH1_INTSTAT_SHIFT))


// COMMONREG_INTSTAT mask
#define DMAC_COMMONREG_INT_STAT_MASK	(1<<DMAC_COMMONREG_INTSTAT_SHIFT)


// offset: 0x038, Writing 1 to specific field clears the corresponding field in DMAC Common register Interrupt Status...
typedef enum
{
	DMAC_CLEAR_SLVIF_COMMONREG_DEC_ERR_INTSTAT_SHIFT  		= 0,	// [0],  W, Slave Interface Common Register Decode Error Interrupt clear Bit.
																	// This bit is used to clear the corresponding channel interrupt 
																	// status bit (SLVIF_CommonReg_DEC_ERR_IntStat in 
																	// DMAC_CommonReg_IntStatusReg.
																	// Values: 
																	// ¡ö 0x1 (CLEAR_SLVIF_CommonReg_DEC_ERR): Clear 
																	// the SLVIF_CommonReg_DEC_ERR interrupt in the 
																	// interrupt register DMAC_CommonReg_IntStatusReg
																	// ¡ö 0x0 (No_ACTION): Inactive signal. No action taken.
																	// Exists: Always
										
	DMAC_CLEAR_SLVIF_COMMONREG_WR2RO_ERR_INTSTAT_SHIFT		= 1,	// [1], W, Slave Interface Common Register Write to Read only Error Interrupt clear Bit.
																	// This bit is used to clear the corresponding channel interrupt 
																	// status bit(SLVIF_CommonReg_WR2RO_ERR_IntStat in 
																	// DMAC_CommonReg_IntStatusReg.
																	// Values: 
																	// ¡ö 0x1 (CLEAR_SLVIF_CommonReg_WR2RO_ERR): 
																	// Clear the SLVIF_CommonReg_WR2RO_ERR interrupt in 
																	// the interrupt register DMAC_CommonReg_IntStatusReg
																	// ¡ö 0x0 (No_ACTION): Inactive signal. No action taken.
																	// Exists: Always

	DMAC_CLEAR_SLVIF_COMMONREG_RD2WO_ERR_INTSTAT_SHIFT		= 2,	// [2], W, Slave Interface Common Register Read to Write only Error Interrupt clear Bit.
																	// This bit is used to clear  the corresponding channel interrupt 
																	// status bit(SLVIF_CommonReg_RD2WO_ERR_IntStat in 
																	// DMAC_CommonReg_IntStatusReg.
																	// Values: 
																	// ¡ö 0x1 (CLEAR_SLVIF_CommonReg_RD2WO_ERR): 
																	// Clear the SLVIF_CommonReg_RD2WO_ERR interrupt in 
																	// the interrupt register DMAC_CommonReg_IntStatusReg
																	// ¡ö 0x0 (No_ACTION): Inactive signal. No action taken.
																	// Exists: Always

	DMAC_CLEAR_SLVIF_COMMONREG_WRONHOLD_ERR_INTSTAT_SHIFT	= 3,	// [3], W, Slave Interface Common Register Write On Hold Error Interrupt clear Bit.
																	// This bit is used to clear  the corresponding channel interrupt 
																	// status bit(SLVIF_CommonReg_WrOnHold_ERR_IntStat in 
																	// DMAC_CommonReg_IntStatusReg.
																	// Values: 
																	// ¡ö 0x1 (CLEAR_SLVIF_CommonReg_WrOnHold_ERR): 
																	// Clear the SLVIF_CommonReg_WrOnHold_ERR interrupt 
																	// in the interrupt register 
																	// DMAC_CommonReg_IntStatusReg
																	// ¡ö 0x0 (No_ACTION): Inactive signal. No action taken.
																	// Exists: Always

	RSVD_DMAC_COMMONREG_INTCLEARREG_7TO4_SHIFT				= 4,	// [7:4], W, DMAC Common Register Interrupt Clear Register (bits 7to4) 
																	// Reserved bits - Read Only
																	// Exists: Always

	DMAC_CLEAR_SLVIF_UNDEFINEDREG_DEC_ERR_INTSTAT_SHIFT		= 8,	// [8], W, Slave Interface Undefined register Decode Error Interrupt clear Bit.
																	// This bit is used to clear the corresponding channel interrupt 
																	// status bit(SLVIF_UndefinedReg_DEC_ERR_IntStat in 
																	// DMAC_CommonReg_IntStatusReg.
																	// Values: 
																	// ¡ö 0x1 (CLEAR_SLVIF_UndefinedReg_DEC_ERR): Clear 
																	// the SLVIF_UndefinedReg_DEC_ERR interrupt in the 
																	// interrupt register DMAC_CommonReg_IntStatusReg
																	// ¡ö 0x0 (No_ACTION): Inactive signal. No action taken.
																	// Exists: Always

	RSVD_DMAC_COMMONREG_INTCLEARREG_63TO9					= 9,	// [63:9], W, DMAC Common Register Interrupt Clear Register (bits 63to9) Reserved bits - Read Only
																	// Exists: Always
}E_DMAC_COMMONREG_INTCLEARREG_SHIFT;

// DMAC_COMMONREG_INTCLEARREG mask value
#define DMAC_DEC_ERR_INTSTAT_MASK					(1<<DMAC_CLEAR_SLVIF_COMMONREG_DEC_ERR_INTSTAT_SHIFT)
#define DMAC_WR2RO_ERR_INTSTAT_MASK					(1<<DMAC_CLEAR_SLVIF_COMMONREG_WR2RO_ERR_INTSTAT_SHIFT)
#define DMAC_RD2WO_ERR_INTSTAT_MASK					(1<<DMAC_CLEAR_SLVIF_COMMONREG_RD2WO_ERR_INTSTAT_SHIFT)
#define	DMAC_WRONHOLD_ERR_INTSTAT_MASK				(1<<DMAC_CLEAR_SLVIF_COMMONREG_WRONHOLD_ERR_INTSTAT_SHIFT)
#define DMAC_UNDEFINEDREG_DEC_ERR_INTSTAT_MASK		(1<<DMAC_CLEAR_SLVIF_UNDEFINEDREG_DEC_ERR_INTSTAT_SHIFT)

#define DMAC_DEC_ERR_INTSTAT_VAL(val)				((val)<<DMAC_CLEAR_SLVIF_COMMONREG_DEC_ERR_INTSTAT_SHIFT)
#define DMAC_WR2RO_ERR_INTSTAT_VAL(val)				((val)<<DMAC_CLEAR_SLVIF_COMMONREG_WR2RO_ERR_INTSTAT_SHIFT)
#define DMAC_RD2WO_ERR_INTSTAT_VAL(val)				((val)<<DMAC_CLEAR_SLVIF_COMMONREG_RD2WO_ERR_INTSTAT_SHIFT)
#define	DMAC_WRONHOLD_ERR_INTSTAT_VAL(val)			((val)<<DMAC_CLEAR_SLVIF_COMMONREG_WRONHOLD_ERR_INTSTAT_SHIFT)
#define DMAC_UNDEFINEDREG_DEC_ERR_INTSTAT_VAL(val)	((val)<<DMAC_CLEAR_SLVIF_UNDEFINEDREG_DEC_ERR_INTSTAT_SHIFT)

// DMAC_COMMONREG_INTCLEARREG 
typedef enum
{
	DMAC_CLEAR_DIS	= 0,
	DMAC_CLEAR_EN	= 1
}E_DMAC_CLEAR_SLVIF_COMMONREG;

// DMAC_COMMONREG_INTSTATUS_ENABLEREG 
typedef enum
{
	DMAC_INTST_DIS	= 0,
	DMAC_INTST_EN	= 1
}E_DMAC_COMMONREG_INTSTATUS_ENABLEREG;

// DMAC_COMMONREG_INTSIGNAL_ENABLEREG
typedef enum
{
	DMAC_INTSIGNAL_DIS	= 0,
	DMAC_INTSIGNAL_EN	= 1
}E_DMAC_COMMONREG_INTSIGNAL_ENABLEREG;

// offset: 0x058, This register is used to initiate the Software Reset to DW_axi_dmac
typedef enum
{
	DMAC_RST_SHIFT  		= 0,	// [0],  R/W, DMAC Reset Request bit
									// Software writes 1 to this bit to reset the DW_axi_dmac and 
									// polls this bit to see it as 0. DW_axi_dmac resets all the 
									// modules except the slave bus interface module and clears this bit to 0.
									// NOTE: Software is not allowed to write 0 to this bit.
									// Exists: Always
									// Volatile: true
										
	RSVD_DMAC_ResetReg_1to63_SHIFT	= 32	// [63:1], R, DMAC_ResetReg (bits 1to63) Reserved bits - Read Only
}E_DMAC_RESETREG;

// reset mask
#define DMAC_RST_MASK	(1<<DMAC_RST_SHIFT) 

// offset: 0x110, When DW_axi_dmac is the flow controller, the DMAC uses this register before the channel is enabled...
typedef enum
{
	DMAC_BLOCK_TS_SHIFT  					= 0,	// [21:0],  R/W, Block Transfer Size.
													// The number programmed into BLOCK_TS field indicates the 
													// total number of data of width CHx_CTL.SRC_TR_WIDTH to 
													// be transferred in a DMA block transfer.
													// Block Transfer Size = BLOCK_TS+1
													// When the transfer starts, the read-back value is the total 
													// number of data items already read from the source 
													// peripheral, regardless of who is the flow controller. When the 
													// source or destination peripheral is assigned as the flow 
													// controller, the value before the transfer starts saturates at 
													// DMAX_CHx_MAX_BLK_SIZE, but the actual block size can be greater.
													// Exists: Always
													// Volatile: true
										
	RSVD_DMAC_CHx_BLOCK_TSREG_63to22_SHIFT	= 22	// [63:22], R, DMAC Channelx Block Transfer Size Register (bits 63to22) Reserved bits - Read Only
}E_CH_BLOCK_TS;

// BLOCK_TS mask and value
#define DMAC_BLOCK_TS_MASK 		(0x3FFFFF << DMAC_BLOCK_TS_SHIFT)
#define DMAC_BLOCK_TS_VAL(val) 	((val) << DMAC_BLOCK_TS_SHIFT)

// offset: 0x118, This register contains fields that control the DMA transfer. This register should be programmed...
typedef enum
{
	DMAC_SMS_SHIFT 							= 0,	// [0], Varies, Source Master Select.
													// Identifies the Master Interface layer from which the source 
													// device (peripheral or memory) is accessed.
													// ¡ö 0: AXI master 1 
													// ¡ö 1: AXI Master 2
													// Values: 
													// ¡ö 0x1 (MASTER2_INTF): Source device on Master-2 interface layer
													// ¡ö 0x0 (MASTER1_INTF): Source device on Master-1 interface layer
	
	RSVD_DMAC_CHX_CTL_1_SHIFT 				= 1,	// [1], R, DMAC Channelx Control Transfer Register bit1 Reserved bits - Read Only
	
	DMAC_DMS_SHIFT 							= 2,	// [2],  Varies, Destination Master Select.
													// Identifies the Master Interface layer from which the 
													// destination device (peripheral or memory) is accessed.
													// ¡ö 0: AXI master 1 
													// ¡ö 1: AXI Master 2
													// Values: 
													// ¡ö 0x1 (MASTER2_INTF): Destination device on Master-2 interface layer
													// ¡ö 0x0 (MASTER1_INTF): Destination device on Master-1 interface layer
	
	RSVD_DMAC_CHX_CTL_3_SHIFT 				= 3,	// [3],  R, DMAC Channelx Control Transfer Register bit3 Reserved bits - Read Only
	
	DMAC_SINC_SHIFT 						= 4,	// [4],  R/W, Source Address Increment.
													// Indicates whether to increment the source address on every 
													// source transfer. If the device is fetching data from a source 
													// peripheral FIFO with a fixed address, then set this field to 'No change'.
													// ¡ö 0: Increment 
													// ¡ö 1: No Change
													// NOTE: Increment aligns the address to the next CHx_CTL.SRC_TR_WIDTH boundary.
													// Values: 
													// ¡ö 0x1 (INCREMENTAL): Source address incremented on every source transfer
													// ¡ö 0x0 (FIXED): Source address is fixed

	RSVD_DMAC_CHX_CTL_5_SHIFT 				= 5,	// [5],  R, DMAC Channelx Control Transfer Register bit5 Reserved bits - Read Only
	
	DMAC_DINC_SHIFT 						= 6,	// [6],  R/W, Destination Address Increment.
													// Indicates whether to increment the destination address on 
													// every destination transfer. If the device is writing data from a 
													// source peripheral FIFO with a fixed address, then set this field to 'No change'.
													// ¡ö 0: Increment 
													// ¡ö 1: No Change
													// NOTE: Increment aligns the address to the next 
													// CHx_CTL.DST_TR_WIDTH boundary.
													// Values: 
													// ¡ö 0x1 (INCREMENTAL): Destination address incremented on every source transfer
													// ¡ö 0x0 (FIXED): Destination address is fixed

	RSVD_DMAC_CHX_CTL_7_SHIFT 				= 7,	// [7],  R, DMAC Channelx Control Transfer Register bit7 Reserved bits - Read Only
	
	DMAC_SRC_TR_WIDTH_SHIFT 				= 8,	// [10:8],  Varies, Source Transfer Width.
													// Mapped to AXI bus arsize, this value must be less than or equal to DMAX_M_DATA_WIDTH.
													// Values: 
													// ¡ö 0x0 (BITS_8): Source Transfer Width is 8 bits
													// ¡ö 0x1 (BITS_16): Source Transfer Width is 16 bits
													// ¡ö 0x2 (BITS_32): Source Transfer Width is 32 bits
													// ¡ö 0x3 (BITS_64): Source Transfer Width is 64 bits
													// ¡ö 0x4 (BITS_128): Source Transfer Width is 128 bits
													// ¡ö 0x5 (BITS_256): Source Transfer Width is 256 bits
													// ¡ö 0x6 (BITS_512): Source Transfer Width is 512 bits
	
	DMAC_DST_TR_WIDTH_SHIFT 				= 11,	// [13:11],  Varies, Destination Transfer Width.
													// Mapped to AXI bus awsize, this value must be less than or equal to DMAX_M_DATA_WIDTH.
													// Values: 
													// ¡ö 0x0 (BITS_8): Destination Transfer Width is 8 bits
													// ¡ö 0x1 (BITS_16): Destination Transfer Width is 16 bits
													// ¡ö 0x2 (BITS_32): Destination Transfer Width is 32 bits
													// ¡ö 0x3 (BITS_64): Destination Transfer Width is 64 bits
													// ¡ö 0x4 (BITS_128): Destination Transfer Width is 128 bits
													// ¡ö 0x5 (BITS_256): Destination Transfer Width is 256 bits
													// ¡ö 0x6 (BITS_512): Destination Transfer Width is 512 bits
	
	DMAC_SRC_MSIZE_SHIFT 					= 14,	// [17:14],  R/W, Source Burst Transaction Length.
													// Number of data items, each of width 
													// CHx_CTL.SRC_TR_WIDTH, to be read from the source 
													// every time a source burst transaction request is made from 
													// the corresponding hardware or software handshaking 
													// interface. The maximum value of DST_MSIZE is limited by 
													// DMAX_CHx_MAX_MULT_SIZE.
													// Note: This Value is not related to the AXI arlen signal.
													// Values: 
													// ¡ö 0x0 (DATA_ITEM_1): 1 Data Item read from Source in the burst transaction
													// ¡ö 0x1 (DATA_ITEMS_4): 4 Data Item read from Source in the burst transaction
													// ¡ö 0x2 (DATA_ITEMS_8): 8 Data Item read from Source inthe burst transaction
													// ¡ö 0x3 (DATA_ITEMS_16): 16 Data Item read from Source in the burst transaction
													// ¡ö 0x4 (DATA_ITEMS_32): 32 Data Item read from Source in the burst transaction
													// ¡ö 0x5 (DATA_ITEMS_64): 64 Data Item read from Source in the burst transaction
													// ¡ö 0x6 (DATA_ITEMS_128): 128 Data Item read from Source in the burst transaction
													// ¡ö 0x7 (DATA_ITEMS_256): 256 Data Item read from Source in the burst transaction
													// ¡ö 0x8 (DATA_ITEMS_512): 512 Data Item read from Source in the burst transaction
													// ¡ö 0x9 (DATA_ITEMS_1024): 1024 Data Item read from Source in the burst transaction
	
	DMAC_DST_MSIZE_SHIFT 					= 18,	// [21:18],  R/W, Destination Burst Transaction Length.
													// Number of data items, each of width 
													// CHx_CTL.DST_TR_WIDTH, to be written to the destination 
													// every time a destination burst transaction request is made 
													// from the corresponding hardware or software handshaking 
													// interface.Note: This Value is not related to the AXI awlen signal.
													// Values: 
													// ¡ö 0x0 (DATA_ITEM_1): 1 Data Item read from Destination in the burst transaction
													// ¡ö 0x1 (DATA_ITEMS_4): 4 Data Item read from Destination in the burst transaction
													// ¡ö 0x2 (DATA_ITEMS_8): 8 Data Item read from Destination in the burst transaction
													// ¡ö 0x3 (DATA_ITEMS_16): 16 Data Item read from Destination in the burst transaction
													// ¡ö 0x4 (DATA_ITEMS_32): 32 Data Item read from Destination in the burst transaction
													// ¡ö 0x5 (DATA_ITEMS_64): 64 Data Item read from Destination in the burst transaction
													// ¡ö 0x6 (DATA_ITEMS_128): 128 Data Item read from Destination in the burst transaction
													// ¡ö 0x7 (DATA_ITEMS_256): 256 Data Item read from Destination in the burst transaction
													// ¡ö 0x8 (DATA_ITEMS_512): 512 Data Item read from Destination in the burst transaction
													// ¡ö 0x9 (DATA_ITEMS_1024): 1024 Data Item read from Destination in the burst transaction
	
	DMAC_AR_CACHE_SHIFT 					= 22,	// [25:22],  R/W, AXI 'ar_cache' signal
	
	DMAC_AW_CACHE_SHIFT 					= 26,	// [29:26],  R/W, AXI 'aw_cache' signal
	
	DMAC_NONPOSTED_LASTWRITE_EN_SHIFT 		= 30,	// [30],  R/W, Non Posted Last Write Enable
													// This bit decides whether posted writes can be used 
													// throughout the block transfer.
													// ¡ö 0: Posted writes may be used throughout the block transfer.
													// ¡ö 1: Posted writes may be used till the end of the block 
													// (inside a block) and the last write in the block must be 
													// non-posted. This is to synchronize block completion 
													// interrupt generation to the last write data reaching the 
													// end memory/peripheral.
													// Values: 
													// ¡ö 0x1 (Enable): Last write in the block must be non-posted
													// ¡ö 0x0 (Disable): Posted writes may be used throughout the block transfer
	
	DMAC_RSVD_DMAC_CHX_CTL_31_SHIFT 		= 31,	// [31],  R, DMAC Channelx Control Transfer Register bit31 Reserved bits - Read Only


	DMAC_AR_PROT_SHIFT 						= 32,	// [34:32],  R/W, AXI 'ar_prot' signal
	
	DMAC_AW_PROT_SHIFT 						= 35,	// [37:35],  R/W, AXI 'aw_prot' signal
		
	DMAC_ARLEN_EN_SHIFT 					= 38,	// [38],  R/W, Source Burst Length Enable
													// If this bit is set to 1, DW_axi_dmac uses the value of 
													// CHx_CTL.ARLEN as AXI Burst length for source data 
													// transfer till the extent possible; remaining transfers use 
													// maximum possible burst length.
													// If this bit is set to 0, DW_axi_dmac uses any possible value 
													// that is less than or equal to 
													// DMAX_CHx_MAX_AMBA_BURST_LENGTH as AXI Burst 
													// length for source data transfer.
													// Values: 
													// ¡ö 0x1 (Enable): AXI Burst Length is CH1_CTL.ARLEN (till the extent possible) for Source data transfers
													// ¡ö 0x0 (Disable): AXI Burst Length is any possible value <= DMAX_CH1_MAX_AMBA_BURST_LENGTH for Source data transfers
														
	DMAC_ARLEN_SHIFT 						= 39,	// [46:39],  R/W, Source Burst Length
													// AXI Burst length used for source data transfer. The specified 
													// burst length is used for source data transfer till the extent 
													// possible; remaining transfers use maximum possible value 
													// that is less than or equal to 
													// DMAX_CHx_MAX_AMBA_BURST_LENGTH.
													// The maximum value of ARLEN is limited by 
													// DMAX_CHx_MAX_AMBA_BURST_LENGTH

	DMAC_AWLEN_EN_SHIFT  					= 47,	// [47],  R/W, Destination Burst Length Enable
													// If this bit is set to 1, DW_axi_dmac uses the value of 
													// CHx_CTL.AWLEN as AXI Burst length for destination data 
													// transfer till the extent possible; remaining transfers use 
													// maximum possible burst length.
													// If this bit is set to 0, DW_axi_dmac uses any possible value 
													// which is less than or equal to 
													// DMAX_CHx_MAX_AMBA_BURST_LENGTH as AXI Burst 
													// length for destination data transfer.
													// Values: 
													// ¡ö 0x1 (Enable): AXI Burst Length is CH1_CTL.AWLEN (till the extent possible) for Destination data transfers
													// ¡ö 0x0 (Disable): AXI Burst Length is any possible value <= DMAX_CH1_MAX_AMBA_BURST_LENGTH for Destination data transfers
	
	DMAC_AWLEN_SHIFT  						= 48,	// [55:48],  R/W, Destination Burst Length
													// AXI Burst length used for destination data transfer. The 
													// specified burst length is used for destination data transfer till 
													// the extent possible; remaining transfers use maximum 
													// possible value that is less than or equal to 
													// DMAX_CHx_MAX_AMBA_BURST_LENGTH.
													// The maximum value of AWLEN is limited by 
													// DMAX_CHx_MAX_AMBA_BURST_LENGTH.
													// Note: The AWLEN setting may not be honored towards end-
													// to-block transfers, the end of a transaction (only applicable to 
													// non-memory peripharals), and during 4K boundary crossings.
	
	DMAC_SRC_STAT_EN_SHIFT  				= 56,	// [56],  Varies, Source Status Enable
													// Enable the logic to fetch status from source peripheral of 
													// channel x pointed to by the content of CHx_SSTATAR 
													// register and stores it in CHx_SSTAT register. This value is 
													// written back to the CHx_SSTAT location of linked list at end 
													// of each block transfer if DMAX_CHx_LLI_WB_EN is set to 1 
													// and if linked list based multi-block transfer is used by either 
													// source or destination peripheral.
													// Values: 
													//¡ö 0x1 (Enable_STAT_FETCH): Enables status fetch for Source and store the value in CH1_SSTAT register
													//¡ö 0x0 (NO_STAT_FETCH): No status fetch for Source device
	
	DMAC_DST_STAT_EN_SHIFT  				= 57,	// [57],  Varies, Destination Status Enable
													// Enable the logic to fetch status from destination peripheral of 
													// channel x pointed to by the content of CHx_DSTATAR 
													// register and stores it in CHx_DSTAT register. This value is 
													// written back to the CHx_DSTAT location of linked list at end 
													// of each block transfer if DMAX_CHx_LLI_WB_EN is set to 1 
													// and if linked list based multi-block transfer is used by either 
													// source or destination peripheral.
													// Values: 
													// ¡ö 0x1 (Enable_STAT_FETCH): Enables status fetch for 
													// Destination and store the value in CH1_DSTAT register
													// ¡ö 0x0 (NO_STAT_FETCH): No status fetch for Destination 
													// device
	
	DMAC_IOC_BLKTFR_SHIFT  					= 58,	// [58],  R/W, Interrupt On completion of Block Transfer
													// This bit is used to control the block transfer completion 
													// interrupt generation on a block by block basis for shadow 
													// register or linked list based multi-block transfers. Writing 1 to 
													// this register field enables 
													// CHx_IntStatusReg.BLOCK_TFR_DONE_IntStat field if this 
													// interrupt generation is enabled in CHx_IntStatus_EnableReg 
													// register and the external interrupt output is is asserted if this 
													// interrupt generation is enabled in CHx_IntSignal_EnableReg 
													// register.
													// Note: If a linked-list or shadow-register-based multi-block 
													// transfer is not used for both source and destination (for 
													// instance if source and destination use contiguous address or 
													// auto-reload-based multi-block transfer), the value of this field 
													// cannot be modified per block. Additionally, the value 
													// programmed before the channel is enabled is used for all the 
													// blocks in the DMA transfer.
													// Values: 
													// ¡ö 0x1 (Enable_BLKTFR_INTR): Enables CHx_IntStatusReg.BLOCK_TFR_DONE_IntStat field
													// ¡ö 0x0 (DISABLE_BLKTFR_INTR): Disables CHx_IntStatusReg.BLOCK_TFR_DONE_IntStat field
													// Exists: Always
		
	RSVD_DMAC_CHX_CTL_59TO61_SHIFT  		= 59,	// [61:59],  R  ,DMAC Channelx Control Transfer Register (bits 59to61) 
													// Reserved bits - Read Only
													// Exists: Always
	
	DMAC_SHADOWREG_OR_LLI_LAST_SHIFT  		= 62,	// [62],  R/W, Last Shadow Register/Linked List Item.
													// Indicates whether shadow register content or the linked list 
													// item fetched from the memory is the last one or not.
													// ¡ö 0: Not last Shadow Register/LLI
													// ¡ö 1: Last Shadow Register/LLI
										
	DMAC_SHADOWREG_OR_LLI_VALID_SHIFT		= 63	// [63], R/W, Shadow Register content/Linked List Item valid.
													// Indicates whether the content of shadow register or
													// linked list item fetched from the memory is valid.
													// ¡ö 0: Shadow Register content/LLI is invalid.
													// ¡ö 1: Last Shadow Register/LLI is valid.
}E_CH_CTL;

// bit [0], Source Master Select.
// bit [2], Destination Master Select.
typedef enum {
	MS0 	= 0,
	MS1 	= 1
}E_DMA_MS;

// bit [4], Source Address Increment.
// bit [6], Destination Address Increment.
typedef enum {
	ADDR_INC 	= 0,
	ADDR_FIXED 	= 1
}E_DMA_ADDR;

// bit[10: 8], Source transfer width mask and value
// bit[13: 11], Destination transfer width mask and value
typedef enum {
	DMA_WIDTH_BITS_8 	= 0,
	DMA_WIDTH_BITS_16 	= 1,
	DMA_WIDTH_BITS_32 	= 2,
	DMA_WIDTH_BITS_64 	= 3,
	DMA_WIDTH_BITS_128	= 4,
	DMA_WIDTH_BITS_256 	= 5,
	DMA_WIDTH_BITS_512 	= 6
}E_DMA_WIDTH;

// bit[17:14], Source Burst Transaction Length.
// bit[21:18], Destination Burst Transaction Length.
typedef enum {
	DATA_ITEM_1 	= 0, // 1 Data Item read from Source in the burst transaction
	DATA_ITEMS_4 	= 1,
	DATA_ITEMS_8 	= 2,
	DATA_ITEMS_16 	= 3,
	DATA_ITEMS_32	= 4,
	DATA_ITEMS_64 	= 5,
	DATA_ITEMS_128 	= 6,
	DATA_ITEMS_256 	= 7,
	DATA_ITEMS_512 	= 8,
	DATA_ITEMS_1024 = 9
}E_DMA_MSIZE;

// bit[58], Interrupt On completion of Block Transfer
typedef enum {
	BLKTFR_INTR_DIS 	= 0, // 0:  Disables CHx_IntStatusReg.BLOCK_TFR_DONE_IntStat field
	BLKTFR_INTR_EN 		= 1	 // 1:  Enables CHx_IntStatusReg.BLOCK_TFR_DONE_IntStat field
}E_DMA_IOC_BLKTFR;


// bit[62], Last Shadow Register/Linked List Item.
typedef enum {
	LLI_NOT_LAST 	= 0, // 0:  Not last Shadow Register/LLI
	LLI_LAST 		= 1	 // 1:  Last Shadow Register/LLI
}E_DMA_LLI_LAST;


// bit[63], Shadow Register content/Linked List Item valid
typedef enum {
	LLI_INVALID 	= 0, // 0: Shadow Register content/LLI is invalid.
	LLI_VALID 		= 1	 // 1: Last Shadow Register/LLI is valid.
}E_DMA_LLI_VALID;

// low 32 bits
#define DMA_CHX_CTL_L32BITS_VAL(dmsize, smsize, dwidth, swidth, dinc, sinc, dms, sms)\
			(((0) << DMAC_NONPOSTED_LASTWRITE_EN_SHIFT)|\
			((dmsize) << DMAC_DST_MSIZE_SHIFT)|\
			((smsize) << DMAC_SRC_MSIZE_SHIFT)|\
			((dwidth) << DMAC_DST_TR_WIDTH_SHIFT)|\
			((swidth) << DMAC_SRC_TR_WIDTH_SHIFT)|\
			((dinc) << DMAC_DINC_SHIFT)|\
			((sinc) << DMAC_SINC_SHIFT)|\
			((dms) << DMAC_DMS_SHIFT)|\
			((sms) << DMAC_SMS_SHIFT))

// high 32 bits
#define DMA_CHX_CTL_H32BITS_VAL(dbust_len, sbust_len)\
			(((LLI_INVALID) << (DMAC_SHADOWREG_OR_LLI_VALID_SHIFT - 32))|\
			((LLI_NOT_LAST) << (DMAC_SHADOWREG_OR_LLI_LAST_SHIFT - 32))|\
			((BLKTFR_INTR_DIS) << (DMAC_IOC_BLKTFR_SHIFT - 32))|\
			((0) << (DMAC_DST_STAT_EN_SHIFT - 32))|\
			((0) << (DMAC_SRC_STAT_EN_SHIFT - 32))|\
			((dbust_len) << (DMAC_AWLEN_SHIFT - 32))|\
			((0) << (DMAC_AWLEN_EN_SHIFT - 32))|\
			((sbust_len) << (DMAC_ARLEN_SHIFT - 32))|\
			((0) << (DMAC_ARLEN_EN_SHIFT - 32))|\
			((0) << (DMAC_AW_PROT_SHIFT - 32))|\
			((0) << (DMAC_AR_PROT_SHIFT - 32)))

// offset: 0x120, This register contains fields that configure the DMA transfer. This register should be programmed...
typedef enum
{
	DMAC_SRC_MULTBLK_TYPE_SHIFT	= 0,	// [1:0], Varies, Source Multi Block Transfer Type.
										// These bits define the type of multi-block transfer used for source peripheral.
										// ¡ö 00: Contiguous
										// ¡ö 01: Reload
										// ¡ö 10: Shadow Register
										// ¡ö 11: Linked List
										// If the type selected is Contiguous, the CHx_SAR register is 
										// loaded with the value of the end source address of previous 
										// block + 1 at the end of every block for multi-block transfers. A 
										// new block transfer is then initiated.
										// If the type selected is Reload, the CHx_SAR register is 
										// reloaded from the initial value of SAR at the end of every 
										// block for multi-block transfers. A new block transfer is then initiated.
										// If the type selected is Shadow Register, the CHx_SAR 
										// register is loaded from the content of its shadow register if 
										// CHx_CTL.ShadowReg_Or_LLI_Valid bit is set to 1 at the 
										// end of every block for multi-block transfers. A new block 
										// transfer is then initiated.
										// If the type selected is Linked List, the CHx_SAR register is 
										// loaded from the Linked List if 
										// CTL.ShadowReg_Or_LLI_Valid bit is set to 1 at the end of 
										// every block for multi-block transfers. A new block transfer is then initiated.
										// CHx_CTL and CHx_BLOCK_TS registers are loaded from 
										// their initial values or from the contents of their shadow 
										// registers (if CHx_CTL.ShadowReg_Or_LLI_Valid bit is set to 
										// 1) or from the linked list (if CTL.ShadowReg_Or_LLI_Valid 
										// bit is set to 1) at the end of every block for multi-block 
										// transfers based on the multi-block transfer type programmed 
										// for source and destination peripherals.
										// Contiguous transfer on both source and destination 
										// peripheral is not a valid multi-block transfer configuration.

										// This field does not exist if the configuration parameter 
										// DMAX_CHx_MULTI_BLK_EN is not selected; in that case, the read-back value is always 0.
										// Values: 
										// ¡ö 0x0 (CONTINGUOUS): Contiguous Multiblock Type used for Source Transfer
										// ¡ö 0x1 (RELOAD): Reload Multiblock Type used for Source Transfer
										// ¡ö 0x2 (SHADOW_REGISTER): Shadow Register based Multiblock Type used for Source Transfer
										// ¡ö 0x3 (LINKED_LIST): Linked List based Multiblock Type used for Source Transfer	
	
	DMAC_DST_MULTBLK_TYPE_SHIFT	= 2,	// [3:2], Varies, Destination Multi Block Transfer Type.
										// These bits define the type of multi-block transfer used for destination peripheral.
										// ¡ö 00: Contiguous
										// ¡ö 01: Reload
										// ¡ö 10: Shadow Register
										// ¡ö 11: Linked List
										// If the type selected is Contiguous, the CHx_DAR register is 
										// loaded with the value of the end source address of previous 
										// block + 1 at the end of every block for multi-block transfers. A 
										// new block transfer is then initiated.
										// If the type selected is Reload, the CHx_DAR register is 
										// reloaded from the initial value of DAR at the end of every 
										// block for multi-block transfers. A new block transfer is then initiated.
										// If the type selected is Shadow Register, the CHx_DAR 
										// register is loaded from the content of its shadow register if 
										// CHx_CTL.ShadowReg_Or_LLI_Valid bit is set to 1 at the 
										// end of every block for multi-block transfers. A new block 
										// transfer is then initiated.
										// If the type selected is Linked List, the CHx_DAR register is 
										// loaded from the Linked List if 
										// CTL.ShadowReg_Or_LLI_Valid bit is set to 1 at the end of 
										// every block for multi-block transfers. A new block transfer is 
										// then initiated.
										// CHx_CTL and CHx_BLOCK_TS registers are loaded from 
										// their initial values or from the contents of their shadow 
										// registers (if CHx_CTL.ShadowReg_Or_LLI_Valid bit is set to 
										// 1) or from the linked list (if CTL.ShadowReg_Or_LLI_Valid 
										// bit is set to 1) at the end of every block for multi-block 
										// transfers based on the multi-block transfer type programmed 
										// for source and destination peripherals.
										// Contiguous transfer on both source and destination 
										// peripheral is not a valid multi-block transfer configuration

										// This field does not exist if the configuration parameter 
										// DMAX_CHx_MULTI_BLK_EN is not selected; in that case, 
										// the read-back value is always 0.
										// Values: 
										// ¡ö 0x0 (CONTINGUOUS): Contiguous Multiblock Type used for Destination Transfer
										// ¡ö 0x1 (RELOAD): Reload Multiblock Type used for Destination Transfer
										// ¡ö 0x2 (SHADOW_REGISTER): Shadow Register based Multiblock Type used for Destination Transfer
										// ¡ö 0x3 (LINKED_LIST): Linked List based Multiblock Type used for Destination Transfer										
	
	RSVD_DMAC_CHX_CFG_4TO31_SHIFT	= 4,	// [31:4], R, DMAC Channelx Transfer Configuration Register (bits 4to31) Reserved bits - Read Only
	
	DMAC_TT_FC_SHIFT			= 32,	// [34:32], Varies, Transfer Type and Flow Control.
										// The following transfer types are supported.
										// ¡ö Memory to Memory 
										// ¡ö Memory to Peripheral 
										// ¡ö Peripheral to Memory 
										// ¡ö Peripheral to Peripheral 
										// Flow Control can be assigned to the DW_axi_dmac, the 
										//source peripheral, or hte destination peripheral.
										//Values: 
										// ¡ö 0x0 (MEM_TO_MEM_DMAC): Transfer Type is memory to memory and Flow Controller is DW_axi_dmac
										// ¡ö 0x1 (MEM_TO_PER_DMAC): Transfer Type is memory to peripheral and Flow Controller is DW_axi_dmac
										// ¡ö 0x2 (PER_TO_MEM_DMAC): Transfer Type is peripheral to memory and Flow Controller is DW_axi_dmac
										// ¡ö 0x3 (PER_TO_PER_DMAC): Transfer Type is peripheral to peripheral and Flow Controller is DW_axi_dmac
										// ¡ö 0x4 (PER_TO_MEM_SRC): Transfer Type is peripheral to Memory and Flow Controller is Source peripheral
										// ¡ö 0x5 (PER_TO_PER_SRC): Transfer Type is peripheral to peripheral and Flow Controller is Source peripheral
										// ¡ö 0x6 (MEM_TO_PER_DST): Transfer Type is memory to peripheral and Flow Controller is Destination peripheral
										// ¡ö 0x7 (PER_TO_PER_DST): Transfer Type is peripheral to peripheral and Flow Controller is Destination peripheral
	
	DMAC_HS_SEL_SRC_SHIFT		= 35,	// [35], R/W, Source Software or Hardware Handshaking Select.
										// This register selects which of the handshaking interfaces 
										// (hardware or software) is active for source requests on this channel.
										// ¡ö 0: Hardware handshaking interface. Software-initiated transaction requests are ignored.
										// ¡ö 1: Software handshaking interface. Hardware-initiated transaction requests are ignored.
										// If the source peripheral is memory, then this bit is ignored.
										// Values: 
										// ¡ö 0x0 (HARDWARE_HS): Hardware Handshaking Interface is used for the Source peripheral
										// ¡ö 0x1 (SOFTWARE_HS): Software Handshaking Interface is used for the Source peripheral

	DMAC_HS_SEL_DST_SHIFT		= 36,	// [36], R/W, Destination Software or Hardware Handshaking Select.
										// This register selects which of the handshaking interfaces 
										// (hardware or software) is active for destination requests on this channel.
										// ¡ö 0: Hardware handshaking interface. Software-initiated transaction requests are ignored.
										// ¡ö 1: Software handshaking interface. Hardware-initiated transaction requests are ignored.
										//If the destination peripheral is memory, then this bit is ignored.
										// Values: 
										// ¡ö 0x0 (HARDWARE_HS): Hardware Handshaking Interface is used for the Destination peripheral
										// ¡ö 0x1 (SOFTWARE_HS): Software Handshaking Interface is used for the Destination peripheral
	
	DMAC_SRC_HWHS_POL_SHIFT		= 37,	// [37], R, Source Hardware Handshaking Interface Polarity.
										// ¡ö 0: ACTIVE HIGH
										// ¡ö 1: ACTIVE LOW
										// Values: 
										// ¡ö 0x0 (ACTIVE_HIGH): Polarity of the Handshaking 
										// Interface used for the Source peripheral is Active High
										// ¡ö 0x1 (ACTIVE_LOW): Polarity of the Handshaking 
										// Interface used for the Source peripheral is Active Low
	
	DMAC_DST_HWHS_POL_SHIFT		= 38,	// [38], R, Destination Hardware Handshaking Interface Polarity.
										// ¡ö 0: ACTIVE HIGH
										// ¡ö 1: ACTIVE LOW
										// Values: 
										// ¡ö 0x0 (ACTIVE_HIGH): Polarity of the Handshaking 
										// Interface used for the Destination peripheral is Active High
										// ¡ö 0x1 (ACTIVE_LOW): Polarity of the Handshaking 
										//Interface used for the Destination peripheral is Active Low
	
	DMAC_SRC_PER_SHIFT			= 39,	// [42:39], Varies, Assigns a hardware handshaking interface (0 - 
										// DMAX_NUM_HS_IF-1) to the source of Channelx if the 
										// CHx_CFG.HS_SEL_SRC field is 0; otherwise, this field is 
										// ignored. The channel can then communicate with the source 
										// peripheral connected to that interface through the assigned 
										// hardware handshaking interface.
										//     Reset Value = 1
										// Note: For correct DW_axi_dmac operation, only one 
										// peripheral (source or destination) should be assigned to the 
										// same handshaking interface.
										// This field does not exist if the configuration parameter 
										// DMAX_NUM_HS_IF is set to 0.
										// x = 39 if DMAC_NUM_HS_IF is 1
										// x = ceil(log2(DMAC_NUM_HS_IF) + 38 if 
										// DMAC_NUM_HS_IF is greater than 1.
										// Bits 42: (x+1) do not exist and return 0 on a read.
	
	RSVD_DMAC_CHx_CFG_43_SHIFT	= 43,	// [43], R, DMAC Channelx Transfer Configuration Register (43bit) Reserved bit - Read Only
	
	DMAC_DST_PER_SHIFT			= 44,	// [47:44], Varies, Assigns a hardware handshaking interface (0 - 
										// DMAX_NUM_HS_IF-1) to the destination of Channelx if the 
										// CHx_CFG.HS_SEL_DST field is 0; otherwise, this field is 
										// ignored. The channel can then communicate with the 
										// destination peripheral connected to that interface through the 
										// assigned hardware handshaking interface.
										//     Reset Value = 1
										// Note: For correct DW_axi_dmac operation, only one 
										// peripheral (source or destination) should be assigned to the 
										// same handshaking interface. 
										// This field does not exist if the configuration parameter 
										// DMAX_NUM_HS_IF is set to 0.
										// x = 44 if DMAC_NUM_HS_IF is 1
										// x = ceil(log2(DMAC_NUM_HS_IF)) + 43 if 
										// DMAC_NUM_HS_IF is greater than 1
										// Bits 47: (x+1) do not exist and return 0 on a read
	
	RSVD_DMAC_CHx_CFG_48_SHIFT	= 48,	// [48], R, DMAC Channelx Transfer Configuration Register (48bit) Reserved bit - Read Only
	
	DMAC_CH_PRIOR_SHIFT	= 49,	// [51:49], R/W, Channel Priority
								// A priority of 7 is the highest priority, and 0 is the lowest. This 
								// field must be programmed within the following range:
								//     0: DMAX_NUM_CHANNELS-1
								//A programmed value outside this range will cause erroneous behavior.

	DMAC_LOCK_CH_SHIFT	= 52,	// [52], Varies, Channel Lock bit
								// When the channel is granted control of the master bus 
								// interface and if the CHx_CFG.LOCK_CH bit is asserted, 
								//then no other channels are granted control of the master bus 
								// interface for the duration specified in 
								// CHx_CFG.LOCK_CH_L. Indicates to the master bus 
								// interface arbiter that this channel wants exclusive access to 
								// the master bus interface for the duration specified in 
								// CHx_CFG.LOCK_CH_L.
								// This field does not exist if the configuration parameter 
								// DMAX_CHx_LOCK_EN is set to False; in this case, the 
								// read-back value is always 0.
								// Locking the channel locks AXI Read Address, Write Address 
								// and Write Data channels on the corresponding master 
								// interface.
								// Note: Channel locking feature is supported only for memory-
								// to-memory transfer at Block Transfer and DMA Transfer 
								// levels. Hardware does not check for the validity of channel 
								// locking setting, hence the software must take care of 
								// enabling the channel locking only for memory-to-memory 
								// transfers at Block Transfer or DMA Transfer levels. Illegal 
								// programming of channel locking might result in unpredictable behavior.
								// Values: 
								// ¡ö 0x0 (NO_CHANNEL_LOCK): Channel is not locked during the transfers
								// ¡ö 0x0 (CHANNEL_LOCK): Channel is locked and granted exclusive access to the Master Bus Interface
		
	DMAC_LOCK_CH_L_SHIFT	= 53,	// [54:53], Varies, Channel Lock Level
									// This bit indicates the duration over which CHx_CFG.LOCK_CH bit applies.
									// ¡ö 00: Over complete DMA transfer
									// ¡ö 01: Over DMA block transfer
									// ¡ö 1x: Reserved
									// This field does not exist if the configuration parameter 
									// DMAX_CHx_LOCK_EN is set to False; in that case, the 
									// read-back value is always 0.
									// Values: 
									// ¡ö 0x0 (DMA_transfer_CH_LOCK): Duration of the Channel locking is for the entire DMA transfer
									// ¡ö 0x1 (BLOCK_TRANFER_CH_LOCK): Duration of the Channel locking is for the current block transfer
	
	DMAC_SRC_OSR_LMT_SHIFT	= 55,	// [58:55], R/W, Source Outstanding Request Limit
									// ¡ö Maximum outstanding request supported is 16.
									// ¡ö Source Outstanding Request Limit = SRC_OSR_LMT + 1
	
	DMAC_DST_OSR_LM_SHIFT	= 59,	// [62:59], R/W, Destination Outstanding Request Limit
									// ¡ö Maximum outstanding request supported is 16.
									// ¡ö Source Outstanding Request Limit = DST_OSR_LMT + 1
										
	RSVD_DMAC_CHx_CFG_63_SHIFT	= 63	// [63], R, DMAC Channelx Transfer Configuration Register (63bit) Reserved bit - Read Only
}E_CH_CFG;

// bit[1:0], Source Multi Block Transfer Type
// bit[3:2], Destination Multi Block Transfer Type
typedef enum {
	MB_TYPE_CONTIGUOUS 	= 0,
	MB_TYPE_RELOAD 		= 1,
	MB_TYPE_SHADOW 		= 2,
	MB_TYPE_LINK_LIST 	= 3
}E_MULTBLK_TYPE;

// bit[34:32], Transfer Type and Flow Control.
typedef enum {
	MEM_TO_MEM_DMAC 	= 0,	// Transfer Type is memory to memory and Flow Controller is DW_axi_dmac
	MEM_TO_PER_DMAC  	= 1,	// Transfer Type is memory to peripheral and Flow Controller is DW_axi_dmac
	PER_TO_MEM_DMAC  	= 2,	// Transfer Type is peripheral to memory and Flow Controller is DW_axi_dmac
	PER_TO_PER_DMAC  	= 3,	// Transfer Type is peripheral to peripheral and Flow Controller is DW_axi_dmac
	PER_TO_MEM_SRC  	= 4,	//  Transfer Type is peripheral to Memory and Flow Controller is Source peripheral
	PER_TO_PER_SRC  	= 5,	// Transfer Type is peripheral to peripheral and Flow Controller is Source peripheral
	MEM_TO_PER_DST  	= 6,	// Transfer Type is memory to peripheral and Flow Controller is Destination periphera
	PER_TO_PER_DST  	= 7		// Transfer Type is peripheral to peripheral and Flow Controller is Destination peripheral
}E_TT_FC;


// bit[35], Source Software or Hardware Handshaking Select.
// bit[36], Destination Software or Hardware Handshaking Select.
typedef enum {
	HS_SEL_HW 	= 0,	// Hardware handshaking interface
	HS_SEL_SW  	= 1		// Software handshaking interface
}E_HS_SEL;

// bit[51:49], Channel priority
typedef enum {
	CH_PRIOR_0 	= 0, 	// lowest priority 
	CH_PRIOR_1 	= 1,
	CH_PRIOR_2 	= 2,
	CH_PRIOR_3 	= 3,
	CH_PRIOR_4 	= 4,
	CH_PRIOR_5 	= 5,
	CH_PRIOR_6 	= 6,
	CH_PRIOR_7 	= 7		// highest priority
}E_CH_PRIOR;

// low 32 bits
#define DMA_CHX_CFG_L32BITS_VAL(dst_mb_type, src_mb_type)\
			(((dst_mb_type) << DMAC_DST_MULTBLK_TYPE_SHIFT)|\
			((src_mb_type) << DMAC_SRC_MULTBLK_TYPE_SHIFT))

// high 32 bits
#define DMA_CHX_CFG_H32BITS_VAL(prior, dst_per, src_per, hs_sel_dst, hs_sel_src, tt_fc)\
			(((0) << (DMAC_DST_OSR_LM_SHIFT - 32))|\
			((0) << (DMAC_SRC_OSR_LMT_SHIFT - 32))|\
			((0) << (DMAC_LOCK_CH_L_SHIFT - 32))|\
			((0) << (DMAC_LOCK_CH_SHIFT - 32))|\
			((prior) << (DMAC_CH_PRIOR_SHIFT - 32))|\
			((dst_per) << (DMAC_DST_PER_SHIFT - 32))|\
			((src_per) << (DMAC_SRC_PER_SHIFT - 32))|\
			((hs_sel_dst) << (DMAC_HS_SEL_DST_SHIFT - 32))|\
			((hs_sel_src) << (DMAC_HS_SEL_SRC_SHIFT - 32))|\
			((tt_fc) << (DMAC_TT_FC_SHIFT - 32)))

// offset: 0x128, This is the Linked List Pointer register. This register must be programmed to point to the first...
typedef enum
{
	DMAC_LMS_SHIFT			= 0,	// [0], Varies, LLI master Select
									// This bit identifies the AXI layer/interface where the memory 
									// device that stores the next linked list item resides. - 0: AXI Master 1
									// ¡ö 1: AXI Master 2
									// This field does not exist if the configuration parameter DMAX_CHx_LMS is not set to NO_HARDCODE.
									// In this case, the read-back value is always the hardcoded 
									// value. The maximum value of this field that can be read back is 'DMAX_NUM_MASTER_IF-1'.
									// Values: 
									// ¡ö 0x0 (MASTER1_INTF): next Linked List item resides on AXI Master1 interface
									// ¡ö 0x1 (MASTER2_INTF): next Linked List item resides on AXI Master2 interface
	
	RSVD_DMAC_CHx_LLP_1to5_SHIFT	= 1,	// [5:1],  R, DMAC Channelx Linked List Pointer Register (bits 1to5) Reserved bits - Read Only
										
	DMAC_LOC_SHIFT				= 6	// [63:6], R/W, Starting Address Memory of LLI block
									// Starting Address In Memory of next LLI if block chaining is 
									// enabled. The six LSBs of the starting address are not stored 
									// because the address is assumed to be aligned to a 64-byte boundary.
									// LLI access always uses the burst size (arsize/awsize) that is 
									// same as the data bus width and cannot be changed or 
									// programmed to anything other than this. Burst length 
									// (awlen/arlen) is chosen based on the data bus width so that 
									// the access does not cross one complete LLI structure of 64 
									// bytes. DW_axi_dmac will fetch the entire LLI (40 bytes) in 
									// one AXI burst if the burst length is not limited by other settings.
}E_DMAC_CH_LLP_SHIFT;

// offset: 0x130, Channelx Status Register contains fields that indicate the status of DMA transfers for...
typedef enum
{
	DMAC_CMPLTD_BLK_TFR_SIZE_SHIFT			= 0,	// [21:0], R, Completed Block Transfer Size.
													// This bit indicates the total number of data of width 
													// CHx_CTL.SRC_TR_WIDTH transferred for the previous block transfer.
													// For normal block transfer completion without any errors, this 
													// value will be equal to the value programmed in BLOCK_TS 
													// field of CHx_BLOCK_TS register.
													// If any error occurs during the dma transfer, the block transfer 
													// might be terminated early and in such a case, this value 
													// indicates the actual data transferred without error in the current block.
													// This field is cleared to zero on enabling the channel.
		
	RSVD_DMAC_CHx_STATUSREG_22to31_SHIFT	= 22,	// [31:22], R, DMAC Channelx Status Register (bits 22to31) Reserved bits - Read Only
	
	DMAC_DATA_LEFT_IN_FIFO_SHIFT			= 32,	// [46:32],  R, Data Left in FIFO.
													// This bit indicates the total number of data left in 
													// DW_axi_dmac channel FIFO after completing the current block transfer.
													// The width of the data in channel FIFO is equal to CHx_CTL.SRC_TR_WIDTH.
													// For normal block transfer completion without errors, Data_Left_In_FIFO = 0.
													// If any error occurs during the dma transfer, the block transfer 
													// might be terminated early and in such a case, 
													// Data_Left_In_FIFO indicates the data remaining in channel 
													// FIFO which could not be transferred to destination peripheral.
													// This field is cleared to zero on enabling the channel.
													// Note: If CHx_CTL.DST_TR_WIDTH > 
													// CHx_CTL.SRC_TR_WIDTH, there may be residual data left 
													// in the FIFO which is not enough to form one 
													// CHx_CTL.SRC_TR_WIDTH of data and Data_Left_In_FIFO will return 0 in this case.
										
	RSVD_DMAC_CHx_STATUSREG_47to63_SHIFT	= 47	// [63:47], R/W, DMAC Channelx Status Register (bits 47to63) Reserved bits - Read Only
}E_DMAC_CH_STATUSREG_SHIFT;

// offset: 0x138, Channelx Software handshake Source Register.
typedef enum
{
	DMAC_SWHS_REQ_SRC_SHIFT			= 0,	// [0], R/W, Software Handshake Request for Channel Source.
											// This bit is used to request dma source data transfer if 
											// software handshaking method is selected for the source of 
											// the corresponding channel.
											// This bit is ignored if software handshaking is not enabled for 
											// the source of the Channelx. The functionality of this field 
											// depends on whether the peripheral is the flow controller or not.
											// DW_axi_dmac clears this bit to 0 once software reads 
											// CHx_SWHSSrcReg.SWHS_Ack_Src bit and sees it as 1. 
											// Software can only set this bit to 1; it is not allowed to clear 
											// this bit to 0; only DW_axi_dmac can clear this bit.
											// Note: SWHS_Req_Src bit is written only if the corresponding 
											// write enable bit, SWHS_Req_Src_WE is asserted on the 
											// same register write operation and if the Channelx is enabled 
											// in the DMAC_ChEnReg register. This allows software to set 
											// a bit in the CHx_SWHSSrcReg register without performing a 
											// read-modified write operation.
											// Values: 
											// ¡ö 0x1 (ACTIVE_SWHS_REQ_SRC): Source peripheral request for a dma transfer
											// ¡ö 0x0 (INACTIVE_SWHS_REQ_SRC): Source peripheral is not requesting for a burst transfer
	
	DMAC_SWHS_REQ_SRC_WE_SHIFT		= 1,	// [1], W, Write Enable bit for Software Handshake Request for Channel Source.
											// Note: This bit always returns 0 on a read back.
											// Values: 
											// ¡ö 0x1 (ENABLE_SWHS_REQ_SRC): Enables write to the SWHS_REQ_SRC bit
											// ¡ö 0x0 (DISABLE_SWHS_REQ_SRC): Disables write to the SWHS_REQ_SRC bit
	
	DMAC_SWHS_SGLREQ_SRC_SHIFT		= 2,	// [2], R/W, Software Handshake Single Request for Channel Source.
											// This bit is used to request SINGLE (AXI burst length = 1) 
											// dma source data transfer if software handshaking method is 
											// selected for the source of the corresponding channel. This 
											// bit is ignored if software handshaking is not enabled for the 
											// source of the Channelx. The functionality of this field 
											// depends on whether the peripheral is the flow controller.
											// DW_axi_dmac clears this bit to 0 once software reads 
											// CHx_SWHSSrcReg.SWHS_Ack_Src bit and sees it as 1. 
											// Software can only set this bit to 1; it is not allowed to clear 
											// this bit to 0; only DW_axi_dmac can clear this bit.
											// Note: SWHS_SglReq_Src bit is written only if the 
											// corresponding write enable bit, SWHS_SglReq_Src_WE is 
											// asserted on the same register write operation and if the 
											// Channelx is enabled in the DMAC_ChEnReg register. This 
											// allows software to set a bit in the CHx_SWHSSrcReg 
											// register without performing a read-modified write operation.
											// Values: 
											// ¡ö 0x1 (ACTIVE_SWHS_SGLREQ_SRC): Source peripheral request for a single dma transfer
											// ¡ö 0x0 (INACTIVE_SWHS_SGLREQ_SRC): Source peripheral is not requesting for a single transfer
	
	DMAC_SWHS_SGLREQ_SRC_WE_SHIFT	= 3,	// [3], W, Write Enable bit for Software Handshake Single Request for Channel Source.
											// Values: 
											// ¡ö 0x1 (ENABLE_SWHS_SGLREQ_SRC): Enables write to the SWHS_SGLREQ_SRC bit
											// ¡ö 0x0 (DISABLE_SWHS_SGLREQ_SRC): Disables write to the SWHS_SGLREQ_SRC bit
		
	DMAC_SWHS_LST_SRC_SHIFT			= 4,	// [4], R/W, Software Handshake Last Request for Channel Source.
											// This bit is used to request LAST dma source data transfer if 
											// software handshaking method is selected for the source of 
											// the corresponding channel.
											// This bit is ignored if software handshaking is not enabled for 
											// the source of the Channelx or if the source of Channelx is 
											// not the flow controller.
											// CHx_SWHSSrcReg.SWHS_Req_Src bit must be set to 1 for 
											// DW_axi_dmac to treat it as a valid software handshaking 
											// request.
											// If CHx_SWHSSrcReg.SWHS_SglReq_Src is set to 1, the 
											// LAST request is for SINGLE dma transaction (AXI burst 
											// length = 1), else the request is treated as a BURST 
											// transaction request.
											// DW_axi_dmac clears this bit to 0 once software reads 
											// CHx_SWHSSrcReg.SWHS_Ack_Src bit and sees it as 1.
											// Software can only set this bit to 1; it is not allowed to clear 
											// this bit to 0; only DW_axi_dmac can clear this bit.
											// Note: SWHS_Lst_Src bit is written only if the corresponding 
											// write enable bit, SWHS_Lst_Src_WE is asserted on the 
											// same register write operation and if the Channelx is enabled 
											// in the DMAC_ChEnReg register. This allows software to set 
											// a bit in the CHx_SWHSSrcReg register without performing a 
											// read-modified write operation.
											// Values: 
											// ¡ö 0x1 (ACTIVE_SWHS_LAST_SRC): Source peripheral 
											// indication to dmac that the current transfer is the last transfer
											// ¡ö 0x0 (INACTIVE_SWHS_LAST_SRC): Source peripheral 
											// indication that the curent transfer is not the last transfer
	
	DMAC_SWHS_LST_SRC_WE_SHIFT		= 5,	// [5],  W, Write Enable bit for Software Handshake Last Request for Channel Source.
											// Values: 
											// ¡ö 0x1 (ENABLE_SWHS_LAST_SRC): Enables write to the SWHS_LAST_SRC bit
											// ¡ö 0x0 (DISABLE_SWHS_LAST_SRC): Disables write to the SWHS_LAST_SRC bit
										
	RSVD_DMAC_CHx_SWHSSRCREG_6to63_SHIFT	= 6	// [63:6], R, DMAC Channelx Software Handshake Source Register (bits 6to63) Reserved bits - Read Only
}E_DMAC_CH_SWHSSRCREG_SHIFT;

#define DMA_CHX_SWHSSRC_VAL(last_req, single_req, req)\
			(((1) << DMAC_SWHS_LST_SRC_WE_SHIFT)|\
			((last_req) << DMAC_SWHS_LST_SRC_SHIFT)|\
			((1) << DMAC_SWHS_SGLREQ_SRC_WE_SHIFT)|\
			((single_req) << DMAC_SWHS_SGLREQ_SRC_SHIFT)|\
			((1) << DMAC_SWHS_REQ_SRC_WE_SHIFT)|\
			((req) << DMAC_SWHS_REQ_SRC_SHIFT))

// offset: 0x140, Channelx Software handshake Destination Register
typedef enum
{
	DMAC_SWHS_REQ_DST_SHIFT			= 0,	// [0], R/W, Software Handshake Request for Channel Destination.
											// This bit is used to request dma destination data transfer if 
											// software handshaking method is selected for the destination 
											// of the corresponding channel.
											// This bit is ignored if software handshaking is not enabled for 
											// the source of the Channelx. The functionality of this field 
											// depends on whether the peripheral is the flow controller.
											// DW_axi_dmac clears this bit to 0 once software reads 
											// CHx_SWHSDstReg.SWHS_Ack_Dst bit and sees it as 1. 
											// Software can only set this bit to 1; it is not allowed to clear 
											// this bit to 0; only DW_axi_dmac can clear this bit.
											// Note: SWHS_Req_Dst bit is written only if the corresponding 
											// write enable bit, SWHS_Req_Dst_WE is asserted on the 
											// same register write operation and if the Channelx is enabled 
											// in the DMAC_ChEnReg register. This allows software to set 
											// a bit in the CHx_SWHSDstReg register without performing a 
											// read-modified write operation.
											// Values: 
											// ¡ö 0x1 (ACTIVE_SWHS_REQ_DST): Destination peripheral request for a dma transfer
											// ¡ö 0x0 (INACTIVE_SWHS_REQ_DST): Destination peripheral is not requesting for a burst transfer

	DMAC_SWHS_REQ_DST_WE_SHIFT		= 1,	// [1], W, Write Enable bit for Software Handshake Request for Channel Destination.
											// Note: This bit always returns 0 on a read block.
											// Values: 
											// ¡ö 0x1 (ENABLE_SWHS_REQ_DST): Enables write to the SWHS_REQ_DST bit
											// ¡ö 0x0 (DISABLE_SWHS_REQ_DST): Disables write to the SWHS_REQ_DST bit

	DMAC_SWHS_SGLREQ_DST_SHIFT		= 2,	// [2], R/W, Software Handshake Single Request for Channel Destination.
											// This bit is used to request SINGLE (AXI burst length = 1) 
											// dma destination data transfer if software handshaking 
											// method is selected for the destination of the corresponding channel.
											// This bit is ignored if software handshaking is not enabled for 
											// the destination of the Channelx. The functionality of this field 
											// depends on whether the peripheral is the flow controller.
											// DW_axi_dmac clears this bit to 0 once software reads 
											// CHx_SWHSDstReg.SWHS_Ack_Dst bit and sees it as 1. 
											// Software can only set this bit to 1; it is not allowed to clear 
											// this bit to 0; only DW_axi_dmac can clear this bit.
											// Note: SWHS_SglReq_Dst bit is written only if the 
											// corresponding write enable bit, SWHS_SglReq_Dst_WE is 
											// asserted on the same register write operation and if the 
											// Channelx is enabled in the DMAC_ChEnReg register. This 
											// allows software to set a bit in the CHx_SWHSDstReg 
											// register without performing a read-modified write operation.
											// Values: 
											// ¡ö 0x1 (ACTIVE_SWHS_SGLREQ_DST): Destination peripheral request for a single dma transfer
											// ¡ö 0x0 (INACTIVE_SWHS_SGLREQ_DST): Destination peripheral is not requesting for a single transfer	

	DMAC_SWHS_SGLREQ_DST_WE_SHIFT	= 3,	// [3], W, Write Enable bit for Software Handshake Single Request for Channel Destination.
											// Note: This bit always returns 0 on a read block.
											// Values: 
											// ¡ö 0x1 (ENABLE_SWHS_SGLREQ_DST): Enables write to the SWHS_SGLREQ_DST bit
											// ¡ö 0x0 (DISABLE_SWHS_SGLREQ_DST): Disables write to the SWHS_SGLREQ_DST bit		

	DMAC_SWHS_LST_DST_SHIFT			= 4,	// [4], R/W, Software Handshake Last Request for Channel Destination.
											// This bit is used to request LAST dma destination data 
											// transfer if software handshaking method is selected for the 
											// destination of the corresponding channel.
											// This bit is ignored if software handshaking is not enabled for 
											// the destination of the Channelx or if the destination of 
											// Channelx is not the flow controller. 
											// CHx_SWHSDstReg.SWHS_Req_Dst bit must be set to 1 for 
											// DW_axi_dmac to treat it as a valid software handshaking request.
											// If CHx_SWHSDstReg.SWHS_SglReq_Dst is set to 1, the 
											// LAST request is for SINGLE dma transaction (AXI burst 
											// length = 1), else the request is treated as a BURST transaction request.
											// DW_axi_dmac clears this bit to 0 once software reads 
											// CHx_SWHSDstReg.SWHS_Ack_Dst bit and sets it as 1. 
											// Software can only set this bit to 1; it is not allowed to clear 
											// this bit to 0; only DW_axi_dmac can clear this bit.
											// Note: SWHS_Lst_Src bit is written only if the corresponding 
											// write enable bit, SWHS_Lst_Src_WE is asserted on the 
											// same register write operation and if the Channelx is enabled 
											// in the DMAC_ChEnReg register. This allows software to set 
											// a bit in the CHx_SWHSDstReg register without performing a 
											// read-modified write operation.
											// Values: 
											// ¡ö 0x1 (ACTIVE_SWHS_LAST_DST): Destination peripheral indication to dmac that the current transfer is the last transfer
											// ¡ö 0x0 (INACTIVE_SWHS_LAST_DST): Destination peripheral indication that the current transfer is not the last transfer
	
	DMAC_SWHS_LST_DST_WE_SHIFT		= 5,	// [5],  W, Write Enable bit for Software Handshake Last Request for Channel Destination.
											// Note: This bit always returns 0 on a read back.
											// Values: 
											// ¡ö 0x1 (ENABLE_SWHS_LAST_DST): Enables write to the SWHS_LAST_DST bit
											// ¡ö 0x0 (DISABLE_SWHS_LAST_DST): Disables write to the SWHS_LAST_DST bit
										
	RSVD_DMAC_CHx_SWHSDSTREG_6to63_SHIFT	= 6	// [63:6], R, DMAC Channelx Software Handshake Destination Register (bits 6to63) Reserved bits - Read Only
}E_DMAC_CH_SWHSDSTREG_SHIFT;

#define DMA_CHX_SWHSDST_VAL(last_req, single_req, req)\
			(((1) << DMAC_SWHS_LST_DST_WE_SHIFT)|\
			((last_req) << DMAC_SWHS_LST_DST_SHIFT)|\
			((1) << DMAC_SWHS_SGLREQ_DST_WE_SHIFT)|\
			((single_req) << DMAC_SWHS_SGLREQ_DST_SHIFT)|\
			((1) << DMAC_SWHS_REQ_DST_WE_SHIFT)|\
			((req) << DMAC_SWHS_REQ_DST_SHIFT))

// offset: 0x148, Channelx Block Transfer Resume Request Register. This register is used during Linked List or Shadow...
typedef enum
{
	DMAC_BLK_TFR_RESUMEREQ_SHIFT					= 0,	// [0], W, lock Transfer Resume Request during Linked-List or 
															// Shadow-Register-based multi-block transfer.
															// Values: 
															// ¡ö 0x1 (ACTIVE_BLK_TFR_RESUMEREQ): Request for resuming the block transfer
															// ¡ö 0x1 (INACTIVE_BLK_TFR_RESUMEREQ): No request to resume the block transfer
			
	RSVD_DMAC_CHx_BLK_TFR_RESUMEREQREG_1to63_SHIFT	= 1		// [63:1], W, DMAC Channelx Block Transfer Resume Request Register (bits 1to63) Reserved bits - Read Only
}E_DMAC_CH_BLK_TFR_RESUMEREQREG_SHIFT;

// offset: 0x150, Channelx AXI ID Register. This register is allowed to be updated only when the channel is disabled,...
typedef enum
{
	DMAC_AXI_READ_ID_SUFFIX_SHIFT					= 0,	// [15:0], R/W, AXI Read ID Suffix
															// These bits form part of the ARID output of AXI3/AXI4 master interface.
															//      IDW = DMAX_M_ID_WIDTH
															//     L2NC = log2(DMAX_NUM_CHANNELS)
															// The upper L2NC+1 bits of aridN is derived from the channel 
															// number which is currently accessing the master interface.
															// This varies for LLI fetch and source data transfer.
															// For source data transfer, aridN  for channel1 4'b0000, aridN 
															// for channel8 4'b0111 and so on.
															// For LLI fetch access, aridN  for channel1 4'b1000, aridN for 
															// channel8 4'b1111 and so on. Lower bits are same as the 
															// value programmed in CHx_AXI_IDReg.AXI_Read_ID_Suffix filed.
	
	DMAC_AXI_WRITE_ID_SUFFIX_SHIFT					= 16,	// [31:16], R/W, AXI Write ID Suffix.
															// These bits form part of the AWID output of AXI3/AXI4 master interface.
															//      IDW = DMAX_M_ID_WIDTH
															//      L2NC = log2(DMAX_NUM_CHANNELS)
															// The upper L2NC+1 bits of awidN is derived from the channel 
															// number which is currently accessing the master interface.
															// This varies for LLI fetch and source data transfer.
															// For source data transfer, awidN for channel1 4'b0000, awidN 
															// for channel8 4'b0111 and so on.
															// For LLI fetch access, awidN for channel1 4'b1000, awidN for 
															// channel8 4'b1111 and so on.
															// Lower bits are same as the value programmed in 
															// CHx_AXI_IDReg.AXI_Write_ID_Suffix filed.
	
	RSVD_DMAC_CHx_AXI_IDREG_32to63_SHIFT	= 32		// [63:32], W, DMAC Channelx AXI ID Register (bits 32to63) Reserved bits - Read Only
}E_DMAC_CH_AXI_IDREG_SHIFT;

// offset: 0x158, Channelx AXI QOS Register. This register is allowed to be updated only when the channel is disabled,
typedef enum
{
	DMAC_AXI_AWQOS_SHIFT					= 0,	// [3:0], Varies, AXI AWQOS.
													// These bits form the awqos output of AXI4 master interface.
	
	DMAC_AXI_ARQOS_SHIFT					= 4,	// [7:4], Varies, AXI ARQOS.
													// These bits form the arqos output of AXI4 master interface.		
													
	RSVD_DMAC_CHx_AXI_QOSREG_8to63_SHIFT	= 8		// [63:8], R, DMAC Channelx AXI QOS Register (bits 8to63) Reserved bits - Read Only
}E_DMAC_CH_AXI_QOSREG_SHIFT;

// offset: 0x160, Channelx Source Status Register. After each block transfer completes, hardware can retrieve the...
typedef enum
{	
	DMAC_SSTAT_SHIFT					= 0,	// [31:0], R, Source Status
												// Source status information retrieved by hardware from the 
												// address pointed to by the contents of the CHx_SSTATAR register.
												// Source peripheral should update the source status 
												// information, if any, at the location pointed to by 
												// CHx_SSTATAR to utilize this feature.This status is not related 
												// to any internal status of DW_axi_dmac.
												// This status is not related to any internal status of DW_axi_dmac.
													
	RSVD_DMAC_CHx_SSTAT_32to63_SHIFT	= 32		// [63:32], R, DMAC Channelx Source Status Register (bits 32to63) Reserved bits - Read Only
}E_DMAC_CH_SSTAT_SHIFT;

// offset: 0x168, Channelx Destination Status Register. After each block transfer completes, hardware can retrieve..
typedef enum
{	
	DMAC_DSTAT_SHIFT					= 0,	// [31:0], R, Destination Status
												// Destination status information retrieved by hardware from 
												// the address pointed to by the contents of the CHx_DSTATAR register.
												// Destination peripheral should update the destination status 
												// information, if any, at the location pointed to by 
												// CHx_DSTATAR to utilize this feature.
												// This status is not related to any internal status of DW_axi_dmac.			

	RSVD_DMAC_CHx_DSTAT_32to63_SHIFT	= 32	// [63:32], R, DMAC Channelx Destination Status Register (bits 32to63) Reserved bits - Read Only
}E_DMAC_CH_DSTAT_SHIFT;

// offset: 0x180, Writing 1 to specific field enables the corresponding interrupt status generation in Channelx Interrupt...
// offset: 0x188, Channelx Interrupt Status Register captures the Channelx specific interrupts 
// offset: 0x190, This register contains fields that are used to enable the generation of port level interrupt at..
// offset: 0x198, Writing 1 to specific field will clear the corresponding field in Channelx Interrupt Status...
typedef enum
{	
	DMAC_ENABLE_BLOCK_TFR_DONE_INTSTAT_SHIFT					= 0,	// [0], R/W, Block Transfer Done Interrupt Status Enable.
																		// ¡ö 0: Disable the generation of Block Transfer Done Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Block Transfer Done Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_BLOCK_TFR_DONE): Enable the generation of Block Transfer Done Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_BLOCK_TFR_DONE): Disable the generation of Block Transfer Done Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_DMA_TFR_DONE_INTSTAT_SHIFT						= 1,	// [1], R/W, MA Transfer Done Interrupt Status Enable.
																		// ¡ö 0: Disable the generation of DMA Transfer Done Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of DMA Transfer Done Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_DMA_TFR_DONE): Enable the generation of DMA Transfer Done Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_DMA_TFR_DONE): Disable the generation of DMA Transfer Done Interrupt in CH1_INTSTATUSREG
	
	RSVD_DMAC_CHX_INTSTATUS_ENABLEREG_2_SHIFT					= 2,	// [2],   R   , DMAC Channelx Interrupt Status Register (bit 2) Reserved bit - Read Only
	
	DMAC_ENABLE_SRC_TRANSCOMP_INTSTAT_SHIFT						= 3,	// [3],  R/W, Source Transaction Completed Status Enable.
																		// ¡ö 0: Disable the generation of Source Transaction Complete Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Source Transaction Complete Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SRC_TRANSCOMP): Enable the generation of Source Transaction Complete Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SRC_TRANSCOMP): Disable the generation of Source Transaction Complete Interrupt in CH1_INTSTATUSREG
	
	DMAC_ENABLE_DST_TRANSCOMP_INTSTAT_SHIFT						= 4,	// [4],  R/W, Destination Transaction Completed Status Enable.
																		// ¡ö 0: Disable the generation of Destination Transaction complete Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Destination Transaction complete Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_DST_TRANSCOMP): Enable the generation of Destination Transaction complete Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_DST_TRANSCOMP): Disable the generation of Destination Transaction complete Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_SRC_DEC_ERR_INTSTAT_SHIFT						= 5,	// [5], R/W, Source Decode Error Status Enable.
																		// ¡ö 0: Disable the generation of Source Decode Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Source Decode Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SRC_DEC_ERR): Enable the generation of Source Decode Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SRC_DEC_ERR): Disable the generation of Source Decode Error Interrupt in CH1_INTSTATUSREG
	
	DMAC_ENABLE_DST_DEC_ERR_INTSTAT_SHIFT						= 6,	// [6],  R/W, Destination Decode Error Status Enable.
																		// ¡ö 0: Disable the generation of Destination Decode Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Destination Decode Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_DST_DEC_ERR): Enable the generation of Destination Decode Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_DST_DEC_ERR): Disable the generation of Destination Decode Error Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_SRC_SLV_ERR_INTSTAT_SHIFT						= 7,	// [7],  R/W, Source Slave Error Status Enable.
																		// ¡ö 0: Disable the generation of Source Slave Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Source Slave Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SRC_SLV_ERR): Enable the generation of Source Slave Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SRC_SLV_ERR): Disable the generation of Source Slave Error Interrupt in CH1_INTSTATUSREG
	
	DMAC_ENABLE_DST_SLV_ERR_INTSTAT_SHIFT						= 8,	// [8],  R/W, Destination Slave Error Status Enable.
																		// ¡ö 0: Disable the generation of Destination Slave Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Destination Slave Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_DST_SLV_ERR): Enable the generation of Destination Slave Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_DST_SLV_ERR): Disable the generation of Destination Slave Error Interrupt in CH1_INTSTATUSREG 
	
	DMAC_ENABLE_LLI_RD_DEC_ERR_INTSTAT_SHIFT					= 9,	// [9], R/W, LLI Read Decode Error Status Enable.
																		// ¡ö 0: Disable the generation of LLI Read Decode Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of LLI Read Decode Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_LLI_RD_DEC_ERR): Enable the generation of LLI Read Decode Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_LLI_RD_DEC_ERR): Disable the generation of LLI Read Decode Error Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_LLI_WR_DEC_ERR_INTSTAT_SHIFT					= 10,	// [10], R/W, LLI WRITE Decode Error Status Enable.
																		// ¡ö 0: Disable the generation of LLI WRITE Decode Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of LLI WRITE Decode Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_LLI_WR_DEC_ERR): Disable the generation of LLI WRITE Decode Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_LLI_WR_DEC_ERR): Enable the generation of LLI WRITE Decode Error Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_LLI_RD_SLV_ERR_INTSTAT_SHIFT					= 11,	// [11], R/W, LLI Read Slave Error Status Enable.
																		// ¡ö 0: Disable the generation of LLI Read Slave Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of LLI Read Slave Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_LLI_RD_SLV_ERR): Enable the generation of LLI Read Slave Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_LLI_RD_SLV_ERR): Disable the generation of LLI Read Slave Error Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_LLI_WR_SLV_ERR_INTSTAT_SHIFT					= 12,	// [12], R/W, LLI WRITE Slave Error Status Enable.
																		// ¡ö 0: Disable the generation of LLI WRITE Slave Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of LLI WRITE Slave Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_LLI_WR_SLV_ERR): Enable the generation of LLI WRITE Slave Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_LLI_WR_SLV_ERR): Disable the generation of LLI WRITE Slave Error Interrupt in CH1_INTSTATUSREG
	
	DMAC_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_INTSTAT_SHIFT		= 13,	// [13], R/W, Shadow register or LLI Invalid Error Status Enable.
																		// ¡ö 0: Disable the generation of Shadow Register or LLI Invalid Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Shadow Register or LLI Invalid  Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SHADOWREG_OR_LLI_INVALID_ERR): Disable the generation of Shadow Register or LLI Invalid Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SHADOWREG_OR_LLI_INVALID_ERR): Enable the generation of Shadow Register or LLI Invalid  Error Interrupt in CH1_INTSTATUSREG
	
	DMAC_ENABLE_SLVIF_MULTIBLKTYPE_ERR_INTSTAT_SHIFT			= 14,	// [14], R/W, Slave Interface Multi Block type Error Status Enable.
																		// ¡ö 0: Disable the generation of Slave Interface Multi Block type Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Slave Interface Multi Block type Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SLVIF_MULTIBLKTYPE_ERR): Enable the generation of Slave Interface Multi Block type Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SLVIF_MULTIBLKTYPE_ERR): Disable the generation of Slave Interface Multi Block type Error Interrupt in CH1_INTSTATUSREG

	RSVD_DMAC_CHX_INTSTATUS_ENABLEREG_15_SHIFT					= 15,	// [15], R/W, DMAC Channelx Interrupt Status Register (bit 15) Reserved bit - Read Only

	DMAC_ENABLE_SLVIF_DEC_ERR_INTSTA_SHIFT						= 16,	// [16], R/W, Slave Interface Decode Error Status Enable.
																		// ¡ö 0: Disable the generation of Slave Interface Decode Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Slave Interface Decode Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SLVIF_DEC_ERR): Enable the generation of Slave Interface Decode Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SLVIF_DEC_ERR): Disable the generation of Slave Interface Decode Error Interrupt in CH1_INTSTATUSREG
	
	DMAC_ENABLE_SLVIF_WR2RO_ERR_INTSTAT_SHIFT					= 17,	// [17], R/W, Slave Interface Write to Read Only Error Status Enable.
																		// ¡ö 0: Disable the generation of Slave Interface Write to Read only Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Slave Interface Write to Read Only Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SLVIF_WR2RO_ERR): Enable the generation of Slave Interface Write to Read Only Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SLVIF_WR2RO_ERR): Disable the generation of Slave Interface Write to Read only Error Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_SLVIF_RD2RWO_ERR_INTSTAT_SHIFT					= 18,	// [18], R/W, Slave Interface Read to write Only Error Status Enable.
																		// ¡ö 0: Disable the generation of Slave Interface Read to Write only Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Slave Interface Read to Write Only Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SLVIF_RD2RWO_ERR): Enable the generation of Slave Interface Read to Write Only Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SLVIF_RD2RWO_ERR): Disable the generation of Slave Interface Read to Write only Error Interrupt in CH1_INTSTATUSREG
	
	DMAC_ENABLE_SLVIF_WRONCHEN_ERR_INTSTAT_SHIFT				= 19,	// [19], R/W, Slave Interface Write On Channel Enabled Error Status Enable.
																		// ¡ö 0: Disable the generation of Slave Interface Write On Channel enabled Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Slave Interface Write On Channel enabled Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SLVIF_WRONCHEN_ERR): Enable the generation of Slave Interface Write On Channel enabled Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SLVIF_WRONCHEN_ERR): Disable the generation of Slave Interface Write On Channel enabled Error Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_SLVIF_SHADOWREG_WRON_VALID_ERR_INTSTA_SHIFT		= 20,	// [20], R/W, Shadow Register Write On Valid Error Status Enable.
																		// ¡ö 0: Disable the generation of Shadow Register Write On Valid Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Shadow register Write On Valid Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SLVIF_SHADOWREG_WRON_VALID_ERR): 
																		// Enable the generation of Shadow register Write On Valid Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SLVIF_SHADOWREG_WRON_VALID_ERR): 
																		// Disable the generation of Shadow Register Write On Valid Error Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_SLVIF_WRONHOLD_ERR_INTSTAT_SHIFT				= 21,	// [21], R/W, Slave Interface Write On Hold Error Status Enable.
																		// ¡ö 0: Disable the generation of Slave Interface Write On Hold Error Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Slave Interface Write On Hold Error Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_SLVIF_WRONHOLD_ERR): Enable the generation of Slave Interface Write On Hold Error Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_SLVIF_WRONHOLD_ERR): Disable the generation of Slave Interface Write On Hold Error Interrupt in CH1_INTSTATUSREG
	
	DMAC_RSVD_DMAC_CHX_INTSTATUS_ENABLEREG_22TO26_INTSTAT_SHIFT	= 22,	// [26:22], R, DMAC Channelx Interrupt Status Register (bits 22to26) Reserved bits - Read Only

	
	DMAC_ENABLE_CH_LOCK_CLEARED_INTSTAT_SHIFT					= 27,	// [27], R/W, Channel Lock Cleared Status Enable.
																		// ¡ö 0: Disable the generation of Channel LOCK CLEARED Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Channel LOCK CLEARED Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_CH_LOCK_CLEARED): Enable the generation of Channel LOCK CLEARED Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_CH_LOCK_CLEARED): Disable the generation of Channel LOCK CLEARED Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_CH_SRC_SUSPENDED_INTSTAT_SHIFT					= 28,	// [28], R/W, Channel Source Suspended Status Enable.
																		// ¡ö 0: Disable the generation of Channel Source Suspended Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Channel Source Suspended Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_CH_SRC_SUSPENDED): Enable the generation of Channel Source Suspended Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_CH_SRC_SUSPENDED): Disable the generation of Channel Source Suspended Interrupt in CH1_INTSTATUSREG

	DMAC_ENABLE_CH_SUSPENDED_INTSTAT_SHIFT						= 29,	// [29], R/W, Channel Suspended Status Enable.
																		// ¡ö 0: Disable the generation of Channel Suspended Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Channel Suspended Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_CH_SUSPENDED): Enable the generation of Channel Suspended Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_CH_SUSPENDED): Disable the generation of Channel Suspended Interrupt in CH1_INTSTATUSREG
		
	DMAC_ENABLE_CH_DISABLED_INTSTAT_SHIFT						= 30,	// [30], R/W, Channel Disabled Status Enable.
																		// ¡ö 0: Disable the generation of Channel Disabled Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Channel Disabled Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_CH_DISABLED): Enable the generation of Channel Disabled Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_CH_DISABLED): Disable the generation of Channel Disabled Interrupt in CH1_INTSTATUSREG
	
	DMAC_ENABLE_CH_ABORTED_INTSTAT_SHIFT						= 31,	// [31], R/W, Channel Aborted Status Enable.
																		// ¡ö 0: Disable the generation of Channel Aborted Interrupt in CHx_INTSTATUSREG
																		// ¡ö 1: Enable the generation of Channel Aborted Interrupt in CHx_INTSTATUSREG
																		// Values: 
																		// ¡ö 0x1 (ENABLE_CH_ABORTED): Enable the generation of Channel Aborted Interrupt in CH1_INTSTATUSREG
																		// ¡ö 0x0 (DISABLE_CH_ABORTED): Disable the generation of Channel Aborted Interrupt in CH1_INTSTATUSREG
	
	RSVD_DMAC_CHX_INTSTATUS_ENABLEREG_32TO63_SHIFT				= 32	// [63:32], R, DMAC Channelx Interrupt Status Register (bits 32to63) Reserved bits - Read Only
}E_DMAC_CH_INTSTATUS_ENABLEREG_SHIFT;

// define mask 
#define DMAC_BLOCK_TFR_DONE_INTSTAT_MASK 				(1<<DMAC_ENABLE_BLOCK_TFR_DONE_INTSTAT_SHIFT)
#define DMAC_DMA_TFR_DONE_INTSTAT_MASK					(1<<DMAC_ENABLE_DMA_TFR_DONE_INTSTAT_SHIFT)
#define DMAC_SRC_TRANSCOMP_INTSTAT_MASK					(1<<DMAC_ENABLE_SRC_TRANSCOMP_INTSTAT_SHIFT)
#define DMAC_DST_TRANSCOMP_INTSTAT_MASK					(1<<DMAC_ENABLE_DST_TRANSCOMP_INTSTAT_SHIFT)
#define DMAC_SRC_DEC_ERR_INTSTAT_MASK					(1<<DMAC_ENABLE_SRC_DEC_ERR_INTSTAT_SHIFT)
#define DMAC_DST_DEC_ERR_INTSTAT_MASK					(1<<DMAC_ENABLE_DST_DEC_ERR_INTSTAT_SHIFT)
#define DMAC_SRC_SLV_ERR_INTSTAT_MASK					(1<<DMAC_ENABLE_SRC_SLV_ERR_INTSTAT_SHIFT)
#define DMAC_DST_SLV_ERR_INTSTAT_MASK					(1<<DMAC_ENABLE_DST_SLV_ERR_INTSTAT_SHIFT)
#define DMAC_LLI_RD_DEC_ERR_INTSTAT_MASK				(1<<DMAC_ENABLE_LLI_RD_DEC_ERR_INTSTAT_SHIFT)
#define DMAC_LLI_WR_DEC_ERR_INTSTAT_MASK				(1<<DMAC_ENABLE_LLI_WR_DEC_ERR_INTSTAT_SHIFT)
#define DMAC_LLI_RD_SLV_ERR_INTSTAT_MASK				(1<<DMAC_ENABLE_LLI_RD_SLV_ERR_INTSTAT_SHIFT)
#define DMAC_LLI_WR_SLV_ERR_INTSTAT_MASK				(1<<DMAC_ENABLE_LLI_WR_SLV_ERR_INTSTAT_SHIFT)
#define DMAC_SHADOWREG_OR_LLI_INVALID_ERR_INTSTAT_MASK	(1<<DMAC_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_INTSTAT_SHIFT)
#define DMAC_SLVIF_MULTIBLKTYPE_ERR_INTSTAT_MASK		(1<<DMAC_ENABLE_SLVIF_MULTIBLKTYPE_ERR_INTSTAT_SHIFT)
#define DMAC_SLVIF_DEC_ERR_INTSTA_MASK					(1<<DMAC_ENABLE_SLVIF_DEC_ERR_INTSTA_SHIFT)
#define DMAC_SLVIF_WR2RO_ERR_INTSTAT_MASK				(1<<DMAC_ENABLE_SLVIF_WR2RO_ERR_INTSTAT_SHIFT)
#define DMAC_SLVIF_RD2RWO_ERR_INTSTAT_MASK				(1<<DMAC_ENABLE_SLVIF_RD2RWO_ERR_INTSTAT_SHIFT)
#define DMAC_SLVIF_WRONCHEN_ERR_INTSTAT_MASK			(1<<DMAC_ENABLE_SLVIF_WRONCHEN_ERR_INTSTAT_SHIFT)
#define DMAC_SLVIF_SHADOWREG_WRON_VALID_ERR_INTSTA_MASK	(1<<DMAC_ENABLE_SLVIF_SHADOWREG_WRON_VALID_ERR_INTSTA_SHIFT)
#define DMAC_SLVIF_WRONHOLD_ERR_INTSTAT_MASK			(1<<DMAC_ENABLE_SLVIF_WRONHOLD_ERR_INTSTAT_SHIFT)
#define DMAC_CH_LOCK_CLEARED_INTSTAT_MASK				(1<<DMAC_ENABLE_CH_LOCK_CLEARED_INTSTAT_SHIFT)
#define DMAC_CH_SRC_SUSPENDED_INTSTAT_MASK				(1<<DMAC_ENABLE_CH_SRC_SUSPENDED_INTSTAT_SHIFT)
#define DMAC_CH_SUSPENDED_INTSTAT_MASK					(1<<DMAC_ENABLE_CH_SUSPENDED_INTSTAT_SHIFT)
#define DMAC_CH_DISABLED_INTSTAT_MASK					(1<<DMAC_ENABLE_CH_DISABLED_INTSTAT_SHIFT)
#define DMAC_CH_ABORTED_INTSTAT_MASK					(1<<DMAC_ENABLE_CH_ABORTED_INTSTAT_SHIFT)

#define DMAC_INTST_TRANS_DONE_MASK		(DMAC_BLOCK_TFR_DONE_INTSTAT_MASK  	|\
										 DMAC_DMA_TFR_DONE_INTSTAT_MASK		|\
										 DMAC_SRC_TRANSCOMP_INTSTAT_MASK	|\
										 DMAC_DST_TRANSCOMP_INTSTAT_MASK)

#define DMAC_INTST_TRANS_ERR_MASK		(DMAC_SRC_DEC_ERR_INTSTAT_MASK	|\
										 DMAC_DST_DEC_ERR_INTSTAT_MASK	|\
										 DMAC_SRC_SLV_ERR_INTSTAT_MASK	|\
										 DMAC_DST_SLV_ERR_INTSTAT_MASK)

#endif

