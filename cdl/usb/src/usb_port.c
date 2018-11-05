#include "usblib_config.h"

#include <asm/io.h>

#include "usblib.h"

#include <common.h>
#include <asm/errno.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>

#include <usb.h>
#include "linux-compat.h"
#include "usb-compat.h"
#include "musb_core.h"
#include "musb_gadget.h"
#include "musb_uboot.h"

#include "hw_maps.h"

static int ctrlc_was_pressed = 0;

int ctrlc(void)
{
	if (console_tstc()) {
		switch (console_getc()) {
		case 0x03:		/* ^C - Control C */
			ctrlc_was_pressed = 1;
			return 1;
		case 'q':		/* Quit */
			return 1;
		default:
			break;
		}
	}


	return 0;
}

static int musb_init(struct musb *musb)
{
	//info("%s\n",__FUNCTION__);

	return 0;
}

static int musb_exit(struct musb *musb)
{
	//info("%s\n",__FUNCTION__);

	return 0;
}

const struct musb_platform_ops musb_ops = {
	.init		= musb_init,
	.exit		= musb_exit,
};

static struct musb_hdrc_config musb_config = {
	.multipoint     = 0,
	.dyn_fifo       = 1,
	.num_eps        = 8,
	.ram_bits       = 12,			/* ram address size */
};

static struct musb_hdrc_platform_data musb_plat = {
	.config         = &musb_config,
	.power          = 250,
	.platform_ops	= &musb_ops,
};


int musb_board_init(struct musb_controller *dev)
{
	int ret = 0;

	musb_plat.mode = dev->usb_mode;
	musb_plat.int_vector = dev->int_vector;
	musb_plat.edma_mode = dev->edma_mode;
	musb_plat.polling_mode = dev->polling_mode;
	musb_plat.int_dma_vector = dev->int_dma_vector;
	musb_plat.i2s_tx_port = dev->i2s_tx_port;
	musb_plat.store= dev->store;

	usb_clk_config();

	ret = musb_register(&musb_plat, NULL, dev->regs);

	return ret;
}

void musb_board_exit(void)
{
	musb_unregister(&musb_plat);
}

