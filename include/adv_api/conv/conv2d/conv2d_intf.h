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

// [zj]TODO: olny interface declaration

#include "conv2d_config.h"
#include "../../impl/conv/conv2d/conv2d_util.h" // #include "../../../../impl/adv_api/detail/conv/conv2d/conv2d_util.h" // [zj]TODO

namespace Conv2dApi {
using namespace Conv2dApiFunc;

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

    // [zj]TODO: del no need

    // [zj]TODO: del config duplicates

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
    #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
        constexpr static bool cl0DBFlag = (aL1DBFlag || bL0DBFlag);
    #else
        constexpr static bool cl0DBFlag = (aL0DBFlag || bL0DBFlag);
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
    constexpr static uint64_t k0e = C0_SIZE / sizeof(EltwiseT);
    constexpr static uint64_t k0c = C0_SIZE / sizeof(OutputT);
    constexpr static uint64_t k0c1 = C0_SIZE / sizeof(Output1T);


public:
    ContextType ctx;
    ImplDataType impl;

    // [zj]TODO: move tools to impl
    using LoadAL1Tools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        LoadAL1ToolsMMode<Conv2dIntf>, LoadAL1ToolsHWMode<Conv2dIntf>>::type;
    using LoadAL0Tools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        LoadAL0ToolsMMode<Conv2dIntf>, LoadAL0ToolsHWMode<Conv2dIntf>>::type;
    using CopyEltwiseTools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        CopyEltwiseToolsMMode<Conv2dIntf>, CopyEltwiseToolsHWMode<Conv2dIntf>>::type;
    using CopyOutTools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        CopyOutToolsMMode<Conv2dIntf>, CopyOutToolsHWMode<Conv2dIntf>>::type;
    LoadAL1Tools loadAl1Ins;
    LoadBL1Tools<Conv2dIntf> loadBL1Ins;
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
    LoadAL0Tools loadAL0Ins;
    LoadBL0Tools<Conv2dIntf> loadBL0Ins;
    LoadBiasBtTools<Conv2dIntf> loadBiasBTIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PreQuantT> loadPreQuantFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PreReluT> loadPreReluFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PreClipReluT> loadPreClipReluFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::AntiQuantT> loadAntiQuantFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PostQuantT> loadPostQuantFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PostReluT> loadPostReluFBIns;
    LoadFixpParamFbTools<Conv2dIntf, typename Config::PostClipReluT> loadPostClipReluFBIns;
    MMadTools<Conv2dIntf> madIns;
    CopyOutTools copyOutIns;

public:
    __aicore__ inline Conv2dIntf() {}

    __aicore__ inline void Init(const TConv2DApiTiling *__restrict convTiling, TPipe *tPipe)
    {
        ctx.convTiling = convTiling;
        ctx.pipe = GetTPipePtr();

        if ASCEND_IS_AIC_CONV {
            printf("ASCEND_IS_AIC_CONV\n");
            InitAIC();
        } else {
            printf("ASCEND_IS_AIV_CONV\n");
            InitAIV(); 
        }
    }

    __aicore__ inline void SetOrgFmapShape(uint64_t orgCi, uint64_t orgHi, uint64_t orgWi)
    {
        ctx.orgCi = orgCi;
        ctx.orgHi = orgHi;
        ctx.orgWi = orgWi;
    }

    __aicore__ inline void SetOrgWeightShape(uint64_t orgCo, uint64_t orgCi, uint64_t orgKh, uint64_t orgKw)
    {
        ctx.orgCo = orgCo;
        ctx.orgCi = orgCi;
        ctx.kernelH = orgKh;
        ctx.kernelW = orgKw;
    }

    __aicore__ inline void SetOrgOutputShape(uint64_t orgCo, uint64_t orgHo, uint64_t orgWo)
    {
        ctx.orgCo = orgCo;
        ctx.orgHo = orgHo;
        ctx.orgWo = orgWo;
    }

    __aicore__ inline void SetSingleFmapShape(uint64_t singleCi, uint64_t singleHi, uint64_t singleWi)
    {
        ctx.singleCoreCi = singleCi;
        InitKDirectionValue();
    }

