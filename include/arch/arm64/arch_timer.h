/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_ARM_ARCH_TIMER_H_
#define ZEPHYR_INCLUDE_ARM_ARCH_TIMER_H_

#include <arch/cpu.h>
#include <zephyr/types.h>

#define CNTP_CTL_STATUS		(1 << 2)
#define CNTP_CTL_MASK		(1 << 1)
#define CNTP_CTL_ENABLE		(1 << 0)

static inline void arch_timer_enable(u64_t val)
{
	write_sysreg(val, cntp_ctl_el0);

	__ISB();
}

static inline void arch_timer_set_cmp_val(u64_t val)
{
	write_sysreg(val, cntp_cval_el0);

	__ISB();
}

static inline u64_t arch_counter_get_cntpct(void)
{
	__ISB();

	return read_sysreg(cntpct_el0);
}

#endif /* ZEPHYR_INCLUDE_ARM_ARCH_TIMER_H_ */
