/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include <xtensa/config/core.h>
#include "test_config.h"
#include "cdl_sys.h"
#include "cdl_i2s.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
static int sys_cache_disable(uint32_t base, uint32_t size)
{
	int ret;
	ret = xthal_set_region_attribute((void *)base, size, XCHAL_CA_BYPASS, XTHAL_CAFLAG_EXACT);
	return ret;
}

static int sys_cache_enable(uint32_t base, uint32_t size)
{
	int ret;
	ret = xthal_set_region_attribute((void *)base, size, XCHAL_CA_WRITETHRU, XTHAL_CAFLAG_EXACT);
	return ret;
}

void sys_peripheral_cache_disable(void)
{
	//unsigned int dc_attr, ic_attr;

	//xthal_dcache_all_invalidate();
	//xthal_icache_all_invalidate();
	
	// peripheral
	//xthal_dcache_all_invalidate();
	//dc_attr = xthal_get_dcacheattr();
	//ic_attr = xthal_get_icacheattr();
	sys_cache_disable(DMAC_BASE, (RSTGEN_BASE - DMAC_BASE + 0x1000));
	sys_cache_disable(SRAM0_BASE, 0x200000);
	//sys_cache_disable(0x60200000, 0x800000);
	//sys_cache_disable(0x40000000, 0x10000000);

	//sys_cache_disable(0x60200000, 0x0FE00000);
	//sys_cache_enable(0x60000000, 0x00200000);
	sys_cache_disable(0x40020000, 0x10000);
	sys_cache_disable(0x40030000, 0x8000);
	sys_cache_disable(0x40010000, 0x10000);
	sys_cache_disable(0x40008000, 0x8000);
}

void sys_dcache_invalidate(void)
{
	//xthal_dcache_hugerange_invalidate(SRAM1_BASE, SRAM1_SIZE);
	//xthal_dcache_hugerange_invalidate(SRAM2_BASE, SRAM2_SIZE);
	//xthal_dcache_hugerange_invalidate(SRAM3_BASE, SRAM3_SIZE);
}

