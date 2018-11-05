/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_gpio.h"

#if (CONFIG_GPIO_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
cmdline_entry gpio_test_menu[];

// for interrupt handler
volatile static int gpio_int_counter = 0;
static E_GPIO_PORT gpio_port = 0;
static int gpio_pin = 0;
static uint32_t g_gpio_int_flag = 0;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/
static uint32_t gpio_vec(E_GPIO_PORT port, E_PIN_PORT pin);

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
static int cmd_help(int argc, char *argv[])
{
	return cmdline_help_general(&gpio_test_menu[0]);
}

E_GPIO_PORT gpio_get_port(char *argv)
{
	E_GPIO_PORT port = GPIO0;

	if ((!strcmp(argv, "e")) || (!strcmp(argv, "E")))
	{
		port = GPIO0;
	}
	else if ((!strcmp(argv, "f")) || (!strcmp(argv, "F")))
	{
		port = GPIO0;
	}
	else
	{
		port = get_arg_ulong(argv);
	}

	if (port >= NUM_GPIO_PORTS)
	{
		info("invalid, default to porta\n");
		port = GPIO0;
	}

	return port;
}

static int gpio_output(int argc, char *argv[])
{
	uint32_t val = 0;
	E_GPIO_PORT port = GPIO0;
	E_PIN_PORT pin = PIN_0;

	info("%s:output test\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 4) 
	{
		info("wrong cmd_format: %s port pin val\n", argv[0]);
		return 0;
	}

	// Get the parameters
	port = gpio_get_port(argv[1]); 
	pin = get_arg_ulong(argv[2]);
	val = get_arg_ulong(argv[3]);

	info("port:%d, pin: %d, val:%d\n", port, pin, val);

	// initialize gpio pin
	gpio_set_ctrl(port, pin, GPIO_CTRL_SW);
	gpio_set_ie(port, pin, GPIO_IE_DIS);
	gpio_set_ic(port, pin);	
	gpio_set_dir(port, pin, GPIO_OUTPUT);

	// output level on the pin
	gpio_set_output(port, pin, val);

	info("%s: set gpio port(%x), pin(%x) to level '%s'\n", argv[0], port, pin, val ? "high" : "low");
	info("%s: view the oscilloscope or multimeter result for pass or fail\n", argv[0]);

	return 0;
}

static int gpio_input(int argc, char *argv[])
{
	uint32_t gpio_val = 0, input_val = 0;
	E_GPIO_PORT port = GPIO0;
	E_PIN_PORT pin = PIN_0;

	info("%s:input test\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 4) 
	{
		info("wrong cmd_format: %s port pin val\n", argv[0]);
		return 0;
	}
	
	// Get the parameters
	port = gpio_get_port(argv[1]);
	pin = get_arg_ulong(argv[2]);
	input_val = get_arg_ulong(argv[3]);
	if (input_val) 
	{
		input_val = 1;
	}

	// initialize gpio pin
	gpio_set_ctrl(port, pin, GPIO_CTRL_SW);
	gpio_set_ie(port, pin, GPIO_IE_DIS);
	gpio_set_ic(port, pin);
	gpio_set_dir(port, pin, GPIO_INPUT);

	gpio_val = gpio_get_input(port, pin);
	debug("gpio input: expect %d act %d\n", input_val, gpio_val);

	if (gpio_val != input_val) 
	{
		info("%s: test fail\n", argv[0]);
	} 
	else 
	{
		info("%s: test pass\n", argv[0]);
	}
	
	return 0;
}

static uint32_t gpio_vec(E_GPIO_PORT port, E_PIN_PORT pin)
{
	uint32_t vec = 0;

	vec = INT_GPIO;

	info("vec:%d\n", vec);

	return vec;
}

void gpio_int_handler(void *ptr)
{
	bool ret_val;
	E_GPIO_PORT port = gpio_port;
	E_PIN_PORT pin = gpio_pin;
	struct brite_gpio *priv = ptr;

	//info("gpio_int_handler\n");
	
	ret_val = gpio_get_mis(port, pin);
	if (ret_val) 
	{
		gpio_int_counter++;
		gpio_set_ic(port, pin);
	}

	info("GPIO%d: gpio_int_handler, base=0x%x status=%x count=%d\n", priv->port, priv->base, ret_val, gpio_int_counter);
	
	// for level interrupt, disable int to prevento much int occur
	if(console_check_char('q') || gpio_int_counter > 20)
	{
		g_gpio_int_flag = 1;
		gpio_set_ie(port, pin, GPIO_IE_DIS);
	}
}

// testcase: gpio interrupt type test
static int gpio_int_type(int argc, char *argv[])
{
	uint32_t fail = 0;
	int32_t int_type = 0;
	E_GPIO_PORT port = GPIO0;
	E_PIN_PORT pin = PIN_0;
	E_GPIO_IS is;
	E_GPIO_IT it;
	uint32_t vec = 0;
	struct brite_gpio *priv = NULL;

	info("%s: gpio interrupt type test,input 'q' to quit this testcase\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 5) 
	{
		info("wrong cmd_format: %s port pin is(level/edge) itype((high/low)/(fall/rise/both))\n", argv[0]);
		return 0;
	}

	// Get the parameters
	port = gpio_get_port(argv[1]); 
	pin = get_arg_ulong(argv[2]);
	if (!strcmp(argv[3], "level")) 
	{
		is = GPIO_IS_LEVEL;
	} 
	else if (!strcmp(argv[3], "edge")) 
	{
		is = GPIO_IS_EDGE;
	} 
	else 
	{
		is = GPIO_IS_LEVEL;
		info("wrong interrupt type, default as level\n");
	}

	// get interrupt type
	if (is == GPIO_IS_EDGE) 
	{
		// edge
		if      (!strcmp(argv[4], "fall")) 	it = GPIO_INT_EDGE_FALLING;
		else if (!strcmp(argv[4], "rise")) 	it = GPIO_INT_EDGE_RISING;
		else								it = GPIO_INT_EDGE_BOTH;  
	} 
	else
	{
		// level
		if (!strcmp(argv[4], "low"))		it = GPIO_INT_LEVEL_LOW;
		else								it = GPIO_INT_LEVEL_HIGH;
	}

	// initialize gpio pin
	priv = &g_gpio[port];

	priv->port = port;
	gpio_set_ctrl(port, pin, GPIO_CTRL_SW);
	gpio_set_ie(port, pin, GPIO_IE_DIS);
	gpio_set_ic(port, pin);
	gpio_set_dir(port, pin, GPIO_INPUT);

	gpio_int_counter = 0;
	gpio_port = port;
	gpio_pin = pin;	
	g_gpio_int_flag = 0;

	// register int handler and enable it
	vec = gpio_vec(port, pin);
	
	if (is == GPIO_IS_EDGE) 
	{
		if      (it == GPIO_INT_EDGE_FALLING) 	int_type = IRQ_TYPE_EDGE_FALLING;
		else if (it == GPIO_INT_EDGE_RISING) 	int_type = IRQ_TYPE_EDGE_RISING;
		else 									int_type = IRQ_TYPE_EDGE_BOTH;
	}
	else 
	{
		if (it == GPIO_INT_LEVEL_LOW)			int_type = IRQ_TYPE_LEVEL_LOW;
		else									int_type = IRQ_TYPE_LEVEL_HIGH;
	}

	info("GPIO%d: base=0x%x int_vector=%d\n", port, g_gpio[port].base, vec);
	irq_generic_install_handler(vec, gpio_int_handler, priv);
	irq_generic_config_int(vec, int_type);
	irq_generic_enable_int(vec);
	_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);

	// enable interrupt
	gpio_set_int_type(port, pin, is, it);
	gpio_set_ie(port, pin, GPIO_IE_EN);

	// wait interrupt handle, and wait user break testcase
	while (g_gpio_int_flag == 0) 
	{
		if(console_check_char('q'))
			break;
	}

	if (gpio_int_counter == 0) 
	{
		fail++;
	}

	if (fail) 
	{
		info("%s: test fail\n", argv[0]);
	} 
	else 
	{
		info("%s: test pass\n", argv[0]);
	}

	// disable interrupt
	irq_generic_disable_int(vec);
	irq_generic_remove_handler(vec, NULL, NULL);
	gpio_set_ie(port, pin, GPIO_IE_DIS);

	return 0;
}

int gpio_test_prepare(int sel)
{
	E_GPIO_PORT port = sel;
	
	gpio_init(sel, (GPIO_BASE + (sel * GPIO_PORT_SIZE)));

	// set pinmux
	//pad_gpio0_pinmux(PINMUX_FUNC0, 0);		// gpio 0
	//pad_gpio1_pinmux(PINMUX_FUNC0, 0);		// gpio 1
	//pad_spicsn_pinmux(PINMUX_FUNC2, 0);		// gpio 3
	//pad_dmic_clk11_pinmux(PINMUX_FUNC3, 0);	// gpio 3
	//pad_i2csscl_pinmux(PINMUX_FUNC2, 0);	// gpio 5
	//pad_i2cssda_pinmux(PINMUX_FUNC2, 0);	// gpio 6
	//pad_pwm0_pinmux(PINMUX_FUNC2, 0);	// gpio 7
	//pad_pwm1_pinmux(PINMUX_FUNC3, 0);	// gpio 2
	//pad_pwm2_pinmux(PINMUX_FUNC2, 0);	// gpio 6
	
#if 1	
	//pad_dmic_clk11_pinmux(PINMUX_FUNC3, 0);
	//pad_dmic_clk12_pinmux(PINMUX_FUNC3, 0);
	//pad_dmic_in1_pinmux(PINMUX_FUNC3, 0);
	//pad_dmic_clk21_pinmux(PINMUX_FUNC3, 0);

	//0716 
	//pad_pwm0_pinmux(PINMUX_FUNC2, 0);			// GPIO 7
	//pad_dmic_in2_pinmux(PINMUX_FUNC1, 0);		// GPIO 5	
	


	pad_dmic_clk21_pinmux(PINMUX_FUNC4, 0);		// GPIO 4
	pad_dmic_clk22_pinmux(PINMUX_FUNC4, 0);		// GPIO 5
	pad_dmic_in2_pinmux(PINMUX_FUNC3, 0);		// GPIO 2	
	//pad_i2smic_ssdata_pinmux(PINMUX_FUNC1, 0);	// GPIO 4, don't care, led on board wrong
	pad_dopwmrn_pinmux(PINMUX_FUNC1, 0);		// GPIO 6
#endif

	// check id
	gpio_verify_periph_id(port);
	gpio_verify_pcell_id(port);	
	
	return ERR_TEST_DONE;
}

static int external_pad_output(int argc, char *argv[])
{
	uint32_t val = 0;
	uint8_t pin = 0;

	info("%s:output test\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("wrong cmd_format: %s pin val\n", argv[0]);
		return 0;
	}

	// Get the parameters
	pin = get_arg_ulong(argv[1]);
	val = get_arg_ulong(argv[2]);

	info("pin: %d, val:%d\n", pin, val);

	// output level on the pin
	sys_external_pad_output(pin, val);

	info("%s: set gpio pin(%x) to level '%s'\n", argv[0], pin, val ? "high" : "low");
	info("%s: view the oscilloscope or multimeter result for pass or fail\n", argv[0]);

	return 0;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry gpio_test_menu[] = {
	{"help", 			cmd_help,		"           : Display list of commands"},
	{"h",				cmd_help, 		"              : alias for help"},
	{"?",				cmd_help, 		"              : alias for help"},
	{"gpio_output",		gpio_output,	"    : gpio output test"},
	{"gpio_input",		gpio_input,		"     : gpio input test"},
	{"gpio_int_type",	gpio_int_type,	"  : gpio interrupt type test"},
	{"external_pad_output",	external_pad_output,	"  : external pad output test"},
	{0, 				0, 				0}
};

#endif

