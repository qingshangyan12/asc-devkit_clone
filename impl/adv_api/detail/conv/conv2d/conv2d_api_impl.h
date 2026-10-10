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
 * \file conv2d_api_impl.h
 * \brief
 */
#ifndef API_CONV2D_API_IMPL_H
#define API_CONV2D_API_IMPL_H

#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "kernel_tiling/kernel_tiling.h"
#include "../../../lib/conv/conv2d/conv2d_config.h"
#include "conv2d_util.h"
#include "impl/conv2d_instr.h"
#else
#include "kernel_tiling/kernel_tiling.h"
#include "include/adv_api/conv/conv2d/conv2d_config.h"
#include "conv2d_util.h"
#include "impl/conv2d_instr.h"
#endif

namespace Conv2dApi {
using namespace AscendC;
using AscendC::tiling::TConv2DApiTiling;

template<typename Intf, class Config>
struct Conv2dApiImpl {
public:
    using ConvParam = typename Config::ConvParam;
    constexpr static uint32_t ImplType = Config::implType;

public:
    __aicore__ inline Conv2dApiImpl() {}

    struct ContextData: public Config::ContextData {
        __aicore__ inline ContextData()
        {
        }

        TPipe *pipe;
        const TConv2DApiTiling* __restrict convTiling;

        GlobalTensor<typename Config::FmapT> fmapGm;
        GlobalTensor<typename Config::WeightT> weightGm;
        GlobalTensor<typename Config::BiasT> biasGm;
        GlobalTensor<typename Config::PreQuantT> preQuantGm;
        GlobalTensor<typename Config::PreReluT> preReluGm;
        GlobalTensor<typename Config::PreClipReluT> preClipReluGm;
        GlobalTensor<typename Config::EltwiseT> eltwiseGm;
        GlobalTensor<typename Config::AntiQuantT> antiQuantGm;
        GlobalTensor<typename Config::PostQuantT> postQuantGm;
        GlobalTensor<typename Config::PostReluT> postReluGm;
        GlobalTensor<typename Config::PostClipReluT> postClipReluGm;
        GlobalTensor<typename Config::PaddingT> paddingGm;

        // zj: preload
        TQue<QuePosition::A1, 1> queueFmapL1;
        TQue<QuePosition::B1, 1> queueWeightL1;
        TQue<QuePosition::B1, 1> queueBiasL1;
        TQue<QuePosition::B1, 1> queuePreQuantL1;
        TQue<QuePosition::B1, 1> queuePreReluL1;
        TQue<QuePosition::B1, 1> queuePreClipReluL1;
        TQue<QuePosition::B1, 1> queueAntiQuantL1;
        TQue<QuePosition::A1, 1> queueEltwiseL1;
        TQue<QuePosition::B1, 1> queuePostQuantL1;
        TQue<QuePosition::B1, 1> queuePostReluL1;
        TQue<QuePosition::B1, 1> queuePostClipReluL1;
        TQue<QuePosition::B1, 1> queuePaddingL1;
        TQue<QuePosition::CO1, 1> queueOutputL1;
        TQue<QuePosition::CO1, 1> queueOutputL0;
        TQue<QuePosition::CO1, 1> queuePartialReadWrite0;
        TQue<QuePosition::CO1, 1> queuePartialReadWrite1;

        TQue<TPosition::C2, 1> queueBiasBT;
        TQue<TPosition::C2PIPE2GM, 1> queuePreQuantFB;
        TQue<TPosition::C2PIPE2GM, 1> queuePreReluFB;
        TQue<TPosition::C2PIPE2GM, 1> queuePreClipReluFB;
        TQue<TPosition::C2PIPE2GM, 1> queueAntiQuantFB;
        TQue<TPosition::C2PIPE2GM, 1> queuePostQuantFB;
        TQue<TPosition::C2PIPE2GM, 1> queuePostReluFB;
        TQue<TPosition::C2PIPE2GM, 1> queuePostClipReluFB;
        TQue<TPosition::C2PT, 1> queuePaddingPT;

        TBuf<TPosition::A2> l0aBuf;
        TBuf<TPosition::B2> l0bBuf;

        LocalTensor<typename Config::FmapT> fmapL1;
        LocalTensor<typename Config::WeightT> weightL1;
        LocalTensor<typename Config::BiasT> biasL1;
        LocalTensor<typename Config::PreQuantT> preQuantL1;
        LocalTensor<typename Config::PreReluT> preReluL1;
        LocalTensor<typename Config::PreClipReluT> preClipReluL1;
        LocalTensor<typename Config::AntiQuantT> antiQuantL1;
        LocalTensor<typename Config::EltwiseT> eltwiseL1;
        LocalTensor<typename Config::PostQuantT> postQuantL1;
        LocalTensor<typename Config::PostReluT> postReluL1;
        LocalTensor<typename Config::PostClipReluT> postClipReluL1;
        LocalTensor<typename Config::PaddingT> paddingL1;
        LocalTensor<typename Config::OutputT> outputL1;
        LocalTensor<typename Config::OutputT> output1L1;

        LocalTensor<typename Config::FmapT> wholeAL0;
        LocalTensor<typename Config::WeightT> wholeBL0;
        LocalTensor<typename Config::FmapT> fmapL0;
        LocalTensor<typename Config::WeightT> weightL0;
        LocalTensor<typename Config::L0cT> outputL0;
        LocalTensor<typename Config::L0cT> partialReadWrite0L1;
        LocalTensor<typename Config::L0cT> partialReadWrite1L1;

