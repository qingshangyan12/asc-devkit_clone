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
 * \file conv2d_intf.h
 * \brief
 */

#ifndef CONV2D_INTF_H
#define CONV2D_INTF_H

// #include "../../impl/conv/conv_common/conv_common_func.h"
#include "../../impl/conv/conv_common/conv_framework_util.h"
// #include "../../impl/conv/conv2d/conv2d_common_func.h"
#include "lib/conv/conv2d_config.h"
#include "../../impl/conv/conv2d/conv2d_util.h"

namespace conv2d {
using namespace conv;

constexpr uint64_t DST_MEM_BLOCK_PRE_QUANT = 0;
constexpr uint64_t DST_MEM_BLOCK_PRE_RELU = 1;
constexpr uint64_t DST_MEM_BLOCK_POST_RELU = 2;
constexpr uint64_t DST_MEM_BLOCK_POST_QUANT = 3;
constexpr uint64_t DST_MEM_BLOCK_ANTI_QUANT = 4;

struct TempIters {
    uint64_t kAL1Iter;
    uint64_t kBL1Iter;
    uint64_t mAL1Iter; // m mode
    uint64_t hoAL1Iter; // hw mode
    uint64_t woAL1Iter; // hw mode
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
    using BiasT = typename Config::BiasT;
    using QuantPreT = typename Config::QuantPreT;
    using ReluPreT = typename Config::ReluPreT;
    using EltAntiQuantT = typename Config::EltAntiQuantT;
    using QuantPostT = typename Config::QuantPostT;
    using EltwiseT = typename Config::EltwiseT;
    using L0cT = typename Config::L0cT;
    using ContextType = typename Ext::ContextData;
    using ImplDataType = typename Ext::ImplDataType;
    using ConvParam = typename Config::ConvParam;

    constexpr static int8_t outputOrder = ConvParam::outputOrder;
    constexpr static int8_t groupType = ConvParam::groupType;
    constexpr static int8_t l0PingPong = ConvParam::l0PingPong;
    constexpr static int8_t l1PingPong = ConvParam::l1PingPong;
    constexpr static bool bL1DBFlag = ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::L1B_OPEN) ||
                                      ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_OPEN);
    //[zj][CHINESE]0PingPong == [CHINESE]onvL0PingPong[CHINESE]    constexpr static bool hasHL1IterFlag = !(ConvParam::l0PingPong == 0 || ConvParam::fmpTiling == 0 ||
        ConvParam::fmpTiling == 1) ||
        !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasWL1IterFlag = !(ConvParam::l0PingPong == 0 || ConvParam::fmpTiling == 0 ||
        ConvParam::fmpTiling == 1) ||
        !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasHL0IterFlag = !(ConvParam::l0PingPong == 0 || ConvParam::fmpTiling == 1) ||
        !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasWL0IterFlag = !(ConvParam::l0PingPong == 0 || ConvParam::fmpTiling == 1) ||
        !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasNL0IterFlag = !(ConvParam::l0PingPong == 0 || ConvParam::weightTiling == 1) ||
        !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasNL1IterFlag = !(ConvParam::l0PingPong == 0 || ConvParam::weightTiling == 1 ||
        ConvParam::weightTiling == 0) ||
        !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasML1IterFlag = !(ConvParam::l0PingPong == 0 || ConvParam::fmpTiling == 0 ||
        ConvParam::fmpTiling == 1) ||
        !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool hasML0IterFlag = !(ConvParam::l0PingPong == 0 || ConvParam::fmpTiling == 1) ||
        !(ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV));
    constexpr static bool kl0FullLoadFlag = ConvParam::l0PingPong == 1 || ConvParam::l0PingPong == 2;

    // [zj]: TODO
    constexpr static bool mPreLoadAFlag = true;

    constexpr static bool kPreLoadAFlag =
        ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::L1A_OPEN) && ConvParam::weightTiling == 0;
    constexpr static bool kPreLoadBFlag =
        ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::L1B_OPEN) && ConvParam::fmpTiling == 0;
    constexpr static bool kPreLoadABFlag =
        ConvParam::l1PingPong == static_cast<int8_t>(ConvL1PingPong::ALL_OPEN) &&
        ConvParam::groupType == static_cast<int8_t>(ConvGroupType::NORMAL_CONV);
    constexpr static bool kPreLoadFlag =
        kPreLoadAFlag || kPreLoadBFlag || kPreLoadABFlag;
    constexpr static bool iterateMFirstFlag = ConvParam::iterOrder == 0;
    constexpr static bool iterateNFirstFlag = ConvParam::iterOrder == 1;

    constexpr static auto formatA = Config::formatA;
    constexpr static auto formatB = Config::formatB;
    constexpr static auto formatC = Config::formatC;
    constexpr static auto formatBias = Config::formatBias;
    constexpr static auto formatQuantPre = Config::formatQuantPre;
    constexpr static auto formatReluPre = Config::formatReluPre;
    constexpr static auto formatEltAntiQuant = Config::formatEltAntiQuant;
    constexpr static auto formatQuantPost = Config::formatQuantPost;
    constexpr static auto formatEltwise = Config::formatEltwise;

    constexpr static uint64_t k0 = C0_SIZE / sizeof(FmapT);
    constexpr static uint64_t k0a = C0_SIZE / sizeof(FmapT);
    constexpr static uint64_t k0b = C0_SIZE / sizeof(WeightT);
    // [zj]ToDo
    constexpr static bool isQuantScene = (AscendC::IsSameType<L0cT, int32_t>::value &&
                                          AscendC::IsSameType<OutputT, half>::value)/* ||
                                         AscendC::IsSameType<FmapT, hifloat8_t>::value ||
                                         AscendC::IsSameType<FmapT, float8_e4m3_t>::value*/;
    constexpr static uint8_t sizeOfFmap = sizeof(FmapT);
    constexpr static uint8_t sizeOfWeight = sizeof(WeightT);
    constexpr static uint8_t sizeOfOutput = sizeof(OutputT);
    constexpr static uint8_t sizeOfBias = sizeof(BiasT);
    constexpr static uint8_t sizeOfQuantPre = sizeof(QuantPreT);
    constexpr static uint8_t sizeOfReluPre = sizeof(ReluPreT);
    constexpr static uint8_t sizeOfEltAntiQuant = sizeof(EltAntiQuantT);
    constexpr static uint8_t sizeOfQuantPost = sizeof(QuantPostT);
    constexpr static uint8_t sizeOfEltwise = sizeof(EltwiseT);
    constexpr static uint8_t sizeOfL0c = sizeof(L0cT);

public:
    ContextType ctx;
    ImplDataType impl;

    // Using Conditional<flag, type1, type2>::type to select M or HW, if flag=true, type=type1, else type=type2
    using LoadAL1Tools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        Conv2dFunc::LoadAL1ToolsMMode<Conv2dIntf>, Conv2dFunc::LoadAL1ToolsHWMode<Conv2dIntf>>::type;
    using LoadAL0Tools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        ConvFunc::LoadAL0ToolsMMode<Conv2dIntf>, ConvFunc::LoadAL0ToolsHWMode<Conv2dIntf>>::type;
    using CopyEltTools = typename Conditional<
        ConvParam::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE),
        ConvFunc::CopyEltToolsMMode<Conv2dIntf>, ConvFunc::CopyEltToolsHWMode<Conv2dIntf>>::type;
    using CopyOutTools = typename Conditional<
     CvParam::oputOrder tatic_cast<int8_t>(ConvOuputOrder::MMODE),
       onvFu::CopyOutToolsMode<Conv2dIn onvFunc::opyOutToolsodevn>::tye;

    LodAL1oosadAl1Ins;
   Conv2dFunc::LoadBL1Tools<Conv2dIntf> loadBL1Ins;
    ConvFunc::LoadChannelWiseLos<Conv2dIntf, tyenameConfig::Bs> loadBiasL1Ins;
    CovFnc::LoadChannelWiseL1ToConv2dIntf, typename Config::QuantPreT> loadQuantPreL1Is;
    ConvFuc::LoadChannelWiseL1Tools<Conv2dIntftypname Confi::RelureT> oadeluPreL1Ins;
    ConvFun:LoCannelWiseL1ToolCv2dIntf, tpname Config::EltAntiQuantT> lodEltAniQuantL1Ins;
   CvFunc::LoadChannelWiseL1Tools<Con2dItf, typename Config:QuantostT> loadQuantPotL1Ins;
   ConvFunc::adChannelWiseL1Tool<Conv2dIntf, typename onfg::EltiseT>EltwiseL1Ins;
    CopyEols loadEltweNonBcL1Ins;
   Lod0Tools lL0In  CovFu::Loa0ToolsConvdIntf> loadBL0Ins;
    nvun::LoadBiasBtTools<onv2dfoaBTIns;
    ConvFunc::LoadFixpParamTools<Cnv2dIntf, typena Config::QuatPrT> loadQuantreFIns;    Convc:LoaFxpParambTools<Conv2dIntf, typename Config::elPreT> loadReluPreFBIns;
   Cu:LoadFixpParambToolConv2dItf typename Config::EltAntiQuanT>adElAntiQuantFBIns;    Convu::LoadFixpPraFToolsConv2dntf,typeameConfg::QuantPostT loadQuantPostFBIns;
    ConvFn::MadToo<Conv2dIntf> madIns;
    oyools copytIns;

