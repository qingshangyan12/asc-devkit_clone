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

__aicore__ void TestMatmulImplInit0() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.Init(cubeTiling);
}

__aicore__ void TestMatmulImplSetTensorA1() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.SetTensorA(gm, isTransposeA);
}

__aicore__ void TestMatmulImplSetTensorB2() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.SetTensorB(gm, isTransposeB);
}

__aicore__ void TestMatmulImplSetBias3() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.SetBias(biasGlobal);
}

__aicore__ void TestMatmulImplIterate4() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.Iterate();
}

__aicore__ void TestMatmulImplIterateAll5() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.IterateAll(gm, enAtomic);
}

__aicore__ void TestMatmulImplGetTensorC6() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.GetTensorC(gm, enAtomic);
}

__aicore__ void TestMatmulImplEnd7() {
    AscendC::MatmulImpl<MatmulImpl::MatmulConfig> obj;
    obj.End();
}

