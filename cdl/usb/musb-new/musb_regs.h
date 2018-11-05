/*
 * MUSB OTG driver register defines
 *
 * Copyright 2005 Mentor Graphics Corporation
 * Copyright (C) 2005-2006 by Texas Instruments
 * Copyright (C) 2006-2007 Nokia Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef __MUSB_REGS_H__
#define __MUSB_REGS_H__

#define MUSB_EP0_FIFOSIZE	64	/* This is non-configurable */

/*
 * MUSB Register bits
 */

/* POWER */
#define MUSB_POWER_ISOUPDATE    0x80
#define MUSB_POWER_VBUSVAL      0x40
#define MUSB_POWER_VBUSSESS     0x20
#define MUSB_POWER_VBUSLO       0x10
#define MUSB_POWER_RESET        0x08
#define MUSB_POWER_RESUME       0x04
#define MUSB_POWER_SUSPENDM     0x02
#define MUSB_POWER_ENSUSPEND    0x01

/* INTRUSB */
#define MUSB_INTR_SUSPEND       0x01
#define MUSB_INTR_RESUME        0x02
#define MUSB_INTR_RESET         0x04
#define MUSB_INTR_BABBLE        0x04
#define MUSB_INTR_SOF           0x08
#define MUSB_INTR_CONNECT       0x10
#define MUSB_INTR_DISCONNECT    0x20
#define MUSB_INTR_SESSREQ       0x40
#define MUSB_INTR_VBUSERROR     0x80	/* For SESSION end */

/* DEVCTL */
#define MUSB_DEVCTL_CID         0x80
#define MUSB_DEVCTL_FSDEV       0x40
#define MUSB_DEVCTL_LSDEV       0x20
#define MUSB_DEVCTL_PUCON       0x10
#define MUSB_DEVCTL_PDCON       0x08
#define MUSB_DEVCTL_HM          0x04
#define MUSB_DEVCTL_HR          0x02
#define MUSB_DEVCTL_SESSION     0x01

/* MUSB ULPI VBUSCONTROL */
#define MUSB_ULPI_USE_EXTVBUS    0x01
#define MUSB_ULPI_USE_EXTVBUSIND 0x02
/* ULPI_REG_CONTROL */
#define MUSB_ULPI_REG_REQ        (1 << 0)
#define MUSB_ULPI_REG_CMPLT      (1 << 1)
#define MUSB_ULPI_RDN_WR         (1 << 2)

/* TESTMODE */
#define MUSB_TEST_FORCE_HOST     0x80
#define MUSB_TEST_FIFO_ACCESS    0x40
#define MUSB_TEST_FORCE_FS       0x20
#define MUSB_TEST_FORCE_HS       0x10
#define MUSB_TEST_PACKET         0x08
#define MUSB_TEST_K              0x04
#define MUSB_TEST_J              0x02
#define MUSB_TEST_SE0_NAK        0x01

/* Allocate for double-packet buffering (effectively doubles assigned _SIZE) */
#define MUSB_FIFOSZ_DPB          0x10
/* Allocation size (8, 16, 32, ... 1024) */
#define MUSB_FIFOSZ_SIZE         0xe0

/* CSR0 */
#define MUSB_CSR0_FLUSHFIFO      0x0100
#define MUSB_CSR0_TXPKTRDY       0x0002
#define MUSB_CSR0_RXPKTRDY       0x0001

/* CSR0 in Peripheral mode */
#define MUSB_CSR0_P_SVDSETUPEND  0x0080
#define MUSB_CSR0_P_SVDRXPKTRDY  0x0040
#define MUSB_CSR0_P_SENDSTALL    0x0020
#define MUSB_CSR0_P_SETUPEND     0x0010
#define MUSB_CSR0_P_DATAEND      0x0008
#define MUSB_CSR0_P_SENTSTALL    0x0004

/* CSR0 in Host mode */
#define MUSB_CSR0_H_NAKTIMEOUT   0x0080
#define MUSB_CSR0_H_STATUSPKT    0x0040
#define MUSB_CSR0_H_REQPKT       0x0020
#define MUSB_CSR0_H_ERROR        0x0010
#define MUSB_CSR0_H_SETUPPKT     0x0008
#define MUSB_CSR0_H_RXSTALL      0x0004

/* CSR0 bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_CSR0_P_WZC_BITS	\
	(MUSB_CSR0_P_SENTSTALL)
#define MUSB_CSR0_H_WZC_BITS	\
	(MUSB_CSR0_H_NAKTIMEOUT | MUSB_CSR0_H_RXSTALL \
	| MUSB_CSR0_RXPKTRDY)

/* TxType/RxType */
#define MUSB_TYPE_PROTO          0x30
#define MUSB_TYPE_PROTO_SHIFT    4
#define MUSB_TYPE_REMOTE_END     0xf

/* CONFIGDATA */
#define MUSB_CONFIGDATA_MPRXE		0x80	/* Auto bulk pkt combining */
#define MUSB_CONFIGDATA_MPTXE		0x40	/* Auto bulk pkt splitting */
#define MUSB_CONFIGDATA_BIGENDIAN	0x20
#define MUSB_CONFIGDATA_HBRXE		0x10	/* HB-ISO for RX */
#define MUSB_CONFIGDATA_HBTXE		0x08	/* HB-ISO for TX */
#define MUSB_CONFIGDATA_DYNFIFO		0x04	/* Dynamic FIFO sizing */
#define MUSB_CONFIGDATA_SOFTCONE	0x02	/* SoftConnect */
#define MUSB_CONFIGDATA_UTMIDW		0x01	/* Data width 0/1 => 8/16bits */

/* TXCSR in Peripheral and Host mode */
#define MUSB_TXCSR_AUTOSET          0x8000
#define MUSB_TXCSR_MODE             0x2000
#define MUSB_TXCSR_DMAENAB          0x1000
#define MUSB_TXCSR_FRCDATATOG       0x0800
#define MUSB_TXCSR_DMAMODE          0x0400
#define MUSB_TXCSR_CLRDATATOG       0x0040
#define MUSB_TXCSR_FLUSHFIFO        0x0008
#define MUSB_TXCSR_FIFONOTEMPTY     0x0002
#define MUSB_TXCSR_TXPKTRDY         0x0001

/* TXCSR in Peripheral mode */
#define MUSB_TXCSR_P_ISO            0x4000
#define MUSB_TXCSR_P_SENTSTALL		0x0020
#define MUSB_TXCSR_P_SENDSTALL		0x0010
#define MUSB_TXCSR_P_UNDERRUN		0x0004

/* TXCSR in Host mode */
#define MUSB_TXCSR_H_NAKTIMEOUT     0x0080
#define MUSB_TXCSR_H_RXSTALL        0x0020
#define MUSB_TXCSR_H_ERROR          0x0004

/* TXCSR bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_TXCSR_P_WZC_BITS	\
	(MUSB_TXCSR_P_SENTSTALL \
	| MUSB_TXCSR_P_UNDERRUN | MUSB_TXCSR_FIFONOTEMPTY)
#define MUSB_TXCSR_H_WZC_BITS	\
	(MUSB_TXCSR_H_NAKTIMEOUT | MUSB_TXCSR_H_RXSTALL \
	| MUSB_TXCSR_H_ERROR | MUSB_TXCSR_FIFONOTEMPTY)

/* RXCSR in Peripheral and Host mode */
#define MUSB_RXCSR_AUTOCLEAR        0x8000
#define MUSB_RXCSR_DMAENAB          0x2000
#define MUSB_RXCSR_DMAMODE          0x1000
#define MUSB_RXCSR_CLRDATATOG       0x0080
#define MUSB_RXCSR_FLUSHFIFO        0x0010
#define MUSB_RXCSR_DATAERROR        0x0008
#define MUSB_RXCSR_FIFOFULL         0x0002
#define MUSB_RXCSR_RXPKTRDY         0x0001

/* RXCSR in Peripheral mode */
#define MUSB_RXCSR_P_ISO            0x4000
#define MUSB_RXCSR_P_SENTSTALL      0x0040
#define MUSB_RXCSR_P_SENDSTALL      0x0020
#define MUSB_RXCSR_P_OVERRUN        0x0004

