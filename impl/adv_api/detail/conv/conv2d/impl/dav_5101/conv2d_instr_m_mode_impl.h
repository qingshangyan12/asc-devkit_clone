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

#include "lib/conv/conv2d/conv2d_config.h" // #include "../../../../../include/adv_api/conv/conv2d/conv2d_config.h"  // [zj]TODO
#include "../../conv2d_util.h"

namespace Conv2dApi { // namespace Conv2dApiFunc { // [zj]TODO: just for cpu
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
        uint64_t offset = (ctx.mAL1Iter * ctx.mAL1 + ctx.mL0Iter * ctx.mL0) * Intf::k0Elt +
                          ctx.batchIter * ctx.outputOneBatchSize +
                          offsetCout / Intf::k0Elt * valueHoWo_ * Intf::k0Elt +
                          offsetCout % Intf::k0Elt;
        return offset;
    }

    __aicore__ inline void CopyElt(const typename Intf::ContextType &ctx)
    {
        uint64_t offset = CalcOffset(ctx);

        DataCopyExtParams intriParams;
        intriParams.blockCount = currentNL0_ / Intf::k0Elt;
        intriParams.blockLen = currentML0_;
        intriParams.srcStride = valueHoWo_ - currentML0_;
        intriParams.dstStride = 0;

        DataCopyPadExtParams<typename Intf::EltwiseT> padParams;
        padParams.isPad = false;
        padParams.leftPadding = 0;
        padParams.rightPadding = 0;
        padParams.paddingValue = 0;

        printf("[eltwiseL1]eltwiseL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
               offset, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
        DataCopy<typename Intf::EltwiseT, PaddingMode::Normal>(ctx.eltwiseL1,
                                                               ctx.eltwiseGm[offset],
                                                               intriParams,
                                                               padParams);
    }

private:
    uint64_t valueHoWo_;
    uint64_t currentML0_;
    uint64_t currentNL0_;
};

template <class Intf>
class LoadFmapL1ToolsMMode {
public:
    __aicore__ inline LoadFmapL1ToolsMMode()
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

    __aicore__ inline void LoadAL1(const typename Intf::ContextType &ctx,
                                   uint64_t kAL1Iter,
                                   uint64_t mAL1Iter,
                                   uint64_t batchIter)
    {
        printf("[fmapL1]mAL1UpdateFlag: %ld\n", ctx.mAL1UpdateFlag);
        if (unlikely(ctx.mAL1UpdateFlag)) {
            CalcHiL1Pad(ctx, mAL1Iter);
        }

        printf("[fmapL1]batchIter: %ld, mAL1Iter: %ld, hiIdx: %ld, kAL1Iter: %ld\n",
        batchIter, mAL1Iter, hiIdx, kAL1Iter);

        cinOffsetGM = (ctx.convTiling->kAL1 / ctx.convTiling->kernelHxkernelW) * ctx.convTiling->oriHixWi;

        if constexpr (Intf::formatFmap == ConvCommonApi::ConvFormat::NC1HWC0) {
            LoadAL1NC1HWC0(ctx, kAL1Iter, mAL1Iter, batchIter);
        } else if constexpr (Intf::formatFmap == ConvCommonApi::ConvFormat::NCHW) {
            LoadAL1NCHW(ctx, kAL1Iter, mAL1Iter, batchIter);
        } else if constexpr (Intf::formatFmap == ConvCommonApi::ConvFormat::NHWC) {
            LoadAL1NHWC(ctx, kAL1Iter, mAL1Iter, batchIter);
        } else if constexpr (Intf::formatFmap == ConvCommonApi::ConvFormat::NC1HWC04) {
            LoadAL1NC1HWC04(ctx, kAL1Iter, mAL1Iter, batchIter);
        } else {
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported format"); });
        }
    }

