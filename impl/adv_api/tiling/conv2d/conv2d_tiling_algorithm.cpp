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
 * \file conv2d_tiling_algorithm.cpp
 * \brief conv2d L1/L0 cut strategy and tiling algorithms
 */

#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "../../../lib/conv/conv2d/conv2d_tiling_algorithm.h"
#include <algorithm>
#include <cstdint>
#else
#include "../../../../include/adv_api/conv/conv2d/conv2d_tiling_algorithm.h"
#include <algorithm>
#include <cstdint>
#endif

namespace Conv2dTilingApi {
using std::max;
using std::min;

// [hxb todo]: partial中的mping特殊判断修改. 当前没有mPing.
// [hxb todo]: 放开可以不用PingPong的场景.
int64_t L1CutStrategy::CalcFilterSize(
    const Conv2dArgs &convArgs, int64_t kH, int64_t kW, int64_t fracCi, int64_t fracCo, int64_t PingPongNum)
{
    return kH * kW *
           Conv3dTilingApi::AlignB(static_cast<uint64_t>(fracCi),
               static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.outputType))) *
           Conv3dTilingApi::AlignB(static_cast<uint64_t>(fracCo),
               static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.weightType))) *
           dtypeSizeMap.at(convArgs.weightType) * PingPongNum;
}

int64_t L1CutStrategy::CalcInputSize(
    const Conv2dArgs &convArgs, int64_t H, int64_t W, int64_t fracCi, int64_t PingPongNum)
{
    return H * W *
           Conv3dTilingApi::AlignB(static_cast<uint64_t>(fracCi),
               static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.weightType))) *
           dtypeSizeMap.at(convArgs.featureMapType) * PingPongNum;
}

int64_t L1CutStrategy::CalcEltSize(const Conv2dArgs &convArgs, int64_t Ho, int64_t Wo, int64_t fracCo)
{
    if (!convArgs.hasEltwise) {
        return 0;
    }
    return Ho * Wo *
           Conv3dTilingApi::AlignB(static_cast<uint64_t>(fracCo),
               static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.outputType))) *
           dtypeSizeMap.at(convArgs.eltwiseType) * L1_PINGPONG;
}

int64_t L1CutStrategy::CalcOutputSize(const Conv2dArgs &convArgs, int64_t Ho, int64_t Wo, int64_t fracCo)
{
    return Ho * Wo *
           Conv3dTilingApi::AlignB(static_cast<uint64_t>(fracCo),
               static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.outputType))) *
           dtypeSizeMap.at(convArgs.outputType) * L1_PINGPONG;
}

int64_t L1CutStrategy::CalcPartialSumSize(const Conv2dArgs &convArgs, int64_t Ho, int64_t Wo, int64_t fracCo)
{
    if (!isPartial) {
        return 0;
    }
    return Ho * Wo *
           Conv3dTilingApi::AlignB(static_cast<uint64_t>(fracCo),
               static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.outputType))) *
           dtypeSizeMap.at(convArgs.partialType) * L1_PARTIAL_PINGPONG;
}

int64_t L1CutStrategy::CalcL1ParamSize(const Conv2dArgs &convArgs, int64_t fracCo)
{
    int64_t auxDataSizePerchannel = sizeof(uint64_t) * 2 + sizeof(uint32_t) * 4;
    int64_t coAlign = ALIGN_SIZE / dtypeSizeMap.at(convArgs.outputType);
    int64_t paramSize =
        Conv3dTilingApi::AlignB(static_cast<uint64_t>(fracCo), static_cast<uint64_t>(coAlign)) * auxDataSizePerchannel;
    if (convArgs.isPerchannelPad) {
        paramSize += Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi),
            static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.weightType))) *
            dtypeSizeMap.at(convArgs.featureMapType);
    }
    return paramSize;
}

void L1CutStrategy::InitL1Support(const Conv2dArgs &convArgs)
{
    isL1SupportDual = (convArgs.dilationH == 1 && convArgs.dilationW == 1 && convArgs.kH != convArgs.strideH);
    // isL1SupportDual = (convArgs.batch >
    // static_cast<int64_t>(bufferInfo.blockDim)) ? false : isL1SupportDual; //
    // 这个实际不需要 input H < 2 * overlap不支持双源, overlap = 0 不支持双源.
    int64_t overlap = convArgs.kH > convArgs.strideH ? convArgs.kH - convArgs.strideH : 0;
    isL1SupportDual = ((convArgs.singleHi <= 2 * overlap) || (overlap == 0)) ? false : isL1SupportDual;
    isL1SupportDual = false;  // 当前Kernel 没有支持 L1Dual.
}

int64_t Tiling1CutHL0CutN::CalcTiling(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTilingResult)
{
    InitL1Support(convArgs);
    isPartial = false;
    if (CalcL0Split(bufferInfo, convArgs) == TILING_SUCCESS) {
        L0FracCo = L0FracResult.co;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            return TILING_SUCCESS;
        }
        isL1SupportDual = false;  // 非dual再尝试.
        L0FracCo = L0FracResult.co;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            return TILING_SUCCESS;
        }
    }
    return TILING_FAILED;
}

int64_t Tiling1CutHL0CutKN::CalcTiling(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTilingResult)
{
    InitL1Support(convArgs);
    isPartial = true;
    if (CalcL0Split(bufferInfo, convArgs) == TILING_SUCCESS) {
        L0FracCo = L0FracResult.co;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            return TILING_SUCCESS;
        }
        isL1SupportDual = false;  // 非dual再尝试.
        L0FracCo = L0FracResult.co;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            return TILING_SUCCESS;
        }
    }
    return TILING_FAILED;
}

int64_t Tiling1CutHL0CutKwN::CalcTiling(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTilingResult)
{
    InitL1Support(convArgs);
    isPartial = true;
    if (CalcL0Split(bufferInfo, convArgs) == TILING_SUCCESS) {
        L0FracCo = L0FracResult.co;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            return TILING_SUCCESS;
        }
        isL1SupportDual = false;  // 非dual再尝试.
        L0FracCo = L0FracResult.co;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            return TILING_SUCCESS;
        }
    }
    return TILING_FAILED;
}

int64_t Tiling2CutCoL0CutKN::CalcTiling(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTilingResult)
{
    isL1SupportDual = false;
    isPartial = true;
    if (CalcL0Split(bufferInfo, convArgs) != TILING_SUCCESS) {
        return TILING_FAILED;
    }
    L0FracCo = L0FracResult.co;
    if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
        L0FracResult.co = L0FracCo;
        return TILING_SUCCESS;
    }
    return TILING_FAILED;
}

