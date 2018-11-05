/*
 * MUSB OTG driver peripheral support
 *
 * Copyright 2005 Mentor Graphics Corporation
 * Copyright (C) 2005-2006 by Texas Instruments
 * Copyright (C) 2006-2007 Nokia Corporation
 * Copyright (C) 2009 MontaVista Software, Inc. <source@mvista.com>
 *
 * SPDX-License-Identifier:	GPL-2.0
 */
#include "musb_core.h"
#include <common.h>
#include <linux/usb/ch9.h>
#include "linux-compat.h"

#include <asm/bitops.h>



/* MUSB PERIPHERAL status 3-mar-2006:
 *
 * - EP0 seems solid.  It passes both USBCV and usbtest control cases.
 *   Minor glitches:
 *
 *     + remote wakeup to Linux hosts work, but saw USBCV failures;
 *       in one test run (operator error?)
 *     + endpoint halt tests -- in both usbtest and usbcv -- seem
 *       to break when dma is enabled ... is something wrongly
 *       clearing SENDSTALL?
 *
 * - Mass storage behaved ok when last tested.  Network traffic patterns
 *   (with lots of short transfers etc) need retesting; they turn up the
 *   worst cases of the DMA, since short packets are typical but are not
 *   required.
 *
 * - TX/IN
 *     + both pio and dma behave in with network and g_zero tests
 *     + no cppi throughput issues other than no-hw-queueing
 *     + failed with FLAT_REG (DaVinci)
 *     + seems to behave with double buffering, PIO -and- CPPI
 *     + with gadgetfs + AIO, requests got lost?
 *
 * - RX/OUT
 *     + both pio and dma behave in with network and g_zero tests
 *     + dma is slow in typical case (short_not_ok is clear)
 *     + double buffering ok with PIO
 *     + double buffering *FAILS* with CPPI, wrong data bytes sometimes
 *     + request lossage observed with gadgetfs
 *
 * - ISO not tested ... might work, but only weakly isochronous
 *
 * - Gadget driver disabling of softconnect during bind() is ignored; so
 *   drivers can't hold off host requests until userspace is ready.
 *   (Workaround:  they can turn it off later.)
 *
 * - PORTABILITY (assumes PIO works):
 *     + DaVinci, basically works with cppi dma
 *     + OMAP 2430, ditto with mentor dma
 *     + TUSB 6010, platform-specific dma in the works
 */

/* ----------------------------------------------------------------------- */

/*
 * Immediately complete a request.
 *
 * @param request the request to complete
 * @param status the status to complete the request with
 * Context: controller locked, IRQs blocked.
 */
void musb_g_giveback(
	struct musb_ep		*ep,
	struct usb_request	*request,
	int			status)
{
	struct musb_request	*req;
	struct musb		*musb;
	int			busy = ep->busy;

	req = to_musb_request(request);

	list_del(&req->list);
	if (req->request.status == -EINPROGRESS)
		req->request.status = status;
	musb = req->musb;

	ep->busy = 1;
	ep->edma_busy = 0;
	if (request->status == 0)
		dev_dbg(musb->controller, "%s done request %p,  %d/%d\n",
				ep->end_point.name, request,
				req->request.actual, req->request.length);
	else
		dev_dbg(musb->controller, "%s request %p, %d/%d fault %d\n",
				ep->end_point.name, request,
				req->request.actual, req->request.length,
				request->status);
	req->request.complete(&req->ep->end_point, &req->request);
	ep->busy = busy;
}

/* ----------------------------------------------------------------------- */

/*
 * Abort requests queued to an endpoint using the status. Synchronous.
 * caller locked controller and blocked irqs, and selected this ep.
 */
static void nuke(struct musb_ep *ep, const int status)
{
	//struct musb		*musb = ep->musb;
	struct musb_request	*req = NULL;

	ep->busy = 1;
#if 0
	void __iomem *epio = musb->endpoints[ep->current_epnum].regs;
    if (musb->edma_mode) {
		if (ep->is_in) {
			/*
			* The programming guide says that we must not clear
			* the DMAMODE bit before DMAENAB, so we only
			* clear it in the second write...
			*/
			musb_write_txcsr(epio, MUSB_TXCSR_DMAMODE | MUSB_TXCSR_FLUSHFIFO);
			musb_write_txcsr(epio, 0 | MUSB_TXCSR_FLUSHFIFO);

			dev_info(NULL, "warning:ch %d still active, force stop it\n", DMA_CH5);
		} else {
			musb_write_rxcsr(epio, 0 | MUSB_RXCSR_FLUSHFIFO);
			musb_write_rxcsr(epio, 0 | MUSB_RXCSR_FLUSHFIFO);
			dev_info(NULL, "warning:ch %d still active, force stop it\n", DMA_CH4);
		}
	}
#endif
	while (!list_empty(&ep->req_list)) {
		req = list_first_entry(&ep->req_list, struct musb_request, list);
		musb_g_giveback(ep, &req->request, status);
	}
}

/* ----------------------------------------------------------------------- */

/* Data transfers - pure PIO, pure DMA, or mixed mode */

/*
 * This assumes the separate CPPI engine is responding to DMA requests
 * from the usb core ... sequenced a bit differently from mentor dma.
 */

#ifdef CONFIG_USB_INVENTRA_DMA

