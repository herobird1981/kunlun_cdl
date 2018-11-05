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

/*gadget*/
struct musb *musb_gadget = NULL;

int usb_gadget_handle_interrupts(int index)
{
	if (musb_gadget->polling_mode) {
		int ret;
		WATCHDOG_RESET();
		if (!musb_gadget || !musb_gadget->isr)
			return -EINVAL;
		ret = musb_gadget->isr(0, musb_gadget);

		return ret;
	} else
		return 0;
}

int usb_gadget_register_driver(struct usb_gadget_driver *driver)
{
	int ret;

	if (!driver || driver->speed < USB_SPEED_FULL || !driver->bind ||
	    !driver->setup) {
		printf("bad parameter.\n");
		return -EINVAL;
	}

	if (!musb_gadget) {
		printf("Controller uninitialized\n");
		return -ENXIO;
	}

	ret = musb_gadget_start(&musb_gadget->g, driver);
	if (ret < 0) {
		printf("gadget_start failed with %d\n", ret);
		return ret;
	}

	ret = driver->bind(&musb_gadget->g);
	if (ret < 0) {
		printf("bind failed with %d\n", ret);
		return ret;
	}

	return 0;
}

int usb_gadget_unregister_driver(struct usb_gadget_driver *driver)
{
	if (driver->disconnect)
		driver->disconnect(&musb_gadget->g);
	if (driver->unbind)
		driver->unbind(&musb_gadget->g);
	return 0;
}

int musb_register(struct musb_hdrc_platform_data *plat, void *bdata,
			void *ctl_regs)
{
	struct musb **musbp;

	switch (plat->mode) {
	case MUSB_PERIPHERAL:
		musbp = &musb_gadget;
		break;

	default:
		return -EINVAL;
	}

	*musbp = musb_init_controller(plat, (struct device *)bdata, ctl_regs);
	if (!*musbp) {
		printf("Failed to init the controller\n");
		return -EIO;
	}

	return 0;
}

void musb_unregister(struct musb_hdrc_platform_data *plat)
{
	struct musb **musbp;

	switch (plat->mode) {
	case MUSB_PERIPHERAL:
		musbp = &musb_gadget;
		break;

	default:
		return;
	}

	if (*musbp) {
		musb_free(*musbp);
		*musbp = NULL;
	}
}

