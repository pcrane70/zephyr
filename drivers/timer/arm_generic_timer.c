/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <drivers/timer/system_timer.h>
#include <sys_clock.h>
#include <spinlock.h>
#include <arch/arm64/arch_timer.h>

#define CYC_PER_TICK ((u32_t)((u64_t)sys_clock_hw_cycles_per_sec()	\
			      / (u64_t)CONFIG_SYS_CLOCK_TICKS_PER_SEC))
/*
 * We are limiting maximum ticks by the maximum value that "s32_t ticks"
 * variable can hold
 */
#define MAX_TICKS INT_MAX
#define MIN_DELAY 1000

#define TICKLESS (IS_ENABLED(CONFIG_TICKLESS_KERNEL) &&		\
		  !IS_ENABLED(CONFIG_QEMU_TICKLESS_WORKAROUND))

static struct k_spinlock lock;
static u64_t last_count;

static u64_t get_system_count(void)
{
	return arch_counter_get_cntpct();
}

void timer_isr(void *arg)
{
	ARG_UNUSED(arg);

	k_spinlock_key_t key = k_spin_lock(&lock);
	u64_t now = get_system_count();
	u32_t dticks = (u32_t)((now - last_count) / CYC_PER_TICK);

	last_count += dticks * CYC_PER_TICK;

	if (!TICKLESS) {
		u64_t next = last_count + CYC_PER_TICK;

		if ((s64_t)(next - now) < MIN_DELAY) {
			next += CYC_PER_TICK;
		}
		arch_timer_set_cmp_val(next);
	}

	k_spin_unlock(&lock, key);
	z_clock_announce(IS_ENABLED(CONFIG_TICKLESS_KERNEL) ? dticks : 1);
}

void z_clock_set_timeout(s32_t ticks, bool idle)
{
	ARG_UNUSED(idle);

#if defined(CONFIG_TICKLESS_KERNEL) && !defined(CONFIG_QEMU_TICKLESS_WORKAROUND)

	ticks = ticks == K_FOREVER ? MAX_TICKS : ticks;
	ticks = MAX(MIN(ticks - 1, (s32_t)MAX_TICKS), 0);

	k_spinlock_key_t key = k_spin_lock(&lock);
	u64_t now = get_system_count();
	u64_t cyc = ticks * CYC_PER_TICK;

	/* Round up to next tick boundary.  Note use of 32 bit math,
	 * max_ticks is calibrated to permit this.
	 */
	cyc += (now - last_count) + (CYC_PER_TICK - 1);
	cyc = (cyc / CYC_PER_TICK) * CYC_PER_TICK;

	if ((s64_t)(cyc + last_count - now) < MIN_DELAY) {
		cyc += CYC_PER_TICK;
	}

	arch_timer_set_cmp_val(cyc + last_count);
	k_spin_unlock(&lock, key);
#endif
}

u32_t z_clock_elapsed(void)
{
	if (!IS_ENABLED(CONFIG_TICKLESS_KERNEL)) {
		return 0;
	}

	k_spinlock_key_t key = k_spin_lock(&lock);
	u32_t ret = (u32_t)((get_system_count() - last_count) / CYC_PER_TICK);

	k_spin_unlock(&lock, key);
	return ret;
}

u32_t z_timer_cycle_get_32(void)
{
	return (u32_t)(get_system_count() - last_count);
}

int z_clock_driver_init(struct device *dev)
{
	ARG_UNUSED(dev);

	/*
	 * Note: cntfrq_el0 is undefined on power-on,
	 * EL3 firmware has set it to correct value
	 */

	/* TODO: Enable timer interrupt */

	/* Set initial timer comparator value */
	arch_timer_set_cmp_val(get_system_count() + CYC_PER_TICK);

	/* Enable timer */
	arch_timer_enable(CNTP_CTL_ENABLE);

	/* Initialize the last_count */
	last_count = get_system_count();

	return 0;
}
