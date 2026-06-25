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

#include "lib/conv/conv2d/conv2d_config.h" // #include "../../../../../include/adv_api/conv/conv2d/conv2d_config.h"  // [zj]TODO
#include "../../conv2d_util.h"

namespace Conv2dApi { // namespace Conv2dApiFunc { // [zj]TODO: just for cpu
using namespace AscendC;

template <class Intf, typename ChannelWiseT>
class LoadChannelWiseL1Tools {
public:
    __aicore__ inline LoadChannelWiseL1Tools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType& ctx)
    {
        (void)ctx;
    }

    __aicore__ inline void SetN(const typename Intf::ContextType& ctx,
                                uint64_t n)
    {
        (void)ctx;
        currentNL0_ = n;
    }

    __aicore__ inline void LoadChannelWiseL1FullLoad(
        const typename Intf::ContextType& ctx,
        const LocalTensor<ChannelWiseT>& tensorL1,
        const GlobalTensor<ChannelWiseT>& tensorGm,
        uint64_t loadNum, uint64_t gmStartAddr)
    {
        (void)ctx;

        uint64_t byteNum = sizeof(ChannelWiseT);
        DataCopyExtParams intriParams;
        intriParams.blockCount = 1;
        intriParams.blockLen = loadNum * byteNum;
        intriParams.srcStride = 0;
        intriParams.dstStride = 0;

        uint8_t rightPadding = (uint8_t)(ConvApi::AlignB(loadNum * byteNum,
            Conv2dApi::PADDING_ALIGN_SIZE) / byteNum - loadNum);
        DataCopyPadExtParams<ChannelWiseT> padParams;
        padParams.isPad = true;
        padParams.leftPadding = 0;
        padParams.rightPadding = rightPadding;
        padParams.paddingValue = 0;

        printf("[channelWiseL1]gmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                gmStartAddr, intriParams.blockCount, intriParams.blockLen,
                intriParams.srcStride, intriParams.dstStride);
        DataCopyPad<ChannelWiseT, PaddingMode::Normal>(tensorL1,
                                                       tensorGm[gmStartAddr],
                                                       intriParams,
                                                       padParams);
    }

    __aicore__ inline void LoadChannelWiseL1(
        const typename Intf::ContextType& ctx,
        const LocalTensor<ChannelWiseT>& tensorL1,
        const GlobalTensor<ChannelWiseT>& tensorGm)
    {
        uint64_t tensorGmOffset = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                                  ctx.nL0Iter * ctx.convTiling->nL0;
        LoadChannelWiseL1FullLoad(ctx, tensorL1, tensorGm,
                                  currentNL0_, tensorGmOffset);
    }

private:
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
        (void)ctx;
    }

    __aicore__ inline void SetN(const typename Intf::ContextType &ctx, uint64_t n)
    {
        (void)ctx;
        currentNL0_ = n;
    }

    __aicore__ inline void LoadBiasBt(const typename Intf::ContextType &ctx)
    {
        uint32_t offset = 0;
        if (unlikely(!ctx.convTiling->biasFullLoadFlag)) {
            offset = ctx.nBL1Iter * ctx.convTiling->nBL1 + 
                     ctx.nL0Iter * ctx.convTiling->nL0;
        }

        // printf("[bt]addr: %ld\n", (uint64_t)ctx.biasBT.GetPhyAddr());
        // if constexpr (IsSameType<typename Intf::BiasT, int32_t>::value) {
        //     copy_cbuf_to_bt((uint64_t)ctx.biasBT.GetPhyAddr(),
        //                     (__cbuf__ typename Intf::BiasT *)ctx.biasL1[offset].GetPhyAddr(),0, 1,
        //                     currentNL0_ * Intf::sizeOfBias / BT_BLOCK_SIZE, 0, 0, 0);
        // } else {
        //     copy_cbuf_to_bt((uint64_t)ctx.biasBT.GetPhyAddr(),
        //                     (__cbuf__ typename Intf::BiasT *)ctx.biasL1[offset].GetPhyAddr(), 2, 1,
        //                     currentNL0_ * Intf::sizeOfBias / BT_BLOCK_SIZE, 0, 0, FIX_VAL);
        // }

        // [zj]TODO: FIX_VAL暂不支持配置
        DataCopyParams intriParams;
        intriParams.blockCount = 1;
        intriParams.blockLen = ConvApi::CeilDIV(currentNL0_ * sizeof(typename Intf::BiasT),
                                                   ConvApi::BT_BLOCK_SIZE);

        printf("[biasBT]l1Offset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
               offset, intriParams.blockCount, intriParams.blockLen,
               intriParams.srcStride, intriParams.dstStride);
        DataCopy<typename Intf::L0cT, typename Intf::BiasT>(ctx.biasBT,
                                                            ctx.biasL1[offset],
                                                            intriParams);
    }

