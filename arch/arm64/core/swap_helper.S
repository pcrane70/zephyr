/*
 * Copyright 2019 Broadcom.
 * The term “Broadcom” refers to Broadcom Inc. and/or its subsidiaries
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Thread context switching for ARMv8
 *
 * This module implements the routines necessary for thread context switching
 * on ARMv8 CPUs.
 */

#include <kernel_structs.h>
#include <offsets_short.h>
#include <toolchain.h>
#include <arch/cpu.h>
#include <syscall.h>

GTEXT(__switch)
GDATA(_kernel)

/**
 *
 * @brief "__switch" swap handler, handling context switches
 *
 */

#define SCRATCH_REG_SIZE (8 * 4)

SECTION_FUNC(TEXT, __switch)

    /* switch to interrupt stack for sched */
    msr spsel, #1

    /* store x0-x3 before using them */
    stp x0, x1, [sp, #-SCRATCH_REG_SIZE]!
    stp x2, x3, [sp, #CTX_GPREG_X2]

    /* load _kernel into x1 and current k_thread into x2 */
    ldr x1, =_kernel
    ldr x2, [x1, #_kernel_offset_to_current]

    /* addr of callee-saved regs in thread in x0 */
    ldr x0, =_thread_offset_to_callee_saved
    add x0, x0, x2

    /* save registers x4-x29, sp_el0, x0-x3 and LR of
     * current(/switched out) thread
     */
    stp x4, x5, [x0, #CTX_GPREG_X4]
    stp x6, x7, [x0, #CTX_GPREG_X6]
    stp x8, x9, [x0, #CTX_GPREG_X8]
    stp x10, x11, [x0, #CTX_GPREG_X10]
    stp x12, x13, [x0, #CTX_GPREG_X12]
    stp x14, x15, [x0, #CTX_GPREG_X14]
    stp x16, x17, [x0, #CTX_GPREG_X16]
    stp x18, x19, [x0, #CTX_GPREG_X18]
    stp x20, x21, [x0, #CTX_GPREG_X20]
    stp x22, x23, [x0, #CTX_GPREG_X22]
    stp x24, x25, [x0, #CTX_GPREG_X24]
    stp x26, x27, [x0, #CTX_GPREG_X26]
    stp x28, x29, [x0, #CTX_GPREG_X28]
    mrs x18, sp_el0
    str x18, [x0, #CTX_GPREG_SP_EL0]

    /* original x0-x3 were saved on sp_el1,
     * let's get and store them as well */
    ldp x21, x22, [sp, #CTX_GPREG_X2]
    ldp x19, x20, [sp], #SCRATCH_REG_SIZE

    stp x19, x20, [x0]
    stp x21, x22, [x0, #CTX_GPREG_X2]

    str x30, [x0, #CTX_GPREG_LR]

    /* _kernel is still in x1 */

    /* fetch the thread to run from the ready queue cache */
    ldr x2, [x1, #_kernel_offset_to_ready_q_cache]

    str x2, [x1, #_kernel_offset_to_current]

    add x0, x2, #_thread_offset_to_callee_saved

    /* Load registers x0-x29, sp_el0 and LR of
     * current(/switched in) thread */
    ldp x4, x5, [x0, #CTX_GPREG_X4]
    ldp x6, x7, [x0, #CTX_GPREG_X6]
    ldp x8, x9, [x0, #CTX_GPREG_X8]
    ldp x10, x11, [x0, #CTX_GPREG_X10]
    ldp x12, x13, [x0, #CTX_GPREG_X12]
    ldp x14, x15, [x0, #CTX_GPREG_X14]
    ldp x16, x17, [x0, #CTX_GPREG_X16]
    ldp x18, x19, [x0, #CTX_GPREG_X18]
    ldp x20, x21, [x0, #CTX_GPREG_X20]
    ldp x22, x23, [x0, #CTX_GPREG_X22]
    ldp x24, x25, [x0, #CTX_GPREG_X24]
    ldp x26, x27, [x0, #CTX_GPREG_X26]
    ldr x28, [x0, #CTX_GPREG_SP_EL0]
    msr sp_el0, x28
    ldp x28, x29, [x0, #CTX_GPREG_X28]

    ldr x30, [x0, #CTX_GPREG_LR]

    ldp x2, x3, [x0, #CTX_GPREG_X2]
    ldp x0, x1, [x0]

    /* switch back to stack of switched in thread */
    msr spsel, #0

#ifdef CONFIG_EXECUTION_BENCHMARKING
    bl read_timer_end_of_swap
#endif

    /* just execute return, based on LR of new thread, it should resume */
    ret
