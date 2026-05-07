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
 * \file copy_atom.h
 * \brief Defines CopyAtom structure for copy operation parameters.
 * \brief CopyAtom encapsulates copy operation configuration and traits.
 */
#ifndef INCLUDE_TENSOR_API_ATOM_CUBE_COPY_ATOM_H
#define INCLUDE_TENSOR_API_ATOM_CUBE_COPY_ATOM_H

#include "impl/tensor_api/atom/copy_atom_impl.h"

namespace AscendC {
namespace Te {

/*
 * @brief Defines traits for copy operations.
 */
template <typename CopyOperation, typename... CopyOpArgs>
struct CopyTraits;

/*
 * @brief Base structure for CopyAtom.
 */
template <typename... Args>
struct CopyAtom;

/*
 * @brief CopyAtom with single operation type.
 */
template <typename CopyOperation>
struct CopyAtom<CopyOperation>;

/*
 * @brief CopyAtom constructed from CopyTraits.
 */
template <typename... Args>
struct CopyAtom<CopyTraits<Args...>>;

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ATOM_CUBE_COPY_ATOM_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif