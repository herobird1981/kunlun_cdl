/*
 * MUSB OTG driver peripheral defines
 *
 * Copyright 2005 Mentor Graphics Corporation
 * Copyright (C) 2005-2006 by Texas Instruments
 * Copyright (C) 2006-2007 Nokia Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef __MUSB_GADGET_H
#define __MUSB_GADGET_H

#include <linux/list.h>
#include <asm/byteorder.h>
#include <asm/errno.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>

struct musb_request {
	struct usb_request	request;
	struct list_head	list;
	struct musb_ep		*ep;
	struct musb		*musb;
	u8 tx;			/* endpoint direction */
	u8 epnum;
};

static inline struct musb_request *to_musb_request(struct usb_request *req)
{
	return req ? container_of(req, struct musb_request, request) : NULL;
}

extern struct usb_request *
musb_alloc_request(struct usb_ep *ep, gfp_t gfp_flags);
extern void musb_free_request(struct usb_ep *ep, struct usb_request *req);


/*
 * struct musb_ep - peripheral side view of endpoint rx or tx side
 */
struct musb_ep {
	/* stuff towards the head is basically write-once. */
	struct usb_ep			end_point;
	char				name[12];
	struct musb_hw_ep		*hw_ep;
	struct musb			*musb;
	u8				current_epnum;

	/* ... when enabled/disabled ... */
	u8				type;
	u8				is_in;
	u16				packet_sz;
	const struct usb_endpoint_descriptor	*desc;

	/* later things are modified based on usage */
	struct list_head		req_list;

	u8				wedged;

	/* true if lock must be dropped but req_list may not be advanced */
	u8				busy;
	u8				edma_busy;

};

static inline struct musb_ep *to_musb_ep(struct usb_ep *ep)
{
	return ep ? container_of(ep, struct musb_ep, end_point) : NULL;
}

static inline struct musb_request *next_request(struct musb_ep *ep)
{
	struct list_head	*queue = &ep->req_list;

	if (list_empty(queue))
		return NULL;
	return container_of(queue->next, struct musb_request, list);
}

int musb_gadget_start(struct usb_gadget *g,
		struct usb_gadget_driver *driver);

extern void musb_g_tx(struct musb *musb, u8 epnum);
extern void musb_g_rx(struct musb *musb, u8 epnum);

extern const struct usb_ep_ops musb_g_ep0_ops;

extern int musb_gadget_setup(struct musb *);
extern void musb_gadget_cleanup(struct musb *);

extern void musb_g_giveback(struct musb_ep *, struct usb_request *, int);

extern void musb_ep_restart(struct musb *, struct musb_request *);

#endif		/* __MUSB_GADGET_H */
