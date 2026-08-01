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
    "impl/tensor_api/arch/cube/l1_to_bt/copy_impl/nd2nd.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file nd2nd.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_BT_COPY_IMPL_ND2ND_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_BT_COPY_IMPL_ND2ND_H

#include "impl/tensor_api/arch/cube/l1_to_bt/copy_impl/instruction.h"

namespace asc {
namespace te {

class copy_l1_to_btnd {
public:
    template <const copy_l1_to_bt_trait& trait, typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src)
    {
        // Batch layouts carry a leading B axis: (B, (row, col)) -> depth 3,
        // (B, ((1, row), (1, col))) -> depth 5. Non-batch layouts are depth 2/4.
        constexpr auto l1_depth = nesting_depth_v<decltype(src.layout().shape())>;
        if constexpr (l1_depth == THREE_DIM_DATA || l1_depth == FIVE_DIM_DATA) {
            batch_data_copy_impl<trait, T, U>(dst, src);
        } else {
            data_copy_impl<trait, T, U>(dst, src);
        }
    }

private:
    template <const copy_l1_to_bt_trait& trait, typename T, typename U>
    __aicore__ inline static constexpr void check_template()
    {
        check_layout_pattern<T, U>();
        check_data_type::check_l12_bt_data_type<T, U>();
    }

    template <const copy_l1_to_bt_trait& trait, typename T, typename U>
    __aicore__ inline static void data_copy_impl(const T& dst, const U& src)
    {
        check_template<trait, T, U>();

        auto dst_layout = dst.layout();
        auto src_layout = src.layout();

        uint16_t src_col;
        uint16_t src_row;
        uint16_t dst_col;
        uint16_t dst_row;
        uint16_t block_count;
        if constexpr (is_satisfied_ptn_format_v<U, NDLayoutPtn>) {
            src_col = get_element<AttrInfo::Shape, AttrInfo::Column>(src_layout);
            src_row = get_element<AttrInfo::Stride, AttrInfo::Row>(src_layout);
            block_count = get_element<AttrInfo::Shape, AttrInfo::Row>(src_layout);
        } else {
            src_col = get_element<AttrInfo::Shape, AttrInfo::Column, 1>(src_layout);
            src_row = get_element<AttrInfo::Stride, AttrInfo::Row, 1>(src_layout);
            block_count = get_element<AttrInfo::Shape, AttrInfo::Row, 1>(src_layout);
        }
        if constexpr (is_satisfied_ptn_format_v<T, NDLayoutPtn>) {
            dst_col = get_element<AttrInfo::Shape, AttrInfo::Column>(dst_layout);
            dst_row = get_element<AttrInfo::Stride, AttrInfo::Row>(dst_layout);
        } else {
            dst_col = get_element<AttrInfo::Shape, AttrInfo::Column, 1>(dst_layout);
            dst_row = get_element<AttrInfo::Stride, AttrInfo::Row, 1>(dst_layout);
        }

        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        bool conv_control = false;
        if (is_one_of_attr_v<srcType, half> && is_one_of_attr_v<dstType, float>) {
            conv_control = true;
        }

        uint16_t block_len = Std::ceil_division(src_col, C0_ELEMENT<srcType>);
        if constexpr (is_one_of_attr_v<srcType, float, int32_t>) {
            block_len = Std::ceil_align(block_len, 2);
        }

        uint16_t src_stride = (src_row - src_col) / C0_ELEMENT<srcType>;
        uint16_t dst_stride = Std::ceil_align((dst_row - src_col) / C0_ELEMENT<dstType>, 2);
        copy_l1_to_bt_instr::data_copy(dst, src, conv_control, block_count, block_len, src_stride, dst_stride);
    }

    // Batch case: layout is (B, (M, N)) with strides (sB, (sM, sN)). Per-matrix internal
    // compactness is assumed (sM == N), so a single matrix can be moved as one block_len-sized block.
    // Four DataCopy params are derived directly from the batched layout:
    //   - get_element<Shape, Row/Column> on the batched layout returns sub-matrix M/N (the
    //     SelectRowColTuples helper in is_format.h handles batch-axis stripping).
    //   - get<0>(Shape/Stride) returns the batch size and per-matrix start-to-start stride.
    //   - When batches are also contiguous (src_batch_stride == M*N && dst_batch_stride == M*N),
    //     fold the B blocks into a single B*M*N block; otherwise emit B blocks, one per matrix.
    //   - Both src_stride and dst_stride encode the end-to-next-start gap in C0_ELEMENT units, in
    //     keeping with the L1->BT instruction's stride convention.
    template <const copy_l1_to_bt_trait& trait, typename T, typename U>
    __aicore__ inline static void batch_data_copy_impl(const T& dst, const U& src)
    {
        check_template<trait, T, U>();

        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        auto src_layout = src.layout();
        auto dst_layout = dst.layout();

        uint16_t batch_size = get<0>(src_layout.shape());
        uint32_t src_batch_stride = get<0>(src_layout.stride());
        uint32_t dst_batch_stride = get<0>(dst_layout.stride());

        // Strip the leading B axis before calling GetElement, otherwise the dim=0/1 split below
        // would read the batch axis as row/column.
        auto src_inner = get<1>(src_layout);

        uint32_t src_shape_rows;
        uint32_t src_shape_columns;
        if constexpr (is_satisfied_ptn_format_v<U, NDLayoutPtn>) {
            src_shape_rows = get_element<AttrInfo::Shape, AttrInfo::Row>(src_inner);
            src_shape_columns = get_element<AttrInfo::Shape, AttrInfo::Column>(src_inner);
        } else {
            src_shape_rows = get_element<AttrInfo::Shape, AttrInfo::Row, 1>(src_inner);
            src_shape_columns = get_element<AttrInfo::Shape, AttrInfo::Column, 1>(src_inner);
        }

        bool conv_control = false;
        if (is_one_of_attr_v<srcType, half> && is_one_of_attr_v<dstType, float>) {
            conv_control = true;
        }

        uint32_t matrix_elems = src_shape_rows * src_shape_columns;

        // batch-strided: B blocks, stride encodes per-matrix end-to-next-start gap.
        uint16_t block_count = batch_size;
        uint16_t block_len = Std::ceil_division(matrix_elems, C0_ELEMENT<srcType>);
        if constexpr (is_one_of_attr_v<srcType, float, int32_t>) {
            block_len = Std::ceil_align(block_len, 2);
        }
        uint16_t src_stride = (src_batch_stride - matrix_elems) / C0_ELEMENT<srcType>;
        uint16_t dst_stride = Std::ceil_align((dst_batch_stride - matrix_elems) / C0_ELEMENT<dstType>, 2);

        copy_l1_to_bt_instr::data_copy(dst, src, conv_control, block_count, block_len, src_stride, dst_stride);
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_BT_COPY_IMPL_ND2ND_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
