/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*!
 * \file conv2d_instr_impl.h
 * \brief
 */

#ifndef CONV2D_INSTR_IMPL_H
#define CONV2D_INSTR_IMPL_H

#include "lib/conv/conv2d_config.h"
#include "../../conv2d_util.h"

namespace Conv2dFunc {
using namespace AscendC;
using namespace conv;

template <class Intf>
class LoadAL1ToolsHWMode {
public:
    __aicore__ inline LoadAL1ToolsHWMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
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
        // [zj]ToDo: padding offset
        // SetLoadDataPaddingValue(ctx.convTiling->offsetx);
        SetMPadding(ctx);
    }

private:
    __aicore__ inline void UpdatePaddingHiWiAL1(const typename Intf::ContextType &ctx)
    {
        // [zj]L1HiWi padding
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
        // [zj]padding
        int64_t hiRealStartPos = ctx.hiStartPos > 0 ? 0 : ctx.hiStartPos;
        // [zj]L1fmfmhiidxpaddinghiRealStartPos < 0L1fmpaddinghiTopPadIdx < 0
        hiTopPadIdx = hoAL1Iter * ctx.convTiling->hoL1 * ctx.convTiling->strideH + hiRealStartPos;
        if (unlikely(ctx.woL1SmallTail != 0 && woAL1Iter == ctx.maxWoL1Iter)) {
            wiLeftPadIdx = ((woAL1Iter - 1) * ctx.convTiling->woL1 + ctx.woAL1Tail) *
                                    ctx.convTiling->strideW - ctx.convTiling->padLeft;
        } else {
            // [zj]L1fmfmwiidxL1fmpaddingwiLeftPadIdx < 0
            wiLeftPadIdx = woAL1Iter * ctx.convTiling->woL1 * ctx.convTiling->strideW -
                            ctx.convTiling->padLeft;
        }
        // [zj]L1fmfmhiidxpaddingkernel
        hiBottomPadIdx = hiTopPadIdx + paddingHiAL1;
        // [zj]L1fmfmwiidxpaddingkernel
        wiRightPadIdx = wiLeftPadIdx + paddingWiAL1;
    }

    __aicore__ inline void UpdateHiWiAL1(const typename Intf::ContextType &ctx)
    {
        // [zj]padding
        // Calculate AL1 pads
        // [zj]L1fmpadding
        padTopL1 = hiTopPadIdx < 0 ? (0 - hiTopPadIdx) : 0;
        hiBottomPadIdx = ctx.hiStartPos > 0 ? hiBottomPadIdx + ctx.hiStartPos : hiBottomPadIdx;
        // [zj]L1fmorgHipadding
        padBottomL1 = hiBottomPadIdx > ctx.orgHi ? hiBottomPadIdx - ctx.orgHi : 0;
        // [zj]L1fmpadding
        padLeftL1 = wiLeftPadIdx < 0 ? (0 - wiLeftPadIdx) : 0;
        // [zj]L1fmorgWipadding
        padRightL1 = wiRightPadIdx > ctx.orgWi ? (wiRightPadIdx - ctx.orgWi) : 0;
        // Calculate hiAL1, wiAL1
        // [zj]L1fmpaddinghi
        hiLoadL1 = paddingHiAL1 - padTopL1 -padBottomL1;
        hiLoadL1 = hiLoadL1 > ctx.orgHi ? ctx.orgHi : hiLoadL1;
        // [zj]L1fmpaddingwi
        wiLoadL1 = paddingWiAL1 - padLeftL1 - padRightL1;
    }

    __aicore__ inline void SetDataCopyIntriParams(const typename Intf::ContextType &ctx, DataCopyParams &intriParams, uint64_t kAL1Iter)
    {
        intriParams.blockCount = CeilDIV(kAL1Iter == ctx.maxKAL1Iter ? ctx.convTiling->cinATailInCore
                                                                            : ctx.convTiling->cinAInCore,
            (C0_SIZE / sizeof(typename Intf::FmapT)));
        intriParams.blockLen = hiLoadL1 * wiLoadL1;
        intriParams.srcStride = ctx.orgHi * ctx.orgWi - hiLoadL1 * wiLoadL1;
        intriParams.dstStride = 0;
    }

