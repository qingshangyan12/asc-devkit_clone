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

#include "lib/conv/conv2d/conv2d_config.h" // #include "../../../../../include/adv_api/conv/conv2d/conv2d_config.h"  // [zj]TODO
#include "../../conv2d_util.h"

namespace Conv2dApiFunc {
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
        } else {
            if (ctx.woAL1Iter == ctx.maxWoL1Iter) {
                offsetW = ((ctx.woAL1Iter - 1) * ctx.convTiling->woL1 + ctx.woAL1Tail) +
                          ctx.woL0Iter * ctx.convTiling->woL0;
            } else {
                offsetW = ctx.woAL1Iter * ctx.convTiling->woL1 +
                          ctx.woL0Iter * ctx.convTiling->woL0;
            }
        }

        uint64_t offset = offsetBatch +
                          offsetH * ctx.orgWo * Intf::k0e + offsetW * Intf::k0e +
                          offsetCout / Intf::k0e * valueHoWo_ * Intf::k0e +
                          offsetCout % Intf::k0e;
        return offset;
    }

    __aicore__ inline void CopyElt(const typename Intf::ContextType &ctx)
    {
        uint64_t offset = CalcOffset(ctx);
        if (unlikely(ctx.convTiling->hoL1 == 1 || ctx.convTiling->woL1 == ctx.orgWo)) {
            DataCopyParams intriParams;
            intriParams.blockCount = currentNL0_ / Intf::k0e;
            intriParams.blockLen = currentML0_;
            intriParams.srcStride = valueHoWo_ - currentML0_;
            intriParams.dstStride = 0;
            printf("[eltwiseL1]eltwiseL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                   offset, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
            DataCopy<typename Intf::EltwiseT>(ctx.eltwiseL1, ctx.eltwiseGm[offset], intriParams);
        } else {
            uint64_t currentN1 = currentNL0_ / Intf::k0e;
            for (uint64_t i = 0; i < currentN1; i++) {
                DataCopyParams intriParams;
                intriParams.blockCount = ctx.currentHoL0;
                intriParams.blockLen = ctx.currentWoL0;
                intriParams.srcStride = ctx.orgWo - ctx.currentWoL0;
                intriParams.dstStride = 0;
                offset = offset + valueHoWo_ * Intf::k0e;
                printf("[eltwiseL1]eltwiseL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                    offset, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
                DataCopy<typename Intf::EltwiseT>(ctx.eltwiseL1, ctx.eltwiseGm[offset], intriParams);
            }
        }
    }

private:
    uint64_t valueHoWo_;
    uint64_t currentML0_;
    uint64_t currentNL0_;
};

template <class Intf>
class LoadAL1ToolsHWMode {
public:
    __aicore__ inline LoadAL1ToolsHWMode()
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

    __aicore__ inline void LoadAL1(const typename Intf::ContextType &ctx, uint64_t kAL1Iter, uint64_t woAL1Iter, uint64_t hoAL1Iter, uint64_t batchIter)
    {
        UpdatePaddingHiWiAL1(ctx);
        UpdatePadIdxAL1(ctx, woAL1Iter, hoAL1Iter);
        UpdateHiWiAL1(ctx);
        LoadAl1Data(ctx, kAL1Iter, batchIter);
    }

    __aicore__ inline void SetSpr(const typename Intf::ContextType &ctx)
    {
        SetMFMatrix(ctx);
        SetFMStepPos(ctx);
        SetMFMatrixDual0(ctx);
        SetMFMatrixDual1(ctx);
        // [zj]TODO: padding offset
        // SetLoadDataPaddingValue(ctx.convTiling->offsetx);
        SetMPadding(ctx);
    }

private:
    __aicore__ inline void UpdatePaddingHiWiAL1(const typename Intf::ContextType &ctx)
    {
        paddingHiAL1 = (ctx.currentHoL1 - 1) * ctx.convTiling->strideH + ctx.dilatedKernelH;
        if (ctx.convTiling->singleCoreWo <= ctx.convTiling->woL1 && ctx.convTiling->isC04Flag) {
            paddingWiAL1 = ctx.convTiling->orgWi + ctx.convTiling->padLeft +
                            ctx.convTiling->padRight;
        } else {
            paddingWiAL1 = (ctx.currentWoL1 - 1) * ctx.convTiling->strideW +
                            ctx.dilatedKernelW;
        }
    }

    __aicore__ inline void UpdatePadIdxAL1(const typename Intf::ContextType &ctx, uint64_t woAL1Iter, uint64_t hoAL1Iter)
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

    // [zj]TODO: hw mode no need
    // __aicore__ inline void SetDataCopyIntriParams(const typename Intf::ContextType &ctx, DataCopyParams &intriParams, uint64_t kAL1Iter)
    // {
    //     intriParams.blockCount = ConvApi::CeilDIV(kAL1Iter == ctx.maxKAL1Iter ? ctx.convTiling->cinATailInCore
    //                                                                  : ctx.convTiling->cinAInCore,
    //                                      Intf::k0a);
    //     intriParams.blockLen = hiLoadL1 * wiLoadL1;
    //     intriParams.srcStride = ctx.orgHi * ctx.orgWi - hiLoadL1 * wiLoadL1;
    //     intriParams.dstStride = 0;
    // }

    // [zj]TODO: api to support
    // __aicore__ inline void SetDn2NzIntriParams(const typename Intf::ContextType &ctx, Dn2NzParams &intriParams, uint64_t kAL1Iter)
    // {
    //     if (unlikely(ctx.convTiling->hoL1 == 1 || wiLoadL1 == ctx.orgWi)) {
    //         intriParams.dnNum = 1;
    //         intriParams.nValue = hiLoadL1 * wiLoadL1;
    //         intriParams.srcDnMatrixStride = 0;
    //         intriParams.dstNzMatrixStride = 0;
    //     } else {
    //         intriParams.dnNum = hiLoadL1;
    //         intriParams.nValue = wiLoadL1;
    //         intriParams.srcDnMatrixStride = ctx.orgWi;
    //         intriParams.dstNzMatrixStride = wiLoadL1 * Intf::k0a;
    //     }

    //     if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
    //         intriParams.dValue = kAL1Iter == ctx.maxKAL1Iter ?
    //             (ctx.kAL1Tail / ctx.convTiling->kernelHxkernelW) : ctx.convTiling->cinAInCore;
    //     } else {
    //         intriParams.dValue = kAL1Iter == ctx.maxKAL1Iter ?
    //             ctx.convTiling->cinATailInCore : ctx.convTiling->cinAInCore;
    //     }
    //     intriParams.srcDValue = ctx.convTiling->oriHixWi;
    //     intriParams.dstNzC0Stride = hiLoadL1 * wiLoadL1;
    //     intriParams.dstNzNStride = 1;
    // }

    // [zj]TODO: api to support
    // __aicore__ inline void SetDn2NzIntriParamsC04(const typename Intf::ContextType &ctx, Dn2NzParams &intriParams)
    // {
    //     intriParams.dnNum = 1;
    //     intriParams.nValue = hiLoadL1 * wiLoadL1;
    //     intriParams.dValue = ctx.convTiling->singleCoreCi;
    //     intriParams.srcDnMatrixStride = 0;
    //     intriParams.srcDValue = ctx.convTiling->oriHixWi;
    //     intriParams.dstNzC0Stride = 0;
    //     intriParams.dstNzNStride = 1;
    //     intriParams.dstNzMatrixStride = 0;
    // }

    __aicore__ inline void LoadAl1Data(const typename Intf::ContextType &ctx, uint64_t kAL1Iter, uint64_t batchIter)
    {
        int64_t realHiTopGmIdx = hiTopPadIdx < 0 ? 0 : hiTopPadIdx;
        int64_t realWiTopGmIdx = wiLeftPadIdx < 0 ? 0 : wiLeftPadIdx;

        printf("[aL1]batchIter: %ld, hiTopPadIdx: %ld, wiLeftPadIdx: %ld, kAL1Iter: %ld\n", batchIter, hiTopPadIdx,
               wiLeftPadIdx, kAL1Iter);

        int64_t cinOffsetGM = (ctx.convTiling->kAL1 / ctx.convTiling->kernelHxkernelW) * ctx.convTiling->oriHixWi;

        if constexpr (Intf::formatA == ConvCommonApi::ConvFormat::NC1HWC0) {
            int64_t aL1GmOffset = kAL1Iter * cinOffsetGM +
                                  realHiTopGmIdx * ctx.orgWi * Intf::k0a +
                                  realWiTopGmIdx * Intf::k0a +
                                  batchIter * ctx.fmapOneBatchSize;
            // [zj]TODO: C04
            // [zj]TODO：OPT_GROUP_CONV
            uint32_t currentCi = kAL1Iter == ctx.maxKAL1Iter ? ctx.convTiling->cinATailInCore
                                                             : ctx.convTiling->cinAInCore;
            if (unlikely(ctx.convTiling->hoL1 == 1 || ctx.convTiling->woL1 == ctx.orgWo)) {
                DataCopyParams intriParams;
                intriParams.blockCount = ConvApi::CeilDIV(currentCi, Intf::k0a);
                intriParams.blockLen = hiLoadL1 * wiLoadL1;
                intriParams.srcStride = ctx.orgHi * ctx.orgWi - hiLoadL1 * wiLoadL1;
                intriParams.dstStride = 0;
                DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
                printf("[aL1]aL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                    aL1GmOffset, intriParams.blockCount, intriParams.blockLen,
                    intriParams.srcStride, intriParams.dstStride);
            } else {
                uint64_t currentC1 = ConvApi::CeilDIV(currentCi, Intf::k0a);
                for (uint64_t i = 0; i < currentC1; i++) {
                    DataCopyParams intriParams;
                    intriParams.blockCount = hiLoadL1;
                    intriParams.blockLen = wiLoadL1;
                    intriParams.srcStride = ctx.orgWi - wiLoadL1;
                    intriParams.dstStride = 0;
                    aL1GmOffset = aL1GmOffset + ctx.orgHi * ctx.orgWi * Intf::k0a;
                    DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
                    printf("[aL1]aL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                        aL1GmOffset, intriParams.blockCount, intriParams.blockLen,
                        intriParams.srcStride, intriParams.dstStride);
                }
            }
        } else if constexpr (Intf::formatA == ConvCommonApi::ConvFormat::NCHW) {
            int64_t aL1GmOffset = kAL1Iter * cinOffsetGM +
                                  realHiTopGmIdx * ctx.orgWi + realWiTopGmIdx +
                                  batchIter * ctx.fmapOneBatchSize;
            // [zj]TODO: api to support
            // Dn2NzParams intriParams;
            // if constexpr (Intf::formatB == ConvCommonApi::ConvFormat::FRACTAL_Z_C04) {
            //     SetDn2NzIntriParamsC04(ctx, intriParams);
            //     DataCopy<typename Intf::FmapT, true>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
            // } else {
            //     SetDn2NzIntriParams(ctx, intriParams, kAL1Iter);
            //     DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
            // }
        }
    }

    // [zj]TODO: API
    __aicore__ inline void SetMFMatrix(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t FM_W = 0;
        constexpr uint32_t FM_H = 16;
        constexpr uint32_t LFET_PAD = 32;
        constexpr uint32_t RIGHT_PAD = 40;
        constexpr uint32_t TOP_PAD = 48;
        constexpr uint32_t BOT_PAD = 56;

        uint64_t fmatrix = 0;
        fmatrix |= (uint64_t)wiLoadL1 << FM_W;
        fmatrix |= (uint64_t)hiLoadL1 << FM_H;
        fmatrix |= (uint64_t)padLeftL1 << LFET_PAD;
        fmatrix |= (uint64_t)padRightL1 << RIGHT_PAD;
        fmatrix |= (uint64_t)padTopL1 << TOP_PAD;
        fmatrix |= (uint64_t)padBottomL1 << BOT_PAD;
        printf("[aL1]fmatrix: %ld\n", fmatrix);
        set_m_fmatrix(fmatrix);
    }

    // [zj]TODO: API
    __aicore__ inline void SetFMStepPos(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t W_START_POS = 0;
        constexpr uint32_t H_START_POS = 16;
        constexpr uint32_t W_STEP = 32;
        constexpr uint32_t H_STEP = 48;

        uint64_t wStep = paddingWiAL1 - ctx.dilatedKernelW + 1;
        uint64_t hStep = paddingHiAL1 - ctx.dilatedKernelH + 1;
        wStep = (wStep - 1) / ctx.convTiling->strideW * ctx.convTiling->strideW + 1;
        hStep = (hStep - 1) / ctx.convTiling->strideH * ctx.convTiling->strideH + 1;

        uint64_t fmStepPos = 0;
        fmStepPos |= (uint64_t)(0) << W_START_POS;
        fmStepPos |= (uint64_t)(0) << H_START_POS;
        fmStepPos |= (uint64_t)wStep << W_STEP;
        fmStepPos |= (uint64_t)hStep << H_STEP;
        printf("[aL1]fmStepPos: %ld\n", fmStepPos);
        set_fm_step_pos(fmStepPos);
    }

    // [zj]TODO: API
    __aicore__ inline void SetMFMatrixDual0(const typename Intf::ContextType &ctx)
    {
        // [zj]TODO: support dual src
        constexpr uint32_t POS_SECOND_FM_ADDR = 0;
        constexpr uint32_t POS_SECOND_FM_H = 32;

        uint64_t fMatrixDual0;
        fMatrixDual0 = 0;
        fMatrixDual0 |= (uint64_t)0 << POS_SECOND_FM_ADDR;
        fMatrixDual0 |= (uint64_t)0 << POS_SECOND_FM_H;
        printf("[aL1]fMatrixDual0: %ld\n", fMatrixDual0);
        set_m_fmatrix_dual_0(fMatrixDual0);
    }

    // [zj]TODO: API
    __aicore__ inline void SetMFMatrixDual1(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t POS_FIRST_HWC0STRIDE = 0;
        constexpr uint32_t POS_SECOND_WC0STRIDE = 16;

        uint64_t fMatrixDual1;
        fMatrixDual1 = 0;
        fMatrixDual1 |= (uint64_t)hiLoadL1 * wiLoadL1 << POS_FIRST_HWC0STRIDE;
        fMatrixDual1 |= (uint64_t)0 << POS_SECOND_WC0STRIDE;
        printf("[aL1]fMatrixDual1: %ld\n", fMatrixDual1);
        set_m_fmatrix_dual_1(fMatrixDual1);
    }

    // [zj]TODO: API
    __aicore__ inline void SetMPadding(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t POS_PADDING_VALUE = 0;
        constexpr uint32_t POS_PADDING_CONTROL_MODE = 32;
        constexpr uint32_t POS_PADDING_VALUE_ADDR = 34;

        uint64_t mPadding = 0;
        // [zj]TODO: perchannel
        mPadding |= (uint64_t)ctx.convTiling->offsetx << POS_PADDING_VALUE;
        mPadding |= (uint64_t)0 << POS_PADDING_CONTROL_MODE;
        mPadding |= (uint64_t)0 << POS_PADDING_VALUE_ADDR;
        printf("[aL1]mPadding: %ld\n", mPadding);
        set_m_padding(mPadding);
    }

private:
    int64_t padTopL1;
    int64_t padLeftL1;
    int64_t padRightL1;
    int64_t hiTopPadIdx;
    int64_t wiLeftPadIdx;
    int64_t padBottomL1;
    int64_t hiBottomPadIdx;
    int64_t wiRightPadIdx;
    int64_t paddingHiAL1;
    int64_t paddingWiAL1;
    int64_t hiLoadL1;
    int64_t wiLoadL1;
};

template <class Intf>
class LoadAL0ToolsHWMode {
public:
    __aicore__ inline LoadAL0ToolsHWMode()
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
class CopyOutToolsHWMode {
public:
    __aicore__ inline CopyOutToolsHWMode()
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
        } else {
            if (ctx.woAL1Iter == ctx.maxWoL1Iter) {
                offsetW = ((ctx.woAL1Iter - 1) * ctx.convTiling->woL1 + ctx.woAL1Tail) +
                          ctx.woL0Iter * ctx.convTiling->woL0;
            } else {
                offsetW = ctx.woAL1Iter * ctx.convTiling->woL1 +
                          ctx.woL0Iter * ctx.convTiling->woL0;
            }
        }

        if constexpr (Intf::formatC == ConvCommonApi::ConvFormat::NC1HWC0) {
            uint64_t offset = offsetBatch +
                offsetH * ctx.orgWo * Intf::k0c + offsetW * Intf::k0c +
                offsetCout / Intf::k0c * valueHoWo_ * Intf::k0c +
                offsetCout % Intf::k0c;
            return offset;
        }

        if constexpr (Intf::formatC == ConvCommonApi::ConvFormat::NCHW) {
            uint64_t offset = offsetBatch + offsetH * ctx.orgWo + offsetW;
            offset += offsetCout * valueHoWo_;
            return offset;
        }

        return 0;
    }

    __aicore__ inline void CopyOut(const typename Intf::ContextType &ctx, const GlobalTensor<typename Intf::OutputT> &output)
    {
        uint64_t offset = CalcOffset(ctx);
        printf("[cL1]batchIter: %ld, mAL1Iter: %ld, mL0Iter: %ld, nBL1Iter: %ld, nL0Iter: %ld\n",
            ctx.batchIter, ctx.mAL1Iter, ctx.mL0Iter, ctx.nBL1Iter, ctx.nL0Iter);

        if constexpr (Intf::formatC == ConvCommonApi::ConvFormat::NC1HWC0) {
            if (unlikely(ctx.convTiling->hoL1 == 1 || ctx.convTiling->woL1 == ctx.orgWo)) {
                DataCopyParams intriParams;
                intriParams.blockCount = currentNL0_ / Intf::k0c;
                intriParams.blockLen = currentML0_;
                intriParams.srcStride = 0;
                intriParams.dstStride = valueHoWo_ - currentML0_;
                printf("[cL1]cL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                       offset, intriParams.blockCount, intriParams.blockLen,
                       intriParams.srcStride, intriParams.dstStride);
                DataCopy<typename Intf::OutputT>(output[offset], ctx.outputL1, intriParams);
                return;
            } else {
                uint64_t currentN1 = ConvApi::CeilDIV(currentNL0_, Intf::k0c);
                for (uint64_t i = 0; i < currentN1; i++) {
                    DataCopyParams intriParams;
                    intriParams.blockCount = ctx.currentHoL0;
                    intriParams.blockLen = ctx.currentWoL0;
                    intriParams.srcStride = ctx.orgWo - ctx.currentWoL0;
                    intriParams.dstStride = 0;
                    offset = offset + ctx.orgHo * ctx.orgWo * Intf::k0c;
                    printf("[cL1]cL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                       offset, intriParams.blockCount, intriParams.blockLen,
                       intriParams.srcStride, intriParams.dstStride);
                    DataCopy<typename Intf::OutputT>(output[offset], ctx.outputL1, intriParams);
                }
                return;
            }
        }

        if constexpr (Intf::formatC == ConvCommonApi::ConvFormat::NCHW) {
            // [zj]TODO：Fixpipe API
            return;
        }
    }

private:
    uint64_t valueHoWo_;
    uint64_t currentML0_;
    uint64_t currentNL0_;
};

};

#endif // API_CONV2D_INSTR_HW_MODE_IMPL_H