/*************************************************************************************************************/
//  Function: void sys_sw_rst(E_SW_RST_PORT port)
//
//  Parameters:
// 		port	: sw reset port
//
//  Return:
//      	none
//
//  Description:
//		SW Reset the port.
//
/*************************************************************************************************************/
void sys_sw_rst(E_SW_RST_PORT port)
{	
	switch (port)
	{
		case SYS_A_RST:
				_ASSERT_RESET_sys_aresetn_116_
				_CLEAR_RESET_sys_aresetn_116_
			break;
		case SYS_H_RST:
				_ASSERT_RESET_sys_hresetn_116_
				_CLEAR_RESET_sys_hresetn_116_
			break;
		case SYS_P_RST:
				_ASSERT_RESET_sys_presetn_
				_CLEAR_RESET_sys_presetn_
			break;
		case SYS_AON_RST:
				_ASSERT_RESET_sys_aon_presetn_
				_CLEAR_RESET_sys_aon_presetn_
			break;
		case DSP0_B_RST:
				_ASSERT_RESET_dsp0_BReset_295_
				_CLEAR_RESET_dsp0_BReset_295_
			break;
		case DSP0_D_RST:
				_ASSERT_RESET_dsp0_Dreset_295_
				_CLEAR_RESET_dsp0_Dreset_295_
			break;			
		case DSP1_B_RST:
				_ASSERT_RESET_dsp1_BReset_296_
				_CLEAR_RESET_dsp1_BReset_296_
			break;
		case DSP1_D_RST:
				_ASSERT_RESET_dsp1_Dreset_296_
				_CLEAR_RESET_dsp1_Dreset_296_
			break;
		case DMAC_RST:
				_ENABLE_CLOCK_dmac_core_clock_115_
				_ENABLE_CLOCK_DMA_aclk_m1_	
				_ENABLE_CLOCK_DMA_hclk_115_
				
				// core 
				_ASSERT_RESET_dmac_core_resetn_115_
				_CLEAR_RESET_dmac_core_resetn_115_

				// ahb bus
				_ASSERT_RESET_dmac_hresetn_115_
				_CLEAR_RESET_dmac_hresetn_115_
			break;
		case GPIO_RST:
				_ASSERT_RESET_gpio_PRESETn_140_
				_CLEAR_RESET_gpio_PRESETn_140_
			break;
		case I2CS_RST:
				// ic
				_ASSERT_RESET_i2cs_ic_rst_n_131_
				_CLEAR_RESET_i2cs_ic_rst_n_131_

				// apb bus
				_ASSERT_RESET_i2cs_presetn_131_
				_CLEAR_RESET_i2cs_presetn_131_	
			break;
		case I2CM_RST:
				// ic
				_ASSERT_RESET_i2cm_ic_rst_n_132_
				_CLEAR_RESET_i2cm_ic_rst_n_132_	

				// apb bus
				_ASSERT_RESET_i2cm_presetn_132_
				_CLEAR_RESET_i2cm_presetn_132_
			break;			
		case SPI_RST:
				// bn
				_ASSERT_RESET_SPI_BnRES_130_
				_CLEAR_RESET_SPI_BnRES_130_

				// ns
				_ASSERT_RESET_SPI_nSSRST_130_
				_CLEAR_RESET_SPI_nSSRST_130_
			break;
		case TIMER_RST:
				_ASSERT_RESET_timer_PRESETn_141_
				_CLEAR_RESET_timer_PRESETn_141_	
			break;
		case UART0_RST:
				// apb bus
				_ASSERT_RESET_uart0_presetn_136_
				_CLEAR_RESET_uart0_presetn_136_

				// s
				_ASSERT_RESET_uart0_s_rst_n_136_
				_CLEAR_RESET_uart0_s_rst_n_136_
			break;
		case UART1_RST:			
				// apb bus
				_ASSERT_RESET_uart1_presetn_137_
				_CLEAR_RESET_uart1_presetn_137_	
				
				// s
				_ASSERT_RESET_uart1_s_rst_n_137_
				_CLEAR_RESET_uart1_s_rst_n_137_					
			break;
		case USB_RST:
				_ASSERT_RESET_USB_NRST_161_
				_CLEAR_RESET_USB_NRST_161_	
			break;
		case I2SADC_RST:
				_ENABLE_CLOCK_i2sadc_pclk_126_
				_ENABLE_CLOCK_i2sadc_bclk_icg_
				
				// abp bus
				_ASSERT_RESET_i2sadc_presetn_126_
				_CLEAR_RESET_i2sadc_presetn_126_	
				
				// s
				_ASSERT_RESET_i2sadc_sresetn_126_
				_CLEAR_RESET_i2sadc_sresetn_126_
			break;			
		case I2SDAC_RST:	
				_ENABLE_CLOCK_i2sdac_pclk_127_
				
				// abp bus
				_ASSERT_RESET_i2sdac_presetn_127_
				_CLEAR_RESET_i2sdac_presetn_127_	
				
				// s
				_ASSERT_RESET_i2sdac_sresetn_127_
				_CLEAR_RESET_i2sdac_sresetn_127_
			break;
		case I2S1_RST:
				_ENABLE_CLOCK_i2s1_pclk_128_
				
				// abp bus
				_ASSERT_RESET_i2s1_presetn_128_
				_CLEAR_RESET_i2s1_presetn_128_
				
				// s
				_ASSERT_RESET_i2s1_sresetn_128_
				_CLEAR_RESET_i2s1_sresetn_128_	
			break;
		case I2SVAD_RST:
				_ENABLE_CLOCK_i2svad_pclk_129_
				
				// abp bus
				_ASSERT_RESET_i2svad_presetn_129_
				_CLEAR_RESET_i2svad_presetn_129_	
				
				// s
				_ASSERT_RESET_i2svad_sresetn_129_
				_CLEAR_RESET_i2svad_sresetn_129_
			break;				
		case ACDCUCP_RST:
				_ASSERT_RESET_acdcucp_presetn_292_
				_CLEAR_RESET_acdcucp_presetn_292_	
			break;
		case WDT_RST:
				// bus
				_ASSERT_RESET_wdt_PRESETn_139_
				_CLEAR_RESET_wdt_PRESETn_139_	
				
				// core
				_ASSERT_RESET_wdt_WDOGRESn_139_
				_CLEAR_RESET_wdt_WDOGRESn_139_	
			break;			
		case NRST_RST:
				_ASSERT_RESET_NRST_347_
				_CLEAR_RESET_NRST_347_	
			break;
		case PTC_RST:
				_ASSERT_RESET_ptc_presetn_138_
				_CLEAR_RESET_ptc_presetn_138_	
			break;
		default:
			break;			
			
	}
}

