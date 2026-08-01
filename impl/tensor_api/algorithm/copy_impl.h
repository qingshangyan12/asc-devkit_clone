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

template <typename Trait>
__aicore__ inline constexpr Trait normalize_copy_trait(const Trait& trait)
{
    return trait;
}

__aicore__ inline constexpr copy_l0c_to_gm_trait normalize_copy_trait(const CopyL0C2GMTrait& trait)
{
    return {normalize_round_mode(trait.roundMode), trait.enableRelu, trait.enableChannelSplit};
}

__aicore__ inline constexpr copy_l0c_to_ub_trait normalize_copy_trait(const CopyL0C2UBTrait& trait)
{
    return {normalize_round_mode(trait.roundMode), trait.enableRelu, trait.enableChannelSplit,
            normalize_dual_dst_mode(trait.dualDstCtl)};
}

__aicore__ inline constexpr copy_l0c_to_l1_trait normalize_copy_trait(const CopyL0C2L1Trait& trait)
{
    return {normalize_round_mode(trait.roundMode), trait.enableRelu, trait.enableChannelSplit};
}

template <typename TraitStruct>
struct copy_trait_adapter {
    using trait_type = Std::remove_cvref_t<decltype(normalize_copy_trait(TraitStruct::value))>;
    static constexpr const trait_type value = normalize_copy_trait(TraitStruct::value);
};

template <typename QuantParam>
constexpr bool is_quant_scalar_v =
    !is_attr_tensor_v<QuantParam> && Std::is_convertible_v<Std::remove_cvref_t<QuantParam>, uint64_t>;

template <typename QuantParam>
constexpr bool is_copy_quant_param_v = is_quant_scalar_v<QuantParam> || is_attr_tensor_v<QuantParam>;

template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void copy(const copy_atom<AtomType>& atom_copy, const DstTensor& dst, const SrcTensor& src)
{
    atom_copy.call(dst, src);
}

template <typename AtomType, typename DstTensor, typename SrcTensor, typename QuantParam,
          Std::enable_if_t<is_copy_quant_param_v<QuantParam>, int> = 0>
__aicore__ inline void copy(const copy_atom<AtomType>& atom_copy, const DstTensor& dst, const SrcTensor& src,
                            const QuantParam& quant)
{
    atom_copy.call(dst, src, quant);
}

template <typename CopyOperationType>
__aicore__ inline constexpr auto make_copy(const CopyOperationType& copy_operation)
{
    return copy_atom<copy_traits<CopyOperationType>>{};
}

template <typename CopyOperationType, typename CopyTraitType>
__aicore__ inline constexpr auto make_copy(const CopyOperationType& copy_operation, const CopyTraitType& copy_trait)
{
    using trait_type = get_trait_member_type_t<CopyTraitType>;
    using normalized_trait_type = Std::remove_cvref_t<decltype(normalize_copy_trait(CopyTraitType::value))>;
    if constexpr (Std::is_same_v<trait_type, normalized_trait_type>) {
        return copy_atom<copy_traits<CopyOperationType, CopyTraitType>>{};
    } else {
        return copy_atom<copy_traits<CopyOperationType, copy_trait_adapter<CopyTraitType>>>{};
    }
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ALGORITHM_COPY_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
