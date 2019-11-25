/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief generic interrupt controller wrapper
 *
 *  Abstracts the common funtionalities and provides a framework to
 *  register interrupt controller chip. Included by arm/arch.h.
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_ITR_COMMON_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_ITR_COMMON_H_

#include <device.h>
#include <sys/slist.h>

#ifndef _ASMLANGUAGE

#ifdef __cplusplus
extern "C" {
#endif

typedef enum intr_return (*intr_handler_t)(void *);

struct intr_desc_t {
	u32_t hwirq;
	intr_handler_t h;
	void *arg;
	sys_snode_t node;
};

struct intr_desc_t *intr_desc_find_map(u32_t hwirq);

void set_intr_handler(void (*arch_intr_handler)(void));
void register_intctlr(struct device *dev);
enum intr_return generic_handle_intr(u32_t intid);

void intr_desc_add(u32_t hwirq, intr_handler_t isr, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_ITR_COMMON_H_ */