/*************************************************************************************************************/
//  Function: void pad_tck_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_tck_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_TCK_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_TCK_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_TCK_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_TCK_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_TCK_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_TCK_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_TCK_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_TCK_func7
			break;			
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_TCK_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_tms_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_tms_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_TMS_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_TMS_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_TMS_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_TMS_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_TMS_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_TMS_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_TMS_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_TMS_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_TMS_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_tdo_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_tdo_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_TDO_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_TDO_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_TDO_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_TDO_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_TDO_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_TDO_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_TDO_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_TDO_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_TDO_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_tdi_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_tdi_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_TDI_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_TDI_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_TDI_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_TDI_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_TDI_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_TDI_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_TDI_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_TDI_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_TDI_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_trst_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_trst_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_TRST_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_TRST_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_TRST_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_TRST_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_TRST_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_TRST_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_TRST_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_TRST_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_TRST_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_spiclk_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_spiclk_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_SPI_CLK_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_SPI_CLK_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_SPI_CLK_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_SPI_CLK_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_SPI_CLK_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_SPI_CLK_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_SPI_CLK_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_SPI_CLK_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_SPI_CLK_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_spicsn_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_spicsn_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_SPI_CSN_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_SPI_CSN_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_SPI_CSN_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_SPI_CSN_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_SPI_CSN_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_SPI_CSN_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_SPI_CSN_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_SPI_CSN_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_SPI_CSN_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_spirxd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_spirxd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_SPI_RXD_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_SPI_RXD_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_SPI_RXD_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_SPI_RXD_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_SPI_RXD_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_SPI_RXD_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_SPI_RXD_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_SPI_RXD_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_SPI_RXD_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_spitxd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_spitxd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_SPI_TXD_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_SPI_TXD_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_SPI_TXD_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_SPI_TXD_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_SPI_TXD_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_SPI_TXD_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_SPI_TXD_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_SPI_TXD_func7
			break;				
		default:
				// not defined on current HW
			break;
	}
	
	_SET_PAD_PAD_SPI_TXD_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_uart0rxd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_uart0rxd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_UART0_RXD_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_UART0_RXD_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_UART0_RXD_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_UART0_RXD_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_UART0_RXD_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_UART0_RXD_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_UART0_RXD_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_UART0_RXD_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_UART0_RXD_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_uart0txd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_uart0txd_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_UART0_TXD_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_UART0_TXD_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_UART0_TXD_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_UART0_TXD_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_UART0_TXD_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_UART0_TXD_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_UART0_TXD_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_UART0_TXD_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_UART0_TXD_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_i2csscl_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_i2csscl_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_I2CSSCL_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_I2CSSCL_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_I2CSSCL_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_I2CSSCL_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_I2CSSCL_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_I2CSSCL_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_I2CSSCL_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_I2CSSCL_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_I2CSSCL_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_i2cssda_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_i2cssda_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_I2CSSDA_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_I2CSSDA_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_I2CSSDA_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_I2CSSDA_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_I2CSSDA_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_I2CSSDA_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_I2CSSDA_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_I2CSSDA_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_I2CSSDA_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_gpio0_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_gpio0_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_GPIO0_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_GPIO0_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_GPIO0_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_GPIO0_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_GPIO0_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_GPIO0_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_GPIO0_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_GPIO0_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_GPIO0_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_gpio1_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_gpio1_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_GPIO1_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_GPIO1_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_GPIO1_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_GPIO1_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_GPIO1_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_GPIO1_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_GPIO1_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_GPIO1_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_GPIO1_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_pwm0_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_pwm0_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_PWM0_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_PWM0_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_PWM0_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_PWM0_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_PWM0_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_PWM0_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_PWM0_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_PWM0_func7
			break;				
		default:
				// not defined on current HW
			break;
	}
}

