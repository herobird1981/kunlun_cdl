/*
 * g_audio.c -- USB Audio Gadget
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
//#include <part.h>
#include <usb.h>
#include <g_audio.h>
#include "gadget_chips.h"

#include "f_uac2.h"  /* fix link f_uac2.o issue */

#define DRIVER_DESC		"USB Audio Gadget"
#define DRIVER_VERSION		"Feb 2, 2012"

/* Thanks to Linux Foundation for donating this product ID. */
#define AUDIO_VENDOR_NUM		0x1d6b	/* Linux Foundation */
#define AUDIO_PRODUCT_NUM		0x0101	/* Linux-USB Audio Gadget */

static struct usb_device_descriptor device_desc = {
	.bLength = sizeof device_desc,
	.bDescriptorType = USB_DT_DEVICE,

	.bcdUSB = __constant_cpu_to_le16(0x0110),
	.bDeviceClass = USB_CLASS_MISC,
	.bDeviceSubClass = 0x02,
	.bDeviceProtocol =	0x01,

	.idVendor = __constant_cpu_to_le16(AUDIO_VENDOR_NUM),
	.idProduct = __constant_cpu_to_le16(AUDIO_PRODUCT_NUM),
	/* .bcdDevice = f(hardware) */
	/* .iManufacturer = DYNAMIC */
	/* .iProduct = DYNAMIC */
	/* NO SERIAL NUMBER */
	.bNumConfigurations = 1,
};

/*
 * static strings, in UTF-8
 * IDs for those strings are assigned dynamically at g_audio_bind()
 */
static struct usb_string g_audio_string_defs[] = {
	[USB_GADGET_MANUFACTURER_IDX].s = CONFIG_G_AUDIO_MANUFACTURER,
	[USB_GADGET_PRODUCT_IDX].s = DRIVER_DESC,
	[USB_GADGET_SERIAL_IDX].s = "",
	{  } /* end of list */
};

static struct usb_gadget_strings g_audio_string_tab = {
	.language = 0x0409, /* en-us */
	.strings = g_audio_string_defs,
};

static struct usb_gadget_strings *g_audio_composite_strings[] = {
	&g_audio_string_tab,
	NULL,
};

static int g_audio_unbind(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;

	printf("%s: calling usb_gadget_disconnect for "
			"controller '%s'\n", __func__, gadget->name);
	usb_gadget_disconnect(gadget);

	return 0;
}

static inline struct g_audio_bind_callback *g_audio_bind_callback_first(void)
{
	//return ll_entry_start(struct g_audio_bind_callback,
	//			g_audio_bind_callbacks);
	return &g_audio_bind_callback_list[0];
}

static inline struct g_audio_bind_callback *g_audio_bind_callback_end(void)
{
	//return ll_entry_end(struct g_audio_bind_callback,
	//			g_audio_bind_callbacks);
	
	return &g_audio_bind_callback_list[0]+ARRAY_SIZE(g_audio_bind_callback_list);
}

static int g_audio_do_config(struct usb_configuration *c)
{
	const char *s = c->cdev->driver->name;
	struct g_audio_bind_callback *callback = g_audio_bind_callback_first();

	dev_dbg(NULL, "%s: configuration: 0x%p composite dev: 0x%p callback:%p\n",
	      __func__, c, c->cdev, callback);

	for (; callback != g_audio_bind_callback_end(); callback++) {
		dev_dbg(NULL, "s:%s, name:%s\n", s, callback->usb_function_name);
		if (!strcmp(s, callback->usb_function_name))
			return callback->fptr(c);
	}
	return -ENODEV;
}

static int g_audio_config_register(struct usb_composite_dev *cdev)
{
	struct usb_configuration *config;
	const char *name = "usb_audio";

	config = memalign(CONFIG_SYS_CACHELINE_SIZE, sizeof(*config));
	if (!config)
		return -ENOMEM;

	memset(config, 0, sizeof(*config));

	config->label = name;
	config->bmAttributes = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER;
	config->bConfigurationValue = 1;
	config->bind = g_audio_do_config;
	return usb_add_config(cdev, config);
}

__weak
int board_usb_init(int index, enum usb_init_type init)
{
	return 0;
}

__weak
int board_usb_cleanup(int index, enum usb_init_type init)
{
	return 0;
}

__weak
int g_audio_bind_fixup(struct usb_device_descriptor *dev, const char *name)
{
	return 0;
}

__weak int g_audio_get_board_bcd_device_number(int gcnum)
{
	return gcnum;
}

__weak int g_audio_board_usb_cable_connected(void)
{
	return -EOPNOTSUPP;
}

static int g_audio_get_bcd_device_number(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;
	int gcnum;

	gcnum = usb_gadget_controller_number(gadget);
	if (gcnum > 0)
		gcnum += 0x200;

	return g_audio_get_board_bcd_device_number(gcnum);
}

static int g_audio_bind(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;
	int status;
	int gcnum;

	status = usb_string_ids_tab(cdev, g_audio_string_defs);
	if (status < 0)
		goto fail;
	device_desc.iManufacturer = g_audio_string_defs[USB_GADGET_MANUFACTURER_IDX].id;
	device_desc.iProduct = g_audio_string_defs[USB_GADGET_PRODUCT_IDX].id;
	g_audio_bind_fixup(&device_desc, cdev->driver->name);

	status = g_audio_config_register(cdev);
	if (status < 0)
		goto fail;

	gcnum = g_audio_get_bcd_device_number(cdev);
	if (gcnum >= 0)
		device_desc.bcdDevice = cpu_to_le16(gcnum);
	else {
		printf("%s: controller '%s' not recognized\n",
			__func__, gadget->name);
		device_desc.bcdDevice = __constant_cpu_to_le16(0x9999);
	}

	dev_dbg(NULL, "%s: calling usb_gadget_connect for "
			"controller '%s'\n", __func__, gadget->name);
	usb_gadget_connect(gadget);

	return 0;
fail:
	g_audio_unbind(cdev);
	return -ENOMEM;
}

static struct usb_composite_driver g_audio_driver = {
	.name = NULL,
	.dev = &device_desc,
	.strings = g_audio_composite_strings,

	.bind = g_audio_bind,
	.unbind = g_audio_unbind,
};

/*
 * NOTICE:
 * Registering via USB function name won't be necessary after rewriting
 * g_audio to support multiple USB functions.
 */
int g_audio_register(const char *name)
{
	int ret;

	dev_dbg(NULL, "%s: g_audio_driver.name = %s\n", __func__, name);
	g_audio_driver.name = name;

	ret = usb_composite_register(&g_audio_driver);
	if (ret) {
		printf("%s: failed!, error: %d\n", __func__, ret);
		return ret;
	}
	return 0;
}

void g_audio_unregister(void)
{
	usb_composite_unregister(&g_audio_driver);
}
