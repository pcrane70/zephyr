/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ARMv8 public exception handling
 *
 * ARM-specific kernel exception handling interface. Included by arm64/arch.h.
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_EXC_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_EXC_H_

#ifndef _ASMLANGUAGE
#include <zephyr/types.h>

/* TODO: Understand which registers are to be part struct __esf */
struct __esf {
	struct __basic_sf {
		u64_t x0;
		u64_t x1;
		u64_t x2;
		u64_t x3;
	} basic;
};

typedef struct __esf z_arch_esf_t;

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_EXC_H_ */
