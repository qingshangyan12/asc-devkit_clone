/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "select/selectwithbytesmask.h"

__aicore__ void TestSelect0() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> src0Tensor;
    float src1Value = 0.0f;
    AscendC::LocalTensor<uint8_t> maskTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::SelectWithBytesMaskShapeInfo info;
    AscendC::Select<float, uint8_t>(dstTensor, src0Tensor, src1Value, maskTensor, sharedTmpBuffer, info);
}

__aicore__ void TestSelect1() {
    AscendC::LocalTensor<float> dstTensor;
    float src0Value = 0.0f;
    AscendC::LocalTensor<float> src1Tensor;
    AscendC::LocalTensor<uint8_t> maskTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::SelectWithBytesMaskShapeInfo info;
    AscendC::Select<float, uint8_t>(dstTensor, src0Value, src1Tensor, maskTensor, sharedTmpBuffer, info);
}