public:
    _aicore_ iline Conv2dIntf()
{
    }

    // __aor__ inline vd Init(const void* _restrict vTilng, TPipe *tPipe)
    / {
    //    sig local = typenameExt::Init;
   //   if conste (CONV_CHEF(ocalonv2dFunc, , convTilin tPie)) {
   //        local::cal(ts, cnvTilig, tPpe);
    /     }
   // }

    // _aicore__ inline void etFap(const GlolTesor<Fmap>&f)
    //{
    //     usilocal = typeEtSetFp;
  //     if constexpr (CONVCKFUN(local, ConvFunc, this, map)) {  //         local:ll(his, fmap);
    //    }
 / }

    // _aico__ inlin void etWeight(cnstGblTensor<WeightT& weight)  // {
   //     using local = typename Ext::SetWeig//    iconspr (CONV_CHECN(oal, ConvFnc, this, weight {
          lcal::call(is, weight);
    //     }   // }
    // __core__inline voiSetBs(cost GlobalTnsor<BiT> bi)  // {
    //     using local = typename Ext::SetBis;
    /     if constexpr (ONV_CHECFUN(local Conv2dFunc,his, bias)
    //         localcall(hi, bis);
   //     }
   / }

   // __aicore inline oidSetQuantPecons GlobaTnor<QuantPreT>& quantPre)
    // {
   //     sioaltypenae Ext::etuaPre;
    //    if constexp(CONVCHEC_UN(local, ConvFunc, tnPr) {
    //        ocal:call(this,quaPr);
    //   }
  
    // _aicore_ inline void SetRePr(const Globalesor<ReluPreT>& relure)
   /
   //     uinglocl = typname ExtSetReluPre;
    //     if costexprONV_CHECK_FN(lal, Convunc, this, reluPre)) 
          local::cal(this, reluPr;
    //  }
   / 

    // __icor__ inline void etEltAntQunt(const GloblTensr<EltntiQutT>& eltntiuant)
   // 
  //    using lcal =typameExt:SetEltAntiQuant;
    //     cstexpr(CONV_CHECK_N(la,Covunc ths eltntiuant)) { :       }          = CoreCi * ctx.convTilin
  
    //      local::call(this,elAntiQuant);
    //    }
    // }

    // _aicore_ le void etQntotonst GlobaTnsorQuntPostT>& quantPost)um);
//         }
//         return false;
//     }

//     template <bool sync = true>
//     __aicore__ inline void
    // 
   //   using local = typename Ext:Sett
//     ifnstxpr(CNV_CHECK_FUN(lo,CovFun, this, quantPost)) {
    //         local::cal(this,nPost);    //     }    // }

   // _aicore__nline void SetQuantFpipe(cntt64_t &quantPreMode, cost uit_treud    //                                  cont uint64_t &clipRePrMod, st int64_&quantPostMode,
    //                                 nst unt6_&reluostode, cont uint64_t &eltwiseOp,
    //                              const uin_t ltwiseAntiqEn, const uint64teweBrcEn,
    //                                    const uint64_T &QUANTPreSp,cnst uint64t RELUPREpr,TensorC(const GlobalTensor<OutputT>& output, bool 
enSequentialWrite 
= false)
//     {
//         using local = typename Ext::GetTensorC;
//         if constexpr (CONV_CHECK_FUN_TEMPLATE(local, Conv2dFunc, sync, this, output, enSequentialWrite)) {
//             local::template call<sync>(this, output, enSequentialWrite);    }

