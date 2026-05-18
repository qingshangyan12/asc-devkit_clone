/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "math/clamp.h"

void TestClampMax0() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    half maxValue;
    uint32_t calCount = 0;
    AscendC::ClampMax<half>(dstTensor, srcTensor, sharedTmpBuffer, maxValue, calCount);
}

void TestClampMax1() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    half maxValue;
    uint32_t calCount = 0;
    AscendC::ClampMax<half>(dstTensor, srcTensor, maxValue, calCount);
}

void TestClampMin0() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    half minValue;
    uint32_t calCount = 0;
    AscendC::ClampMin<half>(dstTensor, srcTensor, sharedTmpBuffer, minValue, calCount);
}

void TestClampMin1() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    half minValue;
    uint32_t calCount = 0;
    AscendC::ClampMin<half>(dstTensor, srcTensor, minValue, calCount);
}

void TestClamp0() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    half minValue;
    half maxValue;
    uint32_t calCount = 0;
    AscendC::Clamp<half, half, half>(dstTensor, srcTensor, minValue, maxValue, calCount);
}
