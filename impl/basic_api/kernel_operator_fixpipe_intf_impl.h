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
#elif (__NPU_ARCH__ == 5102)
#include "dav_m510/kernel_operator_fixpipe_impl.h"
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

template <
    typename ReluPre, typename QuantPre,
    typename Std::enable_if<
        Internal::IsTensorOrPointerOperand<ReluPre> && Internal::IsTensorOrPointerOperand<QuantPre> &&
            (Internal::IsPointerOperand<ReluPre> || Internal::IsPointerOperand<QuantPre>),
        bool>::type>
__aicore__ inline void SetFixPipeConfig(const ReluPre& reluPre, const QuantPre& quantPre, bool isUnitFlag)
{
    using ReluType = Internal::OperandPrimT<ReluPre>;
    using QuantType = Internal::OperandPrimT<QuantPre>;
    static_assert(Std::is_same<ReluType, QuantType>::value, "SetFixPipeConfig requires identical operand types");
    ASCENDC_ASSERT(
        (GetPhyType(GetOperandPosition(reluPre)) == Hardware::FIXBUF &&
         GetPhyType(GetOperandPosition(quantPre)) == Hardware::FIXBUF),
        { KERNEL_LOG(KERNEL_ERROR, "SetFixPipeConfig inputs must be in Fixpipe Buffer(C2PIPE2GM)"); });
    if ASCEND_IS_AIC {
        uint64_t config = (uint64_t)GetUnderlyingPtr(reluPre) >> 6;
        config |= (((uint64_t)GetUnderlyingPtr(quantPre) >> 7) << 8);
        config |= static_cast<uint64_t>(isUnitFlag) << 63;
        set_fpc(config);
    }
}

template <typename PreData, bool setRelu, typename Std::enable_if<Internal::IsPointerOperand<PreData>, bool>::type>
__aicore__ inline void SetFixPipeConfig(const PreData& preData, bool isUnitFlag)
{
    const Hardware position = GetPhyType(GetOperandPosition(preData));
    ASCENDC_ASSERT((position == Hardware::FIXBUF || position == Hardware::L0C), {
        KERNEL_LOG(KERNEL_ERROR, "SetFixPipeConfig input must be in Fixpipe Buffer(C2PIPE2GM) or L0C Buffer(CO1)");
    });
    if ASCEND_IS_AIC {
        uint64_t config = 0;
        if constexpr (setRelu) {
            config = (uint64_t)GetUnderlyingPtr(preData) >> 6;
        } else {
            config = ((uint64_t)GetUnderlyingPtr(preData) >> 7) << 8;
        }
        config |= static_cast<uint64_t>(isUnitFlag) << 63;
        set_fpc(config);
    }
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 5101 || \
                              __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
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

#if defined(__NPU_ARCH__) &&                                                                         \
    (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3002 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3102 || \
     __NPU_ARCH__ == 3113 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
template <Hardware dstScope, typename Dst, typename Src, const FixpipeConfig& config>
__aicore__ inline void FixpipeV220PointerDispatch(const Dst& dst, const Src& src, const FixpipeParamsV220& intriParams)
{
    using DstType = Internal::WritableOperandPrimT<Dst>;
    using SrcType = Internal::OperandPrimT<Src>;
    static_assert(
        dstScope == Hardware::GM || dstScope == Hardware::L1 || dstScope == Hardware::UB,
        "Fixpipe dst only supports GM/L1/UB");
    if constexpr (dstScope == Hardware::GM) {
#ifdef ASCENDC_CPU_DEBUG
        bool isUsedProcessLock = false;
        if (g_isAtomic == true) {
            ProcessLock::GetProcessLock()->Write();
            isUsedProcessLock = true;
        }
#endif
        FixpipeL0C2GMImpl<DstType, SrcType, config>(
            (__gm__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src), intriParams);
#ifdef ASCENDC_CPU_DEBUG
        if (isUsedProcessLock == true) {
            ProcessLock::GetProcessLock()->Unlock();
        }
#endif
    } else if constexpr (dstScope == Hardware::L1) {
        FixpipeL0C2L1Impl<DstType, SrcType, config>(
            (__cbuf__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src), intriParams);
    } else {
        FixpipeL0C2UBImpl<DstType, SrcType, config>(
            (__ubuf__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src), intriParams);
    }
}

template <Hardware dstScope, typename Dst, typename Src, const FixpipeConfig& config, typename Workspace>
__aicore__ inline void FixpipeV220PointerDispatch(
    const Dst& dst, const Src& src, const Workspace& cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    using DstType = Internal::WritableOperandPrimT<Dst>;
    using WorkspaceType = Internal::WritableOperandPrimT<Workspace>;
    using SrcType = Internal::OperandPrimT<Src>;
    static_assert(
        dstScope == Hardware::GM || dstScope == Hardware::L1 || dstScope == Hardware::UB,
        "Fixpipe dst only supports GM/L1/UB");
    if constexpr (dstScope == Hardware::GM) {
        FixpipeL0C2GMImpl<DstType, SrcType, config>(
            (__gm__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src),
            (__cbuf__ uint64_t*)GetUnderlyingPtr(cbufWorkspace), intriParams);
    } else if constexpr (dstScope == Hardware::L1) {
        FixpipeL0C2L1Impl<DstType, SrcType, config>(
            (__cbuf__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src),
            (__cbuf__ uint64_t*)GetUnderlyingPtr(cbufWorkspace), intriParams);
    } else {
        FixpipeL0C2UBImpl<DstType, SrcType, config>(
            (__ubuf__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src),
            (__cbuf__ uint64_t*)GetUnderlyingPtr(cbufWorkspace), intriParams);
    }
}

template <
    typename Dst, typename Src, const FixpipeConfig& config,
    typename Std::enable_if<Internal::IsPointerOperand<Dst> || Internal::IsPointerOperand<Src>, bool>::type>
__aicore__ inline void Fixpipe(const Dst& dst, const Src& src, const FixpipeParamsV220& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckFixpipeParamsV220Common<Internal::OperandPrimT<Dst>, Internal::OperandPrimT<Src>, config>(
        intriParams, "Fixpipe");
#endif
    ASCENDC_ASSERT((GetPhyType(GetOperandPosition(src)) == Hardware::L0C), {
        KERNEL_LOG(KERNEL_ERROR, "Fixpipe src only supports L0C Buffer(CO1)");
    });
    if constexpr (Internal::IsGlobalTensorOperand<Dst>) {
        FixpipeV220PointerDispatch<Hardware::GM, Dst, Src, config>(dst, src, intriParams);
    } else if constexpr (Internal::IsPointerOperand<Dst>) {
        constexpr Hardware dstScope = GetPhyType(GetOperandPosition(static_cast<Dst>(nullptr)));
        FixpipeV220PointerDispatch<dstScope, Dst, Src, config>(dst, src, intriParams);
    } else {
        const Hardware dstScope = GetPhyType(GetOperandPosition(dst));
        if (dstScope == Hardware::L1) {
            FixpipeV220PointerDispatch<Hardware::L1, Dst, Src, config>(dst, src, intriParams);
        } else if (dstScope == Hardware::UB) {
            FixpipeV220PointerDispatch<Hardware::UB, Dst, Src, config>(dst, src, intriParams);
        } else {
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "Fixpipe local dst only supports L1/UB"); });
        }
    }
}

