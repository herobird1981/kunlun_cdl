#ifndef __USBLIB_CONFIG_H__
#define __USBLIB_CONFIG_H__


#if 0
/*#define DEBUG_OUTPUT*/
#ifdef 	DEBUG_OUTPUT
#define debug(...)	console_printf (__VA_ARGS__)
#else
#define debug(...)
#endif
#endif
#define info(...)	console_printf(__VA_ARGS__)

/*
 * USB configuration.  We enable MUSB support, both for host and for
 * gadget.  For host support UMS.
 * For gadget support both UMS, RNDIS ethernet and UAC2
 */
#define CONFIG_USB_MUSB_GADGET
#define CONFIG_USB_GADGET_VBUS_DRAW	2

#ifdef CONFIG_USB_MUSB_GADGET

/* USB Speech's IDs */
#define CONFIG_G_AUDIO_VENDOR_NUM 0x03FD
#define CONFIG_G_AUDIO_PRODUCT_NUM 0x0300
#define CONFIG_G_AUDIO_MANUFACTURER "Speech"

/* UAC2--USB Audio Class 2.0 */
#define CONFIG_USB_GADGET_AUDIO
#define CONFIG_USB_FUNCTION_UAC2

#endif /* CONFIG_USB_MUSB_GADGET */

//#define ARCH_DMA_MINALIGN	64
#define CONFIG_SYS_CACHELINE_SIZE	32
#define CONFIG_ARM

#define CONFIG_PARTITIONS
#define HAVE_BLOCK_DEVICE

#define CONFIG_SYS_HZ	1000

//extern void mdelay(unsigned long msec);
/* dependence on external functions*/
extern int console_printf( const char* fmt, ... );
//extern void console_puts( const char* s );
//extern void console_putc( const char c );
//extern char console_getc(void);
extern int console_tstc(void);
#define puts console_puts
//#define putc console_putc
//#define printf console_printf

#define error info
#ifndef pr_fmt
#define pr_fmt(fmt) fmt
#endif
#define debug_cond(cond, fmt, args...)			\
	do {						\
		if (cond)				\
			printf(pr_fmt(fmt), ##args);	\
	} while (0)

extern unsigned long long get_timer(unsigned long long start);
//extern unsigned int rand_r( unsigned int* seedp );
extern unsigned long simple_strtoul( const char* cp, char** endp, unsigned int base );
extern int ctrlc(void);
//extern void udelay(unsigned long usec);
#endif

