/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/strategies/quant_nz_conversion.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file quant_nz_conversion.h
 * \brief Unified quantized NZ format conversion strategy for Fixpipe operations
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_STRATEGIES_QUANT_NZ_CONVERSION_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_STRATEGIES_QUANT_NZ_CONVERSION_H

#include "impl/experimental/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/fixpipe_common.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/quant_format_policies.h"

namespace AscendC {
namespace Te {

/**
 * \brief Unified quantized NZ format conversion strategy with Simple Quantization
 * \tparam DstFormat Destination format (NZ, ND, DN)
 */
template <Format3510 DstFormat>
class QuantNzConversionSimpleStrategy {
    using Policy = QuantNzFormatPolicy<DstFormat>;

public:
    /**
     * \brief Execute quantized NZ format conversion with Simple quantization
     * \tparam trait Fixpipe trait configuration
     * \tparam quantPre Quantization mode
     * \tparam T Destination tensor type
     * \tparam U Source tensor type
     * \tparam V Quantization parameter type
     * \param dst Destination tensor
     * \param src Source tensor
     * \param quant Quantization parameter
     * \param params Fixpipe parameters
     */
    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U, typename V>
    __aicore__ inline static void Execute(const T& dst, const U& src, const V& quant, const FixpipeParams& params)
    {
        Policy::template CheckTemplate<T>();
        Policy::SetRegisterWithQuant(quant);

        auto layoutParams = CalcLayoutParams(dst, src);
        ExecuteDataCopy<trait, quantPre, T, U>(dst, src, layoutParams, params);
    }

private:
    template <typename T, typename U>
    __aicore__ inline static auto CalcLayoutParams(const T& dst, const U& src)
    {
        const auto& srcLayout = src.Layout();
        const auto& dstLayout = dst.Layout();

        uint32_t mSize =
            Std::min(GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 0>(srcLayout)
                         * GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout),
                     GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::ROW, 0>(dstLayout)
                         * GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(dstLayout));

        uint32_t nSize =
            Std::min(GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(srcLayout)
                         * GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout),
                     GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(dstLayout)
                         * GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(dstLayout));

        uint32_t srcStride =
            GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout) / FRACTAL_FIXED;
        uint32_t dstStride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, Policy::strideAttr, 1>(dstLayout);

        return Std::make_tuple(nSize, mSize, srcStride, dstStride);
    }

    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void ExecuteDataCopy(const T& dst, const U& src, const auto& layoutParams,
                                                  const FixpipeParams& params)
    {
        uint32_t nSize = Std::get<0>(layoutParams);
        uint32_t mSize = Std::get<1>(layoutParams);
        uint32_t srcStride = Std::get<2>(layoutParams);
        uint32_t dstStride = Std::get<3>(layoutParams);

        bool reluEn = trait.enableRelu;
        uint8_t unitFlag = params.unitFlag;

        if constexpr (GetHardPos<T>() == Hardware::GM) {
            uint8_t cacheMode = GetCacheModeFromTensor(dst);
            bool isChannelSplit = trait.enableChannelSplit;
            CopyMatrixCcToGm3510::DataCopy<quantPre, T, U>(dst, src, nSize, mSize, srcStride, dstStride, cacheMode,
                                                           reluEn, unitFlag, isChannelSplit, Policy::nz2ndEn,
                                                           Policy::nz2dnEn);
        } else {
            if constexpr (DstFormat == Format3510::NZ) {
                static_assert(trait.dualDstCtl != DUAL_DST_SPLIT_N, "If split N when fixpipe, NZ is not supported");
            }
            uint8_t dualDstCtl = Policy::AdjustDualDstCtl(trait.dualDstCtl, dstStride);
            bool subBlockId = false;
            CopyMatrixCcToUb3510::DataCopy<quantPre, T, U>(dst, src, nSize, mSize, srcStride, dstStride, dualDstCtl,
                                                           reluEn, unitFlag, subBlockId, Policy::nz2ndEn,
                                                           Policy::nz2dnEn);
        }
    }
};

/**
 * \brief Unified quantized NZ format conversion strategy with Vector Quantization
 * \tparam DstFormat Destination format (NZ, ND, DN)
 */
template <Format3510 DstFormat>
class QuantNzConversionVectorStrategy {
    using Policy = QuantNzFormatPolicy<DstFormat>;

public:
    /**
     * \brief Execute quantized NZ format conversion with Vector quantization
     * \tparam trait Fixpipe trait configuration
     * \tparam quantPre Quantization mode
     * \tparam T Destination tensor type
     * \tparam U Source tensor type
     * \tparam V Quantization parameter type
     * \param dst Destination tensor
     * \param src Source tensor
     * \param quant Quantization parameter
     * \param params Fixpipe parameters
     */
    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U, typename V>
    __aicore__ inline static void Execute(const T& dst, const U& src, const V& quant, const FixpipeParams& params)
    {
        Policy::template CheckTemplate<T>();
        Policy::SetRegisterWithoutQuant();
        ExecuteChunkedDataCopy<trait, quantPre, T, U, V>(dst, src, quant, params);
    }

private:
    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U, typename V>
    __aicore__ inline static void ExecuteChunkedDataCopy(const T& dst, const U& src, const V& quant,
                                                         const FixpipeParams& params)
    {
        const auto& srcLayout = src.Layout();
        uint32_t nSize = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(srcLayout)
                         * GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout);

