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
 * \file kernel_operator_fixpipe_intf_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_fixpipe_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_fixpipe_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_INTF_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_FIXPIPE_INTERFACE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_FIXPIPE_INTERFACE_IMPL_H
#include "../../include/basic_api/kernel_tensor.h"
#include "kernel_check.h"
#include "../../include/basic_api/kernel_struct_fixpipe.h"
#include "kernel_operator_mm_check.h"
#include "kernel_process_lock.h"

#if __NPU_ARCH__ == 1001
#include "dav_c100/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 2002
#include "dav_m200/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 2201
#include "dav_c220/kernel_operator_fixpipe_impl.h"
#include "dav_c220/kernel_operator_fixpipe_v2_impl.h"
#elif __NPU_ARCH__ == 3002
#include "dav_m300/kernel_operator_fixpipe_impl.h"
#include "dav_m300/kernel_operator_fixpipe_v2_impl.h"
#elif __NPU_ARCH__ == 3102
#include "dav_m310/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 3510
#include "dav_3510/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 9201
#include "dav_9201/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 9202
#include "dav_9202/kernel_operator_fixpipe_impl.h"
#elif (__NPU_ARCH__ == 5102)
#include "dav_m510/kernel_operator_fixpipe_impl.h"
#elif (__NPU_ARCH__ == 5162)
#include "dav_5162/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 3003
#include "dav_l300/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 3113
#include "dav_l311/kernel_operator_fixpipe_impl.h"
#elif (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
#include "dav_5161/kernel_operator_fixpipe_impl.h"
#endif

namespace AscendC {
/* **************************************************************************************************
 * Fixpipe                                             *
 * ************************************************************************************************* */
/*
 * @ingroup Fixpipe
 * @brief After calculation, process the results
 * @param [out] dst output GlobalTensor
 * @param [in] src input LocalTensor
 * @param [in] intriParams.cburstNum number of burst
 * @param [in] intriParams.burstLen burst length
 * @param [in] intriParams.srcStride src block stride
 * @param [in] intriParams.dstStride dst block stride
 * @param [in] intriParams.biasParams contains isBias flag and bias LocalTensor
 * @param [in] intriParams.quantParams contains quant mode and quant params
 * @param [in] intriParams.reluEn indicates whether to enable the relu function
 * @param [in] intriParams.nz2ndParams contains the input params for enable the nz2nd function
 */

template <typename T>
__aicore__ inline void SetFixPipeConfig(const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag)
{
    SetFixPipeConfigImpl<T>(reluPre, quantPre, isUnitFlag);
}

template <typename T, bool setRelu>
__aicore__ inline void SetFixPipeConfig(const LocalTensor<T>& preData, bool isUnitFlag)
{
    SetFixPipeConfigImpl<T, setRelu>(preData, isUnitFlag);
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 5101 || \
                              __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163 || \
                              __NPU_ARCH__ == 5162 || __NPU_ARCH__ == 9201 || __NPU_ARCH__ == 9202)
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    SetFixpipeNz2ndFlagImpl(ndNum, srcNdStride, dstNdStride);
}
#else
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride)
{
    SetFixpipeNz2ndFlagImpl(ndNum, srcNdStride, dstNdStride);
}
#endif

__aicore__ inline void SetFixpipePreQuantFlag(uint64_t config) { SetFixpipePreQuantFlagImpl(config); }

__aicore__ inline void SetFixPipeClipRelu(uint64_t config) { SetFixPipeClipReluImpl(config); }

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5162)
template <typename T1, typename T2>
__aicore__ inline void SetFixPipeClipRelu(const FixpipeClipReluParams<T1, T2>& params)
{
    SetFixPipeClipReluImpl(params);
}

__aicore__ inline void SetFixPipeConfig(uint64_t config) { SetFixPipeConfigImpl(config); }

template <typename T>
__aicore__ inline void SetFixpipePreQuantFlag(const FixPipePreQuantParams<T>& params)
{
    SetFixpipePreQuantFlagImpl(params);
}