    __aicore__ inline void SetDn2NzIntriParams(const typename Intf::ContextType &ctx, Dn2NzParams &intriParams, uint64_t kAL1Iter)
    {
        if (likely(wiLoadL1 == ctx.orgWi)) {
            intriParams.dnNum = 1;
            intriParams.nValue = hiLoadL1 * wiLoadL1;
            intriParams.srcDnMatrixStride = 0;
            intriParams.dstNzMatrixStride = 0;
        } else {
            intriParams.dnNum = hiLoadL1;
            intriParams.nValue = wiLoadL1;
            intriParams.srcDnMatrixStride = ctx.orgWi;
            intriParams.dstNzMatrixStride = wiLoadL1 * Intf::k0;
        }

        if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            intriParams.dValue = kAL1Iter == ctx.maxKAL1Iter ?
                (ctx.kAL1Tail / ctx.convTiling->kernelHxkernelW) : ctx.convTiling->cinAInCore;
        } else {
            intriParams.dValue = kAL1Iter == ctx.maxKAL1Iter ?
                ctx.convTiling->cinATailInCore : ctx.convTiling->cinAInCore;
        }
        intriParams.srcDValue = ctx.convTiling->oriHixWi;
        intriParams.dstNzC0Stride = hiLoadL1 * wiLoadL1;
        intriParams.dstNzNStride = 1;
    }

    __aicore__ inline void SetDn2NzIntriParamsC04(const typename Intf::ContextType &ctx, Dn2NzParams &intriParams)
    {
        intriParams.dnNum = 1;
        intriParams.nValue = hiLoadL1 * wiLoadL1;
        intriParams.dValue = ctx.convTiling->singleCoreCi;
        intriParams.srcDnMatrixStride = 0;
        intriParams.srcDValue = ctx.convTiling->oriHixWi;
        intriParams.dstNzC0Stride = 0;
        intriParams.dstNzNStride = 1;
        intriParams.dstNzMatrixStride = 0;
    }

    __aicore__ inline void LoadAl1Data(const typename Intf::ContextType &ctx, uint64_t kAL1Iter, uint64_t batchIter)
    {
        // Calculate aL1GmOffset
        int64_t realHiTopGmIdx = hiTopPadIdx < 0 ? 0 : hiTopPadIdx;
        int64_t realWiTopGmIdx = wiLeftPadIdx < 0 ? 0 : wiLeftPadIdx;

        printf("[aL1]batchIter: %ld, hiTopPadIdx: %ld, wiLeftPadIdx: %ld, kAL1Iter: %ld\n", batchIter, hiTopPadIdx,
               wiLeftPadIdx, kAL1Iter);

        if constexpr (Intf::formatA == ConvFormat::NC1HWC0) {
            // [zj]ToDokAL1IterC0
            int64_t aL1GmOffset = kAL1Iter * ctx.convTiling->cinOffsetBlockInGM +
                                  realHiTopGmIdx * ctx.orgWi * (C0_SIZE / sizeof(typename Intf::FmapT)) +
                                  realWiTopGmIdx * (C0_SIZE / sizeof(typename Intf::FmapT)) +
                                  batchIter * ctx.fmapOneBatchSize;
            // [zj]ToDo: C04
            // [zj]ToDoOPT_GROUP_CONV
            if (likely(wiLoadL1 == ctx.orgWi)) {
                DataCopyParams intriParams;
                SetDataCopyIntriParams(ctx, intriParams, kAL1Iter);
                DataCopy<typename Intf::FmapT>(ctx.al1, ctx.agm[aL1GmOffset], intriParams);
                printf("[aL1]aL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n", aL1GmOffset,
                   intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
                   intriParams.dstStride);
            } else {
                // [zj]ToDo
            }
        } else if constexpr (Intf::formatA == ConvFormat::NCHW) {
            int64_t aL1GmOffset = kAL1Iter * ctx.convTiling->cinOffsetBlockInGM +
                                  realHiTopGmIdx * ctx.orgWi + realWiTopGmIdx +
                                  batchIter * ctx.fmapOneBatchSize;
            Dn2NzParams intriParams;
            if constexpr (Intf::formatB == ConvFormat::FRACTAL_Z_C04) {
                SetDn2NzIntriParamsC04(ctx, intriParams);
                DataCopy<typename Intf::FmapT, true>(ctx.al1, ctx.agm[aL1GmOffset], intriParams);
            } else {
                SetDn2NzIntriParams(ctx, intriParams, kAL1Iter);
                DataCopy<typename Intf::FmapT>(ctx.al1, ctx.agm[aL1GmOffset], intriParams);
            }
        }
        // DumpAccChkPoint(ctx.al1, 6, 0,
        //                 CeilDIV(kAL1Iter == ctx.maxKAL1Iter ? ctx.convTiling->cinATailInCore
        //                                                            : ctx.convTiling->cinAInCore,
        //                         (C0_SIZE / sizeof(typename Intf::FmapT))) *
        //                     hiLoadL1 * wiLoadL1 * (C0_SIZE / sizeof(typename Intf::FmapT)));
    }

    // [zj]ToDo: API
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
        // [zj]ToDo:padding
        // fmatrix |= (uint64_t)MAX_PAD_R << BOT_PAD;
        fmatrix |= (uint64_t)padBottomL1 << BOT_PAD;
        printf("[aL1]fmatrix: %ld\n", fmatrix);
        set_m_fmatrix(fmatrix);
    }

    // [zj]ToDo: API
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

    // [zj]ToDo: API
    __aicore__ inline void SetMFMatrixDual0(const typename Intf::ContextType &ctx)
    {
        // [zj]ToDo: 
        constexpr uint32_t POS_SECOND_FM_ADDR = 0;
        constexpr uint32_t POS_SECOND_FM_H = 32;

        uint64_t fMatrixDual0;
        fMatrixDual0 = 0;
        fMatrixDual0 |= (uint64_t)0 << POS_SECOND_FM_ADDR;
        fMatrixDual0 |= (uint64_t)0 << POS_SECOND_FM_H;
        printf("[aL1]fMatrixDual0: %ld\n", fMatrixDual0);
        set_m_fmatrix_dual_0(fMatrixDual0);
    }

    // [zj]ToDo: API
    __aicore__ inline void SetMFMatrixDual1(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t POS_FIRST_HWC0STRIDE = 0;
        constexpr uint32_t POS_SECOND_WC0STRIDE = 16;

        uint64_t fMatrixDual1;
        fMatrixDual1 = 0;
        // fMatrixDual1 |= (uint64_t)paddingHiAL1 * paddingWiAL1 << POS_FIRST_HWC0STRIDE;
        fMatrixDual1 |= (uint64_t)hiLoadL1 * wiLoadL1 << POS_FIRST_HWC0STRIDE;
        fMatrixDual1 |= (uint64_t)0 << POS_SECOND_WC0STRIDE;
        printf("[aL1]fMatrixDual1: %ld\n", fMatrixDual1);
        set_m_fmatrix_dual_1(fMatrixDual1);
    }

    // [zj]ToDo: API
    __aicore__ inline void SetMPadding(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t POS_PADDING_VALUE = 0;
        constexpr uint32_t POS_PADDING_CONTROL_MODE = 32;
        constexpr uint32_t POS_PADDING_VALUE_ADDR = 34;

        uint64_t mPadding = 0;
        // [zj]ToDo
        mPadding |= (uint64_t)ctx.convTiling->offsetx << POS_PADDING_VALUE;
        mPadding |= (uint64_t)0 << POS_PADDING_CONTROL_MODE;
        mPadding |= (uint64_t)0 << POS_PADDING_VALUE_ADDR;
        printf("[aL1]mPadding: %ld\n", mPadding);
        set_m_padding(mPadding);
    }