/*************************************************************************************************************/
//  Function: void pad_pwm1_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_pwm1_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_PWM1_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_PWM1_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_PWM1_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_PWM1_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_PWM1_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_PWM1_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_PWM1_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_PWM1_func7
			break;				
		default:
				// not defined on current HW
			break;
	}
}

/*************************************************************************************************************/
//  Function: void pad_pwm2_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_pwm2_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_PWM2_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_PWM2_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_PWM2_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_PWM2_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_PWM2_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_PWM2_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_PWM2_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_PWM2_func7
			break;				
		default:
				// not defined on current HW
			break;
	}
}

/*************************************************************************************************************/
//  Function: void pad_dmic_clk11_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_dmic_clk11_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_DMIC_CLK11_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_DMIC_CLK11_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_DMIC_CLK11_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_DMIC_CLK11_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_DMIC_CLK11_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_DMIC_CLK11_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_DMIC_CLK11_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_DMIC_CLK11_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_DMIC_CLK11_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_dmic_clk12_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_dmic_clk12_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_DMIC_CLK12_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_DMIC_CLK12_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_DMIC_CLK12_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_DMIC_CLK12_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_DMIC_CLK12_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_DMIC_CLK12_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_DMIC_CLK12_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_DMIC_CLK12_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_DMIC_CLK12_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_dmic_in1_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_dmic_in1_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_DMIC_IN1_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_DMIC_IN1_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_DMIC_IN1_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_DMIC_IN1_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_DMIC_IN1_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_DMIC_IN1_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_DMIC_IN1_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_DMIC_IN1_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_DMIC_IN1_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_dmic_clk21_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_dmic_clk21_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_DMIC_CLK21_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_DMIC_CLK21_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_DMIC_CLK21_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_DMIC_CLK21_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_DMIC_CLK21_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_DMIC_CLK21_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_DMIC_CLK21_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_DMIC_CLK21_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_DMIC_CLK21_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_dmic_clk22_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_dmic_clk22_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_DMIC_CLK22_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_DMIC_CLK22_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_DMIC_CLK22_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_DMIC_CLK22_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_DMIC_CLK22_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_DMIC_CLK22_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_DMIC_CLK22_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_DMIC_CLK22_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_DMIC_CLK22_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_dmic_in2_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_dmic_in2_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_DMIC_IN2_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_DMIC_IN2_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_DMIC_IN2_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_DMIC_IN2_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_DMIC_IN2_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_DMIC_IN2_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_DMIC_IN2_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_DMIC_IN2_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_DMIC_IN2_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_i2smic_sbclk_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_i2smic_sbclk_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_I2SMIC_SBCLK_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_I2SMIC_SBCLK_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_I2SMIC_SBCLK_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_I2SMIC_SBCLK_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_I2SMIC_SBCLK_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_I2SMIC_SBCLK_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_I2SMIC_SBCLK_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_I2SMIC_SBCLK_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_I2SMIC_SBCLK_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_i2smic_slr0_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_i2smic_slr0_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_I2SMIC_SLRO_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_I2SMIC_SLRO_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_I2SMIC_SLRO_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_I2SMIC_SLRO_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_I2SMIC_SLRO_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_I2SMIC_SLRO_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_I2SMIC_SLRO_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_I2SMIC_SLRO_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_I2SMIC_SLRO_PORT_REN(ren)	
}

