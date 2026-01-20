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
 * \file tensor_tile_mmad_routing.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_TILE_API_MMAD_TENSOR_TILE_MMAD_ROUTING_H
#define IMPL_TENSOR_API_ARCH_TILE_API_MMAD_TENSOR_TILE_MMAD_ROUTING_H

#include "tensor_tile_mmad_four_dim_2201.h"

namespace AscendC {
namespace TileInternal {

class MmadIgnore
{
public:
    template <typename T, typename U, typename S, const MmadTrait& trait>
    __aicore__ inline void Run(const T& dst, const U& fm, const S& filter) {}
};

template<Hardware dstPos, Hardware fmPos, Hardware filterPos, uint32_t Version, size_t dimension>
struct MmadTensor2Tensor
{
    using type = MmadIgnore;
};

template<>
struct MmadTensor2Tensor<Hardware::L0C, Hardware::L0A, Hardware::L0B, ArchVersion::V2201, FOUR_DIM_DATA>
{
    using type = MmadFourDim2201;
};

} // namespace TileInternal
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_TILE_API_MMAD_TENSOR_TILE_MMAD_ROUTING_H