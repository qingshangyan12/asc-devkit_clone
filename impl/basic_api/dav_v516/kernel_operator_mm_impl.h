/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_v516/kernel_operator_mm_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_MM_IMPL_H
#define ASCENDC_MODULE_OPERATOR_MM_IMPL_H

#include "kernel_utils.h"
#include "kernel_struct_aipp.h"
#include "kernel_struct_waipp.h"
#include "kernel_operator_mm_bitmode_intf.h"
#include "kernel_operator_sys_var_intf.h"


namespace AscendC {
/* **************************************************************************************************
 * LoadData 2dv1                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_2d from L1 to A2 on current device"); });
}
template <typename T>
__aicore__ inline void LoadData2DL12L0BCal(__cb__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_2d from L1 to B2 on current device"); });
}
/* **************************************************************************************************
 * LoadData 2dv2                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const Load2DBitModeParam& loadDataParam)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_2d from L1 to A2 on current device"); });
}

template <typename T>
__aicore__ inline void LoadData2DL12L0BCal(__cb__ T* dst, __cbuf__ T* src, const Load2DBitModeParam& loadDataParam)
{
    static_assert(
        SupportType<
            T, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t, uint8_t, int8_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t, half,
            bfloat16_t, uint16_t, int16_t>(),
        "LoadData 2dv2 B1 to B2 only support int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t, uint8_t, int8_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t, half, "
        "bfloat16_t, uint16_t, int16_t on current device!");
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    if constexpr (SupportType<T, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t>()) {
        if (loadDataParam.GetIfTranspose()) {
            load_cbuf_to_cb_s4(
                (__cb__ void*)dst, (__cbuf__ void*)src, loadDataParam.GetMStartPosition(),
                loadDataParam.GetKStartPosition(), loadDataParam.GetMStep(), loadDataParam.GetKStep(),
                loadDataParam.GetSrcStride(), loadDataParam.GetDstStride(), 1);
        } else {
            load_cbuf_to_cb_s4(
                (__cb__ void*)dst, (__cbuf__ void*)src, loadDataParam.GetMStartPosition(),
                loadDataParam.GetKStartPosition(), loadDataParam.GetMStep(), loadDataParam.GetKStep(),
                loadDataParam.GetSrcStride(), loadDataParam.GetDstStride(), 0);
        }
    } else {
        if (loadDataParam.GetIfTranspose()) {
            load_cbuf_to_cb(
                dst, src, loadDataParam.GetMStartPosition(), loadDataParam.GetKStartPosition(),
                loadDataParam.GetMStep(), loadDataParam.GetKStep(), loadDataParam.GetSrcStride(),
                loadDataParam.GetDstStride(), 1);
        } else {
            load_cbuf_to_cb(
                dst, src, loadDataParam.GetMStartPosition(), loadDataParam.GetKStartPosition(),
                loadDataParam.GetMStep(), loadDataParam.GetKStep(), loadDataParam.GetSrcStride(),
                loadDataParam.GetDstStride(), 0);
        }
    }
#else
    if constexpr (SupportType<T, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t>()) {
        if (loadDataParam.GetIfTranspose()) {
            load_cbuf_to_cb_s4(
                (__cb__ T*)dst, (__cbuf__ T*)src, loadDataParam.GetConfig0(), loadDataParam.GetConfig1(), true);
        } else {
            load_cbuf_to_cb_s4(
                (__cb__ T*)dst, (__cbuf__ T*)src, loadDataParam.GetConfig0(), loadDataParam.GetConfig1(), false);
        }
    } else {
        if (loadDataParam.GetIfTranspose()) {
            load_cbuf_to_cb(dst, src, loadDataParam.GetConfig0(), loadDataParam.GetConfig1(), true);
        } else {
            load_cbuf_to_cb(dst, src, loadDataParam.GetConfig0(), loadDataParam.GetConfig1(), false);
        }
    }
#endif
}

template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParamsV2& loadDataParam)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_2d_v2 from A1 to A2 on current device"); });
}

template <typename T>
__aicore__ inline void LoadData2DL12L0BCal(__cb__ T* dst, __cbuf__ T* src, const LoadData2DParamsV2& loadDataParam)
{
    static_assert(
        SupportType<
            T, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t, uint8_t, int8_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t, half,
            bfloat16_t, uint16_t, int16_t>(),
        "LoadData 2dv2 only support  int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t, uint8_t, int8_t, fp8_e5m2_t, fp8_e4m3fn_t, "
        "hifloat8_t, half, bfloat16_t, uint16_t, int16_t on current device!");
    if constexpr (SupportType<T, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t>()) {
        if (loadDataParam.ifTranspose) {
            load_cbuf_to_cb_s4(
                (__cb__ void*)dst, (__cbuf__ void*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
                loadDataParam.mStep, loadDataParam.kStep, loadDataParam.srcStride, loadDataParam.dstStride, 1);
        } else {
            load_cbuf_to_cb_s4(
                (__cb__ void*)dst, (__cbuf__ void*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
                loadDataParam.mStep, loadDataParam.kStep, loadDataParam.srcStride, loadDataParam.dstStride, 0);
        }
    } else {
        if (loadDataParam.ifTranspose) {
            load_cbuf_to_cb(
                dst, src, loadDataParam.mStartPosition, loadDataParam.kStartPosition, loadDataParam.mStep,
                loadDataParam.kStep, loadDataParam.srcStride, loadDataParam.dstStride, 1);
        } else {
            load_cbuf_to_cb(
                dst, src, loadDataParam.mStartPosition, loadDataParam.kStartPosition, loadDataParam.mStep,
                loadDataParam.kStep, loadDataParam.srcStride, loadDataParam.dstStride, 0);
        }
    }
}

template <typename T, typename U>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ U* src, const LoadData2DParamsV2& loadDataParam, const Nd2NzParamsV2& nd2nzParams)
{
    static_assert(
        ((IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value)),
        "LoadData dst type or src type is not supported on current device!");
    set_mte2_qtable0(nd2nzParams.lookupTable0);
    set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    uint8_t decompressMode = 0;
    if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0111;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0001;
    } else if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value) {
        decompressMode = 0b0110;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) {
        set_mte2_qtable1(nd2nzParams.lookupTable1);
        decompressMode = 0b0011;
    }
    load_gm_to_cbuf_2dv2(
        (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
        loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, 0, decompressMode, 0);
}

template <typename T, typename U = T>
__aicore__ inline void LoadData2DL12L0ACal(
    __ca__ U* dst, __cbuf__ T* src0, __cbuf__ fp8_e8m0_t* src1, const LoadData2DParamsV2& loadDataParam,
    const LoadData2DMxParams& loadMxDataParams)
{
#if ASCENDC_CPU_DEBUG
    uint64_t l0ABaseAddr = static_cast<uint64_t>(
        reinterpret_cast<uintptr_t>(ConstDefiner::Instance().GetHardwareBaseAddr(Hardware::L0A)));
    uint64_t bufferOffset = (static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst)) - l0ABaseAddr) / 8;
    uint64_t mxDstAddr =
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(ConstDefiner::Instance().cpuL0AMx)) + bufferOffset;
#else
    uint64_t mxDstAddr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst)) / 8;
#endif
    load_cbuf_to_ca_mx(
        mxDstAddr, static_cast<__cbuf__ void*>(src1), loadMxDataParams.xStartPosition,
        loadMxDataParams.yStartPosition, loadMxDataParams.xStep, loadMxDataParams.yStep, loadMxDataParams.srcStride,
        loadMxDataParams.dstStride);
}

template <typename T, typename U = T>
__aicore__ inline void LoadData2DL12L0BCal(
    __cb__ U* dst, __cbuf__ T* src0, __cbuf__ fp8_e8m0_t* src1, const LoadData2DParamsV2& loadDataParam,
    const LoadData2DMxParams& loadMxDataParams)
{
    LoadData2DL12L0BCal(dst, src0, loadDataParam);
#if ASCENDC_CPU_DEBUG
    uint64_t l0BBaseAddr = static_cast<uint64_t>(
        reinterpret_cast<uintptr_t>(ConstDefiner::Instance().GetHardwareBaseAddr(Hardware::L0B)));
    uint64_t bufferOffset = (static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst)) - l0BBaseAddr) / 8;
    uint64_t mxDstAddr =
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(ConstDefiner::Instance().cpuL0BMx)) + bufferOffset;
#else
    uint64_t mxDstAddr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst)) / 8;
#endif
    load_cbuf_to_cb_mx(
        mxDstAddr, static_cast<__cbuf__ void*>(src1), loadMxDataParams.xStartPosition,
        loadMxDataParams.yStartPosition, loadMxDataParams.xStep, loadMxDataParams.yStep, loadMxDataParams.srcStride,
        loadMxDataParams.dstStride);
}

template <typename T>
__aicore__ inline void LoadData2DGM2L0ACal(
    __ca__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT(
        (false), { KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_2d_v2 from gm to A2 on current device"); });
}

template <typename T>
__aicore__ inline void LoadData2DGM2L0BCal(
    __cb__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam, const uint8_t cacheMode = 0)
{
    set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    if constexpr (SupportType<T, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t>()) {
        load_gm_to_cb_2dv2_s4(
            (__cb__ void*)dst, (__gm__ void*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0, cacheMode);
    } else {
        load_gm_to_cb_2dv2(
            dst, src, loadDataParam.mStartPosition, loadDataParam.kStartPosition, 
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0, cacheMode);
    }
}

template <typename T>
__aicore__ inline void LoadData2DGM2L0BCal(
    __cb__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam, const Nd2NzParamsV2 &nd2nzParams)
{
    static_assert(
        ((IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value)),
        "LoadData dst type or src type is not supported on current device!");
    set_mte2_qtable0(nd2nzParams.lookupTable0);
    set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    uint8_t decompressMode = 0;
    if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0111;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0001;
    } else if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value) {
        decompressMode = 0b0110;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) {
        set_mte2_qtable1(nd2nzParams.lookupTable1);
        decompressMode = 0b0011;
    }
    load_gm_to_cb_2dv2(
        (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
        loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, 0, decompressMode, 0);
}

template <typename T>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam, const uint8_t cacheMode = 0)
{
    static_assert(
        SupportType<
            T, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t, uint8_t, int8_t, hifloat8_t, fp8_e5m2_t, fp8_e4m3fn_t, half,
            bfloat16_t, uint16_t, int16_t, float, int32_t, uint32_t>(),
        "LoadData 2dv2 only support int4b_t fp4x2_e2m1_t, fp4x2_e1m2_t, uint8_t, int8_t, hifloat8_t, fp8_e5m2_t, fp8_e4m3fn_t, "
        "half, bfloat16_t, uint16_t, int16_t, float, int32_t, uint32_t on current device!");
    set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    if constexpr (SupportType<T, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t>()) {
        load_gm_to_cbuf_2dv2_s4(
            (__cbuf__ void*)dst, (__gm__ void*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0, cacheMode);
    } else {
        load_gm_to_cbuf_2dv2(
            dst, src, loadDataParam.mStartPosition, loadDataParam.kStartPosition, loadDataParam.dstStride,
            loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0, cacheMode);
    }
}

/* **************************************************************************************************
 * LoadDataWithTranspose                                        *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParam)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "LoadDataWithTranspose from A1 to A2 is not supported on current device"); });
}

template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParam)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "LoadDataWithTranspose from B1 to B2 is not supported on current device"); });
}

/* **************************************************************************************************
 * Mmad                                             *
 * ************************************************************************************************* */
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadParams& mmadParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "Mmad is not supported on current device"); });
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, const MmadParams& mmadParams, bool cmatrixSource)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "Mmad is not supported on current device"); });
}

