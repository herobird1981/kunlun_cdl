/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_config.h"
#include "test_gpio.h"
#include "test_wdog.h"
#include "test_i2c.h"
#include "test_timer.h"
#include "test_uart.h"
#include "test_spi.h"
#include "test_i2s.h"
#include "test_dma.h"
#include "test_usb.h"
#include "test_ptc.h"
#include "test_mem.h"
#include "cdl_sys.h"
#include "test_dac.h"
#include "test_led.h"
#include "test_ac108.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define CMD_BUF_SIZE		64

typedef struct test_func_menu_s {
	cmdline_entry* menu;
	int (*prepare)(int);
	int dev_sel;
	const char* desc;
	int (*cleanup)(int);
} test_func_menu;

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
static char g_cmd_buf[CMD_BUF_SIZE];
static char *p_cmd_buf_endp;

// device list for test function
const test_func_menu menu[] = {
	{uart_test_menu,	uart_test_prepare,	0,	"uart0",		NULL},
	{uart_test_menu,	uart_test_prepare,	1,	"uart1",		NULL},
	{timer_test_menu,	timer_test_prepare,	0,	"timer",		timer_test_cleanup},
	{wdog_test_menu,	NULL,				0,	"wdog",			NULL},
	{gpio_test_menu,	gpio_test_prepare,	0,	"gpio",			NULL},
	{spi_test_menu,		spi_test_prepare,	0,	"spi",			NULL},
	{i2c_test_menu,		i2c_test_prepare,	0,	"i2c_slave",	i2c_test_cleanup},
	{i2c_test_menu,		i2c_test_prepare,	1,	"i2c_master",	i2c_test_cleanup},
	{i2sadc_test_menu,  i2s_test_prepare,   0,  "i2s_adc",      NULL},
	{i2sdac_test_menu,  i2s_test_prepare,   1,  "i2s_dac",      NULL},
	{i2s1_test_menu,    i2s_test_prepare,   2,  "i2s1",         NULL},
	{i2svad_test_menu,	i2s_test_prepare,	3,	"i2s_vad",		NULL},
	{dac_test_menu,     dac_test_prepare,   0,  "codec_dac0",	NULL},
	{dac_test_menu,	    dac_test_prepare,	1,	"codec_dac1",	NULL},
	{dma_test_menu,		dma_test_prepare,	0,	"dma",			NULL},
	{usb_test_menu,		usb_test_prepare,	0,  "usb",			NULL},
	{ptc_test_menu,		ptc_test_prepare,	0,	"ptc",			ptc_test_cleanup},
	{mem_test_menu,		NULL,				0,	"mem",			NULL},
	{led_test_menu,		led_test_prepare,	0,	"led",			NULL},
	{ac108_test_menu,	ac108_test_prepare,	0,	"ac108",		NULL}
};

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/
extern void console_init(E_UART_PORT port, E_UART_BD bd, E_UART_DLS dls, E_UART_STOP stop, E_UART_EPS parity);

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
int one_device_test(int idx)
{
	int ret;

	// display the help, list all support test command for the device
	info("device: %s test case\n", menu[idx].desc);
	cmdline_process((uint8_t *)"help", menu[idx].menu);

	while (1) 
	{
		// Print a prompt to the console.  Show the CWD.
		info("\ncmd:> ");

		// Get a line of text from the user.
		console_gets((uint8_t *)g_cmd_buf, sizeof(g_cmd_buf));

		// Pass the line from the user to the command processor.
		// It will be parsed and valid commands executed.
		ret = cmdline_process((uint8_t *)g_cmd_buf, menu[idx].menu);

		// Handle the case of bad command.
		if (ret == CMDLINE_QUIT) 
		{
			info("Quit %s test\n", menu[idx].desc);
			break;
		} 
		else if (ret == CMDLINE_BAD_CMD) 
		{
			// Handle the case of too many arguments.
			info("Bad command!\n");
		} 
		else if (ret == CMDLINE_TOO_MANY_ARGS) 
		{
			info("Too many arguments for command processor!\n");
		}
		else if (ret == CMDLINE_CARRIAGE_RET) 
		{
			info("CMDLINE_CARRIAGE_RET\n");
		} 
		else if (ret != 0) 
		{
			// Otherwise the command was executed.	Print the error code if one was returned.
			info("Command returned error code %d\n", ret);
		}
	}

	if (menu[idx].cleanup != NULL)
	{
		(menu[idx].cleanup)(menu[idx].dev_sel);
	}
	
	return 0;
}

