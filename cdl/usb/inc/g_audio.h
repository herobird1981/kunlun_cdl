/*
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __G_AUDIO_H_
#define __G_AUDIO_H_

#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/composite.h>
#include <linker_lists.h>

/*
 * @usb_fname: unescaped USB function name
 * @callback_ptr: bind callback, one per function name
 */
#define DECLARE_GADGET_AUDIO_BIND_CALLBACK(usb_fname, callback_ptr) \
	ll_entry_declare(struct g_audio_bind_callback, \
			__usb_function_name_##usb_fname, \
			g_audio_bind_callbacks) = { \
				.usb_function_name = #usb_fname, \
				.fptr = callback_ptr \
			}

typedef int (*g_audio_bind_callback_f)(struct usb_configuration *);

/* used in Gadget downloader callback linker list */
struct g_audio_bind_callback {
	const char *usb_function_name;
	g_audio_bind_callback_f fptr;
};

int g_audio_bind_fixup(struct usb_device_descriptor *, const char *);
int g_audio_get_board_bcd_device_number(int gcnum);
int g_audio_board_usb_cable_connected(void);
int g_audio_register(const char *s);
void g_audio_unregister(void);
void g_audio_set_serialnumber(char *);

#endif /* __G_AUDIO_H_ */
