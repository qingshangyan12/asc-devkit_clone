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

#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "conv2d_config.h"
#include "../../../impl/conv/conv2d/conv2d_util.h"
#include "../../../impl/conv/conv2d/conv2d_api_impl.h"
#include "../../../impl/conv/conv2d/impl/conv2d_instr.h"
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
#include "../../../impl/conv/conv2d/impl/dav_5101/conv2d_instr_impl.h"
#include "../../../impl/conv/conv2d/impl/dav_5101/conv2d_instr_m_mode_impl.h"
#include "../../../impl/conv/conv2d/impl/dav_5101/conv2d_instr_hw_mode_impl.h"
#endif
#else
#include "conv2d_config.h"
#include "impl/adv_api/detail/conv/conv2d/conv2d_util.h"
#include "impl/adv_api/detail/conv/conv2d/conv2d_api_impl.h"
#include "impl/adv_api/detail/conv/conv2d/impl/conv2d_instr.h"
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
#include "impl/adv_api/detail/conv/conv2d/impl/dav_5101/conv2d_instr_impl.h"
#include "impl/adv_api/detail/conv/conv2d/impl/dav_5101/conv2d_instr_m_mode_impl.h"
#include "impl/adv_api/detail/conv/conv2d/impl/dav_5101/conv2d_instr_hw_mode_impl.h"
#endif
#endif

namespace Conv2dApi {
// using namespace Conv2dApiFunc;

struct Iters {
    uint64_t kAL1Iter;
    uint64_t kBL1Iter;
    uint64_t mAL1Iter;
    uint64_t hoAL1Iter;
    uint64_t woAL1Iter;
    uint64_t nBL1Iter;
    uint64_t batchIter;
    bool endTag;
};

template<class Config, template<typename, class>class Impl>
struct Conv2dIntf {
    using Ext = Impl<Conv2dIntf, Config>;
    using FmapT = typename Config::FmapT;
    using WeightT = typename Config::WeightT;
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
    using PaddingT = typename Config::PaddingT;
    using L0cT = typename Config::L0cT;
    using ContextType = typename Ext::ContextData;
    using ConvParam = typename Config::ConvParam;
    using ImplDataType = typename Ext::ImplDataType;

    constexpr static auto formatFmap = Config::formatFmap;
    constexpr static auto formatWeight = Config::formatWeight;
    constexpr static auto formatOutput = Config::formatOutput;
    constexpr static auto formatOutput1 = Config::formatOutput1;
    constexpr static auto formatBias = Config::formatBias;
    constexpr static auto formatPreQuant = Config::formatPreQuant;
    constexpr static auto formatPreRelu = Config::formatPreRelu;
    constexpr static auto formatPreClipRelu = Config::formatPreClipRelu;
    constexpr static auto formatEltwise = Config::formatEltwise;
    constexpr static auto formatAntiQuant = Config::formatAntiQuant;
    constexpr static auto formatPostQuant = Config::formatPostQuant;
    constexpr static auto formatPostRelu = Config::formatPostRelu;
    constexpr static auto formatPostClipRelu = Config::formatPostClipRelu;
    constexpr static auto formatPadding = Config::formatPadding;

    constexpr static auto posFmap = Config::posFmap;
    constexpr static auto posWeight = Config::posWeight;
    constexpr static auto posOutput = Config::posOutput;
    constexpr static auto posOutput1 = Config::posOutput1;
    constexpr static auto posBias = Config::posBias;
    constexpr static auto posPreQuant = Config::posPreQuant;
    constexpr static auto posPreRelu = Config::posPreRelu;
    constexpr static auto posPreClipRelu = Config::posPreClipRelu;
    constexpr static auto posEltwise = Config::posEltwise;
    constexpr static auto posAntiQuant = Config::posAntiQuant;
    constexpr static auto posPostQuant = Config::posPostQuant;
    constexpr static auto posPostRelu = Config::posPostRelu;
    constexpr static auto posPostClipRelu = Config::posPostClipRelu;
    constexpr static auto posPadding = Config::posPadding;

