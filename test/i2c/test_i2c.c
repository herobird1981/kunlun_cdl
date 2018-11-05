/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                               											 		 */
/*           Copyright (C) 2016 Brite Semiconductor Co., Ltd. All rights reserved.                									 */
/*                                                                                                													 */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_i2c.h"
#include "cdl_pll.h"

#if (CONFIG_I2C_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry i2c_test_menu[];

/*
store i2c slave data: each reg offset can access the coresponding value
continus access is permit
*/
static uint8_t i2c_slave_buf[2048] __attribute__((aligned(64))) = {0};

static int g_i2c_int_cnt[NUM_I2C_PORTS] = {0, 0};

static uint32_t g_i2c_port, g_i2c_slave_port;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/
void i2c_master_int_handler(void *ptr);

void i2c_slave_int_handler(void *ptr);

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
static int cmd_help(int argc, char *argv[])
{
	return cmdline_help_general(&i2c_test_menu[0]);
}

static int i2c_slave(int argc, char* argv[])
{
	uint32_t dev, addr_mode;
	E_I2C_PORT port = g_i2c_port;
	struct dw_i2c *priv = p_i2c_dev[port];

	// get the slave port for interrupt
	g_i2c_slave_port = port;
	//prcm_reset(ARM_I2C0_RST + port);

	info("i2c_slave:i2c slave mode test\n");

	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("wrong cmd_format: i2c_slave dev addr_mode\n");
		return 0;
	}

	// Get the parameters
	dev = get_arg_ulong(argv[1]);
	addr_mode = (!strcmp(argv[2], "10b")) ? I2C_10BIT_ADDR : I2C_7BIT_ADDR;

	info("input cmd_format: i2c_slave dev(0x%x) addr_mode(%s)\n", dev, argv[2]);

	// Set pinmux to i2c
	info("set pinmux to i2c\n");
#if 0
	if (!misc_i2c_pin_init(port))
	{
		info("set pinmux to i2c fail\n");
		return 0;	
	}
#endif

	// Set i2c to slave mode
	mem_set8(i2c_slave_buf, 0, sizeof(i2c_slave_buf));
	i2c_init_slave(priv, dev, addr_mode, 4, sizeof(i2c_slave_buf));

	// Register int handler and enable it
	irq_generic_install_handler(priv->int_vector, i2c_slave_int_handler, NULL);
	irq_generic_config_int(priv->int_vector, IRQ_TYPE_LEVEL_HIGH);
	irq_generic_enable_int(priv->int_vector);

	// Enable interrupt
	i2c_set_int_mask(priv, (I2C_INT_ST_RX_FULL_MASK|I2C_INT_ST_RD_REQ_MASK|I2C_INT_ST_STOP_DET_MASK));	

	info("i2c_slave: setup terminate\n");

	return 0;
}

