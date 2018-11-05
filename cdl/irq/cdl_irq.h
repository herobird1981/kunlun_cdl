/*-----------------------------------------------------------------------------------------------------------*/
/*                                                                                                                                                                                           */
/*          Copyright (C) 2018 Brite Semiconductor Co., Ltd. All rights reserved.                                                                              */
/*                                                                                                                                                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
#ifndef __CDL_IRQ_H__
#define __CDL_IRQ_H__

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define IRQ_TYPE_NONE 			0x00000000	/* Default, unspecified type */
#define IRQ_TYPE_EDGE_RISING	0x00000001	/* Edge rising type */
#define IRQ_TYPE_EDGE_FALLING	0x00000002	/* Edge falling type */
#define IRQ_TYPE_EDGE_BOTH		(IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING)
#define IRQ_TYPE_LEVEL_HIGH		0x00000004	/* Level high type */
#define IRQ_TYPE_LEVEL_LOW		0x00000008	/* Level low type */
#define IRQ_TYPE_SENSE_MASK		0x0000000f	/* Mask of the above */
#define IRQ_TYPE_PROBE			0x00000010	/* Probing in progress */

/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/
#ifdef __cplusplus
typedef void (_xtos_handler_func)(...);
#else
typedef void (_xtos_handler_func)();
#endif
typedef _xtos_handler_func *_xtos_handler;

typedef _xtos_handler_func * xt_irq_handler;


/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/

/*************************************************************************************************************/
// function prototype declaration
/*************************************************************************************************************/
int xt_irq_set_interrupt_handler(unsigned vector, xt_irq_handler handler);

int xt_irq_set_interrupt_handler_arg(unsigned vector, xt_irq_handler handler, void* arg);

unsigned xt_irq_set_intlevel(int level);

unsigned xt_irq_ints_on(unsigned vector);

unsigned xt_irq_ints_off(unsigned vector);

void xt_irq_set_intclear(unsigned vector);

#endif