private:
    // typename Intf::ContextType *ctx_;

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
class LoadAL1ToolsMMode {
public:
    __aicore__ inline LoadAL1ToolsMMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
    }

    __aicore__ inline void LoadAL1(const typename Intf::ContextType &ctx)
    {
        LoadAL1(ctx, ctx.kAL1Iter, ctx.mAL1Iter, ctx.batchIter);
        SetSpr(ctx);
    }

    __aicore__ inline void LoadAL1(const typename Intf::ContextType &ctx, uint64_t kAL1Iter, uint64_t mAL1Iter, uint64_t batchIter)
    {
        printf("[aL1]mAL1UpdateFlag: %ld\n", ctx.mAL1UpdateFlag);
        if (unlikely(ctx.mAL1UpdateFlag)) {
            CalcHiL1Pad(ctx, mAL1Iter);
        }
        printf("[aL1]batchIter: %ld, mAL1Iter: %ld, hiIdx: %ld, kAL1Iter: %ld\n", batchIter, mAL1Iter, hiIdx, kAL1Iter);
        if constexpr (Intf::formatA == ConvFormat::NC1HWC0) {
            // [zj]ToDokAL1IterC0
            uint64_t aL1GmOffset = kAL1Iter * ctx.convTiling->cinOffsetBlockInGM +
                                   hiIdx * ctx.orgWi * (C0_SIZE / sizeof(typename Intf::FmapT)) +
                                   batchIter * ctx.fmapOneBatchSize;
            // [zj]ToDoOPT_GROUP_CONV
            DataCopyParams intriParams;
            SetDataCopyIntriParams(ctx, intriParams, kAL1Iter);
            printf("[aL1]aL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                   aL1GmOffset, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
                   intriParams.dstStride);
            DataCopy<typename Intf::FmapT>(ctx.al1, ctx.agm[aL1GmOffset], intriParams);
        } else if constexpr (Intf::formatA == ConvFormat::NCHW) {
            uint64_t aL1GmOffset = kAL1Iter * ctx.convTiling->cinOffsetBlockInGM +
                                   hiIdx * ctx.orgWi + batchIter * ctx.fmapOneBatchSize;
            Dn2NzParams intriParams;
            SetDn2NzIntriParams(ctx, intriParams, kAL1Iter);
            DataCopy<typename Intf::FmapT>(ctx.al1, ctx.agm[aL1GmOffset], intriParams);
        }
        // DumpAccChkPoint(ctx.al1, 6, 0,
        //                 CeilDIV(kAL1Iter == ctx.maxKAL1Iter ? ctx.convTiling->cinATailInCore
        //                                                            : ctx.convTiling->cinAInCore,
        //                         (C0_SIZE / sizeof(typename Intf::FmapT))) *
        //                     hiLoadL1 * ctx.orgWi * (C0_SIZE / sizeof(typename Intf::FmapT)));
    }

    __aicore__ inline void SetSpr(const typename Intf::ContextType &ctx)
    {
        printf("[aL1]mAL1UpdateFlag: %ld\n", ctx.mAL1UpdateFlag);
        if (unlikely(ctx.mAL1UpdateFlag)) {
            SetMFMatrix(ctx);
            SetFMStepPos(ctx);
            SetMFMatrixDual0(ctx);
            SetMFMatrixDual1(ctx);
            // [zj]ToDo: padding offset
            // SetLoadDataPaddingValue(ctx.convTiling->offsetx);
            SetMPadding(ctx);
            // ctx.mAL1UpdateFlag = false;
        }
    }

