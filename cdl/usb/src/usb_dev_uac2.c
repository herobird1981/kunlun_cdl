#include "usblib_config.h"

#include <asm/io.h>

#include "usblib.h"


#include <errno.h>
#include <common.h>
#include <command.h>
#include <console.h>
#include <g_audio.h>
//#include <part.h>
#include <usb.h>
#include <linux/usb/musb.h>


int musb_board_init(struct musb_controller *dev);
void musb_board_exit(void);

int usb_dev_uac2_init(struct musb_controller *dev)
{
	dev->usb_mode = MUSB_PERIPHERAL; /*device*/
	return musb_board_init(dev);
}

void usb_dev_uac2_exit(void)
{
	musb_board_exit();
}

int usb_dev_uac2(int controller_index)
{
	int ret;

	if (board_usb_init(controller_index, USB_INIT_DEVICE)) {
		info("uac2: Couldn't init USB controller.\n");
		return CMD_RET_FAILURE;
	}

	ret = g_audio_register("usb_audio_uac2");
	if (ret) {
		info("g_uac2_register failed");
		ret = CMD_RET_FAILURE;
		goto cleanup_board;
	}

	if (!g_audio_board_usb_cable_connected()) {
		puts("\rUSB cable not detected.\n" \
		     "Command exit.\n");
		ret = CMD_RET_FAILURE;
		goto cleanup_register;
	}

	while (1) {
		if (ctrlc())
			break;
		usb_gadget_handle_interrupts(controller_index);
	}

	ret = CMD_RET_SUCCESS;

cleanup_register:
	//i2s_exit(port);
	g_audio_unregister();
cleanup_board:
	board_usb_cleanup(controller_index, USB_INIT_DEVICE);

	return ret;
}
