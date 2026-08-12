/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

#include "c_api/stub/cce_stub.h"
#include "tensor_api/experimental/reg/basic_arithmetic.h"

namespace {

__simd_callee__ inline asc::te::reg_tensor<float> CompileBasicArithmeticHeader(
    asc::te::reg_tensor<float> src0, asc::te::reg_tensor<float> src1)
{
    return asc::te::max(src0 + src1, 0.0f);
}

} // namespace
