/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_uart.h"
#include "cdl_dma.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
struct brite_uart g_uart[NUM_UART_PORTS];

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//  Function:  uart0_int_handler(void *ptr)
//
//  Parameters:
//		ptr [in]	: pointer
//
//  Return:
//		none
//		
//  Description:
//     UART0 interrupt hadler.
//
/*****************************************************************************/
void uart0_int_handler(void *ptr)
{
	
}

/*****************************************************************************/
//  Function:  uart1_int_handler(void *ptr)
//
//  Parameters:
//		ptr [in]	: pointer
//
//  Return:
//		none
//		
//  Description:
//     UART1 interrupt hadler.
//
/*****************************************************************************/
void uart1_int_handler(void *ptr)
{
	
}

/*****************************************************************************/
//  Function:  void uart_wr_reg(E_UART_PORT port, uint32_t reg_ofst, uint32_t val)
//
//  Parameters:
//		port			: uart port
//
//		reg_ofst		: the offset of a register
//
//		val			: the value to write 
//
//  Return:
//		none
//		
//  Description:
//      Write a value to a register.
//
/*****************************************************************************/
void uart_wr_reg(E_UART_PORT port, uint32_t reg_ofst, uint32_t val)
{
	__raw_writel(val, (g_uart[port].base + reg_ofst));
}

/*****************************************************************************/
//  Function:  uint32_t uart_rd_reg(E_UART_PORT port, uint32_t reg_ofst)
//
//  Parameters:
//		port			: uart port
//
//		reg_ofst		: the offset of a register
//
//  Return:
//		The value from a register
//		
//  Description:
//      Read a value from a register.
//
/*****************************************************************************/
uint32_t uart_rd_reg(E_UART_PORT port, uint32_t reg_ofst)
{
	return __raw_readl(g_uart[port].base + reg_ofst);
}

/*****************************************************************************/
//  Function:  static void uart_modem_ctrl(E_UART_PORT port, uint32_t mode)
//
//  Parameters:
//		port		: uart port
//
//		mode	: control mode
//
//  Return:
//		none
//		
//  Description:
//      UART modem control.
//
/*****************************************************************************/
static void uart_modem_ctrl(E_UART_PORT port, uint32_t mode)
{
	uart_wr_reg(port, UART_MCR_OFST, mode);
}

/*****************************************************************************/
//  Function:  static void uart_dlab_ctrl(E_UART_PORT port, E_UART_DLAB dlab)
//
//  Parameters:
//		port		: uart port
//
//		dlab		: control mode
//
//  Return:
//		none
//		
//  Description:
//      UART data latch access control.
//
/*****************************************************************************/
static void uart_dlab_ctrl(E_UART_PORT port, E_UART_DLAB dlab)
{
	uint32_t reg_val = uart_rd_reg(port, UART_LCR_OFST);

	reg_val &= ~UART_DLAB_MASK;
	reg_val |= UART_DLAB_VAL(dlab);
	
	uart_wr_reg(port, UART_LCR_OFST, reg_val);
}

/*****************************************************************************/
//  Function:  static void uart_line_ctrl(E_UART_PORT port, E_UART_DLS dls, E_UART_STOP stop, E_UART_EPS parity)
//
//  Parameters:
//		port		: uart port
//
//		dls		: data bits
//
//		stop		: stop bits
//
//		parity	: even/odd parity 
//
//  Return:
//		none
//		
//  Description:
//      UART line control.
//
/*****************************************************************************/
static void uart_line_ctrl(E_UART_PORT port, E_UART_DLS dls, E_UART_STOP stop, E_UART_EPS parity)
{
	struct brite_uart *priv = &g_uart[port];
	
	uint32_t reg_val = UART_DLS_VAL(dls) 	|
					   UART_STOP_VAL(stop) 	|
					   UART_EPS_VAL(parity);
	
	uart_wr_reg(port, UART_LCR_OFST, reg_val);
	
	uart_wr_reg(port, UART_FCR_OFST, priv->flow_ctl ? UART_FCRVAL_AFCE : UART_FCRVAL);
}

