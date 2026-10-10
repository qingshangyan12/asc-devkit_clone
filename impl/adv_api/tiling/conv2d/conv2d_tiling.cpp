/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under
 * the terms and conditions of CANN Open Software License Agreement Version 2.0
 * (the "License"). Please refer to the License for details. You may not use
 * this file except in compliance with the License. THIS SOFTWARE IS PROVIDED ON
 * AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS
 * FOR A PARTICULAR PURPOSE. See LICENSE in the root of the software repository
 * for the full text of the License.
 */

/*!
 * \file conv2d_tiling.cpp
 * \brief
 */

#include <algorithm>
#include <cstdint>
#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "../../../lib/conv/conv2d/conv2d_tiling.h"
#include "../../../lib/conv/conv2d/conv2d_tiling_algorithm.h"
#include "../../../lib/conv/conv2d/conv2d_tiling_base.h"
#else
#include "../../../../include/adv_api/conv/conv2d/conv2d_tiling_algorithm.h"
#include "../../../../include/adv_api/conv/conv2d/conv2d_tiling_base.h"
#include "../../../../include/adv_api/conv/conv2d/conv2d_tiling.h"
#endif

namespace Conv2dTilingApi {

namespace {
bool IsBl1ByPassAlgo(ConvTilingAlgoType algoType)
{
    switch (algoType) {
        case ConvTilingAlgoType::TILING_1_CUT_H_L0_CUT_N_DIRECT_L0:
        case ConvTilingAlgoType::TILING_1_CUT_H_L0_CUT_KN_DIRECT_L0:
        case ConvTilingAlgoType::TILING_1_CUT_H_L0_CUT_KW_N_DIRECT_L0:
        case ConvTilingAlgoType::TILING_5_CUT_HW_CO_L0_CUT_KH_KW_KN_DIRECT_L0:
        case ConvTilingAlgoType::TILING_7_CUT_H_CI_CO_L0_CUT_KN_DIRECT_L0:
            return true;
        default:
            return false;
    }
}
}  // namespace

std::unique_ptr<ConvTilingAlgorithm> Conv2dTiling::CreateAlgorithm()
{
    switch (algoType_) {
        case ConvTilingAlgoType::TILING_1_CUT_H_L0_CUT_N: {
            auto algo = std::make_unique<Tiling1CutHL0CutN>();
            algo->isFilterInL1 = true;
            return algo;
        }
        case ConvTilingAlgoType::TILING_1_CUT_H_L0_CUT_KN: {
            auto algo = std::make_unique<Tiling1CutHL0CutKN>();
            algo->isFilterInL1 = true;
            return algo;
        }
        case ConvTilingAlgoType::TILING_1_CUT_H_L0_CUT_KW_N: {
            auto algo = std::make_unique<Tiling1CutHL0CutKwN>();
            algo->isFilterInL1 = true;
            return algo;
        }
        case ConvTilingAlgoType::TILING_2_CUT_CO_L0_CUT_KN: {
            auto algo = std::make_unique<Tiling2CutCoL0CutKN>();
            algo->isFilterInL1 = true;
            return algo;
        }
        case ConvTilingAlgoType::TILING_5_CUT_HW_CO_L0_CUT_KN: {
            auto algo = std::make_unique<Tiling5CutHWCoL0CutKN>();
            algo->isFilterInL1 = true;
            return algo;
        }
        case ConvTilingAlgoType::TILING_5_CUT_HW_CO_L0_CUT_KH_KW_KN: {
            auto algo = std::make_unique<Tiling5CutHWCoL0CutKhKwKN>();
            algo->isFilterInL1 = true;
            return algo;
        }
        case ConvTilingAlgoType::TILING_7_CUT_H_CI_CO_L0_CUT_KN: {
            auto algo = std::make_unique<Tiling7CutHCiCoL0CutKN>();
            algo->isFilterInL1 = true;
            return algo;
        }
        case ConvTilingAlgoType::TILING_1_CUT_H_L0_CUT_N_DIRECT_L0: {
            auto algo = std::make_unique<Tiling1CutHL0CutN>();
            algo->isFilterInL1 = false;
            return algo;
        }
        case ConvTilingAlgoType::TILING_1_CUT_H_L0_CUT_KN_DIRECT_L0: {
            auto algo = std::make_unique<Tiling1CutHL0CutKN>();
            algo->isFilterInL1 = false;
            return algo;
        }
        case ConvTilingAlgoType::TILING_1_CUT_H_L0_CUT_KW_N_DIRECT_L0: {
            auto algo = std::make_unique<Tiling1CutHL0CutKwN>();
            algo->isFilterInL1 = false;
            return algo;
        }
        case ConvTilingAlgoType::TILING_5_CUT_HW_CO_L0_CUT_KH_KW_KN_DIRECT_L0: {
            auto algo = std::make_unique<Tiling5CutHWCoL0CutKhKwKN>();
            algo->isFilterInL1 = false;
            return algo;
        }
        case ConvTilingAlgoType::TILING_7_CUT_H_CI_CO_L0_CUT_KN_DIRECT_L0: {
            auto algo = std::make_unique<Tiling7CutHCiCoL0CutKN>();
            algo->isFilterInL1 = false;
            return algo;
        }
        default:
            return nullptr;
    }
}

int64_t Conv2dTiling::Compute()
{
    algo_ = CreateAlgorithm();
    if (algo_ == nullptr) {
        return TILING_FAILED;
    }

    if (algo_->CalcTiling(bufferInfo_, convArgs_, innerTilingResult_) != TILING_SUCCESS) {
        return TILING_FAILED;
    }

    SetTilingResult();
    return TILING_SUCCESS;
}

int64_t Conv2dTiling::SetTilingResult()
{
    if (algo_ == nullptr) {
        return TILING_FAILED;
    }

    const L1SplitResult &l1 = algo_->GetL1SplitResult();
    const L0Frac &l0 = algo_->GetL0SplitResult();

    innerTilingResult_.set_hoL1(static_cast<uint32_t>(l1.l1FracHo));
    innerTilingResult_.set_woL1(static_cast<uint32_t>(l1.l1FracWo));
    innerTilingResult_.set_kAL1(static_cast<uint32_t>(l1.l1FracCi * convArgs_.kH * convArgs_.kW));
    innerTilingResult_.set_nBL1(static_cast<uint32_t>(l1.l1FracCo));

    innerTilingResult_.set_kL0(static_cast<uint32_t>(l0.k));
    innerTilingResult_.set_nL0(static_cast<uint32_t>(l0.co));
    innerTilingResult_.set_khL0(static_cast<uint32_t>(l0.kh));
    innerTilingResult_.set_kwL0(static_cast<uint32_t>(l0.kw));

    innerTilingResult_.set_hoL0(static_cast<uint32_t>(l1.l1FracHo));
    innerTilingResult_.set_woL0(static_cast<uint32_t>(l1.l1FracWo));

    innerTilingResult_.set_a1SpaceSize(
        static_cast<uint32_t>(l1.l1FracH * l1.l1FracW * l1.l1FracCi * dtypeSizeMap.at(convArgs_.featureMapType)));
    if (l1.l1FracCi >= convArgs_.singleCi && convArgs_.weightType == ConvCommonApi::ConvDtype::INT8 &&
        convArgs_.featureMapType == ConvCommonApi::ConvDtype::INT16) {
        innerTilingResult_.set_kBL1(static_cast<uint32_t>(
            Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs_.singleCi * convArgs_.kH * convArgs_.kW),
                static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs_.weightType)))));
        innerTilingResult_.set_b1SpaceSize(static_cast<uint32_t>(
            l1.l1FracCo *
            Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs_.singleCi * convArgs_.kH * convArgs_.kW),
                static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs_.weightType))) *
            dtypeSizeMap.at(convArgs_.weightType)));
    } else {
        innerTilingResult_.set_kBL1(static_cast<uint32_t>(l1.l1FracCi * convArgs_.kH * convArgs_.kW));
        innerTilingResult_.set_b1SpaceSize(static_cast<uint32_t>(
            l1.l1FracCi * l1.l1FracCo * convArgs_.kH * convArgs_.kW * dtypeSizeMap.at(convArgs_.weightType)));
    }
    innerTilingResult_.set_c0SpaceSize(
        static_cast<uint32_t>(l1.l1FracHo * l1.l1FracWo * l0.co * dtypeSizeMap.at(convArgs_.outputType)));

    innerTilingResult_.set_multiNBL1(static_cast<uint32_t>(
        Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(l1.l1FracCo), static_cast<uint64_t>(l0.co))));

    innerTilingResult_.set_cinAInCore(static_cast<uint32_t>(l1.l1FracCi));
    uint32_t alignSingleCi = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs_.singleCi),
        static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs_.featureMapType)));
    innerTilingResult_.set_cinATailInCore(
        static_cast<uint32_t>(alignSingleCi % l1.l1FracCi == 0 ? l1.l1FracCi : alignSingleCi % l1.l1FracCi));
    innerTilingResult_.set_cinBInCore(static_cast<uint32_t>(l1.l1FracCi));
    innerTilingResult_.set_cinBTailInCore(
        static_cast<uint32_t>(alignSingleCi % l1.l1FracCi == 0 ? l1.l1FracCi : alignSingleCi % l1.l1FracCi));
    // todo: 根据搬运数据量大小确认M order / N order.
    innerTilingResult_.set_iterateMNOrder(static_cast<uint8_t>(IterateOrder::NFirst));

    innerTilingResult_.set_mStep(l1.l1FracHo * l1.l1FracWo);  // 单位"个"
    innerTilingResult_.set_nStep(
        static_cast<uint32_t>(Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(l0.co), static_cast<uint64_t>(16))));
    const int64_t k0Weight = ALIGN_SIZE / dtypeSizeMap.at(convArgs_.weightType);
    innerTilingResult_.set_kStep(
        static_cast<uint32_t>(Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(l0.k), static_cast<uint64_t>(k0Weight))));
    return TILING_SUCCESS;
}

