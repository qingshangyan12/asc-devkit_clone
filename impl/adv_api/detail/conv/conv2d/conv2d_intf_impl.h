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
 * \file conv2d_intf_impl.h
 * \brief
 */

#ifndef ADV_API_CONV_CONV2D_CONV2D_INTF_IMPL_H
#define ADV_API_CONV_CONV2D_CONV2D_INTF_IMPL_H

#include "conv2d_util.h"

namespace Conv2dApi {

template<class Config, template<typename, class>class Impl>
__aicore__ inline Conv2dIntf<Config, Impl>::Conv2dIntf()
{
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::Init(const TConv2DApiTiling *__restrict convTiling, TPipe *tPipe)
{
    ctx.convTiling = convTiling;
    ctx.pipe = GetTPipePtr();

    if ASCEND_IS_AIC_CONV {
        printf("ASCEND_IS_AIC_CONV\n");
        InitAIC();
    } else {
        printf("ASCEND_IS_AIV_CONV\n");
        InitAIV();
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetOrgFmapShape(uint64_t orgCi, uint64_t orgHi, uint64_t orgWi)
{
    ctx.orgCi = orgCi;
    ctx.orgHi = orgHi;
    ctx.orgWi = orgWi;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetOrgWeightShape(uint64_t orgCo, uint64_t orgCi, uint64_t orgKh, uint64_t orgKw)
{
    ctx.orgCo = orgCo;
    ctx.orgCi = orgCi;
    ctx.kernelH = orgKh;
    ctx.kernelW = orgKw;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetOrgOutputShape(uint64_t orgCo, uint64_t orgHo, uint64_t orgWo)
{
    ctx.orgCo = orgCo;
    ctx.orgHo = orgHo;
    ctx.orgWo = orgWo;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetSingleFmapShape(uint64_t singleCi, uint64_t singleHi, uint64_t singleWi)
{
    ctx.singleCoreCi = singleCi;
    InitKDirectionValue();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetSingleOutputShape(uint64_t singleCo, uint64_t singleHo, uint64_t singleWo)
{
    if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
        ctx.singleCoreCo = singleCo;
        ctx.singleCoreHo = singleHo;
        ctx.singleCoreWo = singleWo;
        if ASCEND_IS_AIC_CONV {
            InitCoDirectionValue();
            InitHoDirectionValue();
        }
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetSingleOutputShape(uint64_t singleCo, uint64_t singleM)
{
    if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
        ctx.singleCoreCo = singleCo;
        ctx.singleCoreM = singleM;
        if ASCEND_IS_AIC_CONV {
            InitCoDirectionValue();
            InitMDirectionValue();
        }
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetSingleBatchNum(uint64_t singleCoreBatch, uint64_t fmapOneBatchSize, uint64_t outputOneBatchSize)
{
    ctx.ddr2l1LoopBatch = singleCoreBatch;
    if ASCEND_IS_AIC_CONV {
        ctx.fmapOneBatchSize = fmapOneBatchSize;
        ctx.outputOneBatchSize = outputOneBatchSize;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetFmapStartPosition(int64_t hiStartPos, int64_t wiStartPos)
{
    if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
        if ASCEND_IS_AIC_CONV {
            ctx.hiStartPos = hiStartPos;
            ctx.wiStartPos = wiStartPos;
        }
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetFmapStartPosition(int64_t mStartPos)
{
    if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
        if ASCEND_IS_AIC_CONV {
            ctx.mStartPos = mStartPos;
        }
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetWeightStartPosition(int64_t coStartPos, int64_t ciStartPos)
{
    ctx.coStartPos = coStartPos;
    ctx.ciStartPos = ciStartPos;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetFmap(const GlobalTensor<FmapT> &fmap)
{
    if ASCEND_IS_AIC_CONV {
        ctx.fmapGm.SetGlobalBuffer(fmap.GetPhyAddr(0), fmap.GetSize());
        ctx.isFirstIterate = true;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetWeight(const GlobalTensor<WeightT> &weight)
{
    ctx.weightGm.SetGlobalBuffer(weight.GetPhyAddr(0), weight.GetSize());
    ctx.isFirstIterate = true;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetBias(const GlobalTensor<BiasT> &bias)
{
    if ASCEND_IS_AIC_CONV {
        ctx.biasGm.SetGlobalBuffer(bias.GetPhyAddr(0), bias.GetSize());
        ctx.isFirstIterate = true;
        ctx.hasBias = true;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetPreQuant(const GlobalTensor<PreQuantT> &preQuant)
{
    if ASCEND_IS_AIC_CONV {
        ctx.preQuantGm.SetGlobalBuffer(preQuant.GetPhyAddr(0), preQuant.GetSize());
        ctx.isFirstIterate = true;
        ctx.hasPreQuant = true;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetPreRelu(const GlobalTensor<PreReluT> &preRelu)
{
    if ASCEND_IS_AIC_CONV {
        ctx.preReluGm.SetGlobalBuffer(preRelu.GetPhyAddr(0), preRelu.GetSize());
        ctx.isFirstIterate = true;
        ctx.hasPreRelu = true;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetPreClipRelu(const GlobalTensor<PreClipReluT> &preClipRelu)
{
    if ASCEND_IS_AIC_CONV {
        ctx.preClipReluGm.SetGlobalBuffer(preClipRelu.GetPhyAddr(0), preClipRelu.GetSize());
        ctx.isFirstIterate = true;
        ctx.hasPreClipRelu = true;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetAntiQuant(const GlobalTensor<AntiQuantT> &antiQuant)
{
    if ASCEND_IS_AIC_CONV {
        ctx.antiQuantGm.SetGlobalBuffer(antiQuant.GetPhyAddr(0), antiQuant.GetSize());
        ctx.isFirstIterate = true;
        ctx.hasAntiQuant = true;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetEltwise(const GlobalTensor<EltwiseT> &eltwise)
{
    if ASCEND_IS_AIC_CONV {
        ctx.eltwiseGm.SetGlobalBuffer(eltwise.GetPhyAddr(0), eltwise.GetSize());
        ctx.isFirstIterate = true;
        ctx.hasEltwise = true;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetPostQuant(const GlobalTensor<PostQuantT> &postQuant)
{
    if ASCEND_IS_AIC_CONV {
        ctx.postQuantGm.SetGlobalBuffer(postQuant.GetPhyAddr(0), postQuant.GetSize());
        ctx.isFirstIterate = true;
        ctx.hasPostQuant = true;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetPostRelu(const GlobalTensor<PostReluT> &postRelu)
{
    if ASCEND_IS_AIC_CONV {
        ctx.postReluGm.SetGlobalBuffer(postRelu.GetPhyAddr(0), postRelu.GetSize());
        ctx.isFirstIterate = true;
        ctx.hasPostRelu = true;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetPostClipRelu(const GlobalTensor<PostClipReluT> &postClipRelu)
{
    if ASCEND_IS_AIC_CONV {
        ctx.postClipReluGm.SetGlobalBuffer(postClipRelu.GetPhyAddr(0), postClipRelu.GetSize());
        ctx.isFirstIterate = true;
        ctx.hasPostClipRelu = true;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetPadding(const GlobalTensor<PaddingT> &padding)
{
    if ASCEND_IS_AIC_CONV {
        ctx.paddingGm.SetGlobalBuffer(padding.GetPhyAddr(0), padding.GetSize());
        ctx.isFirstIterate = true;
        ctx.hasPadding = true;
    }
}

template<class Config, template<typename, class>class Impl>
template <bool sync>
__aicore__ inline bool Conv2dIntf<Config, Impl>::Iterate(bool enPartialSum)
{
    if ASCEND_IS_AIC_CONV {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            return IterateImplMMode(enPartialSum);
        } else {
            return IterateImplHWMode(enPartialSum);
        }
    }
    return false;
}

template<class Config, template<typename, class>class Impl>
template <bool sync>
__aicore__ inline void Conv2dIntf<Config, Impl>::GetTensorC(const GlobalTensor<OutputT> &output, bool enSequentialWrite)
{
    if ASCEND_IS_AIC_CONV {
        copyOutputIns.CopyOut(ctx, output);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
        ctx.queueOutputL1.FreeTensor(ctx.outputL1);
        if (ctx.maxKL0Iter > 0) {
            ctx.queuePartialReadWrite0.FreeTensor(ctx.partialReadWrite0L1);
            ctx.queuePartialReadWrite1.FreeTensor(ctx.partialReadWrite1L1);
        }
#else
        ctx.queueOutputL0.FreeTensor(ctx.outputL0);
#endif
    }
}

template<class Config, template<typename, class>class Impl>
template <bool sync>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateAll(const GlobalTensor<OutputT> &output, bool enPartialSum)
{
    if ASCEND_IS_AIC_CONV {
        LoadBiasL1BeforeIterate();
        LoadFixpL1BeforeIterate();
    }

    while (Iterate(enPartialSum)) {
        GetTensorC(output);
    }

    if ASCEND_IS_AIC_CONV {
        FreeBiasL1AfterIterate();
        FreeFixpL1AfterIterate();
        ctx.isFirstIterate = true;
    }

    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::End()
{
    if ASCEND_IS_AIC_CONV {
        ctx.queueFmapL1.FreeAllEvent();
        ctx.queueWeightL1.FreeAllEvent();
        ctx.queueBiasL1.FreeAllEvent();
        ctx.queuePreQuantL1.FreeAllEvent();
        ctx.queuePreReluL1.FreeAllEvent();
        ctx.queuePreClipReluL1.FreeAllEvent();
        ctx.queueAntiQuantL1.FreeAllEvent();
        ctx.queueEltwiseL1.FreeAllEvent();
        ctx.queuePostQuantL1.FreeAllEvent();
        ctx.queuePostReluL1.FreeAllEvent();
        ctx.queuePostClipReluL1.FreeAllEvent();
        ctx.queuePaddingL1.FreeAllEvent();
        ctx.queueBiasBT.FreeAllEvent();
        ctx.queuePreQuantFB.FreeAllEvent();
        ctx.queuePreReluFB.FreeAllEvent();
        ctx.queuePreClipReluFB.FreeAllEvent();
        ctx.queueAntiQuantFB.FreeAllEvent();
        ctx.queuePostQuantFB.FreeAllEvent();
        ctx.queuePostReluFB.FreeAllEvent();
        ctx.queuePostClipReluFB.FreeAllEvent();
        ctx.queuePaddingPT.FreeAllEvent();
        ctx.queueOutputL1.FreeAllEvent();
        ctx.queueOutputL0.FreeAllEvent();
        ctx.queuePartialReadWrite0.FreeAllEvent();
        ctx.queuePartialReadWrite1.FreeAllEvent();
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitAIC()
{
    ctx.singleCoreCi = ctx.convTiling->singleCoreCi;
    ctx.singleCoreCo = ctx.convTiling->singleCoreCo;
    ctx.orgCi = ctx.convTiling->orgCi;
    ctx.orgCo = ctx.convTiling->orgCo;
    ctx.orgHi = ctx.convTiling->orgHi;
    ctx.orgWi = ctx.convTiling->orgWi;
    ctx.orgHo = ctx.convTiling->orgHo;
    ctx.orgWo = ctx.convTiling->orgWo;
    ctx.kernelH = ctx.convTiling->kernelH;
    ctx.kernelW = ctx.convTiling->kernelW;
    ctx.dilatedKernelH = 1 + (ctx.kernelH - 1) * ctx.convTiling->dilationH;
    ctx.dilatedKernelW = 1 + (ctx.kernelW - 1) * ctx.convTiling->dilationW;

    uint64_t alignCinKhKwA = AlignB(ctx.singleCoreCi, k0Fmap) * ctx.convTiling->kernelHxkernelW;
    uint64_t alignCinKhKwB;
    if constexpr (sizeof(WeightT) == S8SIZE && sizeof(FmapT) == S16SIZE) {
        alignCinKhKwB = AlignB(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, k0Weight); // 16-8 compact
    } else {
        alignCinKhKwB = AlignB(ctx.singleCoreCi, k0Weight) * ctx.convTiling->kernelHxkernelW; // not compact
    }
    ctx.kAL1fullload = alignCinKhKwA <= ctx.convTiling->kAL1;
    ctx.kBL1fullload = alignCinKhKwB <= ctx.convTiling->kBL1;

    ctx.kL0FullLoadAl0PingPongFlag = 0;
    ctx.kL0FullLoadBl0PingPongFlag = 0;

    ctx.hasBias = ctx.convTiling->hasBias;
    ctx.hasPreQuant = ctx.convTiling->hasPreQuant;
    ctx.hasPreRelu = ctx.convTiling->hasPreRelu;
    ctx.hasPreClipRelu = ctx.convTiling->hasPreClipRelu;
    ctx.hasAntiQuant = ctx.convTiling->hasAntiQuant;
    ctx.hasEltwise = ctx.convTiling->hasEltwise;
    ctx.hasPostQuant = ctx.convTiling->hasPostQuant;
    ctx.hasPostRelu = ctx.convTiling->hasPostRelu;
    ctx.hasPostClipRelu = ctx.convTiling->hasPostClipRelu;
    ctx.hasPadding = ctx.convTiling->hasPadding;

    ctx.hasVectorPreQuant = ctx.convTiling->hasPreQuant &&
                            IsVectorMode(static_cast<ConvPreQuantMode>(ctx.convTiling->preQuantMode));
    ctx.hasVectorPreRelu = ctx.convTiling->hasPreRelu &&
                           IsVectorMode(static_cast<ConvPreReluMode>(ctx.convTiling->preReluMode));
    ctx.hasVectorPreClipRelu = ctx.convTiling->hasPreClipRelu &&
                               IsVectorMode(static_cast<ConvPreClipReluMode>(ctx.convTiling->preClipReluMode));
    ctx.hasVectorAntiQuant = ctx.convTiling->hasAntiQuant &&
                             IsVectorMode(static_cast<ConvAntiQuantMode>(ctx.convTiling->antiQuantMode));
    ctx.hasVectorPostQuant = ctx.convTiling->hasPostQuant &&
                             IsVectorMode(static_cast<ConvPostQuantMode>(ctx.convTiling->postQuantMode));
    ctx.hasVectorPostRelu = ctx.convTiling->hasPostRelu &&
                            IsVectorMode(static_cast<ConvPostReluMode>(ctx.convTiling->postReluMode));
    ctx.hasVectorPostClipRelu = ctx.convTiling->hasPostClipRelu &&
                                IsVectorMode(static_cast<ConvPostClipReluMode>(ctx.convTiling->postClipReluMode));
    ctx.hasVectorPadding = ctx.convTiling->hasPadding &&
                           IsVectorMode(static_cast<ConvPaddingMode>(ctx.convTiling->paddingMode));
    ctx.hasVectorFixp = ctx.hasVectorPreQuant || ctx.hasVectorPreRelu || ctx.hasVectorPreClipRelu ||
                        ctx.hasVectorAntiQuant || ctx.hasEltwise ||
                        ctx.hasVectorPostQuant || ctx.hasVectorPostRelu || ctx.hasVectorPostClipRelu ||
                        ctx.hasVectorPadding;
    printf(
        "hasBias: %d, hasPreQuant: %d, hasPreRelu: %d, hasPreClipRelu: %d, hasAntiQuant: %d, "
        "hasEltwise: %d, eltwiseBrcEn: %d, hasPostQuant: %d, hasPostRelu: %d, hasPostClipRelu: %d, "
        "hasPadding: %d, hasVectorPreQuant: %d, hasVectorPreRelu: %d, hasVectorPreClipRelu: %d, "
        "hasVectorAntiQuant: %d, hasVectorPostQuant: %d, hasVectorPostRelu: %d, hasVectorPostClipRelu: %d, "
        "hasVectorPadding: %d, hasVectorFixp: %d\n",
        ctx.hasBias, ctx.hasPreQuant, ctx.hasPreRelu, ctx.hasPreClipRelu, ctx.hasAntiQuant,
        ctx.hasEltwise, ctx.convTiling->eltwiseBrcEn, ctx.hasPostQuant, ctx.hasPostRelu, ctx.hasPostClipRelu,
        ctx.hasPadding, ctx.hasVectorPreQuant, ctx.hasVectorPreRelu, ctx.hasVectorPreClipRelu,
        ctx.hasVectorAntiQuant, ctx.hasVectorPostQuant, ctx.hasVectorPostRelu, ctx.hasVectorPostClipRelu,
        ctx.hasVectorPadding, ctx.hasVectorFixp);

    ctx.iter = 0;
    ctx.kIter = 0;
    ctx.mAL1UpdateFlag = true;

    if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
        InitValueMMode();
    } else {
        InitValueHWMode();
    }
    InitKDirectionValue();
    InitCoDirectionValue();
    InitBuffer();
    InitSubApiParams();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitAIV()
{
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitValueMMode()
{
    ctx.mAL1 = ctx.convTiling->hoL1;
    ctx.mL0 = ctx.convTiling->hoL0;
    ctx.singleCoreM = ctx.convTiling->singleCoreHo;
    InitMDirectionValue();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitValueHWMode()
{
    ctx.singleCoreHo = ctx.convTiling->singleCoreHo;
    ctx.singleCoreWo = ctx.convTiling->singleCoreWo;
    InitHoDirectionValue();
    InitWoDirectionValue();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitMDirectionValue()
{
    ctx.ddr2l1LoopM = CeilDIV(ctx.singleCoreM, ctx.mAL1);
    ctx.maxMAL1Iter = ctx.ddr2l1LoopM - 1;
    ctx.mAL1Tail = ctx.singleCoreM % ctx.mAL1;
    ctx.mAL1Tail = ctx.mAL1Tail == 0 ? ctx.mAL1 : ctx.mAL1Tail;
    ctx.currentMAL1 = ctx.mAL1Tail;

    ctx.l12l0LoopM = CeilDIV(ctx.mAL1, ctx.mL0);
    ctx.maxML0Iter = ctx.l12l0LoopM - 1;
    ctx.mAL0Tail = ctx.mAL1Tail % ctx.mL0;
    ctx.mAL0Tail = ctx.mAL0Tail == 0 ? ctx.mL0 : ctx.mAL0Tail;
    ctx.currentML0Align = AlignB(ctx.mAL0Tail, BLOCK_L0_M);
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitHoDirectionValue()
{
    ctx.hoAL1Tail = ctx.singleCoreHo % ctx.convTiling->hoL1;
    ctx.hoAL1Tail = ctx.hoAL1Tail == 0 ? ctx.convTiling->hoL1 : ctx.hoAL1Tail;
    ctx.currentHoL1 = ctx.hoAL1Tail;

    ctx.hoL0Tail = ctx.currentHoL1 % ctx.convTiling->hoL0;
    ctx.hoL0Tail = ctx.hoL0Tail == 0 ? ctx.convTiling->hoL0 : ctx.hoL0Tail;
    ctx.currentHoL0 = ctx.hoL0Tail;

    if constexpr (!hasHL1IterFlag) {
        ctx.ddr2l1LoopH = 1;
        ctx.maxHoL1Iter = 0;
    } else {
        ctx.ddr2l1LoopH = CeilDIV(ctx.singleCoreHo, ctx.convTiling->hoL1);
        ctx.maxHoL1Iter = ctx.ddr2l1LoopH - 1;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitWoDirectionValue()
{
    ctx.maxWoL1Iter = 0;
    ctx.woL1SmallTail = 0;
    ctx.woAL1Tail = ctx.singleCoreWo % ctx.convTiling->woL1;
    ctx.woAL1Tail = ctx.woAL1Tail == 0 ?  ctx.convTiling->woL1 : ctx.woAL1Tail;
    ctx.currentWoL1 = ctx.woAL1Tail;

    ctx.maxWoL0Iter = 0;
    ctx.woL0Tail = ctx.currentWoL1 % ctx.convTiling->woL0;
    ctx.woL0Tail = ctx.woL0Tail == 0 ? ctx.convTiling->woL0 : ctx.woL0Tail;
    ctx.currentWoL0 = ctx.woL0Tail;

    ctx.l12l0LoopW = 1;

    if constexpr (hasWL1IterFlag) {
        ctx.maxWoL1Iter = CeilDIV(ctx.singleCoreWo, ctx.convTiling->woL1) - 1;
    }

    if (hasWL0IterFlag &&
        ctx.convTiling->hoL0 > 1 &&
        ctx.woAL1Tail % BLOCK_L0_N > 0 &&
        CeilDIV(ctx.woAL1Tail, ctx.convTiling->woL0) > 1) {
        ctx.woL1SmallTail = ctx.woAL1Tail % ctx.convTiling->woL0;
        ctx.woAL1Tail = (ctx.woAL1Tail / ctx.convTiling->woL0) * ctx.convTiling->woL0;
    }

    if (ctx.woL1SmallTail > 0) {
        ctx.maxWoL1Iter += 1;
    }

    ctx.ddr2l1LoopW = ctx.maxWoL1Iter + 1;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitKDirectionValue()
{
    ctx.maxKAL1Iter = CeilDIV(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW,
                              ctx.convTiling->kAL1) - 1;
    ctx.maxKBL1Iter = CeilDIV(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW,
                              ctx.convTiling->kBL1) - 1;
    if constexpr (formatFmap == ConvCommonApi::ConvFormat::NC1HWC0) {
        ctx.kAL1Tail = (AlignB(ctx.singleCoreCi, k0Fmap) * ctx.convTiling->kernelHxkernelW) % ctx.convTiling->kAL1;
        ctx.kAL1Tail = ctx.kAL1Tail == 0 ? ctx.convTiling->kAL1 : ctx.kAL1Tail;
    } else {
        ctx.kAL1Tail = (ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW) % ctx.convTiling->kAL1;
        ctx.kAL1Tail = ctx.kAL1Tail == 0 ? ctx.convTiling->kAL1 : ctx.kAL1Tail;
    }
    if constexpr (formatWeight == ConvCommonApi::ConvFormat::FRACTAL_Z) {
        if constexpr (sizeof(WeightT) == S8SIZE && sizeof(FmapT) == S16SIZE) { // compact
            ctx.kBL1Tail = (AlignB(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, k0Weight)) % ctx.convTiling->kBL1;
        } else {
            ctx.kBL1Tail = (AlignB(ctx.singleCoreCi, k0Weight) * ctx.convTiling->kernelHxkernelW) % ctx.convTiling->kBL1;
        }
        ctx.kBL1Tail = ctx.kBL1Tail == 0 ? ctx.convTiling->kBL1 : ctx.kBL1Tail;
    } else {
        ctx.kBL1Tail = (ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW) % ctx.convTiling->kBL1;
        ctx.kBL1Tail = ctx.kBL1Tail == 0 ? ctx.convTiling->kBL1 : ctx.kBL1Tail;
    }

    ctx.ddr2l0LoopK = CeilDIV(AlignB(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, k0),
                              ctx.convTiling->kL0);
    ctx.maxKL0Iter = ctx.ddr2l0LoopK - 1;
    ctx.kL0Tail = (AlignB(ctx.singleCoreCi, k0Fmap) * ctx.convTiling->kernelHxkernelW) % ctx.convTiling->kL0;
    ctx.kL0Tail = ctx.kL0Tail == 0 ? ctx.convTiling->kL0 : ctx.kL0Tail;

    ctx.multiKAL1 = CeilDIV(ctx.convTiling->kAL1, ctx.convTiling->kL0);
    ctx.multiKBL1 = CeilDIV(ctx.convTiling->kBL1, ctx.convTiling->kL0);

    if constexpr (kPreLoadFlag) {
        ctx.lastLoopKAL1StartPos = ctx.ddr2l0LoopK - ctx.multiKAL1;
        ctx.lastLoopKBL1StartPos = ctx.ddr2l0LoopK - ctx.multiKBL1;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitCoDirectionValue()
{
    if constexpr (!hasNL1IterFlag) {
        ctx.ddr2l1LoopN = 1;
        ctx.maxNBL1Iter = 0;
    } else {
        ctx.ddr2l1LoopN = CeilDIV(ctx.singleCoreCo, ctx.convTiling->nBL1);
        ctx.maxNBL1Iter = ctx.ddr2l1LoopN - 1;
    }
    ctx.nBL1Tail = AlignB(ctx.singleCoreCo, k0Output) % ctx.convTiling->nBL1;
    ctx.nBL1Tail = ctx.nBL1Tail == 0 ? ctx.convTiling->nBL1 : ctx.nBL1Tail;
    ctx.currentNBL1 = ctx.nBL1Tail;

    if constexpr (!hasNL0IterFlag) {
        ctx.l12l0LoopN = 1;
        ctx.maxNL0Iter = 0;
    } else {
        ctx.l12l0LoopN = ctx.nBL1Iter == ctx.maxNBL1Iter ?
            CeilDIV(ctx.nBL1Tail, ctx.convTiling->nL0) : ctx.convTiling->multiNBL1;
        ctx.maxNL0Iter = ctx.l12l0LoopN - 1;
    }
    ctx.nL0Tail = ctx.nBL1Tail % ctx.convTiling->nL0;
    ctx.nL0Tail = ctx.nL0Tail == 0 ? ctx.convTiling->nL0 : ctx.nL0Tail;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitBuffer()
{
    InitBufferFmap();
    InitBufferWeight();
    InitBufferOutput();
    InitBufferBias();
    InitBufferFixp();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitBufferFmap()
{
    ctx.pipe->InitBuffer(ctx.l0aBuf, L0A_SIZE);
    ctx.wholeAL0 = ctx.l0aBuf.template Get<FmapT>();

    printf("a1SpaceSize: %ld\n", ctx.convTiling->a1SpaceSize);
    if constexpr (!fmapL1DBFlag) {
        ctx.pipe->InitBuffer(ctx.queueFmapL1, 1, ctx.convTiling->a1SpaceSize);
    } else {
        ctx.pipe->InitBuffer(ctx.queueFmapL1, DOUBLE_BUF, ctx.convTiling->a1SpaceSize);
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitBufferWeight()
{
    ctx.pipe->InitBuffer(ctx.l0bBuf, L0B_SIZE);
    ctx.wholeBL0 = ctx.l0bBuf.template Get<WeightT>();

    printf("b1SpaceSize: %ld\n", ctx.convTiling->b1SpaceSize);
    if constexpr (!bl1ByPass) {
        if constexpr (!weightL1DBFlag) {
            ctx.pipe->InitBuffer(ctx.queueWeightL1, 1, ctx.convTiling->b1SpaceSize);
        } else {
            ctx.pipe->InitBuffer(ctx.queueWeightL1, DOUBLE_BUF, ctx.convTiling->b1SpaceSize);
        }
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitBufferOutput()
{
    printf("c0SpaceSize: %ld\n", ctx.convTiling->c0SpaceSize);
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
    if constexpr (!outputl0DBFlag) {
        ctx.pipe->InitBuffer(ctx.queueOutputL1, 1, ctx.convTiling->c0SpaceSize);
    } else {
        ctx.pipe->InitBuffer(ctx.queueOutputL1, DOUBLE_BUF, ctx.convTiling->c0SpaceSize);
    }
    if (ctx.maxKL0Iter > 0) {
        const uint32_t partialSize = ctx.convTiling->c0SpaceSize * sizeOfL0c / sizeOfOutput;
        if constexpr (!outputl0DBFlag) {
            ctx.pipe->InitBuffer(ctx.queuePartialReadWrite0, 1, partialSize);
            ctx.pipe->InitBuffer(ctx.queuePartialReadWrite1, 1, partialSize);
        } else {
            ctx.pipe->InitBuffer(ctx.queuePartialReadWrite0, DOUBLE_BUF, partialSize);
            ctx.pipe->InitBuffer(ctx.queuePartialReadWrite1, DOUBLE_BUF, partialSize);
        }
    }
#else
    if constexpr (!outputl0DBFlag) {
        ctx.pipe->InitBuffer(ctx.queueOutputL0, 1, ctx.convTiling->c0SpaceSize);
    } else {
        ctx.pipe->InitBuffer(ctx.queueOutputL0, DOUBLE_BUF, ctx.convTiling->c0SpaceSize);
    }
#endif
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitBufferBias()
{
    constexpr uint8_t bufferNum = outputl0DBFlag ? DOUBLE_BUF : 1;

    if (ctx.hasBias) {
        uint64_t biasL1SpaceSize = 0;
        uint64_t biasBTSpaceSize = 0;
        biasL1SpaceSize = ctx.convTiling->biasFullLoadFlag ?
                                AlignB(ctx.singleCoreCo * sizeOfBias, BLOCK_L0_N * sizeOfBias) :
                                ctx.convTiling->nL0 * sizeOfBias;
        biasBTSpaceSize = BIAS_TABLE_SIZE / static_cast<uint64_t>(bufferNum);
        printf("biasBTSpaceSize: %ld\n", biasBTSpaceSize);
        ctx.pipe->InitBuffer(ctx.queueBiasL1, 1, AlignB(biasL1SpaceSize, C0_SIZE));
        ctx.pipe->InitBuffer(ctx.queueBiasBT, bufferNum, AlignB(biasBTSpaceSize, BIAS_TABLE_ALIGN_SIZE));
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitBufferFixp()
{
    constexpr uint8_t bufferNum = outputl0DBFlag ? DOUBLE_BUF : 1;

    if (ctx.hasVectorPreQuant) {
        uint64_t preQuantL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                           AlignB(ctx.singleCoreCo * sizeOfPreQuant, BLOCK_L0_N * sizeOfPreQuant) :
                                           ctx.convTiling->nL0 * sizeOfPreQuant;
        ctx.pipe->InitBuffer(ctx.queuePreQuantL1, 1, AlignB(preQuantL1SpaceSize, C0_SIZE));
        uint64_t preQuantFBSpaceSize = FB_PRE_QUANT_SIZE / static_cast<uint64_t>(bufferNum);
        printf("preQuantFBSpaceSize: %ld\n", preQuantFBSpaceSize);
        ctx.pipe->InitBuffer(ctx.queuePreQuantFB, bufferNum, AlignB(preQuantFBSpaceSize, FB_PRE_QUANT_ALIGN_SIZE));
    }

    if (ctx.hasVectorPreRelu) {
        uint64_t preReluL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                          AlignB(ctx.singleCoreCo * sizeOfPreRelu, BLOCK_L0_N * sizeOfPreRelu) :
                                          ctx.convTiling->nL0 * sizeOfPreRelu;
        ctx.pipe->InitBuffer(ctx.queuePreReluL1, 1, AlignB(preReluL1SpaceSize, C0_SIZE));
        uint64_t preReluFBSpaceSize = FB_PRE_RELU_SIZE / static_cast<uint64_t>(bufferNum);
        printf("preReluFBSpaceSize: %ld\n", preReluFBSpaceSize);
        ctx.pipe->InitBuffer(ctx.queuePreReluFB, bufferNum, AlignB(preReluFBSpaceSize, FB_PRE_RELU_ALIGN_SIZE));
    }

    if (ctx.hasVectorPreClipRelu) {
        uint64_t preClipReluL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                              AlignB(ctx.singleCoreCo * sizeOfPreClipRelu, BLOCK_L0_N * sizeOfPreClipRelu) :
                                              ctx.convTiling->nL0 * sizeOfPreClipRelu;
        ctx.pipe->InitBuffer(ctx.queuePreClipReluL1, 1, AlignB(preClipReluL1SpaceSize, C0_SIZE));
        uint64_t preClipReluFBSpaceSize = FB_PRE_CLIP_RELU_SIZE / static_cast<uint64_t>(bufferNum);
        printf("preClipReluFBSpaceSize: %ld\n", preClipReluFBSpaceSize);
        ctx.pipe->InitBuffer(ctx.queuePreClipReluFB, bufferNum, AlignB(preClipReluFBSpaceSize, FB_PRE_CLIP_RELU_ALIGN_SIZE));
    }

    if (ctx.hasVectorAntiQuant) {
        uint64_t antiQuantL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                            AlignB(ctx.singleCoreCo * sizeOfAntiQuant, BLOCK_L0_N * sizeOfAntiQuant) :
                                            ctx.convTiling->nL0 * sizeOfAntiQuant;
        ctx.pipe->InitBuffer(ctx.queueAntiQuantL1, 1, AlignB(antiQuantL1SpaceSize, C0_SIZE));
        uint64_t antiQuantFBSpaceSize = FB_ANTI_QUANT_SIZE / static_cast<uint64_t>(bufferNum);
        printf("antiQuantFBSpaceSize: %ld\n", antiQuantFBSpaceSize);
        ctx.pipe->InitBuffer(ctx.queueAntiQuantFB, bufferNum, AlignB(antiQuantFBSpaceSize, FB_ANTI_QUANT_ALIGN_SIZE));
    }

    if (ctx.convTiling->hasEltwise) {
        uint64_t eltwiseL1SpaceSize = 0;
        if (ctx.convTiling->eltwiseBrcEn) {
            eltwiseL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                     AlignB(ctx.singleCoreCo * sizeOfEltwise, BLOCK_L0_N * sizeOfEltwise) :
                                     ctx.convTiling->nL0 * sizeOfEltwise;
        } else {
            eltwiseL1SpaceSize = ctx.convTiling->mStep * ctx.convTiling->nL0 * sizeOfEltwise;
        }
        printf("eltwiseL1SpaceSize: %ld\n", eltwiseL1SpaceSize);
        ctx.pipe->InitBuffer(ctx.queueEltwiseL1, bufferNum, AlignB(eltwiseL1SpaceSize, C0_SIZE));
    }

    if (ctx.hasVectorPostQuant) {
        uint64_t postQuantL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                            AlignB(ctx.singleCoreCo * sizeOfPostQuant, BLOCK_L0_N * sizeOfPostQuant) :
                                            ctx.convTiling->nL0 * sizeOfPostQuant;
        ctx.pipe->InitBuffer(ctx.queuePostQuantL1, 1, AlignB(postQuantL1SpaceSize, C0_SIZE));
        uint64_t postQuantFBSpaceSize = FB_POST_QUANT_SIZE / static_cast<uint64_t>(bufferNum);
        printf("postQuantFBSpaceSize: %ld\n", postQuantFBSpaceSize);
        ctx.pipe->InitBuffer(ctx.queuePostQuantFB, bufferNum, AlignB(postQuantFBSpaceSize, FB_POST_QUANT_ALIGN_SIZE));
    }

    if (ctx.hasVectorPostRelu) {
        uint64_t postReluL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                           AlignB(ctx.singleCoreCo * sizeOfPostRelu, BLOCK_L0_N * sizeOfPostRelu) :
                                           ctx.convTiling->nL0 * sizeOfPostRelu;
        ctx.pipe->InitBuffer(ctx.queuePostReluL1, 1, AlignB(postReluL1SpaceSize, C0_SIZE));
        uint64_t postReluFBSpaceSize = FB_POST_RELU_SIZE / static_cast<uint64_t>(bufferNum);
        printf("postReluFBSpaceSize: %ld\n", postReluFBSpaceSize);
        ctx.pipe->InitBuffer(ctx.queuePostReluFB, bufferNum, AlignB(postReluFBSpaceSize, FB_POST_RELU_ALIGN_SIZE));
    }

    if (ctx.hasVectorPostClipRelu) {
        uint64_t postClipReluL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                              AlignB(ctx.singleCoreCo * sizeOfPostClipRelu, BLOCK_L0_N * sizeOfPostClipRelu) :
                                              ctx.convTiling->nL0 * sizeOfPostClipRelu;
        ctx.pipe->InitBuffer(ctx.queuePostClipReluL1, 1, AlignB(postClipReluL1SpaceSize, C0_SIZE));
        uint64_t postClipReluFBSpaceSize = FB_POST_CLIP_RELU_SIZE / static_cast<uint64_t>(bufferNum);
        printf("postClipReluFBSpaceSize: %ld\n", postClipReluFBSpaceSize);
        ctx.pipe->InitBuffer(ctx.queuePostClipReluFB, bufferNum, AlignB(postClipReluFBSpaceSize, FB_POST_CLIP_RELU_ALIGN_SIZE));
    }

    if (ctx.hasVectorPadding) {
        uint64_t paddingL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                      AlignB(ctx.singleCoreCo * sizeOfPadding, BLOCK_L0_N * sizeOfPadding) :
                                      ctx.convTiling->nL0 * sizeOfPadding;
        ctx.pipe->InitBuffer(ctx.queuePaddingL1, 1, AlignB(paddingL1SpaceSize, C0_SIZE));
        uint64_t paddingPTSpaceSize = PADDING_TABLE_SIZE / static_cast<uint64_t>(bufferNum);
        printf("paddingPTSpaceSize: %ld\n", paddingPTSpaceSize);
        ctx.pipe->InitBuffer(ctx.queuePaddingPT, bufferNum, AlignB(paddingPTSpaceSize, PADDING_TABLE_ALIGN_SIZE));
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::InitSubApiParams()
{
    loadWeightL1Ins.SetParams(ctx);
    loadFmapl1Ins.SetParams(ctx);
    loadFmapL0Ins.SetParams(ctx);
    loadWeightL0Ins.SetParams(ctx);
    loadBiasL1Ins.SetParams(ctx);
    loadBiasNonBrcL1Ins.SetParams(ctx);
    loadPreQuantL1Ins.SetParams(ctx);
    loadPreReluL1Ins.SetParams(ctx);
    loadPreClipReluL1Ins.SetParams(ctx);
    loadAntiQuantL1Ins.SetParams(ctx);
    loadEltwiseL1Ins.SetParams(ctx);
    loadEltwiseNonBrcL1Ins.SetParams(ctx);
    loadPostQuantL1Ins.SetParams(ctx);
    loadPostReluL1Ins.SetParams(ctx);
    loadPostClipReluL1Ins.SetParams(ctx);
    loadPaddingL1Ins.SetParams(ctx);
    loadBiasBTIns.SetParams(ctx);
    loadPreQuantFBIns.SetParams(ctx);
    loadPreReluFBIns.SetParams(ctx);
    loadPreClipReluFBIns.SetParams(ctx);
    loadAntiQuantFBIns.SetParams(ctx);
    loadPostQuantFBIns.SetParams(ctx);
    loadPostReluFBIns.SetParams(ctx);
    loadPostClipReluFBIns.SetParams(ctx);
    loadPaddingPTIns.SetParams(ctx);
    madIns.SetParams(ctx);
    copyOutputIns.SetParams(ctx);
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadBiasL1BeforeIterate()
{
    if (ctx.convTiling->biasFullLoadFlag && ctx.hasBias) {
        ctx.biasL1 = ctx.queueBiasL1.template AllocTensor<BiasT>();
        loadBiasL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.biasL1, ctx.biasGm, ctx.singleCoreCo, 0);
        ctx.queueBiasL1.EnQue(ctx.biasL1);
        ctx.biasL1 = ctx.queueBiasL1.template DeQue<BiasT>();
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadFixpL1BeforeIterate()
{
    if (ctx.convTiling->fixpParamsFullLoadFlag) {
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ != 5101 && __NPU_ARCH__ != 5161 && __NPU_ARCH__ != 5165 && __NPU_ARCH__ != 5163)
        if (ctx.hasVectorFixp) {
            event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
            SetFlag<HardEvent::FIX_MTE2>(eventId);
            WaitFlag<HardEvent::FIX_MTE2>(eventId);
        }
#endif

        if (ctx.hasVectorPreQuant) {
            ctx.preQuantL1 = ctx.queuePreQuantL1.template AllocTensor<PreQuantT>();
            loadPreQuantL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.preQuantL1, ctx.preQuantGm, ctx.singleCoreCo, 0);
            ctx.queuePreQuantL1.EnQue(ctx.preQuantL1);
            ctx.preQuantL1 = ctx.queuePreQuantL1.template DeQue<PreQuantT>();
        }

        if (ctx.hasVectorPreRelu) {
            ctx.preReluL1 = ctx.queuePreReluL1.template AllocTensor<PreReluT>();
            loadPreReluL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.preReluL1, ctx.preReluGm, ctx.singleCoreCo, 0);
            ctx.queuePreReluL1.EnQue(ctx.preReluL1);
            ctx.preReluL1 = ctx.queuePreReluL1.template DeQue<PreReluT>();
        }

        if (ctx.hasVectorPreClipRelu) {
            ctx.preClipReluL1 = ctx.queuePreClipReluL1.template AllocTensor<PreClipReluT>();
            loadPreClipReluL1Ins.LoadChannelWiseL1FullLoad(
                ctx, ctx.preClipReluL1, ctx.preClipReluGm, ctx.singleCoreCo, 0);
            ctx.queuePreClipReluL1.EnQue(ctx.preClipReluL1);
            ctx.preClipReluL1 = ctx.queuePreClipReluL1.template DeQue<PreClipReluT>();
        }

        if (ctx.hasVectorAntiQuant) {
            ctx.antiQuantL1 = ctx.queueAntiQuantL1.template AllocTensor<AntiQuantT>();
            loadAntiQuantL1Ins.LoadChannelWiseL1FullLoad(
                ctx, ctx.antiQuantL1, ctx.antiQuantGm, ctx.singleCoreCo, 0);
            ctx.queueAntiQuantL1.EnQue(ctx.antiQuantL1);
            ctx.antiQuantL1 = ctx.queueAntiQuantL1.template DeQue<AntiQuantT>();
        }

        if (ctx.hasEltwise && ctx.convTiling->eltwiseBrcEn) {
            ctx.eltwiseL1 = ctx.queueEltwiseL1.template AllocTensor<EltwiseT>();
            loadEltwiseL1Ins.LoadChannelWiseL1FullLoad(ctx,
                                                        ctx.eltwiseL1,
                                                        ctx.eltwiseGm,
                                                        ctx.singleCoreCo,
                                                        0);
            ctx.queueEltwiseL1.EnQue(ctx.eltwiseL1);
            ctx.eltwiseL1 = ctx.queueEltwiseL1.template DeQue<EltwiseT>();
        }

        if (ctx.hasVectorPostQuant) {
            ctx.postQuantL1 = ctx.queuePostQuantL1.template AllocTensor<PostQuantT>();
            loadPostQuantL1Ins.LoadChannelWiseL1FullLoad(
                ctx, ctx.postQuantL1, ctx.postQuantGm, ctx.singleCoreCo, 0);
            ctx.queuePostQuantL1.EnQue(ctx.postQuantL1);
            ctx.postQuantL1 = ctx.queuePostQuantL1.template DeQue<PostQuantT>();
        }

        if (ctx.hasVectorPostRelu) {
            ctx.postReluL1 = ctx.queuePostReluL1.template AllocTensor<PostReluT>();
            loadPostReluL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.postReluL1, ctx.postReluGm, ctx.singleCoreCo, 0);
            ctx.queuePostReluL1.EnQue(ctx.postReluL1);
            ctx.postReluL1 = ctx.queuePostReluL1.template DeQue<PostReluT>();
        }

        if (ctx.hasVectorPostClipRelu) {
            ctx.postClipReluL1 = ctx.queuePostClipReluL1.template AllocTensor<PostClipReluT>();
            loadPostClipReluL1Ins.LoadChannelWiseL1FullLoad(
                ctx, ctx.postClipReluL1, ctx.postClipReluGm, ctx.singleCoreCo, 0);
            ctx.queuePostClipReluL1.EnQue(ctx.postClipReluL1);
            ctx.postClipReluL1 = ctx.queuePostClipReluL1.template DeQue<PostClipReluT>();
        }

        if (ctx.hasVectorPadding) {
            ctx.paddingL1 = ctx.queuePaddingL1.template AllocTensor<PaddingT>();
            loadPaddingL1Ins.LoadChannelWiseL1FullLoad(
                ctx, ctx.paddingL1, ctx.paddingGm, ctx.singleCoreCo, 0);
            ctx.queuePaddingL1.EnQue(ctx.paddingL1);
            ctx.paddingL1 = ctx.queuePaddingL1.template DeQue<PaddingT>();
        }

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ != 5101 && __NPU_ARCH__ != 5161 && __NPU_ARCH__ != 5165 && __NPU_ARCH__ != 5163)
        if (ctx.hasVectorFixp) {
            event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::MTE2_FIX));
            SetFlag<HardEvent::MTE2_FIX>(eventId);
            WaitFlag<HardEvent::MTE2_FIX>(eventId);
        }
#endif
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::FreeBiasL1AfterIterate()
{
    if (ctx.convTiling->biasFullLoadFlag && ctx.hasBias) {
        ctx.queueBiasL1.FreeTensor(ctx.biasL1);
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::FreeFixpL1AfterIterate()
{
    if (ctx.convTiling->fixpParamsFullLoadFlag) {
        if (ctx.hasVectorPreQuant) {
            ctx.queuePreQuantL1.FreeTensor(ctx.preQuantL1);
        }
        if (ctx.hasVectorPreRelu) {
            ctx.queuePreReluL1.FreeTensor(ctx.preReluL1);
        }
        if (ctx.hasVectorPreClipRelu) {
            ctx.queuePreClipReluL1.FreeTensor(ctx.preClipReluL1);
        }
        if (ctx.hasVectorAntiQuant) {
            ctx.queueAntiQuantL1.FreeTensor(ctx.antiQuantL1);
        }
        if (ctx.hasEltwise && ctx.convTiling->eltwiseBrcEn) {
            ctx.queueEltwiseL1.FreeTensor(ctx.eltwiseL1);
        }
        if (ctx.hasVectorPostQuant) {
            ctx.queuePostQuantL1.FreeTensor(ctx.postQuantL1);
        }
        if (ctx.hasVectorPostRelu) {
            ctx.queuePostReluL1.FreeTensor(ctx.postReluL1);
        }
        if (ctx.hasVectorPostClipRelu) {
            ctx.queuePostClipReluL1.FreeTensor(ctx.postClipReluL1);
        }
        if (ctx.hasVectorPadding) {
            ctx.queuePaddingL1.FreeTensor(ctx.paddingL1);
        }
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::CalcMDirectionVar()
{
    if constexpr (hasML1IterFlag) {
        ctx.currentMAL1 = ctx.mAL1Iter == ctx.maxMAL1Iter ? ctx.mAL1Tail :
                                                            ctx.mAL1;
        ctx.mAL1UpdateFlag = true; // zj: can be optimized
    }

    if constexpr (hasML0IterFlag) {
        ctx.l12l0LoopM = CeilDIV(ctx.currentMAL1, ctx.mL0);
        ctx.maxML0Iter = ctx.l12l0LoopM - 1;
        ctx.mAL0Tail = ctx.currentMAL1 % ctx.mL0;
        ctx.mAL0Tail = ctx.mAL0Tail == 0 ? ctx.mL0 : ctx.mAL0Tail;
        (void)CalcCurrentML0MMode();
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::CalcHoDirectionVar()
{
    if constexpr (hasHL1IterFlag) {
        ctx.currentHoL1 = ctx.hoAL1Iter == ctx.maxHoL1Iter ? ctx.hoAL1Tail :
                                                             ctx.convTiling->hoL1;
    }

    if constexpr (hasHL0IterFlag) {
        ctx.l12l0LoopH = CeilDIV(ctx.currentHoL1, ctx.convTiling->hoL0);
        ctx.maxHoL0Iter = ctx.l12l0LoopH - 1;
        ctx.hoL0Tail = ctx.currentHoL1 % ctx.convTiling->hoL0;
        ctx.hoL0Tail = ctx.hoL0Tail == 0 ? ctx.convTiling->hoL0 : ctx.hoL0Tail;
        ctx.currentHoL0 = ctx.hoL0Tail;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::CalcWoDirectionVar()
{
    if constexpr (hasWL1IterFlag) {
        if (ctx.woL1SmallTail > 0) {
            if (ctx.woAL1Iter == ctx.maxWoL1Iter) {
                ctx.currentWoL1 = ctx.woL1SmallTail;
            } else if (ctx.woAL1Iter == ctx.maxWoL1Iter - 1) {
                ctx.currentWoL1 = ctx.woAL1Tail;
            } else {
                ctx.currentWoL1 = ctx.convTiling->woL1;
            }
        } else {
            ctx.currentWoL1 = ctx.woAL1Iter == ctx.maxWoL1Iter ? ctx.woAL1Tail :
                                                                 ctx.convTiling->woL1;
        }
    }

    if constexpr (hasWL0IterFlag) {
        ctx.l12l0LoopW = CeilDIV(ctx.currentWoL1, ctx.convTiling->woL0);
        ctx.maxWoL0Iter = ctx.l12l0LoopW - 1;
        ctx.woL0Tail = ctx.currentWoL1 % ctx.convTiling->woL0;
        ctx.woL0Tail = ctx.woL0Tail == 0 ? ctx.convTiling->woL0 : ctx.woL0Tail;
        ctx.currentWoL0 = ctx.woL0Tail;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::CalcCoDirectionVar()
{
    if constexpr (hasNL1IterFlag) {
        ctx.currentNBL1 = ctx.nBL1Iter == ctx.maxNBL1Iter ? ctx.nBL1Tail :
                                                            ctx.convTiling->nBL1;
    }

    if constexpr (hasNL0IterFlag) {
        ctx.l12l0LoopN = CeilDIV(ctx.currentNBL1, ctx.convTiling->nL0);
        ctx.maxNL0Iter = ctx.l12l0LoopN - 1;
        ctx.nL0Tail = ctx.currentNBL1 % ctx.convTiling->nL0;
        ctx.nL0Tail = ctx.nL0Tail == 0 ? ctx.convTiling->nL0 : ctx.nL0Tail;
        (void)CalcCurrentNL0();
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline uint64_t Conv2dIntf<Config, Impl>::CalcCurrentNL0()
{
    bool isNL0Tail = ctx.nBL1Iter == ctx.maxNBL1Iter && ctx.nL0Iter == ctx.maxNL0Iter;
    uint64_t currentNL0 = isNL0Tail ? ctx.nL0Tail : ctx.convTiling->nL0;
    ctx.currentNL0Align = isNL0Tail ? AlignB(currentNL0, BLOCK_L0_N) : currentNL0;
    return currentNL0;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline uint64_t Conv2dIntf<Config, Impl>::CalcCurrentML0MMode()
{
    printf("mL0: %ld, mAL0Tail: %ld, mAL1Iter: %ld, maxMAL1Iter: %ld, mL0Iter: %ld, maxML0Iter: %ld\n", ctx.mL0,
           ctx.mAL0Tail, ctx.mAL1Iter, ctx.maxMAL1Iter, ctx.mL0Iter, ctx.maxML0Iter);
    bool isML0Tail = ctx.mAL1Iter == ctx.maxMAL1Iter && ctx.mL0Iter == ctx.maxML0Iter;
    uint64_t currentML0 = isML0Tail ? ctx.mAL0Tail : ctx.mL0;
    ctx.currentML0Align = isML0Tail ? AlignB(currentML0, BLOCK_L0_M) : currentML0;
    return currentML0;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline uint64_t Conv2dIntf<Config, Impl>::CalcCurrentML0HWMode()
{
    uint64_t currentML0 = ctx.currentHoL0 * ctx.currentWoL0;
    if (ctx.currentWoL0 == ctx.convTiling->woL0 &&
        ctx.currentHoL0 == ctx.convTiling->hoL0) {
        ctx.currentML0Align = ctx.convTiling->mStep;
    } else {
        ctx.currentML0Align = AlignB(currentML0, BLOCK_L0_N);
    }
    return currentML0;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadAL1BaseMoudle()
{
    ctx.fmapL1 = ctx.queueFmapL1.template AllocTensor<FmapT>();
    loadFmapl1Ins.LoadAL1(ctx);
    ctx.mAL1UpdateFlag = false;
    ctx.queueFmapL1.EnQue(ctx.fmapL1);
    ctx.loadAL1Flag = false;  // Only k directrion can be reloaded in LoopK

    if (ctx.hasEltwise && !ctx.convTiling->eltwiseBrcEn) {
        ctx.eltwiseL1 = ctx.queueEltwiseL1.template AllocTensor<EltwiseT>();
        loadEltwiseNonBrcL1Ins.CopyElt(ctx);
        ctx.queueEltwiseL1.EnQue(ctx.eltwiseL1);
        ctx.eltwiseL1 = ctx.queueEltwiseL1.template DeQue<EltwiseT>();
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadAL1BaseMoudle(Iters& iters)
{
    ctx.fmapL1 = ctx.queueFmapL1.template AllocTensor<FmapT>();
    if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
        loadFmapl1Ins.LoadAL1(ctx, iters.kAL1Iter, iters.mAL1Iter, iters.batchIter);
    } else {
        loadFmapl1Ins.LoadAL1(ctx, iters.kAL1Iter, iters.woAL1Iter, iters.hoAL1Iter, iters.batchIter);
    }
    ctx.queueFmapL1.EnQue(ctx.fmapL1);
    ctx.loadAL1Flag = false;  // Only k directrion can be reloaded in LoopK
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadBL1BaseMoudle()
{
    ctx.weightL1 = ctx.queueWeightL1.template AllocTensor<WeightT>();
    loadWeightL1Ins.LoadBL1(ctx);
    ctx.queueWeightL1.EnQue(ctx.weightL1);
    ctx.loadBL1Flag = false;  // Only k directrion can be reloaded in LoopK
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadBL1BaseMoudle(Iters& iters)
{
    ctx.weightL1 = ctx.queueWeightL1.template AllocTensor<WeightT>();
    loadWeightL1Ins.LoadBL1(ctx, iters.kBL1Iter, iters.nBL1Iter);
    ctx.queueWeightL1.EnQue(ctx.weightL1);
    ctx.loadBL1Flag = false;  // Only k directrion can be reloaded in LoopK
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadAL1Moudle()
{
    ctx.kAL1Iter = ctx.kIter / ctx.multiKAL1;
    LoadAL1BaseMoudle();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadBL1Moudle()
{
    ctx.kBL1Iter = ctx.kIter / ctx.multiKBL1;
    LoadBL1BaseMoudle();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::ReduceKCloseL0PingPong(bool isFirst)
{
    printf("isFirst: %d, iter: %d\n", isFirst, ctx.iter);
    if (ctx.iter >= 1) {
        WaitFlag<HardEvent::M_MTE1>(static_cast<TEventID>(0x0));
    }
    if (!((ctx.convTiling->kAL1 <= ctx.convTiling->kL0) && (!ctx.loadAL0Flag))) {
        // 16-8 compact时应<=.
        ctx.fmapL0 = ctx.wholeAL0;
        ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
        loadFmapL0Ins.LoadAL0(ctx, isFirst);
    }
    if (!((ctx.convTiling->kBL1 == ctx.convTiling->kL0) && (!ctx.loadBL0Flag))) {
        ctx.weightL0 = ctx.wholeBL0;
        ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
        loadWeightL0Ins.LoadBL0(ctx, isFirst);
    }
    SetFlag<HardEvent::MTE1_M>(static_cast<TEventID>(0x0));
    WaitFlag<HardEvent::MTE1_M>(static_cast<TEventID>(0x0));
    madIns.Mad(ctx, 0);
    SetFlag<HardEvent::M_MTE1>(static_cast<TEventID>(0x0));
    ctx.kIter++;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::ReduceKOpenL0APingPong(uint16_t& al0PingPongFlag, bool isFirst)
{
    if (ctx.iter >= DOUBLE_BUF) {
        WaitFlag<HardEvent::M_MTE1>(static_cast<TEventID>(al0PingPongFlag & 0x1));
    }
    ctx.fmapL0 = ctx.wholeAL0[(al0PingPongFlag & 0x1) * L0A_HALF_SIZE / sizeOfFmap];
    ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
    loadFmapL0Ins.LoadAL0(ctx, isFirst);

    if (!((ctx.convTiling->kBL1 == ctx.convTiling->kL0) && (!ctx.loadBL0Flag))) {
        ctx.weightL0 = ctx.wholeBL0;
        ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
        loadWeightL0Ins.LoadBL0(ctx, isFirst);
    }
    SetFlag<HardEvent::MTE1_M>(static_cast<TEventID>(al0PingPongFlag & 0x1));
    WaitFlag<HardEvent::MTE1_M>(static_cast<TEventID>(al0PingPongFlag & 0x1));
    madIns.Mad(ctx, al0PingPongFlag & 0x1);
    SetFlag<HardEvent::M_MTE1>(static_cast<TEventID>(al0PingPongFlag & 0x1));
    al0PingPongFlag += 1;
    ctx.kIter++;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::ReduceKOpenL0BPingPong(uint16_t& bl0PingPongFlag, bool isFirst)
{
    printf("bl0PingPongFlag: %u, isFirst: %d, iter: %d\n", bl0PingPongFlag, isFirst, ctx.iter);
    if (ctx.iter >= DOUBLE_BUF) {
        WaitFlag<HardEvent::M_MTE1>(static_cast<TEventID>(bl0PingPongFlag & 0x1));
    }
    if (!((ctx.convTiling->kAL1 == ctx.convTiling->kL0) && (!ctx.loadAL0Flag))) {
        ctx.fmapL0 = ctx.wholeAL0;
        ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
        loadFmapL0Ins.LoadAL0(ctx, isFirst);
    }

    ctx.weightL0 = ctx.wholeBL0[(bl0PingPongFlag & 0x1) * L0B_HALF_SIZE / sizeOfWeight];
    ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
    loadWeightL0Ins.LoadBL0(ctx, isFirst);
    SetFlag<HardEvent::MTE1_M>(static_cast<TEventID>(bl0PingPongFlag & 0x1));
    WaitFlag<HardEvent::MTE1_M>(static_cast<TEventID>(bl0PingPongFlag & 0x1));
    madIns.Mad(ctx, bl0PingPongFlag & 0x1);
    SetFlag<HardEvent::M_MTE1>(static_cast<TEventID>(bl0PingPongFlag & 0x1));
    bl0PingPongFlag += 1;
    ctx.kIter++;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::ReduceKOpenL0AL0BPingPong(uint16_t& al0PingPongFlag, bool isFirst)
{
    if (ctx.iter >= DOUBLE_BUF) {
        WaitFlag<HardEvent::M_MTE1>(static_cast<TEventID>(al0PingPongFlag & 0x1));
    }

    ctx.fmapL0 =
        ctx.wholeAL0[(al0PingPongFlag & 0x1) * L0A_HALF_SIZE / sizeOfFmap];
    ctx.weightL0 =
        ctx.wholeBL0[(al0PingPongFlag & 0x1) * L0B_HALF_SIZE / sizeOfWeight];

    ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
    loadFmapL0Ins.LoadAL0(ctx, isFirst);

    ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
    loadWeightL0Ins.LoadBL0(ctx, isFirst);

    SetFlag<HardEvent::MTE1_M>(static_cast<TEventID>(al0PingPongFlag & 0x1));
    WaitFlag<HardEvent::MTE1_M>(static_cast<TEventID>(al0PingPongFlag & 0x1));
    madIns.Mad(ctx, al0PingPongFlag & 0x1);
    SetFlag<HardEvent::M_MTE1>(static_cast<TEventID>(al0PingPongFlag & 0x1));
    al0PingPongFlag += 1;
    ctx.kIter++;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadL0Moudle(uint16_t &al0PingPongFlag, uint16_t &bl0PingPongFlag, bool isFirst)
{
    if constexpr (l0PingPong == static_cast<int32_t>(ConvL0PingPong::ALL_CLOSE)) {
        ReduceKCloseL0PingPong(isFirst);
    } else if constexpr (l0PingPong == static_cast<int32_t>(ConvL0PingPong::L0A_OPEN)) {
        ReduceKOpenL0APingPong(al0PingPongFlag, isFirst);
    } else if constexpr (l0PingPong == static_cast<int32_t>(ConvL0PingPong::L0B_OPEN)) {
        ReduceKOpenL0BPingPong(bl0PingPongFlag, isFirst);
    } else if constexpr (l0PingPong == static_cast<int32_t>(ConvL0PingPong::ALL_OPEN)) {
        ReduceKOpenL0AL0BPingPong(al0PingPongFlag, isFirst);
    }
    ctx.iter++;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::FirstIterateImplMMode()
{
    ctx.batchIter = 0;
    ctx.mAL1Iter = 0;
    ctx.nBL1Iter = 0;
    ctx.mL0Iter = 0;
    ctx.nL0Iter = 0;

    ctx.loadAL1Flag = true;
    ctx.loadBL1Flag = true;
    ctx.loadAL0Flag = true;
    ctx.loadBL0Flag = true;

    ctx.isFirstIterate = false;

    CalcMDirectionVar();
    CalcCoDirectionVar();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateL0MFirstMMode()
{
    ctx.mL0Iter++;
    if (ctx.mL0Iter == ctx.l12l0LoopM) {
        ctx.mL0Iter = 0;
    } else {
        ctx.loadBL0Flag = false;
        return true;
    }

    if constexpr (kl0FullLoadFlag) {
        ctx.kL0FullLoadAl0PingPongFlag = 0;
    }

    if constexpr (hasNL0IterFlag) {
        ctx.nL0Iter++;
        ctx.loadBL0Flag = true;
        if (ctx.nL0Iter == ctx.l12l0LoopN) {
            ctx.nL0Iter = 0;
        } else {
            return true;
        }
    }

    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateMFirstMMode()
{
    if (IterateL0MFirstMMode()) {
        return true;
    }

    if (ctx.kAL1fullload) {
        ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
    }

    ctx.mAL1Iter++;
    ctx.loadAL1Flag = true;
    if (ctx.mAL1Iter == ctx.ddr2l1LoopM) {
        ctx.mAL1Iter = 0;
        CalcMDirectionVar();
    } else {
        CalcMDirectionVar();
        return true;
    }

    ctx.batchIter++;
    if (ctx.batchIter == ctx.ddr2l1LoopBatch) {
        ctx.batchIter = 0;
    } else {
        ctx.loadAL1Flag = true;
        ctx.loadBL1Flag = false;
        return true;
    }

    if (ctx.kBL1fullload) {
        ctx.queueWeightL1.FreeTensor(ctx.weightL1);
    }

    if constexpr (hasNL1IterFlag) {
        ctx.nBL1Iter++;
        CalcCoDirectionVar();
        ctx.loadBL1Flag = true;
        if (ctx.nBL1Iter != ctx.ddr2l1LoopN) {
            return true;
        }
    }

    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateL0NFirstMMode()
{
    if constexpr (hasNL0IterFlag) {
        ctx.nL0Iter++;
        if (ctx.nL0Iter == ctx.l12l0LoopN) {
            ctx.nL0Iter = 0;
        } else {
            ctx.loadAL0Flag = false;
            return true;
        }
    }

    if constexpr (kl0FullLoadFlag) {
        ctx.kL0FullLoadBl0PingPongFlag = 0;
    }

    ctx.mL0Iter++;
    ctx.loadAL0Flag = true;
    if (ctx.mL0Iter == ctx.l12l0LoopM) {
        ctx.mL0Iter = 0;
    } else {
        return true;
    }

    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateNFirstMMode()
{
    if (IterateL0NFirstMMode()) {
        return true;
    }

    if constexpr (hasNL1IterFlag) {
        ctx.nBL1Iter++;
        ctx.loadBL1Flag = true;
        if (ctx.nBL1Iter == ctx.ddr2l1LoopN) {
            ctx.nBL1Iter = 0;
            CalcCoDirectionVar();
            
        } else {
            CalcCoDirectionVar();
            return true;
        }
    }

    if (ctx.kAL1fullload) {
        ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
    }

    ctx.mAL1Iter++;
    ctx.loadAL1Flag = true;
    if (ctx.mAL1Iter == ctx.ddr2l1LoopM) {
        ctx.mAL1Iter = 0;
        CalcMDirectionVar();
    } else {
        CalcMDirectionVar();
        return true;
    }

    ctx.batchIter++;
    if (likely(ctx.batchIter != ctx.ddr2l1LoopBatch)) {
        ctx.loadAL1Flag = true;
        ctx.loadBL1Flag = true;
        return true;
    }

    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::UpdateCommonItersMModeMFirst(Iters& iters)
{
    iters.mAL1Iter = ctx.mAL1Iter;
    iters.batchIter = ctx.batchIter;
    iters.nBL1Iter = ctx.nBL1Iter;

    iters.mAL1Iter += 1;
    if (iters.mAL1Iter <= ctx.maxMAL1Iter) {
        return false;
    }
    iters.mAL1Iter = 0;
    iters.batchIter += 1;
    if (iters.batchIter < ctx.ddr2l1LoopBatch) {
        return false;
    }
    iters.batchIter = 0;
    iters.nBL1Iter += 1;
    if (iters.nBL1Iter <= ctx.maxNBL1Iter) {
        return false;
    }
    iters.endTag = true;
    return true;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::UpdateCommonItersMModeNFirst(Iters& iters)
{
    iters.nBL1Iter = ctx.nBL1Iter;
    iters.mAL1Iter = ctx.mAL1Iter;
    iters.batchIter = ctx.batchIter;

    iters.nBL1Iter += 1;
    if (iters.nBL1Iter <= ctx.maxNBL1Iter) {
        return false;
    }
    iters.nBL1Iter = 0;
    iters.mAL1Iter += 1;
    if (iters.mAL1Iter <= ctx.maxMAL1Iter) {
        return false;
    }
    iters.mAL1Iter = 0;
    iters.batchIter += 1;
    if (iters.batchIter < ctx.ddr2l1LoopBatch) {
        return false;
    }
    iters.endTag = true;
    return true;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::FirstIterateImplHWMode()
{
    ctx.nL0Iter = 0;
    ctx.hoAL1Iter = 0;
    ctx.woAL1Iter = 0;
    ctx.nBL1Iter = 0;
    ctx.woL0Iter = 0;
    ctx.hoL0Iter = 0;
    ctx.batchIter = 0;
    ctx.loadAL1Flag = true;
    ctx.loadBL1Flag = true;
    ctx.loadAL0Flag = true;
    ctx.loadBL0Flag = true;
    ctx.isFirstIterate = false;

    CalcWoDirectionVar();
    CalcHoDirectionVar();
    CalcCoDirectionVar();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateHWL1()
{
    if constexpr (hasWL1IterFlag) {
        ctx.woAL1Iter++;
        ctx.loadAL1Flag = true;
        if (ctx.woAL1Iter == ctx.ddr2l1LoopW) {
            ctx.woAL1Iter = 0;
            CalcWoDirectionVar();
        } else {
            CalcWoDirectionVar();
            if constexpr (iterateMFirstFlag && !hasNL0IterFlag) {
                ctx.loadBL0Flag = false;
            }
            return true;
        }
    }

    if constexpr (hasHL1IterFlag) {
        ctx.hoAL1Iter++;
        if (ctx.hoAL1Iter == ctx.ddr2l1LoopH) {
            ctx.hoAL1Iter = 0;
            CalcHoDirectionVar();
        } else {
            CalcHoDirectionVar();
            if constexpr (iterateMFirstFlag && !hasNL0IterFlag) {
                ctx.loadBL0Flag = false;
            }
            return true;
        }
    }

    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateL0MFirstHWMode()
{
    if constexpr (hasWL0IterFlag) {
        ctx.woL0Iter++;
        if (ctx.woL0Iter == ctx.l12l0LoopW) {
            ctx.woL0Iter = 0;
        } else {
            ctx.loadBL0Flag = false;
            return true;
        }
    }

    if constexpr (hasHL0IterFlag) {
        ctx.hoL0Iter++;
        if (ctx.hoL0Iter == ctx.l12l0LoopH) {
            ctx.hoL0Iter = 0;
        } else {
            ctx.loadBL0Flag = false;
            return true;
        }
    }

    if constexpr (kl0FullLoadFlag) {
        ctx.kL0FullLoadAl0PingPongFlag = 0;
    }

    if constexpr (hasNL0IterFlag) {
        ctx.nL0Iter++;
        ctx.loadBL0Flag = true;
        if (ctx.nL0Iter == ctx.l12l0LoopN) {
            ctx.nL0Iter = 0;
        } else {
            return true;
        }
    }

    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateMFirstHWMode()
{
    if (IterateL0MFirstHWMode()) {
        return true;
    }

    if (ctx.kAL1fullload) {
        ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
    }

    if (IterateHWL1()) {
        return true;
    }

    ctx.batchIter++;
    if (ctx.batchIter == ctx.ddr2l1LoopBatch) {
        ctx.batchIter = 0;
    } else {
        ctx.loadAL1Flag = true;
        ctx.loadBL1Flag = false;
        return true;
    }

    if (ctx.kBL1fullload) {
        ctx.queueWeightL1.FreeTensor(ctx.weightL1);
    }

    if constexpr (hasNL1IterFlag) {
        ctx.nBL1Iter++;
        CalcCoDirectionVar();
        ctx.loadBL1Flag = true;
        if (likely(ctx.nBL1Iter != ctx.ddr2l1LoopN)) {
            return true;
        }
    }

    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateL0NFirstHWMode()
{
    if constexpr (hasNL0IterFlag) {
        ctx.nL0Iter++;
        if (ctx.nL0Iter == ctx.l12l0LoopN) {
            ctx.nL0Iter = 0;
        } else {
            ctx.loadAL0Flag = false;
            return true;
        }
    }
    if constexpr (kl0FullLoadFlag) {
        ctx.kL0FullLoadBl0PingPongFlag = 0;
    }
    if constexpr (hasWL0IterFlag) {
        ctx.woL0Iter++;
        ctx.loadAL0Flag = true;
        if (ctx.woL0Iter == ctx.l12l0LoopW) {
            ctx.woL0Iter = 0;
        } else {
            return true;
        }
    }
    if constexpr (hasHL0IterFlag) {
        ctx.hoL0Iter++;
        ctx.loadAL0Flag = true;
        if (ctx.hoL0Iter == ctx.l12l0LoopH) {
            ctx.hoL0Iter = 0;
        } else {
            return true;
        }
    }

    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateNFirstHWMode()
{
    if (IterateL0NFirstHWMode()) {
        return true;
    }

    if constexpr (hasNL1IterFlag) {
        if (ctx.kBL1fullload) {
            ctx.queueWeightL1.FreeTensor(ctx.weightL1);
        }
        ctx.nBL1Iter++;
        ctx.loadBL1Flag = true;
        if (ctx.nBL1Iter == ctx.ddr2l1LoopN) {
            ctx.nBL1Iter = 0;
            CalcCoDirectionVar();
        } else {
            CalcCoDirectionVar();
            return true;
        }
    }

    if (ctx.kAL1fullload) {
        ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
    }

    if (IterateHWL1()) {
        return true;
    }

    ctx.batchIter++;
    if (likely(ctx.batchIter != ctx.ddr2l1LoopBatch)) {
        if constexpr (!hasNL1IterFlag) {
            if (ctx.kBL1fullload) {
                ctx.queueWeightL1.FreeTensor(ctx.weightL1);
            }
        }
        ctx.loadAL1Flag = true;
        ctx.loadBL1Flag = true;
        return true;
    }
    if constexpr (!hasNL1IterFlag) {
        if (ctx.kBL1fullload) {
            ctx.queueWeightL1.FreeTensor(ctx.weightL1);
        }
    }
    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::UpdateCommonItersHWModeMFirst(Iters& iters)
{
    iters.woAL1Iter = ctx.woAL1Iter;
    iters.hoAL1Iter = ctx.hoAL1Iter;
    iters.batchIter = ctx.batchIter;
    iters.nBL1Iter = ctx.nBL1Iter;

    iters.woAL1Iter += 1;
    if (iters.woAL1Iter <= ctx.maxWoL1Iter) {
        return false;
    }
    iters.woAL1Iter = 0;
    iters.hoAL1Iter += 1;
    if (iters.hoAL1Iter <= ctx.maxHoL1Iter) {
        return false;
    }
    iters.hoAL1Iter = 0;
    iters.batchIter += 1;
    if (iters.batchIter < ctx.ddr2l1LoopBatch) {
        return false;
    }
    iters.batchIter = 0;
    iters.nBL1Iter += 1;
    if (iters.nBL1Iter <= ctx.maxNBL1Iter) {
        return false;
    }
    iters.endTag = true;
    return true;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::UpdateCommonItersHWModeNFirst(Iters& iters)
{
    iters.nBL1Iter = ctx.nBL1Iter;
    iters.woAL1Iter = ctx.woAL1Iter;
    iters.hoAL1Iter = ctx.hoAL1Iter;
    iters.batchIter = ctx.batchIter;

    iters.nBL1Iter += 1;
    if (iters.nBL1Iter <= ctx.maxNBL1Iter) {
        return false;
    }
    iters.nBL1Iter = 0;
    iters.woAL1Iter = ctx.woAL1Iter + 1;
    if (iters.woAL1Iter <= ctx.maxWoL1Iter) {
        return false;
    }
    iters.woAL1Iter = 0;
    iters.hoAL1Iter += 1;
    if (iters.hoAL1Iter <= ctx.maxHoL1Iter) {
        return false;
    }
    iters.hoAL1Iter = 0;
    iters.batchIter += 1;
    if (iters.batchIter < ctx.ddr2l1LoopBatch) {
        return false;
    }
    iters.endTag = true;
    return true;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::UpdateCommonIters(Iters& iters)
{
    if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
        iterateMFirstFlag) {
        return UpdateCommonItersMModeMFirst(iters);
    } else if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
        iterateNFirstFlag) {
        return UpdateCommonItersMModeNFirst(iters);
    } else if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
        iterateMFirstFlag) {
        return UpdateCommonItersHWModeMFirst(iters);
    } else if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
        iterateNFirstFlag) {
        return UpdateCommonItersHWModeNFirst(iters);
    }
    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::UpdateItersByFmap(Iters& iters, bool updateIterByFmapTag)
{
    if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
        iters.woAL1Iter = ctx.woAL1Iter;
        iters.hoAL1Iter = ctx.hoAL1Iter;
    } else {
        iters.mAL1Iter = ctx.mAL1Iter;
    }
    iters.batchIter = ctx.batchIter;
    ctx.kAL1Iter = ctx.kIter / ctx.multiKAL1;
    printf("kAL1Iter: %ld\n", ctx.kAL1Iter);
    iters.kAL1Iter = ctx.kAL1Iter + 1;
    if (iters.kAL1Iter <= ctx.maxKAL1Iter) {
        printf("kAL1Iter: %ld, maxKAL1Iter: %ld\n", iters.kAL1Iter, ctx.maxKAL1Iter);
        return false;
    }
    iters.kAL1Iter = 0;
    if (updateIterByFmapTag) {
        return UpdateCommonIters(iters);
    }
    if (iters.endTag) {
        return true;
    }
    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::UpdateItersByWeight(Iters& iters, bool updateIterByFmapTag)
{
    iters.nBL1Iter = ctx.nBL1Iter;
    ctx.kBL1Iter = ctx.kIter / ctx.multiKBL1;
    iters.kBL1Iter = ctx.kBL1Iter + 1;
    if (iters.kBL1Iter <= ctx.maxKBL1Iter) {
        return false;
    }
    iters.kBL1Iter = 0;
    if (!updateIterByFmapTag) {
        return UpdateCommonIters(iters);
    }
    if (iters.endTag) {
        return true;
    }
    return false;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::UpdateHoL0WoL0()
{
    ctx.currentHoL0 = ctx.hoL0Iter == ctx.maxHoL0Iter ? ctx.hoL0Tail :
                                                        ctx.convTiling->hoL0;
    ctx.currentWoL0 = ctx.woL0Iter == ctx.maxWoL0Iter ? ctx.woL0Tail :
                                                        ctx.convTiling->woL0;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::SetMNBeforeIterateK()
{
    uint64_t currentML0 = 0;
    if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
        currentML0 = CalcCurrentML0MMode();
    } else {
        currentML0 = CalcCurrentML0HWMode();
    }
    printf("currentML0: %ld\n", currentML0);

    uint64_t currentNL0 = CalcCurrentNL0();
    printf("currentNL0: %ld\n", currentNL0);

    loadFmapL0Ins.SetM(ctx, ctx.currentML0Align, currentML0);
    loadWeightL0Ins.SetN(ctx, ctx.currentNL0Align);
    loadBiasL1Ins.SetN(ctx, currentNL0);
    loadPreQuantL1Ins.SetN(ctx, currentNL0);
    loadPreReluL1Ins.SetN(ctx, currentNL0);
    loadPreClipReluL1Ins.SetN(ctx, currentNL0);
    loadAntiQuantL1Ins.SetN(ctx, currentNL0);
    loadEltwiseL1Ins.SetN(ctx, currentNL0);
    loadEltwiseNonBrcL1Ins.SetMN(ctx, currentML0, ctx.currentNL0Align);
    loadPostQuantL1Ins.SetN(ctx, currentNL0);
    loadPostReluL1Ins.SetN(ctx, currentNL0);
    loadPostClipReluL1Ins.SetN(ctx, currentNL0);
    loadPaddingL1Ins.SetN(ctx, currentNL0);
    loadBiasBTIns.SetN(ctx, ctx.currentNL0Align);
    loadPreQuantFBIns.SetN(ctx, ctx.currentNL0Align);
    loadPreReluFBIns.SetN(ctx, ctx.currentNL0Align);
    loadPreClipReluFBIns.SetN(ctx, ctx.currentNL0Align);
    loadAntiQuantFBIns.SetN(ctx, ctx.currentNL0Align);
    loadPostQuantFBIns.SetN(ctx, ctx.currentNL0Align);
    loadPostReluFBIns.SetN(ctx, ctx.currentNL0Align);
    loadPostClipReluFBIns.SetN(ctx, ctx.currentNL0Align);
    loadPaddingPTIns.SetN(ctx, ctx.currentNL0Align);
    madIns.SetMN(ctx, ctx.currentML0Align, currentML0, ctx.currentNL0Align);
    if constexpr (formatOutput == ConvCommonApi::ConvFormat::NC1HWC0) {
        copyOutputIns.SetMN(ctx, currentML0, ctx.currentNL0Align);
    } else {
        copyOutputIns.SetMN(ctx, currentML0, currentNL0);
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadBiasL1BeforeIterateK()
{
    if (ctx.hasBias) {
        if (!ctx.convTiling->biasFullLoadFlag) {
            ctx.biasL1 = ctx.queueBiasL1.template AllocTensor<BiasT>();
            loadBiasL1Ins.LoadChannelWiseL1(ctx, ctx.biasL1, ctx.biasGm);
            ctx.queueBiasL1.EnQue(ctx.biasL1);
            ctx.biasL1 = ctx.queueBiasL1.template DeQue<BiasT>();
        }
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadBiasL0BeforeIterateK()
{
    if (ctx.hasBias) {
        ctx.biasBT = ctx.queueBiasBT.template AllocTensor<L0cT>();
        loadBiasBTIns.LoadBiasBt(ctx);
        ctx.queueBiasBT.EnQue(ctx.biasBT);
        ctx.biasBT = ctx.queueBiasBT.template DeQue<L0cT>();
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadBiasBeforeIterateK()
{
    LoadBiasL1BeforeIterateK();
    LoadBiasL0BeforeIterateK();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadFixpL1BeforeIterateK()
{
    if (!ctx.convTiling->fixpParamsFullLoadFlag) {
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ != 5101 && __NPU_ARCH__ != 5161 && __NPU_ARCH__ != 5165 && __NPU_ARCH__ != 5163)
        if (ctx.hasVectorFixp) {
            event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
            SetFlag<HardEvent::FIX_MTE2>(eventId);
            WaitFlag<HardEvent::FIX_MTE2>(eventId);
        }
#endif

        if (ctx.hasVectorPreQuant) {
            ctx.preQuantL1 = ctx.queuePreQuantL1.template AllocTensor<PreQuantT>();
            loadPreQuantL1Ins.LoadChannelWiseL1(ctx, ctx.preQuantL1, ctx.preQuantGm);
            ctx.queuePreQuantL1.EnQue(ctx.preQuantL1);
            ctx.preQuantL1 = ctx.queuePreQuantL1.template DeQue<PreQuantT>();
        }

        if (ctx.hasVectorPreRelu) {
            ctx.preReluL1 = ctx.queuePreReluL1.template AllocTensor<PreReluT>();
            loadPreReluL1Ins.LoadChannelWiseL1(ctx, ctx.preReluL1, ctx.preReluGm);
            ctx.queuePreReluL1.EnQue(ctx.preReluL1);
            ctx.preReluL1 = ctx.queuePreReluL1.template DeQue<PreReluT>();
        }

        if (ctx.hasVectorPreClipRelu) {
            ctx.preClipReluL1 = ctx.queuePreClipReluL1.template AllocTensor<PreClipReluT>();
            loadPreClipReluL1Ins.LoadChannelWiseL1(ctx, ctx.preClipReluL1, ctx.preClipReluGm);
            ctx.queuePreClipReluL1.EnQue(ctx.preClipReluL1);
            ctx.preClipReluL1 = ctx.queuePreClipReluL1.template DeQue<PreClipReluT>();
        }

        if (ctx.hasEltwise && ctx.convTiling->eltwiseBrcEn) {
            ctx.eltwiseL1 = ctx.queueEltwiseL1.template AllocTensor<EltwiseT>();
            loadEltwiseL1Ins.LoadChannelWiseL1(ctx, ctx.eltwiseL1, ctx.eltwiseGm);
            ctx.queueEltwiseL1.EnQue(ctx.eltwiseL1);
            ctx.eltwiseL1 = ctx.queueEltwiseL1.template DeQue<EltwiseT>();
        }

        if (ctx.hasVectorAntiQuant) {
            ctx.antiQuantL1 = ctx.queueAntiQuantL1.template AllocTensor<AntiQuantT>();
            loadAntiQuantL1Ins.LoadChannelWiseL1(ctx, ctx.antiQuantL1, ctx.antiQuantGm);
            ctx.queueAntiQuantL1.EnQue(ctx.antiQuantL1);
            ctx.antiQuantL1 = ctx.queueAntiQuantL1.template DeQue<AntiQuantT>();
        }

        if (ctx.hasVectorPostQuant) {
            ctx.postQuantL1 = ctx.queuePostQuantL1.template AllocTensor<PostQuantT>();
            loadPostQuantL1Ins.LoadChannelWiseL1(ctx, ctx.postQuantL1, ctx.postQuantGm);
            ctx.queuePostQuantL1.EnQue(ctx.postQuantL1);
            ctx.postQuantL1 = ctx.queuePostQuantL1.template DeQue<PostQuantT>();
        }

        if (ctx.hasVectorPostRelu) {
            ctx.postReluL1 = ctx.queuePostReluL1.template AllocTensor<PostReluT>();
            loadPostReluL1Ins.LoadChannelWiseL1(ctx, ctx.postReluL1, ctx.postReluGm);
            ctx.queuePostReluL1.EnQue(ctx.postReluL1);
            ctx.postReluL1 = ctx.queuePostReluL1.template DeQue<PostReluT>();
        }

        if (ctx.hasVectorPostClipRelu) {
            ctx.postClipReluL1 = ctx.queuePostClipReluL1.template AllocTensor<PostClipReluT>();
            loadPostClipReluL1Ins.LoadChannelWiseL1(ctx, ctx.postClipReluL1, ctx.postClipReluGm);
            ctx.queuePostClipReluL1.EnQue(ctx.postClipReluL1);
            ctx.postClipReluL1 = ctx.queuePostClipReluL1.template DeQue<PostClipReluT>();
        }

        if (ctx.hasVectorPadding) {
            ctx.paddingL1 = ctx.queuePaddingL1.template AllocTensor<PaddingT>();
            loadPaddingL1Ins.LoadChannelWiseL1(ctx, ctx.paddingL1, ctx.paddingGm);
            ctx.queuePaddingL1.EnQue(ctx.paddingL1);
            ctx.paddingL1 = ctx.queuePaddingL1.template DeQue<PaddingT>();
        }

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ != 5101 && __NPU_ARCH__ != 5161 && __NPU_ARCH__ != 5165 && __NPU_ARCH__ != 5163)
        if (ctx.hasVectorFixp) {
            event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::MTE2_FIX));
            SetFlag<HardEvent::MTE2_FIX>(eventId);
            WaitFlag<HardEvent::MTE2_FIX>(eventId);
        }
#endif
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadFixpL0BeforeIterateK()
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
    if (ctx.hasVectorPreQuant) {
        ctx.preQuantFB = ctx.queuePreQuantFB.template AllocTensor<PreQuantT>();
        loadPreQuantFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_PRE_QUANT, ctx.preQuantFB, ctx.preQuantL1);
        ctx.queuePreQuantFB.EnQue(ctx.preQuantFB);
        ctx.preQuantFB = ctx.queuePreQuantFB.template DeQue<PreQuantT>();
    }

    if (ctx.hasVectorPreRelu) {
        ctx.preReluFB = ctx.queuePreReluFB.template AllocTensor<PreReluT>();
        loadPreReluFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_PRE_RELU, ctx.preReluFB, ctx.preReluL1);
        ctx.queuePreReluFB.EnQue(ctx.preReluFB);
        ctx.preReluFB = ctx.queuePreReluFB.template DeQue<PreReluT>();
    }

    if (ctx.hasVectorPreClipRelu) {
        ctx.preClipReluFB = ctx.queuePreClipReluFB.template AllocTensor<PreClipReluT>();
        loadPreClipReluFBIns.LoadFixpParamFb(
            ctx, DST_MEM_BLOCK_PRE_CLIP_RELU, ctx.preClipReluFB, ctx.preClipReluL1);
        ctx.queuePreClipReluFB.EnQue(ctx.preClipReluFB);
        ctx.preClipReluFB = ctx.queuePreClipReluFB.template DeQue<PreClipReluT>();
    }

    if (ctx.hasVectorAntiQuant) {
        ctx.antiQuantFB = ctx.queueAntiQuantFB.template AllocTensor<AntiQuantT>();
        loadAntiQuantFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_ANTI_QUANT, ctx.antiQuantFB, ctx.antiQuantL1);
        ctx.queueAntiQuantFB.EnQue(ctx.antiQuantFB);
        ctx.antiQuantFB = ctx.queueAntiQuantFB.template DeQue<AntiQuantT>();
    }

    if (ctx.hasVectorPostQuant) {
        ctx.postQuantFB = ctx.queuePostQuantFB.template AllocTensor<PostQuantT>();
        loadPostQuantFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_POST_QUANT, ctx.postQuantFB, ctx.postQuantL1);
        ctx.queuePostQuantFB.EnQue(ctx.postQuantFB);
        ctx.postQuantFB = ctx.queuePostQuantFB.template DeQue<PostQuantT>();
    }

    if (ctx.hasVectorPostRelu) {
        ctx.postReluFB = ctx.queuePostReluFB.template AllocTensor<PostReluT>();
        loadPostReluFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_POST_RELU, ctx.postReluFB, ctx.postReluL1);
        ctx.queuePostReluFB.EnQue(ctx.postReluFB);
        ctx.postReluFB = ctx.queuePostReluFB.template DeQue<PostReluT>();
    }

    if (ctx.hasVectorPostClipRelu) {
        ctx.postClipReluFB = ctx.queuePostClipReluFB.template AllocTensor<PostClipReluT>();
        loadPostClipReluFBIns.LoadFixpParamFb(
            ctx, DST_MEM_BLOCK_POST_CLIP_RELU, ctx.postClipReluFB, ctx.postClipReluL1);
        ctx.queuePostClipReluFB.EnQue(ctx.postClipReluFB);
        ctx.postClipReluFB = ctx.queuePostClipReluFB.template DeQue<PostClipReluT>();
    }

    if (ctx.hasVectorPadding) {
        ctx.paddingPT = ctx.queuePaddingPT.template AllocTensor<PaddingT>();
        loadPaddingPTIns.LoadPaddingPt(ctx);
        ctx.queuePaddingPT.EnQue(ctx.paddingPT);
        ctx.paddingPT = ctx.queuePaddingPT.template DeQue<PaddingT>();
    }
#endif
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::LoadFixpBeforeIterateK() {
    LoadFixpL1BeforeIterateK();
    LoadFixpL0BeforeIterateK();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::ReduceOneK()
{
    if (unlikely(ctx.loadBL1Flag)) {
        LoadBL1Moudle();
        ctx.weightL1 = ctx.queueWeightL1.template DeQue<WeightT>();
    }
    // zj: preload need check
    if (mPreLoadAFlag) {
        if (unlikely(ctx.loadAL1Flag)) {
            loadFmapl1Ins.SetSpr(ctx);
            ctx.mAL1UpdateFlag = false;
            LocalTensor<FmapT> fmapL1 = ctx.queueFmapL1.template DeQue<FmapT>();
            Iters iters;
            if (!UpdateItersByFmap(iters, true)) {
                if ((outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
                     (iters.kAL1Iter != ctx.kAL1Iter || iters.mAL1Iter != ctx.mAL1Iter ||
                      iters.batchIter != ctx.batchIter)) ||
                    (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
                     (iters.hoAL1Iter != ctx.hoAL1Iter || iters.woAL1Iter != ctx.woAL1Iter ||
                      iters.mAL1Iter != ctx.mAL1Iter || iters.batchIter != ctx.batchIter))) {
                    bool mAL1UpdateFlag = ctx.mAL1UpdateFlag;
                    ctx.mAL1UpdateFlag = true;
                    LoadAL1BaseMoudle(iters);
                    ctx.mAL1UpdateFlag = mAL1UpdateFlag;
                }
            }
            ctx.fmapL1 = fmapL1;
            ctx.currentFmapL1Offset = 0;
            ctx.loadAL1Flag = false;
        }
    } else {
        if (unlikely(ctx.loadAL1Flag)) {
            LoadAL1Moudle();
            ctx.fmapL1 = ctx.queueFmapL1.template DeQue<FmapT>();
            ctx.currentFmapL1Offset = 0;
        }
    }

    LoadL0Moudle(ctx.kL0FullLoadAl0PingPongFlag, ctx.kL0FullLoadBl0PingPongFlag, true);
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::ReduceKPreload()
{
    // state
    uint16_t al0PingPongFlag = 0;
    uint16_t bl0PingPongFlag = 0;
    bool isFirst = true;

    Iters iters;
    // updateIterByFmapTag is true means fm update; false means weight update
    bool updateIterByFmapTag = ctx.convTiling->kAL1 > ctx.convTiling->kBL1;
    if (ctx.kAL1fullload && !ctx.kBL1fullload) {
        updateIterByFmapTag = false;
    } else if (!ctx.kAL1fullload && ctx.kBL1fullload) {
        updateIterByFmapTag = true;
    }

    if (ctx.loadAL1Flag || !ctx.kAL1fullload) {
        ctx.kAL1Iter = 0;
        LoadAL1BaseMoudle();
        if (ctx.kAL1fullload) {
            ctx.fmapL1 = ctx.queueFmapL1.template DeQue<FmapT>();
            ctx.loadAL0Flag = true;
            ctx.currentFmapL1Offset = 0;
        }
    }

    if (ctx.loadBL1Flag || !ctx.kBL1fullload) {
        ctx.kBL1Iter = 0;
        LoadBL1BaseMoudle();
        if (ctx.kBL1fullload) {
            ctx.weightL1 = ctx.queueWeightL1.template DeQue<WeightT>();
            ctx.loadBL0Flag = true;
        }
    }

    while (ctx.kIter < ctx.ddr2l0LoopK) {
        if (ctx.loadAL1Flag || !ctx.kAL1fullload) {
            if (ctx.kIter % ctx.multiKAL1 == 0) {
                loadFmapl1Ins.SetSpr(ctx);
                ctx.mAL1UpdateFlag = false;
            }
            if (ctx.kIter % ctx.multiKAL1 == 0 && ctx.kIter < ctx.lastLoopKAL1StartPos) {
                if (ctx.kIter != 0) {
                    ctx.queueFmapL1.FreeTensor(ctx.fmapL1);  // always freeTensor
                }
                if (!UpdateItersByFmap(iters, updateIterByFmapTag)) {
                    LoadAL1BaseMoudle(iters);
                }
            }
            if (ctx.kIter == ctx.lastLoopKAL1StartPos) {
                ctx.queueFmapL1.FreeTensor(ctx.fmapL1); // last iter of multiKAL1 only Free AL1 Tensor
            }
            if (ctx.kIter % ctx.multiKAL1 == 0) {
                ctx.fmapL1 = ctx.queueFmapL1.template DeQue<FmapT>();
                ctx.loadAL0Flag = true;
                ctx.currentFmapL1Offset = 0;
            }
        }

        if (ctx.loadBL1Flag || !ctx.kBL1fullload) {
            if (ctx.kIter % ctx.multiKBL1 == 0 && ctx.kIter < ctx.lastLoopKBL1StartPos) {
                if (ctx.kIter != 0) {
                    ctx.queueWeightL1.FreeTensor(ctx.weightL1); // always freeTensor
                }
                if (!UpdateItersByWeight(iters, updateIterByFmapTag)) {
                    LoadBL1BaseMoudle(iters);
                }
            }
            if (ctx.kIter == ctx.lastLoopKBL1StartPos) {
                ctx.queueWeightL1.FreeTensor(ctx.weightL1); // last iter of multiKBL1 only Free BL1 Tensor
            }
            if (ctx.kIter % ctx.multiKBL1 == 0) {
                ctx.weightL1 = ctx.queueWeightL1.template DeQue<WeightT>();
                ctx.loadBL0Flag = true;
            }
        }

        LoadL0Moudle(al0PingPongFlag, bl0PingPongFlag, isFirst);
        isFirst = false;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::ReduceK()
{
    // state
    uint16_t al0PingPongFlag = 0;
    uint16_t bl0PingPongFlag = 0;
    bool isFirst = true;

    while (ctx.kIter < ctx.ddr2l0LoopK) {
        if (ctx.loadAL1Flag || (!ctx.kAL1fullload && ctx.kIter % ctx.multiKAL1 == 0)) {
            if (ctx.kIter != 0) {
                ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
            }
            LoadAL1Moudle();
            ctx.fmapL1 = ctx.queueFmapL1.template DeQue<FmapT>();
            ctx.loadAL0Flag = true;
            ctx.currentFmapL1Offset = 0;
        }
        if (ctx.loadBL1Flag || (!ctx.kBL1fullload && ctx.kIter % ctx.multiKBL1 == 0)) {
            if (ctx.kIter != 0) {
                ctx.queueWeightL1.FreeTensor(ctx.weightL1);
            }
            LoadBL1Moudle();
            ctx.weightL1 = ctx.queueWeightL1.template DeQue<WeightT>();
            ctx.loadBL0Flag = true;
        }

        LoadL0Moudle(al0PingPongFlag, bl0PingPongFlag, isFirst);
        isFirst = false;
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::FreeBiasL1AfterIterateK()
{
    if (ctx.hasBias && !ctx.convTiling->biasFullLoadFlag) {
        ctx.queueBiasL1.FreeTensor(ctx.biasL1);
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::FreeFixpL1AfterIterateK()
{
    if (!ctx.convTiling->fixpParamsFullLoadFlag) {
        if (ctx.hasVectorPreQuant) {
            ctx.queuePreQuantL1.FreeTensor(ctx.preQuantL1);
        }
        if (ctx.hasVectorPreRelu) {
            ctx.queuePreReluL1.FreeTensor(ctx.preReluL1);
        }
        if (ctx.hasVectorPreClipRelu) {
            ctx.queuePreClipReluL1.FreeTensor(ctx.preClipReluL1);
        }
        if (ctx.hasVectorAntiQuant) {
            ctx.queueAntiQuantL1.FreeTensor(ctx.antiQuantL1);
        }
        if (ctx.hasVectorPostQuant) {
            ctx.queuePostQuantL1.FreeTensor(ctx.postQuantL1);
        }
        if (ctx.hasVectorPostRelu) {
            ctx.queuePostReluL1.FreeTensor(ctx.postReluL1);
        }
        if (ctx.hasVectorPostClipRelu) {
            ctx.queuePostClipReluL1.FreeTensor(ctx.postClipReluL1);
        }
        if (ctx.hasVectorPadding) {
            ctx.queuePaddingL1.FreeTensor(ctx.paddingL1);
        }
    }

    if (ctx.hasEltwise) {
        if (ctx.convTiling->eltwiseBrcEn) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueEltwiseL1.FreeTensor(ctx.eltwiseL1);
            }
        } else {
            ctx.queueEltwiseL1.FreeTensor(ctx.eltwiseL1);
        }
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::FreeL1TensorAfterIterateK()
{
    if (!ctx.kAL1fullload) {
        ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
    }

    if (!ctx.kBL1fullload) {
        ctx.queueWeightL1.FreeTensor(ctx.weightL1);
    }

    FreeBiasL1AfterIterateK();

    FreeFixpL1AfterIterateK();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::FreeBiasL0TensorAfterIterateK()
{
    if (ctx.hasBias) {
        ctx.queueBiasBT.FreeTensor(ctx.biasBT);
    }
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::FreeFixpL0TensorAfterIterateK()
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
    if (ctx.hasVectorPreQuant) {
        ctx.queuePreQuantFB.FreeTensor(ctx.preQuantFB);
    }
    if (ctx.hasVectorPreRelu) {
        ctx.queuePreReluFB.FreeTensor(ctx.preReluFB);
    }
    if (ctx.hasVectorPreClipRelu) {
        ctx.queuePreClipReluFB.FreeTensor(ctx.preClipReluFB);
    }
    if (ctx.hasVectorAntiQuant) {
        ctx.queueAntiQuantFB.FreeTensor(ctx.antiQuantFB);
    }
    if (ctx.hasVectorPostQuant) {
        ctx.queuePostQuantFB.FreeTensor(ctx.postQuantFB);
    }
    if (ctx.hasVectorPostRelu) {
        ctx.queuePostReluFB.FreeTensor(ctx.postReluFB);
    }
    if (ctx.hasVectorPostClipRelu) {
        ctx.queuePostClipReluFB.FreeTensor(ctx.postClipReluFB);
    }
    if (ctx.hasVectorPadding) {
        ctx.queuePaddingPT.FreeTensor(ctx.paddingPT);
    }
#endif
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::FreeL0TensorAfterIterateK()
{
    FreeBiasL0TensorAfterIterateK();
    FreeFixpL0TensorAfterIterateK();
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline void Conv2dIntf<Config, Impl>::IterateK()
{
    SetMNBeforeIterateK();

    LoadBiasBeforeIterateK();
    LoadFixpBeforeIterateK();

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
    ctx.outputL1 = ctx.queueOutputL1.template AllocTensor<OutputT>();
    if (ctx.maxKL0Iter > 0) {
        ctx.partialReadWrite0L1 = ctx.queuePartialReadWrite0.template AllocTensor<L0cT>();
        ctx.partialReadWrite1L1 = ctx.queuePartialReadWrite1.template AllocTensor<L0cT>();
    }
#else
    ctx.outputL0 = ctx.queueOutputL0.template AllocTensor<L0cT>();
#endif

    // reduceK priority: 1.KL0FullLoad 2.L1DB Preload 3. ordinary reduceK
    if constexpr (kl0FullLoadFlag) {
        printf("ReduceOneK");
        ReduceOneK();
    } else if constexpr (kPreLoadFlag) {
        printf("ReduceKPreload");
        ReduceKPreload();
    } else {
        printf("ReduceK");
        ReduceK();
    }

    FreeL1TensorAfterIterateK();
    FreeL0TensorAfterIterateK();

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
    ctx.queueOutputL1.EnQue(ctx.outputL1);
    ctx.outputL1 = ctx.queueOutputL1.template DeQue<OutputT>();
    if (ctx.maxKL0Iter > 0) {
        ctx.queuePartialReadWrite0.EnQue(ctx.partialReadWrite0L1);
        ctx.partialReadWrite0L1 = ctx.queuePartialReadWrite0.template DeQue<L0cT>();
        ctx.queuePartialReadWrite1.EnQue(ctx.partialReadWrite1L1);
        ctx.partialReadWrite1L1 = ctx.queuePartialReadWrite1.template DeQue<L0cT>();
    }
#else
    ctx.queueOutputL0.EnQue(ctx.outputL0);
    ctx.outputL0 = ctx.queueOutputL0.template DeQue<L0cT>();
#endif

    ctx.kIter = 0;
    ctx.currentFmapL1Offset = 0;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateImplMMode(bool enPartialSum)
{
    if (ctx.isFirstIterate) {
        FirstIterateImplMMode();
        if (mPreLoadAFlag) {
            if (ctx.maxKBL1Iter == 0 && ctx.maxNBL1Iter == 0) {
                LoadBL1Moudle();
                ctx.weightL1 = ctx.queueWeightL1.template DeQue<WeightT>();
                ctx.loadBL1Flag = false;
            }
            printf("first iterate fmap preload\n");
            LoadAL1Moudle();
            ctx.loadAL1Flag = true;
        }
    } else if constexpr (iterateMFirstFlag) {
        if (IterateMFirstMMode() == false) {
            return false;
        }
    } else if constexpr (iterateNFirstFlag) {
        if (IterateNFirstMMode() == false) {
            return false;
        }
    }

    IterateK();

    return true;
}

template<class Config, template<typename, class>class Impl>
__aicore__ inline bool Conv2dIntf<Config, Impl>::IterateImplHWMode(bool enPartialSum)
{
    // zj: preload
    if (ctx.isFirstIterate) {
        FirstIterateImplHWMode();
    } else if constexpr (iterateMFirstFlag) {
        if (IterateMFirstHWMode() == false) {
            return false;
        }
    } else if constexpr (iterateNFirstFlag) {
        if (IterateNFirstHWMode() == false) {
            return false;
        }
    }

    UpdateHoL0WoL0();

    IterateK();

    return true;
}

} // namespace Conv2dApi

#endif // ADV_API_CONV_CONV2D_CONV2D_INTF_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_INTF_IMPL_H__
#endif
