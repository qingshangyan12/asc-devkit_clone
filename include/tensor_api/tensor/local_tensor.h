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
 * \file local_tensor.h
 * \brief Defines LocalTensor type and MakeTensor construction function.
 * \brief LocalTensor represents a tensor view on local memory (UB, L1, L0A/B/C).
 * \brief Provides unified tensor abstraction for hardware acceleration.
 * 
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_LOCAL_TENSOR_H
#define INCLUDE_TENSOR_API_TENSOR_LOCAL_TENSOR_H

// tensor construction
namespace AscendC {
namespace Te {

/*
 * @brief Creates a LocalTensor from iterator and layout arguments.
 * @brief Combines iterator (pointer to data in local memory) with layout specification.
 * @brief Automatically deduces tensor type based on iterator and layout parameters.
 * @param [in] iter Iterator pointing to data in local memory
 * @param [in] args Layout arguments (Shape, Stride, or Layout object)
 * @return LocalTensor with deduced types
 * @example
 * // Example 1: Create tensor from UB pointer with shape
 * __ubuf__ half* ptr;
 * auto tensor = MakeTensor(ptr, MakeShape(128, 64));
 * // Example 2: Create tensor with explicit layout
 * auto tensor = MakeTensor(ptr, MakeLayout(MakeShape(128, 64), MakeStride(64, 1)));
 */
template <typename Iterator, typename... Args>
__aicore__ inline constexpr auto MakeTensor(const Iterator& iter, const Args&... args);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_TENSOR_LOCAL_TENSOR_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
