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
    "impl/tensor_api/experimental/reg/mask_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_MASK
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_REG_MASK_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_REG_MASK_IMPL_H

#include "impl/tensor_api/experimental/reg/reg_tensor_impl.h"

namespace asc {
namespace te {

enum class mask_pattern {
    all,
    vl1,
    vl2,
    vl3,
    vl4,
    vl8,
    vl16,
    vl32,
    vl64,
    vl128,
    every3,
    every4,
    half,
    quarter,
    none,
};

template <mask_pattern Pattern = mask_pattern::all, typename DataType>
__simd_callee__ reg_tensor<bool> make_mask();

template <typename DataType>
__simd_callee__ reg_tensor<bool> ones();

template <typename DataType>
__simd_callee__ reg_tensor<bool> zeros();

template <typename DataType>
__simd_callee__ reg_tensor<bool> update_mask(uint32_t& remain);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

namespace detail {

template <mask_pattern Pattern>
__simd_callee__ inline constexpr auto get_mask_pattern()
{
    if constexpr (Pattern == mask_pattern::all) {
        return PAT_ALL;
    } else if constexpr (Pattern == mask_pattern::vl1) {
        return PAT_VL1;
    } else if constexpr (Pattern == mask_pattern::vl2) {
        return PAT_VL2;
    } else if constexpr (Pattern == mask_pattern::vl3) {
        return PAT_VL3;
    } else if constexpr (Pattern == mask_pattern::vl4) {
        return PAT_VL4;
    } else if constexpr (Pattern == mask_pattern::vl8) {
        return PAT_VL8;
    } else if constexpr (Pattern == mask_pattern::vl16) {
        return PAT_VL16;
    } else if constexpr (Pattern == mask_pattern::vl32) {
        return PAT_VL32;
    } else if constexpr (Pattern == mask_pattern::vl64) {
        return PAT_VL64;
    } else if constexpr (Pattern == mask_pattern::vl128) {
        return PAT_VL128;
    } else if constexpr (Pattern == mask_pattern::every3) {
        return PAT_M3;
    } else if constexpr (Pattern == mask_pattern::every4) {
        return PAT_M4;
    } else if constexpr (Pattern == mask_pattern::half) {
        return PAT_H;
    } else if constexpr (Pattern == mask_pattern::quarter) {
        return PAT_Q;
    } else {
        static_assert(Pattern == mask_pattern::none, "unsupported mask pattern");
        return PAT_ALLF;
    }
}

template <typename DataType>
inline constexpr bool is_supported_mask_element_type =
    sizeof(DataType) == sizeof(uint8_t) || sizeof(DataType) == sizeof(uint16_t) ||
    sizeof(DataType) == sizeof(uint32_t);

template <typename DataType, typename PatternType>
__simd_callee__ inline reg_tensor<bool> make_mask(PatternType pattern)
{
    static_assert(is_supported_mask_element_type<DataType>,
        "mask operations only support 8-bit, 16-bit, and 32-bit element types");

    reg_tensor<bool> result;
    if constexpr (sizeof(DataType) == sizeof(uint32_t)) {
        result.reg = asc_create_mask_b32(pattern);
    } else if constexpr (sizeof(DataType) == sizeof(uint16_t)) {
        result.reg = asc_create_mask_b16(pattern);
    } else {
        result.reg = asc_create_mask_b8(pattern);
    }
    return result;
}

} // namespace detail

template <mask_pattern Pattern, typename DataType>
__simd_callee__ inline reg_tensor<bool> make_mask()
{
    return detail::make_mask<DataType>(detail::get_mask_pattern<Pattern>());
}

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> ones()
{
    return make_mask<mask_pattern::all, DataType>();
}

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> zeros()
{
    return make_mask<mask_pattern::none, DataType>();
}

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> update_mask(uint32_t& remain)
{
    static_assert(detail::is_supported_mask_element_type<DataType>,
        "update_mask only supports 8-bit, 16-bit, and 32-bit element types");

    reg_tensor<bool> result;
    if constexpr (sizeof(DataType) == sizeof(uint32_t)) {
        result.reg = asc_update_mask_b32(remain);
    } else if constexpr (sizeof(DataType) == sizeof(uint16_t)) {
        result.reg = asc_update_mask_b16(remain);
    } else {
        result.reg = asc_update_mask_b8(remain);
    }
    return result;
}

#endif

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_REG_MASK_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_MASK)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_MASK
#endif
