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
 * \file conv_tiling_base.h
 * \brief
 */

#ifndef ASCENDC_TIKCFW_TILING_CONV_TILING_BASE_H
#define ASCENDC_TIKCFW_TILING_CONV_TILING_BASE_H

#include "lib/conv/conv_tiling_util.h"

namespace conv_tiling {
struct L1TilingRes {
    uint64_t kAL1 = 0; // with khkw
    uint64_t kBL1 = 0;
    uint64_t hoAL1 = 0;
    uint64_t woAL1 = 0;
    uint64_t nBL1 = 0;
    uint64_t mAL1 = 0;

    IterateMNOrder iterateMNOrder = IterateMNOrder::INVALID;
    bool biasFullLoadFlag = false;
    bool fixpParamsFullLoadFlag = false;
    bool al1FullLoad = false;
    bool bl1FullLoad = false;
    bool isWeightBypass = false;
};

struct L0TilingRes {
    uint64_t hoL0 = 0;
    uint64_t woL0 = 0;
    uint64_t kL0 = 0;
    uint64_t nL0 = 0;
    uint64_t mL0 = 0;
};

struct DoubleBufferRes {
    uint8_t pbAL1 = 1;
    uint8_t pbBL1 = 1;
    uint8_t pbAL0 = 1;
    uint8_t pbBL0 = 1;
    uint8_t pbCL0 = 1;
    uint64_t pBufferFlag = 0;
};

struct ShapeInfo {
    int64_t orgkH = -1;
    int64_t orgkW = -1;
    int64_t orgkD = -1;
    int64_t orgCo = -1;
    int64_t orgCi = -1;
    int64_t orgHi = -1;
    int64_t orgWi = -1;
    int64_t orgDi = -1;
    int64_t orgHo = 0;
    int64_t orgWo = 0;
    int64_t orgDo = 0;

    int64_t singlekH = -1;
    int64_t singlekW = -1;
    int64_t singlekD = -1;
    int64_t singleCo = -1;
    int64_t singleCi = -1;
    int64_t singleDo = -1;
    int64_t singleHo = -1;
    int64_t singleWo = -1;
    int64_t singleM = -1;

    uint64_t singleCi1 = 0;
    uint64_t singleCo1 = 0;
    uint64_t singleM1 = 0;

    int64_t singleGroups = -1;
    int64_t singleGroupOpt = -1;
    int32_t enlarge = 0;
};

struct AttrInfo {
    int32_t padHead = 0;
    int32_t padTail = 0;
    int32_t padTop = 0;
    int32_t padBottom = 0;
    int32_t padLeft = 0;
    int32_t padRight = 0;

    int32_t strideH = 1;
    int32_t strideW = 1;
    int32_t strideD = 1;

    int32_t dilationH = 1;
    int32_t dilationW = 1;
    int32_t dilationD = 1;

    int32_t groups = 1;
    int8_t offsetx = 0;
    int8_t roundMode = 0;
};

struct DescInfo {
    ConvType weightType = {ConvFormat::UNDEFINED, ConvDtype::UNDEFINED, TPosition::GM};
    ConvType fMapType = {ConvFormat::UNDEFINED, ConvDtype::UNDEFINED, TPosition::GM};
    ConvType biasType = {ConvFormat::UNDEFINED, ConvDtype::UNDEFINED, TPosition::GM};
    ConvType outputType = {ConvFormat::UNDEFINED, ConvDtype::UNDEFINED, TPosition::CO1};
    ConvType quantScaleType = {ConvFormat::UNDEFINED, ConvDtype::UNDEFINED, TPosition::GM};
};

struct CubeInfo {
    uint32_t m0 = 0;
    uint32_t k0 = 0;
    uint32_t n0 = 0;
    ConvDtype madType = ConvDtype::UNDEFINED;
    ConvDtype biasType = ConvDtype::UNDEFINED;
};

class ConvTilingBase {
public:
    ConvTilingBase() {};
    explicit ConvTilingBase(const PlatformInfo& platform);
    virtual ~ConvTilingBase() = default;

    DescInfo descInfo;
    ShapeInfo shapeInfo;
    AttrInfo attrInfo;
    CubeInfo cubeInfo;
    PlatformInfo platformInfo;
    L1TilingRes l1TilingInfo;
    L0TilingRes l0TilingInfo;
    DoubleBufferRes dbValue;

    // common usd
    bool hasBias = false;
    bool hasQuantScale = false;
    int8_t outputOrder = static_cast<int8_t>(OutputOrder::M);
    int8_t tilingAlgorithmType = static_cast<int8_t>(TilingAlgorithmType::FORMULAS);

    // usd for conv2d
    bool isC04Flag = false;
    bool hf32Enable = false;
    bool hf32TransMode = false;
    bool optGroupFlag = false;

protected:
    virtual int64_t Compute() = 0;
    void InferCubeInfo();
    int64_t CheckTilingRes();
    bool CheckQuantUniqueAttr();
    bool CheckGroups();
    bool CheckDtype();
    std::vector<std::vector<ConvDtype>> GetSupportedDataTypes() const;
    bool CheckLoad3DLimits();
    bool CheckDataCopyLimits();
    bool CheckFixpipeLimits();
    bool CheckInstructionLimits();
private:
    bool IsPlatformSupportWeightBypass(platform_ascendc::SocVersion curSoc) const;
    uint32_t GetBandWidthCof(platform_ascendc::SocVersion curSoc) const;
};
} // namespace conv_tiling

#endif
