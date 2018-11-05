/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                               											 		 */
/*           Copyright (C) 2016 Brite Semiconductor Co., Ltd. All rights reserved.                									 */
/*                                                                                                													 */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_usb.h"
#include "cdl_sys.h"
#include "cdl_i2s.h"
#include "cdl_dac.h"
#include "musb_core.h"

#if (CONFIG_USB_TEST == 1)

struct musb_controller usb_dev;
struct musb_controller* p_usb = &usb_dev;

cmdline_entry usb_test_menu[];

void dma_int_i2s_handler(void* ptr);
int i2s_test_prepare(int port);
int dac_test_prepare(int port);

extern struct musb *musb_gadget;

static int cmd_help(int argc, char* argv[])
{
	return cmdline_help_general(&usb_test_menu[0]);
}

/**
 * @brief usb_dev_uac2 - controller works as an audio sound and PC do read and write test. 
 *
 * @return directly if failed
 */
static int usb_loopback_test(int argc, char* argv[])
{
	int ret;
	int dma_mode, polling_mode;

	if (argc < 3) {
		info(
			"wrong cmd_format: %s dma_mode polling,'ctrl C' to quit\n",
			argv[0]);
		info("test steps:\n");
		info("1. run usb_loopback_test dma_mode(0 or 1) polling(0 or 1)\n");
		info("2. Connect the EVB to linux host by USB cable\n");
		info("3. Linux host plays one wav file, USB will transfer data to device memory\n");
		info("	 command: aplay -D hw:1,0 a.wav\n");
		info("4. Linux host reads back USB memory data\n");
		info("	 command: arecord -D hw:1,0 -t wav -c 2 -r 48000 -f S16_LE a_readback.wav\n");
		info("5. comprare a.wav and a_readback.wav by yourself\n");
		return 0;
	}
	dma_mode = get_arg_ulong(argv[1]);
	polling_mode = get_arg_ulong(argv[2]);

	p_usb->edma_mode = (dma_mode) ? 1 : 0;
	p_usb->polling_mode = (polling_mode) ? 1 : 0;
	p_usb->store = 1;

	ret = usb_dev_uac2_init(p_usb);
	if (ret) {
		info("usb dev init fail\n");
		return 0;
	}
	/* enable usb audio class*/
	/*do usb process*/
	ret = usb_dev_uac2(p_usb->idx);
	if (ret) {
		info("usb process ret %d\n", ret);
	}
	usb_dev_uac2_exit();
	return 0;
}


static int i2stx_master_set(E_DT_MODE dt_mode, int dma_irq_en, E_I2S_TYPE type, E_I2S_PCM_FORMAT sample_format)
{
	struct musb *musb = musb_gadget;
	struct brite_i2s *p_i2s = musb->p_i2s;
	E_I2S_PORT port = p_i2s->port;
	E_PI_MODE pi_mode = PI_POLLING;
	E_I2S_DAC_DATA_MODE dac_data_mode = I2S_DAC_DATA_TO_CODEC;

	struct snd_wm8766 *wm = &g_wm[port-1];
	int wm_format = WM8766_IF_IWL_16BIT;

	uint32_t len;
	uint32_t max_len = TEST_BUFFER_SIZE;  //__48k_s16_3s_raw_len; //TEST_BUFFER_SIZE/2;
	//uint32_t *p_tx = (uint32_t *)test_txrx_buff;  //__48k_s16_3s_raw;
	int i2s_role = I2S_MASTER ; //I2S_MASTER;I2S_SLAVE
	p_i2s->type = type;
	p_i2s->sample_format = sample_format;

	len = max_len;

	i2s_test_prepare(port);
	dac_test_prepare(port-1);

	if (sample_format == 16)
		wm_format = WM8766_IF_IWL_16BIT;
	else if (sample_format == 24)
		wm_format = WM8766_IF_IWL_24BIT;
	else if (sample_format == 32)
		wm_format = WM8766_IF_IWL_24BIT;
	else
		return -1;

	snd_wm8766_init(wm);
	snd_wm8766_set_if(wm, wm_format | WM8766_IF_FMT_I2S);
	if (i2s_role == I2S_SLAVE) {
		snd_wm8766_set_role(wm, 1);
		if(snd_wm8766_master_lrclk_div(wm, 128))
			return -1;
	}
    snd_wm8766_set_volume(wm, WM8766_CH1, 50, 50);
    snd_wm8766_playback_switch(wm, WM8766_CH1, 1);
    //snd_wm8766_playback_phase_inv(wm, WM8766_CH1, 1);
    //snd_wm8766_playback_de_emph(wm, WM8766_CH1, 1);
    //snd_wm8766_clk_polarity(wm, 1, 1);

	// configure master clock
    /* maybe div need to be multiple of 8 or 16, div = MCLK/sample_rate */
	//i2s_dac_clk_congig(4, sample_format*2, I2S_SLAVE, dac_data_mode);
	if (port == I2S_DAC)
		i2s_dac_clk_config(sample_format*2, i2s_role, dac_data_mode);

	if (port == I2S_1)
		i2s1_master_clk_config((sample_format*2), type, sample_format);

	// initialize the spi hw
	i2s_init(port, 0, type, 0, 0, 0, dt_mode, pi_mode, sample_format, 1, 0, 0);
	p_i2s->dma_irq_en = dma_irq_en;

    // register i2s dma done interrupt
    if (dt_mode == DT_DMA && dma_irq_en) {
		/*register int handler and enable it*/
		irq_generic_install_handler(INT_DMAC,
			dma_int_i2s_handler,
			musb);
		irq_generic_config_int(INT_DMAC, IRQ_TYPE_LEVEL_HIGH);
		irq_generic_enable_int(INT_DMAC);
		_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);
		info("DMA%d: int_vector=%d\n", DMA0, INT_DMAC);
	}

	return 0;	
}

