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
* \brief
*/
#ifndef INCLUDE_TENSOR_API_ARCH_CUBE_COPY_OP_H
#define INCLUDE_TENSOR_API_ARCH_CUBE_COPY_OP_H

namespace AscendC {
namespace Te {

/**
 * @brief Copy operation that transfers data from Global Memory to L1 Buffer.
 */
struct CopyGM2L1;

/**
 * @brief Copy operation that transfers data from L1 Buffer to Unified Buffer.
 */
struct CopyL12UB;

/**
 * @brief Copy operation that transfers data from L1 Buffer to L0B Buffer.
 */
struct CopyL12L0B;

/**
 * @brief Copy operation that transfers data from L1 Buffer to L0A Buffer.
 */
struct CopyL12L0A;

/**
 * @brief Copy operation that transfers data from L1 Buffer to L0ScaleA Buffer.
 */
struct CopyL12L0ScaleA;

/**
 * @brief Copy operation that transfers data from L1 Buffer to L0ScaleB Buffer.
 */
struct CopyL12L0ScaleB;

/**
 * @brief Copy operation that transfers data from L1 Buffer to Fixpipe Buffer.
 */
struct CopyL12FB;

/**
 * @brief Copy operation that transfers data from L1 Buffer to Bias Table Buffer.
 */
struct CopyL12BT;

/**
 * @brief Copy operation that transfers data from L0C Buffer to Global Memory.
 */
struct CopyL0C2GM;

/**
 * @brief L0C-to-Global-Memory copy operation that supports bound runtime parameters.
 */
struct CopyL0C2GMWith;

/**
 * @brief Copy operation that transfers data from L0C Buffer to Unified Buffer.
 */
struct CopyL0C2UB;

/**
 * @brief L0C-to-Unified-Buffer copy operation that supports bound runtime parameters.
 */
struct CopyL0C2UBWith;

/**
 * @brief Copy operation that transfers data from L0C Buffer to L1 Buffer.
 */
struct CopyL0C2L1;

/**
 * @brief L0C-to-L1-Buffer copy operation that supports bound runtime parameters.
 */
struct CopyL0C2L1With;

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ARCH_CUBE_COPY_OP_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
