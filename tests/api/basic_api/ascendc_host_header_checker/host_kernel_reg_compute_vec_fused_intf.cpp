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
#include "reg_compute/kernel_reg_compute_vec_fused_intf.h"
#endif
#include "host_checker.h"

/**
 * host 侧编译校验
 * 测试被 #if defined(__NPU_ARCH__) 包裹且无 #else 的接口
 */

extern "C" void TestKernelRegComputeVecFusedInterfaces() {
    // MulsCast 接口
    using AscendC::MulsCast;

    // AbsSub 接口
    using AscendC::AbsSub;

    // ExpSub 接口
    using AscendC::ExpSub;

    // MulDstAdd 接口
    using AscendC::MulDstAdd;
}