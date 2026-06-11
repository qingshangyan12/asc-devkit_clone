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
 * \file conv_tiling_base.cpp
 * \brief
 */

#include <cstdint>
#include <unordered_set>
#include "conv_tiling_base.h"

using namespace std;

namespace conv_tiling {
ConvTilingBase::ConvTilingBase(const PlatformInfo& platform)
{
    platformInfo.socVersion = platform.socVersion;
    platformInfo.supportWeightBypass = IsPlatformSupportWeightBypass(platformInfo.socVersion);
    platformInfo.abL1mte2BandWidthCof = GetBandWidthCof(platformInfo.socVersion);
    platformInfo.l1Size = static_cast<uint64_t>(platform.l1Size);
    platformInfo.l0ASize = static_cast<uint64_t>(platform.l0ASize);
    platformInfo.l0BSize = static_cast<uint64_t>(platform.l0BSize);
    platformInfo.l0CSize = static_cast<uint64_t>(platform.l0CSize);
    platformInfo.ubSize = static_cast<uint64_t>(platform.ubSize);
    platformInfo.btSize = static_cast<uint64_t>(platform.btSize);
    platformInfo.fbSize = static_cast<uint64_t>(platform.fbSize);
}

void ConvTilingBase::InferCubeInfo()
{
    cubeInfo.m0 = CUBE_MKN_TAB.GetMKN(descInfo.fMapType.dtype, MKN_M_INDEX);
    // [zj]ToDo
    cubeInfo.k0 = CUBE_MKN_TAB.GetMKN(descInfo.fMapType.dtype, MKN_K_INDEX);
    cubeInfo.n0 = CUBE_MKN_TAB.GetMKN(descInfo.fMapType.dtype, MKN_N_INDEX);
    cubeInfo.biasType = CUBE_TYPE_TAB.ToBiasType(descInfo.fMapType.dtype);
    cubeInfo.madType = CUBE_TYPE_TAB.ToMadType(descInfo.fMapType.dtype);
}

int64_t ConvTilingBase::CheckTilingRes()
{
    if (outputOrder == static_cast<int8_t>(OutputOrder::M) &&
        (l1TilingInfo.mAL1 == 0 || l0TilingInfo.mL0 == 0)) {
        TILING_LOG_ERROR("Check tiling res failed: mAL1: %lu, mL0: %lu.", l1TilingInfo.mAL1, l0TilingInfo.mL0);
        return -1;
    }
    if (outputOrder == static_cast<int8_t>(OutputOrder::HW) &&
        (l1TilingInfo.hoAL1 == 0 || l1TilingInfo.woAL1 == 0 || l0TilingInfo.hoL0 == 0 || l0TilingInfo.woL0 == 0)) {
        TILING_LOG_ERROR("Check tiling res failed: hoAL1: %lu, woAL1: %lu, hoL0: %lu, woL0: %lu.",
            l1TilingInfo.hoAL1, l1TilingInfo.woAL1, l0TilingInfo.hoL0, l0TilingInfo.woL0);
        return -1;
    }

    if (l1TilingInfo.kAL1 == 0 || l1TilingInfo.kBL1 == 0 || l1TilingInfo.nBL1 == 0) {
        TILING_LOG_ERROR("Check tiling res failed: kAL1: %lu, kBL1: %lu, nBL1: %lu.",
            l1TilingInfo.kAL1, l1TilingInfo.kBL1, l1TilingInfo.nBL1);
        return -1;
    }

    if (l0TilingInfo.kL0 == 0 || l0TilingInfo.nL0 == 0) {
        TILING_LOG_ERROR("Check tiling res failed: kL0: %lu, nL0: %lu.", l0TilingInfo.kL0, l0TilingInfo.nL0);
        return -1;
    }

    return 0;
}

bool ConvTilingBase::IsPlatformSupportWeightBypass(platform_ascendc::SocVersion curSoc) const
{
    static unordered_set<platform_ascendc::SocVersion> supportedSoc = {platform_ascendc::SocVersion::ASCEND910B};
    if (supportedSoc.find(curSoc) == supportedSoc.end()) {
        return false;
    }
    return true;
}

uint32_t ConvTilingBase::GetBandWidthCof(platform_ascendc::SocVersion curSoc) const
{
    switch (curSoc) {
        // [zj]ToDo
        // case platform_ascendc::SocVersion::ASCEND910B:
        //     return 1;
        // case platform_ascendc::SocVersion::ASCEND910D:
        //     return BAND_WIDTH_COEFF;
        default:
            return 0;
    }
}

bool ConvTilingBase::CheckQuantUniqueAttr()
{
    if (hasQuantScale) {
        ConvDtype fMapDtype = descInfo.fMapType.dtype;
        ConvDtype outputDtype = descInfo.outputType.dtype;
        if (attrInfo.offsetx != 0 &&
            (fMapDtype == ConvDtype::HIFLOAT8 || fMapDtype == ConvDtype::FLOAT8_E4M3FN)) {
                TILING_LOG_ERROR(
                    "Only support offsetX = 0 in hif8 or fp8 mode, actually is %d",
                    attrInfo.offsetx);
                return false;
        }
        if (outputDtype == ConvDtype::HIFLOAT8) {
            if (attrInfo.roundMode != conv_tiling::ROUND_MODE_HALFTOAWAY &&
            attrInfo.roundMode != conv_tiling::ROUND_MODE_HYBRID) {
                TILING_LOG_ERROR(
                    "Only support round or hybrid in hif8 dtype, actually is %d",
                    attrInfo.roundMode);
                return false;
            }
        } else {
            if (attrInfo.roundMode != conv_tiling::ROUND_MODE_RINT) {
                TILING_LOG_ERROR("Only support rint in not_hif8 dtype, actually is %d",
                    attrInfo.roundMode);
                return false;
            }
        }
    }
    return true;
}

bool ConvTilingBase::CheckGroups()
{
    if (attrInfo.groups <= 0) {
        TILING_LOG_ERROR("Illegal attrs have set: groups=%d which must > 0.", attrInfo.groups);
        return false;
    }
    if (optGroupFlag) {//this->optGroupFlag
        if (shapeInfo.singleGroups <= 0) {
            TILING_LOG_ERROR("Illegal attrs have set: singleGroups=%d which must > 0.", shapeInfo.singleGroups);
            return false;
        }
        if (shapeInfo.enlarge <= 0) {
            TILING_LOG_ERROR("Illegal attrs have set: enlarge=%d which must > 0.", shapeInfo.enlarge);
            return false;
        }
        if (shapeInfo.singleGroupOpt <= 0) {
            TILING_LOG_ERROR("Input illegal singleGroupOpt: %ld, which must > 0.", shapeInfo.singleGroupOpt);
            return false;
        }
    }
    return true;
}

bool ConvTilingBase::CheckDtype()
{
    vector<ConvDtype> paramsType;
    if (hasBias) {
        paramsType = {descInfo.fMapType.dtype, descInfo.weightType.dtype,
                      descInfo.biasType.dtype, descInfo.outputType.dtype};
    } else {
        paramsType = {descInfo.fMapType.dtype, descInfo.weightType.dtype,
                      descInfo.outputType.dtype};
    }
    auto supportedTypesList = GetSupportedDataTypes();
    for (uint64_t kindsId = 0; kindsId < supportedTypesList.size(); ++kindsId) {
        if (supportedTypesList[kindsId].size() == 0) {
            continue;
        }
        if (IsEqual(paramsType, supportedTypesList[kindsId], supportedTypesList[kindsId].size())) {
            return true;
        }
    }
    if (hasBias) {
        TILING_LOG_ERROR("unSupported params data type [fmap, weight, bias, output]: [%s, %s, %s, %s].",
            DTYPE_TO_STR.at(descInfo.fMapType.dtype).c_str(),
            DTYPE_TO_STR.at(descInfo.weightType.dtype).c_str(),
            DTYPE_TO_STR.at(descInfo.biasType.dtype).c_str(),
            DTYPE_TO_STR.at(descInfo.outputType.dtype).c_str());
    } else {
        TILING_LOG_ERROR("unSupported params data type [fmap, weight, output]: [%s, %s, %s].",
            DTYPE_TO_STR.at(descInfo.fMapType.dtype).c_str(),
            DTYPE_TO_STR.at(descInfo.weightType.dtype).c_str(),
            DTYPE_TO_STR.at(descInfo.outputType.dtype).c_str());
    }
    return false;
}

vector<vector<ConvDtype>> ConvTilingBase::GetSupportedDataTypes() const
{
    vector<vector<ConvDtype>> res;
    if (hasBias) {
        // [fmap, weight, bias, output]
        if (hasQuantScale) {
            res = {{ConvDtype::INT8, ConvDtype::INT8, ConvDtype::INT32, ConvDtype::FLOAT16},
                {ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8, ConvDtype::FLOAT32, ConvDtype::FLOAT32},
                {ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8, ConvDtype::FLOAT32, ConvDtype::FLOAT16},
                {ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8, ConvDtype::FLOAT32, ConvDtype::BFLOAT16},
                {ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8, ConvDtype::FLOAT32, ConvDtype::HIFLOAT8},
                {ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT32, ConvDtype::FLOAT32},
                {ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT32, ConvDtype::FLOAT16},
                {ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT32, ConvDtype::BFLOAT16},
                {ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT32, ConvDtype::FLOAT8_E4M3FN}};
        } else {
            res = {{ConvDtype::FLOAT16, ConvDtype::FLOAT16, ConvDtype::FLOAT16, ConvDtype::FLOAT16},
                {ConvDtype::FLOAT32, ConvDtype::FLOAT32, ConvDtype::FLOAT32, ConvDtype::FLOAT32},
                {ConvDtype::BFLOAT16, ConvDtype::BFLOAT16, ConvDtype::BFLOAT16, ConvDtype::BFLOAT16},
                {ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8, ConvDtype::FLOAT32, ConvDtype::HIFLOAT8}};
        }
    } else {
        // [fmap, weight, output]
        if (hasQuantScale) {
            res = {{ConvDtype::INT8, ConvDtype::INT8, ConvDtype::FLOAT16},
                {ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8, ConvDtype::FLOAT32},
                {ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8, ConvDtype::FLOAT16},
                {ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8, ConvDtype::BFLOAT16},
                {ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8},
                {ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT32},
                {ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT16},
                {ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT8_E4M3FN, ConvDtype::BFLOAT16},
                {ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT8_E4M3FN, ConvDtype::FLOAT8_E4M3FN}};
        } else {
            res = {{ConvDtype::FLOAT16, ConvDtype::FLOAT16, ConvDtype::FLOAT16},
                {ConvDtype::FLOAT32, ConvDtype::FLOAT32, ConvDtype::FLOAT32},
                {ConvDtype::BFLOAT16, ConvDtype::BFLOAT16, ConvDtype::BFLOAT16},
                {ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8, ConvDtype::HIFLOAT8}};
        }
    }
    return res;
}

bool ConvTilingBase::CheckLoad3DLimits()
{
    if (static_cast<uint32_t>(attrInfo.strideH) > LOAD3D_MAX_STRIDE_H_W ||
        static_cast<uint32_t>(attrInfo.strideW) > LOAD3D_MAX_STRIDE_H_W) {
        TILING_LOG_ERROR(
            "Attrs not satisfying load3d's limits: strideH=%d, strideW=%d, which must <= %u.",
            attrInfo.strideH, attrInfo.strideW, LOAD3D_MAX_STRIDE_H_W);
        return false;
    }
    if (static_cast<uint32_t>(attrInfo.dilationH) > LOAD3D_MAX_DILATION_H_W ||
        static_cast<uint32_t>(attrInfo.dilationW) > LOAD3D_MAX_DILATION_H_W) {
        TILING_LOG_ERROR(
            "Attrs not satisfying load3d's limits: dilationH=%d, dilationW=%d, which must <= %u.",
            attrInfo.dilationH, attrInfo.dilationW, LOAD3D_MAX_DILATION_H_W);
        return false;
    }
    if (static_cast<uint32_t>(attrInfo.padLeft) > LOAD3D_MAX_PAD ||
        static_cast<uint32_t>(attrInfo.padRight) > LOAD3D_MAX_PAD ||
        static_cast<uint32_t>(attrInfo.padTop) > LOAD3D_MAX_PAD ||
        static_cast<uint32_t>(attrInfo.padBottom) > LOAD3D_MAX_PAD) {
        TILING_LOG_ERROR(
            "Attrs not satisfying load3d's limits: padTop=%d, padBottom=%d, padLeft=%d, padRight=%d, which must <= %u.",
            attrInfo.padTop, attrInfo.padBottom, attrInfo.padLeft, attrInfo.padRight,
            LOAD3D_MAX_PAD);
        return false;
    }
    if (shapeInfo.orgkH > LOAD3D_MAX_FILTER_H_W_910D || shapeInfo.orgkW > LOAD3D_MAX_FILTER_H_W_910D) {
        TILING_LOG_ERROR("Weight shpae not satisfy Load3D's limits: kh=%lu, kw=%lu, which must <= %u.",
                         shapeInfo.orgkH, shapeInfo.orgkW, LOAD3D_MAX_FILTER_H_W_910D);
        return false;
    }
    uint64_t tmpkHWSize = shapeInfo.orgkH * shapeInfo.orgkW * cubeInfo.k0;
    if (tmpkHWSize > LOAD3D_MAX_DDR2L1_SIZE) {
        TILING_LOG_ERROR("Weight shape not satisfying load3d's limits: kH*kW*k0=%lu, which must <= %u.",
                         tmpkHWSize, LOAD3D_MAX_DDR2L1_SIZE);
        return false;
    }
    return true;
}

bool ConvTilingBase::CheckDataCopyLimits()
{
    uint64_t loadAL1loop1SrcStrideLimits = MAX_40_BIT_NUM;
    int64_t tmpOrgDi = (shapeInfo.orgDi <= 0) ? 1 : shapeInfo.orgDi;
    uint64_t loadAL1loop1SrcStride = shapeInfo.orgHi * shapeInfo.orgWi * tmpOrgDi *
                                     DTYPE_SIZE_TAB.at(descInfo.fMapType.dtype);
    if (descInfo.fMapType.format == ConvFormat::NCHW && loadAL1loop1SrcStride > loadAL1loop1SrcStrideLimits) {
        TILING_LOG_ERROR(
            "Fmap shape not satisfy DataCopy's limits: din(%ld)*hin(%ld)*win(%ld)*typesize(%u)=%lu, must <= %lu",
            tmpOrgDi, shapeInfo.orgHi, shapeInfo.orgWi,
            DTYPE_SIZE_TAB.at(descInfo.fMapType.dtype),
            loadAL1loop1SrcStride, loadAL1loop1SrcStrideLimits);
        return false;
    }
    return true;
}

bool ConvTilingBase::CheckFixpipeLimits()
{
    uint64_t fixpipeLoop2DstStrideLimit = MAX_32_BIT_NUM;
    int64_t tmpOrgDo = (shapeInfo.orgDo <= 0) ? 1 : shapeInfo.orgDo;
    uint64_t fixpipeLoop2DstStride = static_cast<uint64_t>(shapeInfo.orgHo) * shapeInfo.orgWo * tmpOrgDo;
    if (fixpipeLoop2DstStride > fixpipeLoop2DstStrideLimit) {
        TILING_LOG_ERROR(
            "Output shape not satisfy Fixpipe's limits: dout(%ld)*hout(%ld)*wout(%ld)=%lu, must <= %lu",
            tmpOrgDo, shapeInfo.orgHo, shapeInfo.orgWo,
            fixpipeLoop2DstStride, fixpipeLoop2DstStrideLimit);
        return false;
    }
    return true;
}

bool ConvTilingBase::CheckInstructionLimits()
{
    if (!CheckLoad3DLimits()) {
        return false;
    }
    if (!CheckDataCopyLimits()) {
        return false;
    }
    if (!CheckFixpipeLimits()) {
        return false;
    }
    return true;
}
}
