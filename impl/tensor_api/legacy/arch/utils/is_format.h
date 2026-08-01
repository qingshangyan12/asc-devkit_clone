/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_ARCH_UTILS_IS_FORMAT_H
#define IMPL_TENSOR_API_LEGACY_ARCH_UTILS_IS_FORMAT_H

namespace asc {
namespace te {

template <typename T>
using ToTuple = to_tuple<T>;

namespace attr_info {
using Shape = shape;
using Stride = stride;
using Row = row;
using Column = column;
} // namespace attr_info
namespace AttrInfo = attr_info;

template <typename T, bool IsTuple>
using ToTupleImpl = to_tuple_impl<T, IsTuple>;

template <typename ShapeRows, typename ShapeCols, typename StrideRows, typename StrideCols, bool IsBatched>
using SelectRowColTuples = select_row_col_tuples<ShapeRows, ShapeCols, StrideRows, StrideCols, IsBatched>;

template <typename T>
using GetTypeFromNDimTrait = get_type_from_n_dim_trait<T>;

template <typename T, typename Info1, typename Info2, size_t Dim>
using GetNDimType = get_n_dim_type<T, Info1, Info2, Dim>;

template <typename TensorType, typename TargetLayoutPtn>
using IsSatisfiedPtnFormat = is_satisfied_ptn_format<TensorType, TargetLayoutPtn>;

template <typename TensorType, typename TargetLayoutPtn>
inline constexpr bool IsSatisfiedPtnFormatV = is_satisfied_ptn_format_v<TensorType, TargetLayoutPtn>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_ARCH_UTILS_IS_FORMAT_H
