/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_UART_REG_H__
#define __CDL_UART_REG_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define UART_PORT_SIZE	0x1000

// register layout
#define UART_RBR_OFST			0x00 	// R, 32 bit, Receive Buffer Register
#define UART_THR_OFST			0x00 	// W, 32 bit, Transmit Holding Register
#define UART_Dll_OFST			0x00 	// R/W, 32 bit, Divisor Latch (Low)

#define UART_DLH_OFST			0x04 	// R/W, 32 bit, Divisor Latch (High)
#define UART_IER_OFST			0x04 	// R/W, 32 bit, Interrupt Enable Register

#define UART_IIR_OFST			0x08 	// R, 32 bit, Interrupt Identification Register
#define UART_FCR_OFST			0x08 	// W, 32 bit, FIFO Control Register

#define UART_LCR_OFST			0x0C 	// R/W, 32 bit, Line Control Register
#define UART_MCR_OFST			0x10 	// R/W, 32 bit, Modem Control Register
#define UART_LSR_OFST			0x14 	// R, 32 bit, Line Status Register
#define UART_MSR_OFST			0x18 	// R, 32 bit, Modem Status Register
#define UART_SCR_OFST			0x1C 	// R/W, 32 bit, Scratchpad Register
#define UART_LPDDL_OFST			0x20 	// R/W, 32 bit, Low Power Divisor Latch (Low) Register
#define UART_LPDLH_OFST			0x24 	// R/W, 32 bit, Low Power Divisor Latch (High) Register

// [0X30-0X6C]
#define UART_SRBR_OFST			0x30 	// R, 32 bit, Shadow Receive Buffer Register
#define UART_STHR_OFST			0x30 	// W, 32 bit, Shadow Transmit Holding Register

#define UART_FAR_OFST			0x70 	// R/W, 32 bit, FIFO Access Register
#define UART_TFR_OFST			0x74 	// R, 32 bit, Transmit FIFO Read
#define UART_RFW_OFST			0x78 	// W, 32 bit, Receive FIFO Write
#define UART_USR_OFST			0x7C 	// R, 32 bit, UART Status Register
#define UART_TFL_OFST			0x80 	// R, XX bit, Transmit FIFO Level
#define UART_RFL_OFST			0x84 	// R, XX bit, Receive FIFO Level
#define UART_SRR_OFST			0x88 	// W, 32 bit, Software Reset Register
#define UART_SRTS_OFST			0x8C 	// R/W, 32 bit, Shadow Request to Send
#define UART_SBCR_OFST			0x90 	// R/W, 32 bit, Shadow Break Control Register
#define UART_SDMAM_OFST			0x94 	// R/W, 32 bit, Shadow DMA Mode
#define UART_SFE_OFST			0x98 	// R/W, 32 bit, Shadow FIFO Enable
#define UART_SRT_OFST			0x9C 	// R/W, 32 bit, Shadow RCVR Trigger
#define UART_STET_OFST			0xA0 	// R/W, 32 bit, Shadow TX Empty Trigger
#define UART_HTX_OFST			0xA4 	// R/W, 32 bit, Halt TX
#define UART_DMASA_OFST			0xA8 	// W, 1 bit, DMA Software Acknowledge
#define UART_TCR_OFST			0xAC 	// R/W, 32 bit, Transceiver Control Register
#define UART_DE_EN_OFST			0xB0 	// R/W, 32 bit, Driver Output Enable Register
#define UART_RE_EN_OFST			0xB4 	// R/W, 32 bit, Receiver Output Enable Register.
#define UART_DET_OFST			0xB8 	// R/W, 32 bit, Driver Output Enable Timing Register.
#define UART_TAT_OFST			0xBC 	// R/W, 32 bit, TurnAround Timing Register.
#define UART_DLF_OFST			0xC0 	// R/W, 32 bit, Divisor Latch Fractional Value.
#define UART_RAR_OFST			0xC4 	// R/W, 32 bit, Receive Address Register
#define UART_TAR_OFST			0xC8 	// R/W, 32 bit, Transmit Address Register
#define UART_LCR_EXT_OFST		0xCC 	// R/W, 32 bit,  Line Extended Control Register

// [0xD0-0xF0], reserved

#define UART_CPR_OFST			0xF4 	// R, 32 bit, Component Parameter Register
#define UART_UCV_OFST			0xF8 	// R, 32 bit, UART Component Version
#define UART_CTR_OFST			0xFC 	// R, 32 bit, Component Type Register


// offset: 0x04, Interrupt Enable Register
typedef enum
{
	UART_ERBFI_SHIFT 		= 0,	// [0], R/W, Enable Received Data Available Interrupt. This is used to enable/disable the 
									// generation of Received Data Available Interrupt and the Character Timeout Interrupt 
									// (if in FIFO mode and FIFOs enabled). These are the second highest priority interrupts.
									// ← 0 每 disabled
									// ← 1 每 enabled
									// Reset Value: 0x0
	
	UART_ETBEI_SHIFT 		= 1,	// [1], R/W, Enable Transmit Holding Register Empty Interrupt. This is used to enable/disable the 
									// generation of Transmitter Holding Register Empty Interrupt. This is the third highest 
									// priority interrupt.
									// ← 0 每 disabled
									// ← 1 每 enabled
									// Reset Value: 0x0

	UART_ELSI_SHIFT 		= 2,	// [2], R/W, Enable Receiver Line Status Interrupt. This is used to enable/disable the generation 
									// of Receiver Line Status Interrupt. This is the highest priority interrupt.
									// ← 0 每 disabled
									// ← 1 每 enabled
									// Reset Value: 0x0
	
	UART_EDSSI_SHIFT 		= 3,	// [3], R/W, Enable Modem Status Interrupt. This is used to enable/disable the generation of 
									// Modem Status Interrupt. This is the fourth highest priority interrupt.
									// ← 0 每 disabled
									// ← 1 每 enabled
									// Reset Value: 0x0
	
	UART_IER_REVD0_SHIFT  	= 4,	// [6:4], Read/write, RO, Reserved, read undefined, must read as zeros.

	UART_PTIME_SHIFT 		= 7,	// [7], R/W, Programmable THRE Interrupt Mode Enable that can be written to only when 
									// THRE_MODE_USER = Enabled, always readable. This is used to enable/disable the generation of THRE Interrupt.
									// ← 0 每 disabled
									// ← 1 每 enabled
									// Reset Value: 0x0
										
	UART_IER_REVD1_SHIFT	= 8		// [31:8], RO, Reserved, read undefined, must read as zeros.
}E_UART_IER_SHIFT;

// common
typedef enum
{
	UART_INT_DIS	= 0,
	UART_INT_EN		= 1
}E_UART_IER;

//[0],  ERBFI
#define UART_ERBFI_MASK		(1<<UART_ERBFI_SHIFT)
#define UART_ERBFI_VAL(val)	((val)<<UART_ERBFI_SHIFT)

//[1],  ETBEI
#define UART_ETBEI_MASK		(1<<UART_ETBEI_SHIFT)
#define UART_ETBEI_VAL(val)	((val)<<UART_ETBEI_SHIFT)

//[2],  ELSI
#define UART_ELSI_MASK		(1<<UART_ELSI_SHIFT)
#define UART_ELSI_VAL(val)	((val)<<UART_ELSI_SHIFT)

//[3],  EDSSI
#define UART_EDSSI_MASK		(1<<UART_EDSSI_SHIFT)
#define UART_EDSSI_VAL(val)	((val)<<UART_EDSSI_SHIFT)

//[7],  PTIME
#define UART_PTIME_MASK		(1<<UART_PTIME_SHIFT)
#define UART_PTIME_VAL(val)	((val)<<UART_PTIME_SHIFT)

// offset: 0x08, Interrupt Identity Register
typedef enum
{	
	UART_IID_SHIFT 			= 0,	// [3:0], R, nterrupt ID. This indicates the highest priority pending interrupt which can be one 
									// of the following types:
									// ← 0000 每 modem status
									// ← 0001 每 no interrupt pending
									// ← 0010 每 THR empty
									// ← 0100 每 received data available
									// ← 0110 每 receiver line status
									// ← 0111 每 busy detect
									// ← 1100 每 character timeout
	
	UART_IIR_REVD0_SHIFT	= 4,	// [5:4], Read/write, RO, Reserved, read undefined, must read as zeros.

	UART_FIFOSE_SHIFT 		= 6,	// [7:6], R, FIFOs Enabled. This is used to indicate whether the FIFOs are enabled or disabled.
									// ← 00 每 disabled
									// ← 11 每 enabled
									// Reset Value: 0x00
										
	UART_IIR_REVD1_SHIFT	= 8		// [31:8], RO, Reserved, read undefined, must read as zeros.
}E_UART_IIR_SHIFT;

// [3:0], IID
#define UART_IID_MASK		(0xF<<UART_IID_SHIFT)
#define UART_IID_VAL(val)	((val)<<UART_IID_SHIFT)