__aicore__ inline void SetFixpipePostQuant(const FixPipePostQuantParams& params) { SetFixpipePostQuantImpl(params); }

template <typename T>
__aicore__ inline void SetFixpipeEltAntiq(const FixPipeEltAntiqParams<T>& params)
{
    SetFixpipeEltAntiqImpl(params);
}

__aicore__ inline void SetFixpipeEltwiseAddr(uint64_t config) { SetFixpipeEltwiseAddrImpl(config); }

__aicore__ inline void SetFixpipeEltwiseAddr(const FixpipeEltwiseAddrParams& params)
{
    SetFixpipeEltwiseAddrImpl(params);
}
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 9202)
template <typename T, bool enableCompareWithMaxBuf, bool enableMaxOut2UB, bool enableAbsInput>
__aicore__ inline void SetFixPipeMaxTrack(LocalTensor<T> rowOutput, LocalTensor<T> groupOutput, uint32_t groupN)
{
    SetFixPipeMaxTrackImpl<T, enableCompareWithMaxBuf, enableMaxOut2UB, enableAbsInput>(rowOutput, groupOutput, groupN);
}
#endif

template <typename T>
__aicore__ inline void SetFixPipeAddr(const LocalTensor<T>& eleWiseData, uint16_t c0ChStride)
{
    SetFixPipeAddrImpl(eleWiseData, c0ChStride);
}
// L0C -> L1 for v220
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckFixpipeTensor<T, U, config>(dst, src, intriParams, "Fixpipe");
#endif

    if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1)) {
        FixpipeL0C2L1Impl<PrimT<T>, PrimT<U>, config>(
            (__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
    } else if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB)) {
        FixpipeL0C2UBImpl<PrimT<T>, PrimT<U>, config>(
            (__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
    }
}

// L0C->L1 deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config, typename S,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsV220& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckFixpipeTensor<T, U, config, S>(dst, src, cbufWorkspace, intriParams, "Fixpipe");
#endif
    if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1)) {
        FixpipeL0C2L1Impl<PrimT<T>, PrimT<U>, config>(
            (__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(),
            (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
    } else if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB)) {
        FixpipeL0C2UBImpl<PrimT<T>, PrimT<U>, config>(
            (__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(),
            (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
    }
}

// L0C->GM
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams)
{
#ifdef ASCENDC_CPU_DEBUG
    bool isUsedProcessLock = false;
    if (g_isAtomic == true) {
        ProcessLock::GetProcessLock()->Write();
        isUsedProcessLock = true;
    }
#endif // ASCENDC_CPU_DEBUG
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckFixpipeTensor<T, U, config>(dst, src, intriParams, "Fixpipe");
#endif
    FixpipeL0C2GMImpl<PrimT<T>, PrimT<U>, config>(
        (__gm__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
#ifdef ASCENDC_CPU_DEBUG
    if (isUsedProcessLock == true) {
        isUsedProcessLock = false;
        ProcessLock::GetProcessLock()->Unlock();
    }
#endif // ASCENDC_CPU_DEBUG
}

// L0C->GM deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config, typename S,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsV220& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckFixpipeTensor<T, U, config, S>(dst, src, cbufWorkspace, intriParams, "Fixpipe");
#endif
    FixpipeL0C2GMImpl<PrimT<T>, PrimT<U>, config>(
        (__gm__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(),
        (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
}

#if defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
// L0C->L1/UB
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    using DstPrimType = PrimT<T>;
    using SrcPrimType = PrimT<U>;
    CheckTensorPos(src, Hardware::L0C, "src", "CO1", "Fixpipe");
    ASCENDC_CHECK_TPOSITION(
        (GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1) ||
            (GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB),
        "dst", "L1 Buffer(C1)/UB(VECIN/VECOUT/VECCALC)", "Fixpipe",
        GetPositionDisplay(static_cast<TPosition>(dst.GetPosition())));
    if constexpr (config.isToUB) {
        FixpipeL0C2UBImpl<DstPrimType, SrcPrimType, config>(
            (__ubuf__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(), intriParams);
    } else {
        FixpipeL0C2L1Impl<DstPrimType, SrcPrimType, config>(
            (__cbuf__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(), intriParams);
    }
}

// L0C->L1/UB deq tensor quant
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    using DstPrimType = PrimT<T>;
    using SrcPrimType = PrimT<U>;
    CheckTensorPos(src, Hardware::L0C, "src", "CO1", "Fixpipe");
    ASCENDC_CHECK_TPOSITION(
        (GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1) ||
            (GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB),
        "dst", "L1 Buffer(C1)/UB(VECIN/VECOUT/VECCALC)", "Fixpipe",
        GetPositionDisplay(static_cast<TPosition>(dst.GetPosition())));
    CheckTensorPos(cbufWorkspace, Hardware::L1, "cbufWorkspace", "C1", "Fixpipe");
    if constexpr (config.isToUB) {
        FixpipeL0C2UBImpl<DstPrimType, SrcPrimType, config>(
            (__ubuf__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(),
            (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
    } else {
        FixpipeL0C2L1Impl<DstPrimType, SrcPrimType, config>(
            (__cbuf__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(),
            (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
    }
}

// L0C->GM
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    using DstPrimType = PrimT<T>;
    using SrcPrimType = PrimT<U>;
    CheckTensorPos(src, Hardware::L0C, "src", "CO1", "Fixpipe");
#ifdef ASCENDC_CPU_DEBUG
    bool isUsedProcessLock = false;
    if (g_isAtomic == true) {
        ProcessLock::GetProcessLock()->Write();
        isUsedProcessLock = true;
    }
#endif // ASCENDC_CPU_DEBUG

#if defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
    const uint8_t cacheMode = ExtractCacheMode(dst);
    FixpipeL0C2GMImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(), intriParams, cacheMode);
#else
    FixpipeL0C2GMImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(), intriParams);
#endif
#ifdef ASCENDC_CPU_DEBUG
    if (isUsedProcessLock == true) {
        isUsedProcessLock = false;
        ProcessLock::GetProcessLock()->Unlock();
    }
#endif // ASCENDC_CPU_DEBUG
}

// L0C->GM deq tensor quant
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    using DstPrimType = PrimT<T>;
    using SrcPrimType = PrimT<U>;
    CheckTensorPos(src, Hardware::L0C, "src", "CO1", "Fixpipe");
    CheckTensorPos(cbufWorkspace, Hardware::L1, "cbufWorkspace", "A1", "Fixpipe");
#if defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
    const uint8_t cacheMode = ExtractCacheMode(dst);
    FixpipeL0C2GMImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(),
        (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams, cacheMode);
#else
    FixpipeL0C2GMImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(),
        (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
#endif
}
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 9202)
// L0C->L1/UB
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch9202<config.format>& intriParams)
{
    using DstPrimType = PrimT<T>;
    using SrcPrimType = PrimT<U>;
    CheckTensorPos(src, Hardware::L0C, "src", "CO1", "Fixpipe");
    ASCENDC_CHECK_TPOSITION(
        (GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1) ||
            (GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB),
        "dst", "L1 Buffer(A1)", "Fixpipe",
        ConstDefiner::Instance().logicNameMap.at(static_cast<uint8_t>(dst.GetPosition())));
    if constexpr (config.isToUB) {
        FixpipeL0C2UBImpl<DstPrimType, SrcPrimType, config>(
            (__ubuf__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(), intriParams);
    } else {
        FixpipeL0C2L1Impl<DstPrimType, SrcPrimType, config>(
            (__cbuf__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(), intriParams);
    }
}

// L0C->L1/UB deq tensor quant
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace,
    const FixpipeParamsArch9202<config.format>& intriParams)
{
    using DstPrimType = PrimT<T>;
    using SrcPrimType = PrimT<U>;
    CheckTensorPos(src, Hardware::L0C, "src", "CO1", "Fixpipe");
    ASCENDC_CHECK_TPOSITION(
        (GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1) ||
            (GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB),
        "dst", "L1 Buffer(A1)", "Fixpipe",
        ConstDefiner::Instance().logicNameMap.at(static_cast<uint8_t>(dst.GetPosition())));
    CheckTensorPos(cbufWorkspace, Hardware::L1, "cbufWorkspace", "A1", "Fixpipe");
    if constexpr (config.isToUB) {
        FixpipeL0C2UBImpl<DstPrimType, SrcPrimType, config>(
            (__ubuf__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(),
            (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
    } else {
        FixpipeL0C2L1Impl<DstPrimType, SrcPrimType, config>(
            (__cbuf__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(),
            (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
    }
}

// L0C->GM
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch9202<config.format>& intriParams)
{
    using DstPrimType = PrimT<T>;
    using SrcPrimType = PrimT<U>;
    CheckTensorPos(src, Hardware::L0C, "src", "CO1", "Fixpipe");
#ifdef ASCENDC_CPU_DEBUG
    bool isUsedProcessLock = false;
    if (g_isAtomic == true) {
        ProcessLock::GetProcessLock()->Write();
        isUsedProcessLock = true;
    }
#endif // ASCENDC_CPU_DEBUG

#if defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202) || (__NPU_ARCH__ == 5102))
    const uint8_t cacheMode = ExtractCacheMode(dst);
    FixpipeL0C2GMImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(), intriParams, cacheMode);
#else
    FixpipeL0C2GMImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(), intriParams);
#endif
#ifdef ASCENDC_CPU_DEBUG
    if (isUsedProcessLock == true) {
        isUsedProcessLock = false;
        ProcessLock::GetProcessLock()->Unlock();
    }
#endif // ASCENDC_CPU_DEBUG
}

// L0C->GM deq tensor quant
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace,
    const FixpipeParamsArch9202<config.format>& intriParams)
{
    using DstPrimType = PrimT<T>;
    using SrcPrimType = PrimT<U>;
    CheckTensorPos(src, Hardware::L0C, "src", "CO1", "Fixpipe");
    CheckTensorPos(cbufWorkspace, Hardware::L1, "cbufWorkspace", "A1", "Fixpipe");
#if defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202) || (__NPU_ARCH__ == 5102))
    const uint8_t cacheMode = ExtractCacheMode(dst);
    FixpipeL0C2GMImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(),
        (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams, cacheMode);
#else
    FixpipeL0C2GMImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(),
        (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
#endif
}
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5162)
// L1->L1/UB
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV516& intriParams)
{
    using DstPrimType = PrimT<T>;
    using SrcPrimType = PrimT<U>;
    CheckTensorPos(src, Hardware::L1, "src", "CO1", "Fixpipe");
    ASCENDC_CHECK_TPOSITION(
        (GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1) ||
            (GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB),
        "dst", "A1", "Fixpipe", ConstDefiner::Instance().logicNameMap.at(static_cast<uint8_t>(dst.GetPosition())));
    if constexpr (config.isToUB) {
        FixpipeL12UbImpl<DstPrimType, SrcPrimType, config>(
            (__ubuf__ DstPrimType*)dst.GetPhyAddr(), (__cbuf__ SrcPrimType*)src.GetPhyAddr(), intriParams);
    } else {
        FixpipeL12L1Impl<DstPrimType, SrcPrimType, config>(
            (__cbuf__ DstPrimType*)dst.GetPhyAddr(), (__cbuf__ SrcPrimType*)src.GetPhyAddr(), intriParams);
    }
}

// L1->GM
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV516& intriParams)
{
    using DstPrimType = PrimT<T>;
    using SrcPrimType = PrimT<U>;
    CheckTensorPos(src, Hardware::L1, "src", "CO1", "Fixpipe");
    FixpipeL12GmImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cbuf__ SrcPrimType*)src.GetPhyAddr(), intriParams);
}
#endif

/* **************************************************************************************************
 * Pointer-input overloads for Fixpipe -- definitions.
 *
 * The *Impl backends already take raw address-space pointers, so these are a pure forwarding layer
 * and share the Tensor path's implementation of the arithmetic.  The address space of `dst` selects
 * the destination pipe:  __gm__ -> GM, __cbuf__ -> L1, __ubuf__ -> UB.
 * ************************************************************************************************* */
namespace CubePtrOverload {
// -------------------------------------------------------------------------------------------
// Address-space dispatch, and how it degenerates under ASCENDC_CPU_DEBUG.
//
// On the device build an address-space qualifier is a real, distinct type, so __gm__ / __cbuf__ /
// __ubuf__ select three different destination pipes (GM / L1 / UB) at compile time.
//
// Under ASCENDC_CPU_DEBUG the platform's own tikicpulib/include/stub_def.h defines those
// qualifiers as *empty* macros, so the three signatures are one and defining all three is a
// redefinition.  The call site collapses with them, so the destination simply does not exist in
// that build and no implementation can recover it.  One definition per group is all the build can
// express; it routes to the GM backend.  This carries no numeric consequence -- the CPU-debug
// build does not observably execute the pipe at all (the PV memory model is stubbed out).
// -------------------------------------------------------------------------------------------

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1

template <const FixpipeConfig& config, typename D, typename S, typename P>
__aicore__ inline void Fp(D* dst, S* src, const P& intriParams)
{
    FixpipeL0C2GMImpl<D, S, config>(dst, src, intriParams);
}

template <const FixpipeConfig& config, typename D, typename S, typename P>
__aicore__ inline void Fp(D* dst, S* src, uint64_t* cbufWorkspace, const P& intriParams)
{
    FixpipeL0C2GMImpl<D, S, config>(dst, src, cbufWorkspace, intriParams);
}

#else // device build: the address space selects the destination pipe

template <const FixpipeConfig& config, typename D, typename S, typename P>
__aicore__ inline void Fp(__gm__ D* dst, __cc__ S* src, const P& intriParams)
{
    FixpipeL0C2GMImpl<D, S, config>(dst, src, intriParams);
}

template <const FixpipeConfig& config, typename D, typename S, typename P>
__aicore__ inline void Fp(__cbuf__ D* dst, __cc__ S* src, const P& intriParams)
{
    FixpipeL0C2L1Impl<D, S, config>(dst, src, intriParams);
}

template <const FixpipeConfig& config, typename D, typename S, typename P>
__aicore__ inline void Fp(__ubuf__ D* dst, __cc__ S* src, const P& intriParams)
{
    FixpipeL0C2UBImpl<D, S, config>(dst, src, intriParams);
}

template <const FixpipeConfig& config, typename D, typename S, typename P>
__aicore__ inline void Fp(__gm__ D* dst, __cc__ S* src, __cbuf__ uint64_t* cbufWorkspace, const P& intriParams)
{
    FixpipeL0C2GMImpl<D, S, config>(dst, src, cbufWorkspace, intriParams);
}

template <const FixpipeConfig& config, typename D, typename S, typename P>
__aicore__ inline void Fp(__cbuf__ D* dst, __cc__ S* src, __cbuf__ uint64_t* cbufWorkspace, const P& intriParams)
{
    FixpipeL0C2L1Impl<D, S, config>(dst, src, cbufWorkspace, intriParams);
}

template <const FixpipeConfig& config, typename D, typename S, typename P>
__aicore__ inline void Fp(__ubuf__ D* dst, __cc__ S* src, __cbuf__ uint64_t* cbufWorkspace, const P& intriParams)
{
    FixpipeL0C2UBImpl<D, S, config>(dst, src, cbufWorkspace, intriParams);
}

#endif // ASCENDC_CPU_DEBUG

/* ---- SetFixPipeAddr: the eleWiseData operand is an L1 or UB address ---------- */
/*
 * SetFixPipeAddrImpl takes a LocalTensor that it never reads.  On every implementation that exists
 * -- dav_3510/...:35, dav_c100/...:59, dav_c220/..._v2_impl.h:34, dav_l300/...:34, dav_l311/...:34,
 * dav_m200/...:60, dav_m300/..._v2_impl.h:33, dav_m310/...:76 -- the body is
 * ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr") and nothing else.
 *
 * The pointer overload is still a real forwarding layer rather than a copy of that stub: it
 * re-materialises a LocalTensor over the caller's address and calls the same impl, so the two paths
 * cannot drift apart, and an arch that later starts reading the operand keeps working through the
 * pointer form too.
 *
 * The address space of the pointer says which buffer the operand lives in: __cbuf__ (L1) or
 * __ubuf__ (UB).  Only the buffer class is recoverable -- a bare pointer cannot separate A1 from B1
 * within L1, nor VECIN from VECCALC within UB.  That residue is inert here *because* the callee
 * reads neither the position nor the length: the position is set to the representative the pointer
 * does determine (A1 for L1, VECCALC for UB) and the length is left at the tensor default.
 *
 * Under ASCENDC_CPU_DEBUG the qualifiers are empty macros, so the two overloads would collapse
 * into a single signature; exactly one definition is all that build can express, and it takes the
 * UB representative.
 */
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1

template <typename D>
__aicore__ inline void FpAddr(D* eleWiseData, uint16_t c0ChStride)
{
    LocalTensor<D> eleWise(TPosition::VECCALC, (uint32_t)(uint64_t)eleWiseData, 0);
    SetFixPipeAddrImpl(eleWise, c0ChStride);
}

#else

template <typename D>
__aicore__ inline void FpAddr(__cbuf__ D* eleWiseData, uint16_t c0ChStride)
{
    LocalTensor<D> eleWise(TPosition::A1, (uint32_t)(uint64_t)eleWiseData, 0);
    SetFixPipeAddrImpl(eleWise, c0ChStride);
}

template <typename D>
__aicore__ inline void FpAddr(__ubuf__ D* eleWiseData, uint16_t c0ChStride)
{
    LocalTensor<D> eleWise(TPosition::VECCALC, (uint32_t)(uint64_t)eleWiseData, 0);
    SetFixPipeAddrImpl(eleWise, c0ChStride);
}

#endif
} // namespace CubePtrOverload

template <
    typename T, typename U, const FixpipeConfig& config, typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void Fixpipe(DstPtr dst, SrcPtr src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    CubePtrOverload::Fp<config, T, U>(dst, src, intriParams);
}

template <
    typename T, typename U, const FixpipeConfig& config, typename DstPtr, typename SrcPtr, typename WorkspacePtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, WorkspacePtr>::value, bool>::type>
__aicore__ inline void Fixpipe(
    DstPtr dst, SrcPtr src, WorkspacePtr cbufWorkspace, const FixpipeParamsArch3510<config.format>& intriParams)
{
    CubePtrOverload::Fp<config, T, U>(dst, src, cbufWorkspace, intriParams);
}

template <
    typename T, typename U, const FixpipeConfig& config, typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void Fixpipe(DstPtr dst, SrcPtr src, const FixpipeParamsV220& intriParams)
{
    CubePtrOverload::Fp<config, T, U>(dst, src, intriParams);
}

template <
    typename T, typename U, const FixpipeConfig& config, typename DstPtr, typename SrcPtr, typename WorkspacePtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, WorkspacePtr>::value, bool>::type>
__aicore__ inline void Fixpipe(DstPtr dst, SrcPtr src, WorkspacePtr cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    CubePtrOverload::Fp<config, T, U>(dst, src, cbufWorkspace, intriParams);
}

template <
    typename DstPtr, typename SrcPtr, const FixpipeConfig& config,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void Fixpipe(DstPtr dst, SrcPtr src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    CubePtrOverload::Fp<config>(dst, src, intriParams);
}

template <
    typename DstPtr, typename SrcPtr, typename WorkspacePtr, const FixpipeConfig& config,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, WorkspacePtr>::value, bool>::type>
__aicore__ inline void Fixpipe(
    DstPtr dst, SrcPtr src, WorkspacePtr cbufWorkspace, const FixpipeParamsArch3510<config.format>& intriParams)
{
    CubePtrOverload::Fp<config>(dst, src, cbufWorkspace, intriParams);
}

template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type>
__aicore__ inline void Fixpipe(DstPtr dst, SrcPtr src, const FixpipeParamsV220& intriParams)
{
    CubePtrOverload::Fp<CFG_ROW_MAJOR>(dst, src, intriParams);
}

template <
    typename DstPtr, typename SrcPtr, typename WorkspacePtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, WorkspacePtr>::value, bool>::type>
__aicore__ inline void Fixpipe(DstPtr dst, SrcPtr src, WorkspacePtr cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    CubePtrOverload::Fp<CFG_ROW_MAJOR>(dst, src, cbufWorkspace, intriParams);
}

template <
    bool setRelu, typename DstPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type>
__aicore__ inline void SetFixPipeConfig(DstPtr preData, bool isUnitFlag)
{
    if ASCEND_IS_AIC {
        uint64_t config = 0;
        if constexpr (setRelu) {
            config = config | ((uint64_t)preData >> 6); // FPC[7:0], ReluPreAddr, 64bit aligned
        } else {
            config = config | (((uint64_t)preData >> 7) << 8); // FPC[15:8], QuantPreAddr, 128bit aligned
        }
        config = config | (static_cast<uint64_t>(isUnitFlag) << 63); // FPC[63], UnitFlag
        set_fpc(config);
    }
}
/*
 * SetFixPipeConfig, two-operand form, with raw pointer pre-tensors.
 *
 * SetFixPipeConfigImpl for this form (dav_3510/kernel_operator_fixpipe_impl.h:44-53) reads exactly
 * two things from its operands -- reluPre.GetPhyAddr() and quantPre.GetPhyAddr() -- and nothing
 * else; it never reads a position or a length.  A raw pointer *is* that address, so unlike the
 * TPosition-carrying operands elsewhere in this file there is no information to lose here and
 * nothing to reconstruct: the config word below is the same expression on the same values.
 *
 * This mirrors the single-operand pointer overload directly above, which expresses its operand's
 * address the same way.
 *
 * 两个操作数都必须是裸指针（AllPtr）。若沿用「至少一个是裸指针」的约束，
 * SetFixPipeConfig(ptr, true) 在这里同样可行：bool 会被当作第二个操作数接受，随后在函数体内
 * 转成地址写入 quant 字段，而 unit-flag 位取默认值。要求两处都是指针，才能让该调用回到上面
 * 的单操作数重载。
 */
template <
    typename ReluPtr, typename QuantPtr,
    typename Std::enable_if<CubePtrOverload::AllPtr<ReluPtr, QuantPtr>::value, bool>::type>
__aicore__ inline void SetFixPipeConfig(ReluPtr reluPre, QuantPtr quantPre, bool isUnitFlag)
{
    if ASCEND_IS_AIC {
        uint64_t config = 0;
        config = config | ((uint64_t)reluPre >> 6);                  // FPC[7:0], ReluPreAddr, 64bit aligned
        config = config | (((uint64_t)quantPre >> 7) << 8);          // FPC[15:8], QuantPreAddr, 128bit aligned
        config = config | (static_cast<uint64_t>(isUnitFlag) << 63); // FPC[63], UnitFlag
        set_fpc(config);
    }
}

/*
 * SetFixPipeAddr with a raw pointer eleWise operand.  The operand lives in L1 or UB, so the
 * overloads are selected by the pointer's address space; see CubePtrOverload::FpAddr above for why
 * this is a faithful forwarding layer and not a re-implementation of the callee.
 */
template <typename DstPtr, typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type>
__aicore__ inline void SetFixPipeAddr(DstPtr eleWiseData, uint16_t c0ChStride)
{
    CubePtrOverload::FpAddr(eleWiseData, c0ChStride);
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_FIXPIPE_INTERFACE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_INTF_IMPL_H__
#endif
