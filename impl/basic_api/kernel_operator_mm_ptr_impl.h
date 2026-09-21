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
 * \file kernel_operator_mm_ptr_impl.h
 * \brief Pointer overloads for CUBE Basic API (LoadData / Mmad family).
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_mm_ptr_impl.h is an internal header file and must not be used directly.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_PTR_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_MM_PTR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_MM_PTR_IMPL_H

#include "../../include/basic_api/kernel_struct_mm.h"

namespace AscendC {

/* **************************************************************************************************
 * LoadData (pointer) — L1 -> L0A / L0B
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LoadData(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParams)
{
    LoadData2DL12L0ACal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadData(__cb__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParams)
{
    LoadData2DL12L0BCal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadData(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DL12L0ACal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadData(__cb__ T* dst, __cbuf__ T* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DL12L0BCal(dst, src, loadDataParams);
}

/* **************************************************************************************************
 * LoadData (pointer) — L1 -> L0 with LoadData3DParamsV2
 * ************************************************************************************************* */
template <
    typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = T,
    typename std::enable_if<IsSameType<T, U>::value, bool>::type = true>
__aicore__ inline void LoadData(__ca__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2<U>& loadDataParams)
{
    if constexpr (defaultConfig.isSetFMatrix) {
        Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    }
    if constexpr (defaultConfig.isSetPadding) {
        Load3DSetPaddingCal(loadDataParams.padValue);
    }
    LoadData3DV2L12L0ACal(dst, src, loadDataParams);
}

template <
    typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = T,
    typename std::enable_if<IsSameType<T, U>::value, bool>::type = true>
__aicore__ inline void LoadData(__cb__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2<U>& loadDataParams)
{
    if constexpr (defaultConfig.isSetFMatrix) {
        Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    }
    if constexpr (defaultConfig.isSetPadding) {
        Load3DSetPaddingCal(loadDataParams.padValue);
    }
    LoadData3DV2L12L0BCal(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || \
                              (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
template <typename T, typename U = T, typename V = fp8_e8m0_t>
__aicore__ inline void LoadData(
    __ca__ U* dst, __cbuf__ T* src, __cbuf__ V* srcMx, const LoadData2DParamsV2& loadDataParams,
    const LoadData2DMxParams& loadMxDataParams)
{
#if !(                       \
    defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)))
    LoadData2DL12L0ACal(
        (__ca__ PrimT<U>*)dst, (__cbuf__ PrimT<T>*)src, (__cbuf__ PrimT<V>*)srcMx, loadDataParams, loadMxDataParams);
#else
    (void)(dst);
    (void)(src);
    (void)(srcMx);
    (void)(loadDataParams);
    (void)(loadMxDataParams);
#endif
}

template <typename T, typename U = T, typename V = fp8_e8m0_t>
__aicore__ inline void LoadData(
    __cb__ U* dst, __cbuf__ T* src, __cbuf__ V* srcMx, const LoadData2DParamsV2& loadDataParams,
    const LoadData2DMxParams& loadMxDataParams)
{
    LoadData2DL12L0BCal(
        (__cb__ PrimT<U>*)dst, (__cbuf__ PrimT<T>*)src, (__cbuf__ PrimT<V>*)srcMx, loadDataParams, loadMxDataParams);
}
#endif

/* **************************************************************************************************
 * LoadDataWithTranspose (pointer)
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LoadDataWithTranspose(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParams)
{
    LoadData2DL12L0ATransposeCal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadDataWithTranspose(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParams)
{
    LoadData2DL12L0BTransposeCal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadDataWithTranspose(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParamsV2& loadDataParams)
{
    LoadData2DL12L0BTransposeCal(dst, src, loadDataParams);
}

/* **************************************************************************************************
 * LoadDataWithStride (pointer) — LoadData3DParamsV2 path
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
template <typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG>
__aicore__ inline void LoadDataWithStride(__ca__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2<T>& loadDataParams)
{
    if constexpr (defaultConfig.isSetFMatrix) {
        Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    }
    if constexpr (defaultConfig.isSetPadding) {
        Load3DSetPaddingCal(loadDataParams.padValue);
    }
    LoadData3DV2L12L0AWithStrideCal(dst, src, loadDataParams);
}

template <typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG>
__aicore__ inline void LoadDataWithStride(__cb__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2<T>& loadDataParams)
{
    if constexpr (defaultConfig.isSetFMatrix) {
        Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    }
    if constexpr (defaultConfig.isSetPadding) {
        Load3DSetPaddingCal(loadDataParams.padValue);
    }
    LoadData3DV2L12L0BWithStrideCal(dst, src, loadDataParams);
}
#endif

/* **************************************************************************************************
 * Mmad / MmadMx / MmadWithSparse (pointer)
 * ************************************************************************************************* */
template <typename T, typename U, typename S>
__aicore__ inline void Mmad(__cc__ T* dst, __ca__ U* fm, __cb__ S* filter, const MmadParams& mmadParams)
{
    MmadCal(dst, fm, filter, mmadParams);
}

template <typename T, typename U, typename S>
__aicore__ inline void Mmad(
    __cc__ T* dst, __ca__ U* fm, __cb__ S* filter, __cc__ T* bias, const MmadParams& mmadParams)
{
    // 与 Tensor 路径一致：由 mmadParams.cmatrixSource 区分
    // BiasTable(C2)=true / L0C(CO1)=false；不得写死 false（官方 mmad s3 bias 在 C2）。
    MmadCal(dst, fm, filter, (uint64_t)bias, mmadParams, mmadParams.cmatrixSource);
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
template <typename T, typename U, typename S>
__aicore__ inline void Mmad(__cc__ T* dst, __ca__ U* fm, __cb__ S* filter, const MmadBitModeParams& mmadParams)
{
    MmadCal(dst, fm, filter, mmadParams);
}

template <typename T, typename U, typename S>
__aicore__ inline void Mmad(
    __cc__ T* dst, __ca__ U* fm, __cb__ S* filter, __cc__ T* bias, const MmadBitModeParams& mmadParams)
{
    MmadCal(dst, fm, filter, (uint64_t)bias, mmadParams);
}

// Inline mad_mx (same as MmadMxCal). Do NOT call MmadMxImpl/MmadMxCal by name:
// ptr_impl is included after namespace AscendC closes; those Cal/Impl names are not
// visible under cann-9.1.0-beta.3 overlay include order. Intrinsic mad_mx is.
template <typename T, typename U, typename S>
__aicore__ inline void MmadMx(__cc__ T* dst, __ca__ U* fm, __cb__ S* filter, const MmadParams& mmadParams)
{
    if ASCEND_IS_AIC {
        using DstT = PrimT<T>;
        using Src0T = PrimT<U>;
        using Src1T = PrimT<S>;
        constexpr bool isMx = SupportType<
            Tuple<DstT, Src0T, Src1T>, Tuple<float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e1m2_t, fp4x2_e2m1_t>, Tuple<float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e2m1_t, fp4x2_e2m1_t>, Tuple<float, fp8_e4m3fn_t, fp8_e4m3fn_t>,
            Tuple<float, fp8_e4m3fn_t, fp8_e5m2_t>, Tuple<float, fp8_e5m2_t, fp8_e4m3fn_t>,
            Tuple<float, fp8_e5m2_t, fp8_e5m2_t>>();
        static_assert(isMx, "MmadMx not support data type on current device!");
        bool cmatrixInitVal = mmadParams.cmatrixInitVal && (!mmadParams.isBias);
        mad_mx(
            (__cc__ DstT*)dst, (__ca__ Src0T*)fm, (__cb__ Src1T*)filter, mmadParams.m, mmadParams.k, mmadParams.n,
            mmadParams.unitFlag, mmadParams.disableGemv, mmadParams.cmatrixSource, cmatrixInitVal);
    }
}

template <typename T, typename U, typename S>
__aicore__ inline void MmadMx(
    __cc__ T* dst, __ca__ U* fm, __cb__ S* filter, __cc__ T* bias, const MmadParams& mmadParams)
{
    if ASCEND_IS_AIC {
        using DstT = PrimT<T>;
        using Src0T = PrimT<U>;
        using Src1T = PrimT<S>;
        constexpr bool isMx = SupportType<
            Tuple<DstT, Src0T, Src1T>, Tuple<float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e1m2_t, fp4x2_e2m1_t>, Tuple<float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e2m1_t, fp4x2_e2m1_t>, Tuple<float, fp8_e4m3fn_t, fp8_e4m3fn_t>,
            Tuple<float, fp8_e4m3fn_t, fp8_e5m2_t>, Tuple<float, fp8_e5m2_t, fp8_e4m3fn_t>,
            Tuple<float, fp8_e5m2_t, fp8_e5m2_t>>();
        static_assert(isMx, "MmadMx not support data type on current device!");
        // Explicit bias ptr: treat as BiasTable (C2), matching samples / cmatrixSource=true.
        bool cmatrixSource = true;
        bool cmatrixInitVal = mmadParams.cmatrixInitVal && (!mmadParams.isBias);
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        mad_mx(
            (__cc__ DstT*)dst, (__ca__ Src0T*)fm, (__cb__ Src1T*)filter, (uint64_t)bias, mmadParams.m, mmadParams.k,
            mmadParams.n, mmadParams.unitFlag, mmadParams.disableGemv, cmatrixSource, cmatrixInitVal);
#else
        uint64_t xd = ((uint64_t)dst) & 0xffffffffULL | ((((uint64_t)bias) & 0xffffffffULL) << 32);
        mad_mx(
            (__cc__ DstT*)xd, (__ca__ Src0T*)fm, (__cb__ Src1T*)filter, mmadParams.m, mmadParams.k, mmadParams.n,
            mmadParams.unitFlag, mmadParams.disableGemv, cmatrixSource, cmatrixInitVal);
#endif
    }
}

template <typename T, typename U, typename S>
__aicore__ inline void MmadMx(__cc__ T* dst, __ca__ U* fm, __cb__ S* filter, const MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        using DstT = PrimT<T>;
        using Src0T = PrimT<U>;
        using Src1T = PrimT<S>;
        constexpr bool isMx = SupportType<
            Tuple<DstT, Src0T, Src1T>, Tuple<float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e1m2_t, fp4x2_e2m1_t>, Tuple<float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e2m1_t, fp4x2_e2m1_t>, Tuple<float, fp8_e4m3fn_t, fp8_e4m3fn_t>,
            Tuple<float, fp8_e4m3fn_t, fp8_e5m2_t>, Tuple<float, fp8_e5m2_t, fp8_e4m3fn_t>,
            Tuple<float, fp8_e5m2_t, fp8_e5m2_t>>();
        static_assert(isMx, "MmadMx not support data type on current device!");
        MmadBitModeParams& params = const_cast<MmadBitModeParams&>(mmadParams);
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        mad_mx(
            (__cc__ DstT*)dst, (__ca__ Src0T*)fm, (__cb__ Src1T*)filter, params.GetM(), params.GetK(), params.GetN(),
            params.GetUnitFlag(), params.GetDisableGemv(), params.GetCmatrixSource(), params.GetCmatrixInitVal());
#else
        mad_mx((__cc__ DstT*)dst, (__ca__ Src0T*)fm, (__cb__ Src1T*)filter, params.GetConfig0());
#endif
    }
}

template <typename T, typename U, typename S>
__aicore__ inline void MmadMx(
    __cc__ T* dst, __ca__ U* fm, __cb__ S* filter, __cc__ T* bias, const MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        using DstT = PrimT<T>;
        using Src0T = PrimT<U>;
        using Src1T = PrimT<S>;
        constexpr bool isMx = SupportType<
            Tuple<DstT, Src0T, Src1T>, Tuple<float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e1m2_t, fp4x2_e2m1_t>, Tuple<float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e2m1_t, fp4x2_e2m1_t>, Tuple<float, fp8_e4m3fn_t, fp8_e4m3fn_t>,
            Tuple<float, fp8_e4m3fn_t, fp8_e5m2_t>, Tuple<float, fp8_e5m2_t, fp8_e4m3fn_t>,
            Tuple<float, fp8_e5m2_t, fp8_e5m2_t>>();
        static_assert(isMx, "MmadMx not support data type on current device!");
        MmadBitModeParams& params = const_cast<MmadBitModeParams&>(mmadParams);
        params.SetCmatrixInitVal(false);
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        mad_mx(
            (__cc__ DstT*)dst, (__ca__ Src0T*)fm, (__cb__ Src1T*)filter, (uint64_t)bias, params.GetM(), params.GetK(),
            params.GetN(), params.GetUnitFlag(), params.GetDisableGemv(), params.GetCmatrixSource(),
            params.GetCmatrixInitVal());
#else
        uint64_t xd = ((uint64_t)dst) & 0xffffffffULL | ((((uint64_t)bias) & 0xffffffffULL) << 32);
        mad_mx((__cc__ DstT*)xd, (__ca__ Src0T*)fm, (__cb__ Src1T*)filter, params.GetConfig0());
#endif
    }
}
#endif

