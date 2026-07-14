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
* \file reg_tensor.h
* \brief Register operand construction in the same factory style as MakeTensor.
*
* Analogous trilogy at register granularity:
*   MakeRegTrait<1>()           ~  layout / trait descriptor (REG_NUM)
*   MakeRegTensor<half>()       ~  MakeTensor(ptr, layout) without memory binding
*   MakeMaskReg<half>(count)    ~  predicate for the 256B tile
*
* Example (full pipeline):
*   auto a = MakeRegTensor<half>();
*   auto dst = MakeRegTensor<half>();
*   auto m = MakeMaskReg<half>(elemCount);
*   Load(MakeLoad(load::UbAlign{}), a, ubPtr);
*   Compute(MakeVec(vec::Add{}), dst, a, b, m);
*   Store(MakeStore(load::UbStoreAlign{}), ubDst, dst, m);
*/
#ifndef INCLUDE_TENSOR_API_REG_REG_TENSOR_H
#define INCLUDE_TENSOR_API_REG_REG_TENSOR_H

#include "basic_api/reg_compute/kernel_reg_compute_struct_intf.h"
#include "impl/tensor_api/reg/reg_tensor_impl.h"

namespace AscendC {
namespace Te {

template <typename T, const Reg::RegTrait& Trait = Reg::RegTraitNumOne>
using RegTensor = Reg::RegTensor<T, Trait>;

template <int RegNum>
__aicore__ inline constexpr const Reg::RegTrait& MakeRegTrait();

template <typename T, const Reg::RegTrait& Trait = Reg::RegTraitNumOne>
__simd_callee__ inline RegTensor<T, Trait> MakeRegTensor();

template <typename T, typename U>
__simd_callee__ inline vector_bool MakeMaskReg(U& remaining);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_REG_REG_TENSOR_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
