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
 * \file conv2d_intr_hw_mode_impl.h
 * \brief
 */

#ifndef API_CONV2D_INSTR_HW_MODE_IMPL_H
#define API_CONV2D_INSTR_HW_MODE_IMPL_H

#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "../../../lib/conv/conv2d/conv2d_config.h"
#include "../../conv2d_util.h"
#else
#include "include/adv_api/conv/conv2d/conv2d_config.h"
#include "../../conv2d_util.h"
#endif

namespace Conv2dApi {
using namespace AscendC;

template <class Intf>
class CopyEltwiseToolsHWMode {
public:
    __aicore__ inline CopyEltwiseToolsHWMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        valueHoWo_ = ctx.orgHo * ctx.orgWo;
    }

    __aicore__ inline void SetMN(const typename Intf::ContextType &ctx,
                                 uint64_t m, uint64_t n)
    {
        (void)ctx;
        currentML0_ = m;
        currentNL0_ = n;
    }

    __aicore__ inline uint64_t CalcOffset(const typename Intf::ContextType &ctx)
    {
        printf("[eltwiseL1]batchIter: %ld, mAL1Iter: %ld, mL0Iter: %ld, nBL1Iter: %ld, nL0Iter: %ld\n",
               ctx.batchIter, ctx.mAL1Iter, ctx.mL0Iter, ctx.nBL1Iter, ctx.nL0Iter);
        uint64_t offsetBatch = ctx.batchIter * ctx.outputOneBatchSize;
        uint64_t offsetCout = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                              ctx.nL0Iter * ctx.convTiling->nL0;
        uint64_t offsetH = ctx.hoAL1Iter * ctx.convTiling->hoL1 +
                           ctx.hoL0Iter * ctx.convTiling->hoL0;
        uint64_t offsetW;
        if (ctx.woL1SmallTail == 0) {
            offsetW = ctx.woAL1Iter * ctx.convTiling->woL1 +
                      ctx.woL0Iter * ctx.convTiling->woL0;
        } else if (ctx.woAL1Iter == ctx.maxWoL1Iter) {
            offsetW = ((ctx.woAL1Iter - 1) * ctx.convTiling->woL1 + ctx.woAL1Tail) +
                        ctx.woL0Iter * ctx.convTiling->woL0;
        } else {
            offsetW = ctx.woAL1Iter * ctx.convTiling->woL1 +
                        ctx.woL0Iter * ctx.convTiling->woL0;
        }

        if constexpr (Intf::formatEltwise == ConvCommonApi::ConvFormat::NC1HWC0) {
            uint64_t offset = offsetBatch +
                          offsetH * ctx.orgWo * Intf::k0Elt + offsetW * Intf::k0Elt +
                          offsetCout / Intf::k0Elt * valueHoWo_ * Intf::k0Elt +
                          offsetCout % Intf::k0Elt;
            return offset;
        } else if constexpr (Intf::formatEltwise == ConvCommonApi::ConvFormat::NCHW) {
            // zj: elt nchw
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported elt format: NCHW"); });
        } else if constexpr (Intf::formatEltwise == ConvCommonApi::ConvFormat::NHWC) {
            // zj: elt nhwc
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported elt format: NHWC"); });
        } else {
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported elt format"); });
        }
    }