/*****************************************************************************/
//  Function:  static uint32_t uart_calc_div(E_UART_PORT port, uint32_t clk, E_UART_BD bd)
//
//  Parameters:
//		port		: uart port
//
//		clk		: the input clock of the ip
//
//		bd		: baud rate
//
//  Return:
//		none
//		
//  Description:
//      Caculate the divisor upon input clock and baud rate.
//
/*****************************************************************************/
static uint32_t uart_calc_div(E_UART_PORT port, uint32_t clk, E_UART_BD bd)
{
	const uint32_t mode_x_div = 16;
	uint32_t uart_ip_div = 0;

#if 0 // CONFIG_ASIC_VERSION
	uint32_t uart_pll_div;

	if(!skip_pll_cfg) {
	switch (bd) {
	case 1000000:
	case 1500000:
	case 3000000:
		uart_pll_div = 0x82;			
		break;
	
	default:
		uart_pll_div = 0x0a;
		break;
	}

		/*set the uart pll divisor */
		if(uart_input_clk != clock) {
			/* console_reinit_bd(clock); */
			uart_input_clk = clock;
		}
	}
#endif

	uart_ip_div = (clk + mode_x_div * bd / 2) / (mode_x_div * bd);

	return uart_ip_div;
}

/*****************************************************************************/
//  Function:  void uart_int_ctrl(E_UART_PORT port, uint32_t mask)
//
//  Parameters:
//		port		: uart port
//
//		mask	: interrupt enable mask
//
//  Return:
//		none
//		
//  Description:
//      UART interrupt control.
//
/*****************************************************************************/
void uart_int_ctrl(E_UART_PORT port, uint32_t mask)
{
	uart_wr_reg(port, UART_IER_OFST, mask);
}

/*****************************************************************************/
//  Function:  void uart_set_bd(E_UART_PORT port)
//
//  Parameters:
//		port		: uart port
//
//  Return:
//		none
//		
//  Description:
//      Configure the baud rate divisor.
//
/*****************************************************************************/
void uart_set_bd(E_UART_PORT port)
{
	struct brite_uart *priv = &g_uart[port];
	uint32_t bd_div = uart_calc_div(port, priv->pclk, priv->bd);
	
	// disable & reset fifo
	uart_wr_reg(port, UART_FCR_OFST, 0); 

	// enable access DLAB
	uart_dlab_ctrl(port, UART_DLAB_EN);
	
	uart_wr_reg(port, UART_Dll_OFST, bd_div & 0xFF);
	uart_wr_reg(port, UART_DLH_OFST, (bd_div >> 8) & 0xFF);

	// disable access DLAB
	uart_dlab_ctrl(port, UART_DLAB_DIS);	
}

/*****************************************************************************/
//  Function:  void uart_init(E_UART_PORT port)
//
//  Parameters:
//		port		: uart port
//
//  Return:
//		none
//		
//  Description:
//      UART initialize(bd, stop, parity, etc).
//
/*****************************************************************************/
void uart_init(E_UART_PORT port)
{
	struct brite_uart *priv = &g_uart[port];
	uint32_t reg_val;

	if (port == UART0)
	{
		sys_sw_rst(UART0_RST);
	}
	else
	{
		sys_sw_rst(UART1_RST);
	}
	
	reg_val = uart_rd_reg(port, UART_CTR_OFST);
	
	// disable all interrupt
	uart_int_ctrl(port, 0); 

	// no flow control
	if (priv->flow_ctl)	reg_val = UART_AFCE_MASK | UART_RTS_MASK;
	else				reg_val = UART_DTR_MASK | UART_RTS_MASK;
	uart_modem_ctrl(port, reg_val); 
	
	uart_set_bd(port);
	
	// set bd, stop and parity
	uart_line_ctrl(port, priv->dls, priv->stop, priv->parity);
}