typedef enum
{
	UART_IIR_MSI			= 0x0, // modem status
	UART_IIR_NO_INT			= 0x1, // no interrupt pending
	UART_IIR_THRI			= 0x2, // THR empty
	UART_IIR_RDI			= 0x4, // received data available
	UART_IIR_RLSI			= 0x6, //  receiver line status
	UART_IID_BUSY_DETECT	= 0x7, // busy detect
	UART_IIR_CHTO			= 0xC  // character timeout
}E_UART_IID;

// [7:6], FIFOSE
#define UART_FIFOSE_MASK		(0x3<<UART_FIFOSE_SHIFT)
#define UART_FIFOSE_VAL(val)	((val)<<UART_FIFOSE_SHIFT)

typedef enum
{
	UART_FIFO_DIS	= 0, // modem status
	UART_FIFO_EN	= 3
}E_UART_FIFOSE;

// offset: 0x08, Interrupt Identity Register
typedef enum
{	
	UART_FIFOE_SHIFT 		= 0,	// [0], W, FIFO Enable. This enables/disables the transmit (XMIT) and receive (RCVR) 
									// FIFOs. Whenever the value of this bit is changed both the XMIT and RCVR 
									// controller portion of FIFOs is reset.
									// Reset Value: 0x0

	UART_RFIFOR_SHIFT 		= 1,	// [1], W, RCVR FIFO Reset. This resets the control portion of the receive FIFO and treats 
									// the FIFO as empty. This also de-asserts the DMA RX request and single signals 
									// when additional DMA handshaking signals are selected (DMA_EXTRA = YES). 
									// Note that this bit is 'self-clearing'. It is not necessary to clear this bit.
									// Reset Value: 0x0

	UART_XFIFOR_SHIFT 		= 2,	// [2], W, XMIT FIFO Reset. This resets the control portion of the transmit FIFO and treats 
									// the FIFO as empty. This also de-asserts the DMA TX request and single signals 
									// when additional DMA handshaking signals are selected (DMA_EXTRA = YES). 
									// Note that this bit is 'self-clearing'. It is not necessary to clear this bit.
									// Reset Value: 0x0
	 
	UART_DMAM_SHIFT 		= 3,	// [3], W, DMA Mode. This determines the DMA signalling mode used for the dma_tx_req_n 
									// and dma_rx_req_n output signals when additional DMA handshaking signals are 
									// not selected (DMA_EXTRA = No). For details on DMA support, refer to ※DMA 
									// Support§ on page 72.
									// ← 0 每 mode 0
									// ← 1 每 mode 1
									// Reset Value: 0x0
	
	UART_TET_SHIFT			= 4,	// [5:4], W, TX Empty Trigger. Writes have no effect when THRE_MODE_USER = Disabled. 
									// This is used to select the empty threshold level at which the THRE Interrupts are 
									// generated when the mode is active. It also determines when the dma_tx_req_n 
									// signal is asserted when in certain modes of operation. For details on DMA support, 
									// refer to ※DMA Support§ on page 72. The following trigger levels are supported:
									// ← 00 每 FIFO empty
									// ← 01 每 2 characters in the FIFO
									// ← 10 每 FIFO 1/4 full
									// ← 11 每 FIFO 1/2 full	
									// Reset Value: 0x0

	UART_RT_SHIFT 			= 6,	// [7:6], W, RCVR Trigger. This is used to select the trigger level in the receiver FIFO at which 
									// the Received Data Available Interrupt is generated. In auto flow control mode, this 
									// trigger is used to determine when the rts_n signal is de-asserted only when 
									// RTC_FCT is disabled. It also determines when the dma_rx_req_n signal is 
									// asserted in certain modes of operation. For details on DMA support, refer to ※DMA 
									// Support§ on page 72. The following trigger levels are supported:
									// ← 00 每 1 character in the FIFO
									// ← 01 每 FIFO 1/4 full
									// ← 10 每 FIFO 1/2 full
									// ← 11 每 FIFO 2 less than full
									// Reset Value: 0x0
										
	UART_FCR_REVD1_SHIFT	= 8		// [31:8], RO, Reserved, read undefined, must read as zeros.
}E_UART_FCR_SHIFT;


// [0], FIFOE
#define UART_FCR_FIFOE_MASK		(1<<UART_FIFOE_SHIFT)
#define UART_FCR_FIFOE_VAL(val)	((val)<<UART_FIFOE_SHIFT)