    __aicore__ inline void CopyEltNC1HWC0(const typename Intf::ContextType &ctx)
    {
        uint64_t offset = CalcOffset(ctx);

        DataCopyPadExtParams<typename Intf::EltwiseT> padParams;
        padParams.isPad = false;
        padParams.leftPadding = 0;
        padParams.rightPadding = 0;
        padParams.paddingValue = 0;

        DataCopyExtParams intriParams;
        if (unlikely(ctx.convTiling->hoL1 == 1 || ctx.convTiling->woL1 == ctx.orgWo)) {
            intriParams.blockCount = currentNL0_ / Intf::k0Elt;
            intriParams.blockLen = currentML0_ * ConvApi::C0_SIZE;
            intriParams.srcStride = (valueHoWo_ - currentML0_) * ConvApi::C0_SIZE;
            intriParams.dstStride = 0;
            printf("[eltwiseL1]eltwiseL1GmOffset: %ld, blockCount: %ld, ",
                   "blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                   offset, intriParams.blockCount, intriParams.blockLen,
                   intriParams.srcStride, intriParams.dstStride);
            DataCopyPad<typename Intf::EltwiseT, PaddingMode::Normal>(ctx.eltwiseL1,
                                                                      ctx.eltwiseGm[offset],
                                                                      intriParams,
                                                                      padParams);
        } else {
            uint64_t currentN1 = currentNL0_ / Intf::k0Elt;
            for (uint64_t i = 0; i < currentN1; i++) {
                offset = offset + valueHoWo_ * Intf::k0Elt;
                intriParams.blockCount = ctx.currentHoL0;
                intriParams.blockLen = ctx.currentWoL0 * ConvApi::C0_SIZE;
                intriParams.srcStride = (ctx.orgWo - ctx.currentWoL0) * ConvApi::C0_SIZE;
                intriParams.dstStride = 0;
                printf("[eltwiseL1]eltwiseL1GmOffset: %ld, blockCount: %ld, ",
                       "blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                       offset, intriParams.blockCount, intriParams.blockLen,
                       intriParams.srcStride, intriParams.dstStride);
                DataCopyPad<typename Intf::EltwiseT, PaddingMode::Normal>(ctx.eltwiseL1,
                                                                          ctx.eltwiseGm[offset],
                                                                          intriParams,
                                                                          padParams);
            }
        }
    }

    __aicore__ inline void CopyEltNCHW(const typename Intf::ContextType &ctx)
    {
        // zj: elt nchw
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported elt format: NCHW"); });
    }

    __aicore__ inline void CopyEltNHWC(const typename Intf::ContextType &ctx)
    {
        // zj: elt nhwc
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported elt format: NHWC"); });
    }

    __aicore__ inline void CopyElt(const typename Intf::ContextType &ctx)
    {
        if constexpr (Intf::formatEltwise == ConvCommonApi::ConvFormat::NC1HWC0) {
            CopyEltNC1HWC0(ctx);
        } else if constexpr (Intf::formatEltwise == ConvCommonApi::ConvFormat::NCHW) {
            CopyEltNCHW(ctx);
        } else if constexpr (Intf::formatEltwise == ConvCommonApi::ConvFormat::NHWC) {
            CopyEltNHWC(ctx);
        } else {
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported elt format"); });
        }
    }

private:
    uint64_t valueHoWo_;
    uint64_t currentML0_;
    uint64_t currentNL0_;
};

template <class Intf>
class LoadFmapL1ToolsHWMode {
public:
    __aicore__ inline LoadFmapL1ToolsHWMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        (void)ctx;
    }

    __aicore__ inline void LoadAL1(const typename Intf::ContextType &ctx)
    {
        LoadAL1(ctx, ctx.kAL1Iter, ctx.woAL1Iter, ctx.hoAL1Iter, ctx.batchIter);
        SetSpr(ctx);
    }

    __aicore__ inline void LoadAL1(const typename Intf::ContextType &ctx,
                                   uint64_t kAL1Iter, uint64_t woAL1Iter,
                                   uint64_t hoAL1Iter, uint64_t batchIter)
    {
        UpdatePaddingHiWiAL1(ctx);
        UpdatePadIdxAL1(ctx, woAL1Iter, hoAL1Iter);
        UpdateHiWiAL1(ctx);
        uint64_t c1PerKL0Iter = ctx.convTiling->kStep * Intf::k0Weight /
                                (Intf::k0Fmap * ctx.kernelH * ctx.kernelW);
        ctx.fmapL1OffsetStep = c1PerKL0Iter * (uint64_t)hiLoadL1 * (uint64_t)wiLoadL1 *
                               ConvApi::C0_SIZE;
        LoadAl1Data(ctx, kAL1Iter, batchIter);
    }

    __aicore__ inline void SetSpr(const typename Intf::ContextType &ctx)
    {
        SetMFMatrix(ctx);
        SetFMStepPos(ctx);
        SetMFMatrixDual0(ctx);
        SetMFMatrixDual1(ctx);
        SetMPadding(ctx);
    }