/* **************************************************************************************************
 * BroadCastVecToMM                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void BroadCastVecToMMCal(
    __cc__ T* dstLocal, __ubuf__ T* srcLocal, const int32_t blockCount, const uint8_t blockLen, const uint8_t srcGap,
    const uint8_t dstGap)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "BroadCastVecToMM is not supported on current device"); });
}

/* **************************************************************************************************
 * InitL1Buffer                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void InitL1BufferCal(__cbuf__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (sizeof(T) == 2) {
        T tmpValue = initConstValueParams.initValue;
        half initValue = (half &)tmpValue;
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= (uint64_t)initConstValueParams.repeatTimes;
        config |= (uint64_t)initConstValueParams.blockNum << 16;
        config |= (uint64_t)initConstValueParams.dstGap << 32;
        set_l1_2d((__cbuf_ half *)dst, config);
    } else {
        ASCENDC_ASSERT(
            false, { KERNEL_LOG(KERNEL_ERROR, "InitConstValue doesn't support current data type on current device"); });
    }
}

/* **************************************************************************************************
 * InitL0ANzMatrix                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void InitL0ANzMatrixCal(__ca__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "InitConstValue in A2 is not supported on current device"); });
}

/* **************************************************************************************************
 * InitL0BNzMatrix                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void InitL0BNzMatrixCal(__cb__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "InitConstValue in B2 is not supported on current device"); });
}

/* **************************************************************************************************
 * LoadData 3dv1                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LoadData3DV1L12L0ACal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData3DParamsV1<T>& loadDataParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_3d_v1 from l1 to l0a"); });
}

template <typename T>
__aicore__ inline void LoadData3DV1L12L0BCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData3DParamsV1<T>& loadDataParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_3d_v1 from l1 to l0b"); });
}

template <typename T>
__aicore__ inline void LoadData3DV1L12UBCal(
    __ubuf__ T* dst, __cbuf__ T* src, const LoadData3DParamsV1<T>& loadDataParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_3d_v1 from l1 to ubuf"); });
}

/* **************************************************************************************************
 * LoadData 3dv2                                             *
 * ************************************************************************************************* */
