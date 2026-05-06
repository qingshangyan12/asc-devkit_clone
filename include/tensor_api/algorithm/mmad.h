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
 * \brief Supports matrix multiplication with accumulation for AI Core acceleration.
 * 
 * \ingroup algorithm Level 0
 */

namespace AscendC {
namespace Te {

/*
 * @ingroup mmad Level 0
 * @brief Performs matrix multiply-add with compile-time traits.
 * @brief Executes M = A * B + C (or M = A * B) using Cube unit.
 * @brief Traits and atom parameters control matrix dimensions and data layout.
 * @param [in] atomMmad MmadAtom defining matrix multiply parameters
 * @param [in] params Input matrices (A, B) and optional accumulator (C)
 * @return void (result stored in output tensor)
 * @example
 * // Example: Matrix multiply A * B
 * auto mmadAtom = MakeMmad<MMAD>();
 * Mmad(mmadAtom, tensorA, tensorB, tensorC);
 */
template <typename Tp, const Tp& traits, typename T, typename... Params>
__aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Params& ...params);

/*
 * @ingroup mmad Level 0
 * @brief Performs matrix multiply-add with runtime atom parameters.
 * @brief Simplified version without compile-time traits specification.
 * @brief Executes matrix multiplication using atom parameters provided at runtime.
 * @param [in] atomMmad MmadAtom defining matrix multiply operation
 * @param [in] params Input matrices and configuration parameters
 * @return void
 * @example
 * // Example: Mmad with runtime atom
 * Mmad(mmadAtom, tensorA, tensorB);
 */
template <typename T, typename... Params>
__aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Params& ...params);

/*
 * @ingroup mmad Level 0
 * @brief Creates an MmadAtom with specified traits.
 * @brief Constructs atom operation for matrix multiply-add.
 * @brief Traits define matrix dimensions (M, N, K) and data formats.
 * @param [in] traits Mmad operation traits (dimensions, format, etc.)
 * @return MmadAtom object with specified configuration
 * @example
 * // Example 1: Create Mmad atom for 16x16x16 matrix
 * auto mmadAtom = MakeMmad<MMAD>(16, 16, 16);
 * // Example 2: Create Mmad atom with NZ format
 * auto mmadAtom = MakeMmad<MMAD_NZ>();
 */
template <typename... Args>
__aicore__ inline auto MakeMmad(const Args& ...traits);

}
}
}

#endif // INCLUDE_TENSOR_API_ALGORITHM_MMAD_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
