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
 * \file kernel_operator_mm_intf_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_mm_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_mm_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_INTF_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_MM_INTERFACE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_MM_INTERFACE_IMPL_H
#include "../../include/basic_api/kernel_tensor.h"
#include "kernel_check.h"
#include "kernel_reg.h"
#include "kernel_npu_debug.h"
#include "kernel_operator_mm_base_impl.h"
#include "../../include/basic_api/kernel_struct_mm.h"

namespace AscendC {
__aicore__ inline void PrintTimeStamp(uint32_t descId);

/* **************************************************************************************************
 * LoadData 2d                                             *
 * ************************************************************************************************* */
/*
 * @ingroup DataLoad
 * @brief Cube data loading
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] loadDataParams.startIndex Fractal matrix ID
 * @param [in] loadDataParams.repeatTime repeat times
 * @param [in] loadDataParams.srcStride src block stride
 * @param [in] loadDataParams.sid SMMU SID
 * @param [in] loadDataParams.dstGap interval between the previous tail and the next fractal head
 * @param [in] loadDataParams.ifTranspose enable parameters of transpose function
 */
template <typename T>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2DParams& loadDataParams)
{
    CheckLoadData2dDatatype<T>();
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckLoadData2dLocal2Local(dst, src, "LoadData with LoadData2DParams");
    CheckLoadData2dParams<T>(loadDataParams, true);
#endif
    LoadDataImpl(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DParams& loadDataParams)
{
#ifdef ASCENDC_TIME_STAMP_ON
    PrintTimeStamp(static_cast<uint32_t>(TimeStampId::TIME_STAMP_MTE2_DATACOPY));
#endif
    CheckLoadData2dDatatype<T>();
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckLoadData2dGlobal2Local(dst, "LoadData with LoadData2DParams");
    CheckLoadData2dParams<T>(loadDataParams, false);
#endif
    LoadDataImpl(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DParams& loadDataParams,
    const bool enablePreReadIgnoreSync)
{
#if (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202)
    CheckLoadData2dDatatype<T>();
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckLoadData2dGlobal2Local(dst, "LoadData with LoadData2DParams");
    CheckLoadData2dParams<T>(loadDataParams, false);
#endif
    LoadDataImpl(dst, src, loadDataParams, enablePreReadIgnoreSync);
#endif
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || \
                              (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
template <TPosition Dst, TPosition Src, typename T>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const Load2DBitModeParam& loadDataParams)
{
    CheckLoadData2dDatatype<T>();
    LoadDataImpl<Dst, Src, T>(dst, src, loadDataParams);
}
#endif
/* **************************************************************************************************
 * LoadData 2dV2                                             *
 * ************************************************************************************************* */
/*
 * @ingroup DataLoad
 * @brief Cube data loading
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor/GlobalTensor
 * @param [in] loadDataParams.mStartPosition m start position
 * @param [in] loadDataParams.kStartPosition k start position
 * @param [in] loadDataParams.srcStride src block stride
 * @param [in] loadDataParams.dstStride dst block stride
 * @param [in] loadDataParams.mStep m step
 * @param [in] loadDataParams.kStep k step
 * @param [in] loadDataParams.sid SMMU SID
 * @param [in] loadDataParams.ifTranspose enable parameters of transpose function
 */
template <typename T>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2DParamsV2& loadDataParams)
{
    CheckLoadData2dDatatype<T>();
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckLoadData2dLocal2Local(dst, src, "LoadData with LoadData2DParamsV2");
#endif
    LoadDataImpl(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || \
                              (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163) || \
                              (__NPU_ARCH__ == 5162) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
template <typename T, typename U, typename V>
__aicore__ inline void LoadData(
    const LocalTensor<U>& dst, const LocalTensor<T>& src0, const LocalTensor<V>& srcMx,
    const LoadData2DParamsV2& loadDataParams, const LoadData2DMxParams& loadMxDataParams)
{
    LoadDataImpl(dst, src0, srcMx, loadDataParams, loadMxDataParams);
}
#endif

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DParamsV2& loadDataParams)
{
#ifdef ASCENDC_TIME_STAMP_ON
    PrintTimeStamp(static_cast<uint32_t>(TimeStampId::TIME_STAMP_MTE2_DATACOPY));
#endif
    CheckLoadData2dDatatype<T>();
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckLoadData2dGlobal2Local(dst, "LoadData with LoadData2DParamsV2");
#endif
    LoadDataImpl(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162))
template <typename T, typename U>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<U>& src, const LoadData2DParamsV2& loadDataParams,
    const Nd2NzParamsV2& nd2nzParams)
{
#ifdef ASCENDC_TIME_STAMP_ON
    PrintTimeStamp(static_cast<uint32_t>(TimeStampId::TIME_STAMP_MTE2_DATACOPY));
#endif
    LoadDataImpl(dst, src, loadDataParams, nd2nzParams);
}
#endif

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DParamsV2& loadDataParams,
    const bool enablePreReadIgnoreSync)
{
#if (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202)
    CheckLoadData2dDatatype<T>();
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckLoadData2dGlobal2Local(dst, "LoadData with LoadData2DParamsV2");
#endif
    LoadDataImpl(dst, src, loadDataParams, enablePreReadIgnoreSync);
#endif
}

/* **************************************************************************************************
 * LoadData 3dv1                                             *
 * ************************************************************************************************* */
/*
 * @ingroup DataLoad
 * @brief Cube data loading
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] loadDataParams.padList padding list
 * @param [in] loadDataParams.l1H operand height
 * @param [in] loadDataParams.l1W operand width
 * @param [in] loadDataParams.c1Inde The starting point of the tensor C1 dimension
 * @param [in] loadDataParams.fetchFilterW The starting position of the w dimension on the convolution kernel
 * @param [in] loadDataParams.fetchFilterH The starting position of the H dimension on the convolution kernel
 * @param [in] loadDataParams.leftTopW Start point of the W dimension on the source operand
 * @param [in] loadDataParams.leftTopH Start point of the H dimension on the source operand
 * @param [in] loadDataParams.strideW W dimension stride
 * @param [in] loadDataParams.strideH H dimension stride
 * @param [in] loadDataParams.filterW Convolution kernel width
 * @param [in] loadDataParams.filterH Convolution kernel height
 * @param [in] loadDataParams.dilationFilterW Convolution kernel width expansion coefficient
 * @param [in] loadDataParams.dilationFilterH Convolution kernel height expansion coefficient
 * @param [in] loadDataParams.jumpStride repeat stride
 * @param [in] loadDataParams.repeatMode repeat mode
 * @param [in] loadDataParams.repeatTimes repeat times
 * @param [in] loadDataParams.cSize judge whether to turn on optimization
 * @param [in] loadDataParams.padValue Value of Pad filling value
 */
template <
    typename T, const IsResetLoad3dConfig& defaultConfig, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV1<U>& loadDataParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckTensorPhyPosition<Hardware::L1>(src, "src", "L1 Buffer(A1/B1)", "LoadData with LoadData3DParamsV1");
    CheckTensorAlignment(src, ONE_BLK_SIZE, "src", "LoadData with LoadData3DParamsV1");
    const Hardware dstScope = GetPhyType((TPosition)dst.GetPosition());
    if (dstScope == Hardware::L0A || dstScope == Hardware::L0B) {
        CheckTensorAlignment(dst, VALUE_512, "dst", "LoadData with LoadData3DParamsV1");
    } else if (dstScope == Hardware::UB) {
        CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", "LoadData with LoadData3DParamsV1");
    }
    CheckLoadData3dv1Params<U>(loadDataParams);
    CheckLoadData3dParams(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.strideW, loadDataParams.strideH);
#endif
    LoadDataImpl<T, defaultConfig>(dst, src, loadDataParams);
}

/* **************************************************************************************************
 * LoadData 3dv2                                             *
 * enhanced from v1, suitable for aicore > 200                                             *
 * ************************************************************************************************* */
/*
 * @ingroup DataLoad
 * @brief Cube data loading
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] loadDataParams.padList padding list
 * @param [in] loadDataParams.l1H operand height
 * @param [in] loadDataParams.l1W operand width
 * @param [in] loadDataParams.channelSize number of channels
 * @param [in] loadDataParams.kExtension Transmission length of K dimension
 * @param [in] loadDataParams.mExtension Transmission length of M dimension
 * @param [in] loadDataParams.kStartPt Start point of K dimension
 * @param [in] loadDataParams.mStartPt Start point of M dimension
 * @param [in] loadDataParams.strideW W dimension stride
 * @param [in] loadDataParams.strideH H dimension stride
 * @param [in] loadDataParams.filterW Convolution kernel width
 * @param [in] loadDataParams.filterH Convolution kernel height
 * @param [in] loadDataParams.dilationFilterW Convolution kernel width expansion coefficient
 * @param [in] loadDataParams.dilationFilterH Convolution kernel height expansion coefficient
 * @param [in] loadDataParams.enTranspose judge whether to enable the transpose function
 * @param [in] loadDataParams.enSmallK Whether to enable the small k feature
 * @param [in] loadDataParams.padValue Value of Pad filling value
 */
