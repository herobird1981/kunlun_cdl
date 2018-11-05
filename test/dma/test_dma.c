/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_dma.h"

#if (CONFIG_DMA_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
// the start useful address of each segment
extern char _memmap_seg_dram1_0_end[];
extern char _memmap_seg_dram0_0_end[];
extern char _memmap_seg_iram0_9_end[];
extern char _memmap_seg_iram1_0_end[];
extern char _memmap_seg_sram0_end[];

// the end address of each memory
extern char _memmap_mem_dram1_end[];
extern char _memmap_mem_dram0_end[];
extern char _memmap_mem_iram0_end[];
extern char _memmap_mem_iram1_end[];
extern char _memmap_mem_sram_end[];

cmdline_entry dma_test_menu[];

struct memory_area {
	uint8_t *name;
	uint32_t start;
	uint32_t len;
};

struct memory_area g_memory[9] = { 
		// dsp0 internal memory
		{(uint8_t *)"dsp0_dram1", 0, 0},
		{(uint8_t *)"dsp0_dram0", 0, 0},
		{(uint8_t *)"dsp0_iram0", 0, 0},
		{(uint8_t *)"dsp0_iram1", 0, 0},

		// dsp1 internal memory
		{(uint8_t *)"dsp1_dram1", 0, 0},
		{(uint8_t *)"dsp1_dram0", 0, 0},
		{(uint8_t *)"dsp1_iram0", 0, 0},
		{(uint8_t *)"dsp1_iram1", 0, 0},

		// common sram
		{(uint8_t *)"sram", 	  0, 0}};

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/
static uint32_t dma_test_addr_axi_to_local(uint32_t addr)
{
#if CONFIG_DSP0
	if ((addr & (~DSP_BASE_MASK)) == DSP0_AXI_BASE)
	{
		addr &= DSP_BASE_MASK;
		addr |= DSP_BASE;		
	}
#else	
	if ((addr & (~DSP_BASE_MASK)) == DSP1_AXI_BASE)
	{
		addr &= DSP_BASE_MASK;
		addr |= DSP_BASE;					
	}
#endif
	
	return addr;
}