int64_t Tiling5CutHWCoL0CutKN::CalcTiling(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTilingResult)
{
    InitL1Support(convArgs);
    isPartial = true;
    if (CalcL0Split(bufferInfo, convArgs) == TILING_SUCCESS) {
        L0FracCo = L0FracResult.co;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            L0FracResult.co = L0FracCo;
            return TILING_SUCCESS;
        }
        isL1SupportDual = false;  // 非dual再尝试
        L0FracCo = L0FracResult.co;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            L0FracResult.co = L0FracCo;
            return TILING_SUCCESS;
        }
    }
    return TILING_FAILED;
}

int64_t Tiling5CutHWCoL0CutKhKwKN::CalcTiling(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTilingResult)
{
    InitL1Support(convArgs);
    isPartial = true;
    if (CalcL0Split(bufferInfo, convArgs) == TILING_SUCCESS) {
        L0FracCo = L0FracResult.co;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            L0FracResult.co = L0FracCo;
            return TILING_SUCCESS;
        }
        isL1SupportDual = false;  // 非dual再尝试
        L0FracCo = L0FracResult.co;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            L0FracResult.co = L0FracCo;
            return TILING_SUCCESS;
        }
    }
    return TILING_FAILED;
}

int64_t Tiling7CutHCiCoL0CutKN::CalcTiling(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTilingResult)
{
    InitL1Support(convArgs);
    isPartial = true;
    if (CalcL0Split(bufferInfo, convArgs) == TILING_SUCCESS) {
        L0FracCo = L0FracResult.co;
        L0FracCi = L0FracResult.ci;
        if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) == TILING_SUCCESS) {
            L0FracResult.co = L0FracCo;
            L0FracResult.ci = L0FracCi;
            return TILING_SUCCESS;
        }
    }
    isL1SupportDual = false;  // 非dual再尝试.
    if (CalcL0Split(bufferInfo, convArgs) != TILING_SUCCESS) {
        return TILING_FAILED;
    }
    L0FracCo = L0FracResult.co;
    L0FracCi = L0FracResult.ci;
    if (CalcL1Split(bufferInfo, convArgs, innerTilingResult) != TILING_SUCCESS) {
        return TILING_FAILED;
    }
    L0FracResult.co = L0FracCo;
    L0FracResult.ci = L0FracCi;
    return TILING_SUCCESS;
}

void L0CutStrategy::InitL0AlignFrac(const Conv2dArgs &convArgs)
{
    L0AlignFrac_.ci = ALIGN_SIZE / dtypeSizeMap.at(convArgs.weightType);
    L0AlignFrac_.co = ALIGN_SIZE / dtypeSizeMap.at(convArgs.outputType);
}

int64_t L0CutN::CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs)
{
    InitL0AlignFrac(convArgs);
    minN0_ = Conv3dTilingApi::AlignB(static_cast<uint64_t>(L0MinFrac_.co), static_cast<uint64_t>(L0AlignFrac_.co));
    maxN0_ = min(static_cast<int64_t>(Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(convArgs.singleCo), static_cast<uint64_t>(L0AlignFrac_.co))),
        L0MaxFrac_.co);
    // 是否compact有区别.
    if (convArgs.weightType == ConvCommonApi::ConvDtype::INT8 &&
        convArgs.featureMapType == ConvCommonApi::ConvDtype::INT16) {
        minK0_ = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.kH * convArgs.kW * convArgs.singleCi),
            static_cast<uint64_t>(L0AlignFrac_.ci));
        maxK0_ = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.kH * convArgs.kW * convArgs.singleCi),
            static_cast<uint64_t>(L0AlignFrac_.ci));
    } else {
        minK0_ =
            convArgs.kH * convArgs.kW *
            Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi), static_cast<uint64_t>(L0AlignFrac_.ci));
        maxK0_ =
            convArgs.kH * convArgs.kW *
            Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi), static_cast<uint64_t>(L0AlignFrac_.ci));
    }
    L0FracResult.ci = minK0_ / (convArgs.kH * convArgs.kW);
    L0FracResult.kh = convArgs.kH;
    L0FracResult.kw = convArgs.kW;
    if (convArgs.isPerchannelPad) {
        int64_t padUnitSize = dtypeSizeMap.at(convArgs.featureMapType);
        maxK0_ = min(maxK0_, PT_SIZE / padUnitSize / L0_PINGPONG * convArgs.kH * convArgs.kW);
    }
    if (maxK0_ < minK0_) {
        return TILING_FAILED;
    }
    L0FracResult.k = minK0_;
    int64_t el0b = bufferInfo.l0BSize / L0_PINGPONG / dtypeSizeMap.at(convArgs.weightType);
    maxN0_ = min(ALIGN_FLOOR(el0b / minK0_, L0AlignFrac_.co), maxN0_);
    if (maxN0_ < minN0_) {
        return TILING_FAILED;
    }
    L0FracResult.co = maxN0_;
    return TILING_SUCCESS;
}

int64_t L0CutKNMaxN::CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs)
{
    InitL0AlignFrac(convArgs);
    minN0_ = Conv3dTilingApi::AlignB(static_cast<uint64_t>(L0MinFrac_.co), static_cast<uint64_t>(L0AlignFrac_.co));
    maxN0_ = min(static_cast<int64_t>(Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(convArgs.singleCo), static_cast<uint64_t>(L0AlignFrac_.co))),
        L0MaxFrac_.co);
    minK0_ = convArgs.kH * convArgs.kW *
             Conv3dTilingApi::AlignB(static_cast<uint64_t>(L0MinFrac_.ci), static_cast<uint64_t>(L0AlignFrac_.ci));
    maxK0_ = convArgs.kH * convArgs.kW *
             Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi), static_cast<uint64_t>(L0AlignFrac_.ci));
    if (convArgs.isPerchannelPad) {
        int64_t padUnitSize = dtypeSizeMap.at(convArgs.featureMapType);
        maxK0_ = min(maxK0_, PT_SIZE / padUnitSize / L0_PINGPONG * convArgs.kH * convArgs.kW);
    }
    int64_t el0b = bufferInfo.l0BSize / L0_PINGPONG / dtypeSizeMap.at(convArgs.weightType);  // PINGPONG = 2
    // minK-> maxN; maxN->maxK.
    int64_t nAvailable = ALIGN_FLOOR(el0b / minK0_, L0AlignFrac_.co);
    maxN0_ = min(maxN0_, nAvailable);
    if (maxN0_ < minN0_) {
        return TILING_FAILED;
    }
    maxK0_ = ALIGN_FLOOR(min(el0b / maxN0_, maxK0_), convArgs.kH * convArgs.kW * L0AlignFrac_.ci);
    if (maxK0_ < minK0_) {
        return TILING_FAILED;
    }
    L0FracResult.co = maxN0_;
    L0FracResult.ci = maxK0_ / (convArgs.kH * convArgs.kW);
    L0FracResult.kh = convArgs.kH;
    L0FracResult.kw = convArgs.kW;
    L0FracResult.k = maxK0_;
    return TILING_SUCCESS;
}

