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
 * \file tensor_tile_load_data_four_dim_3101_l1_l0a.h
 * \brief
 */
#ifndef IMPL_EXPERIMENTAL_TENSOR_API_ARCH_TILE_API_LOAD_DATA_TENSOR_TILE_LOAD_DATA_FOUR_DIM_3101_L1_L0A_H
#define IMPL_EXPERIMENTAL_TENSOR_API_ARCH_TILE_API_LOAD_DATA_TENSOR_TILE_LOAD_DATA_FOUR_DIM_3101_L1_L0A_H

#include "impl/experimental/tensor_api/utils/tensor_tile_utils.h"
#include "impl/experimental/tensor_api/struct/tensor_tile_struct.h"

namespace AscendC {
namespace TileInternal {
class LoadDataFourDim3101L12L0A {

public:
    template <const LoadDataTrait& trait, typename T, typename U>
    __aicore__ inline void Run(const T& dst, const U& src) {
        auto params = GenLoadDataParams<trait, T, U>(dst, src);
        LoadDataAlignV2Impl<trait, T, U, decltype(params)>(dst, src, params, tuple_sequence<decltype(params)>{});
    }

private:
    template<typename T>
    __aicore__ inline constexpr void CheckNZTemplate()
    {
        using type = typename T::elementType;
        using ShapeRow0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::ROW, 0>::type;
        using ShapeColumn0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::COLUMN, 0>::type;
        static_assert(Std::is_same_v<ShapeRow0, Std::Int<FRACTAL_FIXED>>,
            "LoadDataFourDim3101L12L0A Layout->Shape->Row->ZeroDim is not Std::Int<16> type!");
        static_assert(Std::is_same_v<ShapeColumn0, Std::Int<C0_SIZE / sizeof(type)>>,
            "LoadDataFourDim3101L12L0A Layout->Shape->Column->ZeroDim is not Std::Int<C0Size/Type> type!"); 

        using StrideRow0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 0>::type;
        using StrideColumn0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 0>::type;
        static_assert(Std::is_same_v<StrideRow0, Std::Int<C0_SIZE / sizeof(type)>>,
            "LoadDataFourDim3101L12L0A Layout->Stride->Row-ZeroDim is not Std::Int<C0Size/Type> type!");
        static_assert(Std::is_same_v<StrideColumn0, Std::Int<1>>,
            "LoadDataFourDim3101L12L0A Layout->Stride->Column->ZeroDim is not Std::Int<1> type!");
    }

    template <const LoadDataTrait& trait, typename T, typename U>
    __aicore__ inline constexpr void CheckTemplate()
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;
        CheckNZTemplate<T>();
        CheckNZTemplate<U>();
#if defined(__NPU_ARCH__ ) && __NPU_ARCH__ == 3101
        static_assert(Std::is_one_of_v<Std::tuple<dstType, srcType>, Std::tuple<__ca__ half, __cbuf__ half>, 
            Std::tuple<__ca__ bfloat16_t, __cbuf__ bfloat16_t>, Std::tuple<__ca__ uint32_t, __cbuf__ uint32_t>, 
            Std::tuple<__ca__ int32_t, __cbuf__ int32_t>, Std::tuple<__ca__ float, __cbuf__ float>, 
            Std::tuple<__ca__ uint8_t, __cbuf__ uint8_t>, Std::tuple<__ca__ int8_t, __cbuf__ int8_t>, 
            Std::tuple<__ca__ fp8_e4m3fn_t, __cbuf__ fp8_e4m3fn_t>, Std::tuple<__ca__ fp8_e5m2_t, __cbuf__ fp8_e5m2_t>, 
            Std::tuple<__ca__ hifloat8_t, __cbuf__ hifloat8_t>>, "The data type is not supported.");
#endif
    }

    template <const LoadDataTrait& trait, typename T, typename U>
    __aicore__ inline auto GenLoadDataParams(const T& dst, const U& src)
    {
        CheckTemplate<trait, GetTensorTraitType<T>, GetTensorTraitType<U>>();
        
        using DstType = typename GetTensorTraitType<T>::elementType;
        auto dstLayout = dst.GetLayout();
        auto srcLayout = src.GetLayout();

        uint16_t mStartPosition = 0;
        uint16_t kStartPosition = 0;
        auto mStep = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout);
        auto kStep = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout);
        // Nz -> Nz
        uint32_t STRIDE_UNIT = FRACTAL_FIXED * (C0_SIZE / sizeof(DstType));
        auto srcStride = GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout) / STRIDE_UNIT;
        auto dstStride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(dstLayout) / STRIDE_UNIT;
        auto params = Std::make_tuple(mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride);
        return params;
    }

    template <const LoadDataTrait& trait, typename T, typename U, typename V, size_t... Is>
    __aicore__ inline void LoadDataAlignV2Impl(const T& dst, const U& src, const V& tupleParams, Std::index_sequence<Is...>)
    {
        // MTE2
        LoadL1ToL0AAlignV2<trait.transposed>(dst.Data().Get(), src.Data().Get(),
            Std::get<Is>(tupleParams)...);
    }

    template <bool transpose, typename T>
    __aicore__ inline void LoadL1ToL0AAlignV2(__ca__ T* dst, __cbuf__ T* src, uint16_t mStartPosition,
        uint16_t kStartPosition, uint8_t mStep, uint8_t kStep, int16_t srcStride, uint16_t dstStride)
    {
        if ASCEND_IS_AIV {
            return;
        }

        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3101) {
            load_cbuf_to_ca(dst, src, mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, transpose);
        }
    }
};

} // namespace ACTE
} // namespace AscendC

#endif // IMPL_EXPERIMENTAL_TENSOR_API_ARCH_TILE_API_LOAD_DATA_TENSOR_TILE_LOAD_DATA_FOUR_DIM_3101_L1_L0A_H