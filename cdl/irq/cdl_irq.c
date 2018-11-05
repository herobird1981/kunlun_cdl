/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include <xtensa/xtruntime.h>
#include "cdl_config.h"
#include "cdl_irq.h"
#include "hw_interrupt.h"

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


int xt_irq_set_interrupt_handler(unsigned vector, xt_irq_handler handler) {
	return (_xtos_set_interrupt_handler(vector, handler) != 0);
}

int xt_irq_set_interrupt_handler_arg(unsigned vector, xt_irq_handler handler, void* arg) {
	return (_xtos_set_interrupt_handler_arg(vector, handler, arg) != 0);
}

unsigned xt_irq_set_intlevel(int level) {
	return _xtos_set_intlevel(level);
}

unsigned xt_irq_ints_on(unsigned vector) {
	return _xtos_ints_on(1UL << vector);
}

unsigned xt_irq_ints_off(unsigned vector) {
	return _xtos_ints_off(1UL << vector);
}

void xt_irq_set_intclear(unsigned vector) {
	xthal_set_intclear(1UL << vector);
}