__aicore__ inline void Load3DSetFMatrixCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Load3DSetFMatrix is not supported on current device"); });
}

__aicore__ inline void Load3DSetFMatrixCal(uint64_t regFMatrix) 
{ 
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Load3DSetFMatrix is not supported on current device"); });
}

__aicore__ inline void Load3DSetFMatrixBCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Load3DSetFMatrixB is not supported on current device"); });
}

__aicore__ inline void Load3DSetFMatrixBCal(uint64_t regFMatrix) 
{ 
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Load3DSetFMatrixB is not supported on current device"); });
}

template <typename T>
__aicore__ inline void Load3DSetPaddingCal(const T padValue)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Load3DSetPadding is not supported on current device"); });
}

__aicore__ inline void SetLoadDataRepeatWithStrideCal(const LoadDataRepeatParamWithStride repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "SetLoadDataRepeatWithStride is not supported on current device"); });

}

__aicore__ inline void SetLoadDataRepeatCal(const LoadDataRepeatParam repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "SetLoadDataRepeat is not supported on current device"); });

/* **************************************************************************************************
 * SetLoadDataBoundary                                             *
 * ************************************************************************************************* */
__aicore__ inline void SetLoadDataBoundaryCal(uint32_t boundaryValue)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "SetLoadDataBoundary is not supported on current device!"); });
}

