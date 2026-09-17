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
 * \file conv2d_tilingdata.h
 * \brief
 */

#ifndef ADV_API_CONV_CONV2D_CONV2D_TILINGDATA_H
#define ADV_API_CONV_CONV2D_CONV2D_TILINGDATA_H

#include "register/tilingdata_base.h"

namespace optiling {
BEGIN_TILING_DATA_DEF(TConv2DApiTiling)
TILING_DATA_FIELD_DEF(uint64_t, orgHi);
TILING_DATA_FIELD_DEF(uint64_t, orgWi);
TILING_DATA_FIELD_DEF(uint64_t, oriHixWi);
TILING_DATA_FIELD_DEF(uint64_t, orgHo);
TILING_DATA_FIELD_DEF(uint64_t, orgWo);
TILING_DATA_FIELD_DEF(uint64_t, singleCoreHo);
TILING_DATA_FIELD_DEF(uint64_t, singleCoreWo);
TILING_DATA_FIELD_DEF(uint32_t, orgCi);
TILING_DATA_FIELD_DEF(uint32_t, orgCo);
TILING_DATA_FIELD_DEF(uint32_t, singleCoreCi);
TILING_DATA_FIELD_DEF(uint32_t, singleCoreCo);
TILING_DATA_FIELD_DEF(uint32_t, kernelH);
TILING_DATA_FIELD_DEF(uint32_t, kernelW);
TILING_DATA_FIELD_DEF(uint32_t, kernelHxkernelW);
TILING_DATA_FIELD_DEF(uint32_t, strideH);
TILING_DATA_FIELD_DEF(uint32_t, strideW);
TILING_DATA_FIELD_DEF(uint32_t, dilationH);
TILING_DATA_FIELD_DEF(uint32_t, dilationW);
TILING_DATA_FIELD_DEF(uint32_t, padTop);
TILING_DATA_FIELD_DEF(uint32_t, padBottom);
TILING_DATA_FIELD_DEF(uint32_t, padLeft);
TILING_DATA_FIELD_DEF(uint32_t, padRight);
TILING_DATA_FIELD_DEF(uint32_t, groups);
TILING_DATA_FIELD_DEF(uint32_t, hoL1);
TILING_DATA_FIELD_DEF(uint32_t, woL1);
TILING_DATA_FIELD_DEF(uint32_t, kAL1);
TILING_DATA_FIELD_DEF(uint32_t, kBL1);
TILING_DATA_FIELD_DEF(uint32_t, nBL1);
TILING_DATA_FIELD_DEF(uint32_t, hoL0);
TILING_DATA_FIELD_DEF(uint32_t, woL0);
TILING_DATA_FIELD_DEF(uint32_t, kL0);
TILING_DATA_FIELD_DEF(uint32_t, nL0);
TILING_DATA_FIELD_DEF(uint32_t, khL0);
TILING_DATA_FIELD_DEF(uint32_t, kwL0);
TILING_DATA_FIELD_DEF(uint32_t, pBufferFlag);
TILING_DATA_FIELD_DEF(uint32_t, cinAInCore);
TILING_DATA_FIELD_DEF(uint32_t, cinATailInCore);
TILING_DATA_FIELD_DEF(uint32_t, cinBInCore);
TILING_DATA_FIELD_DEF(uint32_t, cinBTailInCore);
TILING_DATA_FIELD_DEF(uint32_t, a1SpaceSize);
TILING_DATA_FIELD_DEF(uint32_t, b1SpaceSize);
TILING_DATA_FIELD_DEF(uint32_t, c0SpaceSize);
TILING_DATA_FIELD_DEF(uint32_t, multiNBL1);
TILING_DATA_FIELD_DEF(uint32_t, mStep);
TILING_DATA_FIELD_DEF(uint32_t, kStep);
TILING_DATA_FIELD_DEF(uint32_t, nStep);
TILING_DATA_FIELD_DEF(uint8_t, biasFullLoadFlag);
TILING_DATA_FIELD_DEF(uint8_t, fixpParamsFullLoadFlag);
TILING_DATA_FIELD_DEF(uint8_t, iterateMNOrder);
TILING_DATA_FIELD_DEF(uint8_t, isC04Flag);
TILING_DATA_FIELD_DEF(uint8_t, hasBias);
TILING_DATA_FIELD_DEF(uint8_t, hasPreQuant);
TILING_DATA_FIELD_DEF(uint8_t, hasPreRelu);
TILING_DATA_FIELD_DEF(uint8_t, hasPreClipRelu);
TILING_DATA_FIELD_DEF(uint8_t, hasAntiQuant);
TILING_DATA_FIELD_DEF(uint8_t, hasEltwise);
TILING_DATA_FIELD_DEF(uint8_t, hasPostQuant);
TILING_DATA_FIELD_DEF(uint8_t, hasPostRelu);
TILING_DATA_FIELD_DEF(uint8_t, hasPostClipRelu);
TILING_DATA_FIELD_DEF(uint8_t, hasPadding);
TILING_DATA_FIELD_DEF(uint8_t, preQuantMode);
TILING_DATA_FIELD_DEF(uint8_t, preReluMode);
TILING_DATA_FIELD_DEF(uint8_t, preClipReluMode);
TILING_DATA_FIELD_DEF(uint8_t, antiQuantMode);
TILING_DATA_FIELD_DEF(uint8_t, eltwiseOp);
TILING_DATA_FIELD_DEF(uint8_t, eltwiseBrcEn);
TILING_DATA_FIELD_DEF(uint8_t, postQuantMode);
TILING_DATA_FIELD_DEF(uint8_t, postReluMode);
TILING_DATA_FIELD_DEF(uint8_t, postClipReluMode);
TILING_DATA_FIELD_DEF(uint8_t, paddingMode);
TILING_DATA_FIELD_DEF(uint8_t, dualOutput);
TILING_DATA_FIELD_DEF(uint8_t, bitMask);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(TConv2DApiTilingOpApi, TConv2DApiTiling);
}
#endif