private:
    __aicore__ inline void UpdatePaddingHiWiAL1(const typename Intf::ContextType &ctx)
    {
        paddingHiAL1 = (ctx.currentHoL1 - 1) * ctx.convTiling->strideH + ctx.dilatedKernelH;
        if ((ctx.convTiling->singleCoreWo <= ctx.convTiling->woL1 && ctx.convTiling->isC04Flag) || ctx.convTiling->orgWo <= ctx.convTiling->woL1) {
            paddingWiAL1 = ctx.convTiling->orgWi + ctx.convTiling->padLeft +
                           ctx.convTiling->padRight;
        } else {
            paddingWiAL1 = (ctx.currentWoL1 - 1) * ctx.convTiling->strideW +
                            ctx.dilatedKernelW;
        }
    }

    __aicore__ inline void UpdatePadIdxAL1(const typename Intf::ContextType &ctx,
                                           uint64_t woAL1Iter, uint64_t hoAL1Iter)
    {
        int64_t hiRealStartPos = ctx.hiStartPos > 0 ? 0 : ctx.hiStartPos;
        hiTopPadIdx = hoAL1Iter * ctx.convTiling->hoL1 * ctx.convTiling->strideH + hiRealStartPos;
        if (unlikely(ctx.woL1SmallTail != 0 && woAL1Iter == ctx.maxWoL1Iter)) {
            wiLeftPadIdx = ((woAL1Iter - 1) * ctx.convTiling->woL1 + ctx.woAL1Tail) *
                           ctx.convTiling->strideW - ctx.convTiling->padLeft;
        } else {
            wiLeftPadIdx = woAL1Iter * ctx.convTiling->woL1 * ctx.convTiling->strideW -
                           ctx.convTiling->padLeft;
        }
        hiBottomPadIdx = hiTopPadIdx + paddingHiAL1;
        wiRightPadIdx = wiLeftPadIdx + paddingWiAL1;
    }

    __aicore__ inline void UpdateHiWiAL1(const typename Intf::ContextType &ctx)
    {
        padTopL1 = hiTopPadIdx < 0 ? (0 - hiTopPadIdx) : 0;
        hiBottomPadIdx = ctx.hiStartPos > 0 ? hiBottomPadIdx + ctx.hiStartPos : hiBottomPadIdx;
        padBottomL1 = hiBottomPadIdx > ctx.orgHi ? hiBottomPadIdx - ctx.orgHi : 0;
        padLeftL1 = wiLeftPadIdx < 0 ? (0 - wiLeftPadIdx) : 0;
        padRightL1 = wiRightPadIdx > ctx.orgWi ? (wiRightPadIdx - ctx.orgWi) : 0;
        hiLoadL1 = paddingHiAL1 - padTopL1 -padBottomL1;
        hiLoadL1 = hiLoadL1 > ctx.orgHi ? ctx.orgHi : hiLoadL1;
        wiLoadL1 = paddingWiAL1 - padLeftL1 - padRightL1;
    }

    __aicore__ inline void LoadAl1DataNC1HWC0(const typename Intf::ContextType &ctx,
                                              uint64_t kAL1Iter, uint64_t batchIter)
    {
        int64_t aL1GmOffset = kAL1Iter * cinOffsetGM +
                              realHiTopGmIdx * ctx.orgWi * Intf::k0Fmap +
                              realWiTopGmIdx * Intf::k0Fmap +
                              batchIter * ctx.fmapOneBatchSize;
        uint32_t currentCi = kAL1Iter == ctx.maxKAL1Iter ? ctx.convTiling->cinATailInCore
                                                         : ctx.convTiling->cinAInCore;

        DataCopyPadExtParams<typename Intf::FmapT> padParams;
        padParams.isPad = false;
        padParams.leftPadding = 0;
        padParams.rightPadding = 0;
        padParams.paddingValue = 0;

        DataCopyExtParams intriParams;
        if (unlikely(ctx.convTiling->hoL1 == 1 || ctx.convTiling->woL1 == ctx.orgWo)) {
            intriParams.blockCount = ConvApi::CeilDIV(currentCi, Intf::k0Fmap);
            intriParams.blockLen = (hiLoadL1 * wiLoadL1) * ConvApi::C0_SIZE;
            intriParams.srcStride = (ctx.orgHi * ctx.orgWi - hiLoadL1 * wiLoadL1) *
                ConvApi::C0_SIZE;
            intriParams.dstStride = 0;
            printf("[fmapL1]aL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, ",
                   "srcStride: %ld, dstStride: %ld\n",
                   aL1GmOffset, intriParams.blockCount, intriParams.blockLen,
                   intriParams.srcStride, intriParams.dstStride);
            DataCopyPad<typename Intf::FmapT, PaddingMode::Normal>(ctx.fmapL1,
                                                                   ctx.fmapGm[aL1GmOffset],
                                                                   intriParams,
                                                                   padParams);
        } else {
            intriParams.blockCount = hiLoadL1;
            intriParams.blockLen = wiLoadL1 * ConvApi::C0_SIZE;
            intriParams.srcStride = (ctx.orgWi - wiLoadL1) * ConvApi::C0_SIZE;
            intriParams.dstStride = intriParams.blockLen;
            const uint64_t planeStrideGm = ctx.orgHi * ctx.orgWi * Intf::k0Fmap;
            const uint64_t planeStrideL1 = (uint64_t)hiLoadL1 * wiLoadL1 * Intf::k0Fmap;
            uint64_t currentC1 = ConvApi::CeilDIV(currentCi, Intf::k0Fmap);
            for (uint64_t i = 0; i < currentC1; i++) {
                int64_t planeGmOffset = aL1GmOffset + (int64_t)(i * planeStrideGm);
                printf("[fmapL1]aL1GmOffset: %ld, blockCount: %hd, blockLen: %hd, srcStride: %hd, dstStride: %hd\n",
                    planeGmOffset, intriParams.blockCount, intriParams.blockLen,
                    intriParams.srcStride, intriParams.dstStride);
                DataCopyPad<typename Intf::FmapT, PaddingMode::Normal>(
                    ctx.fmapL1[static_cast<uint32_t>(i * planeStrideL1)],
                    ctx.fmapGm[planeGmOffset],
                    intriParams,
                    padParams);
            }
        }
    }

    __aicore__ inline void LoadAl1DataNCHW(const typename Intf::ContextType &ctx,
                                           uint64_t kAL1Iter, uint64_t batchIter)
    {
        int64_t aL1GmOffset = kAL1Iter * cinOffsetGM +
                              realHiTopGmIdx * ctx.orgWi + realWiTopGmIdx +
                              batchIter * ctx.fmapOneBatchSize;

        Dn2NzParams intriParams;
        if (unlikely(ctx.convTiling->hoL1 == 1 || wiLoadL1 == ctx.orgWi)) {
            intriParams.dnNum = 1;
            intriParams.nValue = hiLoadL1 * wiLoadL1;
            intriParams.srcDnMatrixStride = 0;
            intriParams.dstNzMatrixStride = 0;
        } else {
            intriParams.dnNum = hiLoadL1;
            intriParams.nValue = wiLoadL1;
            intriParams.srcDnMatrixStride = ctx.orgWi;
            intriParams.dstNzMatrixStride = wiLoadL1 * Intf::k0Fmap;
        }
        intriParams.dValue = kAL1Iter == ctx.maxKAL1Iter
                                 ? ctx.convTiling->cinATailInCore
                                 : ctx.convTiling->cinAInCore;
        intriParams.srcDValue = ctx.convTiling->oriHixWi;
        intriParams.dstNzC0Stride = hiLoadL1 * wiLoadL1;
        intriParams.dstNzNStride = 1;

        DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
    }

    __aicore__ inline void LoadAl1DataNHWC(const typename Intf::ContextType &ctx,
                                           uint64_t kAL1Iter, uint64_t batchIter)
    {
        // zj: fmap nhwc
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported fmap format: NHWC"); });
    }

    __aicore__ inline void LoadAl1DataNC1HWC04(const typename Intf::ContextType &ctx,
                                               uint64_t kAL1Iter, uint64_t batchIter)
    {
        // zj: fmap nchwc04
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported fmap format: NC1HWC04"); });
    }

    __aicore__ inline void LoadAl1Data(const typename Intf::ContextType &ctx,
                                       uint64_t kAL1Iter, uint64_t batchIter)
    {
        printf("[fmapL1]batchIter: %ld, hiTopPadIdx: %ld, wiLeftPadIdx: %ld, kAL1Iter: %ld\n",
               batchIter, hiTopPadIdx, wiLeftPadIdx, kAL1Iter);

        realHiTopGmIdx = hiTopPadIdx < 0 ? 0 : hiTopPadIdx;
        realWiTopGmIdx = wiLeftPadIdx < 0 ? 0 : wiLeftPadIdx;

        cinOffsetGM = (ctx.convTiling->kAL1 / ctx.convTiling->kernelHxkernelW) * ctx.convTiling->oriHixWi;

        if constexpr (Intf::formatFmap == ConvCommonApi::ConvFormat::NC1HWC0) {
            LoadAl1DataNC1HWC0(ctx, kAL1Iter, batchIter);
        } else if constexpr (Intf::formatFmap == ConvCommonApi::ConvFormat::NCHW) {
            LoadAl1DataNCHW(ctx, kAL1Iter, batchIter);
        } else if constexpr (Intf::formatFmap == ConvCommonApi::ConvFormat::NHWC) {
            LoadAl1DataNHWC(ctx, kAL1Iter, batchIter);
        } else if constexpr (Intf::formatFmap == ConvCommonApi::ConvFormat::NC1HWC04) {
            LoadAl1DataNC1HWC04(ctx, kAL1Iter, batchIter);
        } else {
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported fmap format"); });
        }
    }

    __aicore__ inline void SetMFMatrix(const typename Intf::ContextType &ctx)
    {
        ConvFMatrixBitModeParams params;
        params.SetWFmap(wiLoadL1);
        params.SetHFmap(hiLoadL1);
        params.SetLeftPaddingSize(padLeftL1);
        params.SetRightPaddingSize(padRightL1);
        params.SetTopPaddingSize(padTopL1);
        params.SetBottomPaddingSize(padBottomL1);
        printf("[fmapL1]fmatrix: %ld\n", params.GetConfig0());
        SetConvFMatrix(params);
    }

    __aicore__ inline void SetFMStepPos(const typename Intf::ContextType &ctx)
    {
        uint64_t wStep = paddingWiAL1 - ctx.dilatedKernelW + 1;
        uint64_t hStep = paddingHiAL1 - ctx.dilatedKernelH + 1;
        wStep = (wStep - 1) / ctx.convTiling->strideW * ctx.convTiling->strideW + 1;
        hStep = (hStep - 1) / ctx.convTiling->strideH * ctx.convTiling->strideH + 1;

        ConvFMatrixStepPosBitModeParams params;
        params.SetWStartPos(0);
        params.SetHStartPos(0);
        params.SetWStep(wStep);
        params.SetHStep(hStep);
        printf("[fmapL1]fmStepPos: %ld\n", params.GetConfig0());
        SetConvFMatrixStepPos(params);
    }

    __aicore__ inline void SetMFMatrixDual0(const typename Intf::ContextType &ctx)
    {
        // zj: support dual src
        ConvFMatrixDual0BitModeParams params;
        params.SetSecondAddr(0);
        params.SetSecondH(0);
        printf("[fmapL1]fMatrixDual0: %ld\n", params.GetConfig0());
        SetConvFMatrixDual0(params);
    }

    __aicore__ inline void SetMFMatrixDual1(const typename Intf::ContextType &ctx)
    {
        ConvFMatrixDual1BitModeParams params;
        params.SetFirstStride(hiLoadL1 * wiLoadL1);
        params.SetSecondStride(0);
        printf("[fmapL1]fMatrixDual1: %ld\n", params.GetConfig0());
        SetConvFMatrixDual1(params);
    }

    __aicore__ inline void SetMPadding(const typename Intf::ContextType &ctx)
    {
        constexpr uint64_t BITS_PADDING_VALUE_ADDR = 0xffff;

        ConvPaddingBitModeParams<typename Intf::PaddingT> params;
        if (ctx.hasPadding) {
            if (ctx.hasVectorPadding) {
                params.SetPaddingValue(0);
                params.SetPaddingMode(1);
                params.SetPaddingAddr((uint16_t)(((uint64_t)(ctx.paddingPT.GetPhyAddr())) & \
                                                 BITS_PADDING_VALUE_ADDR));
            } else {
                typename Intf::PaddingT padding = ctx.paddingGm.GetValue(0);
                params.SetPaddingValue(padding);
                params.SetPaddingMode(0);
                params.SetPaddingAddr(0);
            }
        }
        printf("[fmapL1]mPadding: %ld\n", params.GetConfig0());
        SetConvPadding(params);
    }

