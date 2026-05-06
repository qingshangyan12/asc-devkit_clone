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
 * @file pointer.h
 * @brief Memory pointer interface for TensorAPI.
 * 
 * @details
 * Factory functions for creating memory pointers to different hardware memory spaces.
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_POINTER_H
#define INCLUDE_TENSOR_API_TENSOR_POINTER_H

#include "impl/tensor_api/tensor/pointer_pattern.h"

namespace AscendC {
namespace Te {

/**
 * @brief Create memory pointer with trait or type specification.
 * 
 * @tparam HardWare Hardware type (UB, L1, GM, L0A, L0B, L0C)
 * @tparam TraitOrType Trait or data type for pointer
 * @tparam Args Constructor arguments
 * @param[in] args Pointer initialization arguments
 * @return MemPtr object pointing to specified memory space
 * 
 * @example
 * __cbuf__ half data[1024];
 * auto l1Ptr = MakeL1memPtr<half>(data);
 * auto ubPtr = MakeUBmemPtr<half>(ubBuffer);
 * auto gmPtr = MakeGMmemPtr<half>(gmAddr);
 */
template <typename HardWare, typename TraitOrType, typename... Args>
__aicore__ inline constexpr auto MakeMemPtr(Args... args);

/**
 * @brief Create memory pointer with default type.
 * 
 * @tparam HardWare Hardware type
 * @tparam Args Constructor arguments
 * @param[in] args Pointer initialization arguments
 * @return MemPtr object
 */
template <typename HardWare, typename... Args>
__aicore__ inline constexpr auto MakeMemPtr(Args... args);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_TENSOR_POINTER_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif