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
 * @file mmad_atom.h
 * @brief MmadAtom for matrix multiplication operations.
 * 
 * @details
 * Template structures for configuring matrix multiply-accumulate operations.
 */
#ifndef INCLUDE_TENSOR_API_ATOM_CUBE_MMAD_ATOM_H
#define INCLUDE_TENSOR_API_ATOM_CUBE_MMAD_ATOM_H

#include "impl/tensor_api/atom/mmad_atom_impl.h"

namespace AscendC {
namespace Te {

/**
 * @brief Mmad traits for operation configuration.
 * 
 * @tparam MmaOperation Mmad operation type (MmadOperation)
 * @tparam MmaOpArgs Additional traits (MmadTrait, MmadTraitDefault)
 * 
 * @example
 * MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>
 */
template <typename MmaOperation, typename... MmaOpArgs>
struct MmadTraits;

/**
 * @brief Mmad atom for matrix multiplication.
 * 
 * @details
 * Encapsulates mmad operation and traits for matrix computation.
 * 
 * @tparam Args Operation and trait types
 * 
 * @example
 * // Create MmadAtom for matrix multiplication
 * auto atom = MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>{};
 * MmadParams para;
 * para.m = 128; para.n = 64; para.k = 64;
 * atom.Call(dstTensor, fmTensor, filterTensor, para);
 */
template <typename... Args>
struct MmadAtom;

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ATOM_CUBE_MMAD_ATOM_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif