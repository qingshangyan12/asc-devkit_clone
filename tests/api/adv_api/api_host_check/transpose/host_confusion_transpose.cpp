/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "transpose/confusion_transpose.h"

__aicore__ void TestTranspose0() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::TransposeType transposeType = AscendC::TransposeType::TRANSPOSE_TYPE_NONE;
    ConfusionTransposeTiling tiling;
    AscendC::Transpose<float>(dst, src, sharedTmpBuffer, transposeType, tiling);
}

__aicore__ void TestTranspose1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::TransposeType transposeType = AscendC::TransposeType::TRANSPOSE_TYPE_NONE;
    ConfusionTransposeTiling tiling;
    AscendC::Transpose<float>(dst, src, transposeType, tiling);
}