/* Peripheral tx (IN) using Mentor DMA works as follows:
	Only mode 0 is used for transfers <= wPktSize,
	mode 1 is used for larger transfers,

	One of the following happens:
	- Host sends IN token which causes an endpoint interrupt
		-> TxAvail
			-> if DMA is currently busy, exit.
			-> if queue is non-empty, txstate().

	- Request is queued by the gadget driver.
		-> if queue was previously empty, txstate()

	txstate()
		-> start
		  /\	-> setup DMA
		  |     (data is transferred to the FIFO, then sent out when
		  |	IN token(s) are recd from Host.
		  |		-> DMA interrupt on completion
		  |		   calls TxAvail.
		  |		      -> stop DMA, ~DMAENAB,
		  |		      -> set TxPktRdy for last short pkt or zlp
		  |		      -> Complete Request
		  |		      -> Continue next request (call txstate)
		  |___________________________________|

 * Non-Mentor DMA engines can of course work differently, such as by
 * upleveling from irq-per-packet to irq-per-buffer.
 */

#endif

/*
 * An endpoint is transmitting data. This can be called either from
 * the IRQ routine or from ep.queue() to kickstart a request on an
 * endpoint.
 *
 * Context: controller locked, IRQs blocked, endpoint selected
 */
static void txstate(struct musb *musb, struct musb_request *req)
{
	u8			epnum = req->epnum;
	struct musb_ep		*musb_ep;
	void __iomem		*epio = musb->endpoints[epnum].regs;
	struct usb_request	*request;
	u16			csr;
	int			use_dma = 0;
	int fifo_count = 0;

	musb_ep = req->ep;

	/* read TXCSR before */
	csr = musb_read_txcsr(epio);

	request = &req->request;
	fifo_count = min(musb_ep->packet_sz,
			(int)(request->length - request->actual));

	if (csr & MUSB_TXCSR_TXPKTRDY) {
		dev_dbg(musb->controller, "%s old packet still ready , txcsr %03x\n",
				musb_ep->end_point.name, csr);
		return;
	}

	if (csr & MUSB_TXCSR_P_SENDSTALL) {
		dev_dbg(musb->controller, "%s stalling, txcsr %03x\n",
				musb_ep->end_point.name, csr);
		return;
	}

	if (musb->edma_mode) {
		if (musb_ep->edma_busy == 1) {
			dev_info(musb->controller, "txep DMA busy...\n");
			return;
		}

		if (dma_usb_tx(musb_ep->hw_ep, DMA_CH5, CH_PRIOR_0, (uint32_t)request->buf, request->length, ST_NO_POLLING))
			info("dma_usb_tx error\n");

		csr |= (MUSB_TXCSR_DMAENAB);
		//csr |= MUSB_TXCSR_TXPKTRDY;
		//musb_write_txcsr(epio, csr);
		musb_writeb(epio, MUSB_TXCSR2, (csr >> 8));
	
		csr &= ~MUSB_TXCSR_P_UNDERRUN;
		csr |= MUSB_TXCSR_TXPKTRDY;
		//musb_write_txcsr(epio, csr);
		musb_writeb(epio, MUSB_TXCSR1, csr);
		musb_ep->edma_busy = 1;
		return;
	} 

	dev_dbg(musb->controller, "txstate: hw_ep%d, maxpacket %d, fifo count %d, txcsr %03x\n",
			epnum, musb_ep->packet_sz, fifo_count,
			csr);

	if (!use_dma) {
		/*
		 * Unmap the dma buffer back to cpu if dma channel
		 * programming fails
		 */

		musb_write_fifo(musb_ep->hw_ep, fifo_count,
				(u8 *) (request->buf + request->actual));
		request->actual += fifo_count;
		csr |= MUSB_TXCSR_TXPKTRDY;
		csr &= ~MUSB_TXCSR_P_UNDERRUN;
		musb_write_txcsr(epio, csr);
		//musb_writeb(epio, MUSB_TXCSR1, csr);
	}

	/* host may already have the data when this message shows... */
	dev_dbg(musb->controller, "%s TX/IN %s len %d/%d, txcsr %04x, fifo %d/%d\n",
			musb_ep->end_point.name, use_dma ? "dma" : "pio",
			request->actual, request->length,
			musb_read_txcsr(epio),
			fifo_count,
			musb_readb(epio, MUSB_TXMAXP) << 3);
}

/*
 * FIFO state update (e.g. data ready).
 * Called from IRQ,  with controller locked.
 */
void musb_g_tx(struct musb *musb, u8 epnum)
{
	u16			csr;
	struct musb_request	*req;
	struct usb_request	*request;
	u8 __iomem		*mbase = musb->mregs;
	struct musb_ep		*musb_ep = &musb->endpoints[epnum].ep_in;
	void __iomem		*epio = musb->endpoints[epnum].regs;

	musb_ep_select(mbase, epnum);
	req = next_request(musb_ep);
	request = &req->request;

	//csr = musb_read_txcsr(epio);
	csr = musb_readb(epio, MUSB_TXCSR1);
	dev_dbg(musb->controller, "musb_g_tx: <== %s, txcsr %04x\n", musb_ep->end_point.name, csr);

	/*
	 * REVISIT: for high bandwidth, MUSB_TXCSR_P_INCOMPTX
	 * probably rates reporting as a host error.
	 */
	if (csr & MUSB_TXCSR_P_SENTSTALL) {
		dev_info(musb->controller, "musb_g_tx sent stall\n");
		csr |=	MUSB_TXCSR_P_WZC_BITS;
		csr &= ~MUSB_TXCSR_P_SENTSTALL;
		//musb_write_txcsr(epio, csr);
		musb_writeb(epio, MUSB_TXCSR1, csr);
		return;
	}

	if (csr & MUSB_TXCSR_P_UNDERRUN) {
		/* We NAKed, no big deal... little reason to care. */
		/*csr |=	 MUSB_TXCSR_P_WZC_BITS;*/
		csr &= ~(MUSB_TXCSR_P_UNDERRUN/* | MUSB_TXCSR_TXPKTRDY*/);
		//musb_write_txcsr(epio, csr);
		musb_writeb(epio, MUSB_TXCSR1, csr);
		dev_dbg(musb->controller, "underrun on ep%d, req %p\n",
				epnum, request);
	}
	if (musb_ep->edma_busy == 1) {
		dma_polling_st(DMA0, DMA_CH5);
		request->actual += request->length;

		dev_dbg(musb->controller, "TXCSR%d %04x, dma off, %04x, req %p\n",
			epnum, csr,
			musb_read_txcsr(epio),
			request);
	}
	if (request) {
		if (request->actual == request->length) {
			musb_g_giveback(musb_ep, request, 0);
			/*
			 * In the giveback function the MUSB lock is
			 * released and acquired after sometime. During
			 * this time period the INDEX register could get
			 * changed by the gadget_queue function especially
			 * on SMP systems. Reselect the INDEX to be sure
			 * we are reading/modifying the right registers
			 */
			musb_ep_select(mbase, epnum);
			req = musb_ep->desc ? next_request(musb_ep) : NULL;
			if (!req) {
				dev_dbg(musb->controller, "%s idle now\n",
					musb_ep->end_point.name);
				return;
			}
		}

		txstate(musb, req);
	}
}