/* **************************************************************************************************
 * LoadData 3dv2                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LoadData3DV2L12L0ACal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2<T>& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0A is not supported on current device"); });
}

template <typename T>
__aicore__ inline void LoadData3DV2L12L0BCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2<T>& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0B is not supported on current device"); });
}

__aicore__ inline void LoadData3DV2L12L0ACal(
    __ca__ half* dst, __cbuf__ half* src, const LoadData3DParamsV2<bfloat16_t>& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0A is not supported on current device"); });
}

__aicore__ inline void LoadData3DV2L12L0BCal(
    __cb__ half* dst, __cbuf__ half* src, const LoadData3DParamsV2<bfloat16_t>& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0B is not supported on current device"); });
}

template <typename T>
__aicore__ inline void LoadData3DV2L12UBCal(
    __ubuf__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2<T>& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12UB is not supported on current device"); });
}

template <typename T>
__aicore__ inline void LoadData3DV2L12L0AWithStrideCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2<T>& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0AWithStride is not supported on current device"); });
}

template <typename T>
__aicore__ inline void LoadData3DV2L12L0BWithStrideCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2<T>& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0BWithStride is not supported on current device"); });
}

__aicore__ inline void LoadData3DV2L12L0AWithStrideCal(
    __ca__ half* dst, __cbuf__ half* src, const LoadData3DParamsV2<bfloat16_t>& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0AWithStride is not supported on current device"); });
}

__aicore__ inline void LoadData3DV2L12L0BWithStrideCal(
    __cb__ half* dst, __cbuf__ half* src, const LoadData3DParamsV2<bfloat16_t>& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0BWithStride is not supported on current device"); });
}

template <typename T>
__aicore__ inline void LoadData3DV2L12UBWithStrideCal(
    __ubuf__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2<T>& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12UBWithStride is not supported on current device"); });
}

template <typename T>
__aicore__ inline void LoadData3DV2L12L0ACal(__ca__ T* dst, __cbuf__ T* src, const Load3DBitModeParam& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0A is not supported on current device"); });
}

template <typename T>
__aicore__ inline void LoadData3DV2L12L0BCal(__cb__ T* dst, __cbuf__ T* src, const Load3DBitModeParam& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0B is not supported on current device"); });
}

template <typename T>
__aicore__ inline void LoadData3DV2L12UBCal(__ubuf__ T* dst, __cbuf__ T* src, const Load3DBitModeParam& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12UB is not supported on current device"); });
}
/* **************************************************************************************************
 * LoadData 3dv2Pro                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LoadData3DV2L12L0ACal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    ASCENDC_ASSERT(
        false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0A is not supported 3DParamsV2Pro on current device"); });
}

template <typename T>
__aicore__ inline void LoadData3DV2L12L0BCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    ASCENDC_ASSERT(
        false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12L0B is not supported 3DParamsV2Pro on current device"); });
}

template <typename T>
__aicore__ inline void LoadData3DV2L12UBCal(
    __ubuf__ T* dst, __cbuf__ T* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    ASCENDC_ASSERT(
        false, { KERNEL_LOG(KERNEL_ERROR, "LoadData3DV2L12UB is not supported 3DParamsV2Pro on current device"); });
}

__aicore__ inline uint8_t GetQuantPreMode(pre_quant_t qurantPre)
{
    uint8_t quantPreMode = 0;
    switch (quantPre) {
        case pre_quant_t::No_quant:
            quantPreMode = 0;
            break;
        case pre_quant_t::REQS8_Vector:
            quantPreMode = 2;
            break;
        case pre_quant_t::REQS8_Scalar:
            quantPreMode = 3;
            break;
        case pre_quant_t::DEQF16_Vector:
            quantPreMode = 4;
            break;
        case pre_quant_t::DEQF16_Scalar:
            quantPreMode = 5;
            break;
        case pre_quant_t::DEQS16_Vector:
            quantPreMode = 6;
            break;
        case pre_quant_t::DEQS16_Scalar:
            quantPreMode = 7;
            break;
        case pre_quant_t::DEQS32_Vector:
            quantPreMode = 8;
            break;
        case pre_quant_t::DEQS32_Scalar:
            quantPreMode = 9;
            break;
        case pre_quant_t::REQS4_Vector:
            quantPreMode = 10;
            break;
        case pre_quant_t::REQS4_Scalar:
            quantPreMode = 11;
            break;
        case pre_quant_t::QS322FP8_E4M3_INT_Vector:
            quantPreMode = 12;
            break;
        case pre_quant_t::QS322FP8_E4M3_INT_Scalar:
            quantPreMode = 13;
            break;
        default:
            quantPreMode = 0;
            break;
    }
    return quantPreMode;
}

/* **************************************************************************************************
 * MmadFix                                                                                          *
 * ************************************************************************************************* */
