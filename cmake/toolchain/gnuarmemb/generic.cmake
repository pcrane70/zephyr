# SPDX-License-Identifier: Apache-2.0

# Until we completely deprecate it
if(NOT DEFINED ENV{GNUARMEMB_TOOLCHAIN_PATH})
  if(DEFINED ENV{GCCARMEMB_TOOLCHAIN_PATH})
    message(WARNING "GCCARMEMB_TOOLCHAIN_PATH is deprecated, please use GNUARMEMB_TOOLCHAIN_PATH instead")
    set(GNUARMEMB_TOOLCHAIN_PATH $ENV{GCCARMEMB_TOOLCHAIN_PATH})
  endif()
endif()

set_ifndef(GNUARMEMB_TOOLCHAIN_PATH "$ENV{GNUARMEMB_TOOLCHAIN_PATH}")
set(       GNUARMEMB_TOOLCHAIN_PATH ${GNUARMEMB_TOOLCHAIN_PATH} CACHE PATH "gnuarmemb install directory")
assert(    GNUARMEMB_TOOLCHAIN_PATH "GNUARMEMB_TOOLCHAIN_PATH is not set")

if(NOT EXISTS ${GNUARMEMB_TOOLCHAIN_PATH})
  message(FATAL_ERROR "Nothing found at GNUARMEMB_TOOLCHAIN_PATH: '${GNUARMEMB_TOOLCHAIN_PATH}'")
endif()

set(TOOLCHAIN_HOME ${GNUARMEMB_TOOLCHAIN_PATH})

set(COMPILER gcc)
set(LINKER ld)
set(BINTOOLS gnu)

if("${ARCH}" STREQUAL "arm")
set(CROSS_COMPILE_TARGET arm-none-eabi)
set(SYSROOT_TARGET       arm-none-eabi)
elseif("${ARCH}" STREQUAL "arm64")
set(CROSS_COMPILE_TARGET aarch64-linux-gnu)
set(SYSROOT_TARGET       aarch64-linux-gnu)
else()
  message(WARNING "gnuarmemb toolchain does not support ${ARCH}!")
endif()

set(CROSS_COMPILE ${TOOLCHAIN_HOME}/bin/${CROSS_COMPILE_TARGET}-)
set(SYSROOT_DIR   ${TOOLCHAIN_HOME}/${SYSROOT_TARGET})
set(TOOLCHAIN_HAS_NEWLIB ON CACHE BOOL "True if toolchain supports newlib")