#if __NPU_ARCH__ == 2201
__aicore__ inline void MmadWithSparse(
    __cc__ int32_t* dst, __ca__ int8_t* fm, __cb__ int8_t* filter, const MmadParams& mmadParams)
{
    MmadSpCal(dst, fm, filter, mmadParams);
}

__aicore__ inline void LoadDataWithSparse(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ uint8_t* idx, const LoadData2dParams& loadDataParam)
{
    if ASCEND_IS_AIC {
#ifdef ASCENDC_CPU_DEBUG
        SetIndexMatrix((void*)idx);
        __cbuf__ int8_t* srcAddr = src;
#else
        uint64_t src0tmp = reinterpret_cast<uint64_t>(src);
        uint64_t src1tmp = reinterpret_cast<uint64_t>(idx);
        uint64_t srctmp = (src0tmp & 0xffffffff) | ((src1tmp & 0xffffffff) << 32);
        __cbuf__ int8_t* srcAddr = reinterpret_cast<__cbuf__ int8_t*>(srctmp);
#endif
        load_cbuf_to_cb_sp(dst, srcAddr, loadDataParam.startIndex, loadDataParam.repeatTimes);
    }
}
#endif

/* **************************************************************************************************
 * LoadData (pointer) — GM -> L0A / L0B / L1
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    __ca__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParams)
{
    LoadData2DGM2L0ACal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    __cb__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParams)
{
    LoadData2DGM2L0BCal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    __cbuf__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParams)
{
    LoadData2DGM2L1Cal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    __cbuf__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParams, uint8_t cacheMode)
{
    LoadData2DGM2L1Cal(dst, src, loadDataParams, cacheMode);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    __ca__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DGM2L0ACal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    __cb__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DGM2L0BCal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    __cbuf__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DGM2L1Cal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    __cbuf__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParams, uint8_t cacheMode)
{
    LoadData2DGM2L1Cal(dst, src, loadDataParams, cacheMode);
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5102)
template <typename T, typename U>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(
    __cbuf__ T* dst, __gm__ U* src, const LoadData2DParamsV2& loadDataParams, const Nd2NzParamsV2& nd2nzParams)
{
    LoadData2DGM2L1Cal(dst, src, loadDataParams, nd2nzParams);
}
#endif

/* **************************************************************************************************
 * LoadData (pointer) — LoadData3DParamsV1 / V2Pro / BitMode
 * ************************************************************************************************* */