/* RXCSR in Host mode */
#define MUSB_RXCSR_H_AUTOREQ        0x4000
#define MUSB_RXCSR_H_RXSTALL        0x0040
#define MUSB_RXCSR_H_REQPKT         0x0020
#define MUSB_RXCSR_H_ERROR          0x0004

/* RXCSR bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_RXCSR_P_WZC_BITS	\
	(MUSB_RXCSR_P_SENTSTALL | MUSB_RXCSR_P_OVERRUN \
	| MUSB_RXCSR_RXPKTRDY)
#define MUSB_RXCSR_H_WZC_BITS	\
	(MUSB_RXCSR_H_RXSTALL | MUSB_RXCSR_H_ERROR \
	| MUSB_RXCSR_DATAERROR | MUSB_RXCSR_RXPKTRDY)


/*
 * Common USB registers
 */

#define MUSB_FADDR          0x00	/* 8-bit */
#define MUSB_POWER          0x01	/* 8-bit */

#define MUSB_INTRTX1        0x02	/* 8-bit */
#define MUSB_INTRTX2        0x03	/* 8-bit */
#define MUSB_INTRRX1        0x04
#define MUSB_INTRRX2        0x05
#define MUSB_INTRUSB        0x06	/* 8 bit */

#define MUSB_INTRTX1E       0x07
#define MUSB_INTRTX2E       0x08
#define MUSB_INTRRX1E       0x09
#define MUSB_INTRRX2E       0x0A
#define MUSB_INTRUSBE       0x0B	/* 8 bit */

#define MUSB_FRAME1         0x0C
#define MUSB_FRAME2         0x0D

#define MUSB_INDEX          0x0E	/* 8 bit */
#define MUSB_DEVCTL         0x0F	/* 8 bit */

/* Get offset for a given FIFO from musb->mregs */
#define MUSB_FIFO_OFFSET(epnum)	(0x20 + (epnum * 4))

/*
 * Additional Control Registers
 */

/* These are always controlled through the INDEX register */
#define MUSB_TXFIFOSZ       0x1D	/* 8-bit (see masks) */
#define MUSB_RXFIFOSZ       0x1F	/* 8-bit (see masks) */
#define MUSB_TXFIFOADD      0x1C	/* 16-bit offset shifted right 3 */
#define MUSB_RXFIFOADD      0x1E	/* 16-bit offset shifted right 3 */

/* Offsets to endpoint registers */
#define MUSB_TXMAXP        0x00
#define MUSB_TXCSR1        0x01
#define MUSB_TXCSR2        0x02
#define MUSB_CSR01         MUSB_TXCSR1	/* Re-used for EP0 */
#define MUSB_CSR02         MUSB_TXCSR2	/* Re-used for EP0 */
#define MUSB_RXMAXP        0x03
#define MUSB_RXCSR1        0x04
#define MUSB_RXCSR2        0x05
#define MUSB_RXCOUNT1      0x06
#define MUSB_RXCOUNT2      0x07
#define MUSB_COUNT0        MUSB_RXCOUNT1	/* Re-used for EP0 */
#define MUSB_TXTYPE        0x08
#define MUSB_TXINTERVAL    0x09
#define MUSB_NAKLIMIT0     MUSB_TXINTERVAL	/* Re-used for EP0 */
#define MUSB_RXTYPE        0x0A
#define MUSB_RXINTERVAL    0x0B
#define MUSB_FIFOSIZE      0x0F

/* Offsets to endpoint registers in indexed model (using INDEX register) */
#define MUSB_INDEXED_OFFSET(_epnum, _offset)	\
	(0x10 + (_offset))

static inline unsigned char musb_readb(const void __iomem *addr, unsigned offset)
	{ return __raw_readb(addr + offset); }

static inline unsigned short musb_readw(const void __iomem *addr, unsigned offset)
	{ return __raw_readw(addr + offset); }

static inline unsigned long musb_readl(const void __iomem *addr, unsigned offset)
	{ return __raw_readl(addr + offset); }

static inline void musb_writeb(void __iomem *addr, unsigned offset, unsigned char data)
	{ __raw_writeb(data, addr + offset); }

