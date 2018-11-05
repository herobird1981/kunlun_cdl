#ifndef __IMAGE_HEADER_H
#define __IMAGE_HEADER_H

#define IMG_HDR_MAX_SECT  29
#define IMG_PKT_MAXLEN    4096
#define IMG_HDR_SIZE	  512

#pragma pack(push,1)
//define the MemArea structure
typedef struct
{
	unsigned int length;
	unsigned int deployment_addr;
	unsigned short crc16;
	unsigned char  id;
	unsigned char  name[5];
} img_memarea_t;

// define the image header structure
typedef struct
{
	unsigned int fw_marker;							// firmware marker(fixed to be 0x5AA5)
	unsigned int image_len; 						// image length in bytes

	unsigned char  image_ver;
	unsigned char  image_revsision;
	unsigned short rsv0;

	unsigned int image_appentrypointaddress;  		 //Program Entry Point
	unsigned int image_appvecttableaddress;   		 //To initialize the vector table address register
	unsigned int image_numofmemarea;

	img_memarea_t memarea[IMG_HDR_MAX_SECT];

	unsigned char rsv1[22];                            //Padded with zeros ('0')
	unsigned short header_crc16;                       //Includes from FW_Package.                                                 
} img_header_t;
#pragma pack(pop)

#endif
