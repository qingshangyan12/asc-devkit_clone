/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "activation/gelu.h"

__aicore__ void TestGelu0() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::Gelu<half>(dstLocal, srcLocal, sharedTmpBuffer, dataSize);
}

__aicore__ void TestGelu1() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    uint32_t calCount = 0;
    AscendC::Gelu<half>(dstLocal, srcLocal, dataSize);
}

__aicore__ void TestFasterGelu2() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::FasterGelu<half>(dstLocal, srcLocal, sharedTmpBuffer, dataSize);
}

__aicore__ void TestFasterGelu3() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    uint32_t calCount = 0;
    AscendC::FasterGelu<half>(dstLocal, srcLocal, dataSize);
}

__aicore__ void TestFasterGeluV24() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::FasterGeluV2<half>(dstLocal, srcLocal, sharedTmpBuffer, dataSize);
}

__aicore__ void TestFasterGeluV25() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    uint32_t calCount = 0;
    AscendC::FasterGeluV2<half>(dstLocal, srcLocal, dataSize);
}

