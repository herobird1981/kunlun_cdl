/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CMDLINE_H__
#define __CMDLINE_H__
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "cdl_config.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define CONFIG_SYS_PBSIZE	512
#define CONFIG_PANIC_HANG
//#define ARCH_DMA_MINALIGN	64

// command line process
#define CMDLINE_BAD_CMD			(-1)
#define CMDLINE_TOO_MANY_ARGS	(-2)
#define CMDLINE_TOO_FEW_ARGS	(-3)
#define CMDLINE_INVALID_ARG		(-4)
#define CMDLINE_QUIT			(-5)
#define CMDLINE_CARRIAGE_RET	(-6)

#define assert(x) \
	({ if (!(x) && DEBUG) \
		__assert_fail(#x, __FILE__, __LINE__, __func__); })

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
typedef int (*pfn_cmdline)(int argc, char* argv[]);

typedef struct
{
	const char* cmd;
	pfn_cmdline	pfn_cmd;
	const char* help;
} cmdline_entry;

/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern void __assert_fail(const char* assertion, const char* file, unsigned line, const char* function);

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
int cmdline_process(uint8_t *cmd, cmdline_entry *entry);

int cmdline_help_general(cmdline_entry *entry);

int cmdline_to_argv(char *cmd, int *argc, char** argv);

unsigned long get_arg_ulong(char* buf);

unsigned long simple_strtoul( const char* cp, char** endp, unsigned int base );

#endif

