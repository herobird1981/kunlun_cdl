/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                               											 		 */
/*           Copyright (C) 2016 Brite Semiconductor Co., Ltd. All rights reserved.                									 */
/*                                                                                                													 */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_ptc.h"

#if (CONFIG_PTC_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry ptc_test_menu[];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/


/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
static int cmd_help(int argc, char *argv[])
{
	return cmdline_help_general(&ptc_test_menu[0]);
}

/*****************************************************************************/
//	Function:  static int ptc_test_pwm(int argc, char *argv[])
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
//		 pwm mode test
//
/*****************************************************************************/
static int ptc_test_pwm(int argc, char *argv[])
{
	E_PTC_PORT port;
	uint32_t hrc, lrc;

	info("%s: pwm test mode\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 4) 
	{
		info("format: %s port hrc lrc\n", argv[0]);
		return ERR_TEST_ARGC;
	}	

	// Get and check the parameter of len
	port = get_arg_ulong(argv[1]); 
	if (port >= NUM_PTC_PORTS)
	{
		info("port should be less than 3\n");
		return ERR_TEST_PARA;
	}
	
	hrc = get_arg_ulong(argv[2]);
	lrc = get_arg_ulong(argv[3]);

	// check the validity of the parameter
	if (hrc >= lrc)
	{
		ptc_stop(port);
		info("the hrc should be less than lrc\n");
		return ERR_TEST_ARGC;
	}

	ptc_stop(port);

	ptc_pwm_init(port, hrc, lrc);

	ptc_start(port);

	info("%s: set pwm high(%x), low(%x)\n", argv[0], hrc, lrc);
	info("%s: view the oscilloscope or multimeter result for pass or fail\n", argv[0]);	

	return ERR_TEST_DONE;
}


int ptc_test_prepare(int port)
{
	pad_pwm0_pinmux(PINMUX_FUNC0, 0);
	pad_pwm1_pinmux(PINMUX_FUNC0, 0);
	pad_pwm2_pinmux(PINMUX_FUNC0, 0);

	ptc_init(PTC_BASE, PTC_PCLK);
	
	return ERR_TEST_DONE;
}

int ptc_test_cleanup(int port)
{
	ptc_stop(PTC_0);
	ptc_stop(PTC_1);
	ptc_stop(PTC_2);
	
	return ERR_TEST_DONE;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry ptc_test_menu[] = {
	{"help", 			cmd_help, 		" : Display list of commands"},
	{"h",				cmd_help, 		"    : alias for help"},
	{"?",				cmd_help, 		"    : alias for help"},
	{"ptc_test_pwm",	ptc_test_pwm,	"   : ptc pwm mode test"},
	{"q", 				NULL, 			" : quit uart test"},
	{0, 				0, 				0}
};

#endif