int64_t L0CutKN::CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs)
{
    InitL0AlignFrac(convArgs);
    minN0_ = Conv3dTilingApi::AlignB(static_cast<uint64_t>(L0MinFrac_.co), static_cast<uint64_t>(L0AlignFrac_.co));
    maxN0_ = min(static_cast<int64_t>(Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(convArgs.singleCo), static_cast<uint64_t>(L0AlignFrac_.co))),
        L0MaxFrac_.co);
    minK0_ = convArgs.kH * convArgs.kW *
             Conv3dTilingApi::AlignB(static_cast<uint64_t>(L0MinFrac_.ci), static_cast<uint64_t>(L0AlignFrac_.ci));
    maxK0_ = convArgs.kH * convArgs.kW *
             Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi), static_cast<uint64_t>(L0AlignFrac_.ci));
    if (convArgs.isPerchannelPad) {
        int64_t padUnitSize = dtypeSizeMap.at(convArgs.featureMapType);
        maxK0_ = min(maxK0_, PT_SIZE / padUnitSize / L0_PINGPONG * convArgs.kH * convArgs.kW);
    }
    if (maxK0_ < minK0_) {
        return TILING_FAILED;
    }
    int64_t el0b = bufferInfo.l0BSize / L0_PINGPONG / dtypeSizeMap.at(convArgs.weightType);  // PINGPONG = 2
    int64_t kAvailable = ALIGN_FLOOR(el0b / minN0_, ALIGN_SIZE);
    kAvailable = ALIGN_FLOOR(kAvailable, convArgs.kH * convArgs.kW * L0AlignFrac_.ci);
    maxK0_ = min(maxK0_, kAvailable);
    if (maxK0_ < minK0_) {
        return TILING_FAILED;
    }
    maxN0_ = ALIGN_FLOOR(min(el0b / maxK0_, maxN0_), L0AlignFrac_.co);
    if (maxN0_ < minN0_) {
        return TILING_FAILED;
    }
    L0FracResult.co = maxN0_;
    L0FracResult.ci = maxK0_ / (convArgs.kH * convArgs.kW);
    L0FracResult.kh = convArgs.kH;
    L0FracResult.kw = convArgs.kW;
    L0FracResult.k = maxK0_;
    return TILING_SUCCESS;
}

int64_t L0CutKwN::CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs)
{
    InitL0AlignFrac(convArgs);
    minN0_ = Conv3dTilingApi::AlignB(static_cast<uint64_t>(L0MinFrac_.co), static_cast<uint64_t>(L0AlignFrac_.co));
    maxN0_ = min(static_cast<int64_t>(Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(convArgs.singleCo), static_cast<uint64_t>(L0AlignFrac_.co))),
        L0MaxFrac_.co);
    minK0_ = convArgs.kH *
             Conv3dTilingApi::AlignB(static_cast<uint64_t>(L0MinFrac_.ci), static_cast<uint64_t>(L0AlignFrac_.ci));
    maxK0_ = convArgs.kH * convArgs.kW *
             Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi), static_cast<uint64_t>(L0AlignFrac_.ci));
    if (convArgs.isPerchannelPad) {
        int64_t padUnitSize = dtypeSizeMap.at(convArgs.featureMapType);
        maxK0_ = min(maxK0_, PT_SIZE / padUnitSize / L0_PINGPONG * convArgs.kH * convArgs.kW);
    }
    int64_t el0b = bufferInfo.l0BSize / L0_PINGPONG / dtypeSizeMap.at(convArgs.weightType);  // PINGPONG = 2
    int64_t kAvailable = ALIGN_FLOOR(el0b / minN0_, ALIGN_SIZE);
    kAvailable = ALIGN_FLOOR(kAvailable, convArgs.kH * L0AlignFrac_.ci);
    maxK0_ = min(maxK0_, kAvailable);
    if (maxK0_ < minK0_) {
        return TILING_FAILED;
    }
    maxN0_ = ALIGN_FLOOR(min(el0b / maxK0_, maxN0_), L0AlignFrac_.co);
    if (maxN0_ < minN0_) {
        return TILING_FAILED;
    }
    L0FracResult.co = maxN0_;
    L0FracResult.kh = convArgs.kH;
    int64_t maxCi = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi),
        static_cast<uint64_t>(L0AlignFrac_.ci));  // 算出最大的Ci,
    L0FracResult.ci = min(ALIGN_FLOOR(maxK0_ / (L0FracResult.kh * 1), L0AlignFrac_.ci), maxCi);
    L0FracResult.kw = min(maxK0_ / (L0FracResult.ci * L0FracResult.kh), convArgs.kW);
    L0FracResult.k = maxK0_;
    return TILING_SUCCESS;
}

int64_t L0CutKhKwKN::CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs)
{
    InitL0AlignFrac(convArgs);
    minN0_ = Conv3dTilingApi::AlignB(static_cast<uint64_t>(L0MinFrac_.co), static_cast<uint64_t>(L0AlignFrac_.co));
    maxN0_ = min(static_cast<int64_t>(Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(convArgs.singleCo), static_cast<uint64_t>(L0AlignFrac_.co))),
        L0MaxFrac_.co);
    minK0_ = convArgs.kH *
             Conv3dTilingApi::AlignB(static_cast<uint64_t>(L0MinFrac_.ci), static_cast<uint64_t>(L0AlignFrac_.ci));
    maxK0_ = convArgs.kH * convArgs.kW *
             Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi), static_cast<uint64_t>(L0AlignFrac_.ci));
    if (convArgs.isPerchannelPad) {
        int64_t padUnitSize = dtypeSizeMap.at(convArgs.featureMapType);
        maxK0_ = min(maxK0_, PT_SIZE / padUnitSize / L0_PINGPONG * convArgs.kH * convArgs.kW);
    }
    int64_t el0b = bufferInfo.l0BSize / L0_PINGPONG / dtypeSizeMap.at(convArgs.weightType);  // PINGPONG = 2
    int64_t kAvailable = ALIGN_FLOOR(el0b / minN0_, ALIGN_SIZE);
    kAvailable = ALIGN_FLOOR(kAvailable, convArgs.kH * L0AlignFrac_.ci);
    maxK0_ = min(maxK0_, kAvailable);
    if (maxK0_ < minK0_) {
        return TILING_FAILED;
    }
    maxN0_ = ALIGN_FLOOR(min(el0b / maxK0_, maxN0_), L0AlignFrac_.co);
    if (maxN0_ < minN0_) {
        return TILING_FAILED;
    }
    L0FracResult.co = maxN0_;
    int64_t maxCi = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi),
        static_cast<uint64_t>(L0AlignFrac_.ci));  // 算出最大的Ci,
    L0FracResult.ci = min(maxK0_, maxCi);
    int64_t KhxKw = maxK0_ / L0FracResult.ci;
    L0FracResult.kh = min(KhxKw, static_cast<int64_t>(convArgs.kH));
    L0FracResult.kw = min(KhxKw / L0FracResult.kh, convArgs.kW);
    L0FracResult.k = maxK0_;
    return TILING_SUCCESS;
}

