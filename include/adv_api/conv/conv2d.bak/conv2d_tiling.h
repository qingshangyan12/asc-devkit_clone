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
 * \file conv2d_tiling.h
 * \brief
 */

#ifndef ASCENDC_TIKCFW_TILING_CONV2D_TILING_H
#define ASCENDC_TIKCFW_TILING_CONV2D_TILING_H

#include "conv2d_tilingdata.h"
#include "../../impl/conv/conv_tiling/common/conv_tiling_algorithm_base.h"
namespace conv_tiling {
struct Conv2DBasicBlockTilingParams {
    uint32_t mTile = 0; // Tile is the size of the block in singleCore
    uint32_t nTile = 0;
    uint32_t kTile = 0;
    uint32_t fCut = 0; // Cut is the number of the block in singleCore
    uint32_t mCut = 0;
    uint32_t nCut = 0;
    uint64_t mIn = 0; // mIn is the size of ML1 inferred back on L1
    uint32_t nKernelHKernelW = 0;
    uint32_t cinL1 = 0;
    uint32_t batch = 0;
    BoundType boundType = BoundType::INVALID;
};

struct Conv2DBasicBlockDim {
    uint32_t fDim = 0;
    uint32_t nDim = 0;
    uint32_t batchDim = 0;
    uint32_t mDim = 0;
    uint32_t fwDim = 0;
    uint32_t groupDim = 0;
    uint32_t aicoreNum = 0;
    bool optGroupFlag = false;
    float coreUtilization = 0;
};

struct Conv2DBasicBlockMinLoadStrategy {
    IterateMNOrder iterateMNOrder = IterateMNOrder::INVALID;
    bool kAl1FullLoad = false;
    bool kBl1FullLoad = false;
    bool mAl1FullLoad = false;
    bool nBl1FullLoad = false;
    bool biasFullLoad = false;
    bool fixpFullLoad = false;
    double l1LoadScore = -1.0;
};

// multiple inheritance
struct Conv2DBasicBlockInfo :
    public Conv2DBasicBlockTilingParams,
    public Conv2DBasicBlockDim,
    public Conv2DBasicBlockMinLoadStrategy
{};

class Conv2dTiling : public ConvTilingBase {
public:
    Conv2dTiling() {};
    explicit Conv2dTiling(const PlatformInfo& platform) : ConvTilingBase(platform) {};
    ~Conv2dTiling() override {};
    int64_t GetTiling(optiling::TConv2DTiling &tiling);
    bool GetTiling(Conv2DBasicBlockInfo& conv2DBasicBlockInfo, optiling::TConv2DTiling &tiling);
    int64_t Compute() override;

    void SetOrgWeightShape(int64_t orgCo, int64_t orgkH, int64_t orgkW);
    void SetOrgFmapShape(int64_t orgCi, int64_t orgHi, int64_t orgWi);
    void SetSingleOutputShape(int64_t singleCo, int64_t singleHo, int64_t singleWo);
    void SetSingleOutputShape(int64_t singleCo, int64_t singleM);
    void SetSingleWeightShape(int64_t singleCi, int64_t singlekH, int64_t singlekW);
    void SetWeightType(TPosition pos, ConvFormat format, ConvDtype dtype);
    void SetFmapType(TPosition pos, ConvFormat format, ConvDtype dtype);
    void SetBiasType(TPosition pos, ConvFormat format, ConvDtype dtype);
    void SetOutputType(TPosition pos, ConvFormat format, ConvDtype dtype);
    void SetPadding(int32_t padTop, int32_t padBottom, int32_t padLeft, int32_t padRight);
    void SetDilation(int32_t dilationH, int32_t dilationW);
    void SetStride(int32_t strideH, int32_t strideW);
    void SetHF32(bool hf32Enable, bool hf32TransMode);
    void SetQuantScale(bool hasScale);
    void SetOffsetx(int8_t offsetx);
    void SetC04Flag(bool isC04Flag);
    void SetRoundMode(int8_t roundMode);
    void SetOutputOrder(int8_t outputOrder);
    void SetTilingAlgorithmType(int8_t tilingAlgorithmType);
    bool GetCoreBindingDecisionFactor(Conv2DBasicBlockInfo& conv2DBasicBlockInfo);
    void SetGroups(int32_t groups);
    void SetOptGroupParams(int32_t enlarge, int64_t singleGroups, int64_t singleGroupOpt);
    void CalcOptGroupParams(const ConvOriGroupInfo& oriGroupInfo, ConvOptGroupInfo& optGroupInfo) const;

private:
    shared_ptr<ConvTilingAlgorithmBase> algoPtr;
    void SetTilingData(optiling::TConv2DTiling& tiling);
    void SetAttrsTilingData(optiling::TConv2DTiling& tiling);
    void SetScalarParams(optiling::TConv2DTiling& tiling);
    uint32_t CalcAL1SpaceSize(optiling::TConv2DTiling& tiling);
    void SetDefaultDdim();
    void PrintTilingData() const;
    void Infer5hdShape();
    bool CheckParams();
    bool CheckAttr();
    bool CheckShape();
    bool CheckFormat();
    bool CheckAlgorithmLimit();
    bool CheckFeaMapShape();
    bool CheckWeightShape();
    bool CheckParamsBBModePhase1();
    bool CheckShapeBBModePhase1();
    bool CheckFmapShapeBeforeCoreBind();
    bool CheckFmapShapeBBModePhase1();
    bool CheckWeightShapeBeforeCoreBind();
    bool CheckWeightShapeBBModePhase1();
    bool CheckAttrBeforeCoreBind();
    bool CheckAttrBBModePhase1();
    bool CheckGroupsBBModePhase1();
    bool CheckTilingAlgorithmType(Conv2DBasicBlockInfo& conv2DBasicBlockInfo, int8_t phase);
    bool CheckL1SizeLimit();
    bool CheckConv2DBasicBlockInfoPhase1(Conv2DBasicBlockInfo& conv2DBasicBlockInfo) const;
    bool CheckConv2DBasicBlockInfoPhase2(Conv2DBasicBlockInfo& conv2DBasicBlockInfo) const;
    bool CheckBBmodeLimits(Conv2DBasicBlockInfo& conv2DBasicBlockInfo);
    void PrintConv2DBasicBlockInfoPhase1(const Conv2DBasicBlockInfo& conv2DBasicBlockInfo) const;
    void PrintConv2DBasicBlockInfoPhase2(const Conv2DBasicBlockInfo& conv2DBasicBlockInfo) const;
};
} // namespace conv_tiling

#endif