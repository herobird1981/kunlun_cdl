/*
 * MUSB OTG driver core code
 *
 * Copyright 2005 Mentor Graphics Corporation
 * Copyright (C) 2005-2006 by Texas Instruments
 * Copyright (C) 2006-2007 Nokia Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

/*
 * Inventra (Multipoint) Dual-Role Controller Driver for Linux.
 *
 * This consists of a Host Controller Driver (HCD) and a peripheral
 * controller driver implementing the "Gadget" API; OTG support is
 * in the works.  These are normal Linux-USB controller drivers which
 * use IRQs and have no dedicated thread.
 *
 * This version of the driver has only been used with products from
 * Texas Instruments.  Those products integrate the Inventra logic
 * with other DMA, IRQ, and bus modules, as well as other logic that
 * needs to be reflected in this driver.
 *
 *
 * NOTE:  the original Mentor code here was pretty much a collection
 * of mechanisms that don't seem to have been fully integrated/working
 * for *any* Linux kernel version.  This version aims at Linux 2.6.now,
 * Key open issues include:
 *
 *  - Lack of host-side transaction scheduling, for all transfer types.
 *    The hardware doesn't do it; instead, software must.
 *
 *    This is not an issue for OTG devices that don't support external
 *    hubs, but for more "normal" USB hosts it's a user issue that the
 *    "multipoint" support doesn't scale in the expected ways.  That
 *    includes DaVinci EVM in a common non-OTG mode.
 *
 *      * Control and bulk use dedicated endpoints, and there's as
 *        yet no mechanism to either (a) reclaim the hardware when
 *        peripherals are NAKing, which gets complicated with bulk
 *        endpoints, or (b) use more than a single bulk endpoint in
 *        each direction.
 *
 *        RESULT:  one device may be perceived as blocking another one.
 *
 *      * Interrupt and isochronous will dynamically allocate endpoint
 *        hardware, but (a) there's no record keeping for bandwidth;
 *        (b) in the common case that few endpoints are available, there
 *        is no mechanism to reuse endpoints to talk to multiple devices.
 *
 *        RESULT:  At one extreme, bandwidth can be overcommitted in
 *        some hardware configurations, no faults will be reported.
 *        At the other extreme, the bandwidth capabilities which do
 *        exist tend to be severely undercommitted.  You can't yet hook
 *        up both a keyboard and a mouse to an external USB hub.
 */

/*
 * This gets many kinds of configuration information:
 *	- Kconfig for everything user-configurable
 *	- platform_device for addressing, irq, and platform_data
 *	- platform_data is mostly for board-specific informarion
 *	  (plus recentrly, SOC or family details)
 *
 * Most of the conditional compilation will (someday) vanish.
 */

#include <common.h>
#include <usb.h>
#include <asm/errno.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <asm/io.h>
#include "linux-compat.h"
#include "usb-compat.h"
#include <asm/bitops.h>

#include "musb_core.h"
#include "interrupt.h"
#include "cdl_irq.h"
#include "memleak.h"


#define DRIVER_AUTHOR "Mentor Graphics, Texas Instruments, Nokia"
#define DRIVER_DESC "Inventra Dual-Role USB Controller Driver"

#define MUSB_VERSION "6.0"

#define DRIVER_INFO DRIVER_DESC ", v" MUSB_VERSION

#define MUSB_DRIVER_NAME "musb-hdrc"
const char musb_driver_name[] = MUSB_DRIVER_NAME;


MODULE_DESCRIPTION(DRIVER_INFO);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" MUSB_DRIVER_NAME);


/*-------------------------------------------------------------------------*/


/*
 * Load an endpoint's FIFO
 */
void musb_write_fifo(struct musb_hw_ep *hw_ep, u16 len, const u8 *src)
{
	void __iomem *fifo = hw_ep->fifo;

	if (unlikely(len == 0))
		return;

	prefetch((u8 *)src);

	dev_dbg(NULL, "%cX ep%d fifo %p count %d buf %p\n",
			'T', hw_ep->epnum, fifo, len, src);
#if 0
	print_hex("WF", 1, src, (len > 16 ? 16 : len));
#endif

	/* byte aligned */
	writesb(fifo, src, len);
}


/*
 * Unload an endpoint's FIFO
 */
