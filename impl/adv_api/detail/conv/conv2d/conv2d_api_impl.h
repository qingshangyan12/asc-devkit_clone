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
 * \file conv2d_api_impl.h
 * \brief conv2d api impl class
 */

#ifndef CONV2D_API_IMPL_H
#define CONV2D_API_IMPL_H

#include "../conv_common/conv_framework_util.h"
#include "../../../detail/conv/conv2d/conv2d_common_func.h"
#include "lib/conv/conv2d_config.h"
#include "conv2d_util.h"

namespace conv2d {
using namespace AscendC;
using namespace conv;

template<typename Intf, class Config>
struct Conv2dApiImpl {
public:
    using ConvParam = typename Config::ConvParam;
    constexpr static uint32_t ImplType = Config::implType;

public:
    __aicore__ inline Conv2dApiImpl() {}

    CONV_REG_IMPL(Config, Conv2dFunc, Init);
    CONV_REG_IMPL(Config, ConvFunc, SetFmap);
    CONV_REG_IMPL(Config, ConvFunc, SetWeight);
    CONV_REG_IMPL(Config, ConvFunc, SetBias);
    CONV_REG_IMPL(Config, ConvFunc, SetQuantPre);
    CONV_REG_IMPL(Config, ConvFunc, SetReluPre);
    CONV_REG_IMPL(Config, ConvFunc, SetEltAntiQuant);
    CONV_REG_IMPL(Config, ConvFunc, SetQuantPost);
    CONV_REG_IMPL(Config, ConvFunc, SetQuantFixpipe);
    CONV_REG_IMPL(Config, ConvFunc, SetEltwise);
    CONV_REG_IMPL(Config, Conv2dFunc, SetOrgFmapShape);
    CONV_REG_IMPL(Config, Conv2dFunc, SetOrgWeightShape);
    CONV_REG_IMPL(Config, Conv2dFunc, SetOrgOutputShape);
    CONV_REG_IMPL(Config, Conv2dFunc, SetSingleFmapShape);
    CONV_REG_IMPL(Config, Conv2dFunc, SetSingleOutputShape);
    CONV_REG_IMPL(Config, ConvFunc, SetSingleBatchNum);
    CONV_REG_IMPL(Config, Conv2dFunc, SetOptGroupParams);
    CONV_REG_IMPL(Config, Conv2dFunc, SetFmapStartPosition);
    CONV_REG_IMPL(Config, ConvFunc, SetWeightStartPosition);
    CONV_REG_IMPL(Config, ConvFunc, SetIterIndex);
    CONV_REG_IMPL(Config, ConvFunc, Iterate);
    CONV_REG_IMPL(Config, ConvFunc, IterateAll);
    CONV_REG_IMPL(Config, ConvFunc, GetTensorC);
    CONV_REG_IMPL(Config, ConvFunc, End);

    struct ContextData: public Config::ContextData {
        __aicore__ inline ContextData()
        {
        }
        // __aicore__ inline ContextData(const Intf* __restrict self)
        // {
        //     loadAl1Ins = LoadAL1Tools(self);
        //     loadBL1Ins = Conv2dFunc::LoadBL1Tools<Intf>(self);
        //     loadBiasL1Ins = ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::BiasT>(self);
        //     loadQuantPreL1Ins = ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::QuantPreT>(self);
        //     loadReluPreL1Ins = ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::ReluPreT> loadReluPreL1Ins(self);
        //     loadEltAntiQuantL1Ins = ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::EltAntiQuantT>(self);
        //     loadQuantPostL1Ins = ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::QuantPostT>(self);
        //     loadEltwiseL1Ins = ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::EltwiseT>(self);
        //     loadEltwiseNonBrcL1Ins = CopyEltTools(self);
        //     loadAL0Ins = LoadAL0Tools(self);
        //     loadBL0Ins = ConvFunc::LoadBL0Tools<Intf>(self);
        //     loadBiasBTIns = ConvFunc::LoadBiasBtTools<Intf>(self);
        //     loadQuantPreFBIns = ConvFunc::LoadFixpParamFbTools<Intf, typename Config::QuantPreT>(self);
        //     loadReluPreFBIns = ConvFunc::LoadFixpParamFbTools<Intf, typename Config::ReluPreT>(self);
        //     loadEltAntiQuantFBIns = ConvFunc::LoadFixpParamFbTools<Intf, typename Config::EltAntiQuantT>(self);
        //     loadQuantPostFBIns = ConvFunc::LoadFixpParamFbTools<Intf, typename Config::QuantPostT>(self);
        //     madIns = ConvFunc::MMadTools<Intf>(self);
        //     copyOutIns = CopyOutTools(self);
        // };

        TPipe *pipe;
        const TConv2DTiling* __restrict convTiling;

