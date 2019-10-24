/*
 * Copyright 2019 Broadcom
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Per-arch thread definition
 *
 * This file contains definitions for
 *
 *  struct _thread_arch
 *  struct _callee_saved
 *
 * necessary to instantiate instances of struct k_thread.
 */

#ifndef ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_THREAD_H_
#define ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_THREAD_H_

#define CTX_GPREG_X0		0x0
#define CTX_GPREG_X1		0x8
#define CTX_GPREG_X2		0x10
#define CTX_GPREG_X3		0x18
#define CTX_GPREG_X4		0x20
#define CTX_GPREG_X5		0x28
#define CTX_GPREG_X6		0x30
#define CTX_GPREG_X7		0x38
#define CTX_GPREG_X8		0x40
#define CTX_GPREG_X9		0x48
#define CTX_GPREG_X10		0x50
#define CTX_GPREG_X11		0x58
#define CTX_GPREG_X12		0x60
#define CTX_GPREG_X13		0x68
#define CTX_GPREG_X14		0x70
#define CTX_GPREG_X15		0x78
#define CTX_GPREG_X16		0x80
#define CTX_GPREG_X17		0x88
#define CTX_GPREG_X18		0x90
#define CTX_GPREG_X19		0x98
#define CTX_GPREG_X20		0xa0
#define CTX_GPREG_X21		0xa8
#define CTX_GPREG_X22		0xb0
#define CTX_GPREG_X23		0xb8
#define CTX_GPREG_X24		0xc0
#define CTX_GPREG_X25		0xc8
#define CTX_GPREG_X26		0xd0
#define CTX_GPREG_X27		0xd8
#define CTX_GPREG_X28		0xe0
#define CTX_GPREG_X29		0xe8
#define CTX_GPREG_LR		0xf0
#define CTX_GPREG_SP_EL0	0xf8
#define CTX_GPREG_PC		0x100
#define CTX_PSTATE		0x108
#define CTX_GPREG_SP_ELx	0x110
#define CTX_FRAME_SIZE		0x120

#ifndef _ASMLANGUAGE
#include <zephyr/types.h>

struct _callee_saved {
	/* Parameter and Result Registers */
	u64_t x0;
	u64_t x1;
	u64_t x2;
	u64_t x3;
	u64_t x4;
	u64_t x5;
	u64_t x6;
	u64_t x7;
	/* Indirect result register */
	u64_t x8;
	/* Corruptible Registers */
	u64_t x9;
	u64_t x10;
	u64_t x11;
	u64_t x12;
	u64_t x13;
	u64_t x14;
	u64_t x15;
	/* Intra-procedure-call corruptible registers */
	u64_t x16;
	u64_t x17;
	/* Callee-saved Registers */
	u64_t x18;
	u64_t x19;
	u64_t x20;
	u64_t x21;
	u64_t x22;
	u64_t x23;
	u64_t x24;
	u64_t x25;
	u64_t x26;
	u64_t x27;
	u64_t x28;
	/* Frame pointer */
	u64_t x29;
	/* Link register */
	u64_t lr;
	/* Stack pointer */
	u64_t sp;
	/* Thread entrypoint */
	u64_t pc;
};

typedef struct _callee_saved _callee_saved_t;

struct _thread_arch {

	/* interrupt locking key */
	u32_t basepri;

	/* x0 in stack frame cannot be written to reliably */
	u32_t swap_return_value;

};

typedef struct _thread_arch _thread_arch_t;

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_THREAD_H_ */
