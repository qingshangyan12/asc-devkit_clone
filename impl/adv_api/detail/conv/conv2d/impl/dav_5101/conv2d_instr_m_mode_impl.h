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
 * \file conv2d_intr_m_mode_impl.h
 * \brief
 */

#ifndef API_CONV2D_INSTR_M_MODE_IMPL_H
#define API_CONV2D_INSTR_M_MODE_IMPL_H

#include "lib/conv/conv2d_config.h" // #include "../../../../../include/adv_api/conv/conv2d/conv2d_config.h"  // [zj]TODO
#include "../../conv2d_util.h"

namespace Conv2dApiFunc {
using namespace AscendC;

template <class Intf>
class CopyEltwiseToolsMMode {
public:
    __aicore__ inline CopyEltwiseToolsMMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        valueHoWo_ = ctx.orgHo * ctx.orgWo;
    }

    __aicore__ inline void SetMN(const typename Intf::ContextType &ctx, uint64_t m, uint64_t n)
    {
        currentML0_ = m;
        currentNL0_ = n;
    }

    __aicore__ inline uint64_t CalcOffset(const typename Intf::ContextType &ctx)
    {
        printf("[eltwiseL1]batchIter: %ld, mAL1Iter: %ld, mL0Iter: %ld, nBL1Iter: %ld, nL0Iter: %ld\n",
               ctx.batchIter, ctx.mAL1Iter, ctx.mL0Iter, ctx.nBL1Iter, ctx.nL0Iter);
        uint64_t offsetCout = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                              ctx.nL0Iter * ctx.convTiling->nL0;
        uint64_t offset = (ctx.mAL1Iter * ctx.mAL1 + ctx.mL0Iter * ctx.mL0) * Intf::k0e +
                          ctx.batchIter * ctx.outputOneBatchSize +
                          offsetCout / Intf::k0e * valueHoWo_ * Intf::k0e + offsetCout % Intf::k0e;
        return offset;
    }

    __aicore__ inline void CopyElt(const typename Intf::ContextType &ctx)
    {
        uint64_t offset = CalcOffset(ctx);
        DataCopyParams intriParams;
        intriParams.blockCount = currentNL0_ / Intf::k0e;
        intriParams.blockLen = currentML0_;
        intriParams.srcStride = valueHoWo_ - currentML0_;
        intriParams.dstStride = 0;
        printf("[eltwiseL1]eltwiseL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
               offset, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
        DataCopy<typename Intf::EltwiseT>(ctx.eltwiseL1, ctx.eltwiseGm[offset], intriParams);
    }

private:
    uint64_t valueHoWo_;
    uint64_t currentML0_;
    uint64_t currentNL0_;
};

template <class Intf>
class LoadAL1ToolsMMode {
public:
    __aicore__ inline LoadAL1ToolsMMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        (void)ctx;
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

        int64_t cinOffsetGM = (ctx.convTiling->kAL1 / ctx.convTiling->kernelHxkernelW) * ctx.convTiling->oriHixWi;

        if constexpr (Intf::formatA == ConvFormat::NC1HWC0) {
            uint64_t aL1GmOffset = kAL1Iter * cinOffsetGM +
                                   hiIdx * ctx.orgWi * Intf::k0b +
                                   batchIter * ctx.fmapOneBatchSize;
            DataCopyParams intriParams;
            SetDataCopyIntriParams(ctx, intriParams, kAL1Iter);
            printf("[aL1]aL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                   aL1GmOffset, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
                   intriParams.dstStride);
            DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
        } else if constexpr (Intf::formatA == ConvFormat::NCHW) {
            uint64_t aL1GmOffset = kAL1Iter * cinOffsetGM +
                                   hiIdx * ctx.orgWi +
                                   batchIter * ctx.fmapOneBatchSize;
            Dn2NzParams intriParams;
            SetDn2NzIntriParams(ctx, intriParams, kAL1Iter);
            DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
        }
    }

    __aicore__ inline void SetSpr(const typename Intf::ContextType &ctx)
    {
        printf("[aL1]mAL1UpdateFlag: %ld\n", ctx.mAL1UpdateFlag);
        if (unlikely(ctx.mAL1UpdateFlag)) {
            SetMFMatrix(ctx);
            SetFMStepPos(ctx);
            SetMFMatrixDual0(ctx);
            SetMFMatrixDual1(ctx);
            // [zj]TODO: padding offset
            // SetLoadDataPaddingValue(ctx.convTiling->offsetx);
            SetMPadding(ctx);
        }
    }

private:
    __aicore__ inline bool IsMAL1Tail(const typename Intf::ContextType &ctx, uint64_t mAL1Iter)
    {
        return mAL1Iter == ctx.maxMAL1Iter;
    }