void L1CutH::CalcL1DataSize(const Conv2dArgs &convArgs, TilingL1BufferSize &L1DataSize, int64_t L1FracH,
    int64_t L1FracW, int64_t L1FracHo, int64_t L1FracWo, int64_t L1FracCi, int64_t FracCoL0)
{
    L1DataSize.inputSize =
        CalcInputSize(convArgs, L1FracH, L1FracW, L1FracCi, (isL1SupportDual ? L1_PINGPONGPUNG : L1_PINGPONG));
    L1DataSize.eltwiseSize = CalcEltSize(convArgs, L1FracHo, L1FracWo, FracCoL0);
    L1DataSize.outputSize = CalcOutputSize(convArgs, L1FracHo, L1FracWo, FracCoL0);
    L1DataSize.partialSize = CalcPartialSumSize(convArgs, L1FracHo, L1FracWo, FracCoL0);
}

void L1CutCo::CalcL1DataSize(
    const Conv2dArgs &convArgs, TilingL1BufferSize &L1DataSize, int64_t L1FracCo, int64_t FracCoL0)
{
    L1DataSize.filterSize =
        CalcFilterSize(convArgs, convArgs.kH, convArgs.kW, convArgs.singleCi, L1FracCo, L1_PINGPONG);
    int64_t L1HOut = convArgs.singleHo;
    L1DataSize.eltwiseSize = CalcEltSize(convArgs, L1HOut, convArgs.singleWo, FracCoL0);
    L1DataSize.outputSize = CalcOutputSize(convArgs, L1HOut, convArgs.singleWo, FracCoL0);
    L1DataSize.partialSize = CalcPartialSumSize(convArgs, L1HOut, convArgs.singleWo, FracCoL0);
}

void L1CutHCiCo::CalcL1DataSize(const Conv2dArgs &convArgs, TilingL1BufferSize &L1DataSize, int64_t L1FracH,
    int64_t L1FracW, int64_t L1FracHo, int64_t L1FracWo, int64_t L1FracCi, int64_t FracCoL0)
{
    L1DataSize.inputSize =
        CalcInputSize(convArgs, L1FracH, L1FracW, L1FracCi, (isL1SupportDual ? L1_PINGPONGPUNG : L1_PINGPONG));
    L1DataSize.eltwiseSize = CalcEltSize(convArgs, L1FracHo, L1FracWo, FracCoL0);
    L1DataSize.outputSize = CalcOutputSize(convArgs, L1FracHo, L1FracWo, FracCoL0);
    L1DataSize.partialSize = CalcPartialSumSize(convArgs, L1FracHo, L1FracWo, FracCoL0);
}

int64_t L1CutH::CalcL1Split(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTiling)
{
    // 1. 计算ValidSize (param_szie + filter_size)
    int64_t filterL1Size =
        isFilterInL1 ? CalcFilterSize(convArgs, convArgs.kH, convArgs.kW, convArgs.singleCi, convArgs.singleCo) : 0;
    int64_t l1ParamSize = CalcL1ParamSize(convArgs, convArgs.singleCo);
    int64_t validSize = Conv3dTilingApi::AlignB(static_cast<uint64_t>(filterL1Size + l1ParamSize),
        static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));  // From CCE, 中间断开有问题

    // 2. 计算Base大小;
    int64_t baseL1FracH = convArgs.kHDilated;
    int64_t minH = max(convArgs.padT + max(convArgs.kHDilated - convArgs.strideH + 1LL, 1LL),
        convArgs.padB + max(convArgs.kHDilated - convArgs.strideH + 1LL, 1LL));
    int64_t baseFactor = 1;
    int64_t baseL1FracHo = 1;
    while (baseL1FracH < minH) {
        baseL1FracH += baseFactor * convArgs.strideH;
        baseL1FracHo += 1;
    }
    baseL1FracH = min(baseL1FracH, convArgs.singleHi);
    baseL1FracHo = min(baseL1FracHo,
        static_cast<int64_t>(
            Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(convArgs.singleHo), static_cast<uint64_t>(1))));
    int64_t baseL1FracW = convArgs.singleWi;
    int64_t baseL1FracWo = convArgs.singleWo;
    int64_t baseL1FracCi = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi),
        static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.featureMapType)));
    int64_t baseL1FracCo = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCo),
        static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.outputType)));
    TilingL1BufferSize baseL1DataSize;
    CalcL1DataSize(
        convArgs, baseL1DataSize, baseL1FracH, baseL1FracW, baseL1FracHo, baseL1FracWo, baseL1FracCi, L0FracCo);

    // 3. 计算Joint大小;
    int64_t jointL1FracH = convArgs.strideH;
    int64_t jointL1FracW = convArgs.singleWi;
    int64_t jointL1FracHo = 1;
    int64_t jointL1FracWo = convArgs.singleWo;
    int64_t jointL1FracCi = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi),
        static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.featureMapType)));
    TilingL1BufferSize jointL1DataSize;
    CalcL1DataSize(
        convArgs, jointL1DataSize, jointL1FracH, jointL1FracW, jointL1FracHo, jointL1FracWo, jointL1FracCi, L0FracCo);

    // 4. 计算最大x份Joint. 这里均已考虑过PINGPONG
    int64_t hOutRemain = convArgs.singleHo - baseL1FracHo;
    int64_t jointNumMax =
        hOutRemain > 0 ? Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(hOutRemain), static_cast<uint64_t>(1)) : 0;
    int64_t baseSize =
        baseL1DataSize.inputSize + baseL1DataSize.outputSize + baseL1DataSize.eltwiseSize + baseL1DataSize.partialSize;
    int64_t jointSize = jointL1DataSize.inputSize + jointL1DataSize.outputSize + jointL1DataSize.eltwiseSize +
                        jointL1DataSize.partialSize;
    if (static_cast<int64_t>(bufferInfo.l1Size) - validSize < baseSize) {
        return TILING_FAILED;
    }
    int64_t jointNum = min(static_cast<int64_t>(bufferInfo.l1Size - validSize - baseSize) / jointSize, jointNumMax);

    // 5. 对齐PingPong后再校验 validSize + inputSize + outputSize + eltwiseSize +
    // partialSize <= l1Size.
    int64_t inputPingPong = isL1SupportDual ? L1_PINGPONGPUNG : L1_PINGPONG;
    int64_t eltwisePingPong = L1_PINGPONG;
    int64_t partialPingPong = L1_PARTIAL_PINGPONG;
    int64_t inputSizeAlign =
        Conv3dTilingApi::AlignB(static_cast<uint64_t>(baseL1DataSize.inputSize + jointNum * jointL1DataSize.inputSize),
            static_cast<uint64_t>(ALIGN_SIZE * inputPingPong));
    int64_t validSizeAlign =
        Conv3dTilingApi::AlignB(static_cast<uint64_t>(validSize), static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));
    int64_t outputSizeAlign = Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(baseL1DataSize.outputSize + jointNum * jointL1DataSize.outputSize),
        static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));
    int64_t eltwiseSizeAlign = Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(baseL1DataSize.eltwiseSize + jointNum * jointL1DataSize.eltwiseSize),
        static_cast<uint64_t>(ALIGN_SIZE * eltwisePingPong));
    int64_t partialSizeAlign = Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(baseL1DataSize.partialSize + jointNum * jointL1DataSize.partialSize),
        static_cast<uint64_t>(ALIGN_SIZE * partialPingPong));
    if (inputSizeAlign + validSizeAlign + outputSizeAlign + eltwiseSizeAlign + partialSizeAlign >
        static_cast<int64_t>(bufferInfo.l1Size)) {
        return TILING_FAILED;  // Todo: 这里是否冗余?
                               // 因为在算baseSize和joitSize的时候其实是对齐了的.
    }
    l1SplitResult.l1FracCi = baseL1FracCi;
    l1SplitResult.l1FracCo = baseL1FracCo;
    // 注意, 这里partial要按2来算. partial 含有mpingpong.
    l1SplitResult.l1FracHo = (jointL1FracHo * jointNum + baseL1FracHo) * 1;
    l1SplitResult.l1FracH = jointL1FracH * jointNum + baseL1FracH;
    l1SplitResult.l1FracW = baseL1FracW;
    l1SplitResult.l1FracWo = baseL1FracWo;
    uint32_t pBuffer =
        (1 << pBufferL1APingPongPos) + (1 << pBufferL0BPingPongPos) + (isL1SupportDual ? (1 << pBufferL1DualPos) : 0);
    SetTilingFlag(convArgs, pBuffer, innerTiling);
    return TILING_SUCCESS;
}

