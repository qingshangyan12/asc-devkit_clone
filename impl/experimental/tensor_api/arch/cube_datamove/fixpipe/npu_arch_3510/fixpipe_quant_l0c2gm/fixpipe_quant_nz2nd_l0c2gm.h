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
 * \file fixpipe_quant_nz2nd_l0c2gm.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FIXPIPE_QUANT_NZ2ND_L0C2GM_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FIXPIPE_QUANT_NZ2ND_L0C2GM_H

#include "impl/experimental/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/fixpipe_base.h"

namespace AscendC {
namespace Te {

class FixpipeNZ2NDSimpleQuant3510 : public CopyMatrixCcToGmBase3510, public SetRegisterBase3510,
    public FormatCheckUtils3510 {
public:
    template <const FixpipeTrait& trait, typename T, typename U, typename V, typename Coord>
    __aicore__ inline void Run(const T& dst, const U& src, const V& quant, const Coord& coord)
    {
        auto registerParams = GenRegisterParams<trait, T, U>(dst, src);
        SetRegister<V, decltype(registerParams)>(quant, registerParams);
        auto params = GenFixpipeQuantParams<trait, T, U, Coord>(dst, src, coord);
        auto dstNDTensor = dst(coord, dst.Layout().Shape());
        DataCopy<trait, T, U, decltype(params)>(dstNDTensor, src, params);
    }

private:

    template <const FixpipeTrait& trait, typename T, typename U>
    __aicore__ inline constexpr void CheckTemplate()
    {
        FormatCheckUtils3510::CheckNDTemplate<T>();
        FormatCheckUtils3510::CheckL0CNZTemplate<U>();
    }

    template <const FixpipeTrait& trait, typename T, typename U>
    __aicore__ inline auto GenRegisterParams(const T& dst, const U& src)
    {
        uint32_t ndNum = 1;
        uint32_t srcNDStride = 0;
        uint32_t dstNDStride = 0;
        auto params = Std::make_tuple(ndNum, dstNDStride, srcNDStride);
        return params;
    }

    template <const FixpipeTrait& trait, typename T, typename U, typename Coord>
    __aicore__ inline auto GenFixpipeQuantParams(const T& dst, const U& src, const Coord& coord)
    {
        CheckTemplate<trait, T, U>();
        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();
        uint32_t nSize = Std::min(
            GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(srcLayout) *
            GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout),
            GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(dstLayout) *
            GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(dstLayout) - Std::get<1>(coord));
        uint32_t mSize = Std::min(GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 0>(srcLayout) *
            GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout),
            GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::ROW, 0>(dstLayout) *
            GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(dstLayout) - Std::get<0>(coord));
        uint32_t srcStride =
            GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout) / FRACTAL_FIXED;
        uint32_t dstStride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, AttrInfo::ROW, 1>(dstLayout);
        uint8_t cacheMode = GetCacheModeFromTensor(dst.Data().Get());
        bool reluEn = trait.enableRelu;
        uint8_t unitFlag = trait.unitFlag;
        bool isChannelSplit = trait.enableChannelSplit;
        bool nz2ndEn = true;
        bool nz2dnEn = false;
        auto params = Std::make_tuple(
            nSize, mSize, srcStride, dstStride, cacheMode, reluEn, unitFlag, isChannelSplit, nz2ndEn, nz2dnEn);
        return params;
    }
};

class FixpipeNZ2NDVectorBase3510 : public CopyMatrixCcToGmBase3510, public CopyDeqTensorToFbuf3510 {
public:
    template <const FixpipeTrait& trait, typename T, typename U, typename V, typename S>
    __aicore__ inline void FixpipeNZ2NDVectorEntrance(const T& dst, const U& src, const V& quant, const S& params)
    {
        FixpipeNZ2NDVectorImpl<trait, T, U, V, S>(dst, src, quant, params, tuple_sequence<decltype(params)>{});
    }

private:
    template <const FixpipeTrait& trait, typename T, typename U, typename V, typename S, size_t... Is>
    __aicore__ inline void FixpipeNZ2NDVectorImpl(
        const T& dst, const U& src, const V& quant, const S& tupleParams, Std::index_sequence<Is...>)
    {
        FixpipeNZ2NDVectorCompute<trait, T, U, V>(dst, src, quant, Std::get<Is>(tupleParams)...);
    }

