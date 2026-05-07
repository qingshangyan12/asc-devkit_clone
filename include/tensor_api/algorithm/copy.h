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
 * \file copy.h
 * \brief Defines Copy algorithm for data movement operations.
 * \brief Provides high-level copy interface for transferring data between tensors.
 * \brief Supports copy operations across different memory hierarchies (GM, UB, L1, L0A/B/C).
 * 
 * \ingroup algorithm Level 0
 */

namespace AscendC {
namespace Te {

/*
 * @ingroup copy Level 0
 * @brief Performs copy operation with compile-time traits and atom operation.
 * @brief Copies data from source tensor to destination tensor using specified atom.
 * @brief Traits and atom parameters control copy behavior (stride, repeat, etc.).
 * @param [in] atomCopy CopyAtom defining copy operation parameters
 * @param [in] params Additional parameters (source, destination, etc.)
 * @return void
 * @example
 * // Example: Copy data from GM to UB
 * auto copyAtom = MakeCopy<GM2UB>();
 * Copy(copyAtom, gmTensor, ubTensor);
 */
template <typename Tp, const Tp& traits, typename T, typename... Params>
__aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params& ...params);

/*
 * @ingroup copy Level 0
 * @brief Performs copy operation with runtime atom parameters.
 * @brief Simplified version without compile-time traits specification.
 * @brief Copies data using atom operation parameters provided at runtime.
 * @param [in] atomCopy CopyAtom defining copy operation
 * @param [in] params Additional parameters (source, destination, etc.)
 * @return void
 * @example
 * // Example: Copy data with runtime atom
 * Copy(copyAtom, srcTensor, dstTensor);
 */
template <typename T, typename... Params>
__aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params& ...params);

/*
 * @ingroup copy Level 0
 * @brief Creates a CopyAtom with specified traits.
 * @brief Constructs atom operation for copy with compile-time configuration.
 * @brief Traits define copy operation type (GM2UB, UB2L1, L12L0A, etc.).
 * @param [in] traits Copy operation traits (type, stride, repeat parameters)
 * @return CopyAtom object with specified configuration
 * @example
 * // Example 1: Create GM to UB copy atom
 * auto copyAtom = MakeCopy<GM2UB>();
 * // Example 2: Create UB to L1 copy atom
 * auto copyAtom = MakeCopy<UB2L1>();
 */
template <typename... Args>
__aicore__ inline auto MakeCopy(const Args& ...traits);

}
}
}

#endif // INCLUDE_TENSOR_API_ALGORITHM_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
