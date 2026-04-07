/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"

class Tensor_Api_Utils_Constant : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Utils_Constant, ConstantValues)
{
    using namespace AscendC::Te;

    EXPECT_EQ(TWO_DIM_DATA, 2);
    EXPECT_EQ(FOUR_DIM_DATA, 4);
    EXPECT_EQ(FRACTAL_FIXED, 16);
    EXPECT_EQ(DISABLE_COORD, 0);
    EXPECT_EQ(ENABLE_COORD, 1);
    EXPECT_EQ(SHIFT_LEFT_16, 0x00010000);
    EXPECT_EQ(L2_CACHE_OFFSET, 60);
    EXPECT_EQ(MX_SCALE_K0, 2);
    EXPECT_EQ(BLOCK_CUBE, 16);
}

TEST_F(Tensor_Api_Utils_Constant, ArchVersion)
{
    using namespace AscendC::Te;

    EXPECT_EQ(ArchVersion::V3510, 3510);
    EXPECT_EQ(ArchVersion::V2201, 2201);
}

TEST_F(Tensor_Api_Utils_Constant, CurrentArchVersion)
{
    using namespace AscendC::Te;

    uint32_t archVersion = CURRENT_ARCH_VERSION;
    EXPECT_TRUE(archVersion == 0 || archVersion == 3510 || archVersion == 2201);
}

TEST_F(Tensor_Api_Utils_Constant, GetC0Size)
{
    using namespace AscendC::Te;

    size_t c0SizeFloat = GetC0Size<float>();
    EXPECT_EQ(c0SizeFloat, 32);

    size_t c0SizeHalf = GetC0Size<half>();
    EXPECT_EQ(c0SizeHalf, 32);

    size_t c0SizeInt32 = GetC0Size<int32_t>();
    EXPECT_EQ(c0SizeInt32, 32);
}

TEST_F(Tensor_Api_Utils_Constant, IsIntegralConstant)
{
    using namespace AscendC::Te;

    bool isIntConstant = IsIntegralConstantV<AscendC::Std::Int<5>>;
    EXPECT_TRUE(isIntConstant);

    bool isNotIntConstant = IsIntegralConstantV<int>;
    EXPECT_FALSE(isNotIntConstant);
}
