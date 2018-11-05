/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_utils.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define U8_PER_LINE		16	// 16 bytes per line
#define U16_PER_LINE	8	// 8 x u16 per line
#define U32_PER_LINE	8	// 8 x u32 per line

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
//uint8_t test_txrx_buff[TEST_BUFFER_SIZE+TEST_BUFFER_MARGIN] __attribute__((section( ".dram0.bss")));__attribute__((aligned(64)));
uint8_t test_txrx_buff[TEST_BUFFER_SIZE+TEST_BUFFER_MARGIN] __attribute__((aligned(64)));

uint8_t *test_send_buff = test_txrx_buff;
uint8_t *test_recv_buff = test_txrx_buff + (TEST_BUFFER_SIZE/2);
uint8_t *p_test_tx_buf = test_txrx_buff;
uint8_t *p_test_rx_buf = test_txrx_buff + (TEST_BUFFER_SIZE/2);

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*************************************************************************************************************/
//  Function: void mem_set_random(void *p_dst, uint32_t size)
//
//
//  Parameters:
//		p_dst [out]	: the destination address
//
// 		len			: the length of the data buffer(byte aligned)
//				
//  Return:
//      	none
//
//  Description:
//		Assigned random data to p_dst with specified length.
//
/*************************************************************************************************************/
void mem_set_random(void *p_dst, uint32_t len)
{
	uint8_t *p_data = (uint8_t *)p_dst;
	uint32_t i;
		
	srand(get_ticks());
	
	for (i = 0; i < len; i++) 
	{
		p_data[i] = cb_rand() & 0xff;
	}
}

/*****************************************************************************/
//	Function:  int mem_check_mask(uint8_t *p_dst, uint8_t *p_src, uint32_t len, int dw)
//
//	Parameters:
//		p_dst [in]		: the destination address
//
//		p_src [in]		: the source address
//
// 		len			: the length of the data buffer(byte aligned)
//	
//		dw			: data width(max 32 bits)
//
//	Return:
//		0	: success
//
//		1	: fail
//
//	Description:
//		 Check data between two memorys with specified data width.
//
/*****************************************************************************/
int mem_check_mask(uint8_t *p_dst, uint8_t *p_src, uint32_t len, uint32_t dw)
{
	uint32_t i;
	uint32_t mask = 0;
	uint16_t *p_src16, *p_dst16;
	uint32_t *p_src32, *p_dst32;

	// get data mask to compare
	for (i = 0; i < dw; i++)
	{
		mask |= 1<<i;
	}

	// check data
	if ((dw >= 4) && (dw <= 8))
	{	
		for (i = 0; i < len; i++) 
		{
			if ((p_src[i] & mask) != (p_dst[i] & mask)) 
			{
				info("check data fail @0x%x: expected 0x%x, received 0x%x\n", i, p_src[i], p_dst[i]);
				return 1;
			}
		}
	}
	else if ((dw >= 9) && (dw <= 16))
	{
		p_src16 = (uint16_t *)p_src;
		p_dst16 = (uint16_t *)p_dst;
		len /= 2;
		for (i = 0; i < len; i++) 
		{
			if ((p_src16[i] & mask) != (p_dst16[i] & mask)) 
			{
				info("check data fail @0x%x: expected 0x%x received 0x%x\n", (i*2), p_src16[i], p_dst16[i]);
				return 1;
			}
		}
	}	
	else
	{
		p_src32= (uint32_t *)p_src;
		p_dst32 = (uint32_t *)p_dst;
		len /= 4;
		for (i = 0; i < len; i++) 
		{
			if ((p_src32[i] & mask) != (p_dst32[i] & mask)) 
			{
				info("check data fail @0x%x: expected 0x%x, received 0x%x\n", (i*4), p_src32[i], p_dst32[i]);
				return 1;
			}
		}
	}
	
	return 0;
}

int i2s_mem_check_mask(uint8_t *p_dst, uint8_t *p_src, uint32_t len, uint32_t dw)
{
	uint32_t i;
	uint32_t mask = 0;
	uint32_t *p_src32, *p_dst32;

	// get data mask to compare
	for (i = 0; i < dw; i++)
	{
		mask |= 1<<i;
	}

	// check data
	if ((dw >= 4) && (dw <= 8))
	{	
		for (i = 0; i < len; i++) 
		{
			if ((p_src[i] & mask) != (p_dst[i] & mask)) 
			{
				info("check data fail @0x%x: expected 0x%x, received 0x%x\n", i, p_src[i], p_dst[i]);
				return 1;
			}
		}
	}
	else if ((dw >= 9) && (dw <= 16))
	{
		p_src32= (uint32_t *)p_src;
		p_dst32 = (uint32_t *)p_dst;
		len /= 4;
		for (i = 0; i < len; i++) 
		{
			if ((p_src32[i] & mask) != (p_dst32[i] & mask)) 
			{
				info("check data fail @0x%x: expected 0x%x, received 0x%x\n", (i*4), p_src32[i], p_dst32[i]);
				return 1;
			}
		}
	}	
	else
	{
		p_src32= (uint32_t *)p_src;
		p_dst32 = (uint32_t *)p_dst;
		len /= 4;
		for (i = 0; i < len; i++) 
		{
			if ((p_src32[i] & mask) != (p_dst32[i] & mask)) 
			{
				info("check data fail @0x%x: expected 0x%x, received 0x%x\n", (i*4), p_src32[i], p_dst32[i]);
				return 1;
			}
		}
	}
	
	return 0;
}


/*****************************************************************************/
//	Function:  void print_in_line(uint8_t *name, uint8_t *buf, uint32_t len) 
//
//	Parameters:
//		p_name [in]		: the name address
//
//		p_buf [in]		: the buffer address
//
// 		len				: the length of the buffer(byte aligned)
//
//	Return:
//		none
//
//	Description:
//		 print the buffer in line(16 x u8)
//
/*****************************************************************************/
void print_in_line(uint8_t *p_name, uint8_t *p_buf, uint32_t len) 
{
	uint32_t i = 0, j = 0;
	uint32_t line = 0;
	uint32_t rest = 0;

	printf("%s: 0x%x, 0x%x\n", p_name, (uint32_t)p_buf, len);
	rest = len;
	for (i = 0; i < len; i += U8_PER_LINE) 
	{
		if (!(i % U8_PER_LINE)) 
		{
			info("0x%08x: ", (uint32_t)(p_buf+i));
		}

		line = (rest > U8_PER_LINE) ? U8_PER_LINE : rest;

		for (j = 0; j < line; j++)
		{
			printf("%02x ", *(p_buf + i + j));
		}
		
		printf("\n");

		rest -= line;
	}
}

void print_hex(const char* what, const unsigned long group, const void *p1, const unsigned long len)
{
	const unsigned char* p = p1;
	unsigned long g;
	unsigned long x;
	//unsigned long idx = 0;
	printf("%s contents @ %p: \n", what, p1);
	for (x = 0; x < len; ) {
		if (!(x % 16)) {
			printf("%04lx: ", x);
		}
		for(g=group; g>0; g--) {
			if(x+g-1 >= len)
				continue;
			printf("%02x", p[x+g-1]);
		}
		printf(" ");
		x += group;
		if (group == 1 && !(x % 4) && (x % 16)) {
			printf("  ");
		}
		if (!(x % 16)) {
			printf("\n");
		}
	}
	printf("\n");
}

/*****************************************************************************/
//	Function:  void print_in_line_u16(uint8_t *p_name, uint16_t *p_buf, uint32_t len) 
//
//	Parameters:
//		p_name [in]		: the name address
//
//		p_buf [in]		: the buffer address
//
// 		len				: the length of the buffer(byte aligned)
//
//	Return:
//		none
//
//	Description:
//		 print the buffer in line(8 x u16)
//
/*****************************************************************************/
void print_in_line_u16(uint8_t *p_name, uint16_t *p_buf, uint32_t len) 
{
	uint32_t i = 0, j = 0;
	uint32_t line = 0;
	uint32_t rest = 0;

	printf("%s: 0x%x, 0x%x\n", p_name, (uint32_t)p_buf, len);

	len /= 2; 
	rest = len;
	for (i = 0; i < len; i += U16_PER_LINE) 
	{
		if (!(i % U16_PER_LINE)) 
		{
			info("0x%08x: ", (uint32_t)(p_buf+i));
		}

		line = (rest > U16_PER_LINE) ? U16_PER_LINE : rest;

		for (j = 0; j < line; j++)
		{
			printf("%04x ", *(p_buf + i + j));
		}
		
		printf("\n");

		rest -= line;
	}
}

/*****************************************************************************/
//	Function:  void print_in_line_u32(uint8_t *p_name, uint32_t *p_buf, uint32_t len) 
//
//	Parameters:
//		p_name [in]		: the name address
//
//		p_buf [in]		: the buffer address
//
// 		len				: the length of the buffer(byte aligned)
//
//	Return:
//		none
//
//	Description:
//		 print the buffer in line(8 x u32)
//
/*****************************************************************************/
void print_in_line_u32(uint8_t *p_name, uint32_t *p_buf, uint32_t len) 
{
	uint32_t i = 0, j = 0;
	uint32_t line = 0;
	uint32_t rest = 0;

	printf("%s: 0x%x, 0x%x\n", p_name, (uint32_t)p_buf, len);

	len /= 4; 
	rest = len;
	for (i = 0; i < len; i += U32_PER_LINE) 
	{
		if (!(i % U32_PER_LINE)) 
		{
			info("0x%08x: ", (uint32_t)(p_buf+i));
		}

		line = (rest > U32_PER_LINE) ? U32_PER_LINE : rest;

		for (j = 0; j < line; j++)
		{
			printf("%08x ", *(p_buf + i + j));
		}
		
		printf("\n");

		rest -= line;
	}
}

/*****************************************************************************/
//	Function:  void print_in_line_reg32(uint8_t *p_name, uint32_t *p_buf, uint32_t len) 
//
//	Parameters:
//		p_name [in]		: the name address
//
//		p_buf [in]		: the buffer address
//
// 		len				: the length of the buffer(byte aligned)
//
//	Return:
//		none
//
//	Description:
//		 print the registers in line(8 x u32)
//
/*****************************************************************************/
void print_in_line_reg32(uint8_t *p_name, uint32_t *p_buf, uint32_t len) 
{
	uint32_t i = 0, j = 0;
	uint32_t line = 0;
	uint32_t rest = 0;

	printf("%s: 0x%x, 0x%x\n", p_name, (uint32_t)p_buf, len);

	len /= 4; 
	rest = len;
	for (i = 0; i < len; i += U32_PER_LINE) 
	{
		if (!(i % U32_PER_LINE)) 
		{
			info("0x%08x: ", (uint32_t)(p_buf+i));
		}

		line = (rest > U32_PER_LINE) ? U32_PER_LINE : rest;

		for (j = 0; j < line; j++)
		{
			printf("%08x ", readl(p_buf + i + j));
		}
		
		printf("\n");

		rest -= line;
	}
}