/* ------------------------------------------------------------ */

#ifdef CONFIG_USB_INVENTRA_DMA

/* Peripheral rx (OUT) using Mentor DMA works as follows:
	- Only mode 0 is used.

	- Request is queued by the gadget class driver.
		-> if queue was previously empty, rxstate()

	- Host sends OUT token which causes an endpoint interrupt
	  /\      -> RxReady
	  |	      -> if request queued, call rxstate
	  |		/\	-> setup DMA
	  |		|	     -> DMA interrupt on completion
	  |		|		-> RxReady
	  |		|		      -> stop DMA
	  |		|		      -> ack the read
	  |		|		      -> if data recd = max expected
	  |		|				by the request, or host
	  |		|				sent a short packet,
	  |		|				complete the request,
	  |		|				and start the next one.
	  |		|_____________________________________|
	  |					 else just wait for the host
	  |					    to send the next OUT token.
	  |__________________________________________________|

 * Non-Mentor DMA engines can of course work differently.
 */

#endif

/*
 * Context: controller locked, IRQs blocked, endpoint selected
 */
static void rxstate(struct musb *musb, struct musb_request *req)
{
	const u8		epnum = req->epnum;
	struct usb_request	*request = &req->request;
	struct musb_ep		*musb_ep;
	void __iomem		*epio = musb->endpoints[epnum].regs;
	u16			fifo_count;
	u16			csr = musb_read_rxcsr(epio);
	struct musb_hw_ep	*hw_ep = &musb->endpoints[epnum];

	if (hw_ep->is_shared_fifo)
		musb_ep = &hw_ep->ep_in;
	else
		musb_ep = &hw_ep->ep_out;

	fifo_count = musb_ep->packet_sz;

	if (csr & MUSB_RXCSR_P_SENDSTALL) {
		dev_info(musb->controller, "%s stalling, RXCSR %04x\n",
		    musb_ep->end_point.name, csr);
		return;
	}
#if 1
	if (musb->edma_mode) {
		if (musb_ep->edma_busy == 1) {
			dev_dbg(musb->controller, "ep DMA busy...\n");
			return;
		}
		//request->buf = test_txrx_buff;
		if (dma_usb_rx(musb_ep->hw_ep, DMA_CH4, CH_PRIOR_0, (uint32_t)request->buf, request->length, ST_NO_POLLING))
			info("dma_usb_rx error\n");
		csr |= MUSB_RXCSR_DMAENAB;
		//musb_write_rxcsr(epio, csr);
		musb_writeb(epio, MUSB_RXCSR2, (csr >> 8));
		musb_ep->edma_busy = 1;
		return;
	}
#endif

	if (csr & MUSB_RXCSR_RXPKTRDY) {
		fifo_count = musb_read_rxcount(epio);

		if (request->actual < request->length) {
			musb_read_fifo(musb_ep->hw_ep, fifo_count, (u8 *)
					(request->buf + request->actual));
			request->actual += fifo_count;

			/* REVISIT if we left anything in the fifo, flush
			 * it and report -EOVERFLOW
			 */

			/* ack the read! */
			csr |= MUSB_RXCSR_P_WZC_BITS;
			csr &= ~MUSB_RXCSR_RXPKTRDY;
			//musb_write_rxcsr(epio, csr);
			musb_writeb(epio, MUSB_RXCSR1, csr);
		}
	}

	/* reach the end or short packet detected */
	if (request->actual == request->length ||
	    fifo_count < musb_ep->packet_sz)
		musb_g_giveback(musb_ep, request, 0);
}

/*
 * Data ready for a request; called from IRQ
 */
