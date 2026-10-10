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
 * \file conv2d_tiling_base.cpp
 * \brief
 */

#include <algorithm>
#include <cmath>
#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "../../../lib/conv/conv2d/conv2d_tiling_base.h"
#else
#include "../../../../include/adv_api/conv/conv2d/conv2d_tiling_base.h"
#endif

namespace Conv2dTilingApi {

constexpr uint8_t HASPADDING = 1;

Conv2dTilingBase::Conv2dTilingBase(const platform_ascendc::PlatformAscendC& ascendcPlatform)
{
    bufferInfo_.socVersion = ascendcPlatform.GetSocVersion();
    ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::L1, bufferInfo_.l1Size);
    ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::L0_C, bufferInfo_.l0CSize);
    ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::UB, bufferInfo_.ubSize);
    ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::L0_A, bufferInfo_.l0ASize);
    ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::L0_B, bufferInfo_.l0BSize);
    bufferInfo_.ptSize = PT_SIZE;
}

Conv2dTilingBase::Conv2dTilingBase(const PlatformInfo& platform)
{
    bufferInfo_.socVersion = platform.socVersion;
    bufferInfo_.l1Size = platform.l1Size;
    bufferInfo_.l0ASize = platform.l0ASize;
    bufferInfo_.l0BSize = platform.l0BSize;
    bufferInfo_.l0CSize = platform.l0CSize;
    bufferInfo_.ubSize = platform.ubSize;
    bufferInfo_.ptSize = PT_SIZE;
}

void Conv2dTilingBase::SetOrgWeightShape(int64_t orgCo, int64_t orgKh, int64_t orgKw)
{
    convArgs_.cOut = orgCo;
    convArgs_.kH = orgKh;
    convArgs_.kW = orgKw;

    innerTilingResult_.set_orgCo(static_cast<uint32_t>(orgCo));
    innerTilingResult_.set_kernelH(static_cast<uint32_t>(orgKh));
    innerTilingResult_.set_kernelW(static_cast<uint32_t>(orgKw));
    innerTilingResult_.set_kernelHxkernelW(static_cast<uint32_t>(orgKh) * static_cast<uint32_t>(orgKw));
}

void Conv2dTilingBase::SetOrgInputShape(int64_t orgCi, int64_t orgHi, int64_t orgWi)
{
    convArgs_.cIn = orgCi;
    convArgs_.hIn = orgHi;
    convArgs_.wIn = orgWi;

    innerTilingResult_.set_orgCi(static_cast<uint32_t>(orgCi));
    innerTilingResult_.set_orgHi(static_cast<uint64_t>(orgHi));
    innerTilingResult_.set_orgWi(static_cast<uint64_t>(orgWi));
    innerTilingResult_.set_oriHixWi(static_cast<uint64_t>(orgHi) * static_cast<uint64_t>(orgWi));
}

void Conv2dTilingBase::SetSingleOutputShape(int64_t singleCo, int64_t singleHo, int64_t singleWo)
{
    convArgs_.singleCo = singleCo;
    convArgs_.singleHo = singleHo;
    convArgs_.singleWo = singleWo;
    innerTilingResult_.set_singleCoreCo(static_cast<uint32_t>(singleCo));
    innerTilingResult_.set_singleCoreHo(static_cast<uint64_t>(singleHo));
    innerTilingResult_.set_singleCoreWo(static_cast<uint64_t>(singleWo));
}

void Conv2dTilingBase::SetSingleOutputShape(int64_t singleCo, int64_t singleM)
{
    convArgs_.singleCo = singleCo;
    innerTilingResult_.set_singleCoreCo(static_cast<uint32_t>(singleCo));
    (void)(singleM);
}

void Conv2dTilingBase::SetSingleWeightShape(int64_t singleCi, int64_t singleKh, int64_t singleKw)
{
    innerTilingResult_.set_singleCoreCi(static_cast<uint32_t>(singleCi));
    (void)(singleKh);
    (void)(singleKw);
}

void Conv2dTilingBase::SetInputType(const ConvCommonApi::TPosition pos,
                                    const ConvCommonApi::ConvFormat format,
                                    const ConvCommonApi::ConvDtype dtype)
{
    convArgs_.featureMapType = dtype;
    convArgs_.featureMapFormat = format;
    (void)(pos);
}