int main(void)
{
	int ret_val = 0;
	uint32_t i, j;	
	uint32_t usel = 0;

	// reset each module
	//sys_sw_rst(SYS_A_RST);
	//sys_sw_rst(SYS_H_RST);
	//sys_sw_rst(SYS_P_RST);
	//sys_sw_rst(SYS_AON_RST);
	//sys_sw_rst(DSP0_B_RST);
	//sys_sw_rst(DSP0_D_RST);
	sys_sw_rst(DMAC_RST);
	sys_sw_rst(GPIO_RST);
	sys_sw_rst(SPI_RST);
	sys_sw_rst(TIMER_RST);
	sys_sw_rst(USB_RST);
	sys_sw_rst(ACDCUCP_RST);
	sys_sw_rst(WDT_RST);
	//sys_sw_rst(NRST_RST);
	sys_sw_rst(PTC_RST);

	// init console first
	console_init(CONSOLE_UART_PORT, UART_BD_115200, UART_DLS_8_BITS, UART_STOP_1_BIT, UART_PARITY_NONE);

#if CONFIG_DSP0
	info("\nDSP0 CDL Ver: %s %s\n", __DATE__, __TIME__);
#else
	info("\nDSP1 CDL Ver: %s %s\n", __DATE__, __TIME__);
#endif

	wdog_init(WDT_BASE, WDT_PCLK, WDOG_TIME_OUT);
	wdog_stop();

	sys_timer_init(SYS_TIMER, TIMER_BASE, TIMER_PCLK);

	enable_interrupts();

	// init dmac
	dma_init(DMA0);

	vad_init();

	// init dsp0 and dsp1
	sys_dsp0_config();
	sys_dsp1_config();

	// main loop
	while (1) 
	{
		info("\nKunlun CDL test, please select device first\n");

		// display test device list
		for (i = 0; i < ARRAY_SIZE(menu); i++) 
		{
			if (menu[i].menu == 0)
			{
				info("%d: %s  %s\r\n", i, menu[i].desc, "!!! not implemented");
			}	
			else
			{
				info("%d: %s  %s\r\n", i, menu[i].desc, "");
			}	
		}

		// get user selection and run the test
		info("\r\nSelect the device to test : ");
		console_gets((uint8_t *)g_cmd_buf, sizeof(g_cmd_buf));
		
		usel = simple_strtoul((char *)g_cmd_buf, (char **)&p_cmd_buf_endp, 10);
		if (p_cmd_buf_endp == g_cmd_buf) 
		{
			usel = 0xFFFFFFFF;
			for (j = 0; j < ARRAY_SIZE(menu); j++) 
			{
				if (menu[j].menu && (!strcmp(menu[j].desc, (char *)g_cmd_buf))) 
				{
					usel = j;
					break;
				}
			}
		}
		
		if (usel < ARRAY_SIZE(menu)) 
		{
			if (menu[usel].menu == NULL) 
			{
				info("%s test not imlpemented now\n", menu[usel].desc);
			} 
			else 
			{
				if (menu[usel].prepare != NULL) 
				{
					ret_val = (menu[usel].prepare)(menu[usel].dev_sel);
				}	
				else
				{
					ret_val = 0;
				}
				
				if (!ret_val)  
				{
					one_device_test(usel);
				}
			}
		}
		
		info("\r\n");
	}

	disable_interrupts();
	
	while (1) {};
	
	return 0;
}