template <
    typename Dst, typename Src, const FixpipeConfig& config, typename Workspace,
    typename Std::enable_if<
        (Internal::IsPointerOperand<Dst> || Internal::IsPointerOperand<Src> || Internal::IsPointerOperand<Workspace>) &&
            Std::is_same<Internal::OperandPrimT<Workspace>, uint64_t>::value,
        bool>::type>
__aicore__ inline void Fixpipe(
    const Dst& dst, const Src& src, const Workspace& cbufWorkspace, const FixpipeParamsV220& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckFixpipeParamsV220Common<Internal::OperandPrimT<Dst>, Internal::OperandPrimT<Src>, config>(
        intriParams, "Fixpipe");
    CheckFixpipeQuantPreWithWorkspaceCommon(intriParams.quantPre, "Fixpipe");
#endif
    ASCENDC_ASSERT((GetPhyType(GetOperandPosition(src)) == Hardware::L0C), {
        KERNEL_LOG(KERNEL_ERROR, "Fixpipe src only supports L0C Buffer(CO1)");
    });
    ASCENDC_ASSERT((GetPhyType(GetOperandPosition(cbufWorkspace)) == Hardware::L1), {
        KERNEL_LOG(KERNEL_ERROR, "Fixpipe workspace only supports L1 Buffer(C1)");
    });
    if constexpr (Internal::IsGlobalTensorOperand<Dst>) {
        FixpipeV220PointerDispatch<Hardware::GM, Dst, Src, config>(dst, src, cbufWorkspace, intriParams);
    } else if constexpr (Internal::IsPointerOperand<Dst>) {
        constexpr Hardware dstScope = GetPhyType(GetOperandPosition(static_cast<Dst>(nullptr)));
        FixpipeV220PointerDispatch<dstScope, Dst, Src, config>(dst, src, cbufWorkspace, intriParams);
    } else {
        const Hardware dstScope = GetPhyType(GetOperandPosition(dst));
        if (dstScope == Hardware::L1) {
            FixpipeV220PointerDispatch<Hardware::L1, Dst, Src, config>(dst, src, cbufWorkspace, intriParams);
        } else if (dstScope == Hardware::UB) {
            FixpipeV220PointerDispatch<Hardware::UB, Dst, Src, config>(dst, src, cbufWorkspace, intriParams);
        } else {
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "Fixpipe local dst only supports L1/UB"); });
        }
    }
}
#endif

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
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

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    const uint8_t cacheMode = GetOperandCacheMode(dst);
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
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    const uint8_t cacheMode = GetOperandCacheMode(dst);
    FixpipeL0C2GMImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(),
        (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams, cacheMode);
