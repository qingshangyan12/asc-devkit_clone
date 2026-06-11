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

#include "../../conv_config.h"
#include "../../conv_util.h"

namespace Conv2dApiFunc {
using namespace AscendC;

template <class Intf>
class LoadAL0ToolsHWMode {
public:
    __aicore__ inline LoadAL0ToolsHWMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
        // if constexpr (Intf::formatC == ConvFormat::NCDHW) {
        //     if constexpr (Intf::kPreLoadFlag) {
        //         channelSize_ = ctx.kAL1Iter - 1 != ctx.maxKAL1Iter ? ctx.convTiling->cinAInCore :
        //             ctx.convTiling->cinATailInCore;
        //     } else {
        //         channelSize_ = ctx.kAL1Iter != ctx.maxKAL1Iter ? ctx.convTiling->cinAInCore :
        //             ctx.convTiling->cinATailInCore;
        //     }
        // } else {
        //     if constexpr (Intf::kPreLoadFlag) {
        //         channelSize_ = ctx.convTiling->isC04Flag ? conv::C04_CIN_SIZE :
        //             (ctx.kAL1Iter - 1 != ctx.maxKAL1Iter ? ctx.convTiling->cinAInCore :
        //             AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
        //     } else {
        //         channelSize_ = ctx.convTiling->isC04Flag ? conv::C04_CIN_SIZE :
        //             (ctx.kAL1Iter != ctx.maxKAL1Iter ? ctx.convTiling->cinAInCore :
        //             AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
        //     }
        //     if (ctx.convTiling->isC04Flag) {
        //         c04KStepTail = (channelSize_ * ctx.convTiling->kernelHxkernelW) % ctx.convTiling->kL0;
        //         c04KStepTail = c04KStepTail == 0 ? ctx.convTiling->kL0 : c04KStepTail;
        //     }
        // }
    }

    __aicore__ inline void SetM(const typename Intf::ContextType &ctx, uint64_t m, uint64_t mNotAlign)
    {
        // currentML0_ = m;
        // currentML0Align_ = mNotAlign;
        // mStartPt_ = ctx.hoL0Iter * ctx.currentWoL1 * ctx.convTiling->hoL0 +
        //             ctx.woL0Iter * ctx.convTiling->woL0;
        // SetLoad3dRepeatParams();
    }

    __aicore__ inline void SetLoad3dRepeatParams(const typename Intf::ContextType &ctx)
    {
        // uint16_t dstStride = 0;
        // uint16_t repeatStride = 0;
        // uint8_t repeatTime = 1;
        // uint8_t repeatMode = 0;
        // mExtension_ = currentML0Align_;

        // if (ctx.currentWoL0 == ctx.convTiling->woL0 &&
        //     ctx.currentHoL0 == ctx.convTiling->hoL0) {
        //     dstStride = ctx.convTiling->fmapKStride;
        // } else {
        //     dstStride = currentML0_ / BLOCK_L0_M;
        // }

        // if (unlikely(CeilDIV(ctx.currentWoL1, ctx.convTiling->woL0) > 1)) {
        //     mExtension_ = ctx.currentWoL0;
        //     repeatStride = ctx.currentWoL1 / BLOCK_L0_M;
        //     repeatTime = ctx.currentHoL0;
        // }

        // LoadDataRepeatParam repeatParams = {repeatStride, repeatTime, repeatMode, dstStride};
        // SetLoadDataRepeat(repeatParams);
    }

    __aicore__ inline void LoadAL0(const typename Intf::ContextType &ctx, bool isFirst = true)
    {
        // if (isFirst) {
        //     loadData3Dv2Params.channelSize = channelSize_;
        //     loadData3Dv2Params.strideW = ctx.convTiling->strideW;
        //     loadData3Dv2Params.strideH = ctx.convTiling->strideH;
        //     loadData3Dv2Params.filterW = ctx.kernelW;
        //     loadData3Dv2Params.filterH = ctx.kernelH;
        //     loadData3Dv2Params.dilationFilterW = ctx.convTiling->dilationW;
        //     loadData3Dv2Params.dilationFilterH = ctx.convTiling->dilationH;
        //     loadData3Dv2Params.mExtension = mExtension_;
        //     loadData3Dv2Params.mStartPt = mStartPt_;
        // }
        // if constexpr (Intf::formatC == ConvFormat::NCDHW) {
        //     currentKL0 = IsKL0Tail() ? ctx.kL0Tail : ctx.convTiling->kL0;
        // } else {
        //     currentKL0 = IsKL0Tail() ? (ctx.convTiling->isC04Flag ? c04KStepTail : ctx.kL0Tail) :
        //                       ctx.convTiling->kL0;
        // }
        // loadData3Dv2Params.kExtension = currentKL0;
        // loadData3Dv2Params.kStartPt = ctx.kAL0Iter * ctx.convTiling->kL0;

        // LoadData<typename Intf::FmapT, CONV_LOAD3DV2_DEFAULT_CONFIG>(
        //     ctx.al0, ctx.al1, loadData3Dv2Params);
    };

private:
    __aicore__ inline bool IsKL0Tail(const typename Intf::ContextType &ctx)
    {
        return ctx.kIter == ctx.maxKL0Iter;
    }

private:
    // typename Intf::ContextType *ctx_;
    LoadData3DParamsV2<typename Intf::FmapT> loadData3Dv2Params;
    uint64_t currentML0_;
    uint64_t currentML0Align_;
    uint16_t mExtension_;
    uint16_t mStartPt_;
    uint16_t channelSize_;
    uint64_t c04KStepTail;
    uint64_t currentKL0;
};

