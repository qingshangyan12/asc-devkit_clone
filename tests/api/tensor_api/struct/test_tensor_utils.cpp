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

class Tensor_Api_Utils : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Utils, GetC0Size)
{
    using namespace AscendC::Te;

    size_t c0SizeFloat = GetC0Size<float>();
    EXPECT_EQ(c0SizeFloat, 32);

    size_t c0SizeHalf = GetC0Size<half>();
    EXPECT_EQ(c0SizeHalf, 32);

    size_t c0SizeInt32 = GetC0Size<int32_t>();
    EXPECT_EQ(c0SizeInt32, 32);
}

TEST_F(Tensor_Api_Utils, C0SizeConstant)
{
    using namespace AscendC::Te;

    EXPECT_EQ(C0_SIZE<float>, 32);
    EXPECT_EQ(C0_SIZE<half>, 32);
    EXPECT_EQ(C0_SIZE<int32_t>, 32);
}

TEST_F(Tensor_Api_Utils, C0Element)
{
    using namespace AscendC::Te;

    EXPECT_EQ(C0_ELEMENT<float>, 8);
    EXPECT_EQ(C0_ELEMENT<half>, 16);
    EXPECT_EQ(C0_ELEMENT<int32_t>, 8);
}

TEST_F(Tensor_Api_Utils, IsIntegralConstant)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    bool isIntConstant = IsIntegralConstantV<AscendC::Std::Int<5>>;
    EXPECT_TRUE(isIntConstant);

    bool isNotIntConstant = IsIntegralConstantV<int>;
    EXPECT_FALSE(isNotIntConstant);
}

TEST_F(Tensor_Api_Utils, EmptyShape)
{
    using namespace AscendC::Te;

    using EmptyShape2 = EmptyShape<2>;
    using EmptyShape4 = EmptyShape<4>;

    EXPECT_TRUE(Std::tuple_size_v<EmptyShape2> == 2);
    EXPECT_TRUE(Std::tuple_size_v<EmptyShape4> == 4);
}

TEST_F(Tensor_Api_Utils, EmptyStride)
{
    using namespace AscendC::Te;

    using EmptyStride2 = EmptyStride<2>;
    using EmptyStride4 = EmptyStride<4>;

    EXPECT_TRUE(Std::tuple_size_v<EmptyStride2> == 2);
    EXPECT_TRUE(Std::tuple_size_v<EmptyStride4> == 4);
}

TEST_F(Tensor_Api_Utils, EmptyCoord)
{
    using namespace AscendC::Te;

    using EmptyCoord2 = EmptyCoord<2>;
    using EmptyCoord4 = EmptyCoord<4>;

    EXPECT_TRUE(Std::tuple_size_v<EmptyCoord2> == 2);
    EXPECT_TRUE(Std::tuple_size_v<EmptyCoord4> == 4);
}
