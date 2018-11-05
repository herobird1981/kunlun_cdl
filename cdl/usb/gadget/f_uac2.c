/*
 * f_uac2.c -- USB Audio Class 2.0 Function
 *
 * Copyright (C) 2011
 *    Yadwinder Singh (yadi.brar01@gmail.com)
 *    Jaswinder Singh (jaswinder.singh@linaro.org)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <errno.h>
#include <common.h>
//#include <malloc.h>

#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/composite.h>

#include <linux/usb/audio.h>
#include <linux/usb/audio-v2.h>

#include <g_audio.h>

#include "f_uac2.h"
#include "memleak.h"
#include "cdl_i2s.h"
#include "cdl_dac.h"
#include "musb_core.h"

static int generic_set_cmd(struct usb_audio_control *con, u8 cmd, int value);
static int generic_get_cmd(struct usb_audio_control *con, u8 cmd, int *data);

#define REQ_FIX_MEM_ADDR

#ifdef REQ_FIX_MEM_ADDR
#define TEST_REQ_TX_BUF	(SRAM3_BASE + 0x10000)
#define TEST_REQ_RX_BUF	(SRAM3_BASE + 0x14000)
#endif

#define STORE_AUDIO_TO_DDR
uint8_t *test_audio_buff = test_txrx_buff;
#define DEV_AUDIO_MAX_LEN      TEST_BUFFER_SIZE    //(60*1024*1024) /*60M*/

/* Keep everyone on toes */
#define USB_XFERS	2

/*
 * The driver implements a simple UAC_2 topology.
 * USB-OUT -> IT_1 -> OT_3 -> ALSA_Capture
 * ALSA_Playback -> IT_2 -> OT_4 -> USB-IN
 * Capture and Playback sampling rates are independently
 *  controlled by two clock sources :
 *    CLK_5 := c_srate, and CLK_6 := p_srate
 */
#define USB_OUT_IT_ID	1
#define IO_IN_IT_ID	2
#define IO_OUT_OT_ID	3
#define USB_IN_OT_ID	4
#define USB_OUT_CLK_ID	5
#define USB_IN_CLK_ID	6

#define IO_OUT_FU_ID	7
#define IO_IN_FU_ID	    8

#define CONTROL_ABSENT	0
#define CONTROL_RDONLY	1
#define CONTROL_RDWR	3

#define CLK_FREQ_CTRL	0
#define CLK_VLD_CTRL	2

#define COPY_CTRL	0
#define CONN_CTRL	2
#define OVRLD_CTRL	4
#define CLSTR_CTRL	6
#define UNFLW_CTRL	8
#define OVFLW_CTRL	10

#define UAC2_FU_MUTE_SHIFT		0x0
#define UAC2_FU_VOLUME_SHIFT	0x2
#define VOLUME_MIN	0x8001
#define VOLUME_STEPS	0x0120

//static const char *uac2_name = "snd_uac2";
#define UAC2_RX_BUF_SIZE   (768*1024)
#define I2S_TX_BUF_CNT		2
#define UAC2_RX_BUF_CNT     2
#if UAC2_RX_BUF_CNT > I2S_TX_BUF_CNT
#  error uac2 rx buf can not be larger than i2s tx buf
#endif


#define DMA_I2S_TX_IRQ_TRIG        0
#define DMA_I2S_TX_SCHED_BY_UAC2   1
#define DMA_I2S_TX_WAIT_UAC2_DATA  2

extern struct musb *musb_gadget;

volatile struct uac2_playback_stat ao_stat;

struct uac2_playback_stat {
	int usb_underrun;
	int usb_overrun;
	int i2s_underrun;

	volatile unsigned int uac2_rx_buf_ptr;
	volatile unsigned int uac2_rx_buf_size;
	volatile unsigned int i2s_playback_size;
	
	volatile unsigned int i2s_playback_ptr;

	volatile int dma_i2s_tx_sched;
	volatile int uac2_rx_buf_full;
	
	volatile unsigned long uac2_rxed_size;
	volatile unsigned long i2s_txed_size;
};

struct uac2_req {
	struct uac2_rtd_params *pp; /* parent param */
	struct usb_request *req;
};

struct uac2_rtd_params {
	struct snd_uac2_chip *uac2; /* parent chip */
	bool ep_enabled; /* if the ep is enabled */
	/* Size of the ring buffer */
	size_t dma_bytes;
	unsigned char *dma_area;

	/* Ring buffer */
	ssize_t hw_ptr;

	void *rbuf;

	size_t period_size;

	unsigned max_psize;
	struct uac2_req ureq[USB_XFERS];

	spinlock_t lock;
};

struct snd_uac2_chip {
	struct uac2_rtd_params p_prm;
	struct uac2_rtd_params c_prm;

	//struct snd_card *card;
	//struct snd_pcm *pcm;

	/* timekeeping for the playback endpoint */
	unsigned int p_interval;
	unsigned int p_residue;

	/* pre-calculated values for playback iso completion */
	unsigned int p_pktsize;
	unsigned int p_pktsize_residue;
	unsigned int p_framesize;

	unsigned int rx_buf_offset;
	unsigned int tx_buf_offset;
};

struct audio_dev {
	struct usb_gadget	*gadget;
	u8 ac_intf, ac_alt;
	u8 as_out_intf, as_out_alt;
	u8 as_in_intf, as_in_alt;
	int ctl_id;
	int cs_id;

	struct usb_ep *in_ep, *out_ep;
	struct usb_function func;

	/* Control Set command */
	struct list_head cs;
	u8 set_cmd;
	struct usb_audio_control *set_con;

	/* The ALSA Sound Card it represents on the USB-Client side */
	struct snd_uac2_chip uac2;

	int 			p_chmask;
	int 			p_srate[UAC_MAX_ITEMS]; /* rate in Hz */
	int 			p_srate_active; 	/* selected rate in Hz */
	int 			p_ssize;
	int 			c_chmask;
	int 			c_srate[UAC_MAX_ITEMS]; /* rate in Hz */
	int 			c_srate_active; 	/* selected rate in Hz */
	int 			c_ssize;
};

static int c_srate[UAC_MAX_ITEMS] = {    //max fifo:256, ch:2, bit:16, so max rate:64k
	12000, 16000, 24000, 32000, 48000, 64000,
};

static int p_srate[UAC_MAX_ITEMS] = {
	11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000,
};

static int c_volume[UAC_MAX_ITEMS] = {
	VOLUME_MIN,
};


void dma_int_i2s_handler(void* ptr);
int i2s_xfer_tx_async(E_I2S_PORT port, void *data, uint32_t len);
E_DMA_ST dma_i2s_tx_is_done(E_DMA_PORT dma_port, int i2s_port);

void print_audio_stat(volatile struct uac2_playback_stat *stat) {
	info("\nusb underrun %d\n", stat->usb_underrun);
	info("usb overrun %d\n", stat->usb_overrun);
	info("i2s underrun %d\n\n", stat->i2s_underrun);
}


void * uac2_rx_buf_alloc(unsigned int size) {
	void * buf = test_txrx_buff + ao_stat.uac2_rx_buf_ptr;
	//if (uac2_rx_buf_full)
	//	return test_txrx_buff+uac2_rx_buf_size-size;
	ao_stat.uac2_rx_buf_ptr = (ao_stat.uac2_rx_buf_ptr+size) % ao_stat.uac2_rx_buf_size;
	ao_stat.uac2_rx_buf_ptr &= ~0xF;
	//if (uac2_rx_buf_ptr == 0)
	//	uac2_rx_buf_full = 1;
	return buf;
}

void uac2_rx_buf_init(unsigned int xfer_size)
{
	print_audio_stat(&ao_stat);
	memset((void *)&ao_stat, 0, sizeof(ao_stat));
	info("uac2/i2s ptr 0x%x / 0x%x total 0x%x / 0x%x\n",
			ao_stat.uac2_rx_buf_ptr, ao_stat.i2s_playback_ptr, ao_stat.uac2_rxed_size, ao_stat.i2s_txed_size);
	ao_stat.uac2_rx_buf_ptr = 0;
	ao_stat.i2s_playback_ptr = 0;
	ao_stat.uac2_rxed_size = 0;
	ao_stat.i2s_txed_size = 0;
	ao_stat.uac2_rx_buf_size = (UAC2_RX_BUF_SIZE/xfer_size)*xfer_size;
	info("uac2 rx buf 0x%x/0x%x/0x%x\n", xfer_size, ao_stat.uac2_rx_buf_ptr, ao_stat.uac2_rx_buf_size);
	ao_stat.dma_i2s_tx_sched = DMA_I2S_TX_SCHED_BY_UAC2;
	ao_stat.i2s_playback_size = ao_stat.uac2_rx_buf_size/I2S_TX_BUF_CNT;
}

static inline
struct audio_dev *func_to_agdev(struct usb_function *f)
{
	return container_of(f, struct audio_dev, func);
}

static inline
struct audio_dev *uac2_to_agdev(struct snd_uac2_chip *u)
{
	return container_of(u, struct audio_dev, uac2);
}

static inline
uint num_channels(uint chanmask)
{
	uint num = 0;

	while (chanmask) {
		num += (chanmask & 1);
		chanmask >>= 1;
	}

	return num;
}