void musb_g_rx(struct musb *musb, u8 epnum)
{
	u16			csr;
	struct musb_request	*req;
	struct usb_request	*request;
	void __iomem		*mbase = musb->mregs;
	struct musb_ep		*musb_ep;
	void __iomem		*epio = musb->endpoints[epnum].regs;
	struct musb_hw_ep	*hw_ep = &musb->endpoints[epnum];

	if (hw_ep->is_shared_fifo)
		musb_ep = &hw_ep->ep_in;
	else
		musb_ep = &hw_ep->ep_out;

	musb_ep_select(mbase, epnum);

	req = next_request(musb_ep);
	if (!req)
		return;

	request = &req->request;

	//csr = musb_read_rxcsr(epio);
	csr = musb_readb(epio, MUSB_RXCSR1);

	dev_dbg(musb->controller, "musb_g_rx: <== %s, rxcsr %04x%s %p\n", musb_ep->end_point.name,
			csr, musb->edma_mode ? " (dma)" : "", request);

	if (csr & MUSB_RXCSR_P_SENTSTALL) {
		csr |= MUSB_RXCSR_P_WZC_BITS;
		csr &= ~MUSB_RXCSR_P_SENTSTALL;
		//musb_write_rxcsr(epio, csr);
		musb_writeb(epio, MUSB_RXCSR1, csr);
		return;
	}

	if (csr & MUSB_RXCSR_P_OVERRUN) {
		/* csr |= MUSB_RXCSR_P_WZC_BITS; */
		csr &= ~MUSB_RXCSR_P_OVERRUN;
		//musb_write_rxcsr(epio, csr);
		musb_writeb(epio, MUSB_RXCSR1, csr);

		dev_dbg(musb->controller, "%s iso overrun on %p\n", musb_ep->name, request);
		if (request->status == -EINPROGRESS)
			request->status = -EOVERFLOW;
	}

	if (musb_ep->edma_busy == 1) {
		dma_polling_st(DMA0, DMA_CH4);
		request->actual += request->length;

		dev_dbg(musb->controller, "RXCSR%d %04x, dma off, %04x, len %zu, req %p\n",
			epnum, csr,
			musb_read_rxcsr(epio),
			musb_read_rxcount(epio), request);
#if 1
		/* ack the read! */
		csr |= MUSB_RXCSR_P_WZC_BITS;
		csr &= ~MUSB_RXCSR_RXPKTRDY;
		//musb_write_rxcsr(epio, csr);
		musb_writeb(epio, MUSB_RXCSR1, csr);

		musb_g_giveback(musb_ep, request, 0);
		/*
		 * In the giveback function the MUSB lock is
		 * released and acquired after sometime. During
		 * this time period the INDEX register could get
		 * changed by the gadget_queue function especially
		 * on SMP systems. Reselect the INDEX to be sure
		 * we are reading/modifying the right registers
		 */
		musb_ep_select(mbase, epnum);

		req = next_request(musb_ep);
		if (!req)
			return;
#endif
	}
	/* Analyze request */
	rxstate(musb, req);
}

/* ------------------------------------------------------------ */

