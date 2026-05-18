/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "math/exp.h"

__aicore__ void TestExp0() {
    AscendC::LocalTensor<half> dstLocal;
    AscendC::LocalTensor<half> srcLocal;
    uint32_t calCount = 0;
    AscendC::Exp<half, 0>(dstLocal, srcLocal, calCount);
}

__aicore__ void TestExp1() {
    AscendC::LocalTensor<half> dstLocal;
    AscendC::LocalTensor<half> srcLocal;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::Exp<half, 0>(dstLocal, srcLocal, sharedTmpBuffer, calCount);
}