    __aicore__ inline void SetSingleOutputShape(uint64_t singleCo, uint64_t singleHo, uint64_t singleWo)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
            ctx.singleCoreCo = singleCo;
            ctx.singleCoreHo = singleHo;
            ctx.singleCoreWo = singleWo;
            if ASCEND_IS_AIC_CONV {
                InitCoDirectionValue();
                InitHoDirectionValue();
                // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                //     // OptGroupCalcBL1LoadTimes();
                // }
            } else {
                // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                //     // OptGroupInitNValue();
                //     // OptGroupInitIterValue();
                // }
            }
        }
    }

    __aicore__ inline void SetSingleOutputShape(uint64_t singleCo, uint64_t singleM)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            ctx.singleCoreCo = singleCo;
            ctx.singleCoreM = singleM;
            if ASCEND_IS_AIC_CONV {
                InitCoDirectionValue();
                InitMDirectionValue();
                // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                //     // OptGroupCalcBL1LoadTimes();
                // }
            } else {
                // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                //     // OptGroupInitNValue();
                //     // OptGroupInitIterValue();
                // }
            }
        }
    }

    __aicore__ inline void SetSingleBatchNum(uint64_t singleCoreBatch, uint64_t fmapOneBatchSize, uint64_t outputOneBatchSize)
    {
        ctx.ddr2l1LoopBatch = singleCoreBatch;
        if ASCEND_IS_AIC_CONV {
            ctx.fmapOneBatchSize = fmapOneBatchSize;
            ctx.outputOneBatchSize = outputOneBatchSize;
            // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            //     // OptGroupCalcBL1LoadTimes();
            // }
        } else {
            // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            //     // OptGroupInitIterValue();
            //     if (ctx.vecId == 1) {
            //         // OptGroupCalcBL1LoadTimes();
            //     }
            // }
        }
    }

    __aicore__ inline void SetFmapStartPosition(int64_t hiStartPos, int64_t wiStartPos)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
            if ASCEND_IS_AIC_CONV {
                ctx.hiStartPos = hiStartPos;
                ctx.wiStartPos = wiStartPos;
            }
        }
    }

    __aicore__ inline void SetFmapStartPosition(int64_t mStartPos)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            if ASCEND_IS_AIC_CONV {
                ctx.mStartPos = mStartPos;
            }
        }
    }

    __aicore__ inline void SetWeightStartPosition(int64_t coStartPos, int64_t ciStartPos = 0)
    {
        self->ctx.coStartPos = coStartPos;
        self->ctx.ciStartPos = ciStartPos;
    }

    // __aicore__ inline void SetOptGroupParams(uint64_t singleGroups, uint64_t singleGroupOpt)
    // {
    //     ctx.singleGroups = singleGroups;
    //     ctx.singleGroupOpt = singleGroupOpt;
    //     ctx.singleCoreCi = singleGroups * ctx.ciPerGroup;
    //     if ASCEND_IS_AIC_CONV {
    //         InitKDirectionValue();
    //         if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
    //             // OptGroupCalcBL1LoadTimes();
    //         }
    //     } else {
    //         if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
    //             // OptGroupInitKValue();
    //         }
    //     }
    // }

    __aicore__ inline void SetFmap(const GlobalTensor<FmapT> &fmap)
    {
        if ASCEND_IS_AIC_CONV {
            ctx.fmapGm.SetGlobalBuffer(fmap.GetPhyAddr(0), fmap.GetSize());
            ctx.isFirstIterate = true;
        }
    }

    __aicore__ inline void SetWeight(const GlobalTensor<WeightT> &weight)
    {
        ctx.weightGm.SetGlobalBuffer(weight.GetPhyAddr(0), weight.GetSize());
        ctx.isFirstIterate = true;
    }

    __aicore__ inline void SetBias(const GlobalTensor<BiasT> &bias)
    {
        if ASCEND_IS_AIC_CONV {
            ctx.biasGm.SetGlobalBuffer(bias.GetPhyAddr(0), bias.GetSize());
            ctx.isFirstIterate = true;
            ctx.hasBias = true;
        }
    }

    __aicore__ inline void SetPreQuant(const GlobalTensor<PreQuantT> &preQuant)
    {
        if ASCEND_IS_AIC_CONV {
            ctx.preQuantGm.SetGlobalBuffer(preQuant.GetPhyAddr(0), preQuant.GetSize());
            ctx.isFirstIterate = true;
            ctx.hasPreQuant = true;
        }
    }

    __aicore__ inline void SetPreRelu(const GlobalTensor<PreReluT> &preRelu)
    {
        if ASCEND_IS_AIC_CONV {
            ctx.preReluGm.SetGlobalBuffer(preRelu.GetPhyAddr(0), preRelu.GetSize());
            ctx.isFirstIterate = true;
            ctx.hasPreRelu = true;
        }
    }

    __aicore__ inline void SetPreClipRelu(const GlobalTensor<PreClipReluT> &preClipRelu)
    {
        if ASCEND_IS_AIC_CONV {
            ctx.preClipReluGm.SetGlobalBuffer(preClipRelu.GetPhyAddr(0), preClipRelu.GetSize());
            ctx.isFirstIterate = true;
            ctx.hasPreClipRelu = true;
        }
    }

    __aicore__ inline void SetAntiQuant(const GlobalTensor<AntiQuantT> &antiQuant)
    {
        if ASCEND_IS_AIC_CONV {
            ctx.antiQuantGm.SetGlobalBuffer(antiQuant.GetPhyAddr(0), antiQuant.GetSize());
            ctx.isFirstIterate = true;
            ctx.hasAntiQuant = true;
        }
    }

    __aicore__ inline void SetEltwise(const GlobalTensor<EltwiseT> &eltwise)
    {
        if ASCEND_IS_AIC_CONV {
            ctx.eltwiseGm.SetGlobalBuffer(eltwise.GetPhyAddr(0), eltwise.GetSize());
            ctx.isFirstIterate = true;
            ctx.hasEltwise = true;
            // [zj]TODO: eltwiseBrcEn
        }
    }

    __aicore__ inline void SetPostQuant(const GlobalTensor<PostQuantT> &postQuant)
    {
        if ASCEND_IS_AIC_CONV {
            ctx.postQuantGm.SetGlobalBuffer(postQuant.GetPhyAddr(0), postQuant.GetSize());
            ctx.isFirstIterate = true;
            ctx.hasPostQuant = true;
        }
    }

    __aicore__ inline void SetPostRelu(const GlobalTensor<PostReluT> &postRelu)
    {
        if ASCEND_IS_AIC_CONV {
            ctx.postReluGm.SetGlobalBuffer(postRelu.GetPhyAddr(0), postRelu.GetSize());
            ctx.isFirstIterate = true;
            ctx.hasPostRelu = true;
        }
    }

    __aicore__ inline void SetPostClipRelu(const GlobalTensor<PostClipReluT> &postClipRelu)
    {
        if ASCEND_IS_AIC_CONV {
            ctx.postClipReluGm.SetGlobalBuffer(postClipRelu.GetPhyAddr(0), postClipRelu.GetSize());
            ctx.isFirstIterate = true;
            ctx.hasPostClipRelu = true;
        }
    }

    template <bool sync = true>
    __aicore__ inline bool Iterate(bool enPartialSum = false)
    {
        if ASCEND_IS_AIC_CONV {
            if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
                return IterateImplMMode(enPartialSum);
            } else {
                return IterateImplHWMode(enPartialSum);
            }
        } else {
            // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            //     return OptGroupVecImpl();
            // } else {
            //     return false;
            // }
            return false;
        }
        return false;
    }

    template <bool sync = true>
    __aicore__ inline void GetTensorC(const GlobalTensor<OutputT> &output, bool enSequentialWrite = false)
    {
        if ASCEND_IS_AIC_CONV {
            copyOutIns.CopyOut(ctx, output);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
            ctx.queueOutputL1.FreeTensor(ctx.outputL1);
#else
            ctx.queueCL0.FreeTensor(ctx.cl0);
#endif

            if (ctx.hasVectorPreQuant && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePreQuantL1.FreeTensor(ctx.preQuantL1);
            }
            if (ctx.hasVectorPreRelu && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePreReluL1.FreeTensor(ctx.preReluL1);
            }
            if (ctx.hasVectorPreClipRelu && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePreClipReluL1.FreeTensor(ctx.preClipReluL1);
            }
            if (ctx.hasVectorAntiQuant && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueAntiQuantL1.FreeTensor(ctx.antiQuantL1);
            }
            if (ctx.hasEltwise && (!ctx.eltwiseBrcEn || !ctx.convTiling->fixpParamsFullLoadFlag)) {
                ctx.queueEltwiseL1.FreeTensor(ctx.eltwiseL1);
            }
            if (ctx.hasVectorPostQuant && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePostQuantL1.FreeTensor(ctx.postQuantL1);
            }
            if (ctx.hasVectorPostRelu && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePostReluL1.FreeTensor(ctx.postReluL1);
            }
            if (ctx.hasVectorPostClipRelu && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePostClipReluL1.FreeTensor(ctx.postClipReluL1);
            }
        }
    }

    template <bool sync = true>
    __aicore__ inline bool IterateAll(const GlobalTensor<OutputT> &output, bool enPartialSum = false)
    {
        if ASCEND_IS_AIC_CONV {
            // [zj]TODO: not brc
            if (ctx.convTiling->biasFullLoadFlag && ctx.hasBias) {
                ctx.biasL1 = ctx.queueBiasL1.template AllocTensor<BiasT>();
                loadBiasL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.biasL1, ctx.biasGm, ctx.singleCoreCo, 0);
                ctx.queueBiasL1.EnQue(ctx.biasL1);
                ctx.biasL1 = ctx.queueBiasL1.template DeQue<BiasT>();
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPreQuant) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.preQuantL1 = ctx.queuePreQuantL1.template AllocTensor<PreQuantT>();
                loadPreQuantL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.preQuantL1, ctx.preQuantGm, ctx.singleCoreCo, 0);
                ctx.queuePreQuantL1.EnQue(ctx.preQuantL1);
                ctx.preQuantL1 = ctx.queuePreQuantL1.template DeQue<PreQuantT>();
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPreRelu) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.preReluL1 = ctx.queuePreReluL1.template AllocTensor<PreReluT>();
                loadPreReluL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.preReluL1, ctx.preReluGm, ctx.singleCoreCo, 0);
                ctx.queuePreReluL1.EnQue(ctx.preReluL1);
                ctx.preReluL1 = ctx.queuePreReluL1.template DeQue<PreReluT>();
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPreClipRelu) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.preClipReluL1 = ctx.queuePreClipReluL1.template AllocTensor<PreClipClipReluT>();
                loadPreClipReluL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.preClipReluL1, ctx.preClipReluGm, ctx.singleCoreCo, 0);
                ctx.queuePreClipReluL1.EnQue(ctx.preClipReluL1);
                ctx.preClipReluL1 = ctx.queuePreClipReluL1.template DeQue<PreClipReluT>();
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorAntiQuant) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.antiQuantL1 = ctx.queueAntiQuantL1.template AllocTensor<AntiQuantT>();
                loadAntiQuantL1Ins.LoadChannelWiseL1FullLoad(
                    ctx, ctx.antiQuantL1, ctx.antiQuantGm, ctx.singleCoreCo, 0);
                ctx.queueAntiQuantL1.EnQue(ctx.antiQuantL1);
                ctx.antiQuantL1 = ctx.queueAntiQuantL1.template DeQue<AntiQuantT>();
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasEltwise && ctx.eltwiseBrcEn) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.eltwiseL1 = ctx.queueEltwiseL1.template AllocTensor<EltwiseT>();
                loadEltwiseL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.eltwiseL1, ctx.eltwiseGm, ctx.singleCoreCo, 0);
                ctx.queueEltwiseL1.EnQue(ctx.eltwiseL1);
                ctx.eltwiseL1 = ctx.queueEltwiseL1.template DeQue<EltwiseT>();
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPostQuant) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.postQuantL1 = ctx.queuePostQuantL1.template AllocTensor<PostQuantT>();
                loadPostQuantL1Ins.LoadChannelWiseL1FullLoad(
                    ctx, ctx.postQuantL1, ctx.postQuantGm, ctx.singleCoreCo, 0);
                ctx.queuePostQuantL1.EnQue(ctx.postQuantL1);
                ctx.postQuantL1 = ctx.queuePostQuantL1.template DeQue<PostQuantT>();
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPostRelu) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.postReluL1 = ctx.queuePostReluL1.template AllocTensor<PostReluT>();
                loadPostReluL1Ins.LoadChannelWiseL1FullLoad(
                    ctx, ctx.postReluL1, ctx.postReluGm, ctx.singleCoreCo, 0);
                ctx.queuePostReluL1.EnQue(ctx.postReluL1);
                ctx.postReluL1 = ctx.queuePostReluL1.template DeQue<PostReluT>();
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPostClipRelu) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.postClipReluL1 = ctx.queuePostClipReluL1.template AllocTensor<PostClipReluT>();
                loadPostClipReluL1Ins.LoadChannelWiseL1FullLoad(
                    ctx, ctx.postClipReluL1, ctx.postClipReluGm, ctx.singleCoreCo, 0);
                ctx.queuePostClipReluL1.EnQue(ctx.postClipReluL1);
                ctx.postClipReluL1 = ctx.queuePostClipReluL1.template DeQue<PostClipReluT>();
            }
        }

        while (Iterate(enPartialSum)) {
            GetTensorC(output);
        }

        if ASCEND_IS_AIC_CONV {
            if (ctx.convTiling->biasFullLoadFlag && ctx.hasBias) {
                ctx.queueBiasL1.FreeTensor(ctx.biasL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPreQuant) {
                ctx.queuePreQuantL1.FreeTensor(ctx.preQuantL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPreRelu) {
                ctx.queuePreReluL1.FreeTensor(ctx.preReluL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPreClipRelu) {
                ctx.queuePreClipReluL1.FreeTensor(ctx.preClipReluL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorAntiQuant) {
                ctx.queueAntiQuantL1.FreeTensor(ctx.antiQuantL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasEltwise && ctx.eltwiseBrcEn) {
                ctx.queueEltwiseL1.FreeTensor(ctx.eltwiseL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPostQuant) {
                ctx.queuePostQuantL1.FreeTensor(ctx.postQuantL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPostRelu) {
                ctx.queuePostReluL1.FreeTensor(ctx.postReluL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.hasVectorPostClipRelu) {
                ctx.queuePostClipReluL1.FreeTensor(ctx.postClipReluL1);
            }
            ctx.isFirstIterate = true;
        }

        return false;
    }

    __aicore__ inline void End()
    {
        if ASCEND_IS_AIC_CONV {
            ctx.queueFmapL1.FreeAllEvent();
            ctx.queueWeightL1.FreeAllEvent();
            ctx.queueBiasL1.FreeAllEvent();
            ctx.queuePreQuantL1.FreeAllEvent();
            ctx.queuePreReluL1.FreeAllEvent();
            ctx.queuePreClipReluL1.FreeAllEvent();
            ctx.queueAntiQuantL1.FreeAllEvent();
            ctx.queueEltwiseL1.FreeAllEvent();
            ctx.queuePostQuantL1.FreeAllEvent();
            ctx.queuePostReluL1.FreeAllEvent();
            ctx.queuePostClipReluL1.FreeAllEvent();
            ctx.queueBiasBT.FreeAllEvent();
            ctx.queuePreQuantFB.FreeAllEvent();
            ctx.queuePreReluFB.FreeAllEvent();
            ctx.queuePreClipReluFB.FreeAllEvent();
            ctx.queueAntiQuantFB.FreeAllEvent();
            ctx.queuePostQuantFB.FreeAllEvent();
            ctx.queuePostReluFB.FreeAllEvent();
            ctx.queuePostClipReluFB.FreeAllEvent();
            ctx.queueOutputL1.FreeAllEvent();
            ctx.queueCL0.FreeAllEvent();
        }
    }

private:
    __aicore__ inline void InitAIC()
    {
        // [zj]TODO: check others
        ctx.singleCoreCi = ctx.convTiling->singleCoreCi;
        ctx.singleCoreCo = ctx.convTiling->singleCoreCo;
        ctx.orgCi = ctx.convTiling->orgCi;
        ctx.orgCo = ctx.convTiling->orgCo;
        ctx.orgHi = ctx.convTiling->orgHi;
        ctx.orgWi = ctx.convTiling->orgWi;
        ctx.orgHo = ctx.convTiling->orgHo;
        ctx.orgWo = ctx.convTiling->orgWo;
        ctx.kernelH = ctx.convTiling->kernelH;
        ctx.kernelW = ctx.convTiling->kernelW;
        ctx.dilatedKernelH = 1 + (ctx.kernelH - 1) * ctx.convTiling->dilationH;
        ctx.dilatedKernelW = 1 + (ctx.kernelW - 1) * ctx.convTiling->dilationW;

        uint64_t alignCinKhKwA = AlignB(ctx.convTiling->singleCoreCi * ctx.convTiling->kernelHxkernelW, k0a);
        uint64_t alignCinKhKwB = AlignB(ctx.convTiling->singleCoreCi * ctx.convTiling->kernelHxkernelW, k0b);
        ctx.kAL1fullload = alignCinKhKwA == ctx.convTiling->kAL1;
        ctx.kBL1fullload = alignCinKhKwB == ctx.convTiling->kBL1;

        ctx.kL0FullLoadAl0PingPongFlag = 0;
        ctx.kL0FullLoadBl0PingPongFlag = 0;

        ctx.hasBias = ctx.convTiling->hasBias;
        ctx.hasPreQuant = ctx.convTiling->hasPreQuant;
        ctx.hasPreRelu = ctx.convTiling->hasPreRelu;
        ctx.hasPreClipRelu = ctx.convTiling->hasPreClipRelu;
        ctx.hasAntiQuant = ctx.convTiling->hasAntiQuant;
        ctx.hasEltwise = ctx.convTiling->hasEltwise;
        ctx.hasPostQuant = ctx.convTiling->hasPostQuant;
        ctx.hasPostRelu = ctx.convTiling->hasPostRelu;
        ctx.hasPostClipRelu = ctx.convTiling->hasPostClipRelu;

        ctx.hasVectorPreQuant = ctx.convTiling->hasPreQuant &&
                                IsVectorMode(static_cast<ConvPreQuantMode>(ctx.convTiling->preQuantMode));
        ctx.hasVectorPreRelu = ctx.convTiling->hasPreRelu &&
                               IsVectorMode(static_cast<ConvPreReluMode>(ctx.convTiling->preReluMode));
        ctx.hasVectorPreClipRelu = ctx.convTiling->hasPreClipRelu &&
                                   IsVectorMode(static_cast<ConvPreClipReluMode>(ctx.convTiling->preClipReluMode));
        ctx.hasVectorAntiQuant = ctx.convTiling->hasAntiQuant &&
                                 IsVectorMode(static_cast<ConvAntiQuantMode>(ctx.convTiling->antiQuantMode));
        ctx.hasVectorPostQuant = ctx.convTiling->hasPostQuant &&
                                 IsVectorMode(static_cast<ConvPostQuantMode>(ctx.convTiling->postQuantMode));
        ctx.hasVectorPostRelu = ctx.convTiling->hasPostRelu &&
                                IsVectorMode(static_cast<ConvPostReluMode>(ctx.convTiling->postReluMode));
        ctx.hasVectorPostClipRelu = ctx.convTiling->hasPostClipRelu &&
                                    IsVectorMode(static_cast<ConvPostClipReluMode>(ctx.convTiling->postClipReluMode));

        ctx.iter = 0;
        ctx.kIter = 0;
        ctx.mAL1UpdateFlag = true;

        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            InitValueMMode();
        } else {
            InitValueHWMode();
        }
        InitBuffer();
        InitKDirectionValue();
        InitCoDirectionValue();
        InitHf32Mode();
        InitSubApiParams();

        // [zj]TODO: group conv
        // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
        //     ctx.ciPerGroup = ctx.convTiling->orgCi / ctx.convTiling->groups;
        //     ctx.singleGroupOpt = ctx.convTiling->singleCoreGroupOpt;
        //     // OptGroupCalcBL1LoadTimes();
        // }
    }

    __aicore__ inline void InitAIV()
    {
        // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
        //     // OptGroupVecInit();
        // }
    }

    __aicore__ inline void InitValueMMode()
    {
        ctx.mAL1 = ctx.convTiling->hoL1;
        ctx.mL0 = ctx.convTiling->hoL0;
        ctx.singleCoreM = ctx.convTiling->singleCoreHo;
        InitMDirectionValue();
    }

    __aicore__ inline void InitValueHWMode()
    {
        ctx.singleCoreHo = ctx.convTiling->singleCoreHo;
        ctx.singleCoreWo = ctx.convTiling->singleCoreWo;
        InitHoDirectionValue();
        InitWoDirectionValue();
    }

    __aicore__ inline void InitMDirectionValue()
    {
        ctx.ddr2l1LoopM = CeilDIV(ctx.singleCoreM, ctx.mAL1);
        ctx.maxMAL1Iter = ctx.ddr2l1LoopM - 1;
        ctx.mAL1Tail = ctx.singleCoreM % ctx.mAL1;
        ctx.mAL1Tail = ctx.mAL1Tail == 0 ? ctx.mAL1 : ctx.mAL1Tail;
        ctx.currentMAL1 = ctx.mAL1Tail;

        ctx.l12l0LoopM = CeilDIV(ctx.mAL1, ctx.mL0);
        ctx.maxML0Iter = ctx.l12l0LoopM - 1;
        ctx.mAL0Tail = ctx.mAL1Tail % ctx.mL0;
        ctx.mAL0Tail = ctx.mAL0Tail == 0 ? ctx.mL0 : ctx.mAL0Tail;
        ctx.currentML0Align = AlignB(ctx.mAL0Tail, BLOCK_L0_M);
    }

    __aicore__ inline void InitHoDirectionValue()
    {
        ctx.hoAL1Tail = ctx.singleCoreHo % ctx.convTiling->hoL1;
        ctx.hoAL1Tail = ctx.hoAL1Tail == 0 ? ctx.convTiling->hoL1 : ctx.hoAL1Tail;
        ctx.currentHoL1 = ctx.hoAL1Tail;

        ctx.hoL0Tail = ctx.currentHoL1 % ctx.convTiling->hoL0;
        ctx.hoL0Tail = ctx.hoL0Tail == 0 ? ctx.convTiling->hoL0 : ctx.hoL0Tail;
        ctx.currentHoL0 = ctx.hoL0Tail;

        if constexpr (!hasHL1IterFlag) {
            ctx.ddr2l1LoopH = 1;
            ctx.maxHoL1Iter = 0;
        } else {
            ctx.ddr2l1LoopH = CeilDIV(ctx.singleCoreHo, ctx.convTiling->hoL1);
            ctx.maxHoL1Iter = ctx.ddr2l1LoopH - 1;
        }
    }

    __aicore__ inline void InitWoDirectionValue()
    {
        ctx.maxWoL1Iter = 0;
        ctx.woAL1Tail = ctx.singleCoreWo % ctx.convTiling->woL1;
        ctx.woAL1Tail = ctx.woAL1Tail == 0 ?  ctx.convTiling->woL1 : ctx.woAL1Tail;
        ctx.currentWoL1 = ctx.woAL1Tail;

        ctx.maxWoL0Iter = 0;
        ctx.woL0Tail = ctx.currentWoL1 % ctx.convTiling->woL0;
        ctx.woL0Tail = ctx.woL0Tail == 0 ? ctx.convTiling->woL0 : ctx.woL0Tail;
        ctx.currentWoL0 = ctx.woL0Tail;

        ctx.l12l0LoopW = 1;

        if constexpr (hasWL1IterFlag) {
            ctx.maxWoL1Iter = CeilDIV(ctx.singleCoreWo, ctx.convTiling->woL1) - 1;
        }

        if (hasWL0IterFlag &&
            ctx.convTiling->hoL0 > 1 &&
            ctx.woAL1Tail % BLOCK_L0_N > 0 &&
            CeilDIV(ctx.woAL1Tail, ctx.convTiling->woL0) > 1) {
            ctx.woL1SmallTail = ctx.woAL1Tail % ctx.convTiling->woL0;
            ctx.woAL1Tail = (ctx.woAL1Tail / ctx.convTiling->woL0) * ctx.convTiling->woL0;
        }

        if (ctx.woL1SmallTail > 0) {
            ctx.maxWoL1Iter += 1;
        }

        ctx.ddr2l1LoopW = ctx.maxWoL1Iter + 1;
    }

    __aicore__ inline void InitKDirectionValue()
    {
        // if constexpr (formatWeight == ConvFormat::FRACTAL_Z_C04) {
        //     size_t multiKL1 = ctx.convTiling->kAL1 / ctx.convTiling->kL0;
        //     ctx.maxKAL1Iter = 0;
        //     ctx.maxKBL1Iter = 0;
        //     ctx.kAL1Tail = ctx.convTiling->kAL1;
        //     ctx.kBL1Tail = ctx.convTiling->kBL1;

        //     ctx.ddr2l0LoopK = multiKL1;
        //     ctx.maxKL0Iter = multiKL1 - 1;
        //     ctx.kL0Tail = ctx.convTiling->kL0;

        //     ctx.multiKAL1 = multiKL1;
        //     ctx.multiKBL1 = multiKL1;

        //     ctx.kAL1fullload = true;
        //     ctx.kBL1fullload = true;
        //     return;
        // }

        ctx.maxKAL1Iter = CeilDIV(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW,
                                  ctx.convTiling->kAL1) - 1;
        ctx.maxKBL1Iter = CeilDIV(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW,
                                  ctx.convTiling->kBL1) - 1;
        if constexpr (formatFmap == ConvFormat::NC1HWC0) {
            ctx.kAL1Tail = (AlignB(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, k0a)) % ctx.convTiling->kAL1;
            ctx.kAL1Tail = ctx.kAL1Tail == 0 ? ctx.convTiling->kAL1 : ctx.kAL1Tail;
        } else {
            ctx.kAL1Tail = (ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW) % ctx.convTiling->kAL1;
            ctx.kAL1Tail = ctx.kAL1Tail == 0 ? ctx.convTiling->kAL1 : ctx.kAL1Tail;
        }
        if constexpr (formatWeight == ConvFormat::FRACTAL_Z) {
            ctx.kBL1Tail = (AlignB(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, k0b)) % ctx.convTiling->kBL1;
            ctx.kBL1Tail = ctx.kBL1Tail == 0 ? ctx.convTiling->kBL1 : ctx.kBL1Tail;
        } else {
            ctx.kBL1Tail = (ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW) % ctx.convTiling->kBL1;
            ctx.kBL1Tail = ctx.kBL1Tail == 0 ? ctx.convTiling->kBL1 : ctx.kBL1Tail;
        }

        ctx.ddr2l0LoopK = CeilDIV(AlignB(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, k0),
                                  ctx.convTiling->kL0);
        ctx.maxKL0Iter = ctx.ddr2l0LoopK - 1;
        ctx.kL0Tail = (AlignB(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, k0)) % ctx.convTiling->kL0;
        ctx.kL0Tail = ctx.kL0Tail == 0 ? ctx.convTiling->kL0 : ctx.kL0Tail;

        ctx.multiKAL1 = CeilDIV(ctx.convTiling->kAL1, ctx.convTiling->kL0);
        ctx.multiKBL1 = CeilDIV(ctx.convTiling->kBL1, ctx.convTiling->kL0);

        if constexpr (kPreLoadFlag) {
            ctx.lastLoopKAL1StartPos = ctx.ddr2l0LoopK - ctx.multiKAL1;
            ctx.lastLoopKBL1StartPos = ctx.ddr2l0LoopK - ctx.multiKBL1;
        }
    }

    __aicore__ inline void InitCoDirectionValue()
    {
        if constexpr (!hasNL1IterFlag) {
            ctx.ddr2l1LoopN = 1;
            ctx.maxNBL1Iter = 0;
        } else {
            ctx.ddr2l1LoopN = CeilDIV(ctx.singleCoreCo, ctx.convTiling->nBL1);
            ctx.maxNBL1Iter = ctx.ddr2l1LoopN - 1;
        }
        ctx.nBL1Tail = ctx.singleCoreCo % ctx.convTiling->nBL1;
        ctx.nBL1Tail = ctx.nBL1Tail == 0 ? ctx.convTiling->nBL1 : ctx.nBL1Tail;
        ctx.currentNBL1 = ctx.nBL1Tail;

        if constexpr (!hasNL0IterFlag) {
            ctx.l12l0LoopN = 1;
            ctx.maxNL0Iter = 0;
        } else {
            ctx.l12l0LoopN = ctx.nBL1Iter == ctx.maxNBL1Iter ?
                CeilDIV(ctx.nBL1Tail, ctx.convTiling->nL0) : ctx.convTiling->multiNBL1;
            ctx.maxNL0Iter = ctx.l12l0LoopN - 1;
        }
        ctx.nL0Tail = ctx.nBL1Tail % ctx.convTiling->nL0;
        ctx.nL0Tail = ctx.nL0Tail == 0 ? ctx.convTiling->nL0 : ctx.nL0Tail;
    }

    __aicore__ inline void InitBuffer()
    {
        InitBufferWithDoubleBuf();

        constexpr uint8_t bufferNum = cl0DBFlag ? DOUBLE_BUF : 1;

        if (ctx.convTiling->hasBias) {
            uint64_t biasL1Spacesize = 0;
            uint64_t biasBTSpacesize = 0;
            biasL1Spacesize = ctx.convTiling->biasFullLoadFlag ?
                                    AlignB(ctx.singleCoreCo * sizeOfBias, BLOCK_L0_N * sizeOfBias) :
                                    ctx.convTiling->nL0 * sizeOfBias;
            biasBTSpacesize = ctx.convTiling->nL0 * sizeOfBias;
            // biasBTSpacesize = BIAS_TABLE_SIZE / static_cast<uint64_t>(bufferNum);
            printf("biasBTSpacesize: %ld\n", biasBTSpacesize);
            ctx.pipe->InitBuffer(ctx.queueBiasL1, 1, AlignB(biasL1Spacesize, C0_SIZE));
            ctx.pipe->InitBuffer(ctx.queueBiasBT, bufferNum, AlignB(biasBTSpacesize, BIAS_TABLE_ALIGN_SIZE));
        }

        if (ctx.convTiling->hasVectorPreQuant) {
            uint64_t preQuantL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                               AlignB(ctx.singleCoreCo * sizeOfPreQuant, BLOCK_L0_N * sizeOfPreQuant) :
                                               ctx.convTiling->nL0 * sizeOfPreQuant;
            ctx.pipe->InitBuffer(ctx.queuePreQuantL1, 1, AlignB(preQuantL1SpaceSize, C0_SIZE));
            uint64_t preQuantFBSpacesize = ctx.convTiling->nL0 * sizeOfPreQuant;
            // preQuantFBSpacesize = FB_PRE_QUANT_SIZE / static_cast<uint64_t>(bufferNum);
            printf("preQuantFBSpacesize: %ld\n", preQuantFBSpacesize);
            ctx.pipe->InitBuffer(ctx.queuePreQuantFB, bufferNum, AlignB(preQuantFBSpacesize, FB_PRE_sQUANT_ALIGN_SIZE));
        }

        if (ctx.convTiling->hasVectorPreRelu) {
            uint64_t preReluL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                              AlignB(ctx.singleCoreCo * sizeOfPreRelu, BLOCK_L0_N * sizeOfPreRelu) :
                                              ctx.convTiling->nL0 * sizeOfPreRelu;
            ctx.pipe->InitBuffer(ctx.queuePreReluL1, 1, AlignB(preReluL1SpaceSize, C0_SIZE));
            uint64_t preReluFBSpaceSize = ctx.convTiling->nL0 * sizeOfPreRelu;
            // preReluFBSpaceSize = FB_PRE_RELU_SIZE / static_cast<uint64_t>(bufferNum);
            printf("preReluFBSpaceSize: %ld\n", preReluFBSpaceSize);
            ctx.pipe->InitBuffer(ctx.queuePreReluFB, bufferNum, AlignB(preReluFBSpaceSize, FB_PRE_RELU_ALIGN_SIZE));
        }

        if (ctx.convTiling->hasVectorPreClipRelu) {
            uint64_t preClipReluL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                                  AlignB(ctx.singleCoreCo * sizeOfPreClipRelu, BLOCK_L0_N * sizeOfPreClipRelu) :
                                                  ctx.convTiling->nL0 * sizeOfPreClipRelu;
            ctx.pipe->InitBuffer(ctx.queuePreClipReluL1, 1, AlignB(preClipReluL1SpaceSize, C0_SIZE));
            uint64_t preClipReluFBSpaceSize = ctx.convTiling->nL0 * sizeOfPreClipRelu;
            // preReluFBSpaceSize = FB_PRE_CLIP_RELU_SIZE / static_cast<uint64_t>(bufferNum);
            printf("preClipReluFBSpaceSize: %ld\n", preClipReluFBSpaceSize);
            ctx.pipe->InitBuffer(ctx.queuePreClipReluFB, bufferNum, AlignB(preClipReluFBSpaceSize, FB_PRE_CLIP_RELU_ALIGN_SIZE));
        }

        if (ctx.convTiling->hasVectorAntiQuant) {
            uint64_t antiQuantL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                                AlignB(ctx.singleCoreCo * sizeOfAntiQuant, BLOCK_L0_N * sizeOfAntiQuant) :
                                                ctx.convTiling->nL0 * sizeOfAntiQuant;
            ctx.pipe->InitBuffer(ctx.queueAntiQuantL1, 1, AlignB(antiQuantL1SpaceSize, C0_SIZE));
            uint64_t antiQuantFBSpacesize = ctx.convTiling->nL0 * sizeOfAntiQuant;
            // antiQuantFBSpacesize = FB_ANTI_QUANT_SIZE / static_cast<uint64_t>(bufferNum);
            printf("antiQuantFBSpacesize: %ld\n", antiQuantFBSpacesize);
            ctx.pipe->InitBuffer(ctx.queueAntiQuantFB, bufferNum, AlignB(antiQuantFBSpacesize, FB_ANTI_QUANT_ALIGN_SIZE));
        }

        if (ctx.convTiling->hasEltwise) {
            uint64_t eltwiseL1SpaceSize = 0;
            if (ctx.convTiling->eltwiseBrcEn) {
                eltwiseL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                         AlignB(ctx.singleCoreCo * sizeOfEltwise, BLOCK_L0_N * sizeOfEltwise) :
                                         ctx.convTiling->nL0 * sizeOfEltwise;
            } else {
                eltwiseL1SpaceSize = ctx.convTiling->mStep * ctx.convTiling->nL0 * sizeOfEltwise;
            }
            ctx.pipe->InitBuffer(ctx.queueEltwiseL1, bufferNum, AlignB(eltwiseL1SpaceSize, C0_SIZE));
        }

        if (ctx.convTiling->hasVectorQuantPost) {
            uint64_t postQuantL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                                AlignB(ctx.singleCoreCo * sizeOfPostQuant, BLOCK_L0_N * sizeOfPostQuant) :
                                                ctx.convTiling->nL0 * sizeOfPostQuant;
            ctx.pipe->InitBuffer(ctx.queuePostQuantL1, 1, AlignB(postQuantL1SpaceSize, C0_SIZE));
            uint64_t postQuantFBSpacesize = ctx.convTiling->nL0 * sizeOfPostQuant;
            // postQuantFBSpacesize = FB_POST_QUANT_SIZE / static_cast<uint64_t>(bufferNum);
            printf("postQuantFBSpacesize: %ld\n", postQuantFBSpacesize);
            ctx.pipe->InitBuffer(ctx.queuePostQuantFB, bufferNum, AlignB(postQuantFBSpacesize, FB_POST_QUANT_ALIGN_SIZE));
        }

        if (ctx.convTiling->hasVectorPostRelu) {
            uint64_t postReluL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                               AlignB(ctx.singleCoreCo * sizeOfPostRelu, BLOCK_L0_N * sizeOfPostRelu) :
                                               ctx.convTiling->nL0 * sizeOfPostRelu;
            ctx.pipe->InitBuffer(ctx.queueReluPostL1, 1, AlignB(postReluL1SpaceSize, C0_SIZE));
            uint64_t postReluFBSpaceSize = ctx.convTiling->nL0 * sizeOfPostRelu;
            // postReluFBSpaceSize = FB_POST_RELU_SIZE / static_cast<uint64_t>(bufferNum);
            printf("postReluFBSpaceSize: %ld\n", postReluFBSpaceSize);
            ctx.pipe->InitBuffer(ctx.queuePostReluFB, bufferNum, AlignB(postReluFBSpaceSize, FB_POST_RELU_ALIGN_SIZE));
        }

        if (ctx.convTiling->hasVectorPostClipRelu) {
            uint64_t postClipReluL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag ?
                                                  AlignB(ctx.singleCoreCo * sizeOfPostClipRelu, BLOCK_L0_N * sizeOfPostClipRelu) :
                                                  ctx.convTiling->nL0 * sizeOfPostClipRelu;
            ctx.pipe->InitBuffer(ctx.queuePostClipReluL1, 1, AlignB(postClipReluL1SpaceSize, C0_SIZE));
            uint64_t postClipReluFBSpaceSize = ctx.convTiling->nL0 * sizeOfPostClipRelu;
            // postReluFBSpaceSize = FB_POST_CLIP_RELU_SIZE / static_cast<uint64_t>(bufferNum);
            printf("postClipReluFBSpaceSize: %ld\n", postClipReluFBSpaceSize);
            ctx.pipe->InitBuffer(ctx.queuePostClipReluFB, bufferNum, AlignB(postClipReluFBSpaceSize, FB_POST_CLIP_RELU_ALIGN_SIZE));
        }
    }

    __aicore__ inline void InitBufferWithDoubleBuf()
    {
        ctx.pipe->InitBuffer(ctx.l0aBuf, L0A_SIZE);
        ctx.pipe->InitBuffer(ctx.l0bBuf, L0B_SIZE);
        ctx.wholeAl0Tensor = ctx.l0aBuf.template Get<FmapT>();
        ctx.wholeBl0Tensor = ctx.l0bBuf.template Get<WeightT>();

    #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
        if constexpr (!cl0DBFlag) {
            ctx.pipe->InitBuffer(ctx.queueCL1, 1, ctx.convTiling->c0SpaceSize);
        } else {
            ctx.pipe->InitBuffer(ctx.queueCL1, DOUBLE_BUF, ctx.convTiling->c0SpaceSize);
        }
    #else
        if constexpr (!cl0DBFlag) {
            ctx.pipe->InitBuffer(ctx.queueCL0, 1, ctx.convTiling->c0SpaceSize);
        } else {
            ctx.pipe->InitBuffer(ctx.queueCL0, DOUBLE_BUF, ctx.convTiling->c0SpaceSize);
        }
    #endif

        if constexpr (!aL1DBFlag) {
            ctx.pipe->InitBuffer(ctx.queueAL1, 1, ctx.convTiling->a1SpaceSize);
        } else {
            ctx.pipe->InitBuffer(ctx.queueAL1, DOUBLE_BUF, ctx.convTiling->a1SpaceSize);
        }

        if constexpr (!Intf::bl1ByPass) {
            if constexpr (!bL1DBFlag) {
                ctx.pipe->InitBuffer(ctx.queueBL1, 1, ctx.convTiling->b1SpaceSize);
            } else {
                ctx.pipe->InitBuffer(ctx.queueBL1, DOUBLE_BUF, ctx.convTiling->b1SpaceSize);
            }
        }
    }

    __aicore__ inline void InitHf32Mode()
    {
        if (ctx.convTiling->hf32Enable) {
            SetHF32Mode(ctx.convTiling->hf32Enable);
            SetHF32TransMode(ctx.convTiling->hf32TransMode);
        }
    }

    __aicore__ inline void InitSubApiParams()
    {
        loadBL1Ins.SetParams(ctx);
        loadAl1Ins.SetParams(ctx);
        loadAL0Ins.SetParams(ctx);
        loadBL0Ins.SetParams(ctx);
        loadBiasL1Ins.SetParams(ctx);
        loadBiasNonBrcL1Ins.SetParams(ctx);
        loadPreQuantL1Ins.SetParams(ctx);
        loadPreReluL1Ins.SetParams(ctx);
        loadPreClipReluL1Ins.SetParams(ctx);
        loadAntiQuantL1Ins.SetParams(ctx);
        loadEltwiseL1Ins.SetParams(ctx);
        loadEltwiseNonBrcL1Ins.SetParams(ctx);
        loadPostQuantL1Ins.SetParams(ctx);
        loadPostReluL1Ins.SetParams(ctx);
        loadPostClipReluL1Ins.SetParams(ctx);
        loadBiasBTIns.SetParams(ctx);
        loadPreQuantFBIns.SetParams(ctx);
        loadPreReluFBIns.SetParams(ctx);
        loadPreClipReluFBIns.SetParams(ctx);
        loadAntiQuantFBIns.SetParams(ctx);
        loadPostQuantFBIns.SetParams(ctx);
        loadPostReluFBIns.SetParams(ctx);
        loadPostClipReluFBIns.SetParams(ctx);
        madIns.SetParams(ctx);
        copyOutIns.SetParams(ctx);
    }

    __aicore__ inline void CalcMDirectionVar()
    {
        if constexpr (hasML1IterFlag) {
            ctx.currentMAL1 = ctx.mAL1Iter == ctx.maxMAL1Iter ? ctx.mAL1Tail :
                                                                ctx.mAL1;
            ctx.mAL1UpdateFlag = true; // [zj]TOD0: can be optimized
        }

        if constexpr (hasML0IterFlag) { // [zj]TODO: check if need
            ctx.l12l0LoopM = CeilDIV(ctx.currentMAL1, ctx.mL0);
            ctx.maxML0Iter = ctx.l12l0LoopM - 1;
            ctx.mAL0Tail = ctx.currentMAL1 % ctx.mL0;
            ctx.mAL0Tail = ctx.mAL0Tail == 0 ? ctx.mL0 : ctx.mAL0Tail;
            (void)CalcCurrentML0MMode();
        }
    }

    __aicore__ inline void CalcHoDirectionVar()
    {
        if constexpr (hasHL1IterFlag) {
            ctx.currentHoL1 = ctx.hoAL1Iter == ctx.maxHoL1Iter ? ctx.hoAL1Tail :
                                                                 ctx.convTiling->hoL1;
        }

        if constexpr (hasHL0IterFlag) {
            ctx.l12l0LoopH = CeilDIV(ctx.currentHoL1, ctx.convTiling->hoL0);
            ctx.maxHoL0Iter = ctx.l12l0LoopH - 1;
            ctx.hoL0Tail = ctx.currentHoL1 % ctx.convTiling->hoL0;
            ctx.hoL0Tail = ctx.hoL0Tail == 0 ? ctx.convTiling->hoL0 : ctx.hoL0Tail;
            ctx.currentHoL0 = ctx.hoL0Tail;
        }
    }

    __aicore__ inline void CalcWoDirectionVar()
    {
        if constexpr (hasWL1IterFlag) {
            if (ctx.woL1SmallTail > 0) {
                if (ctx.woAL1Iter == ctx.maxWoL1Iter) {
                    ctx.currentWoL1 = ctx.woL1SmallTail;
                } else if (ctx.woAL1Iter == ctx.maxWoL1Iter - 1) {
                    ctx.currentWoL1 = ctx.woAL1Tail;
                } else {
                    ctx.currentWoL1 = ctx.convTiling->woL1;
                }
            } else {
                ctx.currentWoL1 = ctx.woAL1Iter == ctx.maxWoL1Iter ? ctx.woAL1Tail :
                                                                     ctx.convTiling->woL1;
            }
        }

        if constexpr (hasWL0IterFlag) {
            ctx.l12l0LoopW = CeilDIV(ctx.currentWoL1, ctx.convTiling->woL0);
            ctx.maxWoL0Iter = ctx.l12l0LoopW - 1;
            ctx.woL0Tail = ctx.currentWoL1 % ctx.convTiling->woL0;
            ctx.woL0Tail = ctx.woL0Tail == 0 ? ctx.convTiling->woL0 : ctx.woL0Tail;
            ctx.currentWoL0 = ctx.woL0Tail;
        }
    }

    __aicore__ inline void CalcCoDirectionVar()
    {
        if constexpr (hasNL1IterFlag) {
            ctx.currentNBL1 = ctx.nBL1Iter == ctx.maxNBL1Iter ? ctx.nBL1Tail :
                                                                ctx.convTiling->nBL1;
        }

        if constexpr (hasNL0IterFlag) {
            ctx.l12l0LoopN = CeilDIV(ctx.currentNBL1, ctx.convTiling->nL0);
            ctx.maxNL0Iter = ctx.l12l0LoopN - 1;
            ctx.nL0Tail = ctx.currentNBL1 % ctx.convTiling->nL0;
            ctx.nL0Tail = ctx.nL0Tail == 0 ? ctx.convTiling->nL0 : ctx.nL0Tail;
            (void)CalcCurrentNL0();
        }
    }

    __aicore__ inline uint64_t CalcCurrentNL0()
    {
        bool isNL0Tail = ctx.nBL1Iter == ctx.maxNBL1Iter && ctx.nL0Iter == ctx.maxNL0Iter;
        uint64_t currentNL0 = isNL0Tail ? ctx.nL0Tail : ctx.convTiling->nL0;
        ctx.currentNL0Align = isNL0Tail ? AlignB(currentNL0, BLOCK_L0_N) : currentNL0;
        return currentNL0;
    }

    __aicore__ inline uint64_t CalcCurrentML0MMode()
    {
        printf("mL0: %ld, mAL0Tail: %ld, mAL1Iter: %ld, maxMAL1Iter: %ld, mL0Iter: %ld, maxML0Iter: %ld\n", ctx.mL0,
               ctx.mAL0Tail, ctx.mAL1Iter, ctx.maxMAL1Iter, ctx.mL0Iter, ctx.maxML0Iter);
        bool isML0Tail = ctx.mAL1Iter == ctx.maxMAL1Iter && ctx.mL0Iter == ctx.maxML0Iter;
        uint64_t currentML0 = isML0Tail ? ctx.mAL0Tail : ctx.mL0;
        ctx.currentML0Align = isML0Tail ? AlignB(currentML0, BLOCK_L0_M) : currentML0;
        return currentML0;
    }

    __aicore__ inline uint64_t CalcCurrentML0HWMode()
    {
        uint64_t currentML0 = ctx.currentHoL0 * ctx.currentWoL0;
        if (ctx.currentWoL0 == ctx.convTiling->woL0 &&
            ctx.currentHoL0 == ctx.convTiling->hoL0) {
            ctx.currentML0Align = ctx.convTiling->mStep;
        } else {
            ctx.currentML0Align = AlignB(currentML0, BLOCK_L0_N);
        }
        return currentML0;
    }

    __aicore__ inline void LoadAL1BaseMoudle()
    {
        ctx.fmapL1 = ctx.queueFmapL1.template AllocTensor<FmapT>();
        loadAl1Ins.LoadAL1(ctx);
        ctx.mAL1UpdateFlag = false;
        ctx.queueFmapL1.EnQue(ctx.fmapL1);
        ctx.loadAL1Flag = false;  // Only k directrion can be reloaded in LoopK
    }

    __aicore__ inline void LoadAL1BaseMoudle(Iters& iters)
    {
        ctx.fmapL1 = ctx.queueFmapL1.template AllocTensor<FmapT>();
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            loadAl1Ins.LoadAL1(ctx, iters.kAL1Iter, iters.mAL1Iter, iters.batchIter);
        } else {
            loadAl1Ins.LoadAL1(ctx, iters.kAL1Iter, iters.woAL1Iter, iters.hoAL1Iter, iters.batchIter);
        }
        ctx.queueFmapL1.EnQue(ctx.fmapL1);
        ctx.loadAL1Flag = false;  // Only k directrion can be reloaded in LoopK
    }

    __aicore__ inline void LoadBL1BaseMoudle()
    {
        ctx.weightL1 = ctx.queueWeightL1.template AllocTensor<WeightT>();
        if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            CrossCoreWaitFlag<CV_ENHANCE_MODE, PIPE_MTE1>(ctx.vecId + CV_SYNC_ID_MTE3_MTE1);
            ctx.loadUB2L1Iter++;
        } else {
            loadBL1Ins.LoadBL1(ctx);
        }
        ctx.queueWeightL1.EnQue(ctx.weightL1);
        ctx.loadBL1Flag = false;  // Only k directrion can be reloaded in LoopK
    }

    __aicore__ inline void LoadBL1BaseMoudle(Iters& iters)
    {
        ctx.weightL1 = ctx.queueWeightL1.template AllocTensor<WeightT>();
        loadBL1Ins.LoadBL1(ctx, iters.kBL1Iter, iters.nBL1Iter);
        ctx.queueWeightL1.EnQue(ctx.weightL1);
        ctx.loadBL1Flag = false;  // Only k directrion can be reloaded in LoopK
    }

    __aicore__ inline void LoadAL1Moudle()
    {
        ctx.kAL1Iter = ctx.kIter / ctx.multiKAL1;
        LoadAL1BaseMoudle();
    }

    __aicore__ inline void LoadBL1Moudle()
    {
        ctx.kBL1Iter = ctx.kIter / ctx.multiKBL1;
        LoadBL1BaseMoudle();
    }

    __aicore__ inline void ReduceKCloseL0PingPong(bool isFirst)
    {
        printf("isFirst: %d, iter: %d\n", isFirst, ctx.iter);
        if (ctx.iter >= 1) {
            WaitFlag<HardEvent::M_MTE1>(static_cast<TEventID>(0x0));
        }
        if (!((ctx.convTiling->kAL1 == ctx.convTiling->kL0) && (!ctx.loadAL0Flag))) {
            ctx.al0 = ctx.wholeAl0Tensor;
            ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
            loadAL0Ins.LoadAL0(ctx, isFirst);
        }
        if (!((ctx.convTiling->kBL1 == ctx.convTiling->kL0) && (!ctx.loadBL0Flag))) {
            ctx.bl0 = ctx.wholeBl0Tensor;
            ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
            loadBL0Ins.LoadBL0(ctx, isFirst);
            // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            //     OptGroupSyncSet();
            // }
        }
        SetFlag<HardEvent::MTE1_M>(static_cast<TEventID>(0x0));
        WaitFlag<HardEvent::MTE1_M>(static_cast<TEventID>(0x0));
        madIns.Mad(ctx);
        SetFlag<HardEvent::M_MTE1>(static_cast<TEventID>(0x0));
        ctx.kIter++;
    }

    __aicore__ inline void ReduceKOpenL0APingPong(uint16_t& al0PingPongFlag, bool isFirst)
    {
        if (ctx.iter >= DOUBLE_BUF) {
            WaitFlag<HardEvent::M_MTE1>(static_cast<TEventID>(al0PingPongFlag & 0x1));
        }
        ctx.al0 = ctx.wholeAl0Tensor[(al0PingPongFlag & 0x1) * L0A_HALF_SIZE / sizeOfFmap];
        ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
        loadAL0Ins.LoadAL0(ctx, isFirst);

        if (!((ctx.convTiling->kBL1 == ctx.convTiling->kL0) && (!ctx.loadBL0Flag))) {
            ctx.bl0 = ctx.wholeBl0Tensor;
            ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
            loadBL0Ins.LoadBL0(ctx, isFirst);
            // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            //     OptGroupSyncSet();
            // }
        }
        SetFlag<HardEvent::MTE1_M>(static_cast<TEventID>(al0PingPongFlag & 0x1));
        WaitFlag<HardEvent::MTE1_M>(static_cast<TEventID>(al0PingPongFlag & 0x1));
        madIns.Mad(ctx);
        SetFlag<HardEvent::M_MTE1>(static_cast<TEventID>(al0PingPongFlag & 0x1));
        al0PingPongFlag += 1;
        ctx.kIter++;
    }

    __aicore__ inline void ReduceKOpenL0BPingPong(uint16_t& bl0PingPongFlag, bool isFirst)
    {
        printf("bl0PingPongFlag: %u, isFirst: %d, iter: %d\n", bl0PingPongFlag, isFirst, ctx.iter);
        if (ctx.iter >= DOUBLE_BUF) {
            WaitFlag<HardEvent::M_MTE1>(static_cast<TEventID>(bl0PingPongFlag & 0x1));
        }
        if (!((ctx.convTiling->kAL1 == ctx.convTiling->kL0) && (!ctx.loadAL0Flag))) {
            ctx.al0 = ctx.wholeAl0Tensor;
            ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
            loadAL0Ins.LoadAL0(ctx, isFirst);
        }

        ctx.bl0 = ctx.wholeBl0Tensor[(bl0PingPongFlag & 0x1) * L0B_HALF_SIZE / sizeOfWeight];
        ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
        loadBL0Ins.LoadBL0(ctx, isFirst);
        // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
        //     OptGroupSyncSet();
        // }
        SetFlag<HardEvent::MTE1_M>(static_cast<TEventID>(bl0PingPongFlag & 0x1));
        WaitFlag<HardEvent::MTE1_M>(static_cast<TEventID>(bl0PingPongFlag & 0x1));
        madIns.Mad(ctx);
        SetFlag<HardEvent::M_MTE1>(static_cast<TEventID>(bl0PingPongFlag & 0x1));
        bl0PingPongFlag += 1;
        ctx.kIter++;
    }

    __aicore__ inline void ReduceKOpenL0AL0BPingPong(uint16_t& al0PingPongFlag, bool isFirst)
    {
        if (ctx.iter >= DOUBLE_BUF) {
            WaitFlag<HardEvent::M_MTE1>(static_cast<TEventID>(al0PingPongFlag & 0x1));
        }

        ctx.al0 =
            ctx.wholeAl0Tensor[(al0PingPongFlag & 0x1) * L0A_HALF_SIZE / sizeOfFmap];
        ctx.bl0 =
            ctx.wholeBl0Tensor[(al0PingPongFlag & 0x1) * L0B_HALF_SIZE / sizeOfWeight];

        ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
        loadAL0Ins.LoadAL0(ctx, isFirst);

        ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
        loadBL0Ins.LoadBL0(ctx, isFirst);
        // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
        //     OptGroupSyncSet();
        // }

        SetFlag<HardEvent::MTE1_M>(static_cast<TEventID>(al0PingPongFlag & 0x1));
        WaitFlag<HardEvent::MTE1_M>(static_cast<TEventID>(al0PingPongFlag & 0x1));
        madIns.Mad(ctx);
        SetFlag<HardEvent::M_MTE1>(static_cast<TEventID>(al0PingPongFlag & 0x1));
        al0PingPongFlag += 1;
        ctx.kIter++;
    }

    __aicore__ inline void LoadL0Moudle(uint16_t &al0PingPongFlag, uint16_t &bl0PingPongFlag, bool isFirst)
    {
        if constexpr (l0PingPong == static_cast<int32_t>(ConvL0PingPong::ALL_CLOSE)) {
            ReduceKCloseL0PingPong(isFirst);
        } else if constexpr (l0PingPong == static_cast<int32_t>(ConvL0PingPong::L0A_OPEN)) {
            ReduceKOpenL0APingPong(al0PingPongFlag, isFirst);
        } else if constexpr (l0PingPong == static_cast<int32_t>(ConvL0PingPong::L0B_OPEN)) {
            ReduceKOpenL0BPingPong(bl0PingPongFlag, isFirst);
        } else if constexpr (l0PingPong == static_cast<int32_t>(ConvL0PingPong::ALL_OPEN)) {
            ReduceKOpenL0AL0BPingPong(al0PingPongFlag, isFirst);
        }
        ctx.iter++;
    }

    __aicore__ inline void FirstIterateImplMMode()
    {
        ctx.batchIter = 0;
        ctx.mAL1Iter = 0;
        ctx.nBL1Iter = 0;
        ctx.mL0Iter = 0;
        ctx.nL0Iter = 0;

        ctx.loadAL1Flag = true;
        ctx.loadBL1Flag = true;
        ctx.loadAL0Flag = true;
        ctx.loadBL0Flag = true;

        ctx.isFirstIterate = false;

        // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
        //     ctx.loadUB2L1Iter = 0;
        // }

        // [zj]TODO: check if need
        CalcMDirectionVar();
        CalcCoDirectionVar();
    }

    __aicore__ inline bool IterateL0MFirstMMode()
    {
        ctx.mL0Iter++;
        if (ctx.mL0Iter == ctx.l12l0LoopM) {
            ctx.mL0Iter = 0;
        } else {
            ctx.loadBL0Flag = false;
            return true;
        }

        if constexpr (kl0FullLoadFlag) {
            ctx.kL0FullLoadAl0PingPongFlag = 0;
        }

        if constexpr (hasNL0IterFlag) {
            ctx.nL0Iter++;
            ctx.loadBL0Flag = true;
            if (ctx.nL0Iter == ctx.l12l0LoopN) {
                ctx.nL0Iter = 0;
            } else {
                return true;
            }
        }

        return false;
    }

    __aicore__ inline bool IterateMFirstMMode()
    {
        if (IterateL0MFirstMMode()) {
            return true;
        }

        if (ctx.kAL1fullload) {
            ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
        }

        ctx.mAL1Iter++;
        ctx.loadAL1Flag = true;
        if (ctx.mAL1Iter == ctx.ddr2l1LoopM) {
            ctx.mAL1Iter = 0;
            CalcMDirectionVar();
        } else {
            CalcMDirectionVar();
            return true;
        }

        ctx.batchIter++;
        if (ctx.batchIter == ctx.ddr2l1LoopBatch) {
            ctx.batchIter = 0;
        } else {
            ctx.loadAL1Flag = true;
            ctx.loadBL1Flag = false;
            return true;
        }

        if (ctx.kBL1fullload) {
            ctx.queueWeightL1.FreeTensor(ctx.weightL1);
        }

        if constexpr (hasNL1IterFlag) {
            ctx.nBL1Iter++;
            CalcCoDirectionVar();
            ctx.loadBL1Flag = true;
            if (ctx.nBL1Iter != ctx.ddr2l1LoopN) {
                return true;
            }
        }

        return false;
    }

    __aicore__ inline bool IterateL0NFirstMMode()
    {
        if constexpr (hasNL0IterFlag) {
            ctx.nL0Iter++;
            if (ctx.nL0Iter == ctx.l12l0LoopN) {
                ctx.nL0Iter = 0;
            } else {
                ctx.loadAL0Flag = false;
                return true;
            }
        }

        if constexpr (kl0FullLoadFlag) {
            ctx.kL0FullLoadBl0PingPongFlag = 0;
        }

        ctx.mL0Iter++;
        ctx.loadAL0Flag = true;
        if (ctx.mL0Iter == ctx.l12l0LoopM) {
            ctx.mL0Iter = 0;
        } else {
            return true;
        }

        return false;
    }

    __aicore__ inline bool IterateNFirstMMode()
    {
        if (IterateL0NFirstMMode()) {
            return true;
        }

        // [zj]TODO: check
        // if (ctx.kBL1fullload) {
        //     ctx.queueWeightL1.FreeTensor(ctx.weightL1);
        // }

        if constexpr (hasNL1IterFlag) {
            ctx.nBL1Iter++;
            ctx.loadBL1Flag = true;
            if (ctx.nBL1Iter == ctx.ddr2l1LoopN) {
                ctx.nBL1Iter = 0;
                CalcCoDirectionVar();
                
            } else {
                CalcCoDirectionVar();
                return true;
            }
        }

        if (ctx.kAL1fullload) {
            ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
        }

        ctx.mAL1Iter++;
        ctx.loadAL1Flag = true;
        if (ctx.mAL1Iter == ctx.ddr2l1LoopM) {
            ctx.mAL1Iter = 0;
            CalcMDirectionVar();
        } else {
            CalcMDirectionVar();
            return true;
        }

        ctx.batchIter++;
        if (likely(ctx.batchIter != ctx.ddr2l1LoopBatch)) {
            ctx.loadAL1Flag = true;
            ctx.loadBL1Flag = true;
            return true;
        }

        return false;
    }

    __aicore__ inline bool UpdateCommonItersMModeMFirst(Iters& iters)
    {
        iters.mAL1Iter = ctx.mAL1Iter;
        iters.batchIter = ctx.batchIter;
        iters.nBL1Iter = ctx.nBL1Iter;

        iters.mAL1Iter += 1;
        if (iters.mAL1Iter <= ctx.maxMAL1Iter) {
            return false;
        }
        iters.mAL1Iter = 0;
        iters.batchIter += 1;
        if (iters.batchIter < ctx.ddr2l1LoopBatch) {
            return false;
        }
        iters.batchIter = 0;
        iters.nBL1Iter += 1;
        if (iters.nBL1Iter <= ctx.maxNBL1Iter) {
            return false;
        }
        iters.endTag = true;
        return true;
    }

    __aicore__ inline bool UpdateCommonItersMModeNFirst(Iters& iters)
    {
        iters.nBL1Iter = ctx.nBL1Iter;
        iters.mAL1Iter = ctx.mAL1Iter;
        iters.batchIter = ctx.batchIter;

        iters.nBL1Iter += 1;
        if (iters.nBL1Iter <= ctx.maxNBL1Iter) {
            return false;
        }
        iters.nBL1Iter = 0;
        iters.mAL1Iter += 1;
        if (iters.mAL1Iter <= ctx.maxMAL1Iter) {
            return false;
        }
        iters.mAL1Iter = 0;
        iters.batchIter += 1;
        if (iters.batchIter < ctx.ddr2l1LoopBatch) {
            return false;
        }
        iters.endTag = true;
        return true;
    }

    __aicore__ inline void FirstIterateImplHWMode()
    {
        ctx.nL0Iter = 0;
        ctx.hoAL1Iter = 0;
        ctx.woAL1Iter = 0;
        ctx.nBL1Iter = 0;
        ctx.woL0Iter = 0;
        ctx.hoL0Iter = 0;
        ctx.batchIter = 0;
        ctx.loadAL1Flag = true;
        ctx.loadBL1Flag = true;
        ctx.loadAL0Flag = true;
        ctx.loadBL0Flag = true;
        ctx.isFirstIterate = false;

        if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            ctx.loadUB2L1Iter = 0;
        }

        CalcWoDirectionVar();
        CalcHoDirectionVar();
        CalcCoDirectionVar();
    }

    __aicore__ inline bool IterateHWL1()
    {
        if constexpr (hasWL1IterFlag) {
            ctx.woAL1Iter++;
            ctx.loadAL1Flag = true;
            if (ctx.woAL1Iter == ctx.ddr2l1LoopW) {
                ctx.woAL1Iter = 0;
                CalcWoDirectionVar();
            } else {
                CalcWoDirectionVar();
                if constexpr (iterateMFirstFlag && !hasNL0IterFlag) {
                    ctx.loadBL0Flag = false;
                }
                return true;
            }
        }

        if constexpr (hasHL1IterFlag) {
            ctx.hoAL1Iter++;
            if (ctx.hoAL1Iter == ctx.ddr2l1LoopH) {
                ctx.hoAL1Iter = 0;
                CalcHoDirectionVar();
            } else {
                CalcHoDirectionVar();
                if constexpr (iterateMFirstFlag && !hasNL0IterFlag) {
                    ctx.loadBL0Flag = false;
                }
                return true;
            }
        }

        return false;
    }

    __aicore__ inline bool IterateL0MFirstHWMode()
    {
        if constexpr (hasWL0IterFlag) {
            ctx.woL0Iter++;
            if (ctx.woL0Iter == ctx.l12l0LoopW) {
                ctx.woL0Iter = 0;
            } else {
                ctx.loadBL0Flag = false;
                return true;
            }
        }

        if constexpr (hasHL0IterFlag) {
            ctx.hoL0Iter++;
            if (ctx.hoL0Iter == ctx.l12l0LoopH) {
                ctx.hoL0Iter = 0;
            } else {
                ctx.loadBL0Flag = false;
                return true;
            }
        }

        if constexpr (kl0FullLoadFlag) {
            ctx.kL0FullLoadAl0PingPongFlag = 0;
        }

        if constexpr (hasNL0IterFlag) {
            ctx.nL0Iter++;
            ctx.loadBL0Flag = true;
            if (ctx.nL0Iter == ctx.l12l0LoopN) {
                ctx.nL0Iter = 0;
            } else {
                return true;
            }
        }

        return false;
    }

    __aicore__ inline bool IterateMFirstHWMode()
    {
        if (IterateL0MFirstHWMode()) {
            return true;
        }

        if (ctx.kAL1fullload) {
            ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
        }

        if (IterateHWL1()) {
            return true;
        }

        ctx.batchIter++;
        if (ctx.batchIter == ctx.ddr2l1LoopBatch) {
            ctx.batchIter = 0;
        } else {
            ctx.loadAL1Flag = true;
            ctx.loadBL1Flag = false;
            return true;
        }

        if (ctx.kBL1fullload) {
            ctx.queueWeightL1.FreeTensor(ctx.weightL1);
        }

        if constexpr (hasNL1IterFlag) {
            ctx.nBL1Iter++;
            CalcCoDirectionVar();
            ctx.loadBL1Flag = true;
            if (likely(ctx.nBL1Iter != ctx.ddr2l1LoopN)) {
                return true;
            }
        }

        return false;
    }

    __aicore__ inline bool IterateL0NFirstHWMode()
    {
        if constexpr (hasNL0IterFlag) {
            ctx.nL0Iter++;
            if (ctx.nL0Iter == ctx.l12l0LoopN) {
                ctx.nL0Iter = 0;
            } else {
                ctx.loadAL0Flag = false;
                return true;
            }
        }
        if constexpr (kl0FullLoadFlag) {
            ctx.kL0FullLoadBl0PingPongFlag = 0;
        }
        if constexpr (hasWL0IterFlag) {
            ctx.woL0Iter++;
            ctx.loadAL0Flag = true;
            if (ctx.woL0Iter == ctx.l12l0LoopW) {
                ctx.woL0Iter = 0;
            } else {
                return true;
            }
        }
        if constexpr (hasHL0IterFlag) {
            ctx.hoL0Iter++;
            ctx.loadAL0Flag = true;
            if (ctx.hoL0Iter == ctx.l12l0LoopH) {
                ctx.hoL0Iter = 0;
            } else {
                return true;
            }
        }

        return false;
    }

    __aicore__ inline bool IterateNFirstHWMode()
    {
        if (IterateL0NFirstHWMode()) {
            return true;
        }

        if (ctx.kBL1fullload) {
            ctx.queueWeightL1.FreeTensor(ctx.weightL1);
        }

        if constexpr (hasNL1IterFlag) {
            ctx.nBL1Iter++;
            ctx.loadBL1Flag = true;
            if (ctx.nBL1Iter == ctx.ddr2l1LoopN) {
                ctx.nBL1Iter = 0;
                CalcCoDirectionVar();
            } else {
                CalcCoDirectionVar();
                return true;
            }
        }

        if (ctx.kAL1fullload) {
            ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
        }

        if (IterateHWL1()) {
            return true;
        }

        ctx.batchIter++;
        if (likely(ctx.batchIter != ctx.ddr2l1LoopBatch)) {
            ctx.loadAL1Flag = true;
            ctx.loadBL1Flag = true;
            return true;
        }
        return false;
    }

    __aicore__ inline bool UpdateCommonItersHWModeMFirst(Iters& iters)
    {
        iters.woAL1Iter = ctx.woAL1Iter;
        iters.hoAL1Iter = ctx.hoAL1Iter;
        iters.batchIter = ctx.batchIter;
        iters.nBL1Iter = ctx.nBL1Iter;

        iters.woAL1Iter += 1;
        if (iters.woAL1Iter <= ctx.maxWoL1Iter) {
            return false;
        }
        iters.woAL1Iter = 0;
        iters.hoAL1Iter += 1;
        if (iters.hoAL1Iter <= ctx.maxHoL1Iter) {
            return false;
        }
        iters.hoAL1Iter = 0;
        iters.batchIter += 1;
        if (iters.batchIter < ctx.ddr2l1LoopBatch) {
            return false;
        }
        iters.batchIter = 0;
        iters.nBL1Iter += 1;
        if (iters.nBL1Iter <= ctx.maxNBL1Iter) {
            return false;
        }
        iters.endTag = true;
        return true;
    }

    __aicore__ inline bool UpdateCommonItersHWModeNFirst(Iters& iters)
    {
        iters.nBL1Iter = ctx.nBL1Iter;
        iters.woAL1Iter = ctx.woAL1Iter;
        iters.hoAL1Iter = ctx.hoAL1Iter;
        iters.batchIter = ctx.batchIter;

        iters.nBL1Iter += 1;
        if (iters.nBL1Iter <= ctx.maxNBL1Iter) {
            return false;
        }
        iters.nBL1Iter = 0;
        iters.woAL1Iter = ctx.woAL1Iter + 1;
        if (iters.woAL1Iter <= ctx.maxWoL1Iter) {
            return false;
        }
        iters.woAL1Iter = 0;
        iters.hoAL1Iter += 1;
        if (iters.hoAL1Iter <= ctx.maxHoL1Iter) {
            return false;
        }
        iters.hoAL1Iter = 0;
        iters.batchIter += 1;
        if (iters.batchIter < ctx.ddr2l1LoopBatch) {
            return false;
        }
        iters.endTag = true;
        return true;
    }

    __aicore__ inline bool UpdateCommonIters(Iters& iters)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
            iterateMFirstFlag) {
            return UpdateCommonItersMModeMFirst(iters);
        } else if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
            iterateNFirstFlag) {
            return UpdateCommonItersMModeNFirst(iters);
        } else if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
            iterateMFirstFlag) {
            return UpdateCommonItersHWModeMFirst(iters);
        } else if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
            iterateNFirstFlag) {
            return UpdateCommonItersHWModeNFirst(iters);
        }
    }

    __aicore__ inline bool UpdateItersByFmap(Iters& iters, bool updateIterByFmapTag)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
            iters.woAL1Iter = ctx.woAL1Iter;
            iters.hoAL1Iter = ctx.hoAL1Iter;
        } else {
            iters.mAL1Iter = ctx.mAL1Iter;
        }
        iters.batchIter = ctx.batchIter;
        ctx.kAL1Iter = ctx.kIter / ctx.multiKAL1;
        printf("kAL1Iter: %ld\n", ctx.kAL1Iter);
        iters.kAL1Iter = ctx.kAL1Iter + 1;
        if (iters.kAL1Iter <= ctx.maxKAL1Iter) {
            printf("kAL1Iter: %ld, maxKAL1Iter: %ld\n", iters.kAL1Iter, ctx.maxKAL1Iter);
            return false;
        }
        iters.kAL1Iter = 0;
        if (updateIterByFmapTag) {
            return UpdateCommonIters(iters);
        }
        if (iters.endTag) {
            return true;
        }
        return false;
    }

    __aicore__ inline bool UpdateItersByWeight(Iters& iters, bool updateIterByFmapTag)
    {
        iters.nBL1Iter = ctx.nBL1Iter;
        ctx.kBL1Iter = ctx.kIter / ctx.multiKBL1;
        iters.kBL1Iter = ctx.kBL1Iter + 1;
        if (iters.kBL1Iter <= ctx.maxKBL1Iter) {
            return false;
        }
        iters.kBL1Iter = 0;
        if (!updateIterByFmapTag) {
            return UpdateCommonIters(iters);
        }
        if (iters.endTag) {
            return true;
        }
        return false;
    }

    __aicore__ inline void UpdateHoL0WoL0()
    {
        ctx.currentHoL0 = ctx.hoL0Iter == ctx.maxHoL0Iter ? ctx.hoL0Tail :
                                                            ctx.convTiling->hoL0;
        ctx.currentWoL0 = ctx.woL0Iter == ctx.maxWoL0Iter ? ctx.woL0Tail :
                                                            ctx.convTiling->woL0;
    }

    __aicore__ inline void SetMNBeforeIterateK()
    {
        uint64_t currentML0 = 0;
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            currentML0 = CalcCurrentML0MMode();
        } else {
            currentML0 = CalcCurrentML0HWMode();
        }
        printf("currentML0: %ld\n", currentML0);

        uint64_t currentNL0 = CalcCurrentNL0();
        printf("currentNL0: %ld\n", currentNL0);

        loadAL0Ins.SetM(ctx, ctx.currentML0Align, currentML0);
        loadBL0Ins.SetN(ctx, ctx.currentNL0Align);
        loadBiasL1Ins.SetN(ctx, currentNL0);
        loadPreQuantL1Ins.SetN(ctx, currentNL0);
        loadPreReluL1Ins.SetN(ctx, currentNL0);
        loadPreClipReluL1Ins.SetN(ctx, currentNL0);
        loadAntiQuantL1Ins.SetN(ctx, currentNL0);
        loadEltwiseL1Ins.SetN(ctx, currentNL0);
        loadEltwiseNonBrcL1Ins.SetMN(ctx, currentML0, ctx.currentNL0Align);
        loadPostQuantL1Ins.SetN(ctx, currentNL0);
        loadPostReluL1Ins.SetN(ctx, currentNL0);
        loadPostClipReluL1Ins.SetN(ctx, currentNL0);
        loadBiasBTIns.SetN(ctx, ctx.currentNL0Align);
        loadPreQuantFBIns.SetN(ctx, ctx.currentNL0Align);
        loadPreReluFBIns.SetN(ctx, ctx.currentNL0Align);
        loadPreClipReluFBIns.SetN(ctx, ctx.currentNL0Align);
        loadAntiQuantFBIns.SetN(ctx, ctx.currentNL0Align);
        loadPostQuantFBIns.SetN(ctx, ctx.currentNL0Align);
        loadPostReluFBIns.SetN(ctx, ctx.currentNL0Align);
        loadPostClipReluFBIns.SetN(ctx, ctx.currentNL0Align);
        madIns.SetMN(ctx, ctx.currentML0Align, currentML0, ctx.currentNL0Align);
        if constexpr (formatOutput == ConvFormat::NC1HWC0) {
            copyOutIns.SetMN(ctx, currentML0, ctx.currentNL0Align);
        } else {
            copyOutIns.SetMN(ctx, currentML0, currentNL0);
        }
    }

    __aicore__ inline void ReduceOneK()
    {
        if (unlikely(ctx.loadBL1Flag)) {
            LoadBL1Moudle();
            ctx.weightL1 = ctx.queueWeightL1.template DeQue<WeightT>();
        }
        // [zj]TODO: preload need check
        if (mPreLoadAFlag) {
            if (unlikely(ctx.loadAL1Flag)) {
                printf("iterate fmap preload setspr\n");
                loadAl1Ins.SetSpr(ctx);
                ctx.mAL1UpdateFlag = false;
                printf("iterate fmap preload deque\n");
                LocalTensor<FmapT> fmapL1 = ctx.queueFmapL1.template DeQue<FmapT>();
                printf(
                    "[0]kAL1Iter: %ld, mAL1Iter: %ld, batchIter: %ld,\n[0]kAL1Iter: %ld, hoAL1Iter: %ld, woAL1Iter: "
                    "%ld, batchIter: %ld\n",
                    ctx.kAL1Iter, ctx.mAL1Iter, ctx.batchIter, ctx.kAL1Iter, ctx.hoAL1Iter, ctx.woAL1Iter,
                    ctx.batchIter);
                Iters iters;
                if (!UpdateItersByFmap(iters, true)) {
                    printf("iterate fmap preload\n");
                    printf(
                        "[1]kAL1Iter: %ld, mAL1Iter: %ld, batchIter: %ld,\n[1]kAL1Iter: %ld, hoAL1Iter: %ld, "
                        "woAL1Iter: %ld, batchIter: %ld\n",
                        iters.kAL1Iter, iters.mAL1Iter, iters.batchIter, iters.kAL1Iter, iters.hoAL1Iter,
                        iters.woAL1Iter, iters.batchIter);
                    if ((outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
                         (iters.kAL1Iter != ctx.kAL1Iter || iters.mAL1Iter != ctx.mAL1Iter ||
                          iters.batchIter != ctx.batchIter)) ||
                        (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
                         (iters.hoAL1Iter != ctx.hoAL1Iter || iters.woAL1Iter != ctx.woAL1Iter ||
                          iters.mAL1Iter != ctx.mAL1Iter || iters.batchIter != ctx.batchIter))) {
                        bool mAL1UpdateFlag = ctx.mAL1UpdateFlag;
                        ctx.mAL1UpdateFlag = true;
                        LoadAL1BaseMoudle(iters);
                        ctx.mAL1UpdateFlag = mAL1UpdateFlag;
                    }
                }
                ctx.fmapL1 = fmapL1;
                ctx.loadAL1Flag = false;
            }
        } else {
            if (unlikely(ctx.loadAL1Flag)) {
                LoadAL1Moudle();
                ctx.fmapL1 = ctx.queueFmapL1.template DeQue<FmapT>();
            }
        }

        LoadL0Moudle(ctx.kL0FullLoadAl0PingPongFlag, ctx.kL0FullLoadBl0PingPongFlag, true);
    }

    __aicore__ inline void ReduceKPreload()
    {
        // state
        uint16_t al0PingPongFlag = 0;
        uint16_t bl0PingPongFlag = 0;
        bool isFirst = true;

        Iters iters;
        // updateIterByFmapTag is true means fm update; false means weight update
        bool updateIterByFmapTag = ctx.convTiling->kAL1 > ctx.convTiling->kBL1;
        if (ctx.kAL1fullload && !ctx.kBL1fullload) {
            updateIterByFmapTag = false;
        } else if (!ctx.kAL1fullload && ctx.kBL1fullload) {
            updateIterByFmapTag = true;
        }

        if (ctx.loadAL1Flag || !ctx.kAL1fullload) {
            ctx.kAL1Iter = 0;
            LoadAL1BaseMoudle();
            if (ctx.kAL1fullload) {
                ctx.fmapL1 = ctx.queueFmapL1.template DeQue<FmapT>();
                ctx.loadAL0Flag = true;
            }
        }

        if (ctx.loadBL1Flag || !ctx.kBL1fullload) {
            ctx.kBL1Iter = 0;
            LoadBL1BaseMoudle();
            if (ctx.kBL1fullload) {
                ctx.weightL1 = ctx.queueWeightL1.template DeQue<WeightT>();
                ctx.loadBL0Flag = true;
            }
        }

        while (ctx.kIter < ctx.ddr2l0LoopK) {
            if (ctx.loadAL1Flag || !ctx.kAL1fullload) {
                if (ctx.kIter % ctx.multiKAL1 == 0) {
                    loadAl1Ins.SetSpr(ctx);
                    ctx.mAL1UpdateFlag = false;
                }
                if (ctx.kIter % ctx.multiKAL1 == 0 && ctx.kIter < ctx.lastLoopKAL1StartPos) {
                    if (ctx.kIter != 0) {
                        ctx.queueFmapL1.FreeTensor(ctx.fmapL1);  // always freeTensor
                    }
                    if (!UpdateItersByFmap(iters, updateIterByFmapTag)) {
                        LoadAL1BaseMoudle(iters);
                    }
                }
                if (ctx.kIter == ctx.lastLoopKAL1StartPos) {
                    ctx.queueFmapL1.FreeTensor(ctx.fmapL1); // last iter of multiKAL1 only Free AL1 Tensor
                }
                if (ctx.kIter % ctx.multiKAL1 == 0) {
                    ctx.fmapL1 = ctx.queueFmapL1.template DeQue<FmapT>();
                    ctx.loadAL0Flag = true;
                }
            }

            if (ctx.loadBL1Flag || !ctx.kBL1fullload) {
                if (ctx.kIter % ctx.multiKBL1 == 0 && ctx.kIter < ctx.lastLoopKBL1StartPos) {
                    if (ctx.kIter != 0) {
                        ctx.queueWeightL1.FreeTensor(ctx.weightL1); // always freeTensor
                    }
                    if (!UpdateItersByWeight(iters, updateIterByFmapTag)) {
                        LoadBL1BaseMoudle(iters);
                    }
                }
                if (ctx.kIter == ctx.lastLoopKBL1StartPos) {
                    ctx.queueWeightL1.FreeTensor(ctx.weightL1); // last iter of multiKBL1 only Free BL1 Tensor
                }
                if (ctx.kIter % ctx.multiKBL1 == 0) {
                    ctx.weightL1 = ctx.queueWeightL1.template DeQue<WeightT>();
                    ctx.loadBL0Flag = true;
                }
            }

            LoadL0Moudle(al0PingPongFlag, bl0PingPongFlag, isFirst);
            isFirst = false;
        }
    }

    __aicore__ inline void ReduceK()
    {
        // state
        uint16_t al0PingPongFlag = 0;
        uint16_t bl0PingPongFlag = 0;
        bool isFirst = true;

        while (ctx.kIter < ctx.ddr2l0LoopK) {
            if (ctx.loadAL1Flag || (!ctx.kAL1fullload && ctx.kIter % ctx.multiKAL1 == 0)) {
                if (ctx.kIter != 0) {
                    ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
                }
                LoadAL1Moudle();
                ctx.fmapL1 = ctx.queueFmapL1.template DeQue<FmapT>();
                ctx.loadAL0Flag = true;
            }
            if (ctx.loadBL1Flag || (!ctx.kBL1fullload && ctx.kIter % ctx.multiKBL1 == 0)) {
                if (ctx.kIter != 0) {
                    ctx.queueWeightL1.FreeTensor(ctx.weightL1);
                }
                LoadBL1Moudle();
                ctx.weightL1 = ctx.queueWeightL1.template DeQue<WeightT>();
                ctx.loadBL0Flag = true;
            }

            LoadL0Moudle(al0PingPongFlag, bl0PingPongFlag, isFirst);
            isFirst = false;
        }
    }

    __aicore__ inline void FreeL1Tensor()
    {
        if (!ctx.kAL1fullload) {
            ctx.queueFmapL1.FreeTensor(ctx.fmapL1);
        }
        if (!ctx.kBL1fullload) {
            ctx.queueWeightL1.FreeTensor(ctx.weightL1);
        }
        if (ctx.hasBias) {
            if (!ctx.convTiling->biasFullLoadFlag) {
                ctx.queueBiasL1.FreeTensor(ctx.biasL1);
            }
            ctx.queueBiasBT.FreeTensor(ctx.biasBT);
        }
        if (ctx.hasVectorPreQuant) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePreQuantL1.FreeTensor(ctx.preQuantL1);
            }
            ctx.queuePreQuantFB.FreeTensor(ctx.preQuantFB);
        }
        if (ctx.hasVectorPreRelu) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePreReluL1.FreeTensor(ctx.preReluL1);
            }
            ctx.queuePreReluFB.FreeTensor(ctx.preReluFB);
        }
        if (ctx.hasVectorPreClipRelu) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePreClipReluL1.FreeTensor(ctx.preClipReluL1);
            }
            ctx.queuePreClipReluFB.FreeTensor(ctx.preClipReluFB);
        }
        if (ctx.hasVectorAntiQuant) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueAntiQuantL1.FreeTensor(ctx.antiQuantL1);
            }
            ctx.queueAntiQuantFB.FreeTensor(ctx.antiQuantFB);
        }
        if (ctx.hasEltwise && ctx.eltwiseBrcEn) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueEltwiseL1.FreeTensor(ctx.eltwiseL1);
            }
        }
        if (ctx.hasVectorPostQuant) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePostQuantL1.FreeTensor(ctx.postQuantL1);
            }
            ctx.queuePostQuantFB.FreeTensor(ctx.postQuantFB);
        }
        if (ctx.hasVectorPostRelu) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePostReluL1.FreeTensor(ctx.postReluL1);
            }
            ctx.queuePostReluFB.FreeTensor(ctx.postReluFB);
        }
        if (ctx.hasVectorPostClipRelu) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queuePostClipReluL1.FreeTensor(ctx.postClipReluL1);
            }
            ctx.queuePostClipReluFB.FreeTensor(ctx.postClipReluFB);
        }
    }

    __aicore__ inline void IterateK()
    {
        SetMNBeforeIterateK();

        if (ctx.hasBias) {
            if (!ctx.convTiling->biasFullLoadFlag) {
                ctx.biasL1 = ctx.queueBiasL1.template AllocTensor<BiasT>();
                loadBiasL1Ins.LoadChannelWiseL1(ctx, ctx.biasL1, ctx.biasGm);
                ctx.queueBiasL1.EnQue(ctx.biasL1);
                ctx.biasL1 = ctx.queueBiasL1.template DeQue<BiasT>();
            }
            ctx.biasBT = ctx.queueBiasBT.template AllocTensor<L0cT>();
            loadBiasBTIns.LoadBiasBt(ctx);
            ctx.queueBiasBT.EnQue(ctx.biasBT);
            ctx.biasBT = ctx.queueBiasBT.template DeQue<L0cT>();
        }
        if (ctx.hasVectorPreQuant) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.preQuantL1 = ctx.queuePreQuantL1.template AllocTensor<PreQuantT>();
                loadPreQuantL1Ins.LoadChannelWiseL1(ctx, ctx.preQuantL1, ctx.preQuantGm);
                ctx.queuePreQuantL1.EnQue(ctx.preQuantL1);
                ctx.preQuantL1 = ctx.queuePreQuantL1.template DeQue<PreQuantT>();
            }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
            ctx.preQuantFB = ctx.queuePreQuantFB.template AllocTensor<PreQuantT>();
            loadPreQuantFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_PRE_QUANT, ctx.preQuantFB, ctx.preQuantL1);
            ctx.queuePreQuantFB.EnQue(ctx.preQuantFB);
            ctx.preQuantFB = ctx.queuePreQuantFB.template DeQue<PreQuantT>();
