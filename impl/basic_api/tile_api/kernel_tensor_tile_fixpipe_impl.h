/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file kernel_tensor_tile_fixpipe_impl.h.h
 * \brief
 */
#if !defined(_ASCENDC_INCLUDE_INTERNAL_HEADERS_)
#warning                                                                                                               \
    "impl/basic_api/tile_api/kernel_tensor_tile_fixpipe_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "basic_api/kernel_operator_fixpipe_intf.h"" and use public functions or variables defined in interface headers files."
#define _ASCENDC_INCLUDE_INTERNAL_HEADERS_
#define UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_TENSOR_TILE_FIXPIPE_IMPL_H
#endif

#ifndef IMPL_TENSOR_TILE_API_KERNEL_TENSOR_TILE_FIXPIPE_IMPL_H
#define IMPL_TENSOR_TILE_API_KERNEL_TENSOR_TILE_FIXPIPE_IMPL_H

#include "fixpipe/kernel_tensor_tile_fixpipe_routing.h"

namespace AscendC {

template <const FixpipeTrait& trait, typename T, typename U>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeTemplate<T, U>, void>::type
Fixpipe(const T& dst, const U& src)
{
    constexpr TPosition dstTPos = TileInternal::GetTensorTraitType<T>::tPos;
    constexpr TPosition srcTPos = TileInternal::GetTensorTraitType<U>::tPos;
    using Tensor2Tensor =
        typename TileInternal::FixpipeTensor2Tensor<dstTPos, srcTPos, TileInternal::CURRENT_ARCH_VERSION, TileInternal::FOUR_DIM_DATA>::type;
    Tensor2Tensor{}.template Run<T, U, trait>(dst, src);
}

template <const FixpipeTrait& trait, typename T, typename U, typename V>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeQuantTemplate<T, U, V>, void>::type
Fixpipe(const T& dst, const U& src, const V& quant)
{
    constexpr TPosition dstTPos = TileInternal::GetTensorTraitType<T>::tPos;
    constexpr TPosition srcTPos = TileInternal::GetTensorTraitType<U>::tPos;
    using Tensor2Tensor =
        typename TileInternal::FixpipeQuantTensor2Tensor<dstTPos, srcTPos, TileInternal::CURRENT_ARCH_VERSION, TileInternal::FOUR_DIM_DATA>::type;
    Tensor2Tensor{}.template Run<T, U, V, trait>(dst, src, quant);
}
} // namespace AscendC

#endif // IMPL_TENSOR_TILE_API_KERNEL_TENSOR_TILE_FIXPIPE_IMPL_H
#if defined(UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_TENSOR_TILE_FIXPIPE_IMPL_H)
#undef _ASCENDC_INCLUDE_INTERNAL_HEADERS_
#undef UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_TENSOR_TILE_FIXPIPE_IMPL_H
#endif