void musb_read_fifo(struct musb_hw_ep *hw_ep, u16 len, u8 *dst)
{
	void __iomem *fifo = hw_ep->fifo;

	if (unlikely(len == 0)) {
		dev_info(NULL, "read_fifo len == 0\n");
		return;
	}

	dev_dbg(NULL, "%cX ep%d fifo %p count %d buf %p\n",
			'R', hw_ep->epnum, fifo, len, dst);

	/* byte aligned */
	readsb(fifo, dst, len);
#if 0
	if (hw_ep->epnum)
		print_hex("RF", 1, dst, (len > 128 ? 128 : len));
#endif
}


/*-------------------------------------------------------------------------*/

/*
 * Interrupt Service Routine to record USB "global" interrupts.
 * Since these do not happen often and signify things of
 * paramount importance, it seems OK to check them individually;
 * the order of the tests is specified in the manual
 *
 * @param musb instance pointer
 * @param int_usb register contents
 * @param devctl
 * @param power
 */

static irqreturn_t musb_stage0_irq(struct musb *musb, u8 int_usb,
				u8 devctl, u8 power)
{
	irqreturn_t handled = IRQ_NONE;

	dev_dbg(musb->controller, "<== Power=%02x, DevCtl=%02x, int_usb=0x%x\n", power, devctl,
		int_usb);

	/* in host mode, the peripheral may issue remote wakeup.
	 * in peripheral mode, the host may resume the link.
	 * spurious RESUME irqs happen too, paired with SUSPEND.
	 */
	if (int_usb & MUSB_INTR_RESUME) {
		handled = IRQ_HANDLED;
		info("resume interrupt: Power=%02x, DevCtl=%02x, int_usb=0x%x\n", power, devctl, int_usb);
	}

	/* see manual for the order of the tests */
	if (int_usb & MUSB_INTR_SESSREQ) {
		void __iomem *mbase = musb->mregs;
		info("sessreq interrupt: Power=%02x, DevCtl=%02x, int_usb=0x%x\n", power, devctl, int_usb);

		if ((power & MUSB_POWER_VBUSVAL) == MUSB_POWER_VBUSVAL
				&& (devctl & MUSB_DEVCTL_CID)) {
			info("SessReq while on B state\n");
			return IRQ_HANDLED;
		}

		/* IRQ arrives from ID pin sense or (later, if VBUS power
		 * is removed) SRP.  responses are time critical:
		 *  - turn on VBUS (with silicon-specific mechanism)
		 *  - go through A_WAIT_VRISE
		 *  - ... to A_WAIT_BCON.
		 * a_wait_vrise_tmout triggers VBUS_ERROR transitions
		 */
		musb_writeb(mbase, MUSB_DEVCTL, MUSB_DEVCTL_SESSION);
		MUSB_HST_MODE(musb);
		musb_platform_set_vbus(musb, 1);

		handled = IRQ_HANDLED;
	}

	if (int_usb & MUSB_INTR_VBUSERROR) {
		info("vbuserror interrupt: Power=%02x, DevCtl=%02x, int_usb=0x%x\n", power, devctl, int_usb);
		handled = IRQ_HANDLED;
	}

	if (int_usb & MUSB_INTR_SUSPEND) {
		info("suspend interrupt: Power=%02x, DevCtl=%02x, int_usb=0x%x\n", power, devctl, int_usb);
		handled = IRQ_HANDLED;
	}

	if (int_usb & MUSB_INTR_CONNECT) {
		//struct usb_hcd *hcd = musb_to_hcd(musb);
		info("connect interrupt: Power=%02x, DevCtl=%02x, int_usb=0x%x\n", power, devctl, int_usb);
		handled = IRQ_HANDLED;
	}

	if ((int_usb & MUSB_INTR_DISCONNECT) && !musb->ignore_disconnect) {
		info("disconnect interrupt: (%s) Power=%02x, devctl %02x\n",
				MUSB_MODE(musb), power, devctl);
		handled = IRQ_HANDLED;
		musb->is_disconnect = 1;
		if(!musb->is_host)
			musb_g_disconnect(musb);
	}

	/* mentor saves a bit: bus reset and babble share the same irq.
	 * only host sees babble; only peripheral sees bus reset.
	 */
	if (int_usb & MUSB_INTR_RESET) {
		handled = IRQ_HANDLED;

		info("reset interrupt: Power=%02x, DevCtl=%02x, int_usb=0x%x\n", power, devctl, int_usb);
		if (is_peripheral_capable()) {
			musb_g_reset(musb);
		}
	}
	return handled;
}

