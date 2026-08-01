/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_LEGACY_H
#define IMPL_TENSOR_API_LEGACY_LEGACY_H

#include "impl/tensor_api/legacy/legacy_utils.h"

namespace asc {
namespace te {

namespace location {
using INVALID = invalid;
using GM = gm;
using UB = ub;
using L1 = l1;
using L0A = l0a;
using L0B = l0b;
using L0ScaleA = l0scalea;
using L0ScaleB = l0scaleb;
using L0C = l0c;
using BIAS = bias;
using FIXBUF = fixbuf;
using SSBUF = ssbuf;
} // namespace location
namespace Location = location;

template <typename... Shapes>
using Shape = shape_type<Shapes...>;

template <typename... Strides>
using Stride = stride_type<Strides...>;

template <typename... Coords>
using Coord = coord_type<Coords...>;

template <typename T, typename U, typename Info = Std::ignore_t>
using Layout = layout_type<T, U, Info>;

template <typename T>
using GetLayoutPattern = get_layout_pattern<T>;

using ZNLayoutPtn = zn_layout_ptn;
using ZZLayoutPtn = zz_layout_ptn;
using NNLayoutPtn = nn_layout_ptn;
using NZLayoutPtn = nz_layout_ptn;
using NDLayoutPtn = nd_layout_ptn;
using DNLayoutPtn = dn_layout_ptn;
using NDExtLayoutPtn = nd_ext_layout_ptn;
using DNExtLayoutPtn = dn_ext_layout_ptn;
using ScaleANDLayoutPtn = scalea_nd_layout_ptn;
using ScaleADNLayoutPtn = scalea_dn_layout_ptn;
using ScaleBNDLayoutPtn = scaleb_nd_layout_ptn;
using ScaleBDNLayoutPtn = scaleb_dn_layout_ptn;
using NCHWLayoutPtn = nchw_layout_ptn;
using NHWCLayoutPtn = nhwc_layout_ptn;
using NC1HWC0LayoutPtn = nc1hwc0_layout_ptn;
using NCDHWLayoutPtn = ncdhw_layout_ptn;
using NDC1HWC0LayoutPtn = ndc1hwc0_layout_ptn;

template <typename Iterator>
using ViewEngine = view_engine<Iterator>;

using CopyGM2L1 = copy_gm_to_l1;
using CopyL12UB = copy_l1_to_ub;
using CopyL12L0B = copy_l1_to_l0b;
using CopyL12L0A = copy_l1_to_l0a;
using CopyL12L0ScaleA = copy_l1_to_l0scalea;
using CopyL12L0ScaleB = copy_l1_to_l0scaleb;
using CopyL12FB = copy_l1_to_fb;
using CopyL12BT = copy_l1_to_bt;
using CopyL0C2GM = copy_l0c_to_gm;
using CopyL0C2UB = copy_l0c_to_ub;
using CopyL0C2L1 = copy_l0c_to_l1;
using CopyGM2L1Trait = copy_gm_to_l1_trait;
using CopyGM2L1TraitDefault = copy_gm_to_l1_trait_default;
using CopyL12UBTrait = copy_l1_to_ub_trait;
using CopyL12UBTraitDefault = copy_l1_to_ub_trait_default;
using CopyL12L0ATrait = copy_l1_to_l0a_trait;
using CopyL12L0ATraitDefault = copy_l1_to_l0a_trait_default;
using CopyL12L0BTrait = copy_l1_to_l0b_trait;
using CopyL12L0BTraitDefault = copy_l1_to_l0b_trait_default;
using CopyL12L0ScaleATrait = copy_l1_to_l0scalea_trait;
using CopyL12L0ScaleATraitDefault = copy_l1_to_l0scalea_trait_default;
using CopyL12L0ScaleBTrait = copy_l1_to_l0scaleb_trait;
using CopyL12L0ScaleBTraitDefault = copy_l1_to_l0scaleb_trait_default;
using CopyL12FBTrait = copy_l1_to_fb_trait;
using CopyL12FBTraitDefault = copy_l1_to_fb_trait_default;
using CopyL12BTTrait = copy_l1_to_bt_trait;
using CopyL12BTTraitDefault = copy_l1_to_bt_trait_default;
using CopyL0C2GMTrait = copy_l0c_to_gm_trait;
using CopyL0C2GMTraitDefault = copy_l0c_to_gm_trait_default;
using CopyL0C2UBTrait = copy_l0c_to_ub_trait;
using CopyL0C2UBTraitDefault = copy_l0c_to_ub_trait_default;
using CopyL0C2L1Trait = copy_l0c_to_l1_trait;
using CopyL0C2L1TraitDefault = copy_l0c_to_l1_trait_default;

using CopyGM2UB = copy_gm_to_ub;
using CopyUB2L1 = copy_ub_to_l1;
using CopyUB2GM = copy_ub_to_gm;
using CopyUB2UB = copy_ub_to_ub;
using CopyGM2UBTrait = copy_gm_to_ub_trait;
using CopyGM2UBTraitDefault = copy_gm_to_ub_trait_default;
using CopyUB2L1Trait = copy_ub_to_l1_trait;
using CopyUB2L1TraitDefault = copy_ub_to_l1_trait_default;
using CopyUB2GMTrait = copy_ub_to_gm_trait;
using CopyUB2GMTraitDefault = copy_ub_to_gm_trait_default;
using CopyUB2UBTrait = copy_ub_to_ub_trait;
using CopyUB2UBTraitDefault = copy_ub_to_ub_trait_default;

using MmadTraitDefault = mmad_trait_default;
using MmadOperation = mmad_operation;

template <typename T, typename... Ts>
__aicore__ inline constexpr Shape<T, Ts...> MakeShape(const T& value, const Ts&... values)
{
    return make_shape(value, values...);
}

template <typename T, typename... Ts>
__aicore__ inline constexpr Stride<T, Ts...> MakeStride(const T& value, const Ts&... values)
{
    return make_stride(value, values...);
}

template <typename T, typename... Ts>
__aicore__ inline constexpr Coord<T, Ts...> MakeCoord(const T& value, const Ts&... values)
{
    return make_coord(value, values...);
}

template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride)
{
    return make_layout(shape, stride);
}

template <typename ShapeType>
__aicore__ inline constexpr auto MakeLayout(const ShapeType& shape)
{
    return make_layout(shape);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto GetShape(const LayoutType& layout)
{
    return get_shape<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto GetShape(LayoutType& layout)
{
    return get_shape<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto GetStride(const LayoutType& layout)
{
    return get_stride<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto GetStride(LayoutType& layout)
{
    return get_stride<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Coshape(const LayoutType& layout)
{
    return coshape<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Cosize(const LayoutType& layout)
{
    return cosize<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Rank(const LayoutType& layout)
{
    return rank<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Select(const LayoutType& layout)
{
    return select<Is...>(layout);
}

template <size_t I, typename Tuple, typename = Std::enable_if_t<Std::is_tuple_v<Std::remove_cvref_t<Tuple>>>>
__aicore__ inline constexpr auto Get(Tuple&& tuple)
{
    return get<I>(tuple);
}

template <size_t I0, size_t I1, size_t... Is, typename Tuple,
          typename = Std::enable_if_t<Std::is_tuple_v<Std::remove_cvref_t<Tuple>>>>
__aicore__ inline constexpr auto Get(Tuple&& tuple)
{
    return get<I0, I1, Is...>(tuple);
}

template <typename Tuple, typename = Std::enable_if_t<Std::is_tuple_v<Std::remove_cvref_t<Tuple>>>>
__aicore__ inline constexpr auto Get(Tuple&& tuple)
{
    return get(tuple);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Get(const LayoutType& layout)
{
    return get<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Size(const LayoutType& layout)
{
    return size<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Capacity(const LayoutType& layout)
{
    return capacity<Is...>(layout);
}

template <typename Tensor, typename CoordType, typename Info>
__aicore__ inline constexpr decltype(auto) Slice(Tensor&& tensor, const CoordType& coord, const Info& info)
{
    return slice(tensor, coord, info);
}

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const U& shape, const S& stride)
{
    return crd2idx(coord, shape, stride);
}

template <typename T, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const LayoutType& layout)
{
    return crd2idx(coord, layout);
}

template <typename LayoutPattern, typename TraitType = Std::ignore_t, typename... Args>
__aicore__ inline constexpr decltype(auto) MakeFrameLayout(const Args&... args)
{
    return make_frame_layout<LayoutPattern, TraitType>(args...);
}

template <typename LayoutPattern, size_t C0Element, typename... Args>
__aicore__ inline constexpr decltype(auto) MakeFrameLayout(const Args&... args)
{
    return make_frame_layout<LayoutPattern, C0Element>(args...);
}

template <typename LayoutPattern, typename TraitType, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto MakePatternLayout(const ShapeType& shape, const StrideType& stride)
{
    return make_pattern_layout<LayoutPattern, TraitType>(shape, stride);
}

template <typename PtrPattern, typename Iterator>
__aicore__ inline auto MakeLocationMemPtr(Iterator iterator)
{
    return make_location_mem_ptr<PtrPattern>(iterator);
}

template <typename PtrPattern, typename DataType, typename Addr, enable_make_ptr_by_trait<PtrPattern, Addr> = 0>
__aicore__ inline auto MakeMemPtr(Addr address)
{
    return make_mem_ptr<PtrPattern, DataType>(address);
}

template <typename PtrPattern, typename Iterator, enable_make_hardware_ptr<PtrPattern, Iterator> = 0>
__aicore__ inline constexpr auto MakeMemPtr(Iterator iterator)
{
    return make_mem_ptr<PtrPattern>(iterator);
}

template <typename Iterator, enable_make_ptr_by_iter<Iterator> = 0>
__aicore__ inline constexpr auto MakeMemPtr(Iterator iterator)
{
    return make_mem_ptr(iterator);
}

template <typename Iterator, typename... Args>
__aicore__ inline constexpr auto MakeTensor(const Iterator& iterator, const Args&... args)
{
    return make_tensor(iterator, args...);
}

template <size_t... SqueezeDims, typename T,
          typename = Std::enable_if_t<(is_layout_v<T> || is_attr_tensor_v<T>) && (sizeof...(SqueezeDims) > 0)>>
__aicore__ inline constexpr auto Squeeze(const T& value)
{
    return squeeze<SqueezeDims...>(value);
}

template <typename Pattern, typename T,
          typename = Std::enable_if_t<(is_layout_v<T> || is_attr_tensor_v<T>)
                                      && Std::is_tuple_v<Std::remove_cvref_t<Pattern>>>>
__aicore__ inline constexpr auto Squeeze(const T& value, const Pattern& pattern)
{
    return squeeze(value, pattern);
}

template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void Copy(const copy_atom<AtomType>& atom_copy, const DstTensor& dst, const SrcTensor& src)
{
    copy(atom_copy, dst, src);
}

template <typename AtomType, typename DstTensor, typename SrcTensor, typename QuantParam,
          Std::enable_if_t<is_copy_quant_param_v<QuantParam>, int> = 0>
__aicore__ inline void Copy(const copy_atom<AtomType>& atom_copy, const DstTensor& dst, const SrcTensor& src,
                            const QuantParam& quant)
{
    copy(atom_copy, dst, src, quant);
}

template <typename CopyOperationType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copy_operation)
{
    return make_copy(copy_operation);
}

template <typename CopyOperationType, typename CopyTraitType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copy_operation, const CopyTraitType& copy_trait)
{
    return make_copy(copy_operation, copy_trait);
}

template <typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor>
__aicore__ inline void Mmad(const mmad_atom<AtomType>& atom_mmad, const DstTensor& dst, const FmTensor& fm,
                            const FilterTensor& filter)
{
    return mmad(atom_mmad, dst, fm, filter);
}

template <typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor, typename BiasTensor,
          Std::enable_if_t<is_attr_tensor_v<BiasTensor>, int> = 0>
__aicore__ inline void Mmad(const mmad_atom<AtomType>& atom_mmad, const DstTensor& dst, const FmTensor& fm,
                            const FilterTensor& filter, const BiasTensor& bias)
{
    return mmad(atom_mmad, dst, fm, filter, bias);
}

template <typename MmadOperationType>
__aicore__ inline constexpr auto MakeMmad(const MmadOperationType& operation)
{
    return make_mmad(operation);
}

template <typename MmadOperationType, typename MmadTraitType>
__aicore__ inline constexpr auto MakeMmad(const MmadOperationType& operation, const MmadTraitType& trait)
{
    return make_mmad(operation, trait);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_LEGACY_H