bool L1CutCo::IsL1CoSplitFit(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t curL0FracCo, int64_t l1Co, int64_t validSize)
{
    TilingL1BufferSize baseL1DataSize;
    CalcL1DataSize(convArgs, baseL1DataSize, l1Co, curL0FracCo);
    int64_t eltPingPong = L1_PINGPONG;
    int64_t paramSize = CalcL1ParamSize(convArgs, l1Co);
    int64_t validSizeAlign = Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(validSize + paramSize), static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));
    int64_t filterSizeAlign = Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(baseL1DataSize.filterSize), static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));
    int64_t outputSizeAlign = Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(baseL1DataSize.outputSize), static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));
    int64_t eltwiseSizeAlign = Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(baseL1DataSize.eltwiseSize), static_cast<uint64_t>(ALIGN_SIZE * eltPingPong));
    int64_t partialSizeAlign = Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(baseL1DataSize.partialSize), static_cast<uint64_t>(ALIGN_SIZE * L1_PARTIAL_PINGPONG));
    return (validSizeAlign + filterSizeAlign + outputSizeAlign + eltwiseSizeAlign + partialSizeAlign <=
            static_cast<int64_t>(bufferInfo.l1Size));
}

bool L1CutCo::TryL1CoSplitFit(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t validSize,
    int64_t tryL0FracCo, int64_t tryL1Co, L1CoBestSplit &bestSplit)
{
    if (IsL1CoSplitFit(bufferInfo, convArgs, tryL0FracCo, tryL1Co, validSize)) {
        bestSplit.l0FracCo = tryL0FracCo;
        bestSplit.l1Co = tryL1Co;
        bestSplit.found = true;
        return true;
    }
    return false;
}

int64_t L1CutCo::CalcL1Split(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTiling)
{
    // 已知L0FracCo.
    // 1. 计算ValidSize (paramSize + inputSize)
    // kernel中，均为逐batch搬入L1. 没有多batch搬入L1, 所以tiling2 cut co
    // 不需要用singleBatch 去乘.
    int64_t validSize = CalcInputSize(convArgs,
        convArgs.singleHi,
        convArgs.singleWi,
        Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi),
            static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.weightType))));
    validSize =
        Conv3dTilingApi::AlignB(static_cast<uint64_t>(validSize), static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));
    int64_t coAlign = ALIGN_SIZE / dtypeSizeMap.at(convArgs.outputType);
    int64_t maxL1Co = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCo),
        static_cast<uint64_t>(coAlign));  // maxL1Co改成cOut/2? 要PingPong.
    L1CoBestSplit bestSplit;

    // 2. 枚举Co, 找到最大的Co, 使得切分得下. Loop1: 原L0FracCo,
    // 枚举l1Co(从大到小， 取首个能放的=最大的).
    for (int64_t l1Co = maxL1Co; l1Co >= L0FracCo; l1Co -= L0FracCo) {
        if (TryL1CoSplitFit(bufferInfo, convArgs, validSize, L0FracCo, l1Co, bestSplit)) {
            break;
        }
    }

    if (!bestSplit.found) {
        return TILING_FAILED;
    }

    // 3. SetL1Tiling result. 更新L0FracCo(由CalcTiling同步到L0FracResult)
    L0FracCo = bestSplit.l0FracCo;
    l1SplitResult.l1FracCi = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi),
        static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.featureMapType)));  // 不切
    l1SplitResult.l1FracCo = bestSplit.l1Co;                                            // 切Co.
    l1SplitResult.l1FracH = convArgs.singleHi;                                          // 不切.
    l1SplitResult.l1FracW = convArgs.singleWi;                                          // 不切.
    l1SplitResult.l1FracHo = convArgs.singleHo;                                         // 不切.
    l1SplitResult.l1FracWo = convArgs.singleWo;                                         // 不切.
    uint32_t pBuffer =
        (1 << pBufferL1BPingPongPos) + (1 << pBufferL0BPingPongPos) + (isL1SupportDual ? (1 << pBufferL1DualPos) : 0);
    SetTilingFlag(convArgs, pBuffer, innerTiling);
    return TILING_SUCCESS;
}

int64_t L1CutHWCo::CalcL1FeatureSize(const Conv2dArgs &convArgs, int64_t filterSize, int64_t inH, int64_t inW,
    int64_t outH, int64_t outW, int64_t l1FracCi, int64_t curL0FracCo)
{
    int64_t inputPingPong = (isL1SupportDual ? L1_PINGPONGPUNG : L1_PINGPONG);
    int64_t eltwisePingPong = L1_PINGPONG;
    int64_t partialPingPong = L1_PARTIAL_PINGPONG;
    int64_t L1HOut = outH;
    return Conv3dTilingApi::AlignB(static_cast<uint64_t>(filterSize), static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG)) +
           Conv3dTilingApi::AlignB(static_cast<uint64_t>(CalcInputSize(convArgs, inH, inW, l1FracCi, inputPingPong)),
               static_cast<uint64_t>(ALIGN_SIZE * inputPingPong)) +
           Conv3dTilingApi::AlignB(static_cast<uint64_t>(CalcOutputSize(convArgs, L1HOut, outW, curL0FracCo)),
               static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG)) +
           Conv3dTilingApi::AlignB(static_cast<uint64_t>(CalcEltSize(convArgs, L1HOut, outW, curL0FracCo)),
               static_cast<uint64_t>(ALIGN_SIZE * eltwisePingPong)) +
           Conv3dTilingApi::AlignB(static_cast<uint64_t>(CalcPartialSumSize(convArgs, L1HOut, outW, curL0FracCo)),
               static_cast<uint64_t>(ALIGN_SIZE * partialPingPong));
}

int64_t L1CutHWCo::GetL1OptimiFeatureSize(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t validSize,
    int64_t l1Co, int64_t curL0FracCo, int64_t &L1FracH, int64_t &L1FracW, int64_t &L1FracHOut, int64_t &L1FracWOut)
{
    int64_t outputHWithPadHead =
        Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(convArgs.padT), static_cast<uint64_t>(convArgs.strideH)) + 1;
    // tail是为了body不带pad? asc是否还需要.
    int64_t outputHWithoutPadTail = (convArgs.padT + convArgs.singleHi - convArgs.kHDilated) / convArgs.strideH + 1;
    int64_t outputHWithPadTail = convArgs.singleHo - outputHWithoutPadTail;
    int64_t minimalOutputH = max(max(outputHWithPadHead, outputHWithPadTail) * 2, OUTPUTH_MINIMAL_HCUT);

    int64_t outputWWithPadHead =
        Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(convArgs.padL), static_cast<uint64_t>(convArgs.strideW)) + 1;
    int64_t outputWWithoutPadTail = (convArgs.padL + convArgs.singleWi - convArgs.kWDilated) / convArgs.strideW + 1;
    int64_t outputWWithPadTail = convArgs.singleWo - outputWWithoutPadTail;
    int64_t minimalOutputW = max(max(outputWWithPadHead, outputWWithPadTail), OUTPUTH_MINIMAL_WCUT);

    int64_t l1SizeRemainForFeature = static_cast<int64_t>(bufferInfo.l1Size) - validSize;

    int64_t outH = min(convArgs.singleHo, minimalOutputH);
    int64_t outW = min(convArgs.singleWo, minimalOutputW);
    int64_t inH = (outH - 1) * convArgs.strideH + convArgs.kHDilated;
    int64_t inW = (outW - 1) * convArgs.strideW + convArgs.kWDilated;

    int64_t l1FracCi = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi),
        static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.featureMapType)));

    // Filter不进L1时, filterSize=0, filter需要PingPong.
    int64_t filterSize =
        isFilterInL1 ? CalcFilterSize(convArgs, convArgs.kH, convArgs.kW, convArgs.singleCi, l1Co, L1_PINGPONG) : 0;
    int64_t featureSize = CalcL1FeatureSize(convArgs, filterSize, inH, inW, outH, outW, l1FracCi, curL0FracCo);
    if (featureSize > l1SizeRemainForFeature) {
        return TILING_FAILED;
    }

    // 1. 扩大W. (W尽可能大, W不双源.)
    while ((outW + 1) <= convArgs.singleWo) {
        int64_t newOutW = min(outW + 1, convArgs.singleWo);
        int64_t newInW = min((newOutW - 1) * convArgs.strideW + convArgs.kWDilated, convArgs.singleWi);
        int64_t tryFeatureSize =
            CalcL1FeatureSize(convArgs, filterSize, inH, newInW, outH, newOutW, l1FracCi, curL0FracCo);
        if (tryFeatureSize > l1SizeRemainForFeature) {
            break;
        }
        inW = newInW;
        outW = newOutW;
    }

    // 2. 扩大H
    if (outW >= convArgs.singleWo) {
        while ((outH + 1) <= convArgs.singleHo) {
            int64_t newOutH = outH + 1;
            int64_t newInH = min((newOutH - 1) * convArgs.strideH + convArgs.kHDilated, convArgs.singleHi);
            int64_t tryFeatureSize =
                CalcL1FeatureSize(convArgs, filterSize, newInH, inW, newOutH, outW, l1FracCi, curL0FracCo);
            if (tryFeatureSize > l1SizeRemainForFeature) {
                break;
            }
            inH = newInH;
            outH = newOutH;
        }
    }

    L1FracH = inH;
    L1FracW = inW;
    L1FracHOut = outH;
    L1FracWOut = outW;
    return TILING_SUCCESS;
}

bool L1CutHWCo::TryL1HWCoSplit(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t curL0FracCo,
    int64_t l1Co, int64_t &L1FracH, int64_t &L1FracW, int64_t &L1FracHOut, int64_t &L1FracWOut, uint64_t &movInSize)
{
    int64_t paramSize = CalcL1ParamSize(convArgs, l1Co);
    int64_t totalValidSize =
        Conv3dTilingApi::AlignB(static_cast<uint64_t>(paramSize), static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));
    if (GetL1OptimiFeatureSize(
        bufferInfo, convArgs, totalValidSize, l1Co, curL0FracCo, L1FracH, L1FracW, L1FracHOut, L1FracWOut) !=
        TILING_SUCCESS) {
        return false;
    }
    int64_t filterLoopNum =
        Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(convArgs.singleCo), static_cast<uint64_t>(l1Co));
    int64_t l1HLoopNum =
        Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(convArgs.singleHo), static_cast<uint64_t>(L1FracHOut));
    int64_t l1WLoopNum =
        Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(convArgs.singleWo), static_cast<uint64_t>(L1FracWOut));
    movInSize = static_cast<uint64_t>(l1HLoopNum * l1WLoopNum * L1FracH * L1FracW * filterLoopNum);
    return true;
}

