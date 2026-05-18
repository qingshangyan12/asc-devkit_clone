/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "activation/geglu.h"

__aicore__ void TestGeGLU0() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::GeGLU<half>(dstTensor, srcTensor0, srcTensor1, sharedTmpBuffer, calCount);
}

__aicore__ void TestGeGLU1() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    uint32_t calCount = 0;
    AscendC::GeGLU<half>(dstTensor, srcTensor0, srcTensor1, calCount);
}

__aicore__ void TestGeGLU2() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::GeGLU<half>(dstTensor, srcTensor0, srcTensor1, sharedTmpBuffer);
}

__aicore__ void TestGeGLU3() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    AscendC::GeGLU<half>(dstTensor, srcTensor0, srcTensor1);
}

