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

#define TEST_STORE_ATOMIC(c_api_type, instr_type)                                    \
    namespace {                                                                      \
    void st_atomic_##c_api_type##_stub(instr_type value, __gm__ instr_type* addr) {} \
    }                                                                                \
                                                                                     \
    TEST(TestStoreAtomicCApi, c_api_type##_Succ)                                     \
    {                                                                                \
        __gm__ c_api_type* addr = nullptr;                                           \
        c_api_type value{};                                                          \
        MOCKER_CPP(st_atomic, void(instr_type, __gm__ instr_type*))                  \
            .times(1)                                                                \
            .will(invoke(st_atomic_##c_api_type##_stub));                            \
        asc_store_atomic(addr, value);                                               \
        GlobalMockObject::verify();                                                  \
    }

TEST_STORE_ATOMIC(int8_t, int8_t);
TEST_STORE_ATOMIC(int16_t, int16_t);
TEST_STORE_ATOMIC(int32_t, int32_t);
TEST_STORE_ATOMIC(half, int16_t);
TEST_STORE_ATOMIC(float, int32_t);
TEST_STORE_ATOMIC(bfloat16_t, int16_t);
