/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "quantization/ascend_antiquant.h"

__aicore__ void TestAscendAntiQuant0() {
    AscendC::LocalTensor<half> dst;
    AscendC::LocalTensor<int8_t> src;
    AscendC::LocalTensor<half> offset;
    AscendC::LocalTensor<half> scale;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t k = 0;
    AscendC::AntiQuantShapeInfo shapeInfo;
    AscendC::AscendAntiQuant<int8_t, half, false>(dst, src, offset, scale, sharedTmpBuffer, k, shapeInfo);
}

__aicore__ void TestAscendAntiQuant1() {
    AscendC::LocalTensor<half> dst;
    AscendC::LocalTensor<int8_t> src;
    AscendC::LocalTensor<half> scale;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t k = 0;
    AscendC::AntiQuantShapeInfo shapeInfo;
    AscendC::AscendAntiQuant<int8_t, half, false>(dst, src, scale, sharedTmpBuffer, k, shapeInfo);
}

__aicore__ void TestAscendAntiQuant2() {
    AscendC::LocalTensor<half> dst;
    AscendC::LocalTensor<int8_t> src;
    AscendC::LocalTensor<half> offset;
    AscendC::LocalTensor<half> scale;
    uint32_t k = 0;
    AscendC::AntiQuantShapeInfo shapeInfo;
    AscendC::AscendAntiQuant<int8_t, half, false>(dst, src, offset, scale, k, shapeInfo);
}

__aicore__ void TestAscendAntiQuant3() {
    AscendC::LocalTensor<half> dst;
    AscendC::LocalTensor<int8_t> src;
    half scaleValue = half(1.0f);
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t k = 0;
    AscendC::AntiQuantShapeInfo shapeInfo;
    AscendC::AscendAntiQuant<int8_t, half, false>(dst, src, scaleValue, sharedTmpBuffer, k, shapeInfo);
}

__aicore__ void TestAscendAntiQuant4() {
    AscendC::LocalTensor<half> dst;
    AscendC::LocalTensor<int8_t> src;
    half offsetValue = half(0.0f);
    half scaleValue = half(1.0f);
    uint32_t k = 0;
    AscendC::AntiQuantShapeInfo shapeInfo;
    AscendC::AscendAntiQuant<int8_t, half, false>(dst, src, offsetValue, scaleValue, k, shapeInfo);
}

