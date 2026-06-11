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

// sizeof(TConv2DTiling) must algin with 8Byte, type sort
namespace optiling {
BEGIN_TILING_DATA_DEF(TConv2DTiling)
// 
TILING_DATA_FIELD_DEF(uint64_t, orgHi);
TILING_DATA_FIELD_DEF(uint64_t, orgWi);
TILING_DATA_FIELD_DEF(uint64_t, oriHixWi);
TILING_DATA_FIELD_DEF(uint64_t, orgHo);
TILING_DATA_FIELD_DEF(uint64_t, orgWo);

// 切分块
TILING_DATA_FIELD_DEF(uint64_t, singleCoreBatch);
TILING_DATA_FIELD_DEF(uint64_t, singleCoreHo);
TILING_DATA_FIELD_DEF(uint64_t, singleCoreWo);

TILING_DATA_FIELD_DEF(uint32_t, orgCi);
TILING_DATA_FIELD_DEF(uint32_t, orgCo);

// 
TILING_DATA_FIELD_DEF(uint32_t, singleCoreCi);
TILING_DATA_FIELD_DEF(uint32_t, singleCoreCo);

// 
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

// 
// TILING_DATA_FIELD_DEF(uint32_t, groups);
// TILING_DATA_FIELD_DEF(uint32_t, singleCoreGroups);
// TILING_DATA_FIELD_DEF(uint32_t, singleCoreGroupOpt);
// TILING_DATA_FIELD_DEF(uint32_t, enlarge);
// 切分块
TILING_DATA_FIELD_DEF(uint32_t, hoL1);
TILING_DATA_FIELD_DEF(uint32_t, woL1);
TILING_DATA_FIELD_DEF(uint32_t, kAL1);
TILING_DATA_FIELD_DEF(uint32_t, kBL1);
TILING_DATA_FIELD_DEF(uint32_t, nBL1);
TILING_DATA_FIELD_DEF(uint32_t, hoL0);
TILING_DATA_FIELD_DEF(uint32_t, woL0);
TILING_DATA_FIELD_DEF(uint32_t, kL0);
TILING_DATA_FIELD_DEF(uint32_t, nL0);
// 
TILING_DATA_FIELD_DEF(uint32_t, pBufferFlag);
// [zj]TODO: to del
TILING_DATA_FIELD_DEF(uint32_t, cinAInCore); // min(singleCoreCi, kAL1 // kernelHxkernelW)
TILING_DATA_FIELD_DEF(uint32_t, cinATailInCore); // (kAL1 if (align(singleCoreCi * kernelHxkernelW, BLOCK_SIZE // dataTypeSize]) % kAL1) == 0 else align(singleCoreCi * kernelHxkernelW, BLOCK_SIZE // dataTypeSize]) % kAL1) // kernelHxkernelW
TILING_DATA_FIELD_DEF(uint32_t, cinBInCore); // min(singleCoreCi, kBL1 // kernelHxkernelW)
TILING_DATA_FIELD_DEF(uint32_t, cinBTailInCore); // (kBL1 if (align(singleCoreCi * kernelHxkernelW, BLOCK_SIZE // dataTypeSize) % kBL1) == 0 else align(singleCoreCi * kernelHxkernelW, BLOCK_SIZE // dataTypeSize) % kBL1) // kernelHxkernelW
// [zj]TODO: to del
TILING_DATA_FIELD_DEF(uint32_t, aL1SpaceSize); // align((cinAInCore * min((min(min(hoL1, singleCoreHo) // orgWo, orgHo) - 1) * strideH + ((kernelH - 1) * dilationH + 1), orgHi) * orgWi) * 2, BLOCK_SIZE)
// 
TILING_DATA_FIELD_DEF(uint32_t, multiNBL1); // ceil(nBL1, nL0)
// [zj]TODO: to del
TILING_DATA_FIELD_DEF(uint32_t, mStep); // align(hoL0, m0) // 与kStep、nStep含义不同
TILING_DATA_FIELD_DEF(uint32_t, kStep); // kL0 // k0
TILING_DATA_FIELD_DEF(uint32_t, nStep); // ceil(nL0, n0)
// [zj]TODO: to del
TILING_DATA_FIELD_DEF(uint32_t, fmapKStride); // mStep // m0 // 与weightKStride含义不同
TILING_DATA_FIELD_DEF(uint32_t, weightKStride); // ceil(nBL1, n0) // 这里为什么是nBL1，而不是nBL0
// [zj]TODO: to del
TILING_DATA_FIELD_DEF(uint32_t, cinOffsetBlockInGM); // kAL1 // kernelHxkernelW * oriHinxWin
TILING_DATA_FIELD_DEF(uint32_t, coutOffsetBlock); // (orgCi // groups) * kernelHxkernelW
TILING_DATA_FIELD_DEF(uint32_t, nL1DivBlockSize); // nBL1 // n0
// 
TILING_DATA_FIELD_DEF(uint8_t, biasFullLoadFlag);
TILING_DATA_FIELD_DEF(uint8_t, fixpParamsFullLoadFlag);
// 
TILING_DATA_FIELD_DEF(uint8_t, iterateMNOrder);
// 
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

TILING_DATA_FIELD_DEF(uint8_t, preQuantMode);
TILING_DATA_FIELD_DEF(uint8_t, preReluMode);
TILING_DATA_FIELD_DEF(uint8_t, preClipReluMode);
TILING_DATA_FIELD_DEF(uint8_t, postQuantMode);
TILING_DATA_FIELD_DEF(uint8_t, postReluMode);
TILING_DATA_FIELD_DEF(uint8_t, postClipReluMode);

TILING_DATA_FIELD_DEF(uint8_t, eltwiseOp);
TILING_DATA_FIELD_DEF(uint8_t, eltwiseBrcEn);

TILING_DATA_FIELD_DEF(uint8_t, biasBrcEn);

TILING_DATA_FIELD_DEF(uint8_t, dualOutput);

// TILING_DATA_FIELD_DEF(int8_t, offsetx);
// TILING_DATA_FIELD_DEF(int8_t, roundMode);
// 
// TILING_DATA_FIELD_DEF(uint8_t, hf32Enable);
// TILING_DATA_FIELD_DEF(uint8_t, hf32TransMode);
// 
// TILING_DATA_FIELD_DEF(int8_t, reserved1);
// TILING_DATA_FIELD_DEF(int8_t, reserved2);
END_TILING_DATA_DEF;

// [zj]ToDo重复包含会不会有问题

// REGISTER_TILING_DATA_CLASS(TConv2DTilingOpApi, TConv2DTiling);
}
#endif
