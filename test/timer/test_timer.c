/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_timer.h"

#if (CONFIG_TIMER_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
typedef enum
{
	TIMER_DOWN 		= 0,
	TIMER_FIXED 	= 1
} E_TIMER_COUNT_MODE;

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry timer_test_menu[];

const int timer_int_vector[2] = {
	INT_TIMER, INT_TIMER
};

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
	return cmdline_help_general(&timer_test_menu[0]);
}

// timer test may change the setting of system timer, restore it after test
static void timer_restore_system(E_TIMER_PORT id)
{
	info("timer_restore_system:TMR%d\n", id);

	if (id == SYS_TIMER)
	{
		sys_timer_init(id, TIMER_BASE, TIMER_PCLK);
	}
}

/*****************************************************************************/
//	Function:  static int wdog_chk_counter(uint32_t mode)
//
//	Parameters:
//		mode 	: counter mode
//
//	Return:
//		0	: success
//		>0	: fail
//
//	Description:
//		 Check the counter
//
/*****************************************************************************/
static int timer_chk_counter(E_TIMER_PORT port, E_TIMER_COUNT_MODE mode)
{	
	int32_t i, fail = 0;
	uint32_t timer_cnt[2];

	timer_cnt[0] = timer_get_value(port);

	for (i = 0; i < 256; i++) 
	{ 
		timer_cnt[1] = timer_get_value(port);
		info("port: 0x%x, counter:0x%x\n", port, timer_cnt[1]);
		
		// request timer counter should decrease
		if (mode == TIMER_DOWN) 
		{
			if (timer_cnt[1] >= timer_cnt[0]) 
			{
				fail++;
			}
		}		
		else
		{
			// request timer counter should keep
			if (timer_cnt[1] != timer_cnt[0]) 
			{
				fail++;
			}
		}
		
		timer_cnt[1] = timer_cnt[0];
	}
	
	if (fail) 
	{
		if (mode == TIMER_DOWN) 
		{
			info("%s fail, counter not decrease\n", __func__);
		}
		else
		{
			info("%s fail, counter still change\n", __func__);
		}
	}
	
	return fail;
}

