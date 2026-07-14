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
#warning "impl/tensor_api/algorithm/reg_load_impl.h is an internal header file."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_TENSOR_API_ALGORITHM_REG_LOAD_IMPL_H
#define IMPL_TENSOR_API_ALGORITHM_REG_LOAD_IMPL_H

#include "impl/tensor_api/atom/vector/reg_load_atom_impl.h"

namespace AscendC {
namespace Te {

template <typename OpType>
__aicore__ inline constexpr auto MakeLoad(const OpType&)
{
    return LoadAtom<OpType>{};
}

template <typename OpType>
__aicore__ inline constexpr auto MakeStore(const OpType&)
{
    return StoreAtom<OpType>{};
}

template <typename AtomType, typename Dst, typename T>
__simd_callee__ inline void Load(const LoadAtom<AtomType>& atom, Dst& dst, __ubuf__ T* src)
{
    atom.Call(dst, src);
}

template <typename AtomType, typename T, typename Src, typename MaskT>
__simd_callee__ inline void Store(const StoreAtom<AtomType>& atom, __ubuf__ T* dst, Src& src, MaskT& mask)
{
    atom.Call(dst, src, mask);
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ALGORITHM_REG_LOAD_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
