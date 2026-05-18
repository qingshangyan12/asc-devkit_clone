/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "sort/topk.h"

__aicore__ void TestTopK0() {
    AscendC::LocalTensor<float> dstValueLocal;
    AscendC::LocalTensor<int32_t> dstIndexLocal;
    AscendC::LocalTensor<float> srcLocal;
    AscendC::LocalTensor<int32_t> srcIndexLocal;
    AscendC::LocalTensor<bool> finishLocal;
    AscendC::LocalTensor<uint8_t> tmpLocal;
    int32_t k = 0;
    TopkTiling tiling;
    AscendC::TopKInfo topKInfo;
    AscendC::TopK<float>(dstValueLocal, dstIndexLocal, srcLocal, srcIndexLocal, finishLocal, tmpLocal, k, tiling, topKInfo);
}

__aicore__ void TestTopK1() {
    AscendC::LocalTensor<float> dstValueLocal;
    AscendC::LocalTensor<int32_t> dstIndexLocal;
    AscendC::LocalTensor<float> srcLocal;
    AscendC::LocalTensor<int32_t> srcIndexLocal;
    AscendC::LocalTensor<bool> finishLocal;
    int32_t k = 0;
    TopkTiling tiling;
    AscendC::TopKInfo topKInfo;
    AscendC::TopK<float>(dstValueLocal, dstIndexLocal, srcLocal, srcIndexLocal, finishLocal, k, tiling, topKInfo);
}

