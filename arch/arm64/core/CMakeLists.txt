# SPDX-License-Identifier: Apache-2.0

zephyr_library()

zephyr_library_sources(
  reset.S
  prep_c.c
  vector_table.S
  thread.c
  reboot.c
  fatal.c
  cpu_idle.S
  swap.c
  swap_helper.S
  irq_manage.c
  )
