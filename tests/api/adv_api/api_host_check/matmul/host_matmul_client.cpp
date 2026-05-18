/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "matmul/matmul_client.h"

__aicore__ void TestMatmulClientInit0() {
    AscendC::MatmulClient<MatmulClient::MatmulConfig> obj;
    obj.Init(cubeTiling);
}

__aicore__ void TestMatmulClientSetTensorA1() {
    AscendC::MatmulClient<MatmulClient::MatmulConfig> obj;
    obj.SetTensorA(gm, isTransposeA);
}

__aicore__ void TestMatmulClientSetTensorB2() {
    AscendC::MatmulClient<MatmulClient::MatmulConfig> obj;
    obj.SetTensorB(gm, isTransposeB);
}

__aicore__ void TestMatmulClientSetBias3() {
    AscendC::MatmulClient<MatmulClient::MatmulConfig> obj;
    obj.SetBias(biasGlobal);
}

__aicore__ void TestMatmulClientIterate4() {
    AscendC::MatmulClient<MatmulClient::MatmulConfig> obj;
    obj.Iterate();
}

__aicore__ void TestMatmulClientIterateAll5() {
    AscendC::MatmulClient<MatmulClient::MatmulConfig> obj;
    obj.IterateAll(gm, enAtomic);
}

__aicore__ void TestMatmulClientGetTensorC6() {
    AscendC::MatmulClient<MatmulClient::MatmulConfig> obj;
    obj.GetTensorC(gm, enAtomic);
}

__aicore__ void TestMatmulClientEnd7() {
    AscendC::MatmulClient<MatmulClient::MatmulConfig> obj;
    obj.End();
}

