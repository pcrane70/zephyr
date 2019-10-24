/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Kernel fatal error handler for ARMv8
 *
 */

#include <toolchain.h>
#include <linker/sections.h>
#include <inttypes.h>

#include <kernel.h>
#include <kernel_structs.h>
#include <logging/log_ctrl.h>

static void esf_dump(const z_arch_esf_t *esf)
{

}

void unhandled_exception(unsigned int reason)
{
	const z_arch_esf_t esf;

	/*
	 * TODO: Fill the exception stack frame and dump them,
	 * Understand which registers should be dumped
	 */

	esf_dump(&esf);

	z_fatal_error(reason, &esf);
}
