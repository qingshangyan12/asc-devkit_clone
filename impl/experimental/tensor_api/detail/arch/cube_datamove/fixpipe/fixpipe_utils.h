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
 * \file fixpipe_utils.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_FIXPIPE_UTILS_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_FIXPIPE_UTILS_H

namespace AscendC {
namespace Te{

template <typename T>
__aicore__ inline auto AllocTempBuf(const T& calNSize)
{
    uint64_t deqTensorTempBuf = 0;
    if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510 ||
                  CURRENT_ARCH_VERSION == ArchVersion::V2201) {
        deqTensorTempBuf = reinterpret_cast<uint64_t>(get_imm(0));
    }
    return deqTensorTempBuf;
}

template <typename T>
__aicore__ inline void SetFpc(const T& deqTensorTempBuf)
{
    if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510 ||
                  CURRENT_ARCH_VERSION == ArchVersion::V2201) {
        uint64_t deqTensorAddr = (deqTensorTempBuf >> static_cast<uint64_t>(7)) << 8;
        set_fpc(deqTensorAddr);
    }
}

 __aicore__ inline void InsertSync()
{
 	if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510 || 
 	              CURRENT_ARCH_VERSION == ArchVersion::V2201) {
 	    pipe_barrier(PIPE_FIX);
 	}
}

template <typename T, typename U, typename Coord>
__aicore__ inline void CheckCoord(const T& dst, const U& src, const Coord& coord)
{
    auto coordRow = Std::get<0>(coord);
    auto coordCol = Std::get<1>(coord);
    auto dstLayout = dst.Layout();
    auto srcLayout = src.Layout();
    uint32_t dstRow1 = GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(dstLayout);
    uint32_t dstCol1 = GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(dstLayout);
    uint32_t srcRow1 = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout);
    uint32_t srcCol1 = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout);
    ASSERT((coordRow >= 0) && (coordCol >= 0) && (coordRow <= dstRow1 - srcRow1) && (coordCol <= dstCol1 - srcCol1));
}

template <typename T, typename Coord>
__aicore__ inline auto MakeTensorWithCoord(const T& oldTensor, const Coord& coord, uint32_t offset = 0)
{
    auto oldTensorLayout = oldTensor.Layout();
    auto index = Crd2Idx(coord, oldTensorLayout);
    using oldTensorType = typename T::elementType;
    constexpr Hardware oldTensorPos = GetHardPos<T>();
    auto oldTensorIterator = MakeMemPtr<oldTensorPos>(
        reinterpret_cast<oldTensorType *>(oldTensor.Data().Get() + offset + index));
    auto oldTensorMatrixLayout = MakeLayout(oldTensorLayout.Shape(), oldTensorLayout.Stride());
    auto newTensor = MakeTensor(oldTensorIterator, oldTensorMatrixLayout); 
    return newTensor;
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_FIXPIPE_UTILS_H