    template <const FixpipeTrait& trait, typename T, typename U, bool isTail>
    __aicore__ inline auto GenParams(const T& dst, const U& src)
    {
        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();
        uint32_t nSize = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(srcLayout) *
                         GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout);
        if constexpr (isTail) {
            nSize = nSize % MAIN_LOOP_N_SIZE_3510;
        } else {
            if (nSize > MAIN_LOOP_N_SIZE_3510) {
                nSize = MAIN_LOOP_N_SIZE_3510;
            }
        }
        uint32_t mSize = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 0>(srcLayout) *
                         GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout);
        uint32_t srcStride =
            GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout) / FRACTAL_FIXED;
        uint32_t dstStride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, AttrInfo::ROW, 1>(dstLayout);
        uint8_t cacheMode = GetCacheModeFromTensor(dst.Data().Get());
        bool reluEn = trait.enableRelu;
        uint8_t unitFlag = trait.unitFlag;
        bool isChannelSplit = trait.enableChannelSplit;
        bool nz2ndEn = true;
        bool nz2dnEn = false;
        auto params = Std::make_tuple(
            nSize, mSize, srcStride, dstStride, cacheMode, reluEn, unitFlag, isChannelSplit, nz2ndEn, nz2dnEn);
        return params;
    }

    template <const FixpipeTrait& trait, typename T, typename U, typename V>
    __aicore__ inline void FixpipeNZ2NDVectorCompute(const T& dst, const U& src, const V& quant, uint32_t nIterNum,
        uint32_t calNSize, uint32_t tailNSize)
    {
        auto mainLoopParam = GenParams<trait, T, U, false>(dst, src);
        for (uint16_t i = 0; i < nIterNum; ++i) {
            CopyDeqTensorToFbufImpl(quant, calNSize, i);
            InsertSync();
            auto srcCoord = MakeCoord(MakeCoord(0, 0), MakeCoord(0, i * CBURST_NUM_3510));
            auto dstCoord = MakeCoord(MakeCoord(0, 0), MakeCoord(0, i * MAIN_LOOP_N_SIZE_3510));
            DataCopy<trait>(dst(dstCoord), src(srcCoord), mainLoopParam);
        }
        if (tailNSize) {
            auto tailParam = GenParams<trait, T, U, true>(dst, src);
            CopyDeqTensorToFbufImpl(quant, tailNSize, nIterNum);
            InsertSync();
            auto srcCoord = MakeCoord(MakeCoord(0, 0), MakeCoord(0, nIterNum * CBURST_NUM_3510));
            auto dstCoord = MakeCoord(MakeCoord(0, 0), MakeCoord(0, nIterNum * MAIN_LOOP_N_SIZE_3510));
            DataCopy<trait>(dst(dstCoord), src(srcCoord), tailParam);
        }
    }
};

class FixpipeNZ2NDVectorQuant3510 : public FixpipeNZ2NDVectorBase3510, public SetRegisterBase3510,
    public FormatCheckUtils3510 {
public:
    template <const FixpipeTrait& trait, typename T, typename U, typename V, typename Coord>
    __aicore__ inline void Run(const T& dst, const U& src, const V& quant, const Coord& coord)
    {
        auto registerParams = GenRegisterParams<trait, T, U>(dst, src);
        SetRegister<decltype(registerParams)>(registerParams);
        auto params = GenTileParams<trait, T, U, Coord>(dst, src, coord);
        auto dstNDTensor = dst(coord, dst.Layout().Shape());
        FixpipeNZ2NDVectorEntrance<trait, T, U, V, decltype(params)>(dst, src, quant, params);
    }

private:
    template <const FixpipeTrait& trait, typename T, typename U>
    __aicore__ inline constexpr void CheckTemplate()
    {
        FormatCheckUtils3510::CheckNDTemplate<T>();
        FormatCheckUtils3510::CheckL0CNZTemplate<U>();
    }

    template <const FixpipeTrait& trait, typename T, typename U, typename Coord>
    __aicore__ inline auto GenTileParams(const T& dst, const U& src, const Coord& coord)
    {
        CheckTemplate<trait, T, U>();
        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();
        uint32_t nSize = Std::min(
            GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(srcLayout) *
            GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout),
            GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(dstLayout) *
            GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(dstLayout) - Std::get<1>(coord));
        uint32_t srcStride =
            GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout) / FRACTAL_FIXED;
        uint32_t dstStride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(dstLayout);

        uint16_t nIterNum = 1;
        uint32_t calNSize = nSize;
        uint32_t tailNSize = 0;
        if (calNSize > MAIN_LOOP_N_SIZE_3510) {
            nIterNum = nSize / MAIN_LOOP_N_SIZE_3510;
            tailNSize = nSize % MAIN_LOOP_N_SIZE_3510;
            calNSize = MAIN_LOOP_N_SIZE_3510;
        }
        auto params = Std::make_tuple(nIterNum, calNSize, tailNSize);
        return params;
    }

    template <const FixpipeTrait& trait, typename T, typename U>
    __aicore__ inline auto GenRegisterParams(const T& dst, const U& src)
    {
        uint32_t ndNum = 1;
        uint32_t srcNDStride = 0;
        uint32_t dstNDStride = 0;
        auto params = Std::make_tuple(ndNum, dstNDStride, srcNDStride);
        return params;
    }
};

}  // namespace Te
}  // namespace AscendC

#endif  // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FIXPIPE_QUANT_NZ2ND_L0C2GM_H
