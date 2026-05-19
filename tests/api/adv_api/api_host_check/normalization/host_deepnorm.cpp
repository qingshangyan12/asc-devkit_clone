/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "normalization/deepnorm.h"

__aicore__ void TestDeepNorm0() {
    AscendC::LocalTensor<float> dstLocal;
    AscendC::LocalTensor<float> meanLocal;
    AscendC::LocalTensor<float> rstdLocal;
    AscendC::LocalTensor<float> srcLocal;
    AscendC::LocalTensor<float> gxLocal;
    AscendC::LocalTensor<float> betaLocal;
    AscendC::LocalTensor<float> gammaLocal;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    float alpha = 1.0f;
    float epsilon = 0.0f;
    DeepNormTiling tiling;
    AscendC::DeepNorm<float>(dstLocal, meanLocal, rstdLocal, srcLocal, gxLocal, betaLocal, gammaLocal, sharedTmpBuffer, alpha, epsilon, tiling);
}

__aicore__ void TestDeepNorm1() {
    AscendC::LocalTensor<float> dstLocal;
    AscendC::LocalTensor<float> meanLocal;
    AscendC::LocalTensor<float> rstdLocal;
    AscendC::LocalTensor<float> srcLocal;
    AscendC::LocalTensor<float> gxLocal;
    AscendC::LocalTensor<float> betaLocal;
    AscendC::LocalTensor<float> gammaLocal;
    float alpha = 1.0f;
    float epsilon = 0.0f;
    DeepNormTiling tiling;
    AscendC::DeepNorm<float>(dstLocal, meanLocal, rstdLocal, srcLocal, gxLocal, betaLocal, gammaLocal, alpha, epsilon, tiling);
}