private:
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
        (void)ctx;
    }

    __aicore__ inline void SetN(const typename Intf::ContextType &ctx, uint64_t n)
    {
        (void)ctx;
        currentNL0_ = n;
    }

    // [zj]TODO: API
    __aicore__ inline void LoadFixpParamFb(const typename Intf::ContextType &ctx,
                                           uint64_t dstMemBlock,
                                           const LocalTensor<FixpParamT> &tensorFB,
                                           const LocalTensor<FixpParamT> &tensorL1)
    {
        uint32_t offset = 0;
        if (unlikely(!ctx.convTiling->fixpParamsFullLoadFlag)) {
            offset = ctx.nBL1Iter * ctx.convTiling->nBL1 +
                     ctx.nL0Iter * ctx.convTiling->nL0;
        }

        constexpr uint8_t POS_DST_MEM_BLOCK = 16;

        constexpr uint8_t POS_BURST_NUM = 4;
        constexpr uint8_t POS_BURST_LEN = 16;
        constexpr uint8_t POS_SRC_STRIDE = 32;
        constexpr uint8_t POS_DST_STRIDE = 48;

        constexpr uint64_t BITS_BURST_NUM = 0xfff;
        constexpr uint64_t BITS_BURST_LEN = 0xffff;
        constexpr uint64_t BITS_SRC_STRIDE = 0xffff;
        constexpr uint64_t BITS_DST_STRIDE = 0xffff;

        uint64_t xm = 0;
        xm |= (uint64_t)1 << POS_BURST_NUM;
        xm |= (uint64_t)(currentNL0_ * sizeof(FixpParamT) / Conv2dApi::FB_BLOCK_SIZE) << POS_BURST_LEN;
        xm |= (uint64_t)0 << POS_SRC_STRIDE;
        xm |= (uint64_t)0 << POS_DST_STRIDE;
        printf("[ppbuf]addr: %ld, xm: %ld\n", (uint64_t)tensorFB.GetPhyAddr(), xm);
        copy_cbuf_to_fbuf_v2((__fbuf__ void *)((uint64_t)tensorFB.GetPhyAddr() | dstMemBlock << POS_DST_MEM_BLOCK),
                             (__cbuf__ FixpParamT *)tensorL1[offset].GetPhyAddr(), xm);
    }

private:
    uint64_t currentNL0_;
};

template <class Intf>
class LoadBL1Tools {
public:
    __aicore__ inline LoadBL1Tools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        maxNBL1Iter_ = ctx.maxNBL1Iter;
        maxKBL1Iter_ = ctx.maxKBL1Iter;
        coutOffsetBlock = static_cast<uint32_t>((ctx.orgCi / ctx.convTiling->groups) *
                                                ctx.convTiling->kernelHxkernelW);
        isOneByOneKernelScenario = ctx.convTiling->kernelHxkernelW == 1;
    }

    __aicore__ inline void LoadBL1(const typename Intf::ContextType &ctx)
    {
        LoadBL1(ctx, ctx.kBL1Iter, ctx.nBL1Iter);
    }

    __aicore__ inline void LoadBL1(const typename Intf::ContextType &ctx,
                                   uint64_t kBL1Iter,
                                   uint64_t nBL1Iter)
    {
        printf("[weightL1]kBL1Iter: %ld, nBL1Iter: %ld\n", kBL1Iter, nBL1Iter);
        uint64_t bGmOffset;
        bool isLastNBL1;
        uint64_t currentNBL1;
        if constexpr (!Intf::hasNL1IterFlag) {
            bGmOffset = kBL1Iter * ctx.convTiling->kBL1;
            isLastNBL1 = true;
            currentNBL1 = ctx.currentNBL1;
        } else {
            bGmOffset = nBL1Iter * ctx.convTiling->nBL1 * coutOffsetBlock +
                kBL1Iter * ctx.convTiling->kBL1;
            isLastNBL1 = nBL1Iter == maxNBL1Iter_;
            currentNBL1 = isLastNBL1 ? ctx.nBL1Tail : ctx.convTiling->nBL1;
        }
        bool isLastKBL1 = (kBL1Iter == maxKBL1Iter_);
        uint64_t currentKBL1 = isLastKBL1 ? ctx.kBL1Tail : ctx.convTiling->kBL1;

        if constexpr (Intf::formatWeight == ConvCommonApi::ConvFormat::FRACTAL_Z) {
            bGmOffset = nBL1Iter * ctx.convTiling->nBL1 * Intf::k0Weight +
                        (kBL1Iter * ctx.convTiling->kBL1) / Intf::k0Weight *
                            ctx.convTiling->orgCo +
                        (kBL1Iter * ctx.convTiling->kBL1) % Intf::k0Weight;
            DataCopyExtParams intriParams;
            intriParams.blockCount = currentKBL1 / Intf::k0Weight;
            intriParams.blockLen = currentNBL1;
            intriParams.srcStride = ctx.convTiling->orgCo - currentNBL1;
            intriParams.dstStride = 0;
            DataCopyPadExtParams<typename Intf::WeightT> padParams;
            padParams.isPad = false;
            padParams.leftPadding = 0;
            padParams.rightPadding = 0;
            padParams.paddingValue = 0;
            printf("[weightL1]bGmOffset: %ld, blockCount: %ld, blockLen: %ld, srcStride: %ld, dstStride: %ld\n",
                bGmOffset, intriParams.blockCount, intriParams.blockLen,
                intriParams.srcStride, intriParams.dstStride);
            DataCopyPad<typename Intf::WeightT, PaddingMode::Normal>(ctx.weightL1,
                                                                     ctx.weightGm[bGmOffset],
                                                                     intriParams,
                                                                     padParams);
        } else if constexpr (Intf::formatWeight == ConvCommonApi::ConvFormat::NCHW) {
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
                DataCopy<typename Intf::WeightT>(ctx.weightL1, ctx.weightGm[bGmOffset], intriParams);
            } else {
                uint64_t dValue = isLastKBL1 ? ctx.convTiling->cinBTailInCore : ctx.convTiling->cinBInCore;
                Dn2NzParams intriParams;
                intriParams.dnNum = currentNBL1;
                intriParams.nValue = ctx.convTiling->kernelHxkernelW;
                intriParams.dValue = dValue;
                intriParams.srcDnMatrixStride = coutOffsetBlock;
                intriParams.srcDValue = ctx.convTiling->kernelHxkernelW;
                intriParams.dstNzC0Stride = ctx.convTiling->kernelHxkernelW * ctx.convTiling->nBL1;
                intriParams.dstNzNStride = ctx.convTiling->nBL1;
                intriParams.dstNzMatrixStride = Intf::k0;
                DataCopy<typename Intf::WeightT>(ctx.weightL1, ctx.weightGm[bGmOffset], intriParams);
            }
        } else if constexpr (Intf::formatWeight == ConvCommonApi::ConvFormat::NHWC) {
            // [zj]TODO
        } else {
            ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported format"); });
        }
    }