static void dma_test_get_para(void)
{
	uint32_t i;
	struct memory_area *p_mem;

	// dsp 0 internal memory
	p_mem = &g_memory[0];
	p_mem->name = (uint8_t *)"dsp0_dram1";
	p_mem->start = (uint32_t)(((uint32_t)_memmap_seg_dram1_0_end & DSP_BASE_MASK) + DSP0_AXI_BASE);
	p_mem->len = (uint32_t)(_memmap_mem_dram1_end - _memmap_seg_dram1_0_end);

	p_mem = &g_memory[1];
	p_mem->name = (uint8_t *)"dsp0_dram0";
	p_mem->start = (uint32_t)(((uint32_t)_memmap_seg_dram0_0_end & DSP_BASE_MASK) + DSP0_AXI_BASE);	
	p_mem->len = 0;	// reserve for stack and heap usage
	//p_mem->len = (uint32_t)(_memmap_mem_dram0_end - _memmap_seg_dram0_0_end);

	p_mem = &g_memory[2];
	p_mem->name = (uint8_t *)"dsp0_iram0";
	p_mem->start = (uint32_t)(((uint32_t)_memmap_seg_iram0_9_end & DSP_BASE_MASK) + DSP0_AXI_BASE);
	p_mem->len = (uint32_t)(_memmap_mem_iram0_end - _memmap_seg_iram0_9_end);

	p_mem = &g_memory[3];
	p_mem->name = (uint8_t *)"dsp0_iram1";
	p_mem->start = (uint32_t)(((uint32_t)_memmap_seg_iram1_0_end & DSP_BASE_MASK) + DSP0_AXI_BASE);
	p_mem->len = (uint32_t)(_memmap_mem_iram1_end - _memmap_seg_iram1_0_end);	

	// dsp 1 internal memory
	p_mem = &g_memory[4];
	p_mem->name = (uint8_t *)"dsp1_dram1";
	p_mem->start = (uint32_t)(((uint32_t)_memmap_seg_dram1_0_end & DSP_BASE_MASK) + DSP1_AXI_BASE);
	p_mem->len = (uint32_t)(_memmap_mem_dram1_end - _memmap_seg_dram1_0_end);

	p_mem = &g_memory[5];
	p_mem->name = (uint8_t *)"dsp1_dram0";
	p_mem->start = (uint32_t)(((uint32_t)_memmap_seg_dram0_0_end & DSP_BASE_MASK) + DSP1_AXI_BASE);
	p_mem->len = 0;	// reserve for stack and heap usage
	//p_mem->len = (uint32_t)(_memmap_mem_dram0_end - _memmap_seg_dram0_0_end);

	p_mem = &g_memory[6];
	p_mem->name = (uint8_t *)"dsp1_iram0";
	p_mem->start = (uint32_t)(((uint32_t)_memmap_seg_iram0_9_end & DSP_BASE_MASK) + DSP1_AXI_BASE);
	p_mem->len = (uint32_t)(_memmap_mem_iram0_end - _memmap_seg_iram0_9_end);

	p_mem = &g_memory[7];
	p_mem->name = (uint8_t *)"dsp1_iram1";
	p_mem->start = (uint32_t)(((uint32_t)_memmap_seg_iram1_0_end & DSP_BASE_MASK) + DSP1_AXI_BASE);
	p_mem->len = (uint32_t)(_memmap_mem_iram1_end - _memmap_seg_iram1_0_end);	

	// common sram
	p_mem = &g_memory[8];
	p_mem->name = (uint8_t *)"sram      ";
	p_mem->start = (uint32_t)_memmap_seg_sram0_end;
	p_mem->len = (uint32_t)(_memmap_mem_sram_end - _memmap_seg_sram0_end);		

	info("available memory address and length:\n");
	for (i = 0; i < ARRAY_SIZE(g_memory); i++)
	{
		p_mem = &g_memory[i];
		info("i: %d, name: %s, start: 0x%x, len: 0x%x\n", i, p_mem->name, p_mem->start, p_mem->len);
	}
}

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
/*****************************************************************************/
//	Function:  static void dma_prepare_buf(uint32_t dst, uint32_t src, uint32_t len, E_DMA_WIDTH width)
//
//	Parameters:	
//		dst		: destination address
//
//		src		: source address
//
//		len		: data length(aligned bus width)
//
//		width	: bus width
//
//	Return:
//		none
//
//	Description:
//		 Prepare the dst and src buffer.
//		dst : clear to 0
//		src : filled with random data 
//		|32 bytes	| transfer length 	| 32 bytes	|
//		| all 0xAA	|	random		| all 0x55	|
//
/*****************************************************************************/
static void dma_prepare_buf(uint32_t dst, uint32_t src, uint32_t len, E_DMA_WIDTH width) 
{
	//uint32_t val = 0;
	//uint32_t i, j;
	//uint32_t trans_len = len * (1 << width);
	uint32_t tmp_src = (uint32_t)test_txrx_buff; // to avoid no dword(4 byte aligned) operation in itcm 
	
	// set the first 32 bytes for boundard
	mem_set8((uint8_t *)tmp_src, 0xAA, 32);

	mem_set_seq((uint8_t *)(tmp_src + 32), len);

	// set the last 32 bytes for boundard
	mem_set8((uint8_t *)(tmp_src + 32 + len), 0x55, 32);

	mem_cpy32((void *)src, (void *)tmp_src, (len + 64));

#if 0
	// set the random data to src address 
	srand(get_ticks());
	
	for (src += 32, i = 0; i < len; i++) 
	{
		val = cb_rand();
		if (val == 0) 
		{
			val = 0x12345678;
		}

		// configure data upon the data width
		if (width == DMA_WIDTH_BITS_8) 
		{
			*(uint8_t *)(src + i) = val & 0xFF;
		} 
		else if (width == DMA_WIDTH_BITS_16) 
		{
			*(uint16_t *) (src + (i * 2)) = val & 0xFFFF;
		} 
		else if (width == DMA_WIDTH_BITS_32) 
		{
			*(uint32_t *) (src + (i * 4)) = val;
		} 
		else if (width == DMA_WIDTH_BITS_64) 
		{
			*(uint32_t *) (src + (i * 8))     = val;
			*(uint32_t *) (src + (i * 8) + 4) = val;
		} 
		else if (width == DMA_WIDTH_BITS_128) 
		{
			for (j = 0; j < 16; j += 4)
			{
				*(uint32_t *)(src + (i * 16) + j) = val;	
			}
		} 
		else if (width == DMA_WIDTH_BITS_256) 
		{
			for (j = 0; j < 32; j += 4)
			{
				*(uint32_t *)(src + (i * 32) + j) = val;	
			}
		}
		else	// DMA_WIDTH_BITS_512
		{
			for (j = 0; j < 64; j += 4)
			{
				*(uint32_t *)(src + (i * 64) + j) = val;	
			}			
		}
	}


	// set the last 32 bytes for boundard
	mem_set8((uint8_t *)(src + trans_len), 0x55, 32);
#endif

	// clear the dst buffer
	//mem_set8((uint8_t *)dst, 0, (trans_len + 32 + 32));
	mem_set32((uint8_t *)dst, 0, 2048);
}

