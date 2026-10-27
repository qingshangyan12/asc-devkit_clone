/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_TENSOR_API_ARCH_VECTOR_FRACTAL_COPY_PARAMS_H
#define IMPL_TENSOR_API_ARCH_VECTOR_FRACTAL_COPY_PARAMS_H

#include "impl/tensor_api/utils/utils_impl.h"

namespace asc {
namespace te {

template <typename T, typename U, typename SrcCoord, typename ShapeType>
__aicore__ inline auto make_nz_slice_copy_params(
    const T& dst, const U& src, const SrcCoord& src_coord, const ShapeType& copy_shape)
{
    using src_type = typename U::element_type;
    using dst_type = typename T::element_type;
    auto src_shape = make_slice_shape(src_coord, src.layout(), copy_shape);
    auto block_count =
        get_shape_batch_size(src_shape) * Std::ceil_division(get_shape_columns(src_shape), c0_element<src_type>);
    auto block_len = get_shape_rows(src_shape) * c0_size<>;
    auto src_stride = get_column_stride(src.layout()) * sizeof(src_type);
    auto dst_stride = get_column_stride(dst.layout()) * sizeof(dst_type);
    return Std::make_tuple(block_count, block_len, src_stride, dst_stride);
}

template <typename T, typename U, typename SrcCoord, typename ShapeType>
__aicore__ inline auto make_zn_slice_copy_params(
    const T& dst, const U& src, const SrcCoord& src_coord, const ShapeType& copy_shape)
{
    using src_type = typename U::element_type;
    using dst_type = typename T::element_type;
    auto src_shape = make_slice_shape(src_coord, src.layout(), copy_shape);
    auto block_count =
        get_shape_batch_size(src_shape) * Std::ceil_division(get_shape_rows(src_shape), c0_element<src_type>);
    auto block_len = get_shape_columns(src_shape) * c0_size<>;
    auto src_stride = get_row_stride(src.layout()) * sizeof(src_type);
    auto dst_stride = get_row_stride(dst.layout()) * sizeof(dst_type);
    return Std::make_tuple(block_count, block_len, src_stride, dst_stride);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_FRACTAL_COPY_PARAMS_H
