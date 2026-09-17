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

#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "../../../lib/conv/conv2d/conv2d_tiling_base.h"
#else
#include "include/adv_api/conv/conv2d/conv2d_tiling_base.h"
#endif

namespace Conv2dTilingApi {
Conv2dTilingBase::Conv2dTilingBase(const platform_ascendc::PlatformAscendC& ascendcPlatform)
{
    (void)(ascendcPlatform);
    // uint64_t l1Size = 0;
    // uint64_t l0CSize = 0;
    // uint64_t ubSize = 0;
    // uint64_t l0ASize = 0;
    // uint64_t l0BSize = 0;
    // auto socVersion = ascendcPlatform.GetSocVersion();
    // ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::L1, l1Size);
    // ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::L0_C, l0CSize);
    // ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::UB, ubSize);
    // ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::L0_A, l0ASize);
    // ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::L0_B, l0BSize);

    // this->platformInfo.socVersion = socVersion;
    // this->platformInfo.l1Size = static_cast<uint64_t>(l1Size);
    // this->platformInfo.l0ASize = static_cast<uint64_t>(l0ASize);
    // this->platformInfo.l0BSize = static_cast<uint64_t>(l0BSize);
    // this->platformInfo.l0CSize = static_cast<uint64_t>(l0CSize);
    // this->platformInfo.ubSize = static_cast<uint64_t>(ubSize);
    // this->platformInfo.btSize = BT_SIZE;
    // this->platformInfo.fbSize = FB_SIZE;
}

Conv2dTilingBase::Conv2dTilingBase(const PlatformInfo& platform)
{
    (void)(platform);
    // this->platformInfo.socVersion = platform.socVersion;
    // this->platformInfo.l1Size = static_cast<uint64_t>(platform.l1Size);
    // this->platformInfo.l0ASize = static_cast<uint64_t>(platform.l0ASize);
    // this->platformInfo.l0BSize = static_cast<uint64_t>(platform.l0BSize);
    // this->platformInfo.l0CSize = static_cast<uint64_t>(platform.l0CSize);
    // this->platformInfo.ubSize = static_cast<uint64_t>(platform.ubSize);
    // this->platformInfo.btSize = BT_SIZE;
    // this->platformInfo.fbSize = FB_SIZE;
}

void Conv2dTilingBase::SetOrgWeightShape(int64_t orgCo, int64_t orgKh, int64_t orgKw)
{
    (void)(orgCo);
    (void)(orgKh);
    (void)(orgKw);
    // this->shapeInfo.orgkH = orgKh;
    // this->shapeInfo.orgkW = orgKw;
    // this->shapeInfo.orgCo = orgCo;
}

void Conv2dTilingBase::SetSingleWeightShape(int64_t singleCi, int64_t singleKh, int64_t singleKw)
{
    (void)(singleCi);
    (void)(singleKh);
    (void)(singleKw);
    // this->shapeInfo.singlekH = singleKh;
    // this->shapeInfo.singlekW = singleKw;
    // this->shapeInfo.singleCi = singleCi;
}

void Conv2dTilingBase::SetOrgInputShape(int64_t orgCi, int64_t orgHi, int64_t orgWi)
{
    (void)(orgCi);
    (void)(orgHi);
    (void)(orgWi);
    // this->shapeInfo.orgCi = orgCi;
    // this->shapeInfo.orgHi = orgHi;
    // this->shapeInfo.orgWi = orgWi;
}

void Conv2dTilingBase::SetSingleOutputShape(int64_t singleCo, int64_t singleM)
{
    (void)(singleCo);
    (void)(singleM);
    // this->shapeInfo.singleCo = singleCo;
    // this->shapeInfo.singleM = singleM;
}

void Conv2dTilingBase::SetSingleOutputShape(int64_t singleCo, int64_t singleHo, int64_t singleWo)
{
    (void)(singleCo);
    (void)(singleHo);
    (void)(singleWo);
    // this->shapeInfo.singleCo = singleCo;
    // this->shapeInfo.singleHo = singleHo;
    // this->shapeInfo.singleWo = singleWo;
    // this->shapeInfo.singleM = singleHo * singleWo;
}

void Conv2dTilingBase::SetInputType(const ConvCommonApi::TPosition pos,
                                    const ConvCommonApi::ConvFormat format,
                                    const ConvCommonApi::ConvDtype dtype)
{
    (void)(pos);
    (void)(format);
    (void)(dtype);
    // this->descInfo.fMapType.pos = pos;
    // this->descInfo.fMapType.format = format;
    // this->descInfo.fMapType.dtype = dtype;
}

void Conv2dTilingBase::SetWeightType(const ConvCommonApi::TPosition pos,
                                     const ConvCommonApi::ConvFormat format,
                                     const ConvCommonApi::ConvDtype dtype)
{
    (void)(pos);
    (void)(format);
    (void)(dtype);
    // this->descInfo.weightType.pos = pos;
    // this->descInfo.weightType.format = format;
    // this->descInfo.weightType.dtype = dtype;
}

void Conv2dTilingBase::SetBiasType(const ConvCommonApi::TPosition pos,
                                   const ConvCommonApi::ConvFormat format,
                                   const ConvCommonApi::ConvDtype dtype)
{
    (void)(pos);
    (void)(format);
    (void)(dtype);
    // this->hasBias = true;
    // this->descInfo.biasType.pos = pos;
    // this->descInfo.biasType.format = format;
    // this->descInfo.biasType.dtype = dtype;
}

void Conv2dTilingBase::SetOutputType(
    const ConvCommonApi::TPosition pos, const ConvCommonApi::ConvFormat format, const ConvCommonApi::ConvDtype dtype)
{
    (void)(pos);
    (void)(format);
    (void)(dtype);
    // this->descInfo.outputType.pos = pos;
    // this->descInfo.outputType.format = format;
    // this->descInfo.outputType.dtype = dtype;
}


void Conv2dTilingBase::SetPadding(int64_t padTop, int64_t padBottom,
                                  int64_t padLeft, int64_t padRight)
{
    (void)(padTop);
    (void)(padBottom);
    (void)(padLeft);
    (void)(padRight);
    // this->attrInfo.padTop = padTop;
    // this->attrInfo.padBottom = padBottom;
    // this->attrInfo.padLeft = padLeft;
    // this->attrInfo.padRight = padRight;
}

void Conv2dTilingBase::SetDilation(int64_t dilationH, int64_t dilationW)
{
    (void)(dilationH);
    (void)(dilationW);
    // this->attrInfo.dilationH = dilationH;
    // this->attrInfo.dilationW = dilationW;
}

void Conv2dTilingBase::SetStride(int64_t strideH, int64_t strideW)
{
    (void)(strideH);
    (void)(strideW);
    // this->attrInfo.strideH = strideH;
    // this->attrInfo.strideW = strideW;
}

void Conv2dTilingBase::SetGroups(int64_t groups)
{
    (void)(groups);
    // this->attrInfo.groups = groups;
}

void Conv2dTilingBase::SetPreQuant(uint8_t mode)
{
    (void)(mode);
    // this->fixpInfo
}

void Conv2dTilingBase::SetPreRelu(uint8_t mode)
{
    (void)(mode);
    // this->fixpInfo
}

void Conv2dTilingBase::SetPreClipRelu(uint8_t mode)
{
    (void)(mode);
    // this->fixpInfo
}

void Conv2dTilingBase::SetEltwise(uint8_t eltwiseOp, bool eltwiseBrcEn,
                                  const ConvCommonApi::ConvDtype eltwiseDtype)
{
    (void)(eltwiseOp);
    (void)(eltwiseBrcEn);
    (void)(eltwiseDtype);
    // this->fixpInfo
}

void Conv2dTilingBase::SetAntiQuant(uint8_t mode)
{
    (void)(mode);
    // this->fixpInfo
}

void Conv2dTilingBase::SetPostQuant(uint8_t mode)
{
    (void)(mode);
    // this->fixpInfo
}

void Conv2dTilingBase::SetPostRelu(uint8_t mode)
{
    (void)(mode);
    // this->fixpInfo
}

void Conv2dTilingBase::SetPostClipRelu(uint8_t mode)
{
    (void)(mode);
    // this->fixpInfo
}

void Conv2dTilingBase::SetPadding(uint8_t mode)
{
    (void)(mode);
    // this->fixpInfo
}
} // namespace Conv2dTilingApi
