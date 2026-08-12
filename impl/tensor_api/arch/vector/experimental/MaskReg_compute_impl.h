/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_MASK_REG_COMPUTE_IMPL_H
#define IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_MASK_REG_COMPUTE_IMPL_H

#include "tensor_api/arch/vector/experimental/MaskReg_compute.h"

namespace asc {
namespace te {
namespace detail {

template <typename DataType, auto Pattern>
__simd_callee__ inline reg_tensor<bool> make_reg_mask()
{
    static_assert(sizeof(DataType) == sizeof(uint8_t) || sizeof(DataType) == sizeof(uint16_t)
                      || sizeof(DataType) == sizeof(uint32_t),
                  "mask helpers only support 8-bit, 16-bit, and 32-bit element types");

    reg_tensor<bool> result;
    if constexpr (sizeof(DataType) == sizeof(uint32_t)) {
        result.reg = asc_create_mask_b32(Pattern);
    } else if constexpr (sizeof(DataType) == sizeof(uint16_t)) {
        result.reg = asc_create_mask_b16(Pattern);
    } else if constexpr (sizeof(DataType) == sizeof(uint8_t)) {
        result.reg = asc_create_mask_b8(Pattern);
    }
    return result;
}

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> make_updated_reg_mask(uint32_t& remain)
{
    static_assert(sizeof(DataType) == sizeof(uint8_t) || sizeof(DataType) == sizeof(uint16_t)
                      || sizeof(DataType) == sizeof(uint32_t),
                  "update_mask only supports 8-bit, 16-bit, and 32-bit element types");

    reg_tensor<bool> result;
    if constexpr (sizeof(DataType) == sizeof(uint32_t)) {
        result.reg = asc_update_mask_b32(remain);
    } else if constexpr (sizeof(DataType) == sizeof(uint16_t)) {
        result.reg = asc_update_mask_b16(remain);
    } else if constexpr (sizeof(DataType) == sizeof(uint8_t)) {
        result.reg = asc_update_mask_b8(remain);
    }
    return result;
}

template <std::size_t N, typename DataType>
__simd_callee__ inline reg_tensor<DataType> duplicate_scalar(DataType value, reg_tensor<bool> mask)
{
    static_assert(N == 1, "fill currently supports only one register result");

    reg_tensor<DataType> result(mask.reg);
    asc_duplicate_scalar(result.reg, value, mask.reg);
    return result;
}

template <std::size_t N, typename DataType>
__simd_callee__ inline reg_tensor<DataType> duplicate_register(reg_tensor<DataType> value, reg_tensor<bool> mask)
{
    static_assert(N == 1, "fill currently supports only one register result");

    reg_tensor<DataType> result(mask.reg);
    asc_duplicate(result.reg, value.reg, mask.reg);
    return result;
}

} // namespace detail

template <mask_pattern Pattern, typename DataType>
__simd_callee__ inline reg_tensor<bool> make_mask()
{
    if constexpr (Pattern == mask_pattern::all) {
        return detail::make_reg_mask<DataType, PAT_ALL>();
    } else if constexpr (Pattern == mask_pattern::vl1) {
        return detail::make_reg_mask<DataType, PAT_VL1>();
    } else if constexpr (Pattern == mask_pattern::vl2) {
        return detail::make_reg_mask<DataType, PAT_VL2>();
    } else if constexpr (Pattern == mask_pattern::vl3) {
        return detail::make_reg_mask<DataType, PAT_VL3>();
    } else if constexpr (Pattern == mask_pattern::vl4) {
        return detail::make_reg_mask<DataType, PAT_VL4>();
    } else if constexpr (Pattern == mask_pattern::vl8) {
        return detail::make_reg_mask<DataType, PAT_VL8>();
    } else if constexpr (Pattern == mask_pattern::vl16) {
        return detail::make_reg_mask<DataType, PAT_VL16>();
    } else if constexpr (Pattern == mask_pattern::vl32) {
        return detail::make_reg_mask<DataType, PAT_VL32>();
    } else if constexpr (Pattern == mask_pattern::vl64) {
        return detail::make_reg_mask<DataType, PAT_VL64>();
    } else if constexpr (Pattern == mask_pattern::vl128) {
        return detail::make_reg_mask<DataType, PAT_VL128>();
    } else if constexpr (Pattern == mask_pattern::every3) {
        return detail::make_reg_mask<DataType, PAT_M3>();
    } else if constexpr (Pattern == mask_pattern::every4) {
        return detail::make_reg_mask<DataType, PAT_M4>();
    } else if constexpr (Pattern == mask_pattern::half) {
        return detail::make_reg_mask<DataType, PAT_H>();
    } else if constexpr (Pattern == mask_pattern::quarter) {
        return detail::make_reg_mask<DataType, PAT_Q>();
    } else if constexpr (Pattern == mask_pattern::none) {
        return detail::make_reg_mask<DataType, PAT_ALLF>();
    } else {
        static_assert(Pattern == mask_pattern::all, "unsupported mask pattern");
    }
}

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> all_mask()
{
    return make_mask<mask_pattern::all, DataType>();
}

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> none_mask()
{
    return make_mask<mask_pattern::none, DataType>();
}

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> update_mask(uint32_t& remain)
{
    return detail::make_updated_reg_mask<DataType>(remain);
}

template <std::size_t N, typename DataType>
__simd_callee__ inline reg_tensor<DataType> fill(DataType value, reg_tensor<bool> mask)
{
    return detail::duplicate_scalar<N>(value, mask);
}

template <std::size_t N, typename DataType>
__simd_callee__ inline reg_tensor<DataType> fill(reg_tensor<DataType> value, reg_tensor<bool> mask)
{
    return detail::duplicate_register<N>(value, mask);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_MASK_REG_COMPUTE_IMPL_H
