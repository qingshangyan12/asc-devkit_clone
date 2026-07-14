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
    "impl/tensor_api/atom/vector/vec_atom_impl.h is an internal header file and must not be used directly. Please use "#include "tensor_api/tensor.h""."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file vec_atom_impl.h
* \brief Unified register-level (256B) vector compute atom: registry, traits, MERGING dispatch.
*/
#ifndef IMPL_TENSOR_API_ATOM_VECTOR_VEC_ATOM_IMPL_H
#define IMPL_TENSOR_API_ATOM_VECTOR_VEC_ATOM_IMPL_H

#include "tensor_api/arch/vector/vec_op.h"
#include "impl/tensor_api/arch/vector/unary/instruction.h"
#include "impl/tensor_api/arch/vector/binary/instruction.h"

namespace AscendC {
namespace Te {

namespace vec {
struct Add {}; struct Sub {}; struct Mul {}; struct Div {}; struct Max {}; struct Min {};
struct Abs {}; struct Exp {}; struct Sqrt {}; struct Ln {}; struct Not {};
} // namespace vec

enum class MaskMergeMode : uint8_t { ZEROING = 0, MERGING };

struct VecTrait {
    __aicore__ constexpr VecTrait() {}
    __aicore__ constexpr explicit VecTrait(MaskMergeMode mrgIn) : mrg(mrgIn) {}
    MaskMergeMode mrg = MaskMergeMode::ZEROING;
};

constexpr VecTrait vecTraitDefault{};

struct VecTraitDefaultTag {
    using TraitType = VecTrait;
    static constexpr const TraitType value{};
};

struct VecTraitMergingTag {
    using TraitType = VecTrait;
    static constexpr const TraitType value{MaskMergeMode::MERGING};
};

template <typename Op>
struct VecInstOf {
    static_assert(sizeof(Op) == 0,
        "AscendC::Te vec op tag is not wired. Add ASCENDC_TE_REGISTER_VEC_OP(<Tag>, <Inst>, <arity>) in "
        "impl/tensor_api/atom/vector/vec_atom_impl.h.");
};

#define ASCENDC_TE_REGISTER_VEC_OP(TAG, INST, ARITY)                                                                   \
    template <>                                                                                                        \
    struct VecInstOf<vec::TAG> {                                                                                       \
        using Inst = Te::Inst::INST;                                                                                   \
        static constexpr int arity = (ARITY);                                                                          \
    }

ASCENDC_TE_REGISTER_VEC_OP(Add, Add, 2);
ASCENDC_TE_REGISTER_VEC_OP(Sub, Sub, 2);
ASCENDC_TE_REGISTER_VEC_OP(Mul, Mul, 2);
ASCENDC_TE_REGISTER_VEC_OP(Div, Div, 2);
ASCENDC_TE_REGISTER_VEC_OP(Max, Max, 2);
ASCENDC_TE_REGISTER_VEC_OP(Min, Min, 2);
ASCENDC_TE_REGISTER_VEC_OP(Abs, Abs, 1);
ASCENDC_TE_REGISTER_VEC_OP(Exp, Exp, 1);
ASCENDC_TE_REGISTER_VEC_OP(Sqrt, Sqrt, 1);
ASCENDC_TE_REGISTER_VEC_OP(Ln, Ln, 1);
ASCENDC_TE_REGISTER_VEC_OP(Not, Not, 1);

#undef ASCENDC_TE_REGISTER_VEC_OP

namespace detail {

template <typename Inst, typename RegType, typename Dst, typename MaskT>
__simd_callee__ inline void VecMergeIntoDst(Dst& dst, RegType& tmp, MaskT& mask)
{
    RegType& dstReg = static_cast<RegType&>(dst);
    Te::Inst::Select::Run<RegType>(dstReg, tmp, dstReg, mask);
}

template <const VecTrait& traits, typename Inst, typename Dst, typename Src, typename MaskT>
__simd_callee__ inline void VecRunUnary(Dst& dst, Src& src, MaskT& mask)
{
    using RegType = typename Dst::RegType;
    if constexpr (traits.mrg == MaskMergeMode::ZEROING) {
        Inst::template Run<RegType>(dst, src, mask);
    } else {
        RegType tmp;
        Inst::template Run<RegType>(tmp, src, mask);
        VecMergeIntoDst<Inst, RegType>(dst, tmp, mask);
    }
}

template <const VecTrait& traits, typename Inst, typename Dst, typename Src0, typename Src1, typename MaskT>
__simd_callee__ inline void VecRunBinary(Dst& dst, Src0& src0, Src1& src1, MaskT& mask)
{
    using RegType = typename Dst::RegType;
    if constexpr (traits.mrg == MaskMergeMode::ZEROING) {
        Inst::template Run<RegType>(dst, src0, src1, mask);
    } else {
        RegType tmp;
        Inst::template Run<RegType>(tmp, src0, src1, mask);
        VecMergeIntoDst<Inst, RegType>(dst, tmp, mask);
    }
}

} // namespace detail

template <typename Op, typename... Args>
struct VecTraits;

template <typename Op>
struct VecTraits<Op> {
    using OpType = Op;
    using TraitType = VecTrait;
    using RegEntry = VecInstOf<Op>;
    using Inst = typename RegEntry::Inst;
    static constexpr int arity = RegEntry::arity;
    static constexpr TraitType defaultTrait = vecTraitDefault;

    template <const TraitType& traits = defaultTrait, typename Dst, typename... Rest>
    __simd_callee__ inline static void Unpack(Dst& dst, Rest&... rest)
    {
        static_assert(static_cast<int>(sizeof...(Rest)) == arity + 1,
            "vec op operand count mismatch: expected `arity` source registers plus one mask.");
        UnpackImpl<traits, Dst, Rest...>(dst, rest...);
    }

private:
    template <const TraitType& traits, typename Dst, typename Src, typename MaskT>
    __simd_callee__ inline static void UnpackImpl(Dst& dst, Src& src, MaskT& mask)
    {
        static_assert(arity == 1, "internal arity mismatch for unary unpack");
        detail::VecRunUnary<traits, Inst>(dst, src, mask);
    }

    template <const TraitType& traits, typename Dst, typename Src0, typename Src1, typename MaskT>
    __simd_callee__ inline static void UnpackImpl(Dst& dst, Src0& src0, Src1& src1, MaskT& mask)
    {
        static_assert(arity == 2, "internal arity mismatch for binary unpack");
        detail::VecRunBinary<traits, Inst>(dst, src0, src1, mask);
    }
};

template <typename Op, typename TraitTag>
struct VecTraits<Op, TraitTag> : VecTraits<Op> {
    static constexpr const TraitType defaultTrait = TraitTag::value;
};

template <typename... Args>
struct VecAtom;

template <typename Op>
struct VecAtom<Op> : VecAtom<VecTraits<Op>> {};

template <typename... Args>
struct VecAtom<VecTraits<Args...>> : VecTraits<Args...> {
    using VecTraitsType = VecTraits<Args...>;
    using TraitType = typename VecTraitsType::TraitType;
    static constexpr const TraitType defaultTrait = VecTraitsType::defaultTrait;

    template <const TraitType& traits = defaultTrait, typename Dst, typename... Rest>
    __simd_callee__ inline void Call(Dst& dst, Rest&... rest) const
    {
        VecTraitsType::template Unpack<traits>(dst, rest...);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ATOM_VECTOR_VEC_ATOM_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
