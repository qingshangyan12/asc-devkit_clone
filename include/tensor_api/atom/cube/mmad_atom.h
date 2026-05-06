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
 * \file mmad_atom.h
 * \brief Defines MmadAtom structure for matrix multiply-add operations.
 * \brief MmadAtom encapsulates MMAD operation configuration and traits.
 * \brief Provides atom-level abstraction for Cube unit matrix computations.
 */
#ifndef INCLUDE_TENSOR_API_ATOM_CUBE_MMAD_ATOM_H	 
#define INCLUDE_TENSOR_API_ATOM_CUBE_MMAD_ATOM_H	 

#include "impl/tensor_api/atom/mmad_atom_impl.h"
namespace AscendC {
namespace Te {

/*
 * @brief Defines traits for MMAD operations.
 * @brief Encapsulates matrix dimensions (M, N, K) and data formats.
 * @brief Used for compile-time MMAD configuration.
 */
template <typename MmadOperation, typename... MmadOpArgs>
struct MmadTraits;

/*
 * @brief Base structure for MmadAtom.
 * @brief Atom-level operation for matrix multiply-add.
 * @brief Combines operation type with execution parameters.
 */
template <typename... Args>
struct MmadAtom;

/*
 * @brief MmadAtom with single operation type.
 * @brief Defines atom for specific MMAD operation.
 * @param [in] MmadOperation MMAD operation type
 */
template <typename MmadOperation>
struct MmadAtom<MmadOperation>;

/*
 * @brief MmadAtom constructed from MmadTraits.
 * @brief Creates atom from traits specification.
 * @brief Enables flexible MMAD operation configuration.
 * @param [in] MmadTraits Traits defining matrix parameters
 */
template <typename MmadOperation, typename... Args>
struct MmadAtom<MmadTraits<MmadOperation, Args...>>;

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ATOM_CUBE_MMAD_ATOM_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
