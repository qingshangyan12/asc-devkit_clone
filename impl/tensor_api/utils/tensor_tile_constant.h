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
* \file tensor_tile_constant.h
* \brief
*/
#ifndef IMPL_TENSOR_API_UTILS_TENSOR_TILE_CONSTANT_H
#define IMPL_TENSOR_API_UTILS_TENSOR_TILE_CONSTANT_H

#include <cstdint>
#include <utility>
#include <type_traits>
#include "include/utils/std/tuple.h"
#include "include/utils/std/type_traits.h"
#include "include/utils/std/utility.h"

namespace AscendC{

enum class Hardware : uint8_t { GM, UB, L1, L0A, L0B, L0C, BIAS, FIXBUF, MAX };

namespace TileInternal{
constexpr size_t TWO_DIM_DATA = 2;
constexpr size_t FOUR_DIM_DATA = 4;
constexpr size_t C0_SIZE = 32;
constexpr size_t FRACTAL_FIXED = 16;
}
}

#endif // IMPL_TENSOR_API_UTILS_TENSORTILE_CONSTANT_H