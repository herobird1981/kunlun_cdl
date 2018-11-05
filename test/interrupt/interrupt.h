/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/

#define BSP_INTERRUPT_VECTOR_MIN	0
#define BSP_INTERRUPT_VECTOR_MAX	31
#define BSP_INTERRUPT_VECTOR_NUMBER 	(BSP_INTERRUPT_VECTOR_MAX - BSP_INTERRUPT_VECTOR_MIN + 1)

typedef void (*irq_generic_handler_fn)(void*);


/*apis*/
void irq_generic_init(volatile void *base);
int irq_generic_install_handler(unsigned long vector,
        irq_generic_handler_fn handler, void* arg);
int irq_generic_remove_handler(unsigned long vector,
        irq_generic_handler_fn handler, void* arg);
void irq_generic_enable_int(unsigned long vector);
void irq_generic_disable_int(unsigned long vector);
void irq_generic_clear_int(unsigned long vector);
int irq_generic_config_int(unsigned long vector, int type);

/* irq handler to CPU*/
void irq_generic_handler(void);

/*arm generic IRQ enable/disable*/
void enable_interrupts (void);
int disable_interrupts (void);
#endif
