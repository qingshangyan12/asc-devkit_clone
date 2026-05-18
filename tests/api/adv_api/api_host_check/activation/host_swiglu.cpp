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
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    AscendC::SwiGLU<half>(dstTensor, srcTensor0, srcTensor1, scalarValue);
}

__aicore__ void TestSwiGLU1() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::SwiGLU<half>(dstTensor, srcTensor0, srcTensor1, scalarValue, sharedTmpBuffer);
}

__aicore__ void TestSwiGLU2() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    uint32_t calCount = 0;
    AscendC::SwiGLU<half>(dstTensor, srcTensor0, srcTensor1, scalarValue, calCount);
}

__aicore__ void TestSwiGLU3() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::SwiGLU<half>(dstTensor, srcTensor0, srcTensor1, scalarValue, sharedTmpBuffer, calCount);
}

