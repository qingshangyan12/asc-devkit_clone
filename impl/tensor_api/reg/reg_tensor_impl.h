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
#warning "impl/tensor_api/reg/reg_tensor_impl.h is an internal header file."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_TENSOR_API_REG_REG_TENSOR_IMPL_H
#define IMPL_TENSOR_API_REG_REG_TENSOR_IMPL_H

#include "impl/tensor_api/arch/vector/utils/mask_utils.h"

namespace AscendC {
namespace Te {

template <int RegNum>
struct RegTraitOf {
    static_assert(RegNum == 1 || RegNum == 2, "MakeRegTrait only supports REG_NUM 1 or 2.");
    static constexpr Reg::RegTrait value{RegNum};
};

template <>
struct RegTraitOf<1> {
    static constexpr Reg::RegTrait value = Reg::RegTraitNumOne;
};

template <>
struct RegTraitOf<2> {
    static constexpr Reg::RegTrait value = Reg::RegTraitNumTwo;
};

template <int RegNum>
__aicore__ inline constexpr const Reg::RegTrait& MakeRegTrait()
{
    return RegTraitOf<RegNum>::value;
}

template <typename T, const Reg::RegTrait& Trait>
__simd_callee__ inline RegTensor<T, Trait> MakeRegTensor()
{
    return RegTensor<T, Trait>{};
}

template <typename T, typename U>
__simd_callee__ inline vector_bool MakeMaskReg(U& remaining)
{
    return Inst::UpdateMask::Run<T>(remaining);
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_REG_REG_TENSOR_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
