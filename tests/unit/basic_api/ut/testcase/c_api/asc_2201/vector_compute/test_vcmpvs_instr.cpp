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

#define TEST_VECTOR_COMPUTE_VCMPVS_INSTR(class_name, c_api_name, cce_name, dst_type, src_type)  \
                                                                                                \
class TestVectorCompute##class_name##dst_type##_##src_type : public testing::Test {             \
protected:                                                                                      \
    void SetUp() {}                                                                             \
    void TearDown() {}                                                                          \
};                                                                                              \
                                                                                                \
namespace {                                                                                     \
                                                                                                \
void cce_name##_##dst_type##_##src_type##_uint8_t_uint8_t_uint8_t_uint8_t_uint8_t_uint8_t_uint8_t_Stub(__ubuf__ dst_type *dst,   \
                __ubuf__ src_type *src, src_type value, uint8_t repeat,                         \
                uint16_t dst_block_stride, uint16_t src_block_stride,                           \
                uint16_t dst_repeat_stride, uint16_t src_repeat_stride)                         \
{                                                                                               \
    EXPECT_EQ(dst, reinterpret_cast<__ubuf__ dst_type *>(11));                                  \
    EXPECT_EQ(src, reinterpret_cast<__ubuf__ src_type *>(22));                                 \
    src_type scalar = 3.3;                                                                      \
    EXPECT_EQ(value, scalar);                                                                    \
    EXPECT_EQ(repeat, static_cast<uint8_t>(1));                                                 \
    EXPECT_EQ(dst_block_stride, static_cast<uint16_t>(1));                                      \
    EXPECT_EQ(src_block_stride, static_cast<uint16_t>(1));                                      \
    EXPECT_EQ(dst_repeat_stride, static_cast<uint16_t>(8));                                     \
    EXPECT_EQ(src_repeat_stride, static_cast<uint16_t>(8));                                     \
}                                                                                               \
                                                                                                \
void cce_name##_##dst_type##_##src_type##_InitVcmpvsUnaryConfig(asc_unary_config &config)       \
{                                                                                               \
    config.dst_block_stride = static_cast<uint64_t>(1);                                         \
    config.src_block_stride = static_cast<uint64_t>(1);                                         \
    config.dst_repeat_stride = static_cast<uint64_t>(8);                                        \
    config.src_repeat_stride = static_cast<uint64_t>(8);                                        \
    config.repeat = static_cast<uint64_t>(1);                                                   \
}                                                                                               \
                                                                                                \
}                                                                                               \
                                                                                                \
TEST_F(TestVectorCompute##class_name##dst_type##_##src_type, c_api_name##_half_half_half_UnaryConfig_Succ) \
{                                                                                               \
    __ubuf__ dst_type *dst = reinterpret_cast<__ubuf__ dst_type *>(11);                         \
    __ubuf__ src_type *src = reinterpret_cast<__ubuf__ src_type *>(22);                        \
    src_type value = 3.3;                                                                        \
                                                                                                \
    asc_unary_config config;                                                                    \
    cce_name##_##dst_type##_##src_type##_InitVcmpvsUnaryConfig(config);                         \
                                                                                                \
    MOCKER_CPP(cce_name, void(__ubuf__ dst_type *,__ubuf__ src_type *, src_type,                \
                uint8_t, uint16_t, uint16_t, uint16_t, uint16_t))                               \
            .times(1)                                                                           \
            .will(invoke(cce_name##_##dst_type##_##src_type##_uint8_t_uint8_t_uint8_t_uint8_t_uint8_t_uint8_t_uint8_t_Stub));   \
                                                                                                \
    c_api_name(dst, src, value, config);                                                        \
    GlobalMockObject::verify();                                                                 \
}                                                                                               \
                                                                                                \
                                                                                                \
TEST_F(TestVectorCompute##class_name##dst_type##_##src_type, c_api_name##_sync_half_half_half_int32_t_Succ) \
{                                                                                               \
    __ubuf__ dst_type *dst = reinterpret_cast<__ubuf__ dst_type *>(11);                         \
    __ubuf__ src_type *src = reinterpret_cast<__ubuf__ src_type *>(22);                        \
    src_type value = 3.3;                                                                        \
    asc_unary_config config;                                                                    \
    cce_name##_##dst_type##_##src_type##_InitVcmpvsUnaryConfig(config);                         \
                                                                                                \
    MOCKER_CPP(cce_name, void(__ubuf__ dst_type *,__ubuf__ src_type *,                          \
                src_type, uint8_t, uint16_t, uint16_t, uint16_t, uint16_t))                     \
            .times(1)                                                                           \
            .will(invoke(cce_name##_##dst_type##_##src_type##_uint8_t_uint8_t_uint8_t_uint8_t_uint8_t_uint8_t_uint8_t_Stub));  \
    c_api_name##_sync(dst, src, value, config);                                                 \
    GlobalMockObject::verify();                                                                 \
}                                                                                               \

// ==========asc_gt_scalar(dst:uint8_t, src:half/float)==========
TEST_VECTOR_COMPUTE_VCMPVS_INSTR(GtScalar, asc_gt_scalar, vcmpvs_gt, uint8_t, float);
TEST_VECTOR_COMPUTE_VCMPVS_INSTR(GtScalar, asc_gt_scalar, vcmpvs_gt, uint8_t, half);
// ==========asc_eq_scalar(half/float/int32_t)==========
TEST_VECTOR_COMPUTE_VCMPVS_INSTR(EqScalar, asc_eq_scalar, vcmpvs_eq, uint8_t, half);
TEST_VECTOR_COMPUTE_VCMPVS_INSTR(EqScalar, asc_eq_scalar, vcmpvs_eq, uint8_t, float);
TEST_VECTOR_COMPUTE_VCMPVS_INSTR(EqScalar, asc_eq_scalar, vcmpvs_eq, uint8_t, int32_t);