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
#include "reg_compute/kernel_reg_compute_vec_binary_intf.h"
#endif
#include "host_checker.h"

/**
 * host 侧编译校验
 * 测试被 #if defined(__NPU__ARCH__) 包裹且无 #else 的接口
 * 注意：该文件使用了 __NPU__ARCH__ (双下划线) 而非 __NPU_ARCH__
 */

extern "C" void TestKernelRegComputeVecBinaryInterfaces() {
    // Add 接口
    using AscendC::Reg::Add;

    // Sub 接口
    using AscendC::Reg::Sub;

    // Mul 接口
    using AscendC::Reg::Mul;

    // Div 接口
    using AscendC::Reg::Div;

    // Max 接口
    using AscendC::Reg::Max;

    // Min 接口
    using AscendC::Reg::Min;

    // ShiftLeft 接口
    using AscendC::Reg::ShiftLeft;

    // ShiftRight 接口
    using AscendC::Reg::ShiftRight;

    // And 接口
    using AscendC::Reg::And;

    // Or 接口
    using AscendC::Reg::Or;

    // Xor 接口
    using AscendC::Reg::Xor;

    // Prelu 接口
    using AscendC::Reg::Prelu;

    // Mull 接口
    using AscendC::Reg::Mull;

    // MulAddDst 接口
    using AscendC::Reg::MulAddDst;

    // AddC 接口
    using AscendC::Reg::AddC;

    // SubC 接口
    using AscendC::Reg::SubC;
}