    __aicore__ inline void SetSpr(const typename Intf::ContextType &ctx)
    {
        printf("[fmapL1]mAL1UpdateFlag: %ld\n", ctx.mAL1UpdateFlag);
        if (unlikely(ctx.mAL1UpdateFlag)) {
            SetMFMatrix(ctx);
            SetFMStepPos(ctx);
            SetMFMatrixDual0(ctx);
            SetMFMatrixDual1(ctx);
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
        uint64_t hoEndIdx = ConvApi::CeilDIV(currentMPos + currentML1, ctx.orgWo);
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

    __aicore__ inline bool IsKAL1Tail(const typename Intf::ContextType &ctx, uint64_t kAL1Iter)
    {
        return kAL1Iter == ctx.maxKAL1Iter;
    }

    __aicore__ inline void LoadAL1NC1HWC0(const typename Intf::ContextType &ctx,
                                          uint64_t kAL1Iter,
                                          uint64_t mAL1Iter,
                                          uint64_t batchIter)
    {
        uint64_t aL1GmOffset = kAL1Iter * cinOffsetGM +
                               hiIdx * ctx.orgWi * Intf::k0Fmap +
                               batchIter * ctx.fmapOneBatchSize;

        DataCopyExtParams intriParams;
        intriParams.blockCount = ConvApi::CeilDIV(kAL1Iter == ctx.maxKAL1Iter
                                                      ? ctx.convTiling->cinATailInCore
                                                      : ctx.convTiling->cinAInCore,
                                                  Intf::k0Fmap);
        intriParams.blockLen = hiLoadL1 * ctx.orgWi;
        intriParams.srcStride = ctx.orgHi * ctx.orgWi - hiLoadL1 * ctx.orgWi;
        intriParams.dstStride = 0;

        DataCopyPadExtParams<typename Intf::FmapT> padParams;
        padParams.isPad = false;
        padParams.leftPadding = 0;
        padParams.rightPadding = 0;
        padParams.paddingValue = 0;

        printf("[fmapL1]aL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                aL1GmOffset, intriParams.blockCount, intriParams.blockLen,
                intriParams.srcStride, intriParams.dstStride);
        DataCopy<typename Intf::FmapT, PaddingMode::Normal>(ctx.fmapL1,
                                                            ctx.fmapGm[aL1GmOffset],
                                                            intriParams,
                                                            padParams);
    }

    __aicore__ inline void LoadAL1NCHW(const typename Intf::ContextType &ctx,
                                       uint64_t kAL1Iter,
                                       uint64_t mAL1Iter,
                                       uint64_t batchIter)
    {
        // [zj]TODO: api to support
        // uint64_t aL1GmOffset = kAL1Iter * cinOffsetGM +
        //                        hiIdx * ctx.orgWi +
        //                        batchIter * ctx.fmapOneBatchSize;

        // uint64_t aL1Mi = hiLoadL1 * ctx.orgWi;
        // Dn2NzParams intriParams;
        // intriParams.dnNum = 1;
        // intriParams.nValue = aL1Mi;
        // intriParams.dValue = IsKAL1Tail(kAL1Iter)
        //                         ? ctx.convTiling->cinATailInCore
        //                         : ctx.convTiling->cinAInCore;
        // intriParams.srcDValue = ctx.convTiling->oriHixWi;
        // intriParams.dstNzC0Stride = aL1Mi;
        // intriParams.dstNzNStride = 1;

        // DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
    }

    __aicore__ inline void LoadAL1NHWC(const typename Intf::ContextType &ctx,
                                       uint64_t kAL1Iter,
                                       uint64_t mAL1Iter,
                                       uint64_t batchIter)
    {
        // [zj]TODO: api to support
        // Nd2NzParams intriParams;
    }

    __aicore__ inline void LoadAL1NC1HWC04(const typename Intf::ContextType &ctx,
                                           uint64_t kAL1Iter,
                                           uint64_t mAL1Iter,
                                           uint64_t batchIter)
    {
        // [zj]TODO: 
    }

    // [zj]TODO: API
    __aicore__ inline void SetMFMatrix(const typename Intf::ContextType &ctx)
    {
        constexpr uint8_t POS_FM_W = 0;
        constexpr uint8_t POS_FM_H = 16;
        constexpr uint8_t POS_LFET_PAD = 32;
        constexpr uint8_t POS_RIGHT_PAD = 40;
        constexpr uint8_t POS_TOP_PAD = 48;
        constexpr uint8_t POS_BOT_PAD = 56;

        constexpr uint64_t BITS_FM_W = 0xffff;
        constexpr uint64_t BITS_FM_H = 0xffff;
        constexpr uint64_t BITS_LFET_PAD = 0xff;
        constexpr uint64_t BITS_RIGHT_PAD = 0xff;
        constexpr uint64_t BITS_TOP_PAD = 0xff;
        constexpr uint64_t BITS_BOT_PAD = 0xff;

        uint64_t fmatrix = 0;
        fmatrix |= (uint64_t)ctx.orgWi << POS_FM_W;
        fmatrix |= (uint64_t)hiLoadL1 << POS_FM_H;
        fmatrix |= (uint64_t)ctx.convTiling->padLeft << POS_LFET_PAD;
        fmatrix |= (uint64_t)ctx.convTiling->padRight << POS_RIGHT_PAD;
        fmatrix |= (uint64_t)padTopL1 << POS_TOP_PAD;
        fmatrix |= (uint64_t)padBottomL1 << POS_BOT_PAD;
        printf("[fmapL1]fmatrix: %ld\n", fmatrix);
        set_m_fmatrix(fmatrix);
    }

    // [zj]TODO: API
    __aicore__ inline void SetFMStepPos(const typename Intf::ContextType &ctx)
    {
        constexpr uint8_t POS_W_START_POS = 0;
        constexpr uint8_t POS_H_START_POS = 16;
        constexpr uint8_t POS_W_STEP = 32;
        constexpr uint8_t POS_H_STEP = 48;

        constexpr uint64_t BITS_W_START_POS = 0xffff;
        constexpr uint64_t BITS_H_START_POS = 0xffff;
        constexpr uint64_t BITS_W_STEP = 0xffff;
        constexpr uint64_t BITS_H_STEP = 0xffff;

        uint64_t wStep = ctx.orgWi + ctx.convTiling->padLeft + ctx.convTiling->padRight -
                         ctx.dilatedKernelW + 1;
        uint64_t hStep = hiLoadL1 + padTopL1 + padBottomL1 - ctx.dilatedKernelH + 1;
        wStep = (wStep - 1) / ctx.convTiling->strideW * ctx.convTiling->strideW + 1;
        hStep = (hStep - 1) / ctx.convTiling->strideH * ctx.convTiling->strideH + 1;

        uint64_t fmStepPos = 0;
        fmStepPos |= (uint64_t)(0) << POS_W_START_POS;
        fmStepPos |= (uint64_t)(0) << POS_H_START_POS;
        fmStepPos |= (uint64_t)wStep << POS_W_STEP;
        fmStepPos |= (uint64_t)hStep << POS_H_STEP;
        printf("[fmapL1]fmStepPos: %ld\n", fmStepPos);
        set_fm_step_pos(fmStepPos);
    }

    // [zj]TODO: API
    __aicore__ inline void SetMFMatrixDual0(const typename Intf::ContextType &ctx)
    {
        // [zj]TODO: support dual src
        constexpr uint8_t POS_SECOND_FM_ADDR = 0;
        constexpr uint8_t POS_SECOND_FM_H = 32;

        constexpr uint64_t BITS_SECOND_FM_ADDR = 0xffffffff;
        constexpr uint64_t BITS_SECOND_FM_H = 0xffffffff;

        uint64_t fMatrixDual0;
        fMatrixDual0 = 0;
        fMatrixDual0 |= (uint64_t)0 << POS_SECOND_FM_ADDR;
        fMatrixDual0 |= (uint64_t)0 << POS_SECOND_FM_H;
        printf("[fmapL1]fMatrixDual0: %ld\n", fMatrixDual0);
        set_m_fmatrix_dual_0(fMatrixDual0);
    }

    // [zj]TODO: API
    __aicore__ inline void SetMFMatrixDual1(const typename Intf::ContextType &ctx)
    {
        constexpr uint8_t POS_FIRST_HWC0STRIDE = 0;
        constexpr uint8_t POS_SECOND_WC0STRIDE = 16;

        constexpr uint64_t BITS_FIRST_HWC0STRIDE = 0xffff;
        constexpr uint64_t BITS_SECOND_WC0STRIDE = 0xffff;

        uint64_t fMatrixDual1;
        fMatrixDual1 = 0;
        fMatrixDual1 |= (uint64_t)hiLoadL1 * ctx.orgWi << POS_FIRST_HWC0STRIDE;
        fMatrixDual1 |= (uint64_t)0 << POS_SECOND_WC0STRIDE;
        printf("[fmapL1]fMatrixDual1: %ld\n", fMatrixDual1);
        set_m_fmatrix_dual_1(fMatrixDual1);
    }

    // [zj]TODO: API
    __aicore__ inline void SetMPadding(const typename Intf::ContextType &ctx)
    {
        constexpr uint8_t POS_PADDING_VALUE = 0;
        constexpr uint8_t POS_PADDING_CONTROL_MODE = 32;
        constexpr uint8_t POS_PADDING_VALUE_ADDR = 34;

        constexpr uint64_t BITS_PADDING_VALUE = 0xffff;
        constexpr uint64_t BITS_PADDING_CONTROL_MODE = 0x3;
        constexpr uint64_t BITS_PADDING_VALUE_ADDR = 0xffff;

        uint64_t mPadding = 0;
        // [zj]TODO：perchannel
        // mPadding |= (uint64_t)ctx.convTiling->offsetx << POS_PADDING_VALUE;
        mPadding |= (uint64_t)0 << POS_PADDING_CONTROL_MODE;
        mPadding |= ((uint64_t)(0) & BITS_PADDING_VALUE_ADDR) << POS_PADDING_VALUE_ADDR;
        printf("[fmapL1]mPadding: %ld\n", mPadding);
        set_m_padding(mPadding);
    }

private:
    uint64_t hiLoadL1;
    uint64_t padTopL1;
    uint64_t padBottomL1;
    uint64_t hiIdx;
    uint64_t cinOffsetGM;
};

template <class Intf>
class LoadFmapL0ToolsMMode {
public:
    __aicore__ inline LoadFmapL0ToolsMMode()
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
class CopyOutputToolsMMode {
public:
    __aicore__ inline CopyOutputToolsMMode()
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
        printf("[outputL1]batchIter: %ld, mAL1Iter: %ld, mL0Iter: %ld, nBL1Iter: %ld, nL0Iter: %ld\n",
               ctx.batchIter, ctx.mAL1Iter, ctx.mL0Iter, ctx.nBL1Iter, ctx.nL0Iter);

        uint64_t offsetCout = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                              ctx.nL0Iter * ctx.convTiling->nL0;

        if constexpr (Intf::formatOutput == ConvCommonApi::ConvFormat::NC1HWC0) {
            uint64_t offset = (ctx.mAL1Iter * ctx.mAL1 + ctx.mL0Iter * ctx.mL0) * Intf::k0Output +
                ctx.batchIter * ctx.outputOneBatchSize +
                offsetCout / Intf::k0Output * valueHoWo_ * Intf::k0Output +
                offsetCout % Intf::k0Output;
            return offset;
        } else if constexpr (Intf::formatOutput == ConvCommonApi::ConvFormat::NCHW) {
            uint64_t offset = ctx.mAL1Iter * ctx.mAL1 + ctx.mL0Iter * ctx.mL0 +
                              ctx.batchIter * ctx.outputOneBatchSize +
                              offsetCout * valueHoWo_;
            return offset;
        } else if constexpr (Intf::formatOutput == ConvCommonApi::ConvFormat::NHWC) {
            // [zj]TODO: 
            return 0;
        } else {
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported format"); });
        }
        return 0;
    }

    __aicore__ inline void CopyOutNC1HWC0(const typename Intf::ContextType &ctx,
                                          const GlobalTensor<typename Intf::OutputT> &output,
                                          uint64_t offset)
    {
        DataCopyExtParams intriParams;
        intriParams.blockCount = currentNL0_ / Intf::k0Output;
        intriParams.blockLen = currentML0_;
        intriParams.srcStride = 0;
        intriParams.dstStride = valueHoWo_ - currentML0_;

        DataCopyPadExtParams<typename Intf::OutputT> padParams;
        padParams.isPad = false;
        padParams.leftPadding = 0;
        padParams.rightPadding = 0;
        padParams.paddingValue = 0;

        printf("[outputL1]outputL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
               offset, intriParams.blockCount, intriParams.blockLen,
               intriParams.srcStride, intriParams.dstStride);
        DataCopyPad<typename Intf::OutputT, PaddingMode::Normal>(output[offset],
                                                                 ctx.outputL1,
                                                                 intriParams,
                                                                 padParams);
    }

    __aicore__ inline void CopyOutNCHW(const typename Intf::ContextType &ctx,
                                       const GlobalTensor<typename Intf::OutputT> &output,
                                       uint64_t offset)
    {
        // [zj]TODO：Fixpipe API
    }

    __aicore__ inline void CopyOutNHWC(const typename Intf::ContextType &ctx,
                                       const GlobalTensor<typename Intf::OutputT> &output,
                                       uint64_t offset)
    {
        // [zj]TODO：Fixpipe API
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

#endif // API_CONV2D_INSTR_M_MODE_IMPL_H
