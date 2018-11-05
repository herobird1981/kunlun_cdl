/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                               											 		 */
/*           Copyright (C) 2016 Brite Semiconductor Co., Ltd. All rights reserved.                									 */
/*                                                                                                													 */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_mem.h"

#if (CONFIG_MEM_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry mem_test_menu[];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/


/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
static int cmd_help(int argc, char *argv[])
{
	return cmdline_help_general(&mem_test_menu[0]);
}

int mem_test_prepare(int port)
{
	return ERR_TEST_DONE;
}

int mem_test_cleanup(int port)
{

	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  static int regs(int argc, char *argv[])
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
//		 Display contents of registers
//
/*****************************************************************************/
static int mem_rdreg(int argc, char *argv[])
{
	uint32_t addr;
	uint32_t len;
	
	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("format: %s addr len\n", argv[0]);
		info("len: byte aligned\n");
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters, check the validity of parameter 
	addr = get_arg_ulong(argv[1]);
	if (addr%4)
	{
		printf("invalid addr: 0x%x\n", addr);
		
		return ERR_TEST_PARA;
	}

	len = get_arg_ulong(argv[2]);

	print_in_line_reg32((uint8_t *)"mem_rdreg: ", (uint32_t *)addr, len);
	
	return ERR_TEST_DONE;	
}

/*****************************************************************************/
//	Function:  static int mem_wreg(int argc, char *argv[])
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
//		 Write register. 
//
/*****************************************************************************/
static int mem_wreg(int argc, char *argv[])
{
	uint32_t addr;
	uint32_t data;
	
	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("format: %s reg_addr data\n", argv[0]);
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters, check the validity of parameter 
	addr = get_arg_ulong(argv[1]);
	if (addr%4)
	{
		printf("invalid addr: 0x%x\n", addr);
		
		return ERR_TEST_PARA;
	}

	data = get_arg_ulong(argv[2]);

	writel(data, addr);
	
	return ERR_TEST_DONE;	
}


/*****************************************************************************/
//	Function:  static int mem_rd8(int argc, char *argv[])
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
//		Read  8-bit items. 
//
/*****************************************************************************/
static int mem_rd8(int argc, char *argv[])
{
	uint32_t addr;
	uint32_t len;
	
	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("format: %s addr len\n", argv[0]);
		info("len: byte aligned\n");
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters
	addr = get_arg_ulong(argv[1]);
	len = get_arg_ulong(argv[2]);

	print_in_line((uint8_t *)"mem_rd8: ", (uint8_t *)addr, len);
	
	return ERR_TEST_DONE;	
}

/*****************************************************************************/
//	Function:  static int mem_rd16(int argc, char *argv[])
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
//		Read 16-bit items. 
//
/*****************************************************************************/
static int mem_rd16(int argc, char *argv[])
{
	uint32_t addr;
	uint32_t len;
	
	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("format: %s addr len\n", argv[0]);
		info("len: byte aligned\n");
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters, check the validity of parameter 
	addr = get_arg_ulong(argv[1]);
	if (addr%2)
	{
		printf("invalid addr: 0x%x\n", addr);
		
		return ERR_TEST_PARA;
	}

	len = get_arg_ulong(argv[2]);

	print_in_line_u16((uint8_t *)"mem_rd16: ", (uint16_t *)addr, len);
	
	return ERR_TEST_DONE;	
}

/*****************************************************************************/
//	Function:  static int mem_rd32(int argc, char *argv[])
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
//		Read 32-bit items. 
//
/*****************************************************************************/
static int mem_rd32(int argc, char *argv[])
{
	uint32_t addr;
	uint32_t len;
	
	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("format: %s addr len\n", argv[0]);
		info("len: byte aligned\n");
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters, check the validity of parameter 
	addr = get_arg_ulong(argv[1]);
	if (addr%4)
	{
		printf("invalid addr: 0x%x\n", addr);
		
		return ERR_TEST_PARA;
	}

	len = get_arg_ulong(argv[2]);

	print_in_line_u32((uint8_t *)"mem_rd32: ", (uint32_t *)addr, len);
	
	return ERR_TEST_DONE;	
}

/*****************************************************************************/
//	Function:  static int mem_wr8(int argc, char *argv[])
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
//		Write  8-bit items. 
//
/*****************************************************************************/
static int mem_wr8(int argc, char *argv[])
{
	uint32_t addr;
	uint32_t data;
	
	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("format: %s addr data\n", argv[0]);
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters
	addr = get_arg_ulong(argv[1]);
	data = get_arg_ulong(argv[2]);

	*(uint8_t *)addr = data;
	
	return ERR_TEST_DONE;	
}

/*****************************************************************************/
//	Function:  static int mem_wr16(int argc, char *argv[])
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
//		Write  16-bit items. 
//
/*****************************************************************************/
static int mem_wr16(int argc, char *argv[])
{
	uint32_t addr;
	uint32_t data;
	
	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("format: %s addr data\n", argv[0]);
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters, check the validity of parameter 
	addr = get_arg_ulong(argv[1]);
	if (addr%2)
	{
		printf("invalid addr: 0x%x\n", addr);
		
		return ERR_TEST_PARA;
	}

	data = get_arg_ulong(argv[2]);

	*(uint16_t *)addr = data;
	
	return ERR_TEST_DONE;	
}

/*****************************************************************************/
//	Function:  static int mem_wr32(int argc, char *argv[])
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
//		Write  32-bit items. 
//
/*****************************************************************************/
static int mem_wr32(int argc, char *argv[])
{
	uint32_t addr;
	uint32_t data;
	
	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("format: %s addr data\n", argv[0]);
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters, check the validity of parameter 
	addr = get_arg_ulong(argv[1]);
	if (addr%4)
	{
		printf("invalid addr: 0x%x\n", addr);
		
		return ERR_TEST_PARA;
	}

	data = get_arg_ulong(argv[2]);

	*(uint32_t *)addr = data;
	
	return ERR_TEST_DONE;	
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry mem_test_menu[] = {
	{"help", 		cmd_help, 	" : Display list of commands"},
	{"h",			cmd_help, 	"    : alias for help"},
	{"?",			cmd_help, 	"    : alias for help"},
	{"q", 			NULL, 		" : quit uart test"},
	{"mem_rdreg", 	mem_rdreg, 	" : Display contents of registers"},
	{"mem_wreg", 	mem_wreg, 	" :  Write register"},
	{"mem_rd8", 	mem_rd8, 	" :  Read  8-bit items"},
	{"mem_rd16", 	mem_rd16, 	" :  Read  16-bit items"},
	{"mem_rd32", 	mem_rd32, 	" :  Read  32-bit items"},
	{"mem_wr8", 	mem_wr8, 	" :  Write  8-bit items"},
	{"mem_wr16", 	mem_wr16, 	" :  Write  16-bit items"},
	{"mem_wr32", 	mem_wr32, 	" :  Write  32-bit items"},
	{0, 			0, 			0}
};

#endif


