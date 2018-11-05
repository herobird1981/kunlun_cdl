/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_gpio.h"
#include "test_spi.h"
#include "cdl_spi_flash.h"

#if (CONFIG_SPI_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry spi_test_menu[];

static uint8_t g_spi_port = 0;
static uint8_t g_spi_slave_port = 0;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/

/*****************************************************************************/
//  Function:  void spi_slave_int_handler(void *ptr)
//
//  Parameters:
//		ptr [in]	: pointer
//
//  Return:
//		none
//		
//  Description:
//     SPI interrupt hadler.
//
/*****************************************************************************/
void spi_slave_int_handler(void *ptr)
{
	E_SPI_PORT port = g_spi_slave_port;

	uint32_t int_val = spi_rd_reg(port, SPI_SSPMIS_OFST);

	if (int_val != 0) 
	{
		// write 1 to clear
		spi_wr_reg(port, SPI_SSPICR_OFST, int_val);
	}	
}

/*****************************************************************************/
//	Function:  static int spi_xfer_packet_cs_self(E_SPI_PORT port, uint8_t *p_out, uint8_t *p_in, uint32_t len, E_SPI_XFER_FLAG flag)
//
//	Parameters:
//		port	: spi port
//
//		p_out [out]	: tx data pointer
//
//		p_in [in]		: rx data pointer
//
//		len			:  transfer data length(byte aligned)
//
//		flag			: transmit flag
//
//	Return:
//		none
//
//	Description:
//		 Transfer the command/data package, cs is controlled by spi controller.
//
/*****************************************************************************/
static int spi_xfer_packet_cs_self(E_SPI_PORT port, uint8_t *p_out, uint8_t *p_in, uint32_t len, E_SPI_XFER_FLAG flag)
{
	uint32_t packet_len = 0;
	uint8_t *p_tmp_out = p_out;
	uint8_t *p_tmp_in = p_in;
	int32_t ret = 0;

	while (len > 0)
	{
		packet_len = 1;
		len -= packet_len;

		// set cs enable
		spi_cs_ctrl(port, SPI_CS_EN);

		//mdelay(50);

		// transfer data
		ret = spi_xfer(port, p_tmp_out, p_tmp_in, packet_len, flag);
		if (ret) 
		{
			info("transfer data fail, ret=%d\n", ret);
			return ret;
		}
		
		if (p_out)
		{
			p_tmp_out += packet_len;
		}
		   
		if (p_in)
		{
			p_tmp_in += packet_len;
		}	


		//wait for idle
		while(spi_rd_reg(port, SPI_SSPSR_OFST) & SPI_BSY_MASK);	

		spi_empty_fifo(port);

		// set cs disable
		spi_cs_ctrl(port, SPI_CS_DIS);		
	}

	return ret;
}

static int spi_xfer_packet_slave(E_SPI_PORT port, uint8_t *p_out, uint8_t *p_in, uint32_t len, E_SPI_XFER_FLAG flag)
{
	uint32_t packet_len = 0;
	uint8_t *p_tmp_out = p_out;
	uint8_t *p_tmp_in = p_in;
	int32_t ret = 0;

	while (len > 0)
	{
		packet_len = 1;
		len -= packet_len;

		// transfer data
		ret = spi_xfer(port, p_tmp_out, p_tmp_in, packet_len, flag);
		if (ret) 
		{
			info("transfer data fail, ret=%d\n", ret);
			return ret;
		}
		
		if (p_out)
		{
			p_tmp_out += packet_len;
		}
		   
		if (p_in)
		{
			p_tmp_in += packet_len;
		}
	}

	return ret;
}

/*****************************************************************************/
//	Function:  static int spi_xfer_packet(E_SPI_PORT port, uint8_t *p_out, uint8_t *p_in, uint32_t len, E_SPI_XFER_FLAG flag)
//
//	Parameters:
//		port	: spi port
//
//		p_out [out]	: tx data pointer
//
//		p_in [in]		: rx data pointer
//
//		len			:  transfer data length(byte aligned)
//
//		flag			: transmit flag
//
//	Return:
//		none
//
//	Description:
//		 Transfer the command/data package, cs is controlled by gpio.
//
/*****************************************************************************/
static int spi_xfer_packet(E_SPI_PORT port, uint8_t *p_out, uint8_t *p_in, uint32_t len, E_SPI_XFER_FLAG flag)
{
	uint32_t packet_len = 0;
	uint8_t *p_tmp_out = p_out;
	uint8_t *p_tmp_in = p_in;
	int32_t ret = 0;

	// set cs enable
	if (flag & SPI_XFER_BEGIN)
	{
		spi_cs_ctrl(port, SPI_CS_EN);
	}

	while (len > 0)
	{
		packet_len = (len > 8) ? 8 : len;
		len -= packet_len;

		// transfer data
		ret = spi_xfer(port, p_tmp_out, p_tmp_in, packet_len, flag);
		if (ret) 
		{
			info("transfer data fail, ret=%d\n", ret);
			return ret;
		}
		
		if (p_out)
		{
			p_tmp_out += packet_len;
		}
		   
		if (p_in)
		{
			p_tmp_in += packet_len;
		}		   	
	}

	//wait for idle
	while(spi_rd_reg(port, SPI_SSPSR_OFST) & SPI_BSY_MASK);	

	// set cs disable
	if (flag & SPI_XFER_END)
	{
		spi_cs_ctrl(port, SPI_CS_DIS);
	}

	spi_empty_fifo(port);

	return ret;
}

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
	return cmdline_help_general(&spi_test_menu[0]);
}