void Conv2dTilingBase::SetWeightType(const ConvCommonApi::TPosition pos,
                                     const ConvCommonApi::ConvFormat format,
                                     const ConvCommonApi::ConvDtype dtype)
{
    convArgs_.weightType = dtype;
    convArgs_.weightFormat = format;
    (void)(pos);
}

void Conv2dTilingBase::SetBiasType(const ConvCommonApi::TPosition pos,
                                   const ConvCommonApi::ConvFormat format,
                                   const ConvCommonApi::ConvDtype dtype)
{
    convArgs_.hasBias = true;
    convArgs_.biasType = dtype;
    convArgs_.biasFormat = format;
    innerTilingResult_.set_hasBias(static_cast<uint8_t>(convArgs_.hasBias));
    (void)(pos);
}

void Conv2dTilingBase::SetOutputType(const ConvCommonApi::TPosition pos,
                                     const ConvCommonApi::ConvFormat format,
                                     const ConvCommonApi::ConvDtype dtype)
{
    convArgs_.outputType = dtype;
    convArgs_.outputFormat = format;
    (void)(pos);
}

void Conv2dTilingBase::SetPadding(int64_t padTop, int64_t padBottom,
                                  int64_t padLeft, int64_t padRight)
{
    convArgs_.padT = static_cast<int32_t>(padTop);
    convArgs_.padB = static_cast<int32_t>(padBottom);
    convArgs_.padL = static_cast<int32_t>(padLeft);
    convArgs_.padR = static_cast<int32_t>(padRight);

    innerTilingResult_.set_padTop(static_cast<uint32_t>(padTop));
    innerTilingResult_.set_padBottom(static_cast<uint32_t>(padBottom));
    innerTilingResult_.set_padLeft(static_cast<uint32_t>(padLeft));
    innerTilingResult_.set_padRight(static_cast<uint32_t>(padRight));
}

void Conv2dTilingBase::SetDilation(int64_t dilationH, int64_t dilationW)
{
    convArgs_.dilationH = dilationH;
    convArgs_.dilationW = dilationW;
    innerTilingResult_.set_dilationH(static_cast<uint32_t>(dilationH));
    innerTilingResult_.set_dilationW(static_cast<uint32_t>(dilationW));
}

void Conv2dTilingBase::SetStride(int64_t strideH, int64_t strideW)
{
    convArgs_.strideH = strideH;
    convArgs_.strideW = strideW;
    innerTilingResult_.set_strideH(static_cast<uint32_t>(strideH));
    innerTilingResult_.set_strideW(static_cast<uint32_t>(strideW));
}

void Conv2dTilingBase::SetGroups(int64_t groups)
{
    convArgs_.groups = static_cast<int32_t>(groups);
    innerTilingResult_.set_groups(static_cast<uint32_t>(groups));
}

void Conv2dTilingBase::SetPreQuant(uint8_t mode)
{
    convArgs_.hasQuantPre = (mode != static_cast<uint8_t>(NoQuant));
    convArgs_.fixpipeInformation.quantPreMode = static_cast<QuantMode_t>(mode);
    innerTilingResult_.set_hasPreQuant(static_cast<uint8_t>(convArgs_.hasQuantPre));
    innerTilingResult_.set_preQuantMode(mode);
}

void Conv2dTilingBase::SetPreRelu(uint8_t mode)
{
    convArgs_.hasReluPre = (mode != static_cast<uint8_t>(NoRelu));
    convArgs_.fixpipeInformation.reluMode = static_cast<ReluMode_t>(mode);
    innerTilingResult_.set_hasPreRelu(static_cast<uint8_t>(convArgs_.hasReluPre));
    innerTilingResult_.set_preReluMode(mode);
}

void Conv2dTilingBase::SetPreClipRelu(uint8_t mode)
{
    bool hasPreClipRelu = (mode != static_cast<uint8_t>(ClipReLUMode::NoClipReLU));
    convArgs_.fixpipeInformation.clipReLUMode = static_cast<ClipReLUMode>(mode);
    innerTilingResult_.set_hasPreClipRelu(static_cast<uint8_t>(hasPreClipRelu));
    innerTilingResult_.set_preClipReluMode(mode);
}

