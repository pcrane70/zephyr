/*
 * Copyright 2019 Broadcom
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Populate ARMv8 vector table
 *
 * The table is populated with all the system exception handlers
 */

#include <toolchain.h>
#include <linker/sections.h>

#include <kernel_structs.h>
#include <offsets_short.h>
#include <toolchain.h>
#include <arch/cpu.h>
#include <syscall.h>

GTEXT(_vector_table)

GTEXT(intr_handler)

GTEXT(sync_exception_sp_el0)
GTEXT(irq_sp_el0)
GTEXT(fiq_sp_el0)
GTEXT(serror_sp_el0)

GTEXT(sync_exception_sp_elx)
GTEXT(irq_sp_elx)
GTEXT(fiq_sp_elx)
GTEXT(serror_sp_elx)

GTEXT(sync_exception_aarch64)
GTEXT(irq_aarch64)
GTEXT(fiq_aarch64)
GTEXT(serror_aarch64)

GTEXT(sync_exception_aarch32)
GTEXT(irq_aarch32)
GTEXT(fiq_aarch32)
GTEXT(serror_aarch32)

/*
 * The exception vector table has to be 2 kB aligned as required
 * by ARMv8 architecture.
 */
SECTION_FUNC(vectors,_vector_table)
.align 11

/* Each entry in the vector table has to be 128 B aligned as required
 * by ARMv8 architecture. There are 16 entries in the vector table,
 * this means each entry can have maximum (128 B /4 B) = 32 instructions.
 */

/* Exception from Current EL with SP_EL0 : 0x0 - 0x200 */
	.align 7
sync_exception_sp_el0:
	b  unhandled_exception

	.align 7
irq_sp_el0:
	b	irq_entry

	.align 7
fiq_sp_el0:
	b  unhandled_exception

	.align 7
serror_sp_el0:
	b  unhandled_exception

/* Exception from Current EL with SP_ELx: 0x200 - 0x400 */
	.align 7
sync_exception_sp_elx:
	b  unhandled_exception

	.align 7
irq_sp_elx:
	b  unhandled_exception

	.align 7
fiq_sp_elx:
	b  unhandled_exception

	.align 7
serror_sp_elx:
	b  unhandled_exception

/* Exception from Lower EL using AArch64 : 0x400 - 0x600 */
	.align 7
sync_exception_aarch64:
	b  unhandled_exception

	.align 7
irq_aarch64:
	b  unhandled_exception

	.align 7
fiq_aarch64:
	b  unhandled_exception

	.align 7
serror_aarch64:
	b  unhandled_exception

/* Exception from Lower EL using AArch32 : 0x600 - 0x800 */
	.align 7
sync_exception_aarch32:
	b  unhandled_exception

	.align 7
irq_aarch32:
	b  unhandled_exception

	.align 7
fiq_aarch32:
	b  unhandled_exception

	.align 7
serror_aarch32:
	b  unhandled_exception

