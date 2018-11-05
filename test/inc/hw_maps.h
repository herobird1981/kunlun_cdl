/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __HW_MAPS__
#define __HW_MAPS__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
// remap address mask 
#define DSP_BASE_MASK		0x0FFFFFFF

// DSP configuration
#define DSP_BASE			0x40000000

// DSP AXI configuration
#define DSP0_AXI_BASE		0x30000000
#define DSP1_AXI_BASE		0x50000000

// dsp0/1 core internal memory, local(virtual) address
#define DSP_IRAM0_BASE		0x40020000	// 64KB
#define DSP_IRAM1_BASE		0x40030000	// 32KB
#define DSP_DRAM0_BASE		0x40010000	// 64KB
#define DSP_DRAM1_BASE		0x40008000	// 32KB

// dsp0 core internal memory, AXI(system) address
#define DSP0_IRAM0_AXI_BASE		((DSP_IRAM0_BASE & DSP_BASE_MASK) + DSP0_AXI_BASE)
#define DSP0_IRAM1_AXI_BASE		((DSP_IRAM1_BASE & DSP_BASE_MASK) + DSP0_AXI_BASE)
#define DSP0_DRAM0_AXI_BASE		((DSP_DRAM0_BASE & DSP_BASE_MASK) + DSP0_AXI_BASE)
#define DSP0_DRAM1_AXI_BASE		((DSP_DRAM1_BASE & DSP_BASE_MASK) + DSP0_AXI_BASE)

// dsp1 core internal memory, AXI(system) address
#define DSP1_IRAM0_AXI_BASE		((DSP_IRAM0_BASE & DSP_BASE_MASK) + DSP1_AXI_BASE)
#define DSP1_IRAM1_AXI_BASE		((DSP_IRAM1_BASE & DSP_BASE_MASK) + DSP1_AXI_BASE)
#define DSP1_DRAM0_AXI_BASE		((DSP_DRAM0_BASE & DSP_BASE_MASK) + DSP1_AXI_BASE)
#define DSP1_DRAM1_AXI_BASE		((DSP_DRAM1_BASE & DSP_BASE_MASK) + DSP1_AXI_BASE)

// AXI_S, shared RAM0/1/2/3
#define SRAM0_BASE		0x60000000	
#define SRAM0_SIZE		0x80000

#define XRAM_ADDR		0x60070000	// size 0x10000 used VAD

#define SRAM1_BASE		0x60080000
#define SRAM1_SIZE		0x80000

#define SRAM2_BASE		0x60100000
#define SRAM2_SIZE		0x80000

#define SRAM3_BASE		0x60180000
#define SRAM3_SIZE		0x80000

// AXI_S, ROM
#define ROM_BASE		0x60A00000
#define ROM_SIZE		0x10000

// AHB_S, AXI_M
#define DMAC_BASE		0x60200000

// AHB_S
#define USBC_BASE		0x60300000

// APB0
#define I2S_ADC_BASE	0x60500000
#define I2S_DAC_BASE 	0x60501000
#define I2S1_BASE		0x60502000
#define I2S_VAD_BASE	0x60503000
#define VAD_BASE		0x60503800
#define SPI_BASE		0x60504000  
#define I2C0_BASE		0x60505000	// slave
#define I2C1_BASE		0x60506000	// master
#define ACDC_UCP_BASE	0x60507000

// APB1
#define UART0_BASE		0x60600000
#define UART1_BASE		0x60601000
#define PTC_BASE		0x60602000
#define WDT_BASE		0x60603000  
#define GPIO_BASE		0x60604000  
#define TIMER_BASE		0x60605000

#define PINMUX_BASE		0x60606000
#define SYSCON_BASE		0x60607000
#define CLKGEN_BASE		0x60608000
#define RSTGEN_BASE		0x60609000 

// macro definition for files created by machine
#define PAD_SHARE_BASE_ADDR	PINMUX_BASE
#define SYS_CON_BASE_ADDR	SYSCON_BASE
#define CLK_GEN_BASE_ADDR	CLKGEN_BASE
#define RST_GEN_BASE_ADDR	RSTGEN_BASE

#endif

