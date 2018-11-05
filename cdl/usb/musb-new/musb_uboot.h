/*
 * MUSB OTG driver u-boot specific functions
 *
 * Copyright © 2015 Hans de Goede <hdegoede@redhat.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __MUSB_UBOOT_H__
#define __MUSB_UBOOT_H__

#include <usb.h>
#include "linux-compat.h"
#include "usb-compat.h"

int usb_gadget_handle_interrupts(int index);

int usb_gadget_register_driver(struct usb_gadget_driver *driver);

int usb_gadget_unregister_driver(struct usb_gadget_driver *driver);

int musb_register(struct musb_hdrc_platform_data *plat, void *bdata,
			void *ctl_regs);
void musb_unregister(struct musb_hdrc_platform_data *plat);

#endif
