# SPDX-License-Identifier: Apache-2.0

# TODO: remove "+nofp" when floating point support is added
zephyr_compile_options(
  -march=armv8-a+nofp
  )

add_subdirectory(core)