/*************************************************************************************************************/
//  Function: void pad_i2smic_ssdata_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_i2smic_ssdata_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_I2SMIC_SSDATA_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_I2SMIC_SSDATA_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_I2SMIC_SSDATA_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_I2SMIC_SSDATA_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_I2SMIC_SSDATA_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_I2SMIC_SSDATA_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_I2SMIC_SSDATA_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_I2SMIC_SSDATA_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_I2SMIC_SSDATA_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_dopwmlp_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_dopwmlp_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_DOPWMLP_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_DOPWMLP_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_DOPWMLP_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_DOPWMLP_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_DOPWMLP_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_DOPWMLP_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_DOPWMLP_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_DOPWMLP_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_DOPWMLP_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_dopwmln_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_dopwmln_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_DOPWMLN_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_DOPWMLN_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_DOPWMLN_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_DOPWMLN_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_DOPWMLN_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_DOPWMLN_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_DOPWMLN_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_DOPWMLN_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_DOPWMLN_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_dopwmrp_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_dopwmrp_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_DOPWMRP_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_DOPWMRP_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_DOPWMRP_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_DOPWMRP_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_DOPWMRP_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_DOPWMRP_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_DOPWMRP_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_DOPWMRP_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_DOPWMRP_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_dopwmrn_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_dopwmrn_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_DOPWMRN_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_DOPWMRN_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_DOPWMRN_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_DOPWMRN_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_DOPWMRN_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_DOPWMRN_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_DOPWMRN_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_DOPWMRN_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_DOPWMRN_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void pad_i2s2_mclk_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
//
//  Parameters:
// 		func	: pin function
//
//		ren	: pull down enable/disable
//
//  Return:
//      	none
//
//  Description:
//		set pin mux.
//
/*************************************************************************************************************/
void pad_i2s2_mclk_pinmux(E_PINMUX_FUNC func, E_PIN_REN ren)
{
	switch (func)
	{
		case PINMUX_FUNC0:
				_SET_PAD_PAD_I2S2_MCLK_func0
			break;
		case PINMUX_FUNC1:
				_SET_PAD_PAD_I2S2_MCLK_func1
			break;
		case PINMUX_FUNC2:
				_SET_PAD_PAD_I2S2_MCLK_func2
			break;
		case PINMUX_FUNC3:
				_SET_PAD_PAD_I2S2_MCLK_func3
			break;
		case PINMUX_FUNC4:
				_SET_PAD_PAD_I2S2_MCLK_func4
			break;
		case PINMUX_FUNC5:
				_SET_PAD_PAD_I2S2_MCLK_func5
			break;	
		case PINMUX_FUNC6:
				_SET_PAD_PAD_I2S2_MCLK_func6
			break;			
		case PINMUX_FUNC7:
				_SET_PAD_PAD_I2S2_MCLK_func7
			break;				
		default:
				// not defined on current HW
			break;
	}

	_SET_PAD_PAD_I2S2_MCLK_PORT_REN(ren)
}

/*************************************************************************************************************/
//  Function: void i2s_adc_clk_congig(uint8_t div, E_I2S_ADC_DATA_MODE mode)
//
//  Parameters:
// 		div		: divisor
//
//		mode	: i2s_adc data mode
//
//  Return:
//      	none
//
//  Description:
//		Set i2s_adc clock.
//		The i2s_adc must work as master mode, which clock is from the clk_gen only.
//
/*************************************************************************************************************/
void i2s_adc_clk_congig(uint8_t div, E_I2S_ADC_DATA_MODE mode)
{
	_ENABLE_CLOCK_i2sadc_pclk_126_

	if (mode == I2S_ADC_DATA_FROM_CODEC)
	{
		_SET_SYSCON_REG_cdc_control_adc_drvp_mntn_sel_288(1)			
	}
	else
	{
		_SET_SYSCON_REG_cdc_control_adc_drvp_mntn_sel_288(0)
	}

	_DIVIDE_CLOCK_i2sadc_lrck_internal_(div)
	_ENABLE_CLOCK_i2sadc_bclk_icg_

	sys_sw_rst(I2SADC_RST);

	sys_sw_rst(I2SVAD_RST);

	vad_sw_ctrl(VAD_SW_MODE_ADC);
}