// testcase: basic master mode write/read test
static int i2c_master(int argc, char *argv[])
{
	uint32_t dma_mode, speed, dev, addr_mode, len;
	int32_t ret_val;
	uint32_t fail = 0;
	uint32_t i, pattern, random_flag = 1;
	E_I2C_PORT port = g_i2c_port;
	struct dw_i2c *priv = p_i2c_dev[port];

	//prcm_reset(ARM_I2C0_RST + port);

	info("i2c_master: master mode basic write/read test\n");

	// Check the validity of parameter count
	if (argc < 6) 
	{
		info("wrong cmd_format: i2c_master dma_mode speed dev addr_mode len [pattern]\n");
		return 0;
	}

	if (argc >= 7)
	{
		pattern = get_arg_ulong(argv[6]);
		random_flag = 0;
	}

	// Get the parameters
	dma_mode = get_arg_ulong(argv[1]) ? 1 : 0;	
	speed = get_arg_ulong(argv[2]);
	dev = get_arg_ulong(argv[3]);	
	addr_mode = (!strcmp(argv[4], "10b")) ? I2C_10BIT_ADDR : I2C_7BIT_ADDR;
	len = get_arg_ulong(argv[5]);

	info("input cmd_format: i2c_master dma_mode(%d) speed(%d) dev(0x%x) addr_mode(%s) len(%d)\n",
			dma_mode, speed, dev, argv[4], len);

	if(len>4095)
	{
		info("len = %d > 4095, too long\r\n", len);
		return 0;
	}
	// Set pinmux to i2c
	info("set pinmux to i2c\n");
#if 0
	if (!misc_i2c_pin_init(port))
	{
		return 0;	
	}
#endif

	// Set i2c to master mode
	i2c_init(priv, (uint32_t)priv->regs, priv->pclk, dma_mode);
	i2c_init_master(priv, speed, addr_mode);

	// Generate random/pattern data
	srand(get_ticks());
	if (random_flag)
	{
		for (i = 0; i < len; i++)
		{
			test_send_buff[i] = cb_rand() & 0xFF;
		}
	}
	else
	{
		for (i = 0; i < len; i++)
		{
			test_send_buff[i] = pattern & 0xFF;
		}
	}

	// Clear receive buffer
	mem_set8(test_recv_buff, 0, sizeof(test_recv_buff));

	// i2c write data 
	info("i2c write data, len: 0x%x\n", len);
	ret_val = i2c_write(priv, dev, 0, 0, test_send_buff, len);
	if (ret_val) 
	{
		debug("i2c write data fail, ret=%d\n", ret_val);
		fail++;
		goto end;
	}

	// i2c read data
	info("i2c read data, len: 0x%x\n", len);
	ret_val = i2c_read(priv, dev, 0, 0, test_recv_buff, len);
	if (ret_val) 
	{
		debug("i2c read data fail, ret=%d\n", ret_val);
		fail++;
		goto end;
	}

	// Check result
	info("check data, len: 0x%x\n", len);
	for (i = 0; i < len; i++) 
	{
		//debug("i2c check data @%d: expected %x receive %x\n", i, test_send_buff[i], test_recv_buff[i]);
		
		if (test_send_buff[i] != test_recv_buff[i]) 
		{
			debug("i2c check data fail @%d: expected %x receive %x\n", i, test_send_buff[i], test_recv_buff[i]);
			fail++;
			break;
		}
	}

end:
	if (fail) 
	{			    
		info("i2c_master: test fail, abrt_sts = 0x%x\n", i2c_get_tx_abrt_source(priv));
		//dw_i2c_tx_abort_recovery(priv);
		info("!!!this failure should affect next time's transfer if abrt_sts is not zero and dma is used!!!\n"); 
	} 
	else 
	{
		info("i2c_master: test pass\n");
	}

	// disable interrupt(master)
	irq_generic_disable_int(priv->int_vector);
	irq_generic_remove_handler(priv->int_vector, NULL, NULL);
	i2c_set_int_mask(priv, 0);

	// disable interrupt(slave)
	priv = p_i2c_dev[g_i2c_slave_port];
	irq_generic_disable_int(priv->int_vector);
	irq_generic_remove_handler(priv->int_vector, NULL, NULL);
	i2c_set_int_mask(priv, 0);

	return 0;
}

