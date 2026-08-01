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
    "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/nz2nz.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file nz2nz.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_IMPL_NZ2NZ_H
#define IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_IMPL_NZ2NZ_H

#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/copy_common.h"

namespace asc {
namespace te {

class CopyGmToCbufAlignV2NZ {
public:
    template <const copy_gm_to_l1_trait& trait, typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src)
    {
        run_gm_to_l1_batched<trait, CopyGmToCbufAlignV2NZ, T, U>(dst, src);
    }

    template <const copy_gm_to_l1_trait& trait, typename T, typename U>
    __aicore__ inline static constexpr void check_template()
    {
        check_layout_pattern<U, T>();
        CheckDataType::check_gm2_l1_align_v2_nd_data_type<T, U>();
    }

    // Extracts single-matrix parameters from the (batch-stripped) src/dst layouts and emits the
    // copy. matrixNum carries the batch dimension (1 when there is no batch). The align_v2
    // instruction has no hardware batch loop, so the batch axis is folded into blockCount: NZ
    // matrices are laid out along the column (big-fractal) direction, so matrixNum matrices of
    // bigFractalSize big-fractals each become matrixNum * bigFractalSize contiguous blocks that
    // share the same per-big-fractal stride. srcNdMatrixStride/dstNzMatrixStride are unused here
    // because that contiguity makes the single srcStride/dstStride describe the whole sequence.
    template <typename T, typename U, typename SrcLayout, typename DstLayout>
    __aicore__ inline static void emit_copy(const T& dst, const U& src, const SrcLayout& src_layout,
                                           const DstLayout& dst_layout, uint16_t matrix_num,
                                           uint64_t src_nd_matrix_stride, uint32_t dst_nz_matrix_stride)
    {
        using type = typename U::elementType;

        auto small_fractal_size = get_element<AttrInfo::Shape, AttrInfo::Row, 0>(src_layout)
                                * get_element<AttrInfo::Shape, AttrInfo::Row, 1>(src_layout);
        auto big_fractal_size = get_element<AttrInfo::Shape, AttrInfo::Column, 1>(src_layout);
        auto src_stride_size = get_element<AttrInfo::Stride, AttrInfo::Column, 1>(src_layout);
        auto dst_stride_size = get_element<AttrInfo::Stride, AttrInfo::Column, 1>(dst_layout);

        uint8_t left_padding_cnt = 0;
        uint8_t right_padding_cnt = 0;
        uint8_t cache_mode = src.engine().get_cache_mode();

        auto block_count = big_fractal_size * matrix_num;
        auto block_len = small_fractal_size * C0_SIZE<>;
        auto src_stride = src_stride_size * sizeof(type);
        auto dst_stride = dst_stride_size * sizeof(type);
        if constexpr (IsB4Type<type>) {
            // move fp4 as b8, need to be divided by 2
            src_stride = src_stride >> 1;
            dst_stride = dst_stride >> 1;
        }
        CopyGmToCbufAlignV2Base::data_copy(dst, src, block_count, block_len, left_padding_cnt, right_padding_cnt, cache_mode,
                                          src_stride, dst_stride);
    }
};
} // namespace te
} // namespace asc

#endif

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
