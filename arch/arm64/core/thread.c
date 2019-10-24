/*
 * Copyright 2019 Broadcom
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief New thread creation for ARMv8
 *
 * Core thread related primitives for the ARMv8 architecture.
 */

#include <kernel.h>
#include <toolchain.h>
#include <kernel_structs.h>
#include <wait_q.h>

/**
 *
 * @brief Initialize a new thread from its stack space
 *
 *
 * <options> is currently unused.
 *
 * @param stack      pointer to the aligned stack memory
 * @param stackSize  size of the available stack memory in bytes
 * @param pEntry the entry point
 * @param parameter1 entry point to the first param
 * @param parameter2 entry point to the second param
 * @param parameter3 entry point to the third param
 * @param priority   thread priority
 * @param options    thread options: K_ESSENTIAL, K_FP_REGS
 *
 * @return N/A
 */

void z_new_thread(struct k_thread *thread, k_thread_stack_t *stack,
		 size_t stackSize, k_thread_entry_t pEntry,
		 void *parameter1, void *parameter2, void *parameter3,
		 int priority, unsigned int options)
{
	char *pStackMem = Z_THREAD_STACK_BUFFER(stack);
	char *stackEnd;

	Z_ASSERT_VALID_PRIO(priority, pEntry);

	stackEnd = pStackMem + stackSize;

	z_new_thread_init(thread, pStackMem, stackSize, priority,
			 options);

	/*
	 * We do not need to carve out any region from the "base" of the stack
	 * as on exception, no registers get saved by default on the stack frame
	 * for ARMv8-A
	 */

	thread->callee_saved.sp = (u64_t)stackEnd;
	thread->callee_saved.x0 = (u64_t)pEntry;
	thread->callee_saved.x1 = (u64_t)parameter1;
	thread->callee_saved.x2 = (u64_t)parameter2;
	thread->callee_saved.x3 = (u64_t)parameter3;
	thread->callee_saved.lr = (u64_t)z_thread_entry;

	thread->arch.basepri = 0;

	/* swap_return_value can contain garbage */

	/*
	 * initial values in all other registers/thread entries are
	 * irrelevant.
	 */
}
