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
#include <arch/cpu.h>
#include <sys/__assert.h>
#include <toolchain.h>
#include <linker/sections.h>
#include <sw_isr_table.h>
#include <irq.h>
#include <kernel_structs.h>
#include <debug/tracing.h>

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
	/* TODO: enable irq */
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
	/* TODO: disable irq */
}

/**
 * @brief Return IRQ enable state
 *
 * @param irq IRQ line
 * @return interrupt enable state, true or false
 */
int z_arch_irq_is_enabled(unsigned int irq)
{
	/* TODO: Return IRQ enable state */
	return 0;
}