static void
agdev_iso_complete(struct usb_ep *ep, struct usb_request *req)
{
	int status = req->status;
	struct uac2_req *ur = req->context;
	struct uac2_rtd_params *prm = ur->pp;
	struct snd_uac2_chip *uac2 = prm->uac2;


	/* i/f shutting down */
	if (!prm->ep_enabled || req->status == -ESHUTDOWN)
		return;

	/*
	 * We can't really do much about bad xfers.
	 * Afterall, the ISOCH xfers could fail legitimately.
	 */
	if (status) {
		++ao_stat.usb_underrun;
		//dev_info(NULL, "%s: iso_complete status(%d) %d/%d\n",
		//	__func__, status, req->actual, req->length);
		//info("ST %d\n", status);
	}

	/* write rx ep_out data to ddr */
	if (!(ep->address&0x80)) {
		if (musb_gadget->store) {
			dev_dbg(NULL, "iso_complete_rx: rx_buf_offset %d\n", uac2->rx_buf_offset);
			if ((uac2->rx_buf_offset + req->actual) <= DEV_AUDIO_MAX_LEN) {
				dev_dbg(NULL, "iso_complete_rx: rx_buf_offset %d, buf:%p, actual:%d\n", uac2->rx_buf_offset, req->buf, req->actual);
				memcpy(test_audio_buff + uac2->rx_buf_offset, req->buf, req->actual);

				//print_hex("RF2", 1, test_audio_buff + uac2->rx_buf_offset, req->actual);
				uac2->rx_buf_offset += req->actual;
			
			} else {
				printf("rx data overflow ddr, giveup store\n");
			}
		} else {
			if (musb_gadget->p_i2s->dt_mode) {
				if (musb_gadget->p_i2s->dma_irq_en) {
					ao_stat.uac2_rxed_size += req->length;
					//int dma_done = dma_i2s_tx_is_done(DMA0, I2S_DAC);
					//info("$D%d ", dma_done);

					//info("RD %p L %d\n", req->buf, req->length);
					if (ao_stat.dma_i2s_tx_sched == DMA_I2S_TX_SCHED_BY_UAC2)
						dma_int_i2s_handler(musb_gadget);

					if (ao_stat.dma_i2s_tx_sched == DMA_I2S_TX_WAIT_UAC2_DATA && ao_stat.uac2_rxed_size > ao_stat.i2s_txed_size)
						ao_stat.dma_i2s_tx_sched = DMA_I2S_TX_SCHED_BY_UAC2;
					if (ao_stat.uac2_rxed_size < ao_stat.i2s_txed_size + ao_stat.i2s_playback_size*UAC2_RX_BUF_CNT) {
						req->buf = uac2_rx_buf_alloc(req->length);
						//info("*SA ");
						//info("uac2 inc alloc req 0x%x 0x%x 0x%x\n", uac2_rxed_size, i2s_txed_size, i2s_txed_size+i2s_playback_size);
					} else {
						++ao_stat.usb_overrun;
						ao_stat.uac2_rxed_size -= req->length;
						//info("*S%x/%x ", uac2_rxed_size, i2s_txed_size);
						//info("uac2 drop this packet 0x%x 0x%x 0x%x\n", uac2_rxed_size, i2s_txed_size, i2s_txed_size+i2s_playback_size);
					}
				} else {
					i2s_xfer_tx_async(musb_gadget->p_i2s->port, req->buf, req->actual);
				}
			} else {
				i2s_xfer_tx_async(musb_gadget->p_i2s->port, req->buf, req->actual);
			}
		}
	}

	if (musb_gadget->store) {
		/* memcpy tx ep_in from ddr */
		if (ep->address&0x80) {
			dev_dbg(NULL, "iso_complete_tx: tx/rx_buf_offset %d %d\n", uac2->tx_buf_offset, uac2->rx_buf_offset);
			if (uac2->tx_buf_offset <= uac2->rx_buf_offset) {
				memcpy(req->buf, test_audio_buff + uac2->tx_buf_offset, req->actual);
				uac2->tx_buf_offset += req->actual;
			} else {
				if ((uac2->tx_buf_offset - uac2->rx_buf_offset) < req->actual * 3)
					dev_info(NULL, "tx data big than last rx data\n");
				memset(req->buf, 0x55, req->actual);
				uac2->tx_buf_offset += req->actual;
			}
		}
	}
	if (usb_ep_queue(ep, req, GFP_ATOMIC))
		printf("%d Error!\n", __LINE__);

	return;
}


void dma_int_i2s_handler(void* ptr)
{
	//u32 int_status,  int_com_status = 0;
	//unsigned long ch_status;
	struct musb	*musb = ptr;
	struct brite_i2s *p_i2s = musb->p_i2s;

	int dma_done = dma_i2s_tx_is_done(p_i2s->dma_core_idx, p_i2s->port);
	//info("#Ii%d/%d ", dma_done, dma_i2s_tx_sched);
	if (dma_done || ao_stat.dma_i2s_tx_sched == DMA_I2S_TX_SCHED_BY_UAC2)
	{
		//info("i2s dac dma_done %d tx_sched %d, playback ptr 0x%x uac2 ptr 0x%x.\n",
		//	dma_done, dma_i2s_tx_sched, i2s_playback_ptr, uac2_rx_buf_ptr);
		//info("0x%x/0x%x\n", i2s_playback_ptr, uac2_rx_buf_ptr);
		//if (dma_done)
		//	info("$d");
		//if (dma_i2s_tx_sched)
		//	info("^S%d", dma_i2s_tx_sched);
		if(dma_done) {
			ao_stat.i2s_txed_size += ao_stat.i2s_playback_size;
		//	i2s_stop(&g_i2s[I2S_DAC]);
		}
		if (ao_stat.uac2_rxed_size <= ao_stat.i2s_txed_size) {
			ao_stat.dma_i2s_tx_sched = DMA_I2S_TX_WAIT_UAC2_DATA;
			++ao_stat.i2s_underrun;
			if (ao_stat.i2s_underrun % 8 == 0)
				info("uac2/i2s ptr 0x%x / 0x%x total 0x%x / 0x%x\n",
						ao_stat.uac2_rx_buf_ptr, ao_stat.i2s_playback_ptr, ao_stat.uac2_rxed_size, ao_stat.i2s_txed_size);
			//info("uac2 < i2s, 0x%x < 0x%x\n", uac2_rxed_size, i2s_txed_size);
			//info("#IU ");
		} else {
			i2s_xfer_tx_async(p_i2s->port, test_txrx_buff+ao_stat.i2s_playback_ptr, ao_stat.i2s_playback_size);
			ao_stat.i2s_playback_ptr = (ao_stat.i2s_playback_ptr+ao_stat.i2s_playback_size) % ao_stat.uac2_rx_buf_size;
			ao_stat.dma_i2s_tx_sched = DMA_I2S_TX_IRQ_TRIG;
			//info("#T ");
		}
	}
	//info("#Io/%d ", dma_i2s_tx_sched);
}


/* --------- USB Function Interface ------------- */

enum {
	STR_ASSOC,
	STR_IF_CTRL,
	STR_CLKSRC_IN,
	STR_CLKSRC_OUT,
	STR_USB_IT,
	STR_IO_IT,
	STR_IO_FU_OUT,
	STR_USB_OT,
	STR_IO_OT,
	STR_AS_OUT_ALT0,
	STR_AS_OUT_ALT1,
	STR_AS_IN_ALT0,
	STR_AS_IN_ALT1,
};

static struct usb_string strings_fn[] = {
	[STR_ASSOC].s = "Source/Sink",
	[STR_IF_CTRL].s = "Topology Control",
	[STR_CLKSRC_IN].s = "Input clock",
	[STR_CLKSRC_OUT].s = "Output clock",
	[STR_USB_IT].s = "USBH Out",
	[STR_IO_IT].s = "USBD Out",
	[STR_IO_FU_OUT].s = "OUT Feature",
	[STR_USB_OT].s = "USBH In",
	[STR_IO_OT].s = "USBD In",
	[STR_AS_OUT_ALT0].s = "Playback Inactive",
	[STR_AS_OUT_ALT1].s = "Playback Active",
	[STR_AS_IN_ALT0].s = "Capture Inactive",
	[STR_AS_IN_ALT1].s = "Capture Active",
	{ },
};

static struct usb_gadget_strings str_fn = {
	.language = 0x0409,	/* en-us */
	.strings = strings_fn,
};

static struct usb_gadget_strings *fn_strings[] = {
	&str_fn,
	NULL,
};

static struct usb_qualifier_descriptor devqual_desc = {
	.bLength = sizeof devqual_desc,
	.bDescriptorType = USB_DT_DEVICE_QUALIFIER,

	.bcdUSB = cpu_to_le16(0x0110),
	.bDeviceClass = USB_CLASS_MISC,
	.bDeviceSubClass = 0x02,
	.bDeviceProtocol = 0x01,
	.bNumConfigurations = 1,
	.bRESERVED = 0,
};

static struct usb_interface_assoc_descriptor iad_desc = {
	.bLength = sizeof iad_desc,
	.bDescriptorType = USB_DT_INTERFACE_ASSOCIATION,

	.bFirstInterface = 0,
	.bInterfaceCount = 3,
	.bFunctionClass = USB_CLASS_AUDIO,
	.bFunctionSubClass = UAC2_FUNCTION_SUBCLASS_UNDEFINED,
	.bFunctionProtocol = UAC_VERSION_2,
};