/*****************************************************************************/
//	Function:  static int timer_start_stop(int argc, char *argv[])
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
//		 timer start/stop test.
//
/*****************************************************************************/
static int timer_start_stop(int argc, char *argv[])
{
	int32_t fail = 0;
	E_TIMER_PORT port = TIMER0;

	info("%s: timer start/stop test\n", argv[0]);

	timer_init(port, TIMER_BASE, TIMER_PCLK);
	timer_set_load(port, 0xFFFFFFFF);
	timer_set_bgload(port, 0xFFFFFFFF);

	if (argc < 2) 
	{
		info("wrong cmd_format: %s port\n", argv[0]);
		return ERR_TEST_ARGC;
	}
	
	port = get_arg_ulong(argv[1]);
	if (port >= NUM_TIMER_PORTS)
	{
		info("timer port should from 0-1\n");
		return ERR_TEST_PARA;
	}

	// start timer and check counter
	timer_enable_ctrl(port, TIMER_EN);
	info("timer %d started,counter should decrease\n", port);
	fail += timer_chk_counter(port, TIMER_DOWN);
	
	// stop timer and check counter
	timer_enable_ctrl(port, TIMER_DIS);
	info("timer %d stoped,counter should keep fixed\n", port);
	fail += timer_chk_counter(port, TIMER_FIXED);
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
//	Function:  static int timer_start_stop(int argc, char *argv[])
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
//		 timer reload test.
//
/*****************************************************************************/
static int timer_reload(int argc, char *argv[])
{
	int32_t fail = 0;
	uint32_t counter;
	uint32_t load_val = 0x4000000;
	E_TIMER_PORT port;
	uint32_t i;

	info("%s: timer reload test\n", argv[0]);
	
	if (argc < 2) 
	{
		info("wrong cmd_format: %s port\n", argv[0]);
		return ERR_TEST_ARGC;
	}
	
	port = get_arg_ulong(argv[1]);
	if (port >= NUM_TIMER_PORTS)
	{
		info("timer port should be from 0-1\n");
		return ERR_TEST_PARA;
	}

	timer_init(port, TIMER_BASE, TIMER_PCLK);
	timer_set_load(port, load_val);

	// start timer and check counter
	info("timer(%d) reload, counter should near %x\n", port, load_val);
	timer_enable_ctrl(port, TIMER_EN);

	counter = timer_get_value(port);
	info("counter:%x\n", counter);
	
	// init counter should be near to load_val
	if (counter < (load_val - 0x1000)) 
	{
		fail++;
	}

	for (i = 0; i < 256; i++)
	{
		counter = timer_get_value(port);
		info("counter:%x\n", counter);
	}
	
	info("timer reload, counter should near %x\n", load_val);
	timer_set_load(port, load_val);
	counter = timer_get_value(port);

	info("counter:%x\n", counter);
	
	if (counter < (load_val - 0x1000)) 
	{
		fail++;
	}

	for (i = 0; i < 256; i++)
	{
		counter = timer_get_value(port);
		info("counter:%x\n", counter);
	}	

	// stop timer and check counter
	timer_enable_ctrl(port, TIMER_DIS);
	info("timer %d stoped\n", port);

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
//	Function:  static int timer_test_div(int argc, char *argv[])
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
//		 timer frequency division test
//
/*****************************************************************************/
static int timer_test_div(int argc, char* argv[])
{
	E_TIMER_PORT port;
	uint32_t div, divisor;
	uint32_t load_val;
	uint32_t overflow_counter = 0, second = 0;

	info("%s: timer clock division test, press 'q' to quit\n", argv[0]);

	if (argc < 3)
	{
		info("wrong cmd_format: %s port div\n", argv[0]);
		info("div: 0: divided by 1 \n");
		info("div: 1: divided by 16 \n");
		info("div: 2: divided by 256 \n");
		return 1;
	}
	
	port = get_arg_ulong(argv[1]);
	if (port >= NUM_TIMER_PORTS)
	{
		info("timer port should be from 0-1\n");
		return 1;
	}

	div = get_arg_ulong(argv[2]);
	if ((div != TIMER_DIV_1) 	&& 
		(div != TIMER_DIV_16)	&&
		(div != TIMER_DIV_256)) 
	{
		info("timer div is should from 0/1/2\n");
		return 1;
	}

	timer_init(TIMER0, TIMER_BASE, TIMER_PCLK);
	timer_set_ctrl(port, TIMER_WRAPPING_MODE, TIMER_SIZE_BITS32, div,
						 TIMER_INT_DIS, TIMER_FREE_MODE, TIMER_DIS);

	// set overflow period: 50 us for min div (0: 1/1)
	load_val = 0xffffffff - (TIMER_PCLK/20000) + 1;
	timer_set_load(port, load_val);
	
	info("load value=0x%x overflow period counter=0x%x\n", load_val, (0xffffffff - load_val));
	timer_enable_ctrl(port, TIMER_EN);
	
	info("timer %d started\n", port); 
	while (1) 
	{
		if (console_tstc()) 
		{
			if ('q' == console_getc()) 
			{
				break;
			}
		}
	
		if (timer_get_ris(port)) 
		{
			overflow_counter++;
			
			//if((overflow_counter % 10) == 0)
			//{
			//	info(".");
			//}
			
			timer_clr_int(port);
		}
		
		if ((overflow_counter * divisor) > 20000) 
		{
			second++;
			info("%d second elapsed\n", second);

			overflow_counter = 0;
		}
	}

	timer_enable_ctrl(port, TIMER_DIS);

	return ERR_TEST_DONE;
}

//************************************************************************************************************
// timer interrupt handler
//************************************************************************************************************
volatile static int timer_int_counter = 0;
static int g_timer_port;
void timer_int_handler(void *ptr)
{
	uint32_t reg_val;
	struct brite_timer *priv = ptr;	
	E_TIMER_PORT port = priv->port;

	reg_val = timer_get_mis(port);
	info("timer%d: timer_int_handler, base=0x%x int_status: 0x%x\n", port, priv->base, reg_val);	
	if (reg_val) 
	{
		timer_int_counter++;
		timer_clr_int(port);
	}

}

/*****************************************************************************/
//	Function:  static int timer_test_div(int argc, char *argv[])
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
//		 timer interrupt test
//
/*****************************************************************************/
static int timer_test_int(int argc, char *argv[])
{
	int32_t fail = 0;
	uint32_t load_val;
	E_TIMER_PORT port;
	uint32_t i;
	struct brite_timer *priv = NULL;	

	info("timer_int: interrupt handle test\n");

	if (argc < 3) 
	{
		info("wrong cmd_format: %s id trig_value\n", argv[0]);
		debug("id: 0-1;\n");
		debug("trig_value: second unit\n");
		return 1;
	}
	
	port = get_arg_ulong(argv[1]);
	if (port>= NUM_TIMER_PORTS)
	{
		info("timer id should from 0-1\n");
		return 1;
	}

	g_timer_port = port;
	priv = &g_timer[port];

	load_val = get_arg_ulong(argv[2]);
	if (load_val > (0xffffffff/TIMER_PCLK) || (load_val == 0)) 
	{
		info("trigger value 1~max %d second can be meet\n", (0xffffffff/TIMER_PCLK));
		return 1;
	}

	timer_int_counter = 0;
	timer_init(port, TIMER_BASE, TIMER_PCLK);
	timer_clr_int(port);
	timer_set_ctrl(port, TIMER_WRAPPING_MODE, TIMER_SIZE_BITS32, TIMER_DIV_1,
						 TIMER_INT_EN, TIMER_FREE_MODE, TIMER_DIS);

	info("TIMER%d: base=0x%x int_vector=%d trigger val %d second\n", port, priv->base, timer_int_vector[port], load_val);
	// register int handler and enable it
	irq_generic_install_handler(timer_int_vector[port], timer_int_handler, priv);
	irq_generic_config_int(timer_int_vector[port], IRQ_TYPE_LEVEL_HIGH);
	irq_generic_enable_int(timer_int_vector[port]);
	_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);

	load_val *= TIMER_PCLK;
	timer_set_load(port, load_val);
	timer_start(port);

	for (i = 0; i < 0xfffffff; i++)
	{
		if (timer_int_counter != 0)
		{
			break;
		}	
	}
	
	if (i >= 0xfffffff)
	{
		fail++;
	}
	
	timer_stop(port);

	if (fail) 
	{
		info("%s: test fail\n", argv[0]);
	} 
	else 
	{
		info("%s: test pass\n", argv[0]);
	}

	// disable interrupt
	irq_generic_disable_int(timer_int_vector[port]);
	irq_generic_remove_handler(timer_int_vector[port], NULL, NULL);

	return ERR_TEST_DONE;
}

int timer_test_prepare(int sel)
{
	timer_init(TIMER0, TIMER_BASE, TIMER_PCLK);
	
	return ERR_TEST_DONE;
}

int timer_test_cleanup(int sel)
{
	E_TIMER_PORT port = sel;

	info("%s\n", __func__);

	// stop all timer
	for (port = TIMER0; port < NUM_TIMER_PORTS; port++)
	{
		timer_enable_ctrl(port, TIMER_DIS);
	}

	// timer test may change the setting of system timer, restore it after test
	timer_restore_system(SYS_TIMER);

	return ERR_TEST_DONE;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry timer_test_menu[] = {
	{"help", 				cmd_help,  			" : Display list of commands"},
	{"h",					cmd_help,			"    : alias for help"},
	{"?",					cmd_help, 			"    : alias for help"},
	{"timer_start_stop",	timer_start_stop,	": timer start/stop test"},
	{"timer_reload",		timer_reload,		": timer reload test"},
	//{"timer_test_div",		timer_test_div,		": timer frequency division test"},
	{"timer_test_int",		timer_test_int,			": timer interrupt test"},
	//{"timer_reg_dump",		timer_reg_dump,		": dump timer register"},	
	{0, 					0, 					0}
};


#endif

