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
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"

#define TEST_ASC_LOAD_DEV(data_type, return_type, value)                                             \
    namespace {                                                                                      \
    return_type ld_dev_##data_type##_stub(__gm__ data_type* addr, int16_t offset)                   \
    {                                                                                                \
        EXPECT_EQ(offset, 0);                                                                        \
        return static_cast<return_type>(value);                                                      \
    }                                                                                                \
    }                                                                                                \
                                                                                                     \
    TEST(AscLoadDev, data_type)                                                                      \
    {                                                                                                \
        __gm__ data_type* addr = nullptr;                                                            \
        MOCKER_CPP(ld_dev, return_type(__gm__ data_type*, int16_t))                                 \
            .times(1)                                                                                \
            .will(invoke(ld_dev_##data_type##_stub));                                                \
        EXPECT_EQ(asc_load_dev(addr), static_cast<return_type>(value));                              \
        GlobalMockObject::verify();                                                                  \
    }

TEST_ASC_LOAD_DEV(int8_t, int64_t, -1)
TEST_ASC_LOAD_DEV(uint8_t, uint64_t, 1)
TEST_ASC_LOAD_DEV(int16_t, int64_t, -2)
TEST_ASC_LOAD_DEV(uint16_t, uint64_t, 2)
TEST_ASC_LOAD_DEV(int32_t, int64_t, -3)
TEST_ASC_LOAD_DEV(uint32_t, uint64_t, 3)
TEST_ASC_LOAD_DEV(int64_t, int64_t, -4)
TEST_ASC_LOAD_DEV(uint64_t, uint64_t, 4)

#undef TEST_ASC_LOAD_DEV