int64_t Conv2dTiling::GetTiling(optiling::TConv2DApiTiling &tiling)
{
    convArgs_.kHDilated = convArgs_.dilationH * (convArgs_.kH - 1) + 1;
    convArgs_.kWDilated = convArgs_.dilationW * (convArgs_.kW - 1) + 1;
    convArgs_.singleHi = (convArgs_.singleHo - 1) * convArgs_.strideH + convArgs_.kHDilated;
    convArgs_.singleWi = (convArgs_.singleWo - 1) * convArgs_.strideW + convArgs_.kWDilated;
    // 避免奇数需要跳搬.
    if (convArgs_.singleWo >= convArgs_.wOut) {
        convArgs_.singleWi = convArgs_.wIn;
    }
    convArgs_.singleCi = convArgs_.cIn;
    convArgs_.hOut = (convArgs_.hIn + convArgs_.padT + convArgs_.padB - convArgs_.kHDilated) / convArgs_.strideH + 1;
    convArgs_.wOut = (convArgs_.wIn + convArgs_.padL + convArgs_.padR - convArgs_.kWDilated) / convArgs_.strideW + 1;
    innerTilingResult_.set_orgHo(static_cast<uint64_t>(convArgs_.hOut));
    innerTilingResult_.set_orgWo(static_cast<uint64_t>(convArgs_.wOut));
    int64_t ret = TILING_FAILED;
    for (int32_t algo = static_cast<int32_t>(algoType_);
         algo < static_cast<int32_t>(ConvTilingAlgoType::TILING_ALGO_MAX);
         algo++) {
        algoType_ = static_cast<ConvTilingAlgoType>(algo);
        ret = Compute();
        if (ret == TILING_SUCCESS) {
            break;
        }
    }
    if (ret != TILING_SUCCESS) {
        return ret;
    }
    if (IsBl1ByPassAlgo(algoType_)) {
        innerTilingResult_.set_pBufferFlag(innerTilingResult_.get_pBufferFlag() | (1u << pBufferBl1ByPassPos));
    }
    SaveTilingResult(tiling);
    return TILING_SUCCESS;
}

