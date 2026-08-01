/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_TUPLE_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_TUPLE_H

namespace asc {
namespace te {

using MultipliesUnaryLeftFold = multiplies_unary_left_fold;
using Product = product;

template <size_t Idx, typename Seq>
using PrependIndex = prepend_index<Idx, Seq>;

template <size_t Idx, bool... Mask>
using KeepIndexSeq = keep_index_seq<Idx, Mask...>;

template <bool... Mask>
using KeepIndexSeqT = keep_index_seq_t<Mask...>;

template <typename BoolSeq>
using KeepIndexSeqFromSeq = keep_index_seq_from_seq<BoolSeq>;

template <typename T, typename U, typename Enable = void>
using IsSameStructure = is_same_structure<T, U, Enable>;

template <typename T, typename U, typename Seq>
using AllChildrenSame = all_children_same<T, U, Seq>;

template <typename T, typename U>
constexpr bool IsSameStructureV = is_same_structure_v<T, U>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_TUPLE_H
