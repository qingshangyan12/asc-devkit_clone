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
#include "kernel_common.h"
#endif
#include "host_checker.h"

/**
 * host 侧编译校验
 * 测试被 #if __NPU_ARCH__ 包裹且无 #else 的接口
 */

extern "C" void TestKernelCommonInterfaces() {
    // asc_vf_call: 被 #if (defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510) 包裹，无 #else
    // 这是一个模板函数，需要在特定架构下使用

    // SetVectorMask: 被 #if __NPU_ARCH__ == 3002 || __NPU_ARCH__ == 3102 包裹，无 #else
    using AscendC::SetVectorMask;

    // ResetMask: 被 #if __NPU_ARCH__ == 3002 || __NPU_ARCH__ == 3102 包裹，无 #else
    using AscendC::ResetMask;

    // Mutex::Lock: 被 #if __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 包裹，无 #else
    using AscendC::Mutex;

    // Mutex::Unlock: 被 #if __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 包裹，无 #else
    // Mutex类已包含Lock和Unlock

    // ReleaseMutexID: 被 #if __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 包裹，无 #else
    using AscendC::ReleaseMutexID;

    // SetAippFunctions: 被 #if __NPU_ARCH__ 包裹，无 #else
    using AscendC::SetAippFunctions;
}