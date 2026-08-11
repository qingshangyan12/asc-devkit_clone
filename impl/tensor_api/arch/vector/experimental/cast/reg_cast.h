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
    "reg_cast.h is internal; include <tensor_api/arch/vector/experimental/reg_cast.h> instead."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_CAST
#endif

#ifndef IMPL_TENSOR_API_ARCH_VECTOR_REG_CAST_H
#define IMPL_TENSOR_API_ARCH_VECTOR_REG_CAST_H

#if !defined(INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_REG_CAST_H)
#include "tensor_api/arch/vector/experimental/reg_cast.h"
#endif
#include "c_api/reg_compute/reg_convert.h"
#include "impl/tensor_api/arch/utils/arch_utils.h"

namespace asc {
namespace te {

namespace reg_cast_detail {

template <typename To, typename From, cast_layout Layout, cast_round_mode Round, cast_sat_mode Sat>
struct reg_cast_op {
    static constexpr bool supported = false;
};

#define ASC_REG_CAST_OP(TO, FROM, LAYOUT, ROUND, SAT, FUNC)                                                           \
    template <>                                                                                                       \
    struct reg_cast_op<TO, FROM, cast_layout::LAYOUT, cast_round_mode::ROUND, cast_sat_mode::SAT> {                  \
        static constexpr bool supported = true;                                                                       \
                                                                                                                       \
        template <typename DstReg, typename SrcReg>                                                                   \
        __simd_callee__ static void run(DstReg& dst, SrcReg src, vector_bool mask)                                   \
        {                                                                                                              \
            FUNC(dst, src, mask);                                                                                      \
        }                                                                                                              \
    }

#include "impl/tensor_api/arch/vector/experimental/cast/reg_cast_table.h"

#undef ASC_REG_CAST_OP

template <typename To, cast_layout Layout, cast_round_mode Round, cast_sat_mode Sat, typename From>
__simd_callee__ inline reg_tensor<To> cast_impl(reg_tensor<From> src)
{
    static_assert(!AscendC::Std::is_same_v<To, From>,
        "reg_tensor cast requires different source and destination element types");

    using op = reg_cast_op<To, From, Layout, Round, Sat>;
    static_assert(op::supported,
        "register cast type or option combination is unsupported on the current architecture");

    reg_tensor<To> dst;
    if constexpr (op::supported) {
        op::run(dst.reg, src.reg, src.mask);
    }
    dst.mask = src.mask;
    return dst;
}

} // namespace reg_cast_detail

#if __cplusplus >= 202002L
template <typename To, cast_options Options, typename From>
__simd_callee__ inline reg_tensor<To> cast(reg_tensor<From> src)
{
    return reg_cast_detail::cast_impl<To, Options.layout, Options.round, Options.sat>(src);
}
#else
template <typename To, cast_layout Layout, cast_round_mode Round, cast_sat_mode Sat, typename From>
__simd_callee__ inline reg_tensor<To> cast(reg_tensor<From> src)
{
    return reg_cast_detail::cast_impl<To, Layout, Round, Sat>(src);
}
#endif

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_REG_CAST_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_CAST)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_CAST
#endif