static int musb_gadget_enable(struct usb_ep *ep,
			const struct usb_endpoint_descriptor *desc)
{
	unsigned long		flags;
	struct musb_ep		*musb_ep;
	struct musb_hw_ep	*hw_ep;
	void __iomem		*regs;
	struct musb		*musb;
	void __iomem	*mbase;
	u8		epnum;
	u16		csr;
	unsigned	tmp;
	int		status = -EINVAL;

	if (!ep || !desc)
		return -EINVAL;

	musb_ep = to_musb_ep(ep);
	hw_ep = musb_ep->hw_ep;
	regs = hw_ep->regs;
	musb = musb_ep->musb;
	mbase = musb->mregs;
	epnum = musb_ep->current_epnum;

	spin_lock_irqsave(&musb->lock, flags);

	if (musb_ep->desc) {
		status = -EBUSY;
		goto fail;
	}
	musb_ep->type = usb_endpoint_type(desc);

	/* check direction and (later) maxpacket size against endpoint */
	if (usb_endpoint_num(desc) != epnum)
		goto fail;

	/* REVISIT this rules out high bandwidth periodic transfers */
	tmp = usb_endpoint_maxp(desc);

	musb_ep->packet_sz = tmp & 0x7ff;

	/* enable the interrupts for the endpoint, set the endpoint
	 * packet size (or fail), set the mode, clear the fifo
	 */
	musb_ep_select(mbase, epnum);
	if (usb_endpoint_dir_in(desc)) {
		u16 int_txe = musb_readb(mbase, MUSB_INTRTX1E);

		if (hw_ep->is_shared_fifo)
			musb_ep->is_in = 1;
		if (!musb_ep->is_in)
			goto fail;

		if (tmp > hw_ep->max_packet_sz_tx) {
			dev_info(musb->controller, "packet size beyond hardware FIFO size\n");
			goto fail;
		}

		int_txe |= (1 << epnum);
		musb_writeb(mbase, MUSB_INTRTX1E, int_txe);

		/* REVISIT if can_bulk_split(), use by updating "tmp";
		 * likewise high bandwidth periodic tx
		 */
		/* Set TXMAXP with the FIFO size of the endpoint
		 * to disable double buffering mode.
		 */
		if (musb->double_buffer_not_ok) {
			musb_writeb(regs, MUSB_TXMAXP, hw_ep->max_packet_sz_tx >> 3);
		} else {
			musb_writeb(regs, MUSB_TXMAXP, (musb_ep->packet_sz >> 3));
		}

		csr = MUSB_TXCSR_MODE | MUSB_TXCSR_CLRDATATOG;
		if (musb_read_txcsr(regs)
				& MUSB_TXCSR_FIFONOTEMPTY)
			csr |= MUSB_TXCSR_FLUSHFIFO;
		if (musb_ep->type == USB_ENDPOINT_XFER_ISOC)
			csr |= MUSB_TXCSR_P_ISO;

		/* set twice in case of double buffering */
		musb_write_txcsr(regs, csr);
		/* REVISIT may be inappropriate w/o FIFONOTEMPTY ... */
		musb_write_txcsr(regs, csr);

#if 0
		if (musb->edma_mode) {
			u16	maxpacket = musb_ep->packet_sz;
			u8	c_size;
			int size = 0;
			u8 txfifo2;

			size = ffs(max(maxpacket, (u16) 8)) - 1;
			maxpacket = 1 << size;

			c_size = size - 3;
			c_size <<= 5;

			txfifo2 = musb_readb(musb->mregs, MUSB_TXFIFOSZ);
			txfifo2 = (txfifo2 & ~MUSB_FIFOSZ_SIZE) | c_size;
			musb_writeb(musb->mregs, MUSB_TXFIFOSZ, txfifo2);
			hw_ep->max_packet_sz_tx = musb_ep->packet_sz;
        }

		if (hw_ep->max_packet_sz_tx >= 2 * musb_ep->packet_sz)
			hw_ep->tx_double_buffered = true;
#endif
	} else {
		u16 int_rxe = musb_readb(mbase, MUSB_INTRRX1E);

		if (hw_ep->is_shared_fifo)
			musb_ep->is_in = 0;
		if (musb_ep->is_in)
			goto fail;

		if (tmp > hw_ep->max_packet_sz_rx) {
			dev_info(musb->controller, "packet size beyond hardware FIFO size\n");
			goto fail;
		}

		int_rxe |= (1 << epnum);
		musb_writeb(mbase, MUSB_INTRRX1E, int_rxe);

		/* REVISIT if can_bulk_combine() use by updating "tmp"
		 * likewise high bandwidth periodic rx
		 */
		/* Set RXMAXP with the FIFO size of the endpoint
		 * to disable double buffering mode.
		 */
		if (musb->double_buffer_not_ok)
			musb_writeb(regs, MUSB_RXMAXP, hw_ep->max_packet_sz_tx >> 3);
		else
			musb_writeb(regs, MUSB_RXMAXP, (musb_ep->packet_sz >> 3));

		/* force shared fifo to OUT-only mode */
		if (hw_ep->is_shared_fifo) {
			csr = musb_read_txcsr(regs);
			csr &= ~(MUSB_TXCSR_MODE | MUSB_TXCSR_TXPKTRDY);
			musb_write_txcsr(regs, csr);
		}

		csr = MUSB_RXCSR_FLUSHFIFO | MUSB_RXCSR_CLRDATATOG;
		if (musb_ep->type == USB_ENDPOINT_XFER_ISOC)
			csr |= MUSB_RXCSR_P_ISO;

		/* set twice in case of double buffering */
		musb_write_rxcsr(regs, csr);
		musb_write_rxcsr(regs, csr);
#if 0
		if (musb->edma_mode) {
			u16	maxpacket = musb_ep->packet_sz;
			u8	c_size;
			int size = 0;
			u8 rxfifo2;

			size = ffs(max(maxpacket, (u16) 8)) - 1;
			maxpacket = 1 << size;

			c_size = size - 3;
			c_size <<= 5;

			rxfifo2 = musb_readb(musb->mregs, MUSB_RXFIFOSZ);
			rxfifo2 = (rxfifo2 & ~MUSB_FIFOSZ_SIZE) | c_size;
			musb_writeb(musb->mregs, MUSB_RXFIFOSZ, rxfifo2);
			hw_ep->max_packet_sz_rx = musb_ep->packet_sz;
        }

		if (hw_ep->max_packet_sz_rx >= 2 * musb_ep->packet_sz)
			hw_ep->rx_double_buffered = true;
#endif
	}

	/* NOTE:  all the I/O code _should_ work fine without DMA, in case
	 * for some reason you run out of channels here.
	 */
	musb_ep->desc = desc;
	musb_ep->busy = 0;
	musb_ep->wedged = 0;
	status = 0;

	printf("%s periph: enabled %s for %s %s, %s %smaxpacket %d ep%x\n",
			musb_driver_name, musb_ep->end_point.name,
			({ char *s; switch (musb_ep->type) {
			case USB_ENDPOINT_XFER_BULK:	s = "bulk"; break;
			case USB_ENDPOINT_XFER_INT:	s = "int"; break;
			default:			s = "iso"; break;
			}; s; }),
			musb_ep->is_in ? "IN" : "OUT",
			musb->edma_mode ? "edma" : "pio",
			musb->polling_mode ? "polling, " : "interrupt, ",
			musb_ep->packet_sz, ep->address);

fail:
	spin_unlock_irqrestore(&musb->lock, flags);
	return status;
}

/*
 * Disable an endpoint flushing all requests queued.
 */
static int musb_gadget_disable(struct usb_ep *ep)
{
	unsigned long	flags;
	struct musb	*musb;
	u8		epnum;
	struct musb_ep	*musb_ep;
	void __iomem	*epio;
	int		status = 0;

	musb_ep = to_musb_ep(ep);
	musb = musb_ep->musb;
	epnum = musb_ep->current_epnum;
	epio = musb->endpoints[epnum].regs;

	spin_lock_irqsave(&musb->lock, flags);
	musb_ep_select(musb->mregs, epnum);

	/* zero the endpoint sizes */
	if (musb_ep->is_in) {
		u16 int_txe =  musb_readb(musb->mregs, MUSB_INTRTX1E);
		int_txe &= ~(1 << epnum);
		musb_writeb(musb->mregs, MUSB_INTRTX1E, int_txe);
		musb_writeb(epio, MUSB_TXMAXP, 0);
	} else {
		u16 int_rxe = musb_readb(musb->mregs, MUSB_INTRRX1E);
		int_rxe &= ~(1 << epnum);
		musb_writeb(musb->mregs, MUSB_INTRRX1E, int_rxe);
		musb_writeb(epio, MUSB_RXMAXP, 0);
	}

	musb_ep->desc = NULL;

	/* abort all pending DMA and requests */
	nuke(musb_ep, -ESHUTDOWN);

	spin_unlock_irqrestore(&(musb->lock), flags);

	dev_dbg(musb->controller, "%s\n", musb_ep->end_point.name);

	return status;
}