        // // Using Conditional<flag, type1, type2>::type to select M or HW, if flag=true, type=type1, else type=type2
        // using LoadAL1Tools = typename Conditional<
        //     ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        //     Conv2dFunc::LoadAL1ToolsMMode<Intf>, Conv2dFunc::LoadAL1ToolsHWMode<Intf>>::type;
        // using LoadAL0Tools = typename Conditional<
        //     ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        //     ConvFunc::LoadAL0ToolsMMode<Intf>, ConvFunc::LoadAL0ToolsHWMode<Intf>>::type;
        // using CopyEltTools = typename Conditional<
        //     ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        //     ConvFunc::CopyEltToolsMMode<Intf>, ConvFunc::CopyEltToolsHWMode<Intf>>::type;
        // using CopyOutTools = typename Conditional<
        //     ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        //     ConvFunc::CopyOutToolsMMode<Intf>, ConvFunc::CopyOutToolsHWMode<Intf>>::type;

        // LoadAL1Tools loadAl1Ins;
        // Conv2dFunc::LoadBL1Tools<Intf> loadBL1Ins;
        // ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::BiasT> loadBiasL1Ins;
        // ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::QuantPreT> loadQuantPreL1Ins;
        // ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::ReluPreT> loadReluPreL1Ins;
        // ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::EltAntiQuantT> loadEltAntiQuantL1Ins;
        // ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::QuantPostT> loadQuantPostL1Ins;
        // ConvFunc::LoadChannelWiseL1Tools<Intf, typename Config::EltwiseT> loadEltwiseL1Ins;
        // CopyEltTools loadEltwiseNonBrcL1Ins;
        // LoadAL0Tools loadAL0Ins;
        // ConvFunc::LoadBL0Tools<Intf> loadBL0Ins;
        // ConvFunc::LoadBiasBtTools<Intf> loadBiasBTIns;
        // ConvFunc::LoadFixpParamFbTools<Intf, typename Config::QuantPreT> loadQuantPreFBIns;
        // ConvFunc::LoadFixpParamFbTools<Intf, typename Config::ReluPreT> loadReluPreFBIns;
        // ConvFunc::LoadFixpParamFbTools<Intf, typename Config::EltAntiQuantT> loadEltAntiQuantFBIns;
        // ConvFunc::LoadFixpParamFbTools<Intf, typename Config::QuantPostT> loadQuantPostFBIns;
        // ConvFunc::MMadTools<Intf> madIns;
        // CopyOutTools copyOutIns;

        // GM Tensor
        GlobalTensor<typename Config::FmapT> agm;
        GlobalTensor<typename Config::WeightT> bgm;
        GlobalTensor<typename Config::BiasT> biasgm;
        GlobalTensor<typename Config::QuantPreT> quantPreGm;
        GlobalTensor<typename Config::ReluPreT> reluPreGm;
        GlobalTensor<typename Config::EltAntiQuantT> eltAntiQuantGm;
        GlobalTensor<typename Config::QuantPostT> quantPostGm;
        GlobalTensor<typename Config::EltwiseT> eltwiseGm;

        // normal: <A1, 1>, <B1, 1>; preload: <A1, 2>, <B1, 2>
        // TQue<QuePosition::A1, static_cast<int8_t>(Intf::kPreLoadAFlag || Intf::kPreLoadABFlag || Intf::mPreLoadAFlag) + 1> queueAL1; // AL1
        // TQue<QuePosition::B1, static_cast<int8_t>(Intf::kPreLoadBFlag || Intf::kPreLoadABFlag) + 1> queueBL1; // BL1
        TQue<QuePosition::A1, 1> queueAL1; // AL1
        TQue<QuePosition::B1, 1> queueBL1; // BL1
        // [zj]ToDo: broadcast
        TQue<QuePosition::B1, 1> queueBiasL1; // BiasL1
        TQue<QuePosition::B1, 1> queueQuantPreL1; // L1
        TQue<QuePosition::B1, 1> queueReluPreL1; // L1
        TQue<QuePosition::B1, 1> queueEltAntiQuantL1; // L1
        TQue<QuePosition::B1, 1> queueQuantPostL1; // L1
        TQue<QuePosition::A1, 1> queueEltwiseL1; // L1
        TQue<TPosition::C2, 1> queueBiasBT; // BT
        TQue<TPosition::C2PIPE2GM, 1> queueQuantPreFB; // FB
        TQue<TPosition::C2PIPE2GM, 1> queueReluPreFB; // FB
        TQue<TPosition::C2PIPE2GM, 1> queueEltAntiQuantFB; // FB
        TQue<TPosition::C2PIPE2GM, 1> queueQuantPostFB; // FB
        TQue<QuePosition::CO1, 1> queueCL0; // CL0
        TQue<QuePosition::CO1, 1> queueCL1; // CL1
        TBuf<TPosition::A2> l0aBuf;
        TBuf<TPosition::B2> l0bBuf;

