/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_MASK_IMPL_H
#define IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_MASK_IMPL_H

#include "tensor_api/arch/vector/experimental/mask.h"

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

} // namespace detail

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> all_mask()
{
    return detail::make_reg_mask<DataType, PAT_ALL>();
}

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> none_mask()
{
    return detail::make_reg_mask<DataType, PAT_ALLF>();
}

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> update_mask(uint32_t& remain)
{
    return detail::make_updated_reg_mask<DataType>(remain);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_MASK_IMPL_H