/*************************************************************************************************************/
//  Function: void i2s1_master_clk_config(uint8_t div, uint8_t type, uint8_t sample_format)
//
//  Parameters:
// 		div				: divisor
//
//		type				: i2s type
//
//		sample_format	: i2s sample format
//  Return:
//      	none
//
//  Description:
//		Set i2s1 clock.
//		The i2s1 could work as master mode, which clock is from the clk_gen only.
//		The i2s1 could work as slave mode, which clock is from the pad.
//
/*************************************************************************************************************/
void i2s1_master_clk_config(uint8_t div, uint8_t type, uint8_t sample_format)
{
	uint32_t reg_val;
	uint8_t sdo_dly_cycle = 0;
	uint8_t lrck_dly_cycle = 0;
	uint8_t skip_head = 0;
	
	_ENABLE_CLOCK_i2s1_pclk_128_

#if 1
	 _GET_SYSCON_REG_i2s1_control_i2s_wrap_bypass_346(reg_val)
	info("i2s1 wrapper bypass_0: %x\n", reg_val);

	// standard mode, bypass left/right justified mode
	_SET_SYSCON_REG_i2s1_control_i2s_wrap_bypass_346(0)

	 _GET_SYSCON_REG_i2s1_control_i2s_wrap_bypass_346(reg_val)
	info("i2s1 wrapper bypass_0: %x\n", reg_val);

	switch (type)
	{
		case I2S_STANDARD:
				sdo_dly_cycle = 0;
				lrck_dly_cycle = 0;
				skip_head = 0;
			break;
		case I2S_LEFT_JUSTIFIED:
				if (div <= 64)
				{
					// fixed div to 64 to justify the left data  
					div = 64;	
				}
				
				sdo_dly_cycle = 0;
				lrck_dly_cycle = 1;
				skip_head = 0;
			break;
		case I2S_RITHT_JUSTIFIED:					
				if (div <= 64)
				{
					// fixed div to 64 to justify the right data  
					div = 64;	
				}

				if (sample_format < 32)
				{
					sdo_dly_cycle = 32 - sample_format - 1;
					lrck_dly_cycle = 0;	
				}
				else
				{
					sdo_dly_cycle = 0;
					lrck_dly_cycle = 1;			
				}

				skip_head = 0;
			break;
		case I2S_TDM_POL0:				
				sdo_dly_cycle = 0;
				lrck_dly_cycle = 0;
				skip_head = 3;
			break;
		case I2S_TDM_POL1:			
				sdo_dly_cycle = 1;
				lrck_dly_cycle = 0;
				skip_head = 3;
			break;				
		default:
				info("type not exit\n");
			break;
	}

	_DIVIDE_CLOCK_i2s1_in_lrck_346_(div)
	
	_SET_SYSCON_REG_i2s1_control_i2s1_mode_346(1)							// i2s1_mode
	_SET_SYSCON_REG_i2s1_control_audio_mode_346(type)						// audio_mode
	_SET_SYSCON_REG_i2s1_control_lr_ratio_346(0)							// lr_ratio
	_SET_SYSCON_REG_i2s1_control_slot_num_346(0)							// slot_num
	_SET_SYSCON_REG_i2s1_delay_control_sdo_dly_cycle_346(sdo_dly_cycle)		// sdo_dly_cycle
	_SET_SYSCON_REG_i2s1_delay_control_lrck_dly_cycle_346(lrck_dly_cycle)	// lrck_dly_cycle
	_SET_SYSCON_REG_i2s1_control_skip_head_346(skip_head)					// skip_head
	_SET_SYSCON_REG_i2s1_control_i2s1_enable_346(0)							// i2s1_enable
#else
	// standard mode, bypass left/right justified mode
	_SET_SYSCON_REG_i2s1_control_i2s_wrap_bypass_346(1)
	_SET_SYSCON_REG_i2s1_control_i2s1_mode_346(1)

	_DIVIDE_CLOCK_i2s1_in_lrck_346_(div)
	_SET_SYSCON_REG_i2s1_control_i2s1_enable_346(1)
#endif	
	
	sys_sw_rst(I2S1_RST);
}

