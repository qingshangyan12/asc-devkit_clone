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
#include "kernel_operator_vec_binary_scalar_intf.h"
#endif
#include "host_checker.h"

/**
 * host 侧编译校验
 * 测试被 #if defined(__NPU_ARCH__) 包裹且无 #else 的接口
 * 该文件所有接口都被条件编译包裹
 */

extern "C" void TestKernelOperatorVecBinaryScalarInterfaces() {
    // Adds 接口 - 10个重载
    using AscendC::Adds;

    // Muls 接口 - 10个重载
    using AscendC::Muls;

    // Maxs 接口 - 10个重载
    using AscendC::Maxs;

    // Mins 接口 - 10个重载
    using AscendC::Mins;

    // ShiftLeft 接口 - 6个重载
    using AscendC::ShiftLeft;

    // ShiftRight 接口 - 6个重载
    using AscendC::ShiftRight;

    // LeakyRelu 接口 - 6个重载
    using AscendC::LeakyRelu;

    // Subs 接口 - 4个重载
    using AscendC::Subs;

    // Divs 接口 - 4个重载
    using AscendC::Divs;

    // Ands 接口 - 4个重载
    using AscendC::Ands;

    // Ors 接口 - 4个重载
    using AscendC::Ors;

    // MulsCast 接口
    using AscendC::MulsCast;

    // FusedMulsCast 接口
    using AscendC::FusedMulsCast;
}