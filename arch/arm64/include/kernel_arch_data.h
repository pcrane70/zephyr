/*
 * Copyright 2019 Broadcom
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Private kernel definitions (ARM64)
 *
 * This file contains private kernel structures definitions and various
 * other definitions for the ARMv8 architecture family.
 *
 * This file is also included by assembly language files which must #define
 * _ASMLANGUAGE before including this header file.  Note that kernel
 * assembly source files obtains structure offset values via "absolute symbols"
 * in the offsets.o module.
 */

#ifndef ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_DATA_H_
#define ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_DATA_H_

#include <toolchain.h>
#include <linker/sections.h>
#include <arch/cpu.h>
#include <kernel_arch_thread.h>

/* stacks */

#define STACK_ROUND_UP(x) ROUND_UP(x, STACK_ALIGN_SIZE)
#define STACK_ROUND_DOWN(x) ROUND_DOWN(x, STACK_ALIGN_SIZE)

#ifndef _ASMLANGUAGE
#include <kernel.h>
#include <kernel_internal.h>
#include <zephyr/types.h>
#include <sys/dlist.h>
#include <sys/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __esf _esf_t;
typedef struct __basic_sf _basic_sf_t;

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_DATA_H_ */