#endif
        }
        if (ctx.hasVectorPreRelu) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.preReluL1 = ctx.queuePreReluL1.template AllocTensor<PreReluT>();
                loadPreReluL1Ins.LoadChannelWiseL1(ctx, ctx.preReluL1, ctx.preReluGm);
                ctx.queuePreReluL1.EnQue(ctx.preReluL1);
                ctx.preReluL1 = ctx.queuePreReluL1.template DeQue<PreReluT>();
            }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
            ctx.preReluFB = ctx.queuePreReluFB.template AllocTensor<PreReluT>();
            loadPreReluFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_PRE_RELU, ctx.preReluFB, ctx.preReluL1);
            ctx.queuePreReluFB.EnQue(ctx.preReluFB);
            ctx.preReluFB = ctx.queuePreReluFB.template DeQue<PreReluT>();
#endif
        }
        if (ctx.hasVectorPreClipRelu) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.preClipReluL1 = ctx.queuePreClipReluL1.template AllocTensor<PreClipReluT>();
                loadPreClipReluL1Ins.LoadChannelWiseL1(ctx, ctx.preClipReluL1, ctx.preClipReluGm);
                ctx.queuePreClipReluL1.EnQue(ctx.preClipReluL1);
                ctx.preClipReluL1 = ctx.queuePreClipReluL1.template DeQue<PreClipReluT>();
            }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
            ctx.preClipReluFB = ctx.queuePreClipReluFB.template AllocTensor<PreClipReluT>();
            loadPreClipReluFBIns.LoadFixpParamFb(
                ctx, DST_MEM_BLOCK_PRE_CLIP_RELU, ctx.preClipReluFB, ctx.preClipReluL1);
            ctx.queuePreClipReluFB.EnQue(ctx.preClipReluFB);
            ctx.preClipReluFB = ctx.queuePreClipReluFB.template DeQue<PreClipReluT>();
