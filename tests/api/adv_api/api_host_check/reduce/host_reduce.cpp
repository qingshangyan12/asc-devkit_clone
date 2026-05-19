/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "reduce/reduce.h"

__aicore__ void TestReduceMean0() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceMean<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceMean1() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceMean<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceMax2() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceMax<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceMax3() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceMax<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceMin4() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceMin<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceMin5() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceMin<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceSum6() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceSum<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceSum7() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceSum<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceProd8() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceProd<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceProd9() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceProd<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceAny10() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceAny<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceAny11() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceAny<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceAll12() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceAll<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceAll13() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    uint32_t srcShape[2] = {0, 0};
    bool srcInnerPad = false;
    AscendC::ReduceAll<float, AscendC::Pattern::Reduce::AR>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

