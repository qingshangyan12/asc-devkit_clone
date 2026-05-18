/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "math/philox.h"

__aicore__ void TestPhiloxRandom0() {
    AscendC::LocalTensor<float> dstLocal;
    AscendC::PhiloxKey philoxKey;
    AscendC::PhiloxCounter philoxCounter;
    uint16_t count = 0;
    AscendC::PhiloxRandom<7>(dstLocal, philoxKey, philoxCounter, count);
}

__aicore__ void TestPhiloxRandom1() {
    AscendC::LocalTensor<float> dstLocal;
    AscendC::PhiloxKey philoxKey;
    AscendC::PhiloxCounter philoxCounter;
    AscendC::PhiloxRandomParams params;
    AscendC::PhiloxRandom<7>(dstLocal, philoxKey, philoxCounter, params);
}