/*-------------------------------------------------------------------------*/

/*
* Program the HDRC to start (enable interrupts, dma, etc.).
*/
int musb_start(struct musb *musb)
{
	void __iomem	*regs = musb->mregs;
	u8		devctl = musb_readb(regs, MUSB_DEVCTL);

	int ret;


	dev_dbg(musb->controller, "<== devctl %02x\n", devctl);

	/*  Set INT enable registers, enable interrupts */
	musb_writeb(regs, MUSB_INTRTX1E, musb->epmask & 0xff);
	musb_writeb(regs, MUSB_INTRRX1E, musb->epmask & 0xfe);
	musb_writeb(regs, MUSB_INTRUSBE, 0xf7);

	//musb_writeb(regs, MUSB_TESTMODE, 0);

	/* put into basic highspeed mode and start session */
	musb_writeb(regs, MUSB_POWER, MUSB_POWER_ISOUPDATE
						 /*| MUSB_POWER_HSENAB*/
						/* | ENSUSPEND wedges tusb */
						/* | MUSB_POWER_ENSUSPEND */
						);

	musb->is_active = 0;
	devctl = musb_readb(regs, MUSB_DEVCTL);
	devctl &= ~MUSB_DEVCTL_SESSION;

	if (is_otg_enabled(musb)) {
		/* session started after:
		 * (a) ID-grounded irq, host mode;
		 * (b) vbus present/connect IRQ, peripheral mode;
		 * (c) peripheral initiates, using SRP
		 */
		if ((musb_readb(regs, MUSB_POWER) & MUSB_POWER_VBUSVAL) == MUSB_POWER_VBUSVAL)
			musb->is_active = 1;
		else
			devctl |= MUSB_DEVCTL_SESSION;

	} else if (is_host_enabled(musb)) {
		/* assume ID pin is hard-wired to ground */
		devctl |= MUSB_DEVCTL_SESSION;

	} else /* peripheral is enabled */ {
		if ((musb_readb(regs, MUSB_POWER) & MUSB_POWER_VBUSVAL) == MUSB_POWER_VBUSVAL)
			musb->is_active = 1;
		//devctl |= MUSB_DEVCTL_SESSION;
	}


	ret = musb_platform_enable(musb);
	if (ret) {
		musb->is_active = 0;
		return ret;
	}

	musb_writeb(regs, MUSB_DEVCTL, devctl);

	dev_info(musb->controller, "musb_start: %s devctl %02x power: %02x\n",
			musb->g.name, musb_readb(regs, MUSB_DEVCTL), musb_readb(regs, MUSB_POWER));

	return 0;

}


static void musb_generic_disable(struct musb *musb)
{
	void __iomem	*mbase = musb->mregs;

	/* disable interrupts */
	musb_writeb(mbase, MUSB_INTRUSBE, 0);
	musb_writeb(mbase, MUSB_INTRTX1E, 0);
	musb_writeb(mbase, MUSB_INTRTX2E, 0);
	musb_writeb(mbase, MUSB_INTRRX1E, 0);
	musb_writeb(mbase, MUSB_INTRRX2E, 0);

	/* off */
	musb_writeb(mbase, MUSB_DEVCTL, 0);

	/*  flush pending interrupts */
	musb_readb(mbase, MUSB_INTRUSB);
	musb_readb(musb->mregs, MUSB_INTRTX1);
	musb_readb(musb->mregs, MUSB_INTRTX2);
	musb_readb(musb->mregs, MUSB_INTRRX1);
	musb_readb(musb->mregs, MUSB_INTRRX2);
}

/*
 * Make the HDRC stop (disable interrupts, etc.);
 * reversible by musb_start
 * called on gadget driver unregister
 * with controller locked, irqs blocked
 * acts as a NOP unless some role activated the hardware
 */
void musb_stop(struct musb *musb)
{
	/* stop IRQs, timers, ... */
	musb_platform_disable(musb);
	musb_generic_disable(musb);
	info("HDRC disabled\n");
}


/*-------------------------------------------------------------------------*/

