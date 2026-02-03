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
 * \file tensor_tile_data_copy_four_dim_3101_l1_bt.h
 * \brief
 */
#ifndef IMPL_EXPERIMENTAL_TENSOR_API_ARCH_TILE_API_DATA_COPY_TENSOR_TILE_DATA_COPY_FOUR_DIM_3101_GM_BT_H
#define IMPL_EXPERIMENTAL_TENSOR_API_ARCH_TILE_API_DATA_COPY_TENSOR_TILE_DATA_COPY_FOUR_DIM_3101_GM_BT_H

#include "impl/experimental/tensor_api/utils/tensor_tile_utils.h"
#include "impl/experimental/tensor_api/struct/tensor_tile_struct.h"

namespace AscendC {
namespace TileInternal {

class CopyCbufToBT3101 {
public:
    template <const DataCopyTrait& trait, typename T, typename U>
    __aicore__ inline void Run(const T& dst, const U& src) {
        auto params = GenDataCopyParams<trait,T, U>(dst, src);
        DataCopyImpl<trait, T, U, decltype(params)>(dst, src, params, tuple_sequence<decltype(params)>{});
    }

private:
    template <typename T>
    __aicore__ inline constexpr void CheckNDTemplate()
    {
        using ShapeRow0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::ROW, 0>::type;
        using ShapeColumn0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::COLUMN, 0>::type;
        static_assert(Std::is_same_v<ShapeRow0, Std::Int<1>>, "CopyCbufToBT Layout->Shape->Row->ZeroDim, is not Std::Int<1> type!");
        static_assert(Std::is_same_v<ShapeColumn0, Std::Int<1>>, "CopyCbufToBT Layout->Shape->Column->ZeroDim, is not Std::Int<1> type!");

        using StrideRow0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 0>::type;
        using StrideColumn0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 0>::type;
        using StrideColumn1 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 1>::type;
        static_assert(Std::is_same_v<StrideRow0, Std::Int<0>>, "CopyCbufToBT Layout->Stride->Row->ZeroDim, is not Std::Int<0> type!");
        static_assert(Std::is_same_v<StrideColumn0, Std::Int<0>>, "CopyCbufToBT Layout->Stride->Column->ZeroDim, is not Std::Int<0> type!");
        static_assert(Std::is_same_v<StrideColumn1, Std::Int<1>>, "CopyCbufToBT Layout->Stride->Column->OneDim, is not Std::Int<1> type!");
    }

    template <const DataCopyTrait& trait, typename T, typename U>
    __aicore__ inline constexpr void CheckTemplate()
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        CheckNDTemplate<T>();
        CheckNDTemplate<U>();

#if defined(__NPU_ARCH__ ) && __NPU_ARCH__ == 3101
        static_assert(Std::is_one_of_v<Std::tuple<dstType, srcType>, Std::tuple<__biasbuf__ bfloat16_t, __cbuf__ bfloat16_t>, 
            Std::tuple<__biasbuf__ half, __cbuf__ half>, Std::tuple<__biasbuf__ float, __cbuf__ float>, 
            Std::tuple<__biasbuf__ int32_t, __cbuf__ int32_t>>, "The data type is not supported.");
#endif
    }

    template <const DataCopyTrait& trait, typename T, typename U>
    __aicore__ inline auto GenDataCopyParams(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();

        auto dstLayout = dst.GetTensorTrait().GetLayout();
        auto srcLayout = src.GetTensorTrait().GetLayout();

        uint16_t srcCol = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout);
        uint16_t srcRow = GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::ROW, 1>(srcLayout);
        uint16_t dstRow = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, AttrInfo::ROW, 1>(dstLayout);

        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        bool convControl = false;
        uint16_t blockCount = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout);
        uint16_t blockLen = srcCol * sizeof(dstType) / C0_SIZE;
        uint16_t srcStride = (srcRow - srcCol) * sizeof(srcType) / C0_SIZE;
        uint16_t dstStride = (dstRow - srcCol) * sizeof(dstType) / C0_SIZE;

        return Std::make_tuple(convControl, blockCount, blockLen, srcStride, dstStride);
    }

    template <const DataCopyTrait& trait, typename T, typename U, typename V, size_t... Is>
    __aicore__ inline void DataCopyImpl(const T& dst, const U& src, const V& tupleParams, Std::index_sequence<Is...>)
    {
        CopyCbufToBt(dst.Data().Get(), src.Data().Get(), Std::get<Is>(tupleParams)...);
    }

    template <typename T>
    __aicore__ inline void CopyCbufToBt(uint64_t dst, __cbuf__ T* src, bool convControl, uint16_t blockCount, uint16_t blockLen,
        uint16_t srcStride, uint16_t dstStride)
    {
        if ASCEND_IS_AIV {
            return;
        }

        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3101) {
            copy_cbuf_to_bt(dst, src, convControl, blockCount, blockLen, srcStride, dstStride);
        }
    }
};

} // namespace TileInternal
} // namespace AscendC

#endif // IMPL_EXPERIMENTAL_TENSOR_API_ARCH_TILE_API_DATA_COPY_TENSOR_TILE_DATA_COPY_FOUR_DIM_3101_GM_BT_H