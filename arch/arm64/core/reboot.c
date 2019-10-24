/*
 * Copyright 2019 Broadcom
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ARMv8 system reboot interface
 */

#include <kernel.h>
#include <arch/cpu.h>
#include <misc/util.h>

/**
 *
 * @brief Reset the system
 *
 * This routine resets the processor.
 *
 * @return N/A
 */

void __weak sys_arch_reboot(int type)
{
	ARG_UNUSED(type);
}