// [1], RFIFOR
#define UART_FCR_RFIFOR_MASK		(1<<UART_RFIFOR_SHIFT)
#define UART_FCR_RFIFOR_VAL(val)	((val)<<UART_RFIFOR_SHIFT)

// [2], XFIFOR
#define UART_FCR_XFIFOR_MASK		(1<<UART_XFIFOR_SHIFT)
#define UART_FCR_XFIFOR_VAL(val)	((val)<<UART_XFIFOR_SHIFT)

// [3], DMAM
#define UART_FCR_DMAM_MASK		(1<<UART_DMAM_SHIFT)
#define UART_FCR_DMAM_VAL(val)	((val)<<UART_DMAM_SHIFT)

typedef enum
{
	UART_DMA_MODE0	= 0,
	UART_DMA_MODE1	= 1
}E_UART_DMAM;

// [5:4], TET
#define UART_FCR_TET_MASK		(3<<UART_TET_SHIFT)
#define UART_FCR_TET_VAL(val)	((val)<<UART_TET_SHIFT)

typedef enum
{
	UART_TET_FIFO_EMPTY				= 0, // FIFO empty
	UART_TET_FIFO_2_CHARACTER		= 1, // 2 characters in the FIFO
	UART_TET_FIFO_1_4_FULL			= 2, // FIFO 1/4 full
	UART_TET_FIFO_1_2_FULL			= 3  // FIFO 1/2 full
}E_UART_TET_FIFO;

// [7:6], RT
#define UART_FCR_RT_MASK		(3<<UART_RT_SHIFT)
#define UART_FCR_RT_VAL(val)	((val)<<UART_RT_SHIFT)

typedef enum
{
	UART_RT_FIFO_1_CHARACTER		= 0, // 1 character in the FIFO
	UART_RT_FIFO_1_4_FULL			= 1, // FIFO 1/4 full
	UART_RT_FIFO_1_2_FULL			= 2, // FIFO 1/2 full
	UART_RT_FIFO_2_LESS_THAN_FULL	= 3	 // FIFO 2 less than full
}E_UART_RT_FIFO;

// default FCR
#define UART_FCRVAL (UART_FCR_TET_VAL(UART_TET_FIFO_1_2_FULL)|\
					UART_FCR_RT_VAL(UART_RT_FIFO_1_4_FULL)|\
					UART_FCR_DMAM_MASK|\
					UART_FCR_RFIFOR_MASK|\
					UART_FCR_XFIFOR_MASK|\
					UART_FCR_FIFOE_MASK)

#define UART_FCRVAL_AFCE (UART_FCR_TET_VAL(UART_TET_FIFO_1_2_FULL)|\
							UART_FCR_RT_VAL(UART_RT_FIFO_1_2_FULL)|\
							UART_FCR_DMAM_MASK|\
							UART_FCR_RFIFOR_MASK|\
							UART_FCR_XFIFOR_MASK|\
							UART_FCR_FIFOE_MASK)