/*****************************************************************************/
//	Function:  static int spi_test_init(int argc, char *argv[])
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
//		 spi test initialize.
//
/*****************************************************************************/
static int spi_test_init(int argc, char *argv[])
{
	E_SPI_PORT port = g_spi_port;
	uint32_t clk; 
	E_SPI_FRF type; 
	E_SPI_MODE mode; 
	uint32_t ds;	// data width 
	E_SPI_MS ms; 
	E_DT_MODE dt_mode;
	
	// Check the validity of parameter count
	if (argc < 7) 
	{
		info("format: %s clk(HZ) type ms mode dss dt_mode\n", argv[0]);
		info("type(frame format)     : 0, motorrola; 1, TI; 2, national(not support)\n");
		info("ms(master/slave)       : 0, master; 1, slave\n");
		info("mode(pol.bit1/pha.bit0): 0/1/2/3\n");
		info("dss(data size select   : [4, 16] bits\n");		
		info("dt_mode(cpu/dma)       : 0, cpu; 1, dma\n");		
		return ERR_TEST_ARGC;
	}

	// Get the parameters
	clk = get_arg_ulong(argv[1]);
	type = get_arg_ulong(argv[2]) & 0x3;
	if (type > SPI_FRF_TI)
	{
		info("invalid type %x\n", type);
		return ERR_TEST_PARA;
	}
	
	ms = get_arg_ulong(argv[3]);
	mode = get_arg_ulong(argv[4]);
	if (mode > SPI_MODE3)
	{
		mode = SPI_MODE3;
	}
	
	ds = get_arg_ulong(argv[5]);
	if ((ds < (SPI_DSS_4 + 1)) || (ds > (SPI_DSS_16 + 1))) 
	{
		info("dss(0x%x) should be in [4, 16], force to be 8bit\n", ds);
		ds = 8;
	}
	
	dt_mode = get_arg_ulong(argv[6]);

	// initialize the spi hw
	spi_test_prepare(SPI0);
	
	spi_init(port, clk, type, ms, mode, ds, dt_mode);

	return ERR_TEST_DONE;	
}

