/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "math/frac.h"

__aicore__ void TestFrac0() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::Frac<float>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
}

__aicore__ void TestFrac1() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::Frac<float>(dstTensor, srcTensor, sharedTmpBuffer);
}

__aicore__ void TestFrac2() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::Frac<float>(dstTensor, srcTensor);
}

__aicore__ void TestFrac3() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    uint32_t calCount = 0;
    AscendC::Frac<float>(dstTensor, srcTensor, calCount);
}

