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
 * \file layout_struct.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_LAYOUT_STRUCT_H
#define INCLUDE_TENSOR_API_TENSOR_LAYOUT_STRUCT_H

#include "impl/experimental/tensor_api/tensor/layout_struct.h"

namespace AscendC {
namespace Te {

// make_fractal.h: MakeNzLayout 同时有函数 (T,U,S)(row,col) 和仿函数 MakeNzLayout<T>{}(row,col)，前向声明省略以免与函数声明冲突
template <typename U, typename S>
struct MakeL0CLayoutOp;

template <typename T, typename U, typename S>
struct MakeDNLayoutOp;

template <typename T, typename U, typename S>
struct MakeNDLayoutOp;

template <typename T, typename U, typename S>
struct MakeZnLayoutOp;

template <typename T, typename U, typename S>
struct MakeZzLayoutOp;

template <typename T, typename U, typename S>
struct MakeNnLayoutOp;

template <typename T, typename U, typename S>
struct MakeScaleANDLayoutOp;

template <typename T, typename U, typename S>
struct MakeScaleADNLayoutOp;

template <typename T, typename U, typename S>
struct MakeScaleBNDLayoutOp;

template <typename T, typename U, typename S>
struct MakeScaleBDNLayoutOp;

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_TENSOR_LAYOUT_STRUCT_H