.macro exception_entry
	/*
	 * save processor context on to interrupt stack
	 */
	stp x0, x1, [sp, #-CTX_FRAME_SIZE]!
	stp x2, x3, [sp, #CTX_GPREG_X2]
	stp x4, x5, [sp, #CTX_GPREG_X4]
	stp x6, x7, [sp, #CTX_GPREG_X6]
	stp x8, x9, [sp, #CTX_GPREG_X8]
	stp x10, x11, [sp, #CTX_GPREG_X10]
	stp x12, x13, [sp, #CTX_GPREG_X12]
	stp x14, x15, [sp, #CTX_GPREG_X14]
	stp x16, x17, [sp, #CTX_GPREG_X16]
	stp x18, x19, [sp, #CTX_GPREG_X18]
	stp x20, x21, [sp, #CTX_GPREG_X20]
	stp x22, x23, [sp, #CTX_GPREG_X22]
	stp x24, x25, [sp, #CTX_GPREG_X24]
	stp x26, x27, [sp, #CTX_GPREG_X26]
	stp x28, x29, [sp, #CTX_GPREG_X28]
	/* clober x18 or any gpreg */
	mrs x18, sp_el0
	stp x30, x18, [sp, #CTX_GPREG_LR]

	mrs x21, elr_el1
	mrs x22, spsr_el1
	stp x21, x22, [sp, #CTX_GPREG_PC]
	# save the current sp aswell that points to this stack frame
	mov x0, sp
	str x0, [sp, #CTX_GPREG_SP_ELx]
.endm

.macro	exception_exit
	/*
	 * restore processor context on to interrupt stack
	 */
	/* clober x18 which is restored later. */
	ldp x30, x18, [sp, #CTX_GPREG_LR]
	msr sp_el0, x18
	/* restore spsr and elr to support nested irq */
	ldp x21, x22, [sp, #CTX_GPREG_PC]
	msr elr_el1, x21
	msr spsr_el1, x22

	ldp x0, x1, [sp, #CTX_GPREG_X0]
	ldp x2, x3, [sp, #CTX_GPREG_X2]
	ldp x4, x5, [sp, #CTX_GPREG_X4]
	ldp x6, x7, [sp, #CTX_GPREG_X6]
	ldp x8, x9, [sp, #CTX_GPREG_X8]
	ldp x10, x11, [sp, #CTX_GPREG_X10]
	ldp x12, x13, [sp, #CTX_GPREG_X12]
	ldp x14, x15, [sp, #CTX_GPREG_X14]
	ldp x16, x17, [sp, #CTX_GPREG_X16]
	ldp x18, x19, [sp, #CTX_GPREG_X18]
	ldp x20, x21, [sp, #CTX_GPREG_X20]
	ldp x22, x23, [sp, #CTX_GPREG_X22]
	ldp x24, x25, [sp, #CTX_GPREG_X24]
	ldp x26, x27, [sp, #CTX_GPREG_X26]
	ldp x28, x29, [sp, #CTX_GPREG_X28]
	/* restore stack */
	add sp, sp, #CTX_FRAME_SIZE
.endm

SECTION_FUNC(TEXT, irq_entry)
	exception_entry
#ifdef CONFIG_EXECUTION_BENCHMARKING
	bl read_timer_start_of_isr
#endif

#ifdef CONFIG_TRACING
	bl z_sys_trace_isr_enter
#endif

	ldr x19, =_kernel
#ifdef CONFIG_SYS_POWER_MANAGEMENT
	/*
	 * All interrupts are disabled when handling idle wakeup.  For tickless
	 * idle, this ensures that the calculation and programming of the device
	 * for the next timer deadline is not interrupted.For non-tickless idle,
	 * this ensures that the clearing of the kernel idle state is not
	 * interrupted.  In each case, z_sys_power_save_idle_exit is called with
	 * interrupts disabled.
	 */
	/* is this a wakeup from idle ? */
	/* requested idle duration, in ticks */
	ldr x3, [x19, #_kernel_offset_to_idle]
	cmp x3, #0

	beq _idle_state_cleared
	movs x1, #0
	/* clear kernel idle state */
	str x1, [x19, #_kernel_offset_to_idle]
	bl z_sys_power_save_idle_exit
_idle_state_cleared:
#endif
	/* _kernel.nested++ */
	ldr x1, [x19, #_kernel_offset_to_nested]
	add x1, x1, #1
	str x1, [x19, #_kernel_offset_to_nested]

	ldr x1, intr_handler
	blr x1

	ldr x19, =_kernel
	/* _kernel.nested-- */
	ldr x1, [x19, #_kernel_offset_to_nested]
	sub x1, x1, #1
	str x1, [x19, #_kernel_offset_to_nested]

#ifdef CONFIG_EXECUTION_BENCHMARKING
	bl read_timer_end_of_isr
#endif /* CONFIG_EXECUTION_BENCHMARKING */

#ifdef CONFIG_TRACING
	bl z_sys_trace_isr_exit
#endif
	exception_exit
	/*
	 * pc and pstate are restored implicitly
	 * from elr_el1 and spsr_el1
	 */
	eret