int64_t Conv2dTiling::GetTiling(AscendC::tiling::TConv2DApiTiling &tiling)
{
    // AscendC::tiling::TConv2DApiTiling
    // 是新tiling框架的镜像结构(kernel_tiling.h生成, 字段与
    // optiling::TConv2DApiTiling一一对应的plain struct), 实现:
    // 先走optiling版计算, 再逐字段转换.
    optiling::TConv2DApiTiling optilingTiling;
    int64_t ret = GetTiling(optilingTiling);
    if (ret != TILING_SUCCESS) {
        return ret;
    }

    tiling.orgHi = optilingTiling.get_orgHi();
    tiling.orgWi = optilingTiling.get_orgWi();
    tiling.oriHixWi = optilingTiling.get_oriHixWi();
    tiling.orgHo = optilingTiling.get_orgHo();
    tiling.orgWo = optilingTiling.get_orgWo();
    tiling.singleCoreHo = optilingTiling.get_singleCoreHo();
    tiling.singleCoreWo = optilingTiling.get_singleCoreWo();
    tiling.orgCi = optilingTiling.get_orgCi();
    tiling.orgCo = optilingTiling.get_orgCo();
    tiling.singleCoreCi = optilingTiling.get_singleCoreCi();
    tiling.singleCoreCo = optilingTiling.get_singleCoreCo();
    tiling.kernelH = optilingTiling.get_kernelH();
    tiling.kernelW = optilingTiling.get_kernelW();
    tiling.kernelHxkernelW = optilingTiling.get_kernelHxkernelW();
    tiling.strideH = optilingTiling.get_strideH();
    tiling.strideW = optilingTiling.get_strideW();
    tiling.dilationH = optilingTiling.get_dilationH();
    tiling.dilationW = optilingTiling.get_dilationW();
    tiling.padTop = optilingTiling.get_padTop();
    tiling.padBottom = optilingTiling.get_padBottom();
    tiling.padLeft = optilingTiling.get_padLeft();
    tiling.padRight = optilingTiling.get_padRight();
    tiling.groups = optilingTiling.get_groups();
    tiling.hoL1 = optilingTiling.get_hoL1();
    tiling.woL1 = optilingTiling.get_woL1();
    tiling.kAL1 = optilingTiling.get_kAL1();
    tiling.kBL1 = optilingTiling.get_kBL1();
    tiling.nBL1 = optilingTiling.get_nBL1();
    tiling.hoL0 = optilingTiling.get_hoL0();
    tiling.woL0 = optilingTiling.get_woL0();
    tiling.kL0 = optilingTiling.get_kL0();
    tiling.nL0 = optilingTiling.get_nL0();
    tiling.khL0 = optilingTiling.get_khL0();
    tiling.kwL0 = optilingTiling.get_kwL0();
    tiling.pBufferFlag = optilingTiling.get_pBufferFlag();
    tiling.cinAInCore = optilingTiling.get_cinAInCore();
    tiling.cinATailInCore = optilingTiling.get_cinATailInCore();
    tiling.cinBInCore = optilingTiling.get_cinBInCore();
    tiling.cinBTailInCore = optilingTiling.get_cinBTailInCore();
    tiling.a1SpaceSize = optilingTiling.get_a1SpaceSize();
    tiling.b1SpaceSize = optilingTiling.get_b1SpaceSize();
    tiling.c0SpaceSize = optilingTiling.get_c0SpaceSize();
    tiling.multiNBL1 = optilingTiling.get_multiNBL1();
    tiling.mStep = optilingTiling.get_mStep();
    tiling.kStep = optilingTiling.get_kStep();
    tiling.nStep = optilingTiling.get_nStep();
    tiling.biasFullLoadFlag = optilingTiling.get_biasFullLoadFlag();
    tiling.fixpParamsFullLoadFlag = optilingTiling.get_fixpParamsFullLoadFlag();
    tiling.iterateMNOrder = optilingTiling.get_iterateMNOrder();
    tiling.isC04Flag = optilingTiling.get_isC04Flag();
    tiling.hasBias = optilingTiling.get_hasBias();
    tiling.hasPreQuant = optilingTiling.get_hasPreQuant();
    tiling.hasPreRelu = optilingTiling.get_hasPreRelu();
    tiling.hasPreClipRelu = optilingTiling.get_hasPreClipRelu();
    tiling.hasAntiQuant = optilingTiling.get_hasAntiQuant();
    tiling.hasEltwise = optilingTiling.get_hasEltwise();
    tiling.hasPostQuant = optilingTiling.get_hasPostQuant();
    tiling.hasPostRelu = optilingTiling.get_hasPostRelu();
    tiling.hasPostClipRelu = optilingTiling.get_hasPostClipRelu();
    tiling.hasPadding = optilingTiling.get_hasPadding();
    tiling.preQuantMode = optilingTiling.get_preQuantMode();
    tiling.preReluMode = optilingTiling.get_preReluMode();
    tiling.preClipReluMode = optilingTiling.get_preClipReluMode();
    tiling.antiQuantMode = optilingTiling.get_antiQuantMode();
    tiling.eltwiseOp = optilingTiling.get_eltwiseOp();
    tiling.eltwiseBrcEn = optilingTiling.get_eltwiseBrcEn();
    tiling.postQuantMode = optilingTiling.get_postQuantMode();
    tiling.postReluMode = optilingTiling.get_postReluMode();
    tiling.postClipReluMode = optilingTiling.get_postClipReluMode();
    tiling.paddingMode = optilingTiling.get_paddingMode();
    tiling.dualOutput = optilingTiling.get_dualOutput();
    return TILING_SUCCESS;
}

}  // namespace Conv2dTilingApi
