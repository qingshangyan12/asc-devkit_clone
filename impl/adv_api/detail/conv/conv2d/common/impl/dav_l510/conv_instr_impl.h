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
 * \file conv_instr_impl.h
 * \brief
 */

#ifndef CONV_INSTR_IMPL_H
#define CONV_INSTR_IMPL_H

#include "../../conv_config.h"
#include "../../conv_util.h"

namespace ConvFunc {
using namespace AscendC;
using namespace conv;

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
        LoadData<typename Intf::WeightT>(ctx.bl0, ctx.bl1, loadData2dParams);
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
        convolution((__cbuf__ void *)ctx.cl1.GetPhyAddr(),
                    (__cbuf__ typename Intf::FmapT *)ctx.al1.GetPhyAddr(),
                    (__cb__ typename Intf::WeightT *)ctx.bl0.GetPhyAddr(),
                    (__cbuf__ typename Intf::BiasT *)((ctx.enableBias && !biasMatrixBroadcast)
                                                          ? ctx.biasL1.GetPhyAddr()
                                                          : 0),
                    (uint64_t)((ctx.enableBias && biasMatrixBroadcast) ? ctx.biasBT.GetPhyAddr() : 0),
                    convConfig);
        // DumpAccChkPoint(ctx.cl1, 8, 0, currentML0_ * currentNL0_);
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
        mfpc |= (uint64_t)(ctx.enableReluPre ? (ctx.reluPreFB.GetPhyAddr()) : 0) >> 6 << POS_RELU_PRE_ADDR;
        mfpc |= (uint64_t)(ctx.enableQuantPre ? (ctx.quantPreFB.GetPhyAddr()) : 0) >> 7 << POS_QUANT_PRE_ADDR;
        mfpc |= (uint64_t)(ctx.enableQuantPost ? (ctx.quantPostFB.GetPhyAddr()) : 0) >> 7 << POS_QUANT_POST_ADDR;
        mfpc |= (uint64_t)(ctx.enableEltAntiQuant ? (ctx.eltAntiQuantFB.GetPhyAddr()) : 0) >> 6 << POS_ANTIQ_PARA_ADDR;
        mfpc |= (uint64_t)FIXED_SHIFT_VAL << POS_FIXED_SHIFT_VAL;
        printf("[conv]reluPreFB: %ld, quantPreFB: %ld, quantPostFB: %ld, eltAntiQuantFB: %ld\n",
            (uint64_t)(ctx.enableReluPre ? ctx.reluPreFB.GetPhyAddr() : 0) >> 6,
            (uint64_t)(ctx.enableQuantPre ? ctx.quantPreFB.GetPhyAddr() : 0) >> 7,
            (uint64_t)(ctx.enableQuantPost ? ctx.quantPostFB.GetPhyAddr() : 0) >> 7,
            (uint64_t)(ctx.enableEltAntiQuant ? ctx.eltAntiQuantFB.GetPhyAddr() : 0) >> 6);
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
        preQuantMode = (pre_quant_t)(ctx.quantPreMode);
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
};

#endif // CONV_INSTR_IMPL_H