// offset: 0x0C, Line Control Register
typedef enum
{	
	UART_DLS_SHIFT 			= 0,	// [1:0], R/W, Data Length Select. If UART_16550_COMPATIBLE = NO, then writable only 
									// when UART is not busy (USR[0] is 0); otherwise always writable and readable. 
									// When DLS_E in LCR_EXT is set to 0, this register is used to select the number 
									// of data bits per character that the peripheral transmits and receives. The number 
									// of bits that may be selected are as follows:
									// ← 00 每 5 bits
									// ← 01 每 6 bits
									// ← 10 每 7 bits
									// ← 11 每 8 bits
									// Reset Value: 0x0

	UART_STOP_SHIFT 		= 2,	// [2], R/W, Number of stop bits. If UART_16550_COMPATIBLE = NO, then writeable only 
									// when UART is not busy (USR[0] is 0); otherwise always writable, always 
									// readable. This is used to select the number of stop bits per character that the 
									// peripheral transmits and receives. If set to 0, one stop bit is transmitted in the 
									// serial data.
									// If set to 1 and the data bits are set to 5 (LCR[1:0] set to 0) one and a half stop 
									// bits is transmitted. Otherwise, two stop bits are transmitted. Note that regardless 
									// of the number of stop bits selected, the receiver checks only the first stop bit.
									// ← 0 每 1 stop bit
									// ← 1 每 1.5 stop bits when DLS (LCR[1:0]) is 0, else 2 stop bit
									// NOTE: The STOP bit duration implemented by DW_apb_uart may appear longer 
									// due to idle time inserted between characters for some configurations and baud 
									// clock divisor values in the transmit direction; for details on idle time between 
									// transmitted transfers, refer to ※Back-to-Back Character Stream Transmission§ on page 62.
									// Reset Value: 0x0
	
	UART_PEN_SHIFT 			= 3,	// [3], R/W, Parity Enable. If UART_16550_COMPATIBLE = NO, then writeable only when 
									// UART is not busy (USR[0] is 0); otherwise always writable, always readable. This 
									// bit is used to enable and disable parity generation and detection in transmitted 
									// and received serial character respectively.
									// ← 0 每 parity disabled
									// ← 1 每 parity enabled
									// Reset Value: 0x0
	
	UART_EPS_SHIFT 			= 4,	// [4], R/W, Even Parity Select. If UART_16550_COMPATIBLE = NO, then writeable only 
									// when UART is not busy (USR[0] is 0); otherwise always writable, always 
									// readable. This is used to select between even and odd parity, when parity is 
									// enabled (PEN set to 1). If set to 1, an even number of logic 1s is transmitted or 
									// checked. If set to 0, an odd number of logic 1s is transmitted or checked.
									// Reset Value: 0x0
	
	UART_STICK_PARITY_SHIFT = 5,	// [5], R/W, Stick Parity. If UART_16550_COMPATIBLE = NO, then writeable only when 
									// UART is not busy (USR[0] is 0); otherwise always writable, always readable. This 
									// bit is used to force parity value. When PEN, EPS, and Stick Parity are set to 1, 
									// the parity bit is transmitted and checked as logic 0. If PEN and Stick Parity are 
									// set to 1 and EPS is a logic 0, then parity bit is transmitted and checked as a 
									// logic 1. If this bit is set to 0, Stick Parity is disabled.
									// Reset Value: 0x0
	
	UART_BC_SHIFT 			= 6,	// [6], R/W, Break Control Bit.This is used to cause a break condition to be transmitted to the 
									// receiving device. If set to 1, the serial output is forced to the spacing (logic 0) 
									// state. When not in Loopback Mode, as determined by MCR[4], the sout line is 
									// forced low until the Break bit is cleared. If SIR_MODE = Enabled and active 
									// (MCR[6] set to 1) the sir_out_n line is continuously pulsed. When in Loopback 
									// Mode, the break condition is internally looped back to the receiver and the 
									// sir_out_n line is forced low.
									// Reset Value: 0x0

	UART_DLAB_SHIFT 		= 7,	// [7], R/W, Divisor Latch Access Bit. If UART_16550_COMPATIBLE = NO, then writeable 
									// only when UART is not busy (USR[0] is 0); otherwise always writable, always 
									// readable. This bit is used to enable reading and writing of the Divisor Latch 
									// register (DLL and DLH/LPDLL and LPDLH) to set the baud rate of the UART. 
									// This bit must be cleared after initial baud rate setup in order to access other registers.
									// Reset Value: 0x0
										
	UART_LCR_REVD1_SHIFT	= 8		// [31:8], RO, Reserved, read undefined, must read as zeros.
}E_UART_LCR_SHIFT;

// [1:0], DLS (Data Length Select)
#define UART_DLS_MASK		(3<<UART_DLS_SHIFT)
#define UART_DLS_VAL(val)	((val)<<UART_DLS_SHIFT)

typedef enum
{
	UART_DLS_5_BITS		= 0, 
	UART_DLS_6_BITS		= 1, 
	UART_DLS_7_BITS		= 2, 
	UART_DLS_8_BITS		= 3 
}E_UART_DLS;

// [2], STOP
#define UART_STOP_MASK		(1<<UART_STOP_SHIFT)
#define UART_STOP_VAL(val)	((val)<<UART_STOP_SHIFT)

typedef enum
{
	UART_STOP_1_BIT		= 0, 
	UART_STOP_1_5_BIT	= 1 
}E_UART_STOP;

// [3], PEN
// [4], EPS
#define UART_EPS_MASK		(3<<UART_PEN_SHIFT)
#define UART_EPS_VAL(val)	((val)<<UART_PEN_SHIFT)

typedef enum
{
	UART_PARITY_NONE	= 0,
	UART_PARITY_ODD		= 1, 
	UART_PARITY_EVEN	= 3 
}E_UART_EPS;

// [5], Stick Parity
#define UART_STICK_PARITY_MASK		(1<<UART_STICK_PARITY_SHIFT)
#define UART_STICK_PARITY_VAL(val)	((val)<<UART_STICK_PARITY_SHIFT)

