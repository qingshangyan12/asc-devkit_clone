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
 * \file copy_op.h
 * \brief Defines Vector unit copy operation types.
 * \brief Provides copy operation structures for Vector-specific data movement.
 * \brief Supports copy operations between GM, UB, L1.
 * 
 * \ingroup arch Level 0
 */

namespace AscendC {
namespace Te {

/*
 * @ingroup copy_op Level 0
 * @brief Copy operation: Global Memory to Unified Buffer.
 * @brief Loads data from GM to UB for Vector processing.
 */
struct CopyGM2UB;

/*
 * @ingroup copy_op Level 0
 * @brief Copy operation: Unified Buffer to L1 Buffer.
 * @brief Transfers data from UB to L1 for Cube processing.
 */
struct CopyUB2L1;

/*
 * @ingroup copy_op Level 0
 * @brief Copy operation: Unified Buffer to Global Memory.
 * @brief Writes Vector computation results back to GM.
 */
struct CopyUB2GM;

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ARCH_VECTOR_COPY_OP_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
