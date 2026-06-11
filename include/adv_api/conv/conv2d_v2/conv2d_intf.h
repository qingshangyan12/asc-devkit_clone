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
 * \file conv2d_intf.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_INTF_H__
#endif

#ifndef ADV_API_CONV_CONV2D_CONV2D_INTF_H
#define ADV_API_CONV_CONV2D_CONV2D_INTF_H

#include "conv2d_config.h"
#include "../../../../impl/adv_api/detail/conv/conv2d/conv2d_util.h"

namespace Conv2dApi {

template<class Config, template<typename, class>class Impl>
struct Conv2dIntf {
    using Ext = Impl<Conv2dIntf, Config>;
    using FmapT = typename Config::SrcAT;
    using WeightT = typename Config::SrcBT;
    using OutputT = typename Config::OutputT;
    using Output1T = typename Config::Output1T;
    using BiasT = typename Config::BiasT;
    using PreQuantT = typename Config::PreQuantT;
    using PreReluT = typename Config::PreReluT;
    using PreClipReluT = typename Config::PreClipReluT;
    using EltwiseT = typename Config::EltwiseT;
    using AntiQuantT = typename Config::AntiQuantT;
    using PostQuantT = typename Config::PostQuantT;
    using PostReluT = typename Config::PostReluT;
    using PostClipReluT = typename Config::PostClipReluT;
    using L0cT = typename Config::L0cT;
    using ContextType = typename Ext::ContextData;
    using ImplDataType = typename Ext::ImplDataType;
    using ConvParam = typename Config::ConvParam;

    constexpr static auto formatFmap = Config::formatA;
    constexpr static auto formatWeight = Config::formatB;
    constexpr static auto formatOutput = Config::formatC;
    constexpr static auto formatOutput1 = Config::formatC1;
    constexpr static auto formatBias = Config::formatBias;
    constexpr static auto formatPreQuant = Config::formatPreQuant;
    constexpr static auto formatPreRelu = Config::formatPreRelu;
    constexpr static auto formatPreClipRelu = Config::formatPreClipRelu;
    constexpr static auto formatEltwise = Config::formatEltwise;
    constexpr static auto formatAntiQuant = Config::formatAntiQuant;
    constexpr static auto formatPostQuant = Config::formatPostQuant;
    constexpr static auto formatPostRelu = Config::formatPostRelu;
    constexpr static auto formatPostClipRelu = Config::formatPostClipRelu;

    constexpr static auto posFmap = Config::posA;
    constexpr static auto posWeight = Config::posB;
    constexpr static auto posOutput = Config::posC;
    constexpr static auto posOutput1 = Config::posC1;
    constexpr static auto posBias = Config::posBias;
    constexpr static auto posPreQuant = Config::posPreQuant;
    constexpr static auto posPreRelu = Config::posPreRelu;
    constexpr static auto posPreClipRelu = Config::posPreClipRelu;
    constexpr static auto posEltwise = Config::posEltwise;
    constexpr static auto posAntiQuant = Config::posAntiQuant;
    constexpr static auto posPostQuant = Config::posPostQuant;
    constexpr static auto posPostRelu = Config::posPostRelu;
    constexpr static auto posPostClipRelu = Config::posPostClipRelu;

    constexpr static uint8_t sizeOfFmap = sizeof(FmapT);
    constexpr static uint8_t sizeOfWeight = sizeof(WeightT);
    constexpr static uint8_t sizeOfBias = sizeof(BiasT);
    constexpr static uint8_t sizeOfPreQuant = sizeof(PreQuantT);
    constexpr static uint8_t sizeOfPreRelu = sizeof(PreReluT);
    constexpr static uint8_t sizeOfPreClipRelu = sizeof(PreClipReluT);
    constexpr static uint8_t sizeOfEltwise = sizeof(EltwiseT);
    constexpr static uint8_t sizeOfAntiQuant = sizeof(AntiQuantT);
    constexpr static uint8_t sizeOfPostQuant = sizeof(PostQuantT);
    constexpr static uint8_t sizeOfPostRelu = sizeof(PostReluT);
    constexpr static uint8_t sizeOfPostClipRelu = sizeof(PostClipReluT);
    constexpr static uint8_t sizeOfL0c = sizeof(L0cT);

    constexpr static int8_t fmapTiling = ConvParam::fmapTiling;
    constexpr static int8_t weightTiling = ConvParam::weightTiling;
    constexpr static int8_t l1PingPong = ConvParam::l1PingPong;
    constexpr static int8_t l0PingPong = ConvParam::l0PingPong;
    constexpr static int8_t outputOrder = ConvParam::outputOrder;
    constexpr static int8_t iterOrder = ConvParam::iterOrder;
    constexpr static int8_t bl1ByPass = ConvParam::bl1ByPass;
    constexpr static int8_t groupType = ConvParam::groupType;

    constexpr static bool aL1DBFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::AL1_OPEN) ||
                                      ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_OPEN);
    constexpr static bool bL1DBFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::BL1_OPEN) ||
                                      ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_OPEN);
    constexpr static bool aL0DBFlag = ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::AL0_OPEN) ||
                                      ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::ALL_OPEN);
    constexpr static bool bL0DBFlag = ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::BL0_OPEN) ||
                                      ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::ALL_OPEN);

    constexpr static bool hasHL1IterFlag = !(ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_CLOSE) ||
                                             ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::FULLLOAD_AL1) ||
                                             ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::ONLY_M_FULLLOAD_AL1_AL0)) ||
                                           !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasHL0IterFlag = !(ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::ALL_CLOSE) ||
                                             ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::ONLY_M_FULLLOAD_AL1_AL0)) ||
                                           !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasWL1IterFlag = !(ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_CLOSE) ||
                                             ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::FULLLOAD_AL1) ||
                                             ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::ONLY_M_FULLLOAD_AL1_AL0)) ||
                                           !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasWL0IterFlag = !(ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::ALL_CLOSE) ||
                                             ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::ONLY_M_FULLLOAD_AL1_AL0)) ||
                                           !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasML1IterFlag = !(ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_CLOSE) ||
                                             ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::FULLLOAD_AL1) ||
                                             ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::ONLY_M_FULLLOAD_AL1_AL0)) ||
                                           !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasML0IterFlag = !(ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::ALL_CLOSE) ||
                                             ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::ONLY_M_FULLLOAD_AL1_AL0)) ||
                                           !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasNL1IterFlag = !(ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_CLOSE) ||
                                             ConvParam::weightTiling == static_cast<int8_t>(ConvWeightTiling::FULLLOAD_BL1) ||
                                             ConvParam::weightTiling == static_cast<int8_t>(ConvWeightTiling::ONLY_N_FULLLOAD_BL1_BL0)) ||
                                           !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasNL0IterFlag = !(ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::ALL_CLOSE) ||
                                             ConvParam::weightTiling == static_cast<int8_t>(ConvWeightTiling::ONLY_N_FULLLOAD_BL1_BL0)) ||
                                           !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));

    constexpr static bool mPreLoadFlag = outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
                                         ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::AL1_OPEN);
    constexpr static bool kPreLoadAFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::AL1_OPEN) &&
                                          ConvParam::weightTiling == static_cast<int8_t>(ConvWeightTiling::FULLLOAD_BL1);
    constexpr static bool kPreLoadBFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::BL1_OPEN) &&
                                          ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::FULLLOAD_AL1);
    constexpr static bool kPreLoadABFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_OPEN) &&
                                           ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV);
    constexpr static bool kPreLoadFlag = kPreLoadAFlag || kPreLoadBFlag || kPreLoadABFlag;

    constexpr static bool kl0FullLoadFlag = ((ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::AL0_OPEN) ||
                                              ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::BL0_OPEN)) &&
                                             (ConvParam::groupType != static_cast<int8_t>(ConvGroupType::ORI_GROUP_CONV)));

    constexpr static bool iterateMFirstFlag = ConvParam::iterOrder == static_cast<int8_t>(ConvIterOrder::MITER_FIRST);
    constexpr static bool iterateNFirstFlag = ConvParam::iterOrder == static_cast<int8_t>(ConvIterOrder::NITER_FIRST);

    constexpr static uint64_t k0 = C0_SIZE / sizeof(WeightT);
    constexpr static uint64_t k0a = C0_SIZE / sizeof(FmapT);
    constexpr static uint64_t k0b = C0_SIZE / sizeof(WeightT);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5102)
    constexpr static bool groupOptPreloadFlag = false;
#else
    constexpr static bool groupOptPreloadFlag =
        ConvParam::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV) &&
        ConvParam::weightTiling == static_cast<int8_t>(ConvWeightTiling::FULLLOAD_BL1) &&
        (ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::FULLLOAD_AL1) ||
         ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::OTHER)) &&
         ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_OPEN);