private:
    __aicore__ inline void SetDataCopyIntriParams(const typename Intf::ContextType &ctx, DataCopyParams &intriParams, uint64_t kAL1Iter)
    {
        intriParams.blockCount = CeilDIV(kAL1Iter == ctx.maxKAL1Iter ? ctx.convTiling->cinATailInCore
                                                                            : ctx.convTiling->cinAInCore,
            (C0_SIZE / sizeof(typename Intf::FmapT)));
        intriParams.blockLen = hiLoadL1 * ctx.orgWi;
        intriParams.srcStride = ctx.orgHi * ctx.orgWi - hiLoadL1 * ctx.orgWi;
        intriParams.dstStride = 0;
    }

    __aicore__ inline void CalcHiL1Pad(const typename Intf::ContextType &ctx, uint64_t mAL1Iter)
    {
        padTopL1 = 0;
        padBottomL1 = 0;

        // [zj]mgm
        uint64_t currentM = ctx.mStartPos + mAL1Iter * ctx.mAL1;
        uint64_t currentML1 = IsMAL1Tail(ctx, mAL1Iter) ? ctx.mAL1Tail : ctx.mAL1;
        uint64_t hoStartIdx = currentM / ctx.orgWo;
        uint64_t hoEndIdx = CeilDIV(currentM + currentML1, ctx.orgWo);
        hiLoadL1 = ((hoEndIdx - hoStartIdx) - 1) * ctx.convTiling->strideH + ctx.dilatedKernelH;

        uint64_t hiStartIdxWithPad = hoStartIdx * ctx.convTiling->strideH;
        uint64_t hiEndIdxWithPad = hiStartIdxWithPad + hiLoadL1;
        hiIdx = hiStartIdxWithPad - ctx.convTiling->padTop;

        if (hiStartIdxWithPad < ctx.convTiling->padTop) {
            hiIdx = 0;
            padTopL1 = ctx.convTiling->padTop - hiStartIdxWithPad;
            hiLoadL1 -= padTopL1;
        }

        if (hiEndIdxWithPad > ctx.orgHi + ctx.convTiling->padTop) {
            padBottomL1 = hiEndIdxWithPad - (ctx.orgHi + ctx.convTiling->padTop);
            hiLoadL1 -= padBottomL1;
        }
    }

    __aicore__ inline void SetDn2NzIntriParams(const typename Intf::ContextType &ctx, Dn2NzParams &intriParams, uint64_t kAL1Iter)
    {
        uint64_t aL1Mi = hiLoadL1 * ctx.orgWi;
        intriParams.dnNum = 1;
        intriParams.nValue = aL1Mi;
        if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            intriParams.dValue = IsKAL1Tail(kAL1Iter) ?
                (ctx.kAL1Tail / ctx.convTiling->kernelHxkernelW) : ctx.convTiling->cinAInCore;
        } else {
            intriParams.dValue = IsKAL1Tail(kAL1Iter) ?
                ctx.convTiling->cinATailInCore : ctx.convTiling->cinAInCore;
        }
        intriParams.srcDValue = ctx.convTiling->oriHixWi;
        intriParams.dstNzC0Stride = aL1Mi;
        intriParams.dstNzNStride = 1;
    }

    // [zj]ToDo: API
    __aicore__ inline void SetMFMatrix(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t FM_W = 0;
        constexpr uint32_t FM_H = 16;
        constexpr uint32_t LFET_PAD = 32;
        constexpr uint32_t RIGHT_PAD = 40;
        constexpr uint32_t TOP_PAD = 48;
        constexpr uint32_t BOT_PAD = 56;

        uint64_t fmatrix = 0;
        fmatrix |= (uint64_t)ctx.orgWi << FM_W;
        fmatrix |= (uint64_t)hiLoadL1 << FM_H;
        fmatrix |= (uint64_t)ctx.convTiling->padLeft << LFET_PAD;
        fmatrix |= (uint64_t)ctx.convTiling->padRight << RIGHT_PAD;
        fmatrix |= (uint64_t)padTopL1 << TOP_PAD;
        // [zj]ToDo:padding
        // fmatrix |= (uint64_t)MAX_PAD_R << BOT_PAD;
        fmatrix |= (uint64_t)padBottomL1 << BOT_PAD;
        printf("[aL1]fmatrix: %ld\n", fmatrix);
        set_m_fmatrix(fmatrix);
    }

    // [zj]ToDo: API
    __aicore__ inline void SetFMStepPos(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t W_START_POS = 0;
        constexpr uint32_t H_START_POS = 16;
        constexpr uint32_t W_STEP = 32;
        constexpr uint32_t H_STEP = 48;

        uint64_t wStep = ctx.orgWi + ctx.convTiling->padLeft + ctx.convTiling->padRight -
                         ctx.dilatedKernelW + 1;
        uint64_t hStep = hiLoadL1 + padTopL1 + padBottomL1 - ctx.dilatedKernelH + 1;
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

    // [zj]ToDo: API
    __aicore__ inline void SetMFMatrixDual0(const typename Intf::ContextType &ctx)
    {
        // [zj]ToDo: 
        constexpr uint32_t POS_SECOND_FM_ADDR = 0;
        constexpr uint32_t POS_SECOND_FM_H = 32;

        uint64_t fMatrixDual0;
        fMatrixDual0 = 0;
        fMatrixDual0 |= (uint64_t)0 << POS_SECOND_FM_ADDR;
        fMatrixDual0 |= (uint64_t)0 << POS_SECOND_FM_H;
        printf("[aL1]fMatrixDual0: %ld\n", fMatrixDual0);
        set_m_fmatrix_dual_0(fMatrixDual0);
    }

    // [zj]ToDo: API
    __aicore__ inline void SetMFMatrixDual1(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t POS_FIRST_HWC0STRIDE = 0;
        constexpr uint32_t POS_SECOND_WC0STRIDE = 16;

        uint64_t fMatrixDual1;
        fMatrixDual1 = 0;
        // fMatrixDual1 |=
        //     (uint64_t)(ctx.orgWi + ctx.convTiling->padLeft + ctx.convTiling->padRight) *
        //         (hiLoadL1 + padTopL1 + padBottomL1)
        //     << POS_FIRST_HWC0STRIDE;
        fMatrixDual1 |= (uint64_t)hiLoadL1 * ctx.orgWi << POS_FIRST_HWC0STRIDE;
        fMatrixDual1 |= (uint64_t)0 << POS_SECOND_WC0STRIDE;
        printf("[aL1]fMatrixDual1: %ld\n", fMatrixDual1);
        set_m_fmatrix_dual_1(fMatrixDual1);
    }

    // [zj]ToDo: API
    __aicore__ inline void SetMPadding(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t POS_PADDING_VALUE = 0;
        constexpr uint32_t POS_PADDING_CONTROL_MODE = 32;
        constexpr uint32_t POS_PADDING_VALUE_ADDR = 34;

        uint64_t mPadding = 0;
        // [zj]ToDo
        mPadding |= (uint64_t)ctx.convTiling->offsetx << POS_PADDING_VALUE;
        mPadding |= (uint64_t)0 << POS_PADDING_CONTROL_MODE;
        mPadding |= (uint64_t)0 << POS_PADDING_VALUE_ADDR;
        printf("[aL1]mPadding: %ld\n", mPadding);
        set_m_padding(mPadding);
    }

    __aicore__ inline bool IsMAL1Tail(const typename Intf::ContextType &ctx, uint64_t mAL1Iter)
    {
        return mAL1Iter == ctx.maxMAL1Iter;
    }

    __aicore__ inline bool IsKAL1Tail(const typename Intf::ContextType &ctx, uint64_t kAL1Iter)
    {
        return kAL1Iter == ctx.maxKAL1Iter;
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t hiLoadL1;
    uint64_t padTopL1;
    uint64_t padBottomL1;
    uint64_t hiIdx;
};

template <class Intf>
class LoadBL1Tools {
public:
    __aicore__ inline LoadBL1Tools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
        maxNBL1Iter_ = ctx.maxNBL1Iter;
        maxKBL1Iter_ = ctx.maxKBL1Iter;
        orgCo1 = static_cast<uint32_t>(CeilDIV(ctx.convTiling->orgCo, BLOCK_L0_N));
        isOneByOneKernelScenario = ctx.convTiling->kernelHxkernelW  == 1 /*&&
                                   !(AscendC::IsSameType<typename Intf::WeightT, hifloat8_t>::value)*/ &&
                                   !(AscendC::IsSameType<typename Intf::WeightT, float8_e4m3_t>::value);
    }

    __aicore__ inline void LoadBL1(const typename Intf::ContextType &ctx)
    {
        LoadBL1(ctx, ctx.kBL1Iter, ctx.nBL1Iter);
    }

    __aicore__ inline void LoadBL1(const typename Intf::ContextType &ctx, uint64_t kBL1Iter, uint64_t nBL1Iter)
    {
        printf("[bL1]kBL1Iter: %ld, nBL1Iter: %ld\n", kBL1Iter, nBL1Iter);
        uint64_t bL1GmOffset;
        bool isLastNBL1;
        uint64_t currentNBL1 = ctx.currentNBL1;
        if constexpr (!Intf::hasNL1IterFlag) {
            bL1GmOffset = kBL1Iter * ctx.convTiling->kBL1;
            isLastNBL1 = true;
        } else {
            bL1GmOffset = nBL1Iter * ctx.convTiling->nBL1 * ctx.convTiling->coutOffsetBlock +
                kBL1Iter * ctx.convTiling->kBL1;
            isLastNBL1 = nBL1Iter == maxNBL1Iter_;
            currentNBL1 = isLastNBL1 ? ctx.nBL1Tail : ctx.convTiling->nBL1;
        }

        if constexpr (Intf::formatB == ConvFormat::FRACTAL_Z_C04) {
            uint64_t mStep = isLastNBL1 ? CeilDIV(ctx.nBL1Tail, BLOCK_L0_N) :
                                          ctx.convTiling->weightKStride;
            LoadData2DParamsV2 loadData2dParams;
            loadData2dParams.mStartPosition = nBL1Iter * ctx.convTiling->nL1DivBlockSize;
            loadData2dParams.kStartPosition = 0;
            loadData2dParams.mStep = mStep;
            loadData2dParams.kStep = ctx.convTiling->kBL1 / Intf::k0b;
            loadData2dParams.srcStride = orgCo1;
            loadData2dParams.dstStride = ctx.convTiling->nL1DivBlockSize;
            LoadData<typename Intf::WeightT>(ctx.bl1, ctx.bgm, loadData2dParams);
            return;
        }
        bool isLastKBL1 = (kBL1Iter == maxKBL1Iter_);
        uint64_t currentKBL1 = isLastKBL1 ? ctx.kBL1Tail : ctx.convTiling->kBL1;
        if constexpr (Intf::formatB == ConvFormat::FRACTAL_Z) {
            // [zj]ToDo: check
            bL1GmOffset = nBL1Iter * ctx.convTiling->nBL1 * (C0_SIZE / sizeof(typename Intf::WeightT)) +
                          (kBL1Iter * ctx.convTiling->kBL1) / (C0_SIZE / sizeof(typename Intf::WeightT)) *
                              ctx.convTiling->orgCo +
                          (kBL1Iter * ctx.convTiling->kBL1) % (C0_SIZE / sizeof(typename Intf::WeightT));
            DataCopyParams intriParams;
            intriParams.blockCount = currentKBL1 / (C0_SIZE / sizeof(typename Intf::WeightT));
            intriParams.blockLen = currentNBL1;
            intriParams.srcStride = ctx.convTiling->orgCo - currentNBL1;
            intriParams.dstStride = 0;
            printf("[bL1]bL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n", bL1GmOffset,
                   intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
                   intriParams.dstStride);
            DataCopy<typename Intf::WeightT>(ctx.bl1, ctx.bgm[bL1GmOffset], intriParams);
            // DumpAccChkPoint(ctx.bl1, 7, 0, currentNBL1 * currentKBL1);
            return;
        }
        uint64_t dValue = isLastKBL1 ? ctx.convTiling->cinBTailInCore : ctx.convTiling->cinBInCore;
        if (isOneByOneKernelScenario) {
            Nd2NzParams intriParams;
            intriParams.ndNum = 1;
            intriParams.nValue = currentNBL1;
            intriParams.dValue = currentKBL1;
            intriParams.srcNdMatrixStride = 0;
            intriParams.srcDValue = ctx.orgCi / ctx.convTiling->groups;
            intriParams.dstNzC0Stride = ctx.convTiling->nBL1;
            intriParams.dstNzNStride = 1;
            intriParams.dstNzMatrixStride = 0;
            DataCopy<typename Intf::WeightT>(ctx.bl1, ctx.bgm[bL1GmOffset], intriParams);
            // DumpAccChkPoint(ctx.bl1, 7, 0, currentNBL1 * currentKBL1);
            return;
        }
        Dn2NzParams intriParams;
        intriParams.dnNum = currentNBL1;
        intriParams.nValue = ctx.convTiling->kernelHxkernelW;
        intriParams.dValue = dValue;
        intriParams.srcDnMatrixStride =  ctx.convTiling->coutOffsetBlock;
        intriParams.srcDValue = ctx.convTiling->kernelHxkernelW;
        intriParams.dstNzC0Stride = ctx.convTiling->kernelHxkernelW * ctx.convTiling->nBL1;
        intriParams.dstNzNStride = ctx.convTiling->nBL1;
        intriParams.dstNzMatrixStride = Intf::k0;
        DataCopy<typename Intf::WeightT>(ctx.bl1, ctx.bgm[bL1GmOffset], intriParams);
        // DumpAccChkPoint(ctx.bl1, 7, 0, currentNBL1 * currentKBL1);
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t maxNBL1Iter_;
    uint64_t maxKBL1Iter_;
    uint32_t orgCo1;
    bool isOneByOneKernelScenario;
};

};

#endif // CONV2D_INSTR_IMPL_H