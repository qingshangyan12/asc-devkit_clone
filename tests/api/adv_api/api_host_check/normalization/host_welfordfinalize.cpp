/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "normalization/welfordfinalize.h"

__aicore__ void TestWelfordFinalize0() {
    AscendC::LocalTensor<float> outputMean;
    AscendC::LocalTensor<float> outputVariance;
    AscendC::LocalTensor<float> inputMean;
    AscendC::LocalTensor<float> inputVariance;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::WelfordFinalizePara para;
    AscendC::WelfordFinalize(outputMean, outputVariance, inputMean, inputVariance, sharedTmpBuffer, para);
}

__aicore__ void TestWelfordFinalize1() {
    AscendC::LocalTensor<float> outputMean;
    AscendC::LocalTensor<float> outputVariance;
    AscendC::LocalTensor<float> inputMean;
    AscendC::LocalTensor<float> inputVariance;
    AscendC::LocalTensor<int32_t> counts;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::WelfordFinalizePara para;
    AscendC::WelfordFinalize(outputMean, outputVariance, inputMean, inputVariance, counts, sharedTmpBuffer, para);
}

__aicore__ void TestWelfordFinalize2() {
    AscendC::LocalTensor<float> outputMean;
    AscendC::LocalTensor<float> outputVariance;
    AscendC::LocalTensor<float> inputMean;
    AscendC::LocalTensor<float> inputVariance;
    AscendC::WelfordFinalizePara para;
    AscendC::WelfordFinalize(outputMean, outputVariance, inputMean, inputVariance, para);
}

__aicore__ void TestWelfordFinalize3() {
    AscendC::LocalTensor<float> outputMean;
    AscendC::LocalTensor<float> outputVariance;
    AscendC::LocalTensor<float> inputMean;
    AscendC::LocalTensor<float> inputVariance;
    AscendC::LocalTensor<int32_t> counts;
    AscendC::WelfordFinalizePara para;
    AscendC::WelfordFinalize(outputMean, outputVariance, inputMean, inputVariance, counts, para);
}

