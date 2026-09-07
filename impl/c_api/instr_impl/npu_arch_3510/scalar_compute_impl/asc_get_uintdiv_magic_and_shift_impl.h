/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_SCALAR_COMPUTE_IMPL_ASC_GET_UINTDIV_MAGIC_AND_SHIFT_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_SCALAR_COMPUTE_IMPL_ASC_GET_UINTDIV_MAGIC_AND_SHIFT_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/utils_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_clz_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_popc_impl.h"

__aicore__ inline uint64_t asc_get_uintdiv_magic_impl(uint64_t dividend, uint64_t divisor)
{
    uint64_t quotient = 0;
    uint64_t remainder = dividend;
    for (int i = 0; i < 64; ++i) {
        quotient <<= 1;
        const uint64_t borrow = remainder >> 63;
        remainder <<= 1;
        if (borrow != 0) {
            remainder -= divisor;
            quotient |= 1;
        } else if (remainder >= divisor) {
            remainder -= divisor;
            quotient |= 1;
        }
    }
    return quotient + 1;
}

__aicore__ inline void asc_get_uintdiv_magic_and_shift_impl(uint32_t* magic, uint32_t* shift, uint32_t divisor)
{
    const int64_t pos = 64 - asc_clz_impl(static_cast<uint64_t>(divisor));
    const int64_t cnt1 = asc_popc_impl(static_cast<uint64_t>(divisor));
    *shift = cnt1 == 1 ? pos - 1 : pos;
    *magic = (1ULL << 32) * ((1ULL << *shift) - divisor) / divisor + 1;
}

__aicore__ inline void asc_get_uintdiv_magic_and_shift_impl(uint64_t* magic, uint64_t* shift, uint64_t divisor)
{
    const int64_t pos = 64 - asc_clz_impl(divisor);
    const int64_t cnt1 = asc_popc_impl(divisor);
    *shift = cnt1 == 1 ? pos - 1 : pos;
    uint64_t dividend = 0;
    if (*shift < 64) {
        dividend = (1ULL << *shift) - divisor;
    } else {
        dividend = UINT64_MAX - divisor + 1;
    }
    *magic = asc_get_uintdiv_magic_impl(dividend, divisor);
}

#endif
