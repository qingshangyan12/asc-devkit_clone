/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "normalization/batchnorm.h"

__aicore__ void TestBatchNorm0() {
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::BatchNorm<half>(output, outputMean, outputVariance, inputX, gamma, beta, sharedTmpBuffer, epsilon, tiling);
}

__aicore__ void TestBatchNorm1() {
    AscendC::BatchNorm<half>(output, outputMean, outputVariance, inputX, gamma, beta, epsilon, tiling);
}

