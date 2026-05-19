/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_basic_intf.h"
#endif
#include "host_checker.h"
/**
 * host 侧编译校验
 */

using namespace AscendC;
 // class MutexID
MutexID id = 1;
using MutexAlias = Mutex;

 // 全局方法声明检查
using AllocMutexIDFunc = MutexID(*)();
using ReleaseMutexIDFunc = void(*)(MutexID);

extern "C" __global__ __aicore__ void TestSetAippFuncionsOnDevice() {
    AscendC::GlobalTensor<uint8_t> src0, src1;
    AscendC::AippInputFormat format;
    AscendC::AippParams<uint8_t> config;
    AscendC::SetAippFunctions(src0, src1, format, config);
}

extern "C" void TestSetAippFuncionsOnHost() {
    AscendC::AippInputFormat format;
    AscendC::AippParams<uint8_t> config;
    using AscendC::SetAippFunctions;
}