/*****************************************************************************/
//  Function:  void uart_flush(E_UART_PORT port)
//
//  Parameters:
//		port		: uart port
//
//  Return:
//		none
//		
//  Description:
//      Wait transmit fifo empty.
//
/*****************************************************************************/
void uart_flush(E_UART_PORT port)
{
	// wait transmit fifo empty
	while ((uart_rd_reg(port, UART_LSR_OFST) & UART_TEMT_MASK) == 0);
}

/*****************************************************************************/
//  Function:  void uart_putc(E_UART_PORT port, uint8_t ch)
//
//  Parameters:
//		port		: uart port
//
//  Return:
//		none
//		
//  Description:
//      Transmit a character to the FIFO.
//
/*****************************************************************************/
void uart_putc(E_UART_PORT port, uint8_t ch)
{
	// wait transmit fifo idle
	while ((uart_rd_reg(port, UART_LSR_OFST) & UART_THRE_MASK) == 0) 
	{}

	uart_wr_reg(port, UART_THR_OFST, ch);
}

/*****************************************************************************/
//  Function:  uint8_t uart_getc(E_UART_PORT port)
//
//  Parameters:
//		port		: uart port
//
//  Return:
//		received character
//		
//  Description:
//      Receive a character from the FIFO.
//
/*****************************************************************************/
uint8_t uart_getc(E_UART_PORT port)
{
	volatile uint32_t times = 0;

	while ((uart_rd_reg(port, UART_LSR_OFST) & UART_DR_MASK) == 0)
	{
		if (times++ >= (1UL<<23))
		{
			//return 0xff;
		}
	}		
	
	return uart_rd_reg(port, UART_RBR_OFST);
}

/*****************************************************************************/
//  Function:  int uart_tstc(E_UART_PORT port)
//
//  Parameters:
//		port		: uart port
//
//  Return:
//		data ready status
//		
//  Description:
//      Get the data ready status.
//
/*****************************************************************************/
int uart_tstc(E_UART_PORT port)
{
	return (uart_rd_reg(port, UART_LSR_OFST) & UART_DR_MASK);
}

/*****************************************************************************/
//  Function:  void uart_empty_fifo(E_UART_PORT port)
//
//  Parameters:
//		port		: uart port
//
//  Return:
//		none
//		
//  Description:
//      Empty the fifo.
//
/*****************************************************************************/
void uart_empty_fifo(E_UART_PORT port)
{
	while (uart_tstc(port))
	{
		uart_getc(port);
	}
	
	// force read RBR register to resolve previous deadlock condition
	uart_rd_reg(port, UART_RBR_OFST);
}

/*****************************************************************************/
//  Function:  uint32_t uart_write(E_UART_PORT port, const uint8_t *p_buf, uint32_t len)
//
//  Parameters:
//		port			: uart port
//
//		p_buf [out]	: data buffer to transmit
//
//		len			: data length
//
//  Return:
//		transmitted data length
//		
//  Description:
//      UART transmit(CPU/DMA data).
//
/*****************************************************************************/
uint32_t uart_write(E_UART_PORT port, const uint8_t *p_buf, uint32_t len)
{
	struct brite_uart *priv = &g_uart[port];
	uint32_t i;

	if (priv->dt_mode == DT_CPU) 
	{
		for (i = 0; i < len; i++) 
		{
			uart_putc(port, p_buf[i]);
		}

		return i;
	} 
	else 
	{
		if (DMA_ST_DONE == dma_uart_tx(port, DMA_CH0, CH_PRIOR_7, (uint32_t)p_buf, len, ST_POLLING))
		{
			return len;
		}
		else
		{
			return 0;
		}
	}
}

