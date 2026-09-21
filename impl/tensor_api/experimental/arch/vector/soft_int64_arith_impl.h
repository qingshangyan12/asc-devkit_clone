/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SOFT_INT64_ARITH_IMPL_H
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_SOFT_INT64_ARITH_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_SOFT_INT64_ARITH_IMPL_H

#include "impl/tensor_api/experimental/arch/utils/reg_utils.h"

namespace asc {
namespace te {
namespace experimental {
namespace detail {

/**
 * @brief Soft B64 add aligned with basic_api AddB64Impl (int64 branch).
 * @details lo: vaddc produces carry; hi: vaddcs consumes that carry.
 *          Uses sticky @c src0.mask for both halves and syncs dst / lo / hi masks.
 */
__simd_callee__ inline reg_tensor<int64_t> soft_int64_add(
    const reg_tensor<int64_t>& src0, const reg_tensor<int64_t>& src1)
{
    reg_tensor<int64_t> dst;
    reg_tensor<bool> carry;
    reg_tensor<bool> carry_out;
    // Align AddB64Impl: Add(carry, lo) then AddC(carry_out, hi, carry_src=carry).
    asc_add(carry.reg, dst.lo.reg, src0.lo.reg, src1.lo.reg, src0.mask);
    asc_addc(carry_out.reg, dst.hi.reg, src0.hi.reg, src1.hi.reg, carry.reg, src0.mask);
    (void)carry_out;
    dst.mask = src0.mask;
    dst.lo.mask = src0.mask;
    dst.hi.mask = src0.mask;
    return dst;
}

} // namespace detail
} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_SOFT_INT64_ARITH_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SOFT_INT64_ARITH_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SOFT_INT64_ARITH_IMPL_H
#endif
