#ifndef __USBLIB_H__
#define __USBLIB_H__

struct musb_controller {
	void *regs;
	void *aux_regs;
	int usb_mode;
	int edma_mode;
	int polling_mode;
	int int_vector;
	int int_dma_vector;
	int idx;
	int i2s_tx_port;
	int store;
};
#if 0
/*
 * You can initialize platform's USB host or device
 * ports by passing this enum as an argument to
 * board_usb_init().
 */
enum usb_init_type {
	USB_INIT_HOST,
	USB_INIT_DEVICE
};
#endif
struct dev_ums {
	int (*read_sector)(unsigned long start,
			   unsigned long blkcnt,
			   void *buf);
	int (*write_sector)(unsigned long start,
			    unsigned long blkcnt,
			    const void *buf);
	unsigned int start_sector;
	unsigned int num_sectors;
	const char *name;
};

/*apis*/
int usb_host_init(struct musb_controller *dev);
void usb_host_exit(void);

int usb_host_start(void);
int usb_host_stop(void);
int usb_host_reset(void);
int usb_host_write(unsigned char* buf, unsigned long start,unsigned long cnt);
int usb_host_read(unsigned char* buf, unsigned long start,unsigned long cnt);

/* device usb mass storage */
int usb_dev_mass_storage(void);
int usb_dev_init(struct musb_controller *dev);
void usb_dev_exit(void);

/* device usb audio class 2.0 */
int usb_dev_uac2(int controller_index);
int usb_dev_uac2_init(struct musb_controller *dev);
void usb_dev_uac2_exit(void);
#endif
