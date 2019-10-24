/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel.h>
#include <toolchain.h>
#include <kernel_structs.h>

void __switch(void);

#ifdef CONFIG_EXECUTION_BENCHMARKING
extern void read_timer_start_of_swap(void);
extern void read_timer_end_of_swap(void);
#endif
extern const int _k_neg_eagain;

/**
 *
 * @brief Initiate a cooperative context switch
 *
 * The __swap() routine is invoked by various kernel services to effect
 * a cooperative context context switch.  Prior to invoking __swap(), the caller
 * disables interrupts via irq_lock() and the return 'key' is passed as a
 * parameter to __swap().
 *
 * __swap() simply stores the intlock key parameter into current->basepri
 * and then calls __switch(), which does the heavy lifting of context switching.

 * Given that __swap() is called to effect a cooperative context switch,
 * only the callee-saved integer registers need to be saved in the thread of the
 * outgoing thread.
 *
 * @return -EAGAIN, or a return value set by a call to
 * z_set_thread_return_value()
 *
 */
int __swap(int key)
{
#ifdef CONFIG_EXECUTION_BENCHMARKING
	read_timer_start_of_swap();
#endif

	/* store off key and return value */
	_current->arch.basepri = key;
	_current->arch.swap_return_value = _k_neg_eagain;

	__switch();

	irq_unlock(key);

	/* Context switch is performed here. Returning implies the
	 * thread has been context-switched-in again.
	 */
	return _current->arch.swap_return_value;
}
