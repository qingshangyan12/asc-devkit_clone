/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "activation/swiglu.h"

__aicore__ void TestSwiGLU0() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> src0Tensor;
    AscendC::LocalTensor<float> src1Tensor;
    float scalarValue = 0.0f;
    AscendC::SwiGLU<float>(dstTensor, src0Tensor, src1Tensor, scalarValue);
}

__aicore__ void TestSwiGLU1() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> src0Tensor;
    AscendC::LocalTensor<float> src1Tensor;
    float scalarValue = 0.0f;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::SwiGLU<float>(dstTensor, src0Tensor, src1Tensor, scalarValue, sharedTmpBuffer);
}

__aicore__ void TestSwiGLU2() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> src0Tensor;
    AscendC::LocalTensor<float> src1Tensor;
    float scalarValue = 0.0f;
    uint32_t calCount = 0;
    AscendC::SwiGLU<float>(dstTensor, src0Tensor, src1Tensor, scalarValue, calCount);
}

__aicore__ void TestSwiGLU3() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> src0Tensor;
    AscendC::LocalTensor<float> src1Tensor;
    float scalarValue = 0.0f;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::SwiGLU<float>(dstTensor, src0Tensor, src1Tensor, scalarValue, sharedTmpBuffer, calCount);
}