// testcase: basic loopback test
static int i2c_test_loopback(int argc, char *argv[])
{
	uint32_t dma_mode, addr_mode, speed, len;
	uint32_t dev;
	int32_t ret_val;
	uint32_t fail = 0;
	uint32_t i, pattern, random_flag = 1;
	struct dw_i2c *master = p_i2c_dev[I2C1];
	struct dw_i2c *slave = p_i2c_dev[I2C0];
	uint8_t *p_tx = (uint8_t *)test_txrx_buff;
	uint8_t *p_rx = (uint8_t *)(test_txrx_buff + (TEST_BUFFER_SIZE/2));		
	uint32_t max_len = TEST_BUFFER_SIZE/2;

	info("i2c_test_loopback: i2c master and slave basic write/read test\n");

	// Check the validity of parameter count
	if (argc < 6) 
	{
		debug("wrong cmd_format: %s speed dma_mode addr_mode slave_dev len [pattern]\n", argv[0]);
		debug("dma_mode: 0, cpu; 1, dma\n");
		debug("addr_mode: 7b/10b");
		return 0;
	}

	if (argc >= 7)
	{
		pattern = get_arg_ulong(argv[6]);
		random_flag = 0;
	}

	// Get the master parameters
	speed = get_arg_ulong(argv[1]);
	dma_mode = get_arg_ulong(argv[2]) ? 1 : 0;	
	addr_mode = (!strcmp(argv[3], "10b")) ? I2C_10BIT_ADDR : I2C_7BIT_ADDR;

	// Get the slave parameters
	dev = get_arg_ulong(argv[4]);
	len = get_arg_ulong(argv[5]);

	info("input cmd_format: i2c_test_loopback speed(%d) dma_mode(%d) addr_mode(%d) slave_dev(0x%x) len(%d)\n",
		speed, dma_mode, addr_mode, dev, len);

	if(len > max_len)
	{
		info("len = %d > 2048, too long\r\n", len);
		return 0;
	}
	
	// Set pinmux to i2c
	i2c_test_prepare(I2C0);
	i2c_test_prepare(I2C1);

	// Set i2c to master mode
	i2c_init(master, (uint32_t)master->regs, master->pclk, dma_mode);
	i2c_init_master(master, speed, addr_mode);

	// Set i2c to slave mode
	mem_set8(i2c_slave_buf, 0, sizeof(i2c_slave_buf));
	i2c_init_slave(slave, dev, addr_mode, 4, sizeof(i2c_slave_buf));

	// Generate random/pattern data
	srand(get_ticks());
	if (random_flag)
	{
		mem_set_seq(p_tx, len);		
	}
	else
	{
		for (i = 0; i < len; i++)
		{
			p_tx[i] = pattern & 0xFF;
		}
	}

	// Clear receive buffer
	mem_set8(p_rx, 0, len);

	//print_in_line((uint8_t *)"tx", (uint8_t *)p_tx, len);
	//print_in_line((uint8_t *)"rx", (uint8_t *)p_rx, len);	

#if 1
	// i2c write data 
	info("i2c master write data, len: 0x%x\n", len);
	ret_val = i2c_master_tx_slave_rx(master, slave, dev, 0x05, 0, p_tx, i2c_slave_buf, len);
	if (ret_val) 
	{
		debug("i2c master write data fail, ret=%d\n", ret_val);
		fail++;
		goto end;
	}

	//print_in_line((uint8_t *)"tx", (uint8_t *)p_tx, len);
	//print_in_line((uint8_t *)"rx", (uint8_t *)i2c_slave_buf, len);

	//return 0;
#endif

	//mem_set_seq(i2c_slave_buf, len);	

	// i2c read data
	info("i2c master read data, len: 0x%x\n", len);
	ret_val = i2c_master_rx_slave_tx(master, slave, dev, 0x05, 0, p_rx, i2c_slave_buf, len);
	if (ret_val) 
	{
		debug("i2c read data fail, ret=%d\n", ret_val);
		fail++;
		goto end;
	}

	//print_in_line((uint8_t *)"tx", (uint8_t *)i2c_slave_buf, len);
	//print_in_line((uint8_t *)"rx", (uint8_t *)p_rx, len);

	//return 0;
	
	// Check result
	info("check data, len: 0x%x\n", len);
	for (i = 0; i < len; i++) 
	{
		if (p_tx[i] != p_rx[i]) 
		{
			debug("i2c check data fail @%d: expected %x receive %x\n", i, p_tx[i], p_rx[i]);
			fail++;
			break;
		}
	}

end:
	if (fail)
	{
		print_hex("tx", 1, p_tx, len);
		print_hex("rx", 1, p_rx, len);
		info("i2c_master: test fail, abrt_sts = 0x%x\n", i2c_get_tx_abrt_source(master));
	} 
	else 
	{
		info("i2c_master: test pass\n");
	}
	
	return 0;
}