#endif
        }
        if (ctx.hasVectorAntiQuant) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.antiQuantL1 = ctx.queueAntiQuantL1.template AllocTensor<AntiQuantT>();
                loadAntiQuantL1Ins.LoadChannelWiseL1(ctx, ctx.antiQuantL1, ctx.antiQuantGm);
                ctx.queueAntiQuantL1.EnQue(ctx.antiQuantL1);
                ctx.antiQuantL1 = ctx.queueAntiQuantL1.template DeQue<AntiQuantT>();
            }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
            ctx.antiQuantFB = ctx.queueAntiQuantFB.template AllocTensor<AntiQuantT>();
            loadAntiQuantFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_ANTI_QUANT, ctx.antiQuantFB, ctx.antiQuantL1);
            ctx.queueAntiQuantFB.EnQue(ctx.antiQuantFB);
            ctx.antiQuantFB = ctx.queueAntiQuantFB.template DeQue<AntiQuantT>();
#endif
        }
        if (ctx.hasEltwise) {
            if (ctx.eltwiseBrcEn) {
                if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                    // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                    //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                    // }
                    ctx.eltwiseL1 = ctx.queueEltwiseL1.template AllocTensor<EltwiseT>();
                    loadEltwiseL1Ins.LoadChannelWiseL1(ctx, ctx.eltwiseL1, ctx.eltwiseGm);
                    ctx.queueEltwiseL1.EnQue(ctx.eltwiseL1);
                    ctx.eltwiseL1 = ctx.queueEltwiseL1.template DeQue<EltwiseT>();
                }
            } else {
                ctx.eltwiseL1 = ctx.queueEltwiseL1.template AllocTensor<EltwiseT>();
                loadEltwiseNonBrcL1Ins.CopyElt(ctx);
                ctx.queueEltwiseL1.EnQue(ctx.eltwiseL1);
                ctx.eltwiseL1 = ctx.queueEltwiseL1.template DeQue<EltwiseT>();
            }
        }
        if (ctx.hasVectorPostQuant) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.postQuantL1 = ctx.queuePostQuantL1.template AllocTensor<PostQuantT>();
                loadPostQuantL1Ins.LoadChannelWiseL1(ctx, ctx.postQuantL1, ctx.postQuantGm);
                ctx.queuePostQuantL1.EnQue(ctx.postQuantL1);
                ctx.postQuantL1 = ctx.queuePostQuantL1.template DeQue<PostQuantT>();
            }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
            ctx.postQuantFB = ctx.queuePostQuantFB.template AllocTensor<PostQuantT>();
            loadPostQuantFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_POST_QUANT, ctx.postQuantFB, ctx.postQuantL1);
            ctx.queuePostQuantFB.EnQue(ctx.postQuantFB);
            ctx.postQuantFB = ctx.queuePostQuantFB.template DeQue<PostQuantT>();
