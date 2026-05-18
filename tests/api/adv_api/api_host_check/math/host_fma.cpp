/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "math/fma.h"

__aicore__ void TestFma0() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::LocalTensor<float> src0Tensor;
    AscendC::LocalTensor<float> src1Tensor;
    AscendC::LocalTensor<float> src2Tensor;
    uint32_t calCount = 0;
    AscendC::Fma<float>(dstTensor, src0Tensor, src1Tensor, src2Tensor, sharedTmpBuffer, calCount);
}

__aicore__ void TestFma1() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<float> src0Tensor;
    AscendC::LocalTensor<float> src1Tensor;
    AscendC::LocalTensor<float> src2Tensor;
    uint32_t calCount = 0;
    AscendC::Fma<float>(dstTensor, src0Tensor, src1Tensor, src2Tensor, calCount);
}