/*
 * The silicon either has hard-wired endpoint configurations, or else
 * "dynamic fifo" sizing.  The driver has support for both, though at this
 * writing only the dynamic sizing is very well tested.   Since we switched
 * away from compile-time hardware parameters, we can no longer rely on
 * dead code elimination to leave only the relevant one in the object file.
 *
 * We don't currently use dynamic fifo setup capability to do anything
 * more than selecting one of a bunch of predefined configurations.
 */
static ushort __devinitdata fifo_mode = 2;


/*
 * tables defining fifo_mode values.  define more if you like.
 * for host side, make sure both halves of ep1 are set up.
 */

/* mode 0 - fits in 2KB */
static struct musb_fifo_cfg __devinitdata mode_0_cfg[] = {
{ .hw_ep_num = 1, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num = 1, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num = 2, .style = FIFO_RXTX, .maxpacket = 512, },
{ .hw_ep_num = 3, .style = FIFO_RXTX, .maxpacket = 256, },
{ .hw_ep_num = 4, .style = FIFO_RXTX, .maxpacket = 256, },
};

/* mode 1 - fits in 4KB */
static struct musb_fifo_cfg __devinitdata mode_1_cfg[] = {
{ .hw_ep_num = 1, .style = FIFO_TX,   .maxpacket = 512, .mode = BUF_DOUBLE, },
{ .hw_ep_num = 1, .style = FIFO_RX,   .maxpacket = 512, .mode = BUF_DOUBLE, },
{ .hw_ep_num = 2, .style = FIFO_RXTX, .maxpacket = 512, .mode = BUF_DOUBLE, },
{ .hw_ep_num = 3, .style = FIFO_RXTX, .maxpacket = 256, },
{ .hw_ep_num = 4, .style = FIFO_RXTX, .maxpacket = 256, },
};

/* mode 2 - fits in 4KB */
static struct musb_fifo_cfg __devinitdata mode_2_cfg[] = {
{ .hw_ep_num = 1, .style = FIFO_RXTX,   .maxpacket = 256, },
{ .hw_ep_num = 2, .style = FIFO_RXTX,   .maxpacket = 256, },
#if 0
{ .hw_ep_num = 3, .style = FIFO_RXTX, .maxpacket = 1024, },
{ .hw_ep_num = 4, .style = FIFO_RXTX, .maxpacket = 1024, },
#endif
};


/*
 * configure a fifo; for non-shared endpoints, this may be called
 * once for a tx fifo and once for an rx fifo.
 *
 * returns negative errno or offset for next fifo.
 */
static int __devinit
fifo_setup(struct musb *musb, struct musb_hw_ep  *hw_ep,
		const struct musb_fifo_cfg *cfg, u16 offset)
{
	void __iomem	*mbase = musb->mregs;
	int	size = 0;
	u16	maxpacket = cfg->maxpacket;
	u16	c_off = offset >> 3;
	u8	c_size;

	/* expect hw_ep has already been zero-initialized */

	size = ffs(max(maxpacket, (u16) 8)) - 1;
	maxpacket = 1 << size;

	c_size = size - 3;
	c_size <<= 5;
	if (cfg->mode == BUF_DOUBLE) {
		if ((offset + (maxpacket << 1)) >
				(1 << (musb->config->ram_bits + 2)))
			return -EMSGSIZE;
		c_size |= MUSB_FIFOSZ_DPB;
	} else {
		if ((offset + maxpacket) > (1 << (musb->config->ram_bits + 2)))
			return -EMSGSIZE;
	}

	/* configure the FIFO */
	musb_writeb(mbase, MUSB_INDEX, hw_ep->epnum);

	/* EP0 reserved endpoint for control, bidirectional;
	 * EP1 reserved for bulk, two unidirection halves.
	 */
	if (hw_ep->epnum == 1)
		musb->bulk_ep = hw_ep;
	/* REVISIT error check:  be sure ep0 can both rx and tx ... */
	switch (cfg->style) {
	case FIFO_TX:
		//musb_write_txfifosz(mbase, c_size);
		musb_write_txfifoadd(mbase, c_size, c_off);
		hw_ep->tx_double_buffered = !!(c_size & MUSB_FIFOSZ_DPB);
		hw_ep->max_packet_sz_tx = maxpacket;
		break;
	case FIFO_RX:
		//musb_write_rxfifosz(mbase, c_size);
		musb_write_rxfifoadd(mbase, c_size, c_off);
		hw_ep->rx_double_buffered = !!(c_size & MUSB_FIFOSZ_DPB);
		hw_ep->max_packet_sz_rx = maxpacket;
		break;
	case FIFO_RXTX:
		//musb_write_txfifosz(mbase, c_size);
		musb_write_txfifoadd(mbase, c_size, c_off);
		hw_ep->rx_double_buffered = !!(c_size & MUSB_FIFOSZ_DPB);
		hw_ep->max_packet_sz_rx = maxpacket;

		//musb_write_rxfifosz(mbase, c_size);
		musb_write_rxfifoadd(mbase, c_size, c_off);
		hw_ep->tx_double_buffered = hw_ep->rx_double_buffered;
		hw_ep->max_packet_sz_tx = maxpacket;

		hw_ep->is_shared_fifo = true;
		break;
	}

	/* NOTE rx and tx endpoint irqs aren't managed separately,
	 * which happens to be ok
	 */
	musb->epmask |= (1 << hw_ep->epnum);

	return offset + (maxpacket << ((c_size & MUSB_FIFOSZ_DPB) ? 1 : 0));
}

