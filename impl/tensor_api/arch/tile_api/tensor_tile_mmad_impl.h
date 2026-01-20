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
 * \file tensor_tile_mmad_impl.h.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_TENSOR_TILE_MMAD_IMPL_H
#define IMPL_TENSOR_API_TENSOR_TILE_MMAD_IMPL_H
namespace AscendC {
struct MmadTrait {
    int32_t fmOffset = 0;
    bool enSsparse = false;
    bool enWinogradA = false;
    bool enWinogradB = false;
    int8_t unitFlag = 0;
    bool kDirectionAlign = false;
    bool cmatrixSource = false;
    bool cmatrixInitVal = false;
};
constexpr MmadTrait DEFAULT_MMAD_TRAIT; 
}
#include "mmad/tensor_tile_mmad_routing.h"

namespace AscendC {

template <const MmadTrait& trait = DEFAULT_MMAD_TRAIT, typename T, typename U, typename S>
__aicore__ inline typename Std::enable_if<VerifyingMmadTemplate<T, U, S>, void>::type 
Mmad(const T& dst, const U& fm, const S& filter)
{
   constexpr Hardware dstPos = TileInternal::GetHardPos<T>();
   constexpr Hardware fmPos = TileInternal::GetHardPos<U>();
   constexpr Hardware filterPos = TileInternal::GetHardPos<S>();
   using Tensor2Tensor = typename TileInternal::MmadTensor2Tensor<dstPos, fmPos, filterPos, 
      TileInternal::CURRENT_ARCH_VERSION, TileInternal::FOUR_DIM_DATA>::type;
   Tensor2Tensor{}.template Run<T, U, S, trait>(dst, fm, filter);
}

} // namespace AscendC
#endif // IMPL_TENSOR_API_TENSOR_TILE_MMAD_IMPL_H