private:
    int64_t padTopL1;
    int64_t padBottomL1;
    int64_t padLeftL1;
    int64_t padRightL1;

    int64_t hiTopPadIdx;
    int64_t hiBottomPadIdx;
    int64_t wiLeftPadIdx;
    int64_t wiRightPadIdx;

    int64_t realHiTopGmIdx;
    int64_t realWiTopGmIdx;

    int64_t paddingHiAL1;
    int64_t paddingWiAL1;

    int64_t hiLoadL1;
    int64_t wiLoadL1;

    int64_t cinOffsetGM;
};

template <class Intf>
class LoadFmapL0ToolsHWMode {
public:
    __aicore__ inline LoadFmapL0ToolsHWMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        (void)ctx;
    }

    __aicore__ inline void SetM(const typename Intf::ContextType &ctx,
                                uint64_t m, uint64_t mNotAlign)
    {
        (void)ctx;
        (void)m;
        (void)mNotAlign;
    }

    __aicore__ inline void SetLoad3dRepeatParams(const typename Intf::ContextType &ctx)
    {
        (void)ctx;
    }

    __aicore__ inline void LoadAL0(const typename Intf::ContextType &ctx,
                                   bool isFirst = true)
    {
        (void)ctx;
        (void)isFirst;
    };
};

