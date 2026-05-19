/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "normalization/layernormgrad.h"

__aicore__ void TestLayerNormGrad0() {
    AscendC::LocalTensor<float> outputPdX;
    AscendC::LocalTensor<float> resForGamma;
    AscendC::LocalTensor<float> inputDy;
    AscendC::LocalTensor<float> inputX;
    AscendC::LocalTensor<float> inputVariance;
    AscendC::LocalTensor<float> inputMean;
    AscendC::LocalTensor<float> inputGamma;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    float epsilon = 0.0f;
    LayerNormGradTiling tiling;
    AscendC::LayerNormGrad<float>(outputPdX, resForGamma, inputDy, inputX, inputVariance, inputMean, inputGamma, sharedTmpBuffer, epsilon, tiling);
}

__aicore__ void TestLayerNormGrad1() {
    AscendC::LocalTensor<float> outputPdX;
    AscendC::LocalTensor<float> resForGamma;
    AscendC::LocalTensor<float> inputDy;
    AscendC::LocalTensor<float> inputX;
    AscendC::LocalTensor<float> inputVariance;
    AscendC::LocalTensor<float> inputMean;
    AscendC::LocalTensor<float> inputGamma;
    float epsilon = 0.0f;
    LayerNormGradTiling tiling;
    AscendC::LayerNormGrad<float>(outputPdX, resForGamma, inputDy, inputX, inputVariance, inputMean, inputGamma, epsilon, tiling);
}