/*
 * Allocate a request for an endpoint.
 * Reused by ep0 code.
 */
struct usb_request *musb_alloc_request(struct usb_ep *ep, gfp_t gfp_flags)
{
	struct musb_ep		*musb_ep = to_musb_ep(ep);
	//struct musb		*musb = musb_ep->musb;
	struct musb_request	*request = NULL;

	request = calloc(sizeof *request, 1);
	if (!request) {
		dev_info(NULL, "not enough memory\n");
		return NULL;
	}

	request->request.dma = DMA_ADDR_INVALID;
	request->epnum = musb_ep->current_epnum;
	request->ep = musb_ep;
	dev_dbg(NULL, "musb_alloc_request req:%p\n", &request->request);

	return &request->request;
}

/*
 * Free a request
 * Reused by ep0 code.
 */
void musb_free_request(struct usb_ep *ep, struct usb_request *req)
{
	dev_dbg(NULL, "musb_free_request req:%p\n", req);
	kfree(to_musb_request(req));
}

static LIST_HEAD(buffers);

/*
 * Context: controller locked, IRQs blocked.
 */
void musb_ep_restart(struct musb *musb, struct musb_request *req)
{
	dev_dbg(musb->controller, "<== %s request %p len %u on hw_ep%d\n",
		req->tx ? "TX/IN" : "RX/OUT",
		&req->request, req->request.length, req->epnum);

	musb_ep_select(musb->mregs, req->epnum);
	if (req->tx)
		txstate(musb, req);
	else
		rxstate(musb, req);
}

static int musb_gadget_queue(struct usb_ep *ep, struct usb_request *req,
			gfp_t gfp_flags)
{
	struct musb_ep		*musb_ep;
	struct musb_request	*request;
	struct musb		*musb;
	int			status = 0;

	if (!ep || !req)
		return -EINVAL;
	if (!req->buf)
		return -ENODATA;

	musb_ep = to_musb_ep(ep);
	musb = musb_ep->musb;

	request = to_musb_request(req);
	request->musb = musb;

	if (request->ep != musb_ep)
		return -EINVAL;

	dev_dbg(musb->controller, "\n");
	dev_dbg(musb->controller, "queue: <== to %s request=%p\n", ep->name, req);

	/* request is mine now... */
	request->request.actual = 0;
	request->request.status = -EINPROGRESS;
	request->epnum = musb_ep->current_epnum;
	request->tx = musb_ep->is_in;

	/* don't queue if the ep is down */
	if (!musb_ep->desc) {
		dev_dbg(musb->controller, "req %p queued to %s while ep %s\n",
				req, ep->name, "disabled");
		status = -ESHUTDOWN;
		goto unlock;
	}

	/* add request to the list */
	list_add_tail(&request->list, &musb_ep->req_list);

	/* it this is the head of the queue, start i/o ... */
	if (!musb_ep->busy && &request->list == musb_ep->req_list.next)
		musb_ep_restart(musb, request);

unlock:
	return status;
}

static int musb_gadget_dequeue(struct usb_ep *ep, struct usb_request *request)
{
	struct musb_ep		*musb_ep = to_musb_ep(ep);
	struct musb_request	*req = to_musb_request(request);
	struct musb_request	*r;
	unsigned long		flags;
	int			status = 0;

	if (!ep || !request || to_musb_request(request)->ep != musb_ep)
		return -EINVAL;

	spin_lock_irqsave(&musb->lock, flags);

	list_for_each_entry(r, &musb_ep->req_list, list) {
		if (r == req)
			break;
	}
	if (r != req) {
		dev_dbg(musb->controller, "request %p not queued to %s\n", request, ep->name);
		status = -EINVAL;
		goto done;
	}

	/* if the hardware doesn't have the request, easy ... */
	if (musb_ep->req_list.next != &req->list || musb_ep->busy)
		musb_g_giveback(musb_ep, request, -ECONNRESET);

	/* ... else abort the dma transfer ... */
	else {
		/* NOTE: by sticking to easily tested hardware/driver states,
		 * we leave counting of in-flight packets imprecise.
		 */
		musb_g_giveback(musb_ep, request, -ECONNRESET);
	}

done:
	spin_unlock_irqrestore(&musb->lock, flags);
	return status;
}

/*
 * Set or clear the halt bit of an endpoint. A halted enpoint won't tx/rx any
 * data but will queue requests.
 *
 * exported to ep0 code
 */
static int musb_gadget_set_halt(struct usb_ep *ep, int value)
{
	struct musb_ep		*musb_ep = to_musb_ep(ep);
	int			status = 0;

	if (!ep)
		return -EINVAL;

	if ((USB_ENDPOINT_XFER_ISOC == musb_ep->type)) {
		status = -EINVAL;
	}

	return status;
}

static int musb_gadget_fifo_status(struct usb_ep *ep)
{
	struct musb_ep		*musb_ep = to_musb_ep(ep);
	void __iomem		*epio = musb_ep->hw_ep->regs;
	int			retval = -EINVAL;

	if (musb_ep->desc && !musb_ep->is_in) {
		struct musb		*musb = musb_ep->musb;
		int			epnum = musb_ep->current_epnum;
		void __iomem		*mbase = musb->mregs;
		unsigned long		flags;

		spin_lock_irqsave(&musb->lock, flags);

		musb_ep_select(mbase, epnum);
		/* FIXME return zero unless RXPKTRDY is set */
		retval = musb_read_rxcount(epio);

		spin_unlock_irqrestore(&musb->lock, flags);
	}
	return retval;
}

