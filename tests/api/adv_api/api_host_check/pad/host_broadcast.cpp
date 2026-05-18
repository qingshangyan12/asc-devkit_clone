/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "pad/broadcast.h"

__aicore__ void TestBroadcast0() {
    AscendC::LocalTensor<float> dstLocal;
    AscendC::LocalTensor<float> srcLocal;
    uint32_t dstShape[2] = {0, 0};
    uint32_t srcShape[2] = {0, 0};
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::Broadcast<float, 2, 0>(dstLocal, srcLocal, dstShape, srcShape, sharedTmpBuffer);
}

__aicore__ void TestBroadcast1() {
    AscendC::LocalTensor<float> dstLocal;
    AscendC::LocalTensor<float> srcLocal;
    uint32_t dstShape[2] = {0, 0};
    uint32_t srcShape[2] = {0, 0};
    AscendC::Broadcast<float, 2, 0>(dstLocal, srcLocal, dstShape, srcShape);
}

