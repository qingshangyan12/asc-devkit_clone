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

#include "lib/conv/conv2d/conv2d_config.h" // [zj]TODO: blue
#include "conv2d_util.h"

namespace Conv2dApi {
using namespace AscendC;

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
        const TConv2DTiling* __restrict convTiling;

        // GM Tensor
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

        // [zj]TODO: preload
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
        TQue<QuePosition::CO1, 1> queueOutputL1;

        TQue<TPosition::C2, 1> queueBiasBT;
        TQue<TPosition::C2PIPE2GM, 1> queuePreQuantFB;
        TQue<TPosition::C2PIPE2GM, 1> queuePreReluFB;
        TQue<TPosition::C2PIPE2GM, 1> queuePreClipReluFB;
        TQue<TPosition::C2PIPE2GM, 1> queueAntiQuantFB;
        TQue<TPosition::C2PIPE2GM, 1> queuePostQuantFB;
        TQue<TPosition::C2PIPE2GM, 1> queuePostReluFB;
        TQue<TPosition::C2PIPE2GM, 1> queuePostClipReluFB;

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
        LocalTensor<typename Config::OutputT> outputL1;
        LocalTensor<typename Config::OutputT> output1L1;

        LocalTensor<typename Config::FmapT> wholeAl0Tensor;
        LocalTensor<typename Config::FmapT> al0;
        LocalTensor<typename Config::WeightT> wholeBl0Tensor;
        LocalTensor<typename Config::WeightT> bl0;
        LocalTensor<typename Config::L0cT> cl0;

        LocalTensor<typename Config::BiasT> biasBT;
        LocalTensor<typename Config::PreQuantT> preQuantFB;
        LocalTensor<typename Config::PreReluT> preReluFB;
        LocalTensor<typename Config::PreClipReluT> preClipReluFB;
        LocalTensor<typename Config::AntiQuantT> antiQuantFB;
        LocalTensor<typename Config::PostQuantT> postQuantFB;
        LocalTensor<typename Config::PostReluT> postReluFB;
        LocalTensor<typename Config::PostClipReluT> postClipReluFB;

        // [zj]TODO: del no need
        int64_t ciStartPos;
        int64_t coStartPos;

        uint64_t fmapOneBatchSize;
        uint64_t outputOneBatchSize;
        uint64_t dilatedKernelH;
        uint64_t dilatedKernelW;

        // Iterate Var
        uint8_t isFirstIterate; // Iterate
        uint8_t kAL1fullload; // A gm->L1k
        uint8_t kBL1fullload; // B gm->L1k
        uint8_t loadAL1Flag; // AL1
        uint8_t loadBL1Flag; // BL1
        uint8_t loadAL0Flag; // AL0
        uint8_t loadBL0Flag; // BL0

        uint64_t iter;
        uint64_t kIter; // kDDRL0
        uint64_t batchIter; // batch
        uint64_t kAL1Iter; // AL1k
        uint64_t kBL1Iter; // BL1k
        uint64_t nBL1Iter; // BL1n
        uint64_t kAL0Iter; // L1A L0
        uint64_t kBL0Iter; // L1B L0
        uint64_t nL0Iter; // BL0n

        uint64_t multiKAL1;
        uint64_t multiKBL1;
        uint64_t maxKAL1Iter;
        uint64_t maxKBL1Iter;
        uint64_t maxNBL1Iter;
        uint64_t maxKL0Iter;
        uint64_t maxNL0Iter ;

        uint64_t ddr2l1LoopBatch;
        uint64_t ddr2l1LoopN;
        uint64_t ddr2l0LoopK;
        uint64_t l12l0LoopN;

        uint64_t kAL1Tail;  // AL1k
        uint64_t kBL1Tail; // BL1k
        uint64_t nBL1Tail; // BL1N
        uint64_t kL0Tail;
        uint64_t nL0Tail; // BL0n

        uint64_t currentNBL1;
        uint64_t currentNL0Align;
        uint64_t currentML0Align;

        uint64_t lastLoopKAL1StartPos;
        uint64_t lastLoopKBL1StartPos;
        uint16_t kL0FullLoadAl0PingPongFlag;
        uint16_t kL0FullLoadBl0PingPongFlag;

        // Used in m_mode
        uint64_t mStartPos;
        uint64_t singleCoreM;
        uint8_t mAL1UpdateFlag;
        uint64_t mAL1Iter;
        uint64_t mL0Iter;
        uint64_t maxMAL1Iter;
        uint64_t maxML0Iter;
        uint64_t ddr2l1LoopM;
        uint64_t l12l0LoopM;
        uint64_t mAL1;
        uint64_t mAL1Tail;
        uint64_t mL0;
        uint64_t mAL0Tail;

        // Used in hw_mode
        int64_t hiStartPos;
        int64_t wiStartPos;
        uint64_t hoAL1Iter;  // AL1ho
        uint64_t woAL1Iter;  // AL1wo
        uint64_t hoL0Iter; // L0Aho
        uint64_t woL0Iter; // L0Awo
        uint64_t maxHoL1Iter;
        uint64_t maxWoL1Iter;
        uint64_t maxHoL0Iter;
        uint64_t maxWoL0Iter;
        
        uint64_t ddr2l1LoopH;
        uint64_t ddr2l1LoopW;
        uint64_t l12l0LoopH;
        uint64_t l12l0LoopW;
        
        uint64_t hoAL1Tail; // AL1ho
        uint64_t woAL1Tail; // AL1wo
        uint64_t woL1SmallTail; // wo16
        uint64_t hoL0Tail;
        uint64_t woL0Tail;
        
        uint64_t currentHoL1;
        uint64_t currentWoL1;
        uint64_t currentHoL0;
        uint64_t currentWoL0;

        // // Used in opt group mode(groups > 1)
        // ConvFunc::LoadGM2UBTools<Intf> loadGm2UBTools;
        // ConvFunc::TransND2NZTools<Intf> transND2NZTools;
        // ConvFunc::LoadUB2L1Tools<Intf> loadUB2L1Tools;

        // TBuf<TPosition::VECIN> ndUbBuf;
        // TBuf<TPosition::VECOUT> nzUbBuf;
        // TBuf<TPosition::VECCALC> indexUbBuf;
        // TBuf<TPosition::B1> l1Buf;

        // LocalTensor<typename Config::WeightT> nzTensor;

        // uint16_t ciPerGroup;
        // uint16_t coPerGroup;
        // uint16_t ciOpt;
        // uint16_t ci1Opt;
        // uint16_t ciOptAlign;
        // uint16_t kUbSize;
        // uint16_t coOpt;
        // uint16_t co1Opt;
        // uint16_t coOptAlign;
        // uint16_t singleGroups;       // groups
        // uint16_t singleGroupOpt;     // groupOpt
        // uint16_t groupOptIter;
        // uint16_t vecId;              // V: vec id(0/1) C: groupOptvecid(0/16)
        // uint16_t ddr2l1LoopKB;
        // uint16_t loadUB2L1Iter;
        // uint16_t ddr2l1LoopInner;
        // uint16_t ddr2l1LoopTmp;
        // uint16_t innerIter;
        // uint16_t ddr2l1LoopOuter;
        // uint16_t outerIter;
        // uint16_t bL1LoadTimes;

        // uint32_t ubBufSize;          // WeightNDNZUB
        // uint32_t bL1SpaceSize;       // WeightL1

        // uint8_t pingPongFlag;
    };

    struct ImplDataType {
        __aicore__ inline ImplDataType() {};
    };
};
}  // namespace Conv2dApi

#endif // API_CONV2D_API_IMPL_H