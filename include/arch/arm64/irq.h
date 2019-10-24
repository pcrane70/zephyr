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

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_IRQ_H_ */