void i2c_master_int_test_ISR(void *ptr)
{
	uint32_t int_sts = 0;
	struct dw_i2c *priv = ptr;
	E_I2C_PORT port = priv->port;

	int_sts = i2c_get_int_status(priv);
	info("\nI2C%d:i2c_master_int_test_ISR: base=0x%x int_sts=0x%x\n", priv->port, priv->regs, int_sts);
	if (int_sts & I2C_INT_ST_ACTIVITY_MASK) 
	{
		dw_i2c_readl(priv, I2C_CLR_ACTIVITY_OFST);
		i2c_set_int_mask(priv, 0xFFFF & (~I2C_INT_ST_ACTIVITY_MASK));
		info("I2C_INT_ST_ACTIVITY_MASK:%x\n", I2C_INT_ST_ACTIVITY_MASK);
		g_i2c_int_cnt[port]++;
	}
	if (int_sts & I2C_INT_ST_START_DET_MASK) 
	{
		dw_i2c_readl(priv, I2C_CLR_START_DET_OFST);
		i2c_set_int_mask(priv, 0xFFFF & (~I2C_INT_ST_START_DET_MASK));
		info("I2C_INT_ST_START_DET_MASK:%x\n", I2C_INT_ST_START_DET_MASK);
		g_i2c_int_cnt[port]++;
	}
	if (int_sts & I2C_INT_ST_TX_EMPTY_MASK) 
	{
	    i2c_set_int_mask(priv, 0xFFFF & (~I2C_INT_ST_TX_EMPTY_MASK));
		info("I2C_INT_ST_TX_EMPTY_MASK:%x\n", I2C_INT_ST_TX_EMPTY_MASK);
		g_i2c_int_cnt[port]++;
	}

	if(g_i2c_int_cnt[port] > 0x20)
		i2c_set_int_mask(priv, 0x0);
	
	i2c_clear_all_int(priv);

}

void i2c_slave_int_test_ISR(void *ptr)
{
	struct dw_i2c *priv = p_i2c_dev[g_i2c_slave_port];
	uint32_t int_sts = 0;
	E_I2C_PORT port = g_i2c_slave_port;

	// master read request	  
	int_sts = dw_i2c_readl(priv, I2C_INTR_STAT_OFST);
	info("\ni2c_slave_int_test_ISR: int_sts=0x%x\n", int_sts);
	if (int_sts & I2C_INT_ST_RD_REQ_MASK) 
	{
		info("I2C_INT_ST_RD_REQ_MASK:%x\n", I2C_INT_ST_RD_REQ_MASK);
		g_i2c_int_cnt[port]++;
	}
		
	// reg off not enough,something wrong for the protocol
	if (int_sts & I2C_INT_ST_START_DET_MASK) 
	{
		info("I2C_INT_ST_START_DET_MASK:%x\n", I2C_INT_ST_START_DET_MASK);
		g_i2c_int_cnt[port]++;
	}

	// tx fifo not empty when request read,clr the interrupt
	if (int_sts & I2C_INT_ST_TX_ABRT_MASK) 
	{
		info("I2C_INT_ST_TX_ABRT_MASK:%x\n", I2C_INT_ST_TX_ABRT_MASK);
		g_i2c_int_cnt[port]++;
	}
		
	if (int_sts & I2C_INT_ST_STOP_DET_MASK) 
	{
		info("I2C_INT_ST_STOP_DET_MASK:%x\n", I2C_INT_ST_STOP_DET_MASK);		
		g_i2c_int_cnt[port]++;
	}
	
	if (int_sts & I2C_INT_ST_RX_DONE_MASK) 
	{
		dw_i2c_readl(priv, I2C_CLR_RX_DONE_OFST);
		info("I2C_INT_ST_RX_DONE_MASK:%x\n", I2C_INT_ST_RX_DONE_MASK);		
		g_i2c_int_cnt[port]++;
	}
	if(g_i2c_int_cnt[port] > 0x20)
		i2c_set_int_mask(priv, 0x0);

	i2c_slave_transfer_rx(priv, i2c_slave_buf);
	i2c_slave_transfer_tx(priv, i2c_slave_buf, 8);

}