static void musb_gadget_fifo_flush(struct usb_ep *ep)
{
	struct musb_ep	*musb_ep = to_musb_ep(ep);
	struct musb	*musb = musb_ep->musb;
	u8		epnum = musb_ep->current_epnum;
	void __iomem	*epio = musb->endpoints[epnum].regs;
	void __iomem	*mbase;
	unsigned long	flags;
	u16		csr, int_txe;

	mbase = musb->mregs;

	spin_lock_irqsave(&musb->lock, flags);
	musb_ep_select(mbase, (u8) epnum);

	/* disable interrupts */
	int_txe = musb_readb(musb->mregs, MUSB_INTRTX1E);
	musb_writeb(musb->mregs, MUSB_INTRTX1E, (int_txe & ~(1 << epnum)));

	if (musb_ep->is_in) {
		csr = musb_read_txcsr(epio);
		if (csr & MUSB_TXCSR_FIFONOTEMPTY) {
			csr |= MUSB_TXCSR_FLUSHFIFO | MUSB_TXCSR_P_WZC_BITS;
			/*
			 * Setting both TXPKTRDY and FLUSHFIFO makes controller
			 * to interrupt current FIFO loading, but not flushing
			 * the already loaded ones.
			 */
			csr &= ~MUSB_TXCSR_TXPKTRDY;
			musb_write_txcsr(epio, csr);
			/* REVISIT may be inappropriate w/o FIFONOTEMPTY ... */
			musb_write_txcsr(epio, csr);
		}
	} else {
		csr = musb_read_rxcsr(epio);
		csr |= MUSB_RXCSR_FLUSHFIFO | MUSB_RXCSR_P_WZC_BITS;
		musb_write_rxcsr(epio, csr);
		musb_write_rxcsr(epio, csr);
	}

	/* re-enable interrupt */
	musb_writeb(mbase, MUSB_INTRTX1E, int_txe);
	spin_unlock_irqrestore(&musb->lock, flags);
}

static const struct usb_ep_ops musb_ep_ops = {
	.enable		= musb_gadget_enable,
	.disable	= musb_gadget_disable,
	.alloc_request	= musb_alloc_request,
	.free_request	= musb_free_request,
	.queue		= musb_gadget_queue,
	.dequeue	= musb_gadget_dequeue,
	.set_halt	= musb_gadget_set_halt,
	.fifo_status	= musb_gadget_fifo_status,
	.fifo_flush	= musb_gadget_fifo_flush
};

/* ----------------------------------------------------------------------- */

static int musb_gadget_get_frame(struct usb_gadget *gadget)
{
	struct musb	*musb = gadget_to_musb(gadget);

	return (int)((musb_readb(musb->mregs, MUSB_FRAME2) << 8) | musb_readb(musb->mregs, MUSB_FRAME1));
}

static void musb_pullup(struct musb *musb, int is_on)
{
	u8 devcrl;

	devcrl = musb_readb(musb->mregs, MUSB_DEVCTL);
	if (is_on)
		devcrl |= MUSB_DEVCTL_SESSION;
	else
		devcrl &= ~MUSB_DEVCTL_SESSION;

	/* FIXME if on, HdrcStart; if off, HdrcStop */

	info("gadget D+ pullup %s\n",
		is_on ? "on" : "off");
	musb_writeb(musb->mregs, MUSB_DEVCTL, devcrl);
	dev_info(musb->controller, "musb_pullup: devctl %02x power: %02x\n",
			musb_readb(musb->mregs, MUSB_DEVCTL), musb_readb(musb->mregs, MUSB_POWER));
}

static int musb_gadget_pullup(struct usb_gadget *gadget, int is_on)
{
	struct musb	*musb = gadget_to_musb(gadget);
	unsigned long	flags;

	is_on = !!is_on;


	/* NOTE: this assumes we are sensing vbus; we'd rather
	 * not pullup unless the B-session is active.
	 */
	spin_lock_irqsave(&musb->lock, flags);
	if (is_on != musb->softconnect) {
		musb->softconnect = is_on;
		musb_pullup(musb, is_on);
	}
	spin_unlock_irqrestore(&musb->lock, flags);

	return 0;
}

static const struct usb_gadget_ops musb_gadget_operations = {
	.get_frame		= musb_gadget_get_frame,
	.pullup			= musb_gadget_pullup,
};

/* ----------------------------------------------------------------------- */

/* Registration */

/* Only this registration code "knows" the rule (from USB standards)
 * about there being only one external upstream port.  It assumes
 * all peripheral ports are external...
 */

static void __devinit
init_peripheral_ep(struct musb *musb, struct musb_ep *ep, u8 epnum, int is_in)
{
	struct musb_hw_ep	*hw_ep = musb->endpoints + epnum;

	memset(ep, 0, sizeof *ep);

	ep->current_epnum = epnum;
	ep->musb = musb;
	ep->hw_ep = hw_ep;
	ep->is_in = is_in;

	INIT_LIST_HEAD(&ep->req_list);

	sprintf(ep->name, "ep%d%s", epnum,
			(!epnum || hw_ep->is_shared_fifo) ? "" : (
				is_in ? "in" : "out"));
	ep->end_point.name = ep->name;
	INIT_LIST_HEAD(&ep->end_point.ep_list);
	if (!epnum) {
		ep->end_point.maxpacket = 64;
		ep->end_point.ops = &musb_g_ep0_ops;
		musb->g.ep0 = &ep->end_point;
	} else {
		if (is_in)
			ep->end_point.maxpacket = hw_ep->max_packet_sz_tx;
		else
			ep->end_point.maxpacket = hw_ep->max_packet_sz_rx;
		ep->end_point.ops = &musb_ep_ops;
		list_add_tail(&ep->end_point.ep_list, &musb->g.ep_list);
	}
}