/* Audio Control Interface */
static struct usb_interface_descriptor std_ac_if_desc = {
	.bLength = sizeof std_ac_if_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 0,
	.bNumEndpoints = 0,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOCONTROL,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Clock source for IN traffic */
static struct uac_clock_source_descriptor in_clk_src_desc = {
	.bLength = sizeof in_clk_src_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC2_CLOCK_SOURCE,
	.bClockID = USB_IN_CLK_ID,
	.bmAttributes = UAC_CLOCK_SOURCE_TYPE_INT_FIXED,
	.bmControls = (CONTROL_RDWR << CLK_FREQ_CTRL),
	.bAssocTerminal = 0,
};

/* Clock source for OUT traffic */
static struct uac_clock_source_descriptor out_clk_src_desc = {
	.bLength = sizeof out_clk_src_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC2_CLOCK_SOURCE,
	.bClockID = USB_OUT_CLK_ID,
	.bmAttributes = UAC_CLOCK_SOURCE_TYPE_INT_FIXED,
	.bmControls = (CONTROL_RDWR << CLK_FREQ_CTRL),
	.bAssocTerminal = 0,
};

/* Input Terminal for USB_OUT */
static struct uac2_input_terminal_descriptor usb_out_it_desc = {
	.bLength = sizeof usb_out_it_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_INPUT_TERMINAL,
	.bTerminalID = USB_OUT_IT_ID,
	.wTerminalType = cpu_to_le16(UAC_TERMINAL_STREAMING),
	.bAssocTerminal = 0,
	.bCSourceID = USB_OUT_CLK_ID,
	.iChannelNames = 0,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
};

/* Input Terminal for I/O-In */
static struct uac2_input_terminal_descriptor io_in_it_desc = {
	.bLength = sizeof io_in_it_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_INPUT_TERMINAL,
	.bTerminalID = IO_IN_IT_ID,
	.wTerminalType = cpu_to_le16(UAC_INPUT_TERMINAL_UNDEFINED),
	.bAssocTerminal = 0,
	.bCSourceID = USB_IN_CLK_ID,
	.iChannelNames = 0,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
};

/* Feature Unit for I/O-Out */

static struct uac2_feature_unit_descriptor_0 io_out_fu_desc = {
	.bLength = UAC2_DT_FEATURE_UNIT_SIZE(0),
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_FEATURE_UNIT,
	.bUnitID = IO_OUT_FU_ID,
	.bSourceID = USB_OUT_IT_ID,
	.bmaControls[0] = cpu_to_le32((CONTROL_RDWR << UAC2_FU_MUTE_SHIFT) | (CONTROL_RDWR << UAC2_FU_VOLUME_SHIFT)),
};

/* Ouput Terminal for USB_IN */
static struct uac2_output_terminal_descriptor usb_in_ot_desc = {
	.bLength = sizeof usb_in_ot_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_OUTPUT_TERMINAL,
	.bTerminalID = USB_IN_OT_ID,
	.wTerminalType = cpu_to_le16(UAC_TERMINAL_STREAMING),
	.bAssocTerminal = 0,
	.bSourceID = IO_IN_IT_ID,
	.bCSourceID = USB_IN_CLK_ID,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
};

/* Ouput Terminal for I/O-Out */
static struct uac2_output_terminal_descriptor io_out_ot_desc = {
	.bLength = sizeof io_out_ot_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_OUTPUT_TERMINAL,
	.bTerminalID = IO_OUT_OT_ID,
	.wTerminalType = cpu_to_le16(UAC_OUTPUT_TERMINAL_UNDEFINED),
	.bAssocTerminal = 0,
	.bSourceID = IO_OUT_FU_ID,
	.bCSourceID = USB_OUT_CLK_ID,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
};

static struct uac2_ac_header_descriptor ac_hdr_desc = {
	.bLength = sizeof ac_hdr_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_MS_HEADER,
	.bcdADC = cpu_to_le16(0x200),
	.bCategory = UAC2_FUNCTION_IO_BOX,
	.wTotalLength = sizeof in_clk_src_desc + sizeof out_clk_src_desc
			 + sizeof usb_out_it_desc + sizeof io_in_it_desc
			 + UAC2_DT_FEATURE_UNIT_SIZE(0)
			+ sizeof usb_in_ot_desc + sizeof io_out_ot_desc,
	.bmControls = 0,
};

/* Audio Streaming OUT Interface - Alt0 */
static struct usb_interface_descriptor std_as_out_if0_desc = {
	.bLength = sizeof std_as_out_if0_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 0,
	.bNumEndpoints = 0,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Streaming OUT Interface - Alt1 */
static struct usb_interface_descriptor std_as_out_if1_desc = {
	.bLength = sizeof std_as_out_if1_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 1,
	.bNumEndpoints = 1,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Stream OUT Intface Desc */
static struct uac2_as_header_descriptor as_out_hdr_desc = {
	.bLength = sizeof as_out_hdr_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_AS_GENERAL,
	.bTerminalLink = USB_OUT_IT_ID,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bmFormats = cpu_to_le32(UAC_FORMAT_TYPE_I_PCM),
	.iChannelNames = 0,
};

/* Audio USB_OUT Format */
static struct uac2_format_type_i_descriptor as_out_fmt1_desc = {
	.bLength = sizeof as_out_fmt1_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC_FORMAT_TYPE,
	.bFormatType = UAC_FORMAT_TYPE_I,
};

/* STD AS ISO OUT Endpoint */
static struct usb_endpoint_descriptor fs_epout_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bEndpointAddress = USB_DIR_OUT,
	.bmAttributes = USB_ENDPOINT_XFER_ISOC | USB_ENDPOINT_SYNC_ASYNC,
	.wMaxPacketSize = cpu_to_le16(1023),
	.bInterval = 1,
};

static struct usb_endpoint_descriptor hs_epout_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bmAttributes = USB_ENDPOINT_XFER_ISOC | USB_ENDPOINT_SYNC_ASYNC,
	.wMaxPacketSize = cpu_to_le16(1024),
	.bInterval = 4,
};

/* CS AS ISO OUT Endpoint */
static struct uac2_iso_endpoint_descriptor as_iso_out_desc = {
	.bLength = sizeof as_iso_out_desc,
	.bDescriptorType = USB_DT_CS_ENDPOINT,

	.bDescriptorSubtype = UAC_EP_GENERAL,
	.bmAttributes = 0,
	.bmControls = 0,
	.bLockDelayUnits = 0,
	.wLockDelay = 0,
};

/* Audio Streaming OUT Interface - Alt2 */
static struct usb_interface_descriptor std_as_out_if2_desc = {
	.bLength = sizeof std_as_out_if2_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 2,
	.bNumEndpoints = 1,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Stream OUT Intface Desc */
static struct uac2_as_header_descriptor as_out_hdr2_desc = {
	.bLength = sizeof as_out_hdr2_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_AS_GENERAL,
	.bTerminalLink = USB_OUT_IT_ID,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bmFormats = cpu_to_le32(UAC_FORMAT_TYPE_I_PCM),
	.bNrChannels = 1,
	.bmChannelConfig = cpu_to_le32(0x1),
	.iChannelNames = 0,
};

/* Audio USB_OUT Format */
static struct uac2_format_type_i_descriptor as_out_fmt1_desc2 = {
	.bLength = sizeof as_out_fmt1_desc2,
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC_FORMAT_TYPE,
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bSubslotSize = 4,
	.bBitResolution = 4 * 8,
};

/* Audio Streaming OUT Interface - Alt3 */
static struct usb_interface_descriptor std_as_out_if3_desc = {
	.bLength = sizeof std_as_out_if3_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 3,
	.bNumEndpoints = 1,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Stream OUT Intface Desc */
static struct uac2_as_header_descriptor as_out_hdr3_desc = {
	.bLength = sizeof as_out_hdr3_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_AS_GENERAL,
	.bTerminalLink = USB_OUT_IT_ID,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bmFormats = cpu_to_le32(UAC_FORMAT_TYPE_I_PCM),
	.bNrChannels = 2,
	.bmChannelConfig = cpu_to_le32(0x3),
	.iChannelNames = 0,
};

/* Audio USB_OUT Format */
static struct uac2_format_type_i_descriptor as_out_fmt1_desc3 = {
	.bLength = sizeof as_out_fmt1_desc3,
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC_FORMAT_TYPE,
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bSubslotSize = 4,
	.bBitResolution = 4 * 8,
};

/* Audio Streaming OUT Interface - Alt4 */
static struct usb_interface_descriptor std_as_out_if4_desc = {
	.bLength = sizeof std_as_out_if4_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 4,
	.bNumEndpoints = 1,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Stream OUT Intface Desc */
static struct uac2_as_header_descriptor as_out_hdr4_desc = {
	.bLength = sizeof as_out_hdr4_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_AS_GENERAL,
	.bTerminalLink = USB_OUT_IT_ID,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bmFormats = cpu_to_le32(UAC_FORMAT_TYPE_I_PCM),
	.bNrChannels = 6,
	.bmChannelConfig = cpu_to_le32(0x3F),
	.iChannelNames = 0,
};

/* Audio USB_OUT Format */
static struct uac2_format_type_i_descriptor as_out_fmt1_desc4 = {
	.bLength = sizeof as_out_fmt1_desc4,
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC_FORMAT_TYPE,
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bSubslotSize = 2,
	.bBitResolution = 2 * 8,
};

/* Audio Streaming OUT Interface - Alt5 */
static struct usb_interface_descriptor std_as_out_if5_desc = {
	.bLength = sizeof std_as_out_if5_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 5,
	.bNumEndpoints = 1,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Stream OUT Intface Desc */
static struct uac2_as_header_descriptor as_out_hdr5_desc = {
	.bLength = sizeof as_out_hdr5_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_AS_GENERAL,
	.bTerminalLink = USB_OUT_IT_ID,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bmFormats = cpu_to_le32(UAC_FORMAT_TYPE_I_PCM),
	.bNrChannels = 8,
	.bmChannelConfig = cpu_to_le32(0xFF),
	.iChannelNames = 0,
};

/* Audio USB_OUT Format */
static struct uac2_format_type_i_descriptor as_out_fmt1_desc5 = {
	.bLength = sizeof as_out_fmt1_desc5,
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC_FORMAT_TYPE,
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bSubslotSize = 2,
	.bBitResolution = 2 * 8,
};

/* Audio Streaming IN Interface - Alt0 */
static struct usb_interface_descriptor std_as_in_if0_desc = {
	.bLength = sizeof std_as_in_if0_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 0,
	.bNumEndpoints = 0,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Streaming IN Interface - Alt1 */
static struct usb_interface_descriptor std_as_in_if1_desc = {
	.bLength = sizeof std_as_in_if1_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 1,
	.bNumEndpoints = 1,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Stream IN Intface Desc */
static struct uac2_as_header_descriptor as_in_hdr_desc = {
	.bLength = sizeof as_in_hdr_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_AS_GENERAL,
	.bTerminalLink = USB_IN_OT_ID,
	.bmControls = 0,
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bmFormats = cpu_to_le32(UAC_FORMAT_TYPE_I_PCM),
	.iChannelNames = 0,
};

/* Audio USB_IN Format */
static struct uac2_format_type_i_descriptor as_in_fmt1_desc = {
	.bLength = sizeof as_in_fmt1_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC_FORMAT_TYPE,
	.bFormatType = UAC_FORMAT_TYPE_I,
};

/* STD AS ISO IN Endpoint */
static struct usb_endpoint_descriptor fs_epin_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_ISOC | USB_ENDPOINT_SYNC_ASYNC,
	.wMaxPacketSize = cpu_to_le16(1023),
	.bInterval = 1,
};

static struct usb_endpoint_descriptor hs_epin_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bmAttributes = USB_ENDPOINT_XFER_ISOC | USB_ENDPOINT_SYNC_ASYNC,
	.wMaxPacketSize = cpu_to_le16(1024),
	.bInterval = 4,
};

/* CS AS ISO IN Endpoint */
static struct uac2_iso_endpoint_descriptor as_iso_in_desc = {
	.bLength = sizeof as_iso_in_desc,
	.bDescriptorType = USB_DT_CS_ENDPOINT,

