/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_IMPL_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_IMPL_H

namespace asc {
namespace te {

using MinOp = min_op;
using DiffOp = diff_op;

template <size_t I, size_t... Dims>
using IsInPack = is_in_pack<I, Dims...>;

template <size_t I, size_t... Dims>
constexpr bool IsInPackV = is_in_pack_v<I, Dims...>;

template <typename T>
using IsSqueezeMark = is_squeeze_mark<T>;

template <typename T>
constexpr bool IsSqueezeMarkV = is_squeeze_mark_v<T>;

template <typename T, bool IsConst = is_integral_constant_v<Std::remove_cvref_t<T>>>
using SqueezableSize = squeezable_size<T, IsConst>;

template <typename T>
constexpr bool SqueezableSizeV = squeezable_size_v<T>;

template <typename Shape, typename DimsSeq, typename IdxSeq>
using DimsMaskImpl = dims_mask_impl<Shape, DimsSeq, IdxSeq>;

template <typename Shape, size_t... Dims>
using DimsMask = dims_mask<Shape, Dims...>;

template <typename Shape, typename Pattern, typename IdxSeq>
using PatternMaskImpl = pattern_mask_impl<Shape, Pattern, IdxSeq>;

template <typename Shape, typename Pattern>
using PatternMask = pattern_mask<Shape, Pattern>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_IMPL_H
