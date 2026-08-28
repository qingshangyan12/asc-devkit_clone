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
#pragma message( \
    "impl/tensor_api/arch/vector/gm_to_ub/copy_impl/one_dim.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"tensor_api/tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_VECTOR_GM_TO_UB_COPY_IMPL_ONE_DIM_H
#endif

/*!
 * \file one_dim.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_GM_TO_UB_COPY_IMPL_ONE_DIM_H
#define IMPL_TENSOR_API_ARCH_VECTOR_GM_TO_UB_COPY_IMPL_ONE_DIM_H

#include "impl/tensor_api/arch/vector/gm_to_ub/copy_impl/data_copy.h"

namespace asc {
namespace te {

class copy_gm_to_ub_one_dim : private copy_gm_to_ub_common {
public:
    template <const gm_to_ub_trait& trait, typename DstTensor, typename SrcTensor>
    __aicore__ inline static void run(const DstTensor& dst, const SrcTensor& src, const gm_to_ub_params& params)
    {
        check_template<trait, DstTensor, SrcTensor>();
        using src_type = typename SrcTensor::element_type;
        constexpr uint16_t block_count = 1;
        uint32_t block_len = get<0>(src.layout().shape()) * sizeof(src_type);
        emit_copy(dst, src, block_count, block_len, 0, 0, params);
    }

    template <
        const gm_to_ub_trait& trait, typename T, typename U, typename DstCoord, typename SrcCoord, typename ShapeType>
    __aicore__ inline static void run(
        const T& dst, const U& src, const DstCoord& dst_coord, const SrcCoord& src_coord, const ShapeType& copy_shape,
        const gm_to_ub_params& params)
    {
        check_template<trait, T, U>();
        using src_type = typename U::element_type;
        constexpr uint16_t block_count = 1;
        uint32_t block_len = get<0>(copy_shape) * sizeof(src_type);
        emit_copy(dst, src, dst.layout()(dst_coord), src.layout()(src_coord), block_count, block_len, 0, 0, params);
    }

private:
    template <const gm_to_ub_trait& trait, typename DstTensor, typename SrcTensor>
    __aicore__ inline static constexpr void check_template()
    {
        check_layout_pattern<SrcTensor, DstTensor>();
        check_data_type::check_gm_to_ub_data_type<DstTensor, SrcTensor>();
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_GM_TO_UB_COPY_IMPL_ONE_DIM_H

#if defined( \
    UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_VECTOR_GM_TO_UB_COPY_IMPL_ONE_DIM_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_VECTOR_GM_TO_UB_COPY_IMPL_ONE_DIM_H
#endif
