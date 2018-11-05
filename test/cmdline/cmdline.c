/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cmdline.h"
#include "console.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#ifndef CMDLINE_MAX_ARGS
#define CMDLINE_MAX_ARGS				16
#endif

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
static char *cmdline_argv[CMDLINE_MAX_ARGS + 1];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
int cmdline_to_argv(char *cmd, int *argc, char** argv)
{
	char *p_cmd;
	int argc_num;
	int find_flag = 1;

	argc_num = 0;
	p_cmd = cmd;

	while (*p_cmd)
	{
		// If there is a space, then replace it with a zero, and set the flag
		// to search for the next argument.
		if (*p_cmd == ' ')
		{
			*p_cmd = 0;
			find_flag = 1;
		}

		// Otherwise it is not a space, so it must be a character that is part
		// of an argument.
		else
		{
			// If bFindArg is set, then that means we are looking for the start
			// of the next argument.
			if (find_flag)
			{
				// As long as the maximum number of arguments has not been
				// reached, then save the pointer to the start of this new arg
				// in the argv array, and increment the count of args, argc.
				if (argc_num < CMDLINE_MAX_ARGS)
				{
					cmdline_argv[argc_num] = p_cmd;
					argc_num++;
					find_flag = 0;
				}
				else
				{
					return CMDLINE_TOO_MANY_ARGS;
				}
			}
		}
		
		p_cmd++;
	}

	*argc =  argc_num;
	*argv = (char *)cmdline_argv;

	return 0;
}

int cmdline_process(uint8_t *cmd, cmdline_entry *entry) 
{
	static uint8_t *argv[CMDLINE_MAX_ARGS + 1];
	uint8_t *p_cmd;
	int argc;
	int find_flag = 1;

	argc = 0;
	p_cmd = cmd;

	while (*p_cmd)
	{
		//
		// If there is a space, then replace it with a zero, and set the flag
		// to search for the next argument.
		//
		if (*p_cmd == ' ')
		{
			*p_cmd = 0;
			find_flag = 1;
		}

		//
		// Otherwise it is not a space, so it must be a character that is part
		// of an argument.
		//
		else
		{
			//
			// If bFindArg is set, then that means we are looking for the start
			// of the next argument.
			//
			if (find_flag)
			{
				//
				// As long as the maximum number of arguments has not been
				// reached, then save the pointer to the start of this new arg
				// in the argv array, and increment the count of args, argc.
				//
				if (argc < CMDLINE_MAX_ARGS)
				{
					argv[argc] = p_cmd;
					argc++;
					find_flag = 0;
				}
				else
				{
					return CMDLINE_TOO_MANY_ARGS;
				}
			}
		}
		
		p_cmd++;
	}

	//
	// If one or more arguments was found, then process the command.
	//
	if (argc)
	{
		//check quit 'q' command
		if (!strcmp((char *)argv[0], "q"))
		{
			return CMDLINE_QUIT;
		}

		while (entry->cmd)
		{
			if (!strcmp((char *)argv[0], entry->cmd))
			{
				return (int)(entry->pfn_cmd(argc, argv));
			}
			
			entry++;
		}
	}
	else
	{
		return CMDLINE_CARRIAGE_RET;
	}

	return CMDLINE_BAD_CMD;
}

int cmdline_help_general(cmdline_entry *entry)
{
	console_printf( "\nAvailable commands\n" );
	console_printf( "------------------\n" );

	while (entry->cmd)
	{
		console_printf("%s%s\n", entry->cmd, entry->help);

		entry++;
	}
	
	return 0;
}

unsigned long simple_strtoul(const char* cp, char** endp, unsigned int base)
{
	unsigned long result = 0;
	unsigned long value;

	if ( *cp == '0' )
	{
		cp++;
		if ( ( *cp == 'x' ) && isxdigit( cp[1] ) )
		{
			base = 16;
			cp++;
		}

		if ( !base )
		{
			base = 8;
		}
	}

	if ( !base )
	{
		base = 10;
	}

	while ( isxdigit( *cp ) &&
		( value = isdigit( *cp ) ? *cp - '0' :
		( islower( *cp ) ? toupper( *cp ) : *cp ) - 'A' + 10 ) < base )
	{
		result = result * base + value;
		cp++;
	}

	if ( endp )
	{
		*endp = ( char* )cp;
	}

	return result;
}

unsigned long get_arg_ulong(char* buf)
{
	unsigned long ret = 0;
	int base = 10;
	char *ptr = buf;

	if (!strncmp(buf,"0x",2) || !strncmp(buf,"0X",2)) {
		base = 16;
		ptr = buf+2;
	}
	ret = simple_strtoul((const char*)ptr, 0, base);

	return ret;
}