public:
    __aicore__ inline void InitKDirectionValue()
    {
        // K[CHINESE]        if constexpr(formatB == ConvFormat::FRACTAL_Z_C04) {
            size_t multiKL1 = ctx.convTiling->kAL1 / ctx.convTiling->kL0;
            ctx.maxKAL1Iter = 0;
            ctx.maxKBL1Iter = 0;
            ctx.maxKL0Iter = multiKL1 - 1;
            ctx.ddr2l0LoopK = multiKL1;
            ctx.kAL1Tail = ctx.convTiling->kAL1;
            ctx.kBL1Tail = ctx.convTiling->kBL1;
            ctx.kL0Tail = ctx.convTiling->kL0;
            ctx.multiKAL1 = multiKL1;
            ctx.multiKBL1 = multiKL1;
            ctx.kAL1fullload = true;
            ctx.kBL1fullload = true;
            return;
        }
        ctx.maxKAL1Iter =
            CeilDIV(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, ctx.convTiling->kAL1) - 1;
        ctx.maxKBL1Iter =
            CeilDIV(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, ctx.convTiling->kBL1) - 1;
        ctx.ddr2l0LoopK = CeilDIV(AlignB(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, k0),
                                        ctx.convTiling->kL0);
        ctx.maxKL0Iter = ctx.ddr2l0LoopK - 1;
        if constexpr(formatA == ConvFormat::NC1HWC0) {
            ctx.kAL1Tail = (AlignB(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, k0a)) % ctx.convTiling->kAL1;
            ctx.kAL1Tail = ctx.kAL1Tail == 0 ? ctx.convTiling->kAL1 : ctx.kAL1Tail;
        } else {
            ctx.kAL1Tail = (ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW) % ctx.convTiling->kAL1;
            ctx.kAL1Tail = ctx.kAL1Tail == 0 ? ctx.convTiling->kAL1 : ctx.kAL1Tail;
        }
        if constexpr(formatB == ConvFormat::FRACTAL_Z) {
            ctx.kBL1Tail = (AlignB(ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW, k0b)) % ctx.convTiling->kBL1;
            ctx.kBL1Tail = ctx.kBL1Tail == 0 ? ctx.convTiling->kBL1 : ctx.kBL1Tail;
        } else {
            ctx.kBL1Tail = (ctx.singleCoreCi * ctx.convTiling->kernelHxkernelW) % ctx.convTiling->kBL1;
            ctx.kBL1Tail = ctx.kBL1Tail == 0 ? ctx.convTiling->kBL1 : ctx.kBL1Tail;
        }
        ctx.kL0Tail = (AlignB(ctx.singleCoreCi *
            ctx.convTiling->kernelHxkernelW, k0)) % ctx.convTiling->kL0;
        ctx.kL0Tail = ctx.kL0Tail == 0 ? ctx.convTiling->kL0 : ctx.kL0Tail;
        ctx.multiKAL1 = CeilDIV(ctx.convTiling->kAL1, ctx.convTiling->kL0);
        ctx.multiKBL1 = CeilDIV(ctx.convTiling->kBL1, ctx.convTiling->kL0);
        if constexpr (kPreLoadFlag) {
            ctx.lastLoopKAL1StartPos = ctx.ddr2l0LoopK - ctx.multiKAL1;
            ctx.lastLoopKBL1StartPos = ctx.ddr2l0LoopK - ctx.multiKBL1;
        }
    }

    __aicore__ inline void Init(const TConv2DTiling *__restrict convTiling, TPipe *tPipe)
    {
        ctx.convTiling = convTiling;
        ctx.pipe = GetTPipePtr();
        if ASCEND_IS_AIC {
            printf("ASCEND_IS_AIC\n");

            // [zj]ToDocheck[CHINESE]            ctx.enableBias = false;
            ctx.enableQuantPre = false;
            ctx.enableReluPre = false;
            ctx.enableEltAntiQuant = false;
            ctx.enableQuantPost = false;
            ctx.enableEltwise = false;
            ctx.mAL1UpdateFlag = true;
            ctx.iter = 0;
            ctx.kIter = 0;

            ctx.orgCi = ctx.convTiling->orgCi;
            ctx.orgHi = ctx.convTiling->orgHi;
            ctx.orgWi = ctx.convTiling->orgWi;
            ctx.orgCo = ctx.convTiling->orgCo;
            ctx.orgHo = ctx.convTiling->orgHo;
            ctx.orgWo = ctx.convTiling->orgWo;
            ctx.kernelH = ctx.convTiling->kernelH;
            ctx.kernelW = ctx.convTiling->kernelW;
            ctx.dilatedKernelH = 1 + (ctx.kernelH - 1) * ctx.convTiling->dilationH;
            ctx.dilatedKernelW = 1 + (ctx.kernelW - 1) * ctx.convTiling->dilationW;
            ctx.singleCoreCi = ctx.convTiling->singleCoreCi; // [CHINESE]in[CHINESE]            uint64_t alignCinKhKwA =
                AlignB(ctx.convTiling->singleCoreCi * ctx.convTiling->kernelHxkernelW, k0a);
            uint64_t alignCinKhKwB =
                AlignB(ctx.convTiling->singleCoreCi * ctx.convTiling->kernelHxkernelW, k0b);
            ctx.kAL1fullload = alignCinKhKwA == ctx.convTiling->kAL1;
            ctx.kBL1fullload = alignCinKhKwB == ctx.convTiling->kBL1;
            ctx.singleCoreCo = ctx.convTiling->singleCoreCo; // [CHINESE]o[CHINESE]
            // [zj][CHINESE]M[CHINESE][CHINESE]oWo[CHINESE]1L0[CHINESE]            if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
                InitValueMMode();
            } else {
                InitValueHWMode();
            }

            // [zj][CHINESE]0al0bl0cl1al1bl1biasl0biasl1scale[CHINESE]ocalTensor
            InitBuffer();
            // [zj][CHINESE]K[CHINESE]1L0[CHINESE]            InitKDirectionValue();
            // [zj][CHINESE]Co[CHINESE]1L0[CHINESE]            InitCoDirectionValue();
            InitHf32Mode();
            // [CHINESE]ools[CHINESE]            InitSubApiParams();

            if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                ctx.ciPerGroup = ctx.convTiling->orgCi / ctx.convTiling->groups;
                ctx.singleGroupOpt = ctx.convTiling->singleCoreGroupOpt;
                // OptGroupCalcBL1LoadTimes();
            }
        } else {
            
            if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                // OptGroupVecInit();
            }
        }
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
        ctx.singleCoreHo = ctx.convTiling->singleCoreHo; // [CHINESE]o[CHINESE]        ctx.singleCoreWo = ctx.convTiling->singleCoreWo; // [CHINESE]o[CHINESE]        InitHoDirectionValue();
        InitWoDirectionValue();
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
    }

    __aicore__ inline void SetSingleOutputShape(uint64_t singleCo, uint64_t singleHo, uint64_t singleWo)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
            // [zj][CHINESE]o[CHINESE]o
            ctx.singleCoreCo = singleCo;
            ctx.singleCoreHo = singleHo;
            if ASCEND_IS_AIC {
                // [zj][CHINESE]ingleCoreCosingleCoreHo[CHINESE]                InitCoDirectionValue();
                InitHoDirectionValue();
                if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                    // OptGroupCalcBL1LoadTimes();
                }
            } else {
                if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                    // OptGroupInitNValue();
                    // OptGroupInitIterValue();
                }
            }
        }
    }

    __aicore__ inline void SetSingleOutputShape(uint64_t singleCo, uint64_t singleM)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            ctx.singleCoreCo = singleCo;
            ctx.singleCoreM = singleM;
            if ASCEND_IS_AIC {
                // [zj][CHINESE]ingleCoreCosingleCoreM[CHINESE]                InitCoDirectionValue();
                InitMDirectionValue();
                if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                    // OptGroupCalcBL1LoadTimes();
                }
            } else {
                if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                    // OptGroupInitNValue();
                    // OptGroupInitIterValue();
                }
            }
        }
    }

    // [zj][CHINESE]ilingData[CHINESE]    // [CHINESE]    __aicore__ inline void SetFmapStartPosition(int64_t hiStartPos, int64_t wiStartPos)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
            if ASCEND_IS_AIC {
                ctx.hiStartPos = hiStartPos;
                ctx.wiStartPos = wiStartPos;
            }
        }
    }

    __aicore__ inline void SetFmapStartPosition(int64_t mStartPos)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            if ASCEND_IS_AIC {
                ctx.mStartPos = mStartPos;
            }
        }
    }

    __aicore__ inline void SetOptGroupParams(uint64_t singleGroups, uint64_t singleGroupOpt)
    {
        ctx.singleGroups = singleGroups;
        ctx.singleGroupOpt = singleGroupOpt;
        ctx.singleCoreCi = singleGroups * ctx.ciPerGroup;

        if ASCEND_IS_AIC {
            InitKDirectionValue();
            if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                // OptGroupCalcBL1LoadTimes();
            }
        } else {
            if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                // OptGroupInitKValue();
            }
        }
    }





    __aicore__ inline void SetFmap(const GlobalTensor<FmapT> &fmap)
    {
        if ASCEND_IS_AIC {
            ctx.agm.SetGlobalBuffer(fmap.GetPhyAddr(0), fmap.GetSize());
            ctx.isFirstIterate = true;
        }
    }


    __aicore__ inline void SetWeight(const GlobalTensor<WeightT> &weight)
    {
        ctx.bgm.SetGlobalBuffer(weight.GetPhyAddr(0), weight.GetSize());
        ctx.isFirstIterate = true;
    }

    __aicore__ inline void SetBias(const GlobalTensor<BiasT> &bias)
    {
        if ASCEND_IS_AIC {
            ctx.biasgm.SetGlobalBuffer(bias.GetPhyAddr(0), bias.GetSize());
            ctx.isFirstIterate = true;
            ctx.enableBias = true;
        }
    }

    __aicore__ inline void SetQuantPre(const GlobalTensor<QuantPreT> &quantPre)
    {
        if ASCEND_IS_AIC {
            ctx.quantPreGm.SetGlobalBuffer(quantPre.GetPhyAddr(0), quantPre.GetSize());
            ctx.isFirstIterate = true;
            ctx.enableQuantPre = true;
        }
    }

    __aicore__ inline void SetReluPre(const GlobalTensor<ReluPreT> &reluPre)
    {
        if ASCEND_IS_AIC {
            ctx.reluPreGm.SetGlobalBuffer(reluPre.GetPhyAddr(0), reluPre.GetSize());
            ctx.isFirstIterate = true;
            ctx.enableReluPre = true;
        }
    }

    __aicore__ inline void SetEltAntiQuant(const GlobalTensor<EltAntiQuantT> &eltAntiQuant)
    {
        if ASCEND_IS_AIC {
            ctx.eltAntiQuantGm.SetGlobalBuffer(eltAntiQuant.GetPhyAddr(0), eltAntiQuant.GetSize());
            ctx.isFirstIterate = true;
            ctx.enableEltAntiQuant = true;
        }
    }

    __aicore__ inline void SetQuantPost(const GlobalTensor<QuantPostT> &quantPost)
    {
        if ASCEND_IS_AIC {
            ctx.quantPostGm.SetGlobalBuffer(quantPost.GetPhyAddr(0), quantPost.GetSize());
            ctx.isFirstIterate = true;
            ctx.enableQuantPost = true;
        }
    }

    __aicore__ inline void SetQuantFixpipe(const uint64_t &quantPreMode, const uint64_t &reluPreMode,
                                       const uint64_t &clipReluPreMode, const uint64_t &quantPostMode,
                                       const uint64_t &reluPostMode, const uint64_t &eltwiseOp,
                                       const uint64_t &eltwiseAntiqEn, const uint64_t &eltwiseBrcEn,
                                       const uint64_t &quantPreSpr, const uint64_t &reluPreSpr,
                                       const uint64_t &antiQuantSpr, const uint64_t &quantPostSpr)
    {
        if ASCEND_IS_AIC {
            ctx.quantPreMode = quantPreMode;
            ctx.reluPreMode = reluPreMode;
            ctx.clipReluPreMode = clipReluPreMode;
            ctx.quantPostMode = quantPostMode;
            ctx.reluPostMode = reluPostMode;
            ctx.eltwiseOp = eltwiseOp;
            ctx.eltwiseAntiqEn = eltwiseAntiqEn;
            ctx.eltwiseBrcEn = eltwiseBrcEn;
            ctx.quantPreSpr = quantPreSpr;
            ctx.reluPreSpr = reluPreSpr;
            ctx.antiQuantSpr = antiQuantSpr;
            ctx.quantPostSpr = quantPostSpr;
        }
    }

    __aicore__ inline void SetEltwise(const GlobalTensor<EltwiseT> &eltwise)
    {
        if ASCEND_IS_AIC {
            ctx.eltwiseGm.SetGlobalBuffer(eltwise.GetPhyAddr(0), eltwise.GetSize());
            ctx.isFirstIterate = true;
            ctx.enableEltwise = true;
        }
    }

    __aicore__ inline void SetSingleBatchNum(uint64_t singleCoreBatch,
                                       uint64_t fmapOneBatchSize, uint64_t outputOneBatchSize)
    {
        ctx.ddr2l1LoopBatch = singleCoreBatch;
        if ASCEND_IS_AIC {
            ctx.fmapOneBatchSize = fmapOneBatchSize;
            ctx.outputOneBatchSize = outputOneBatchSize;
            if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                // OptGroupCalcBL1LoadTimes();
            }
        } else {
            if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                // OptGroupInitIterValue();
                if (ctx.vecId == 1) {
                    // OptGroupCalcBL1LoadTimes();
                }
            }
        }
    }

    template <bool sync = true>
    __aicore__ inline void GetTensorC(const GlobalTensor<OutputT> &output, bool enSequentialWrite = false)
    {
        if ASCEND_IS_AIC {
            copyOutIns.CopyOut(ctx, output);

#if defined(__DAV_L510__)
            ctx.queueCL1.FreeTensor(ctx.cl1);
#else
            ctx.queueCL0.FreeTensor(ctx.cl0);
#endif

            if (ctx.enableQuantPre && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueQuantPreL1.FreeTensor(ctx.quantPreL1);
            }
            if (ctx.enableReluPre && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueReluPreL1.FreeTensor(ctx.reluPreL1);
            }
            if (ctx.enableEltAntiQuant && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueEltAntiQuantL1.FreeTensor(ctx.eltAntiQuantL1);
            }
            if (ctx.enableQuantPost && !ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueQuantPostL1.FreeTensor(ctx.quantPostL1);
            }
            if (ctx.enableEltwise && (!ctx.eltwiseBrcEn || !ctx.convTiling->fixpParamsFullLoadFlag)) {
                ctx.queueEltwiseL1.FreeTensor(ctx.eltwiseL1);
            }
        }
    }

    template <bool sync = true>
    __aicore__ inline bool IterateAll(const GlobalTensor<OutputT> &output, bool enPartialSum = false)
    {
        if ASCEND_IS_AIC {
            if (ctx.convTiling->biasFullLoadFlag && ctx.enableBias) {
                ctx.biasL1 = ctx.queueBiasL1.template AllocTensor<BiasT>();
                loadBiasL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.biasL1, ctx.biasgm,
                                                                  ctx.singleCoreCo, 0);
                ctx.queueBiasL1.EnQue(ctx.biasL1);
                ctx.biasL1 = ctx.queueBiasL1.template DeQue<BiasT>();
                // DumpAccChkPoint(ctx.biasL1, 0, 0, ctx.singleCoreCo);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.enableQuantPre) {
                if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                ctx.quantPreL1 = ctx.queueQuantPreL1.template AllocTensor<QuantPreT>();
                loadQuantPreL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.quantPreL1, ctx.quantPreGm,
                                                                      ctx.singleCoreCo, 0);
                ctx.queueQuantPreL1.EnQue(ctx.quantPreL1);
                ctx.quantPreL1 = ctx.queueQuantPreL1.template DeQue<QuantPreT>();
                // // DumpAccChkPoint(ctx.quantPreL1.ReinterpretCast<uint32_t>(), 1, 0, ctx.singleCoreCo * 2);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.enableReluPre) {
                if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                ctx.reluPreL1 = ctx.queueReluPreL1.template AllocTensor<ReluPreT>();
                loadReluPreL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.reluPreL1, ctx.reluPreGm,
                                                                     ctx.singleCoreCo, 0);
                ctx.queueReluPreL1.EnQue(ctx.reluPreL1);
                ctx.reluPreL1 = ctx.queueReluPreL1.template DeQue<ReluPreT>();
                // DumpAccChkPoint(ctx.reluPreL1, 2, 0, ctx.singleCoreCo);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.enableEltAntiQuant) {
                if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                ctx.eltAntiQuantL1 =
                    ctx.queueEltAntiQuantL1.template AllocTensor<EltAntiQuantT>();
                loadEltAntiQuantL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.eltAntiQuantL1,
                                                                          ctx.eltAntiQuantGm,
                                                                          ctx.singleCoreCo, 0);
                ctx.queueEltAntiQuantL1.EnQue(ctx.eltAntiQuantL1);
                ctx.eltAntiQuantL1 = ctx.queueEltAntiQuantL1.template DeQue<EltAntiQuantT>();
                // DumpAccChkPoint(ctx.eltAntiQuantL1, 3, 0, ctx.singleCoreCo);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.enableQuantPost) {
                if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                ctx.quantPostL1 = ctx.queueQuantPostL1.template AllocTensor<QuantPostT>();
                loadQuantPostL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.quantPostL1, ctx.quantPostGm,
                                                                       ctx.singleCoreCo, 0);
                ctx.queueQuantPostL1.EnQue(ctx.quantPostL1);
                ctx.quantPostL1 = ctx.queueQuantPostL1.template DeQue<QuantPostT>();
                // // DumpAccChkPoint(ctx.quantPostL1.ReinterpretCast<uint32_t>(), 4, 0, ctx.singleCoreCo * 2);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.enableEltwise && ctx.eltwiseBrcEn) {
                if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                ctx.eltwiseL1 = ctx.queueEltwiseL1.template AllocTensor<EltwiseT>();
                loadEltwiseL1Ins.LoadChannelWiseL1FullLoad(ctx, ctx.eltwiseL1, ctx.eltwiseGm,
                                                                     ctx.singleCoreCo, 0);
                ctx.queueEltwiseL1.EnQue(ctx.eltwiseL1);
                ctx.eltwiseL1 = ctx.queueEltwiseL1.template DeQue<EltwiseT>();
                // DumpAccChkPoint(ctx.eltwiseL1, 5, 0, ctx.singleCoreCo);
            }
        }

        while (Iterate(enPartialSum)) {
            GetTensorC(output);
        }

        if ASCEND_IS_AIC {
            if (ctx.convTiling->biasFullLoadFlag && ctx.enableBias) {
                ctx.queueBiasL1.FreeTensor(ctx.biasL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.enableQuantPre) {
                ctx.queueQuantPreL1.FreeTensor(ctx.quantPreL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.enableReluPre) {
                ctx.queueReluPreL1.FreeTensor(ctx.reluPreL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.enableEltAntiQuant) {
                ctx.queueEltAntiQuantL1.FreeTensor(ctx.eltAntiQuantL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.enableQuantPost) {
                ctx.queueQuantPostL1.FreeTensor(ctx.quantPostL1);
            }
            if (ctx.convTiling->fixpParamsFullLoadFlag && ctx.enableEltwise && ctx.eltwiseBrcEn) {
                ctx.queueEltwiseL1.FreeTensor(ctx.eltwiseL1);
            }

            ctx.isFirstIterate = true;
        }

        return false;
    }

    __aicore__ inline void End()
    {
        if ASCEND_IS_AIC {
            ctx.queueAL1.FreeAllEvent();
            ctx.queueBL1.FreeAllEvent();
            ctx.queueBiasL1.FreeAllEvent();
            ctx.queueQuantPreL1.FreeAllEvent();
            ctx.queueReluPreL1.FreeAllEvent();
            ctx.queueEltAntiQuantL1.FreeAllEvent();
            ctx.queueQuantPostL1.FreeAllEvent();
            ctx.queueEltwiseL1.FreeAllEvent();
            ctx.queueBiasBT.FreeAllEvent();
            ctx.queueQuantPreFB.FreeAllEvent();
            ctx.queueReluPreFB.FreeAllEvent();
            ctx.queueEltAntiQuantFB.FreeAllEvent();
            ctx.queueQuantPostFB.FreeAllEvent();
            ctx.queueCL1.FreeAllEvent();
            ctx.queueCL0.FreeAllEvent();
        }
    }

    __aicore__ inline void InitBufferWithDoubleBuf(int64_t cl0Spacesize, uint64_t bl1Spacesize)
    {
        // [zj]L0aL0b[CHINESE]Que[CHINESE]ocalTensor[CHINESE]ventid[CHINESE][CHINESE]        ctx.pipe->InitBuffer(ctx.l0aBuf, L0A_SIZE);
        ctx.pipe->InitBuffer(ctx.l0bBuf, L0B_SIZE);
        ctx.wholeAl0Tensor = ctx.l0aBuf.template Get<FmapT>();
        ctx.wholeBl0Tensor = ctx.l0bBuf.template Get<WeightT>();

        int8_t cl0db = (ctx.convTiling->pBufferFlag & 0x04) >> 2;
        int8_t al1db = (ctx.convTiling->pBufferFlag & 0x08) >> 3;
        int8_t bl1db = (ctx.convTiling->pBufferFlag & 0x10) >> 4;
        printf("cl0db: %d, al1db: %d, bl1db: %d\n", cl0db, al1db, bL1DBFlag);

    #if defined(__DAV_L510__)
        if (!cl0db) {
            ctx.pipe->InitBuffer(ctx.queueCL1, 1, cl0Spacesize * sizeOfOutput);
        } else {
            ctx.pipe->InitBuffer(ctx.queueCL1, DOUBLE_BUF, cl0Spacesize * sizeOfOutput);
        }
    #else
        if (!cl0db) {
            ctx.pipe->InitBuffer(ctx.queueCL0, 1, cl0Spacesize * sizeOfL0c);
        } else {
            ctx.pipe->InitBuffer(ctx.queueCL0, DOUBLE_BUF, cl0Spacesize * sizeOfL0c);
        }
    #endif

        // [zj]aL1SpaceSize[CHINESE]izeOfFmap
        if (!al1db) {
            ctx.pipe->InitBuffer(ctx.queueAL1, 1, ctx.convTiling->aL1SpaceSize);
        } else {
            ctx.pipe->InitBuffer(ctx.queueAL1, DOUBLE_BUF, ctx.convTiling->aL1SpaceSize);
        }

        // [zj]ToDo[CHINESE]b[CHINESE]        if constexpr (!bL1DBFlag) {
            ctx.pipe->InitBuffer(
                ctx.queueBL1, 1, bl1Spacesize * sizeOfWeight);
        } else {
            ctx.pipe->InitBuffer(
                ctx.queueBL1, DOUBLE_BUF, bl1Spacesize * sizeOfWeight);
        }
    }

    __aicore__ inline void InitBuffer()
    {
        uint64_t cl0Spacesize = ctx.convTiling->mStep * ctx.convTiling->nL0;
        uint64_t bl1Spacesize = ctx.convTiling->nBL1 * ctx.convTiling->kBL1;

        // [zj][CHINESE]0al0bl0cl1al1b[CHINESE]ocalTensor
        InitBufferWithDoubleBuf(cl0Spacesize, bl1Spacesize);

        int8_t cl0db = (ctx.convTiling->pBufferFlag & 0x04) >> 2;
        uint8_t bufferNum = cl0db ? DOUBLE_BUF : 1;

        // [zj][CHINESE]1biasl0bias[CHINESE]ocalTensor
        if (ctx.convTiling->hasBias) {
            uint64_t biasl1Spacesize = ctx.convTiling->biasFullLoadFlag
                                           ? AlignB(ctx.singleCoreCo * sizeOfBias,
                                                    BLOCK_L0_N * sizeOfBias)
                                           : ctx.convTiling->nL0 * sizeOfBias;
            uint64_t biasBTSpacesize = ctx.convTiling->nL0 * sizeOfL0c;
            // [zj]ToDo[CHINESE]    #if defined(__DAV_L510__)
            biasBTSpacesize = 1 * 1024 / static_cast<uint64_t>(bufferNum);
    #endif
            printf("biasBTSpacesize: %ld\n", biasBTSpacesize);
            ctx.pipe->InitBuffer(ctx.queueBiasL1, 1, AlignB(biasl1Spacesize, C0_SIZE));
            ctx.pipe->InitBuffer(ctx.queueBiasBT, bufferNum, AlignB(biasBTSpacesize, BT_SIZE));
        }
        // [zj]ToDo[CHINESE]        if (ctx.convTiling->hasQuantPre) {
            uint64_t quantPreL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag
                                               ? AlignB(ctx.singleCoreCo * sizeOfQuantPre,
                                                        BLOCK_L0_N * sizeOfQuantPre)
                                               : ctx.convTiling->nL0 * sizeOfQuantPre;
            ctx.pipe->InitBuffer(ctx.queueQuantPreL1, 1, AlignB(quantPreL1SpaceSize, C0_SIZE));
    #if defined(__DAV_L510__)
            uint64_t quantPreFBSpacesize = ctx.convTiling->nL0 * sizeOfQuantPre;
            // [zj]ToDo[CHINESE]            quantPreFBSpacesize = 2 * 1024 / static_cast<uint64_t>(bufferNum);
            printf("quantPreFBSpacesize: %ld\n", quantPreFBSpacesize);
            ctx.pipe->InitBuffer(ctx.queueQuantPreFB, bufferNum, AlignB(quantPreFBSpacesize, FB_QUANT_PRE_SIZE));
    #endif
        }
        if (ctx.convTiling->hasReluPre) {
            uint64_t reluPreL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag
                                              ? AlignB(ctx.singleCoreCo * sizeOfReluPre,
                                                       BLOCK_L0_N * sizeOfReluPre)
                                              : ctx.convTiling->nL0 * sizeOfReluPre;
            ctx.pipe->InitBuffer(ctx.queueReluPreL1, 1, AlignB(reluPreL1SpaceSize, C0_SIZE));
    #if defined(__DAV_L510__)
            uint64_t reluPreFBSpacesize = ctx.convTiling->nL0 * sizeOfReluPre;
            // [zj]ToDo[CHINESE]            reluPreFBSpacesize = 1 * 1024 / static_cast<uint64_t>(bufferNum);
            printf("reluPreFBSpacesize: %ld\n", reluPreFBSpacesize);
            ctx.pipe->InitBuffer(ctx.queueReluPreFB, bufferNum, AlignB(reluPreFBSpacesize, FB_RELU_PRE_SIZE));
    #endif
        }
        if (ctx.convTiling->hasEltAntiQuant) {
            uint64_t eltAntiQuantL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag
                                                   ? AlignB(ctx.singleCoreCo * sizeOfEltAntiQuant,
                                                            BLOCK_L0_N * sizeOfEltAntiQuant)
                                                   : ctx.convTiling->nL0 * sizeOfEltAntiQuant;
            ctx.pipe->InitBuffer(ctx.queueEltAntiQuantL1, 1, AlignB(eltAntiQuantL1SpaceSize, C0_SIZE));
    #if defined(__DAV_L510__)
            uint64_t eltAntiQuantFBSpacesize = ctx.convTiling->nL0 * sizeOfEltAntiQuant;
            // [zj]ToDo[CHINESE]            eltAntiQuantFBSpacesize = 1 * 1024 / static_cast<uint64_t>(bufferNum);
            printf("eltAntiQuantFBSpacesize: %ld\n", eltAntiQuantFBSpacesize);
            ctx.pipe->InitBuffer(ctx.queueEltAntiQuantFB, bufferNum,
                                      AlignB(eltAntiQuantFBSpacesize, FB_ELT_ANTIQ_SIZE));
    #endif
        }
        if (ctx.convTiling->hasQuantPost) {
            uint64_t quantPostL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag
                                                ? AlignB(ctx.singleCoreCo * sizeOfQuantPost,
                                                         BLOCK_L0_N * sizeOfQuantPost)
                                                : ctx.convTiling->nL0 * sizeOfQuantPost;
            ctx.pipe->InitBuffer(ctx.queueQuantPostL1, 1, AlignB(quantPostL1SpaceSize, C0_SIZE));
    #if defined(__DAV_L510__)
            uint64_t quantPostFBSpacesize = ctx.convTiling->nL0 * sizeOfQuantPost;
            // [zj]ToDo[CHINESE]            quantPostFBSpacesize = 2 * 1024 / static_cast<uint64_t>(bufferNum);
            printf("quantPostFBSpacesize: %ld\n", quantPostFBSpacesize);
            ctx.pipe->InitBuffer(ctx.queueQuantPostFB, bufferNum,
                                      AlignB(quantPostFBSpacesize, FB_QUANT_POST_SIZE));
    #endif
        }
        if (ctx.convTiling->hasEltwise) {
            uint64_t eltwiseL1SpaceSize = 0;
            if (ctx.eltwiseBrcEn) {
                eltwiseL1SpaceSize = ctx.convTiling->fixpParamsFullLoadFlag
                                         ? AlignB(ctx.singleCoreCo * sizeOfEltwise,
                                                  BLOCK_L0_N * sizeOfEltwise)
                                         : ctx.convTiling->nL0 * sizeOfEltwise;
            } else {
                eltwiseL1SpaceSize = ctx.convTiling->mStep * ctx.convTiling->nL0 * sizeOfEltwise;
            }
            ctx.pipe->InitBuffer(ctx.queueEltwiseL1, bufferNum, AlignB(eltwiseL1SpaceSize, C0_SIZE));
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
        // [CHINESE]ools[CHINESE]        loadBiasL1Ins.SetParams(ctx);
        loadQuantPreL1Ins.SetParams(ctx);
        loadReluPreL1Ins.SetParams(ctx);
        loadEltAntiQuantL1Ins.SetParams(ctx);
        loadQuantPostL1Ins.SetParams(ctx);
        loadEltwiseL1Ins.SetParams(ctx);
        loadEltwiseNonBrcL1Ins.SetParams(ctx);
        loadBL1Ins.SetParams(ctx);
        loadAl1Ins.SetParams(ctx);
        loadAL0Ins.SetParams(ctx);
        loadBL0Ins.SetParams(ctx);
        madIns.SetParams(ctx);
        loadBiasBTIns.SetParams(ctx);
        loadQuantPreFBIns.SetParams(ctx);
        loadReluPreFBIns.SetParams(ctx);
        loadEltAntiQuantFBIns.SetParams(ctx);
        loadQuantPostFBIns.SetParams(ctx);
        copyOutIns.SetParams(ctx);
    }



private:

    __aicore__ inline void InitCoDirectionValue()
    {
        ctx.nBL1Tail = ctx.singleCoreCo % ctx.convTiling->nBL1;
        ctx.nBL1Tail = ctx.nBL1Tail == 0 ? ctx.convTiling->nBL1 : ctx.nBL1Tail;
        ctx.nL0Tail = ctx.nBL1Tail % ctx.convTiling->nL0;
        ctx.nL0Tail = ctx.nL0Tail == 0 ? ctx.convTiling->nL0 : ctx.nL0Tail;

        if constexpr (!hasNL1IterFlag) {
            ctx.maxNBL1Iter = 0;
            ctx.ddr2l1LoopN = 1;
        } else {
            ctx.ddr2l1LoopN = CeilDIV(ctx.singleCoreCo, ctx.convTiling->nBL1);
            ctx.maxNBL1Iter = ctx.ddr2l1LoopN - 1;
        }

        if constexpr (!hasNL0IterFlag) {
            ctx.l12l0LoopN = 1;
            ctx.maxNL0Iter = 0;
        } else if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            ctx.l12l0LoopN = ctx.nBL1Iter == ctx.maxNBL1Iter ?
                CeilDIV(ctx.nBL1Tail, ctx.convTiling->nL0) : ctx.convTiling->multiNBL1;
        }

        ctx.currentNBL1 = ctx.nBL1Tail;
    }

    __aicore__ inline void CalcCoDirectionVar()
    {
        if constexpr (hasNL0IterFlag) {
            ctx.l12l0LoopN = ctx.nBL1Iter == ctx.maxNBL1Iter ?
                CeilDIV(ctx.nBL1Tail, ctx.convTiling->nL0) : ctx.convTiling->multiNBL1;
            ctx.maxNL0Iter = ctx.l12l0LoopN - 1;
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
        ctx.al1 = ctx.queueAL1.template AllocTensor<FmapT>();
        loadAl1Ins.LoadAL1(ctx);
        ctx.mAL1UpdateFlag = false;
        ctx.queueAL1.EnQue(ctx.al1);
        ctx.loadAL1Flag = false;  // Only k directrion can be reloaded in LoopK
    }

    __aicore__ inline void LoadAL1BaseMoudle(TempIters& tempIters)
    {
        ctx.al1 = ctx.queueAL1.template AllocTensor<FmapT>();
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            loadAl1Ins.LoadAL1(ctx, tempIters.kAL1Iter, tempIters.mAL1Iter, tempIters.batchIter);
        } else {
            loadAl1Ins.LoadAL1(ctx, tempIters.kAL1Iter, tempIters.woAL1Iter, tempIters.hoAL1Iter, tempIters.batchIter);
        }
        ctx.queueAL1.EnQue(ctx.al1);
        ctx.loadAL1Flag = false;  // Only k directrion can be reloaded in LoopK
    }

    __aicore__ inline void LoadBL1BaseMoudle()
    {
        ctx.bl1 = ctx.queueBL1.template AllocTensor<WeightT>();
        if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            // CrossCoreWaitFlag<CV_ENHANCE_MODE, PIPE_MTE1>(ctx.vecId + CV_SYNC_ID_MTE3_MTE1);
            ctx.loadUB2L1Iter++;
        } else {
            loadBL1Ins.LoadBL1(ctx);
        }
        ctx.queueBL1.EnQue(ctx.bl1);
        ctx.loadBL1Flag = false;  // Only k directrion can be reloaded in LoopK
    }

    __aicore__ inline void LoadBL1BaseMoudle(TempIters& tempIters)
    {
        ctx.bl1 = ctx.queueBL1.template AllocTensor<WeightT>();
        loadBL1Ins.LoadBL1(ctx, tempIters.kBL1Iter, tempIters.nBL1Iter);
        ctx.queueBL1.EnQue(ctx.bl1);
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
        // [zj]ToDo[CHINESE]        printf("isFirst: %d, iter: %d\n", isFirst, ctx.iter);
        if (ctx.iter >= 1) {
            wait_flag(PIPE_M, PIPE_MTE1, 0x0);
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
            if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                // OptGroupSyncSet();
            }
        }
        set_flag(PIPE_MTE1, PIPE_M, 0x0);
        wait_flag(PIPE_MTE1, PIPE_M, 0x0);
        madIns.Mad(ctx);
        set_flag(PIPE_M, PIPE_MTE1, 0x0);
        ctx.kIter++;
    }

    __aicore__ inline void ReduceKOpenL0APingPong(uint16_t& al0PingPongFlag, bool isFirst)
    {
        // [zj]ToDo[CHINESE]        if (ctx.iter >= DOUBLE_BUF) {
            wait_flag(PIPE_M, PIPE_MTE1, al0PingPongFlag & 0x1);
        }
        ctx.al0 =
            ctx.wholeAl0Tensor[(al0PingPongFlag & 0x1) * L0A_HALF_SIZE / sizeOfFmap];
        ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
        loadAL0Ins.LoadAL0(ctx, isFirst);

        if (!((ctx.convTiling->kBL1 == ctx.convTiling->kL0) && (!ctx.loadBL0Flag))) {
            ctx.bl0 = ctx.wholeBl0Tensor;
            ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
            loadBL0Ins.LoadBL0(ctx, isFirst);
            if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                // OptGroupSyncSet();
            }
        }
        set_flag(PIPE_MTE1, PIPE_M, al0PingPongFlag & 0x1);
        wait_flag(PIPE_MTE1, PIPE_M, al0PingPongFlag & 0x1);
        madIns.Mad(ctx);
        set_flag(PIPE_M, PIPE_MTE1, al0PingPongFlag & 0x1);
        al0PingPongFlag += 1;
        ctx.kIter++;
    }

    __aicore__ inline void ReduceKOpenL0BPingPong(uint16_t& bl0PingPongFlag, bool isFirst)
    {
        // [zj]ToDo[CHINESE]        printf("bl0PingPongFlag: %u, isFirst: %d, iter: %d\n", bl0PingPongFlag, isFirst, ctx.iter);
        if (ctx.iter >= DOUBLE_BUF) {
            wait_flag(PIPE_M, PIPE_MTE1, (bl0PingPongFlag & 0x1));
        }
        if (!((ctx.convTiling->kAL1 == ctx.convTiling->kL0) && (!ctx.loadAL0Flag))) {
            ctx.al0 = ctx.wholeAl0Tensor;
            ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
            loadAL0Ins.LoadAL0(ctx, isFirst);
        }

        ctx.bl0 =
            ctx.wholeBl0Tensor[(bl0PingPongFlag & 0x1) * L0B_HALF_SIZE / sizeOfWeight];
        ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
        loadBL0Ins.LoadBL0(ctx, isFirst);
        if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            // OptGroupSyncSet();
        }
        set_flag(PIPE_MTE1, PIPE_M, bl0PingPongFlag & 0x1);
        wait_flag(PIPE_MTE1, PIPE_M, bl0PingPongFlag & 0x1);
        madIns.Mad(ctx);
        set_flag(PIPE_M, PIPE_MTE1, bl0PingPongFlag & 0x1);
        bl0PingPongFlag += 1;
        ctx.kIter++;
    }

    __aicore__ inline void ReduceKOpenL0AL0BPingPong(uint16_t& al0PingPongFlag, bool isFirst)
    {
        ctx.al0 =
            ctx.wholeAl0Tensor[(al0PingPongFlag & 0x1) * L0A_HALF_SIZE / sizeOfFmap];
        ctx.bl0 =
            ctx.wholeBl0Tensor[(al0PingPongFlag & 0x1) * L0B_HALF_SIZE / sizeOfWeight];
        // [zj]ToDo[CHINESE]        if (ctx.iter >= DOUBLE_BUF) {
            wait_flag(PIPE_M, PIPE_MTE1, al0PingPongFlag & 0x1);
        }
        ctx.kAL0Iter = ctx.kIter % ctx.multiKAL1;
        loadAL0Ins.LoadAL0(ctx, isFirst);
        ctx.kBL0Iter = ctx.kIter % ctx.multiKBL1;
        loadBL0Ins.LoadBL0(ctx, isFirst);
        if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            // OptGroupSyncSet();
        }
        set_flag(PIPE_MTE1, PIPE_M, al0PingPongFlag & 0x1);
        wait_flag(PIPE_MTE1, PIPE_M, al0PingPongFlag & 0x1);
        madIns.Mad(ctx);
        set_flag(PIPE_M, PIPE_MTE1, al0PingPongFlag & 0x1);
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
        loadAL0Ins.SetM(ctx, ctx.currentML0Align, currentML0);
        loadBL0Ins.SetN(ctx, ctx.currentNL0Align);
        loadBiasL1Ins.SetN(ctx, currentNL0);
        loadQuantPreL1Ins.SetN(ctx, currentNL0);
        loadReluPreL1Ins.SetN(ctx, currentNL0);
        loadEltAntiQuantL1Ins.SetN(ctx, currentNL0);
        loadQuantPostL1Ins.SetN(ctx, currentNL0);
        loadEltwiseL1Ins.SetN(ctx, currentNL0);
        loadEltwiseNonBrcL1Ins.SetMN(ctx, currentML0, ctx.currentNL0Align);
        loadBiasBTIns.SetN(ctx, ctx.currentNL0Align);
        loadQuantPreFBIns.SetN(ctx, ctx.currentNL0Align);
        loadReluPreFBIns.SetN(ctx, ctx.currentNL0Align);
        loadEltAntiQuantFBIns.SetN(ctx, ctx.currentNL0Align);
        loadQuantPostFBIns.SetN(ctx, ctx.currentNL0Align);
        madIns.SetMN(ctx, ctx.currentML0Align, currentML0, ctx.currentNL0Align);
        if constexpr (formatC == ConvFormat::NC1HWC0) {
            copyOutIns.SetMN(ctx, currentML0, ctx.currentNL0Align);
        } else {
            copyOutIns.SetMN(ctx, currentML0, currentNL0);
        }
    }

    __aicore__ inline void FreeL1Tensor()
    {
        if (!ctx.kAL1fullload) {
            ctx.queueAL1.FreeTensor(ctx.al1);
        }
        if (!ctx.kBL1fullload) {
            ctx.queueBL1.FreeTensor(ctx.bl1);
        }
        if (ctx.enableBias) {
            if (!ctx.convTiling->biasFullLoadFlag) {
                ctx.queueBiasL1.FreeTensor(ctx.biasL1);
            }
            ctx.queueBiasBT.FreeTensor(ctx.biasBT);
        }
        // [zj][CHINESE]        if (ctx.enableQuantPre) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueQuantPreL1.FreeTensor(ctx.quantPreL1);
            }
            ctx.queueQuantPreFB.FreeTensor(ctx.quantPreFB);
        }
        if (ctx.enableReluPre) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueReluPreL1.FreeTensor(ctx.reluPreL1);
            }
            ctx.queueReluPreFB.FreeTensor(ctx.reluPreFB);
        }
        if (ctx.enableEltAntiQuant) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueEltAntiQuantL1.FreeTensor(ctx.eltAntiQuantL1);
            }
            ctx.queueEltAntiQuantFB.FreeTensor(ctx.eltAntiQuantFB);
        }
        if (ctx.enableQuantPost) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueQuantPostL1.FreeTensor(ctx.quantPostL1);
            }
            ctx.queueQuantPostFB.FreeTensor(ctx.quantPostFB);
        }
        if (ctx.enableEltwise && ctx.eltwiseBrcEn) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                ctx.queueEltwiseL1.FreeTensor(ctx.eltwiseL1);
            }
        }
    }

    __aicore__ inline void InitHoDirectionValue()
    {
        // Ho[CHINESE]1[CHINESE]0[CHINESE]        ctx.hoAL1Tail = ctx.singleCoreHo % ctx.convTiling->hoL1;
        ctx.hoAL1Tail = ctx.hoAL1Tail == 0 ? ctx.convTiling->hoL1 : ctx.hoAL1Tail;
        ctx.currentHoL1 = ctx.hoAL1Tail;
        ctx.hoL0Tail = ctx.currentHoL1 % ctx.convTiling->hoL0;
        ctx.hoL0Tail = ctx.hoL0Tail == 0 ? ctx.convTiling->hoL0 : ctx.hoL0Tail;
        ctx.currentHoL0 = ctx.hoL0Tail;

        // Ho[CHINESE]        if constexpr (!hasHL1IterFlag) {
            ctx.ddr2l1LoopH = 1; // [CHINESE]            ctx.maxHoL1Iter = 0; // [CHINESE]- 1[CHINESE]        } else {
            ctx.ddr2l1LoopH = CeilDIV(ctx.singleCoreHo, ctx.convTiling->hoL1);
            ctx.maxHoL1Iter = ctx.ddr2l1LoopH - 1;
        }
    }

    __aicore__ inline void InitWoDirectionValue()
    {
        // Wo[CHINESE]1[CHINESE]0[CHINESE]        ctx.woAL1Tail = ctx.singleCoreWo % ctx.convTiling->woL1;
        ctx.woAL1Tail = ctx.woAL1Tail == 0 ?  ctx.convTiling->woL1 : ctx.woAL1Tail;
        ctx.currentWoL1 = ctx.woAL1Tail;
        ctx.maxWoL1Iter = 0;
        ctx.woL0Tail = ctx.currentWoL1 % ctx.convTiling->woL0;
        ctx.woL0Tail = ctx.woL0Tail == 0 ? ctx.convTiling->woL0 : ctx.woL0Tail;
        ctx.currentWoL0 = ctx.woL0Tail;
        ctx.maxWoL0Iter = 0;
        ctx.l12l0LoopW = 1;

        // Wo[CHINESE]        if constexpr (hasWL1IterFlag) {
            ctx.maxWoL1Iter = CeilDIV(ctx.singleCoreWo, ctx.convTiling->woL1) - 1;
        }

        if (hasWL0IterFlag && ctx.convTiling->hoL0 > 1 && ctx.woAL1Tail % BLOCK_L0_N > 0 &&
            CeilDIV(ctx.woAL1Tail, ctx.convTiling->woL0) > 1) {
            ctx.woL1SmallTail = ctx.woAL1Tail % ctx.convTiling->woL0;
            ctx.woAL1Tail = (ctx.woAL1Tail / ctx.convTiling->woL0) * ctx.convTiling->woL0;
        }
        
        if (ctx.woL1SmallTail > 0) {
            ctx.maxWoL1Iter += 1;
        }
        ctx.ddr2l1LoopW = ctx.maxWoL1Iter + 1;
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
                ctx.currentWoL1 = ctx.woAL1Iter == ctx.maxWoL1Iter ?
                    ctx.woAL1Tail : ctx.convTiling->woL1;
            }
        }
        if constexpr (hasWL0IterFlag) {
            ctx.woL0Tail = ctx.currentWoL1 % ctx.convTiling->woL0;
            ctx.woL0Tail = ctx.woL0Tail == 0 ? ctx.convTiling->woL0 : ctx.woL0Tail;
            ctx.maxWoL0Iter = CeilDIV(ctx.currentWoL1, ctx.convTiling->woL0) - 1;
            ctx.l12l0LoopW = ctx.maxWoL0Iter + 1;
        }
    }

    __aicore__ inline void CalcHoDirectionVar()
    {
        if constexpr (hasHL1IterFlag) {
            ctx.currentHoL1 = ctx.hoAL1Iter == ctx.maxHoL1Iter ?
                ctx.hoAL1Tail : ctx.convTiling->hoL1;
        }
        
        if constexpr (hasHL0IterFlag) {
            ctx.hoL0Tail = ctx.currentHoL1 % ctx.convTiling->hoL0;
            ctx.hoL0Tail = ctx.hoL0Tail == 0 ? ctx.convTiling->hoL0 : ctx.hoL0Tail;
            ctx.maxHoL0Iter = CeilDIV(ctx.currentHoL1, ctx.convTiling->hoL0) - 1;
            ctx.l12l0LoopH = ctx.maxHoL0Iter + 1;
        }
    }

    __aicore__ inline void UpdateHoL0WoL0()
    {
        ctx.currentHoL0 = ctx.hoL0Iter == ctx.maxHoL0Iter ?
            ctx.hoL0Tail : ctx.convTiling->hoL0;
        ctx.currentWoL0 = ctx.woL0Iter == ctx.maxWoL0Iter ?
            ctx.woL0Tail : ctx.convTiling->woL0;
    }

    __aicore__ inline void FirstIterateImplHWMode()
    {
        ctx.nL0Iter = 0;
        ctx.hoAL1Iter = 0;
        ctx.woAL1Iter = 0;
        ctx.nBL1Iter = 0;
        ctx.woL0Iter = 0;
        ctx.hoL0Iter = 0;
        if constexpr (formatC == ConvFormat::NCDHW) {
            ctx.dOutIter = 0;
        }
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
            ctx.queueAL1.FreeTensor(ctx.al1);
        }

        if (IterateHWL1()) {
            return true;
        }

        if constexpr (formatC == ConvFormat::NCDHW) {
            ctx.dOutIter++;
            if (ctx.dOutIter == ctx.ddr2l1LoopD) {
                ctx.dOutIter = 0;
            } else {
                return true;
            }
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
            ctx.queueBL1.FreeTensor(ctx.bl1);
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
            ctx.queueBL1.FreeTensor(ctx.bl1);
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
            ctx.queueAL1.FreeTensor(ctx.al1);
        }

        if (IterateHWL1()) {
            return true;
        }

        if constexpr (formatC == ConvFormat::NCDHW) {
            ctx.dOutIter++;
            if (ctx.dOutIter == ctx.ddr2l1LoopD) {
                ctx.dOutIter = 0;
            } else {
                return true;
            }
        }

        ctx.batchIter++;
        if (likely(ctx.batchIter != ctx.ddr2l1LoopBatch)) {
            ctx.loadAL1Flag = true;
            ctx.loadBL1Flag = true;
            return true;
        }
        return false;
    }

    __aicore__ inline bool UpdateCommonItersHWModeMFirst(TempIters& tempIters)
    {
        tempIters.woAL1Iter = ctx.woAL1Iter;
        tempIters.hoAL1Iter = ctx.hoAL1Iter;
        tempIters.batchIter = ctx.batchIter;
        tempIters.nBL1Iter = ctx.nBL1Iter;

        tempIters.woAL1Iter += 1;
        if (tempIters.woAL1Iter <= ctx.maxWoL1Iter) {
            return false;
        }
        tempIters.woAL1Iter = 0;
        tempIters.hoAL1Iter += 1;
        if (tempIters.hoAL1Iter <= ctx.maxHoL1Iter) {
            return false;
        }
        tempIters.hoAL1Iter = 0;
        tempIters.batchIter += 1;
        if (tempIters.batchIter < ctx.ddr2l1LoopBatch) {
            return false;
        }
        tempIters.batchIter = 0;
        tempIters.nBL1Iter += 1;
        if (tempIters.nBL1Iter <= ctx.maxNBL1Iter) {
            return false;
        }
        tempIters.endTag = true;
        return true;
    }

    __aicore__ inline bool UpdateCommonItersHWModeNFirst(TempIters& tempIters)
    {
        tempIters.nBL1Iter = ctx.nBL1Iter;
        tempIters.woAL1Iter = ctx.woAL1Iter;
        tempIters.hoAL1Iter = ctx.hoAL1Iter;
        tempIters.batchIter = ctx.batchIter;

        tempIters.nBL1Iter += 1;
        if (tempIters.nBL1Iter <= ctx.maxNBL1Iter) {
            return false;
        }
        tempIters.nBL1Iter = 0;
        tempIters.woAL1Iter = ctx.woAL1Iter + 1;
        if (tempIters.woAL1Iter <= ctx.maxWoL1Iter) {
            return false;
        }
        tempIters.woAL1Iter = 0;
        tempIters.hoAL1Iter += 1;
        if (tempIters.hoAL1Iter <= ctx.maxHoL1Iter) {
            return false;
        }
        tempIters.hoAL1Iter = 0;
        tempIters.batchIter += 1;
        if (tempIters.batchIter < ctx.ddr2l1LoopBatch) {
            return false;
        }
        tempIters.endTag = true;
        return true;
    }

    __aicore__ inline void InitMDirectionValue()
    {
        // M[CHINESE]        ctx.mAL1Tail = ctx.singleCoreM % ctx.mAL1; // [zj]ToDo: [CHINESE]verlap
        ctx.mAL1Tail = ctx.mAL1Tail == 0 ? ctx.mAL1 : ctx.mAL1Tail;
        ctx.ddr2l1LoopM = CeilDIV(ctx.singleCoreM, ctx.mAL1);
        ctx.maxMAL1Iter = ctx.ddr2l1LoopM - 1;
        ctx.mAL0Tail = ctx.mAL1Tail % ctx.mL0;
        ctx.mAL0Tail = ctx.mAL0Tail == 0 ? ctx.mL0 : ctx.mAL0Tail;
        ctx.l12l0LoopM = CeilDIV(ctx.mAL1, ctx.mL0);
        ctx.maxML0Iter = ctx.l12l0LoopM - 1;
        printf("singleCoreM: %ld, mAL1: %ld, mAL1Tail: %ld, ddr2l1LoopM: %ld, maxMAL1Iter: %ld\n",
            ctx.singleCoreM,
            ctx.mAL1,
            ctx.mAL1Tail,
            ctx.ddr2l1LoopM,
            ctx.maxMAL1Iter);
    }

    __aicore__ inline void CalcMDirectionVar()
    {
        ctx.l12l0LoopM = ctx.mAL1Iter == ctx.maxMAL1Iter ?
            CeilDIV(ctx.mAL1Tail, ctx.mL0) : CeilDIV(ctx.mAL1, ctx.mL0);
        ctx.maxML0Iter = ctx.l12l0LoopM - 1;
        ctx.mAL1UpdateFlag = true; // [zj]ToDo[CHINESE]    }

    __aicore__ inline void FirstIterateImplMMode()
    {
        ctx.mL0Iter = 0;
        ctx.nL0Iter = 0;
        ctx.mAL1Iter = 0;
        ctx.nBL1Iter = 0;
        if constexpr (formatC == ConvFormat::NCDHW) {
            ctx.dOutIter = 0;
        }
        ctx.batchIter = 0;
        ctx.loadAL1Flag = true;
        ctx.loadBL1Flag = true;
        ctx.loadAL0Flag = true;
        ctx.loadBL0Flag = true;
        ctx.isFirstIterate = false;

        if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            ctx.loadUB2L1Iter = 0;
        }

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
            ctx.queueAL1.FreeTensor(ctx.al1);
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

        if constexpr (formatC == ConvFormat::NCDHW) {
            ctx.dOutIter++;
            if (ctx.dOutIter == ctx.ddr2l1LoopD) {
                ctx.dOutIter = 0;
            } else {
                return true;
            }
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
            ctx.queueBL1.FreeTensor(ctx.bl1);
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

        // if (ctx.kBL1fullload) {
        //     ctx.queueBL1.FreeTensor(ctx.bl1);
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
            ctx.queueAL1.FreeTensor(ctx.al1);
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

        if constexpr (formatC == ConvFormat::NCDHW) {
            ctx.dOutIter++;
            if (ctx.dOutIter == ctx.ddr2l1LoopD) {
                ctx.dOutIter = 0;
            } else {
                return true;
            }
        }

        ctx.batchIter++;
        if (likely(ctx.batchIter != ctx.ddr2l1LoopBatch)) {
            ctx.loadAL1Flag = true;
            ctx.loadBL1Flag = true;
            return true;
        }

        return false;
    }

    __aicore__ inline bool UpdateCommonItersMModeMFirst(TempIters& tempIters)
    {
        tempIters.mAL1Iter = ctx.mAL1Iter;
        tempIters.batchIter = ctx.batchIter;
        tempIters.nBL1Iter = ctx.nBL1Iter;

        tempIters.mAL1Iter += 1;
        if (tempIters.mAL1Iter <= ctx.maxMAL1Iter) {
            return false;
        }
        tempIters.mAL1Iter = 0;
        tempIters.batchIter += 1;
        if (tempIters.batchIter < ctx.ddr2l1LoopBatch) {
            return false;
        }
        tempIters.batchIter = 0;
        tempIters.nBL1Iter += 1;
        if (tempIters.nBL1Iter <= ctx.maxNBL1Iter) {
            return false;
        }
        tempIters.endTag = true;
        return true;
    }

    __aicore__ inline bool UpdateCommonItersMModeNFirst(TempIters& tempIters)
    {
        tempIters.nBL1Iter = ctx.nBL1Iter;
        tempIters.mAL1Iter = ctx.mAL1Iter;
        tempIters.batchIter = ctx.batchIter;

        tempIters.nBL1Iter += 1;
        if (tempIters.nBL1Iter <= ctx.maxNBL1Iter) {
            return false;
        }
        tempIters.nBL1Iter = 0;
        tempIters.mAL1Iter += 1;
        if (tempIters.mAL1Iter <= ctx.maxMAL1Iter) {
            return false;
        }
        tempIters.mAL1Iter = 0;
        tempIters.batchIter += 1;
        if (tempIters.batchIter < ctx.ddr2l1LoopBatch) {
            return false;
        }
        tempIters.endTag = true;
        return true;
    }

    template <bool sync = true>
    __aicore__ inline bool Iterate(bool enPartialSum = false)
    {
        if ASCEND_IS_AIC {
            if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
                return IterateImplMMode(enPartialSum);
            } else {
                return IterateImplHWMode(enPartialSum);
            }
        } else {
            // if constexpr (groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                // return OptGroupVecImpl();
            // } else {
                // return false;
            // }
            return false;
        }
        return false;
    }

    __aicore__ inline void ReduceOneK()
    {
        if (unlikely(ctx.loadBL1Flag)) {
            LoadBL1Moudle();
            ctx.bl1 = ctx.queueBL1.template DeQue<WeightT>();
        }
        // [zj]ToDo[CHINESE]        if (mPreLoadAFlag) {
            if (unlikely(ctx.loadAL1Flag)) {
                printf("iterate fmap preload setspr\n");
                loadAl1Ins.SetSpr(ctx);
                ctx.mAL1UpdateFlag = false;
                printf("iterate fmap preload deque\n");
                LocalTensor<FmapT> al1 = ctx.queueAL1.template DeQue<FmapT>();
                printf("[0]kAL1Iter: %ld, mAL1Iter: %ld, batchIter: %ld\n", ctx.kAL1Iter, ctx.mAL1Iter, ctx.batchIter);
                printf("[0]kAL1Iter: %ld, hoAL1Iter: %ld, woAL1Iter: %ld, batchIter: %ld\n", ctx.kAL1Iter, ctx.hoAL1Iter, ctx.woAL1Iter, ctx.batchIter);
                TempIters tempIters;
                if (!UpdateItersByFmap(tempIters, true)) {
                    printf("iterate fmap preload\n");
                    printf("[1]kAL1Iter: %ld, mAL1Iter: %ld, batchIter: %ld\n", tempIters.kAL1Iter, tempIters.mAL1Iter, tempIters.batchIter);
                    printf("[1]kAL1Iter: %ld, hoAL1Iter: %ld, woAL1Iter: %ld, batchIter: %ld\n", tempIters.kAL1Iter, tempIters.hoAL1Iter, tempIters.woAL1Iter, tempIters.batchIter);
                    if ((outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
                            (tempIters.kAL1Iter != ctx.kAL1Iter || tempIters.mAL1Iter != ctx.mAL1Iter ||
                            tempIters.batchIter != ctx.batchIter)) ||
                        (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
                            (tempIters.hoAL1Iter != ctx.hoAL1Iter || tempIters.woAL1Iter != ctx.woAL1Iter ||
                            tempIters.mAL1Iter != ctx.mAL1Iter || tempIters.batchIter != ctx.batchIter))) {
                        bool mAL1UpdateFlag = ctx.mAL1UpdateFlag;
                        ctx.mAL1UpdateFlag = true;
                        LoadAL1BaseMoudle(tempIters);
                        ctx.mAL1UpdateFlag = mAL1UpdateFlag;
                    }
                }
                ctx.al1 = al1;
                ctx.loadAL1Flag = false;
            }
        } else {
            if (unlikely(ctx.loadAL1Flag)) {
                LoadAL1Moudle();
                ctx.al1 = ctx.queueAL1.template DeQue<FmapT>();
            }
        }

        LoadL0Moudle(ctx.kL0FullLoadAl0PingPongFlag, ctx.kL0FullLoadBl0PingPongFlag, true);
    }

    __aicore__ inline bool UpdateCommonIters(TempIters& tempIters)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
            iterateMFirstFlag) {
            return UpdateCommonItersMModeMFirst(tempIters);
        } else if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
            iterateNFirstFlag) {
            return UpdateCommonItersMModeNFirst(tempIters);
        } else if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
            iterateMFirstFlag) {
            return UpdateCommonItersHWModeMFirst(tempIters);
        } else if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
            iterateNFirstFlag) {
            return UpdateCommonItersHWModeNFirst(tempIters);
        }
    }

    __aicore__ inline bool UpdateItersByFmap(TempIters& tempIters, bool updateIterByFmapTag)
    {
        if constexpr (outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
            tempIters.woAL1Iter = ctx.woAL1Iter;
            tempIters.hoAL1Iter = ctx.hoAL1Iter;
        } else {
            tempIters.mAL1Iter = ctx.mAL1Iter;
        }
        tempIters.batchIter = ctx.batchIter;
        ctx.kAL1Iter = ctx.kIter / ctx.multiKAL1;
        printf("kAL1Iter: %ld\n", ctx.kAL1Iter);
        tempIters.kAL1Iter = ctx.kAL1Iter + 1;
        if (tempIters.kAL1Iter <= ctx.maxKAL1Iter) {
            printf("kAL1Iter: %ld, maxKAL1Iter: %ld\n", tempIters.kAL1Iter, ctx.maxKAL1Iter);
            return false;
        }
        tempIters.kAL1Iter = 0;
        if (updateIterByFmapTag) {
            return UpdateCommonIters(tempIters);
        }
        if (tempIters.endTag) {
            return true;
        }
        return false;
    }

    __aicore__ inline bool UpdateItersByWeight(TempIters& tempIters, bool updateIterByFmapTag)
    {
        tempIters.nBL1Iter = ctx.nBL1Iter;
        ctx.kBL1Iter = ctx.kIter / ctx.multiKBL1;
        tempIters.kBL1Iter = ctx.kBL1Iter + 1;
        if (tempIters.kBL1Iter <= ctx.maxKBL1Iter) {
            return false;
        }
        tempIters.kBL1Iter = 0;
        if (!updateIterByFmapTag) {
            return UpdateCommonIters(tempIters);
        }
        if (tempIters.endTag) {
            return true;
        }
        return false;
    }

    __aicore__ inline void ReduceKPreload()
    {
        // state
        uint16_t al0PingPongFlag = 0;
        uint16_t bl0PingPongFlag = 0;
        bool isFirst = true;

        TempIters tempIters;
        // updateIterByFmapTag is true means fm update; false means weight update
        bool updateIterByFmapTag = ctx.convTiling->kAL1 > ctx.convTiling->kBL1;
        if (ctx.kAL1fullload && !ctx.kBL1fullload) {
            updateIterByFmapTag = false;
        } else if (!ctx.kAL1fullload && ctx.kBL1fullload) {
            updateIterByFmapTag = true;
        }
        if (ctx.loadAL1Flag || !ctx.kAL1fullload) {
            ctx.kAL1Iter = 0;
            // [zj]ToDo[CHINESE]            LoadAL1BaseMoudle();
            if (ctx.kAL1fullload) {
                ctx.al1 = ctx.queueAL1.template DeQue<FmapT>();
                ctx.loadAL0Flag = true;
            }
        }

        if (ctx.loadBL1Flag || !ctx.kBL1fullload) {
            ctx.kBL1Iter = 0;
            LoadBL1BaseMoudle();
            if (ctx.kBL1fullload) {
                ctx.bl1 = ctx.queueBL1.template DeQue<WeightT>();
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
                        ctx.queueAL1.FreeTensor(ctx.al1);  // always freeTensor
                    }
                    if (!UpdateItersByFmap(tempIters, updateIterByFmapTag)) {
                        LoadAL1BaseMoudle(tempIters);
                    }
                }
                if (ctx.kIter == ctx.lastLoopKAL1StartPos) {
                    ctx.queueAL1.FreeTensor(ctx.al1); // last iter of multiKAL1 only Free AL1 Tensor
                }
                if (ctx.kIter % ctx.multiKAL1 == 0) {
                    ctx.al1 = ctx.queueAL1.template DeQue<FmapT>();
                    ctx.loadAL0Flag = true;
                }
            }

            if (ctx.loadBL1Flag || !ctx.kBL1fullload) {
                if (ctx.kIter % ctx.multiKBL1 == 0 && ctx.kIter < ctx.lastLoopKBL1StartPos) {
                    if (ctx.kIter != 0) {
                        ctx.queueBL1.FreeTensor(ctx.bl1); // always freeTensor
                    }
                    if (!UpdateItersByWeight(tempIters, updateIterByFmapTag)) {
                        LoadBL1BaseMoudle(tempIters);
                    }
                }
                if (ctx.kIter == ctx.lastLoopKBL1StartPos) {
                    ctx.queueBL1.FreeTensor(ctx.bl1); // last iter of multiKBL1 only Free BL1 Tensor
                }
                if (ctx.kIter % ctx.multiKBL1 == 0) {
                    ctx.bl1 = ctx.queueBL1.template DeQue<WeightT>();
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
                    ctx.queueAL1.FreeTensor(ctx.al1);
                }
                LoadAL1Moudle();
                ctx.al1 = ctx.queueAL1.template DeQue<FmapT>();
                ctx.loadAL0Flag = true;
            }
            if (ctx.loadBL1Flag || (!ctx.kBL1fullload && ctx.kIter % ctx.multiKBL1 == 0)) {
                if (ctx.kIter != 0) {
                    ctx.queueBL1.FreeTensor(ctx.bl1);
                }
                LoadBL1Moudle();
                ctx.bl1 = ctx.queueBL1.template DeQue<WeightT>();
                ctx.loadBL0Flag = true;
            }

            LoadL0Moudle(al0PingPongFlag, bl0PingPongFlag, isFirst);
            isFirst = false;
        }
    }

    __aicore__ inline void IterateK()
    {
        SetMNBeforeIterateK();

        // [zj]ToDo[CHINESE]        if (ctx.enableBias) {
            if (!ctx.convTiling->biasFullLoadFlag) {
                ctx.biasL1 = ctx.queueBiasL1.template AllocTensor<BiasT>();
                loadBiasL1Ins.LoadChannelWiseL1(ctx, ctx.biasL1, ctx.biasgm);
                ctx.queueBiasL1.EnQue(ctx.biasL1);
                ctx.biasL1 = ctx.queueBiasL1.template DeQue<BiasT>();
            }
            ctx.biasBT = ctx.queueBiasBT.template AllocTensor<L0cT>();
            loadBiasBTIns.LoadBiasBt(ctx);
            ctx.queueBiasBT.EnQue(ctx.biasBT);
            ctx.biasBT = ctx.queueBiasBT.template DeQue<L0cT>();
        }
        // [zj]ToDo[CHINESE]        if (ctx.enableQuantPre) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                ctx.quantPreL1 = ctx.queueQuantPreL1.template AllocTensor<QuantPreT>();
                loadQuantPreL1Ins.LoadChannelWiseL1(ctx, ctx.quantPreL1, ctx.quantPreGm);
                ctx.queueQuantPreL1.EnQue(ctx.quantPreL1);
                ctx.quantPreL1 = ctx.queueQuantPreL1.template DeQue<QuantPreT>();
            }
    #if defined(__DAV_L510__)
            ctx.quantPreFB = ctx.queueQuantPreFB.template AllocTensor<QuantPreT>();
            loadQuantPreFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_PRE_QUANT, ctx.quantPreFB,
                                                        ctx.quantPreL1);
            ctx.queueQuantPreFB.EnQue(ctx.quantPreFB);
            ctx.quantPreFB = ctx.queueQuantPreFB.template DeQue<QuantPreT>();
    #endif
        }
        if (ctx.enableReluPre) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                ctx.reluPreL1 = ctx.queueReluPreL1.template AllocTensor<ReluPreT>();
                loadReluPreL1Ins.LoadChannelWiseL1(ctx, ctx.reluPreL1, ctx.reluPreGm);
                ctx.queueReluPreL1.EnQue(ctx.reluPreL1);
                ctx.reluPreL1 = ctx.queueReluPreL1.template DeQue<ReluPreT>();
            }
    #if defined(__DAV_L510__)
            ctx.reluPreFB = ctx.queueReluPreFB.template AllocTensor<ReluPreT>();
            loadReluPreFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_PRE_RELU, ctx.reluPreFB, ctx.reluPreL1);
            ctx.queueReluPreFB.EnQue(ctx.reluPreFB);
            ctx.reluPreFB = ctx.queueReluPreFB.template DeQue<ReluPreT>();
    #endif
        }
        if (ctx.enableEltAntiQuant) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                ctx.eltAntiQuantL1 =
                    ctx.queueEltAntiQuantL1.template AllocTensor<EltAntiQuantT>();
                loadEltAntiQuantL1Ins.LoadChannelWiseL1(ctx, ctx.eltAntiQuantL1, ctx.eltAntiQuantGm);
                ctx.queueEltAntiQuantL1.EnQue(ctx.eltAntiQuantL1);
                ctx.eltAntiQuantL1 = ctx.queueEltAntiQuantL1.template DeQue<EltAntiQuantT>();
            }
    #if defined(__DAV_L510__)
            ctx.eltAntiQuantFB = ctx.queueEltAntiQuantFB.template AllocTensor<EltAntiQuantT>();
            loadEltAntiQuantFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_ANTI_QUANT, ctx.eltAntiQuantFB,
                                                            ctx.eltAntiQuantL1);
            ctx.queueEltAntiQuantFB.EnQue(ctx.eltAntiQuantFB);
            ctx.eltAntiQuantFB = ctx.queueEltAntiQuantFB.template DeQue<EltAntiQuantT>();
    #endif
        }
        if (ctx.enableQuantPost) {
            if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                ctx.quantPostL1 = ctx.queueQuantPostL1.template AllocTensor<QuantPostT>();
                loadQuantPostL1Ins.LoadChannelWiseL1(ctx, ctx.quantPostL1, ctx.quantPostGm);
                ctx.queueQuantPostL1.EnQue(ctx.quantPostL1);
                ctx.quantPostL1 = ctx.queueQuantPostL1.template DeQue<QuantPostT>();
            }
    #if defined(__DAV_L510__)
            ctx.quantPostFB = ctx.queueQuantPostFB.template AllocTensor<QuantPostT>();
            loadQuantPostFBIns.LoadFixpParamFb(ctx, DST_MEM_BLOCK_POST_QUANT, ctx.quantPostFB,
                                                         ctx.quantPostL1);
            ctx.queueQuantPostFB.EnQue(ctx.quantPostFB);
            ctx.quantPostFB = ctx.queueQuantPostFB.template DeQue<QuantPostT>();
    #endif
        }
        if (ctx.enableEltwise) {
            if (ctx.eltwiseBrcEn) {
                if (!ctx.convTiling->fixpParamsFullLoadFlag) {
                    if constexpr (groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                        event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                        SetFlag<HardEvent::FIX_MTE2>(eventId);
                        WaitFlag<HardEvent::FIX_MTE2>(eventId);
                    }
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

    #if defined(__DAV_L510__)
        ctx.cl1 = ctx.queueCL1.template AllocTensor<OutputT>();
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

    #if defined(__DAV_L510__)
        ctx.queueCL1.EnQue(ctx.cl1);
        ctx.cl1 = ctx.queueCL1.template DeQue<OutputT>();
    #else
        ctx.queueCL0.EnQue(ctx.cl0);
        ctx.cl0 = ctx.queueCL0.template DeQue<L0cT>();
    #endif

        ctx.kIter = 0;
    }

    __aicore__ inline bool IterateImplHWMode(bool enPartialSum)
    {
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

    __aicore__ inline bool IterateImplMMode(bool enPartialSum)
    {
        if (ctx.isFirstIterate) {
            FirstIterateImplMMode();
            // [zj]ToDo[CHINESE]            if (mPreLoadAFlag) {
                if (ctx.maxKBL1Iter == 0 && ctx.maxNBL1Iter == 0) {
                    LoadBL1Moudle();
                    ctx.bl1 = ctx.queueBL1.template DeQue<WeightT>();
                    ctx.loadBL1Flag = false;
                }
                printf("first iterate fmap preload\n");
                ctx.kAL1Iter = ctx.kIter / ctx.multiKAL1;
                LoadAL1BaseMoudle();
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

};

}  // namespace conv2d

#endif // CONV2D_INTF_H