#endif
        }
        if (ctx.hasVectorPostRelu) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.postReluL1 = ctx.queuePostReluL1.template AllocTensor<PostReluT>();
                loadPostReluL1Ins.LoadChannelWiseL1(ctx, ctx.postReluL1, ctx.postReluGm);
                ctx.queuePostReluL1.EnQue(ctx.postReluL1);
                ctx.postReluL1 = ctx.queuePostReluL1.template DeQue<PostReluT>();
            }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
            ctx.postReluFB = ctx.queuePostReluFB.template AllocTensor<PostReluT>();
            loadPostReluFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_POST_RELU, ctx.postReluFB, ctx.postReluL1);
            ctx.queuePostReluFB.EnQue(ctx.postReluFB);
            ctx.postReluFB = ctx.queuePostReluFB.template DeQue<PostReluT>();
#endif
        }
        if (ctx.hasVectorPostClipRelu) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                // if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                //     SetFlag<HardEvent::FIX_MTE2>(eventId);
                //     WaitFlag<HardEvent::FIX_MTE2>(eventId);
                // }
                ctx.postClipReluL1 = ctx.queuePostClipReluL1.template AllocTensor<PostClipReluT>();
                loadPostClipReluL1Ins.LoadChannelWiseL1(ctx, ctx.postClipReluL1, ctx.postClipReluGm);
                ctx.queuePostClipReluL1.EnQue(ctx.postClipReluL1);
                ctx.postClipReluL1 = ctx.queuePostClipReluL1.template DeQue<PostClipReluT>();
            }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
            ctx.postClipReluFB = ctx.queuePostClipReluFB.template AllocTensor<PostClipReluT>();
            loadPostClipReluFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_POST_CLIP_RELU, ctx.postClipReluFB, ctx.postClipReluL1);
            ctx.queuePostClipReluFB.EnQue(ctx.postClipReluFB);
            ctx.postClipReluFB = ctx.queuePostClipReluFB.template DeQue<PostClipReluT>();
