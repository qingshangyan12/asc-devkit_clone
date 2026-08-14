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
#warning                                                                                                               \
    "impl/tensor_api/arch/vector/experimental/composite_compute_impl.h is internal and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE_IMPL
#endif

#ifndef IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_COMPOSITE_COMPUTE_IMPL_H
#define IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_COMPOSITE_COMPUTE_IMPL_H

#include "impl/tensor_api/arch/utils/reg_compute_traits.h"

namespace asc {
namespace te {

template <typename T, typename scalar_type>
__simd_callee__ inline decltype(auto) axpy(reg_tensor<T> dst, reg_tensor<T> src, scalar_type scalar)
{
    static_assert(detail::supports_axpy_v<T>, "axpy supports half and float");
    static_assert(::AscendC::Std::is_same_v<::AscendC::Std::remove_cvref_t<scalar_type>, T>,
        "axpy requires the scalar and reg_tensor element types to match");
    asc_axpy(dst.reg, src.reg, scalar, dst.mask);
    return dst;
}

template <typename T>
__simd_callee__ inline decltype(auto) abs_sub(reg_tensor<T> src0, reg_tensor<T> src1)
{
    static_assert(detail::supports_abs_sub_v<T>, "abs_sub supports half and float");
    reg_tensor<T> dst;
    asc_abs_sub(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <reg_layout layout, typename T>
__simd_callee__ inline decltype(auto) exp_sub(reg_tensor<T> src0, reg_tensor<T> src1)
{
    static_assert(detail::supports_exp_sub_v<T>, "exp_sub supports half and float");
    static_assert(layout == reg_layout::zero, "exp_sub only supports reg_layout::zero on the current device");
    reg_tensor<float> dst;
    // asc_exp_sub returns float for both half and float inputs.
    asc_exp_sub(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T, typename U>
__simd_callee__ inline decltype(auto) mul_dst_add(
    reg_tensor<T> dst, reg_tensor<U> src0, reg_tensor<U> src1)
{
    static_assert(detail::supports_mul_dst_add_v<T, U>,
        "mul_dst_add requires matching half or float element types");
    asc_madd(dst.reg, src0.reg, src1.reg, dst.mask);
    return dst;
}

template <typename T>
__simd_callee__ inline decltype(auto) mul_add_dst(
    reg_tensor<T> dst, reg_tensor<T> src0, reg_tensor<T> src1)
{
    static_assert(detail::supports_mul_add_dst_v<T>, "mul_add_dst supports half and float");
    reg_tensor<T> result = src0;
    asc_madd(result.reg, src1.reg, dst.reg, dst.mask);
    result.mask = dst.mask;
    return result;
}

template <reg_layout layout>
__simd_callee__ inline decltype(auto) muls_cast(reg_tensor<float> src, float scalar)
{
    static_assert(layout == reg_layout::zero, "muls_cast only supports reg_layout::zero");
    reg_tensor<half> dst;
    asc_muls(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_COMPOSITE_COMPUTE_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE_IMPL)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE_IMPL
#endif