// [6], Break (or BC)
#define UART_BC_MASK		(1<<UART_BC_SHIFT)
#define UART_BC_VAL(val)	((val)<<UART_BC_SHIFT)

// [7], DLAB
#define UART_DLAB_MASK		(1<<UART_DLAB_SHIFT)
#define UART_DLAB_VAL(val)	((val)<<UART_DLAB_SHIFT)

typedef enum
{
	UART_DLAB_DIS	= 0, 
	UART_DLAB_EN	= 1 
}E_UART_DLAB;

// offset: 0x10, Modem Control Register
typedef enum
{
	UART_DTR_SHIFT 			= 0,	// [0], R/W, Data Terminal Ready. This is used to directly control the Data Terminal Ready (dtr_n) 
									// output. The value written to this location is inverted and driven out on dtr_n, that is:
									// ← 0 每 dtr_n de-asserted (logic 1)
									// ← 1 每 dtr_n asserted (logic 0)
									// The Data Terminal Ready output is used to inform the modem or data set that the 
									// UART is ready to establish communications. Note that in Loopback mode (MCR[4] set 
									// to 1), the dtr_n output is held inactive high while the value of this location is internally 
									// looped back to an input.
	
	UART_RTS_SHIFT 			= 1,	// [1], R/W, Request to Send. This is used to directly control the Request to Send (rts_n) output. 
									// The Request To Send (rts_n) output is used to inform the modem or data set that the 
									// UART is ready to exchange data.
									// When Auto RTS Flow Control is not enabled (MCR[5] set to 0), the rts_n signal is set 
									// low by programming MCR[1] (RTS) to a high.In Auto Flow Control, AFCE_MODE = 
									// Enabled and active (MCR[5] set to 1) and FIFOs enable (FCR[0] set to 1), the rts_n 
									// output is controlled in the same way, but is also gated with the receiver FIFO 
									// threshold trigger (rts_n is inactive high when above the threshold) only when the RTC 
									// Flow Trigger is disabled; otherwise it is gated by the receiver FIFO almost-full trigger, 
									// where ※almost full§ refers to two available slots in the FIFO (rts_n is inactive high 
									// when above the threshold). The rts_n signal is de-asserted when MCR[1] is set low.
									// Note that in Loopback mode (MCR[4] set to 1), the rts_n output is held inactive high 
									// while the value of this location is internally looped back to an input.
	
	UART_OUT1_SHIFT 		= 2,	// [2], R/W, OUT1. This is used to directly control the user-designated Output1 (out1_n) output. 
									// The value written to this location is inverted and driven out on out1_n, that is:
									// ← 0 每 out1_n de-asserted (logic 1)
									// ← 1 每 out1_n asserted (logic 0)
									// Note that in Loopback mode (MCR[4] set to 1), the out1_n output is held inactive high 
									// while the value of this location is internally looped back to an input.  
	
	UART_OUT2_SHIFT 		= 3,	// [3], R/W, OUT2. This is used to directly control the user-designated Output2 (out2_n) output. 
									// The value written to this location is inverted and driven out on out2_n, that is:
									// ← 0 每 out2_n de-asserted (logic 1)
									// ← 1 每 out2_n asserted (logic 0)
									// Note that in Loopback mode (MCR[4] set to 1), the out2_n output is held inactive high 
									// while the value of this location is internally looped back to an input
	
	UART_LOOPBACK_SHIFT 	= 4,	// [4], R/W, LoopBack Bit. This is used to put the UART into a diagnostic mode for test purposes.
									// If operating in UART mode (SIR_MODE != Enabled or not active, MCR[6] set to 0), 
									// data on the sout line is held high, while serial data output is looped back to the sin 
									// line, internally. In this mode all the interrupts are fully functional. Also, in loopback 
									// mode, the modem control inputs (dsr_n, cts_n, ri_n, dcd_n) are disconnected and the 
									// modem control outputs (dtr_n, rts_n, out1_n, out2_n) are looped back to the inputs, 
									// internally.
									// If operating in infrared mode (SIR_MODE = Enabled AND active, MCR[6] set to 1), 
									// data on the sir_out_n line is held low, while serial data output is inverted and looped 
									// back to the sir_in line.
	
	UART_AFCE_SHIFT			= 5,	// [5], R/W, Auto Flow Control Enable. Writeable only when AFCE_MODE = Enabled, always 
									// readable. When FIFOs are enabled and the Auto Flow Control Enable (AFCE) bit is 
									// set, Auto Flow Control features are enabled as described in ※Auto Flow Control§ on page 64.
									// ← 0 每 Auto Flow Control Mode disabled
									// ← 1 每 Auto Flow Control Mode enabled

	UART_SIRE_SHIFT 		= 6,	// [6], R/W, SIR Mode Enable. Writeable only when SIR_MODE = Enabled, always readable. This 
									// is used to enable/disable the IrDA SIR Mode features as described in ※IrDA 1.0 SIR 
									// Protocol§ on page 55.
									// ← 0 每 IrDA SIR Mode disabled
									// ← 1 每 IrDA SIR Mode enabled
										
	UART_MCR_REVD1_SHIFT	= 7		// [31:7], RO, Reserved, read undefined, must read as zeros.
}E_UART_MCR_SHIFT;

