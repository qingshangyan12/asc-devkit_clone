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
    "reg_data_load_impl.h is internal; include <tensor_api/experimental/arch/vector/reg_data_load.h> instead."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_LOAD_IMPL
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_LOAD_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_LOAD_IMPL_H

#if !defined(INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_LOAD_H)
#include "tensor_api/experimental/arch/vector/reg_data_load.h"
#endif
#include "tensor_api/tensor/tensor.h"

namespace asc {
namespace te {
namespace experimental {

template <load_sideband_mode sideband_mode, typename Tensor, typename Coord>
__simd_callee__ inline decltype(auto) load(const Tensor& tensor, const Coord& coord)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "load only supports tensors located in UB");
    TENSOR_API_DEBUG_CHECK(debug_check_coord, tensor.layout(), coord, "load");

    auto src_engine = tensor.engine() + tensor.layout()(coord);
    auto src = src_engine.begin().get();

    reg_tensor<data_type> dst;
    if constexpr (sideband_mode == load_sideband_mode::direct) {
        asc_loadalign(dst.reg, src);
    } else if constexpr (sideband_mode == load_sideband_mode::downsample) {
        asc_loadalign_downsample(dst.reg, src);
    } else if constexpr (sideband_mode == load_sideband_mode::unpack) {
        asc_loadalign_unpack(dst.reg, src);
    } else if constexpr (sideband_mode == load_sideband_mode::unpack4) {
        asc_loadalign_unpack4(dst.reg, src);
    } else if constexpr (sideband_mode == load_sideband_mode::upsample) {
        asc_loadalign_upsample(dst.reg, src);
    } else if constexpr (sideband_mode == load_sideband_mode::deintlv) {
        static_assert(sideband_mode != load_sideband_mode::deintlv,
            "deintlv requires the load(tensor, coord, dst0, dst1) overload");
    } else {
        static_assert(sideband_mode == load_sideband_mode::direct, "unsupported load sideband mode");
    }
    return dst;
}

template <load_sideband_mode sideband_mode, typename Tensor, typename Coord, typename RegDataType>
__simd_callee__ inline void load(const Tensor& tensor, const Coord& coord, reg_tensor<RegDataType>& dst0,
    reg_tensor<RegDataType>& dst1)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "load only supports tensors located in UB");
    static_assert(Std::is_same_v<data_type, RegDataType>,
        "load requires matching tensor and register element types");
    TENSOR_API_DEBUG_CHECK(debug_check_coord, tensor.layout(), coord, "load");

    if constexpr (sideband_mode == load_sideband_mode::deintlv) {
        auto src_engine = tensor.engine() + tensor.layout()(coord);
        asc_loadalign_deintlv(dst0.reg, dst1.reg, src_engine.begin().get());
    } else {
        static_assert(sideband_mode == load_sideband_mode::deintlv,
            "the two-destination load overload only supports deintlv");
    }
}

template <broadcast_mode brc_mode, typename Tensor, typename Coord>
__simd_callee__ inline decltype(auto) load_broadcast(const Tensor& tensor, const Coord& coord)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "load_broadcast only supports tensors located in UB");
    TENSOR_API_DEBUG_CHECK(debug_check_coord, tensor.layout(), coord, "load_broadcast");

    auto src_engine = tensor.engine() + tensor.layout()(coord);
    auto src = src_engine.begin().get();

    reg_tensor<data_type> dst;
    if constexpr (brc_mode == broadcast_mode::elem) {
        asc_loadalign_brc_elem(dst.reg, src);
    } else if constexpr (brc_mode == broadcast_mode::datablock) {
        asc_loadalign_brc_datablock(dst.reg, src);
    } else if constexpr (brc_mode == broadcast_mode::elem2datablock) {
        asc_loadalign_brc_elem2datablock(dst.reg, src);
    } else {
        static_assert(brc_mode == broadcast_mode::elem, "unsupported broadcast mode");
    }
    return dst;
}

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_LOAD_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_LOAD_IMPL)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_LOAD_IMPL
#endif
