/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "quantization/antiquantize.h"

__aicore__ void TestAntiQuantize0() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<int8_t> srcTensor;
    half scale = half(1.0f);
    half offset = half(0.0f);
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::AntiQuantizeParams params;
    static constexpr AscendC::AntiQuantizeConfig config = {AscendC::AntiQuantizePolicy::PER_TENSOR, true, 1};
    AscendC::AntiQuantize<config>(dstTensor, srcTensor, scale, offset, sharedTmpBuffer, params);
}

__aicore__ void TestAntiQuantize1() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<int8_t> srcTensor;
    half scale = half(1.0f);
    half offset = half(0.0f);
    AscendC::AntiQuantizeParams params;
    static constexpr AscendC::AntiQuantizeConfig config = {AscendC::AntiQuantizePolicy::PER_TENSOR, true, 1};
    AscendC::AntiQuantize<config>(dstTensor, srcTensor, scale, offset, params);
}

