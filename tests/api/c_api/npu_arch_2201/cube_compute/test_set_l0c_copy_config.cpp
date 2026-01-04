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
#include <mockcpp/mockcpp.hpp>
#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"

class TestCubeComputeSetL0cCopyConfig : public testing::Test { 
protected:
    void SetUp() {
        g_c_api_core_type = C_API_AIC_TYPE;
    }
    void TearDown() {
        g_c_api_core_type = C_API_AIV_TYPE;
    }
};

namespace {
void set_l0c_copy_config_Stub(uint64_t value)
{
    EXPECT_EQ(value, static_cast<uint64_t>(0));
}
}

TEST_F(TestCubeComputeSetL0cCopyConfig, set_l0c_copy_config_Succ)
{
    uint64_t value = 0;
    MOCKER(set_fpc, void(uint64_t))
        .times(1)
        .will(invoke(set_l0c_copy_config_Stub));

    asc_set_l0c_copy_config(0, 0, false);
    GlobalMockObject::verify();
}