static struct musb_fifo_cfg __devinitdata ep0_cfg = {
	.style = FIFO_RXTX, .maxpacket = 64,
};

static int __devinit ep_config_from_table(struct musb *musb)
{
	const struct musb_fifo_cfg	*cfg;
	unsigned		i, n;
	int			offset;
	struct musb_hw_ep	*hw_ep = musb->endpoints;

	if (musb->config->fifo_cfg) {
		cfg = musb->config->fifo_cfg;
		n = musb->config->fifo_cfg_size;
		goto done;
	}

	switch (fifo_mode) {
	default:
		fifo_mode = 0;
		/* FALLTHROUGH */
	case 0:
		cfg = mode_0_cfg;
		n = ARRAY_SIZE(mode_0_cfg);
		break;
	case 1:
		cfg = mode_1_cfg;
		n = ARRAY_SIZE(mode_1_cfg);
		break;
	case 2:
		cfg = mode_2_cfg;
		n = ARRAY_SIZE(mode_2_cfg);
		break;
	}

	dev_dbg(NULL, "%s: setup fifo_mode %d\n", musb_driver_name, fifo_mode);

done:
	offset = fifo_setup(musb, hw_ep, &ep0_cfg, 0);
	/* assert(offset > 0) */

	/* NOTE:  for RTL versions >= 1.400 EPINFO and RAMINFO would
	 * be better than static musb->config->num_eps and DYN_FIFO_SIZE...
	 */

	for (i = 0; i < n; i++) {
		u8	epn = cfg->hw_ep_num;

		if (epn >= musb->config->num_eps) {
			dev_info(NULL, "%s: invalid ep %d\n",
					musb_driver_name, epn);
			return -EINVAL;
		}
		offset = fifo_setup(musb, hw_ep + epn, cfg++, offset);
		if (offset < 0) {
			dev_info(NULL, "%s: mem overrun, ep %d\n",
					musb_driver_name, epn);
			return -EINVAL;
		}
		epn++;
		musb->nr_endpoints = max(epn, musb->nr_endpoints);
	}

	dev_dbg(NULL, "%s: %d/%d max ep, %d/%d memory\n", musb_driver_name, n + 1,
		 musb->config->num_eps * 2 - 1, offset,
		 (1 << (musb->config->ram_bits + 2)));

	if (!musb->bulk_ep) {
		dev_info(NULL, "%s: missing bulk\n", musb_driver_name);
		return -EINVAL;
	}

	return 0;
}


/*
 * ep_config_from_hw - when MUSB_C_DYNFIFO_DEF is false
 * @param musb the controller
 */
static int __devinit ep_config_from_hw(struct musb *musb)
{
	u8 epnum = 0;
	struct musb_hw_ep *hw_ep;
	void *mbase = musb->mregs;
	int ret = 0;

	dev_dbg(musb->controller, "<== static silicon ep config devctl:0x%02x, MUSB_INDEX:0x%02x\n", musb_readb(mbase, MUSB_DEVCTL), musb_readb(mbase, MUSB_INDEX));

	/* FIXME pick up ep0 maxpacket size */

	for (epnum = 1; epnum < musb->config->num_eps; epnum++) {
		musb_ep_select(mbase, epnum);
		hw_ep = musb->endpoints + epnum;

		ret = musb_read_fifosize(musb, hw_ep, epnum);
		if (ret < 0)
			break;

		/* FIXME set up hw_ep->{rx,tx}_double_buffered */

		/* pick an RX/TX endpoint for bulk */
		if (hw_ep->max_packet_sz_tx < 512
				|| hw_ep->max_packet_sz_rx < 512)
			continue;

		/* REVISIT:  this algorithm is lazy, we should at least
		 * try to pick a double buffered endpoint.
		 */
		if (musb->bulk_ep)
			continue;
		musb->bulk_ep = hw_ep;
	}

	if (!musb->bulk_ep) {
		pr_debug("%s: missing bulk\n", musb_driver_name);
		return -EINVAL;
	}

	return 0;
}