#else
    FixpipeL0C2GMImpl<DstPrimType, SrcPrimType, config>(
        (__gm__ DstPrimType*)dst.GetPhyAddr(), (__cc__ SrcPrimType*)src.GetPhyAddr(),
        (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
#endif
}

template <Hardware dstScope, typename Dst, typename Src, const FixpipeConfig& config>
__aicore__ inline void FixpipeArch3510PointerDispatch(
    const Dst& dst, const Src& src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    using DstType = Internal::WritableOperandPrimT<Dst>;
    using SrcType = Internal::OperandPrimT<Src>;
    static_assert(
        dstScope == Hardware::GM || dstScope == Hardware::L1 || dstScope == Hardware::UB,
        "Fixpipe dst only supports GM/L1/UB");
    if constexpr (dstScope == Hardware::GM) {
        const uint8_t cacheMode = GetOperandCacheMode(dst);
        FixpipeL0C2GMImpl<DstType, SrcType, config>(
            (__gm__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src), intriParams, cacheMode);
    } else if constexpr (dstScope == Hardware::UB) {
        FixpipeL0C2UBImpl<DstType, SrcType, config>(
            (__ubuf__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src), intriParams);
    } else {
        FixpipeL0C2L1Impl<DstType, SrcType, config>(
            (__cbuf__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src), intriParams);
    }
}

template <Hardware dstScope, typename Dst, typename Src, const FixpipeConfig& config, typename Workspace>
__aicore__ inline void FixpipeArch3510PointerDispatch(
    const Dst& dst, const Src& src, const Workspace& cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    using DstType = Internal::WritableOperandPrimT<Dst>;
    using WorkspaceType = Internal::WritableOperandPrimT<Workspace>;
    using SrcType = Internal::OperandPrimT<Src>;
    static_assert(
        dstScope == Hardware::GM || dstScope == Hardware::L1 || dstScope == Hardware::UB,
        "Fixpipe dst only supports GM/L1/UB");
    if constexpr (dstScope == Hardware::GM) {
        const uint8_t cacheMode = GetOperandCacheMode(dst);
        FixpipeL0C2GMImpl<DstType, SrcType, config>(
            (__gm__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src),
            (__cbuf__ uint64_t*)GetUnderlyingPtr(cbufWorkspace), intriParams, cacheMode);
    } else if constexpr (dstScope == Hardware::UB) {
        FixpipeL0C2UBImpl<DstType, SrcType, config>(
            (__ubuf__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src),
            (__cbuf__ uint64_t*)GetUnderlyingPtr(cbufWorkspace), intriParams);
    } else {
        FixpipeL0C2L1Impl<DstType, SrcType, config>(
            (__cbuf__ DstType*)GetUnderlyingPtr(dst), (__cc__ SrcType*)GetUnderlyingPtr(src),
            (__cbuf__ uint64_t*)GetUnderlyingPtr(cbufWorkspace), intriParams);
    }
}

template <
    typename Dst, typename Src, const FixpipeConfig& config,
    typename Std::enable_if<Internal::IsPointerOperand<Dst> || Internal::IsPointerOperand<Src>, bool>::type>
__aicore__ inline void Fixpipe(const Dst& dst, const Src& src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    ASCENDC_ASSERT((GetPhyType(GetOperandPosition(src)) == Hardware::L0C), {
        KERNEL_LOG(KERNEL_ERROR, "Fixpipe src only supports L0C Buffer(CO1)");
    });
    if constexpr (Internal::IsGlobalTensorOperand<Dst>) {
        FixpipeArch3510PointerDispatch<Hardware::GM, Dst, Src, config>(dst, src, intriParams);
    } else if constexpr (Internal::IsPointerOperand<Dst>) {
        constexpr Hardware dstScope = GetPhyType(GetOperandPosition(static_cast<Dst>(nullptr)));
        if constexpr (dstScope == Hardware::GM) {
            FixpipeArch3510PointerDispatch<Hardware::GM, Dst, Src, config>(dst, src, intriParams);
        } else if constexpr (config.isToUB) {
            ASCENDC_ASSERT((dstScope == Hardware::UB), {
                KERNEL_LOG(KERNEL_ERROR, "Fixpipe dst must be UB when config.isToUB is enabled");
            });
            FixpipeArch3510PointerDispatch<Hardware::UB, Dst, Src, config>(dst, src, intriParams);
        } else {
            ASCENDC_ASSERT((dstScope == Hardware::L1), {
                KERNEL_LOG(KERNEL_ERROR, "Fixpipe dst must be L1 when config.isToUB is disabled");
            });
            FixpipeArch3510PointerDispatch<Hardware::L1, Dst, Src, config>(dst, src, intriParams);
        }
    } else if constexpr (config.isToUB) {
        ASCENDC_ASSERT((GetPhyType(GetOperandPosition(dst)) == Hardware::UB), {
            KERNEL_LOG(KERNEL_ERROR, "Fixpipe dst must be UB when config.isToUB is enabled");
        });
        FixpipeArch3510PointerDispatch<Hardware::UB, Dst, Src, config>(dst, src, intriParams);
    } else {
        ASCENDC_ASSERT((GetPhyType(GetOperandPosition(dst)) == Hardware::L1), {
            KERNEL_LOG(KERNEL_ERROR, "Fixpipe dst must be L1 when config.isToUB is disabled");
        });
        FixpipeArch3510PointerDispatch<Hardware::L1, Dst, Src, config>(dst, src, intriParams);
    }
}

template <
    typename Dst, typename Src, const FixpipeConfig& config, typename Workspace,
    typename Std::enable_if<
        (Internal::IsPointerOperand<Dst> || Internal::IsPointerOperand<Src> || Internal::IsPointerOperand<Workspace>) &&
            Std::is_same<Internal::OperandPrimT<Workspace>, uint64_t>::value,
        bool>::type>
__aicore__ inline void Fixpipe(
    const Dst& dst, const Src& src, const Workspace& cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    ASCENDC_ASSERT((GetPhyType(GetOperandPosition(src)) == Hardware::L0C), {
        KERNEL_LOG(KERNEL_ERROR, "Fixpipe src only supports L0C Buffer(CO1)");
    });
    ASCENDC_ASSERT((GetPhyType(GetOperandPosition(cbufWorkspace)) == Hardware::L1), {
        KERNEL_LOG(KERNEL_ERROR, "Fixpipe workspace only supports L1 Buffer(C1)");
    });
    if constexpr (Internal::IsGlobalTensorOperand<Dst>) {
        FixpipeArch3510PointerDispatch<Hardware::GM, Dst, Src, config>(dst, src, cbufWorkspace, intriParams);
    } else if constexpr (Internal::IsPointerOperand<Dst>) {
        constexpr Hardware dstScope = GetPhyType(GetOperandPosition(static_cast<Dst>(nullptr)));
        if constexpr (dstScope == Hardware::GM) {
            FixpipeArch3510PointerDispatch<Hardware::GM, Dst, Src, config>(dst, src, cbufWorkspace, intriParams);
        } else if constexpr (config.isToUB) {
            ASCENDC_ASSERT((dstScope == Hardware::UB), {
                KERNEL_LOG(KERNEL_ERROR, "Fixpipe dst must be UB when config.isToUB is enabled");
            });
            FixpipeArch3510PointerDispatch<Hardware::UB, Dst, Src, config>(dst, src, cbufWorkspace, intriParams);
        } else {
            ASCENDC_ASSERT((dstScope == Hardware::L1), {
                KERNEL_LOG(KERNEL_ERROR, "Fixpipe dst must be L1 when config.isToUB is disabled");
            });
            FixpipeArch3510PointerDispatch<Hardware::L1, Dst, Src, config>(dst, src, cbufWorkspace, intriParams);
        }
    } else if constexpr (config.isToUB) {
        ASCENDC_ASSERT((GetPhyType(GetOperandPosition(dst)) == Hardware::UB), {
            KERNEL_LOG(KERNEL_ERROR, "Fixpipe dst must be UB when config.isToUB is enabled");
        });
        FixpipeArch3510PointerDispatch<Hardware::UB, Dst, Src, config>(dst, src, cbufWorkspace, intriParams);
    } else {
        ASCENDC_ASSERT((GetPhyType(GetOperandPosition(dst)) == Hardware::L1), {
            KERNEL_LOG(KERNEL_ERROR, "Fixpipe dst must be L1 when config.isToUB is disabled");
        });
        FixpipeArch3510PointerDispatch<Hardware::L1, Dst, Src, config>(dst, src, cbufWorkspace, intriParams);
    }
}
#endif
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_FIXPIPE_INTERFACE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_INTF_IMPL_H__
#endif
