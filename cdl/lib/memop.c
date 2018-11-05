/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "memop.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/


/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/

/*************************************************************************************************************/
//  Function: void mem_cpy8(void *p_dst, const void *p_src, uint32_t size)
//
//  Parameters:
//		p_dst [out]	: the destination address
//		p_src [in]		: the source address
// 		size			: the size of the data buffer(byte aligned)
//				
//  Return:
//      	none
//
//  Description:
//		Copy 8bit data from p_src to p_dst with specified size.
//
/*************************************************************************************************************/
void mem_cpy8(void *p_dst, const void *p_src, uint32_t size)
{
	volatile uint8_t *p_tmp_dst = (uint8_t *)p_dst;
	volatile uint8_t *p_tmp_src = (uint8_t *)p_src;

	if ((p_dst == NULL) || (p_src == NULL)) 
	{
		return;
	}
	
	if (size) 
	{
		do 
		{
			*p_tmp_dst++ = *p_tmp_src++;
		} while(--size);
	}
}

/*************************************************************************************************************/
//  Function: void mem_cpy32(void *p_dst, const void *p_src, uint32_t size)
//
//  Parameters:
//		p_dst [out]	: the destination address
//		p_src [in]		: the source address
// 		size			: the size of the data buffer(4bytes aligned)
//
//  Return:
//      	none
//
//  Description:
//		Copy 32bit data from p_src to p_dst with specified size.
//
/*************************************************************************************************************/
void mem_cpy32(void *p_dst, const void *p_src, uint32_t size)
{
	volatile uint32_t *p_tmp_dst = (uint32_t *)p_dst;
	volatile uint32_t *p_tmp_src = (uint32_t *)p_src;
	uint32_t len = size/4;

	if ((p_dst == NULL) || (p_src == NULL)) 
	{
		return;
	}

	if (len) 
	{
		do 
		{
			*p_tmp_dst++ = *p_tmp_src++;
		} while(--len);
	}
}

/*************************************************************************************************************/
//  Function: void mem_set8(void *p_dst, uint8_t val, uint32_t size)
//
//
//  Parameters:
//		p_dst [out]	: the destination address
//		val			: data to be assigned to the destination address
// 		size			: the size of the data buffer(byte aligned)
//				
//  Return:
//      	none
//
//  Description:
//		Assigned the 8bit data to p_dst with specified size
//
/*************************************************************************************************************/
void mem_set8(void *p_dst, uint8_t val, uint32_t size)
{
	volatile uint8_t *p_tmpdst = (uint8_t *)p_dst;

	if (p_dst == NULL) 
	{
		return;
	}
	
	if (size) 
	{
		do 
		{
			*p_tmpdst++ = val;
		} while(--size);
	}
}


/*************************************************************************************************************/
//  Function: void mem_set32(void *p_dst, uint32_t val, uint32_t size)
//
//
//  Parameters:
//		p_dst [out]	: the destination address
//		val			: data to be assigned to the destination address
// 		size			: data size(byte aligned)
//				
//  Return:
//      	none
//
//  Description:
//		Assigned the 32bit data to p_dst with specified size
//
/*************************************************************************************************************/
void mem_set32(void *p_dst, uint32_t val, uint32_t size)
{
	volatile uint32_t *p_tmpdst = (uint32_t *)p_dst;

	if (p_dst == NULL) 
	{
		return;
	}

	size >>= 2;
	if (size) 
	{		
		do 
		{
			*p_tmpdst++ = val;
		} while(--size);
	}
}

/*************************************************************************************************************/
//  Function: int mem_cmp8(void *p_dst, void *p_src, uint32_t size)
//
//  Parameters:
//		p_dst [out]	: the destination address
//		p_src [in]		: the source address
// 		size			: compare data size(byte aligned)
//				
//  Return:
//      	equal data size
//		-1  : left < right
//		0	: all data are equal
//		1   : left > right
//
//  Description:
//		compare 8bit data between p_src and p_dst with specified size
//
/*************************************************************************************************************/
int mem_cmp8(void *p_dst, void *p_src, uint32_t size)
{
	volatile uint8_t *p_tmpDst = (uint8_t *)p_dst;
	volatile uint8_t *p_tmpSrc = (uint8_t *)p_src;
	uint32_t i = 0;

	if ((p_dst == NULL) || (p_src == NULL) || (size == 0)) 
	{
		return 0;
	}
	
	while (i < size) 
	{
		if (p_tmpDst[i] < p_tmpSrc[i])	
		{
			return -1;
		} 
		else if (p_tmpDst[i] > p_tmpSrc[i])	
		{
			return 1;
		}
		
		i++;
	}

	return 0;
}

/*************************************************************************************************************/
//  Function: uint32_t mem_cmp32(void *p_dst, void *p_src, uint32_t size)
//
//  Parameters:
//		p_dst [out]	: the destination address
//		p_src [in]		: the source address
// 		size			: compare data size(byte aligned)
//				
//  Return:
//      	equal data size
//		0	: no data is equal
//		size	: all data are equal
//
//  Description:
//		compare 32bit data between p_src and p_dst with specified size
//
/*************************************************************************************************************/
uint32_t mem_cmp32(void *p_dst, void *p_src, uint32_t size)
{
	volatile uint32_t *p_tmpDst = (uint32_t *)p_dst;
	volatile uint32_t *p_tmpSrc = (uint32_t *)p_src;
	uint32_t i = 0;

	size >>= 2;
	if ((p_dst == NULL) || (p_src == NULL) || (size == 0)) 
	{
		return 0;
	}
	
	while (i < size) 
	{		
		if (p_tmpDst[i] != p_tmpSrc[i]) 
		{
			return i; 
		}

		i++;
	}

	return (i << 2);
}

void mem_set_seq(uint8_t *p_data, uint32_t size)
{
	uint32_t i;

	for (i = 0; i < size; i++) 
	{
		p_data[i] = i & 0xFF;
	}
}

/*************************************************************************************************************/
//  Function: uint8_t power(uint32_t size)
//
//  Parameters:
// 		size			: the input size
//				
//  Return:
//      	the power of the size
//
//  Description:
//		Caculate the power of the input size
//		size			power
//		0			0
//		1			0
//		2			1
//		...			...
//		0x80000000	31
//
/*************************************************************************************************************/
uint8_t power(uint32_t size)
{
	uint8_t i;

	for (i = 31; i >= 1; i--)
	{
		if (size & (1UL << i))
		{
			return i;
		}
	}

	return 0;
}