    constexpr static uint8_t sizeOfFmap = sizeof(FmapT);
    constexpr static uint8_t sizeOfWeight = sizeof(WeightT);
    constexpr static uint8_t sizeOfOutput = sizeof(OutputT);
    constexpr static uint8_t sizeOfOutput1 = sizeof(Output1T);
    constexpr static uint8_t sizeOfBias = sizeof(BiasT);
    constexpr static uint8_t sizeOfPreQuant = sizeof(PreQuantT);
    constexpr static uint8_t sizeOfPreRelu = sizeof(PreReluT);
    constexpr static uint8_t sizeOfPreClipRelu = sizeof(PreClipReluT);
    constexpr static uint8_t sizeOfEltwise = sizeof(EltwiseT);
    constexpr static uint8_t sizeOfAntiQuant = sizeof(AntiQuantT);
    constexpr static uint8_t sizeOfPostQuant = sizeof(PostQuantT);
    constexpr static uint8_t sizeOfPostRelu = sizeof(PostReluT);
    constexpr static uint8_t sizeOfPostClipRelu = sizeof(PostClipReluT);
    constexpr static uint8_t sizeOfPadding = sizeof(PaddingT);
    constexpr static uint8_t sizeOfL0c = sizeof(L0cT);

    constexpr static int8_t fmapTiling = ConvParam::fmapTiling;
    constexpr static int8_t weightTiling = ConvParam::weightTiling;
    constexpr static int8_t l1PingPong = ConvParam::l1PingPong;
    constexpr static int8_t l0PingPong = ConvParam::l0PingPong;
    constexpr static int8_t outputOrder = ConvParam::outputOrder;
    constexpr static int8_t iterOrder = ConvParam::iterOrder;
    constexpr static int8_t bl1ByPass = ConvParam::bl1ByPass;
    constexpr static int8_t groupType = ConvParam::groupType;

    constexpr static bool fmapL1DBFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::L1A_OPEN) ||
                                         ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_OPEN);
    constexpr static bool weightL1DBFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::L1B_OPEN) ||
                                           ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_OPEN);
    constexpr static bool fmapL0DBFlag = ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::L0A_OPEN) ||
                                         ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::ALL_OPEN);
    constexpr static bool weightL0DBFlag = ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::L0B_OPEN) ||
                                           ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::ALL_OPEN);
    #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
        constexpr static bool outputl0DBFlag = (fmapL1DBFlag || weightL0DBFlag);
    #else
        constexpr static bool outputl0DBFlag = (fmapL0DBFlag || weightL0DBFlag);
    #endif

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

    constexpr static bool mPreLoadAFlag = outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
                                          ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::L1A_OPEN) && false;
    constexpr static bool kPreLoadAFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::L1A_OPEN) &&
                                          ConvParam::weightTiling == static_cast<int8_t>(ConvWeightTiling::FULLLOAD_BL1);
    constexpr static bool kPreLoadBFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::L1B_OPEN) &&
                                          ConvParam::fmapTiling == static_cast<int8_t>(ConvFmapTiling::FULLLOAD_AL1);
    constexpr static bool kPreLoadABFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_OPEN) &&
                                           ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV);
    constexpr static bool kPreLoadFlag = kPreLoadAFlag || kPreLoadBFlag || kPreLoadABFlag;

    constexpr static bool kl0FullLoadFlag = ((ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::L0A_OPEN) ||
                                              ConvParam::l0PingPong == static_cast<int8_t>(ConvL0PingPong::L0B_OPEN)) &&
                                             (ConvParam::groupType != static_cast<int8_t>(ConvGroupType::ORI_GROUP_CONV)));

    constexpr static bool iterateMFirstFlag = ConvParam::iterOrder == static_cast<int8_t>(ConvIterOrder::MITER_FIRST);
    constexpr static bool iterateNFirstFlag = ConvParam::iterOrder == static_cast<int8_t>(ConvIterOrder::NITER_FIRST);

    constexpr static uint64_t k0 = C0_SIZE / sizeof(WeightT);
    constexpr static uint64_t k0Fmap = C0_SIZE / sizeof(FmapT);
    constexpr static uint64_t k0Weight = C0_SIZE / sizeof(WeightT);
    constexpr static uint64_t k0Elt = C0_SIZE / sizeof(EltwiseT);
    constexpr static uint64_t k0Output = C0_SIZE / sizeof(OutputT);
    constexpr static uint64_t k0Output1 = C0_SIZE / sizeof(Output1T);

public:
    ContextType ctx;
    ImplDataType impl;

    using LoadFmapL1Tools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        LoadFmapL1ToolsMMode<Conv2dIntf>, LoadFmapL1ToolsHWMode<Conv2dIntf>>::type;
    using LoadFmapL0Tools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        LoadFmapL0ToolsMMode<Conv2dIntf>, LoadFmapL0ToolsHWMode<Conv2dIntf>>::type;
    using CopyEltwiseTools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        CopyEltwiseToolsMMode<Conv2dIntf>, CopyEltwiseToolsHWMode<Conv2dIntf>>::type;
    using CopyOutputTools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        CopyOutputToolsMMode<Conv2dIntf>, CopyOutputToolsHWMode<Conv2dIntf>>::type;
    LoadFmapL1Tools loadFmapl1Ins;
    LoadBL1Tools<Conv2dIntf> loadWeightL1Ins;
    LoadChannelWiseL1Tools<Conv2dIntf, typename Config::BiasT> loadBiasL1Ins;
    CopyEltwiseTools loadBiasNonBrcL1Ins;
    LoadChannelWiseL1Tools<Conv2dIntf, typename Config::PreQuantT> loadPreQuantL1Ins;
    LoadChannelWiseL1Tools<Conv2dIntf, typename Config::PreReluT> loadPreReluL1Ins;
    LoadChannelWiseL1Tools<Conv2dIntf, typename Config::PreClipReluT> loadPreClipReluL1Ins;
    LoadChannelWiseL1Tools<Conv2dIntf, typename Config::AntiQuantT> loadAntiQuantL1Ins;
    LoadChannelWiseL1Tools<Conv2dIntf, typename Config::EltwiseT> loadEltwiseL1Ins;
    CopyEltwiseTools loadEltwiseNonBrcL1Ins;
    LoadChannelWiseL1Tools<Conv2dIntf, typename Config::PostQuantT> loadPostQuantL1Ins;
    LoadChannelWiseL1Tools<Conv2dIntf, typename Config::PostReluT> loadPostReluL1Ins;
    LoadChannelWiseL1Tools<Conv2dIntf, typename Config::PostClipReluT> loadPostClipReluL1Ins;
    LoadChannelWiseL1Tools<Conv2dIntf, typename Config::PaddingT> loadPaddingL1Ins;
    LoadFmapL0Tools loadFmapL0Ins;
    LoadBL0Tools<Conv2dIntf> loadWeightL0Ins;
    LoadBiasBtTools<Conv2dIntf> loadBiasBTIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PreQuantT> loadPreQuantFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PreReluT> loadPreReluFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PreClipReluT> loadPreClipReluFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::AntiQuantT> loadAntiQuantFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PostQuantT> loadPostQuantFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PostReluT> loadPostReluFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PostClipReluT> loadPostClipReluFBIns;
    LoadPaddingPtTools<Conv2dIntf> loadPaddingPTIns;
    MMadTools<Conv2dIntf> madIns;
    CopyOutputTools copyOutputIns;

public:
    __aicore__ inline Conv2dIntf();

    __aicore__ inline void Init(const TConv2DApiTiling *__restrict convTiling, TPipe *tPipe);

    __aicore__ inline void SetOrgFmapShape(uint64_t orgCi, uint64_t orgHi, uint64_t orgWi);

    __aicore__ inline void SetOrgWeightShape(uint64_t orgCo, uint64_t orgCi, uint64_t orgKh, uint64_t orgKw);

    __aicore__ inline void SetOrgOutputShape(uint64_t orgCo, uint64_t orgHo, uint64_t orgWo);

    __aicore__ inline void SetSingleFmapShape(uint64_t singleCi, uint64_t singleHi, uint64_t singleWi);

    __aicore__ inline void SetSingleOutputShape(uint64_t singleCo, uint64_t singleHo, uint64_t singleWo);

    __aicore__ inline void SetSingleOutputShape(uint64_t singleCo, uint64_t singleM);

    __aicore__ inline void SetSingleBatchNum(uint64_t singleCoreBatch, uint64_t fmapOneBatchSize, uint64_t outputOneBatchSize);

    __aicore__ inline void SetFmapStartPosition(int64_t hiStartPos, int64_t wiStartPos);

    __aicore__ inline void SetFmapStartPosition(int64_t mStartPos);

    __aicore__ inline void SetWeightStartPosition(int64_t coStartPos, int64_t ciStartPos = 0);

    __aicore__ inline void SetFmap(const GlobalTensor<FmapT> &fmap);

    __aicore__ inline void SetWeight(const GlobalTensor<WeightT> &weight);

    __aicore__ inline void SetBias(const GlobalTensor<BiasT> &bias);

    __aicore__ inline void SetPreQuant(const GlobalTensor<PreQuantT> &preQuant);

    __aicore__ inline void SetPreRelu(const GlobalTensor<PreReluT> &preRelu);

    __aicore__ inline void SetPreClipRelu(const GlobalTensor<PreClipReluT> &preClipRelu);

    __aicore__ inline void SetAntiQuant(const GlobalTensor<AntiQuantT> &antiQuant);

    __aicore__ inline void SetEltwise(const GlobalTensor<EltwiseT> &eltwise);

    __aicore__ inline void SetPostQuant(const GlobalTensor<PostQuantT> &postQuant);

    __aicore__ inline void SetPostRelu(const GlobalTensor<PostReluT> &postRelu);

    __aicore__ inline void SetPostClipRelu(const GlobalTensor<PostClipReluT> &postClipRelu);

    __aicore__ inline void SetPadding(const GlobalTensor<PaddingT> &padding);

    template <bool sync = true>
    __aicore__ inline bool Iterate(bool enPartialSum = false);

    template <bool sync = true>
    __aicore__ inline void GetTensorC(const GlobalTensor<OutputT> &output, bool enSequentialWrite = false);

    template <bool sync = true>
    __aicore__ inline bool IterateAll(const GlobalTensor<OutputT> &output, bool enPartialSum = false);

    __aicore__ inline void End();