static int usb_playback(int argc, char* argv[])
{
	int ret;
	int usb_dma_mode, usb_polling_mode, i2s_tx_port, i2s_dma_mode;
	int sample_format, i2s_dma_irq_en;

	if (argc < 7) {
		info(
			"wrong cmd_format: %s usb_dma_mode usb_polling_mode i2s_tx_port sample_format i2s_dma_mode i2s_dma_irq_en,'ctrl C' to quit\n",
			argv[0]);
		info("test steps:\n");
		info("1. run %s usb_dma_mode(0|1) usb_polling_mode(0|1) i2s_tx_port(1|2) sample_format(16|32) i2s_dma_mode(0|1) i2s_dma_irq_en\n", argv[0]);
		info("i2s_tx_port: 1 I2S_DAC; 2 I2S_1;\n");
		info("2. Connect the EVB to linux host by USB cable\n");
		info("3. Linux host plays one wav file, USB will transfer data to device and playback through I2Stx\n");
		info("	 command: aplay -D hw:1,0 a.wav\n");
		return 0;
	}
	usb_dma_mode = get_arg_ulong(argv[1]);
	usb_polling_mode = get_arg_ulong(argv[2]);
	i2s_tx_port = get_arg_ulong(argv[3]);
	sample_format = get_arg_ulong(argv[4]);
	i2s_dma_mode = get_arg_ulong(argv[5]);
	i2s_dma_irq_en = get_arg_ulong(argv[6]);

	p_usb->edma_mode = (usb_dma_mode) ? 1 : 0;
	p_usb->polling_mode = (usb_polling_mode) ? 1 : 0;
	p_usb->i2s_tx_port = i2s_tx_port;
	p_usb->store = 0;

	ret = usb_dev_uac2_init(p_usb);
	if (ret) {
		info("usb dev init fail\n");
		return 0;
	}
	i2stx_master_set(i2s_dma_mode, i2s_dma_irq_en, I2S_STANDARD, sample_format);

	/* enable usb audio class*/
	/*do usb process*/
	ret = usb_dev_uac2(p_usb->idx);
	if (ret) {
		info("usb process ret %d\n", ret);
	}

    // register i2s dma done interrupt
    if (i2s_dma_mode == DT_DMA && i2s_dma_irq_en) {
		/*disable interrupt*/
		irq_generic_disable_int(INT_DMAC);
		irq_generic_remove_handler(INT_DMAC,NULL,NULL);
		_SET_SYSCON_REG_interrupt_control_interrupt_en_227(0);
	}
	usb_dev_uac2_exit();
	return 0;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry usb_test_menu[] = {
	{ "help", 	cmd_help,  " : Display list of commands" },
	{ "h",		cmd_help, 	"    : alias for help" },
	{ "?",		cmd_help, 	"    : alias for help" },
	{ "usb_loopback_test",usb_loopback_test, 	 "   :usb audio class 2.0 rx and tx test" },
	{ "usb_playback",usb_playback, 	 "   :usb rx I2S playback test" },
	{ 0, 0, 0 }
};

/* if return none zero, something wrong,please check and can't continue test*/
int usb_test_prepare(int sel)
{
	int ret = 0;

	mem_set8(p_usb, 0, sizeof(struct musb_controller));

	if (sel == 0) 
	{
		p_usb->regs = (void*)USBC_BASE;
		p_usb->int_vector = INT_USB;
		p_usb->int_dma_vector = INT_USB_DMA;
		p_usb->idx = USB0;
		info("%s: USB0_BASE=0x%x, INT_USB0=%d, INT_USB0_DMA=%d\n", __func__, USBC_BASE, INT_USB, INT_USB_DMA);
	} 
	else 
	{
		info("wrong device id\n");
		ret = 1;
	}

	return ret;
}
#endif
