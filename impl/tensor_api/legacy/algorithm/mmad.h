/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_ALGORITHM_MMAD_H
#define IMPL_TENSOR_API_LEGACY_ALGORITHM_MMAD_H

namespace asc {
namespace te {

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

#endif // IMPL_TENSOR_API_LEGACY_ALGORITHM_MMAD_H
