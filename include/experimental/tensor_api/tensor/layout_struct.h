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
 * \file layout.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_LAYOUT_H
#define INCLUDE_TENSOR_API_TENSOR_LAYOUT_H

#include "impl/experimental/tensor_api/tensor/layout_struct.h"

namespace AscendC {
namespace Te {

// make_fractal.h
template <typename T, typename U, typename S>
struct MakeNzLayout;

struct MakeL0CLayout;

template <typename T, typename U, typename S>
struct MakeDNLayout;

template <typename T, typename U, typename S>
struct MakeNDLayout;

template <typename T, typename U, typename S>
struct MakeZnLayout;

template <typename T, typename U, typename S>
struct MakeZzLayout;

template <typename T, typename U, typename S>
struct MakeNnLayout;

template <typename T, typename U, typename S>
struct MakeScaleANDLayout;

template <typename T, typename U, typename S>
struct MakeScaleADNLayout;

template <typename T, typename U, typename S>
struct MakeScaleBNDLayout;

template <typename T, typename U, typename S>
struct MakeScaleBDNLayout;

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_TENSOR_LAYOUT_H