private:
    uint64_t maxNBL1Iter_;
    uint64_t maxKBL1Iter_;
    uint32_t coutOffsetBlock;
    bool isOneByOneKernelScenario;
};

template <class Intf>
class LoadBL0Tools {
public:
    __aicore__ inline LoadBL0Tools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        (void)ctx;
    }

    __aicore__ inline void SetN(const typename Intf::ContextType &ctx,
                                uint64_t n)
    {
        ratioOfNToN0 = (ctx.nBL1Iter == ctx.maxNBL1Iter && ctx.nL0Iter == ctx.maxNL0Iter)
                           ? n / ConvApi::BLOCK_L0_N
                           : ctx.convTiling->nStep;
    }

    __aicore__ inline void LoadBL0(const typename Intf::ContextType &ctx, bool isFirst)
    {
        LoadData2DParamsV2 loadData2dParams;
        if (unlikely(isFirst)) {
            loadData2dParams.mStartPosition = ctx.nL0Iter * ctx.convTiling->nL0 / ConvApi::BLOCK_L0_N;
            loadData2dParams.mStep = ratioOfNToN0;
            loadData2dParams.srcStride = ctx.convTiling->nBL1 / ConvApi::BLOCK_L0_N;
            loadData2dParams.dstStride = ratioOfNToN0;
        }
        loadData2dParams.kStartPosition = ctx.kBL0Iter * ctx.convTiling->kStep * Intf::k0 / ConvApi::C0_SIZE;
        loadData2dParams.kStep = ctx.kIter == ctx.maxKL0Iter
                                     ? ConvApi::CeilDIV(ctx.kL0Tail, ConvApi::C0_SIZE)
                                     : ConvApi::CeilDIV(ctx.convTiling->kStep * Intf::k0, ConvApi::C0_SIZE);
        LoadData<typename Intf::WeightT>(ctx.weightL0, ctx.weightL1, loadData2dParams);
    }

private:
    uint64_t ratioOfNToN0;
};

template <class Intf>
class MMadTools {
public:
    __aicore__ inline MMadTools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        (void)ctx;
    }

    __aicore__ inline void SetMN(const typename Intf::ContextType &ctx,
                                 uint64_t m, uint64_t mNotAlign, uint64_t n)
    {
        (void)ctx;
        currentML0_ = m;
        currentMNotAlignL0_ = mNotAlign;
        currentNL0_ = n;
    }

    // [zj]TODO: API
    __aicore__ inline void Mad(const typename Intf::ContextType &ctx)
    {
        SetKernel(ctx);
        SetMFpc(ctx);
        SetMpreQuant(ctx);
        SetMReluAlpha(ctx);
        SetMClipRelu(ctx);
        SetMQuantPost(ctx);
        SetMEltAntiPara(ctx);
        SetMEltSrcPara(ctx);
        SetCubeStridePara(ctx);
        SetChannelSize(ctx);
        SetConvParams(ctx);

        printf("[conv]convConfig: %ld\n", convConfig);

        // [zj]TODO: PartialSum
        convolution((__cbuf__ void *)ctx.outputL1.GetPhyAddr(),
                    (__cbuf__ typename Intf::FmapT *)ctx.fmapL1.GetPhyAddr(),
                    (__cb__ typename Intf::WeightT *)ctx.weightL0.GetPhyAddr(),
                    (__cbuf__ typename Intf::BiasT *)((ctx.hasBias && !biasMatrixBrc)
                                                          ? ctx.biasL1.GetPhyAddr()
                                                          : 0),
                    (uint64_t)((ctx.hasBias && biasMatrixBrc)
                                   ? ctx.biasBT.GetPhyAddr()
                                   : 0),
                    convConfig);
    }

