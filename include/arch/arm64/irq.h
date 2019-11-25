/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ARMv8 public interrupt handling
 *
 * ARM-specific kernel interrupt handling interface. Included by arm/arch.h.
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_IRQ_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_IRQ_H_

#include <irq.h>

#ifndef _ASMLANGUAGE

#ifdef __cplusplus
extern "C" {
#endif

extern void z_arch_irq_enable(unsigned int irq);
extern void z_arch_irq_disable(unsigned int irq);
extern int z_arch_irq_is_enabled(unsigned int irq);

extern void gic_intr_config(u32_t intid, u32_t prio, u32_t flags);

enum intr_return {
	INTR_HANDLED,
	INTR_UNHANDLED,
};

typedef enum intr_return (*intr_handler_t)(void *);
extern void intr_desc_add(u32_t hwirq, intr_handler_t isr, void *arg);

/**
 * Configure a static interrupt.
 *
 * All arguments must be computable by the compiler at build time.
 *
 * Z_ISR_DECLARE will populate the .intList section with the interrupt's
 * parameters, which will then be used by gen_irq_tables.py to create
 * the vector table and the software ISR table. This is all done at
 * build-time.
 *
 * We additionally set the priority in the interrupt controller at
 * runtime.
 *
 * @param irq_p IRQ line number
 * @param priority_p Interrupt priority
 * @param isr_p Interrupt service routine
 * @param isr_param_p ISR parameter
 * @param flags_p IRQ options
 *
 * @return The vector assigned to this interrupt
 */
#if CONFIG_GEN_SW_ISR_TABLE
#define Z_ARCH_IRQ_CONNECT(irq_p, priority_p, isr_p, isr_param_p, flags_p) \
({ \
	Z_ISR_DECLARE(irq_p, 0, isr_p, isr_param_p); \
	z_irq_priority_set(irq_p, priority_p, flags_p); \
	irq_p; \
})
#else
#define Z_ARCH_IRQ_CONNECT(irq_p, priority_p, isr_p, isr_param_p, flags_p) \
({ \
	intr_desc_add(irq_p, isr_p, isr_param_p); \
	gic_intr_config(irq_p, priority_p, flags_p);\
})
#endif


#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_IRQ_H_ */