void Conv2dTilingBase::SetEltwise(uint8_t eltwiseOp, bool eltwiseBrcEn,
                                  const ConvCommonApi::ConvDtype eltwiseDtype)
{
    convArgs_.hasEltwise = (eltwiseOp != static_cast<uint8_t>(EltwiseType::NoEltwise));
    convArgs_.eltwiseType = eltwiseDtype;
    convArgs_.fixpipeInformation.eltwiseOP = static_cast<EltwiseType>(eltwiseOp);
    convArgs_.fixpipeInformation.eltwiseBrcEn = eltwiseBrcEn;
    innerTilingResult_.set_hasEltwise(static_cast<uint8_t>(convArgs_.hasEltwise));
    innerTilingResult_.set_eltwiseOp(eltwiseOp);
    innerTilingResult_.set_eltwiseBrcEn(static_cast<uint8_t>(eltwiseBrcEn));
}

void Conv2dTilingBase::SetAntiQuant(uint8_t mode)
{
    convArgs_.hasEltAntiquant = (mode != 0);
    convArgs_.fixpipeInformation.eltwiseAntiqEn = convArgs_.hasEltAntiquant;
    innerTilingResult_.set_hasAntiQuant(static_cast<uint8_t>(convArgs_.hasEltAntiquant));
    innerTilingResult_.set_antiQuantMode(mode);
}

void Conv2dTilingBase::SetPostQuant(uint8_t mode)
{
    convArgs_.hasQuantPost = (mode != static_cast<uint8_t>(QuantMode_post::NoConv));
    convArgs_.fixpipeInformation.quantPostMode = static_cast<QuantMode_post>(mode);
    innerTilingResult_.set_hasPostQuant(static_cast<uint8_t>(convArgs_.hasQuantPost));
    innerTilingResult_.set_postQuantMode(mode);
}

void Conv2dTilingBase::SetPostRelu(uint8_t mode)
{
    convArgs_.hasReluPost = (mode != static_cast<uint8_t>(NoRelu));
    convArgs_.fixpipeInformation.reluPostMode = static_cast<ReluMode_t>(mode);
    innerTilingResult_.set_hasPostRelu(static_cast<uint8_t>(convArgs_.hasReluPost));
    innerTilingResult_.set_postReluMode(mode);
}

void Conv2dTilingBase::SetPostClipRelu(uint8_t mode)
{
    bool hasPostClipRelu = (mode != static_cast<uint8_t>(ClipReLUMode::NoClipReLU));
    innerTilingResult_.set_hasPostClipRelu(static_cast<uint8_t>(hasPostClipRelu));
    innerTilingResult_.set_postClipReluMode(mode);
}

void Conv2dTilingBase::SetPadding(uint8_t mode)
{
    // 约定调用SetPaddingMode即为haspadding
    convArgs_.isPerchannelPad = (mode != 0);
    innerTilingResult_.set_hasPadding(HASPADDING);
    innerTilingResult_.set_paddingMode(mode);
}