void i2c_slave_int_handler(void *ptr)
{
	struct dw_i2c *priv = p_i2c_dev[g_i2c_slave_port];
	
	i2c_slave_transfer_rx(priv, i2c_slave_buf);
	i2c_slave_transfer_tx(priv, i2c_slave_buf, 8);
}

/*testcase:i2c interrupt test*/
static int i2c_int_master(int argc, char* argv[])
{
	uint32_t dev, addr_mode, len=10;
	int32_t ret_val;
	uint32_t fail = 0;
	E_I2C_PORT port = g_i2c_port;
	struct dw_i2c *priv = p_i2c_dev[port];

	//prcm_reset(ARM_I2C0_RST + port);

	info("i2c_int_master: master mode interrupt test\n");

	// Check the validity of parameter count
	if (argc < 3)
	{
		info("wrong cmd_format: i2c_int_master dev addr_mode \n");
		return 0;
	}

	// Get the parameters
	dev = get_arg_ulong(argv[1]);
	addr_mode = (!strcmp(argv[2], "10b")) ? I2C_10BIT_ADDR : I2C_7BIT_ADDR;

	info("input cmd_format: i2c_int_master dev(0x%x) addr_mode(%s)\n", dev, argv[2]);

#if 0
	// Set pinmux to i2c
	info("set pinmux to i2c\n");
	if (!misc_i2c_pin_init(port))
	{
		return 0;	
	}
#endif

	g_i2c_int_cnt[port] = 0;

	// Set i2c to master mode
	i2c_init(priv, (uint32_t)priv->regs, priv->pclk, 1);
	i2c_init_master(priv, 100000, addr_mode);

	info("I2C%d: base=0x%x int_vector=%d\n", port, priv->regs, priv->int_vector);
	// register int handler and enable it
	irq_generic_install_handler(priv->int_vector, i2c_master_int_test_ISR, priv);
	irq_generic_config_int(priv->int_vector, IRQ_TYPE_LEVEL_HIGH);
	irq_generic_enable_int(priv->int_vector);
	_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);

	// enable interrupt
	i2c_set_int_mask(priv, 0xFFFF);

	// i2c write data 
	info("i2c write data, len: 0x%x\n", len);
	ret_val = i2c_write(priv, dev, 0, 0, test_send_buff, len);
	if (ret_val) 
	{
		debug("i2c write data fail, ret=%d\n", ret_val);
		fail++;
		goto end;
	}

	// i2c read data
	info("i2c read data, len: 0x%x\n", len);
	ret_val = i2c_read(priv, dev, 0, 0, test_recv_buff, len);
	if (ret_val) 
	{
		debug("i2c read data fail, ret=%d\n", ret_val);
		fail++;
		goto end;
	}

end:
	// disable interrupt(master)
	irq_generic_disable_int(priv->int_vector);
	irq_generic_remove_handler(priv->int_vector, NULL, NULL);
	i2c_set_int_mask(priv, 0);

	// disable interrupt(slave)
	priv = p_i2c_dev[g_i2c_slave_port];
	irq_generic_disable_int(priv->int_vector);
	irq_generic_remove_handler(priv->int_vector, NULL, NULL);
	i2c_set_int_mask(priv, 0);
	
	return 0;
}

/*testcase:i2c interrupt test*/
static int i2c_int_slave(int argc, char* argv[])
{
	uint32_t dev, addr_mode;
	E_I2C_PORT port = g_i2c_port;
	struct dw_i2c *priv = p_i2c_dev[port];

	// get the slave port for interrupt
	g_i2c_slave_port = port;
	//prcm_reset(ARM_I2C0_RST + port);

	info("i2c_int_slave:i2c slave mode interrupt test\n");

	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("wrong cmd_format: i2c_int_slave dev addr_mode\n");
		return 0;
	}

	// Get the parameters
	dev = get_arg_ulong(argv[1]);
	addr_mode = (!strcmp(argv[2], "10b")) ? I2C_10BIT_ADDR : I2C_7BIT_ADDR;

	// Set pinmux to i2c
	info("set pinmux to i2c\n");
#if 0
	if (!misc_i2c_pin_init(port))
	{
		return 0;	
	}