        LocalTensor<typename Config::L0cT> biasBT;
        LocalTensor<typename Config::PreQuantT> preQuantFB;
        LocalTensor<typename Config::PreReluT> preReluFB;
        LocalTensor<typename Config::PreClipReluT> preClipReluFB;
        LocalTensor<typename Config::AntiQuantT> antiQuantFB;
        LocalTensor<typename Config::PostQuantT> postQuantFB;
        LocalTensor<typename Config::PostReluT> postReluFB;
        LocalTensor<typename Config::PostClipReluT> postClipReluFB;
        LocalTensor<typename Config::PaddingT> paddingPT;

        uint64_t singleCoreCi;
        uint64_t singleCoreCo;
        uint64_t singleCoreM;
        uint64_t singleCoreHo;
        uint64_t singleCoreWo;

        uint64_t orgCi;
        uint64_t orgCo;
        uint64_t orgHi;
        uint64_t orgWi;
        uint64_t orgHo;
        uint64_t orgWo;
        uint64_t kernelH;
        uint64_t kernelW;

        uint64_t dilatedKernelH;
        uint64_t dilatedKernelW;

        uint8_t hasBias;
        uint8_t hasPreQuant;
        uint8_t hasPreRelu;
        uint8_t hasPreClipRelu;
        uint8_t hasAntiQuant;
        uint8_t hasEltwise;
        uint8_t hasPostQuant;
        uint8_t hasPostRelu;
        uint8_t hasPostClipRelu;
        uint8_t hasPadding;

        uint8_t hasVectorPreQuant;
        uint8_t hasVectorPreRelu;
        uint8_t hasVectorPreClipRelu;
        uint8_t hasVectorAntiQuant;
        uint8_t hasVectorPostQuant;
        uint8_t hasVectorPostRelu;
        uint8_t hasVectorPostClipRelu;
        uint8_t hasVectorPadding;
        uint8_t hasVectorFixp;

        uint64_t fmapOneBatchSize;
        uint64_t outputOneBatchSize;

        uint64_t mAL1;
        uint64_t mL0;
        uint8_t mAL1UpdateFlag;

        int64_t ciStartPos;
        int64_t coStartPos;
        uint64_t mStartPos;
        int64_t hiStartPos;
        int64_t wiStartPos;

        uint8_t isFirstIterate;

        uint64_t iter;
        uint64_t kIter;
        mutable uint64_t currentFmapL1Offset;
        mutable uint64_t fmapL1OffsetStep;
        uint64_t batchIter;
        uint64_t kAL1Iter;
        uint64_t kBL1Iter;
        uint64_t nBL1Iter;
        uint64_t kAL0Iter;
        uint64_t kBL0Iter;
        uint64_t nL0Iter;
        uint64_t mAL1Iter;
        uint64_t mL0Iter;
        uint64_t hoAL1Iter;
        uint64_t woAL1Iter;
        uint64_t hoL0Iter;
        uint64_t woL0Iter;

        uint64_t ddr2l1LoopBatch;
        uint64_t ddr2l1LoopN;
        uint64_t l12l0LoopN;
        uint64_t ddr2l0LoopK;
        uint64_t ddr2l1LoopM;
        uint64_t l12l0LoopM;
        uint64_t ddr2l1LoopH;
        uint64_t ddr2l1LoopW;
        uint64_t l12l0LoopH;
        uint64_t l12l0LoopW;

        uint64_t multiKAL1;
        uint64_t multiKBL1;

        uint64_t maxKAL1Iter;
        uint64_t maxKBL1Iter;
        uint64_t maxNBL1Iter;
        uint64_t maxKL0Iter;
        uint64_t maxNL0Iter;
        uint64_t maxMAL1Iter;
        uint64_t maxML0Iter;
        uint64_t maxHoL1Iter;
        uint64_t maxWoL1Iter;
        uint64_t maxHoL0Iter;
        uint64_t maxWoL0Iter;

        uint64_t kAL1Tail;
        uint64_t kBL1Tail;
        uint64_t nBL1Tail;
        uint64_t kL0Tail;
        uint64_t nL0Tail;
        uint64_t mAL1Tail;
        uint64_t mAL0Tail;
        uint64_t hoAL1Tail;
        uint64_t woAL1Tail;
        uint64_t woL1SmallTail;
        uint64_t hoL0Tail;
        uint64_t woL0Tail;

        uint64_t currentNBL1;
        uint64_t currentNL0Align;
        uint64_t currentMAL1;
        uint64_t currentML0Align;
        uint64_t currentHoL1;
        uint64_t currentWoL1;
        uint64_t currentHoL0;
        uint64_t currentWoL0;

        uint8_t kAL1fullload;
        uint8_t kBL1fullload;

        uint8_t loadAL1Flag;
        uint8_t loadBL1Flag;
        uint8_t loadAL0Flag;
        uint8_t loadBL0Flag;

        uint64_t lastLoopKAL1StartPos;
        uint64_t lastLoopKBL1StartPos;

        uint16_t kL0FullLoadAl0PingPongFlag;
        uint16_t kL0FullLoadBl0PingPongFlag;

        // zj: group conv
    };

    struct ImplDataType {
        __aicore__ inline ImplDataType() {};
    };
};
}  // namespace Conv2dApi

#endif // API_CONV2D_API_IMPL_H