#endif
        }

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
        ctx.outputL1 = ctx.queueOutputL1.template AllocTensor<OutputT>();
#else
        ctx.cl0 = ctx.queueCL0.template AllocTensor<L0cT>();
#endif

        // reduceK priority: 1.KL0FullLoad 2.L1DB Preload 3. ordinary reduceK
        if constexpr (kl0FullLoadFlag) {
            ReduceOneK();
        } else if constexpr (kPreLoadFlag) {
            ReduceKPreload();
        } else {
            ReduceK();
        }

        FreeL1Tensor();

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
        ctx.queueOutputL1.EnQue(ctx.outputL1);
        ctx.outputL1 = ctx.queueOutputL1.template DeQue<OutputT>();
#else
        ctx.queueCL0.EnQue(ctx.cl0);
        ctx.cl0 = ctx.queueCL0.template DeQue<L0cT>();
#endif

        ctx.kIter = 0;
    }

    __aicore__ inline bool IterateImplMMode(bool enPartialSum)
    {
        if (ctx.isFirstIterate) {
            FirstIterateImplMMode();
            if (mPreLoadAFlag) {
                if (ctx.maxKBL1Iter == 0 && ctx.maxNBL1Iter == 0) {
                    LoadBL1Moudle();
                    ctx.weightL1 = ctx.queueWeightL1.template DeQue<WeightT>();
                    ctx.loadBL1Flag = false;
                }
                printf("first iterate fmap preload\n");
                LoadAL1Moudle();
                ctx.loadAL1Flag = true;
            }
        } else if constexpr (iterateMFirstFlag) {
            if (IterateMFirstMMode() == false) {
                return false;
            }
        } else if constexpr (iterateNFirstFlag) {
            if (IterateNFirstMMode() == false) {
                return false;
            }
        }

        IterateK();

        return true;
    }

    __aicore__ inline bool IterateImplHWMode(bool enPartialSum)
    {
        // [zj]TODO: preload
        if (ctx.isFirstIterate) {
            FirstIterateImplHWMode();
        } else if constexpr (iterateMFirstFlag) {
            if (IterateMFirstHWMode() == false) {
                return false;
            }
        } else if constexpr (iterateNFirstFlag) {
            if (IterateNFirstHWMode() == false) {
                return false;
            }
        }

        UpdateHoL0WoL0();

        IterateK();

        return true;
    }
};

} // namespace Conv2dApi

#endif // ADV_API_CONV_CONV2D_CONV2D_INTF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_INTF_H__
#endif