/*****************************************************************************/
//	Function:  static int dma_chk_param_validity(uint32_t addr, uint32_t len) 
//
//	Parameters:
//		addr		: memory address
//
//		len		: data length(aligned bus width)
//
//	Return:
//		others	: the index of a valid address in g_memory array
//
//		-1		: invalid index 
//
//	Description:
//		 Check the validity of the parameter.
//
/*****************************************************************************/
static int dma_chk_param_validity(uint32_t addr, uint32_t len) 
{
	uint32_t i;
	struct memory_area *p_mem;

	for (i = 0; i < ARRAY_SIZE(g_memory); i++)
	{
		p_mem = &g_memory[i];

		//info("i: %d, name: %s, start: 0x%x, len: 0x%x\n", i, p_mem->name, p_mem->start, p_mem->len);
		
		if (((addr >= p_mem->start) && (addr < p_mem->start + p_mem->len)) &&
			(((addr + len) >= p_mem->start) && ((addr + len) < p_mem->start + p_mem->len)))
		{
			info("addr(0x%x) in %s\n", addr, p_mem->name);
			return i;		
		}
	}	

	return -1;
}


/*****************************************************************************/
//	Function:  static int dma_chk_buf_single(uint32_t dst, uint32_t src, uint32_t ofst, E_DMA_WIDTH width) 
//
//	Parameters:
//		dst		: destination address
//
//		src		: source address
//
//		ofst		: the offset of the source/destination address
//
//		width	: bus width
//
//	Return:
//		0		: address is valid
//
//		others	: address in invalid
//
//	Description:
//		Compare data between the source and destination address.
//
/*****************************************************************************/
static int dma_chk_buf_single(uint32_t dst, uint32_t src, uint32_t ofst, E_DMA_WIDTH width) 
{
	if (width == DMA_WIDTH_BITS_8) 
	{
		if (*(uint8_t *)src == *(uint8_t *)dst)
		{
			return 0;
		}
		else 
		{
			debug("memory diff @0x%x src=0x%x(0x%x) dst=0x%x(0x%x)\n", ofst, src, *(uint8_t *)src, dst, *(uint8_t *)dst);
			return 1;
		}
	} 
	else if (width == DMA_WIDTH_BITS_16) 
	{
		if (*(uint16_t *)src == *(uint16_t *)dst)
		{
			return 0;
		}
		else 
		{
			debug("memory diff @0x%x src=0x%x(0x%x) dst=0x%x(0x%x)\n", ofst, src, *(uint16_t *)src, dst, *(uint16_t *)dst);
			return 1;
		}
	} 
	else // DMA_WIDTH_BITS_32
	{
		if (*(uint32_t *)src == *(uint32_t *)dst)
		{
			return 0;
		}	
		else 
		{
			debug("memory diff @0x%x src=0x%x(0x%x) dst=0x%x(0x%x)\n", ofst, src, *(uint32_t *)src, dst, *(uint32_t *)dst);
			return 1;
		}
	}
}

