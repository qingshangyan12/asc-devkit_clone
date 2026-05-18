/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "math/erf.h"

__aicore__ void TestErf0() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::Erf<float>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
}

__aicore__ void TestErf1() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::Erf<float>(dstTensor, srcTensor, sharedTmpBuffer);
}

__aicore__ void TestErf2() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    uint32_t calCount = 0;
    AscendC::Erf<float>(dstTensor, srcTensor, calCount);
}

__aicore__ void TestErf3() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::Erf<float>(dstTensor, srcTensor);
}