template <
    typename T, const IsResetLoad3dConfig& defaultConfig, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV2<U>& loadDataParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckTensorPhyPosition<Hardware::L1>(src, "src", "L1 Buffer(A1/B1)", "LoadData with LoadData3DParamsV2");
    CheckTensorAlignment(src, ONE_BLK_SIZE, "src", "LoadData with LoadData3DParamsV2");
    const Hardware dstScope = GetPhyType((TPosition)dst.GetPosition());
    if (dstScope == Hardware::L0A || dstScope == Hardware::L0B) {
        CheckTensorAlignment(dst, VALUE_512, "dst", "LoadData with LoadData3DParamsV2");
    } else if (dstScope == Hardware::UB) {
        CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", "LoadData with LoadData3DParamsV2");
    }
    CheckLoadData3dv2Params<U>(loadDataParams);
    CheckLoadData3dv2ChannelSize<T>(loadDataParams.channelSize);
    CheckLoadData3dParams(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.strideW, loadDataParams.strideH);
    CheckLoadData3dv2MatrixParams<T>(
        loadDataParams.kExtension, loadDataParams.mExtension, loadDataParams.kStartPt, loadDataParams.mStartPt);
#endif
    LoadDataImpl<T, defaultConfig>(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || \
                              (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
template <
    typename T, const IsResetLoad3dConfig& defaultConfig, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type>
__aicore__ inline void LoadDataWithStride(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV2<U>& loadDataParams)
{
#if ASCENDC_CPU_DEBUG
    CheckLoadData3dv2ChannelSize<T>(loadDataParams.channelSize);
    CheckLoadData3dParams(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.strideW, loadDataParams.strideH);
    CheckLoadData3dv2MatrixParams<T>(
        loadDataParams.kExtension, loadDataParams.mExtension, loadDataParams.kStartPt, loadDataParams.mStartPt);
#endif
    LoadDataWithStrideImpl<T, defaultConfig>(dst, src, loadDataParams);
}
#endif

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || \
                              (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
template <TPosition Dst, TPosition Src, typename T>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const Load3DBitModeParam& loadDataParams)
{
    LoadDataImpl<Dst, Src, T>(dst, src, loadDataParams);
}
#endif
/* **************************************************************************************************
 * LoadData 3dv2Pro                                             *
 * enhanced from v1, suitable for aicore > 200                                             *
 * ************************************************************************************************* */
/*
 * @ingroup DataLoad
 * @brief Cube data loading
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] loadDataParams.padList padding list
 * @param [in] loadDataParams.l1H operand height
 * @param [in] loadDataParams.l1W operand width
 * @param [in] loadDataParams.channelSize number of channels
 * @param [in] loadDataParams.kExtension Transmission length of K dimension
 * @param [in] loadDataParams.mExtension Transmission length of M dimension
 * @param [in] loadDataParams.kStartPt Start point of K dimension
 * @param [in] loadDataParams.mStartPt Start point of M dimension
 * @param [in] loadDataParams.strideW W dimension stride
 * @param [in] loadDataParams.strideH H dimension stride
 * @param [in] loadDataParams.filterW Convolution kernel width
 * @param [in] loadDataParams.filterH Convolution kernel height
 * @param [in] loadDataParams.dilationFilterW Convolution kernel width expansion coefficient
 * @param [in] loadDataParams.dilationFilterH Convolution kernel height expansion coefficient
 * @param [in] loadDataParams.enTranspose judge whether to enable the transpose function
 * @param [in] loadDataParams.enSmallK Whether to enable the small k feature
 * @param [in] loadDataParams.padValue Value of Pad filling value
 */
template <typename T>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadDataImpl<T>(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003))
// cce compiler process load3d bfloat16_t using B8, so use the half dtype instead
template <>
__aicore__ inline void LoadData(
    const LocalTensor<bfloat16_t>& dst, const LocalTensor<bfloat16_t>& src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadDataImpl(dst, src, loadDataParams);
}
#endif

/* **************************************************************************************************
 * LoadDataWithTranspose                                             *
 * ************************************************************************************************* */
/*
 * @ingroup DataLoad
 * @brief Cube data loading
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] loadDataParams.startIndex index of the first fractal in the first repeat in the source matrix
 * in unit of frac num
 * @param [in] loadDataParams.repeatTimes the repeat times
 * @param [in] loadDataParams.srcStride source stride between consecutive repeat times in unit of frac num
 * @param [in] loadDataParams.dstGap destination gap between consecutive repeat times in unit of 512byte
 * @param [in] loadDataParams.dstFracGap dst fractal gap in unit of one 512byte fractal
 */
template <typename T>
__aicore__ inline void LoadDataWithTranspose(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2dTransposeParams& loadDataParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckLoadDataWithTranspose(dst, src, "LoadDataWithTranspose with LoadData2dTransposeParams");
    ReportNopWarning<uint8_t>(
        loadDataParams.repeatTimes, "loadDataParams.repeatTimes",
        "LoadDataWithTranspose with LoadData2dTransposeParams");
#endif
    LoadDataWithTransposeImpl(dst, src, loadDataParams);
}

/*
 * @ingroup DataLoad
 * @brief Cube data loading
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] loadDataParams.startIndex index of the first fractal in the first repeat in the source matrix
 * in unit of 512byte fractal
 * @param [in] loadDataParams.repeatTime the repeat times
 * @param [in] loadDataParams.srcStride source stride between consecutive repeat times in unit of 512byte
 * @param [in] loadDataParams.dstGap destination gap between consecutive repeat times in unit of 512byte
 * @param [in] loadDataParams.dstFracGap dst fractal gap in unit of one 512byte fractal
 * @param [in] loadDataParams.srcFracGap dst fractal gap in unit of one 512byte fractal
 */
template <typename T>
__aicore__ inline void LoadDataWithTranspose(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2dTransposeParamsV2& loadDataParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckLoadDataWithTranspose(dst, src, "LoadDataWithTranspose with LoadData2dTransposeParamsV2");
    ReportNopWarning<uint8_t>(
        loadDataParams.repeatTimes, "loadDataParams.repeatTimes",
        "LoadDataWithTranspose with LoadData2dTransposeParamsV2");
#endif
    LoadDataWithTransposeImpl(dst, src, loadDataParams);
}

/* **************************************************************************************************
 * LoadDataWithDecomp                                             *
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
__aicore__ inline void EnableS4ToS8() { EnableS4ToS8Impl(); }

__aicore__ inline void EnableS3ToS8() { EnableS3ToS8Impl(); }

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DDecompParamsV2& loadDataDecompParams)
{
    LoadDataImpl<T>(dst, src, loadDataDecompParams);
}
#endif

/* **************************************************************************************************
 * Mmad                                             *
 * ************************************************************************************************* */
/*
 * @ingroup Mmad
 * @brief Matrix multiplication and addition
 * @param [out] dst output LocalTensor
 * @param [in] fm input LocalTensor
 * @param [in] filter input LocalTensor
 * @param [in] mmadParams.m Left matrix row number
 * @param [in] mmadParams.n right matrix column number
 * @param [in] mmadParams.k Left matrix column number m
 * @param [in] mmadParams.unitFlag whether enable unit flag
 * @param [in] mmadParams.kDirectionAlign is the indicator for alignment in L0A/L0B in the K direction
 * @param [in] mmadParams.cmatrixSource indicates the C matrix source, 1: the C matrix is in bias table buffer, 0: the C
 * matrix is in L0C
 * @param [in] mmadParams.cmatrixInitVal indicates the initial matrix, 1: the number in C matrix is 0, 0：use the real
 * number in C matrix
 */

template <typename T, typename U, typename S>
__aicore__ inline void Mmad(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const MmadParams& mmadParams)
{
    MmadImpl(dst, fm, filter, mmadParams);
}

template <typename T, typename U, typename S, typename V>
__aicore__ inline void Mmad(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const LocalTensor<V>& bias,
    const MmadParams& mmadParams)
{
    MmadImpl(dst, fm, filter, bias, mmadParams);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
template <typename T, typename U, typename S>
__aicore__ inline void Mmad(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter,
    const MmadBitModeParams& mmadParams)
{
    MmadImpl(dst, fm, filter, mmadParams);
}

template <typename T, typename U, typename S, typename V>
__aicore__ inline void Mmad(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const LocalTensor<V>& bias,
    const MmadBitModeParams& mmadParams)
{
    MmadImpl(dst, fm, filter, bias, mmadParams);
}

template <typename T, typename U, typename S>
__aicore__ inline void MmadMx(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const MmadParams& mmadParams)
{
    MmadMxImpl(dst, fm, filter, mmadParams);
}

template <typename T, typename U, typename S, typename V>
__aicore__ inline void MmadMx(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const LocalTensor<V>& bias,
    const MmadParams& mmadParams)
{
    MmadMxImpl(dst, fm, filter, bias, mmadParams);
}

template <typename T, typename U, typename S>
__aicore__ inline void MmadMx(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter,
    const MmadBitModeParams& mmadParams)
{
    MmadMxImpl(dst, fm, filter, mmadParams);
}

template <typename T, typename U, typename S, typename V>
__aicore__ inline void MmadMx(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const LocalTensor<V>& bias,
    const MmadBitModeParams& mmadParams)
{
    MmadMxImpl(dst, fm, filter, bias, mmadParams);
}
#endif

#if __NPU_ARCH__ == 2201
template <
    typename T, typename U, typename Std::enable_if<Std::is_same<PrimT<T>, int32_t>::value, bool>::type,
    typename Std::enable_if<Std::is_same<PrimT<U>, int8_t>::value, bool>::type>
__aicore__ inline void MmadWithSparse(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<U>& filter, const MmadParams& mmadParams)
{
    MmadSpImpl(dst, fm, filter, mmadParams);
}

template <
    typename T, typename U, typename Std::enable_if<Std::is_same<PrimT<T>, int8_t>::value, bool>::type,
    typename Std::enable_if<Std::is_same<PrimT<U>, uint8_t>::value, bool>::type>
__aicore__ inline void LoadDataWithSparse(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<U>& idx,
    const LoadData2dParams& loadDataParam)
{
    LoadDataWithSparseImpl(dst, src, idx, loadDataParam);
}
#endif

#if __NPU_ARCH__ == 2002
template <typename T, typename Std::enable_if<Std::is_same<PrimT<T>, int8_t>::value, bool>::type>
__aicore__ inline void LoadUnzipIndex(const GlobalTensor<T>& src, uint32_t numOfIndexTabEntry)
{
    LoadUnzipIndexImpl(src, numOfIndexTabEntry);
}
#endif

/* **************************************************************************************************
 * BroadCastVecToMM                                             *
 * ************************************************************************************************* */
template <typename T, typename U>
__aicore__ inline __inout_pipe__(V) void BroadCastVecToMM(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const int32_t blockCount, const uint8_t blockLen,
    const uint8_t srcGap, const uint8_t dstGap)
{
    BroadCastVecToMMImpl(dst, src, blockCount, blockLen, srcGap, dstGap);
}

/* **************************************************************************************************
 * Fill                                             *
 * ************************************************************************************************* */
/*
 * @ingroup Fill
 * @brief L0A/L0B/L1 value initializing
 * @param [out] dst output LocalTensor
 * @param [in] initConstValueParams.repeatTimes repeat times
 * @param [in] initConstValueParams.repeatTimes blockNum block number
 * @param [in] initConstValueParams.dstGap interval between the previous tail and the next block head
 * @param [in] initConstValueParams.initValue initialize Value
 */
template <typename T, typename U>
__aicore__ inline void CheckInitParams(
    const LocalTensor<T>& dst, const InitConstValueParams<U>& initConstValueParams, const char* intriName)
{
#if ASCENDC_CPU_DEBUG
    uint16_t repeatTime = initConstValueParams.repeatTimes;
#if __NPU_ARCH__ == 2201
    uint16_t blockNum = initConstValueParams.blockNum;
    uint16_t dstGap = initConstValueParams.dstGap;
#else
    uint16_t blockNum = 1;
    uint16_t dstGap = 0;
#endif
    if (!CheckFuncInitConstValue(dst, repeatTime, blockNum, dstGap, intriName)) {
        ASCENDC_REPORT_CHECK_ERROR(intriName, KernelFuncType::NONE_MODE);
    }
#endif
}

namespace Impl {
constexpr const char* FILL_INTRINSIC_NAME = "Fill";
constexpr const char* INITCONSTVALUE_INTRINSIC_NAME = "InitConstValue";
} // namespace Impl

template <typename T, typename U, typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type>
__aicore__ inline void Fill(const LocalTensor<T>& dst, const InitConstValueParams<U>& initConstValueParams)
{
    CheckInitParams(dst, initConstValueParams, Impl::FILL_INTRINSIC_NAME);
    FillImpl(dst, initConstValueParams);
}

// InitConstValue has been updated, please use Fill instead.
template <typename T, typename U, typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type>
__aicore__ inline void InitConstValue(const LocalTensor<T>& dst, const InitConstValueParams<U>& initConstValueParams)
{
    CheckInitParams(dst, initConstValueParams, Impl::INITCONSTVALUE_INTRINSIC_NAME);
    InitConstValueImpl(dst, initConstValueParams);
}

/* **************************************************************************************************
 * SetLoadDataPaddingValue                                             *
 * ************************************************************************************************* */
/*
 * @ingroup SetLoadDataPaddingValue
 * @brief setting loadData pad value
 * @param [in]padValue padding value
 */
template <typename T>
__aicore__ inline void SetLoadDataPaddingValue(const T padValue)
{
    Load3DSetPaddingImpl(padValue);
}

/* **************************************************************************************************
 * SetFmatrix                                             *
 * ************************************************************************************************* */
/*
 * @ingroup SetFmatrix
 * @brief setting fmatrix
 * @param [in]l1H operand height
 * @param [in]l1W operand width
 * @param [in]padList padding list
 */
__aicore__ inline void SetFmatrix(uint16_t l1H, uint16_t l1W, const uint8_t padList[4], const FmatrixMode& fmatrixMode)
{
    ASCENDC_CHECK_VALUE_RANGE(l1H, MIN_LOAD3D_L1, MAX_LOAD3D_L1, "l1H", "SetFmatrix");
    ASCENDC_CHECK_VALUE_RANGE(l1W, MIN_LOAD3D_L1, MAX_LOAD3D_L1, "l1W", "SetFmatrix");
    SetFmatrixImpl(l1H, l1W, padList, fmatrixMode);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || \
                              (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
__aicore__ inline void SetFmatrix(const SetFMatrixBitModeParams& param, const FmatrixMode& fmatrixMode)
{
    SetFmatrixImpl(param, fmatrixMode);
}
#endif

/* **************************************************************************************************
 * SetLoadDataBoundary                                             *
 * ************************************************************************************************* */
/*
 * @ingroup SetLoadDataBoundary
 * @brief setting loaddata boundary
 * @param [in]boundaryValue
 */
__aicore__ inline void SetLoadDataBoundary(uint32_t boundaryValue) { SetLoadDataBoundaryImpl(boundaryValue); }

/* **************************************************************************************************
 * SetLoadDataRepeat                                             *
 * ************************************************************************************************* */
__aicore__ inline void SetLoadDataRepeat(const LoadDataRepeatParam& repeatParams)
{
    ASCENDC_CHECK_VALUE_RANGE(repeatParams.repeatMode, 0, 1, "repeatMode", "SetLoadDataRepeat");
    SetLoadDataRepeatImpl(repeatParams);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || \
                              (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
__aicore__ inline void SetLoadDataRepeatWithStride(const LoadDataRepeatParamWithStride& repeatParams)
{
    ASCENDC_CHECK_VALUE_RANGE(repeatParams.repeatMode, 0, 1, "repeatMode", "SetLoadDataRepeat");
    SetLoadDataRepeatWithStrideImpl(repeatParams);
}
#endif

/* **************************************************************************************************
 * LoadImageToLocal                                             *
 * ************************************************************************************************* */
/*
 * @ingroup LoadImageToLocal
 * @brief loadData image from gm to L1
 * @param [out] dst output LocalTensor
 * @param [in] loadImageToLocalParams.horizSize operand height
 * @param [in] loadImageToLocalParams.vertSize operand width
 * @param [in] loadImageToLocalParams.horizStartPos horizontal start position
 * @param [in] loadImageToLocalParams.vertStartPos vertical start position
 * @param [in] loadImageToLocalParams.srcHorizSize src horizontal size
 * @param [in] loadImageToLocalParams.topPadSize top padding size
 * @param [in] loadImageToLocalParams.botPadSize bottom padding size
 * @param [in] loadImageToLocalParams.leftPadSize left hblank/padding size
 * @param [in] loadImageToLocalParams.rightPadSize right hblank/padding size
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadImageToLocal(
    const LocalTensor<T>& dst, const LoadImageToLocalParams& loadDataParams)
{
#ifdef ASCENDC_TIME_STAMP_ON
    PrintTimeStamp(static_cast<uint32_t>(TimeStampId::TIME_STAMP_MTE2_DATACOPY));
#endif
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    ASCENDC_ASSERT(CheckFuncLoadImageToLocal(dst, loadDataParams, "LoadImageToLocal"), {
        ASCENDC_REPORT_CHECK_ERROR("LoadImageToLocal", KernelFuncType::NONE_MODE);
    });
#endif
    ASCENDC_CHECK_VALUE_RANGE(loadDataParams.horizSize, 2, UINT12_MAX, "horizSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(loadDataParams.vertSize, 2, UINT12_MAX, "vertSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(loadDataParams.horizStartPos, 0, UINT12_MAX, "horizStartPos", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(loadDataParams.vertStartPos, 0, UINT12_MAX, "vertStartPos", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(loadDataParams.srcHorizSize, 2, UINT12_MAX, "srcHorizSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(loadDataParams.topPadSize, 0, 32, "topPadSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(loadDataParams.botPadSize, 0, 32, "botPadSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(loadDataParams.leftPadSize, 0, 32, "leftPadSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(loadDataParams.rightPadSize, 0, 32, "rightPadSize", "LoadImageToLocal");
    const Hardware dstScope = GetPhyType((TPosition)dst.GetPosition());
    if (dstScope == Hardware::L1) {
        LoadImageToLocalCal((__cbuf__ PrimT<T>*)dst.GetPhyAddr(), loadDataParams);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "dst", "L1 Buffer(A1/B1)", "LoadImageToLocal",
            GetPositionDisplay(static_cast<TPosition>(dst.GetPosition())));
    }
}

/* **************************************************************************************************
 * StoreImageToGlobal
 * ************************************************************************************************* */
/*
 * @ingroup StoreImageToGlobal
 * @brief store image from L1 to global memory
 * @param [out] dst output GlobalTensor
 * @param [in] src input LocalTensor, stored in L1
 * @param [in] storeImageToGlobalParams.outputDataType output data type
 * @param [in] storeImageToGlobalParams.channelTransformMode channel transform mode
 * @param [in] storeImageToGlobalParams.dtcParams data type conversion parameters (dtcGain, dtcOffset, roundingMode)
 * @param [in] storeImageToGlobalParams.dstImageWidth destination image width
 * @param [in] storeImageToGlobalParams.dstImageHeight destination image height
 * @param [in] storeImageToGlobalParams.srcImageStride source image stride
 * @param [in] storeImageToGlobalParams.dstImageStride destination image stride
 */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5162)
template <typename T, typename U>
__aicore__ inline __inout_pipe__(MTE3) void StoreImageToGlobal(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const StoreImageToGlobalParams& params)
{
    const Hardware srcScope = GetPhyType((TPosition)src.GetPosition());
    if (srcScope == Hardware::L1) {
        StoreImageToGlobalCal((__gm__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<U>*)src.GetPhyAddr(), params);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "src", "L1 Buffer(A1/B1)", "StoreImageToGlobal",
            GetPositionDisplay(static_cast<TPosition>(src.GetPosition())));
    }
}
#endif

/* **************************************************************************************************
 * LoadDataUnzip                                             *
 * ************************************************************************************************* */
/*
 * @ingroup LoadDataUnzip
 * @brief loadData and unzip
 * @param [out] dst output LocalTensor
 * @param [in] src input GlobalTensor
 */
template <typename T>
__aicore__ inline void LoadDataUnzip(const LocalTensor<T>& dst, const GlobalTensor<T>& src)
{
    LoadDataUnzipImpl(dst, src);
}

/*
 * @brief Sets whether to enable HF32 mode for Mmad computation
 * @param [in] mode HF32 mode enumeration
 * @note When mode is HF32Mode::ENABLE, FP32 data in L0A/L0B will be rounded to HF32 before matrix multiplication;
 * when mode is HF32Mode::DISABLE, regular FP32 matrix multiplication will be executed
 */
__aicore__ inline void SetHF32Mode(HF32Mode mode) { SetHF32ModeImpl(mode == AscendC::HF32Mode::ENABLE); }

/*
 * @brief Sets whether to enable HF32 mode for Mmad computation
 * @param [in] hf32Mode Control parameter for Mmad HF32 mode
 * @note When hf32Mode is true, FP32 data in L0A/L0B will be rounded to HF32 before matrix multiplication; when false,
 * regular FP32 matrix multiplication will be executed
 */
// SetHF32Mode(bool hf32Mode) has been updated, please use SetHF32Mode(HF32Mode mode) instead.
__aicore__ inline void SetHF32Mode(bool hf32Mode) { SetHF32ModeImpl(hf32Mode); }

/*
 * @brief Sets the rounding method for HF32 rounding mode
 * @param [in] mode HF32 trans mode enumeration
 * @note Must call SetHF32Mode to enable HF32 rounding mode first.
 */
__aicore__ inline void SetHF32TransMode(HF32TransMode mode)
{
    SetHF32TransModeImpl(mode == AscendC::HF32TransMode::NEAREST_AWAY);
}

/*
 * @brief Sets the rounding method for HF32 rounding mode
 * @param [in] hf32TransMode Control parameter for Mmad HF32 mode
 * @note Must Call SetHF32Mode to enable HF32 rounding mode first.When hf32TransMode is true, FP32 is rounded to HF32
 * with rounding to nearest, with ties away from zero; when false, rounded to nearest, with ties to even
 */
// SetHF32TransMode(bool hf32TransMode) has been updated, please use SetHF32TransMode(HF32TransMode mode) instead.
__aicore__ inline void SetHF32TransMode(bool hf32TransMode) { SetHF32TransModeImpl(hf32TransMode); }

/*
 * @ingroup MMLayout
 * @brief Sets matrix multiplication result layout to row major
 * @note This function sets the CUBE output to row major format (M direction first, then N direction)
 */
__aicore__ inline void SetMMRowMajor() { SetMMLayoutTransformImpl(true); }

/*
 * @ingroup MMLayout
 * @brief Sets matrix multiplication result layout to column major
 * @note This function sets the CUBE output to column major format (N direction first, then M direction)
 */
__aicore__ inline void SetMMColumnMajor() { SetMMLayoutTransformImpl(false); }

/*
 * @brief Sets the priority direction (M or N) for Mmad/MmadWithSparse computation
 * @param [in] mmLayoutMode Control parameter for Mmad/MmadWithSparse priority direction
 * @note When mmLayoutMode is true, CUBE generates results first through N direction then M direction; when false, first
 * through M direction then N direction
 */
// SetMMLayoutTransform has been updated, please use SetMMRowMajor/SetMMColumnMajor instead.
__aicore__ inline void SetMMLayoutTransform(bool mmLayoutMode) { SetMMLayoutTransformImpl(mmLayoutMode); }

/* **************************************************************************************************
 * Pointer-input overloads for the CUBE basic API -- definitions.
 *
 * All of these land on the same *Cal backend that the Tensor overload in this header reaches, so
 * the Tensor path and the pointer path share one implementation of the arithmetic.
 * ************************************************************************************************* */
namespace CubePtrOverload {
// -------------------------------------------------------------------------------------------
// Address-space dispatch, and how it degenerates under ASCENDC_CPU_DEBUG.
//
// On the device build an address-space qualifier is a real, distinct type (bisheng defines
// __ca__ as __attribute__((cce_cube_a))), so `__ca__ D*` and `__cb__ D*` are two different
// overloads and the target pipe is selected at compile time -- no TPosition lookup is needed.
//
// Under ASCENDC_CPU_DEBUG the platform's own tikicpulib/include/stub_def.h defines
// __gm__ / __cbuf__ / __ubuf__ / __cc__ / __ca__ / __cb__ as *empty* macros.  Those two
// signatures therefore collapse into one and defining both is a redefinition.  The call site
// collapses with them -- `(__ca__ T*)p` and `(__cb__ T*)p` are the same expression in that build
// -- so the position simply does not exist there and no implementation can recover it.  Exactly
// one definition per helper is thus both necessary and all the build can express.
// -------------------------------------------------------------------------------------------

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1

// L1 -> L0A.  The address space is erased in this build, so the A pipe is taken as the single
// representative.  This carries no numeric consequence: the CPU-debug build does not observably
// execute the CUBE pipe at all (the PV memory model behind it is stubbed out).
template <typename D, typename S, typename P>
__aicore__ inline void Ld12L0(D* dst, S* src, const P& loadDataParams)
{
    LoadData2DL12L0ACal(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
// L1 -> L0A together with the scale matrix (mx) operand.
template <typename D, typename S>
__aicore__ inline void Ld12L0Mx(
    D* dst, S* src, fp8_e8m0_t* srcMx, const LoadData2DParamsV2& loadDataParams,
    const LoadData2DMxParams& loadMxDataParams)
{
    // No explicit template arguments: the callee is `template <typename T, typename U = T>`
    // with the parameter list `(__ca__ U* dst, __cbuf__ T* src0, ...)` -- dst's element type
    // is U and src0's is T.  Naming them here as <D, S> (D = dst pointee) would bind T to the
    // dst element type and U to the src one, i.e. exactly swapped, so it would compile only
    // when both element types are the same.  The Tensor path relies on deduction; so do we.
    LoadData2DL12L0ACal(dst, src, srcMx, loadDataParams, loadMxDataParams);
}
#endif

template <typename D, typename S, typename U>
__aicore__ inline void Ld3DV2(D* dst, S* src, const LoadData3DParamsV2<U>& loadDataParams)
{
    // Same two global load-3d settings the device variant applies, and for the same reason: the
    // Tensor path reaches LoadDataImpl<T, IS_RESER_LOAD3D_DEFAULT_CONFIG> in this build too.
    Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    Load3DSetPaddingCal(loadDataParams.padValue);
    LoadData3DV2L12L0ACal<U>((U*)dst, (U*)src, loadDataParams);
}

// The ASCENDC_CPU_DEBUG representative of the Ld3DV1 group; see the note on the device
// overloads below.
template <typename D, typename S, typename U>
__aicore__ inline void Ld3DV1(D* dst, S* src, const LoadData3DParamsV1<U>& loadDataParams)
{
    Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    Load3DSetPaddingCal(loadDataParams.padValue);
    LoadData3DV1L12L0ACal<U>((U*)dst, (U*)src, loadDataParams);
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
// LoadDataWithStride additionally reprograms two global load-3d settings, exactly as the Tensor
// path does in LoadDataWithStrideImpl: F-matrix (l1H / l1W / padList) and the padding value.
// Those are global machine state, not parameters of the transfer, so the *Cal backend does not
// touch them -- the pointer path must reproduce them per DefaultConfig flag rather than forward.
template <const IsResetLoad3dConfig& DefaultConfig, typename D, typename S, typename U>
__aicore__ inline void Ld3DV2Stride(D* dst, S* src, const LoadData3DParamsV2<U>& loadDataParams)
{
    if constexpr (DefaultConfig.isSetFMatrix) {
        Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    }
    if constexpr (DefaultConfig.isSetPadding) {
        Load3DSetPaddingCal(loadDataParams.padValue);
    }
    LoadData3DV2L12L0AWithStrideCal<U>((U*)dst, (U*)src, loadDataParams);
}
#endif

// L1 -> L0A with transpose.
template <typename D, typename S, typename P>
__aicore__ inline void Ld12L0Transpose(D* dst, S* src, const P& loadDataParams)
{
    LoadData2DL12L0ATransposeCal(dst, src, loadDataParams);
}
/*
 * The V2 descriptor reaches a different backend set from the V1 one, so it gets its own
 * overloads rather than being absorbed by the generic `const P&` forms above.
 *
 * LoadDataWithTransposeImpl's V2 overload (impl/basic_api/kernel_operator_mm_load2d_impl.h:155-166)
 * has a branch for L0B *only*: for an L0A destination it compiles and performs no transfer.  The
 * address space that would distinguish the two is erased in this build, and the single V2 backend
 * that exists -- LoadData2DL12L0BTransposeCal(dav_3510/kernel_operator_mm_impl.h:289) -- is the L0B
 * one, so that is the representative taken here.
 */
template <typename D, typename S>
__aicore__ inline void Ld12L0Transpose(D* dst, S* src, const LoadData2dTransposeParamsV2& loadDataParams)
{
    LoadData2DL12L0BTransposeCal(dst, src, loadDataParams);
}

#else // device build: the address space selects the pipe

// L1 -> L0A / L0B.  The address space of `dst` selects the pipe, so no TPosition lookup is needed.
template <typename D, typename S, typename P>
__aicore__ inline void Ld12L0(__ca__ D* dst, __cbuf__ S* src, const P& loadDataParams)
{
    LoadData2DL12L0ACal(dst, src, loadDataParams);
}

template <typename D, typename S, typename P>
__aicore__ inline void Ld12L0(__cb__ D* dst, __cbuf__ S* src, const P& loadDataParams)
{
    LoadData2DL12L0BCal(dst, src, loadDataParams);
}

/*
 * GM -> L1。上两条只覆盖「源在 L1」的两种目的地，源在 GM 时没有可用形参，因此
 * LoadData((__cbuf__ half*)p, gmSrc, params) 会直接 no matching function，而不是派发错。
 * 两个描述符版本各有对应后端：dav_3510/kernel_operator_mm_impl.h:47（V1）与 :430（V2），
 * 这里与本节其余重载一样按描述符类型选择。
 */
template <typename D, typename S, typename P>
__aicore__ inline void Ld12L0(__cbuf__ D* dst, __gm__ S* src, const P& loadDataParams)
{
    LoadData2DGM2L1Cal(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
// L1 -> L0A / L0B together with the scale matrix (mx) operand.
template <typename D, typename S>
__aicore__ inline void Ld12L0Mx(
    __ca__ D* dst, __cbuf__ S* src, __cbuf__ fp8_e8m0_t* srcMx, const LoadData2DParamsV2& loadDataParams,
    const LoadData2DMxParams& loadMxDataParams)
{
    // No explicit template arguments: the callee is `template <typename T, typename U = T>`
    // with the parameter list `(__ca__ U* dst, __cbuf__ T* src0, ...)` -- dst's element type
    // is U and src0's is T.  Naming them here as <D, S> (D = dst pointee) would bind T to the
    // dst element type and U to the src one, i.e. exactly swapped, so it would compile only
    // when both element types are the same.  The Tensor path relies on deduction; so do we.
    LoadData2DL12L0ACal(dst, src, srcMx, loadDataParams, loadMxDataParams);
}

template <typename D, typename S>
__aicore__ inline void Ld12L0Mx(
    __cb__ D* dst, __cbuf__ S* src, __cbuf__ fp8_e8m0_t* srcMx, const LoadData2DParamsV2& loadDataParams,
    const LoadData2DMxParams& loadMxDataParams)
{
    // No explicit template arguments: the callee is `template <typename T, typename U = T>`
    // with the parameter list `(__ca__ U* dst, __cbuf__ T* src0, ...)` -- dst's element type
    // is U and src0's is T.  Naming them here as <D, S> (D = dst pointee) would bind T to the
    // dst element type and U to the src one, i.e. exactly swapped, so it would compile only
    // when both element types are the same.  The Tensor path relies on deduction; so do we.
    LoadData2DL12L0BCal(dst, src, srcMx, loadDataParams, loadMxDataParams);
}
#endif

/*
 * L1 -> L0A / L0B, 3dv2 (plain and with L1 stride).
 *
 * LoadData3DParamsV2<U> is authoritative for the transfer geometry: kExtension / mExtension /
 * channelSize / strideW / strideH are all expressed in U units.  Equally, the *Cal backends below
 * never use their element type for address arithmetic -- they forward dst and src unchanged to
 * img2colv2_cbuf_to_ca / _cb, and T only selects the overload (a half / bfloat16 specialisation
 * exists) and feeds a supported-dtype static_assert.  The element type of the pointer operand is
 * therefore a *view*, not part of the transfer description, and a caller may present the L1
 * operand through a wider view of the same data (a float* view of half data, an int* view of int8
 * data) exactly as the address-space cast in the call site expresses.  The pointer is re-viewed as
 * U so that the backend is selected by the authoritative type; no address is recomputed and no
 * parameter is reinterpreted, so the instruction issued is byte-for-byte the same.
 */
/*
 * F-matrix / padding: the Tensor path reaches LoadDataImpl<T, IS_RESER_LOAD3D_DEFAULT_CONFIG>
 * (defaultConfig = {true, true}), which programs these two pieces of GLOBAL load-3d state before the
 * transfer.  They are not parameters of the transfer, so the *Cal backend does not touch them and
 * the pointer path must reproduce them itself.  Measured on dav-2201: without these two calls the
 * pointer arm of batch_matmul diverged from the Tensor arm (rdiff 0.10-0.24) at asc:186, the single
 * LoadData L1->L0B site; with them the case passes.  LoadData3DParamsV2Pro's Tensor path does not
 * program them, so Ld3DV2Pro deliberately does not either.
 */
template <typename D, typename S, typename U>
__aicore__ inline void Ld3DV2(__ca__ D* dst, __cbuf__ S* src, const LoadData3DParamsV2<U>& loadDataParams)
{
    Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    Load3DSetPaddingCal(loadDataParams.padValue);
    LoadData3DV2L12L0ACal<U>((__ca__ U*)dst, (__cbuf__ U*)src, loadDataParams);
}

template <typename D, typename S, typename U>
__aicore__ inline void Ld3DV2(__cb__ D* dst, __cbuf__ S* src, const LoadData3DParamsV2<U>& loadDataParams)
{
    Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    Load3DSetPaddingCal(loadDataParams.padValue);
    LoadData3DV2L12L0BCal<U>((__cb__ U*)dst, (__cbuf__ U*)src, loadDataParams);
}

/* ---- LoadData 3dv1: L1 -> L0A / L0B / UB ------------------------------------- */
/*
 * The Tensor path reaches LoadDataImpl<T, defaultConfig>, which programs the two pieces of GLOBAL
 * load-3d state (F-matrix from l1H / l1W / padList, and the padding value) and then picks the
 * backend from GetPhyType(dst.GetPosition()) -- L0A, L0B or UB
 * (kernel_operator_mm_base_impl.h:112-136).  On the device build the address space of a raw pointer
 * names exactly that role, so the three overloads below fix the position the impl would have
 * derived and forward to the same *Cal backend with the same descriptor.  The two global settings
 * are reproduced here for the same reason Ld3DV2 reproduces them: they are machine state, not
 * parameters of the transfer, so the *Cal entry does not touch them.
 *
 * Only the default IS_RESER_LOAD3D_DEFAULT_CONFIG is offered, matching the precedent the delivered
 * pointer form of LoadData 3dv2 set.  A caller that needs a non-default IsResetLoad3dConfig (to
 * leave F-matrix or padding alone) must keep using the Tensor interface, where the flag is a
 * template argument.
 *
 * Under ASCENDC_CPU_DEBUG the three address spaces collapse into one signature, so a single
 * representative is defined -- the A pipe, the same choice Ld12L0 and InitConst make, and one that
 * carries no numeric consequence because that build does not execute the CUBE pipe.
 */
template <typename D, typename S, typename U>
__aicore__ inline void Ld3DV1(__ca__ D* dst, __cbuf__ S* src, const LoadData3DParamsV1<U>& loadDataParams)
{
    Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    Load3DSetPaddingCal(loadDataParams.padValue);
    LoadData3DV1L12L0ACal<U>((__ca__ U*)dst, (__cbuf__ U*)src, loadDataParams);
}

template <typename D, typename S, typename U>
__aicore__ inline void Ld3DV1(__cb__ D* dst, __cbuf__ S* src, const LoadData3DParamsV1<U>& loadDataParams)
{
    Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    Load3DSetPaddingCal(loadDataParams.padValue);
    LoadData3DV1L12L0BCal<U>((__cb__ U*)dst, (__cbuf__ U*)src, loadDataParams);
}

template <typename D, typename S, typename U>
__aicore__ inline void Ld3DV1(__ubuf__ D* dst, __cbuf__ S* src, const LoadData3DParamsV1<U>& loadDataParams)
{
    Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    Load3DSetPaddingCal(loadDataParams.padValue);
    LoadData3DV1L12UBCal<U>((__ubuf__ U*)dst, (__cbuf__ U*)src, loadDataParams);
}

/*
 * LoadDataWithStride additionally reprograms two global load-3d settings, exactly as the Tensor
 * path does in LoadDataWithStrideImpl: F-matrix (l1H / l1W / padList) and the padding value.  Those
 * are global machine state, not parameters of the transfer, so the backend *Cal entry does not
 * touch them -- the pointer path must reproduce them per DefaultConfig flag rather than forward.
 * A caller that passes explicit <T, {false,false}> gets the same behaviour as the Tensor overload
 * with that config: both settings are left alone (the call site is expected to have set them).
 */
template <const IsResetLoad3dConfig& DefaultConfig, typename D, typename S, typename U>
__aicore__ inline void Ld3DV2Stride(__ca__ D* dst, __cbuf__ S* src, const LoadData3DParamsV2<U>& loadDataParams)
{
    if constexpr (DefaultConfig.isSetFMatrix) {
        Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    }
    if constexpr (DefaultConfig.isSetPadding) {
        Load3DSetPaddingCal(loadDataParams.padValue);
    }
    LoadData3DV2L12L0AWithStrideCal<U>((__ca__ U*)dst, (__cbuf__ U*)src, loadDataParams);
}

template <const IsResetLoad3dConfig& DefaultConfig, typename D, typename S, typename U>
__aicore__ inline void Ld3DV2Stride(__cb__ D* dst, __cbuf__ S* src, const LoadData3DParamsV2<U>& loadDataParams)
{
    if constexpr (DefaultConfig.isSetFMatrix) {
        Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    }
    if constexpr (DefaultConfig.isSetPadding) {
        Load3DSetPaddingCal(loadDataParams.padValue);
    }
    LoadData3DV2L12L0BWithStrideCal<U>((__cb__ U*)dst, (__cbuf__ U*)src, loadDataParams);
}

// L1 -> L0A / L0B with transpose.
template <typename D, typename S, typename P>
__aicore__ inline void Ld12L0Transpose(__ca__ D* dst, __cbuf__ S* src, const P& loadDataParams)
{
    LoadData2DL12L0ATransposeCal(dst, src, loadDataParams);
}

template <typename D, typename S, typename P>
__aicore__ inline void Ld12L0Transpose(__cb__ D* dst, __cbuf__ S* src, const P& loadDataParams)
{
    LoadData2DL12L0BTransposeCal(dst, src, loadDataParams);
}
/*
 * V2 descriptor, device build -- it reaches a backend for L0B only, exactly as
 * LoadDataWithTransposeImpl does (impl/basic_api/kernel_operator_mm_load2d_impl.h:155-166).
 *
 *   - L0B forwards to LoadData2DL12L0BTransposeCal(dav_3510/kernel_operator_mm_impl.h:289), the
 *     real V2 implementation.
 *   - L0A has no V2 backend to forward to.  The only A1 -> A2 transpose backend,
 *     LoadData2DL12L0ATransposeCal(dav_3510/kernel_operator_mm_impl.h:445), takes the *V1*
 *     descriptor and reports "not supported on current device".  The Tensor V2 overload calls no
 *     backend at all for an L0A destination -- its L0A branch does not exist -- so this overload
 *     performs no transfer either.  Mirroring that is what keeps a pointer call identical to the
 *     equivalent Tensor call; the silence is inherited from the Tensor path, not introduced here.
 */
template <typename D, typename S>
__aicore__ inline void Ld12L0Transpose(
    __ca__ D* dst, __cbuf__ S* src, const LoadData2dTransposeParamsV2& loadDataParams)
{
    (void)dst;
    (void)src;
    (void)loadDataParams;
}

template <typename D, typename S>
__aicore__ inline void Ld12L0Transpose(
    __cb__ D* dst, __cbuf__ S* src, const LoadData2dTransposeParamsV2& loadDataParams)
{
    LoadData2DL12L0BTransposeCal(dst, src, loadDataParams);
}

#endif // ASCENDC_CPU_DEBUG

/* ---- InitConstValue: L1 (A1/B1) / L0A (A2) / L0B (B2) ----------------------- */
/*
 * The Tensor path picks the backend from the operand's TPosition (InitConstValueImpl,
 * kernel_operator_mm_base_impl.h:737).  A raw pointer carries that role in its own address space,
 * so the device overloads below recover the same selection at compile time.
 *
 * On dav_3510 the A2 and B2 backends are themselves "not supported on current device" stubs, so L1
 * is both the only form with real behaviour there and the representative kept under
 * ASCENDC_CPU_DEBUG, where the three address spaces collapse into a single signature.
 */
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1

template <typename D, typename P>
__aicore__ inline void InitConst(D* dst, const P& initConstValueParams)
{
    InitL1BufferCal(dst, initConstValueParams);
}

#else

template <typename D, typename P>
__aicore__ inline void InitConst(__cbuf__ D* dst, const P& initConstValueParams)
{
    InitL1BufferCal(dst, initConstValueParams);
}

template <typename D, typename P>
__aicore__ inline void InitConst(__ca__ D* dst, const P& initConstValueParams)
{
    InitL0ANzMatrixCal(dst, initConstValueParams);
}

template <typename D, typename P>
__aicore__ inline void InitConst(__cb__ D* dst, const P& initConstValueParams)
{
    InitL0BNzMatrixCal(dst, initConstValueParams);
}

#endif

/* ---- LoadImageToLocal: the destination is always L1 (__cbuf__) -------------- */
/*
 * LoadImageToLocal reaches exactly one backend, LoadImageToLocalCal(__cbuf__ T*,
 * const LoadImageToLocalParams&) (dav_3510/kernel_operator_mm_impl.h:1495), so there is no pipe
 * to select and therefore no address-space dispatch to perform.
 */
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1

template <typename D>
__aicore__ inline void LdImage(D* dst, const LoadImageToLocalParams& loadDataParams)
{
    LoadImageToLocalCal(dst, loadDataParams);
}

#else

template <typename D>
__aicore__ inline void LdImage(__cbuf__ D* dst, const LoadImageToLocalParams& loadDataParams)
{
    LoadImageToLocalCal(dst, loadDataParams);
}

#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

/* ---- LoadData with a bit-mode descriptor: L1 -> L0A / L0B ------------------- */
/*
 * The Tensor path selects the backend from the Dst TPosition *template argument*
 * (kernel_operator_mm_base_impl.h:333 for 3D, kernel_operator_mm_load2d_impl.h:83 for 2D, both
 * dav_3510-only).  The pointer path keeps that explicit Dst / Src template argument list, and
 * additionally checks it against the address space of the pointer, which is the same information
 * expressed in the type of the operand itself.
 *
 * No cast to the address space is available on this path: bisheng rejects casting a generic `D*`
 * to `__ca__ T*` inside an aicore function ("casting 'D *' to type '__ca__ T *' is not allowed in
 * aicore function").  The backend is therefore selected by an address-space overload -- the same
 * shape already used for Ld12L0 above -- and dst / src must carry the address space of the operand
 * they name.  The backend also takes one element type for both operands, exactly as the Tensor form
 * takes LocalTensor<T> twice, so dst and src must share a pointee type; no re-view is possible and
 * none is performed, so the instruction issued is the one the operands describe.
 */
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1

// The address space is erased in this build, so the A pipe is the single representative, exactly
// as for Ld12L0 above.  Dst still has to name a destination the bit-mode form supports.
template <TPosition Dst, typename D, typename S>
__aicore__ inline void LdBm3D(D* dst, S* src, const Load3DBitModeParam& loadDataParams)
{
    static_assert(
        Dst == TPosition::A2 || Dst == TPosition::B2,
        "LoadData with Load3DBitModeParam supports dst TPosition A2 / B2 only.");
    LoadData3DV2L12L0ACal(dst, src, loadDataParams);
}

template <TPosition Dst, typename D, typename S>
__aicore__ inline void LdBm2D(D* dst, S* src, const Load2DBitModeParam& loadDataParams)
{
    static_assert(
        Dst == TPosition::A2 || Dst == TPosition::B2,
        "LoadData with Load2DBitModeParam supports dst TPosition A2 / B2 only.");
    LoadData2DL12L0ACal(dst, src, loadDataParams);
}

#else

template <TPosition Dst, typename D, typename S>
__aicore__ inline void LdBm3D(__ca__ D* dst, __cbuf__ S* src, const Load3DBitModeParam& loadDataParams)
{
    static_assert(
        Dst == TPosition::A2, "LoadData with Load3DBitModeParam: dst TPosition A2 requires an L0A (__ca__) pointer.");
    LoadData3DV2L12L0ACal(dst, src, loadDataParams);
}

template <TPosition Dst, typename D, typename S>
__aicore__ inline void LdBm3D(__cb__ D* dst, __cbuf__ S* src, const Load3DBitModeParam& loadDataParams)
{
    static_assert(
        Dst == TPosition::B2, "LoadData with Load3DBitModeParam: dst TPosition B2 requires an L0B (__cb__) pointer.");
    LoadData3DV2L12L0BCal(dst, src, loadDataParams);
}

template <TPosition Dst, typename D, typename S>
__aicore__ inline void LdBm2D(__ca__ D* dst, __cbuf__ S* src, const Load2DBitModeParam& loadDataParams)
{
    static_assert(
        Dst == TPosition::A2, "LoadData with Load2DBitModeParam: dst TPosition A2 requires an L0A (__ca__) pointer.");
    LoadData2DL12L0ACal(dst, src, loadDataParams);
}

template <TPosition Dst, typename D, typename S>
__aicore__ inline void LdBm2D(__cb__ D* dst, __cbuf__ S* src, const Load2DBitModeParam& loadDataParams)
{
    static_assert(
        Dst == TPosition::B2, "LoadData with Load2DBitModeParam: dst TPosition B2 requires an L0B (__cb__) pointer.");
    LoadData2DL12L0BCal(dst, src, loadDataParams);
}

#endif

#endif

/* ---- LoadData 3dv2Pro: L1 -> L0A / L0B / UB --------------------------------- */
/*
 * LoadDataImpl for LoadData3DParamsV2Pro (kernel_operator_mm_base_impl.h:379) selects the pipe
 * from GetPhyType(dst.GetPosition()) and forwards the address to LoadData3DV2L12L0{A,B,UB}Cal.
 * The pointer path makes the same selection from the destination's address space; under
 * ASCENDC_CPU_DEBUG the three spaces collapse into one signature, so exactly one definition is
 * both necessary and all that build can express.
 */
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1

template <typename D, typename S>
__aicore__ inline void Ld3DV2Pro(D* dst, S* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadData3DV2L12L0ACal(dst, src, loadDataParams);
}

#else

template <typename D, typename S>
__aicore__ inline void Ld3DV2Pro(__ca__ D* dst, __cbuf__ S* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadData3DV2L12L0ACal(dst, src, loadDataParams);
}

template <typename D, typename S>
__aicore__ inline void Ld3DV2Pro(__cb__ D* dst, __cbuf__ S* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadData3DV2L12L0BCal(dst, src, loadDataParams);
}

template <typename D, typename S>
__aicore__ inline void Ld3DV2Pro(__ubuf__ D* dst, __cbuf__ S* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadData3DV2L12UBCal(dst, src, loadDataParams);
}

#endif

/* ---- LoadDataUnzip: L1 / L0A / L0B destination, GM source -------------------- */
/*
 * LoadDataUnzipImpl (kernel_operator_mm_base_impl.h:824) selects the backend from
 * GetPhyType(dst.GetPosition()) and forwards (__cbuf__|__ca__|__cb__ PrimT<T>*, __gm__ PrimT<T>*)
 * to LoadDataUnzipToL1Cal / ToL0ACal / ToL0BCal.  All three take raw pointers, so the pointer form
 * is a pure forwarding layer over the destination's address space.
 *
 * ARCH AVAILABILITY IS THE TENSOR OVERLOAD'S, EXACTLY -- AND IT IS EMPTY ON dav_3510.  The three
 * backends exist only in dav_c100:395, dav_c220:697, dav_m200:605, dav_m300:611 and dav_m310:606;
 * there is no LoadDataUnzip symbol of any kind in impl/basic_api/dav_3510/.  On dav_3510
 * LoadDataUnzipImpl therefore does not compile for ANY operand form -- measured, not inferred:
 * the Tensor probe fails there with "use of undeclared identifier 'LoadDataUnzipToL1Cal'".  The
 * repository says the same thing itself, wrapping its own header-check test in
 * `#if __NPU_ARCH__ != 3510` (tests/api/basic_api/ascendc_header_checker/kernel_operator_mm_intf.cpp:262).
 *
 * This overload deliberately carries NO arch guard, so that on dav_3510 a pointer call fails in the
 * same way, with the same message, as the Tensor call it mirrors, rather than appearing available.
 */
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1

// The address space is erased in this build, so the L1 backend is the single representative.
template <typename D, typename S>
__aicore__ inline void LdUnzip(D* dst, S* src)
{
    LoadDataUnzipToL1Cal(dst, src);
}

#else

template <typename D, typename S>
__aicore__ inline void LdUnzip(__cbuf__ D* dst, __gm__ S* src)
{
    LoadDataUnzipToL1Cal(dst, src);
}

template <typename D, typename S>
__aicore__ inline void LdUnzip(__ca__ D* dst, __gm__ S* src)
{
    LoadDataUnzipToL0ACal(dst, src);
}

template <typename D, typename S>
__aicore__ inline void LdUnzip(__cb__ D* dst, __gm__ S* src)
{
    LoadDataUnzipToL0BCal(dst, src);
}

#endif
} // namespace CubePtrOverload

/* ---- LoadDataUnzip with raw pointer operands -------------------------------- */
// Same arch availability as the Tensor overload, deliberately and for the reason spelled out at
// CubePtrOverload::LdUnzip above.
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002) || (__NPU_ARCH__ == 2201) || \
                              (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3102))
template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadDataUnzip(DstPtr dst, SrcPtr src)
{
    CubePtrOverload::LdUnzip(dst, src);
}
#endif

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr>::value, bool>::type>
__aicore__ inline void Mmad(DstPtr dst, Src0Ptr fm, Src1Ptr filter, const MmadParams& mmadParams)
{
    MmadCal(dst, fm, filter, mmadParams);
}

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr, typename BiasPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr, BiasPtr>::value, bool>::type>
__aicore__ inline void Mmad(DstPtr dst, Src0Ptr fm, Src1Ptr filter, BiasPtr bias, const MmadParams& mmadParams)
{
    // A raw pointer no longer carries the TPosition of the bias operand, so the CO1 / C2 distinction
    // that MmadImpl() derives from it cannot be recovered here.  This signature exists to source the
    // C matrix from the bias table buffer, which is the only role the Tensor overload maps onto it,
    // and the corpus passes a C2 operand; cmatrixSource is therefore fixed to true.  A caller that
    // needs a CO1 bias and cmatrixSource == false must keep using the LocalTensor overload, where the
    // operand position is still known.
    //
    // MEASURED, and the reason the address space is NOT used to derive the flag instead: the corpus'
    // own bias scenario passes `(__cc__ T*)bias2Local.GetPhyAddr()` for a buffer whose TPosition is
    // C2 -- the cast exists only to satisfy this signature.  So the qualifier does not identify the
    // buffer role, and hard-coding true is what reproduces the Tensor call byte for byte (golden
    // error ratio 0.0000; writing false there does not).
    MmadCal(dst, fm, filter, (uint64_t)bias, mmadParams, true);
}

/*
 * BitMode forms of Mmad with raw pointer operands.
 *
 * Both land on the pointer-parameter BitMode backends in dav_3510/kernel_operator_mm_impl.h --
 * MmadCal(__cc__ DstT*, __ca__ Src0T*, __cb__ Src1T*, const MmadBitModeParams&) at :534 and
 * MmadCal(__cc__ DstT*, __ca__ Src0T*, __cb__ Src1T*, uint64_t, MmadBitModeParams&) at :568 --
 * which take the same operands the pointer form receives, so these are pure forwarding layers:
 * no address is recomputed and no parameter is reinterpreted.
 *
 * 带 bias 的后端会对拿到的描述符执行 SetCmatrixInitVal(false)
 * （dav_3510/kernel_operator_mm_impl.h:568 起，写入在 :572）。Tensor 路径与下面的指针形都在
 * 本地拷贝后再传，调用方对象因此不会被改写——也正因为写入由副本承接，这里的形参可以收
 * const 引用，const 对象与临时对象都能绑定。
 */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr>::value, bool>::type>
__aicore__ inline void Mmad(DstPtr dst, Src0Ptr fm, Src1Ptr filter, const MmadBitModeParams& mmadParams)
{
    MmadCal(dst, fm, filter, mmadParams);
}

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr, typename BiasPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr, BiasPtr>::value, bool>::type>
__aicore__ inline void Mmad(DstPtr dst, Src0Ptr fm, Src1Ptr filter, BiasPtr bias, const MmadBitModeParams& mmadParams)
{
    // The bias backend calls mmadParams.SetCmatrixInitVal(false) on the object it is handed.  Pass a
    // copy, as the Tensor path does, so the caller's descriptor is left untouched.
    MmadBitModeParams bitModeParams = mmadParams;
    MmadCal(dst, fm, filter, (uint64_t)bias, bitModeParams);
}

#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr>::value, bool>::type>
__aicore__ inline void MmadMx(DstPtr dst, Src0Ptr fm, Src1Ptr filter, const MmadParams& mmadParams)
{
    MmadMxCal(dst, fm, filter, mmadParams);
}

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr, typename BiasPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr, BiasPtr>::value, bool>::type>
__aicore__ inline void MmadMx(DstPtr dst, Src0Ptr fm, Src1Ptr filter, BiasPtr bias, const MmadParams& mmadParams)
{
    // Same TPosition caveat, and the same measurement, as the pointer form of Mmad above.
    MmadMxCal(dst, fm, filter, (uint64_t)bias, mmadParams, true);
}

// BitMode 形：转发到 MmadMxCal 的对应后端（dav_3510/kernel_operator_mm_impl.h:656 与 :677）。
// 后端形参是非 const 的 MmadBitModeParams&，带 bias 的那条会写调用方对象
// （:688 的 SetCmatrixInitVal(false)）；两条都在本地拷贝后再传，
// 因此这里可以收 const 引用——调用方传 const 对象或临时对象都能绑定。
template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr>::value, bool>::type>
__aicore__ inline void MmadMx(DstPtr dst, Src0Ptr fm, Src1Ptr filter, const MmadBitModeParams& mmadParams)
{
    MmadBitModeParams bitModeParams = mmadParams;
    MmadMxCal(dst, fm, filter, bitModeParams);
}

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr, typename BiasPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr, BiasPtr>::value, bool>::type>
__aicore__ inline void MmadMx(
    DstPtr dst, Src0Ptr fm, Src1Ptr filter, BiasPtr bias, const MmadBitModeParams& mmadParams)
{
    MmadBitModeParams bitModeParams = mmadParams;
    MmadMxCal(dst, fm, filter, (uint64_t)bias, bitModeParams);
}
#endif

template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const LoadData2DParams& loadDataParams)
{
    CubePtrOverload::Ld12L0(dst, src, loadDataParams);
}

template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const LoadData2DParamsV2& loadDataParams)
{
    CubePtrOverload::Ld12L0(dst, src, loadDataParams);
}

template <
    typename DstPtr, typename SrcPtr, typename U,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const LoadData3DParamsV2<U>& loadDataParams)
{
    CubePtrOverload::Ld3DV2(dst, src, loadDataParams);
}

template <
    typename DstPtr, typename SrcPtr, typename U,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const LoadData3DParamsV1<U>& loadDataParams)
{
    CubePtrOverload::Ld3DV1(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
template <
    typename DstPtr, typename SrcPtr, typename SrcMxPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, SrcMxPtr>::value, bool>::type>
__aicore__ inline void LoadData(
    DstPtr dst, SrcPtr src, SrcMxPtr srcMx, const LoadData2DParamsV2& loadDataParams,
    const LoadData2DMxParams& loadMxDataParams)
{
    CubePtrOverload::Ld12L0Mx(dst, src, srcMx, loadDataParams, loadMxDataParams);
}
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
// LoadDataWithStride reaches LoadData3DV2L12L0{A,B}WithStrideCal, which exists only in
// impl/basic_api/dav_3510.  The Tensor declaration and definition carry the same guard,
// so the pointer overloads must too, or a non-3510 arch fails to declare the backend.
template <
    typename DstPtr, typename SrcPtr, typename U,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadDataWithStride(DstPtr dst, SrcPtr src, const LoadData3DParamsV2<U>& loadDataParams)
{
    CubePtrOverload::Ld3DV2Stride<IS_RESER_LOAD3D_DEFAULT_CONFIG>(dst, src, loadDataParams);
}

template <
    typename T, const IsResetLoad3dConfig& defaultConfig, typename U, typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadDataWithStride(DstPtr dst, SrcPtr src, const LoadData3DParamsV2<U>& loadDataParams)
{
    CubePtrOverload::Ld3DV2Stride<defaultConfig>(dst, src, loadDataParams);
}
#endif

/* ---- LoadData with a bit-mode descriptor, raw pointer operands -------------- */
/*
 * Dst and Src stay explicit because neither is deducible from the arguments -- the same reason the
 * Tensor declaration carries them -- and the pointer operands follow them, so a call site written
 * as LoadData<TPosition::A2, TPosition::A1>(ptr, ptr, params) resolves.
 *
 * There is no `<..., T>` form here, unlike the Tensor overload where T is deduced from the
 * LocalTensor pair: with the address-space cast unavailable on this path (see the helpers above) no
 * re-view is possible, so an explicitly named T could not change the backend that is reached.  It
 * would be exactly equivalent to the deduced form and is therefore not provided.
 *
 * As in the overloads above, a definition repeats no default argument that the declaration in
 * kernel_operator_mm_intf.h already gives.
 */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

template <
    TPosition Dst, TPosition Src, typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const Load3DBitModeParam& loadDataParams)
{
    CubePtrOverload::LdBm3D<Dst>(dst, src, loadDataParams);
}

template <
    TPosition Dst, TPosition Src, typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const Load2DBitModeParam& loadDataParams)
{
    CubePtrOverload::LdBm2D<Dst>(dst, src, loadDataParams);
}

#endif

/* ---- LoadData 3dv2Pro with raw pointer operands ----------------------------- */
template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const LoadData3DParamsV2Pro& loadDataParams)
{
    CubePtrOverload::Ld3DV2Pro(dst, src, loadDataParams);
}

/* ---- LoadImageToLocal with a raw pointer destination ------------------------ */
template <typename DstPtr, typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type>
__aicore__ inline void LoadImageToLocal(DstPtr dst, const LoadImageToLocalParams& loadDataParams)
{
    CubePtrOverload::LdImage(dst, loadDataParams);
}

/* ---- InitConstValue with a raw pointer destination -------------------------- */
template <typename DstPtr, typename U, typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type>
__aicore__ inline void InitConstValue(DstPtr dst, const InitConstValueParams<U>& initConstValueParams)
{
    CubePtrOverload::InitConst(dst, initConstValueParams);
}

// The explicit element type mirrors the Tensor declaration's <T, U = PrimT<T>>: a call site written
// InitConstValue<half>(ptr, params) resolves here, exactly as it does against the Tensor overload.
template <
    typename T, typename U, typename DstPtr, typename Std::enable_if<IsSameType<PrimT<T>, U>::value, bool>::type,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type>
__aicore__ inline void InitConstValue(DstPtr dst, const InitConstValueParams<U>& initConstValueParams)
{
    CubePtrOverload::InitConst(dst, initConstValueParams);
}

template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadDataWithTranspose(DstPtr dst, SrcPtr src, const LoadData2dTransposeParams& loadDataParams)
{
    CubePtrOverload::Ld12L0Transpose(dst, src, loadDataParams);
}

template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void LoadDataWithTranspose(DstPtr dst, SrcPtr src, const LoadData2dTransposeParamsV2& loadDataParams)
{
    CubePtrOverload::Ld12L0Transpose(dst, src, loadDataParams);
}

#if __NPU_ARCH__ == 2201
template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr>::value, bool>::type>
__aicore__ inline void MmadWithSparse(DstPtr dst, Src0Ptr fm, Src1Ptr filter, const MmadParams& mmadParams)
{
    MmadSpCal(dst, fm, filter, mmadParams);
}
#endif

#if __NPU_ARCH__ == 2201
/* Raw-pointer form of LoadDataWithSparse -- definition.
 *
 * Isomorphic rewrite: statement for statement the same as LoadDataWithSparseCal in
 * impl/basic_api/dav_c220/kernel_operator_mm_impl.h:395-413, with each `X.GetPhyAddr()` replaced by
 * the operand itself.  Nothing else changes -- no address is recomputed, no parameter is
 * reinterpreted, and the two primitives called at the bottom (SetIndexMatrix, load_cbuf_to_cb_sp)
 * were already raw-pointer interfaces.  The Tensor body of LoadDataWithSparse is
 * LoadDataWithSparseImpl -> LoadDataWithSparseCal, so the two paths issue the same instruction with
 * the same operands.
 *
 * NOT reproduced: the debug-only checks that LoadDataWithSparseImpl performs on the caller's behalf
 * before dispatching -- CheckTensorPhyPosition on dst/src/idx and CheckTensorAlignment on all three
 * (impl/basic_api/kernel_operator_mm_base_impl.h:640-647).  Those read the tensor's TPosition and
 * its buffer base, neither of which a raw pointer carries, and they are compiled in only under
 * `#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)`, so they have no effect on the device
 * build.  A pointer caller gives up that diagnosis; it is the one place where the pointer path is
 * weaker than the Tensor path rather than merely different.
 */
/*
 * NOTE (measured, not assumed): this overload takes no element-type template parameter.  An earlier
 * revision carried `typename T = int8_t, typename U = uint8_t` so that the enable_ifs could pin the
 * operand types, and bisheng treats such a declaration-with-defaults as a DIFFERENT template from its
 * own definition: the call resolved to the declaration while the definition was never instantiated,
 * giving `-Wundefined-inline` and then `undefined symbol (.cube)` at link.  On dav-2201 the reading was
 * undefined-inline 2 -> 0 and undefined symbol 1 -> 0 when the two parameters were dropped.  The
 * sibling pointer overload LoadDataWithTranspose carries no element-type constraint either, so the
 * operand types are left to the caller here as well.
 */
template <
    typename DstPtr, typename SrcPtr, typename IdxPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, IdxPtr>::value, bool>::type>
__aicore__ inline void LoadDataWithSparse(DstPtr dst, SrcPtr src, IdxPtr idx, const LoadData2dParams& loadDataParam)
{
    if ASCEND_IS_AIC {
#ifdef ASCENDC_CPU_DEBUG
        SetIndexMatrix((void*)idx);
        __cbuf__ int8_t* srcAddr = (__cbuf__ int8_t*)src;
#else
        uint64_t src0tmp = reinterpret_cast<uint64_t>(src);
        uint64_t src1tmp = reinterpret_cast<uint64_t>(idx);
        // src[31:0], dense weight matrix addr.
        // src[63:32], index matrix addr.
        uint64_t srctmp = (src0tmp & 0xffffffff) | ((src1tmp & 0xffffffff) << 32);
        __cbuf__ int8_t* srcAddr = reinterpret_cast<__cbuf__ int8_t*>(srctmp);
#endif
        load_cbuf_to_cb_sp((__cb__ int8_t*)dst, srcAddr, loadDataParam.startIndex, loadDataParam.repeatTimes);
    }
}
#endif
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_MM_INTERFACE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_INTF_IMPL_H__
#endif