    __aicore__ inline void CalcHiL1Pad(const typename Intf::ContextType &ctx, uint64_t mAL1Iter)
    {
        padTopL1 = 0;
        padBottomL1 = 0;

        uint64_t currentMPos = ctx.mStartPos + mAL1Iter * ctx.mAL1;
        uint64_t currentML1 = IsMAL1Tail(ctx, mAL1Iter) ? ctx.mAL1Tail : ctx.mAL1;
        uint64_t hoStartIdx = currentMPos / ctx.orgWo;
        uint64_t hoEndIdx = CeilDIV(currentMPos + currentML1, ctx.orgWo);
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

    __aicore__ inline void SetDataCopyIntriParams(const typename Intf::ContextType &ctx, DataCopyParams &intriParams, uint64_t kAL1Iter)
    {
        intriParams.blockCount = CeilDIV(kAL1Iter == ctx.maxKAL1Iter ? ctx.convTiling->cinATailInCore
                                                                     : ctx.convTiling->cinAInCore,
            Intf::k0b);
        intriParams.blockLen = hiLoadL1 * ctx.orgWi;
        intriParams.srcStride = ctx.orgHi * ctx.orgWi - hiLoadL1 * ctx.orgWi;
        intriParams.dstStride = 0;
    }

    __aicore__ inline bool IsKAL1Tail(const typename Intf::ContextType &ctx, uint64_t kAL1Iter)
    {
        return kAL1Iter == ctx.maxKAL1Iter;
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
        fmatrix |= (uint64_t)ctx.orgWi << FM_W;
        fmatrix |= (uint64_t)hiLoadL1 << FM_H;
        fmatrix |= (uint64_t)ctx.convTiling->padLeft << LFET_PAD;
        fmatrix |= (uint64_t)ctx.convTiling->padRight << RIGHT_PAD;
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
        fMatrixDual1 |= (uint64_t)hiLoadL1 * ctx.orgWi << POS_FIRST_HWC0STRIDE;
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
        // [zj]TODO：perchannel
        mPadding |= (uint64_t)ctx.convTiling->offsetx << POS_PADDING_VALUE;
        mPadding |= (uint64_t)0 << POS_PADDING_CONTROL_MODE;
        mPadding |= (uint64_t)0 << POS_PADDING_VALUE_ADDR;
        printf("[aL1]mPadding: %ld\n", mPadding);
        set_m_padding(mPadding);
    }

private:
    uint64_t hiLoadL1;
    uint64_t padTopL1;
    uint64_t padBottomL1;
    uint64_t hiIdx;
};

template <class Intf>
class LoadAL0ToolsMMode {
public:
    __aicore__ inline LoadAL0ToolsMMode()
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
class CopyOutToolsMMode {
public:
    __aicore__ inline CopyOutToolsMMode()
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
        printf("[cL1]batchIter: %ld, mAL1Iter: %ld, mL0Iter: %ld, nBL1Iter: %ld, nL0Iter: %ld\n",
            ctx.batchIter, ctx.mAL1Iter, ctx.mL0Iter, ctx.nBL1Iter, ctx.nL0Iter);
        uint64_t offsetCout = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                              ctx.nL0Iter * ctx.convTiling->nL0;

        if constexpr (Intf::formatC == ConvFormat::NC1HWC0) {
            uint64_t offset = (ctx.mAL1Iter * ctx.mAL1 + ctx.mL0Iter * ctx.mL0) * Intf::k0c +
                ctx.batchIter * ctx.outputOneBatchSize +
                offsetCout / Intf::k0c * valueHoWo_ * Intf::k0c +
                offsetCout % Intf::k0c;
            return offset;
        }

        if constexpr (Intf::formatC == ConvFormat::NCHW) {
            uint64_t offset = ctx.mAL1Iter * ctx.mAL1 + ctx.mL0Iter * ctx.mL0 +
                            ctx.batchIter * ctx.outputOneBatchSize;
            offset += offsetCout * valueHoWo_;
            return offset;
        }

        return 0;
    }

    __aicore__ inline void CopyOut(const typename Intf::ContextType &ctx, const GlobalTensor<typename Intf::OutputT> &output)
    {
        uint64_t offset = CalcOffset(ctx);

        if constexpr (Intf::formatC == ConvFormat::NC1HWC0) {
            DataCopyParams intriParams;
            intriParams.blockCount = currentNL0_ / Intf::k0c;
            intriParams.blockLen = currentML0_;
            intriParams.srcStride = 0;
            intriParams.dstStride = valueHoWo_ - currentML0_;
            printf("[cL1]cL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n", offset,
                   intriParams.blockCount, intriParams.blockLen,
                   intriParams.srcStride, intriParams.dstStride);
            DataCopy<typename Intf::OutputT>(output[offset], ctx.cl1, intriParams);
            return;
        }

        if constexpr (Intf::formatC == ConvFormat::NCHW) {
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

#endif // API_CONV2D_INSTR_M_MODE_IMPL_H