        uint16_t nIterNum = 1;
        uint32_t calNSize = nSize;
        uint32_t tailNSize = 0;

        if (calNSize > MAIN_LOOP_N_SIZE_3510) {
            nIterNum = nSize / MAIN_LOOP_N_SIZE_3510;
            tailNSize = nSize % MAIN_LOOP_N_SIZE_3510;
            calNSize = MAIN_LOOP_N_SIZE_3510;
        }

        ExecuteVectorQuantLoop<trait, quantPre, T, U, V>(dst, src, quant, nIterNum, calNSize, tailNSize, params);
    }

    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U, typename V>
    __aicore__ inline static void ExecuteVectorQuantLoop(const T& dst, const U& src, const V& quant, uint16_t nIterNum,
                                                         uint32_t calNSize, uint32_t tailNSize,
                                                         const FixpipeParams& params)
    {
        auto mainLoopParam = GenParams<trait, T, U, false>(dst, src, params);
        for (uint16_t i = 0; i < nIterNum; ++i) {
            CopyDeqTensorToFbuf3510::CopyDeqTensorToFbufImpl(quant, calNSize, i);
            InsertSync();
            auto srcCoord = Policy::MakeVectorSrcCoord(i);
            auto dstCoord = Policy::MakeVectorDstCoord(i);
            DataCopyWrapper<trait, quantPre>(dst(dstCoord), src(srcCoord), mainLoopParam,
                                             tuple_sequence<decltype(mainLoopParam)>{});
        }
        if (tailNSize) {
            auto tailParam = GenParams<trait, T, U, true>(dst, src, params);
            CopyDeqTensorToFbuf3510::CopyDeqTensorToFbufImpl(quant, tailNSize, nIterNum);
            InsertSync();
            auto srcCoord = Policy::MakeVectorSrcCoord(nIterNum);
            auto dstCoord = Policy::MakeVectorDstCoord(nIterNum);
            DataCopyWrapper<trait, quantPre>(dst(dstCoord), src(srcCoord), tailParam,
                                             tuple_sequence<decltype(tailParam)>{});
        }
    }

    template <const FixpipeTrait& trait, typename T, typename U, bool isTail>
    __aicore__ inline static auto GenParams(const T& dst, const U& src, const FixpipeParams& params)
    {
        const auto& dstLayout = dst.Layout();
        const auto& srcLayout = src.Layout();
        uint32_t nSize = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(srcLayout)
                         * GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout);
        if constexpr (isTail) {
            nSize = nSize % MAIN_LOOP_N_SIZE_3510;
        } else {
            if (nSize > MAIN_LOOP_N_SIZE_3510) {
                nSize = MAIN_LOOP_N_SIZE_3510;
            }
        }
        uint32_t mSize = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 0>(srcLayout)
                         * GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout);
        uint32_t srcStride =
            GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout) / FRACTAL_FIXED;
        uint32_t dstStride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, Policy::strideAttr, 1>(dstLayout);

        bool reluEn = trait.enableRelu;
        uint8_t unitFlag = params.unitFlag;

        if constexpr (GetHardPos<T>() == Hardware::GM) {
            uint8_t cacheMode = GetCacheModeFromTensor(dst);
            bool isChannelSplit = trait.enableChannelSplit;
            return Std::make_tuple(nSize, mSize, srcStride, dstStride, cacheMode, reluEn, unitFlag, isChannelSplit,
                                   Policy::nz2ndEn, Policy::nz2dnEn);
        } else {
            if constexpr (DstFormat == Format3510::NZ) {
                static_assert(trait.dualDstCtl != DUAL_DST_SPLIT_N, "If split N when fixpipe, NZ is not supported");
            }
            uint8_t dualDstCtl = Policy::AdjustDualDstCtl(trait.dualDstCtl, dstStride);
            bool subBlockId = false;
            return Std::make_tuple(nSize, mSize, srcStride, dstStride, dualDstCtl, reluEn, unitFlag, subBlockId,
                                   Policy::nz2ndEn, Policy::nz2dnEn);
        }
    }

    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U, typename V, size_t... Is>
    __aicore__ inline static void DataCopyWrapper(const T& dst, const U& src, const V& tupleParams,
                                                  Std::index_sequence<Is...>)
    {
        if constexpr (GetHardPos<T>() == Hardware::GM) {
            CopyMatrixCcToGm3510::DataCopy<quantPre>(dst, src, Std::get<Is>(tupleParams)...);
        } else {
            CopyMatrixCcToUb3510::DataCopy<quantPre>(dst, src, Std::get<Is>(tupleParams)...);
        }
    }
};

/**
 * \brief Type aliases for backward compatibility
 */
using QuantNzToNzSimpleStrategy = QuantNzConversionSimpleStrategy<Format3510::NZ>;
using QuantNzToNdSimpleStrategy = QuantNzConversionSimpleStrategy<Format3510::ND>;
using QuantNzToDnSimpleStrategy = QuantNzConversionSimpleStrategy<Format3510::DN>;

using QuantNzToNzVectorStrategy = QuantNzConversionVectorStrategy<Format3510::NZ>;
using QuantNzToNdVectorStrategy = QuantNzConversionVectorStrategy<Format3510::ND>;
using QuantNzToDnVectorStrategy = QuantNzConversionVectorStrategy<Format3510::DN>;

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_STRATEGIES_QUANT_NZ_CONVERSION_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
