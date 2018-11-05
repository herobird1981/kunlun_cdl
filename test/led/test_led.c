/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                               											 		 */
/*           Copyright (C) 2016 Brite Semiconductor Co., Ltd. All rights reserved.                									 */
/*                                                                                                													 */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_led.h"

#if (CONFIG_LED_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry led_test_menu[];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/


/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
static int cmd_help(int argc, char *argv[])
{
	return cmdline_help_general(&led_test_menu[0]);
}

/*****************************************************************************/
//	Function:  static int led_test_reset(int argc, char *argv[])
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
//		 led reset test.	
//
/*****************************************************************************/
static int led_test_reset(int argc, char *argv[])
{
	int ret_val;
	E_LED_RST_MODE rst_mode;
	
	info("%s: led reset test\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 2) 
	{
		info("format: %s rst_mode\n", argv[0]);
		info("rst_mode : 0, poweron; 1, shutdown\n");
		
		return ERR_TEST_ARGC;
	}
	
	// Get the parameters
	rst_mode = get_arg_ulong(argv[1]) & 0x1;

	info("%s , reset mode: %d\n", argv[0], rst_mode);

	ret_val = led_reset(rst_mode);
	if (ret_val) 
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
//	Function:  static int led_test_oe(int argc, char *argv[])
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
//		 led output enable test.	
//
/*****************************************************************************/
static int led_test_oe(int argc, char *argv[])
{
	int fail = 0;
	E_LED_OE led_oe;
	
	info("%s: led oe test\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 2) 
	{
		info("format: %s led_oe\n", argv[0]);
		info("led_oe : \n");
		info("    0x1  : LED_RGB_0_2_OE\n");
		info("    0x2  : LED_RGB_3_5_OE\n");
		info("    0x4  : LED_RGB_6_7_OE\n");
		info("    0x8  : LED_RGB_8_9_OE\n");
		info("    0x10 : LED_RGB_10_12_OE\n");
		info("    0x20 : LED_RGB_13_15_OE\n");
		info("    0x3F : LED_RGB_0_15_OE\n");
		
		return ERR_TEST_ARGC;
	}

	if (led_reset(LED_POWERON))
	{
		fail++;
		goto end;
	}
	 
	// Get the parameters
	led_oe = get_arg_ulong(argv[1]) & LED_RGB_OE_MASK;

	info("%s , oe_no: %d\n", argv[0], led_oe);

	if (led_rgb_oe(led_oe))
	{	
		fail++;
	}

end:	
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
//	Function:  static int led_test_brightness(int argc, char *argv[])
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
//		 led brightness test.	
//
/*****************************************************************************/
static int led_test_brightness(int argc, char *argv[])
{
	int fail = 0;
	E_LED_PORT port;
	uint8_t brightness;
	
	info("%s: led brightness test\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("format: %s port brightness\n", argv[0]);
		info("port       : [0, 15]\n");
		info("brightness : [0, 0x7F], 0, the min light, 0x7F, the max light\n");
		
		return ERR_TEST_ARGC;
	}
	
	// Get the parameters
	port = get_arg_ulong(argv[1]);
	brightness = get_arg_ulong(argv[2]);

	info("%s , port: %d, brightness: %d\n", argv[0], port, brightness);

	// set bfightness
	if (led_fix_brightness(port, brightness))
	{
		fail++;
		goto end;
	}

end:	
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


int led_test_prepare(int port)
{
	led_init();
	
	return ERR_TEST_DONE;
}

int led_test_cleanup(int port)
{

	return ERR_TEST_DONE;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry led_test_menu[] = {
	{"help", 				cmd_help, 				" : Display list of commands"},
	{"h",					cmd_help, 				"    : alias for help"},
	{"?",					cmd_help, 				"    : alias for help"},
	{"led_test_reset",		led_test_reset,			"  : led reset test"},
	{"led_test_oe",			led_test_oe,			"  : led output enable test"},
	{"led_test_brightness",	led_test_brightness,	"  : led brightness test"},
	{"q", 					NULL, 					" : quit uart test"},
	{0, 					0, 						0}
};

#endif


