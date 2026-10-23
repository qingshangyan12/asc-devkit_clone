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
 * \file kernel_operator_mm_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_mm_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_INTF_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_MM_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_MM_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "../../impl/basic_api/common_types.h"
#include "../../impl/basic_api/kernel_operator_mm_base_impl.h"
#include "kernel_struct_mm.h"
#include "kernel_tensor.h"
#include "../../impl/basic_api/utils/kernel_utils_constants.h"
#include "../../impl/basic_api/utils/kernel_utils_macros.h"

#include "../../impl/basic_api/utils/kernel_utils_ptr_overload.h"

#include "kernel_operator_mm_bitmode_intf.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {

enum class HF32Mode { ENABLE, DISABLE };

enum class HF32TransMode {
    NEAREST_AWAY = 0,
    NEAREST_EVEN = 1,
    NEAREST_ZERO = NEAREST_AWAY // Compatible with the old name; the actual behavior is equivalent to NEAREST_AWAY.
};

/* **************************************************************************************************
 * LoadData 2d                                             *
 * ************************************************************************************************* */
/*
 * @ingroup DataLoad
 * @brief Cube data loading
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] loadDataParams.startIndex Fractal matrix ID
 * @param [in] loadDataParams.repeatTimes repeat times
 * @param [in] loadDataParams.srcStride src block stride
 * @param [in] loadDataParams.sid SMMU SID
 * @param [in] loadDataParams.dstGap interval between the previous tail and the next fractal head
 * @param [in] loadDataParams.ifTranspose enable parameters of transpose function
 */
template <typename T>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2DParams& loadDataParams);

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DParams& loadDataParams);

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
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2DParamsV2& loadDataParams);

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DParamsV2& loadDataParams);

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DParamsV2& loadDataParams,
    const bool enablePreReadIgnoreSync);

#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
     (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163) || (__NPU_ARCH__ == 5162))
template <typename T, typename U = T, typename V = fp8_e8m0_t>
__aicore__ inline void LoadData(
    const LocalTensor<U>& dst, const LocalTensor<T>& src, const LocalTensor<V>& srcMx,
    const LoadData2DParamsV2& loadDataParams, const LoadData2DMxParams& loadMxDataParams);
#endif

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162))
template <typename T, typename U>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<U>& src, const LoadData2DParamsV2& loadDataParams,
    const Nd2NzParamsV2& nd2nzParams);
#endif

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
    typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = PrimT<T>,
    typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type = true>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV1<U>& loadDataParams);

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
    typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = PrimT<T>,
    typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type = true>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV2<U>& loadDataParams);

template <
    typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = PrimT<T>,
    typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type = true>
