/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_wdog.h"
#include "cdl_pll.h"

#if (CONFIG_WDOG_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry wdog_test_menu[];

volatile uint32_t wdt_nmi_counter = 0;

volatile uint32_t wdt_int_counter = 0;

static uint32_t g_max_load_val = ((0xFFFFFFFF/WDT_PCLK)*1000)*2;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//	Function:  static int cmd_help(int argc, char *argv[])
//
//	Parameters:
//		argc	: argument count
//	
//		argv	: argument
//
//	Return:
//		0	: success
//
//	Description:
//		 Help command line.
//
/*****************************************************************************/
static int cmd_help(int argc, char *argv[])
{
	return cmdline_help_general(&wdog_test_menu[0]);
}

/*****************************************************************************/
//	Function:  static int wdog_chk_counter(uint32_t mode)
//
//	Parameters:
//		mode 	: counter mode
//					= 0, decrease counter
//					= 1, fixed counter
//
//	Return:
//		0	: success
//		>0	: fail
//
//	Description:
//		 Check the counter
//
/*****************************************************************************/
static int wdog_chk_counter(uint32_t mode)
{
	int32_t fail = 0;	
	uint32_t i, hi32, low32;
	uint32_t wdog_cnt[2];
	u64 timer_cnt[2];

	// get the current counter of timer and wdog
	timer_cnt[0] = get_timer(0);
	wdog_cnt[0] = wdog_get_value();
	
	for (i = 0; i < 10; i++) 
	{
		udelay(1000);

		// get the current counter of timer and wdog
		wdog_cnt[1] = wdog_get_value();
		timer_cnt[1] = get_timer(timer_cnt[0]);
		
		hi32 = (timer_cnt[1] >> 32) & 0xFFFFFFFF;
		low32 = (timer_cnt[1] & 0xFFFFFFFF);
		if (hi32)	info("time: 0x%x_%x ms, wdog counter: 0x%x\n", hi32, low32, wdog_cnt[1]);
		else		info("time: %d ms, wdog counter: 0x%x\n", low32, wdog_cnt[1]);

		if (mode == 0) 
		{
			// expect wdt counter should decrease
			if (wdog_cnt[1] >= wdog_cnt[0]) 
			{
				fail++;
			}
		}
		else
		{
			// expect wdt counter should keep
			if (wdog_cnt[1] != wdog_cnt[0]) 
			{
				fail++;
			}
		}
		
		wdog_cnt[1] = wdog_cnt[0];
	}

	if (fail) 
	{
		if (mode == 0) 
		{
			info("%s fail for counter not decrease\n", __func__);
		} 
		else
		{
		  	info("%s fail for counter still change\n", __func__);
		}
	}
	
	return fail;
}