bool L1CutHWCo::TryL1HWCoSplitBest(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t tryL0FracCo,
    int64_t tryL1Co, L1HWCoBestSplit &bestSplit)
{
    int64_t L1FracH = 0;
    int64_t L1FracW = 0;
    int64_t L1FracHOut = 0;
    int64_t L1FracWOut = 0;
    uint64_t movInSize = 0;
    if (TryL1HWCoSplit(
        bufferInfo, convArgs, tryL0FracCo, tryL1Co, L1FracH, L1FracW, L1FracHOut, L1FracWOut, movInSize) &&
        movInSize < bestSplit.movInSize) {
        bestSplit.movInSize = movInSize;
        bestSplit.l1Co = tryL1Co;
        bestSplit.l0FracCo = tryL0FracCo;
        bestSplit.l1FracH = L1FracH;
        bestSplit.l1FracW = L1FracW;
        bestSplit.l1FracHOut = L1FracHOut;
        bestSplit.l1FracWOut = L1FracWOut;
        return true;
    }
    return false;
}

int64_t L1CutHWCo::CalcL1Split(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTiling)
{
    int64_t coAlign = ALIGN_SIZE / dtypeSizeMap.at(convArgs.outputType);
    int64_t maxL1Co = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCo), static_cast<uint64_t>(coAlign));
    L1HWCoBestSplit bestSplit;

    // 1. 枚举Co, 找movInSize最小的切分.
    // Loop 1: 枚举l1Co.
    for (int64_t L1Co = maxL1Co; L1Co >= L0FracCo; L1Co -= L0FracCo) {
        TryL1HWCoSplitBest(bufferInfo, convArgs, L0FracCo, L1Co, bestSplit);
    }
    // Loop 2: L0FracCo还切失败, 同时减小L0FracCo和l1Co, 以coAlign为粒度,
    // 极端情况下, L1Co会很小, L0B用不满.
    if (bestSplit.movInSize == UINT64_MAX) {
        for (int64_t curL0FracCo = L0FracCo - coAlign; curL0FracCo >= coAlign; curL0FracCo -= coAlign) {
            TryL1HWCoSplitBest(bufferInfo, convArgs, curL0FracCo, curL0FracCo, bestSplit);
        }
    }
    if (bestSplit.movInSize == UINT64_MAX) {
        return TILING_FAILED;
    }

    // 3. SetTiling Result. 更新L0FracCo(由CalcTiling同步到L0FracResult)
    L0FracCo = bestSplit.l0FracCo;
    l1SplitResult.l1FracCi = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi),
        static_cast<uint64_t>(ALIGN_SIZE / dtypeSizeMap.at(convArgs.featureMapType)));  // 不切.
    l1SplitResult.l1FracCo = bestSplit.l1Co;
    l1SplitResult.l1FracH = bestSplit.l1FracH;
    l1SplitResult.l1FracHo = bestSplit.l1FracHOut;
    l1SplitResult.l1FracW = bestSplit.l1FracW;
    l1SplitResult.l1FracWo = bestSplit.l1FracWOut;
    uint32_t pBuffer = (1 << pBufferL1APingPongPos) + (1 << pBufferL1BPingPongPos) + (1 << pBufferL0BPingPongPos) +
                       (isL1SupportDual ? (1 << pBufferL1DualPos) : 0);
    SetTilingFlag(convArgs, pBuffer, innerTiling);
    return TILING_SUCCESS;
}

void L1CutHCiCo::CalcSingleBaseJoint(const Conv2dArgs &convArgs, int64_t tryCi, int64_t FracCoL0, int64_t &baseL1FracH,
    int64_t &baseL1FracHo, int64_t &jointL1FracH, TilingL1BufferSize &baseDataSize, TilingL1BufferSize &jointDataSize)
{
    // Base: 无overlap
    baseL1FracH = convArgs.kHDilated;
    int64_t minH = max(convArgs.padT + max(convArgs.kHDilated - convArgs.strideH + 1LL, 1LL),
        convArgs.padB + max(convArgs.kHDilated - convArgs.strideH + 1LL, 1LL));
    int64_t baseFactor = 1;
    baseL1FracHo = 1;
    while (baseL1FracH < minH) {
        baseL1FracH += baseFactor * convArgs.strideH;
        baseL1FracHo += 1;
    }
    baseL1FracH = min(baseL1FracH, convArgs.singleHi);
    int64_t maxBaseL1FracHo =
        Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(convArgs.singleHo), static_cast<uint64_t>(1));
    baseL1FracHo = min(baseL1FracHo, maxBaseL1FracHo);
    CalcL1DataSize(
        convArgs, baseDataSize, baseL1FracH, convArgs.singleWi, baseL1FracHo, convArgs.singleWo, tryCi, FracCoL0);

    jointL1FracH = convArgs.strideH;
    int64_t jointL1FracHo = 1;
    CalcL1DataSize(
        convArgs, jointDataSize, jointL1FracH, convArgs.singleWi, jointL1FracHo, convArgs.singleWo, tryCi, FracCoL0);
}

bool L1CutHCiCo::CalcL1JointNum(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t validSize,
    int64_t &outJointNum, const TilingL1BufferSize &baseDataSize, const TilingL1BufferSize &jointDataSize,
    int64_t baseL1FracHo)
{
    int64_t baseTotalSize =
        baseDataSize.inputSize + baseDataSize.outputSize + baseDataSize.eltwiseSize + baseDataSize.partialSize;
    int64_t jointTotalSize =
        jointDataSize.inputSize + jointDataSize.outputSize + jointDataSize.eltwiseSize + jointDataSize.partialSize;
    if (static_cast<int64_t>(bufferInfo.l1Size) - validSize < baseTotalSize) {
        return false;
    }
    int64_t hOutRemain = convArgs.singleHo - baseL1FracHo * 1;
    int64_t maxJointNum =
        hOutRemain > 0 ? Conv3dTilingApi::CeilDiv(static_cast<uint64_t>(hOutRemain), static_cast<uint64_t>(1)) : 0;
    outJointNum = 0;
    if (jointTotalSize > 0) {
        outJointNum =
            min(static_cast<int64_t>(bufferInfo.l1Size - validSize - baseTotalSize) / jointTotalSize, maxJointNum);
    }
    outJointNum = max(outJointNum, static_cast<int64_t>(0));
    int64_t inputPingPong = isL1SupportDual ? L1_PINGPONGPUNG : L1_PINGPONG;
    int64_t eltwisePingPong = L1_PINGPONG;
    int64_t partialPingPong = L1_PARTIAL_PINGPONG;
    int64_t inputSizeAlign =
        Conv3dTilingApi::AlignB(static_cast<uint64_t>(baseDataSize.inputSize + outJointNum * jointDataSize.inputSize),
            static_cast<uint64_t>(ALIGN_SIZE * inputPingPong));
    int64_t validSizeAlign =
        Conv3dTilingApi::AlignB(static_cast<uint64_t>(validSize), static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));
    int64_t outputSizeAlign =
        Conv3dTilingApi::AlignB(static_cast<uint64_t>(baseDataSize.outputSize + outJointNum * jointDataSize.outputSize),
            static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));
    int64_t eltwiseSizeAlign = Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(baseDataSize.eltwiseSize + outJointNum * jointDataSize.eltwiseSize),
        static_cast<uint64_t>(ALIGN_SIZE * eltwisePingPong));
    int64_t partialSizeAlign = Conv3dTilingApi::AlignB(
        static_cast<uint64_t>(baseDataSize.partialSize + outJointNum * jointDataSize.partialSize),
        static_cast<uint64_t>(ALIGN_SIZE * partialPingPong));
    return (inputSizeAlign + validSizeAlign + outputSizeAlign + eltwiseSizeAlign + partialSizeAlign) <=
           static_cast<int64_t>(bufferInfo.l1Size);
}

int64_t L1CutHCiCo::CalcMaxCiByL0B(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t fracCo)
{
    int64_t l0bElements = static_cast<int64_t>(bufferInfo.l0BSize) / L0_PINGPONG / dtypeSizeMap.at(convArgs.weightType);
    int64_t alignCi = ALIGN_SIZE / dtypeSizeMap.at(convArgs.weightType);
    int64_t maxK = ALIGN_FLOOR(
        min(l0bElements / fracCo,
            static_cast<int64_t>(
                convArgs.kH * convArgs.kW *
                Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi), static_cast<uint64_t>(alignCi)))),
        convArgs.kH * convArgs.kW * alignCi);
    int64_t maxCiFromL0B = maxK / (convArgs.kH * convArgs.kW);
    return maxCiFromL0B;
}

bool L1CutHCiCo::TrySingleBaseJoint(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t validSize,
    int64_t tryCo, int64_t tryCi, int64_t FracCoL0, L1HCiCoBestSplit &bestSplit)
{
    int64_t baseHin = 0;
    int64_t baseHout = 0;
    int64_t jointHin = 0;
    TilingL1BufferSize baseDataSize;
    TilingL1BufferSize jointDataSize;
    CalcSingleBaseJoint(convArgs, tryCi, FracCoL0, baseHin, baseHout, jointHin, baseDataSize, jointDataSize);
    if (CalcL1JointNum(bufferInfo, convArgs, validSize, bestSplit.jointNum, baseDataSize, jointDataSize, baseHout)) {
        bestSplit.co = tryCo;
        bestSplit.ci = tryCi;
        bestSplit.baseHin = baseHin;
        bestSplit.baseHout = baseHout;
        bestSplit.jointHin = jointHin;
        bestSplit.baseDataSize = baseDataSize;
        bestSplit.jointDataSize = jointDataSize;
        return true;
    }
    return false;
}

void L1CutStrategy::SetTilingFlag(const Conv2dArgs &convArgs, uint32_t pBuffer, optiling::TConv2DApiTiling &innerTiling)
{
    innerTiling.set_pBufferFlag(pBuffer);
    if (l1SplitResult.l1FracCo >= convArgs.singleCo) {
        innerTiling.set_biasFullLoadFlag(1);
        innerTiling.set_fixpParamsFullLoadFlag(1);
    } else {
        innerTiling.set_biasFullLoadFlag(0);
        innerTiling.set_fixpParamsFullLoadFlag(0);
    }
}

int64_t L1CutHCiCo::CalcL1Split(
    const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, optiling::TConv2DApiTiling &innerTiling)
{
    int64_t alignCo = ALIGN_SIZE / dtypeSizeMap.at(convArgs.outputType);
    int64_t alignCi = ALIGN_SIZE / dtypeSizeMap.at(convArgs.weightType);

    int64_t coMax;
    int64_t coMin;
    int64_t coStep;
    int64_t ciMaxBase;
    int64_t ciMin;
    int64_t ciStep;
    if (isFilterInL1) {
        coMax = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCo), static_cast<uint64_t>(alignCo));
        coMin = L0FracCo;
        coStep = L0FracCo;
        ciMaxBase = Conv3dTilingApi::AlignB(static_cast<uint64_t>(convArgs.singleCi), static_cast<uint64_t>(alignCi));
        ciMin = L0FracCi;
        ciStep = L0FracCi;
    } else {
        coMax = L0FracCo;
        coMin = alignCo;
        coStep = alignCo;
        ciMaxBase = L0FracCi + alignCi;
        ciMin = alignCi;
        ciStep = alignCi;
    }
    L1HCiCoBestSplit bestSplit;
    bool isSplitSuccess = false;
    for (int64_t tryCo = coMax; tryCo >= coMin && !isSplitSuccess; tryCo -= coStep) {
        int64_t ciUpper = isFilterInL1 ? ciMaxBase : CalcMaxCiByL0B(bufferInfo, convArgs, tryCo);
        for (int64_t tryCi = ciUpper; tryCi >= ciMin; tryCi -= ciStep) {
            int64_t FracCoL0 = isFilterInL1 ? L0FracCo : tryCo;
            int64_t curValidSize =
                isFilterInL1 ? CalcL1ParamSize(convArgs, tryCo) +
                                   CalcFilterSize(convArgs, convArgs.kH, convArgs.kW, tryCi, tryCo, L1_PINGPONG)
                             : CalcL1ParamSize(convArgs, tryCo);
            curValidSize = Conv3dTilingApi::AlignB(
                static_cast<uint64_t>(curValidSize), static_cast<uint64_t>(ALIGN_SIZE * L1_PINGPONG));
            if (TrySingleBaseJoint(bufferInfo, convArgs, curValidSize, tryCo, tryCi, FracCoL0, bestSplit)) {
                isSplitSuccess = true;
                break;
            }
        }
    }

    if (!isSplitSuccess) {
        return TILING_FAILED;
    }

    l1SplitResult.l1FracCo = bestSplit.co;
    l1SplitResult.l1FracCi = bestSplit.ci;
    if (!isFilterInL1) {
        L0FracCo = bestSplit.co;
        L0FracCi = bestSplit.ci;
    }
    l1SplitResult.l1FracH = bestSplit.jointHin * bestSplit.jointNum + bestSplit.baseHin;
    l1SplitResult.l1FracW = convArgs.singleWi;
    l1SplitResult.l1FracHo = (bestSplit.jointNum + bestSplit.baseHout) * 1;
    l1SplitResult.l1FracWo = convArgs.singleWo;
    uint32_t pBuffer = (1 << pBufferL1APingPongPos) + (1 << pBufferL1BPingPongPos) + (1 << pBufferL0BPingPongPos) +
                       (isL1SupportDual ? (1 << pBufferL1DualPos) : 0);
    SetTilingFlag(convArgs, pBuffer, innerTiling);
    return TILING_SUCCESS;
}
}  // namespace Conv2dTilingApi
