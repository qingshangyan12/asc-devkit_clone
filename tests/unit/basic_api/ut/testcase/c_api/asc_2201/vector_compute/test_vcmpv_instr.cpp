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

#define TEST_VECTOR_COMPUTE_COMPARE_VCMPV_INSTR(class_name, c_api_name, cce_name, dst_type, src_type)  \
                                                                                                 \
class TestVectorCompute##class_name##_##dst_type##_##src_type : public testing :: Test {                      \
    protected:                                                                                   \
    void SetUp() {}                                                                              \
    void TearDown() {}                                                                           \
};                                                                                               \
                                                                                                 \
namespace {                                                                                      \
                                                                                                 \
void cce_name##_##src_type##uint8_t_Stub(__ubuf__ dst_type *dst, __ubuf__ src_type *src0,       \
            __ubuf__ src_type *src1, uint8_t repeat, uint8_t dst_block_stride,                   \
            uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,    \
            uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)                             \
{                                                                                               \
    EXPECT_EQ(src0, reinterpret_cast<__ubuf__ src_type *>(11));                                \
    EXPECT_EQ(src1, reinterpret_cast<__ubuf__ src_type *>(22));                                \
    EXPECT_EQ(dst, reinterpret_cast<__ubuf__ dst_type *>(33));                                 \
    EXPECT_EQ(repeat, static_cast<uint8_t>(1));                                                 \
    EXPECT_EQ(dst_block_stride, static_cast<uint8_t>(1));                                       \
    EXPECT_EQ(src0_block_stride, static_cast<uint8_t>(1));                                      \
    EXPECT_EQ(src1_block_stride, static_cast<uint8_t>(1));                                      \
    EXPECT_EQ(dst_repeat_stride, static_cast<uint8_t>(8));                                      \
    EXPECT_EQ(src0_repeat_stride, static_cast<uint8_t>(8));                                     \
    EXPECT_EQ(src1_repeat_stride, static_cast<uint8_t>(8));                                     \
}                                                                                               \
}                                                                                               \
                                                                                            \
TEST_F(TestVectorCompute##class_name##_##dst_type##_##src_type, c_api_name##_asc_binary_config_succ)     \
{                                                                                           \
    __ubuf__ src_type *src0 = reinterpret_cast<__ubuf__ src_type *>(11);                    \
    __ubuf__ src_type *src1 = reinterpret_cast<__ubuf__ src_type *>(22);                    \
    __ubuf__ dst_type *dst = reinterpret_cast<__ubuf__ dst_type *>(33);                    \
                                                                                            \
    asc_binary_config config;                                                                   \
    config.dst_block_stride = static_cast<uint64_t>(1);                                         \
    config.src0_block_stride = static_cast<uint64_t>(1);                                        \
    config.src1_block_stride = static_cast<uint64_t>(1);                                        \
    config.dst_repeat_stride = static_cast<uint64_t>(8);                                        \
    config.src0_repeat_stride = static_cast<uint64_t>(8);                                       \
    config.src1_repeat_stride = static_cast<uint64_t>(8);                                       \
    config.repeat = static_cast<uint64_t>(1);                                                   \
                                                                                                \
    MOCKER_CPP(cce_name, void(__ubuf__ dst_type *, __ubuf__ src_type *,__ubuf__ src_type *,     \
                uint8_t, uint8_t, uint8_t,  uint8_t, uint8_t, uint8_t, uint8_t))                \
                .times(1)                                                                       \
                .will(invoke(cce_name##_##src_type##uint8_t_Stub));                             \
                                                                                                \
    c_api_name(dst, src0, src1, config);                                                        \
    GlobalMockObject::verify();                                                                 \
}                                                                                               \
                                                                                                \
TEST_F(TestVectorCompute##class_name##_##dst_type##_##src_type, c_api_name##_sync_binary_config_succ)        \
{                                                                                               \
    __ubuf__ src_type *src0 = reinterpret_cast<__ubuf__ src_type *>(11);                        \
    __ubuf__ src_type *src1 = reinterpret_cast<__ubuf__ src_type *>(22);                        \
    __ubuf__ dst_type *dst = reinterpret_cast<__ubuf__ dst_type *>(33);                         \
                                                                                                \
    asc_binary_config config;                                                                   \
    config.dst_block_stride = static_cast<uint64_t>(1);                                         \
    config.src0_block_stride = static_cast<uint64_t>(1);                                        \
    config.src1_block_stride = static_cast<uint64_t>(1);                                        \
    config.dst_repeat_stride = static_cast<uint64_t>(8);                                        \
    config.src0_repeat_stride = static_cast<uint64_t>(8);                                       \
    config.src1_repeat_stride = static_cast<uint64_t>(8);                                       \
    config.repeat = static_cast<uint64_t>(1);                                                   \
                                                                                                \
    MOCKER_CPP(cce_name, void(__ubuf__ dst_type *, __ubuf__ src_type *,__ubuf__ src_type *,     \
                uint8_t, uint8_t, uint8_t,  uint8_t, uint8_t, uint8_t, uint8_t))                \
                .times(1)                                                                       \
                .will(invoke(cce_name##_##src_type##uint8_t_Stub));                             \
                                                                                                \
                                                                                                \
    c_api_name##_sync(dst, src0, src1, config);                                                 \
    GlobalMockObject::verify();                                                                 \
}

// ==========asc_lt(half/float)==========
TEST_VECTOR_COMPUTE_COMPARE_VCMPV_INSTR(VCMPLT, asc_lt, vcmpv_lt, uint8_t, half);
TEST_VECTOR_COMPUTE_COMPARE_VCMPV_INSTR(VCMPLT, asc_lt, vcmpv_lt, uint8_t, float);
