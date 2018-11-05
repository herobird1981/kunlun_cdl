/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#include "cdl_types.h"
#include "hw_maps.h"
#include "cdl_irq.h"
#include "interrupt.h"

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

#if 0
/* each vector locate at fixed position in hander table*/
static void irq_generic_empty_handler(void* arg)
{
	u32 vector = (u32)arg;
	info("empty handler,vector=%d\n", vector);
}
#endif

static bool irq_generic_is_valid_vector(u32 vector)
{
	return (vector <= BSP_INTERRUPT_VECTOR_MAX);
}


/*register a interrupt handler*/
int irq_generic_install_handler(unsigned long vector,
        irq_generic_handler_fn handler, void* arg)
{
	if (handler == NULL) {
		return 1;
	} else if (!irq_generic_is_valid_vector(vector)) {
		return 2;
	}

	return xt_irq_set_interrupt_handler_arg(vector, handler, arg);

}

/*remove a interrupt handler,replace with default one*/
int irq_generic_remove_handler(unsigned long vector,
        irq_generic_handler_fn handler, void* arg)
{	
	if (!irq_generic_is_valid_vector(vector)) {
		return 2;
	}
	
	return xt_irq_set_interrupt_handler_arg(vector, NULL, NULL);
}

void irq_generic_enable_int(unsigned long vector)
{
	if (irq_generic_is_valid_vector(vector)) {
		xt_irq_ints_on(vector);
	}
}

void irq_generic_disable_int(unsigned long vector)
{
	if (irq_generic_is_valid_vector(vector)) {
		xt_irq_ints_off(vector);
	}
}
void irq_generic_clear_int(unsigned long vector)
{
	if (irq_generic_is_valid_vector(vector)) {
		xt_irq_set_intclear(vector);
	}
}
int irq_generic_config_int(unsigned long vector, int type)
{
	return 0;
}

void irq_generic_init(volatile void *base)
{
	xt_irq_set_intlevel(0);
}

/* enable IRQ interrupts */
void enable_interrupts (void)
{
	xt_irq_set_intlevel(0);
}

/*
 * disable IRQ/FIQ interrupts
 * returns true if interrupts had been enabled before we disabled them
 */
int disable_interrupts (void)
{
	xt_irq_set_intlevel(15);

	return 0;
}
