/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cmath>
#include <gtest/gtest.h>
#include "impl/simt_api/cpp/dav_3510/kernel_simt_constant.h"

namespace AscendC {
namespace {
TEST(TestKernelSimtConstant, IntegerAndBitConstants)
{
    using namespace Simt::ConstantsInternal;

    EXPECT_EQ(ONE_INT32, 1);
    EXPECT_EQ(ONE_INT64, 1);
    EXPECT_EQ(ONE_UINT32, 1U);
    EXPECT_EQ(ONE_UINT64, 1U);
    EXPECT_EQ(ONE_BYTE_LEN, 8U);
    EXPECT_EQ(TWO_BYTE_LEN, 16U);
    EXPECT_EQ(FOUR_BYTE_LEN, 32U);
    EXPECT_EQ(EIGHT_BYTE_LEN, 64U);
    EXPECT_EQ(INF, 0x7F800000U);
    EXPECT_EQ(NEG_INF, 0xFF800000U);
    EXPECT_EQ(HALF_INF, 0x7C00U);
    EXPECT_EQ(HALF_NEG_INF, 0xFC00U);
    EXPECT_EQ(FULL_MASK_B32, 0xffffffffU);
    EXPECT_EQ(SIMT_DUMP_BLOCK_NUM, 72U);
    EXPECT_EQ(SIMT_DUMP_SIZE, 2048U);
    EXPECT_EQ(SIMT_MAX_THREAD_NUM, 2048U);
}

TEST(TestKernelSimtConstant, FloatingConstants)
{
    using namespace Simt::ConstantsInternal;

    EXPECT_FLOAT_EQ(PI_OF_2 * 2.0F, PI);
    EXPECT_FLOAT_EQ(PI_OF_4 * 2.0F, PI_OF_2);
    EXPECT_FLOAT_EQ(PI_OF_8 * 2.0F, PI_OF_4);
    EXPECT_FLOAT_EQ(SCALAR_LN2, 0.69314718055994530941723212145818F);
    EXPECT_TRUE(std::isinf(SIMT_FP32_INF));
    EXPECT_TRUE(std::isinf(SIMT_INFINITY));
}
} // namespace
} // namespace AscendC
