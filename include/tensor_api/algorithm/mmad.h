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
 * \file mmad.h
 * \brief Defines Mmad algorithm for matrix multiply-add operations.
 * \brief Provides high-level interface for Cube unit matrix computations.
 */
#ifndef INCLUDE_TENSOR_API_ALGORITHM_MMAD_H
#define INCLUDE_TENSOR_API_ALGORITHM_MMAD_H

#include "impl/tensor_api/algorithm/mmad_impl.h"

namespace AscendC {
namespace Te {

/*
 * @brief Performs matrix multiply-add with compile-time traits.
 * @brief Executes M = A * B + C (or M = A * B) using Cube unit.
 * @param [in] atomMmad MmadAtom defining matrix multiply parameters
 * @param [in] params Input matrices (A, B) and optional accumulator (C)
 * @example
 * auto mmadAtom = MakeMmad<MmadOperation>();
 * Mmad(mmadAtom, tensorA, tensorB, tensorC);
 */
template <typename Tp, const Tp& traits, typename T, typename... Params>
__aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Params& ...params);

/*
 * @brief Performs matrix multiply-add with runtime atom parameters.
 * @brief Simplified version without compile-time traits specification.
 * @param [in] atomMmad MmadAtom defining matrix multiply operation
 * @param [in] params Input matrices and configuration parameters
 * @example
 * Mmad(mmadAtom, tensorA, tensorB);
 */
template <typename T, typename... Params>
__aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Params& ...params);

/*
 * @brief Creates an MmadAtom with specified traits.
 * @brief Constructs atom operation for matrix multiply-add.
 * @param [in] traits Mmad operation traits (dimensions, format, etc.)
 * @return MmadAtom object with specified configuration
 * @example
 * // Create MMAD atom for matrix multiply
 * auto mmadAtom = MakeMmad<MmadOperation>();
 */
template <typename... Args>
__aicore__ inline auto MakeMmad(const Args& ...traits);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ALGORITHM_MMAD_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif