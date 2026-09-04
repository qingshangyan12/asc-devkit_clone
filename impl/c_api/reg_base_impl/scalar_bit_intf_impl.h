/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/c_api/reg_base_impl/scalar_bit_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_SCALAR_BIT_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_SCALAR_BIT_INTF_IMPL_H

__aicore__ inline int64_t asc_clz(uint64_t value_in) { return clz(value_in); }

__aicore__ inline int64_t asc_zero_bits_cnt(uint64_t value) { return bcnt0(value); }

__aicore__ inline int64_t asc_ffs(uint64_t value) { return sff1(value); }

__aicore__ inline int64_t asc_popc(uint64_t value) { return bcnt1(value); }

__aicore__ inline int64_t asc_sflbits(int64_t value) { return sflbits(value); }

__aicore__ inline int64_t asc_ffz(uint64_t value) { return sff0(value); }

__aicore__ inline uint64_t asc_clear_nthbit(uint64_t bits, int64_t idx) { return sbitset0(bits, idx); }

__aicore__ inline uint64_t asc_set_nthbit(uint64_t bits, int64_t idx) { return sbitset1(bits, idx); }

__aicore__ inline uint64_t asc_get_uintdiv_magic_impl(uint64_t dividend, uint64_t divisor)
{
    uint64_t quotient = 0;
    uint64_t remainder = dividend;
    for (int i = 0; i < 64; ++i) { // 64: process every bit of the uint64_t dividend.
        quotient <<= 1;
        const uint64_t borrow = remainder >> 63; // 63: highest bit index of uint64_t.
        remainder <<= 1;
        if (borrow != 0) {
            remainder -= divisor;
            quotient |= 1; // 1: set the current quotient bit.
        } else if (remainder >= divisor) {
            remainder -= divisor;
            quotient |= 1; // 1: set the current quotient bit.
        }
    }
    return quotient + 1; // 1: round the reciprocal up for exact division.
}

__aicore__ inline void asc_get_uintdiv_magic_and_shift(uint32_t* magic, uint32_t* shift, uint32_t divisor)
{
    const int64_t pos = 64 - asc_clz(static_cast<uint64_t>(divisor)); // 64: width of the uint64_t clz input.
    const int64_t cnt1 = asc_popc(static_cast<uint64_t>(divisor));
    *shift = cnt1 == 1 ? pos - 1 : pos;
    *magic = (1ULL << 32) * ((1ULL << *shift) - divisor) / divisor + 1; // 32: uint32_t bit width.
}

__aicore__ inline void asc_get_uintdiv_magic_and_shift(uint64_t* magic, uint64_t* shift, uint64_t divisor)
{
    const int64_t pos = 64 - asc_clz(divisor); // 64: uint64_t bit width.
    const int64_t cnt1 = asc_popc(divisor);
    *shift = cnt1 == 1 ? pos - 1 : pos;
    uint64_t dividend = 0;
    if (*shift < 64) { // 64: one-past-the-highest valid uint64_t shift.
        dividend = (1ULL << *shift) - divisor;
    } else {
        dividend = UINT64_MAX - divisor + 1; // 1: two's-complement form of 2^64 - divisor.
    }
    *magic = asc_get_uintdiv_magic_impl(dividend, divisor);
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