#endif

	g_i2c_int_cnt[port] = 0;

	// Set i2c to slave mode
	mem_set8(i2c_slave_buf, 0, sizeof(i2c_slave_buf));
	i2c_init_slave(priv, dev, addr_mode, 4, sizeof(i2c_slave_buf));

	info("I2C%d: base=0x%x int_vector=%d\n", port, priv->regs, priv->int_vector);

	// Register int handler and enable it
	irq_generic_install_handler(priv->int_vector, i2c_slave_int_test_ISR, NULL);
	irq_generic_config_int(priv->int_vector, IRQ_TYPE_LEVEL_HIGH);
	irq_generic_enable_int(priv->int_vector);	
	_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);

	// Enable interrupt
	//i2c_set_int_mask(priv, I2C_INT_ST_RX_FULL_MASK|I2C_INT_ST_RD_REQ_MASK|I2C_INT_ST_STOP_DET_MASK | I2C_INT_ST_RX_DONE_MASK);	
	i2c_set_int_mask(priv, (I2C_INT_ST_RX_FULL_MASK|I2C_INT_ST_RD_REQ_MASK|I2C_INT_ST_STOP_DET_MASK));

	info("i2c_int_slave: setup terminate\n");

	return 0;

}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry i2c_test_menu[] = {
	{"help", 		cmd_help, 	" : Display list of commands"},
	{"h",			cmd_help, 	"    : alias for help"},
	{"?",			cmd_help, 	"    : alias for help"},
	//{"i2c_slave",	i2c_slave, 	"   : i2c slave mode test"},
	//{"i2c_master",	i2c_master, "  : i2c master basic read/write test"},
	{"i2c_test_loopback", i2c_test_loopback, ":master and slave controller loopback test"},
	{"i2c_int_master",		i2c_int_master,  	"  : i2c interrupt handle test"},
    //{"i2c_int_slave",		i2c_int_slave,  	"  : i2c interrupt handle test"},	
	{0, 			0, 			0}
};

int i2c_test_prepare(int port)
{
	int32_t ret_val = 0;
	struct dw_i2c *priv = p_i2c_dev[port];

	g_i2c_port = port;

	mem_set8(priv, 0, sizeof(struct dw_i2c));
	if (port == I2C0) 
	{
		i2c_init(priv, I2C0_BASE, I2C0_PCLK, 0);
		priv->dmac_idx = 0;
		priv->dma_ch_tx = 0;
		priv->dma_ch_rx = 1;
		priv->dma_req_tx = DMA_REQ_I2C_S_TX;
		priv->dma_req_rx = DMA_REQ_I2C_S_RX;
		priv->int_vector = INT_I2CS;

		// set slave pinmux scl3  -- port0 --j57
		pad_i2csscl_pinmux(PINMUX_FUNC0, PIN_PD_DIS);
		pad_i2cssda_pinmux(PINMUX_FUNC0, PIN_PD_DIS);			
	} 
	else if (port == I2C1)
	{
		i2c_init(priv, I2C1_BASE, I2C1_PCLK, 0);
			
		priv->dmac_idx = 1;
		priv->dma_ch_tx = 0;
		priv->dma_ch_rx = 1;
		priv->dma_req_tx = DMA_REQ_I2C_M_TX;
		priv->dma_req_rx = DMA_REQ_I2C_M_RX;
		priv->int_vector = INT_I2CM;

		// set master pinmux scl1  -- port1  --j36	
		pad_i2smic_sbclk_pinmux(PINMUX_FUNC4, PIN_PD_DIS);
		pad_i2smic_slr0_pinmux(PINMUX_FUNC4, PIN_PD_DIS);
	} 
	else 
	{
		info("wrong device id\n");
		ret_val = 1;
	}
	priv->port = port;

	return ret_val;
}

int i2c_test_cleanup(int port)
{
	//struct dw_i2c *priv = p_i2c_dev[port];
	//struct dw_dmac* p_dma = &dma_dev_list[priv->dmac_idx];
	
	return 0;
}

#endif


