/* This linker script generated from xt-genldscripts.tpp for LSP D:/SW/Britesemi/kunlun/workspace_hifi/kunlun_0710/RG-2018.9/KL_prod_v7/kunlun_0710 */
/* Linker Script for default link */
MEMORY
{
  dram1_0_seg :                       	org = 0x40008000, len = 0x8000
  dram0_0_seg :                       	org = 0x40010000, len = 0x10000
  iram0_0_seg :                       	org = 0x40020000, len = 0x10000
  iram1_0_seg :                       	org = 0x40030000, len = 0x8000
  sram0_seg :                         	org = 0x60000000, len = 0x200000
  srom0_seg :                         	org = 0x60A00000, len = 0x400
  srom1_seg :                         	org = 0x60A00400, len = 0x190
  srom2_seg :                         	org = 0x60A00590, len = 0x50
  srom3_seg :                         	org = 0x60A005E0, len = 0x50
  srom4_seg :                         	org = 0x60A00630, len = 0x50
  srom5_seg :                         	org = 0x60A00680, len = 0x50
  srom6_seg :                         	org = 0x60A006D0, len = 0x50
  srom7_seg :                         	org = 0x60A00720, len = 0x50
  srom8_seg :                         	org = 0x60A00770, len = 0x50
  srom9_seg :                         	org = 0x60A007C0, len = 0xFF840
}

PHDRS
{
  dram1_0_phdr PT_LOAD;
  dram1_0_bss_phdr PT_LOAD;
  dram0_0_phdr PT_LOAD;
  dram0_0_bss_phdr PT_LOAD;
  iram0_0_phdr PT_LOAD;
  iram1_0_phdr PT_LOAD;
  sram0_phdr PT_LOAD;
  sram0_bss_phdr PT_LOAD;
  srom0_phdr PT_LOAD;
  srom1_phdr PT_LOAD;
  srom2_phdr PT_LOAD;
  srom3_phdr PT_LOAD;
  srom4_phdr PT_LOAD;
  srom5_phdr PT_LOAD;
  srom6_phdr PT_LOAD;
  srom7_phdr PT_LOAD;
  srom8_phdr PT_LOAD;
  srom9_phdr PT_LOAD;
}


/*  Default entry point:  */
ENTRY(_ResetVector)

/*  Memory boundary addresses:  */
_memmap_mem_dram1_start = 0x40008000;
_memmap_mem_dram1_end   = 0x40010000;
_memmap_mem_dram0_start = 0x40010000;
_memmap_mem_dram0_end   = 0x40020000;
_memmap_mem_iram0_start = 0x40020000;
_memmap_mem_iram0_end   = 0x40030000;
_memmap_mem_iram1_start = 0x40030000;
_memmap_mem_iram1_end   = 0x40038000;
_memmap_mem_sram_start = 0x60000000;
_memmap_mem_sram_end   = 0x60200000;
_memmap_mem_srom_start = 0x60a00000;
_memmap_mem_srom_end   = 0x60b00000;

/*  Memory segment boundary addresses:  */
_memmap_seg_dram1_0_start = 0x40008000;
_memmap_seg_dram1_0_max   = 0x40010000;
_memmap_seg_dram0_0_start = 0x40010000;
_memmap_seg_dram0_0_max   = 0x40020000;
_memmap_seg_iram0_0_start = 0x40020000;
_memmap_seg_iram0_0_max   = 0x40030000;
_memmap_seg_iram1_0_start = 0x40030000;
_memmap_seg_iram1_0_max   = 0x40038000;
_memmap_seg_sram0_start = 0x60000000;
_memmap_seg_sram0_max   = 0x60200000;
_memmap_seg_srom0_start = 0x60a00000;
_memmap_seg_srom0_max   = 0x60a00400;
_memmap_seg_srom1_start = 0x60a00400;
_memmap_seg_srom1_max   = 0x60a00590;
_memmap_seg_srom2_start = 0x60a00590;
_memmap_seg_srom2_max   = 0x60a005e0;
_memmap_seg_srom3_start = 0x60a005e0;
_memmap_seg_srom3_max   = 0x60a00630;
_memmap_seg_srom4_start = 0x60a00630;
_memmap_seg_srom4_max   = 0x60a00680;
_memmap_seg_srom5_start = 0x60a00680;
_memmap_seg_srom5_max   = 0x60a006d0;
_memmap_seg_srom6_start = 0x60a006d0;
_memmap_seg_srom6_max   = 0x60a00720;
_memmap_seg_srom7_start = 0x60a00720;
_memmap_seg_srom7_max   = 0x60a00770;
_memmap_seg_srom8_start = 0x60a00770;
_memmap_seg_srom8_max   = 0x60a007c0;
_memmap_seg_srom9_start = 0x60a007c0;
_memmap_seg_srom9_max   = 0x60b00000;