void i2s1_slave_clk_config(uint8_t div, uint8_t type, uint8_t sample_format)
{
	uint32_t reg_val;
	uint8_t lr_ratio = 0;
	uint8_t slot_num = 0;
	uint8_t sdo_dly_cycle = 0;
	
	_ENABLE_CLOCK_i2s1_pclk_128_

#if 1
	 _GET_SYSCON_REG_i2s1_control_i2s_wrap_bypass_346(reg_val)
	info("i2s1 wrapper bypass_0: %x\n", reg_val);

	// standard mode, bypass left/right justified mode
	_SET_SYSCON_REG_i2s1_control_i2s_wrap_bypass_346(0)

	 _GET_SYSCON_REG_i2s1_control_i2s_wrap_bypass_346(reg_val)
	info("i2s1 wrapper bypass_0: %x\n", reg_val);

	switch (type)
	{
		case I2S_STANDARD:
				lr_ratio = 0;
				slot_num = 0;
				sdo_dly_cycle = 0;
			break;
		case I2S_LEFT_JUSTIFIED:				
				lr_ratio = 0;
				slot_num = 0;
				sdo_dly_cycle = (32 * 2) - 1;
			break;
		case I2S_RITHT_JUSTIFIED:					
				lr_ratio = 0;
				slot_num = 0;

				if (sample_format == 32)
				{
					sdo_dly_cycle = 63;	
				}
				else if (sample_format == 24)
				{
					sdo_dly_cycle = 63 + 8;
				}
				else
				{
					sdo_dly_cycle = 63 + 8 + 8;
				}				
			break;
		case I2S_TDM_POL0:				
				lr_ratio = sample_format;
				slot_num = 0x8;
				sdo_dly_cycle = (lr_ratio * slot_num) - 2;
				//sdo_dly_cycle = 1;
			break;
		case I2S_TDM_POL1:			
				lr_ratio = sample_format;
				slot_num = 0x8;
				sdo_dly_cycle = (lr_ratio * slot_num) - 2;
			break;				
		default:
				info("type not exit\n");
			break;
	}
	
	_SET_SYSCON_REG_i2s1_control_i2s1_mode_346(0)							// i2s1_mode
	_SET_SYSCON_REG_i2s1_control_audio_mode_346(type)						// audio_mode
	_SET_SYSCON_REG_i2s1_control_lr_ratio_346(lr_ratio)						// lr_ratio
	_SET_SYSCON_REG_i2s1_control_slot_num_346(slot_num)						// slot_num
	_SET_SYSCON_REG_i2s1_delay_control_sdo_dly_cycle_346(sdo_dly_cycle) 	// sdo_dly_cycle
	_SET_SYSCON_REG_i2s1_delay_control_lrck_dly_cycle_346(0)				// lrck_dly_cycle
	_SET_SYSCON_REG_i2s1_control_skip_head_346(0)							// skip_head
	_SET_SYSCON_REG_i2s1_control_i2s1_enable_346(0) 						// i2s1_enable
	
#else
	// standard mode, bypass left/right justified mode
	_SET_SYSCON_REG_i2s1_control_i2s_wrap_bypass_346(1)

	_SET_SYSCON_REG_i2s1_control_i2s1_mode_346(0)
	_SET_SYSCON_REG_i2s1_control_i2s1_enable_346(0)
#endif	
	
	sys_sw_rst(I2S1_RST);
}

/*************************************************************************************************************/
//  Function: void i2s_dac_clk_config(uint8_t div, E_I2S_MS_MODE mode, E_I2S_DAC_DATA_MODE dac_mode)
//
//  Parameters:
// 		div		: divisor
//
//		mode	: i2s master/slave mode
//
//  Return:
//      	none
//
//  Description:
//		Set i2s_dac clock.
//		The i2s_dac could work as master mode, which clock is from the clk_gen only.
//		The i2s_dac could work as slave mode, which clock is from the pad.
//
/*************************************************************************************************************/
void i2s_dac_clk_config(uint8_t div, E_I2S_MS_MODE mode, E_I2S_DAC_DATA_MODE dac_mode)
{
	_ENABLE_CLOCK_i2sdac_pclk_127_
	
	if (mode == I2S_MASTER)
	{
		_SET_SYSCON_REG_i2s0_control_i2s0_mode_345(1)
		_SET_SYSCON_REG_cdc_control_acdc_dac_clk_sel_288(0)
		
		//_DIVIDE_CLOCK_i2s0_in_bclk_(div)
		_DIVIDE_CLOCK_i2s0_in_lrck_345_(div)
		_SET_SYSCON_REG_i2s0_control_i2s0_enable_345(1)
	}
	else
	{
		_SET_SYSCON_REG_i2s0_control_i2s0_enable_345(0)
		_SET_SYSCON_REG_i2s0_control_i2s0_mode_345(0)
		//_SWITCH_CLOCK_i2s0_ms_bclk_SOURCE_i2s0_pad_bclk
		//_SWITCH_CLOCK_i2s0_ms_lrck_SOURCE_i2s0_pad_lrck

		_SET_SYSCON_REG_cdc_control_acdc_dac_clk_sel_288(1)
	}

	if (dac_mode == I2S_DAC_DATA_TO_CODEC)
	{	
		_SET_SYSCON_REG_cdc_control_acdc_dac_data_sel_288(1)
	}
	else
	{
		_SET_SYSCON_REG_cdc_control_acdc_dac_data_sel_288(0)
	}
	
	sys_sw_rst(I2SDAC_RST);
}

