/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ARMv8 specific kernel interface header
 *
 * This header contains the ARMv8 specific kernel interface.  It is
 * included by the kernel interface architecture-abstraction header
 * (include/arch/cpu.h)
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_H_

/* Add include for DTS generated information */
#include <generated_dts_board.h>

/* Few ARMv8 registers are designated by two different names */
#define sys_define_gpr_with_alias(name1, name2) union { u64_t name1, name2; }

#include <arch/arm64/armv8_a.h>
#include <arch/arm64/barrier.h>
#include <arch/arm64/irq.h>
#include <arch/arm64/exc.h>
#include <arch/arm64/misc.h>
#include <arch/common/addr_types.h>
#include <arch/common/ffs.h>
#include <arch/common/sys_io.h>

/**
 * @brief Declare the STACK_ALIGN_SIZE
 *
 * Denotes the required alignment of the stack pointer on public API
 * boundaries
 * Align stack to cache line size
 *
 */
#define STACK_ALIGN_SIZE	(1 << 6)

/**
 * @brief Declare the minimum alignment for a thread stack
 *
 * Denotes the minimum required alignment of a thread stack.
 *
 * Note:
 * User thread stacks must respect the minimum MPU region
 * alignment requirement.
 */
#define Z_THREAD_MIN_STACK_ALIGN STACK_ALIGN_SIZE

/**
 * @brief Define alignment of a stack buffer
 *
 * This is used for two different things:
 * 1) Used in checks for stack size to be a multiple of the stack buffer
 *    alignment
 * 2) Used to determine the alignment of a stack buffer
 *
 */
#define STACK_ALIGN Z_THREAD_MIN_STACK_ALIGN

#define Z_ARCH_THREAD_STACK_DEFINE(sym, size) \
	struct _k_thread_stack_element __noinit __aligned(STACK_ALIGN) \
		sym[size]

#define Z_ARCH_THREAD_STACK_LEN(size) (size)

#define Z_ARCH_THREAD_STACK_ARRAY_DEFINE(sym, nmemb, size) \
	struct _k_thread_stack_element __noinit \
		__aligned(STACK_ALIGN) \
		sym[nmemb][Z_ARCH_THREAD_STACK_LEN(size)]

#define Z_ARCH_THREAD_STACK_MEMBER(sym, size) \
	struct _k_thread_stack_element __aligned(STACK_ALIGN) sym[size]

#define Z_ARCH_THREAD_STACK_SIZEOF(sym) (sizeof(sym))

#define Z_ARCH_THREAD_STACK_BUFFER(sym) ((char *)(sym))

#ifndef _ASMLANGUAGE

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @brief Disable all interrupts on the CPU
 *
 * This routine disables interrupts.  It can be called from either interrupt or
 * thread level.  This routine returns an architecture-dependent
 * lock-out key representing the "interrupt disable state" prior to the call;
 * this key can be passed to irq_unlock() to re-enable interrupts.
 *
 * @return An architecture-dependent lock-out key representing the
 * "interrupt disable state" prior to the call.
 *
 */

static ALWAYS_INLINE unsigned int z_arch_irq_lock(void)
{
	unsigned int key;

	__asm__ volatile("mrs %0, daif;"
		"msr daifset, %1"
		: "=r" (key)
		: "i" (DAIF_IRQ_BIT)
		: "memory", "cc");

	return key;
}


/**
 *
 * @brief Enable all interrupts on the CPU (inline)
 *
 * This routine re-enables interrupts on the CPU.  The @a key parameter is an
 * architecture-dependent lock-out key that is returned by a previous
 * invocation of irq_lock().
 *
 * This routine can be called from either interrupt or thread level.
 *
 * @param key architecture-dependent lock-out key
 *
 * @return N/A
 *
 */

static ALWAYS_INLINE void z_arch_irq_unlock(unsigned int key)
{
	__asm__ volatile("msr daif, %0"
			:
			: "r" (key)
			: "memory", "cc");
}

/**
 * Returns true if interrupts were unlocked prior to the
 * z_arch_irq_lock() call that produced the key argument.
 */
static ALWAYS_INLINE bool z_arch_irq_unlocked(unsigned int key)
{
	__asm__ volatile("mrs %0, daif;"
		: "=r" (key)
		:
		: "memory", "cc");

	return ((key & DAIF_IRQ_BIT) == 0);
}

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_H_ */
