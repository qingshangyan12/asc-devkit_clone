/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file format.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_UTILS_FORMAT_H
#define INCLUDE_TENSOR_API_UTILS_FORMAT_H

#include "impl/experimental/tensor_api/utils/format_impl.h"

namespace AscendC {
namespace Te {

struct ArchVersion {};

struct GetArchVersion {};

template <typename T>
__aicore__ inline uint8_t GetCacheModeFromTensor(__gm__ T* src);

// IsIntegralConstant
template <typename T>
struct IsIntegralConstant;

template <typename T>
constexpr bool IsIntegralConstantV;

template <typename T>
struct GetTypeFromFourDimTrait;

template <typename T>
struct IsZZFormat {};

template <typename T>
struct IsZNFormat {};

template <typename T>
struct IsNZFormat {};

template <typename T>
struct IsL0cNZFormat {};

template <typename T>
struct IsNDFormat {};

template <typename T>
struct IsDNFormat {};

template<typename T>
__aicore__ inline constexpr auto GetHardPos();

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_UTILS_FORMAT_H