static int dma_chk_buf_val(uint32_t addr, uint32_t val, uint32_t ofst, E_DMA_WIDTH width) 
{
	if (width == BYTE_WIDTH) 
	{
		if (*(uint8_t *)addr == (uint8_t)val)
		{
			return 0;
		}
		else 
		{
			debug("memory diff @0x%x src=%x dst=%x\n", ofst, *(uint8_t *)addr, val);
			return 1;
		}
	} 
	else if (width == HWORD_WIDTH) 
	{
		if (*(uint16_t *)addr == (uint16_t)val)
		{
			return 0;
		}
		else 
		{
			debug("memory diff @0x%x src=%x dst=%x\n", ofst, *(uint16_t *)addr, val);
			return 1;
		}
	} 
	else 
	{
		if (*(uint32_t *)addr == (uint32_t)val)
		{
			return 0;
		}
		else 
		{
			debug("memory diff @0x%x src=%x dst=%x\n", ofst, *(uint32_t *)addr, val);
			return 1;
		}
	}
}


static int dma_chk_buf(uint32_t dst, uint32_t src, uint32_t len, int src_dst, E_DMA_WIDTH width, int addr_mode) 
{
	int i = 0;
	int ret = 0;
	uint32_t trans_len = 0;
	uint32_t ofst;
	
#if 1
	trans_len = (src_dst) ? (len * 4) : (len * (1 << width)); 

	// check buffer have same data
	if (addr_mode == INCREASE) 
	{
		for (i = 0; i < trans_len; i++) 
		{
			ofst = 32 + i;
			if (dma_chk_buf_single((dst + ofst), (src + ofst), ofst, DMA_WIDTH_BITS_8)) 
			{
				ret++;
				break;
			}
		}
	} 
	else if (addr_mode == FIXED) 
	{
#if 0	
		if (src_dst) 
		{
			ofst = 32;
			
			// source using default setting
			if (dma_chk_buf_single((dst + ofst), (src + ofst + trans_len - (1 << width)), ofst, width)) 
			{
				ret++;
			}
			
			for (i = 1; i < trans_len / 4; i++) 
			{
				ofst = 32 + (4 * i);
				if (dma_chk_buf_val((dst + ofst), 0, ofst, DMA_WIDTH_BITS_32)) 
				{
					ret++;
					break;
				}
			}		
		} 
		else 
		{
			for (i = 0; i < trans_len / (1 << width); i++) 
			{
				if (dma_chk_buf_single(32 + i * (1 << width), width, src + 32, dst + 32 + i * (1 << width))) 
				{
					ret++;
					break;
				}
			}
		}
#endif			
	} 
	else 
	{
#if 0	
		for (i = 0; i < trans_len / 4; i++) 
		{
			if (width == BYTE_WIDTH) 
			{
				if (dma_chk_buf_single(
						dst + trans_len + 32 - i * 4 - 4 + 3, width,
						src + 32 + i * 4,
						dst + trans_len + 32 - i * 4 - 4 + 3)) {
					ret++;
					break;
				}
				
				if (dma_chk_buf_single(
						dst + trans_len + 32 - i * 4 - 4 + 2, width,
						src + 32 + i * 4 + 1,
						dst + trans_len + 32 - i * 4 - 4 + 2)) {
					ret++;
					break;
				}
				
				if (dma_chk_buf_single(
						dst + trans_len + 32 - i * 4 - 4 + 1, width,
						src + 32 + i * 4 + 2,
						dst + trans_len + 32 - i * 4 - 4 + 1)) {
					ret++;
					break;
				}
				
				if (dma_chk_buf_single(dst + trans_len + 32 - i * 4 - 4,
						width, src + 32 + i * 4 + 3,
						dst + trans_len + 32 - i * 4 - 4)) {
					ret++;
					break;
				}
			} 
			else if (width == HWORD_WIDTH) 
			{
				if (dma_chk_buf_single(
						dst + trans_len + 32 - i * 4 - 4 + 2, width,
						src + 32 + i * 4,
						dst + trans_len + 32 - i * 4 - 4 + 2)) {
					ret++;
					break;
				}
				
				if (dma_chk_buf_single(dst + trans_len + 32 - i * 4 - 4,
						width, src + 32 + i * 4 + 2,
						dst + trans_len + 32 - i * 4 - 4)) {
					ret++;
					break;
				}
			} 
			else 
			{
				if (dma_chk_buf_single(dst + trans_len + 32 - i * 4 - 4,
						width, src + 32 + i * 4,
						dst + trans_len + 32 - i * 4 - 4)) {
					ret++;
					break;
				}
			}
		}
#endif		
	}

	// check dst buffer not overflow
	for (i = 0; i < 32; i++) 
	{
		if (*(uint8_t *) (dst + i) != 0) 
		{
			debug("dst unexpected @0x%x, val=%x should be zero\n", i, *(uint8_t *)(dst + i));
			ret++;
			break;
		}
	}
	
	for (i = 0; i < 32; i++) 
	{
		if (*(uint8_t *) (dst + 32 + trans_len + i) != 0) 
		{
			debug("dst unexpected @0x%x val=%x should be zero\n", trans_len + i + 32, *(unsigned char*)(dst + 32 + trans_len + i));
			ret++;
			break;
		}
	}
#endif	
	return ret;
}



