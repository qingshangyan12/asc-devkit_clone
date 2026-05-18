/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "math/logical_ors.h"

__aicore__ void TestLogicalOrs0() {
    AscendC::LocalTensor<bool> dstTensor;
    AscendC::LocalTensor<float> src0Tensor;
    float src1Value;
    uint32_t calCount = 0;
    AscendC::LogicalOrs<AscendC::DEFAULT_LOGICAL_ORS_CONFIG, bool>(dstTensor, src0Tensor, src1Value, calCount);
}
