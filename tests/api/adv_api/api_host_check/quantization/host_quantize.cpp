/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "quantization/quantize.h"

__aicore__ void TestQuantize0() {
    AscendC::LocalTensor<int8_t> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    half scale = half(1.0f);
    half offset = half(0.0f);
    AscendC::QuantizeParams params;
    static constexpr AscendC::QuantizeConfig config = {AscendC::QuantizePolicy::PER_TENSOR, false, AscendC::RoundMode::CAST_RINT, 1};
    AscendC::Quantize<config>(dstTensor, srcTensor, scale, offset, params);
}

__aicore__ void TestQuantize1() {
    AscendC::LocalTensor<int8_t> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    half scale = half(1.0f);
    half offset = half(0.0f);
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::QuantizeParams params;
    static constexpr AscendC::QuantizeConfig config = {AscendC::QuantizePolicy::PER_TENSOR, false, AscendC::RoundMode::CAST_RINT, 1};
    AscendC::Quantize<config>(dstTensor, srcTensor, sharedTmpBuffer, scale, offset, params);
}

