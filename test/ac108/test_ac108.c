/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                               											 		 */
/*           Copyright (C) 2016 Brite Semiconductor Co., Ltd. All rights reserved.                									 */
/*                                                                                                													 */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_ac108.h"

#if (CONFIG_AC108_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry ac108_test_menu[];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/


/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
static int cmd_help(int argc, char *argv[])
{
	return cmdline_help_general(&ac108_test_menu[0]);
}

/*****************************************************************************/
//	Function:  static int ac108_test_init(int argc, char *argv[])
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
//		 ac108 initialize test.	
//
/*****************************************************************************/
static int ac108_test_init(int argc, char *argv[])
{
	int ret_val;
	E_AC108_PORT port;
	
	info("%s: ac108 init test\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 2) 
	{
		info("format: %s port\n", argv[0]);
		info("port : 0, ac108_0; 1, ac108_1\n");
		
		return ERR_TEST_ARGC;
	}
	
	// Get the parameters
	port = get_arg_ulong(argv[1]) & 0x1;

	info("%s , port: %d\n", argv[0], port);

	ret_val = ac108_hw_init(port);
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


int ac108_test_prepare(int port)
{
	ac108_init();
	
	return ERR_TEST_DONE;
}

int ac108_test_cleanup(int port)
{

	return ERR_TEST_DONE;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry ac108_test_menu[] = {
	{"help", 			cmd_help, 			" : Display list of commands"},
	{"h",				cmd_help, 			"    : alias for help"},
	{"?",				cmd_help, 			"    : alias for help"},
	{"ac108_test_init", ac108_test_init, 	" : ac108 init test"},
	{"q", 				NULL, 				" : quit test"},
	{0, 				0, 					0}
};

#endif


