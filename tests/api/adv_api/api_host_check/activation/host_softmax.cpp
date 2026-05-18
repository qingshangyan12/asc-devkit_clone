/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "activation/softmax.h"

__aicore__ void TestSoftMax0() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> sumTensor;
    AscendC::LocalTensor<float> maxTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::tiling::SoftMaxTiling tiling;
    AscendC::SoftMax<float>(dstTensor, sumTensor, maxTensor, srcTensor, tiling);
}

__aicore__ void TestSoftMax1() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::tiling::SoftMaxTiling tiling;
    AscendC::SoftMax<float>(dstTensor, srcTensor, tiling);
}

__aicore__ void TestSoftMax2() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::tiling::SoftMaxTiling tiling;
    AscendC::SoftMax<float>(dstTensor, srcTensor, sharedTmpBuffer, tiling);
}

__aicore__ void TestSoftMax3() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<float> sumTensor;
    AscendC::LocalTensor<float> maxTensor;
    AscendC::LocalTensor<float> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::tiling::SoftMaxTiling tiling;
    AscendC::SoftMax<float>(dstTensor, sumTensor, maxTensor, srcTensor, sharedTmpBuffer, tiling);
}

__aicore__ void TestAdjustSoftMaxRes4() {
    AscendC::LocalTensor<float> softMaxRes;
    AscendC::LocalTensor<float> maxTensor;
    uint32_t from = 0;
    float to = 0.0f;
    AscendC::SoftMaxShapeInfo softmaxShapeInfo;
    AscendC::AdjustSoftMaxRes<float>(softMaxRes, maxTensor, from, to, softmaxShapeInfo);
}