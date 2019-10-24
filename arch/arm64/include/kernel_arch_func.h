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
 * This file contains private kernel function definitions and various
 * other definitions for the ARMv8 architecture family.
 *
 * This file is also included by assembly language files which must #define
 * _ASMLANGUAGE before including this header file.  Note that kernel
 * assembly source files obtains structure offset values via "absolute symbols"
 * in the offsets.o module.
 */

/* this file is only meant to be included by kernel_structs.h */

#ifndef ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_FUNC_H_
#define ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_FUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE

static ALWAYS_INLINE void kernel_arch_init(void)
{
	/* Add any architecture specific initialization here */

}

static ALWAYS_INLINE void
z_arch_switch_to_main_thread(struct k_thread *main_thread,
			    k_thread_stack_t *main_stack,
			    size_t main_stack_size, k_thread_entry_t _main)
{
	/* Get high address of the stack, i.e. its start (stack grows down) */
	char *start_of_main_stack;

	start_of_main_stack =
		Z_THREAD_STACK_BUFFER(main_stack) + main_stack_size;

	start_of_main_stack = (char *)STACK_ROUND_DOWN(start_of_main_stack);

	_current = main_thread;
#ifdef CONFIG_TRACING
	z_sys_trace_thread_switched_in();
#endif

	/* the ready queue cache already contains the main thread */

	/*
	 * Set SP to the highest address of the main stack
	 * before enabling interrupts and jumping to main.
	 */
	__asm__ volatile (
	"mov   x0,  %0;"   /* Store _main in R0 */
	"mov   sp, %1;"    /* set sp (start_of_main_stack) */
	 /* __enable_irq() */
	"isb;"
	"mov x1, #0;"
	"mov x2, #0;"
	"mov x3, #0;"
	"bl z_thread_entry"   /* z_thread_entry(_main, 0, 0, 0); */
	:
	: "r" (_main), "r" (start_of_main_stack)
	);

	CODE_UNREACHABLE;
}

static ALWAYS_INLINE void
z_set_thread_return_value(struct k_thread *thread, unsigned int value)
{
	thread->arch.swap_return_value = value;
}

extern void k_cpu_atomic_idle(unsigned int key);

#define z_is_in_isr() (_kernel.nested != 0U)

extern void unhandled_exception(unsigned int reason);

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_FUNC_H_ */
