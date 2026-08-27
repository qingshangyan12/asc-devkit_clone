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

class TestSimdAtomicGetStoreAtomicConfigCApi : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(TestSimdAtomicGetStoreAtomicConfigCApi, GetStoreAtomicConfigByReference)
{
    constexpr uint16_t atomicType = 4;
    constexpr uint16_t atomicOp = 0;
    constexpr int64_t atomicConfig = atomicType | (atomicOp << 3);
    MOCKER(get_st_atomic_cfg, int64_t(void)).times(1).will(returnValue(atomicConfig));

    asc_store_atomic_config config;
    asc_get_store_atomic_config(config);

    EXPECT_EQ(config.config, atomicConfig);
    EXPECT_EQ(config.atomic_type, atomicType);
    EXPECT_EQ(config.atomic_op, atomicOp);
    GlobalMockObject::verify();
}

TEST_F(TestSimdAtomicGetStoreAtomicConfigCApi, GetStoreAtomicConfigByReturnValue)
{
    constexpr uint16_t atomicType = 4;
    constexpr uint16_t atomicOp = 0;
    constexpr int64_t atomicConfig = atomicType | (atomicOp << 3);
    MOCKER(get_st_atomic_cfg, int64_t(void)).times(1).will(returnValue(atomicConfig));

    asc_store_atomic_config config = asc_get_store_atomic_config();

    EXPECT_EQ(config.config, atomicConfig);
    EXPECT_EQ(config.atomic_type, atomicType);
    EXPECT_EQ(config.atomic_op, atomicOp);
    GlobalMockObject::verify();
}
