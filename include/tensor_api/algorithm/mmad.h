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

/**
 * @file mmad.h
 * @brief Matrix multiplication and accumulation (Mmad) for TensorAPI.
 * 
 * @details
 * Matrix multiply-accumulate operations: C += A * B or C = A * B + Bias.
 */
#ifndef INCLUDE_TENSOR_API_ALGORITHM_MMAD_H
#define INCLUDE_TENSOR_API_ALGORITHM_MMAD_H

#include "impl/tensor_api/algorithm/mmad_impl.h"

namespace AscendC {
namespace Te {

/**
 * @brief Execute Mmad operation with traits.
 * 
 * @details
 * Matrix multiplication using MmadAtom.
 * Supports: half, float, int8, bfloat16, fp8, hifloat8.
 * 
 * @tparam Tp MmadTrait type
 * @tparam T Atom type
 * @tparam Params Input and output tensors
 * @param[in] atomMad MmadAtom configuration
 * @param[in] params dst tensor C, fm tensor A, filter tensor B, bias tensor (optional), MmadParams
 * 
 * @example
 * using namespace AscendC::Te;
 * constexpr int32_t L0_MAX = 128 * 128 * 2;
 * constexpr int32_t L0C_MAX = 128 * 128;
 * 
 * size_t m = 128, n = 64, k = 64;
 * auto l0ALayout = MakeNzLayout<half>(m, k);
 * auto l0BLayout = MakeZnLayout<half>(k, n);
 * auto l0CLayout = MakeL0CLayout(m, n);
 * 
 * __ca__ half l0AAddr[L0_MAX];
 * __cb__ half l0BAddr[L0_MAX];
 * __cc__ float l0CAddr[L0C_MAX];
 * 
 * auto l0ATensor = MakeTensor(MakeL0AmemPtr(l0AAddr), l0ALayout);
 * auto l0BTensor = MakeTensor(MakeL0BmemPtr(l0BAddr), l0BLayout);
 * auto l0CTensor = MakeTensor(MakeL0CmemPtr(l0CAddr), l0CLayout);
 * 
 * MmadParams para;
 * para.m = m; para.n = n; para.k = k;
 * para.cmatrixInitVal = true;
 * 
 * // Method 1: Direct call
 * Mmad(MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>{}, 
 *      l0CTensor, l0ATensor, l0BTensor, para);
 * 
 * // Method 2: Atom.Call method
 * MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>{}.Call(
 *     l0CTensor, l0ATensor, l0BTensor, para);
 * 
 * // Method 3: MakeMmad factory
 * auto atomMad = MakeMmad(MmadOperation{}, MmadTraitDefault{});
 * atomMad.Call(l0CTensor, l0ATensor, l0BTensor, para);
 */
template <typename Tp, const Tp& traits, typename T, typename... Params>
__aicore__ inline void Mmad(const MmadAtom<T>& atomMad, const Params&... params);

/**
 * @brief Execute Mmad operation with default traits.
 * 
 * @tparam T Atom type
 * @tparam Params Input and output tensors
 * @param[in] atomMad MmadAtom configuration
 * @param[in] params dst, fm, filter tensors and params
 */
template <typename T, typename... Params>
__aicore__ inline void Mmad(const MmadAtom<T>& atomMad, const Params&... params);

/**
 * @brief Create MmadAtom with traits.
 * 
 * @tparam Args Trait types
 * @param[in] traits Mmad traits (MmadOperation, MmadTraitDefault, etc.)
 * @return MmadAtom object
 */
template <typename... Args>
__aicore__ inline auto MakeMmad(const Args&... traits);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ALGORITHM_MMAD_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif