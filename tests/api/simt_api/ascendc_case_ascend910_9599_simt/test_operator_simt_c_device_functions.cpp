/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <gtest/gtest.h>
#include <type_traits>
#include <random>
#include "simt_compiler_stub.h"
#include "kernel_operator.h"
#include "simt_api/device_functions.h"

using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

struct DeviceFuncParams {
    int32_t mode;
};

class DeviceTestsuite : public testing::Test, public testing::WithParamInterface<DeviceFuncParams> {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(DeviceTestsuite, MathApiTest)
{
    int result = __float_as_int(1);
    EXPECT_EQ(1065353216, result);
}