/*****************************************************************************/
//  Function:  uint32_t uart_read(E_UART_PORT port, uint8_t *p_buf, uint32_t len)
//
//  Parameters:
//		port			: uart port
//
//		p_buf [in]	: data buffer to receive
//
//		len			: data length
//
//  Return:
//		received data length
//		
//  Description:
//      UART receive(CPU/DMA) data.
//
/*****************************************************************************/
uint32_t uart_read(E_UART_PORT port, uint8_t *p_buf, uint32_t len)
{
	struct brite_uart *priv = &g_uart[port];
	uint32_t i;
	
	if (priv->dt_mode == DT_CPU)  
	{
		for (i = 0; i < len; i++) 
		{
			p_buf[i] = uart_getc(port);
		}
	} 
	else 
	{
		if (DMA_ST_DONE == dma_uart_rx(port, DMA_CH0, CH_PRIOR_0, (uint32_t)p_buf, len, ST_POLLING))
		{
			return len;
		}
		else
		{
			return 0;
		}		
	}
	
	return i;
}

/*****************************************************************************/
//	Function:  uint32_t uart_rx_non_block(E_UART_PORT port, uint8_t *p_buf, uint32_t len)//
//	Parameters:
//		port		: uart port
//
//		p_buf [in] : data buffer to receive
//
//		len 		: data length
//
//	Return:
//		received data length
//		
//	Description:
//		when timeout occur; returned, don't need read until all request 'len' reached
//	note: len can't exceed 4095
//
/*****************************************************************************/
uint32_t uart_rx_non_block(E_UART_PORT port, uint8_t *p_buf, uint32_t len, uint32_t timeout)
{
	struct brite_uart *priv = &g_uart[port];
	uint32_t rlen = 0;
	u64 start;

	if (priv->dt_mode == DT_CPU) 
	{
		start = get_timer(0);
		
		while (rlen < len) 
		{
			if (uart_tstc(port)) 
			{
				p_buf[rlen++] = uart_rd_reg(port, UART_RBR_OFST);
				continue;
			} 

			// check timeout
			if (get_timer(start) > timeout) 
			{
				break;
			}
		}
	} 
	else 
	{
		if (DMA_ST_DONE == dma_uart_rx(port, DMA_CH0, CH_PRIOR_0, (uint32_t)p_buf, len, ST_POLLING))
		{
			return len;
		}
		else
		{
			return 0;
		}
	}
	
	return rlen;
}

/*****************************************************************************/
//	Function:  void uart_set_rx_fifo_thres(E_UART_PORT port, E_UART_RT_FIFO thres)
//	Parameters:
//		port		: uart port
//
//		thres	: rx fifo threshold
//
//	Return:
//		none
//		
//	Description:
//		Set rx fifo threshold.
//
/*****************************************************************************/
void uart_set_rx_fifo_thres(E_UART_PORT port, E_UART_RT_FIFO thres)
{
	uint32_t reg_val = uart_rd_reg(port, UART_FCR_OFST);

	reg_val &= ~UART_FCR_RT_MASK;
	reg_val |= UART_FCR_RT_VAL(thres);
	
	uart_wr_reg(port, UART_FCR_OFST, reg_val);
}

/*****************************************************************************/
//	Function:  void uart_set_tx_fifo_thres(E_UART_PORT port, E_UART_TET_FIFO thres)
//	Parameters:
//		port		: uart port
//
//		thres	: tx fifo threshold
//
//	Return:
//		none
//		
//	Description:
//		Set tx fifo threshold.
//
/*****************************************************************************/
void uart_set_tx_fifo_thres(E_UART_PORT port, E_UART_TET_FIFO thres)
{
	uint32_t reg_val = uart_rd_reg(port, UART_FCR_OFST);

	reg_val &= ~UART_FCR_TET_MASK;
	reg_val |= UART_FCR_TET_VAL(thres);
	
	uart_wr_reg(port, UART_FCR_OFST, reg_val);
}

