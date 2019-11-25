/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ARMv8 interrupt management
 *
 */

#include <kernel.h>
#include <assert.h>
#include <arch/cpu.h>
#include <device.h>
#include <sys/__assert.h>
#include <sys/slist.h>
#include <toolchain.h>
#include <linker/sections.h>
#include <irq.h>
#include <irq_nextlevel.h>
#include <drivers/interrupt_controller/itr_common.h>
#include <kernel_structs.h>
#include <debug/tracing.h>

sys_slist_t intr_desc_list = SYS_SLIST_STATIC_INIT(NULL);

void intr_desc_add(u32_t hwirq, intr_handler_t isr, void *arg)
{
	struct intr_desc_t *intr_desc;

	intr_desc = k_malloc(sizeof(*intr_desc));

	intr_desc->hwirq = hwirq;
	intr_desc->h = isr;
	intr_desc->arg = arg;

	sys_slist_append(&intr_desc_list, &intr_desc->node);
}

struct intr_desc_t *intr_desc_find_map(u32_t hwirq)
{
	struct intr_desc_t *intr_desc;

	SYS_SLIST_FOR_EACH_CONTAINER(&intr_desc_list, intr_desc, node) {
		if (intr_desc->hwirq == hwirq)
			return intr_desc;
	}
	return NULL;
}

/*
 * interrupt controller driver interrupt handler can retrieve the intid
 * and invoke the generic handler.
 * TODO: Zephyr currently have isr prototype as void.
 * Need to change to have return the result.
 */
enum intr_return generic_handle_intr(u32_t intid)
{
	struct intr_desc_t *intr_desc;
	enum intr_return ret = INTR_UNHANDLED;

	intr_desc = intr_desc_find_map(intid);

	if (intr_desc) {
		ret = intr_desc->h(intr_desc->arg);
		ret = INTR_HANDLED;
	} else {
		z_arch_irq_disable(intid);
		printk("Disabled spurious interrupt %d\n", intid);
	}
	return	ret;
}

/* interrupt controller should register its handler */
static void no_handler(void)
{
	/* panic : no interrupt handler registered */
	printk("Interrupt controller handler not registered!\n");
	assert(0);
}

void (*intr_handler)(void) = no_handler;

void set_intr_handler(void (*arch_intr_handler)(void))
{
	intr_handler = arch_intr_handler;
}

static struct device *intctlr_dev;
/* interrupt controller chip device */
void register_intctlr(struct device *dev)
{
	intctlr_dev = dev;
}

/**
 *
 * @brief Enable an interrupt line
 *
 * Enable the interrupt. After this call, the CPU will receive interrupts for
 * the specified <irq>.
 *
 * @return N/A
 */
void z_arch_irq_enable(unsigned int irq)
{
	irq_enable_next_level(intctlr_dev, irq);
}

/**
 *
 * @brief Disable an interrupt line
 *
 * Disable an interrupt line. After this call, the CPU will stop receiving
 * interrupts for the specified <irq>.
 *
 * @return N/A
 */
void z_arch_irq_disable(unsigned int irq)
{
	irq_disable_next_level(intctlr_dev, irq);
}

