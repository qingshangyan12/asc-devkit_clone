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
    "impl/tensor_api/algorithm/vector_compute_impl.h is an internal header file and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_TENSOR_API_ALGORITHM_VECTOR_COMPUTE_IMPL_H
#define IMPL_TENSOR_API_ALGORITHM_VECTOR_COMPUTE_IMPL_H

#include "impl/tensor_api/atom/vector/vec_atom_impl.h"

namespace AscendC {
namespace Te {

template <typename AtomType, typename Dst, typename... Rest>
__simd_callee__ inline void Compute(const VecAtom<AtomType>& atom, Dst& dst, Rest&... rest)
{
    atom.Call(dst, rest...);
}

template <typename OpType>
__aicore__ inline constexpr auto MakeVec(const OpType&)
{
    return VecAtom<VecTraits<OpType>>{};
}

template <typename OpType>
__aicore__ inline constexpr auto MakeVec(const OpType&, const VecTrait& trait)
{
    if constexpr (trait.mrg == MaskMergeMode::MERGING) {
        return VecAtom<VecTraits<OpType, VecTraitMergingTag>>{};
    } else {
        return VecAtom<VecTraits<OpType, VecTraitDefaultTag>>{};
    }
}

#define ASCENDC_TE_VEC_SUGAR2(NAME, TAG)                                                                               \
    template <typename Dst, typename Src0, typename Src1, typename MaskT>                                             \
    __simd_callee__ inline void NAME(Dst& dst, Src0& src0, Src1& src1, MaskT& mask)                                   \
    {                                                                                                                  \
        Compute(MakeVec(vec::TAG{}), dst, src0, src1, mask);                                                           \
    }

#define ASCENDC_TE_VEC_SUGAR1(NAME, TAG)                                                                               \
    template <typename Dst, typename Src, typename MaskT>                                                             \
    __simd_callee__ inline void NAME(Dst& dst, Src& src, MaskT& mask)                                                 \
    {                                                                                                                  \
        Compute(MakeVec(vec::TAG{}), dst, src, mask);                                                                \
    }

ASCENDC_TE_VEC_SUGAR2(Add, Add)
ASCENDC_TE_VEC_SUGAR2(Sub, Sub)
ASCENDC_TE_VEC_SUGAR2(Mul, Mul)
ASCENDC_TE_VEC_SUGAR2(Div, Div)
ASCENDC_TE_VEC_SUGAR2(Max, Max)
ASCENDC_TE_VEC_SUGAR2(Min, Min)
ASCENDC_TE_VEC_SUGAR1(Abs, Abs)
ASCENDC_TE_VEC_SUGAR1(Exp, Exp)
ASCENDC_TE_VEC_SUGAR1(Sqrt, Sqrt)
ASCENDC_TE_VEC_SUGAR1(Ln, Ln)
ASCENDC_TE_VEC_SUGAR1(Not, Not)

#undef ASCENDC_TE_VEC_SUGAR2
#undef ASCENDC_TE_VEC_SUGAR1

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ALGORITHM_VECTOR_COMPUTE_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
