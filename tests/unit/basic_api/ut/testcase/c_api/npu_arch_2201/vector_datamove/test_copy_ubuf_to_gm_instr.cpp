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
#include "c_api/c_api_utils_intf.h"

#define TEST_VECTOR_DATAMOVE_COPY_UBUF_TO_GM_INSTR(class_name, c_api_name, cce_name)            \
                                                                                                \
class TestVectorDatamove##class_name : public testing::Test {                                   \
protected:                                                                                      \
    void SetUp() {}                                                                             \
    void TearDown() {}                                                                          \
};                                                                                              \
                                                                                                \
namespace {                                                                                     \
                                                                                                \
void cce_name##_uint64_t_Stub(__gm__ void *dst, __ubuf__ void *src, uint64_t config)            \
{                                                                                               \
    EXPECT_EQ(dst, reinterpret_cast<__gm__ void *>(11));                                        \
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ void *>(22));                                      \
    EXPECT_EQ(config, 0x10010);                                                                 \
}                                                                                               \
                                                                                                \
}                                                                                               \
                                                                                                \
TEST_F(TestVectorDatamove##class_name, c_api_name##_CopyConfig_Succ)                            \
{                                                                                               \
    __gm__ void *dst = reinterpret_cast<__gm__ void *>(11);                                     \
    __ubuf__ void *src = reinterpret_cast<__ubuf__ void *>(22);                                 \
                                                                                                \
    asc_copy_config config;                                                                     \
    config.sid = static_cast<uint64_t>(0);                                                      \
    config.n_burst = static_cast<uint64_t>(1);                                                  \
    config.burst_len = static_cast<uint64_t>(1);                                                \
    config.src_gap = static_cast<uint64_t>(0);                                                  \
    config.dst_gap = static_cast<uint64_t>(0);                                                  \
                                                                                                \
    MOCKER_CPP(cce_name, void(__gm__ void *, __ubuf__ void *, uint64_t))                        \
            .times(1)                                                                           \
            .will(invoke(cce_name##_uint64_t_Stub));                                            \
                                                                                                \
    c_api_name(dst, src, config);                                                               \
    GlobalMockObject::verify();                                                                 \
}                                                                                               \
                                                                                                \
TEST_F(TestVectorDatamove##class_name, c_api_name##_size_Succ)                                  \
{                                                                                               \
    __gm__ void *dst = reinterpret_cast<__gm__ void *>(11);                                     \
    __ubuf__ void *src = reinterpret_cast<__ubuf__ void *>(22);                                 \
    uint32_t size = static_cast<uint32_t>(44);                                                  \
                                                                                                \
    MOCKER_CPP(cce_name, void(__gm__ void *, __ubuf__ void *, uint64_t))                        \
            .times(1)                                                                           \
            .will(invoke(cce_name##_uint64_t_Stub));                                            \
                                                                                                \
    c_api_name(dst, src, size);                                                                 \
    GlobalMockObject::verify();                                                                 \
}                                                                                               \
                                                                                                \
TEST_F(TestVectorDatamove##class_name, c_api_name##_sync_Succ)                                  \
{                                                                                               \
    __gm__ void *dst = reinterpret_cast<__gm__ void *>(11);                                     \
    __ubuf__ void *src = reinterpret_cast<__ubuf__ void *>(22);                                 \
    uint32_t size = static_cast<uint32_t>(44);                                                  \
                                                                                                \
    MOCKER_CPP(cce_name, void(__gm__ void *, __ubuf__ void *, uint64_t))                        \
            .times(1)                                                                           \
            .will(invoke(cce_name##_uint64_t_Stub));                                            \
    c_api_name##_sync(dst, src, size);                                                          \
    GlobalMockObject::verify();                                                                 \
}                                                                                               \

// ==========asc_copy_ub2gm==========
TEST_VECTOR_DATAMOVE_COPY_UBUF_TO_GM_INSTR(CopyUB2GM, asc_copy_ub2gm, copy_ubuf_to_gm);