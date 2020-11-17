/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x0,        LENGTH = 0x40000
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 0x4000
}

INCLUDE "nrf_sdk10/components/toolchain/gcc/gcc_nrf51_common.ld"