/*
 * Initialize the endpoints exposed to peripheral drivers, with backlinks
 * to the rest of the driver state.
 */
static inline void __devinit musb_g_init_endpoints(struct musb *musb)
{
	u8			epnum;
	struct musb_hw_ep	*hw_ep;
	unsigned		count = 0;

	/* initialize endpoint list just once */
	INIT_LIST_HEAD(&(musb->g.ep_list));

	for (epnum = 0, hw_ep = musb->endpoints;
			epnum < musb->nr_endpoints;
			epnum++, hw_ep++) {
		if (hw_ep->is_shared_fifo /* || !epnum */) {
			init_peripheral_ep(musb, &hw_ep->ep_in, epnum, 0);
			count++;
		} else {
			if (hw_ep->max_packet_sz_tx) {
				init_peripheral_ep(musb, &hw_ep->ep_in,
							epnum, 1);
				count++;
			}
			if (hw_ep->max_packet_sz_rx) {
				init_peripheral_ep(musb, &hw_ep->ep_out,
							epnum, 0);
				count++;
			}
		}
	}
}

/* called once during driver setup to initialize and link into
 * the driver model; memory is zeroed.
 */
int __devinit musb_gadget_setup(struct musb *musb)
{
	/* REVISIT minor race:  if (erroneously) setting up two
	 * musb peripherals at the same time, only the bus lock
	 * is probably held.
	 */

	musb->g.ops = &musb_gadget_operations;
	musb->g.speed = USB_SPEED_UNKNOWN;
	musb->g.name = musb_driver_name;

	musb_g_init_endpoints(musb);

	musb->is_active = 0;

	return 0;
}

void musb_gadget_cleanup(struct musb *musb)
{
}

/*
 * Register the gadget driver. Used by gadget drivers when
 * registering themselves with the controller.
 *
 * -EINVAL something went wrong (not driver)
 * -EBUSY another gadget is already using the controller
 * -ENOMEM no memory to perform the operation
 *
 * @param driver the gadget driver
 * @return <0 if error, 0 if everything is fine
 */

int musb_gadget_start(struct usb_gadget *g,
		struct usb_gadget_driver *driver)
{
	struct musb		*musb = gadget_to_musb(g);
	unsigned long		flags;

	musb->softconnect = 0;
	musb->gadget_driver = driver;

	spin_lock_irqsave(&musb->lock, flags);
	musb->is_active = 1;

		musb_start(musb);

	spin_unlock_irqrestore(&musb->lock, flags);

	return 0;
}

static void stop_activity(struct musb *musb, struct usb_gadget_driver *driver)
{
	int			i;
	struct musb_hw_ep	*hw_ep;

	/* don't disconnect if it's not connected */
	if (musb->g.speed == USB_SPEED_UNKNOWN)
		driver = NULL;
	else
		musb->g.speed = USB_SPEED_UNKNOWN;

#if 0
	/* deactivate the hardware */
	if (musb->softconnect) {
		musb->softconnect = 0;
		musb_pullup(musb, 0);
	}
	musb_stop(musb);
#endif
	/* killing any outstanding requests will quiesce the driver;
	 * then report disconnect
	 */
	if (driver) {
		for (i = 0, hw_ep = musb->endpoints;
				i < musb->nr_endpoints;
				i++, hw_ep++) {
			musb_ep_select(musb->mregs, i);
			if (hw_ep->is_shared_fifo /* || !epnum */) {
				nuke(&hw_ep->ep_in, -ECONNRESET);
			} else {
				if (hw_ep->max_packet_sz_tx)
					nuke(&hw_ep->ep_in, -ECONNRESET);
				if (hw_ep->max_packet_sz_rx)
					nuke(&hw_ep->ep_out, -ECONNRESET);
			}
		}
	}
}


/* ----------------------------------------------------------------------- */

/* called when VBUS drops below session threshold, and in other cases */
void musb_g_disconnect(struct musb *musb)
{
	void __iomem	*mregs = musb->mregs;
	u8	devctl = musb_readb(mregs, MUSB_DEVCTL);

	info("musb_g_disconnect: devctl %02x\n", devctl);
	stop_activity(musb, musb->gadget_driver);

	/* clear HR */
	musb_writeb(mregs, MUSB_DEVCTL, devctl & MUSB_DEVCTL_SESSION);

	musb->g.speed = USB_SPEED_UNKNOWN;
	if (musb->gadget_driver && musb->gadget_driver->disconnect) {
		spin_unlock(&musb->lock);
		musb->gadget_driver->disconnect(&musb->g);
		spin_lock(&musb->lock);
	}

	musb->is_active = 0;
}

void musb_g_reset(struct musb *musb)
{
	dev_info(NULL, "musb_g_reset start\n");

	musb->g.speed = USB_SPEED_FULL;

	/* start in USB_STATE_DEFAULT */
	musb->is_active = 1;
	MUSB_DEV_MODE(musb);
	musb->address = 0;
	musb->ep0_state = MUSB_EP0_STAGE_SETUP;

	musb->g.b_hnp_enable = 0;
	musb->g.a_alt_hnp_support = 0;
	musb->g.a_hnp_support = 0;
}
