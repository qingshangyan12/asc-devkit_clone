/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/**
 * @file copy.h
 * @brief Copy algorithm for TensorAPI.
 * 
 * @details
 * Unified data copy interface supporting different memory space transfers.
 */
#ifndef INCLUDE_TENSOR_API_ALGORITHM_COPY_H
#define INCLUDE_TENSOR_API_ALGORITHM_COPY_H

#include "impl/tensor_api/algorithm/copy_impl.h"

namespace AscendC {
namespace Te {

/**
 * @brief Execute copy operation with traits.
 * 
 * @details
 * Data copy between memory spaces using CopyAtom.
 * Supports: GM2L1, L12L0A/B, L12BT/FB, L0C2GM/UB.
 * 
 * @tparam Tp Traits type
 * @tparam T Atom type
 * @tparam Params Source and destination tensors
 * @param[in] atomCopy CopyAtom with operation and traits
 * @param[in] params Destination tensor, source tensor
 * 
 * @example
 * using namespace AscendC::Te;
 * // Method 1: Direct call with CopyAtom
 * Copy(CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}, globalC, l0CTensor);
 * 
 * // Method 2: Atom.Call method
 * CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}.Call(globalC, l0CTensor);
 * 
 * // Method 3: MakeCopy factory
 * auto atomCopyL0C2GM = MakeCopy(CopyL0C2GM{}, FixpipeTraitDefault{});
 * atomCopyL0C2GM.Call(globalC, l0CTensor);
 */
template <typename Tp, const Tp& traits, typename T, typename... Params>
__aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params&... params);

/**
 * @brief Execute copy operation with default traits.
 * 
 * @tparam T Atom type
 * @tparam Params Source and destination tensors
 * @param[in] atomCopy CopyAtom configuration
 * @param[in] params Source tensor, destination tensor
 */
template <typename T, typename... Params>
__aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params&... params);

/**
 * @brief Create CopyAtom with traits.
 * 
 * @tparam Args Trait types
 * @param[in] traits Copy traits (CopyL0C2GM, FixpipeTraitDefault, etc.)
 * @return CopyAtom object
 */
template <typename... Args>
__aicore__ inline auto MakeCopy(const Args&... traits);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ALGORITHM_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif