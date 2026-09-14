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
#warning "impl/tensor_api/experimental/arch/vector/composite_compute_impl.h is internal and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE_IMPL
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPOSITE_COMPUTE_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPOSITE_COMPUTE_IMPL_H

#include "impl/tensor_api/experimental/arch/utils/reg_utils.h"

namespace asc {
namespace te {
namespace experimental {

template <typename T>
__simd_callee__ inline reg_tensor<T> axpy(const reg_tensor<T>& dst, const reg_tensor<T>& src, const T& scalar)
{
    static_assert(detail::supports_axpy_v<T>, "axpy does not support this element type");
    reg_tensor<T> result = dst;
    asc_axpy(result.reg, src.reg, scalar, dst.mask);
    return result;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> abs_diff(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_abs_diff_v<T>, "abs_diff does not support this element type");
    reg_tensor<T> dst;
    asc_abs_sub(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<float> exp_diff(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_exp_diff_v<T>, "exp_diff does not support this element type");
    if constexpr (::AscendC::Std::is_same_v<T, half>) {
        return exp_diff(src0, src1, ASC_POSITION_EVEN);
    } else {
        reg_tensor<float> dst(src0.mask);
        dst.reg = asc_exp_sub(src0.reg, src1.reg, src0.mask);
        return dst;
    }
}

template <typename position_type>
__simd_callee__ inline reg_tensor<float> exp_diff(
    const reg_tensor<half>& src0, const reg_tensor<half>& src1, position_type src_pos)
{
    using pos_type = ::AscendC::Std::remove_cvref_t<position_type>;
    static_assert(
        ::AscendC::Std::is_one_of_v<
            pos_type, ::AscendC::Std::remove_cvref_t<decltype(ASC_POSITION_EVEN)>,
            ::AscendC::Std::remove_cvref_t<decltype(ASC_POSITION_ODD)>>,
        "exp_diff position supports ASC_POSITION_EVEN and ASC_POSITION_ODD");
    reg_tensor<float> dst(src0.mask);
    dst.reg = asc_exp_sub_half2float(src0.reg, src1.reg, src0.mask, src_pos);
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> madd(
    const reg_tensor<T>& dst, const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_madd_v<T>, "madd does not support this element type");
    reg_tensor<T> result = dst;
    asc_madd(result.reg, src0.reg, src1.reg, dst.mask);
    return result;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> mula(
    const reg_tensor<T>& dst, const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_mula_v<T>, "mula does not support this element type");
    reg_tensor<T> result = dst;
    asc_mula(result.reg, src0.reg, src1.reg, dst.mask);
    return result;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> fma(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1, const reg_tensor<T>& src2)
{
    static_assert(detail::supports_fma_v<T>, "fma does not support this element type");
    reg_tensor<T> dst;
    dst.reg = asc_fma(src0.reg, src1.reg, src2.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T, typename U, cast_layout Layout>
__simd_callee__ inline reg_tensor<T> muls_cast(const reg_tensor<U>& src, const U& scalar)
{
    static_assert(detail::supports_muls_cast_v<T, U>, "muls_cast supports only float to half");
    static_assert(Layout == cast_layout::zero, "muls_cast layout supports only cast_layout::zero");
    reg_tensor<T> dst(src.mask);
    dst.reg = asc_mul_scalar_float2half_rn(src.reg, scalar, src.mask, ASC_POSITION_EVEN);
    return dst;
}

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPOSITE_COMPUTE_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE_IMPL)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_COMPUTE_IMPL
#endif
