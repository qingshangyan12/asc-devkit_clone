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
 * \file pointer.h
 * \brief Defines pointer types and construction functions.
 * \brief Provides memory pointer abstraction for different memory hierarchies.
 * \brief Supports GM, UB, L1, L0A/B/C pointer types with address qualifiers.
 * 
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_POINTER_H
#define INCLUDE_TENSOR_API_TENSOR_POINTER_H

namespace AscendC {
namespace Te {

/*
 * @brief Creates a memory pointer with hardware type and traits.
 * @brief Constructs pointer to specific memory location (GM, UB, L1, etc.).
 * @brief Trait or type parameter specifies memory type and data type.
 * @param [in] args Pointer construction arguments (address, offset, etc.)
 * @return Memory pointer object
 * @example
 * // Example: Create GM pointer
 * auto ptr = MakeMemPtr<GM, half>(address);
 * // Example: Create UB pointer
 * auto ptr = MakeMemPtr<UB, half>(bufferAddress);
 */
template <typename Hardware, typename TraitOrType, typename... Args>
__aicore__ inline constexpr auto MakeMemPtr(Args... args);

/*
 * @brief Creates a memory pointer with hardware type only.
 * @brief Simplified pointer construction without trait specification.
 * @brief Hardware parameter specifies memory hierarchy (GM, UB, L1, L0A/B/C).
 * @param [in] args Pointer construction arguments
 * @return Memory pointer object
 * @example
 * // Example: Create simple GM pointer
 * auto ptr = MakeMemPtr<GM>(address);
 */
template <typename Hardware, typename... Args>
__aicore__ inline constexpr auto MakeMemPtr(Args... args);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_TENSOR_POINTER_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