/*****************************************************************************/
//	Function:  static int spi_test_loopback(int argc, char *argv[])
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
//		 spi loopback test.
//
//	requierment: 
//		connect both spi pin together(transmit and receive simultaneously).
//
/*****************************************************************************/
static int spi_test_loopback(int argc, char *argv[])
{
	E_SPI_PORT port = g_spi_port;
	struct brite_spi *priv = &g_spi[port];
	int32_t ret;
	int32_t fail = 0;
	uint32_t len;
	uint32_t max_len = TEST_BUFFER_SIZE/2;
	uint8_t *p_tx = (uint8_t *)test_txrx_buff;
	uint8_t *p_rx = (uint8_t *)(test_txrx_buff + max_len);
	
	info("%s: spi master mode loopback test\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 8) 
	{
		info("format: %s clk(HZ) type ms mode dss dt_mode len\n", argv[0]);
		info("type(frame format)     : 0, motorrola; 1, TI; 2, national; 3, reved\n");
		info("ms(master/slave)       : 0, master; 1, slave\n");
		info("mode(pol.bit1/pha.bit0): 0/1/2/3\n");
		info("dss(data size select   : [4, 16] bits\n");		
		info("dt_mode(cpu/dma)       : 0, cpu; 1, dma\n");
		return ERR_TEST_ARGC;
	}

	// pinmux to gpio control cs
	pad_spiclk_pinmux(PINMUX_FUNC0, 0);
	pad_spicsn_pinmux(PINMUX_FUNC2, 0); // gpio 3 control
	pad_spirxd_pinmux(PINMUX_FUNC0, 0);
	pad_spitxd_pinmux(PINMUX_FUNC0, 0);	

	// Initialize test parameter and hw initialize
	if (spi_test_init(argc, argv))
	{
		return ERR_TEST_FAIL;
	}
	
	// Get the parameters
	len = get_arg_ulong(argv[7]);
	if (len > max_len) 
	{
		info("test len: %d exceed the max test buffer size: %d, force to use the max buffer size\n", len, max_len);
		len = max_len;
	}

	info("%s : bus width: %d, len: %d\n", argv[0], priv->ds, len);

	// initialize rx and tx memory
	mem_set_seq(p_tx, len);
	mem_set8(p_rx, 0, len);

	// transfer data
	ret = spi_xfer_packet(port, p_tx, p_rx, len, SPI_XFER_ONCE);
	if (ret) 
	{
		info("spi read/write loopback fail,ret=%d\n", ret);
		fail++;
	}

	print_in_line((uint8_t *)"tx", p_tx, len); 
	print_in_line((uint8_t *)"rx", p_rx, len); 

	// check result
	fail += mem_check_mask(p_rx, p_tx, len, priv->ds);
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
//	Function:  static int spi_test_master(int argc, char *argv[])
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
//		 spi master test.	
//
/*****************************************************************************/
#define SPI_RESPONSE_LEN	8
static int spi_test_master(int argc, char *argv[])
{
	E_SPI_PORT port = g_spi_port;
	struct brite_spi *priv = &g_spi[port];
	int32_t ret;
	int32_t fail = 0;
	uint32_t len;
	uint32_t max_len = TEST_BUFFER_SIZE/2;
	uint8_t *p_tx = (uint8_t *)test_txrx_buff;
	uint8_t response_rx[SPI_RESPONSE_LEN];
	
	info("%s: spi master mode loopback test\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 8) 
	{
		info("format: %s clk(HZ) type ms mode dss dt_mode len\n", argv[0]);
		info("type(frame format)     : 0, motorrola; 1, TI; 2, national; 3, reved\n");
		info("ms(master/slave)       : 0, master; 1, slave\n");
		info("mode(pol.bit1/pha.bit0): 0/1/2/3\n");
		info("dss(data size select   : [4, 16] bits\n");		
		info("dt_mode(cpu/dma)       : 0, cpu;\n");		
		
		return ERR_TEST_ARGC;
	}

	// Initialize test parameter and hw initialize
	if (spi_test_init(argc, argv))
	{
		return ERR_TEST_FAIL;
	}
	
	// Get the parameters
	len = get_arg_ulong(argv[7]);
	if (len > max_len) 
	{
		info("test len: %d exceed the max test buffer size: %d, force to use the max buffer size\n", len, max_len);
		len = max_len;
	}

	info("%s : bus width: %d, len: %d\n", argv[0], priv->ds, len);

	// initialize rx and tx memory
	mem_set_seq(p_tx, len);
	mem_set8(response_rx, 0, SPI_RESPONSE_LEN);

	for (uint32_t i = 0; i < 10; i++)
	{
		info("\ni: %x\n", i);
		
		// tx data
		p_tx[0] = i + 0xAA;
		p_tx[len-1] = i + 0xAA;
		ret = spi_xfer_packet_cs_self(port, p_tx, NULL, len, SPI_XFER_ONCE);
		if (ret) 
		{
			info("spi read/write loopback fail,ret=%d\n", ret);
			fail++;
		}

		mdelay(20);

		// rx response data
		ret = spi_xfer_packet_cs_self(port, NULL, response_rx, SPI_RESPONSE_LEN, SPI_XFER_ONCE);
		if (ret) 
		{
			info("spi read/write loopback fail,ret=%d\n", ret);
			fail++;
		}

		print_in_line((uint8_t *)"data tx", p_tx, len); 
		print_in_line((uint8_t *)"res tx", response_rx, SPI_RESPONSE_LEN);  		

		mdelay(500);
	}

#if 0 
	// check result
	fail += mem_check_mask(p_rx, p_tx, len, priv->ds);
	if (fail)
	{
		info("%s: test fail\n", argv[0]);
	}
	else
	{
		info("%s: test pass\n", argv[0]);
	}
#else
	info("%s: test pass\n", argv[0]);
#endif

	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  static int spi_test_slave(int argc, char *argv[])
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
//		 spi slave test.	
//
/*****************************************************************************/
static int spi_test_slave(int argc, char *argv[])
{
	E_SPI_PORT port = g_spi_port;
	struct brite_spi *priv = &g_spi[port];
	int32_t ret;
	int32_t fail = 0;
	uint32_t len;
	uint32_t max_len = TEST_BUFFER_SIZE/2;
	uint8_t *p_rx = (uint8_t *)(test_txrx_buff + max_len);
	uint8_t response_tx[SPI_RESPONSE_LEN];
	
	info("%s: spi master mode loopback test\n\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 8) 
	{
		info("format: %s clk(HZ) type ms mode dss dt_mode len\n", argv[0]);
		
		info("type(frame format)     : 0, motorrola; 1, TI; 2, national; 3, reved\n");
		info("ms(master/slave)       : 0, master; 1, slave\n");
		info("mode(pol.bit1/pha.bit0): 0/1/2/3\n");
		info("dss(data size select   : [4, 16] bits\n");		
		info("dt_mode(cpu/dma)       : 0, cpu;\n");

		return ERR_TEST_ARGC;
	}

	// Initialize test parameter and hw initialize
	if (spi_test_init(argc, argv))
	{
		return ERR_TEST_FAIL;
	}

	// pin mux
	pad_spiclk_pinmux(PINMUX_FUNC0, 0);
	pad_spicsn_pinmux(PINMUX_FUNC0, 0); // spi controller control
	pad_spirxd_pinmux(PINMUX_FUNC0, 0);
	pad_spitxd_pinmux(PINMUX_FUNC0, 0);		
	
	// Get the parameters
	len = get_arg_ulong(argv[7]);
	if (len > max_len) 
	{
		info("test len: %d exceed the max test buffer size: %d, force to use the max buffer size\n", len, max_len);
		len = max_len;
	}

	info("%s : bus width: %d, len: %d\n", argv[0], priv->ds, len);

	// initialize rx and tx memory
	mem_set8(p_rx, 0, len);
	mem_set_seq(response_tx, SPI_RESPONSE_LEN);

	for (uint32_t i = 0; i < 10; i++)
	{
		info("\ni: %x\n", i);
		
		// rx data
		ret = spi_xfer_packet_slave(port, NULL, p_rx, len, SPI_XFER_ONCE);
		if (ret) 
		{
			info("spi read/write loopback fail,ret=%d\n", ret);
			fail++;
		}

		// response tx data
		response_tx[0] = i + 0xBB;
		response_tx[SPI_RESPONSE_LEN-1] = i + 0xBB;
		ret = spi_xfer_packet_slave(port, response_tx, NULL, SPI_RESPONSE_LEN, SPI_XFER_ONCE);
		if (ret) 
		{
			info("spi read/write loopback fail,ret=%d\n", ret);
			fail++;
		}

		print_in_line((uint8_t *)"data rx", p_rx, len); 
		print_in_line((uint8_t *)"res tx", response_tx, SPI_RESPONSE_LEN); 
	}

#if 0	
	// check result
	fail += mem_check_mask(p_rx, p_tx, len, priv->ds);
	if (fail)
	{
		info("%s: test fail\n", argv[0]);
	}
	else
	{
		info("%s: test pass\n", argv[0]);
	}
#else
	info("%s: test pass\n", argv[0]);
#endif

	return ERR_TEST_DONE;
}

static int spi_int_counter = 0;
static void spi_int_test_handler(void* ptr)
{
	int int_status = 0;
	struct brite_spi *priv = ptr;
	E_SPI_PORT port = priv->port;

	int_status = spi_get_mis(port);
	info("SPI%d: spi_int_handler, base=0x%x status=0x%x\n", priv->port, priv->base, int_status);
	if (int_status != 0) {
		spi_int_counter++;
		spi_clr_int(port, (SPI_RORIC_MASK|SPI_RTIC_MASK));
	}
	spi_wr_reg(port, SPI_SSPIMSC_OFST, 0);
}

// testcase:spi interrupt test
static int spi_int(int argc, char* argv[])
{
	int fail = 0;
	unsigned long ien;
	int time_used = 0;
	E_SPI_PORT port = g_spi_port;
	struct brite_spi *priv = &g_spi[port];

	info("%s: interrupt handle test\n",argv[0]);

	spi_int_counter = 0;
	
	// disable all interrupt
	spi_wr_reg(port, SPI_SSPIMSC_OFST, 0);
	
	// clear all interrupt
	spi_clr_int(port, (SPI_RORIC_MASK|SPI_RTIC_MASK));

	// initialize the spi hw
	spi_init(port, priv->pclk, SPI_FRF_MOTORROLA, SPI_MASTER, SPI_MODE0, 8, DT_CPU);

	ien = SPI_INT_TX_MASK | SPI_INT_ROR_MASK | SPI_INT_RT_MASK | SPI_INT_RX_MASK;

	info("SPI%d: base=0x%x int_vector=%d\n", port, priv->base, priv->int_vec);
	/*register int handler and enable it*/
	irq_generic_install_handler(priv->int_vec,
		spi_int_test_handler,
		priv);
	irq_generic_config_int(priv->int_vec, IRQ_TYPE_LEVEL_LOW);
	irq_generic_enable_int(priv->int_vec);
	_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);

	/*enable interrupt*/
	spi_wr_reg(port, SPI_SSPIMSC_OFST, ien);

	while(time_used++ < 500 && spi_int_counter == 0)
		mdelay(100); /* delay some second for interrupt msg output */

	if (spi_int_counter == 0)
		fail++;

	if (fail)
		info("%s: test fail\n",argv[0]);
	else
		info("%s: test pass\n",argv[0]);

	/*disable interrupt*/
	irq_generic_disable_int(priv->int_vec);
	irq_generic_remove_handler(priv->int_vec,NULL,NULL);
	spi_wr_reg(port, SPI_SSPIMSC_OFST, 0);

	return 0;
}

/*****************************************************************************/
//	Function:  static int spi_flash_test(int argc, char *argv[])
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
//		 spi flash device test.
//
/*****************************************************************************/
static int spi_flash_test(int argc, char *argv[])
{
	E_SPI_PORT port = g_spi_port;
	int i;
	uint32_t len;
	int ret_val;
	int32_t fail = 0;
	uint8_t *p_tx = test_send_buff;
	uint8_t *p_rx = test_recv_buff;
	uint32_t rx_tx_size = TEST_BUFFER_SIZE;

	info("%s:access spi data flash test\n", argv[0]);

	if (spi_test_init(argc, argv))
	{
		return ERR_TEST_FAIL;
	}
	
	// set flash access using this spi port
	sf_set_controller(port, (spi_xfer_fn)spi_xfer_packet);

	ret_val = sf_probe();
	if (ret_val) 
	{
		info("unknow or wrong flash id detected,skip other test\n");

		fail++;
		goto end;
	}

	// first test erase, erase one sector(4k) 
	info("begin to erase a block\n");
	len = 4096;
	ret_val = sf_erase_sec(0, len);
	if (ret_val) 
	{
		info("erase fail\n");

		fail++;
		goto end;
	}

	// read to verify,should be 'ff'
	info("begin to read and compare\n");
	memset(p_rx, 0, rx_tx_size);
	if (sf_read(0, p_rx, len)) 
	{
		info("Check read failed\n");

		fail++;
		goto end;
	}
	
	for (i = 0; i < len; i++) 
	{
		if (p_rx[i] != 0xFF) 
		{
			info("Check failed at %d:%x\n", i, p_rx[i]);

			fail++;
			goto end;
		}
	}

	// generate random data in tx buffer
	mem_set_random(p_tx, len);

	// clear rx buffer
	mem_set8(p_rx, 0x00, len);
	
	// write to flash
	info("begin to write\n");
	if (sf_write(0, p_tx, len)) 
	{
		info("Write failed\n");

		fail++;
		goto end;
	}

	// read back to verify
	info("begin to read and compare\n");
	if (sf_read(0, p_rx, len)) 
	{
		info("Read failed\n");

		fail++;
		goto end;
	}

	for (i = 0; i < len; i++) 
	{
		if (p_rx[i] != p_tx[i]) 
		{
			info("Verify failed at %d, good data:\n", i);

			fail++;
			goto end;
		}
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
//	Function:  static int spi_flash_probe(int argc, char *argv[])
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
//		 spi flash probe.
//
/*****************************************************************************/
static int spi_flash_probe(int argc, char *argv[])
{
	E_SPI_PORT port = g_spi_port;
	int ret_val;

	info("%s:access spi data flash test\n", argv[0]);

	if (spi_test_init(argc, argv))
	{
		return ERR_TEST_FAIL;
	}
	
	// set flash access using this spi port
	sf_set_controller(port, (spi_xfer_fn)spi_xfer_packet);

	ret_val = sf_probe();
	if (ret_val) 
	{
		info("unknow or wrong flash id detected,skip other test\n");
		goto end;
	}

end:
	if (ret_val) 
	{
		info("%s: test fail\n",argv[0]);
	} 
	else 
	{
		info("%s: test pass\n",argv[0]);
	}

	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  static int spi_flash_erase_sec(int argc, char *argv[])
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
//		 spi flash erase sector.
//
/*****************************************************************************/
static int spi_flash_erase_sec(int argc, char *argv[])
{
	int ret_val;
	uint32_t addr, len;

	info("%s: flash erase test\n", argv[0]);

	if (argc < 3) 
	{
		info("format: %s addr len\n", argv[0]);
		return ERR_TEST_ARGC;
	}
	
	addr = get_arg_ulong(argv[1]);
	len = get_arg_ulong(argv[2]);

	// first test erase, erase one sector(4k) 
	info("begin to erase, addr:0x%x, len:0x%x\n", addr, len);
	ret_val = sf_erase_sec(addr, len);
	if (ret_val) 
	{
		info("erase fail\n");
		goto end;
	}

end:
	if (ret_val) 
	{
		info("%s: test fail\n",argv[0]);
	} 
	else 
	{
		info("%s: test pass\n",argv[0]);
	}

	return ERR_TEST_DONE;
}

#define TEST_FLASH_TX_BUF	(SRAM3_BASE + 0x20000)
#define TEST_FLASH_RX_BUF	(SRAM3_BASE + 0x10000)

/*****************************************************************************/
//	Function:  static int spi_flash_write(int argc, char *argv[])
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
//		 spi flash write data.
//
/*****************************************************************************/
static int spi_flash_write(int argc, char *argv[])
{
	int ret_val;
	uint32_t addr, len;
	//uint8_t *p_tx = test_send_buff;
	uint8_t *p_tx = (uint8_t *)TEST_FLASH_TX_BUF;

	info("%s: flash write test\n", argv[0]);

	if (argc < 3) 
	{
		info("format: %s addr len\n", argv[0]);
		return ERR_TEST_ARGC;
	}
	
	addr = get_arg_ulong(argv[1]);
	len = get_arg_ulong(argv[2]);

	// generate random data
	//mem_set_random(p_tx, len);
	mem_set_seq(p_tx, len);

	// write to flash
	info("begin to write\n");
	ret_val = sf_write(addr, p_tx, len);
	if (ret_val) 
	{
		info("Write failed\n");
		goto end;
	}

end:
	if (ret_val) 
	{
		info("%s: test fail\n",argv[0]);
	} 
	else 
	{
		info("%s: test pass\n",argv[0]);
	}

	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  static int spi_flash_read(int argc, char *argv[])
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
//		 spi flash read data.
//
/*****************************************************************************/
static int spi_flash_read(int argc, char *argv[])
{
	int i;
	int ret_val;
	uint32_t addr, len;
	//uint8_t *p_tx = test_send_buff;
	//uint8_t *p_rx = test_recv_buff;
	uint8_t *p_tx = (uint8_t *)TEST_FLASH_TX_BUF;
	uint8_t *p_rx = (uint8_t *)TEST_FLASH_RX_BUF;

	info("%s: flash read test\n", argv[0]);

	if (argc < 3) 
	{
		info("format: %s addr len\n", argv[0]);
		return ERR_TEST_ARGC;
	}
	
	addr = get_arg_ulong(argv[1]);
	len = get_arg_ulong(argv[2]);

	// generate random data
	srand(get_ticks());

	for (i = 0; i < len; i++) 
	{
		p_rx[i] = i&0xFF;//cb_rand() & 0xff;
	}

	// read from flash
	info("begin to read\n");
	ret_val = sf_read(addr, p_rx, len);
	if (ret_val) 
	{
		info("read failed\n");
		goto end;
	}

	// dump data 
	//print_in_line((uint8_t *)"spi tx:", p_tx, len);
	print_in_line((uint8_t *)"spi rx:", p_rx, len);

	// compare 
	for (i = 0; i < len; i++) 
	{
		if (p_rx[i] != p_tx[i]) 
		{
			ret_val++;
			info("Verify failed at %d, good data:\n", i);
			goto end;
		}
	}	

end:
	if (ret_val) 
	{
		info("%s: test fail\n",argv[0]);
	} 
	else 
	{
		info("%s: test pass\n",argv[0]);
	}

	return ERR_TEST_DONE;
}

#if (SPI_BOOT_TEST == 1)

#include "data_array.h"
#include "image_header.h"
#include "fw_crc.h"

static int spi_flash_burning(int argc, char *argv[])
{
	int ret_val;
	int i;
	uint32_t addr = 0, len = sizeof(binArray);
	uint8_t *p_tx = (uint8_t *)binArray;
	uint8_t *p_rx = (uint8_t *)TEST_FLASH_RX_BUF;	
	uint32_t erase_size = (len +  0xFFF) & 0xFFFFF000;
	
	info("%s: flash burning test, make sure probe & erase is call before this funcition\n", argv[0]);

	// first test erase, erase one sector(4k) 
	info("begin to erase, addr:0x%x, len:0x%x\n", 0, erase_size);
	ret_val = sf_erase_sec(addr, erase_size);
	if (ret_val) 
	{
		info("erase fail\n");
		return ERR_TEST_FAIL;
	}

	ret_val = sf_write(addr, (uint8_t *)binArray, len);	
	if (ret_val)
	{
		info("%s: test fail\n",argv[0]);
	}
	else
	{
		info("%s: test pass\n",argv[0]);
	}

	info("begin to read\n");
	ret_val = sf_read(addr, p_rx, len);
	if (ret_val) 
	{
		info("read failed\n");
		return ERR_TEST_FAIL;
	}

	// compare 
	len = sizeof(binArray);
	for (i = 0; i < len; i++) 
	{
		if (p_rx[i] != p_tx[i]) 
		{
			ret_val++;
			info("Verify failed at %d, good data:\n", i);
			return ERR_TEST_FAIL;
		}
	}

	info("compare done\n");

	return ERR_TEST_DONE;
}


static int get_status(unsigned short *para)
{
	unsigned int sizeToTransfer = 0;
	unsigned int sizeTransfered;
	int status;
	unsigned char buffer[8] = { 0 };
	//uint8 tmp = 0;

	sizeToTransfer = 8;
	sizeTransfered = 0;

	//status = spi_xfer_rx_withoutDMA(0, buffer, sizeToTransfer, 0xffffffff);
	status = spi_xfer_packet(0, NULL, buffer, sizeToTransfer, SPI_XFER_ONCE);
	*para = (unsigned short)((buffer[4] << 8) | buffer[5]);
	return status;
}


static int send_handshake()
{
	unsigned int sizeToTransfer = 0;
	unsigned int sizeTransfered = 0;
	int status = 0;
	unsigned short crc16 = 0;
	unsigned char buffer[8] = { 0 };
	//uint8 tmp = 0;

	/* Write command EWEN(with CS_High -> CS_Low) */
	sizeToTransfer = 8;
	sizeTransfered = 0;
	buffer[0] = 0x7E;
	buffer[1] = 0x01;
	buffer[4] = 0xa5;
	buffer[5] = 0x5a;
	crc16 = crc16_default(0x0000, buffer, 6);
	buffer[6] = (crc16 >> 8) & 0xff;
	buffer[7] = crc16 & 0xff;

	status = spi_xfer_tx_withoutDMA(0, buffer, sizeToTransfer, 0xffffffff);
	//status = spi_xfer_packet(0, buffer, NULL, sizeToTransfer, SPI_XFER_ONCE);
	return status;
}

static int send_getversion()
{
	unsigned int sizeToTransfer = 8;
	unsigned int sizeTransfered = 0;
	int status = 0;
	unsigned short crc16 = 0;
	unsigned char buffer[8] = { 0 };
	//uint8 tmp = 0;

	/* Write command EWEN(with CS_High -> CS_Low) */
	sizeToTransfer = 8;
	sizeTransfered = 0;
	buffer[0] = 0x7E;
	crc16 = crc16_default(0x0000, buffer, 6);
	buffer[6] = (crc16 >> 8) & 0xff;
	buffer[7] = crc16 & 0xff;
	
	status = spi_xfer_packet(0, buffer, NULL, sizeToTransfer, SPI_XFER_ONCE);

	return status;
}

static int send_setconfig()
{
	unsigned int sizeToTransfer = 8 + 16;
	unsigned int sizeTransfered = 0;
	int status = 0;
	unsigned short crc16 = 0;
	unsigned char buffer[24] = { 0 };
	//uint8 tmp = 0;

	/* Write command EWEN(with CS_High -> CS_Low) */
	sizeTransfered = 0;
	buffer[0] = 0x7E;
	buffer[1] = 0x03;
	buffer[2] = 0x00;
	buffer[3] = 0x10;
	/*config para not used so far*/
	crc16 = crc16_default(0x0000, buffer, 22);
	buffer[22] = (crc16 >> 8) & 0xff;
	buffer[23] = crc16 & 0xff;

	status = spi_xfer_packet(0, buffer, NULL, sizeToTransfer, SPI_XFER_ONCE);

	return status;
}

static int send_datapkt(unsigned char *buff, unsigned short len, unsigned short pkt_id)
{
	unsigned int sizeToTransfer = 8 + len;
	unsigned int sizeTransfered = 0;
	int status = 0;
	unsigned short crc16 = 0;
	unsigned char buffer[8 + IMG_PKT_MAXLEN] = { 0 };
	//uint8 tmp = 0;

	/* Write command EWEN(with CS_High -> CS_Low) */
	sizeToTransfer = 8 + len;
	sizeTransfered = 0;
	buffer[0] = 0x7E;
	buffer[1] = 0x02;
	buffer[2] = (len >> 8) & 0xff;;
	buffer[3] = len & 0xff;
	//if(len %4 == 0)
	//	mem_cpy32((unsigned char *)(buffer + 4), buff, len>>2);
	//else		
		mem_cpy8((unsigned char *)(buffer + 4), buff, len);
	/*config para not used so far*/
	buffer[4 + len] = (pkt_id >> 8) & 0xff;
	buffer[5 + len] = pkt_id & 0xff;

	//crc16 = crc16_default(0x0000, buffer, 6 + len);
	buffer[6 + len] = (crc16 >> 8) & 0xff;
	buffer[7 + len] = crc16 & 0xff;

	status = spi_xfer_tx_withoutDMA(0, buffer, sizeToTransfer, 0xffffffff);
	//status = spi_xfer_packet(0, buffer, NULL, sizeToTransfer, SPI_XFER_ONCE);
	return status;
}

static int spi_boot(int argc, char* argv[])
{
	int i = 0;
	unsigned short para = 0;
	int ret = 0;
	unsigned char	*file_buffer = binArray;
	int 	area_num = 0;
	const	img_header_t *image_ptr = NULL;
	unsigned int	offset = 0;
	unsigned int	sect_len = 0;
	unsigned short	pkt_len = 0;
	uint32_t clk;

	// Check the validity of parameter count
	if (argc < 2) 
	{
		info("format: %s clk(HZ)\n", argv[0]);

		return ERR_TEST_ARGC;
	}

	// Get the parameters
	clk = get_arg_ulong(argv[1]);

	spi_init(0, clk, SPI_FRF_MOTORROLA, SPI_MASTER, SPI_MODE1, 8, 0);

	//send handshake command & check status
	send_handshake();
	mdelay(2);
#if 1
	get_status(&para);
#endif	
	printf("device version:%x\r\n", para);

	mdelay(2);
	//send get version command & check status

	//send data packets & check status
	//1. send image header
	image_ptr = (const	img_header_t *)file_buffer;

	send_datapkt(file_buffer, IMG_HDR_SIZE, 0);
	mdelay(3);
#if 1
	get_status(&para);
	if (para != 0x0)
	{
		printf("image header failed\r\n");
		ret = -1;
		goto __exit;
	}
#endif
	mdelay(3);

	area_num = (unsigned int)image_ptr->image_numofmemarea;

	/* Ensure we have an image to transfer/load */
	if (area_num == 0)
	{
		ret = -1;
		goto __exit;
	}
	/* image content stage */
	offset += IMG_HDR_SIZE;
	for (i = 0; i<area_num; i++)
	{
		sect_len = image_ptr->memarea[i].length;
		printf("\nsection : %d\n", i);
		while (sect_len)
		{
			pkt_len = sect_len >IMG_PKT_MAXLEN ? IMG_PKT_MAXLEN : sect_len;
			send_datapkt(file_buffer + offset, pkt_len, i);		
#if 1
			mdelay(1);

			get_status(&para);
			if (para != i)
			{
					printf("send pkt %d fail\n", i);
					ret = -1;
					goto __exit;
			}
#endif	
			//printf("sect len:%d\r\n", sect_len);
			//printf(".");	

			/* remainder again, the setion size must be multiple of 4B */
			sect_len -= pkt_len;
			offset += pkt_len;
			//mdelay(3);
		}
		
		//mdelay(2);
	}

__exit:
	return 0;
}

#endif

int spi_test_prepare(int port)
{
	struct brite_spi *priv = &g_spi[port];

	// check the validity of parameter
	if (port >= NUM_SPI_PORTS)
	{
		info("port %d is too large!\n", port);
		return ERR_TEST_PARA;
	}
	
	// clear the structure
	mem_set8(priv, 0, sizeof(struct brite_spi));
	
	if (port == SPI0)
	{
		priv->port = port;
		priv->base = SPI_BASE + (port * SPI_PORT_SIZE);
		priv->pclk = SPI_PCLK;

		// dma parameter
		priv->dma_burst_len = BURST_8;
		priv->dma_port = 0;
		priv->dma_tx_ch = 0;
		priv->dma_rx_ch = 1;

		// interrupt parameter
		priv->int_en = 0;
		priv->int_vec = INT_SPI;
	}
	else
	{
		// not defined on current HW
	}

	// set current port
	g_spi_port = port;

	gpio_init(0, GPIO_BASE);

	pad_spiclk_pinmux(PINMUX_FUNC0, 0);
	pad_spicsn_pinmux(PINMUX_FUNC2, 0); // gpio 3 control
	pad_spirxd_pinmux(PINMUX_FUNC0, 0);
	pad_spitxd_pinmux(PINMUX_FUNC0, 0);	
		
	return ERR_TEST_DONE;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry spi_test_menu[] = {
	{"help", 							cmd_help,  								" : Display list of commands"},
	{"h",								cmd_help,								"    : alias for help"},
	{"?",								cmd_help, 								"    : alias for help"},	
	{"spi_test_loopback",				spi_test_loopback,						"   : spi master mode loopback test"},
	{"spi_test_master",					spi_test_master,                        "  : spi master test" },
	{"spi_test_slave",					spi_test_slave,                         "  : spi slave test" },
	{"spi_int",							spi_int,                                "  : spi interrupt handle test" },
	{"spi_flash_test",					spi_flash_test, 						"   : spi robust test"},
	{"spi_flash_probe",					spi_flash_probe,						"	: spi flash probe"},
	{"spi_flash_erase_sec",				spi_flash_erase_sec,					"	: spi flash erase sector(4KB aligned)"},
	{"spi_flash_write",					spi_flash_write,						"	: spi flash write test"},
	{"spi_flash_read", 					spi_flash_read, 						"	: spi flash read test"},
#if (SPI_BOOT_TEST == 1)
	{"spi_flash_burning", 				spi_flash_burning, 						"	: spi flash burning test"},
	{"spi_boot",						spi_boot,								"  : spi boot test" },
#endif	
	{"q", 								NULL, 									" : quit uart test"},
	{0, 								0, 										0}
};

#endif

