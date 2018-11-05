/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_TYPES_H__
#define __CDL_TYPES_H__

#include <unistd.h>

#define __KERNEL__
#define __UBOOT__

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned int uint;

typedef unsigned char __u8;
typedef unsigned short __le16;
typedef unsigned long __u32;
typedef unsigned long  __le32;
typedef unsigned long long __u64;

typedef unsigned char u_char;
typedef unsigned long u_long;

typedef long long loff_t;
typedef ulong lbaint_t;

#ifndef NULL
#define NULL	0
#endif

typedef int	bool;
#define true	1
#define false	0

typedef unsigned int		BOOL;

#define TRUE		1
#define FALSE		0


typedef u32 dma_addr_t;
#define ULONG_MAX	(0xffffffff) //(~0UL)

#define __raw_writeb(v,a)	(*(volatile u8 *)(a) = ((u8)v))
#define __raw_writew(v,a)	(*(volatile u16 *)(a) = ((u16)v))
#define __raw_writel(v,a)	(*(volatile u32 *)(a) = ((u32)v))
#define __raw_readb(a)		(*(volatile u8 *)(a))
#define __raw_readw(a)		(*(volatile u16 *)(a))
#define __raw_readl(a)		(*(volatile u32 *)(a))

#define readl	__raw_readl
#define writel	__raw_writel
#define readw	__raw_readw
#define writew	__raw_writew
#define readb	__raw_readb
#define writeb	__raw_writeb

#define typeof	__typeof__
#define __aligned(x)	__attribute__((aligned(x)))
#define __user
#define __iomem
#define __packed	__attribute__((packed))

#define BITS_PER_LONG 32

#define BIT(nr)	(1UL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)

#define GENMASK(h, l) \
	(((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define ROUND(a,b)	(((a) + (b) - 1) & ~((b) - 1))
#define DIV_ROUND_UP(n,d)	(((n) + (d) - 1) / (d))
#define DIV_ROUND_HALF_UP(n,d)	(((n) + (d)/2) / (d))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

/* memory operation definition*/
#define REG32(addr)   		(*((volatile u32 *)(addr)))
#define MEM32(addr)   		(*((u32 *)(addr)))

#define REG16(addr)   		(*((volatile u16 *)(addr)))
#define MEM16(addr)  		(*((u16 *)(addr)))

#define REG8(addr)    		(*((volatile u8 *)(addr)))
#define MEM8(addr)    		(*((u8 *)(addr)))

/* byte operation*/
#define get_16_bits_val(d) ((d[1] << 8) | (d[0] << 0)) 
#define get_32_bits_val(d) ((d[3] << 24) | (d[2] << 16) | (d[1] << 8) | (d[0] << 0)) 

#define min(a, b) (((a) < (b)) ? (a) : (b))
	
#define upper_32_bits(n) ((u32)(((n) >> 16) >> 16))
#define lower_32_bits(n) ((u32)(n))

#define __swab32(x) \
	((u32)( \
		(((u32)(x) & (u32)0x000000ffUL) << 24) | \
		(((u32)(x) & (u32)0x0000ff00UL) <<  8) | \
		(((u32)(x) & (u32)0x00ff0000UL) >>  8) | \
		(((u32)(x) & (u32)0xff000000UL) >> 24) ))

#define ALIGN(x,a)		__ALIGN_MASK((x),(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)	(((x)+(mask))&~(mask))



#define BUG() do { \
	info("CDL BUG at %s:%d!\n", __FILE__, __LINE__); \
} while (0)

#define BUG_ON(condition) do { if (condition) BUG(); } while(0)

#define WARN_ON(x) if (x) {debug("WARNING in %s line %d\n" \
				  , __FILE__, __LINE__); }

#define ARCH_DMA_MINALIGN	64
#define WATCHDOG_RESET() {}
#define __weak				__attribute__((weak))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// for pad share
#define MA_INW	__raw_readl
#define MA_OUTW(a, v)	(*(volatile u32 *)(a) = ((u32)v))

#ifdef __inline
#	warning __inline already defined.
#else
#	define __inline __attribute__ ((always_inline))
//#define __inline
#endif
#endif

