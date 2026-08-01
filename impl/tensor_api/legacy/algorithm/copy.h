/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_ALGORITHM_COPY_H
#define IMPL_TENSOR_API_LEGACY_ALGORITHM_COPY_H

namespace asc {
namespace te {

template <typename QuantParam>
constexpr bool IsQuantScalarV = is_quant_scalar_v<QuantParam>;

template <typename QuantParam>
constexpr bool IsCopyQuantParamV = is_copy_quant_param_v<QuantParam>;

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

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_ALGORITHM_COPY_H
