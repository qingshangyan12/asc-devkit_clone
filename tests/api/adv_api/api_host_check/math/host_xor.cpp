/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "math/xor.h"

__aicore__ void TestXor0() {
    AscendC::LocalTensor<int16_t> dstTensor;
    AscendC::LocalTensor<int16_t> src0Tensor;
    AscendC::LocalTensor<int16_t> src1Tensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::Xor<int16_t>(dstTensor, src0Tensor, src1Tensor, sharedTmpBuffer, calCount);
}

__aicore__ void TestXor1() {
    AscendC::LocalTensor<int16_t> dstTensor;
    AscendC::LocalTensor<int16_t> src0Tensor;
    AscendC::LocalTensor<int16_t> src1Tensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::Xor<int16_t>(dstTensor, src0Tensor, src1Tensor, sharedTmpBuffer);
}

__aicore__ void TestXor2() {
    AscendC::LocalTensor<int16_t> dstTensor;
    AscendC::LocalTensor<int16_t> src0Tensor;
    AscendC::LocalTensor<int16_t> src1Tensor;
    uint32_t calCount = 0;
    AscendC::Xor<int16_t>(dstTensor, src0Tensor, src1Tensor, calCount);
}

__aicore__ void TestXor3() {
    AscendC::LocalTensor<int16_t> dstTensor;
    AscendC::LocalTensor<int16_t> src0Tensor;
    AscendC::LocalTensor<int16_t> src1Tensor;
    AscendC::Xor<int16_t>(dstTensor, src0Tensor, src1Tensor);
}