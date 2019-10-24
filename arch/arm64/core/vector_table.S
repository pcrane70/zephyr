/*
 * Copyright 2019 Broadcom
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Populate ARMv8 vector table
 *
 * The table is populated with all the system exception handlers
 */

#include <toolchain.h>
#include <linker/sections.h>

#include <kernel_structs.h>
#include <offsets_short.h>
#include <toolchain.h>
#include <arch/cpu.h>
#include <syscall.h>

GTEXT(_vector_table)

GTEXT(sync_exception_sp_el0)
GTEXT(irq_sp_el0)
GTEXT(fiq_sp_el0)
GTEXT(serror_sp_el0)

GTEXT(sync_exception_sp_elx)
GTEXT(irq_sp_elx)
GTEXT(fiq_sp_elx)
GTEXT(serror_sp_elx)

GTEXT(sync_exception_aarch64)
GTEXT(irq_aarch64)
GTEXT(fiq_aarch64)
GTEXT(serror_aarch64)

GTEXT(sync_exception_aarch32)
GTEXT(irq_aarch32)
GTEXT(fiq_aarch32)
GTEXT(serror_aarch32)

/*
 * The exception vector table has to be 2 kB aligned as required
 * by ARMv8 architecture.
 */
SECTION_FUNC(vectors,_vector_table)
.align 11

/* Each entry in the vector table has to be 128 B aligned as required
 * by ARMv8 architecture. There are 16 entries in the vector table,
 * this means each entry can have maximum (128 B /4 B) = 32 instructions.
 */

/* Exception from Current EL with SP_EL0 : 0x0 - 0x200 */
	.align 7
sync_exception_sp_el0:
	b  unhandled_exception

	.align 7
irq_sp_el0:
	b  unhandled_exception

	.align 7
fiq_sp_el0:
	b  unhandled_exception

	.align 7
serror_sp_el0:
	b  unhandled_exception

/* Exception from Current EL with SP_ELx: 0x200 - 0x400 */
	.align 7
sync_exception_sp_elx:
	b  unhandled_exception

	.align 7
irq_sp_elx:
	b  unhandled_exception

	.align 7
fiq_sp_elx:
	b  unhandled_exception

	.align 7
serror_sp_elx:
	b  unhandled_exception

/* Exception from Lower EL using AArch64 : 0x400 - 0x600 */
	.align 7
sync_exception_aarch64:
	b  unhandled_exception

	.align 7
irq_aarch64:
	b  unhandled_exception

	.align 7
fiq_aarch64:
	b  unhandled_exception

	.align 7
serror_aarch64:
	b  unhandled_exception

/* Exception from Lower EL using AArch32 : 0x600 - 0x800 */
	.align 7
sync_exception_aarch32:
	b  unhandled_exception

	.align 7
irq_aarch32:
	b  unhandled_exception

	.align 7
fiq_aarch32:
	b  unhandled_exception

	.align 7
serror_aarch32:
	b  unhandled_exception