_rom_store_table = 0;
PROVIDE(_memmap_vecbase_reset = 0x60a00400);
PROVIDE(_memmap_reset_vector = 0x60a00000);
/* Various memory-map dependent cache attribute settings: */
_memmap_cacheattr_wb_base = 0x00001400;
_memmap_cacheattr_wt_base = 0x00003400;
_memmap_cacheattr_bp_base = 0x00004400;
_memmap_cacheattr_unused_mask = 0xFFFF00FF;
_memmap_cacheattr_wb_trapnull = 0x44441440;
_memmap_cacheattr_wba_trapnull = 0x44441440;
_memmap_cacheattr_wbna_trapnull = 0x44442440;
_memmap_cacheattr_wt_trapnull = 0x44443440;
_memmap_cacheattr_bp_trapnull = 0x44444440;
_memmap_cacheattr_wb_strict = 0x00001400;
_memmap_cacheattr_wt_strict = 0x00003400;
_memmap_cacheattr_bp_strict = 0x00004400;
_memmap_cacheattr_wb_allvalid = 0x44441444;
_memmap_cacheattr_wt_allvalid = 0x44443444;
_memmap_cacheattr_bp_allvalid = 0x44444444;
PROVIDE(_memmap_cacheattr_reset = _memmap_cacheattr_wb_trapnull);

