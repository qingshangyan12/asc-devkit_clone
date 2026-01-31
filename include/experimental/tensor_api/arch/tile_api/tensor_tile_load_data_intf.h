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
 * \file tensor_tile_load_data_intf.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_ARCH_TILE_API_TENSOR_TILE_LOAD_DATA_INTF_H
#define INCLUDE_TENSOR_API_ARCH_TILE_API_TENSOR_TILE_LOAD_DATA_INTF_H

#include "impl/experimental/tensor_api/arch/tile_api/load_data/tensor_tile_load_data_impl.h"

namespace AscendC {
template<const LoadDataTrait& trait = DEFAULT_LOAD_DATA_TRAIT, typename T, typename U>
__aicore__ inline typename Std::enable_if<VerifyingLoadDataTemplate<T, U>, void>::type 
LoadData(const T& dst, const U& src);

template<const LoadDataTrait& trait = DEFAULT_LOAD_DATA_TRAIT, typename T, typename U, class Coord>
__aicore__ inline typename Std::enable_if<VerifyingLoadDataTemplateWithCoord<T, U, Coord>, void>::type 
LoadData(const T& dst, const U& src, const Coord& coord);
} // namespace AscendC
#endif // INCLUDE_TENSOR_API_ARCH_TILE_API_TENSOR_TILE_LOAD_DATA_INTF_H