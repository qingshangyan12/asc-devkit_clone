/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_BIT_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_BIT_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/scalar_bit_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_BIT_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/common_impl.h"

__aicore__ inline int64_t asc_clz_impl(uint64_t value_in) { return clz(value_in); }

__aicore__ inline int64_t asc_ffs_impl(uint64_t value) { return sff1(value); }

__aicore__ inline int64_t asc_sflbits_impl(int64_t value) { return sflbits(value); }

__aicore__ inline int64_t asc_zero_bits_cnt_impl(uint64_t value) { return bcnt0(value); }

__aicore__ inline int64_t asc_ffz_impl(uint64_t value) { return sff0(value); }

__aicore__ inline int64_t asc_popc_impl(uint64_t value) { return bcnt1(value); }

__aicore__ inline uint64_t asc_set_nthbit_impl(uint64_t bits, int64_t idx) { return sbitset1(bits, idx); }

__aicore__ inline uint64_t asc_clear_nthbit_impl(uint64_t bits, int64_t idx) { return sbitset0(bits, idx); }

__aicore__ inline int64_t asc_clz(uint64_t value_in) { return asc_clz_impl(value_in); }

__aicore__ inline int64_t asc_ffs(uint64_t value) { return asc_ffs_impl(value); }

__aicore__ inline int64_t asc_sflbits(int64_t value) { return asc_sflbits_impl(value); }

__aicore__ inline int64_t asc_zero_bits_cnt(uint64_t value) { return asc_zero_bits_cnt_impl(value); }

__aicore__ inline int64_t asc_ffz(uint64_t value) { return asc_ffz_impl(value); }

__aicore__ inline int64_t asc_popc(uint64_t value) { return asc_popc_impl(value); }

// ==========asc_set_nthbit==========
__aicore__ inline uint64_t asc_set_nthbit(uint64_t bits, int64_t idx) { return asc_set_nthbit_impl(bits, idx); }

__aicore__ inline uint64_t asc_clear_nthbit(uint64_t bits, int64_t idx) { return asc_clear_nthbit_impl(bits, idx); }

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_BIT_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_BIT_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_BIT_INTF_IMPL_H