private:
    __aicore__ inline void InitAIC();

    __aicore__ inline void InitAIV();

    __aicore__ inline void InitValueMMode();

    __aicore__ inline void InitValueHWMode();

    __aicore__ inline void InitMDirectionValue();

    __aicore__ inline void InitHoDirectionValue();

    __aicore__ inline void InitWoDirectionValue();

    __aicore__ inline void InitKDirectionValue();

    __aicore__ inline void InitCoDirectionValue();

    __aicore__ inline void InitBuffer();

    __aicore__ inline void InitBufferFmap();

    __aicore__ inline void InitBufferWeight();

    __aicore__ inline void InitBufferOutput();

    __aicore__ inline void InitBufferBias();

    __aicore__ inline void InitBufferFixp();

    __aicore__ inline void InitSubApiParams();

    __aicore__ inline void LoadBiasL1BeforeIterate();

    __aicore__ inline void LoadFixpL1BeforeIterate();

    __aicore__ inline void FreeBiasL1AfterIterate();

    __aicore__ inline void FreeFixpL1AfterIterate();

    __aicore__ inline void CalcMDirectionVar();

    __aicore__ inline void CalcHoDirectionVar();

    __aicore__ inline void CalcWoDirectionVar();

    __aicore__ inline void CalcCoDirectionVar();

    __aicore__ inline uint64_t CalcCurrentNL0();

    __aicore__ inline uint64_t CalcCurrentML0MMode();

    __aicore__ inline uint64_t CalcCurrentML0HWMode();

    __aicore__ inline void LoadAL1BaseMoudle();

    __aicore__ inline void LoadAL1BaseMoudle(Iters& iters);

    __aicore__ inline void LoadBL1BaseMoudle();

    __aicore__ inline void LoadBL1BaseMoudle(Iters& iters);

    __aicore__ inline void LoadAL1Moudle();

    __aicore__ inline void LoadBL1Moudle();

    __aicore__ inline void ReduceKCloseL0PingPong(bool isFirst);

    __aicore__ inline void ReduceKOpenL0APingPong(uint16_t& al0PingPongFlag, bool isFirst);

    __aicore__ inline void ReduceKOpenL0BPingPong(uint16_t& bl0PingPongFlag, bool isFirst);

    __aicore__ inline void ReduceKOpenL0AL0BPingPong(uint16_t& al0PingPongFlag, bool isFirst);

    __aicore__ inline void LoadL0Moudle(uint16_t &al0PingPongFlag, uint16_t &bl0PingPongFlag, bool isFirst);

    __aicore__ inline void FirstIterateImplMMode();

    __aicore__ inline bool IterateL0MFirstMMode();

    __aicore__ inline bool IterateMFirstMMode();

    __aicore__ inline bool IterateL0NFirstMMode();

    __aicore__ inline bool IterateNFirstMMode();

    __aicore__ inline bool UpdateCommonItersMModeMFirst(Iters& iters);

    __aicore__ inline bool UpdateCommonItersMModeNFirst(Iters& iters);

    __aicore__ inline void FirstIterateImplHWMode();

    __aicore__ inline bool IterateHWL1();

    __aicore__ inline bool IterateL0MFirstHWMode();

    __aicore__ inline bool IterateMFirstHWMode();

    __aicore__ inline bool IterateL0NFirstHWMode();

    __aicore__ inline bool IterateNFirstHWMode();

    __aicore__ inline bool UpdateCommonItersHWModeMFirst(Iters& iters);

    __aicore__ inline bool UpdateCommonItersHWModeNFirst(Iters& iters);

    __aicore__ inline bool UpdateCommonIters(Iters& iters);

    __aicore__ inline bool UpdateItersByFmap(Iters& iters, bool updateIterByFmapTag);

    __aicore__ inline bool UpdateItersByWeight(Iters& iters, bool updateIterByFmapTag);

    __aicore__ inline void UpdateHoL0WoL0();

    __aicore__ inline void SetMNBeforeIterateK();

    __aicore__ inline void LoadBiasL1BeforeIterateK();

    __aicore__ inline void LoadBiasL0BeforeIterateK();

    __aicore__ inline void LoadBiasBeforeIterateK();

    __aicore__ inline void LoadFixpL1BeforeIterateK();

    __aicore__ inline void LoadFixpL0BeforeIterateK();

    __aicore__ inline void LoadFixpBeforeIterateK();

    __aicore__ inline void ReduceOneK();

    __aicore__ inline void ReduceKPreload();

    __aicore__ inline void ReduceK();

    __aicore__ inline void FreeBiasL1AfterIterateK();

    __aicore__ inline void FreeFixpL1AfterIterateK();

    __aicore__ inline void FreeL1TensorAfterIterateK();

    __aicore__ inline void FreeBiasL0TensorAfterIterateK();

    __aicore__ inline void FreeFixpL0TensorAfterIterateK();

    __aicore__ inline void FreeL0TensorAfterIterateK();

    __aicore__ inline void IterateK();

    __aicore__ inline bool IterateImplMMode(bool enPartialSum);

    __aicore__ inline bool IterateImplHWMode(bool enPartialSum);
};

} // namespace Conv2dApi

#if defined(__NPU_ARCH__)
#include "impl/adv_api/detail/conv/conv2d/conv2d_intf_impl.h"
#endif

#endif // ADV_API_CONV_CONV2D_CONV2D_INTF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_INTF_H__
#endif
