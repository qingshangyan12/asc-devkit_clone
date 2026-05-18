/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "math/hypot.h"

void TestHypot0() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    AscendC::Hypot<half>(dstTensor, src0Tensor, src1Tensor, sharedTmpBuffer);
}

void TestHypot1() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    uint32_t calCount = 0;
    AscendC::Hypot<half>(dstTensor, src0Tensor, src1Tensor, sharedTmpBuffer, calCount);
}

void TestHypot2() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    AscendC::Hypot<half>(dstTensor, src0Tensor, src1Tensor);
}

void TestHypot3() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> src0Tensor;
    AscendC::LocalTensor<half> src1Tensor;
    uint32_t calCount = 0;
    AscendC::Hypot<half>(dstTensor, src0Tensor, src1Tensor, calCount);
}