SECTIONS
{

  .dram1.rodata : ALIGN(4)
  {
    _dram1_rodata_start = ABSOLUTE(.);
    *(.dram1.rodata)
    . = ALIGN (4);
    _dram1_rodata_end = ABSOLUTE(.);
  } >dram1_0_seg :dram1_0_phdr

  .dram1.literal : ALIGN(4)
  {
    _dram1_literal_start = ABSOLUTE(.);
    *(.dram1.literal)
    . = ALIGN (4);
    _dram1_literal_end = ABSOLUTE(.);
  } >dram1_0_seg :dram1_0_phdr

  .dram1.data : ALIGN(4)
  {
    _dram1_data_start = ABSOLUTE(.);
    *(.dram1.data)
    . = ALIGN (4);
    _dram1_data_end = ABSOLUTE(.);
  } >dram1_0_seg :dram1_0_phdr

  .dram1.bss (NOLOAD) : ALIGN(8)
  {
    . = ALIGN (8);
    _dram1_bss_start = ABSOLUTE(.);
    *(.dram1.bss)
    . = ALIGN (8);
    _dram1_bss_end = ABSOLUTE(.);
    _end = ALIGN(0x8);
    PROVIDE(end = ALIGN(0x8));
    _stack_sentry = ALIGN(0x8);
    _memmap_seg_dram1_0_end = ALIGN(0x8);
  } >dram1_0_seg :dram1_0_bss_phdr
  PROVIDE(__stack = 0x40010000);
  _heap_sentry = 0x40010000;

  .dram0.rodata : ALIGN(4)
  {
    _dram0_rodata_start = ABSOLUTE(.);
    *(.dram0.rodata)
    . = ALIGN (4);
    _dram0_rodata_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .iram0.literal : ALIGN(4)
  {
    _iram0_literal_start = ABSOLUTE(.);
    *(.iram0.literal)
    . = ALIGN (4);
    _iram0_literal_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .iram1.literal : ALIGN(4)
  {
    _iram1_literal_start = ABSOLUTE(.);
    *(.iram1.literal)
    . = ALIGN (4);
    _iram1_literal_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .dram0.literal : ALIGN(4)
  {
    _dram0_literal_start = ABSOLUTE(.);
    *(.dram0.literal)
    . = ALIGN (4);
    _dram0_literal_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .dram0.data : ALIGN(4)
  {
    _dram0_data_start = ABSOLUTE(.);
    *(.dram0.data)
    . = ALIGN (4);
    _dram0_data_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .dram0.bss (NOLOAD) : ALIGN(8)
  {
    . = ALIGN (8);
    _dram0_bss_start = ABSOLUTE(.);
    *(.dram0.bss)
    . = ALIGN (8);
    _dram0_bss_end = ABSOLUTE(.);
    _memmap_seg_dram0_0_end = ALIGN(0x8);
  } >dram0_0_seg :dram0_0_bss_phdr

  .iram0.text : ALIGN(4)
  {
    _iram0_text_start = ABSOLUTE(.);
    *(.iram0.text)
    . = ALIGN (4);
    _iram0_text_end = ABSOLUTE(.);
    _memmap_seg_iram0_0_end = ALIGN(0x8);
  } >iram0_0_seg :iram0_0_phdr

  .iram1.text : ALIGN(4)
  {
    _iram1_text_start = ABSOLUTE(.);
    *(.iram1.text)
    . = ALIGN (4);
    _iram1_text_end = ABSOLUTE(.);
    _memmap_seg_iram1_0_end = ALIGN(0x8);
  } >iram1_0_seg :iram1_0_phdr

  .u_boot_list_2_g_audio_bind_callbacks_1 : ALIGN(4)
  {
    _u_boot_list_2_g_audio_bind_callbacks_1_start = ABSOLUTE(.);
    *(.u_boot_list_2_g_audio_bind_callbacks_1)
    . = ALIGN (4);
    _u_boot_list_2_g_audio_bind_callbacks_1_end = ABSOLUTE(.);
  } >sram0_seg :sram0_phdr

  .u_boot_list_2_g_audio_bind_callbacks_3 : ALIGN(4)
  {
    _u_boot_list_2_g_audio_bind_callbacks_3_start = ABSOLUTE(.);
    *(.u_boot_list_2_g_audio_bind_callbacks_3)
    . = ALIGN (4);
    _u_boot_list_2_g_audio_bind_callbacks_3_end = ABSOLUTE(.);
  } >sram0_seg :sram0_phdr

  .sram.rodata : ALIGN(4)
  {
    _sram_rodata_start = ABSOLUTE(.);
    *(.sram.rodata)
    . = ALIGN (4);
    _sram_rodata_end = ABSOLUTE(.);
  } >sram0_seg :sram0_phdr

  .rodata : ALIGN(4)
  {
    _rodata_start = ABSOLUTE(.);
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r.*)
    *(.rodata1)
    __XT_EXCEPTION_TABLE__ = ABSOLUTE(.);
    KEEP (*(.xt_except_table))
    KEEP (*(.gcc_except_table))
    *(.gnu.linkonce.e.*)
    *(.gnu.version_r)
    KEEP (*(.eh_frame))
    /*  C++ constructor and destructor tables, properly ordered:  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    /*  C++ exception handlers table:  */
    __XT_EXCEPTION_DESCS__ = ABSOLUTE(.);
    *(.xt_except_desc)
    *(.gnu.linkonce.h.*)
    __XT_EXCEPTION_DESCS_END__ = ABSOLUTE(.);
    *(.xt_except_desc_end)
    *(.dynamic)
    *(.gnu.version_d)
    . = ALIGN(4);		/* this table MUST be 4-byte aligned */
    _bss_table_start = ABSOLUTE(.);
    LONG(_dram1_bss_start)
    LONG(_dram1_bss_end)
    LONG(_dram0_bss_start)
    LONG(_dram0_bss_end)
    LONG(_bss_start)
    LONG(_bss_end)
    _bss_table_end = ABSOLUTE(.);
    . = ALIGN (4);
    _rodata_end = ABSOLUTE(.);
  } >sram0_seg :sram0_phdr

  .sram.text : ALIGN(4)
  {
    _sram_text_start = ABSOLUTE(.);
    *(.sram.literal .sram.text)
    . = ALIGN (4);
    _sram_text_end = ABSOLUTE(.);
  } >sram0_seg :sram0_phdr

  .text : ALIGN(4)
  {
    _stext = .;
    _text_start = ABSOLUTE(.);
    *(.entry.text)
    *(.init.literal)
    KEEP(*(.init))
    *(.literal .text .literal.* .text.* .stub .gnu.warning .gnu.linkonce.literal.* .gnu.linkonce.t.*.literal .gnu.linkonce.t.*)
    *(.fini.literal)
    KEEP(*(.fini))
    *(.gnu.version)
    . = ALIGN (4);
    _text_end = ABSOLUTE(.);
    _etext = .;
  } >sram0_seg :sram0_phdr

  .sram.data : ALIGN(4)
  {
    _sram_data_start = ABSOLUTE(.);
    *(.sram.data)
    . = ALIGN (4);
    _sram_data_end = ABSOLUTE(.);
  } >sram0_seg :sram0_phdr

  .data : ALIGN(4)
  {
    _data_start = ABSOLUTE(.);
    *(.data)
    *(.data.*)
    *(.gnu.linkonce.d.*)
    KEEP(*(.gnu.linkonce.d.*personality*))
    *(.data1)
    *(.sdata)
    *(.sdata.*)
    *(.gnu.linkonce.s.*)
    *(.sdata2)
    *(.sdata2.*)
    *(.gnu.linkonce.s2.*)
    KEEP(*(.jcr))
    . = ALIGN (4);
    _data_end = ABSOLUTE(.);
  } >sram0_seg :sram0_phdr

  .bss (NOLOAD) : ALIGN(8)
  {
    . = ALIGN (8);
    _bss_start = ABSOLUTE(.);
    *(.dynsbss)
    *(.sbss)
    *(.sbss.*)
    *(.gnu.linkonce.sb.*)
    *(.scommon)
    *(.sbss2)
    *(.sbss2.*)
    *(.gnu.linkonce.sb2.*)
    *(.dynbss)
    *(.bss)
    *(.bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    *(.sram.bss)
    . = ALIGN (8);
    _bss_end = ABSOLUTE(.);
    _memmap_seg_sram0_end = ALIGN(0x8);
  } >sram0_seg :sram0_bss_phdr

  .ResetVector.text : ALIGN(4)
  {
    _ResetVector_text_start = ABSOLUTE(.);
    KEEP (*(.ResetVector.text))
    . = ALIGN (4);
    _ResetVector_text_end = ABSOLUTE(.);
  } >srom0_seg :srom0_phdr

  .ResetHandler.text : ALIGN(4)
  {
    _ResetHandler_text_start = ABSOLUTE(.);
    *(.ResetHandler.literal .ResetHandler.text)
    . = ALIGN (4);
    _ResetHandler_text_end = ABSOLUTE(.);
    _memmap_seg_srom0_end = ALIGN(0x8);
  } >srom0_seg :srom0_phdr

  .WindowVectors.text : ALIGN(4)
  {
    _WindowVectors_text_start = ABSOLUTE(.);
    KEEP (*(.WindowVectors.text))
    . = ALIGN (4);
    _WindowVectors_text_end = ABSOLUTE(.);
  } >srom1_seg :srom1_phdr

  .Level2InterruptVector.literal : ALIGN(4)
  {
    _Level2InterruptVector_literal_start = ABSOLUTE(.);
    *(.Level2InterruptVector.literal)
    . = ALIGN (4);
    _Level2InterruptVector_literal_end = ABSOLUTE(.);
    _memmap_seg_srom1_end = ALIGN(0x8);
  } >srom1_seg :srom1_phdr

  .Level2InterruptVector.text : ALIGN(4)
  {
    _Level2InterruptVector_text_start = ABSOLUTE(.);
    KEEP (*(.Level2InterruptVector.text))
    . = ALIGN (4);
    _Level2InterruptVector_text_end = ABSOLUTE(.);
  } >srom2_seg :srom2_phdr

  .Level3InterruptVector.literal : ALIGN(4)
  {
    _Level3InterruptVector_literal_start = ABSOLUTE(.);
    *(.Level3InterruptVector.literal)
    . = ALIGN (4);
    _Level3InterruptVector_literal_end = ABSOLUTE(.);
    _memmap_seg_srom2_end = ALIGN(0x8);
  } >srom2_seg :srom2_phdr

  .Level3InterruptVector.text : ALIGN(4)
  {
    _Level3InterruptVector_text_start = ABSOLUTE(.);
    KEEP (*(.Level3InterruptVector.text))
    . = ALIGN (4);
    _Level3InterruptVector_text_end = ABSOLUTE(.);
  } >srom3_seg :srom3_phdr

  .Level4InterruptVector.literal : ALIGN(4)
  {
    _Level4InterruptVector_literal_start = ABSOLUTE(.);
    *(.Level4InterruptVector.literal)
    . = ALIGN (4);
    _Level4InterruptVector_literal_end = ABSOLUTE(.);
    _memmap_seg_srom3_end = ALIGN(0x8);
  } >srom3_seg :srom3_phdr

  .Level4InterruptVector.text : ALIGN(4)
  {
    _Level4InterruptVector_text_start = ABSOLUTE(.);
    KEEP (*(.Level4InterruptVector.text))
    . = ALIGN (4);
    _Level4InterruptVector_text_end = ABSOLUTE(.);
  } >srom4_seg :srom4_phdr

  .DebugExceptionVector.literal : ALIGN(4)
  {
    _DebugExceptionVector_literal_start = ABSOLUTE(.);
    *(.DebugExceptionVector.literal)
    . = ALIGN (4);
    _DebugExceptionVector_literal_end = ABSOLUTE(.);
    _memmap_seg_srom4_end = ALIGN(0x8);
  } >srom4_seg :srom4_phdr

  .DebugExceptionVector.text : ALIGN(4)
  {
    _DebugExceptionVector_text_start = ABSOLUTE(.);
    KEEP (*(.DebugExceptionVector.text))
    . = ALIGN (4);
    _DebugExceptionVector_text_end = ABSOLUTE(.);
  } >srom5_seg :srom5_phdr

  .NMIExceptionVector.literal : ALIGN(4)
  {
    _NMIExceptionVector_literal_start = ABSOLUTE(.);
    *(.NMIExceptionVector.literal)
    . = ALIGN (4);
    _NMIExceptionVector_literal_end = ABSOLUTE(.);
    _memmap_seg_srom5_end = ALIGN(0x8);
  } >srom5_seg :srom5_phdr

  .NMIExceptionVector.text : ALIGN(4)
  {
    _NMIExceptionVector_text_start = ABSOLUTE(.);
    KEEP (*(.NMIExceptionVector.text))
    . = ALIGN (4);
    _NMIExceptionVector_text_end = ABSOLUTE(.);
  } >srom6_seg :srom6_phdr

  .KernelExceptionVector.literal : ALIGN(4)
  {
    _KernelExceptionVector_literal_start = ABSOLUTE(.);
    *(.KernelExceptionVector.literal)
    . = ALIGN (4);
    _KernelExceptionVector_literal_end = ABSOLUTE(.);
    _memmap_seg_srom6_end = ALIGN(0x8);
  } >srom6_seg :srom6_phdr

  .KernelExceptionVector.text : ALIGN(4)
  {
    _KernelExceptionVector_text_start = ABSOLUTE(.);
    KEEP (*(.KernelExceptionVector.text))
    . = ALIGN (4);
    _KernelExceptionVector_text_end = ABSOLUTE(.);
  } >srom7_seg :srom7_phdr

  .UserExceptionVector.literal : ALIGN(4)
  {
    _UserExceptionVector_literal_start = ABSOLUTE(.);
    *(.UserExceptionVector.literal)
    . = ALIGN (4);
    _UserExceptionVector_literal_end = ABSOLUTE(.);
    _memmap_seg_srom7_end = ALIGN(0x8);
  } >srom7_seg :srom7_phdr

  .UserExceptionVector.text : ALIGN(4)
  {
    _UserExceptionVector_text_start = ABSOLUTE(.);
    KEEP (*(.UserExceptionVector.text))
    . = ALIGN (4);
    _UserExceptionVector_text_end = ABSOLUTE(.);
  } >srom8_seg :srom8_phdr

  .DoubleExceptionVector.literal : ALIGN(4)
  {
    _DoubleExceptionVector_literal_start = ABSOLUTE(.);
    *(.DoubleExceptionVector.literal)
    . = ALIGN (4);
    _DoubleExceptionVector_literal_end = ABSOLUTE(.);
    _memmap_seg_srom8_end = ALIGN(0x8);
  } >srom8_seg :srom8_phdr

  .DoubleExceptionVector.text : ALIGN(4)
  {
    _DoubleExceptionVector_text_start = ABSOLUTE(.);
    KEEP (*(.DoubleExceptionVector.text))
    . = ALIGN (4);
    _DoubleExceptionVector_text_end = ABSOLUTE(.);
  } >srom9_seg :srom9_phdr

  .srom.rodata : ALIGN(4)
  {
    _srom_rodata_start = ABSOLUTE(.);
    *(.srom.rodata)
    . = ALIGN (4);
    _srom_rodata_end = ABSOLUTE(.);
  } >srom9_seg :srom9_phdr

  .srom.text : ALIGN(4)
  {
    _srom_text_start = ABSOLUTE(.);
    *(.srom.literal .srom.text)
    . = ALIGN (4);
    _srom_text_end = ABSOLUTE(.);
    _memmap_seg_srom9_end = ALIGN(0x8);
  } >srom9_seg :srom9_phdr
  .debug  0 :  { *(.debug) }
  .line  0 :  { *(.line) }
  .debug_srcinfo  0 :  { *(.debug_srcinfo) }
  .debug_sfnames  0 :  { *(.debug_sfnames) }
  .debug_aranges  0 :  { *(.debug_aranges) }
  .debug_pubnames  0 :  { *(.debug_pubnames) }
  .debug_info  0 :  { *(.debug_info) }
  .debug_abbrev  0 :  { *(.debug_abbrev) }
  .debug_line  0 :  { *(.debug_line) }
  .debug_frame  0 :  { *(.debug_frame) }
  .debug_str  0 :  { *(.debug_str) }
  .debug_loc  0 :  { *(.debug_loc) }
  .debug_macinfo  0 :  { *(.debug_macinfo) }
  .debug_weaknames  0 :  { *(.debug_weaknames) }
  .debug_funcnames  0 :  { *(.debug_funcnames) }
  .debug_typenames  0 :  { *(.debug_typenames) }
  .debug_varnames  0 :  { *(.debug_varnames) }
  .xt.insn 0 :
  {
    KEEP (*(.xt.insn))
    KEEP (*(.gnu.linkonce.x.*))
  }
  .xt.prop 0 :
  {
    KEEP (*(.xt.prop))
    KEEP (*(.xt.prop.*))
    KEEP (*(.gnu.linkonce.prop.*))
  }
  .xt.lit 0 :
  {
    KEEP (*(.xt.lit))
    KEEP (*(.xt.lit.*))
    KEEP (*(.gnu.linkonce.p.*))
  }
  .debug.xt.callgraph 0 :
  {
    KEEP (*(.debug.xt.callgraph .debug.xt.callgraph.* .gnu.linkonce.xt.callgraph.*))
  }
}
