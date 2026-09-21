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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BITCAST_IMPL_H
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BITCAST_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BITCAST_IMPL_H

#include "impl/tensor_api/experimental/arch/vector/type_get.h"

namespace asc {
namespace te {
namespace experimental {
namespace detail {

template <typename DstType, typename SrcType>
__simd_callee__ inline void assert_same_width_bitcast()
{
    using D = AscendC::Std::remove_cvref_t<DstType>;
    using S = AscendC::Std::remove_cvref_t<SrcType>;
    using DstReg = typename type_get<D>::type;
    using SrcReg = typename type_get<S>::type;

    static_assert(
        !AscendC::Std::is_same_v<D, bool> && !AscendC::Std::is_same_v<S, bool>,
        "bitcast: bool element type not supported");
    static_assert(sizeof(D) == sizeof(S), "bitcast: element sizeof must match");
    static_assert(sizeof(DstReg) == sizeof(SrcReg), "bitcast: underlying reg sizeof must match");
}

template <typename DstReg, typename SrcReg>
__simd_callee__ inline DstReg pun_reg(const SrcReg& src)
{
    if constexpr (AscendC::Std::is_same_v<DstReg, SrcReg>) {
        return src;
    } else {
        union {
            SrcReg s;
            DstReg d;
        } u{};
        u.s = src;
        return u.d;
    }
}

} // namespace detail

template <typename DstType, typename SrcType>
__simd_callee__ inline reg_tensor<DstType> bitcast(const reg_tensor<SrcType>& src)
{
    detail::assert_same_width_bitcast<DstType, SrcType>();

    using D = AscendC::Std::remove_cvref_t<DstType>;
    using S = AscendC::Std::remove_cvref_t<SrcType>;
    using DstReg = typename detail::type_get<D>::type;
    using SrcReg = typename detail::type_get<S>::type;

    reg_tensor<D> out;
    out.reg = detail::pun_reg<DstReg, SrcReg>(src.reg);
    out.mask = src.mask;
    return out;
}

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BITCAST_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BITCAST_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BITCAST_IMPL_H
#endif
