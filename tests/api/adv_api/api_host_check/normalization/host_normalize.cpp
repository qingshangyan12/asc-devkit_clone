/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "normalization/normalize.h"

__aicore__ void TestNormalize0() {
    AscendC::LocalTensor<float> output;
    AscendC::LocalTensor<float> outputRstd;
    AscendC::LocalTensor<float> inputMean;
    AscendC::LocalTensor<float> inputVariance;
    AscendC::LocalTensor<float> inputX;
    AscendC::LocalTensor<float> gamma;
    AscendC::LocalTensor<float> beta;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    float epsilon = 0.0f;
    AscendC::NormalizePara para;
    AscendC::Normalize<float, float>(output, outputRstd, inputMean, inputVariance, inputX, gamma, beta, sharedTmpBuffer, epsilon, para);
}

__aicore__ void TestNormalize1() {
    AscendC::LocalTensor<float> output;
    AscendC::LocalTensor<float> outputRstd;
    AscendC::LocalTensor<float> inputMean;
    AscendC::LocalTensor<float> inputVariance;
    AscendC::LocalTensor<float> inputX;
    AscendC::LocalTensor<float> gamma;
    AscendC::LocalTensor<float> beta;
    float epsilon = 0.0f;
    AscendC::NormalizePara para;
    AscendC::Normalize<float, float>(output, outputRstd, inputMean, inputVariance, inputX, gamma, beta, epsilon, para);
}

