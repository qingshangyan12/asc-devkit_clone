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
 * \file conv2d_instr_impl.h
 * \brief
 */

#ifndef API_CONV2D_INSTR_IMPL_H
#define API_CONV2D_INSTR_IMPL_H

#include "lib/conv/conv2d_config.h"
#include "../../conv2d_util.h"

namespace Conv2dApiFunc {
using namespace AscendC;

#define FIX_VAL 16

#define FIXED_SHIFT_VAL 42 // 58 - 16

template <class Intf, typename ChannelWiseT>
class LoadChannelWiseL1Tools {
public:
    __aicore__ inline LoadChannelWiseL1Tools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
    }

    __aicore__ inline void SetN(const typename Intf::ContextType &ctx, uint64_t n)
    {
        currentNL0_ = n;
    }

    __aicore__ inline void LoadChannelWiseL1FullLoad(const typename Intf::ContextType &ctx, const LocalTensor<ChannelWiseT> &tensorL1,
        const GlobalTensor<ChannelWiseT> &tensorGm, uint64_t loadNum, uint64_t gmStartAddr)
    {
        uint64_t byteNum = sizeof(ChannelWiseT);
        DataCopyParams dataCopyParams(1, loadNum * byteNum, 0, 0);
        uint8_t rightPadding = (uint8_t)(AlignB(loadNum * byteNum, PADDING_ALIGN_SIZE) / byteNum - loadNum);
        DataCopyPadParams padParams(true, 0, rightPadding, 0);
        DataCopyPad<ChannelWiseT>(tensorL1, tensorGm[gmStartAddr], dataCopyParams, padParams);
    }

    __aicore__ inline void LoadChannelWiseL1(const typename Intf::ContextType &ctx, const LocalTensor<ChannelWiseT> &tensorL1,
                                             const GlobalTensor<ChannelWiseT> &tensorGm)
    {
        uint64_t tensorGmOffset = ctx.nBL1Iter * ctx.convTiling->nBL1 +
            ctx.nL0Iter * ctx.convTiling->nL0;

        LoadChannelWiseL1FullLoad(ctx, tensorL1, tensorGm, currentNL0_, tensorGmOffset);
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t currentNL0_;
};

template <class Intf>
class LoadBiasBtTools {
public:
    __aicore__ inline LoadBiasBtTools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
    }

    __aicore__ inline void SetN(const typename Intf::ContextType &ctx, uint64_t n)
    {
        currentNL0_ = n;
    }

    __aicore__ inline void LoadBiasBt(const typename Intf::ContextType &ctx)
    {
        uint32_t offset = 0;
        if (ctx.convTiling->biasFullLoadFlag) {
            offset = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                     ctx.nL0Iter * ctx.convTiling->nL0;
        }

        // [zj]ToDoAPI
        printf("[bt]addr: %ld\n", (uint64_t)ctx.biasBT.GetPhyAddr());
        if constexpr (AscendC::IsSameType<typename Intf::BiasT, int32_t>::value) {
            copy_cbuf_to_bt((uint64_t)ctx.biasBT.GetPhyAddr(),
                            (__cbuf__ typename Intf::BiasT *)ctx.biasL1[offset].GetPhyAddr(), 0, 1,
                            currentNL0_ * Intf::sizeOfBias / BT_BLOCK_SIZE, 0, 0, 0);
        } else {
            copy_cbuf_to_bt((uint64_t)ctx.biasBT.GetPhyAddr(),
                            (__cbuf__ typename Intf::BiasT *)ctx.biasL1[offset].GetPhyAddr(), 2, 1,
                            currentNL0_ * Intf::sizeOfBias / BT_BLOCK_SIZE, 0, 0, FIX_VAL);
        }
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t currentNL0_;
};

template <class Intf, typename FixpParamT>
class LoadFixpParamFbTools {
public:
    __aicore__ inline LoadFixpParamFbTools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
    }

    __aicore__ inline void SetN(const typename Intf::ContextType &ctx, uint64_t n)
    {
        currentNL0_ = n;
    }

    __aicore__ inline void LoadFixpParamFb(const typename Intf::ContextType &ctx, uint64_t dstMemBlock,
                                           const LocalTensor<FixpParamT> &tensorFB,
                                           const LocalTensor<FixpParamT> &tensorL1)
    {
        uint32_t offset = 0;
        if (ctx.convTiling->fixpParamsFullLoadFlag) {
            offset = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                     ctx.nL0Iter * ctx.convTiling->nL0;
        }

        constexpr uint64_t POS_DST_MEM_BLOCK = 16;

        constexpr uint64_t POS_BURST_NUM = 4;
        constexpr uint64_t POS_BURST_LEN = 16;
        constexpr uint64_t POS_SRC_STRIDE = 32;
        constexpr uint64_t POS_DST_STRIDE = 48;

        uint64_t xm = 0;
        xm |= (uint64_t)1 << POS_BURST_NUM;
        xm |= (uint64_t)(currentNL0_ * sizeof(FixpParamT) / FB_BLOCK_SIZE) << POS_BURST_LEN;
        xm |= (uint64_t)0 << POS_SRC_STRIDE;
        xm |= (uint64_t)0 << POS_DST_STRIDE;
        printf("[ppbuf]addr: %ld, xm: %ld\n", (uint64_t)tensorFB.GetPhyAddr(), xm);
        copy_cbuf_to_fbuf_v2((__fbuf__ void *)((uint64_t)tensorFB.GetPhyAddr() | dstMemBlock << POS_DST_MEM_BLOCK),
                             (__cbuf__ FixpParamT *)tensorL1[offset].GetPhyAddr(), xm);
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t currentNL0_;
};

template <class Intf>
class LoadBL0Tools {
public:
    __aicore__ inline LoadBL0Tools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
    }

    __aicore__ inline void SetN(const typename Intf::ContextType &ctx, uint64_t n)
    {
        ratioOfNToN0 = (ctx.nBL1Iter == ctx.maxNBL1Iter && ctx.nL0Iter == ctx.maxNL0Iter) ?
            n / BLOCK_L0_N : ctx.convTiling->nStep;
    }

    __aicore__ inline void LoadBL0(const typename Intf::ContextType &ctx, bool isFirst)
    {
        if (unlikely(isFirst)) {
            loadData2dParams.mStartPosition = ctx.nL0Iter * ctx.convTiling->nL0 / BLOCK_L0_N;
            loadData2dParams.mStep = ratioOfNToN0;
            loadData2dParams.srcStride = ctx.convTiling->nL1DivBlockSize;
            loadData2dParams.dstStride = ratioOfNToN0;
        }
        loadData2dParams.kStartPosition = ctx.kBL0Iter * ctx.convTiling->kStep * Intf::k0 / C0_SIZE;
        loadData2dParams.kStep = ctx.kIter == ctx.maxKL0Iter
                                     ? CeilDIV(ctx.kL0Tail, C0_SIZE)
                                     : CeilDIV(ctx.convTiling->kStep * Intf::k0, C0_SIZE);
        LoadData<typename Intf::WeightT>(ctx.bl0, ctx.weightL1, loadData2dParams);
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t ratioOfNToN0;
    LoadData2DParamsV2 loadData2dParams;
};

template <class Intf>
class MMadTools {
public:
    __aicore__ inline MMadTools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
    }

    __aicore__ inline void SetMN(const typename Intf::ContextType &ctx, uint64_t m, uint64_t mNotAlign, uint64_t n)
    {
        currentML0_ = m;
        currentMNotAlignL0_ = mNotAlign;
        currentNL0_ = n;
    }