	.bDescriptorSubtype = UAC_EP_GENERAL,
	.bmAttributes = 0,
	.bmControls = 0,
	.bLockDelayUnits = 0,
	.wLockDelay = 0,
};

static struct usb_descriptor_header *fs_audio_desc[] = {
	(struct usb_descriptor_header *)&iad_desc,
	(struct usb_descriptor_header *)&std_ac_if_desc,

	(struct usb_descriptor_header *)&ac_hdr_desc,
	(struct usb_descriptor_header *)&in_clk_src_desc,
	(struct usb_descriptor_header *)&out_clk_src_desc,
	(struct usb_descriptor_header *)&usb_out_it_desc,
	(struct usb_descriptor_header *)&io_in_it_desc,
	(struct usb_descriptor_header *)&usb_in_ot_desc,
	(struct usb_descriptor_header *)&io_out_ot_desc,

	(struct usb_descriptor_header *)&io_out_fu_desc,

	(struct usb_descriptor_header *)&std_as_out_if0_desc,
	(struct usb_descriptor_header *)&std_as_out_if1_desc,

	(struct usb_descriptor_header *)&as_out_hdr_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc,
	(struct usb_descriptor_header *)&fs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,

	(struct usb_descriptor_header *)&std_as_out_if2_desc,
	(struct usb_descriptor_header *)&as_out_hdr2_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc2,
	(struct usb_descriptor_header *)&fs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,

	(struct usb_descriptor_header *)&std_as_out_if3_desc,
	(struct usb_descriptor_header *)&as_out_hdr3_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc3,
	(struct usb_descriptor_header *)&fs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,

	(struct usb_descriptor_header *)&std_as_out_if4_desc,
	(struct usb_descriptor_header *)&as_out_hdr4_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc4,
	(struct usb_descriptor_header *)&fs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,

	(struct usb_descriptor_header *)&std_as_out_if5_desc,
	(struct usb_descriptor_header *)&as_out_hdr5_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc5,
	(struct usb_descriptor_header *)&fs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,

	(struct usb_descriptor_header *)&std_as_in_if0_desc,
	(struct usb_descriptor_header *)&std_as_in_if1_desc,

	(struct usb_descriptor_header *)&as_in_hdr_desc,
	(struct usb_descriptor_header *)&as_in_fmt1_desc,
	(struct usb_descriptor_header *)&fs_epin_desc,
	(struct usb_descriptor_header *)&as_iso_in_desc,
	NULL,
};

static struct usb_descriptor_header *hs_audio_desc[] = {
	(struct usb_descriptor_header *)&iad_desc,
	(struct usb_descriptor_header *)&std_ac_if_desc,

	(struct usb_descriptor_header *)&ac_hdr_desc,
	(struct usb_descriptor_header *)&in_clk_src_desc,
	(struct usb_descriptor_header *)&out_clk_src_desc,
	(struct usb_descriptor_header *)&usb_out_it_desc,
	(struct usb_descriptor_header *)&io_in_it_desc,
	(struct usb_descriptor_header *)&usb_in_ot_desc,
	(struct usb_descriptor_header *)&io_out_ot_desc,

	(struct usb_descriptor_header *)&io_out_fu_desc,

	(struct usb_descriptor_header *)&std_as_out_if0_desc,
	(struct usb_descriptor_header *)&std_as_out_if1_desc,

	(struct usb_descriptor_header *)&as_out_hdr_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc,
	(struct usb_descriptor_header *)&hs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,

	(struct usb_descriptor_header *)&std_as_out_if2_desc,
	(struct usb_descriptor_header *)&as_out_hdr2_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc2,
	(struct usb_descriptor_header *)&hs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,

	(struct usb_descriptor_header *)&std_as_out_if3_desc,
	(struct usb_descriptor_header *)&as_out_hdr3_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc3,
	(struct usb_descriptor_header *)&hs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,

	(struct usb_descriptor_header *)&std_as_out_if4_desc,
	(struct usb_descriptor_header *)&as_out_hdr4_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc4,
	(struct usb_descriptor_header *)&hs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,

	(struct usb_descriptor_header *)&std_as_out_if5_desc,
	(struct usb_descriptor_header *)&as_out_hdr5_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc5,
	(struct usb_descriptor_header *)&hs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,

	(struct usb_descriptor_header *)&std_as_in_if0_desc,
	(struct usb_descriptor_header *)&std_as_in_if1_desc,

