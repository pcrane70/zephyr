/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ARMv8 power management
 *
 */

#include <offsets_short.h>
#include <toolchain.h>
#include <linker/sections.h>
#include <arch/cpu.h>

GTEXT(k_cpu_idle)

/**
 *
 * @brief Power save idle routine for ARMv8
 *
 * This function will be called by the kernel idle loop or possibly within
 * an implementation of _sys_power_save_idle in the kernel when the
 * '_sys_power_save_flag' variable is non-zero.  The ARM 'wfi' instruction
 * will be issued, causing a low-power consumption sleep mode.
 *
 * @return N/A
 *
 * C function prototype:
 *
 * void k_cpu_idle (void);
 */

SECTION_FUNC(TEXT, k_cpu_idle)
	wfi
	msr daifclr, #DAIF_IRQ_BIT
	ret