template <class Intf>
class CopyOutputToolsHWMode {
public:
    __aicore__ inline CopyOutputToolsHWMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        (void)ctx;
        valueHoWo_ = ctx.orgHo * ctx.orgWo;
    }

    __aicore__ inline void SetMN(const typename Intf::ContextType &ctx,
                                 uint64_t m, uint64_t n)
    {
        (void)ctx;
        currentML0_ = m;
        currentNL0_ = n;
    }

    __aicore__ inline uint64_t CalcOffset(const typename Intf::ContextType &ctx)
    {
        uint64_t offsetBatch = ctx.batchIter * ctx.outputOneBatchSize;
        uint64_t offsetCout = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                              ctx.nL0Iter * ctx.convTiling->nL0;
        uint64_t offsetH = ctx.hoAL1Iter * ctx.convTiling->hoL1 +
                           ctx.hoL0Iter * ctx.convTiling->hoL0;
        uint64_t offsetW;
        if (ctx.woL1SmallTail == 0) {
            offsetW = ctx.woAL1Iter * ctx.convTiling->woL1 +
                      ctx.woL0Iter * ctx.convTiling->woL0;
        } else if (ctx.woAL1Iter == ctx.maxWoL1Iter) {
            offsetW = ((ctx.woAL1Iter - 1) * ctx.convTiling->woL1 + ctx.woAL1Tail) +
                       ctx.woL0Iter * ctx.convTiling->woL0;
        } else {
            offsetW = ctx.woAL1Iter * ctx.convTiling->woL1 +
                      ctx.woL0Iter * ctx.convTiling->woL0;
        }

        if constexpr (Intf::formatOutput == ConvCommonApi::ConvFormat::NC1HWC0) {
            uint64_t offset = offsetBatch +
                offsetH * ctx.orgWo * Intf::k0Output +
                offsetW * Intf::k0Output +
                offsetCout / Intf::k0Output * valueHoWo_ * Intf::k0Output +
                offsetCout % Intf::k0Output;
            return offset;
        } else if constexpr (Intf::formatOutput == ConvCommonApi::ConvFormat::NCHW) {
            uint64_t offset = offsetBatch +
                offsetH * ctx.orgWo + offsetW +
                offsetCout * valueHoWo_;
            return offset;
        } else if constexpr (Intf::formatOutput == ConvCommonApi::ConvFormat::NHWC) {
            // zj: out nhwc
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported out format: NHWC"); });
        } else {
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported out format"); });
        }
        return 0;
    }

    __aicore__ inline void CopyOutNC1HWC0(const typename Intf::ContextType &ctx,
                                          const GlobalTensor<typename Intf::OutputT> &output,
                                          uint64_t offset)
    {
        DataCopyExtParams intriParams;
        // fixpipe 的 loop2_size 有 16bit 位宽限制.
        constexpr uint16_t MAX_LOOP2_SIZE = 0xffff;
        if (unlikely((ctx.convTiling->hoL1 == 1 || ctx.convTiling->woL1 == ctx.orgWo) &&
                     currentML0_ * ConvApi::C0_SIZE / sizeof(typename Intf::OutputT) <= MAX_LOOP2_SIZE)) {
            intriParams.blockCount = currentNL0_ / Intf::k0Output;
            intriParams.blockLen = currentML0_ * ConvApi::C0_SIZE;
            intriParams.srcStride = 0;
            intriParams.dstStride = (valueHoWo_ - currentML0_) * ConvApi::C0_SIZE;
            printf("[cL1]cL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                offset, intriParams.blockCount, intriParams.blockLen,
                intriParams.srcStride, intriParams.dstStride);
            DataCopyPad<typename Intf::OutputT, PaddingMode::Normal>(output[offset],
                                                                     ctx.outputL1,
                                                                     intriParams);
        } else {
            uint64_t currentN1 = ConvApi::CeilDIV(currentNL0_, Intf::k0Output);
            for (uint64_t i = 0; i < currentN1; i++) {
                intriParams.blockCount = ctx.currentHoL0;
                intriParams.blockLen = ctx.currentWoL0 * ConvApi::C0_SIZE;
                intriParams.srcStride = 0;
                intriParams.dstStride = (ctx.orgWo - ctx.currentWoL0) * ConvApi::C0_SIZE;
                DataCopyPad<typename Intf::OutputT, PaddingMode::Normal>(
                    output[offset + i * ctx.orgHo * ctx.orgWo * Intf::k0Output],
                    ctx.outputL1[i * ctx.currentHoL0 * ctx.currentWoL0 * Intf::k0Output],
                    intriParams);
                printf("[cL1]cL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                    offset + i * ctx.orgHo * ctx.orgWo * Intf::k0Output, intriParams.blockCount, intriParams.blockLen,
                    intriParams.srcStride, intriParams.dstStride);
            }
        }
    }

    __aicore__ inline void CopyOutNCHW(const typename Intf::ContextType &ctx,
                                       const GlobalTensor<typename Intf::OutputT> &output,
                                       uint64_t offset)
    {
        // zj: out nchw
        Nz2DnParamsFull params;
        params.dnNum = 0;
        params.nValue = 0;
        params.dValue = 0;
        params.srcNzMatrixStride = 0;
        params.srcNStride = 0;
        params.dstNStride = 0;
        params.dstDnMatrixStride = 0;
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported out format: NCHW"); });
    }

    __aicore__ inline void CopyOutNHWC(const typename Intf::ContextType &ctx,
                                       const GlobalTensor<typename Intf::OutputT> &output,
                                       uint64_t offset)
    {
        // zj: out nhwc
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported out format: NHWC"); });
    }

    __aicore__ inline void CopyOut(const typename Intf::ContextType &ctx, const GlobalTensor<typename Intf::OutputT> &output)
    {
        printf("[cL1]batchIter: %ld, mAL1Iter: %ld, mL0Iter: %ld, nBL1Iter: %ld, nL0Iter: %ld\n",
            ctx.batchIter, ctx.mAL1Iter, ctx.mL0Iter, ctx.nBL1Iter, ctx.nL0Iter);

        uint64_t offset = CalcOffset(ctx);

        if constexpr (Intf::formatOutput == ConvCommonApi::ConvFormat::NC1HWC0) {
            CopyOutNC1HWC0(ctx, output, offset);
        } else if constexpr (Intf::formatOutput == ConvCommonApi::ConvFormat::NCHW) {
            CopyOutNCHW(ctx, output, offset);
        } else if constexpr (Intf::formatOutput == ConvCommonApi::ConvFormat::NHWC) {
            CopyOutNHWC(ctx, output, offset);
        } else {
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported format"); });
        }
    }

private:
    uint64_t valueHoWo_;
    uint64_t currentML0_;
    uint64_t currentNL0_;
};

};

#endif // API_CONV2D_INSTR_HW_MODE_IMPL_H