static inline void musb_writew(void __iomem *addr, unsigned offset, unsigned short data)
	{ __raw_writew(data, addr + offset); }

static inline void musb_writel(void __iomem *addr, unsigned offset, unsigned long data)
	{ __raw_writel(data, addr + offset); }

static inline void musb_write_txfifosz(void __iomem *mbase, u8 c_size)
{
	//musb_writeb(mbase, MUSB_TXFIFOSZ, c_size);
}

static inline void musb_write_txfifoadd(void __iomem *mbase, u8 c_size, u16 c_off)
{
	u8 txfifo2;

	txfifo2 = c_size; //musb_readb(mbase, MUSB_TXFIFOSZ);
	txfifo2 &= (MUSB_FIFOSZ_SIZE | MUSB_FIFOSZ_DPB);
	txfifo2 |= (c_off >> 8 & 0x0f);
	musb_writeb(mbase, MUSB_TXFIFOSZ, txfifo2);
	musb_writeb(mbase, MUSB_TXFIFOADD, c_off & 0xff);
}

static inline void musb_write_rxfifosz(void __iomem *mbase, u8 c_size)
{
	//musb_writeb(mbase, MUSB_RXFIFOSZ, c_size);
}

static inline void  musb_write_rxfifoadd(void __iomem *mbase, u8 c_size, u16 c_off)
{
	u8 rxfifo2;

	rxfifo2 = c_size; //musb_readb(mbase, MUSB_RXFIFOSZ);
	rxfifo2 &= (MUSB_FIFOSZ_SIZE | MUSB_FIFOSZ_DPB);
	rxfifo2 |= (c_off >> 8 & 0x0f);
	musb_writeb(mbase, MUSB_RXFIFOSZ, rxfifo2);
	musb_writeb(mbase, MUSB_RXFIFOADD, c_off & 0xff);
}

static inline u8 musb_read_txfifosz(void __iomem *mbase)
{
	return musb_readb(mbase, MUSB_TXFIFOSZ);
}

static inline u16 musb_read_txfifoadd(void __iomem *mbase)
{
	return (((musb_readb(mbase, MUSB_TXFIFOSZ) << 8) & 0x0f) | musb_readb(mbase, MUSB_TXFIFOADD));
}

static inline u8 musb_read_rxfifosz(void __iomem *mbase)
{
	return musb_readb(mbase, MUSB_RXFIFOSZ);
}

static inline u16  musb_read_rxfifoadd(void __iomem *mbase)
{
	return (((musb_readb(mbase, MUSB_RXFIFOSZ) << 8) & 0x0f) | musb_readb(mbase, MUSB_RXFIFOADD));
}


static inline u8 musb_read_configdata(void __iomem *mbase)
{
#if 0
	musb_writeb(mbase, MUSB_INDEX, 0);
	return musb_readb(mbase, 0x10 + MUSB_CONFIGDATA);
#else
	return 0x2E;
#endif
}


/*add for epx(0-15) txcsr/rxcsr read and write */
static inline u16 musb_read_txcsr(void __iomem *epio)
{
	return (musb_readb(epio, MUSB_TXCSR2) << 8) | musb_readb(epio, MUSB_TXCSR1);
}

static inline u16 musb_read_rxcsr(void __iomem *epio)
{
	return (musb_readb(epio, MUSB_RXCSR2) << 8) | musb_readb(epio, MUSB_RXCSR1);
}

static inline u16 musb_read_rxcount(void __iomem *epio)
{
	return (musb_readb(epio, MUSB_RXCOUNT2) << 8) | musb_readb(epio, MUSB_RXCOUNT1);
}

static inline void musb_write_txcsr(void __iomem *epio, u16 csr)
{
	musb_writeb(epio, MUSB_TXCSR2, (csr >> 8));
	musb_writeb(epio, MUSB_TXCSR1, csr);
}

static inline void musb_write_rxcsr(void __iomem *epio, u16 csr)
{
	musb_writeb(epio, MUSB_RXCSR2, (csr >> 8));
	musb_writeb(epio, MUSB_RXCSR1, csr);
}

#endif	/* __MUSB_REGS_H__ */
