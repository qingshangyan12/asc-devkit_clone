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
 * \file zn2nzb8.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_LOAD_DATA_L12L0B_NZ2ZNB8_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_LOAD_DATA_L12L0B_NZ2ZNB8_H

#include "impl/experimental/tensor_api/arch/cube_datamove/load_data/npu_arch_3510/instruction.h"

namespace AscendC {
namespace Te {

class LoadDataFourDim3510L12L0BNZ2ZNB8 : public LoadDataFourDim3510L12L0BBase {

public:
    template <const LoadDataTrait& trait, typename T, typename U, typename Coord>
    __aicore__ inline void Run(const T& dst, const U& src, const Coord& coord) {
        auto params = GenLoadDataParams<trait, T, U, Coord>(dst, src, coord);
        LoadDataAlignV2Impl<trait, T, U, decltype(params)>(dst, src, params, tuple_sequence<decltype(params)>{});
    }

private:
    template <const LoadDataTrait& trait, typename T, typename U, typename Coord>
    __aicore__ inline auto GenLoadDataParams(const T& dst, const U& src, const Coord& coord)
    {
        CheckTemplate<trait, T, U>();

        using DstType = typename T::elementType;
        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();

        // calc offset from coord structure
        auto mStartPosition = Std::get<0>(coord) / FRACTAL_FIXED;
        auto kStartPosition = Std::get<1>(coord) * sizeof(DstType) / C0_SIZE;

        auto mStep = GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(dstLayout);
        auto kStep = GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(dstLayout);
        // Nz -> Zn
        uint32_t STRIDE_UNIT = FRACTAL_FIXED * (C0_SIZE / sizeof(DstType));
        auto srcStride = GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout) / STRIDE_UNIT;
        auto dstStride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, AttrInfo::ROW, 1>(dstLayout) / STRIDE_UNIT;
        auto params = Std::make_tuple(mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride);
        return params;
    }

    template <const LoadDataTrait& trait, typename T, typename U, typename V, size_t... Is>
    __aicore__ inline void LoadDataAlignV2Impl(const T& dst, const U& src, const V& tupleParams, Std::index_sequence<Is...>)
    {
        // MTE2
        LoadL1ToL0BImpl<trait.transposed>(dst.Data().Get(), src.Data().Get(),
            Std::get<Is>(tupleParams)...);
    }

    template <bool transpose, typename T>
    __aicore__ inline void LoadL1ToL0BImpl(__ca__ T* dst, __cbuf__ T* src, uint16_t mStartPosition,
        uint16_t kStartPosition, uint8_t mStep, uint8_t kStep, int16_t srcStride, uint16_t dstStride)
    {
        if ASCEND_IS_AIV {
            return;
        }

        constexpr const int SHIFT_M_STEP_B8 = 1;
        constexpr const int M_STEP_MIN_VAL_B8 = 2;
        uint16_t nLoop = mStep >> SHIFT_M_STEP_B8;
        uint16_t dstAddrStride = kStep * FRACTAL_FIXED * C0_SIZE;
        mStep = M_STEP_MIN_VAL_B8;
        for (uint16_t idx = 0; idx < nLoop; ++idx) {
            LoadL1ToL0BAlignV2<transpose>(dst, src, mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride);
            dst += dstAddrStride;
            mStartPosition += M_STEP_MIN_VAL_B8;
        }
    }
};


} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_LOAD_DATA_L12L0A_NZ2ZNB8_H