/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "console.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define CONSOLE_BUF_SIZE	1024

#define CHAR_DEL			0x7F

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
static uint8_t console_buf[CONSOLE_BUF_SIZE];

E_UART_PORT g_console_port = CONSOLE_UART_PORT;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
static void console_putc(const char c)
{
	if (c == '\n') 
	{
		uart_putc(g_console_port, '\r');
	}
	
	uart_putc(g_console_port, c);
}

void console_init(E_UART_PORT port, E_UART_BD bd, E_UART_DLS dls, E_UART_STOP stop, E_UART_EPS parity)
{
	struct brite_uart *priv = &g_uart[port];
	
	mem_set8(priv, 0, sizeof(struct brite_uart));

	//g_console_port = port;
		
	if (port == UART0)
	{
		// uart 0
		pad_gpio0_pinmux(PINMUX_FUNC2, 0);
		pad_gpio1_pinmux(PINMUX_FUNC2, 0);

		priv->base = UART0_BASE;
		priv->pclk = UART0_PCLK;
	}
	else if (port == UART1)
	{
		// uart 1
		pad_uart0rxd_pinmux(PINMUX_FUNC1, 0);
		pad_uart0txd_pinmux(PINMUX_FUNC1, 0);

		priv->base = UART1_BASE;
		priv->pclk = UART1_PCLK;
	}
	else
	{
		// not defined on current HW
	}	

	priv->bd = bd;
	priv->dls = dls;
	priv->stop = stop;
	priv->parity = parity;	
	priv->dt_mode = DT_CPU;

	uart_init(port);
}

void console_reinit_bd(uint32_t pclk)
{
	struct brite_uart *priv = &g_uart[g_console_port];
	
	priv->pclk = pclk;
	uart_set_bd(g_console_port);
}

int console_tstc(void)
{
	return uart_tstc(g_console_port);
}

void console_flush(void) 
{
	uart_flush(g_console_port);
}

uint8_t console_getc(void)
{
	return uart_getc(g_console_port);
}

void console_puts(const uint8_t *s)
{
	while (*s) 
	{
		console_putc(*s++);
	}
}

// get chars from console until newline('\n') received
int console_gets(uint8_t *buf, uint32_t len)
{
	static uint8_t last_was_cr = 0;
	E_UART_PORT port = g_console_port;	
	uint32_t count = 0;
	uint8_t cc = 0;

	// Adjust the length back by 1 to leave space for the trailing null terminator.
	len--;

	// Process characters until a newline is received.
	while (1) 
	{
		cc = console_getc();

		if (cc == '\b' || cc == CHAR_DEL) 
		{
			 // if the backspace key was pressed.
			 // If there are any characters already in the buffer, then delete the last.
			if (count) {
				uart_write(port, (uint8_t *)"\b \b", 3);
				count--;
			}

			continue;
		}

		// If this character is LF and last was CR, then just gobble up the
		// character because the EOL processing was taken care of with the CR.
		if ((cc == '\n') && last_was_cr) {
			last_was_cr = 0;
			continue;
		}

		//  See if a newline or escape character was received.
		if ((cc == '\r') || (cc == '\n') || (cc == 0x1b)) 
		{
			// If the character is a CR, then it may be followed by a LF which
			//  should be paired with the CR.	So remember that a CR was received.
			if (cc == '\r') 
			{
				last_was_cr = 1;
			}

			break;
		}

		// Process the received character as long as we are not at the end of
		// the buffer.	If the end of the buffer has been reached then all
		// additional characters are ignored until a newline is received.
		if (count < len) 
		{
			buf[count] = cc;
			count++;

			// echo on to user
			uart_putc(port, cc);
		}
	}

	// Add a null termination to the string.
	buf[count] = 0;

	// Send a CRLF pair to the terminal to end the line.
	uart_write(port, (uint8_t *)"\r\n", 2);

	return count;
}

int console_printf(const char* fmt, ...)
{
	va_list args;
	uint32_t i = 0;

	va_start(args, fmt);

	// For this to work, printbuffer must be larger than
	// anything we ever want to print.
	i = vsnprintf((char *)console_buf, (CONSOLE_BUF_SIZE - 1), fmt, args);
	va_end(args);

	// Print the string
	console_buf[i] = 0;
	console_puts(console_buf);
	
	return i;
}

int console_check_char(uint8_t check)
{
	uint8_t cc = 0;

	if (console_tstc()) 
	{
		cc = console_getc();

		if (cc == check) 
		{
			return 1;
		}
	}
	
	return 0;
}

