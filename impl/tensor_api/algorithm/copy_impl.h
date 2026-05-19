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

namespace AscendC {
namespace Te {

template <typename Quant>
constexpr bool IsQuantScalarV =
    !IsAttrTensorV<Quant> && Std::is_convertible_v<Std::remove_cvref_t<Quant>, uint64_t>;

template <typename Quant>
constexpr bool IsCopyQuantParamV = IsQuantScalarV<Quant> || IsAttrTensorV<Quant>;

template <typename T, typename Dst, typename Src>
__aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Dst& dst, const Src& src)
{
    atomCopy.Call(dst, src);
}

template <typename T, typename Dst, typename Src, typename Coord,
    Std::enable_if_t<Std::is_tuple_v<Std::remove_cvref_t<Coord>>, int> = 0>
__aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Dst& dst, const Src& src,
    const Coord& coord)
{
    atomCopy.Call(dst, src, coord);
}

template <typename T, typename Dst, typename Src, typename Quant,
    Std::enable_if_t<IsCopyQuantParamV<Quant>, int> = 0>
__aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Dst& dst, const Src& src,
    const Quant& quant)
{
    atomCopy.Call(dst, src, quant);
}

template <typename CopyOperation>
__aicore__ inline constexpr auto MakeCopy()
{
    return CopyAtom<CopyTraits<CopyOperation>>{};
}

template <typename CopyOperation, typename Trait>
__aicore__ inline constexpr auto MakeCopy()
{
    return CopyAtom<CopyTraits<CopyOperation, Trait>>{};
}

}
}

#endif // IMPL_TENSOR_API_ALGORITHM_COPY_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