        LocalTensor<typename Config::FmapT> al1;
        LocalTensor<typename Config::WeightT> bl1;
        LocalTensor<typename Config::BiasT> biasL1;
        LocalTensor<typename Config::QuantPreT> quantPreL1;
        LocalTensor<typename Config::ReluPreT> reluPreL1;
        LocalTensor<typename Config::EltAntiQuantT> eltAntiQuantL1;
        LocalTensor<typename Config::QuantPostT> quantPostL1;
        LocalTensor<typename Config::EltwiseT> eltwiseL1;
        LocalTensor<typename Config::FmapT> wholeAl0Tensor;
        LocalTensor<typename Config::FmapT> al0;
        LocalTensor<typename Config::WeightT> wholeBl0Tensor;
        LocalTensor<typename Config::WeightT> bl0;
        LocalTensor<typename Config::L0cT> biasBT;
        LocalTensor<typename Config::QuantPreT> quantPreFB;
        LocalTensor<typename Config::ReluPreT> reluPreFB;
        LocalTensor<typename Config::EltAntiQuantT> eltAntiQuantFB;
        LocalTensor<typename Config::QuantPostT> quantPostFB;
        LocalTensor<typename Config::L0cT> cl0;
        LocalTensor<typename Config::OutputT> cl1;

        int64_t quantPreMode; // pre_quant_t
        int64_t reluPreMode; // pre_relu_t
        int64_t clipReluPreMode; // bool
        int64_t quantPostMode; // post_quant_t
        int64_t reluPostMode; // bool
        int64_t eltwiseOp; // eltwise_op_t
        int64_t eltwiseAntiqEn; // bool
        int64_t eltwiseBrcEn; // bool
        int64_t quantPreSpr;
        int64_t reluPreSpr;
        int64_t antiQuantSpr;
        int64_t quantPostSpr;

        uint8_t enableBias;  // non broadcast bias
        uint8_t enableQuantPre;  // vector QuantPre
        uint8_t enableReluPre;  // vector ReluPre
        uint8_t enableEltAntiQuant;  // vector EltAntiQuant
        uint8_t enableQuantPost;  // vector QuantPost
        uint8_t enableEltwise;  // non broadcast Eltwise

        int64_t ciStartPos;
        int64_t coStartPos;
        uint64_t singleCoreCi; // Cin
        uint64_t singleCoreCo; // Co

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
        uint64_t kAL1Iter;    // AL1k
        uint64_t kBL1Iter;    // BL1k
        uint64_t nBL1Iter;   // BL1n
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
        uint64_t singleCoreHo; // Ho
        uint64_t singleCoreWo; // Wo
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

        // Used in opt group mode(groups > 1)
        ConvFunc::LoadGM2UBTools<Intf> loadGm2UBTools;
        ConvFunc::TransND2NZTools<Intf> transND2NZTools;
        ConvFunc::LoadUB2L1Tools<Intf> loadUB2L1Tools;

        TBuf<TPosition::VECIN> ndUbBuf;
        TBuf<TPosition::VECOUT> nzUbBuf;
        TBuf<TPosition::VECCALC> indexUbBuf;
        TBuf<TPosition::B1> l1Buf;

        LocalTensor<typename Config::WeightT> nzTensor;

        uint16_t ciPerGroup;
        uint16_t coPerGroup;
        uint16_t ciOpt;
        uint16_t ci1Opt;
        uint16_t ciOptAlign;
        uint16_t kUbSize;
        uint16_t coOpt;
        uint16_t co1Opt;
        uint16_t coOptAlign;
        uint16_t singleGroups;       // groups
        uint16_t singleGroupOpt;     // groupOpt
        uint16_t groupOptIter;
        uint16_t vecId;              // V: vec id(0/1) C: groupOptvecid(0/16)
        uint16_t ddr2l1LoopKB;
        uint16_t loadUB2L1Iter;
        uint16_t ddr2l1LoopInner;
        uint16_t ddr2l1LoopTmp;
        uint16_t innerIter;
        uint16_t ddr2l1LoopOuter;
        uint16_t outerIter;
        uint16_t bL1LoadTimes;

        uint32_t ubBufSize;          // WeightNDNZUB
        uint32_t bL1SpaceSize;       // WeightL1

        uint8_t pingPongFlag;

        // Input shape info
        uint64_t orgCi;     // fmapcin
        uint64_t orgCo;     // weightcout
        uint64_t orgHi;     // fmaph
        uint64_t orgWi;     // fmapw
        uint64_t orgHo;     // outputh
        uint64_t orgWo;     // outputw
        uint64_t kernelH;   // weighth
        uint64_t kernelW;   // weightw
    };

    struct ImplDataType {
        __aicore__ inline ImplDataType() {};
    };
};
}  // namespace conv2d

#endif // CONV2D_API_IMPL_H