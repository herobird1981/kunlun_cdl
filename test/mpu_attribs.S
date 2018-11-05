/*
 * mpu_attribs.S
 *
 *  Created on: 2018-7-26
 */


/* _xtos_mpu_attribs:
 * 	.word   0x00006000	// Illegal	(---)
 * 	.word   0x000F7700	// Writeback	(rwx Cacheable Non-shareable wb rd-alloc wr-alloc)
 * 	.word   0x000D5700	// WBNA		(rwx Cacheable Non-shareable wb rd-alloc)
 * 	.word   0x000C4700	// Writethru	(rwx Cacheable Non-shareable wt rd-alloc)
 * 	.word   0x00006700	// Bypass	(rwx Device non-interruptible system-shareable)
 */
 #define MPU_ATTR_ILL 0x00006000
 #define MPU_ATTR_BYP 0x00006700
 #define MPU_ATTR_WB  0x000F7700
 #define MPU_ATTR_WT  0x000C4700
 #define MPU_ATTR_RO  0x000C4500
 
 #define MPU_ATTR_EN 0x1

	.global __xt_mpu_init_table
	.align 4
__xt_mpu_init_table:
	.word 0x0 | MPU_ATTR_EN
	.word MPU_ATTR_ILL
	.word 0x30008000 | MPU_ATTR_EN	// dram start address
	.word MPU_ATTR_BYP
	.word 0x30038000 | MPU_ATTR_EN	// iram end address
	.word MPU_ATTR_ILL
	.word 0x40008000 | MPU_ATTR_EN	// dram start address
	.word MPU_ATTR_BYP
	.word 0x40038000 | MPU_ATTR_EN	// iram end address
	.word MPU_ATTR_ILL
	.word 0x50008000 | MPU_ATTR_EN	// dram start address
	.word MPU_ATTR_BYP
	.word 0x50038000 | MPU_ATTR_EN	// iram end address
	.word MPU_ATTR_ILL
	.word 0x60000000 | MPU_ATTR_EN	// sram start address
	.word MPU_ATTR_BYP
	.word 0x60200000 | MPU_ATTR_EN	// register start address
	.word MPU_ATTR_BYP
	.word 0x6060A000 | MPU_ATTR_EN	// register end address
	.word MPU_ATTR_ILL
	.word 0x60A00000 | MPU_ATTR_EN	// rom start address
	.word MPU_ATTR_WT
	.word 0x60B00000 | MPU_ATTR_EN	// rom end address
	.word MPU_ATTR_ILL
	
	.global __xt_mpu_init_table_size
__xt_mpu_init_table_size:
	.word 12
