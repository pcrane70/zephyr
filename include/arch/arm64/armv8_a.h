/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_SYSREG_H
#define ZEPHYR_INCLUDE_ARCH_ARM64_SYSREG_H

#include <sys/util.h>

/* SCTLR definitions */
#define SCTLR_M_BIT		BIT(0)
#define SCTLR_A_BIT		BIT(1)
#define SCTLR_C_BIT		BIT(2)
#define SCTLR_SA_BIT		BIT(3)
#define SCTLR_I_BIT		BIT(12)

/* CPSR/SPSR definitions */
#define DAIF_FIQ_BIT		BIT(0)
#define DAIF_IRQ_BIT		BIT(1)
#define DAIF_ABT_BIT		BIT(2)
#define DAIF_DBG_BIT		BIT(3)


#ifndef _ASMLANGUAGE

#define xstr(a)	str(a)
#define str(a)	#a

#define read_sysreg(reg) ({					\
	u64_t val;						\
	__asm__ volatile("mrs %0, " xstr(reg) : "=r" (val));	\
	val;							\
})

#define write_sysreg(val, reg) ({				\
	__asm__ volatile("msr " xstr(reg) ", %0": : "r" (val));	\
})

#endif  /* !_ASMLANGUAGE */

#endif	/* ZEPHYR_INCLUDE_ARCH_ARM64_SYSREG_H */