    __aicore__ inline void Mad(const typename Intf::ContextType &ctx)
    {
        // [zj]ToDo: 
        // [zj]ToDo: 
        SetKernel(ctx);
        SetMFpc(ctx);
        SetMQuantPre(ctx);
        SetMReluAlpha(ctx);
        SetMQuantPost(ctx);
        SetMEltAntiPara(ctx);
        SetMEltSrcPara(ctx);
        SetCubeStridePara_(ctx);

        SetChannelSize(ctx);
        SetConvParams(ctx);

        printf("[conv]convConfig: %ld\n", convConfig);

        // [zj]ToDo: PartialSum
        convolution((__cbuf__ void *)ctx.outputL1.GetPhyAddr(),
                    (__cbuf__ typename Intf::FmapT *)ctx.fmapL1.GetPhyAddr(),
                    (__cb__ typename Intf::WeightT *)ctx.bl0.GetPhyAddr(),
                    (__cbuf__ typename Intf::BiasT *)((ctx.enableBias && !biasMatrixBroadcast)
                                                          ? ctx.biasL1.GetPhyAddr()
                                                          : 0),
                    (uint64_t)((ctx.enableBias && biasMatrixBroadcast) ? ctx.biasBT.GetPhyAddr() : 0),
                    convConfig);
        // DumpAccChkPoint(ctx.outputL1, 8, 0, currentML0_ * currentNL0_);
    }

private:
    // [zj]ToDo: API
    __aicore__ inline void SetKernel(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t POS_STRIDE_W = 0;
        constexpr uint32_t POS_STRIDE_H = 6;
        constexpr uint32_t POS_FILTER_W = 12;
        constexpr uint32_t POS_FILTER_H = 21;
        constexpr uint32_t POS_DILATION_W = 30;
        constexpr uint32_t POS_DILATION_H = 38;
        constexpr uint32_t POS_KERNEL_NUM = 46;

        uint64_t kernel = 0;
        kernel |= (uint64_t)ctx.convTiling->strideW << POS_STRIDE_W;
        kernel |= (uint64_t)ctx.convTiling->strideH << POS_STRIDE_H;
        kernel |= (uint64_t)ctx.kernelW << POS_FILTER_W;
        kernel |= (uint64_t)ctx.kernelH << POS_FILTER_H;
        kernel |= (uint64_t)ctx.convTiling->dilationW << POS_DILATION_W;
        kernel |= (uint64_t)ctx.convTiling->dilationH << POS_DILATION_H;
        kernel |= (uint64_t)currentNL0_ << POS_KERNEL_NUM;
        printf("[conv]kernel: %ld\n", kernel);
        set_kernel(kernel);
    }

    // [zj]ToDo: SetFixPipeConfig
    __aicore__ inline void SetMFpc(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t POS_RELU_PRE_ADDR = 0;
        constexpr uint32_t POS_QUANT_PRE_ADDR = 8;
        constexpr uint32_t POS_QUANT_POST_ADDR = 24;
        constexpr uint32_t POS_ANTIQ_PARA_ADDR = 32;
        constexpr uint32_t POS_FIXED_SHIFT_VAL = 40;

        uint64_t mfpc = 0;
        mfpc |= (uint64_t)(ctx.enableReluPre ? (ctx.preReluFB.GetPhyAddr()) : 0) >> 6 << POS_RELU_PRE_ADDR;
        mfpc |= (uint64_t)(ctx.enableQuantPre ? (ctx.preQuantFB.GetPhyAddr()) : 0) >> 7 << POS_QUANT_PRE_ADDR;
        mfpc |= (uint64_t)(ctx.enableQuantPost ? (ctx.postQuantFB.GetPhyAddr()) : 0) >> 7 << POS_QUANT_POST_ADDR;
        mfpc |= (uint64_t)(ctx.enableEltAntiQuant ? (ctx.antiQuantFB.GetPhyAddr()) : 0) >> 6 << POS_ANTIQ_PARA_ADDR;
        mfpc |= (uint64_t)FIXED_SHIFT_VAL << POS_FIXED_SHIFT_VAL;
        printf("[conv]preReluFB: %ld, preQuantFB: %ld, postQuantFB: %ld, antiQuantFB: %ld\n",
            (uint64_t)(ctx.enableReluPre ? ctx.preReluFB.GetPhyAddr() : 0) >> 6,
            (uint64_t)(ctx.enableQuantPre ? ctx.preQuantFB.GetPhyAddr() : 0) >> 7,
            (uint64_t)(ctx.enableQuantPost ? ctx.postQuantFB.GetPhyAddr() : 0) >> 7,
            (uint64_t)(ctx.enableEltAntiQuant ? ctx.antiQuantFB.GetPhyAddr() : 0) >> 6);
        printf("[conv]mfpc: %ld\n", mfpc);
        set_m_fpc(mfpc);
    }