// bit[0], Data Terminal Ready mask
#define UART_DTR_MASK	(1<<UART_DTR_SHIFT)

// bit[1], Request to Send mask
#define UART_RTS_MASK	(1<<UART_RTS_SHIFT)

// bit[5], 
#define UART_AFCE_MASK	(1<<UART_AFCE_SHIFT)

// offset: 0x14, Line Status Register
typedef enum
{
	UART_DR_SHIFT 			= 0,	// [0], R, Data Ready bit. This is used to indicate that the receiver contains at least one character in 
									// the RBR or the receiver FIFO. 
									// ← 0 每 no data ready
									// ← 1 每 data ready
									// This bit is cleared when the RBR is read in non-FIFO mode, or when the receiver FIFO is 
									// empty, in FIFO mode.
									// Reset Value: 0x0

	UART_OE_SHIFT 			= 1,	// [1], R, Overrun error bit. This is used to indicate the occurrence of an overrun error. This occurs 
									// if a new data character was received before the previous data was read.
									// In the non-FIFO mode, the OE bit is set when a new character arrives in the receiver 
									// before the previous character was read from the RBR. When this happens, the data in the 
									// RBR is overwritten. In the FIFO mode, an overrun error occurs when the FIFO is full and a 
									// new character arrives at the receiver. The data in the FIFO is retained and the data in the 
									// receive shift register is lost.
									// ← 0 每 no overrun error
									// ← 1 每 overrun error
									// Reading the LSR clears the OE bit.
	
	UART_PE_SHIFT 			= 2,	// [2], R, Parity Error bit. This is used to indicate the occurrence of a parity error in the receiver if 
									// the Parity Enable (PEN) bit (LCR[3]) is set. 
									// In the FIFO mode, since the parity error is associated with a character received, it is 
									// revealed when the character with the parity error arrives at the top of the FIFO. 
									// It should be noted that the Parity Error (PE) bit (LSR[2]) can be set if a break interrupt has 
									// occurred, as indicated by Break Interrupt (BI) bit (LSR[4]). In this situation, the Parity Error 
									// bit is set if parity generation and detection is enabled (LCR[3]=1) and the parity is set to odd (LCR[4]=0).
									// ← 0 每 no parity error
									// ← 1 每 parity error
									// Reading the LSR clears the PE bit.
	
	UART_FE_SHIFT 			= 3,	// [3], R, Framing Error bit. This is used to indicate the occurrence of a framing error in the receiver. 
									// A framing error occurs when the receiver does not detect a valid STOP bit in the received data. 
									// In the FIFO mode, since the framing error is associated with a character received, it is 
									// revealed when the character with the framing error is at the top of the FIFO. When a 
									// framing error occurs, the DW_apb_uart tries to resynchronize. It does this by assuming 
									// that the error was due to the start bit of the next character and then continues receiving 
									// the other bit; that is, data, and/or parity and stop. 
									// It should be noted that the Framing Error (FE) bit (LSR[3]) is set if a break interrupt has 
									// occurred, as indicated by Break Interrupt (BI) bit (LSR[4]). This happens because the 
									// break character implicitly generates a framing error by holding the sin input to logic 0 for 
									// longer than the duration of a character.
									// ← 0 每 no framing error
									// ← 1 每 framing error
									// Reading the LSR clears the FE bit	
									
	UART_BI_SHIFT 			= 4,	// [4], R, Break Interrupt bit. This is used to indicate the detection of a break sequence on the serial input data. 
									// If in UART mode (SIR_MODE = Disabled), it is set whenever the serial input, sin, is held in 
									// a logic '0' state for longer than the sum of start time + data bits + parity + stop bits.
									// If in infrared mode (SIR_MODE = Enabled), it is set whenever the serial input, sir_in, is 
									// continuously pulsed to logic '0' for longer than the sum of start time + data bits + parity + 
									// stop bits. A break condition on serial input causes one and only one character, consisting 
									// of all 0s, to be received by the UART. 
									// In FIFO mode, the character associated with the break condition is carried through the 
									// FIFO and is revealed when the character is at the top of the FIFO. Reading the LSR clears 
									// the BI bit. In non-FIFO mode, the BI indication occurs immediately and persists until the 
									// LSR is read.
									// NOTE: If a FIFO is full when a break condition is received, a FIFO overrun occurs. The 
									// break condition and all the information associated with it〞parity and framing errors〞is 
									// discarded; any information that a break character was received is lost.
	
	UART_THRE_SHIFT 		= 5,	// [5], R, Transmit Holding Register Empty bit. If THRE_MODE_USER = Disabled or THRE mode 
									// is disabled (IER[7] set to 0) and regardless of FIFO's being implemented/enabled or not, 
									// this bit indicates that the THR or TX FIFO is empty. 
									// This bit is set whenever data is transferred from the THR or TX FIFO to the transmitter 
									// shift register and no new data has been written to the THR or TX FIFO. This also causes 
									// a THRE Interrupt to occur, if the THRE Interrupt is enabled. If THRE_MODE_USER = 
									// Enabled AND FIFO_MODE != NONE and both modes are active (IER[7] set to 1 and 
									// FCR[0] set to 1 respectively), the functionality is switched to indicate the transmitter FIFO 
									// is full, and no longer controls THRE interrupts, which are then controlled by the FCR[5:4] 
									// threshold setting. 
									
	UART_TEMT_SHIFT 		= 6,	// [6], R, Transmitter Empty bit. If in FIFO mode (FIFO_MODE != NONE) and FIFOs enabled 
									// (FCR[0] set to 1), this bit is set whenever the Transmitter Shift Register and the FIFO are 
									// both empty. If in non-FIFO mode or FIFOs are disabled, this bit is set whenever the 
									// Transmitter Holding Register and the Transmitter Shift Register are both empty.
	
	UART_RFE_SHIFT			= 7,	// [7], R, Receiver FIFO Error bit. This bit is only relevant when FIFO_MODE != NONE AND FIFOs 
									// are enabled (FCR[0] set to 1). This is used to indicate if there is at least one parity error, 
									// framing error, or break indication in the FIFO.
									// ← 0 每 no error in RX FIFO
									// ← 1 每 error in RX FIFO
									// This bit is cleared when the LSR is read and the character with the error is at the top of the 
									// receiver FIFO and there are no subsequent errors in the FIFO

	UART_ADDR_RCVD_SHIFT 	= 8,	// [8], R/W, Address Received bit
									// If 9-bit data mode (LCR_EXT[0]=1) is enabled, this bit is used to indicate that the 9th bit of 
									// the receive data is set to 1. This bit can also be used to indicate whether the incoming 
									// character is an address or data.
									// ← 1 - Indicates that the character is an address.
									// ← 0 - Indicates that the character is data.
									// In the FIFO mode, since the 9th bit is associated with the received character, it is revealed 
									// when the character with the 9th bit set to 1 is at the top of the FIFO list. Reading the LSR 
									// clears the 9th bit.
									// NOTE: You must ensure that an interrupt gets cleared (reading LSR register) before the 
									// next address byte arrives. If there is a delay in clearing the interrupt, then software will not 
									// be able to distinguish between multiple address related interrupt.
										
	UART_LSR_REVD1_SHIFT	= 9		// [31:9], RO, Reserved, read undefined, must read as zeros.
}E_UART_LSR_SHIFT;

// mask
#define UART_DR_MASK		(1<<UART_DR_SHIFT)
#define UART_OE_MASK		(1<<UART_OE_SHIFT)
#define UART_PE_MASK		(1<<UART_PE_SHIFT)
#define UART_FE_MASK		(1<<UART_FE_SHIFT)
#define UART_BI_MASK		(1<<UART_BI_SHIFT)
#define UART_THRE_MASK		(1<<UART_THRE_SHIFT)
#define UART_TEMT_MASK		(1<<UART_TEMT_SHIFT)
#define UART_RFE_MASK		(1<<UART_RFE_SHIFT)
#define UART_ADDR_RCVD_MASK	(1<<UART_ADDR_RCVD_SHIFT)


#endif

