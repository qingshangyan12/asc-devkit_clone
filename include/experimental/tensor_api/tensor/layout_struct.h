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

struct MakeL0CLayoutOp;

template <typename T>
struct MakeDNLayoutOp;

template <typename T>
struct MakeNDLayoutOp;

template <typename T>
struct MakeZnLayoutOp;

template <typename T>
struct MakeZzLayoutOp;

template <typename T>
struct MakeNnLayoutOp;

template <typename T>
struct MakeScaleANDLayoutOp;

template <typename T>
struct MakeScaleADNLayoutOp;

template <typename T>
struct MakeScaleBNDLayoutOp;

template <typename T>
struct MakeScaleBDNLayoutOp;

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_TENSOR_LAYOUT_STRUCT_H
