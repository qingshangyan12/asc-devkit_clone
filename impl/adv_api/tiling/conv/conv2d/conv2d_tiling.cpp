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
 * \file conv2d_tiling.cpp
 * \brief
 */

#include <cstdint>
#include <algorithm>
#include "../conv_tiling/algorithm/conv_tiling_algorithm_HWmode.h"
#include "../conv_tiling/algorithm/conv_tiling_algorithm_Mmode.h"
#include "../conv_tiling/algorithm/conv_tiling_algorithm_BBmode.h"
#include "lib/conv/conv2d_tiling.h"

using namespace conv_tiling_algo_m;
using namespace conv_tiling_algo_hw;
using namespace conv_tiling_algo_bb;
using namespace std;
#define UNUSED __attribute__((unused))

namespace optiling {
REGISTER_TILING_DATA_CLASS(TConv2DTilingOpApi, TConv2DTiling)
}

namespace conv_tiling {
int64_t Conv2dTiling::GetTiling(optiling::TConv2DTiling &tiling)
{
    if (!CheckParams()) {
        TILING_LOG_ERROR("conv2d api tiling check params failed.");
        return -1;
    }
    InferCubeInfo();
    Infer5hdShape();

    int64_t ret = Compute();
    if (ret == -1) {
        TILING_LOG_ERROR("conv2d api tiling compute failed.");
        return -1;
    }
    ret = CheckTilingRes();
    if (ret == -1) {
        TILING_LOG_ERROR("conv2d api tiling check tiling result failed.");
        return -1;
    }
    SetTilingData(tiling);
    PrintTilingData();
    return ret;
}

bool Conv2dTiling::GetTiling(Conv2DBasicBlockInfo& conv2DBasicBlockInfo, optiling::TConv2DTiling &tiling)
{
    if (!CheckTilingAlgorithmType(conv2DBasicBlockInfo, BB_PHASE_2)) {
        TILING_LOG_ERROR("conv2d api tiling Process failed.");
        return false;
    }

    InferCubeInfo();
    Infer5hdShape();
 
    std::shared_ptr<ConvTilingAlgorithmBBmode> algoBBPtr = std::make_shared<ConvTilingAlgorithmBBmode>(this,
        conv2DBasicBlockInfo);
    int64_t ret = algoBBPtr->Process();
    if (ret == -1) {
        TILING_LOG_ERROR("conv2d api tiling Process failed.");
        return false;
    }
    ret = CheckTilingRes();
    if (ret == -1) {
        TILING_LOG_ERROR("conv2d api tiling check tiling result failed.");
        return false;
    }
    SetTilingData(tiling);
    PrintTilingData();
    return true;
}

void Conv2dTiling::SetDefaultDdim()
{
    shapeInfo.orgkD = 1;
    shapeInfo.orgDi = 1;
    shapeInfo.singleDo = 1;
    shapeInfo.singlekD = 1;
    attrInfo.padHead = 0;
    attrInfo.padTail = 0;
    attrInfo.dilationD = 1;
    attrInfo.strideD = 1;
}

void Conv2dTiling::Infer5hdShape()
{
    shapeInfo.singleCi1 = isC04Flag ? CONST_VALUE_1 : CeilDiv(shapeInfo.singleCi, cubeInfo.k0);
    shapeInfo.singleCo1 = CeilDiv(shapeInfo.singleCo, cubeInfo.n0);
    if (outputOrder == static_cast<int8_t>(OutputOrder::M)) {
        shapeInfo.singleM1 = CeilDiv(shapeInfo.singleM, cubeInfo.m0);
    }
}

int64_t Conv2dTiling::Compute()
{
    SetDefaultDdim();
    switch (outputOrder) {
        case static_cast<int8_t>(OutputOrder::M):
            algoPtr = std::make_shared<ConvTilingAlgorithmMmode>(this);
            break;
        case static_cast<int8_t>(OutputOrder::HW):
            algoPtr = std::make_shared<ConvTilingAlgorithmHWmode>(this);
            break;
        default:
            TILING_LOG_ERROR("Unsupported output order %d, only support Mmode(%d) and HWmode(%d).",
                outputOrder, OutputOrder::M, OutputOrder::HW);
            return -1;
    }
    TILING_LOG_DEBUG("output order: %d", outputOrder);
    int64_t ret = algoPtr->Process();

    return ret;
}

bool Conv2dTiling::CheckParams()
{
    if (!CheckAlgorithmLimit()) {
        return false;
    }
    if (!CheckAttr()) {
        return false;
    }
    if (!CheckShape()) {
        return false;
    }
    if (!CheckFormat()) {
        return false;
    }
    if (!CheckDtype()) {
        return false;
    }
    if (!CheckInstructionLimits()) {
        return false;
    }
    return true;
}

bool Conv2dTiling::CheckParamsBBModePhase1()
{
    if (!CheckAlgorithmLimit()) {
        return false;
    }
    if (!CheckAttrBBModePhase1()) {
        return false;
    }
    if (!CheckShapeBBModePhase1()) {
        return false;
    }
    if (!CheckFormat()) {
        return false;
    }
    if (!CheckDtype()) {
        return false;
    }
    if (!CheckInstructionLimits()) {
        return false;
    }
    return true;
}

void Conv2dTiling::SetScalarParams(optiling::TConv2DTiling& tiling)
{
    // calculate the follow params in tiling process, for scalar optimization in kernel
    uint32_t kernelHxkernelW = tiling.get_kernelH() * tiling.get_kernelW();
    // [zj]TODO:5D
    uint32_t cinAInCore = tiling.get_kAL1() / kernelHxkernelW;
    uint32_t kAL1Tail = (tiling.get_singleCoreCi() * kernelHxkernelW) % tiling.get_kAL1();
    kAL1Tail = kAL1Tail == 0 ? tiling.get_kAL1() : kAL1Tail;
    uint32_t kBL1Tail = (tiling.get_singleCoreCi() * kernelHxkernelW) % tiling.get_kBL1();
    kBL1Tail = kBL1Tail == 0 ? tiling.get_kBL1() : kBL1Tail;
    uint32_t cinATailInCore = kAL1Tail / kernelHxkernelW;
    uint32_t cinBTailInCore = kBL1Tail / kernelHxkernelW;
    uint32_t oriHixWi = tiling.get_orgHi() * tiling.get_orgWi();
    uint32_t cinOffsetBlockInGM = tiling.get_kAL1() / kernelHxkernelW * oriHixWi;
    uint32_t mStep = 0;
    if (outputOrder == static_cast<int8_t>(OutputOrder::M)) {
        mStep = AlignB(tiling.get_hoL0(), cubeInfo.m0);
    } else {
        mStep = AlignB(tiling.get_hoL0() * tiling.get_woL0(), cubeInfo.m0);
    }
    uint32_t fmapKStride = mStep / cubeInfo.m0;
    uint32_t nStep = CeilDiv(tiling.get_nL0(), cubeInfo.n0);
    uint32_t kStep = tiling.get_kL0() / cubeInfo.k0;
    uint32_t weightKStride = CeilDiv(tiling.get_nBL1(), cubeInfo.n0);
    uint32_t coutOffsetBlock = (tiling.get_orgCi() / tiling.get_groups()) * kernelHxkernelW;
    uint32_t cinBInCore = tiling.get_kBL1() / kernelHxkernelW;
    uint32_t nL1DivBlockSize = tiling.get_nBL1() / cubeInfo.n0;

    tiling.set_kernelHxkernelW(kernelHxkernelW);
    tiling.set_kernelHxkernelWxkernelD(kernelHxkernelW);
    // l0TilingInfo.nL0 != 0 is checked before
    tiling.set_multiNBL1(static_cast<uint32_t>(CeilDiv(tiling.get_nBL1(), tiling.get_nL0())));
    tiling.set_cinAInCore(cinAInCore);
    tiling.set_cinATailInCore(cinATailInCore);
    tiling.set_oriHixWi(oriHixWi);
    tiling.set_cinOffsetBlockInGM(cinOffsetBlockInGM);
    tiling.set_mStep(mStep);
    tiling.set_fmapKStride(fmapKStride);
    tiling.set_nStep(nStep);
    tiling.set_kStep(kStep);
    tiling.set_weightKStride(weightKStride);
    tiling.set_coutOffsetBlock(coutOffsetBlock);
    tiling.set_cinBInCore(cinBInCore);
    tiling.set_cinBTailInCore(cinBTailInCore);
    tiling.set_nL1DivBlockSize(nL1DivBlockSize);
    tiling.set_aL1SpaceSize(CalcAL1SpaceSize(tiling));
}

void Conv2dTiling::SetAttrsTilingData(optiling::TConv2DTiling& tiling)
{
    tiling.set_strideH(static_cast<uint32_t>(attrInfo.strideH));
    tiling.set_strideW(static_cast<uint32_t>(attrInfo.strideW));
    tiling.set_dilationH(static_cast<uint32_t>(attrInfo.dilationH));
    tiling.set_dilationW(static_cast<uint32_t>(attrInfo.dilationW));
    tiling.set_padTop(static_cast<uint32_t>(attrInfo.padTop));
    tiling.set_padBottom(static_cast<uint32_t>(attrInfo.padBottom));
    tiling.set_padLeft(static_cast<uint32_t>(attrInfo.padLeft));
    tiling.set_padRight(static_cast<uint32_t>(attrInfo.padRight));

    tiling.set_groups(static_cast<uint32_t>(attrInfo.groups));

    if (this->optGroupFlag) {
        tiling.set_singleCoreGroups(static_cast<uint32_t>(shapeInfo.singleGroups));
        tiling.set_singleCoreGroupOpt(static_cast<uint32_t>(shapeInfo.singleGroupOpt));
        tiling.set_enlarge(static_cast<uint32_t>(shapeInfo.enlarge));
    }

    tiling.set_offsetx(attrInfo.offsetx);
    tiling.set_roundMode(attrInfo.roundMode);
}

void Conv2dTiling::SetTilingData(optiling::TConv2DTiling& tiling)
{
    if (outputOrder == static_cast<int8_t>(OutputOrder::M)) {
        tiling.set_singleCoreHo(static_cast<uint64_t>(shapeInfo.singleM));
        tiling.set_hoL1(static_cast<uint32_t>(l1TilingInfo.mAL1));
        tiling.set_hoL0(static_cast<uint32_t>(l0TilingInfo.mL0));
    } else {
        tiling.set_singleCoreHo(static_cast<uint64_t>(shapeInfo.singleHo));
        tiling.set_singleCoreWo(static_cast<uint64_t>(shapeInfo.singleWo));
        tiling.set_hoL1(static_cast<uint32_t>(l1TilingInfo.hoAL1));
        tiling.set_woL1(static_cast<uint32_t>(l1TilingInfo.woAL1));
        tiling.set_hoL0(static_cast<uint32_t>(l0TilingInfo.hoL0));
        tiling.set_woL0(static_cast<uint32_t>(l0TilingInfo.woL0));
    }

    tiling.set_kAL1(static_cast<uint32_t>(l1TilingInfo.kAL1));
    tiling.set_kBL1(static_cast<uint32_t>(l1TilingInfo.kBL1));
    // l0TilingInfo.nL0 != 0 is checked before
    tiling.set_multiNBL1(static_cast<uint32_t>(CeilDiv(l1TilingInfo.nBL1, l0TilingInfo.nL0)));
    tiling.set_nBL1(static_cast<uint32_t>(l1TilingInfo.nBL1));
    tiling.set_kL0(static_cast<uint32_t>(l0TilingInfo.kL0));
    tiling.set_nL0(static_cast<uint32_t>(l0TilingInfo.nL0));

    tiling.set_orgHi(static_cast<uint64_t>(shapeInfo.orgHi));
    tiling.set_orgWi(static_cast<uint64_t>(shapeInfo.orgWi));
    tiling.set_orgHo(static_cast<uint64_t>(shapeInfo.orgHo));
    tiling.set_orgWo(static_cast<uint64_t>(shapeInfo.orgWo));

    tiling.set_orgCi(static_cast<uint32_t>(shapeInfo.orgCi));
    tiling.set_orgCo(static_cast<uint32_t>(shapeInfo.orgCo));
    tiling.set_singleCoreCi(static_cast<uint32_t>(shapeInfo.singleCi));
    tiling.set_singleCoreCo(static_cast<uint32_t>(shapeInfo.singleCo));
    tiling.set_kernelH(static_cast<uint32_t>(shapeInfo.orgkH));
    tiling.set_kernelW(static_cast<uint32_t>(shapeInfo.orgkW));

    SetAttrsTilingData(tiling);

    tiling.set_pBufferFlag(static_cast<uint32_t>(dbValue.pBufferFlag));
    tiling.set_hf32Enable(this->hf32Enable);
    tiling.set_hf32TransMode(this->hf32TransMode);
    tiling.set_isC04Flag(static_cast<uint8_t>(isC04Flag));
    tiling.set_hasBias(static_cast<uint8_t>(hasBias));
    // tiling.set_hasScale(static_cast<uint8_t>(hasQuantScale));
    tiling.set_iterateMNOrder(static_cast<uint8_t>(l1TilingInfo.iterateMNOrder));
    tiling.set_biasFullLoadFlag(static_cast<uint8_t>(l1TilingInfo.biasFullLoadFlag));
    tiling.set_fixpParamsFullLoadFlag(static_cast<uint8_t>(l1TilingInfo.fixpParamsFullLoadFlag));

    SetScalarParams(tiling);
}

uint32_t Conv2dTiling::CalcAL1SpaceSize(optiling::TConv2DTiling& tiling)
{
    uint64_t aL1SpaceSize = 0;
    uint64_t fmapSize = DTYPE_SIZE_TAB.at(descInfo.fMapType.dtype);

    uint64_t dilatedKernelH = (tiling.get_kernelH() - 1) * tiling.get_dilationH() + 1;
    if (outputOrder == static_cast<int8_t>(OutputOrder::M)) {
        uint64_t mL1Max = tiling.get_hoL1() < tiling.get_singleCoreHo() ? tiling.get_hoL1() : tiling.get_singleCoreHo();
        uint64_t hoL1Max = std::min(mL1Max / tiling.get_orgWo() + CONST_VALUE_2, tiling.get_orgHo());
        uint64_t hiAL1Max = (hoL1Max - 1) * tiling.get_strideH() + dilatedKernelH;
        hiAL1Max = hiAL1Max > tiling.get_orgHi() ? tiling.get_orgHi() : hiAL1Max;

        aL1SpaceSize = tiling.get_cinAInCore() * hiAL1Max * tiling.get_orgWi();
    } else {
        uint64_t hiAL1Max = (tiling.get_hoL1() - 1) * tiling.get_strideH() + dilatedKernelH;
        hiAL1Max = hiAL1Max > tiling.get_orgHi() ? tiling.get_orgHi() : hiAL1Max;

        uint64_t wiAL1Max = 0;
        if (isC04Flag && tiling.get_singleCoreWo() == tiling.get_woL1()) {
            wiAL1Max = tiling.get_orgWi();

            aL1SpaceSize = AlignB(hiAL1Max * wiAL1Max, C0_SIZE / (fmapSize * C04_CIN_SIZE)) * C04_CIN_SIZE;
        } else {
            uint64_t dilatedKernelW = (tiling.get_kernelW() - 1) * tiling.get_dilationW() + 1;
            wiAL1Max = (tiling.get_woL1() - 1) * tiling.get_strideW() + dilatedKernelW;
            wiAL1Max = wiAL1Max > tiling.get_orgWi() ? tiling.get_orgWi() : wiAL1Max;

            aL1SpaceSize = tiling.get_cinAInCore() * hiAL1Max * wiAL1Max;
        }
    }
    aL1SpaceSize = AlignB(aL1SpaceSize * fmapSize, C0_SIZE);

    return static_cast<uint32_t>(aL1SpaceSize);
}

void Conv2dTiling::PrintTilingData() const
{
    TILING_LOG_DEBUG("orgHi: %ld", shapeInfo.orgHi);
    TILING_LOG_DEBUG("orgWi: %ld", shapeInfo.orgWi);
    TILING_LOG_DEBUG("orgHo: %ld", shapeInfo.orgHo);
    TILING_LOG_DEBUG("orgWo: %ld", shapeInfo.orgWo);
    if (outputOrder == static_cast<int8_t>(OutputOrder::M)) {
        TILING_LOG_DEBUG("singleCoreM: %ld", shapeInfo.singleM);
        TILING_LOG_DEBUG("mL1: %lu", l1TilingInfo.mAL1);
        TILING_LOG_DEBUG("hoL0: %lu", l0TilingInfo.mL0);
    } else {
        TILING_LOG_DEBUG("singleCoreHo: %ld", shapeInfo.singleHo);
        TILING_LOG_DEBUG("singleCoreWo: %ld", shapeInfo.singleWo);
        TILING_LOG_DEBUG("hoL1: %lu", l1TilingInfo.hoAL1);
        TILING_LOG_DEBUG("woL1: %lu", l1TilingInfo.woAL1);
        TILING_LOG_DEBUG("hoL0: %lu", l0TilingInfo.hoL0);
        TILING_LOG_DEBUG("woL0: %lu", l0TilingInfo.woL0);
    }
    TILING_LOG_DEBUG("groups: %d", attrInfo.groups);
    TILING_LOG_DEBUG("orgCi: %ld", shapeInfo.orgCi);
    TILING_LOG_DEBUG("orgCo: %ld", shapeInfo.orgCo);
    TILING_LOG_DEBUG("kernelH: %ld", shapeInfo.orgkH);
    TILING_LOG_DEBUG("kernelW: %ld", shapeInfo.orgkW);
    TILING_LOG_DEBUG("singleCoreCi: %ld", shapeInfo.singleCi);
    TILING_LOG_DEBUG("singleCoreCo: %ld", shapeInfo.singleCo);
    TILING_LOG_DEBUG("kAL1: %lu", l1TilingInfo.kAL1);
    TILING_LOG_DEBUG("kBL1: %lu", l1TilingInfo.kBL1);
    TILING_LOG_DEBUG("nBL1: %lu", l1TilingInfo.nBL1);
    TILING_LOG_DEBUG("kL0: %lu", l0TilingInfo.kL0);
    TILING_LOG_DEBUG("nL0: %lu", l0TilingInfo.nL0);
    TILING_LOG_DEBUG("pBufferFlag: %lu", dbValue.pBufferFlag);
    TILING_LOG_DEBUG("strideH: %d", attrInfo.strideH);
    TILING_LOG_DEBUG("strideW: %d", attrInfo.strideW);
    TILING_LOG_DEBUG("dilationH: %d", attrInfo.dilationH);
    TILING_LOG_DEBUG("dilationW: %d", attrInfo.dilationW);
    TILING_LOG_DEBUG("padTop: %d", attrInfo.padTop);
    TILING_LOG_DEBUG("padBottom: %d", attrInfo.padBottom);
    TILING_LOG_DEBUG("padLeft: %d", attrInfo.padLeft);
    TILING_LOG_DEBUG("padRight: %d", attrInfo.padRight);
    TILING_LOG_DEBUG("iterateMNOrder: %u", l1TilingInfo.iterateMNOrder);
    TILING_LOG_DEBUG("biasFullLoadFlag: %u", l1TilingInfo.biasFullLoadFlag);
    TILING_LOG_DEBUG("fixpParamsFullLoadFlag: %u", l1TilingInfo.fixpParamsFullLoadFlag);
    TILING_LOG_DEBUG("offsetx: %d", attrInfo.offsetx);
    TILING_LOG_DEBUG("hf32Mode: %u", hf32Enable);
    TILING_LOG_DEBUG("hf32TransMode: %u", hf32TransMode);
    TILING_LOG_DEBUG("isC04Flag: %u", isC04Flag);
}

void Conv2dTiling::SetOutputOrder(int8_t outputOrder)
{
    this->outputOrder = outputOrder;
}

void Conv2dTiling::SetOrgWeightShape(int64_t orgCo, int64_t orgkH, int64_t orgkW)
{
    shapeInfo.orgkH = orgkH;
    shapeInfo.orgkW = orgkW;
    shapeInfo.orgCo = orgCo;
}

void Conv2dTiling::SetSingleWeightShape(int64_t singleCi, int64_t singlekH, int64_t singlekW)
{
    shapeInfo.singlekH = singlekH;
    shapeInfo.singlekW = singlekW;
    shapeInfo.singleCi = singleCi;
}

void Conv2dTiling::SetOrgFmapShape(int64_t orgCi, int64_t orgHi, int64_t orgWi)
{
    shapeInfo.orgCi = orgCi;
    shapeInfo.orgHi = orgHi;
    shapeInfo.orgWi = orgWi;
}

void Conv2dTiling::SetSingleOutputShape(int64_t singleCo, int64_t singleHo, int64_t singleWo)
{
    shapeInfo.singleCo = singleCo;
    shapeInfo.singleHo = singleHo;
    shapeInfo.singleWo = singleWo;
}

void Conv2dTiling::SetSingleOutputShape(int64_t singleCo, int64_t singleM)
{
    shapeInfo.singleCo = singleCo;
    shapeInfo.singleM = singleM;
}

void Conv2dTiling::SetWeightType(TPosition pos, ConvFormat format, ConvDtype dtype)
{
    descInfo.weightType.pos = pos;
    descInfo.weightType.format = format;
    descInfo.weightType.dtype = dtype;
}

void Conv2dTiling::SetFmapType(TPosition pos, ConvFormat format, ConvDtype dtype)
{
    descInfo.fMapType.pos = pos;
    descInfo.fMapType.format = format;
    descInfo.fMapType.dtype = dtype;
}

void Conv2dTiling::SetPadding(int32_t padTop, int32_t padBottom, int32_t padLeft, int32_t padRight)
{
    attrInfo.padTop = padTop;
    attrInfo.padBottom = padBottom;
    attrInfo.padLeft = padLeft;
    attrInfo.padRight = padRight;
}

void Conv2dTiling::SetDilation(int32_t dilationH, int32_t dilationW)
{
    attrInfo.dilationH = dilationH;
    attrInfo.dilationW = dilationW;
}

void Conv2dTiling::SetStride(int32_t strideH, int32_t strideW)
{
    attrInfo.strideH = strideH;
    attrInfo.strideW = strideW;
}

void Conv2dTiling::SetBiasType(TPosition pos, ConvFormat format, ConvDtype dtype)
{
    this->hasBias = true;
    descInfo.biasType.pos = pos;
    descInfo.biasType.format = format;
    descInfo.biasType.dtype = dtype;
}

void Conv2dTiling::SetOutputType(TPosition pos, ConvFormat format, ConvDtype dtype)
{
    descInfo.outputType.pos = pos;
    descInfo.outputType.format = format;
    descInfo.outputType.dtype = dtype;
}
 
void Conv2dTiling::SetHF32(bool hf32Enable, bool hf32TransMode = false)
{
    this->hf32Enable = hf32Enable;
    this->hf32TransMode = hf32TransMode;
}

void Conv2dTiling::SetQuantScale(bool hasScale)
{
    this->hasQuantScale = hasScale;
    if (hasScale) {
        this->descInfo.quantScaleType.dtype = ConvDtype::INT64;
    }
}

void Conv2dTiling::SetOffsetx(int8_t offsetx)
{
    attrInfo.offsetx = offsetx;
}

void Conv2dTiling::SetC04Flag(bool isC04Flag)
{
    this->isC04Flag = isC04Flag;
}

void Conv2dTiling::SetRoundMode(int8_t roundMode)
{
    attrInfo.roundMode = roundMode;
}

void Conv2dTiling::SetGroups(int32_t groups)
{
    attrInfo.groups = groups;
}

void Conv2dTiling::SetOptGroupParams(int32_t enlarge, int64_t singleGroups, int64_t singleGroupOpt)
{
    this->optGroupFlag = true;
    shapeInfo.enlarge = enlarge;
    shapeInfo.singleGroups = singleGroups;
    shapeInfo.singleGroupOpt = singleGroupOpt;
}

void Conv2dTiling::CalcOptGroupParams(const ConvOriGroupInfo& oriGroupInfo, ConvOptGroupInfo& optGroupInfo) const
{
    if (oriGroupInfo.groups <= 0) {
        TILING_LOG_ERROR("Illegal params : groups=%d which must > 0.", oriGroupInfo.groups);
        return;
    }

    if (oriGroupInfo.ciPerGroup <= 0) {
        TILING_LOG_ERROR("Illegal params : ciPerGroup=%d which must > 0.", oriGroupInfo.ciPerGroup);
        return;
    }

    if (oriGroupInfo.coPerGroup <= 0) {
        TILING_LOG_ERROR("Illegal params : coPerGroup=%d which must > 0.", oriGroupInfo.coPerGroup);
        return;
    }

    uint32_t k0 = CUBE_MKN_TAB.GetMKN(oriGroupInfo.weightDtype, MKN_K_INDEX);
    uint32_t n0 = CUBE_MKN_TAB.GetMKN(oriGroupInfo.weightDtype, MKN_N_INDEX);

    optGroupInfo.enlarge = std::min(Lcm(Lcm(oriGroupInfo.ciPerGroup, k0) / oriGroupInfo.ciPerGroup,
        Lcm(oriGroupInfo.coPerGroup, n0) / oriGroupInfo.coPerGroup), static_cast<uint64_t>(oriGroupInfo.groups));
    optGroupInfo.groupOpt = CeilDiv(oriGroupInfo.groups, optGroupInfo.enlarge);
    optGroupInfo.cinOpt = oriGroupInfo.ciPerGroup * optGroupInfo.enlarge;
    optGroupInfo.coutOpt = oriGroupInfo.coPerGroup * optGroupInfo.enlarge;
}


bool Conv2dTiling::CheckAttrBeforeCoreBind()
{
    if (attrInfo.padLeft < 0 || attrInfo.padRight < 0 ||
        attrInfo.padTop < 0 || attrInfo.padBottom < 0) {
        TILING_LOG_ERROR(
            "Illlegal attrs have set: padTop=%d, padBottom=%d, padLeft=%d, padRight=%d, which must >= 0.",
            attrInfo.padTop, attrInfo.padBottom, attrInfo.padLeft, attrInfo.padRight);
        return false;
    }

    if (attrInfo.strideH <= 0 || attrInfo.strideW <= 0) {
        TILING_LOG_ERROR("Illegal attrs have set: strideH=%d, strideW=%d, which must > 0.",
                         attrInfo.strideH, attrInfo.strideW);
        return false;
    }

    if (attrInfo.dilationH <= 0 || attrInfo.dilationW <= 0) {
        TILING_LOG_ERROR("Illegal attrs have set: dilationH=%d, dilationW=%d, which must > 0.",
                         attrInfo.dilationH, attrInfo.dilationW);
        return false;
    }
    return true;
}

bool Conv2dTiling::CheckAttrBBModePhase1()
{
    if (!CheckAttrBeforeCoreBind()) {
        return false;
    }
    if (!CheckGroupsBBModePhase1()) {
        return false;
    }
    return true;
}

bool Conv2dTiling::CheckAttr()
{
    if (!CheckAttrBeforeCoreBind()) {
        return false;
    }
    if (!CheckQuantUniqueAttr()) {
        return false;
    }
    if (!CheckGroups()) {
        return false;
    }
    return true;
}

bool Conv2dTiling::CheckGroupsBBModePhase1()
{
    if (attrInfo.groups <= 0) {
        TILING_LOG_ERROR("Illegal attrs have set: groups=%d which must > 0.", attrInfo.groups);
        return false;
    }

    if (this->optGroupFlag) {
        if (shapeInfo.enlarge <= 0) {
            TILING_LOG_ERROR("Illegal attrs have set: enlarge=%d which must > 0.", shapeInfo.enlarge);
            return false;
        }
    }

    return true;
}

bool Conv2dTiling::CheckFmapShapeBeforeCoreBind()
{
    if (shapeInfo.orgHi <= 0 || shapeInfo.orgWi <= 0) {
        TILING_LOG_ERROR("Input illegal orgHi: %ld, orgWi: %ld, which must > 0.", shapeInfo.orgHi, shapeInfo.orgWi);
        return false;
    }
    if (shapeInfo.orgCi <= 0 || static_cast<uint64_t>(shapeInfo.orgCi) > MAX_31_BIT_NUM) {
        TILING_LOG_ERROR("Input illegal orgCi: %ld, which must in range [1, %lu].", shapeInfo.orgCi, MAX_31_BIT_NUM);
        return false;
    }
    int64_t ciPerg = this->optGroupFlag ? shapeInfo.singleCi / shapeInfo.enlarge : shapeInfo.singleCi;
    if (ciPerg * attrInfo.groups != shapeInfo.orgCi) {
        TILING_LOG_ERROR("only support groups * singleCi = orgCi, current groups: %d, singleCi: %ld, orgCi: %ld",
            attrInfo.groups, ciPerg, shapeInfo.orgCi);
        return false;
    }

    if (attrInfo.strideH == 0 || attrInfo.strideW == 0) {
        TILING_LOG_ERROR("div zero when calculating output shape, strideH: %d, strideW: %d.",
            attrInfo.strideH, attrInfo.strideW);
        return false;
    }
    shapeInfo.orgHo = (shapeInfo.orgHi + attrInfo.padTop + attrInfo.padBottom -
                       attrInfo.dilationH * (shapeInfo.orgkH - 1) - 1) / attrInfo.strideH + 1;
    shapeInfo.orgWo = (shapeInfo.orgWi + attrInfo.padLeft + attrInfo.padRight -
                       attrInfo.dilationW * (shapeInfo.orgkW - 1) - 1) / attrInfo.strideW + 1;
    if (shapeInfo.orgHo <= 0 || shapeInfo.orgWo <= 0) {
        TILING_LOG_ERROR("Calculated output orgHo: %ld, orgWo: %ld, which must > 0", shapeInfo.orgHo, shapeInfo.orgWo);
        return false;
    }

    return true;
}

bool Conv2dTiling::CheckFmapShapeBBModePhase1()
{
    bool ret = CheckFmapShapeBeforeCoreBind();
    return ret;
}

bool Conv2dTiling::CheckFeaMapShape()
{
    if (outputOrder == static_cast<int8_t>(OutputOrder::HW) &&
        (shapeInfo.singleHo <= 0 || shapeInfo.singleWo <= 0)) {
        TILING_LOG_ERROR("Input illegal singleHo: %ld, singleWo: %ld, which must > 0.",
            shapeInfo.singleHo, shapeInfo.singleWo);
        return false;
    }
    if (outputOrder == static_cast<int8_t>(OutputOrder::M) && shapeInfo.singleM <= 0) {
        TILING_LOG_ERROR("Input illegal singleM: %ld, which must > 0.", shapeInfo.singleM);
        return false;
    }
    bool ret = CheckFmapShapeBeforeCoreBind();
    return ret;
}

bool Conv2dTiling::CheckWeightShapeBeforeCoreBind()
{
    if (shapeInfo.orgkW <= 0 || static_cast<uint64_t>(shapeInfo.orgkW) > MAX_31_BIT_NUM) {
        TILING_LOG_ERROR("Input illegal kW: %ld, which must in range [1, %lu].", shapeInfo.orgkW, MAX_31_BIT_NUM);
        return false;
    }

    if (shapeInfo.orgkH <= 0 || static_cast<uint64_t>(shapeInfo.orgkH) > MAX_31_BIT_NUM) {
        TILING_LOG_ERROR("Input illegal kH: %ld, which must in range [1, %lu].", shapeInfo.orgkH, MAX_31_BIT_NUM);
        return false;
    }

    if (shapeInfo.orgCo <= 0 || static_cast<uint64_t>(shapeInfo.orgCo) > MAX_31_BIT_NUM) {
        TILING_LOG_ERROR("Input illegal orgCo: %ld, which must in range [1, %lu].", shapeInfo.orgCo, MAX_31_BIT_NUM);
        return false;
    }

    if (shapeInfo.singlekW != shapeInfo.orgkW) {
        TILING_LOG_ERROR("Only support singlekW = orgkW, current singlekW: %ld, orgkW: %ld",
            shapeInfo.singlekW, shapeInfo.orgkW);
        return false;
    }

    if (shapeInfo.singlekH != shapeInfo.orgkH) {
        TILING_LOG_ERROR("Only support singlekH = orgkH, current singlekH: %ld, orgkH: %ld,",
            shapeInfo.singlekH, shapeInfo.orgkH);
        return false;
    }

    return true;
}

bool Conv2dTiling::CheckWeightShapeBBModePhase1()
{
    bool ret = CheckWeightShapeBeforeCoreBind();
    return ret;
}

bool Conv2dTiling::CheckWeightShape()
{
    if (shapeInfo.singleCo <= 0 || static_cast<uint64_t>(shapeInfo.singleCo) > MAX_31_BIT_NUM) {
        TILING_LOG_ERROR("Input illegal SingleCo: %ld, which must in range [1, %lu].",
            shapeInfo.singleCo, MAX_31_BIT_NUM);
        return false;
    }

    bool ret = CheckWeightShapeBeforeCoreBind();
    return ret;
}

bool Conv2dTiling::CheckShapeBBModePhase1()
{
    if (!CheckFmapShapeBBModePhase1()) {
        return false;
    }

    if (!CheckWeightShapeBBModePhase1()) {
        return false;
    }

    return true;
}

bool Conv2dTiling::CheckShape()
{
    if (!CheckFeaMapShape()) {
        return false;
    }

    if (!CheckWeightShape()) {
        return false;
    }

    return true;
}

bool Conv2dTiling::CheckFormat()
{
    if (descInfo.weightType.format != ConvFormat::NCHW &&
        descInfo.weightType.format != ConvFormat::FRACTAL_Z_C04) {
        TILING_LOG_ERROR("unSupported weight format: %s.",
                         FORMAT_TO_STR.at(descInfo.weightType.format).c_str());
        return false;
    }

    if (descInfo.fMapType.format != ConvFormat::NCHW) {
        TILING_LOG_ERROR("unSupported feature map format: %s.",
                         FORMAT_TO_STR.at(descInfo.fMapType.format).c_str());
        return false;
    }

    return true;
}

bool Conv2dTiling::CheckAlgorithmLimit()
{
    if (outputOrder == static_cast<int8_t>(OutputOrder::M) && isC04Flag) {
        // M split mode currently not consider fixpParams
        TILING_LOG_ERROR("M split mode temporarily unSupport C04 format.");
        return false;
    }
    if (attrInfo.groups > 1 && isC04Flag) {
        // group conv2d currently not support c04 mode
        TILING_LOG_ERROR("not support C04 mode in group Conv.");
        return false;
    }

    return true;
}

bool Conv2dTiling::CheckBBmodeLimits(Conv2DBasicBlockInfo& conv2DBasicBlockInfo)
{
    int64_t groups = attrInfo.groups;
    if (this->optGroupFlag) {
        groups = shapeInfo.singleGroupOpt; // actually not single, it is ori groupopt
    }
    if (CeilDiv(shapeInfo.orgWo * shapeInfo.orgHo, conv2DBasicBlockInfo.mTile) *
        CeilDiv(shapeInfo.orgCo, conv2DBasicBlockInfo.nTile) * conv2DBasicBlockInfo.batch * groups <=
        conv2DBasicBlockInfo.aicoreNum) {
        return false;
    }
    int64_t biasSize = 0;
    int64_t scaleSize = 0;
    if (hasBias) {
        biasSize = DTYPE_SIZE_TAB.at(descInfo.biasType.dtype) * conv2DBasicBlockInfo.nTile;
    }
    if (hasQuantScale) {
        scaleSize = DTYPE_SIZE_TAB.at(descInfo.quantScaleType.dtype) * conv2DBasicBlockInfo.nTile;
    }
    int64_t availableL1Size = L1_SIZE_VALUE - biasSize - scaleSize;
    int64_t fMapDtypeSize = static_cast<int64_t>(DTYPE_SIZE_TAB.at(descInfo.fMapType.dtype));
    int64_t maxHiWiL1 = availableL1Size / fMapDtypeSize /
        static_cast<int64_t>(CONST_VALUE_2) / static_cast<int64_t>(cubeInfo.k0);
    if (maxHiWiL1 <= 0) {
        return false;
    }
    int64_t maxhiL1 = maxHiWiL1 / shapeInfo.orgWi;
    if (maxhiL1 <= static_cast<int64_t>(CONST_VALUE_2)) {
        return false;
    }
    if (conv2DBasicBlockInfo.mTile < cubeInfo.m0 || conv2DBasicBlockInfo.nTile < cubeInfo.n0) {
        return false;
    }
    return true;
}

bool Conv2dTiling::CheckL1SizeLimit()
{
    uint32_t fMapDtypeSize = DTYPE_SIZE_TAB.at(descInfo.fMapType.dtype);
    uint32_t weightDtypeSize = DTYPE_SIZE_TAB.at(descInfo.weightType.dtype);
    // ensure 32-byte alignment
    uint64_t minBiasSize = hasBias ? AlignB(cubeInfo.n0 * DTYPE_SIZE_TAB.at(descInfo.biasType.dtype), C0_SIZE) : 0;
    uint64_t minScaleSize = hasQuantScale ? AlignB(cubeInfo.n0 * DTYPE_SIZE_TAB.at(descInfo.quantScaleType.dtype),
        C0_SIZE) : 0;
    uint64_t minBL1Size = shapeInfo.orgkH * shapeInfo.orgkW * cubeInfo.n0 * cubeInfo.k0 * weightDtypeSize;
    uint64_t minAL1Size = 0;
    uint64_t minHoAL1 = min(MIN_M_L1_SIZE / shapeInfo.orgWo + CONST_VALUE_2, shapeInfo.orgHo);
    uint64_t khd = attrInfo.dilationH * (shapeInfo.orgkH - 1) + 1; // d means dilation
    uint64_t minHiAL1 = (minHoAL1 - 1) * attrInfo.strideH + khd;
    minHiAL1 = (minHiAL1 < static_cast<uint64_t>(shapeInfo.orgHi)) ? minHiAL1 : static_cast<uint64_t>(shapeInfo.orgHi);
    minAL1Size = minHiAL1 * shapeInfo.orgWi * cubeInfo.k0 * fMapDtypeSize;
    uint64_t minL1LoadSize = minAL1Size + minBL1Size + minBiasSize + minScaleSize;
    if (minL1LoadSize > L1_SIZE_VALUE) {
        return false;
    }
    return true;
}

void Conv2dTiling::PrintConv2DBasicBlockInfoPhase1(UNUSED const Conv2DBasicBlockInfo& conv2DBasicBlockInfo) const
{
    TILING_LOG_DEBUG("[Phase1]");
    TILING_LOG_DEBUG("Conv2DBasicBlock fDim is: %u", conv2DBasicBlockInfo.fDim);
    TILING_LOG_DEBUG("Conv2DBasicBlock nDim is: %u", conv2DBasicBlockInfo.nDim);
    TILING_LOG_DEBUG("Conv2DBasicBlock groupDim is: %u", conv2DBasicBlockInfo.groupDim);
    TILING_LOG_DEBUG("Conv2DBasicBlock aicoreNum is: %u", conv2DBasicBlockInfo.aicoreNum);
    TILING_LOG_DEBUG("Conv2DBasicBlock mIn is: %lu", conv2DBasicBlockInfo.mIn);
    TILING_LOG_DEBUG("Conv2DBasicBlock mTile is: %u", conv2DBasicBlockInfo.mTile);
    TILING_LOG_DEBUG("Conv2DBasicBlock nTile is: %u", conv2DBasicBlockInfo.nTile);
    TILING_LOG_DEBUG("Conv2DBasicBlock mCut is: %u", conv2DBasicBlockInfo.mCut);
    TILING_LOG_DEBUG("Conv2DBasicBlock nCut is: %u", conv2DBasicBlockInfo.nCut);
    TILING_LOG_DEBUG("Conv2DBasicBlock batch is: %u", conv2DBasicBlockInfo.batch);
}

bool Conv2dTiling::CheckConv2DBasicBlockInfoPhase1(Conv2DBasicBlockInfo& conv2DBasicBlockInfo) const
{
    PrintConv2DBasicBlockInfoPhase1(conv2DBasicBlockInfo);
    if (conv2DBasicBlockInfo.fDim == 0) {
        TILING_LOG_ERROR("fDim is illegal which is: %u", conv2DBasicBlockInfo.fDim);
        return false;
    }
    if (conv2DBasicBlockInfo.nDim == 0) {
        TILING_LOG_ERROR("nDim is illegal which is: %u", conv2DBasicBlockInfo.nDim);
        return false;
    }
    if (conv2DBasicBlockInfo.groupDim == 0) {
        TILING_LOG_ERROR("groupDim is illegal which is: %u", conv2DBasicBlockInfo.groupDim);
        return false;
    }
    if (conv2DBasicBlockInfo.aicoreNum == 0) {
        TILING_LOG_ERROR("aicoreNum is illegal which is: %u", conv2DBasicBlockInfo.aicoreNum);
        return false;
    }
    if (conv2DBasicBlockInfo.mIn == 0) {
        TILING_LOG_ERROR("mIn is illegal which is: %lu", conv2DBasicBlockInfo.mIn);
        return false;
    }
    if (conv2DBasicBlockInfo.mTile == 0) {
        TILING_LOG_ERROR("mTile is illegal which is: %u", conv2DBasicBlockInfo.mTile);
        return false;
    }
    if (conv2DBasicBlockInfo.nTile == 0) {
        TILING_LOG_ERROR("nTile is illegal which is: %u", conv2DBasicBlockInfo.nTile);
        return false;
    }
    if (conv2DBasicBlockInfo.mCut == 0) {
        TILING_LOG_ERROR("mCut is illegal which is: %u", conv2DBasicBlockInfo.mCut);
        return false;
    }
    if (conv2DBasicBlockInfo.nCut == 0) {
        TILING_LOG_ERROR("nCut is illegal which is: %u", conv2DBasicBlockInfo.nCut);
        return false;
    }
    if (conv2DBasicBlockInfo.batch == 0) {
        TILING_LOG_ERROR("batch is illegal which is: %u", conv2DBasicBlockInfo.batch);
        return false;
    }
    return true;
}

void Conv2dTiling::PrintConv2DBasicBlockInfoPhase2(UNUSED const Conv2DBasicBlockInfo& conv2DBasicBlockInfo) const
{
    TILING_LOG_DEBUG("[Phase2]");
    TILING_LOG_DEBUG("Conv2DBasicBlock batchDim is: %u", conv2DBasicBlockInfo.batchDim);
    TILING_LOG_DEBUG("Conv2DBasicBlock mDim is: %u", conv2DBasicBlockInfo.mDim);
    TILING_LOG_DEBUG("Conv2DBasicBlock nDim is: %u", conv2DBasicBlockInfo.nDim);
    TILING_LOG_DEBUG("Conv2DBasicBlock groupDim is: %u", conv2DBasicBlockInfo.groupDim);
    TILING_LOG_DEBUG("Conv2DBasicBlock mCut is: %u", conv2DBasicBlockInfo.mCut);
    TILING_LOG_DEBUG("Conv2DBasicBlock nCut is: %u", conv2DBasicBlockInfo.nCut);
    TILING_LOG_DEBUG("Conv2DBasicBlock mTile is: %u", conv2DBasicBlockInfo.mTile);
    TILING_LOG_DEBUG("Conv2DBasicBlock nTile is: %u", conv2DBasicBlockInfo.nTile);
    TILING_LOG_DEBUG("Conv2DBasicBlock mIn is: %u", conv2DBasicBlockInfo.mIn);
}

bool Conv2dTiling::CheckConv2DBasicBlockInfoPhase2(Conv2DBasicBlockInfo& conv2DBasicBlockInfo) const
{
    PrintConv2DBasicBlockInfoPhase2(conv2DBasicBlockInfo);
    if (conv2DBasicBlockInfo.batchDim == 0) {
        TILING_LOG_ERROR("batchDim is illegal which is: %u", conv2DBasicBlockInfo.batchDim);
        return false;
    }
    if (conv2DBasicBlockInfo.mDim == 0) {
        TILING_LOG_ERROR("mDim is illegal which is: %u", conv2DBasicBlockInfo.mDim);
        return false;
    }
    if (conv2DBasicBlockInfo.nDim == 0) {
        TILING_LOG_ERROR("nDim is illegal which is: %u", conv2DBasicBlockInfo.nDim);
        return false;
    }
    if (conv2DBasicBlockInfo.mTile == 0) {
        TILING_LOG_ERROR("mTile is illegal which is: %u", conv2DBasicBlockInfo.mTile);
        return false;
    }
    if (conv2DBasicBlockInfo.nTile == 0) {
        TILING_LOG_ERROR("nTile is illegal which is: %u", conv2DBasicBlockInfo.nTile);
        return false;
    }
    if (conv2DBasicBlockInfo.mIn == 0) {
        TILING_LOG_ERROR("mIn is illegal which is: %u", conv2DBasicBlockInfo.mIn);
        return false;
    }
    if (conv2DBasicBlockInfo.iterateMNOrder == IterateMNOrder::INVALID) {
        TILING_LOG_ERROR("iterateMNOrder is illegal which is: %u (INVALID)", conv2DBasicBlockInfo.iterateMNOrder);
        return false;
    }
    return true;
}

bool Conv2dTiling::CheckTilingAlgorithmType(Conv2DBasicBlockInfo& conv2DBasicBlockInfo, int8_t phase)
{
    if(phase == BB_PHASE_1) {
        if (!CheckParamsBBModePhase1()) { // General Check
            TILING_LOG_ERROR("conv2d api tiling general params check failed in phase 1.");
            return false;
        }
    } else {
        if (!CheckParams()) {
            TILING_LOG_ERROR("conv2d api tiling general params check failed in phase 2.");
            return false;
        }
    }
    InferCubeInfo();
    if(!CheckL1SizeLimit()) { // L1size check
        TILING_LOG_ERROR("conv2d api tiling L1size check failed.");
        return false;
    }

    if(phase == BB_PHASE_1) {
        if (!CheckConv2DBasicBlockInfoPhase1(conv2DBasicBlockInfo)) {
            TILING_LOG_ERROR("conv2d basic block info check failed in phase 1.");
            return false;
        }
    } else {
        if(!CheckConv2DBasicBlockInfoPhase2(conv2DBasicBlockInfo)) {
            TILING_LOG_ERROR("conv2d basic block info check failed in phase 2.");
            return false;
        }
    }
    if(CheckBBmodeLimits(conv2DBasicBlockInfo)) { // Basic Block Algorithm Check
        tilingAlgorithmType = static_cast<int8_t>(TilingAlgorithmType::BASIC_BLOCK);
    }
    switch (tilingAlgorithmType) {
        case static_cast<int8_t>(TilingAlgorithmType::BASIC_BLOCK):
            return true;
        case static_cast<int8_t>(TilingAlgorithmType::FORMULAS):
        default:
            TILING_LOG_ERROR("Unsupported tiling algorithm type %d, only support BASIC_BLOCK(%d).",
                tilingAlgorithmType, TilingAlgorithmType::BASIC_BLOCK);
            return false;
    }
    return true;
}

bool Conv2dTiling::GetCoreBindingDecisionFactor(Conv2DBasicBlockInfo& conv2DBasicBlockInfo)
{
    if (!CheckTilingAlgorithmType(conv2DBasicBlockInfo, BB_PHASE_1)) {
        return false;
    }

    Infer5hdShape();
    std::shared_ptr<ConvTilingAlgorithmBBmode> algoBBPtr = std::make_shared<ConvTilingAlgorithmBBmode>(this,
        conv2DBasicBlockInfo);
    algoBBPtr->AdjustM();
    algoBBPtr->AdjustN();
    algoBBPtr->CalcBestL1LoadStratgy();
    algoBBPtr->CalcCoreUtilization();
    return true;
}
} // namespace conv_tiling