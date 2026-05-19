/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "quantization/dequantize.h"

__aicore__ void TestDequantize0() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<int32_t> srcTensor;
    float scale = 1.0f;
    float offset = 0.0f;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::DequantizeParams params;
    static constexpr AscendC::DequantizeConfig config = {AscendC::DequantizePolicy::PER_TENSOR, false, 1};
    AscendC::Dequantize<config, float, int32_t, float, float>(dstTensor, srcTensor, scale, offset, sharedTmpBuffer, params);
}

__aicore__ void TestDequantize1() {
    AscendC::LocalTensor<float> dstTensor;
    AscendC::LocalTensor<int32_t> srcTensor;
    float scale = 1.0f;
    float offset = 0.0f;
    AscendC::DequantizeParams params;
    static constexpr AscendC::DequantizeConfig config = {AscendC::DequantizePolicy::PER_TENSOR, false, 1};
    AscendC::Dequantize<config, float, int32_t, float, float>(dstTensor, srcTensor, scale, offset, params);
}

