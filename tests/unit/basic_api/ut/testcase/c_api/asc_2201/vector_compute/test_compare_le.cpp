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

class TestCompareLe : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

template<typename SRC_T>
void vcmp_le_stub(__ubuf__ SRC_T *src0, __ubuf__ SRC_T *src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    EXPECT_EQ(src0, reinterpret_cast<__ubuf__ SRC_T *>(222));
    EXPECT_EQ(src1, reinterpret_cast<__ubuf__ SRC_T *>(333));
    EXPECT_EQ(repeat, static_cast<uint8_t>(1));
    EXPECT_EQ(dst_block_stride, static_cast<uint8_t>(1));
    EXPECT_EQ(src0_block_stride, static_cast<uint8_t>(1));
    EXPECT_EQ(src1_block_stride, static_cast<uint8_t>(1));
    EXPECT_EQ(dst_repeat_stride, static_cast<uint8_t>(8));
    EXPECT_EQ(src0_repeat_stride, static_cast<uint8_t>(8));
    EXPECT_EQ(src1_repeat_stride, static_cast<uint8_t>(8));
}

#define TEST_CAPI_COMPARE_LE_TO_REG(test_name, target_name, cce_name, src_type, sync_times)                                                                 \
TEST_F(TestCompareLe, test_name) {                                                                                                                          \
    __ubuf__ src_type* src0 = (__ubuf__ src_type*)222;                                                                                                      \
    __ubuf__ src_type* src1 = (__ubuf__ src_type*)333;                                                                                                      \
    asc_binary_config config;                                                                                                                               \
    config.repeat = 1;                                                                                                                                      \
    config.dst_block_stride = 1;                                                                                                                            \
    config.src0_block_stride = 1;                                                                                                                           \
    config.src1_block_stride = 1;                                                                                                                           \
    config.dst_repeat_stride = 8;                                                                                                                           \
    config.src0_repeat_stride = 8;                                                                                                                          \
    config.src1_repeat_stride = 8;                                                                                                                          \
    MOCKER(set_mask_count).times(0);                                                                                                                        \
    MOCKER(set_mask_norm).times(0);                                                                                                                         \
    MOCKER(pipe_barrier).times(sync_times);                                                                                                                 \
    MOCKER(cce_name, void(__ubuf__ src_type*, __ubuf__ src_type*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t))                           \
        .times(1)                                                                                                                                           \
        .will(invoke(cce_name##_stub<src_type>));                                                                                                              \
    target_name(src0, src1, config);                                                                                                                        \
    GlobalMockObject::verify();                                                                                                                             \
}

TEST_CAPI_COMPARE_LE_TO_REG(asc_le_to_reg_half, asc_le, vcmp_le, half, 0);
TEST_CAPI_COMPARE_LE_TO_REG(asc_le_sync_to_reg_half, asc_le_sync, vcmp_le, half, 1);
TEST_CAPI_COMPARE_LE_TO_REG(asc_le_to_reg_float, asc_le, vcmp_le, float, 0);
TEST_CAPI_COMPARE_LE_TO_REG(asc_le_sync_to_reg_float, asc_le_sync, vcmp_le, float, 1);