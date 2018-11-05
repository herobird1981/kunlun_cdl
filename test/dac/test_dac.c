/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_i2s.h"
#include "cdl_dac.h"
#if (CONFIG_DAC_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry dac_test_menu[];

static uint8_t g_dac_port = 0;

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
static int dac_cmd_help(int argc, char *argv[])
{	
	return cmdline_help_general(&dac_test_menu[0]);
}

/*****************************************************************************/
//	Function:  static int i2s_test_init(int argc, char *argv[])
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
//		 i2s test initialize.
//
/*****************************************************************************/
static int dac_cfg(int argc, char *argv[]) {
	int port = g_dac_port;
	int as_master = 1;
	int master_lrclk_div;
	int bus_type = WM8766_IF_FMT_I2S;
	if (argc < 5)
	{
		debug("help: %s role master_lrclk_div bus_type sample_format\n", argv[0]);
		debug("\trole: m for master, s for salve\n");
		debug("\tmaster lrclk div: 128 / 192 / 256 / 384 / 512 / 768\n");
		debug("\tbus_type: i2s | left_jus | right_jus\n");
		debug("\tsample_format: 16 | 24 | 32\n");
		return -1;
	}
	if (port != 0 && port != 1)
		return -1;

	if (argv[1][0] == 'm')
		as_master = 1;
	else if (argv[1][0] == 's')
		as_master = 0;
	else
		return -1;

	master_lrclk_div = get_arg_ulong(argv[2]);

	if (!strcmp(argv[3], "i2s"))
		bus_type = WM8766_IF_FMT_I2S;
	else if (!strcmp(argv[3], "left_jus"))
		bus_type = WM8766_IF_FMT_LEFTJ;
	else if (!strcmp(argv[3], "right_jus"))
		bus_type = WM8766_IF_FMT_RIGHTJ;
	else
		return -1;

	int sample_format = get_arg_ulong(argv[4]);
	if (sample_format == 16)
		sample_format = WM8766_IF_IWL_16BIT;
	else if (sample_format == 24)
		sample_format = WM8766_IF_IWL_24BIT;
	else if (sample_format == 32)
		sample_format = WM8766_IF_IWL_24BIT;
	else
		return -1;

	struct snd_wm8766 *wm = &g_wm[port];
	
	snd_wm8766_init(wm);
	snd_wm8766_set_if(wm, sample_format | bus_type);
	snd_wm8766_set_role(wm, as_master);
	if(snd_wm8766_master_lrclk_div(wm, master_lrclk_div))
		return -1;
    snd_wm8766_set_volume(wm, WM8766_CH1, 80, 80);
    snd_wm8766_playback_switch(wm, WM8766_CH1, 1);

    return 0;
}

int dac_test_prepare(int port)
{
	struct snd_wm8766 *priv = &g_wm[port];

	// check the validity of parameter
	if (port >= 2)
	{
		debug("port %d is too large!\n", port);
		return ERR_TEST_PARA;
	}

	// clear the structure
	mem_set8(priv, 0, sizeof(struct snd_wm8766));
	if (port == 0)
		snd_wm8766_soft_spi_init(priv, 2, 3, 4, 5);

	if (port == 1)
		snd_wm8766_soft_spi_init(priv, 7, 8, 9, 10);

	g_dac_port = port;

	return ERR_TEST_DONE;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry dac_test_menu[] = {
	{"help",                dac_cmd_help,           " : Display list of commands"},
	{"h",                   dac_cmd_help,           "    : alias for help"},
	{"?",                   dac_cmd_help,           "    : alias for help"},
	{"dac_cfg",             dac_cfg,  "       : wm8766 dac configure"},
	{0,                     0,                  0}
};

#endif

