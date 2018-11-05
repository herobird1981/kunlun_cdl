/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_uart.h"

#if (CONFIG_UART_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry uart_test_menu[];

static uint8_t g_uart_port = 0;
static uint8_t g_uart_echo_port = 0;

uint8_t g_uart_int_in_use[4];
uint8_t g_dma_int_in_use[4];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/
extern void console_init(E_UART_PORT port, E_UART_BD bd, E_UART_DLS dls, E_UART_STOP stop, E_UART_EPS parity);

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
static int cmd_help(int argc, char *argv[])
{	
	return cmdline_help_general(&uart_test_menu[0]);
}

/*****************************************************************************/
//	Function:  static int uart_test_init(int argc, char *argv[])
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
//		 uart test initialize.
//
/*****************************************************************************/
static int uart_test_init(int argc, char *argv[])
{
	E_UART_PORT port = g_uart_port;
	struct brite_uart *priv = &g_uart[port];
	uint32_t val;

	// Check the validity of parameter count
	if (argc < 7) 
	{
		info("\nwrong cmd_format:%s dt_mode bd parity data_bits stop_bits flow_ctl\n\n", argv[0]);
		info("dt_mode(cpu/dma): 0, cpu; 1, dma\n");
		info("bd: baud rate(115200, etc)\n");
		info("parity: 'n', no parity; 'e', even parity; 'o', odd parity\n");
		info("data_bits: 5/6/7/8 bits\n");
		info("stop_bits: 0, 1 stop bit; 1, 1.5 stop bits\n");
		info("flow_ctl: 0/1\n");
		
		return ERR_TEST_ARGC;
	}
	
	// Initialize test parameter and hw initialize
	priv->dt_mode = get_arg_ulong(argv[1]) & 1;	
	priv->bd = get_arg_ulong(argv[2]);

	// get parity
	if 		(!strcmp(argv[3], "n")) priv->parity = UART_PARITY_NONE;
	else if (!strcmp(argv[3], "e")) priv->parity = UART_PARITY_EVEN;
	else if (!strcmp(argv[3], "o")) priv->parity = UART_PARITY_ODD;
	else
	{
		info("unknown parity setting\n");
		return ERR_TEST_PARA;
	}

	// get data bits
	val = get_arg_ulong(argv[4]);
	if ((val < 5) || (val > 8)) 
	{
		info("wrong data bits, supported: 5-8\n");
		return ERR_TEST_PARA;
	}

	priv->data_bits = val;
	priv->dls = priv->data_bits - 5;

	// get stop bits
	val = get_arg_ulong(argv[5]);
	if (val > 2) 
	{
		info("wrong stop bits, supported: 1, 1.5 bits\n");
		return ERR_TEST_PARA;
	}

	priv->stop = val;

	// get flow control
	priv->flow_ctl = get_arg_ulong(argv[6]) & 1;

	// set baud rate
	uart_init(port);

	// set control
	//priv->len_per_xfer = priv->flow_ctl ? (UART_FIFO_DEPTH / 2) : (UART_FIFO_DEPTH / 4);
	//priv->tx_dma_burst_size = BURST_32;
	//priv->rx_dma_burst_size = priv->flow_ctl ? BURST_32 : BURST_16;

	// FIFO should be empty already,
	// still force read RBR register to resolve previous deadlock condition
	uart_empty_fifo(port);

	info("uart config: using %s mode\n", priv->dt_mode ? "dma" : "polling");
	info("uart config: '%d %s %d %d' %s flow ctrl\n",
		priv->bd,
		(priv->parity == UART_PARITY_EVEN) ? "e" : ((priv->parity == UART_PARITY_ODD) ? "o" : "n"),
		priv->data_bits,
		priv->stop,
		priv->flow_ctl ? "hw" : "no");

	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  static int uart_test_loopback(int argc, char *argv[])
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
//		uart loopback transmit/receive test.
//		rx and tx pin connected together in one uart.
//
/*****************************************************************************/
static int uart_test_loopback(int argc, char *argv[])
{
	E_UART_PORT port = g_uart_port;
	struct brite_uart *priv = &g_uart[port];
	uint8_t tx_ch, rx_ch;
	uint32_t len;
	uint32_t i;
	int fail = 0;
	u64 start = 0, end = 0;
	uint32_t pattern;
	uint32_t random_flag = 1;
	uint32_t bits_mask;

	info("%s: uart loopback tx/rx test\n",argv[0]);

	// Check the validity of parameter count
	if (argc < 8) 
	{
		info("wrong cmd_format:%s dt_mode bd parity data_bits stop_bits flow_ctl len [pattern]", argv[0]);

		return ERR_TEST_ARGC;
	}
	
	if (uart_test_init(argc, argv))
	{
		return ERR_TEST_DONE;
	}

	// get len parameter		
	len = get_arg_ulong(argv[7]);

	// get random parameter
	if (argc > 8) 
	{
		pattern = get_arg_ulong(argv[8]);
		random_flag = 0;
	}	
	
	bits_mask = (1 << priv->data_bits) - 1;
	srand(get_ticks());

	for (i = 0; i < len; i++) 
	{
		// transmit a character
		if (random_flag)	tx_ch = cb_rand() & 0xFF;
		else				tx_ch = pattern & 0xFF;
		uart_putc(port, tx_ch);

		// receive a character
		start = get_timer(0);
		while (1)
		{
			if (uart_tstc(port))
			{
				rx_ch = uart_getc(port);
				if (rx_ch != (tx_ch & bits_mask)) 
				{
					fail++;
					
					debug("external loopback: send 0x%x but received 0x%x\n", tx_ch, rx_ch);
					goto end;
				}
				
				break;
			}

			end = get_timer(start);
			if (end >= 100) 
			{
				fail++;
				break;
			}
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
//	Function:  static void uart_generate_random_data(uint8_t *p_buf, uint32_t len, 
//												uint8_t random_flag, uint8_t pattern)
//
//	Parameters:
//		p_buf [in]	: data buffer
//	
//		len			: buffer length
//
//		random_flag	: random flag
//						= 0, fixed pattern data
//						= 1, random
//
//		pattern		: data pattern 
//
//	Return:
//		0	: success
//
//	Description:
//		uart loopback transmit/receive test.
//		rx and tx pin connected together in one uart.
//
/*****************************************************************************/
static void uart_generate_random_data(uint8_t *p_buf, uint32_t len, 
											uint8_t random_flag, uint8_t pattern)
{
	uint32_t i;

	if (random_flag) 
	{
		srand(get_ticks());

		// generate random data: printable
		for (i = 0; i < len; i++) 
		{
			p_buf[i] = (cb_rand() % 0x4B) + 0x30;
		}
	} 
	else 
	{
		for (i = 0; i < len; i++) 
		{
			p_buf[i] = pattern;
		}
	}	
}

static int uart_result_comp(E_UART_PORT port, uint8_t *p_tx, uint8_t *p_rx, uint32_t len)
{
	struct brite_uart *priv = &g_uart[port];
	int i;
	int ret = 0;
	unsigned char mask=0xff;

	mask = (1 << priv->data_bits) - 1;

	for (i=0;i<len;i++) 
	{
		if ((test_send_buff[i] & mask) != test_recv_buff[i]) 
		{
			info("send 0x%x(%c) but received 0x%x at 0x%x(%d)\n",
				test_send_buff[i],test_send_buff[i],
				test_recv_buff[i],i,i);

			ret = 1;

			break;
		}
	}
	
	if (ret) 
	{
		print_in_line((uint8_t *)"send", test_send_buff, len);
		print_in_line((uint8_t *)"recv", test_recv_buff, len);
	}
	
	return ret;
}

static int uart_transfer_loop(E_UART_PORT port, uint8_t *p_tx, uint8_t *p_rx, uint32_t len, 
								uint32_t *p_loop, uint8_t random_flag, uint8_t pattern)
{
	struct brite_uart *priv = &g_uart[port];
	uint32_t tx_len = 0;
	uint32_t rx_len = 0;
	uint32_t trans_len = 0;
	uint32_t fail = 0;	
	uint32_t loop = *p_loop;
	uint32_t real_loop = 0;
	

	while (loop--) 
	{
		if (console_check_char('q'))
		{
			break;
		}
							
		info("loop %d err %d\n", real_loop++, fail);
				
		// initialzie tx/rx len to be 0
		tx_len = 0;
		rx_len = 0;

		// initiallize the data of tx and rx buffer
		uart_generate_random_data(p_tx, len, random_flag, pattern);
		mem_set8(p_rx, 0x00, len);

		while (tx_len < len) 
		{
			trans_len = min(priv->len_per_xfer, (len - tx_len));
			
			info("trans_len: %d\n", trans_len);

			tx_len += uart_write(port, (uint8_t *)(test_send_buff+tx_len), trans_len);
			info("tx len %d\n", tx_len);
			
			rx_len += uart_read(port, (uint8_t *)(test_recv_buff+rx_len), trans_len);
			info("rx len %d\n", rx_len);
		}
		
		// comapre result
		if (uart_result_comp(port, test_send_buff, test_recv_buff, len))
		{
			fail++;
		}			
	}
	
	*p_loop = real_loop;

	return fail;
}

/*****************************************************************************/
//	Function:  void uart_echo_int_handler(void *ptr)
//
//	Parameters:
//		ptr	: parameter
//
//	Return:
//		none
//
//	Description:
//		uart rx interrupt routine. 
//	Receive available data until timeout and transmit the received data out. 
//
/*****************************************************************************/
void uart_echo_int_handler(void *ptr)
{
	E_UART_PORT port = g_uart_echo_port;
	uint32_t reg_val;	
	uint32_t len;
	uint32_t fifo_level;
	uint8_t echo_buf[UART_FIFO_DEPTH];

	info("%s\n", __func__);

	// check line status
	reg_val = uart_rd_reg(port, UART_LSR_OFST);
	if (reg_val & 0x1E)
	{
		info("irq UART_LSR_OFST err 0x%x\n", reg_val);
	}		

	// check int status
	reg_val = uart_rd_reg(port, UART_IIR_OFST);
	if (reg_val == UART_IIR_NO_INT) 
	{
		info("irq UART_IIR_OFST err 0x%x\n", reg_val);
		return;
	}

	// get receive fifo level
	fifo_level = uart_rd_reg(port, UART_RFL_OFST);

	len = uart_rx_non_block(port, echo_buf, fifo_level, 1000);
	info("irq rx len %d of fifo_level %d\n", len, fifo_level);
	
	len = uart_write(port, echo_buf, len);
	info("irq tx len %d\n", len);
}


/*****************************************************************************/
//	Function:  static int uart_test_echo(int argc, char *argv[])
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
//		receive x data and transmit x data
//
/*****************************************************************************/
static int uart_test_echo(int argc, char* argv[])
{
	E_UART_PORT port = g_uart_port;
	struct brite_uart *priv = &g_uart[port];
	
	info("%s: uart echo server\n", __func__);

	// disable interrupt 	
	uart_int_ctrl(port, 0);

	// initialize the uart
	if (uart_test_init(argc, argv))
	{
		return ERR_TEST_DONE;
	}

	// set rx fifo threshold
	uart_set_rx_fifo_thres(port, UART_RT_FIFO_1_4_FULL);

	g_uart_int_in_use[priv->line] = 1;
	g_uart_echo_port = port;
	
	// register int handler and enable it
	irq_generic_install_handler(priv->int_vec, uart_echo_int_handler, priv);
	irq_generic_config_int(priv->int_vec, IRQ_TYPE_LEVEL_LOW);
	irq_generic_enable_int(priv->int_vec);

	// enable rx interrupt 	
	uart_int_ctrl(port, UART_ERBFI_MASK);

	info("uart echo server run in background\n");

	return ERR_TEST_DONE;
}

static uint8_t g_tx_line_error = 0;
void uart_tx_int_handler(void *ptr)
{
	E_UART_PORT port = g_uart_port;
	uint32_t reg_val;

	info("%s\n", __func__);

	// check line status
	reg_val = uart_rd_reg(port, UART_LSR_OFST);
	if (reg_val & 0x1E)
	{
		g_tx_line_error++;
		info("Line error 0x%x\n", reg_val);
	}

	// check int status
	reg_val = uart_rd_reg(port, UART_IIR_OFST);		
	info("irq st 0x%x\n", reg_val);
}

/*****************************************************************************/
//	Function:  static int uart_test_transfer(int argc, char *argv[])
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
//		receive x data and transmit x data
//
/*****************************************************************************/
static int uart_test_transfer(int argc, char *argv[])
{
	E_UART_PORT port = g_uart_port;
	struct brite_uart *priv = &g_uart[port];

	uint32_t loop = 0;
	int fail = 0;
	unsigned char pattern = 0;
	int random_flag = 1;

	info("%s: rs232 transmit/receive test, press 'q' to quit\n", __func__);

	// Check the validity of parameter count
	if (argc < 8) 
	{
		info("wrong cmd_format:%s dt_mode bd parity data_bits stop_bits flow_ctl loop [pattern]", argv[0]);
		return ERR_TEST_ARGC;
	}

	// initialize the uart
	if (uart_test_init(argc, argv))
	{
		return ERR_TEST_DONE;
	}

	// get the parameter
	loop = get_arg_ulong(argv[7]);
	
	if (argc >= 9) 
	{
		pattern = get_arg_ulong(argv[8]);
		random_flag = 0;
	}

	g_tx_line_error = 0;
	
	// register int handler and enable it
	irq_generic_install_handler(priv->int_vec, uart_tx_int_handler, NULL);
	irq_generic_config_int(priv->int_vec, IRQ_TYPE_LEVEL_LOW);
	irq_generic_enable_int(priv->int_vec);

	// enable receiver line status interrupt
	uart_int_ctrl(port, UART_ELSI_MASK);
	
	fail = uart_transfer_loop(port, test_send_buff, test_recv_buff, 240, &loop, random_flag, pattern);

	info("total %d loop, err %d\n", loop, fail);
	
	if (fail || g_tx_line_error) 
	{
		info("%s: test fail\n",argv[0]);
	} 
	else 
	{
		info("%s: test pass\n",argv[0]);
	}
	
	// disable all interrupt
	irq_generic_disable_int(priv->int_vec);
	irq_generic_remove_handler(priv->int_vec, NULL, NULL);
	uart_int_ctrl(port, 0);

	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  static int uart_test_tx(int argc, char *argv[])
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
//		uart send with fixed string.
//
/*****************************************************************************/
static int uart_test_tx(int argc, char *argv[])
{
	E_UART_PORT port = g_uart_port;
	struct brite_uart *priv = &g_uart[port];	
	char str[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	uint32_t len = strlen(str);
	uint8_t *p_tx = (uint8_t *)test_send_buff;

	info("%s: tx fixed string\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 2) 
	{
		info("wrong cmd_format: %s dt_mode\n", argv[0]);
		info("dt_mode(cpu/dma): 0, cpu; 1, dma\n");
		
		return ERR_TEST_DONE;
	}

	// uart port initialize 
	console_init(port, UART_BD_115200, UART_DLS_8_BITS, UART_STOP_1_BIT, UART_PARITY_NONE);

	priv->dt_mode = get_arg_ulong(argv[1]);

	// clear the tx buffer
	mem_cpy8(p_tx, str, len);

	// transmit fixed character
	uart_write(port, p_tx, len);

	info("\n");

	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  static int uart_test_rx(int argc, char *argv[])
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
//		uart received and display received data.
//
/*****************************************************************************/
static int uart_test_rx(int argc, char *argv[])
{
	E_UART_PORT port = g_uart_port;
	struct brite_uart *priv = &g_uart[port];
	uint32_t len;
	uint8_t *p_rx = (uint8_t *)test_recv_buff;

	info("%s:received and display, press 'q' to quit\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 2) 
	{
		info("wrong cmd_format: %s dt_mode\n", argv[0]);
		info("dt_mode(cpu/dma): 0, cpu; 1, dma\n");
		
		return ERR_TEST_DONE;
	}

	// uart port initialize 
	console_init(port, UART_BD_115200, UART_DLS_8_BITS, UART_STOP_1_BIT, UART_PARITY_NONE);

	priv->dt_mode = get_arg_ulong(argv[1]);

	// receive data until the user pressed 'q'
	while (1) 
	{
		if (console_check_char('q'))
		{
			break;
		}

		mem_set8(p_rx, 0, (TEST_BUFFER_SIZE/2));
		
		len = uart_rx_non_block(port, p_rx, 1, 100);
		if (len > 0)
		{
			info("recv: %d, bytes: %s\n", len, p_rx);
		}
	}
	
	return ERR_TEST_DONE;
}

static int uart_int_counter = 0;
void uart_int_handler(void* ptr)
{
	struct brite_uart *priv = ptr;	

	int int_status = 0;

	int_status = uart_rd_reg(priv->port, UART_IIR_OFST) & 0xF;
	if (int_status != 0x1) {
		uart_int_counter++;
	}
	info("uart%d:uart_int_handler, base=0x%x status=%x\n", priv->port, priv->base, int_status);

	_SET_SYSCON_REG_interrupt_control_interrupt_en_227(0);
}

/*testcase:uart interrupt test*/
static int uart_int(int argc, char* argv[])
{
	E_UART_PORT port = g_uart_port;
	struct brite_uart *priv = &g_uart[port];	
	int fail = 0;
	int time_used = 0;

	// Check the validity of parameter count
	if (argc < 2) 
	{
		info("wrong cmd_format: %s dt_mode\n", argv[0]);
		info("dt_mode(cpu/dma): 0, cpu; 1, dma\n");
		
		return ERR_TEST_DONE;
	}

	priv->dt_mode = get_arg_ulong(argv[1]);

    uart_wr_reg(port, UART_FCR_OFST, 0x6);

	info("UART%d: base=0x%x int_vector=%d\n", port, g_uart[port].base, priv->int_vec);
	/*register int handler and enable it*/
	irq_generic_install_handler(priv->int_vec,
		uart_int_handler,
		priv);
	irq_generic_config_int(priv->int_vec, IRQ_TYPE_LEVEL_LOW);
	irq_generic_enable_int(priv->int_vec);
	_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);
	/*enable interrupt*/
	uart_int_ctrl(port, 0xf);

	//wait interrupt handle, and wait user break testcase
	while(time_used++ < 5000 && uart_int_counter == 0)
		mdelay(1);

	if (uart_int_counter == 0) {
		fail++;
	}

	if (fail) {
		info("%s: test fail\n",argv[0]);
	} else {
		info("%s: test pass\n",argv[0]);
	}

	/*disable interrupt*/
	irq_generic_disable_int(priv->int_vec);
	irq_generic_remove_handler(priv->int_vec,NULL,NULL);
	uart_int_ctrl(port, 0);

	return 0;
}

#if (UART_BOOT_TEST == 1)

#include "data_array.h"
#include "image_header.h"
#include "fw_crc.h"

static int get_status(unsigned short *para)
{
	unsigned int sizeToTransfer = 0;
	unsigned int sizeTransfered;
	int status;
	unsigned char buffer[8] = { 0 };
	//uint8 tmp = 0;

	sizeToTransfer = 8;
	sizeTransfered = 0;

    status = uart_read(0, buffer, sizeToTransfer);
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

	status = uart_write(0, buffer, sizeToTransfer);

	return status;
}

static int send_getversion()
{
	unsigned int sizeToTransfer = 8;
	unsigned int sizeTransfered = 0;
	int status;
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

	status = uart_write(0, buffer, sizeToTransfer);
	return status;
}

static int send_setconfig()
{
	unsigned int sizeToTransfer = 8 + 16;
	unsigned int sizeTransfered = 0;
	int status;
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


	status = uart_write(0, buffer, sizeToTransfer);

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
	mem_cpy8((unsigned char *)(buffer + 4), buff, len);
	/*config para not used so far*/
	buffer[4 + len] = (pkt_id >> 8) & 0xff;
	buffer[5 + len] = pkt_id & 0xff;

	crc16 = crc16_default(0x0000, buffer, 6 + len);
	buffer[6 + len] = (crc16 >> 8) & 0xff;
	buffer[7 + len] = crc16 & 0xff;

	status = uart_write(0, buffer, sizeToTransfer);

	return status;
}

/*testcase:uart interrupt test*/
int uart_boot(int argc, char* argv[])
{
	E_UART_PORT port = UART0;
	struct brite_uart *priv = &g_uart[port];	

	int i = 0;
	unsigned short para = 0;

	int ret = 0;

	int 	file_len = sizeof(binArray);
	unsigned char	*file_buffer = binArray;
	int 	area_num = 0;
	const	img_header_t *image_ptr = NULL;
	unsigned int	offset = 0;
	unsigned int	sect_len = 0;
	unsigned short	pkt_len = 0;

	console_init(port, UART_BD_115200, UART_DLS_8_BITS, UART_STOP_1_BIT, UART_PARITY_NONE);

	//send handshake command & check status
	send_handshake();
	mdelay(100);
#if 1
	
	get_status(&para);
#endif	
	printf("device version:%x\r\n", para);

	mdelay(100);
	//send get version command & check status

	//send data packets & check status
	//1. send image header
	image_ptr = (const	img_header_t *)file_buffer;

	send_datapkt(file_buffer, IMG_HDR_SIZE, 0);
	mdelay(100);
#if 1
	get_status(&para);
	if (para != 0x0)
	{
		printf("image header failed\r\n");
		ret = -1;
		goto __exit;
	}
#endif
	mdelay(10);

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
			mdelay(20);
#if 1
			get_status(&para);
			if (para != i)
			{
					printf("send pkt %d fail\n", i);
					ret = -1;
					goto __exit;
			}
#endif	
			printf("sect len:%d\r\n", sect_len);
			/* remainder again, the setion size must be multiple of 4B */
			sect_len -= pkt_len;
			offset += pkt_len;
			mdelay(20);
		}
		mdelay(20);
	}

__exit:
	return 0;
}

#endif
/*****************************************************************************/
//	Function:  static int uart_test_console_switch(int argc, char *argv[])
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
//		switch the uart console port.
//
/*****************************************************************************/
static int uart_test_switch_console(int argc, char *argv[])
{
	E_UART_PORT port;

	info("%s:switch console to other uart port\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 2) 
	{
		info("wrong cmd_format: %s port\n", argv[0]);
		info("port: 0/1\n");
		
		return ERR_TEST_DONE;
	}

	port = get_arg_ulong(argv[1]);
	if (port >= NUM_UART_PORTS)
	{
		info("invalid port %d\n", port);
		
		return ERR_TEST_PARA;
	}
	
	// uart port initialize 
	console_init(port, UART_BD_115200, UART_DLS_8_BITS, UART_STOP_1_BIT, UART_PARITY_NONE);
	
	info("switch console done, press 'enter' to active new console\n");

	return ERR_TEST_DONE;
}

/*****************************************************************************/
//	Function:  int uart_test_prepare(int port)
//
//	Parameters:
//		port	: device port
//
//	Return:
//		0	: success
//
//	Description:
//		Initialize basic parameter before testing the device.
//
/*****************************************************************************/
int uart_test_prepare(int port)
{
	struct brite_uart *priv = &g_uart[port];

	// Check the current console port
	if (port == g_console_port)
	{
		info("current console use uart %d\n", port);
		info("please change console setting to test uart %d\n", port);
		//return ERR_TEST_FAIL;
	} 

	// clear the uart structure memory
	mem_set8(priv, 0, sizeof(struct brite_uart));

	// configure parameter for each port
	if (port == UART0) 
	{
		// uart 0
		pad_gpio0_pinmux(PINMUX_FUNC2, 0);
		pad_gpio1_pinmux(PINMUX_FUNC2, 0);	

		priv->port = port;
		priv->base = UART0_BASE;
		priv->pclk = UART0_PCLK;

		// interrupt parameter
		priv->int_en = 0;
		priv->int_vec = INT_UART0;	
	}
	else if (port == UART1)
	{
		// uart 1
		pad_uart0rxd_pinmux(PINMUX_FUNC1, 0);
		pad_uart0txd_pinmux(PINMUX_FUNC1, 0);

		priv->port = port;
		priv->base = UART1_BASE;
		priv->pclk = UART1_PCLK;

		// interrupt parameter
		priv->int_en = 0;
		priv->int_vec = INT_UART1;	
	}
	else
	{
		// not defined on current HW
		return ERR_TEST_PARA;
	}

	// set current port
	g_uart_port = port;

	priv->bd = UART_BD_115200;
	priv->dls = UART_DLS_8_BITS;
	priv->stop = UART_STOP_1_BIT;
	priv->parity = UART_PARITY_NONE;	
	priv->dt_mode = DT_CPU;
	uart_init(port);
		
	return ERR_TEST_DONE;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry uart_test_menu[] = {
	{"help", 						cmd_help, 					"                     : display list of commands"},
	{"h",							cmd_help, 					"                        : alias for help"},
	{"?",							cmd_help, 					"                        : alias for help"},
	//{"uart_test_loopback",			uart_test_loopback,  		"       : uart loopback tx/rx test"},
	//{"uart_test_echo",				uart_test_echo,				"           : uart echo server in background"},
	//{"uart_test_transfer",			uart_test_transfer,			"       : uart dma mode,baudrate,parity,data bits,stop bits test"},
	{"uart_test_tx",				uart_test_tx,				"             : debug transmit"},
	{"uart_test_rx",				uart_test_rx,				"             : debug receive"},
	//{"uart_test_switch_console",	uart_test_switch_console,  	" : switch current console to other uart port"},
	{ "uart_int",                   uart_int,                   "  : uart interrupt test" },
#if (UART_BOOT_TEST == 1)
	{ "uart_boot",					uart_boot,					"  : uart boot test" },
#endif
	{"q", 							NULL, 						" : quit uart test"},	
	{0, 							0, 							0}
};

#endif

