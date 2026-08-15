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
    "impl/tensor_api/experimental/reg/data_reorder_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DATA_REORDER_IMPL_H
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_VECTOR_DATA_REORDER_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_VECTOR_DATA_REORDER_IMPL_H

#include <cstdint>

namespace asc {
namespace te {
namespace detail {
//TODO: support uint64_t/int64_t
inline constexpr bool is_data_reorder_support_type = Std::is_one_of_v<T, uint8_t, int8_t,
    fp8_e4m3fn_t, fp8_e8m0_t, fp8_e5m2_t, uint16_t, int16_t, half, bfloat16_t, uint32_t,
    int32_t, float>;
}

template <typename T>
__simd_callee__ inline decltype(auto) interleave(reg_tensor<T> src0, reg_tensor<T> src1) {
    static_assert(detail::is_data_reorder_support_type<T>, "interleave only supports uint8_t, int8_t,
        fp8_e4m3fn_t, fp8_e8m0_t, fp8_e5m2_t, uint16_t, int16_t, half, bfloat16_t, uint32_t,
        int32_t and float.");
    reg_tensor<T> dst0;
    reg_tensor<T> dst1;    
    asc_intlv(dst0.reg, dst1.reg, src0.reg, src1.reg);
    //dst0.mask = with_mask(all_mask<uint8_t>());;
    //dst1.mask = dst0.mask;
    return [dst0, dst1];
}

template <typename T>
__simd_callee__ inline decltype(auto) deinterleave(reg_tensor<T> src0, reg_tensor<T> src1) {
    static_assert(detail::is_data_reorder_support_type<T>, "deinterleave only supports uint8_t, int8_t,
        fp8_e4m3fn_t, fp8_e8m0_t, fp8_e5m2_t, uint16_t, int16_t, half, bfloat16_t, uint32_t,
        int32_t and float.");
    reg_tensor<T> dst0;
    reg_tensor<T> dst1;    
    asc_deintlv(dst0.reg, dst1.reg, src0.reg, src1.reg);
    //dst0.mask = with_mask(all_mask<uint8_t>());;
    //dst1.mask = dst0.mask;
    return [dst0, dst1];
}
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_REG_DATA_REORDER_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DATA_REORDER_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DATA_REORDER_IMPL_H
#endif
