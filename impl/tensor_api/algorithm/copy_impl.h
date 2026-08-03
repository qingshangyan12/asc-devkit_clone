/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/algorithm/copy_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file copy_impl.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ALGORITHM_COPY_IMPL_H
#define IMPL_TENSOR_API_ALGORITHM_COPY_IMPL_H

#include "impl/tensor_api/atom/copy_atom_impl.h"

namespace asc {
namespace te {

template <typename quant_param_type>
constexpr bool is_quant_scalar_v =
    !is_attr_tensor_v<quant_param_type> && Std::is_convertible_v<Std::remove_cvref_t<quant_param_type>, uint64_t>;

template <typename quant_param_type>
constexpr bool is_copy_quant_param_v = is_quant_scalar_v<quant_param_type> || is_attr_tensor_v<quant_param_type>;

template <typename coord_type>
constexpr bool is_copy_coord_param_v = Std::is_tuple_v<Std::remove_cvref_t<coord_type>> ||
    Std::is_same_v<Std::remove_cvref_t<coord_type>, zero_coord_type>;

template <typename coord_type, typename shape_type>
constexpr bool is_copy_coord_shape_compatible_v =
    Std::is_same_v<Std::remove_cvref_t<coord_type>, zero_coord_type> || is_same_structure_v<coord_type, shape_type>;

template <typename coord_dst_type, typename coord_src_type, typename shape_type>
constexpr bool is_copy_region_param_v = is_copy_coord_param_v<coord_dst_type> && is_copy_coord_param_v<coord_src_type> &&
    Std::is_tuple_v<Std::remove_cvref_t<shape_type>>;

template <typename pattern_type>
constexpr bool is_conv_layout_pattern_v = Std::is_same_v<pattern_type, nchw_layout_ptn> ||
    Std::is_same_v<pattern_type, nhwc_layout_ptn> || Std::is_same_v<pattern_type, nc1hwc0_layout_ptn> ||
    Std::is_same_v<pattern_type, ncdhw_layout_ptn> || Std::is_same_v<pattern_type, ndc1hwc0_layout_ptn>;

template <typename src_tensor_type, typename shape_param_type>
struct is_copy_shape_compatible_with_src_layout {
private:
    using layout_type = typename Std::remove_cvref_t<src_tensor_type>::layout_type;
    using src_shape_type = Std::remove_cvref_t<decltype(Std::declval<layout_type>().shape())>;
    using shape_type = Std::remove_cvref_t<shape_param_type>;
    using pattern_type = get_layout_pattern<layout_type>;

    static constexpr size_t src_depth = nesting_depth_v<src_shape_type>;
    static constexpr size_t shape_depth = nesting_depth_v<shape_type>;
    static constexpr size_t src_rank = Std::tuple_size_v<src_shape_type>;
    static constexpr size_t shape_rank = Std::tuple_size_v<shape_type>;
    static constexpr size_t batch_num = src_rank - 1;

    static constexpr bool is_same_structure = is_same_structure_v<src_shape_type, shape_type>;
    static constexpr bool is_logical_matrix = !is_conv_layout_pattern_v<pattern_type> && src_rank == TWO_DIM_DATA &&
        src_depth == FOUR_DIM_DATA && shape_depth == TWO_DIM_DATA;
    static constexpr bool is_logical_flat_batch = !is_conv_layout_pattern_v<pattern_type> && src_rank == shape_rank &&
        src_rank >= TWO_DIM_DATA && src_depth == batch_num + FOUR_DIM_DATA &&
        shape_depth == batch_num + TWO_DIM_DATA;

public:
    static constexpr bool value = is_same_structure || is_logical_matrix || is_logical_flat_batch;
};

template <typename src_tensor_type, typename shape_type>
constexpr bool is_copy_shape_compatible_with_src_layout_v = is_copy_shape_compatible_with_src_layout<src_tensor_type, shape_type>::value;

template <typename src_tensor_type, typename coord_src_type, typename shape_type>
__aicore__ inline constexpr void check_copy_region_params()
{
    static_assert(is_copy_coord_shape_compatible_v<coord_src_type, shape_type>,
        "Explicit coord_src and shape must have the same tuple structure.");
    static_assert(is_copy_shape_compatible_with_src_layout_v<src_tensor_type, shape_type>,
        "shape_type must match the source layout_type shape, or use a supported logical matrix/batch shape.");
}

template <typename atom_type, typename dst_tensor_type, typename src_tensor_type>
__aicore__ inline void copy(const copy_atom<atom_type>& atom_copy, const dst_tensor_type& dst, const src_tensor_type& src)
{
    atom_copy.call(dst, src);
}

template <typename atom_type, typename dst_tensor_type, typename src_tensor_type, typename quant_param_type,
    Std::enable_if_t<is_copy_quant_param_v<quant_param_type>, int> = 0>
__aicore__ inline void copy(const copy_atom<atom_type>& atom_copy, const dst_tensor_type& dst, const src_tensor_type& src,
    const quant_param_type& quant)
{
    atom_copy.call(dst, src, quant);
}

template <typename atom_type, typename dst_tensor_type, typename src_tensor_type, typename coord_dst_type, typename coord_src_type,
    typename shape_type, Std::enable_if_t<is_copy_region_param_v<coord_dst_type, coord_src_type, shape_type>, int> = 0>
__aicore__ inline void copy(const copy_atom<atom_type>& atom_copy, const dst_tensor_type& dst, const src_tensor_type& src,
    const coord_dst_type& coord_dst, const coord_src_type& coord_src, const shape_type& shape)
{
    check_copy_region_params<src_tensor_type, coord_src_type, shape_type>();
    atom_copy.call(dst, src, coord_dst, coord_src, shape);
}

template <typename atom_type, typename dst_tensor_type, typename src_tensor_type, typename quant_param_type, typename coord_dst_type,
    typename coord_src_type, typename shape_type,
    Std::enable_if_t<is_copy_quant_param_v<quant_param_type> && is_copy_region_param_v<coord_dst_type, coord_src_type, shape_type>, int> = 0>
__aicore__ inline void copy(const copy_atom<atom_type>& atom_copy, const dst_tensor_type& dst, const src_tensor_type& src,
    const quant_param_type& quant, const coord_dst_type& coord_dst, const coord_src_type& coord_src, const shape_type& shape)
{
    check_copy_region_params<src_tensor_type, coord_src_type, shape_type>();
    atom_copy.call(dst, src, quant, coord_dst, coord_src, shape);
}

template <typename CopyOperationType>
__aicore__ inline constexpr auto make_copy(const CopyOperationType& copy_operation)
{
    return copy_atom<copy_traits<CopyOperationType>>{};
}

template <typename CopyOperationType, typename CopyTraitType>
__aicore__ inline constexpr auto make_copy(const CopyOperationType& copy_operation, const CopyTraitType& copy_trait)
{
    return copy_atom<copy_traits<CopyOperationType, CopyTraitType>>{};
}

}
}

#endif // IMPL_TENSOR_API_ALGORITHM_COPY_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