    // [zj]ToDo: API
    __aicore__ inline void SetMQuantPre(const typename Intf::ContextType &ctx)
    {
        printf("[conv]quantPreSpr: %ld\n", ctx.quantPreSpr);
        set_m_quant_pre(ctx.quantPreSpr);
    }

    // [zj]ToDo: API
    __aicore__ inline void SetMReluAlpha(const typename Intf::ContextType &ctx)
    {
        printf("[conv]reluPreSpr: %ld\n", ctx.reluPreSpr);
        set_m_relu_alpha(ctx.reluPreSpr);
    }

    // [zj]ToDo: API
    __aicore__ inline void SetMQuantPost(const typename Intf::ContextType &ctx)
    {
        printf("[conv]quantPostSpr: %ld\n", ctx.quantPostSpr);
        set_m_quant_post(ctx.quantPostSpr);
    }

    // [zj]ToDo: API
    __aicore__ inline void SetMEltAntiPara(const typename Intf::ContextType &ctx)
    {
        printf("[conv]antiQuantSpr: %ld\n", ctx.antiQuantSpr);
        set_m_elt_antiq_para(ctx.antiQuantSpr);
    }

    // [zj]ToDo: API
    __aicore__ inline void SetMEltSrcPara(const typename Intf::ContextType &ctx)
    {
        if (ctx.enableEltwise) {
            constexpr uint32_t POS_C0_INDICATOR = 0;
            constexpr uint32_t POS_L1_ADDR = 16;
            constexpr uint32_t POS_WCO_STRIDE = 32;
            constexpr uint32_t POS_HWCO_STRIDE = 48;

            uint64_t WC0_stride = 1;
            uint64_t HWC0_stride = 1;
            uint64_t offsetL1 = ctx.eltwiseBrcEn && ctx.convTiling->fixpParamsFullLoadFlag
                                    ? ctx.nBL1Iter * ctx.convTiling->nBL1 +
                                        ctx.nL0Iter * ctx.convTiling->nL0
                                    : 0;
            if (!ctx.eltwiseBrcEn) {
                if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
                    WC0_stride = ctx.orgWo;
                    HWC0_stride = currentMNotAlignL0_;
                } else {
                    WC0_stride = ctx.currentWoL1;
                    HWC0_stride = ctx.currentHoL1 * ctx.currentWoL1;
                }
            }
            // [zj]ToDo: C0_INDICATOR
            uint64_t meltSrcPara = 0;
            meltSrcPara |= (uint64_t)0 << POS_C0_INDICATOR;
            meltSrcPara |= (uint64_t)(ctx.eltwiseL1[offsetL1].GetPhyAddr()) >> 5 << POS_L1_ADDR;
            meltSrcPara |= (uint64_t)WC0_stride << POS_WCO_STRIDE;
            meltSrcPara |= (uint64_t)HWC0_stride << POS_HWCO_STRIDE;
            printf("[conv]meltSrcPara: %ld\n", meltSrcPara);
            set_m_elt_src_para(meltSrcPara);
        }
    }

    // [zj]ToDo: API
    __aicore__ inline void SetCubeStridePara_(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t POS_LOOP3_DST_STRIDE = 0;
        constexpr uint32_t POS_LOOP4_DST_STRIDE = 16;

        uint64_t WC0_stride = 0;
        uint64_t HWC0_stride = 0;
        if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            WC0_stride = ctx.orgWo; 
            HWC0_stride = currentMNotAlignL0_;
        } else {
            WC0_stride = ctx.currentWoL1; 
            HWC0_stride = ctx.currentHoL1 * ctx.currentWoL1;
        }

        uint64_t config = 0;
        config |= (uint64_t)WC0_stride << POS_LOOP3_DST_STRIDE;
        config |= (uint64_t)HWC0_stride << POS_LOOP4_DST_STRIDE;
        printf("[conv]CubeStridePara: %ld\n", config);
        set_cube_stride_para(config);
    }

    __aicore__ inline bool IsKL0Tail(const typename Intf::ContextType &ctx)
    {
        return ctx.kIter == ctx.maxKL0Iter;
    }

    __aicore__ inline void SetChannelSize(const typename Intf::ContextType &ctx)
    {
        if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
            if constexpr (Intf::kPreLoadFlag) {
                channelSize_ = ctx.convTiling->isC04Flag
                                   ? conv::C04_CIN_SIZE
                                   : (ctx.kAL1Iter - 1 != ctx.maxKAL1Iter
                                             ? ctx.convTiling->cinAInCore
                                             : AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
            } else {
                channelSize_ = ctx.convTiling->isC04Flag
                                   ? conv::C04_CIN_SIZE
                                   : (ctx.kAL1Iter != ctx.maxKAL1Iter
                                             ? ctx.convTiling->cinAInCore
                                             : AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
            }
        } else if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            if constexpr (Intf::kPreLoadFlag) {
                channelSize_ = (ctx.kAL1Iter - 1 != ctx.maxKAL1Iter
                                   ? ctx.convTiling->cinAInCore
                                   : AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
            } else {
                channelSize_ = (ctx.kAL1Iter != ctx.maxKAL1Iter
                                   ? ctx.convTiling->cinAInCore
                                   : AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
            }
        }
        printf("[conv]channelSize_: %ld\n", channelSize_);
    }

    __aicore__ inline void SetConvParams(const typename Intf::ContextType &ctx)
    {
        // bias
        biasMatrixInitVal = (!ctx.enableBias && ctx.kIter != 0);
        biasMatrixBroadcast = (ctx.enableBias && ctx.kIter == 0);

        // fixpipe
        preQuantMode = (pre_quant_t)(ctx.preQuantMode);
        preReluMode = (pre_relu_t)(ctx.reluPreMode);
        postQuantMode = (post_quant_t)(ctx.quantPostMode);
        actPostEnable = (ctx.reluPostMode);
        preClipReluMode = (ctx.clipReluPreMode);
        eltwiseOp = (eltwise_op_t)(ctx.eltwiseOp);
        eltwiseAntiqEnable = (ctx.eltwiseAntiqEn);
        eltwiseBroadcastEnable = (ctx.eltwiseBrcEn);

        // [zj]ToDo
        // convFixParams.SetDependEnable(dependEnable); // depend
        // convFixParams.SetInstrId(instrId); // id

        // channel size
        n = (channelSize_);

        convConfig = 0;
        convConfig |= (((uint64_t)biasMatrixInitVal & 0x1) << 0);
        convConfig |= (((uint64_t)biasMatrixBroadcast & 0x1) << 1);
        convConfig |= (((uint64_t)preQuantMode & 0x1f) << 2);
        convConfig |= (((uint64_t)preReluMode & 0x7) << 7);
        convConfig |= (((uint64_t)postQuantMode & 0x1f) << 10);
        convConfig |= (((uint64_t)actPostEnable & 0x7) << 15); // new
        convConfig |= (((uint64_t)preClipReluMode & 0x1) << 18);
        convConfig |= (((uint64_t)eltwiseOp & 0x7) << 20);
        convConfig |= (((uint64_t)eltwiseAntiqEnable & 0x1) << 23);
        convConfig |= (((uint64_t)eltwiseBroadcastEnable & 0x1) << 24);
        // [zj]ToDo
        // convConfig |= (((uint64_t)lsbMask & 0x7) << 28);
        // convConfig |= (((uint64_t)dualSrcEnable & 0x1) << 34);
        // convConfig |= (((uint64_t)compactEnable & 0x1) << 35);
        // convConfig |= (((uint64_t)dependEnable & 0x1) << 36);
        // convConfig |= (((uint64_t)instrId & 0x3) << 37);
        // convConfig |= (((uint64_t)postProcessBandwithCtrl & 0x1) << 39);
        convConfig |= (((uint64_t)n & 0xffff) << 48);
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t currentML0_;
    uint64_t currentMNotAlignL0_;
    uint64_t currentNL0_;
    uint64_t channelSize_;

    // 
    uint64_t convConfig;

    bool biasMatrixInitVal;
    bool biasMatrixBroadcast;
    pre_quant_t preQuantMode;
    pre_relu_t preReluMode;
    post_quant_t postQuantMode;
    bool actPostEnable;
    bool preClipReluMode;
    eltwise_op_t eltwiseOp;
    bool eltwiseAntiqEnable;
    bool eltwiseBroadcastEnable;
    // lsb_mask_t lsbMask = lsb_mask_t::Disable;
    // bool dualSrcEnable = false;
    // bool compactEnable = false;
    // bool dependEnable = false;
    // instr_id_t instrId = instr_id_t::ID_0;
    // bool postProcessBandwithCtrl = false;
    uint16_t n;
};

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
                DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
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
                DataCopy<typename Intf::FmapT, true>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
            } else {
                SetDn2NzIntriParams(ctx, intriParams, kAL1Iter);
                DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
            }
        }
        // DumpAccChkPoint(ctx.fmapL1, 6, 0,
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
            DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
        } else if constexpr (Intf::formatA == ConvFormat::NCHW) {
            uint64_t aL1GmOffset = kAL1Iter * ctx.convTiling->cinOffsetBlockInGM +
                                   hiIdx * ctx.orgWi + batchIter * ctx.fmapOneBatchSize;
            Dn2NzParams intriParams;
            SetDn2NzIntriParams(ctx, intriParams, kAL1Iter);
            DataCopy<typename Intf::FmapT>(ctx.fmapL1, ctx.fmapGm[aL1GmOffset], intriParams);
        }
        // DumpAccChkPoint(ctx.fmapL1, 6, 0,
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
            LoadData<typename Intf::WeightT>(ctx.weightL1, ctx.weightGm, loadData2dParams);
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
            DataCopy<typename Intf::WeightT>(ctx.weightL1, ctx.weightGm[bL1GmOffset], intriParams);
            // DumpAccChkPoint(ctx.weightL1, 7, 0, currentNBL1 * currentKBL1);
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
            DataCopy<typename Intf::WeightT>(ctx.weightL1, ctx.weightGm[bL1GmOffset], intriParams);
            // DumpAccChkPoint(ctx.weightL1, 7, 0, currentNBL1 * currentKBL1);
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
        DataCopy<typename Intf::WeightT>(ctx.weightL1, ctx.weightGm[bL1GmOffset], intriParams);
        // DumpAccChkPoint(ctx.weightL1, 7, 0, currentNBL1 * currentKBL1);
    }

private:
    // typename Intf::ContextType *ctx_;
    uint64_t maxNBL1Iter_;
    uint64_t maxKBL1Iter_;
    uint32_t orgCo1;
    bool isOneByOneKernelScenario;
};

};

#endif // API_CONV2D_INSTR_IMPL_H