private:
    // [zj]TODO: API
    __aicore__ inline void SetKernel(const typename Intf::ContextType &ctx)
    {
        constexpr uint8_t POS_STRIDE_W = 0;
        constexpr uint8_t POS_STRIDE_H = 6;
        constexpr uint8_t POS_FILTER_W = 12;
        constexpr uint8_t POS_FILTER_H = 21;
        constexpr uint8_t POS_DILATION_W = 30;
        constexpr uint8_t POS_DILATION_H = 38;
        constexpr uint8_t POS_KERNEL_NUM = 46;

        constexpr uint64_t BITS_STRIDE_W = 0x3f;
        constexpr uint64_t BITS_STRIDE_H = 0x3f;
        constexpr uint64_t BITS_FILTER_W = 0x1ff;
        constexpr uint64_t BITS_FILTER_H = 0x1ff;
        constexpr uint64_t BITS_DILATION_W = 0xff;
        constexpr uint64_t BITS_DILATION_H = 0xff;
        constexpr uint64_t BITS_KERNEL_NUM = 0xfff;

        uint64_t kernel = 0;
        kernel |= (uint64_t)(ctx.convTiling->strideW) << POS_STRIDE_W;
        kernel |= (uint64_t)(ctx.convTiling->strideH) << POS_STRIDE_H;
        kernel |= (uint64_t)(ctx.kernelW) << POS_FILTER_W;
        kernel |= (uint64_t)(ctx.kernelH) << POS_FILTER_H;
        kernel |= (uint64_t)(ctx.convTiling->dilationW) << POS_DILATION_W;
        kernel |= (uint64_t)(ctx.convTiling->dilationH) << POS_DILATION_H;
        kernel |= (uint64_t)(currentNL0_) << POS_KERNEL_NUM;
        printf("[conv]kernel: %ld\n", kernel);
        set_kernel(kernel);
    }

    // [zj]TODO: API
    __aicore__ inline void SetMFpc(const typename Intf::ContextType &ctx)
    {
        constexpr uint8_t POS_RELU_PRE_ADDR = 0;
        constexpr uint8_t POS_QUANT_PRE_ADDR = 8;
        constexpr uint8_t POS_QUANT_POST_ADDR = 24;
        constexpr uint8_t POS_ANTIQ_PARA_ADDR = 32;
        constexpr uint8_t POS_FIXED_SHIFT_VAL = 40;

        constexpr uint64_t BITS_RELU_PRE_ADDR = 0xff;
        constexpr uint64_t BITS_QUANT_PRE_ADDR = 0xff;
        constexpr uint64_t BITS_QUANT_POST_ADDR = 0xff;
        constexpr uint64_t BITS_ANTIQ_PARA_ADDR = 0xff;
        constexpr uint64_t BITS_FIXED_SHIFT_VAL = 0x7f;

        constexpr uint32_t ALIGN_64B = 6;
        constexpr uint32_t ALIGN_128B = 7;

        uint64_t preReluFBAddr = 
            (uint64_t)(ctx.hasVectorPreRelu ? (ctx.preReluFB.GetPhyAddr()) : 0) >> ALIGN_64B;
        uint64_t preQuantFBAddr = 
            (uint64_t)(ctx.hasVectorPreQuant ? (ctx.preQuantFB.GetPhyAddr()) : 0) >> ALIGN_128B;
        uint64_t postQuantFBAddr =
            (uint64_t)(ctx.hasVectorPostQuant ? (ctx.postQuantFB.GetPhyAddr()) : 0) >> ALIGN_128B;
        uint64_t antiQuantFBAddr = 
            (uint64_t)(ctx.hasVectorAntiQuant ? (ctx.antiQuantFB.GetPhyAddr()) : 0) >> ALIGN_64B;
        printf("[conv]preReluFB: %ld, preQuantFB: %ld, postQuantFB: %ld, antiQuantFB: %ld\n",
            preReluFBAddr,
            preQuantFBAddr,
            postQuantFBAddr,
            antiQuantFBAddr);

        preReluFBAddr = preReluFBAddr & BITS_RELU_PRE_ADDR;
        preQuantFBAddr = preQuantFBAddr & BITS_QUANT_PRE_ADDR;
        postQuantFBAddr = postQuantFBAddr & BITS_QUANT_POST_ADDR;
        antiQuantFBAddr = antiQuantFBAddr & BITS_ANTIQ_PARA_ADDR;
        printf("[conv]preReluFB: %ld, preQuantFB: %ld, postQuantFB: %ld, antiQuantFB: %ld\n",
            preReluFBAddr,
            preQuantFBAddr,
            postQuantFBAddr,
            antiQuantFBAddr);

        uint64_t mfpc = 0;
        mfpc |= preReluFBAddr << POS_RELU_PRE_ADDR;
        mfpc |= preQuantFBAddr << POS_QUANT_PRE_ADDR;
        mfpc |= postQuantFBAddr << POS_QUANT_POST_ADDR;
        mfpc |= antiQuantFBAddr << POS_ANTIQ_PARA_ADDR;
        mfpc |= (uint64_t)FIXED_SHIFT_VAL << POS_FIXED_SHIFT_VAL;
        printf("[conv]mfpc: %ld\n", mfpc);
        set_m_fpc(mfpc);
    }

    // [zj]TODO: API
    __aicore__ inline void SetMpreQuant(const typename Intf::ContextType &ctx)
    {
        if (ctx.hasPreQuant && !ctx.hasVectorPreQuant) {
            typename Intf::PreQuantT preQuant = ctx.preQuantGm.GetValue(0);
            uint64_t preQuantSpr = *reinterpret_cast<typename Intf::PreQuantT*>(&preQuant);
            printf("[conv]preQuantSpr: %ld\n", preQuantSpr);
            set_m_quant_pre(preQuantSpr);
        }
    }

    // [zj]TODO: API
    __aicore__ inline void SetMReluAlpha(const typename Intf::ContextType &ctx)
    {
        if (ctx.hasPreRelu && !ctx.hasVectorPreRelu) {
            typename Intf::PreReluT preRelu = ctx.preReluGm.GetValue(0);
            uint64_t preReluSpr = *reinterpret_cast<typename Intf::PreReluT*>(&preRelu);
            printf("[conv]preReluSpr: %ld\n", preReluSpr);
            set_m_relu_alpha(preReluSpr);
        }
    }

    // [zj]TODO: API
    __aicore__ inline void SetMClipRelu(const typename Intf::ContextType &ctx)
    {
        if (ctx.hasPreClipRelu && !ctx.hasVectorPreClipRelu) {
            typename Intf::PreClipReluT preClipRelu = ctx.preClipReluGm.GetValue(0);
            // [zj]TODO: to fix error
            // uint32_t preClipReluSpr = *reinterpret_cast<typename Intf::PreClipReluT*>(&preClipRelu);
            // printf("[conv]preClipReluSpr: %d\n", preClipReluSpr);
            // set_m_clip_relu(preClipReluSpr);
        }
    }

    // [zj]TODO: API
    __aicore__ inline void SetMQuantPost(const typename Intf::ContextType &ctx)
    {
        if (ctx.hasPostQuant && !ctx.hasVectorPostQuant) {
            typename Intf::PostQuantT postQuant = ctx.postQuantGm.GetValue(0);
            uint64_t postQuantSpr = *reinterpret_cast<typename Intf::PostQuantT*>(&postQuant);
            printf("[conv]postQuantSpr: %ld\n", postQuantSpr);
            set_m_quant_post(postQuantSpr);
        }
    }

    // [zj]TODO: API
    __aicore__ inline void SetMEltAntiPara(const typename Intf::ContextType &ctx)
    {
        constexpr uint32_t POS_ANTIQ_MODE = 44;

        constexpr uint32_t BITS_ANTIQ_MODE = 0xf;

        uint64_t antiQuantSpr = 0;
        if (ctx.hasAntiQuant && !ctx.hasVectorAntiQuant) {
            typename Intf::AntiQuantT antiQuant = ctx.antiQuantGm.GetValue(0);
            uint64_t antiQuantSpr = *reinterpret_cast<typename Intf::AntiQuantT*>(&antiQuant);
            printf("[conv]antiQuantSpr: %ld\n", antiQuantSpr);
        }
        antiQuantSpr |= (uint64_t)(GetAntiQuantMode(static_cast<uint8_t>(ctx.convTiling->antiQuantMode))) << POS_ANTIQ_MODE;
        set_m_elt_antiq_para(antiQuantSpr);
    }

    // [zj]TODO: API
    __aicore__ inline void SetMEltSrcPara(const typename Intf::ContextType &ctx)
    {
        if (ctx.hasEltwise) {
            constexpr uint8_t POS_C0_INDICATOR = 0;
            constexpr uint8_t POS_L1_ADDR = 16;
            constexpr uint8_t POS_WCO_STRIDE = 32;
            constexpr uint8_t POS_HWCO_STRIDE = 48;

            constexpr uint64_t BITS_C0_INDICATOR = 1;
            constexpr uint64_t BITS_L1_ADDR = 0xffff;
            constexpr uint64_t BITS_WCO_STRIDE = 0xffff;
            constexpr uint64_t BITS_HWCO_STRIDE = 0xffff;

            constexpr uint32_t ALIGN_32B = 5;

            uint64_t WC0_stride = 1;
            uint64_t HWC0_stride = 1;
            uint64_t offsetL1 = ctx.convTiling->eltwiseBrcEn && ctx.convTiling->fixpParamsFullLoadFlag
                                    ? ctx.nBL1Iter * ctx.convTiling->nBL1 +
                                        ctx.nL0Iter * ctx.convTiling->nL0
                                    : 0;
            if (!ctx.convTiling->eltwiseBrcEn) {
                if constexpr (Intf::outputOrder == static_cast<int8_t>(Conv2dApi::ConvOutputOrder::M_MODE)) {
                    WC0_stride = ctx.orgWo;
                    HWC0_stride = currentMNotAlignL0_;
                } else {
                    WC0_stride = ctx.currentWoL1;
                    HWC0_stride = ctx.currentHoL1 * ctx.currentWoL1;
                }
            }

            // [zj]TODO: C0_INDICATOR
            uint64_t meltSrcPara = 0;
            meltSrcPara |= (uint64_t)(0) << POS_C0_INDICATOR;
            meltSrcPara |= (((uint64_t)(ctx.eltwiseL1[offsetL1].GetPhyAddr()) & BITS_L1_ADDR) >> ALIGN_32B) << POS_L1_ADDR;
            meltSrcPara |= (uint64_t)(WC0_stride) << POS_WCO_STRIDE;
            meltSrcPara |= (uint64_t)(HWC0_stride) << POS_HWCO_STRIDE;
            printf("[conv]meltSrcPara: %ld\n", meltSrcPara);
            set_m_elt_src_para(meltSrcPara);
        }
    }

    // [zj]TODO: API
    __aicore__ inline void SetCubeStridePara(const typename Intf::ContextType &ctx)
    {
        constexpr uint8_t POS_LOOP3_DST_STRIDE = 0;
        constexpr uint8_t POS_LOOP4_DST_STRIDE = 16;

        constexpr uint64_t BITS_LOOP3_DST_STRIDE = 0xffff;
        constexpr uint64_t BITS_LOOP4_DST_STRIDE = 0xffff;

        uint64_t WC0_stride = 0;
        uint64_t HWC0_stride = 0;
        if constexpr (Intf::outputOrder == static_cast<int8_t>(Conv2dApi::ConvOutputOrder::M_MODE)) {
            WC0_stride = ctx.orgWo; 
            HWC0_stride = currentMNotAlignL0_;
        } else {
            WC0_stride = ctx.currentWoL1; 
            HWC0_stride = ctx.currentHoL1 * ctx.currentWoL1;
        }

        uint64_t config = 0;
        config |= (uint64_t)(WC0_stride) << POS_LOOP3_DST_STRIDE;
        config |= (uint64_t)(HWC0_stride) << POS_LOOP4_DST_STRIDE;
        printf("[conv]CubeStridePara: %ld\n", config);
        set_cube_stride_para(config);
    }

    __aicore__ inline bool IsKL0Tail(const typename Intf::ContextType &ctx)
    {
        return ctx.kIter == ctx.maxKL0Iter;
    }

    __aicore__ inline void SetChannelSize(const typename Intf::ContextType &ctx)
    {
        if constexpr (Intf::outputOrder == static_cast<int8_t>(Conv2dApi::ConvOutputOrder::HW_MODE)) {
            if constexpr (Intf::kPreLoadFlag) {
                channelSize_ = ctx.convTiling->isC04Flag
                                   ? Conv2dApi::C04_CIN_SIZE
                                   : (ctx.kAL1Iter - 1 != ctx.maxKAL1Iter
                                             ? ctx.convTiling->cinAInCore
                                             : ConvApi::AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
            } else {
                channelSize_ = ctx.convTiling->isC04Flag
                                   ? Conv2dApi::C04_CIN_SIZE
                                   : (ctx.kAL1Iter != ctx.maxKAL1Iter
                                             ? ctx.convTiling->cinAInCore
                                             : ConvApi::AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
            }
        } else if constexpr (Intf::outputOrder == static_cast<int8_t>(Conv2dApi::ConvOutputOrder::M_MODE)) {
            if constexpr (Intf::kPreLoadFlag) {
                channelSize_ = (ctx.kAL1Iter - 1 != ctx.maxKAL1Iter
                                   ? ctx.convTiling->cinAInCore
                                   : ConvApi::AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
            } else {
                channelSize_ = (ctx.kAL1Iter != ctx.maxKAL1Iter
                                   ? ctx.convTiling->cinAInCore
                                   : ConvApi::AlignB(ctx.convTiling->cinATailInCore, Intf::k0));
            }
        }
        printf("[conv]channelSize_: %ld\n", channelSize_);
    }

    __aicore__ inline void SetConvParams(const typename Intf::ContextType &ctx)
    {
        constexpr uint8_t POS_BIAS_MATRIX_INIT = 0;
        constexpr uint8_t POS_BIAS_MATRIX_BRC = 1;
        constexpr uint8_t POS_PRE_QUANT_MODE = 2;
        constexpr uint8_t POS_PRE_RELU_MODE = 7;
        constexpr uint8_t POS_POST_QUANT_MODE = 10;
        constexpr uint8_t POS_POST_RELU_MODE = 15;
        constexpr uint8_t POS_PRE_CLIP_RELU_MODE = 18;
        constexpr uint8_t POS_ELTWISE_OP = 20;
        constexpr uint8_t POS_ELTWISE_ANTIQ_EN = 23;
        constexpr uint8_t POS_ELTWISE_M_BRC = 24;
        constexpr uint8_t POS_MASK_CTRL = 28;
        constexpr uint8_t POS_DUAL_SRC_EN = 34;
        constexpr uint8_t POS_COMPACT_EN = 35;
        constexpr uint8_t POS_DEPEND_EN = 36;
        constexpr uint8_t POS_INSTR_ID = 37;
        constexpr uint8_t POS_POST_PROC_HALF = 39;
        constexpr uint8_t POS_NBRC_BIAS_CTRL = 40;
        constexpr uint8_t POS_CIN = 48;

        constexpr uint64_t BITS_BIAS_MATRIX_INIT = 0x1;
        constexpr uint64_t BITS_BIAS_MATRIX_BRC = 0x1;
        constexpr uint64_t BITS_PRE_QUANT_MODE = 0x1f;
        constexpr uint64_t BITS_PRE_RELU_MODE = 0x7;
        constexpr uint64_t BITS_POST_QUANT_MODE = 0x1f;
        constexpr uint64_t BITS_POST_RELU_MODE = 0x7;
        constexpr uint64_t BITS_PRE_CLIP_RELU_MODE = 0x1;
        constexpr uint64_t BITS_ELTWISE_OP = 0x7;
        constexpr uint64_t BITS_ELTWISE_ANTIQ_EN = 0x1;
        constexpr uint64_t BITS_ELTWISE_M_BRC = 0x1;
        constexpr uint64_t BITS_MASK_CTRL = 0x7;
        constexpr uint64_t BITS_DUAL_SRC_EN = 0x1;
        constexpr uint64_t BITS_COMPACT_EN = 0x1;
        constexpr uint64_t BITS_DEPEND_EN = 0x1;
        constexpr uint64_t BITS_INSTR_ID = 0x3;
        constexpr uint64_t BITS_POST_PROC_HALF = 0x1;
        constexpr uint64_t BITS_NBRC_BIAS_CTRL = 0x1;
        constexpr uint64_t BITS_CIN = 0xffff;

        // [zj]TODO：bias
        biasMatrixInitVal = (!ctx.hasBias && ctx.kIter != 0);
        biasMatrixBrc = (ctx.hasBias && ctx.kIter == 0);

        // fixpipe
        preQuantMode = GetPreQuantMode(static_cast<QuantMode_t>(ctx.convTiling->preQuantMode));
        preReluMode = GetPreReluMode(static_cast<ReluMode>(ctx.convTiling->preReluMode));
        preClipReluMode = GetPreClipReluMode(static_cast<ClipReluMode>(ctx.convTiling->preClipReluMode));
        postQuantMode = GetPostQuantMode(static_cast<QuantMode_post>(ctx.convTiling->postQuantMode));

        // eltwise
        eltwiseOp = (eltwise_op_t)(ctx.convTiling->eltwiseOp);
        eltwiseAntiqEn = (ctx.hasAntiQuant);
        eltwiseBrcEn = (ctx.convTiling->eltwiseBrcEn);

        // [zj]TODO
        // convFixParams.SetDependEnable(dependEnable); // depend
        // convFixParams.SetInstrId(instrId); // id

        // channel size
        n = (channelSize_);

        convConfig = 0;
        convConfig |= (((uint64_t)biasMatrixInitVal) << POS_BIAS_MATRIX_INIT);
        convConfig |= (((uint64_t)biasMatrixBrc) << POS_BIAS_MATRIX_BRC);
        convConfig |= (((uint64_t)preQuantMode) << POS_PRE_QUANT_MODE);
        convConfig |= (((uint64_t)preReluMode) << POS_PRE_RELU_MODE);
        convConfig |= (((uint64_t)postQuantMode) << POS_POST_QUANT_MODE);
        convConfig |= (((uint64_t)preClipReluMode) << POS_PRE_CLIP_RELU_MODE);
        convConfig |= (((uint64_t)eltwiseOp) << POS_ELTWISE_OP);
        convConfig |= (((uint64_t)eltwiseAntiqEn) << POS_ELTWISE_ANTIQ_EN);
        convConfig |= (((uint64_t)eltwiseBrcEn) << POS_ELTWISE_M_BRC);
        // [zj]TODO
        // convConfig |= (((uint64_t)lsbMask) << POS_MASK_CTRL);
        // convConfig |= (((uint64_t)dualSrcEnable) << POS_DUAL_SRC_EN);
        // convConfig |= (((uint64_t)compactEnable) << POS_COMPACT_EN);
        // convConfig |= (((uint64_t)dependEnable) << POS_DEPEND_EN);
        // convConfig |= (((uint64_t)instrId) << POS_INSTR_ID);
        // convConfig |= (((uint64_t)postProcessBandwithCtrl) << POS_POST_PROC_HALF);
        convConfig |= (((uint64_t)n) << POS_CIN);
    }

private:
    inline __aicore__ pre_quant_t GetPreQuantMode(QuantMode_t preQuant)
    {
        switch (preQuant) {
            case QuantMode_t::NoQuant:
                return pre_quant_t::No_Quant;
            case QuantMode_t::VREQ8:
                return pre_quant_t::REQS8_Vector;
            case QuantMode_t::REQ8:
                return pre_quant_t::REQS8_Scalar;
            case QuantMode_t::VDEQF16:
                return pre_quant_t::DEQF16_Vector;
            case QuantMode_t::DEQF16:
                return pre_quant_t::DEQF16_Scalar;
            case QuantMode_t::VDEQS16:
                return pre_quant_t::DEQS16_Vector;
            case QuantMode_t::DEQS16:
                return pre_quant_t::DEQS16_Scalar;
            case QuantMode_t::VDEQS32_INT:
                return pre_quant_t::DEQS32_Vector;
            case QuantMode_t::DEQS32_INT:
                return pre_quant_t::DEQS32_Scalar;
            case QuantMode_t::VREQ4:
                return pre_quant_t::REQS4_Vector;
            case QuantMode_t::REQ4:
                return pre_quant_t::REQS4_Scalar;
            // [zj]TODO: fp8
            // pre_quant_t::QS322FP8_E4M3_INT_Vector,
            // pre_quant_t::QS322FP8_E4M3_INT_Scalar,
            default:
                return pre_quant_t::No_Quant;
        }
    }

    inline __aicore__ pre_quant_t GetPreQuantMode(pre_quant_t preQuant)
    {
        return preQuant;
    }

    inline __aicore__ pre_relu_t GetPreReluMode(ReluMode preRelu)
    {
        switch (preRelu) {
            case ReluMode::NO_RELU:
                return pre_relu_t::NoRelu;
            case ReluMode::NORMAL_RELU:
                return pre_relu_t::NormalRelu;
            case ReluMode::SCALAR_RELU:
                return pre_relu_t::ScalarRelu;
            case ReluMode::VECTOR_RELU:
                return pre_relu_t::VectorRelu;
            case ReluMode::LUT_GELU:
            case ReluMode::LUT_SILU:
            case ReluMode::LUT_SIGMOID:
            case ReluMode::LUT_TANH:
                return pre_relu_t::LUTActivation;
            default:
                return pre_relu_t::NoRelu;
        }
    }

    inline __aicore__ pre_relu_t GetPreReluMode(pre_relu_t preRelu)
    {
        return preRelu;
    }

    inline __aicore__ uint8_t GetPreClipReluMode(ClipReluMode preClipRelu)
    {
        switch (preClipRelu) {
            case ClipReluMode::NOCLIP_RELU:
                return 0;
            case ClipReluMode::CLIP_RELU:
                return 1;
            default:
                return 0;
        }
    }

    inline __aicore__ uint8_t GetAntiQuantMode(uint8_t antiQuant)
    {
        return antiQuant;
    }

    inline __aicore__ post_quant_t GetPostQuantMode(QuantMode_post postQuant)
    {
        switch (postQuant) {
            case QuantMode_post::NoConv:
                return post_quant_t::No_Quant;
            case QuantMode_post::VSHIFT2S16_POST:
                return post_quant_t::SHIFT2S16_Vector;
            case QuantMode_post::SHIFT2S16_POST:
                return post_quant_t::SHIFT2S16_Scalar;
            case QuantMode_post::VSHIFT2S8_POST:
                return post_quant_t::SHIFT2S8_Vector;
            case QuantMode_post::SHIFT2S8_POST:
                return post_quant_t::SHIFT2S8_Scalar;
            case QuantMode_post::VSHIFT2S4_POST:
                return post_quant_t::SHIFT2S4_Vector;
            case QuantMode_post::SHIFT2S4_POST:
                return post_quant_t::SHIFT2S4_Scalar;
            // [zj]TODO: 
            // SHIFT2S32_Scalar;
            // SHIFT2S32_Vector;
            default:
                return post_quant_t::No_Quant;
        }
    }

    inline __aicore__ post_quant_t GetPostQuantMode(post_quant_t postQuant)
    {
        return postQuant;
    }

    inline __aicore__ post_relu_t GetPostReluMode(ReluMode postRelu)
    {
        switch (postRelu) {
            case ReluMode::NO_RELU:
                return post_relu_t::NoRelu;
            case ReluMode::NORMAL_RELU:
                return post_relu_t::NormalRelu;
            default:
                return post_relu_t::NoRelu;
        }
    }

    inline __aicore__ post_relu_t GetPostReluMode(post_relu_t postRelu)
    {
        return postRelu;
    }

    inline __aicore__ uint8_t GetPostClipReluMode(ClipReluMode postClipRelu)
    {
        (void)postClipRelu;
        return 0;
    }

private:
    uint64_t currentML0_;
    uint64_t currentMNotAlignL0_;
    uint64_t currentNL0_;
    uint64_t channelSize_;

    uint64_t convConfig;

    bool biasMatrixInitVal;
    bool biasMatrixBrc;
    pre_quant_t preQuantMode;
    pre_relu_t preReluMode;
    post_quant_t postQuantMode;
    uint8_t preClipReluMode;
    eltwise_op_t eltwiseOp;
    bool eltwiseAntiqEn;
    bool eltwiseBrcEn;
    // lsb_mask_t lsbMask = lsb_mask_t::Disable;
    // bool dualSrcEnable = false;
    // bool compactEnable = false;
    // bool dependEnable = false;
    // instr_id_t instrId = instr_id_t::ID_0;
    // bool postProcessBandwithCtrl = false;
    uint16_t n;
};
};

#endif // API_CONV2D_INSTR_IMPL_H