void Conv2dTilingBase::SaveTilingResult(optiling::TConv2DApiTiling& tiling)
{
    // 原始 shape
    tiling.set_orgHi(innerTilingResult_.get_orgHi());
    tiling.set_orgWi(innerTilingResult_.get_orgWi());
    tiling.set_oriHixWi(innerTilingResult_.get_oriHixWi());
    tiling.set_orgHo(innerTilingResult_.get_orgHo());
    tiling.set_orgWo(innerTilingResult_.get_orgWo());
    tiling.set_orgCi(innerTilingResult_.get_orgCi());
    tiling.set_orgCo(innerTilingResult_.get_orgCo());

    // 单核 shape
    tiling.set_singleCoreHo(innerTilingResult_.get_singleCoreHo());
    tiling.set_singleCoreWo(innerTilingResult_.get_singleCoreWo());
    tiling.set_singleCoreCi(innerTilingResult_.get_singleCoreCi());
    tiling.set_singleCoreCo(innerTilingResult_.get_singleCoreCo());

    tiling.set_kernelH(innerTilingResult_.get_kernelH());
    tiling.set_kernelW(innerTilingResult_.get_kernelW());
    tiling.set_kernelHxkernelW(innerTilingResult_.get_kernelHxkernelW());
    tiling.set_strideH(innerTilingResult_.get_strideH());
    tiling.set_strideW(innerTilingResult_.get_strideW());
    tiling.set_dilationH(innerTilingResult_.get_dilationH());
    tiling.set_dilationW(innerTilingResult_.get_dilationW());
    tiling.set_padTop(innerTilingResult_.get_padTop());
    tiling.set_padBottom(innerTilingResult_.get_padBottom());
    tiling.set_padLeft(innerTilingResult_.get_padLeft());
    tiling.set_padRight(innerTilingResult_.get_padRight());

    tiling.set_groups(innerTilingResult_.get_groups());

    // L1 切分
    tiling.set_hoL1(innerTilingResult_.get_hoL1());
    tiling.set_woL1(innerTilingResult_.get_woL1());
    tiling.set_kAL1(innerTilingResult_.get_kAL1());
    tiling.set_kBL1(innerTilingResult_.get_kBL1());
    tiling.set_nBL1(innerTilingResult_.get_nBL1());

    // L0 切分
    tiling.set_hoL0(innerTilingResult_.get_hoL0());
    tiling.set_woL0(innerTilingResult_.get_woL0());
    tiling.set_kL0(innerTilingResult_.get_kL0());
    tiling.set_nL0(innerTilingResult_.get_nL0());
    tiling.set_khL0(innerTilingResult_.get_khL0());
    tiling.set_kwL0(innerTilingResult_.get_kwL0());

    tiling.set_pBufferFlag(innerTilingResult_.get_pBufferFlag());
    tiling.set_cinAInCore(innerTilingResult_.get_cinAInCore());
    tiling.set_cinATailInCore(innerTilingResult_.get_cinATailInCore());
    tiling.set_cinBInCore(innerTilingResult_.get_cinBInCore());
    tiling.set_cinBTailInCore(innerTilingResult_.get_cinBTailInCore());
    tiling.set_a1SpaceSize(innerTilingResult_.get_a1SpaceSize());
    tiling.set_b1SpaceSize(innerTilingResult_.get_b1SpaceSize());
    tiling.set_c0SpaceSize(innerTilingResult_.get_c0SpaceSize());
    tiling.set_multiNBL1(innerTilingResult_.get_multiNBL1());
    tiling.set_mStep(innerTilingResult_.get_mStep());
    tiling.set_kStep(innerTilingResult_.get_kStep());
    tiling.set_nStep(innerTilingResult_.get_nStep());

    tiling.set_biasFullLoadFlag(innerTilingResult_.get_biasFullLoadFlag());
    tiling.set_fixpParamsFullLoadFlag(innerTilingResult_.get_fixpParamsFullLoadFlag());
    tiling.set_iterateMNOrder(innerTilingResult_.get_iterateMNOrder());
    tiling.set_isC04Flag(innerTilingResult_.get_isC04Flag());
    tiling.set_hasBias(innerTilingResult_.get_hasBias());
    tiling.set_hasPreQuant(innerTilingResult_.get_hasPreQuant());
    tiling.set_hasPreRelu(innerTilingResult_.get_hasPreRelu());
    tiling.set_hasPreClipRelu(innerTilingResult_.get_hasPreClipRelu());
    tiling.set_hasAntiQuant(innerTilingResult_.get_hasAntiQuant());
    tiling.set_hasEltwise(innerTilingResult_.get_hasEltwise());
    tiling.set_hasPostQuant(innerTilingResult_.get_hasPostQuant());
    tiling.set_hasPostRelu(innerTilingResult_.get_hasPostRelu());
    tiling.set_hasPostClipRelu(innerTilingResult_.get_hasPostClipRelu());
    tiling.set_hasPadding(innerTilingResult_.get_hasPadding());
    tiling.set_preQuantMode(innerTilingResult_.get_preQuantMode());
    tiling.set_preReluMode(innerTilingResult_.get_preReluMode());
    tiling.set_preClipReluMode(innerTilingResult_.get_preClipReluMode());
    tiling.set_antiQuantMode(innerTilingResult_.get_antiQuantMode());
    tiling.set_eltwiseOp(innerTilingResult_.get_eltwiseOp());
    tiling.set_eltwiseBrcEn(innerTilingResult_.get_eltwiseBrcEn());
    tiling.set_postQuantMode(innerTilingResult_.get_postQuantMode());
    tiling.set_postReluMode(innerTilingResult_.get_postReluMode());
    tiling.set_postClipReluMode(innerTilingResult_.get_postClipReluMode());
    tiling.set_paddingMode(innerTilingResult_.get_paddingMode());
    tiling.set_dualOutput(innerTilingResult_.get_dualOutput());
}
} // namespace Conv2dTilingApi