/*****************************************************************************/
//	Function:  static int cmd_help(int argc, char *argv[])
//
//	Parameters:
//		argc	: argument count
//	
//		argv	: argument
//
//	Return:
//		0	: success
//
//	Description:
//		 Help command line.
//
/*****************************************************************************/
static int cmd_help(int argc, char *argv[])
{	
	return cmdline_help_general(&dma_test_menu[0]);
}

/*****************************************************************************/
//	Function:  static int dma_test_m2m(int argc, char *argv[]) 
//
//	Parameters:
//		argc	: argument count
//	
//		argv	: argument
//
//	Return:
//		0	: success
//
//	Description:
//		dma memory to memory test.
//
/*****************************************************************************/
static int dma_test_m2m(int argc, char *argv[]) 
{
	E_DMA_ST st;
	uint32_t src_addr, dst_addr, src_axi_addr, dst_axi_addr;
	E_DMA_CH ch = DMA_CH0;
	uint32_t len = 0;
	int fail = 0;
	int display_flag = 1;
	E_CH_PRIOR prior = CH_PRIOR_7;
	uint32_t i;
	struct memory_area *p_mem;

	info("%s: dma m2m test: sram/itcm/dtcm\n", argv[0]);
	
	// Check the validity of parameter count
	if (argc < 5) 
	{
		info("format: %s ch dst_addr src_addr len [display]\n", argv[0]);

		info("\nformat: %s ch dst_addr src_addr len [display]\n", argv[0]);
		info("ch	  :0-7\n");
		info("dst_addr: destination address\n");
		info("src_addr: source address\n");
		info("len	  : length(8 bytes aligned)\n");

		info("available memory for DMA(address and len should be 8 byte aligned):\n");
		for (i = 0; i < ARRAY_SIZE(g_memory); i++)
		{
			p_mem = &g_memory[i];
			info("i: %d, name: %s, start: 0x%x, len: 0x%x\n", i, p_mem->name, p_mem->start, p_mem->len);
		}

		return ERR_TEST_ARGC;
	}

	// Get the parameters
	ch = get_arg_ulong(argv[1]);
	if (ch >= NUM_DMA_CHANNELS) 
	{
		info("wrong ch number %d,should be 0-7\n", ch);
		return ERR_TEST_PARA;
	}
	
	dst_axi_addr = get_arg_ulong(argv[2]);
	src_axi_addr = get_arg_ulong(argv[3]);
	len = get_arg_ulong(argv[4]);
	if (argc > 5)
	{
		display_flag = get_arg_ulong(argv[5]);
	}

	// change axi address to local
	dst_addr = dma_test_addr_axi_to_local(dst_axi_addr);	
	src_addr = dma_test_addr_axi_to_local(src_axi_addr);

	info("dst_axi_addr: 0x%x\n", dst_axi_addr);
	info("src_axi_addr: 0x%x\n", src_axi_addr);
	info("dst_addr: 0x%x\n", dst_addr);
	info("src_addr: 0x%x\n", src_addr);

	// check the validity of dst_addr
	if (-1 == dma_chk_param_validity(dst_axi_addr, len)) 
	{
		info("dst_addr or trans_len invalid\n");
		return ERR_TEST_PARA;
	}

	// check the validity of src_addr
	if (-1 == dma_chk_param_validity(src_axi_addr, (len + 64))) 
	{
		info("src_addr or trans_len invalid\n");
		return ERR_TEST_PARA;
	}	

	info("\nch=%d, axi_dst=0x%x, axi_src=0x%x, dst=0x%x, src=0x%x, len=0x%x byte\n\n", ch, dst_axi_addr, src_axi_addr, dst_addr, src_addr, len);

	// prepare buffer data, src buffer fill with random value; dst clear to 0
	dma_prepare_buf(dst_addr, src_addr, len, WORD_WIDTH);

	if (display_flag) 
	{
		info("initial buffer before dma operation:\n");
		info("note: first 32 and last 32 bytes for check boundary correct\n");
		print_in_line_u32((uint8_t *)"src", (uint32_t *)src_addr, (len + 64));
		print_in_line_u32((uint8_t *)"dst", (uint32_t *)dst_addr, (len + 64));
	}

	// dma transfer data
	st = dma_m2m(ch, prior, dst_axi_addr, (src_axi_addr + 32), len, ST_POLLING);
	if (st != DMA_ST_DONE) 
	{
		debug("dmac ch %d transfer %s\n", ch, (st == DMA_ST_FAIL) ? "fail" : "timeout");
		fail++;
	}

	if (display_flag) 
	{
		info("initial buffer after dma operation:\n");
		info("note: first 32 and last 32 bytes for check boundary correct\n");
		print_in_line_u32((uint8_t *)"src", (uint32_t *)src_addr, (len + 64));
		print_in_line_u32((uint8_t *)"dst", (uint32_t *)dst_addr, (len + 64));
	}

	if (!fail) 
	{
		if (len != mem_cmp32((void *)dst_addr, (void *)(src_addr + 32), len))
		{
			debug("dmac ch %d transfer result check error\n", ch);
			fail++;
		}
	}

	if (fail) 
	{
		info("%s: test fail\n", argv[0]);
	} 
	else 
	{
		info("%s: test pass\n", argv[0]);
	}
	
	return ERR_TEST_DONE;
}

