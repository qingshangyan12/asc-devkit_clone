/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_REG_LOAD_STORE_H
#define IMPL_TENSOR_API_EXPERIMENTAL_REG_LOAD_STORE_H

#include "impl/tensor_api/tensor/tensor_impl.h"

namespace asc {
namespace te {
namespace detail {

template <load_sideband_mode sideband_mode, typename TensorType, typename Coord>
__simd_callee__ inline decltype(auto) load_reg_tensor(const TensorType& tensor, const Coord& coord)
{
    using engine_type = typename TensorType::engine_type;
    using data_type = typename TensorType::data_type;
    static_assert(Std::is_same_v<get_mem_location<engine_type>, location::ub>,
                  "base_tensor::load only supports tensors located in UB");

    reg_tensor<data_type> dst;
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    auto src_engine = tensor.engine() + tensor.layout()(coord);
    auto src = src_engine.begin().get();
    if constexpr (sideband_mode == load_sideband_mode::direct) {
        asc_loadalign(dst.reg, src);
    }
#endif
    return dst;
}

template <typename TensorType, typename Coord, typename RegDataType>
__simd_callee__ inline void store_reg_tensor(TensorType& tensor, const Coord& coord, const reg_tensor<RegDataType>& src)
{
    using engine_type = typename TensorType::engine_type;
    using data_type = typename TensorType::data_type;
    static_assert(Std::is_same_v<get_mem_location<engine_type>, location::ub>,
                  "base_tensor::store only supports tensors located in UB");
    static_assert(Std::is_same_v<data_type, RegDataType>,
                  "base_tensor::store requires the tensor and reg_tensor to have the same element type");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    auto dst_engine = tensor.engine() + tensor.layout()(coord);
    asc_storealign(dst_engine.begin().get(), src.reg, src.mask);
#endif
}

} // namespace detail

template <typename EngineType, typename LayoutType>
template <load_sideband_mode sideband_mode, typename Coord>
__simd_callee__ inline decltype(auto) base_tensor<EngineType, LayoutType>::load(const Coord& coord) const
{
    return detail::load_reg_tensor<sideband_mode>(*this, coord);
}

template <typename EngineType, typename LayoutType>
template <typename Coord, typename RegDataType>
__simd_callee__ inline void base_tensor<EngineType, LayoutType>::store(const Coord& coord,
                                                                       const reg_tensor<RegDataType>& src)
{
    detail::store_reg_tensor(*this, coord, src);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_REG_LOAD_STORE_H
