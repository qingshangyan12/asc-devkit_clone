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
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::ReduceMean<half>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceMean1() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::ReduceMean<half>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceMax2() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::ReduceMax<half>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceMax3() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::ReduceMax<half>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceMin4() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::ReduceMin<half>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceMin5() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::ReduceMin<half>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceSum6() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::ReduceSum<half>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceSum7() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::ReduceSum<half>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceProd8() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::ReduceProd<half>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceProd9() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::ReduceProd<half>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceAny10() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::ReduceAny<half>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceAny11() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::ReduceAny<half>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

__aicore__ void TestReduceAll12() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::ReduceAll<half>(dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

__aicore__ void TestReduceAll13() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::ReduceAll<half>(dstTensor, srcTensor, srcShape, srcInnerPad);
}

