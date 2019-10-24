#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_BARRIER_H
#define ZEPHYR_INCLUDE_ARCH_ARM64_BARRIER_H

#ifndef _ASMLANGUAGE

#define __ISB()	__asm__ volatile ("isb sy" : : : "memory")
#define __DMB()	__asm__ volatile ("dmb sy" : : : "memory")

#endif	/* _ASMLANGUAGE */

#endif	/* ZEPHYR_INCLUDE_ARCH_ARM64_BARRIER_H */
