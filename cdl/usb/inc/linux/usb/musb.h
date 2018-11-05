/*
 * This is used to for host and peripheral modes of the driver for
 * Inventra (Multidrop) Highspeed Dual-Role Controllers:  (M)HDRC.
 *
 * Board initialization should put one of these into dev->platform_data,
 * probably on some platform_device named "musb-hdrc".  It encapsulates
 * key configuration differences between boards.
 */

#ifndef __LINUX_USB_MUSB_H
#define __LINUX_USB_MUSB_H

#ifndef __deprecated
#define __deprecated
#endif

#include <linux/compat.h>

/* The USB role is defined by the connector used on the board, so long as
 * standards are being followed.  (Developer boards sometimes won't.)
 */
enum musb_mode {
	MUSB_UNDEFINED = 0,
	MUSB_HOST,		/* A or Mini-A connector */
	MUSB_PERIPHERAL,	/* B or Mini-B connector */
	MUSB_OTG		/* Mini-AB connector */
};

struct clk;

enum musb_fifo_style {
	FIFO_RXTX,
	FIFO_TX,
	FIFO_RX
} __attribute__ ((packed));

enum musb_buf_mode {
	BUF_SINGLE,
	BUF_DOUBLE
} __attribute__ ((packed));

struct musb_fifo_cfg {
	u8			hw_ep_num;
	enum musb_fifo_style	style;
	enum musb_buf_mode	mode;
	u16			maxpacket;
};

#define MUSB_EP_FIFO(ep, st, m, pkt)		\
{						\
	.hw_ep_num	= ep,			\
	.style		= st,			\
	.mode		= m,			\
	.maxpacket	= pkt,			\
}

#define MUSB_EP_FIFO_SINGLE(ep, st, pkt)	\
	MUSB_EP_FIFO(ep, st, BUF_SINGLE, pkt)

#define MUSB_EP_FIFO_DOUBLE(ep, st, pkt)	\
	MUSB_EP_FIFO(ep, st, BUF_DOUBLE, pkt)

struct musb_hdrc_eps_bits {
	const char	name[16];
	u8		bits;
};

struct musb_hdrc_config {
	struct musb_fifo_cfg	*fifo_cfg;	/* board fifo configuration */
	unsigned		fifo_cfg_size;	/* size of the fifo configuration */

	/* MUSB configuration-specific details */
	unsigned	multipoint:1;	/* multipoint device */
	unsigned	dyn_fifo:1 __deprecated; /* supports dynamic fifo sizing */
	unsigned	dma:1 __deprecated; /* supports DMA */

	u8		num_eps;	/* number of endpoints _with_ ep0 */
	u8		ram_bits;	/* ram address size */
};

struct musb_hdrc_platform_data {
	/* MUSB_HOST, MUSB_PERIPHERAL, or MUSB_OTG */
	u8		mode;

	/* for clk_get() */
	const char	*clock;

	/* (HOST or OTG) switch VBUS on/off */
	int		(*set_vbus)(struct device *dev, int is_on);

	/* (HOST or OTG) mA/2 power supplied on (default = 8mA) */
	u8		power;

	/* (PERIPHERAL) mA/2 max power consumed (default = 100mA) */
	/* (HOST or OTG) msec/2 after VBUS on till power good */
	u8		potpgt;

	/* (HOST or OTG) program PHY for external Vbus */
	unsigned	extvbus:1;

	/* MUSB configuration-specific details */
	struct musb_hdrc_config	*config;

	/* Architecture specific board data	*/
	void		*board_data;

	/* Platform specific struct musb_ops pointer */
	const void	*platform_ops;
	int int_vector;
	
	int edma_mode;
	int polling_mode;
	int int_dma_vector;
	int i2s_tx_port;
    int store;
};

/*
 * U-Boot specfic stuff
 */
int musb_register(struct musb_hdrc_platform_data *plat, void *bdata,
			void *ctl_regs);
void musb_unregister(struct musb_hdrc_platform_data *plat);

#endif /* __LINUX_USB_MUSB_H */