enum { MUSB_CONTROLLER_MFDRC, MUSB_CONTROLLER_FDRC, };

/* Initialize MUSB (M)HDRC part of the USB hardware subsystem;
 * configure endpoints, or take their config from silicon
 */
static int __devinit musb_core_init(u16 musb_type, struct musb *musb)
{
	u8 reg;
	char *type;
	char aInfo[90], aDate[12];
	void __iomem	*mbase = musb->mregs;
	int		status = 0;
	int		i;

	/* log core options (read using indexed model) */
	reg = musb_read_configdata(mbase);

	strcpy(aInfo, (reg & MUSB_CONFIGDATA_UTMIDW) ? "UTMI-16" : "UTMI-8");
	if (reg & MUSB_CONFIGDATA_DYNFIFO) {
		strcat(aInfo, ", dyn FIFOs");
		musb->dyn_fifo = true;
	}
	if (reg & MUSB_CONFIGDATA_SOFTCONE)
		strcat(aInfo, ", SoftConn");

	dev_info(NULL, "%s:ConfigData=0x%02x (%s)\n", musb_driver_name, reg, aInfo);

	aDate[0] = 0;
	if (MUSB_CONTROLLER_MFDRC == musb_type) {
		musb->is_multipoint = 1;
		type = "M";
	} else {
		musb->is_multipoint = 0;
		type = "";
	}

	/* log release info */
	//pr_debug("%s: %sHDRC %s\n", musb_driver_name, type, aDate);

	/* configure ep0 */
	musb_configure_ep0(musb);

	/* discover endpoint configuration */
	musb->nr_endpoints = 1;
	musb->epmask = 1;

	if (musb->dyn_fifo)
		status = ep_config_from_table(musb);
	else
		status = ep_config_from_hw(musb);

	if (status < 0)
		return status;

	/* finish init, and print endpoint config */
	for (i = 0; i < musb->nr_endpoints; i++) {
		struct musb_hw_ep	*hw_ep = musb->endpoints + i;

		hw_ep->fifo = MUSB_FIFO_OFFSET(i) + mbase;
		hw_ep->regs = MUSB_EP_OFFSET(i, 0) + mbase;
		hw_ep->rx_reinit = 1;
		hw_ep->tx_reinit = 1;

		if (hw_ep->max_packet_sz_tx) {
			dev_info(musb->controller,
				"%s: hw_ep %d%s, %smax %d\n",
				musb_driver_name, i,
				hw_ep->is_shared_fifo ? "shared" : "tx",
				hw_ep->tx_double_buffered
					? "doublebuffer, " : "",
				hw_ep->max_packet_sz_tx);
		}
		if (hw_ep->max_packet_sz_rx && !hw_ep->is_shared_fifo) {
			dev_info(musb->controller,
				"%s: hw_ep %d%s, %smax %d\n",
				musb_driver_name, i,
				"rx",
				hw_ep->rx_double_buffered
					? "doublebuffer, " : "",
				hw_ep->max_packet_sz_rx);
		}
		if (!(hw_ep->max_packet_sz_tx || hw_ep->max_packet_sz_rx))
			dev_info(musb->controller, "hw_ep %d not configured\n", i);
	}

	return 0;
}

/*-------------------------------------------------------------------------*/
void usb_interrupt_handler(void *ptr)
{
	int	retval = IRQ_NONE;
	struct musb	*musb = ptr;

	if (!musb || !musb->isr) {
		info("usb interrupt err: musb or musb->irq null\n");
		return;
	}

	retval = musb->isr(musb->nIrq, musb);
	if (retval == IRQ_NONE) {
		dev_info(musb->controller, "usb interrupt not handled\n");
	}
	return;
}