template <class Intf>
class CopyEltToolsHWMode {
public:
    __aicore__ inline CopyEltToolsHWMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
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

        uint64_t c0 = (C0_SIZE / sizeof(typename Intf::EltwiseT));
        uint64_t offset = offsetBatch + offsetH * ctx.orgWo * c0 + offsetW * c0 +
                          offsetCout / c0 * valueHoWo_ * c0 + offsetCout % c0;
        return offset;
    }

    __aicore__ inline void CopyElt(const typename Intf::ContextType &ctx)
    {
        uint64_t offset = CalcOffset(ctx);
        if (likely(ctx.convTiling->woL1 == ctx.orgWo)) {
            uint64_t c0 = (C0_SIZE / sizeof(typename Intf::EltwiseT));
            DataCopyParams intriParams;
            intriParams.blockCount = currentNL0_ / c0;
            intriParams.blockLen = currentML0_;
            intriParams.srcStride = valueHoWo_ - currentML0_;
            intriParams.dstStride = 0;
            printf("[eltwiseL1]eltwiseL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                   offset, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
            DataCopy<typename Intf::EltwiseT>(ctx.eltwiseL1, ctx.eltwiseGm[offset], intriParams);
        } else {
            // [zj]ToDo
        }
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t valueHoWo_;
    uint64_t currentML0_;
    uint64_t currentNL0_;
};

template <class Intf>
class CopyOutToolsHWMode {
public:
    __aicore__ inline CopyOutToolsHWMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
        valueHoWo_ = ctx.orgHo * ctx.orgWo;
    }

    __aicore__ inline void SetMN(const typename Intf::ContextType &ctx, uint64_t m, uint64_t n)
    {
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

        if constexpr (Intf::formatC == ConvFormat::NC1HWC0) {
            uint64_t c0 = (C0_SIZE / sizeof(typename Intf::OutputT));
            uint64_t offset = offsetBatch + offsetH * ctx.orgWo * c0 + offsetW * c0 +
                              offsetCout / c0 * valueHoWo_ * c0 + offsetCout % c0;
            return offset;
        }
        uint64_t offset = offsetBatch + offsetH * ctx.orgWo + offsetW;
        if constexpr (Intf::formatC == ConvFormat::NCDHW) {
            offset += offsetCout * ctx.orgDo * valueHoWo_ + ctx.dOutIter * valueHoWo_;
        } else {
            offset += offsetCout * valueHoWo_;
        }

        return offset;
    }

    __aicore__ inline void CopyOut(const typename Intf::ContextType &ctx, const GlobalTensor<typename Intf::OutputT> &output)
    {
        uint64_t offset = CalcOffset(ctx);
        printf("[cL1]batchIter: %ld, mAL1Iter: %ld, mL0Iter: %ld, nBL1Iter: %ld, nL0Iter: %ld\n", ctx.batchIter,
               ctx.mAL1Iter, ctx.mL0Iter, ctx.nBL1Iter, ctx.nL0Iter);
        if constexpr (Intf::formatC == ConvFormat::NC1HWC0) {
            if (likely(ctx.convTiling->woL1 == ctx.orgWo)) {
                uint64_t c0 = (C0_SIZE / sizeof(typename Intf::OutputT));
                DataCopyParams intriParams;
                intriParams.blockCount = currentNL0_ / c0;
                intriParams.blockLen = currentML0_;
                intriParams.srcStride = 0;
                intriParams.dstStride = valueHoWo_ - currentML0_;
                printf("[cL1]cL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                       offset, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
                       intriParams.dstStride);
                DataCopy<typename Intf::OutputT>(output[offset], ctx.outputL1, intriParams);
            } else {
                // [zj]ToDo
            }
        } else {
            // [zj]ToDo
        }
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t valueHoWo_;
    uint64_t currentML0_;
    uint64_t currentNL0_;
};

};

#endif // API_CONV2D_INSTR_HW_MODE_IMPL_H