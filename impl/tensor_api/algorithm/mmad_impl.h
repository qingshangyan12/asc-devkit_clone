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
    "impl/tensor_api/algorithm/mmad_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file mmad_impl.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ALGORITHM_MMAD_IMPL_H
#define IMPL_TENSOR_API_ALGORITHM_MMAD_IMPL_H

#include "impl/tensor_api/atom/mmad_atom_impl.h"

namespace AscendC {
namespace Te {

template <typename Tp, const Tp& traits, typename T, typename Dst, typename Fm, typename Filter>
__aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Dst& dst, const Fm& fm, const Filter& filter)
{
    atomMmad.template Call<traits>(dst, fm, filter);
}

template <typename Tp, const Tp& traits, typename T, typename Dst, typename Fm, typename Filter>
__aicore__ inline void Mmad(
    const MmadAtom<T>& atomMmad, const Dst& dst, const Fm& fm, const Filter& filter, const MmadParams& params)
{
    atomMmad.template Call<traits>(dst, fm, filter, params);
}

template <typename Tp, const Tp& traits, typename T, typename Dst, typename Fm, typename Filter, typename Bias,
    Std::enable_if_t<IsAttrTensorV<Bias>, int> = 0>
__aicore__ inline void Mmad(
    const MmadAtom<T>& atomMmad, const Dst& dst, const Fm& fm, const Filter& filter, const Bias& bias)
{
    atomMmad.template Call<traits>(dst, fm, filter, bias);
}

template <typename Tp, const Tp& traits, typename T, typename Dst, typename Fm, typename Filter, typename Bias,
    Std::enable_if_t<IsAttrTensorV<Bias>, int> = 0>
__aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Dst& dst, const Fm& fm, const Filter& filter,
    const Bias& bias, const MmadParams& params)
{
    atomMmad.template Call<traits>(dst, fm, filter, bias, params);
}

template <typename T, typename Dst, typename Fm, typename Filter>
__aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Dst& dst, const Fm& fm, const Filter& filter)
{
    atomMmad.Call(dst, fm, filter);
}

template <typename T, typename Dst, typename Fm, typename Filter>
__aicore__ inline void Mmad(
    const MmadAtom<T>& atomMmad, const Dst& dst, const Fm& fm, const Filter& filter, const MmadParams& params)
{
    atomMmad.Call(dst, fm, filter, params);
}

template <typename T, typename Dst, typename Fm, typename Filter, typename Bias,
    Std::enable_if_t<IsAttrTensorV<Bias>, int> = 0>
__aicore__ inline void Mmad(
    const MmadAtom<T>& atomMmad, const Dst& dst, const Fm& fm, const Filter& filter, const Bias& bias)
{
    atomMmad.Call(dst, fm, filter, bias);
}

template <typename T, typename Dst, typename Fm, typename Filter, typename Bias,
    Std::enable_if_t<IsAttrTensorV<Bias>, int> = 0>
__aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Dst& dst, const Fm& fm, const Filter& filter,
    const Bias& bias, const MmadParams& params)
{
    atomMmad.Call(dst, fm, filter, bias, params);
}

template <typename T, typename U, typename S>
__aicore__ inline void Mmad(const T& dst, const U& fm, const S& filter, const MmadParams& params)
{
    MmadAtom<MmadOperation>{}.Call(dst, fm, filter, params);
}

template <typename T, typename U, typename S, typename V,
    Std::enable_if_t<IsAttrTensorV<T> && IsAttrTensorV<U> && IsAttrTensorV<S> && IsAttrTensorV<V>, int> = 0>
__aicore__ inline void Mmad(const T& dst, const U& fm, const S& filter, const V& bias, const MmadParams& params)
{
    MmadAtom<MmadOperation>{}.Call(dst, fm, filter, bias, params);
}

template <typename MmadOperationType>
__aicore__ inline constexpr auto MakeMmad(const MmadOperationType&)
{
    return MmadAtom<MmadTraits<MmadOperationType>>{};
}

template <typename MmadOperationType, typename Trait>
__aicore__ inline constexpr auto MakeMmad(const MmadOperationType&, const Trait&)
{
    return MmadAtom<MmadTraits<MmadOperationType, Trait>>{};
}

}
}

#endif // IMPL_TENSOR_API_ALGORITHM_MMAD_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
