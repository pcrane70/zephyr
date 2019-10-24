/*
 * Copyright 2019 Broadcom
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Full C support initialization
 *
 *
 * Initialization of full C support: zero the .bss, copy the .data if XIP,
 * call z_cstart().
 *
 * Stack is available in this module, but not the global data/bss until their
 * initialization is performed.
 */

#include <kernel.h>
#include <kernel_structs.h>
#include <zephyr/types.h>
#include <toolchain.h>
#include <linker/linker-defs.h>
#include <kernel_internal.h>
#include <arch/cpu.h>

#ifdef CONFIG_FLOAT
static inline void enable_floating_point(void)
{
	/* TODO: Enable ARMv8 FPU */
}
#else
static inline void enable_floating_point(void)
{
}
#endif

extern FUNC_NORETURN void z_cstart(void);
/**
 *
 * @brief Prepare to and run C code
 *
 * This routine prepares for the execution of and runs C code.
 *
 * @return N/A
 */
#ifdef CONFIG_BOOT_TIME_MEASUREMENT
	extern u64_t __start_time_stamp;
#endif
void _PrepC(void)
{
	_kernel.nested = 0;

	enable_floating_point();
	z_bss_zero();
	z_data_copy();
#ifdef CONFIG_BOOT_TIME_MEASUREMENT
	__start_time_stamp = 0U;
#endif
	z_cstart();
	CODE_UNREACHABLE;
}