	(struct usb_descriptor_header *)&as_in_hdr_desc,
	(struct usb_descriptor_header *)&as_in_fmt1_desc,
	(struct usb_descriptor_header *)&hs_epin_desc,
	(struct usb_descriptor_header *)&as_iso_in_desc,
	NULL,
};

static struct usb_audio_control mute_control = {
	.list = LIST_HEAD_INIT(mute_control.list),
	.name = "Mute Control",
	.type = UAC_FU_MUTE,
	/* Todo: add real Mute control code */
	.set = generic_set_cmd,
	.get = generic_get_cmd,
};

static struct usb_audio_control volume_control = {
	.list = LIST_HEAD_INIT(volume_control.list),
	.name = "Volume Control",
	.type = UAC_FU_VOLUME,
	/* Todo: add real Volume control code */
	.set = generic_set_cmd,
	.get = generic_get_cmd,
};

static struct usb_audio_control_selector feature_unit = {
	.list = LIST_HEAD_INIT(feature_unit.list),
	.id = IO_OUT_FU_ID,
	.name = "Mute & Volume Control",
	.type = UAC_FEATURE_UNIT,
	.desc = (struct usb_descriptor_header *)&io_out_fu_desc,
};

/*Layout 1 Parameter Block */
struct cntrl_cur_lay1 {
	__u8	dCUR;
};

/*Layout 2 Parameter Block */
struct cntrl_cur_lay2 {
	__u16	dCUR;
};

struct cntrl_range_lay2 {
	__u16	dMIN;
	__u16	dMAX;
	__u16	dRES;
} __packed;

#define ranges_lay2_size(c) (sizeof(c.wNumSubRanges) + c.wNumSubRanges \
		* sizeof(struct cntrl_ranges_lay2))
struct cntrl_ranges_lay2 {
	__u16	wNumSubRanges;
	struct cntrl_range_lay2 r[UAC_MAX_ITEMS];
} __packed;

/*Layout 3 Parameter Block */
struct cntrl_cur_lay3 {
	__u32	dCUR;
};

struct cntrl_range_lay3 {
	__u32	dMIN;
	__u32	dMAX;
	__u32	dRES;
} __packed;

#define ranges_lay3_size(c) (sizeof(c.wNumSubRanges) + c.wNumSubRanges \
		* sizeof(struct cntrl_ranges_lay3))
struct cntrl_ranges_lay3 {
	__u16	wNumSubRanges;
	struct cntrl_range_lay3 r[UAC_MAX_ITEMS];
} __packed;

/* Maxpacket and other transfer characteristics vary by speed. */
static struct usb_endpoint_descriptor *
uac2_ep_desc(struct usb_gadget *g, struct usb_endpoint_descriptor *fs,
		struct usb_endpoint_descriptor *hs)
{
	if (gadget_is_dualspeed(g) && g->speed == USB_SPEED_HIGH)
		return hs;
	return fs;
}

static inline void
free_ep(struct uac2_rtd_params *prm, struct usb_ep *ep)
{
	//struct snd_uac2_chip *uac2 = prm->uac2;
	int i;

	if (!prm->ep_enabled) {
		dev_dbg(NULL, "free ep%x hasn't enabled\n", ep->address);
		return;
	}
	prm->ep_enabled = false;
	ep->driver_data = NULL;

	for (i = 0; i < USB_XFERS; i++) {
		if (prm->ureq[i].req) {
			usb_ep_dequeue(ep, prm->ureq[i].req);
			usb_ep_free_request(ep, prm->ureq[i].req);
			prm->ureq[i].req = NULL;
		}
	}

	if (usb_ep_disable(ep))
		printf("free_ep disable ep Error!\n");
}

static void set_ep_max_packet_size(const struct audio_dev *agdev,
	struct usb_endpoint_descriptor *ep_desc,
	unsigned int factor, bool is_playback)
{
	int chmask, srate = 0, ssize;
	u16 max_packet_size;
	int i;

	if (is_playback) {
		chmask = agdev->p_chmask;
		for (i = 0; i < UAC_MAX_ITEMS; i++) {
			if (agdev->p_srate[i] == 0)
				break;
			if (agdev->p_srate[i] > srate)
				srate = agdev->p_srate[i];
		}
		ssize = agdev->p_ssize;
	} else {
		chmask = agdev->c_chmask;
		for (i = 0; i < UAC_MAX_ITEMS; i++) {
			if (agdev->c_srate[i] == 0)
				break;
			if (agdev->c_srate[i] > srate)
				srate = agdev->c_srate[i];
		}
		ssize = agdev->c_ssize;
	}

	max_packet_size = num_channels(chmask) * ssize *
		DIV_ROUND_UP(srate, factor / (1 << (ep_desc->bInterval - 1)));
	ep_desc->wMaxPacketSize = cpu_to_le16(min_t(u16, max_packet_size,
				le16_to_cpu(ep_desc->wMaxPacketSize)));
	dev_info(NULL, "set wMaxPacketSize:%d\n", ep_desc->wMaxPacketSize); 
}

static void set_ep_cur_max_packet_size(struct audio_dev *agdev,
	struct usb_endpoint_descriptor *ep_desc,
	unsigned int factor, bool is_playback)
{
	int chmask, srate = 0, ssize;
	u16 max_packet_size;

	if (is_playback) {
		chmask = agdev->p_chmask;
		srate = agdev->p_srate_active;
		ssize = agdev->p_ssize;
	} else {
		if (agdev->c_chmask == 0x3F || agdev->c_chmask == 0xFF) {
			if (agdev->c_srate_active > 16000) {
				agdev->c_srate_active = 16000;
				dev_info(NULL, "\n\n------------------------------------------\n");
				dev_info(NULL, "Force to max rate 16k for multi-channles\n");
				dev_info(NULL, "----------------------------------------------\n\n");
			}
		} else if (agdev->c_chmask == 0x3){
			if (agdev->c_ssize == 4) {
				if (agdev->c_srate_active > 32000) {
					agdev->c_srate_active = 32000;
					dev_info(NULL, "\n\n--------------------------------------\n");
					dev_info(NULL, "Force to max rate 32k for 32 bits format\n");
					dev_info(NULL, "------------------------------------------\n\n");
				}
			}
		}
		chmask = agdev->c_chmask;
		srate = agdev->c_srate_active;
		ssize = agdev->c_ssize;
	}

	max_packet_size = num_channels(chmask) * ssize *
		DIV_ROUND_UP(srate, factor / (1 << (ep_desc->bInterval - 1)));
	ep_desc->wMaxPacketSize = cpu_to_le16(max_packet_size);
	dev_dbg(NULL, "set current ep_desc %p wMaxPacketSize:%d\n", ep_desc, ep_desc->wMaxPacketSize); 
}

static int
afunc_bind(struct usb_configuration *cfg, struct usb_function *fn)
{
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;
	struct usb_composite_dev *cdev = cfg->cdev;
	struct usb_gadget *gadget = cdev->gadget;
	struct uac2_rtd_params *prm;
	struct usb_string *us;
	int ret;

	us = usb_gstrings_attach(cdev, fn_strings, ARRAY_SIZE(strings_fn));
	if (IS_ERR(us))
		return PTR_ERR(us);
	iad_desc.iFunction = us[STR_ASSOC].id;
	std_ac_if_desc.iInterface = us[STR_IF_CTRL].id;
	in_clk_src_desc.iClockSource = us[STR_CLKSRC_IN].id;
	out_clk_src_desc.iClockSource = us[STR_CLKSRC_OUT].id;
	usb_out_it_desc.iTerminal = us[STR_USB_IT].id;
	io_in_it_desc.iTerminal = us[STR_IO_IT].id;
	io_out_fu_desc.iFeature = us[STR_IO_FU_OUT].id;
	usb_in_ot_desc.iTerminal = us[STR_USB_OT].id;
	io_out_ot_desc.iTerminal = us[STR_IO_OT].id;
	std_as_out_if0_desc.iInterface = us[STR_AS_OUT_ALT0].id;
	std_as_out_if1_desc.iInterface = us[STR_AS_OUT_ALT1].id;
	std_as_out_if3_desc.iInterface = us[STR_AS_OUT_ALT1].id;
	std_as_out_if4_desc.iInterface = us[STR_AS_OUT_ALT1].id;
	std_as_out_if5_desc.iInterface = us[STR_AS_OUT_ALT1].id;
	std_as_in_if0_desc.iInterface = us[STR_AS_IN_ALT0].id;
	std_as_in_if1_desc.iInterface = us[STR_AS_IN_ALT1].id;

    dev_dbg(NULL, "fuc id: %d/%d/%d/%d\n", us[STR_ASSOC].id, us[STR_IF_CTRL].id,
			us[STR_CLKSRC_IN].id, us[STR_CLKSRC_OUT].id);
    dev_dbg(NULL, "fuc id: %d/%d/%d/%d\n", us[STR_USB_IT].id, us[STR_IO_IT].id,
			us[STR_USB_OT].id, us[STR_IO_OT].id);
    dev_dbg(NULL, "fuc id: %d/%d/%d/%d\n", us[STR_AS_OUT_ALT0].id, us[STR_AS_OUT_ALT1].id,
			us[STR_AS_IN_ALT0].id, us[STR_AS_IN_ALT1].id);

	/* Initialize the configurable parameters */
	usb_out_it_desc.bNrChannels = num_channels(agdev->c_chmask);
	usb_out_it_desc.bmChannelConfig = cpu_to_le32(agdev->c_chmask);
	io_in_it_desc.bNrChannels = num_channels(agdev->p_chmask);
	io_in_it_desc.bmChannelConfig = cpu_to_le32(agdev->p_chmask);
	as_out_hdr_desc.bNrChannels = num_channels(agdev->c_chmask);
	as_out_hdr_desc.bmChannelConfig = cpu_to_le32(agdev->c_chmask);
	as_in_hdr_desc.bNrChannels = num_channels(agdev->p_chmask);
	as_in_hdr_desc.bmChannelConfig = cpu_to_le32(agdev->p_chmask);
	as_out_fmt1_desc.bSubslotSize = agdev->c_ssize;
	as_out_fmt1_desc.bBitResolution = agdev->c_ssize * 8;
	as_in_fmt1_desc.bSubslotSize = agdev->p_ssize;
	as_in_fmt1_desc.bBitResolution = agdev->p_ssize * 8;

	ret = usb_interface_id(cfg, fn);
	if (ret < 0) {
		printf("afunc_bind: std_ac_if_desc id Error!\n");
		return ret;
	}
	std_ac_if_desc.bInterfaceNumber = ret;
	agdev->ac_intf = ret;
	agdev->ac_alt = 0;

	ret = usb_interface_id(cfg, fn);
	if (ret < 0) {
		printf("afunc_bind: std_as_out_if0/1_desc id Error!\n");
		return ret;
	}
	std_as_out_if0_desc.bInterfaceNumber = ret;
	std_as_out_if1_desc.bInterfaceNumber = ret;
	std_as_out_if2_desc.bInterfaceNumber = ret;
	std_as_out_if3_desc.bInterfaceNumber = ret;
	std_as_out_if4_desc.bInterfaceNumber = ret;
	std_as_out_if5_desc.bInterfaceNumber = ret;
	agdev->as_out_intf = ret;
	agdev->as_out_alt = 0;

	ret = usb_interface_id(cfg, fn);
	if (ret < 0) {
		printf("afunc_bind: std_as_in_if0/1_desc id Error!\n");
		return ret;
	}
	std_as_in_if0_desc.bInterfaceNumber = ret;
	std_as_in_if1_desc.bInterfaceNumber = ret;
	agdev->as_in_intf = ret;
	agdev->as_in_alt = 0;

	agdev->out_ep = usb_ep_autoconfig(gadget, &fs_epout_desc);
	if (!agdev->out_ep) {
		printf("afunc_bind: out_ep Error!\n");
		return ret;
	}
	agdev->out_ep->driver_data = agdev;	/* claim the endpoint */

	agdev->in_ep = usb_ep_autoconfig(gadget, &fs_epin_desc);
	if (!agdev->in_ep) {
		printf("afunc_bind: in_ep Error!\n");
		return ret;
	}
	agdev->in_ep->driver_data = agdev;	/* claim the endpoint */

	uac2->p_prm.uac2 = uac2;
	uac2->c_prm.uac2 = uac2;

	/* Calculate wMaxPacketSize according to audio bandwidth */
	fs_epin_desc.wMaxPacketSize = cpu_to_le16(1023);
	fs_epout_desc.wMaxPacketSize = cpu_to_le16(1023),
	hs_epin_desc.wMaxPacketSize = cpu_to_le16(1024),
	hs_epout_desc.wMaxPacketSize = cpu_to_le16(1024),
	set_ep_max_packet_size(agdev, &fs_epin_desc, 1000, true);
	set_ep_max_packet_size(agdev, &fs_epout_desc, 1000, false);
	set_ep_max_packet_size(agdev, &hs_epin_desc, 8000, true);
	set_ep_max_packet_size(agdev, &hs_epout_desc, 8000, false);

	hs_epout_desc.bEndpointAddress = fs_epout_desc.bEndpointAddress;
	hs_epin_desc.bEndpointAddress = fs_epin_desc.bEndpointAddress;

	ret = usb_assign_descriptors(fn, fs_audio_desc, hs_audio_desc, NULL);
	if (ret)
		return ret;

	prm = &agdev->uac2.c_prm;
	prm->max_psize = hs_epout_desc.wMaxPacketSize;
#ifdef REQ_FIX_MEM_ADDR
	if (musb_gadget->store) {
		prm->rbuf = (void *)TEST_REQ_RX_BUF;
	} else {
		uac2_rx_buf_init(prm->max_psize * USB_XFERS);
		prm->rbuf = uac2_rx_buf_alloc(prm->max_psize * USB_XFERS);//(void *)TEST_REQ_RX_BUF;
	}
#else
	prm->rbuf = calloc(prm->max_psize * USB_XFERS, 1);
#endif
	if (!prm->rbuf) {
		prm->max_psize = 0;
		goto err_free_descs;
	}

	prm = &agdev->uac2.p_prm;
	prm->max_psize = hs_epin_desc.wMaxPacketSize;
#ifdef REQ_FIX_MEM_ADDR
	prm->rbuf = (void *)TEST_REQ_TX_BUF;
#else
	prm->rbuf = calloc(prm->max_psize * USB_XFERS, 1);
#endif
	if (!prm->rbuf) {
		prm->max_psize = 0;
		goto err;
	}

#if 0
	ret = alsa_uac2_init(agdev);
	if (ret)
		goto err;
#endif	
	return 0;

err:
	kfree(agdev->uac2.p_prm.rbuf);
	kfree(agdev->uac2.c_prm.rbuf);
err_free_descs:
	usb_free_all_descriptors(fn);
	return -EINVAL;
}

/*-------------------------------------------------------------------------*/

static int generic_set_cmd(struct usb_audio_control *con, u8 cmd, int value)
{
	struct snd_wm8766 *wm = &g_wm[musb_gadget->p_i2s->port-1];
	int volume = 0;

	con->data_active = value;
	switch (con->type) {
	case UAC_FU_MUTE:
		if (value == 1) {
			volume = 0;
		} else {
			volume = volume_control.data_active - VOLUME_MIN;
			volume /= VOLUME_STEPS;
		}
		break;
	case UAC_FU_VOLUME:
		volume = value - VOLUME_MIN;
		volume /= VOLUME_STEPS;
		break;
	default:
		volume = 80;
		break;
	}
	snd_wm8766_set_volume(wm, WM8766_CH1, volume, volume);
	return 0;
}

static int generic_get_cmd(struct usb_audio_control *con, u8 cmd, int *data)
{
	if (cmd == UAC2_CS_CUR) {
		*data = con->data_active;
	} else {
		memcpy(data, con->data_range, sizeof(con->data_range));
	}
	return 0;
}

/* Todo: add more control selecotor dynamically */
static int control_selector_init(struct audio_dev *audio)
{
	INIT_LIST_HEAD(&audio->cs);
	list_add(&feature_unit.list, &audio->cs);

	INIT_LIST_HEAD(&feature_unit.control);
	list_add(&mute_control.list, &feature_unit.control);
	list_add(&volume_control.list, &feature_unit.control);
#if 0
	volume_control.data[UAC__CUR] = 0xffc0;
	volume_control.data[UAC__MIN] = 0xe3a0;
	volume_control.data[UAC__MAX] = 0xfff0;
	volume_control.data[UAC__RES] = 0x0030;
#endif
	memcpy(volume_control.data_range, c_volume,
			sizeof(volume_control.data_range));
	volume_control.data_active = 0xffc0;

	return 0;
}

int u_audio_set_capture_srate(struct audio_dev *audio_dev, int srate)
{
	int i;

	for (i = 0; i < UAC_MAX_ITEMS; i++) {
		if (audio_dev->c_srate[i] == srate) {
			audio_dev->c_srate_active = srate;
			return 0;
		}
		if (audio_dev->c_srate[i] == 0)
			break;
	}

	return -EINVAL;
}

int u_audio_set_playback_srate(struct audio_dev *audio_dev, int srate)
{
	int i;

	for (i = 0; i < UAC_MAX_ITEMS; i++) {
		if (audio_dev->p_srate[i] == srate) {
			audio_dev->p_srate_active = srate;
			return 0;
		}
		if (audio_dev->p_srate[i] == 0)
			break;
	}

	return -EINVAL;
}

static int
afunc_set_alt(struct usb_function *fn, unsigned intf, unsigned alt)
{
	struct usb_composite_dev *cdev = fn->config->cdev;
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;
	struct usb_gadget *gadget = cdev->gadget;
	struct usb_request *req;
	const struct usb_endpoint_descriptor *ep_desc;
	struct usb_ep *ep;
	struct uac2_rtd_params *prm;
	int req_len, i;

	/* No i/f has more than 5 alt settings */
	if (alt > 5) {
		printf("afunc_set_alt alt Error!\n");
		return -EINVAL;
	}

	if (intf == agdev->ac_intf) {
		/* Control I/f has only 1 AltSetting - 0 */
		if (alt) {
			printf("afunc_set_alt Control I/f Error!\n");
			return -EINVAL;
		}
		return 0;
	}

	if (intf == agdev->as_out_intf) {
		ep = agdev->out_ep;
		prm = &uac2->c_prm;
		switch (alt) {
		case 0:
			dev_dbg(NULL, "don't need process alt setting 0\n");
			break;
		case 1:
			agdev->c_chmask = le32_to_cpu(as_out_hdr_desc.bmChannelConfig);
			agdev->c_ssize = as_out_fmt1_desc.bSubslotSize;
			break;
		case 2:
			agdev->c_chmask = le32_to_cpu(as_out_hdr2_desc.bmChannelConfig);
			agdev->c_ssize = as_out_fmt1_desc2.bSubslotSize;
			break;
		case 3:
			agdev->c_chmask = le32_to_cpu(as_out_hdr3_desc.bmChannelConfig);
			agdev->c_ssize = as_out_fmt1_desc3.bSubslotSize;
			break;
		case 4:
			agdev->c_chmask = le32_to_cpu(as_out_hdr4_desc.bmChannelConfig);
			agdev->c_ssize = as_out_fmt1_desc4.bSubslotSize;
			break;
		case 5:
			agdev->c_chmask = le32_to_cpu(as_out_hdr5_desc.bmChannelConfig);
			agdev->c_ssize = as_out_fmt1_desc5.bSubslotSize;
			break;
		default:
			dev_info(NULL, "unsupport alt setting %d\n", alt);
			break;
		}
		if (gadget->speed == USB_SPEED_FULL) {
			set_ep_cur_max_packet_size(agdev, &fs_epout_desc, 1000, false);
			prm->max_psize = fs_epout_desc.wMaxPacketSize;
		} else {
			set_ep_cur_max_packet_size(agdev, &hs_epout_desc, 8000, false);
			prm->max_psize = hs_epout_desc.wMaxPacketSize;
		}

		ep_desc = uac2_ep_desc(gadget,
				&fs_epout_desc, &hs_epout_desc);
		agdev->as_out_alt = alt;
		req_len = prm->max_psize;
	} else if (intf == agdev->as_in_intf) {
		unsigned int factor, rate;

		ep = agdev->in_ep;
		prm = &uac2->p_prm;
		if (gadget->speed == USB_SPEED_FULL) {
			set_ep_cur_max_packet_size(agdev, &fs_epin_desc, 1000, true);
			prm->max_psize = fs_epin_desc.wMaxPacketSize;
		} else {
			set_ep_cur_max_packet_size(agdev, &hs_epin_desc, 8000, true);
			prm->max_psize = hs_epin_desc.wMaxPacketSize;
		}

		ep_desc = uac2_ep_desc(gadget,
				&fs_epin_desc, &hs_epin_desc);
		agdev->as_in_alt = alt;

		/* pre-calculate the playback endpoint's interval */
		if (gadget->speed == USB_SPEED_FULL) {
			factor = 1000;
		} else {
			factor = 8000;
		}

		/* pre-compute some values for iso_complete() */
		uac2->p_framesize = agdev->p_ssize *
				    num_channels(agdev->p_chmask);
		rate = agdev->p_srate_active * uac2->p_framesize;
		uac2->p_interval = factor / (1 << (ep_desc->bInterval - 1));
		uac2->p_pktsize = min_t(unsigned int, rate / uac2->p_interval,
					prm->max_psize);

		if (uac2->p_pktsize < prm->max_psize)
			uac2->p_pktsize_residue = rate % uac2->p_interval;
		else
			uac2->p_pktsize_residue = 0;

		req_len = uac2->p_pktsize;
		uac2->p_residue = 0;
	} else {
		printf("afunc_set_alt unsupport ep Error!\n");
		return -EINVAL;
	}

	if (alt == 0) {
		free_ep(prm, ep);
		return 0;
	} else {
		free_ep(prm, ep);
	}

	ep->driver_data = agdev; /* claim */
	if (usb_ep_enable(ep, ep_desc)) {
		printf("afunc_set_alt enable ep%x Error!\n", ep->address);
		return -EINVAL;
	}
	prm->ep_enabled = true;
	if (musb_gadget->store) {
		if (intf == agdev->as_out_intf) {
			dev_dbg(NULL, "afunc_set_alt out_ep%x memset ddr %p\n", ep->address, test_audio_buff);
			memset(test_audio_buff, 0, DEV_AUDIO_MAX_LEN);
			uac2->rx_buf_offset = 0;
		}

		if (intf == agdev->as_in_intf) {
			dev_dbg(NULL, "afunc_set_alt in_ep%x rx_data_len:%d\n", ep->address, uac2->rx_buf_offset);
			uac2->tx_buf_offset = 0;
		}
	} else {
		if (intf == agdev->as_out_intf) {
			uac2_rx_buf_init(prm->max_psize * USB_XFERS);
			prm->rbuf = uac2_rx_buf_alloc(prm->max_psize * USB_XFERS);//(void *)TEST_REQ_RX_BUF;
			dev_dbg(NULL, "afunc_set_alt out_ep%x i2s_mode:%d\n", ep->address, musb_gadget->p_i2s->dt_mode);
		}
	}
	for (i = 0; i < USB_XFERS; i++) {
		if (!prm->ureq[i].req) {
			req = usb_ep_alloc_request(ep, GFP_ATOMIC);
			if (req == NULL) {
				dev_info(NULL, "afunc_set_alt alloc request Error!\n");
				return -ENOMEM;
			}

			prm->ureq[i].req = req;
			prm->ureq[i].pp = prm;

			req->zero = 0;
			req->context = &prm->ureq[i];
			req->length = req_len;
			req->complete = agdev_iso_complete;
			req->buf = prm->rbuf + i * prm->max_psize;
		}

		if (usb_ep_queue(ep, prm->ureq[i].req, GFP_ATOMIC))
			dev_info(NULL, "afunc_set_alt queue ep Error!\n");
	}

	return 0;
}

static int
afunc_get_alt(struct usb_function *fn, unsigned intf)
{
	struct audio_dev *agdev = func_to_agdev(fn);

	if (intf == agdev->ac_intf)
		return agdev->ac_alt;
	else if (intf == agdev->as_out_intf)
		return agdev->as_out_alt;
	else if (intf == agdev->as_in_intf)
		return agdev->as_in_alt;
	else
		printf("afunc_get_alt: Invalid Interface %d!\n", intf);

	return -EINVAL;
}

static void
afunc_disable(struct usb_function *fn)
{
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;

	free_ep(&uac2->p_prm, agdev->in_ep);
	agdev->as_in_alt = 0;

	free_ep(&uac2->c_prm, agdev->out_ep);
	agdev->as_out_alt = 0;
}

static int
in_rq_cur(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	struct usb_request *req = fn->config->cdev->req;
	struct audio_dev *agdev = func_to_agdev(fn);
	//struct snd_uac2_chip *uac2 = &agdev->uac2;
	u16 w_length = le16_to_cpu(cr->wLength);
	u16 w_index = le16_to_cpu(cr->wIndex);
	u16 w_value = le16_to_cpu(cr->wValue);
	u8 entity_id = (w_index >> 8) & 0xff;
	u8 control_selector = w_value >> 8;
	u8 cmd = (cr->bRequest & 0x0F);
	struct usb_audio_control_selector *cs;
	struct usb_audio_control *con;
	int data, value = -EOPNOTSUPP;

	switch (entity_id) {
	case USB_IN_CLK_ID:
		if (control_selector == UAC2_CS_CONTROL_SAM_FREQ) {
			struct cntrl_cur_lay3 c;
			memset(&c, 0, sizeof(struct cntrl_cur_lay3));

			c.dCUR = agdev->p_srate_active;
			dev_info(NULL, "%s(): %d\n", __func__, c.dCUR);
			value = min_t(unsigned, w_length, sizeof c);
			memcpy(req->buf, &c, value);
		} else if (control_selector == UAC2_CS_CONTROL_CLOCK_VALID) {
			*(u8 *)req->buf = 1;
			value = min_t(unsigned, w_length, 1);
		} else {
			printf("%s:%d control_selector=%d TODO!\n",
				__func__, __LINE__, control_selector);
		}
		break;
	case USB_OUT_CLK_ID:
		if (control_selector == UAC2_CS_CONTROL_SAM_FREQ) {
			struct cntrl_cur_lay3 c;
			memset(&c, 0, sizeof(struct cntrl_cur_lay3));
		
			c.dCUR = agdev->c_srate_active;
			dev_info(NULL, "%s(): %d\n", __func__, c.dCUR);
			value = min_t(unsigned, w_length, sizeof c);
			memcpy(req->buf, &c, value);
		} else if (control_selector == UAC2_CS_CONTROL_CLOCK_VALID) {
			*(u8 *)req->buf = 1;
			value = min_t(unsigned, w_length, 1);
		} else {
			printf("%s:%d control_selector=%d TODO!\n",
				__func__, __LINE__, control_selector);
		}
		break;
	case IO_OUT_FU_ID:
		list_for_each_entry(cs, &agdev->cs, list) {
			if (cs->id == entity_id) {
				list_for_each_entry(con, &cs->control, list) {
					if (con->type == control_selector && con->get) {
						con->get(con, cmd, &data);
						break;
					}
				}
				break;
			}
		}
		if (control_selector == UAC_FU_MUTE) {
			struct cntrl_cur_lay1 c;
			memset(&c, 0, sizeof(struct cntrl_cur_lay1));

			c.dCUR = data;
			dev_info(NULL, "%s(): Mute %d\n", __func__, c.dCUR);
			value = min_t(unsigned, w_length, sizeof c);
			memcpy(req->buf, &c, value);
			break;
		} else if (control_selector == UAC_FU_VOLUME) {
			struct cntrl_cur_lay2 c;
			memset(&c, 0, sizeof(struct cntrl_cur_lay2));

			c.dCUR = data;
			dev_info(NULL, "%s(): Volume %x\n", __func__, c.dCUR);
			value = min_t(unsigned, w_length, sizeof c);
			memcpy(req->buf, &c, value);
			break;
		} else {
			printf("%s:%d control_selector=%d TODO!\n",
					__func__, __LINE__, control_selector);
			break;
		}
	default:
		dev_info(NULL, "unsupport entity id\n");
		break;
	}
	return value;
}

static int
in_rq_range(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	struct usb_request *req = fn->config->cdev->req;
	struct audio_dev *agdev = func_to_agdev(fn);
	//struct snd_uac2_chip *uac2 = &agdev->uac2;
	u16 w_length = le16_to_cpu(cr->wLength);
	u16 w_index = le16_to_cpu(cr->wIndex);
	u16 w_value = le16_to_cpu(cr->wValue);
	u8 entity_id = (w_index >> 8) & 0xff;
	u8 control_selector = w_value >> 8;
	u8 cmd = (cr->bRequest & 0x0F);
	struct cntrl_ranges_lay2 rs2;
	struct usb_audio_control_selector *cs;
	struct usb_audio_control *con;
	int data[UAC_MAX_ITEMS], value = -EOPNOTSUPP;
	int item = 0;
	int i;

	switch (entity_id) {
	case USB_IN_CLK_ID:
		if (control_selector == UAC2_CS_CONTROL_SAM_FREQ) {
			struct cntrl_ranges_lay3 rs;
			rs.wNumSubRanges = 0;
			for (i = 0; i < UAC_MAX_ITEMS; i++) {
				item = agdev->p_srate[i];
				if (item == 0)
					break;
		
				rs.r[rs.wNumSubRanges].dMIN = item;
				rs.r[rs.wNumSubRanges].dMAX = item;
				rs.r[rs.wNumSubRanges].dRES = 0;
				rs.wNumSubRanges++;
				dev_dbg(NULL,
						"%s(): clk %d: report rate %d. %d\n",
						__func__, entity_id, rs.wNumSubRanges,
						item);
			}
		
			value = min_t(unsigned int, w_length, ranges_lay3_size(rs));
			dev_info(NULL, "%s(): send %d rates, size %d\n",
					__func__, rs.wNumSubRanges, value);
			memcpy(req->buf, &rs, value);
		} else {
			printf("%s:%d control_selector=%d TODO!\n",
				__func__, __LINE__, control_selector);
		}
		break;
	case USB_OUT_CLK_ID:
		if (control_selector == UAC2_CS_CONTROL_SAM_FREQ) {
			struct cntrl_ranges_lay3 rs;
			rs.wNumSubRanges = 0;
			for (i = 0; i < UAC_MAX_ITEMS; i++) {
				item = agdev->c_srate[i];
				if (item == 0)
					break;
		
				rs.r[rs.wNumSubRanges].dMIN = item;
				rs.r[rs.wNumSubRanges].dMAX = item;
				rs.r[rs.wNumSubRanges].dRES = 0;
				rs.wNumSubRanges++;
				dev_dbg(NULL,
						"%s(): clk %d: report rate %d. %d\n",
						__func__, entity_id, rs.wNumSubRanges,
						item);
			}
		
			value = min_t(unsigned int, w_length, ranges_lay3_size(rs));
			dev_info(NULL, "%s(): send %d rates, size %d\n",
					__func__, rs.wNumSubRanges, value);
			memcpy(req->buf, &rs, value);
		} else {
			printf("%s:%d control_selector=%d TODO!\n",
				__func__, __LINE__, control_selector);
		}
		break;
	case IO_OUT_FU_ID:

		list_for_each_entry(cs, &agdev->cs, list) {
			if (cs->id == entity_id) {
				list_for_each_entry(con, &cs->control, list) {
					if (con->type == control_selector && con->get) {
						con->get(con, cmd, data);
						break;
					}
				}
				break;
			}
		}

		rs2.wNumSubRanges = 0;
		for (i = 0; i < UAC_MAX_ITEMS; i++) {
			item = data[i];
			if (item == 0)
				break;
		
			rs2.r[rs2.wNumSubRanges].dMIN = item;
			rs2.r[rs2.wNumSubRanges].dMAX = item + 0x7FF0;
			rs2.r[rs2.wNumSubRanges].dRES = VOLUME_STEPS;
			rs2.wNumSubRanges++;
			dev_dbg(NULL,
					"%s(): entity %d: report volume %d. %x\n",
					__func__, entity_id, rs2.wNumSubRanges,
					item);
		}
	
		value = min_t(unsigned int, w_length, ranges_lay2_size(rs2));
		dev_info(NULL, "%s(): send %d volumes, size %d\n",
				__func__, rs2.wNumSubRanges, value);
		memcpy(req->buf, &rs2, value);
		break;
	default:
		dev_info(NULL, "unsupport entity id\n");
		break;
	}

	return value;
}

static int
ac_rq_in(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	if (cr->bRequest == UAC2_CS_CUR)
		return in_rq_cur(fn, cr);
	else if (cr->bRequest == UAC2_CS_RANGE)
		return in_rq_range(fn, cr);
	else
		return -EOPNOTSUPP;
}

static void uac2_cs_control_sam_freq(struct usb_ep *ep, struct usb_request *req)
{
	struct usb_function *fn = ep->driver_data;
	//struct usb_composite_dev *cdev = fn->config->cdev;
	struct audio_dev *agdev = func_to_agdev(fn);
	u32 val;

	if (req->actual != 4) {
		dev_info(NULL, "Invalid data size for UAC2_CS_CONTROL_SAM_FREQ.\n");
		return;
	}

	val = le32_to_cpu(*((u32 *)req->buf));
	if (agdev->ctl_id == USB_IN_CLK_ID) {
		agdev->p_srate_active = val;
		u_audio_set_playback_srate(agdev, agdev->p_srate_active);
	} else if (agdev->ctl_id == USB_OUT_CLK_ID) {
		agdev->c_srate_active = val;
		u_audio_set_capture_srate(agdev, agdev->c_srate_active);
	}
}

static void uac2_fu_control(struct usb_ep *ep, struct usb_request *req)
{
	struct usb_function *fn = ep->driver_data;
	//struct usb_composite_dev *cdev = fn->config->cdev;
	struct audio_dev *agdev = func_to_agdev(fn);
	int status = req->status;
	u32 data = 0;

	if (agdev->cs_id == UAC_FU_MUTE) {
		data = *((u8 *)req->buf);
		dev_info(NULL, "Set Mute: data:%x\n", data);
	} else if (agdev->cs_id == UAC_FU_VOLUME) {
		if (req->actual != 2) {
			dev_info(NULL, "Invalid data size for uac2_fu_control volume.\n");
			return;
		}
		data = le16_to_cpu(*((u16 *)req->buf));
		dev_info(NULL, "Set Volume: data:%x\n", data);
	} else {
		printf("%s:%d control_selector=%d TODO!\n",
				__func__, __LINE__, agdev->cs_id);
	}


	switch (status) {

	case 0:				/* normal completion? */
		if (agdev->set_con) {
			agdev->set_con->set(agdev->set_con, agdev->set_cmd,
					data);
			agdev->set_con = NULL;
		}
		break;
	default:
		break;
	}
}

static int
out_rq_cur(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	struct audio_dev *agdev = func_to_agdev(fn);
	struct usb_composite_dev *cdev = fn->config->cdev;
	struct usb_request *req = cdev->req;
	u16 w_length = le16_to_cpu(cr->wLength);
	u16 w_index = le16_to_cpu(cr->wIndex);
	u16 w_value = le16_to_cpu(cr->wValue);
	u8 control_selector = w_value >> 8;
	u8 entity_id = w_index >> 8 & 0xFF;
	u8 cmd = (cr->bRequest & 0x0F);
	struct usb_audio_control_selector *cs;
	struct usb_audio_control *con;

	switch (entity_id) {
	case USB_IN_CLK_ID:
	case USB_OUT_CLK_ID:
		if (control_selector == UAC2_CS_CONTROL_SAM_FREQ) {
			dev_info(NULL, "control_selector UAC2_CS_CONTROL_SAM_FREQ, clock: %d\n",
					entity_id);
			cdev->gadget->ep0->driver_data = fn;
			agdev->ctl_id = entity_id;
			req->complete = uac2_cs_control_sam_freq;
			return w_length;
		}
		break;
	case IO_OUT_FU_ID:
		list_for_each_entry(cs, &agdev->cs, list) {
			if (cs->id == entity_id) {
				list_for_each_entry(con, &cs->control, list) {
					if (con->type == control_selector) {
						agdev->set_con = con;
						break;
					}
				}
				break;
			}
		}

		agdev->set_cmd = cmd;
		agdev->cs_id = control_selector;
		cdev->gadget->ep0->driver_data = fn;
		req->complete = uac2_fu_control;

		return w_length;
		break;
	default:
		dev_info(NULL, "unsupport entity id\n");
		break;
	}
	return -EOPNOTSUPP;
}

static int
setup_rq_inf(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	struct audio_dev *agdev = func_to_agdev(fn);
	//struct snd_uac2_chip *uac2 = &agdev->uac2;
	u16 w_index = le16_to_cpu(cr->wIndex);
	u8 intf = w_index & 0xff;

	if (intf != agdev->ac_intf) {
		printf("%s:%d Error!\n", __func__, __LINE__);
		return -EOPNOTSUPP;
	}

	if (cr->bRequestType & USB_DIR_IN)
		return ac_rq_in(fn, cr);
	else if (cr->bRequest == UAC2_CS_CUR)
		return out_rq_cur(fn, cr);

	return -EOPNOTSUPP;
}

static int
afunc_setup(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	struct usb_composite_dev *cdev = fn->config->cdev;
	//struct audio_dev *agdev = func_to_agdev(fn);
	//struct snd_uac2_chip *uac2 = &agdev->uac2;
	struct usb_request *req = cdev->req;
	u16 w_length = le16_to_cpu(cr->wLength);
	int value = -EOPNOTSUPP;

	/* Only Class specific requests are supposed to reach here */
	if ((cr->bRequestType & USB_TYPE_MASK) != USB_TYPE_CLASS)
		return -EOPNOTSUPP;

	if ((cr->bRequestType & USB_RECIP_MASK) == USB_RECIP_INTERFACE)
		value = setup_rq_inf(fn, cr);
	else
		printf("afunc_setup: Error!\n");

	if (value >= 0) {
		req->length = value;
		req->zero = value < w_length;
		value = usb_ep_queue(cdev->gadget->ep0, req, GFP_ATOMIC);
		if (value < 0) {
			printf("afunc_setup: queue ep Error!\n");
			req->status = 0;
		}
	}

	return value;
}

static void afunc_free(struct usb_function *f)
{
	struct audio_dev *agdev;

	agdev = func_to_agdev(f);
	kfree(agdev);
}

static void afunc_unbind(struct usb_configuration *c, struct usb_function *f)
{
	struct audio_dev *agdev = func_to_agdev(f);
	struct uac2_rtd_params *prm;
#if 0
	alsa_uac2_exit(agdev);
#endif
	prm = &agdev->uac2.p_prm;
#ifndef REQ_FIX_MEM_ADDR
	kfree(prm->rbuf);
#endif
	prm = &agdev->uac2.c_prm;
#ifndef REQ_FIX_MEM_ADDR
	kfree(prm->rbuf);
#endif
	usb_free_all_descriptors(f);
}

static int uac2_bind_config(struct usb_configuration *c)
{
	struct audio_dev *agdev;
	int status;

	agdev = calloc(sizeof(*agdev), 1);
	if (!agdev)
		return -ENOMEM;

	agdev->gadget = c->cdev->gadget;

	agdev->func.name = "uac2_func";
	agdev->func.strings = fn_strings;
	agdev->func.bind = afunc_bind;
	agdev->func.unbind = afunc_unbind;
	agdev->func.set_alt = afunc_set_alt;
	agdev->func.get_alt = afunc_get_alt;
	agdev->func.disable = afunc_disable;
	agdev->func.setup = afunc_setup;
	agdev->func.free_func = afunc_free;

	agdev->p_chmask = UAC2_DEF_PCHMASK;
	memcpy(agdev->p_srate, p_srate,
			sizeof(agdev->p_srate));
	agdev->p_srate_active = UAC2_DEF_PSRATE;
	agdev->p_ssize = UAC2_DEF_PSSIZE;
	agdev->c_chmask = UAC2_DEF_CCHMASK;
	memcpy(agdev->c_srate, c_srate,
			sizeof(agdev->c_srate));
	agdev->c_srate_active = UAC2_DEF_CSRATE;
	agdev->c_ssize = UAC2_DEF_CSSIZE;

	status = usb_add_function(c, &agdev->func);
	if (status)
		free(agdev);

	control_selector_init(agdev);

	return status;
}

int uac2_add(struct usb_configuration *c)
{
	return uac2_bind_config(c);
}

//DECLARE_GADGET_AUDIO_BIND_CALLBACK(usb_audio_uac2, uac2_add);
struct g_audio_bind_callback g_audio_bind_callback_list[1] = {
		{
			.usb_function_name = "usb_audio_uac2",
			.fptr = uac2_add,
		},
};
