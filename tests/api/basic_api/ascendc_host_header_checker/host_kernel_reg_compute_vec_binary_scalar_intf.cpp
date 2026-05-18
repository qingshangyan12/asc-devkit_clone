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
#include "reg_compute/kernel_reg_compute_vec_binary_scalar_intf.h"
#endif
#include "host_checker.h"

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3103) || (__NPU_ARCH__ == 2103)

extern "C" void TestKernelRegComputeVecBinaryScalarInterfaces() {
    // Adds 接口
    using AscendC::Reg::Adds;

    // Muls 接口
    using AscendC::Reg::Muls;

    // Maxs 接口
    using AscendC::Reg::Maxs;

    // Mins 接口
    using AscendC::Reg::Mins;

    // ShiftLefts 接口
    using AscendC::Reg::ShiftLefts;

    // ShiftRights 接口
    using AscendC::Reg::ShiftRights;

    // LeakyRelu 接口
    using AscendC::Reg::LeakyRelu;
}

#endif