template <
    typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = T,
    typename std::enable_if<IsSameType<T, U>::value, bool>::type = true>
__aicore__ inline void LoadData(__ca__ T* dst, __cbuf__ T* src, const LoadData3DParamsV1<U>& loadDataParams)
{
    if constexpr (defaultConfig.isSetFMatrix) {
        Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    }
    if constexpr (defaultConfig.isSetPadding) {
        Load3DSetPaddingCal(loadDataParams.padValue);
    }
    LoadData3DV1L12L0ACal(dst, src, loadDataParams);
}

template <
    typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = T,
    typename std::enable_if<IsSameType<T, U>::value, bool>::type = true>
__aicore__ inline void LoadData(__cb__ T* dst, __cbuf__ T* src, const LoadData3DParamsV1<U>& loadDataParams)
{
    if constexpr (defaultConfig.isSetFMatrix) {
        Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    }
    if constexpr (defaultConfig.isSetPadding) {
        Load3DSetPaddingCal(loadDataParams.padValue);
    }
    LoadData3DV1L12L0BCal(dst, src, loadDataParams);
}

template <
    typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = T,
    typename std::enable_if<IsSameType<T, U>::value, bool>::type = true>
__aicore__ inline void LoadData(__ubuf__ T* dst, __cbuf__ T* src, const LoadData3DParamsV1<U>& loadDataParams)
{
    if constexpr (defaultConfig.isSetFMatrix) {
        Load3DSetFMatrixCal(loadDataParams.l1H, loadDataParams.l1W, loadDataParams.padList);
    }
    if constexpr (defaultConfig.isSetPadding) {
        Load3DSetPaddingCal(loadDataParams.padValue);
    }
    LoadData3DV1L12UBCal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadData(__ca__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadData3DV2L12L0ACal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadData(__cb__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadData3DV2L12L0BCal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadData(__ubuf__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadData3DV2L12UBCal(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
template <typename T>
__aicore__ inline void LoadData(__ca__ T* dst, __cbuf__ T* src, const Load2DBitModeParam& loadDataParams)
{
    LoadData2DL12L0ACal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadData(__cb__ T* dst, __cbuf__ T* src, const Load2DBitModeParam& loadDataParams)
{
    LoadData2DL12L0BCal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadData(__ca__ T* dst, __cbuf__ T* src, const Load3DBitModeParam& loadDataParams)
{
    LoadData3DV2L12L0ACal(dst, src, loadDataParams);
}

template <typename T>
__aicore__ inline void LoadData(__cb__ T* dst, __cbuf__ T* src, const Load3DBitModeParam& loadDataParams)
{
    LoadData3DV2L12L0BCal(dst, src, loadDataParams);
}
#endif

/* **************************************************************************************************
 * InitConstValue / LoadImageToLocal (pointer)
 * ************************************************************************************************* */
template <typename T, typename U = T, typename std::enable_if<IsSameType<T, U>::value, bool>::type = true>
__aicore__ inline void InitConstValue(__ca__ T* dst, const InitConstValueParams<U>& initConstValueParams)
{
    InitL0ANzMatrixCal(dst, initConstValueParams);
}

template <typename T, typename U = T, typename std::enable_if<IsSameType<T, U>::value, bool>::type = true>
__aicore__ inline void InitConstValue(__cb__ T* dst, const InitConstValueParams<U>& initConstValueParams)
{
    InitL0BNzMatrixCal(dst, initConstValueParams);
}

template <typename T, typename U = T, typename std::enable_if<IsSameType<T, U>::value, bool>::type = true>
__aicore__ inline void InitConstValue(__cbuf__ T* dst, const InitConstValueParams<U>& initConstValueParams)
{
    InitL1BufferCal(dst, initConstValueParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void LoadImageToLocal(
    __cbuf__ T* dst, const LoadImageToLocalParams& loadDataParams)
{
    LoadImageToLocalCal(dst, loadDataParams);
}

/* **************************************************************************************************
 * LoadDataUnzip (pointer) — GM -> L1 / L0A / L0B
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LoadDataUnzip(__cbuf__ T* dst, __gm__ T* src)
{
    LoadDataUnzipToL1Cal(dst, src);
}

template <typename T>
__aicore__ inline void LoadDataUnzip(__ca__ T* dst, __gm__ T* src)
{
    LoadDataUnzipToL0ACal(dst, src);
}

template <typename T>
__aicore__ inline void LoadDataUnzip(__cb__ T* dst, __gm__ T* src)
{
    LoadDataUnzipToL0BCal(dst, src);
}

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_MM_PTR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_PTR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_PTR_IMPL_H__
#endif
