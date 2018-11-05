#ifndef __FW_CRC_H__
#define __FW_CRC_H__

unsigned short crc_ccitt(unsigned short crc, void *buf, int len);

unsigned short crc16_xmodem(unsigned short crc, void *buf, int len);

#define crc16_default crc_ccitt

#endif
