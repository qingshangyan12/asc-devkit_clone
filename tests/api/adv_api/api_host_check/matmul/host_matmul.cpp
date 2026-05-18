/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "matmul/matmul.h"

void TestMatmulImplInit0() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.Init(cubeTiling);
}

void TestMatmulImplSetTensorA1() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.SetTensorA(gm, isTransposeA);
}

void TestMatmulImplSetTensorB2() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.SetTensorB(gm, isTransposeB);
}

void TestMatmulImplSetBias3() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.SetBias(biasGlobal);
}

void TestMatmulImplIterate4() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.Iterate();
}

void TestMatmulImplIterateAll5() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.IterateAll(gm, enAtomic);
}

void TestMatmulImplGetTensorC6() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.GetTensorC(gm, enAtomic);
}

void TestMatmulImplEnd7() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.End();
}