static irqreturn_t generic_interrupt(int irq, void *__hci)
{
    irqreturn_t	retval = IRQ_NONE;
	struct musb	*musb = __hci;

	musb->int_usb = musb_readb(musb->mregs, MUSB_INTRUSB);
	musb->int_tx = musb_readb(musb->mregs, MUSB_INTRTX1);
	musb->int_rx = musb_readb(musb->mregs, MUSB_INTRRX1);

	musb->int_usb &= musb_readb(musb->mregs, MUSB_INTRUSBE);

	if (musb->int_usb || musb->int_tx || musb->int_rx)
		retval = musb_interrupt(musb);

	return retval;
}

/*
 * handle all the irqs defined by the HDRC core. for now we expect:  other
 * irq sources (phy, dma, etc) will be handled first, musb->int_* values
 * will be assigned, and the irq will already have been acked.
 *
 * called in irq context with spinlock held, irqs blocked
 */
irqreturn_t musb_interrupt(struct musb *musb)
{
	irqreturn_t	retval = IRQ_NONE;
	u8		devctl, power;
	int		ep_num;
	u32		reg;

	devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
	power = musb_readb(musb->mregs, MUSB_POWER);

	dev_dbg(musb->controller, "** IRQ %s usb%04x tx%04x rx%04x\n",
		(devctl & MUSB_DEVCTL_HM) ? "host" : "peripheral",
		musb->int_usb, musb->int_tx, musb->int_rx);

	/* the core can interrupt us for multiple reasons; docs have
	 * a generic interrupt flowchart to follow
	 */
	if (musb->int_usb)
		retval |= musb_stage0_irq(musb, musb->int_usb,
				devctl, power);

	/* "stage 1" is handling endpoint irqs */

	/* handle endpoint 0 first */
	if (musb->int_tx & 1) {
		retval |= musb_g_ep0_irq(musb);
	}

	/* RX on endpoints 1-15 */
	reg = musb->int_rx >> 1;
	ep_num = 1;
	while (reg) {
		if (reg & 1) {
			/* musb_ep_select(musb->mregs, ep_num); */
			/* REVISIT just retval = ep->rx_irq(...) */
			retval = IRQ_HANDLED;
			musb_g_rx(musb, ep_num);
		}

		reg >>= 1;
		ep_num++;
	}

	/* TX on endpoints 1-15 */
	reg = musb->int_tx >> 1;
	ep_num = 1;
	while (reg) {
		if (reg & 1) {
			/* musb_ep_select(musb->mregs, ep_num); */
			/* REVISIT just retval |= ep->tx_irq(...) */
			retval = IRQ_HANDLED;
			musb_g_tx(musb, ep_num);
		}
		reg >>= 1;
		ep_num++;
	}

	return retval;
}

/*-------------------------------------------------------------------------*/


/* --------------------------------------------------------------------------
 * Init support
 */

static struct musb *__devinit
allocate_instance(struct device *dev,
		struct musb_hdrc_config *config, void __iomem *mbase)
{
	struct musb		*musb;
	struct musb_hw_ep	*ep;
	int			epnum;

	musb = calloc(1, sizeof(*musb));
	if (!musb)
		return NULL;

	INIT_LIST_HEAD(&musb->control);
	INIT_LIST_HEAD(&musb->in_bulk);
	INIT_LIST_HEAD(&musb->out_bulk);


	musb->vbuserr_retry = VBUSERR_RETRY_COUNT;
	dev_set_drvdata(dev, musb);
	musb->mregs = mbase;
	musb->nIrq = -ENODEV;
	musb->config = config;
	BUG_ON(musb->config->num_eps > MUSB_C_NUM_EPS);
	for (epnum = 0, ep = musb->endpoints;
			epnum < musb->config->num_eps;
			epnum++, ep++) {
		ep->musb = musb;
		ep->epnum = epnum;
	}

	musb->controller = dev;

	return musb;
}

void musb_free(struct musb *musb)
{
	/* this has multiple entry modes. it handles fault cleanup after
	 * probe(), where things may be partially set up, as well as rmmod
	 * cleanup after everything's been de-activated.
	 */

	if (musb->nIrq >= 0) {
		if (!musb->polling_mode) {
			/*disable interrupt*/
			irq_generic_disable_int(musb->nIrq);
			irq_generic_remove_handler(musb->nIrq,NULL,NULL);
		}
	}

	kfree(musb);
}

