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
 * \file conv_intr_m_mode_impl.h
 * \brief
 */

#ifndef CONV_INSTR_M_MODE_IMPL_H
#define CONV_INSTR_M_MODE_IMPL_H

#include "../../conv_config.h"
#include "../../conv_util.h"

namespace ConvFunc {
using namespace AscendC;
using namespace conv;

template <class Intf>
class LoadAL0ToolsMMode {
public:
    __aicore__ inline LoadAL0ToolsMMode()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
    }

    __aicore__ inline void SetM(const typename Intf::ContextType &ctx, uint64_t m, uint64_t mNotAlign)
    {
        // currentML0_ = m;
        // currentML0Align_ = mNotAlign;
        // LoadDataRepeatParam repeatParams = {0, 1, 0, static_cast<uint16_t>(currentML0_ / BLOCK_L0_M)};
        // SetLoadDataRepeat(repeatParams);
    }

    __aicore__ inline void LoadAL0(const typename Intf::ContextType &ctx, bool isFirst = true)
    {
        // uint64_t currentKL0 = IsKL0Tail() ? ctx.kL0Tail : ctx.convTiling->kL0;
        // uint64_t posK = ctx.kAL0Iter * ctx.convTiling->kL0;
        // uint64_t channelsize;
        // if constexpr (Intf::kPreLoadFlag) {
        //     channelsize = (ctx.kAL1Iter - 1 != ctx.maxKAL1Iter ? ctx.convTiling->cinAInCore :
        //                    AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
        // } else {
        //     channelsize = (ctx.kAL1Iter != ctx.maxKAL1Iter ? ctx.convTiling->cinAInCore :
        //                    AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
        // }
        // if (isFirst) {
        //     uint64_t posM = ctx.mL0Iter * ctx.mL0 +
        //                     (ctx.mStartPos + ctx.mAL1Iter * ctx.mAL1) % ctx.orgWo;
        //     xmtmp_ = ((currentML0Align_ & 0xffff) << MSTEP_OFFSET) | ((posM & 0xffff) << POSM_OFFSET);
        //     xm_ = ((currentKL0 & 0xffff) << 0) | ((posK & 0xffff) << POSK_OFFSET) | xmtmp_;
        //     xt_ = (((uint64_t)ctx.convTiling->strideW & 0x3f) << 0) |
        //           (((uint64_t)ctx.convTiling->strideH & 0x3f) << STRIDEH_OFFSET) |
        //           (((uint64_t)ctx.kernelW & 0xff) << KERNELW_OFFSET) |
        //           (((uint64_t)ctx.kernelH & 0xff) << KERNELH_OFFSET) |
        //           (((uint64_t)ctx.convTiling->dilationW & 0xff) << DILATIONW_OFFSET) |
        //           (((uint64_t)ctx.convTiling->dilationH & 0xff) << DILATIONH_OFFSET) |
        //           (((uint64_t)channelsize & 0xffff) << CIN_OFFSET);
        // } else {
        //     xm_ = ((currentKL0 & 0xffff) << 0) | ((posK & 0xffff) << POSK_OFFSET) | xmtmp_;
        // }
        // img2colv2_cbuf_to_ca((__ca__ typename Intf::FmapT *)ctx.al0.GetPhyAddr(),
        //     (__cbuf__ typename Intf::FmapT *)ctx.al1.GetPhyAddr(),
        //     xm_,
        //     xt_);
    };

private:
    __aicore__ inline bool IsKL0Tail(const typename Intf::ContextType &ctx)
    {
        return ctx.kIter == ctx.maxKL0Iter;
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t currentML0_;
    uint64_t currentML0Align_;
    uint64_t xm_;
    uint64_t xt_;
    uint64_t xmtmp_;
};

template <class Intf>
class CopyEltToolsMMode {
public:
    __aicore__ inline CopyEltToolsMMode()
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
        uint64_t c0 = (C0_SIZE / sizeof(typename Intf::EltwiseT));
        uint64_t offsetCout = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                              ctx.nL0Iter * ctx.convTiling->nL0;
        uint64_t offset = (ctx.mAL1Iter * ctx.mAL1 + ctx.mL0Iter * ctx.mL0) * c0 +
                          ctx.batchIter * ctx.outputOneBatchSize +
                          offsetCout / c0 * valueHoWo_ * c0 + offsetCout % c0;
        return offset;
    }

    __aicore__ inline void CopyElt(const typename Intf::ContextType &ctx)
    {
        uint64_t offset = CalcOffset(ctx);
        uint64_t c0 = (C0_SIZE / sizeof(typename Intf::EltwiseT));
        DataCopyParams intriParams;
        intriParams.blockCount = currentNL0_ / c0;
        intriParams.blockLen = currentML0_;
        intriParams.srcStride = valueHoWo_ - currentML0_;
        intriParams.dstStride = 0;
        printf("[eltwiseL1]eltwiseL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
               offset, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
        DataCopy<typename Intf::EltwiseT>(ctx.eltwiseL1, ctx.eltwiseGm[offset], intriParams);
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t valueHoWo_;
    uint64_t currentML0_;
    uint64_t currentNL0_;
};

template <class Intf>
class CopyOutToolsMMode {
public:
    __aicore__ inline CopyOutToolsMMode()
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
        printf("[cL1]batchIter: %ld, mAL1Iter: %ld, mL0Iter: %ld, nBL1Iter: %ld, nL0Iter: %ld\n", ctx.batchIter,
               ctx.mAL1Iter, ctx.mL0Iter, ctx.nBL1Iter, ctx.nL0Iter);
        uint64_t c0 = (C0_SIZE / sizeof(typename Intf::OutputT));
        uint64_t offsetCout = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                              ctx.nL0Iter * ctx.convTiling->nL0;
        if constexpr (Intf::formatC == ConvFormat::NC1HWC0) {
            uint64_t offset = (ctx.mAL1Iter * ctx.mAL1 + ctx.mL0Iter * ctx.mL0) * c0 +
                              ctx.batchIter * ctx.outputOneBatchSize +
                              offsetCout / c0 * valueHoWo_ * c0 + offsetCout % c0;
            return offset;
        }
        uint64_t offset = ctx.mAL1Iter * ctx.mAL1 + ctx.mL0Iter * ctx.mL0 +
                          ctx.batchIter * ctx.outputOneBatchSize;
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
        uint64_t c0 = (C0_SIZE / sizeof(typename Intf::OutputT));
        if constexpr (Intf::formatC == ConvFormat::NC1HWC0) {
            DataCopyParams intriParams;
            intriParams.blockCount = currentNL0_ / c0;
            intriParams.blockLen = currentML0_;
            intriParams.srcStride = 0;
            intriParams.dstStride = valueHoWo_ - currentML0_;
            printf("[cL1]cL1GmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n", offset,
                   intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
            DataCopy<typename Intf::OutputT>(output[offset], ctx.cl1, intriParams);
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

#endif // CONV_INSTR_M_MODE_IMPL_H