static int dma_int_counter[8] = {0};
void dma_int_test_handler(void* ptr)
{
	struct brite_dma *priv = ptr;
	u32 int_status,  int_com_status = 0;
	unsigned long ch_status;
	int i;

	int_status = dma_rd_int_st(priv->port);
	int_com_status = dma_rd_com_int_st(priv->port);
	info("dma_int_handler,base=0x%x int_status=0x%x, int_com_status:0x%x\n", priv->base, int_status, int_com_status);

	for (i = 0; i < 8; i++) {
		ch_status = dma_rd_ch_int_st(priv->port, i);
		if (ch_status) {
			info("ch%d int triggered ch_status:0x%x\n", i, ch_status);
			dma_int_counter[i]++;
		}
		dma_clr_ch_int_st(priv->port, i, ch_status);
	}
}

/*testcase:dma interrupt test*/
static int dma_int(int argc, char* argv[])
{
	int fail = 0;
	unsigned long src_addr,dst_addr;
	unsigned long len;
	E_DMA_PORT port = DMA0;
	E_DMA_CH ch = DMA_CH0;
	E_CH_PRIOR prior = CH_PRIOR_7;
	struct brite_dma *priv = &g_dma[port];
	int time_used = 0;

	info("dma_int: interrupt handle test\n");
	if (argc < 2) {
		info("wrong cmd_format: dma_int ch\n");
		info("ch:0-7");
		return 0;
	}
	ch = get_arg_ulong(argv[1]);
	if (ch > 7) {
		info("wrong ch number %d,should be 0-7\n", ch);
		return 0;
	}

	src_addr = SRAM3_BASE;
	dst_addr = SRAM3_BASE + 8192 + 64;
	len = 4096;

	// disable the interrupt of the controller
	dma_int_enable(port, DMAC_INT_DIS);

	// clear the interrupt
	dma_clr_com_int_st(port, 0xFFFFFFFF);
	dma_clr_ch_int_st(port, ch, 0xFFFFFFFF);

	info("DMA%d: base=0x%x int_vector=%d\n", port, priv->base, priv->int_vector);
	/*register int handler and enable it*/
	irq_generic_install_handler(priv->int_vector,
		dma_int_test_handler,
		priv);
	irq_generic_config_int(priv->int_vector, IRQ_TYPE_LEVEL_HIGH);
	irq_generic_enable_int(priv->int_vector);
	_SET_SYSCON_REG_interrupt_control_interrupt_en_227(1);

	/*enable interrupt and configre transfer start*/
	info("ch=%d, src=0x%x, dst=0x%x, len=%d word\n", ch, src_addr, dst_addr, len);

	// prepare buffer data, src buffer fill with random value; dst clear to 0
	dma_prepare_buf(dst_addr, src_addr, len, WORD_WIDTH);

	// enable the interrupt of the controller
	dma_int_enable(port, DMAC_INT_EN);
	dma_en_com_int(port, 0xFFFFFFFF);
	dma_en_com_intsignal(port, 0xFFFFFFFF);

	/*enable the ch interrupt*/
	dma_en_ch_int(port, ch, 0xFFFFFFFF);
	dma_en_ch_intsignal(port, ch, 0xFFFFFFFF);

	/*start ch transfer*/
	dma_int_counter[ch] = 0;
	/*start ch transfer*/
	// dma transfer data	
	dma_m2m(ch, prior, dst_addr, src_addr, len, ST_NO_POLLING);

	while(time_used++ < 500 && dma_int_counter[ch] == 0)
		mdelay(100); /* delay some second for interrupt msg output */

	if (dma_int_counter[ch] == 0) {
		fail++;
	}

	if (fail) {
		info("%s: test fail\n",argv[0]);
	} else {
		info("%s: test pass\n",argv[0]);
	}

	/*disable interrupt*/
	irq_generic_disable_int(priv->int_vector);
	irq_generic_remove_handler(priv->int_vector,NULL,NULL);
	/*clear all pending int status*/
	// disable the interrupt of the controller
	dma_int_enable(port, DMAC_INT_DIS);

	// clear the interrupt
	dma_clr_com_int_st(port, 0xFFFFFFFF);
	dma_clr_ch_int_st(port, ch, 0xFFFFFFFF);

	return 0;
}

int dma_test_prepare(int port)
{
	struct brite_dma *priv = &g_dma[port];

	// check the validity of parameter
	if (port >= NUM_DMA_PORTS)
	{
		info("port %d is too large!\n", port);
		return ERR_TEST_PARA;
	}
	
	// clear the structure
	mem_set8(priv, 0, sizeof(struct brite_dma));

	priv->port = port;
	if (port == DMA0)
	{
		priv->base = DMAC_BASE;
		priv->int_vector = INT_DMAC;
	}
	else
	{
		// not defined on current HW
	}

	dma_init(port);

	dma_test_get_para();
		
	return ERR_TEST_DONE;
}

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry dma_test_menu[] = {
	{"help", 				cmd_help,  				" : Display list of commands"},
	{"h",					cmd_help,				"    : alias for help"},
	{"?",					cmd_help, 				"    : alias for help"},	
	{"dma_test_m2m", 		dma_test_m2m, 			"   : dma mem to mem test"}, 
	{"dma_int",				dma_int,				"  : dma interrupt test" },
	{0, 					0, 						0}
};

#endif

