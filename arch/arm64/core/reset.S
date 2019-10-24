/*
 * Copyright 2019 Broadcom
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Reset handler
 *
 * Reset handler that prepares the system for running C code.
 */

#include <toolchain.h>
#include <linker/sections.h>
#include <arch/cpu.h>

GTEXT(__reset)
GTEXT(__start)
GTEXT(memset)
#if defined(CONFIG_PLATFORM_SPECIFIC_INIT)
GTEXT(z_platform_init)
#endif

/**
 *
 * @brief Reset vector
 *
 * Ran when the system comes out of reset. The processor is in thread mode with
 * privileged level. At this point, the stack pointers (sp_el0/sp_elx) are not
 * pointing to a valid area in SRAM.
 *
 * Here, we initialize stack with valid address, once done, jump to _PrepC(),
 * which will finish setting up the system for running C code.
 *
 * @return N/A
 */

SECTION_SUBSEC_FUNC(TEXT,_reset_section,__reset)

/*
 * The entry point is located at the __reset symbol, which
 * is fetched by a XIP image playing the role of a bootloader, which jumps to
 * it, not through the reset vector mechanism. Such bootloaders might want to
 * search for a __start symbol instead, so create that alias here.
 */
SECTION_SUBSEC_FUNC(TEXT,_reset_section,__start)

#if defined(CONFIG_PLATFORM_SPECIFIC_INIT)
    bl z_platform_init
#endif

    /* Set the exception vectors. */
    adr x0, _vector_table
    msr vbar_el1, x0
    isb

    /*
     * Enable the SError interrupt now that the
     * exception vectors have been setup.
     */
    msr daifclr, #DAIF_ABT_BIT

    /*
     * Enable the instruction cache, stack pointer
     * and data access alignment checks and disable
     * speculative loads.
     */
    mov	x1, #(SCTLR_I_BIT | SCTLR_A_BIT | SCTLR_SA_BIT)
    mrs	x0, sctlr_el1
    orr	x0, x0, x1
    msr	sctlr_el1, x0
    isb

#ifdef CONFIG_INIT_STACKS
    ldr x0, =_interrupt_stack
    ldr x1, =0xab
    ldr x2, =CONFIG_ISR_STACK_SIZE
    bl memset
#endif

    /*
     * We want to use sp_el0 for general exectution(C runtime stack) and
     * will switch to sp_el1 "automatically" for exception handling,
     * Set SPSel to 0 to achieve the same.
     *
     * Section D1.6.2 in the ARMv8-A ARM (DDI 0487E.a) mentions:
     * "If the PE is executing at EL1 and the PE is using the SP_EL0
     * stack pointer, then on taking an exception that targets EL1,
     * the stack pointer changes to SP_EL1."
     */

    /*
     * First, let's switch to use sp_elx and initialize sp_el1
     * to get ready for exception and interrupt handling later
     */
    msr spsel, #1
    ldr x0, =_interrupt_stack
    add x0, x0, #CONFIG_ISR_STACK_SIZE
    mov sp, x0

    /*
     * As exception/interrupt will use sp_elx, let's use sp_el0 for
     * normal execution; initialize it with _interrupt_stack for
     * initial C runtime exuection,
     * Later kernel will switch to use _main_stack
     */
    msr spsel, #0
    ldr x0, =_interrupt_stack
    add x0, x0, #CONFIG_ISR_STACK_SIZE
    mov sp, x0

    bl _PrepC