__aicore__ inline void LoadDataWithStride(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV2<U>& loadDataParams);

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || \
                              (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
template <TPosition DstPos, TPosition SrcPos, typename T>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const Load3DBitModeParam& loadDataParams);
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
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV2Pro& loadDataParams);

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || \
                              (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
template <TPosition DstPos, TPosition SrcPos, typename T>
__aicore__ inline void LoadData(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const Load2DBitModeParam& loadDataParams);
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
 * @param [in] loadDataParams.srcStride source stride between consequent repeat times in unit of frac num
 * @param [in] loadDataParams.dstGap destination gap between consequent repeat times in unit of 512byte
 * @param [in] loadDataParams.dstFracGap dst fractal gap in unit of one 512byte fractal
 */
template <typename T>
__aicore__ inline void LoadDataWithTranspose(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2dTransposeParams& loadDataParams);

/*
 * @ingroup DataLoad
 * @brief Cube data loading
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] loadDataParams.startIndex index of the first fractal in the first repeat in the source matrix
 * in unit of 512byte fractal
 * @param [in] loadDataParams.repeatTimes the repeat times
 * @param [in] loadDataParams.srcStride source stride between consequent repeat times in unit of 512byte
 * @param [in] loadDataParams.dstGap destination gap between consequent repeat times in unit of 512byte
 * @param [in] loadDataParams.dstFracGap dst fractal gap in unit of one 512byte fractal
 * @param [in] loadDataParams.srcFracGap dst fractal gap in unit of one 512byte fractal
 */
template <typename T>
__aicore__ inline void LoadDataWithTranspose(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2dTransposeParamsV2& loadDataParams);

/* **************************************************************************************************
 * LoadDataWithDecomp                                             *
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
__aicore__ inline void EnableS4ToS8();

__aicore__ inline void EnableS3ToS8();

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DDecompParamsV2& loadDataDecompParams);
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
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const MmadParams& mmadParams);

template <typename T, typename U, typename S, typename V>
__aicore__ inline void Mmad(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const LocalTensor<V>& bias,
    const MmadParams& mmadParams);

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
template <typename T, typename U, typename S>
__aicore__ inline void Mmad(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter,
    const MmadBitModeParams& mmadParams);

template <typename T, typename U, typename S, typename V>
__aicore__ inline void Mmad(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const LocalTensor<V>& bias,
    const MmadBitModeParams& mmadParams);
#endif

/* **************************************************************************************************
 * MmadMx                                                                                           *
 * **************************************************************************************************/
/*
 * @ingroup MmadMx
 * @brief Matrix multiplication and addition in microscaling scenario
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
__aicore__ inline void MmadMx(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const MmadParams& mmadParams);

template <typename T, typename U, typename S, typename V>
__aicore__ inline void MmadMx(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const LocalTensor<V>& bias,
    const MmadParams& mmadParams);

template <typename T, typename U, typename S>
__aicore__ inline void MmadMx(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter,
    const MmadBitModeParams& mmadParams);

template <typename T, typename U, typename S, typename V>
__aicore__ inline void MmadMx(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const LocalTensor<V>& bias,
    const MmadBitModeParams& mmadParams);

template <
    typename T = int32_t, typename U = int8_t,
    typename Std::enable_if<Std::is_same<PrimT<T>, int32_t>::value, bool>::type = true,
    typename Std::enable_if<Std::is_same<PrimT<U>, int8_t>::value, bool>::type = true>
__aicore__ inline void MmadWithSparse(
    const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<U>& filter, const MmadParams& mmadParams);

template <
    typename T = int8_t, typename U = uint8_t,
    typename Std::enable_if<Std::is_same<PrimT<T>, int8_t>::value, bool>::type = true,
    typename Std::enable_if<Std::is_same<PrimT<U>, uint8_t>::value, bool>::type = true>
__aicore__ inline void LoadDataWithSparse(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<U>& idx,
    const LoadData2dParams& loadDataParam);

#if __NPU_ARCH__ == 2201
/* Raw-pointer form of LoadDataWithSparse.
 *
 * CLASS OF CHANGE: isomorphic rewrite, not a forwarding layer and not a lower-level
 * reconstruction.  LoadDataWithSparseCal uses its three tensor operands *only* through
 * GetPhyAddr() -- the two primitives underneath it, SetIndexMatrix and load_cbuf_to_cb_sp, are
 * already raw-pointer interfaces -- so the pointer form is the same statements with the address
 * carrier supplied directly instead of extracted.  See the definition in
 * impl/basic_api/kernel_operator_mm_intf_impl.h.
 *
 * ARCH REACHABILITY -- deliberate, and narrower than the Tensor declaration above.  The Tensor
 * declaration is not arch-guarded, but its definition and everything behind it exists only for
 * __NPU_ARCH__ == 2201:
 *   impl/basic_api/kernel_operator_mm_base_impl.h:610   `#if __NPU_ARCH__ == 2201` around
 *                                                       LoadDataWithSparseImpl
 *   impl/basic_api/kernel_operator_mm_intf_impl.h:435   `#if __NPU_ARCH__ == 2201` around the
 *                                                       Tensor definition
 *   impl/basic_api/dav_c220/kernel_operator_mm_impl.h:395  the only LoadDataWithSparseCal that
 *                                                       computes anything; dav_c100 :214 and
 *                                                       dav_m200 :419 ASCENDC_REPORT_NOT_SUPPORT,
 *                                                       dav_m300 :353 and dav_m310 :335 assert false.
 * dav_3510 has no LoadDataWithSparseCal at all.  A pointer overload declared for every
 * architecture would therefore be a declaration with no definition on 3510 -- it would satisfy a
 * compile-only probe and then fail to link, which would read as "implemented" when it is not.
 * Guarding it to the one architecture that has a backend keeps that visible as "no matching
 * function" instead.  The pre-existing gap is on the upstream side: the Tensor declaration itself
 * is not arch-guarded, so on 3510 the Tensor form is also declared-but-undefined.  This overload
 * does not reproduce that inconsistency.
 */
template <
    typename DstPtr, typename SrcPtr, typename IdxPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, IdxPtr>::value, bool>::type = true>
__aicore__ inline void LoadDataWithSparse(DstPtr dst, SrcPtr src, IdxPtr idx, const LoadData2dParams& loadDataParam);
#endif

template <typename T = int8_t, typename Std::enable_if<Std::is_same<PrimT<T>, int8_t>::value, bool>::type = true>
__aicore__ inline void LoadUnzipIndex(const GlobalTensor<T>& src, uint32_t numOfIndexTabEntry);

/* **************************************************************************************************
 * BroadCastVecToMM                                             *
 * ************************************************************************************************* */
template <typename T, typename U>
__aicore__ inline __inout_pipe__(V) void BroadCastVecToMM(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const int32_t blockCount, const uint8_t blockLen,
    const uint8_t srcGap, const uint8_t dstGap);

/* **************************************************************************************************
 * Fill                                             *
 * ************************************************************************************************* */
/*
 * @ingroup Fill
 * @brief L0A/L0B/L1 value initializing
 * @param [out] dst output LocalTensor
 * @param [in] InitConstValueParams.repeatTimes repeat times
 * @param [in] InitConstValueParams.repeatTimes blockNum block number
 * @param [in] InitConstValueParams.dstGap interval between the previous tail and the next block head
 * @param [in] InitConstValueParams.initValue initialize Value
 */
template <
    typename T, typename U = PrimT<T>, typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type = true>
__aicore__ inline void Fill(const LocalTensor<T>& dst, const InitConstValueParams<U>& initConstValueParams);

// InitConstValue has been updated, please use Fill instead.
template <
    typename T, typename U = PrimT<T>, typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type = true>
__aicore__ inline void InitConstValue(const LocalTensor<T>& dst, const InitConstValueParams<U>& initConstValueParams);

/* **************************************************************************************************
 * SetLoadDataPaddingValue                                             *
 * ************************************************************************************************* */
/*
 * @ingroup SetLoadDataPaddingValue
 * @brief setting loadData pad value
 * @param [in]padValue padding value
 */
template <typename T>
__aicore__ inline void SetLoadDataPaddingValue(const T padValue);

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
__aicore__ inline void SetFmatrix(uint16_t l1H, uint16_t l1W, const uint8_t padList[4], const FmatrixMode& fmatrixMode);

#if defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
__aicore__ inline void SetFmatrix(const SetFMatrixBitModeParams& param, const FmatrixMode& fmatrixMode);
#endif
/* **************************************************************************************************
 * SetLoadDataBoundary                                             *
 * ************************************************************************************************* */
/*
 * @ingroup SetFmatrix
 * @brief setting loaddata boundary
 * @param [in]boundaryValue
 */
__aicore__ inline void SetLoadDataBoundary(uint32_t boundaryValue);

__aicore__ inline void SetLoadDataRepeat(const LoadDataRepeatParam& repeatParams);

__aicore__ inline void SetLoadDataRepeatWithStride(const LoadDataRepeatParamWithStride& repeatParams);

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
__aicore__ inline void LoadImageToLocal(const LocalTensor<T>& dst, const LoadImageToLocalParams& loadDataParams);

/* **************************************************************************************************
 * StoreImageToGlobal                                             *
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
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const StoreImageToGlobalParams& params);
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
__aicore__ inline void LoadDataUnzip(const LocalTensor<T>& dst, const GlobalTensor<T>& src);

/*
 * @brief Sets whether to enable HF32 mode for Mmad computation
 * @param [in] mode HF32 mode enumeration
 * @note When mode is HF32Mode::ENABLE, FP32 data in L0A/L0B will be rounded to HF32 before matrix multiplication;
 * when mode is HF32Mode::DISABLE, regular FP32 matrix multiplication will be executed
 */
__aicore__ inline void SetHF32Mode(HF32Mode mode);

/*
 * @brief Sets the rounding method for HF32 rounding mode
 * @param [in] hf32TransMode Control parameter for Mmad HF32 mode
 * @note Must Call SetHF32Mode to enable HF32 rounding mode first.When hf32TransMode is true, FP32 is rounded to HF32
 * with rounding towards zero; when false, rounded to nearest even
 */
// SetHF32Mode(bool hf32Mode) has been updated, please use SetHF32Mode(HF32Mode mode) instead.
__aicore__ inline void SetHF32Mode(bool hf32Mode);

/*
 * @brief Sets the rounding method for HF32 rounding mode
 * @param [in] mode HF32 trans mode enumeration
 * @note Must call SetHF32Mode to enable HF32 rounding mode first.
 */
__aicore__ inline void SetHF32TransMode(HF32TransMode mode);

/*
 * @brief Sets the rounding method for HF32 rounding mode
 * @param [in] hf32TransMode Control parameter for Mmad HF32 mode
 * @note Must Call SetHF32Mode to enable HF32 rounding mode first.When hf32TransMode is true, FP32 is rounded to HF32
 * with rounding to nearest, with ties away from zero; when false, rounded to nearest, with ties to even
 */
// SetHF32TransMode(bool hf32TransMode) has been updated, please use SetHF32TransMode(HF32TransMode mode) instead.
__aicore__ inline void SetHF32TransMode(bool hf32TransMode);

/*
 * @ingroup MMLayout
 * @brief Sets matrix multiplication result layout to row major
 * @note This function sets the CUBE output to row major format (M direction first, then N direction)
 */
__aicore__ inline void SetMMRowMajor();

/*
 * @ingroup MMLayout
 * @brief Sets matrix multiplication result layout to column major
 * @note This function sets the CUBE output to column major format (N direction first, then M direction)
 */
__aicore__ inline void SetMMColumnMajor();

/*
 * @brief Sets the priority direction (M or N) for Mmad/MmadWithSparse computation
 * @param [in] mmLayoutMode Control parameter for Mmad/MmadWithSparse priority direction
 * @note When mmLayoutMode is true, CUBE generates results first through N direction then M direction; when false, first
 * through M direction then N direction
 */
// SetMMLayoutTransform has been updated, please use SetMMRowMajor/SetMMColumnMajor instead.
__aicore__ inline void SetMMLayoutTransform(bool mmLayoutMode);

/* **************************************************************************************************
 * Pointer-input overloads for the CUBE basic API
 *
 * A raw hardware pointer carries its memory role in its own type -- __ca__ (L0A), __cb__ (L0B),
 * __cc__ (L0C), __cbuf__ (L1), __ubuf__ (UB), __gm__ (GM) -- so overload resolution selects exactly
 * the backend implementation that the equivalent LocalTensor / GlobalTensor call reaches.
 * The overloads below are a pure forwarding layer: they add no arithmetic, reinterpret no parameter
 * and silence no check that the Tensor path performs on the caller's behalf.  A pointer call is
 * therefore numerically identical to the equivalent Tensor call.  One place needs care and is stated
 * in full at the definition rather than glossed: a raw pointer carries no TPosition, so the bias
 * forms of Mmad / MmadMx cannot recover the CO1 / C2 distinction the Tensor form reads off the
 * operand.  See the comment on those definitions.
 *
 * Every overload is SFINAE-constrained on "at least one operand is a raw pointer", so no existing
 * Tensor call site can become ambiguous.
 * ************************************************************************************************* */

/* ---- Mmad / MmadMx with raw pointer operands -------------------------------- */
template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr>::value, bool>::type = true>
__aicore__ inline void Mmad(DstPtr dst, Src0Ptr fm, Src1Ptr filter, const MmadParams& mmadParams);

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr, typename BiasPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr, BiasPtr>::value, bool>::type = true>
__aicore__ inline void Mmad(DstPtr dst, Src0Ptr fm, Src1Ptr filter, BiasPtr bias, const MmadParams& mmadParams);
/*
 * BitMode forms of Mmad with raw pointer operands.
 *
 * The bias form keeps the non-const `MmadBitModeParams&` of the Tensor declaration above, because
 * the backend it forwards to -- MmadCal(dav_3510/kernel_operator_mm_impl.h:556) -- calls
 * mmadParams.SetCmatrixInitVal(false) on it.  A const reference could not carry that write.
 */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr>::value, bool>::type = true>
__aicore__ inline void Mmad(DstPtr dst, Src0Ptr fm, Src1Ptr filter, const MmadBitModeParams& mmadParams);

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr, typename BiasPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr, BiasPtr>::value, bool>::type = true>
__aicore__ inline void Mmad(DstPtr dst, Src0Ptr fm, Src1Ptr filter, BiasPtr bias, MmadBitModeParams& mmadParams);

#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr>::value, bool>::type = true>
__aicore__ inline void MmadMx(DstPtr dst, Src0Ptr fm, Src1Ptr filter, const MmadParams& mmadParams);

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr, typename BiasPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr, BiasPtr>::value, bool>::type = true>
__aicore__ inline void MmadMx(DstPtr dst, Src0Ptr fm, Src1Ptr filter, BiasPtr bias, const MmadParams& mmadParams);

// BitMode forms: same shape as the two declarations above, with the parameter struct replaced by
// MmadBitModeParams.  The interface list this task is measured against counts MmadBitMode as a
// group of four overloads in its own right.
// Note the back end (MmadMxCal, dav_3510/kernel_operator_mm_impl.h:654 and :675) takes a
// non-const MmadBitModeParams& -- these declarations match it.
template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr>::value, bool>::type = true>
__aicore__ inline void MmadMx(DstPtr dst, Src0Ptr fm, Src1Ptr filter, MmadBitModeParams& mmadParams);

template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr, typename BiasPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr, BiasPtr>::value, bool>::type = true>
__aicore__ inline void MmadMx(DstPtr dst, Src0Ptr fm, Src1Ptr filter, BiasPtr bias, MmadBitModeParams& mmadParams);
#endif

#if __NPU_ARCH__ == 2201
template <
    typename DstPtr, typename Src0Ptr, typename Src1Ptr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, Src0Ptr, Src1Ptr>::value, bool>::type = true>
__aicore__ inline void MmadWithSparse(DstPtr dst, Src0Ptr fm, Src1Ptr filter, const MmadParams& mmadParams);
#endif

/* ---- LoadData family with raw pointer operands ------------------------------ */
template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const LoadData2DParams& loadDataParams);

template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const LoadData2DParamsV2& loadDataParams);

template <
    typename DstPtr, typename SrcPtr, typename U,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const LoadData3DParamsV2<U>& loadDataParams);

/* LoadData 3dv1.  The element type comes from the pointer operands and U from the descriptor, so
 * the pointer form carries no explicit element type -- the same shape as the 3dv2 overload above. */
template <
    typename DstPtr, typename SrcPtr, typename U,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const LoadData3DParamsV1<U>& loadDataParams);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
template <
    typename DstPtr, typename SrcPtr, typename SrcMxPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, SrcMxPtr>::value, bool>::type = true>
__aicore__ inline void LoadData(
    DstPtr dst, SrcPtr src, SrcMxPtr srcMx, const LoadData2DParamsV2& loadDataParams,
    const LoadData2DMxParams& loadMxDataParams);
#endif

// No explicit template arguments: the element type comes from the pointer operands.
template <
    typename DstPtr, typename SrcPtr, typename U,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadDataWithStride(DstPtr dst, SrcPtr src, const LoadData3DParamsV2<U>& loadDataParams);

// Explicit <element type, IsResetLoad3dConfig>: mirrors the Tensor overload's parameter list, so a
// call site written as LoadDataWithStride<SrcT, LOAD3D_CONFIG>(ptr, ptr, params) still resolves.
// `config` selects whether the two global load-3d settings are reprogrammed on the pointer path.
template <
    typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = PrimT<T>,
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadDataWithStride(DstPtr dst, SrcPtr src, const LoadData3DParamsV2<U>& loadDataParams);

