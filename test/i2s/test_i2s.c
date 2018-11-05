/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_i2s.h"
#include "vad_data0.h"
#include "vad_data1.h"

#if (CONFIG_I2S_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
//cmdline_entry i2s_test_menu[];
cmdline_entry i2sadc_test_menu[];
cmdline_entry i2sdac_test_menu[];
cmdline_entry i2s1_test_menu[];
cmdline_entry i2svad_test_menu[];

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
static int i2sadc_cmd_help(int argc, char *argv[])
{	
	return cmdline_help_general(&i2sadc_test_menu[0]);
}

static int i2sdac_cmd_help(int argc, char *argv[])
{	
	return cmdline_help_general(&i2sdac_test_menu[0]);
}

static int i2s1_cmd_help(int argc, char *argv[])
{	
	return cmdline_help_general(&i2s1_test_menu[0]);
}

static int i2svad_cmd_help(int argc, char *argv[])
{	
	return cmdline_help_general(&i2svad_test_menu[0]);
}

/*****************************************************************************/
//	Function:  static int i2s1_master_tx_test(int argc, char *argv[])
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
static int i2s1_master_tx_test(int argc, char *argv[])
{
	E_I2S_PORT port = I2S_1;
	E_I2S_TYPE type; 
	E_DT_MODE dt_mode;
	E_PI_MODE pi_mode;
	E_I2S_PCM_FORMAT sample_format;
	int32_t ret;
	int32_t fail = 0;
	uint32_t len;
	uint32_t max_len = TEST_BUFFER_SIZE/2;
	uint32_t *p_tx = (uint32_t *)test_txrx_buff;

	// Check the validity of parameter count
	if (argc < 6) 
	{
		debug("format: %s type dt_mode pi_mode sample_format len\n", argv[0]);

		debug("type(data transfer format): 0, standard; 1, left justified; 2, right justified; 3, I2S_TDM_POL0, 4, I2S_TDM_POL1\n");
		debug("dt_mode(cpu/dma)          : 0, cpu; 1, dma\n");
		debug("pi_mode(polling/interrupt): 0, polling; 1, interrupt\n");
		debug("sample_format             : 16, S16_LE; 24, S24_LE; 32, S32_LE\n");
		debug("len                       : data length in bytes\n");

		return ERR_TEST_ARGC;
	}

	// Get the parameters
	type = get_arg_ulong(argv[1]);
	dt_mode = get_arg_ulong(argv[2]);
	pi_mode = get_arg_ulong(argv[3]);
	sample_format = get_arg_ulong(argv[4]);
	len = get_arg_ulong(argv[5]);

	// configure clock
	i2s1_master_clk_config((sample_format*2), type, sample_format);

	// initialize the spi hw
	i2s_init(port, 0, 0, 0, 0, 0, dt_mode, pi_mode, sample_format, 1, 0, 0);

	// Get the parameters
	int data_step = g_i2s[port].data_step;
	len = ((len + (data_step * 2) - 1) / (data_step* 2 )) * (data_step * 2);
	info("actual aligned transfer length %d\n", len);
	if (len > max_len)
	{
		len = max_len;
		debug("test len exceed the max test buffer size %d, align down to %d\n", max_len, len);
	}

	// initialize tx data buffer
	mem_set_seq((uint8_t *)p_tx, len);

	// transfer data
	ret = i2s_xfer_tx(port, p_tx, len);
	if (ret) 
	{
		debug("i2s write fail,ret=%d\n", ret);
		fail++;
	}
	
	i2s_exit(port);

	if (fail)
	{
		info("%s: test fail\n", argv[0]);
	}
	else
	{
		info("%s: view the oscilloscope result for pass or fail\n", argv[0]);
	}
	
	return ERR_TEST_DONE;	
}

/*****************************************************************************/
//	Function:  static int i2s1_slave_tx_test(int argc, char *argv[])
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
static int i2s1_slave_tx_test(int argc, char *argv[])
{
	E_I2S_PORT port = I2S_1;
	E_I2S_TYPE type; 
	E_DT_MODE dt_mode;
	E_PI_MODE pi_mode;
	E_I2S_PCM_FORMAT sample_format;
	int32_t ret;
	int32_t fail = 0;
	uint32_t len;
	uint32_t max_len = TEST_BUFFER_SIZE/2;
	uint32_t *p_tx = (uint32_t *)test_txrx_buff;

	// Check the validity of parameter count
	if (argc < 6) 
	{
		debug("format: %s type dt_mode pi_mode sample_format len\n", argv[0]);

		debug("type(data transfer format): 0, standard; 1, left justified; 2, right justified; 3, I2S_TDM_POL0, 4, I2S_TDM_POL1\n");
		debug("dt_mode(cpu/dma)          : 0, cpu; 1, dma\n");
		debug("pi_mode(polling/interrupt): 0, polling; 1, interrupt\n");
		debug("sample_format             : 16, S16_LE; 24, S24_LE; 32, S32_LE\n");
		debug("len                       : data length in bytes\n");

		return ERR_TEST_ARGC;
	}

	// Get the parameters
	type = get_arg_ulong(argv[1]);
	dt_mode = get_arg_ulong(argv[2]);
	pi_mode = get_arg_ulong(argv[3]);
	sample_format = get_arg_ulong(argv[4]);
	len = get_arg_ulong(argv[5]);

	// configure clock
	i2s1_slave_clk_config((sample_format*2), type, sample_format);

	// initialize the spi hw
	i2s_init(port, 0, 0, 0, 0, 0, dt_mode, pi_mode, sample_format, 1, 0, 0);

	// Get the parameters
	int data_step = g_i2s[port].data_step;
	len = ((len + (data_step * 2) - 1) / (data_step* 2 )) * (data_step * 2);
	info("actual aligned transfer length %d\n", len);
	if (len > max_len)
	{
		len = max_len;
		debug("test len exceed the max test buffer size %d, align down to %d\n", max_len, len);
	}

	// initialize tx data buffer
	mem_set_seq((uint8_t *)p_tx, len);

	// transfer data
	ret = i2s_xfer_tx(port, p_tx, len);
	if (ret) 
	{
		debug("i2s write fail,ret=%d\n", ret);
		fail++;
	}
	
	i2s_exit(port);

	if (fail)
	{
		info("%s: test fail\n", argv[0]);
	}
	else
	{
		info("%s: view the oscilloscope result for pass or fail\n", argv[0]);
	}
	
	return ERR_TEST_DONE;	
}


/*****************************************************************************/
//	Function:  static int i2sdac_master_tx_test(int argc, char *argv[])
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
static int i2sdac_master_tx_test(int argc, char *argv[])
{
	E_I2S_PORT port = I2S_DAC;
	E_I2S_TYPE type; 
	E_DT_MODE dt_mode;
	E_PI_MODE pi_mode;
	E_I2S_PCM_FORMAT sample_format;
	E_I2S_DAC_DATA_MODE dac_data_mode;
	int32_t ret;
	int32_t fail = 0;
	uint32_t len;
	uint32_t max_len = TEST_BUFFER_SIZE/2;
	uint32_t *p_tx = (uint32_t *)test_txrx_buff;

	// Check the validity of parameter count
	if (argc < 7) 
	{
		debug("format: %s data_mode type dt_mode pi_mode sample_format len\n", argv[0]);

		debug("data_mode(dac data mode)  : 0, to codec; 1, to PAD\n");
		debug("type(data transfer format): 0, standard; 1, left justified; 2, right justified\n");
		debug("dt_mode(cpu/dma)          : 0, cpu; 1, dma\n");
		debug("pi_mode(polling/interrupt): 0, polling; 1, interrupt\n");
		debug("sample_format             : 16, S16_LE; 24, S24_LE; 32, S32_LE\n");
		debug("len                       : data length in bytes\n");
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters
	dac_data_mode = get_arg_ulong(argv[1]) & 0x1;
	type = get_arg_ulong(argv[2]);
	dt_mode = get_arg_ulong(argv[3]);
	pi_mode = get_arg_ulong(argv[4]);
	sample_format = get_arg_ulong(argv[5]);
	len = get_arg_ulong(argv[6]);

	// configure master clock
	i2s_dac_clk_config((sample_format * 2), I2S_MASTER, dac_data_mode);

	// initialize the spi hw
	i2s_init(port, 0, 0, 0, 0, 0, dt_mode, pi_mode, sample_format, 1, 0, 0);

	// Get the parameters
	int data_step = g_i2s[port].data_step;
	len = ((len + (data_step * 2) - 1) / (data_step* 2 )) * (data_step * 2);
	info("actual aligned transfer length %d\n", len);
	if (len > max_len)
	{
		len = max_len;
		debug("test len exceed the max test buffer size %d, align down to %d\n", max_len, len);
	}

	// initialize tx data buffer
	mem_set_seq((uint8_t *)p_tx, len);

	// transfer data
	ret = i2s_xfer_tx(port, p_tx, len);
	if (ret) 
	{
		debug("i2s write fail,ret=%d\n", ret);
		fail++;
	}
	
	i2s_exit(port);

	if (fail)
	{
		info("%s: test fail\n", argv[0]);
	}
	else
	{
		info("%s: view the oscilloscope result for pass or fail\n", argv[0]);
	}
	
	return ERR_TEST_DONE;	
}

static int i2s_test_loopback_init(int argc, char *argv[])
{
	E_I2S_PORT port_tx;
	E_I2S_PORT port_rx;
	E_DT_MODE dt_mode;
	E_PI_MODE pi_mode;
	E_I2S_PCM_FORMAT sample_format;
	int ch0_en, ch1_en, ch2_en;		
	uint8_t div;
	E_I2S_TYPE type = I2S_STANDARD; 
	E_I2S_WSS wss = I2S_WSS_16_CLK_CYCLE; 
	E_I2S_WLEN wlen = I2S_WLEN_IGNORE; 
	E_I2S_CTRL clk_en = I2S_DIS;

	// Check the validity of parameter count
	if (argc < 9) 
	{
		debug("format: %s port_rx port_tx dt_mode pi_mode sample_format rx_ch0_en rx_ch1_en rx_ch2_en\n", argv[0]);
		debug("port_rx                   : 0, I2S_ADC\n");
		debug("port_tx                   : 1, I2S_DAC; 2, I2S_1;\n");
		debug("dt_mode(cpu/dma)          : 0, cpu; 1, dma\n");
		debug("pi_mode(polling/interrupt): 0, polling; 1, interrupt\n");
		debug("sample_format             : 16, S16_LE; 24, S24_LE; 32, S32_LE\n");
		debug("rx_chx_en                 : 0, chx disable; 1, chx enable\n");
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters
	port_rx = get_arg_ulong(argv[1]);
	port_tx = get_arg_ulong(argv[2]);
	dt_mode = get_arg_ulong(argv[3]);
	pi_mode = get_arg_ulong(argv[4]);
	sample_format = get_arg_ulong(argv[5]);
	if ((sample_format != SNDRV_PCM_FORMAT_S16_LE) && 
		(sample_format != SNDRV_PCM_FORMAT_S24_LE) &&
		(sample_format != SNDRV_PCM_FORMAT_S32_LE))
	{
		debug("designware-i2s: unsupported PCM fmt");
		
		return ERR_TEST_PARA;
	}
	
	ch0_en = get_arg_ulong(argv[6]);
	ch1_en = get_arg_ulong(argv[7]);
	ch2_en = get_arg_ulong(argv[8]);

	i2s_test_prepare(port_rx);
	i2s_test_prepare(port_tx);
	
	// configure master clock
	div = sample_format << 1;
	i2s_adc_clk_congig(div, I2S_ADC_DATA_FROM_CODEC);

	// configure slave	clock
	if (port_tx == I2S_DAC) 
	{
		i2s_dac_clk_config(div, I2S_SLAVE, I2S_DAC_DATA_TO_PAD);
	} 
	else if (port_tx == I2S_1) 
	{
		i2s1_slave_clk_config(div, type, sample_format);
	}

	// initialize the spi hw
	i2s_init(port_rx, 0, type, wss, wlen, clk_en, dt_mode, pi_mode, sample_format, ch0_en, ch1_en, ch2_en);
	i2s_init(port_tx, 0, type, wss, wlen, clk_en, dt_mode, pi_mode, sample_format, 1, 0, 0);
	
	return ERR_TEST_DONE;	
}

/*****************************************************************************/
//	Function:  static int i2s_test_loopback(int argc, char *argv[])
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
//		 i2s loopback test.
//
//	requierment: 
//		connect tx controller channel and rx controller channel
//
/*****************************************************************************/
static int i2s_test_loopback(int argc, char *argv[])
{
	E_I2S_PORT port_tx, port_rx;
	int32_t ret;
	int32_t fail = 0;
	uint32_t len;
	uint32_t max_len = TEST_BUFFER_SIZE/2;
	uint32_t *p_tx = (uint32_t *)test_txrx_buff;
	uint32_t *p_rx = (uint32_t *)(test_txrx_buff + max_len);

	debug("%s: i2s tx and rx test\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 10) 
	{
		debug("format: %s port_rx port_tx dt_mode pi_mode sample_format rx_ch0_en, rx_ch1_en, rx_ch2_en len\n", argv[0]);
		debug("port_rx                   : 0 I2S_ADC\n");
		debug("port_tx                   : 1 I2S_DAC; 2 I2S_1;\n");
		debug("dt_mode(cpu/dma)          : 0, cpu; 1, dma\n");
		debug("pi_mode(polling/interrupt): 0, polling; 1, interrupt\n");
		debug("sample_format             : 16, S16_LE; 24, S24_LE; 32, S32_LE\n");
		debug("rx_chx_en                 : 0, chx disable; 1, chx enable\n");
		debug("len                       : data length in bytes\n");
		
		return ERR_TEST_ARGC;
	}
	
	// Initialize test parameter and hw initialize
	if (i2s_test_loopback_init(argc, argv))
	{
		return ERR_TEST_FAIL;
	}
	
	port_rx = get_arg_ulong(argv[1]);
	port_tx = get_arg_ulong(argv[2]);
	len = get_arg_ulong(argv[9]);

	// Get the parameters
	int data_step = g_i2s[port_tx].data_step;
	len = ((len+(data_step*2)-1)/(data_step*2)) * (data_step*2);
	printf("actual aligned transfer length %d\n", len);
	if (len > max_len)
	{
		len = max_len;
		debug("test len exceed the max test buffer size %d, align down to %d\n", max_len, len);
	}

	// initialize rx and tx data buffer
	mem_set8((uint8_t *)p_rx, 0, len);
	mem_set_seq((uint8_t *)p_tx, len);

	// transfer data
	ret = i2s_xfer(port_tx, port_rx, p_tx, p_rx, len);
	if (ret) 
	{
		debug("i2s read/write fail,ret=%d\n", ret);
		fail++;
	}
	i2s_exit(port_tx);
	i2s_exit(port_rx);

	// search for the 1st valid data position of rx bffer
	uint32_t *p_rx_ch = p_rx;	
	uint32_t dummy_len;
	while ((*p_rx_ch == 0) && ((p_rx_ch - p_rx) < (len>>2)))
	{
		p_rx_ch++;
	}

	dummy_len = (uint32_t)((p_rx_ch - p_rx)<<2);

	// printf tx and rx
	print_in_line_reg32((uint8_t *)"tx", (uint32_t *)p_tx, len);
	printf("\nrx dummy byte count: %d\n\n", dummy_len);
	print_in_line_reg32((uint8_t *)"rx", (uint32_t *)p_rx_ch, (len - dummy_len));

	// compare tx and rx(byte by byte)
	fail += i2s_mem_check_mask((uint8_t *)p_rx_ch, (uint8_t *)p_tx, (len - dummy_len), g_i2s[port_rx].sample_format); 
	if (fail)
	{
		debug("%s: test fail\n", argv[0]);
	}
	else
	{
		debug("%s: test pass\n", argv[0]);
	}

	return ERR_TEST_DONE;
}

int i2s_adc_rx_int(int argc, char *argv[])
{
	E_I2S_PORT port_rx;
	E_DT_MODE dt_mode;

	/*add by Sunny*/
	E_PI_MODE pi_mode;
	E_I2S_PCM_FORMAT sample_format;
	int len, ch0_en, ch1_en, ch2_en;			
	int ret, fail = 0;			
	uint32_t *p_rx = (uint32_t *)test_txrx_buff;

	// Check the validity of parameter count
	if (argc < 9) 
	{
		debug("format: %s port_rx dt_mode pi_mode sample_format rx_ch0_en, rx_ch1_en, rx_ch2_en len\n", argv[0]);
		debug("port_rx                    : 0 I2S_ADC\n");
		debug("dt_mode(cpu/dma)           : 0, cpu; 1, dma\n");
		debug("pi_mode(polling/interrupt) : 0, polling; 1, interrupt\n");
		debug("sample_format              : 16, S16_LE; 24, S24_LE; 32, S32_LE\n");
		debug("rx_chx_en                  : enable channel x for receiver\n");
		
		return ERR_TEST_ARGC;
	}

	// Get the parameters
	port_rx = get_arg_ulong(argv[1]);
	dt_mode = get_arg_ulong(argv[2]);

	pi_mode = get_arg_ulong(argv[3]);
	sample_format = get_arg_ulong(argv[4]);
	ch0_en = get_arg_ulong(argv[5]);
	ch1_en = get_arg_ulong(argv[6]);
	ch2_en = get_arg_ulong(argv[7]);
	len = get_arg_ulong(argv[8]);

	i2s_test_prepare(port_rx);

	// configure master clock
	i2s_adc_clk_congig(sample_format * 2, I2S_ADC_DATA_FROM_CODEC);

	// initialize the spi hw
	i2s_init(port_rx, 0, 0, 0, 0, 0, dt_mode, pi_mode, sample_format, ch0_en, ch1_en, ch2_en);

	// Get the parameters
	int data_step = g_i2s[port_rx].data_step;
	len = ((len+data_step*2-1)/(data_step*2)) * (data_step*2);
	printf("actual aligned transfer length %d\n", len);
	if (len > TEST_BUFFER_SIZE/2)
	{
		while( len > TEST_BUFFER_SIZE/2)
			len -= data_step*2;
		debug("test len exceed the max test buffer size %d, align down to %d\n", TEST_BUFFER_SIZE/2, len);
	}

	// initialize rx and tx data buffer
	mem_set8(p_rx, 2, len);

	// transfer data
	ret = i2s_xfer_rx(port_rx, p_rx, len);
	if (ret) 
	{
		debug("i2s read fail,ret=%d\n", ret);
		fail++;
	}
	i2s_exit(port_rx);
	if (fail)
	{
		debug("%s: test fail\n", argv[0]);
	}
	else
	{
		debug("%s: test pass\n", argv[0]);
	}
	return ERR_TEST_DONE;	

}

static int i2s_vad_loopback_init(int argc, char *argv[])
{
	E_I2S_PORT port_rx = I2S_VAD;
	struct brite_i2s *p_i2s_rx = &g_i2s[port_rx];
	E_DT_MODE dt_mode = DT_CPU;
	E_PI_MODE pi_mode;
	E_I2S_PCM_FORMAT sample_format = SNDRV_PCM_FORMAT_S32_LE;
	E_I2S_TYPE type = I2S_STANDARD; 
	E_I2S_WSS wss = I2S_WSS_16_CLK_CYCLE; 
	E_I2S_WLEN wlen = I2S_WLEN_IGNORE; 
	E_I2S_CTRL clk_en = I2S_DIS;
	E_VAD_LR_SEL lr_sel;
	E_VAD_STOP_DLY stop_dly;
	
	// Check the validity of parameter count
	if (argc < 12) 
	{
		debug("format: %s port_rx port_tx dt_mode pi_mode sample_format rx_ch0_en, rx_ch1_en, rx_ch2_en len lr_sel stop_dly\n", argv[0]);
		debug("port_rx                            : 0, I2S_ADC\n");
		debug("port_tx                            : 1, I2S_DAC; 2, I2S_1\n");
		debug("dt_mode(cpu/dma)                   : 0, cpu; 1, dma\n");
		debug("pi_mode(polling/interrupt)         : 0, polling; 1, interrupt\n");
		debug("sample_format                      : 16, S16_LE; 24, S24_LE; 32, S32_LE\n");
		debug("rx_chx_en                          : 0, chx disable; 1, chx enable\n");
		debug("len                                : data length in bytes\n");		
		debug("lr_sel                             : 0, left; 1 right\n");
		debug("stop_dly                           : 0, no delay; 1, 1 sample delay; 2: 2 sample delay; 3, 3 sample delay\n");
		
		return ERR_TEST_ARGC;
	}

	//sys_sw_rst(I2SVAD_RST);

	i2s_test_prepare(port_rx);

	// Get the parameters
	pi_mode = get_arg_ulong(argv[4]);

	// initialize the spi hw
	i2s_init(port_rx, 0, type, wss, wlen, clk_en, dt_mode, PI_INT, sample_format, 1, 0, 0);	

	p_i2s_rx->stream_dir = SNDRV_PCM_STREAM_CAPTURE;
	i2s_start_transfer(p_i2s_rx, NULL, 0);

	// configure vad(normal -> idle)
	lr_sel = get_arg_ulong(argv[10]) & 0x1;
	stop_dly = get_arg_ulong(argv[11]) & 0x3;
	vad_normal_to_idle_init(lr_sel, stop_dly);

	g_spint_cnt = 0;
	g_slint_cnt = 0;

	// enable sp and sl interrupt
	vad_wr_reg(VAD_SPINT_EN_OFST, 1);
	vad_wr_reg(VAD_SLINT_EN_OFST, 1);
	
	return ERR_TEST_DONE;	
}

/*****************************************************************************/
//	Function:  static int i2s_vad_test_loopback(int argc, char *argv[])
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
//		 i2s loopback test.
//
//	requierment: 
//		connect tx controller channel and rx controller channel with vad enable upon test data
//
/*****************************************************************************/
static int i2s_vad_test_loopback(int argc, char *argv[])
{
	E_I2S_PORT port_rx;
	E_I2S_PORT port_tx;
	E_VAD_LR_SEL lr_sel;
	int32_t ret;
	int32_t fail = 0;
	uint32_t len;
	uint32_t max_len = TEST_BUFFER_SIZE/2;
	uint32_t *p_tx = (uint32_t *)test_txrx_buff;
	uint32_t *p_rx = (uint32_t *)(test_txrx_buff + max_len);

	int read_value;
	int j;

	debug("%s: i2s tx and rx test\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 12) 
	{
		debug("format: %s port_rx port_tx dt_mode pi_mode sample_format rx_ch0_en, rx_ch1_en, rx_ch2_en len lr_sel stop_dly\n", argv[0]);
		debug("port_rx                            : 0, I2S_ADC\n");
		debug("port_tx                            : 1, I2S_DAC; 2, I2S_1\n");
		debug("dt_mode(dma)                       : 1, dma\n");
		debug("pi_mode(polling)                   : 0, polling\n");
		debug("sample_format                      : 32, S32_LE\n");
		debug("rx_chx_en                          : 0, chx disable; 1, chx enable\n");
		debug("len                                : data length in bytes\n");		
		debug("lr_sel                             : 0, left; 1 right\n");
		debug("stop_dly                           : 0, no delay; 1, 1 sample delay; 2: 2 sample delay; 3, 3 sample delay\n");
		
		return ERR_TEST_ARGC;
	}	
	
	// Initialize rx/tx test parameter and hw initialize
	if (i2s_test_loopback_init(argc, argv))
	{
		return ERR_TEST_FAIL;
	}
	
	port_rx = I2S_ADC;					// fixed
	port_tx = get_arg_ulong(argv[2]);
	len = get_arg_ulong(argv[9]);
	lr_sel = get_arg_ulong(argv[10]) & 0x1;

	// Get the parameters
	int data_step = g_i2s[port_tx].data_step;
	len = ((len+(data_step*2)-1)/(data_step*2)) * (data_step*2);
	printf("actual aligned transfer length %d\n", len);
	if (len > max_len)
	{
		len = max_len;
		debug("test len exceed the max test buffer size %d, align down to %d\n", max_len, len);
	}

	// initialize rx and tx data buffer
	mem_set8((uint8_t *)p_rx, 0, len);
	mem_set8((uint8_t *)XRAM_ADDR, 0xFF, 0x10000);
	//mem_set_seq((uint8_t *)p_tx, 0x4000);
	for (j = 0; j < 512; j = j + 1) 
	{
		p_tx[j] = 0x10000*j;
	}

	// Initialize i2s_vad parameter and hw initialize
	if (i2s_vad_loopback_init(argc, argv))
	{
		return ERR_TEST_FAIL;
	}

	// transfer data
	ret = i2s_vad_xfer(port_tx, port_rx, p_tx, p_rx, len);
	if (ret) 
	{
		debug("i2s read/write fail,ret=%d\n", ret);
		fail++;
	}

    while (vad_rd_reg(VAD_XMEM_ADDR_OFST) < 0x80) 
	{
		//info("xmem: 0x%x\n", vad_rd_reg(VAD_XMEM_ADDR_OFST));
	}
	
	vad_sw_ctrl(VAD_SW_MODE_ADC);
    _DISABLE_CLOCK_vad_mem_clk_129_
    vad_switch_xmem(SWITCH_XMEM_TO_AXI);
    _SWITCH_CLOCK_vad_mem_clk_129_SOURCE_axi_clk
    _ENABLE_CLOCK_vad_mem_clk_129_
    vad_sw_ctrl(VAD_SW_MODE_BOTH);

	dma_polling_st(DMA0, g_i2s[port_tx].dma_ch_rx);
	dma_polling_st(DMA0, g_i2s[port_tx].dma_ch_tx);

	i2s_exit(port_tx);
	i2s_exit(port_rx);

	// search for the 1st valid data position of rx bffer
	uint32_t *p_rx_ch = p_rx;	
	uint32_t dummy_len;
	while ((*p_rx_ch == 0) && ((p_rx_ch - p_rx) < (len>>2)))
	{
		p_rx_ch++;
	}

	dummy_len = (uint32_t)((p_rx_ch - p_rx)<<2);

	// printf tx and rx
	print_in_line_reg32((uint8_t *)"tx", (uint32_t *)p_tx, len);
	printf("\nrx dummy byte count: %d\n\n", dummy_len);
	print_in_line_reg32((uint8_t *)"rx", (uint32_t *)p_rx_ch, (len - dummy_len));
	print_in_line_reg32((uint8_t *)"XRAM_ADDR", (uint32_t *)XRAM_ADDR, len);

	// search for the 1st valid data position of rx bffer
	uint32_t *p_xram = (uint32_t *)XRAM_ADDR;	
	for (j = 0; j < ((0x80*2)/4); j++)
	{
		// don't care about the unvaliable data
		if (p_xram[j] == 0)
		{
			continue;
		}

		read_value = p_xram[j];
		
		info("j = %x, read_value: 0x%x\n", j, read_value);
		if (lr_sel == 0)
		{
			if ((read_value & (1<<16)) == (1<<16))
			{
				fail++;
				break;	
			}					
		}
		else
		{
			if ((read_value & (1<<16)) == 0)
			{
				fail++;
				break;	
			}								
		}		
	}

	print_in_line_reg32((uint8_t *)"VAD_BASE", (uint32_t *)VAD_BASE, 0x84);	

	if (fail)
	{
		debug("%s: test fail\n", argv[0]);
	}
	else
	{
		debug("%s: test pass\n", argv[0]);
	}

	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  static int i2s_vad_test_int(int argc, char *argv[])
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
//		 i2s loopback test.
//
//	requierment: 
//		connect tx controller channel and rx controller channel with vad enable upon real data.
//
/*****************************************************************************/
static int i2s_vad_test_int(int argc, char *argv[])
{
	E_I2S_PORT port_rx;
	E_I2S_PORT port_tx;
	E_VAD_LR_SEL lr_sel;
	int32_t ret;
	int32_t fail = 0;
	uint32_t len = (sizeof(vad_data0) + 7) & (~7);	// fixed;
	uint32_t max_len = TEST_BUFFER_SIZE/2;
	uint32_t *p_tx = (uint32_t *)vad_data0;
	uint32_t *p_rx = (uint32_t *)(test_txrx_buff + max_len);

	debug("%s: i2s vad sp/sl interrupt test\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 12) 
	{
		debug("format: %s port_rx port_tx dt_mode pi_mode sample_format rx_ch0_en, rx_ch1_en, rx_ch2_en len lr_sel stop_dly\n", argv[0]);
		debug("port_rx                            : 0, I2S_ADC\n");
		debug("port_tx                            : 1, I2S_DAC; 2, I2S_1\n");
		debug("dt_mode(cpu)                       : 0, cpu;\n");
		debug("pi_mode(polling/interrupt)         : 0, polling; 1, interrupt\n");
		debug("sample_format                      : 32, S32_LE\n");
		debug("rx_chx_en                          : 0, chx disable; 1, chx enable\n");
		debug("len                                : non't care\n");		
		debug("lr_sel                             : 0, left; 1 right\n");
		debug("stop_dly                           : 0, no delay; 1, 1 sample delay; 2: 2 sample delay; 3, 3 sample delay\n");
		
		return ERR_TEST_ARGC;
	}	
	
	// Initialize rx/tx test parameter and hw initialize
	if (i2s_test_loopback_init(argc, argv))
	{
		return ERR_TEST_FAIL;
	}
	
	port_rx = I2S_ADC;						// fixed
	port_tx = get_arg_ulong(argv[2]);
	lr_sel = get_arg_ulong(argv[10]) & 0x1;

	// Get the parameters
	int data_step = g_i2s[port_tx].data_step;

	info("len: %d, data_step: %d\n", len, data_step);
	
	len = ((len+(data_step*2)-1)/(data_step*2)) * (data_step*2);
	info("actual aligned transfer length %d\n", len);

	// initialize rx and tx data buffer
	mem_set8((uint8_t *)p_rx, 0, len);
	mem_set8((uint8_t *)XRAM_ADDR, 0xFF, 0x10000);

	// Initialize i2s_vad parameter and hw initialize
	if (i2s_vad_loopback_init(argc, argv))
	{
		return ERR_TEST_FAIL;
	}	

	// transfer data
	ret = i2s_xfer_tx(port_tx, p_tx, len);
	if (ret) 
	{
		debug("i2s read/write fail,ret=%d\n", ret);
		fail++;
	}

    _DISABLE_CLOCK_vad_mem_clk_129_
    vad_switch_xmem(SWITCH_XMEM_TO_AXI);
    _SWITCH_CLOCK_vad_mem_clk_129_SOURCE_axi_clk
    _ENABLE_CLOCK_vad_mem_clk_129_
    vad_sw_ctrl(VAD_SW_MODE_BOTH);

	i2s_exit(port_tx);
	i2s_exit(port_rx);

	// polling the interrrupt triggered
	while ((g_spint_cnt == 0) || (g_slint_cnt == 0))
	{
		// check the input character
		if (console_tstc()) 
		{
			if ('q' == console_getc()) 
			{
				fail++;
				break;
			}
		}	
	}
	
	// printf tx and rx
	print_in_line_reg32((uint8_t *)"tx", (uint32_t *)p_tx, 0x200);
	print_in_line_reg32((uint8_t *)"XRAM_ADDR", (uint32_t *)XRAM_ADDR, 0x200);

	print_in_line_reg32((uint8_t *)"VAD_BASE", (uint32_t *)VAD_BASE, 0x84);	

	if (fail)
	{
		debug("%s: test fail\n", argv[0]);
	}
	else
	{
		debug("%s: test pass\n", argv[0]);
	}

	return ERR_TEST_DONE;
}

int i2s_test_prepare(int port)
{
	struct brite_i2s *priv = &g_i2s[port];

	// check the validity of parameter
	if (port >= NUM_I2S_PORTS)
	{
		debug("port %d is too large!\n", port);
		return ERR_TEST_PARA;
	}
	
	// clear the structure
	mem_set8(priv, 0, sizeof(struct brite_i2s));

	if (port == I2S_ADC)
	{
		priv->port = port;
		priv->base = I2S_ADC_BASE;
		priv->pclk = I2S_ADC_PCLK;

		// dma parameter
		priv->dma_burst_len = BURST_8;
		priv->dma_core_idx = 0;
		priv->dma_ch_rx = DMA_CH0;
		priv->dma_req_rx = DMA_REQ_I2S_ADC;

		// interrupt parameter
		priv->int_en = 0;
		priv->int_vec = INT_I2S_ADC_DA_0;
	}
	else if (port == I2S_DAC)
	{
		priv->port = port;
		priv->base = I2S_DAC_BASE;
		priv->pclk = I2S_DAC_PCLK;

		// dma parameter
		priv->dma_burst_len = BURST_8;
		priv->dma_core_idx = 0;
		priv->dma_ch_tx = DMA_CH2;
		priv->dma_req_tx = DMA_REQ_I2S_DAC;

		// interrupt parameter
		priv->int_en = 0;
		priv->int_vec = INT_I2S_DAC_EMP_0;

		// configure mode and pinmux
		pad_dmic_clk11_pinmux(PINMUX_FUNC2, PIN_PD_DIS);  	// i2s0_bclk
		pad_dmic_clk12_pinmux(PINMUX_FUNC2, PIN_PD_DIS);  	// i2s0_lrck
		pad_dmic_in1_pinmux(PINMUX_FUNC2, PIN_PD_DIS);  	// i2s0_d0
	}
	else if (port == I2S_1)
	{
		priv->port = port;
		priv->base = I2S1_BASE;
		priv->pclk = I2S1_PCLK;

		// dma parameter
		priv->dma_burst_len = BURST_8;
		priv->dma_core_idx = 0;
		priv->dma_ch_tx = DMA_CH1;
		priv->dma_req_tx = DMA_REQ_I2S1;

		// interrupt parameter
		priv->int_en = 0;
		priv->int_vec = INT_I2S1_EMP_0;

		// configure mode and pinmux
		pad_dopwmlp_pinmux(PINMUX_FUNC1, PIN_PD_DIS);	// i2s1_bclk_out
		pad_dopwmln_pinmux(PINMUX_FUNC1, PIN_PD_DIS);	// i2s1_lrck_out
		pad_dopwmrp_pinmux(PINMUX_FUNC1, PIN_PD_DIS);	// i2s1_sdo0
	}
	else if (port == I2S_VAD)
	{
		priv->port = port;
		priv->base = I2S_VAD_BASE;
		priv->pclk = I2S1_VAD_PCLK;

		// interrupt parameter
		priv->int_en = 0;
		priv->int_vec = INT_I2S_VAD_DA;	
	}
	else
	{
		// not defined on current HW
	}

	return ERR_TEST_DONE;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry i2sadc_test_menu[] = {
	{"help", 					i2sadc_cmd_help,  	" : Display list of commands"},
	{"h",						i2sadc_cmd_help,	"    : alias for help"},
	{"?",						i2sadc_cmd_help, 	"    : alias for help"},	
	{"i2s_test_loopback",		i2s_test_loopback,	"	: i2s loopback test, need one board"},
	{"i2s_adc_rx_int", 	    	i2s_adc_rx_int,	    "	: i2s_adc_rx_int"},		
	{0, 						0, 					0}
};

cmdline_entry i2sdac_test_menu[] = {
	{"help", 					i2sdac_cmd_help, 		" : Display list of commands"},
	{"h",						i2sdac_cmd_help,		"    : alias for help"},
	{"?",						i2sdac_cmd_help, 		"    : alias for help"},	
	{"i2sdac_master_tx_test", 	i2sdac_master_tx_test,	"	: i2sdac master mode test"},
	{0, 						0, 						0}
};

cmdline_entry i2s1_test_menu[] = {
	{"help", 					i2s1_cmd_help,  		" : Display list of commands"},
	{"h",						i2s1_cmd_help,			"    : alias for help"},
	{"?",						i2s1_cmd_help, 			"    : alias for help"},	
	{"i2s1_master_tx_test", 	i2s1_master_tx_test,	"	: i2s1 master mode test"},
	{"i2s1_slave_tx_test", 		i2s1_slave_tx_test,		"	: i2s1 slave mode test"},
	{0, 						0, 						0}
};

cmdline_entry i2svad_test_menu[] = {
	{"help",                	i2svad_cmd_help,    	" : Display list of commands"},
	{"h",                   	i2svad_cmd_help,    	"    : alias for help"},
	{"?",                   	i2svad_cmd_help,    	"    : alias for help"},
	{"i2s_vad_test_loopback",   i2s_vad_test_loopback,  "       : i2s vad loopback test"},
	{"i2s_vad_test_int",   		i2s_vad_test_int,  		"       : i2s vad interrupt test"},
	{0,                     	0,                  	0}
};

#endif