#endif
    constexpr static bool groupOptFlag = ConvParam::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV);

public:
    ContextType ctx;
    ImplDataType impl;

public:
    __aicore__ inline Conv2dIntf() {}

    __aicore__ inline void Init(const void* __restrict convTiling)
    {

    }

    __aicore__ inline void SetFmap(const GlobalTensor<FmapT>& fmap)
    {

    }

    __aicore__ inline void SetWeight(const GlobalTensor<WeightT>& weight)
    {

    }

    __aicore__ inline void SetBias(const GlobalTensor<BiasT>& bias)
    {
    }

    __aicore__ inline void SetScale(const GlobalTensor<ScaleT>& scale)
    {
    }

    __aicore__ inline void SetFixpipeParams(
        const Extendconv2dFixpipeParams<ScaleT, ReluWeightT, ClipValue0T, ClipValue1T>& fixpipeParams)
    {
    }

    __aicore__ inline void SetOrgBatch(uint64_t orgBatch)
    {
    }

    __aicore__ inline void SetOrgFmapShape(uint64_t orgCi, uint64_t orgHi, uint64_t orgWi)
    {
    }

    __aicore__ inline void SetOrgWeightShape(uint64_t orgCo, uint64_t orgCi, uint64_t orgKh, uint64_t orgKw)
    {
    }

    __aicore__ inline void SetOrgOutputShape(uint64_t orgCo, uint64_t orgHo, uint64_t orgWo)
    {
    }

    __aicore__ inline void SetSingleFmapShape(uint64_t singleCi, uint64_t singleHi, uint64_t singleWi)
    {
    }

    __aicore__ inline void SetSingleOutputShape(uint64_t singleCo, uint64_t singleHo, uint64_t singleWo,
                                                uint64_t singleCoreBatch)
    {
    }

    __aicore__ inline void SetSingleOutputShape(uint64_t singleCo, uint64_t singleM, uint64_t singleCoreBatch)
    {
    }

    __aicore__ inline void SetOptGroupParams(uint64_t singleGroups, uint64_t singleGroupOpt, uint64_t singleCoOpt)
    {
    }

    __aicore__ inline void SetFmapStartPosition(int64_t hiStartPos, int64_t wiStartPos)
    {
    }

    __aicore__ inline void SetFmapStartPosition(int64_t mStartPos)
    {
    }

    __aicore__ inline void SetWeightStartPosition(int64_t coStartPos, int64_t ciStartPos = 0)
    {
    }

    __aicore__ inline void SetIterIndex(uint64_t groupOptIter)
    {
    }

    __aicore__ inline void ConvPreProcess()
    {
    }

    __aicore__ inline void ConvPostProcess()
    {
    }

    template <bool sync = true>
    __aicore__ inline void IterateAll(const GlobalTensor<OutputT>& output, bool enPartialSum = false)
    {
    }

    template <bool sync = true>
    __aicore__ inline void IterateAll(const GlobalTensor<OutputT>& output0,
                                      const GlobalTensor<Output1T>& output1, bool enPartialSum = false)
    {
    }

    __aicore__ inline void End()
    {
    }

    template <bool sync = true>
     __aicore__ inline bool Iterate(bool enPartialSum = false, bool *updateL1Flag = nullptr)
    {
    }

    template <bool sync = true>
    __aicore__ inline void PreFusionProcess(uint64_t& aL1Offset, AscendC::TEventID& eventIdMte3ToMte1,
                                            AscendC::TEventID& eventIdMte1ToMte3)

    {
    }

    template <template <typename> class TensorTypeT, const FixpipeConfig &config, bool sync = true>
    __aicore__ inline void GetTensorC(const TensorTypeT<OutputT>& output,
                                      CopyUbInfo* ubInfo = nullptr, bool enSequentialWrite = false)
    {
    }

    template <template <typename> class TensorTypeT, const FixpipeConfig &config, bool sync = true>
    __aicore__ inline void GetTensorC(const TensorTypeT<OutputT>& output0, const GlobalTensor<Output1T>& output1,
                                      CopyUbInfo* ubInfo = nullptr, bool enSequentialWrite = false)
    {
    }
};

} // namespace Conv2dApi

#endif // ADV_API_CONV_CONV2D_CONV2D_INTF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_INTF_H__
#endif