/*****************************************************************************/
//	Function:  static int wdog_start_stop(int argc, char *argv[])
//
//	Parameters:
//		argc	: argument count
//	
//		argv	: argument
//
//	Return:
//		0	: success
//
//	Description:
//		 wdog start/stop test.
//
/*****************************************************************************/
static int wdog_start_stop(int argc, char *argv[])
{
	int fail = 0;

	info("%s: wdog start/stop test\n", argv[0]);
	wdog_init(WDT_BASE, WDT_PCLK, WDOG_TIME_OUT);

	// start wdog and check counter
	info("wdog started\n");	
	wdog_start();
	
	fail += wdog_chk_counter(0);

	// stop wdog and check counter
	info("wdt stoped\n");
	wdog_stop();
	
	fail += wdog_chk_counter(1);
	if (fail) 
	{
		info("%s: test fail\n", argv[0]);
	} 
	else 
	{
		info("%s: test pass\n", argv[0]);
	}
	
	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  static int wdog_reload(int argc, char *argv[])
//
//	Parameters:
//		argc	: argument count
//	
//		argv	: argument
//
//	Return:
//		0	: success
//
//	Description:
//		 wdog reload test.
//
/*****************************************************************************/
static int wdog_reload(int argc, char *argv[])
{
	int fail = 0;
	uint32_t load_val = 0;
	uint32_t i, hi32, low32;
	uint32_t wdog_cnt;
	u64 timer_cnt[2];

	info("%s: wdog reload test\n", argv[0]);
	
	timer_cnt[0] = get_timer(0);

	// check the validity of the argc
	if (argc < 3) 
	{
		info("wrong cmd_format: %s start|restart load_val(ms)\n", argv[0]);
		return ERR_TEST_ARGC;
	}

	// get the parameter
	load_val = get_arg_ulong(argv[2]);
	if (load_val > g_max_load_val)
	{
		info("load_val(%d) should be less than %d\n", load_val, g_max_load_val);	
		return ERR_TEST_PARA;
	}
	
	info("cmd: %s load_val 0x%x\n", argv[1], load_val);
	
	// wdog initialize
	wdog_init(WDT_BASE, WDT_PCLK, load_val);
	info("wdt load_val 0x%x\n", p_wdog->load_val);
	
	// start timer and check counter
	timer_cnt[1] = get_timer(timer_cnt[0]);
	hi32 = (timer_cnt[1] >> 32) & 0xFFFFFFFF;
	low32 = timer_cnt[1] & 0xFFFFFFFF;
	if (hi32)	info("time: 0x%x_%x ms, wdt started\n", hi32, low32);
	else		info("time: %d ms, wdt started\n", low32);

	wdog_start();

	if (!strcmp(argv[1], "start")) 
	{
		wdog_cnt = wdog_get_value();
		if (p_wdog->load_val - wdog_cnt > 0x1000) 
		{
			info("Fail: loal_val 0x%x, counter 0x%lx\n", p_wdog->load_val, wdog_cnt);
			fail++;
		}
		
		for (i = 0; i < 5; i++) 
		{
			mdelay(100);
			
			wdog_cnt = wdog_get_value();
			timer_cnt[1] = get_timer(timer_cnt[0]);
			hi32 = (timer_cnt[1] >> 32) & 0xFFFFFFFF;
			low32 = timer_cnt[1] & 0xFFFFFFFF;

			if (hi32)	info("time: 0x%x_%x ms, wdog counter 0x%x\n", hi32, low32, wdog_cnt);
			else		info("time: %d ms, wdog counter 0x%x\n", low32, wdog_cnt);
		}
	} 
	else if (!strcmp(argv[1], "restart")) 
	{
		wdog_cnt = wdog_get_value();
		timer_cnt[1] = get_timer(timer_cnt[0]);
		hi32 = (timer_cnt[1] >> 32) & 0xFFFFFFFF;
		low32 = timer_cnt[1] & 0xFFFFFFFF;
		if (hi32)	info("time 0x%x_%x ms, init counter 0x%lx\n", hi32, low32, wdog_cnt);
		else		info("time %d ms, init counter 0x%lx\n", low32, wdog_cnt);

		for (i = 0; i < 5; i++) 
		{
			mdelay(100);
			
			wdog_cnt = wdog_get_value();
			timer_cnt[1] = get_timer(timer_cnt[0]);
			hi32 = (timer_cnt[1] >> 32) & 0xFFFFFFFF;
			low32 = timer_cnt[1] & 0xFFFFFFFF;
			
			if (hi32)	info("time 0x%x_%x ms, counter after delay 0x%lx\n", hi32, low32, wdog_cnt);
			else		info("time %d ms, counter after delay 0x%lx\n", low32, wdog_cnt);
		}
		
		wdog_restart();
		
		wdog_cnt = wdog_get_value();
		if (p_wdog->load_val - wdog_cnt > 0x1000) 
		{
			info("Fail: loal_val %lx counter 0x%lx\n", p_wdog->load_val, wdog_cnt);
			fail++;
		}
		
		for (i = 0; i < 5; i++) 
		{
			mdelay(100);
			
			wdog_cnt = wdog_get_value();
			timer_cnt[1] = get_timer(timer_cnt[0]);
			hi32 = (timer_cnt[1] >> 32) & 0xFFFFFFFF;
			low32 = timer_cnt[1] & 0xFFFFFFFF;

			if (hi32)	info("time: 0x%x_%x ms, counter after reload: 0x%lx\n", hi32, low32, wdog_cnt);
			else		info("time: %ld ms, counter after reload: 0x%lx\n", low32, wdog_cnt);
		}
	} 
	else
	{
		info("invalid sub command %s\n", argv[1]);
		return ERR_TEST_PARA;
	}
	
	// stop timer and check counter
	info("wdt stoped\n");
	wdog_stop();

	if (fail) 
	{
		info("%s: test fail\n", argv[0]);
	} 
	else 
	{
		info("%s: test pass\n", argv[0]);
	}
	
	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  static int wdog_reset(int argc, char *argv[])
//
//	Parameters:
//		argc	: argument count
//	
//		argv	: argument
//
//	Return:
//		0	: success
//
//	Description:
//		 wdog reset.
//
/*****************************************************************************/
static int wdog_reset(int argc, char *argv[])
{
	uint32_t ms_elapsed = 0;
	uint32_t timeout;
	uint32_t hi32, low32;
	u64 timer_cnt[2];
	uint8_t cc = 0;

	info("%s: wdog reset test\n", argv[0]);

	// check the validity of the argc
	if (argc < 2)
	{
		info("wrong cmd_format: %s timeout(ms)\n", argv[0]);
		return ERR_TEST_ARGC;
	}

	// get the parameter
	timeout = get_arg_ulong(argv[1]);
	if (timeout > g_max_load_val)
	{
		info("timeout(%d) should be less than %d\n", timeout, g_max_load_val);	
		return ERR_TEST_PARA;
	}
	
	info("cmd: timeout(ms)=%ld s\n", timeout);

	// start timer and check counter
	info("wdt started\n");
	wdog_init(WDT_BASE, WDT_PCLK, timeout);
	wdog_start();

	// when watch dog reset, system restart, so this case can't execute to end
	info("reset will occur after %ld ms, cdl test will restart\n", timeout);
	timer_cnt[0] = get_timer(0);
	ms_elapsed = 1;

	do {
		timer_cnt[1] = get_timer(timer_cnt[0]);
		if (timer_cnt[1] > ms_elapsed) 
		{
			hi32 = ((timer_cnt[1] >> 32)) & 0xFFFFFFFF;
			low32 = timer_cnt[1] & 0xFFFFFFFF;
			if (hi32)	info("time elapse 0x%x_%x ms\n", hi32, low32);
			else		info("time elapse %d ms\n", low32);
			
			//printf("irq 0x%x\n", wdog_get_mis());
			
			ms_elapsed += 500;
			ms_elapsed = min(ms_elapsed, (timeout - 200));
		}

		// check the input character
		if (console_tstc()) 
		{
			cc = console_getc();
			if (cc == 'q') 
			{
				break;
			}
			else if (cc == 'r')
			{
				wdog_restart();
				timer_cnt[0] = get_timer(0);	
				ms_elapsed = 1;
			}
			else
			{}
		}
	} while (timer_cnt[1] < (timeout + 10000));

	wdog_stop();

	info("%s: test fail\n", argv[0]);
	
	return ERR_TEST_DONE;
}

#if 0
/*****************************************************************************/
//	Function:  static void wdog_int_handler(void *ptr)
//
//	Parameters:
//		ptr	[in]	: int handler parameter
//
//	Return:
//		none
//
//	Description:
//		 wdog interrupt handler.
//
/*****************************************************************************/
static void wdog_int_handler(void *ptr)
{
	uint32_t reg_val = wdog_get_mis();
	
	if (reg_val) 
	{
		wdt_int_counter++;
		wdog_clr_int();
		wdog_restart();
	}
	
	info("wdog_int_handler, status = 0x%x\n", reg_val);
}
#endif

/*****************************************************************************/
//	Function:  static int wdog_int(int argc, char *argv[])
//
//	Parameters:
//		argc	: argument count
//	
//		argv	: argument
//
//	Return:
//		0	: success
//
//	Description:
//		 wdog interrupt handle test.
//
/*****************************************************************************/
static int wdog_int(int argc, char* argv[])
{
	int fail = 0;
	int i = 0;
	
	info("%s: wdog interrupt handle test\n",argv[0]);
	_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);

	// 1 second to generate reset
	wdog_init(WDT_BASE, WDT_PCLK, 1000);

	// start timer and check counter
	info("wdog started\n");
	wdt_nmi_counter = 0;
	wdog_clr_int();
	wdog_set_ctrl(WDOG_INTEN_EN, WDOG_RES_DIS);

	// wait alarm int generated
	while (i++ < 20) 
	{
		if (wdt_nmi_counter == (volatile unsigned int)&wdt_nmi_counter) 
		{
			debug("wdt nmi test passed!\n");
			break;
		}
		
		mdelay(100);
	}
	
	if (wdt_nmi_counter != (volatile unsigned int)&wdt_nmi_counter)
		fail++;

	wdog_stop();

	if (fail) 
	{
		info("%s: test fail\n",argv[0]);
	} 
	else 
	{
		info("%s: test pass\n",argv[0]);
	}

	return ERR_TEST_DONE;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry wdog_test_menu[] = {
	{"help",					cmd_help,				" : Display list of commands"},
	{"h",						cmd_help,				"    : alias for help"},
	{"?",						cmd_help,				"    : alias for help"},
	{"wdog_start_stop", 		wdog_start_stop,		"  : start/stop test"},
	{"wdog_reload",				wdog_reload,			"  : wdog load value and restart test"},
	{"wdog_reset",				wdog_reset,				"  : wdog generate reset test"},
	{"wdog_int",				wdog_int,				"  : wdog interrupt handle test"},
	{"q",						NULL,					" : quit wdog test"},
	{0,							0,						0}
};

#endif

