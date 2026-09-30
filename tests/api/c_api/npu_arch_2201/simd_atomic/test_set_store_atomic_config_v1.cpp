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
#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"

void st_atomic(int8_t, __gm__ int8_t*) {}
void st_atomic(int16_t, __gm__ int16_t*) {}
void st_atomic(int32_t, __gm__ int32_t*) {}

#define TEST_SET_STORE_ATOMIC_CONFIG(c_api_type, instr_type, atomic_type)                                           \
    namespace {                                                                                                     \
    void set_st_atomic_cfg_##c_api_type##_stub(uint64_t config) { EXPECT_EQ(config, atomic_type); }                 \
    void st_atomic_##c_api_type##_stub(instr_type value, __gm__ instr_type* addr) {}                                \
    }                                                                                                               \
                                                                                                                    \
    TEST(TestSetStoreAtomicConfigV1CApi, c_api_type##_Succ)                                                         \
    {                                                                                                               \
        c_api_type value{};                                                                                         \
        __gm__ c_api_type* addr = nullptr;                                                                          \
        MOCKER_CPP(set_st_atomic_cfg, void(uint64_t)).times(1).will(invoke(set_st_atomic_cfg_##c_api_type##_stub)); \
        MOCKER_CPP(st_atomic, void(instr_type, __gm__ instr_type*))                                                 \
            .times(1)                                                                                               \
            .will(invoke(st_atomic_##c_api_type##_stub));                                                           \
        asc_set_store_atomic_config_v1(atomic_type, 0, value, addr);                                                \
        GlobalMockObject::verify();                                                                                 \
    }

TEST_SET_STORE_ATOMIC_CONFIG(int8_t, int8_t, 5);
TEST_SET_STORE_ATOMIC_CONFIG(int16_t, int16_t, 3);
TEST_SET_STORE_ATOMIC_CONFIG(int32_t, int32_t, 4);
TEST_SET_STORE_ATOMIC_CONFIG(half, int16_t, 2);
TEST_SET_STORE_ATOMIC_CONFIG(float, int32_t, 1);
TEST_SET_STORE_ATOMIC_CONFIG(bfloat16_t, int16_t, 6);