/*
 * Perform generic per-controller initialization.
 *
 * @pDevice: the controller (already clocked, etc)
 * @nIrq: irq
 * @mregs: virtual address of controller registers,
 *	not yet corrected for platform-specific offsets
 */
struct musb *
musb_init_controller(struct musb_hdrc_platform_data *plat, struct device *dev,
			     void *ctrl)
{
	int			status;
	struct musb		*musb;
	int nIrq = 0;


	/* The driver might handle more features than the board; OK.
	 * Fail when the board needs a feature that's not enabled.
	 */
	if (!plat) {
		dev_info(dev, "no platform_data?\n");
		status = -ENODEV;
		goto fail0;
	}

	/* allocate */
	musb = allocate_instance(dev, plat->config, ctrl);
	if (!musb) {
		status = -ENOMEM;
		goto fail0;
	}

	spin_lock_init(&musb->lock);
	nIrq = plat->int_vector;
	musb->edma_mode = plat->edma_mode;
	musb->polling_mode = plat->polling_mode;
	musb->store = plat->store;
	musb->p_i2s = &g_i2s[plat->i2s_tx_port];
	musb->p_i2s->port = plat->i2s_tx_port;
	musb->board_mode = plat->mode;
	musb->ops = plat->platform_ops;

	/* The musb_platform_init() call:
	 *   - adjusts musb->mregs and musb->isr if needed,
	 *   - may initialize an integrated tranceiver
	 *   - initializes musb->xceiv, usually by otg_get_phy()
	 *   - stops powering VBUS
	 *
	 * There are various transceiver configurations.  Blackfin,
	 * DaVinci, TUSB60x0, and others integrate them.  OMAP3 uses
	 * external/discrete ones in various flavors (twl4030 family,
	 * isp1504, non-OTG, etc) mostly hooking up through ULPI.
	 */
	musb->isr = generic_interrupt;
	status = musb_platform_init(musb);
	if (status < 0)
		goto fail1;

	if (!musb->isr) {
		status = -ENODEV;
		goto fail2;
	}


	/* be sure interrupts are disabled before connecting ISR */
	musb_platform_disable(musb);
	musb_generic_disable(musb);

	/* setup musb parts of the core (especially endpoints) */
	status = musb_core_init(plat->config->multipoint
			? MUSB_CONTROLLER_MFDRC
			: MUSB_CONTROLLER_FDRC, musb);
	if (status < 0)
		goto fail2;


	/* attach to the IRQ */
	musb->nIrq = nIrq;
	if (!musb->polling_mode) {
		dev_dbg(NULL, "USB0: interrupt mode - base=0x%x int_vector=%d\n", musb->mregs, nIrq);

		/*register usb int handler and enable it*/
		irq_generic_install_handler(nIrq,
				usb_interrupt_handler,
				musb);
		irq_generic_config_int(nIrq, IRQ_TYPE_EDGE_BOTH);
		irq_generic_enable_int(nIrq);

		_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);
	}

	/* For the host-only role, we can activate right away.
	 * (We expect the ID pin to be forcibly grounded!!)
	 * Otherwise, wait till the gadget driver hooks up.
	 */
	if (!is_otg_enabled(musb) && is_host_enabled(musb)) {
		MUSB_HST_MODE(musb);
	} else /* peripheral is enabled */ {
		MUSB_DEV_MODE(musb);
		if (is_peripheral_capable())
			status = musb_gadget_setup(musb);
	}
	if (status < 0)
		goto fail3;

	dev_info(NULL, "USB %s mode controller at %p using %s, IRQ %d, status %d\n",
			({char *s;
			 switch (musb->board_mode) {
			 case MUSB_HOST:		s = "Host"; break;
			 case MUSB_PERIPHERAL:	s = "Peripheral"; break;
			 default:		s = "OTG"; break;
			 }; s; }),
			ctrl,
			musb->edma_mode ? "DMA" : "PIO",
			musb->nIrq, status);

	return status == 0 ? musb : NULL;

fail3:
	musb_gadget_cleanup(musb);

fail2:
	musb_platform_exit(musb);

fail1:
	musb_free(musb);

fail0:
	printf("musb_init_controller failed with status %d\n", status);

	return status == 0 ? musb : NULL;

}
