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
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif

/*!
* \file vector_compute.h
* \brief Public interface for register-level (256B) element-wise vector compute.
*
* Three ways to use it, from most explicit to most concise:
*
*   // 1) Atom + verb (reusable, configurable):
*   auto a = MakeRegTensor<half>();
*   auto b = MakeRegTensor<half>();
*   auto dst = MakeRegTensor<half>();
*   auto mask = MakeMaskReg<half>(elemCount);
*   auto atom = MakeVec(vec::Add{});                 // or MakeVec(vec::Add{}, VecTrait{MaskMergeMode::MERGING})
*   Compute(atom, dst, a, b, mask);                 // binary
*   Compute(MakeVec(vec::Abs{}), dst, src, mask);    // unary, same verb
*
*   // 2) Generic over the operation (op is a type):
*   template <typename Op> void Elt(...) { Compute(MakeVec(Op{}), dst, a, b, mask); }
*
*   // 3) Semantic sugar (defined via a single macro in vector_compute_impl.h):
*   Add(dst, src0, src1, mask);  Mul(dst, a, b, mask);  Abs(dst, src, mask);  Exp(dst, src, mask);
*
* Operand convention: dst first, then source register(s), then the predicate mask register.
* Construct registers with MakeRegTensor<T>() (see tensor_api/reg/reg_tensor.h); dual-register
* operands use MakeRegTensor<T, Reg::RegTraitNumTwo>() or MakeRegTrait<2>().
*/
#ifndef INCLUDE_TENSOR_API_ALGORITHM_VECTOR_COMPUTE_H
#define INCLUDE_TENSOR_API_ALGORITHM_VECTOR_COMPUTE_H

#include "tensor_api/arch/vector/vec_op.h"
#include "tensor_api/atom/vector/vec_atom.h"
#include "impl/tensor_api/algorithm/vector_compute_impl.h"

namespace AscendC {
namespace Te {

/**
 * @brief Run one element-wise vector instruction with a preconstructed atom.
 * @param atom : Atom built by MakeVec (carries the op tag + trait).
 * @param dst  : Destination register (RegTensor).
 * @param rest : The source register(s) for this op's arity, followed by the mask register.
 */
template <typename AtomType, typename Dst, typename... Rest>
__simd_callee__ inline void Compute(const VecAtom<AtomType>& atom, Dst& dst, Rest&... rest);

/**
 * @brief Construct a vector-compute atom from an operation tag.
 * @param op : Operation tag object (e.g. vec::Add{}, vec::Abs{}).
 */
template <typename OpType>
__aicore__ inline constexpr auto MakeVec(const OpType& op);

/**
 * @brief Construct a vector-compute atom from an operation tag and a trait object.
 * @param op    : Operation tag object (e.g. vec::Add{}).
 * @param trait : Compile-time behavior (mask merge mode, ...).
 */
template <typename OpType>
__aicore__ inline constexpr auto MakeVec(const OpType& op, const VecTrait& trait);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ALGORITHM_VECTOR_COMPUTE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