void usb_clk_config(void)
{
	int lock = 0;

	_SET_SYSCON_REG_pll1_config_PDRST_218(0);
	while(1) {
		_GET_SYSCON_REG_pll1_config_LKDT_218(lock);
		if (lock==1) {
			break;
		}
	}
	//debug("pll locked\n");

	_DIVIDE_CLOCK_dsp_clk_(2);
	_SWITCH_CLOCK_dsp_clk_SOURCE_pll1_out_div3;
	//debug("switch dsp clk\n");

	_ENABLE_CLOCK_USB_HCLK_161_;
	_ENABLE_CLOCK_USB_FCLK_161_;
	_ENABLE_CLOCK_usb_ram_clk_232_;

	//_ENABLE_CLOCK_USB_CLK_
	_SET_SYSCON_REG_usb_config_VBUSVAL_161(1);
	_SET_SYSCON_REG_usb_config_VBUSSES_161(1);
	_SET_SYSCON_REG_usb_config_VBUSLO_161(1);
	_SET_SYSCON_REG_usb_config_CID_161(1);
	_SET_SYSCON_REG_usb_config_TM1_161(1);

	_CLEAR_RESET_USB_NRST_161_;
	//_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);

	mdelay(300);
	
	sys_sw_rst(USB_RST);
}

uint32_t boot_strap(void)
{
	return REG32(boot_strap_REG_ADDR);
}


void sys_dsp0_config(void)
{
	uint32_t reg_val;

	_GET_SYSCON_REG_dsp0config_PRID_295(reg_val)
	info("prid_0: %x\n", reg_val);
	
	_GET_SYSCON_REG_dsp0config_dsp0_en_344(reg_val)
	info("dsp0_0 : %x\n", reg_val);	
	_SET_SYSCON_REG_dsp0config_dsp0_en_344(1)
	_GET_SYSCON_REG_dsp0config_dsp0_en_344(reg_val)
	info("dsp0 set : %x\n", reg_val);
}

void sys_dummy_loop(void)
{
	while (1)
	{}
}

void sys_dsp1_config(void)
{
	uint32_t reg_val;
	
#if CONFIG_DSP1_RESET
	reg_val = (uint32_t)sys_dummy_loop;
	info("sys_dummy_loop: 0x%x\n", reg_val);
#if 0
	// dsp1 halt on reset
	_SET_SYSCON_REG_dsp1config_OCDHaltOnReset_296(1)
#else
	// set invalid address to make the dsp1 go to execption when reset
	_SET_SYSCON_REG_dsp1resetvec_AltResetVec_296(reg_val)
	_SET_SYSCON_REG_dsp1config_StatVectorSel_296(1)
#endif

	sys_sw_rst(DSP1_D_RST);
	info("DSP1_D_RST\n");

	sys_sw_rst(DSP1_B_RST);	
	info("DSP1_B_RST\n");

#if CONFIG_DSP1_RESET	
	_GET_SYSCON_REG_dsp1config_PRID_296(reg_val)
	info("prid_1: %x\n", reg_val);	
		
	_GET_SYSCON_REG_dsp1config_dsp1_en_344(reg_val)
	info("dsp1_1 : %x\n", reg_val);	
	_SET_SYSCON_REG_dsp1config_dsp1_en_344(1)
	_GET_SYSCON_REG_dsp1config_dsp1_en_344(reg_val)
	info("dsp1 set : %x\n", reg_val);
#endif	
#endif
}

// pin range [0, 15]
void sys_external_pad_output(uint8_t pin, uint8_t v)
{
	uint32_t reg_val;
	
	_GET_SYSCON_REG_debug_register_debug_field(reg_val);

	//info("reg_val_0: 0x%x\n", reg_val);

	reg_val &= ~(1<<pin);
	reg_val |= 0xFFFF0000 | (v<<pin);
	_SET_SYSCON_REG_debug_register_debug_field(reg_val);

	//info("reg_val_1: 0x%x\n", reg_val);
}


