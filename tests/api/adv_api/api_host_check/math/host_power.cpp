/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "math/power.h"

__aicore__ void TestPower0() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> expTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::Power<half>(dstTensor, srcTensor, expTensor, sharedTmpBuffer, calCount);
}

__aicore__ void TestPower1() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> expTensor;
    uint32_t calCount = 0;
    AscendC::Power<half>(dstTensor, srcTensor, expTensor, calCount);
}

__aicore__ void TestPower2() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> expTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::Power<half>(dstTensor, srcTensor, expTensor, sharedTmpBuffer);
}

__aicore__ void TestPower3() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<half> expTensor;
    AscendC::Power<half>(dstTensor, srcTensor, expTensor);
}

__aicore__ void TestPower4() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    half expValue;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::Power<half>(dstTensor, srcTensor, expValue, sharedTmpBuffer, calCount);
}

__aicore__ void TestPower5() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    half expValue;
    uint32_t calCount = 0;
    AscendC::Power<half>(dstTensor, srcTensor, expValue, calCount);
}

__aicore__ void TestPower6() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    half expValue;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::Power<half>(dstTensor, srcTensor, expValue, sharedTmpBuffer);
}

__aicore__ void TestPower7() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    half expValue;
    AscendC::Power<half>(dstTensor, srcTensor, expValue);
}

__aicore__ void TestPower8() {
    AscendC::LocalTensor<half> dstTensor;
    half srcValue;
    AscendC::LocalTensor<half> expTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    uint32_t calCount = 0;
    AscendC::Power<half>(dstTensor, srcValue, expTensor, sharedTmpBuffer, calCount);
}

__aicore__ void TestPower9() {
    AscendC::LocalTensor<half> dstTensor;
    half srcValue;
    AscendC::LocalTensor<half> expTensor;
    uint32_t calCount = 0;
    AscendC::Power<half>(dstTensor, srcValue, expTensor, calCount);
}

__aicore__ void TestPower10() {
    AscendC::LocalTensor<half> dstTensor;
    half srcValue;
    AscendC::LocalTensor<half> expTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::Power<half>(dstTensor, srcValue, expTensor, sharedTmpBuffer);
}

__aicore__ void TestPower11() {
    AscendC::LocalTensor<half> dstTensor;
    half srcValue;
    AscendC::LocalTensor<half> expTensor;
    AscendC::Power<half>(dstTensor, srcValue, expTensor);
}