__aicore__ inline void SetMatrixParaCal(uint64_t config)
{
    set_matrix_para(config);
}

__aicore__ inline void SetMatrixParaCal(const MatrixParams &params)
{
    uint64_t config = 0;

    confog = (((uint64_t)params.m & 0xffff) << 0) | 
             (((uint64_t)params.k & 0xffff) << 16);
    set_matrix_para(config);
}

__aicore__ inline void SetMmadStrideParaCal(uint64_t config)
{
    set_cube_stride_para(config);
}

__aicore__ inline void SetMmadStrideParaCal(const MmadStrideParams &params)
{
    uint64_t config = 0;

    confog = (((uint64_t)params.loop4SrcStride & 0xffff) << 0) | 
             (((uint64_t)params.loop4DstStride & 0xffff) << 16);
    set_cube_stride_para(config);
}

__aicore__ inline void SetMatrixMxParaCal(uint64_t config)
{
    set_mx_buf_addr(config);
}

__aicore__ inline void SetMatrixMxParaCal(const MatrixMxParams &params)
{
    uint64_t config = 0;

    confog = (((uint64_t)params.l0AMxAddr & 0xffff) << 0) | 
             (((uint64_t)params.l0BMxAddr & 0xffff) << 32);
    set_mx_buf_addr(config);
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void CheckCubeFixSupportTypes(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b)
{
    static_assert(
        ((IsSameType<Src0T, int8_t>::value && IsSameType<Src1T, int8_t>::value) ||
         (IsSameType<Src0T, int16_t>::value && IsSameType<Src1T, int8_t>::value) ||
         (IsSameType<Src0T, int8_t>::value && IsSameType<Src1T, int4b_t>::value) ||
         (IsSameType<Src0T, int4b_t>::value && IsSameType<Src1T, int4b_t>::value) ||
         (IsSameType<Src0T, half>::value && IsSameType<Src1T, half>::value) ||
         (IsSameType<Src0T, fp8_e4m3_fn_t>::value && IsSameType<Src1T, fp8_e4m3_fn_t>::value) ||
         (IsSameType<Src0T, fp8_e4m3_fn_t>::value && IsSameType<Src1T, int8_t>::value) ||
         (IsSameType<Src0T, fp8_e4m3_fn_t>::value && IsSameType<Src1T, int4b_t>::value)),
        "MmadFix dst type or src type is not supported on current device!");
}

__aicore__ inline void GenMmadFixConfig(const MmadFixParams& params, uint64_t &config)
{
    config = 0;
    config | = (((uint64_t)params.biasMatrixInitVal & 0x1) << 0);
    config | = (((uint64_t)params.biasMatrixBroadcast & 0x1) << 1);
    config | = (((uint64_t)GetQuantPreMode(params.preQuantMode) & 0x1F) << 2);
    config | = (((uint64_t)params.preReluMode & 0x7) << 7);
    config | = (((uint64_t)params.postQuantMode & 0x1f) << 10);
    config | = (((uint64_t)params.actPostEnable & 0x7) << 15);
    config | = (((uint64_t)params.preClipReluMode & 0x1) << 18);
    config | = (((uint64_t)params.eltwiseOp & 0x7) << 20);
    config | = (((uint64_t)params.eltwiseAntiqEnable & 0x1) << 23);
    config | = (((uint64_t)params.eltwiseBroadcastEnable & 0x1) << 24);
    config | = (((uint64_t)params.lsbMask & 0x7) << 28);
    config | = (((uint64_t)params.gemvCtrl & 0x1) << 31);
    config | = (((uint64_t)params.dependEnable & 0x1) << 36);
    config | = (((uint64_t)params.instrId & 0x3) << 37);
    config | = (((uint64_t)params.postProcessBandwithCtrl & 0x1) << 39);
    config | = (((uint64_t)params.n & 0xffff) << 48);
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MatmulToCbuf(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t bias, uint64_t config)
{
    if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<int4b_t, int4b_t>>()) {
        matmul_to_cbuf_s4s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), bias, config);
    } else if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<int8_t, int4b_t>>()) {
        matmul_to_cbuf_s8s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), bias, config);
    } else if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<fp8_e4m3fn_t, int4b_t>>()) {
        matmul_to_cbuf_e4m3s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), bias, config);
    } else {
        matmul_to_cbuf((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), bias, config);
    }
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MatmulToCbufNoBroadcast(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t bias, uint64_t config)
{

    if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<int4b_t, int4b_t>>()) {
        matmul_to_cbuf_s4s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)bias, 0ULL, config);
    } else if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<int8_t, int4b_t>>()) {
        matmul_to_cbuf_s8s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)bias, 0ULL, config);
    } else if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<fp8_e4m3fn_t, int4b_t>>()) {
        matmul_to_cbuf_e4m3s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)bias, 0ULL, config);
    } else {
        matmul_to_cbuf((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)bias, 0ULL, config);
    }
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, const MmadFixParams &params)
{
    uint64_t config = 0;
    CheckCubeFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    GenMmadFixConfig(params, config);
    MatmulToCbuf<DstT, Src0T, Src1T>(c, a, b, 0ULL, config);
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t bias,
    const MmadFixParams& params)
{
    uint64_t config = 0;
    CheckCubeFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    GenMmadFixConfig(params, config);
    if (params.biasMatrixBroadcast) {
        MatmulToCbuf<DstT, Src0T, Src1T>(c, a, b, bias, config);
    } else {
        MatmulToCbufNoBroadcast<DstT, Src0T, Src1T>(c, a, b, bias, config);
    }
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t config)
{
    CheckCubeFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    MatmulToCbuf<DstT, Src0T, Src1T>(c, a, b, 0ULL, config);
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t bias, 
    uint64_t config)
{
    CheckCubeFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    if (config & 0x2) {
        MatmulToCbuf<DstT, Src0T, Src1T>(c, a, b, bias, config);
    } else {
        MatmulToCbufNoBroadcast<DstT, Src0T, Src1T>(c, a, b, bias, config);
    }
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void CheckMmadMxFixSupportTypes(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b)
{
    static_assert(
        ((IsSameType<Src0T, hifloat4x2_t>::value && IsSameType<Src1T, int4b_t>::value) ||
         (IsSameType<Src0T, half>::value && IsSameType<Src1T, fp8_e4m3fn_t>::value) ||
         (IsSameType<Src0T, fp8_e4m3fn_t>::value && IsSameType<Src1T, fp8_e4m3fn_t>::value) ||
         (IsSameType<Src0T, fp8_e4m3fn_t>::value && IsSameType<Src1T, int4b_t>::value)),
        "MmadMxFix dst type or src type is not supported on current device!");
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadMxFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, const MmadFixParams &params)
{
    uint64_t config = 0;
    CheckMmadMxFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    GenMmadFixConfig(params, config);
    matmul_to_cbuf((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), 0ULL, config);
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadMxFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t bias,
    const MmadFixParams &params)
{
    uint64_t config = 0;
    CheckMmadMxFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    GenMmadFixConfig(params, config);
    if (params.biasMatrixBroadcast) {
        matmul_to_cbuf((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), bias, config);
    } else {
        matmul_to_cbuf((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)bias, 0ULL, config);
    }
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadMxFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t config)
{
    CheckMmadMxFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    matmul_to_cbuf((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), 0ULL, config);
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadMxFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t bias,
    uint64_t config)
{
    CheckMmadMxFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    if (params.biasMatrixBroadcast) {
        matmul_to_cbuf((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), bias, config);
    } else {
        matmul_to_cbuf((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)bias, 0ULL, config);
    }
}

/* **************************************************************************************************
 * ConvFix                                                                                          *
 * ************************************************************************************************* */
__aicore__ inline void SetConvStrideParaCal(uint64_t config)
{
    set_cube_stride_para(config);
}

__aicore__ inline void SetConvStrideParaCal(const ConvStrideParams &params)
{
    uint64_t config = 0;

    confog = (((uint64_t)params.loop3DstStride & 0xffff) << 0) | 
             (((uint64_t)params.loop4DstStride & 0xffff) << 16);
    set_cube_stride_para(config);
}

__aicore__ inline void SetConvPaddingCal(uint64_t config)
{
    set_m_padding(config);
}

template <typename T>
__aicore__ inline void SetConvPaddingCal(const ConvPaddingParams<T> &params)
{
    T tmpValue = params.paddingValue;
    uint64_t config = 0;

    if constexpr (sizeof(T) == B16_BYTE_SIZE) {
        uint64_t paddingValue = static_cast<uint64_t>(tmpValue) & 0xffff;
        config = paddingValue;
    } else if constexpr (sizeof(T) == B8_BYTE_SIZE) {
        uint64_t paddingValue = static_cast<uint64_t>(tmpValue) & 0xff;
        config = (paddingValue << 8 | paddingValue);
    } else {
        uint64_t paddingValue = static_cast<uint64_t>(tmpValue) & 0xf;
        config = (paddingValue << 12 | paddingValue << 8 | paddingValue << 4 | paddingValue);
    }

    confog = (((uint64_t)params.paddingMode & 0x3) << 32) | 
             (((uint64_t)params.paddingAddr & 0xffff) << 34);
    set_m_padding(config);
}

__aicore__ inline void SetConvFMatrixDualCal(uint64_t config0, uint64_t config1)
{
    set_m_fmatrix_dual_0(config0);
    set_m_fmatrix_dual_0(config1);
}

__aicore__ inline void SetConvFMatrixDualCal(const ConvFMatrixDualParams &params)
{
    uint64_t config0 = 0;
    uint64_t config1 = 0;

    config0 |= (uint64_t)params.dualAddr & 0xffffffff
    config0 |= (((uint64_t)params.dualSize & 0xffff) << 32);

    config1 |= (uint64_t)params.dualStride1 & 0xffff
    config1 |= (((uint64_t)params.dualStride2 & 0xffff) << 32);

    set_m_fmatrix_dual_0(config0);
    set_m_fmatrix_dual_0(config1);
}

__aicore__ inline void SetConvFMatrixStepPosCal(uint64_t config)
{
    set_m_step_pos(config);
}

__aicore__ inline void SetConvFMatrixStepPosCal(const ConvFMatrixStepPosParams &params)
{
    uint64_t config = 0;
    config |= (((uint64_t)params.wStartPos & 0xffff) << 0);
    config |= (((uint64_t)params.hStartPos & 0xffff) << 16);
    config |= (((uint64_t)params.wStep & 0xffff) << 32);
    config |= (((uint64_t)params.hStep & 0xffff) << 48);

    set_m_step_pos(config);
}

__aicore__ inline void SetConvFMatrixCal(uint64_t config)
{
    set_m_fmatrix(config);
}

__aicore__ inline void SetConvFMatrixCal(const ConvFMatrixParams &params)
{
    uint64_t config = 0;
    config |= (((uint64_t)params.w & 0xffff) << 0);
    config |= (((uint64_t)params.h & 0xffff) << 16);
    config |= (((uint64_t)params.leftPaddingSize & 0xff) << 32);
    config |= (((uint64_t)params.rightPaddingSize & 0xff) << 40);
    config |= (((uint64_t)params.topPaddingSize & 0xff) << 48);
    config |= (((uint64_t)params.bottomPaddingSize & 0xff) << 56);

    set_m_fmatrix(config);
}

__aicore__ inline void SetConvKernelCal(uint64_t config)
{
    set_kernel(config);
}

__aicore__ inline void SetConvFMatrixCal(const ConvKernelParams &params)
{
    uint64_t config = 0;
    config |= (((uint64_t)params.wStride & 0x3f) << 0);
    config |= (((uint64_t)params.hStride & 0x3f) << 6);
    config |= (((uint64_t)params.wKernelSize & 0x1ff) << 12);
    config |= (((uint64_t)params.hKernelSize & 0x1ff) << 21);
    config |= (((uint64_t)params.wDilation & 0xff) << 30);
    config |= (((uint64_t)params.hDilation & 0xff) << 38);
    config |= (((uint64_t)params.kernelNum & 0xfff) << 46);

    set_kernel(config);
}

__aicore__ inline void GenConvFixConfig(const ConvFixParams& params, uint64_t &config)
{
    config = 0;
    config | = (((uint64_t)params.biasMatrixInitVal & 0x1) << 0);
    config | = (((uint64_t)params.biasMatrixBroadcast & 0x1) << 1);
    config | = (((uint64_t)GetQuantPreMode(params.preQuantMode) & 0x1F) << 2);
    config | = (((uint64_t)params.preReluMode & 0x7) << 7);
    config | = (((uint64_t)params.postQuantMode & 0x1f) << 10);
    config | = (((uint64_t)params.actPostEnable & 0x7) << 15);
    config | = (((uint64_t)params.preClipReluMode & 0x1) << 18);
    config | = (((uint64_t)params.eltwiseOp & 0x7) << 20);
    config | = (((uint64_t)params.eltwiseAntiqEnable & 0x1) << 23);
    config | = (((uint64_t)params.eltwiseBroadcastEnable & 0x1) << 24);
    config | = (((uint64_t)params.lsbMask & 0x7) << 28);
    config | = (((uint64_t)params.dualSrcEnable & 0x1) << 34);
    config | = (((uint64_t)params.dependEnable & 0x1) << 36);
    config | = (((uint64_t)params.instrId & 0x3) << 37);
    config | = (((uint64_t)params.postProcessBandwithCtrl & 0x1) << 39);
    config | = (((uint64_t)params.nonBroadcastBias & 0x1) << 40);
    config | = (((uint64_t)params.n & 0xffff) << 48);
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void Convolution(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t bias, uint64_t config)
{
    if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<int4b_t, int4b_t>>()) {
        convolution_s4s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), bias, config);
    } else if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<int8_t, int4b_t>>()) {
        convolution_s8s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), bias, config);
    } else if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<fp8_e4m3fn_t, int4b_t>>()) {
        convolution_e4m3s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), bias, config);
    } else {
        convolution((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)get_imm(0), bias, config);
    }
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void ConvolutionNoBroadcast(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t bias, uint64_t config)
{

    if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<int4b_t, int4b_t>>()) {
        convolution_s4s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)bias, 0ULL, config);
    } else if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<int8_t, int4b_t>>()) {
        convolution_s8s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)bias, 0ULL, config);
    } else if constexpr (SupportType<Tuple<Src0T, Src1T>, Tuple<fp8_e4m3fn_t, int4b_t>>()) {
        convolution_e4m3s4((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)bias, 0ULL, config);
    } else {
        convolution((__cbuf__ void *)c, a, b, (__cbuf__ int32_t *)bias, 0ULL, config);
    }
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void ConvFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, const ConvFixParams &params)
{
    uint64_t config = 0;
    CheckCubeFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    GenConvFixConfig(params, config);
    Convolution<DstT, Src0T, Src1T>(c, a, b, 0ULL, config);
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void ConvFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t bias,
    const MmadFixParams& params)
{
    uint64_t config = 0;
    CheckCubeFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    GenConvFixConfig(params, config);
    if (params.biasMatrixBroadcast) {
        Convolution<DstT, Src0T, Src1T>(c, a, b, bias, config);
    } else {
        ConvolutionNoBroadcast<DstT, Src0T, Src1T>(c, a, b, bias, config);
    }
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void ConvFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t config)
{
    CheckCubeFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    Convolution<DstT, Src0T, Src1T>(c, a, b, 0ULL, config);
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void ConvFixCal(__cbuf__ DstT* c, __cbuf__ Src0T* a, __cb__ Src1T* b, uint64_t bias, 
    uint64_t config)
{
    CheckCubeFixSupportTypes<DstT, Src0T, Src1T>(c, a, b);
    if (config & 0x2) {
        Convolution<DstT, Src0T, Src1T>(c, a, b, bias, config);
    } else {
        ConvolutionNoBroadcast<DstT, Src0T, Src1T>(c, a, b, bias, config);
    }
}


/* **************************************************************************************************
 * Aipp/Waipp                                                                                        *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LoadImageToLocalCal(__cbuf__ T* dest,  const LoadImageToLocalParams &params)
{
    static_assert((SupportType<T, uint8_t, int8_t, int16_t>()), 
        "LoadImageToLocal only support uint8_t/int8_t/int16_t on current device")
    load_image_to_cbuf(dst,
        static_cast<uint16_t>(params.horizSize - 1),
        static_cast<uint16_t>(params.vertSize - 1),
        params.horizStartPos,
        params.vertStartPos,
        static_cast<uint16_t>(params.srcHorizSize - 1),
        params.topPadSize,
        params.botPadSize,
        params.leftPadSize,
        params.rightPadSize,
        params.sid);
}

tepmplate <typename T, typename U>
__aicore__ inline void StoreImageToGlobalCal(__gm__ T *dest,  __cbuf__ U *src, const StoreImageToGlobalParams &params)
{
    static_assert((SupportType<T, uint8_t, int16_t>()), 
        "StoreImageToGlobal only support uint8_t/int16_t on current device")
    store_l1_to_out_image((__gm__ void *)dst, src,
        static_cast<uint16_t>(params.dstImageWidth - 1),
        static_cast<uint8_t>(params.outputDataType),
        static_cast<uint16_t>(params.dstImageHeight - 1),
        static_cast<uint8_t>(params.channelTransformMode),
        static_cast<uint16_t>(params.dstImageStride),
        static_cast<uint8_t>(params.sid),
        static_cast<bool>(params.dtcParams.roundingMode),
        static_cast<uint32_t>(params.dtcParams.dstOffset),
        static_cast<uint8_t>(params.dtcParams.dtcGain),
        static_cast<uint16_t>(params.srcImageStride));
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_MM_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_IMPL_H__
#endif