template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadDataWithTranspose(DstPtr dst, SrcPtr src, const LoadData2dTransposeParams& loadDataParams);

/* ---- LoadData with a bit-mode descriptor, raw pointer operands -------------- */
// Dst / Src stay explicit and the pointer operands follow them, so a call site written as
// LoadData<TPosition::A2, TPosition::A1>(ptr, ptr, params) resolves.  No <..., T> form is provided:
// with the address-space cast unavailable on this path no re-view is possible, so an explicit T
// could not reach a different backend than the deduced form.
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
template <
    TPosition Dst, TPosition Src, typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const Load3DBitModeParam& loadDataParams);

template <
    TPosition Dst, TPosition Src, typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const Load2DBitModeParam& loadDataParams);
#endif

/* ---- LoadData 3dv2Pro with raw pointer operands ----------------------------- */
template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadData(DstPtr dst, SrcPtr src, const LoadData3DParamsV2Pro& loadDataParams);

/* ---- LoadImageToLocal with a raw pointer destination ------------------------ */
template <typename DstPtr, typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type = true>
__aicore__ inline void LoadImageToLocal(DstPtr dst, const LoadImageToLocalParams& loadDataParams);

/* ---- InitConstValue with a raw pointer destination -------------------------- */
template <
    typename DstPtr, typename U, typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type = true>
__aicore__ inline void InitConstValue(DstPtr dst, const InitConstValueParams<U>& initConstValueParams);

template <
    typename T, typename U = PrimT<T>, typename DstPtr,
    typename Std::enable_if<IsSameType<PrimT<T>, U>::value, bool>::type = true,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type = true>
__aicore__ inline void InitConstValue(DstPtr dst, const InitConstValueParams<U>& initConstValueParams);

template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadDataWithTranspose(DstPtr dst, SrcPtr src, const LoadData2dTransposeParamsV2& loadDataParams);

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002) || (__NPU_ARCH__ == 2201) || \
                              (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3102))
/* ---- LoadDataUnzip with raw pointer operands -------------------------------- */
// Selected by the destination's address space (L1 / L0A / L0B); the source is a GM pointer.
template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadDataUnzip(DstPtr dst, SrcPtr src);
#endif

} // namespace AscendC
#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_mm_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_MM_INTERFACE_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_INTF_H__
#endif
