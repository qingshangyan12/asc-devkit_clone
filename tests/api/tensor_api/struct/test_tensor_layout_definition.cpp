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

class Tensor_Api_Layout_Definition : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Layout_Definition, MakeLayout)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(16, 8);
    auto stride = MakeStride(8, 1);
    auto layout = MakeLayout(shape, stride);

    EXPECT_EQ(layout.Size(), 16 * 8);
}

TEST_F(Tensor_Api_Layout_Definition, MakeLayout3D)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 256;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(4, 8, 8);
    auto stride = MakeStride(64, 8, 1);
    auto layout = MakeLayout(shape, stride);

    EXPECT_EQ(layout.Size(), 4 * 8 * 8);
}

TEST_F(Tensor_Api_Layout_Definition, MakeLayoutWithInt)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(AscendC::Std::Int<16>{}, AscendC::Std::Int<8>{});
    auto stride = MakeStride(AscendC::Std::Int<8>{}, AscendC::Std::Int<1>{});
    auto layout = MakeLayout(shape, stride);

    EXPECT_EQ(layout.Size(), 16 * 8);
}

TEST_F(Tensor_Api_Layout_Definition, MakeShape)
{
    using namespace AscendC::Te;

    auto shape2D = MakeShape(16, 8);
    auto shape3D = MakeShape(4, 8, 8);
    auto shape4D = MakeShape(2, 2, 4, 8);

    EXPECT_TRUE(Std::tuple_size_v<decltype(shape2D)> == 2);
    EXPECT_TRUE(Std::tuple_size_v<decltype(shape3D)> == 3);
    EXPECT_TRUE(Std::tuple_size_v<decltype(shape4D)> == 4);
}

TEST_F(Tensor_Api_Layout_Definition, MakeStride)
{
    using namespace AscendC::Te;

    auto stride2D = MakeStride(8, 1);
    auto stride3D = MakeStride(64, 8, 1);
    auto stride4D = MakeStride(64, 32, 8, 1);

    EXPECT_TRUE(Std::tuple_size_v<decltype(stride2D)> == 2);
    EXPECT_TRUE(Std::tuple_size_v<decltype(stride3D)> == 3);
    EXPECT_TRUE(Std::tuple_size_v<decltype(stride4D)> == 4);
}

TEST_F(Tensor_Api_Layout_Definition, MakeCoord)
{
    using namespace AscendC::Te;

    auto coord2D = MakeCoord(0, 0);
    auto coord3D = MakeCoord(1, 2, 3);
    auto coord4D = MakeCoord(0, 1, 2, 3);

    EXPECT_TRUE(Std::tuple_size_v<decltype(coord2D)> == 2);
    EXPECT_TRUE(Std::tuple_size_v<decltype(coord3D)> == 3);
    EXPECT_TRUE(Std::tuple_size_v<decltype(coord4D)> == 4);
}

TEST_F(Tensor_Api_Layout_Definition, MakeShapeWithInt)
{
    using namespace AscendC::Te;

    auto shape2D = MakeShape(AscendC::Std::Int<16>{}, AscendC::Std::Int<8>{});
    auto shape3D = MakeShape(AscendC::Std::Int<4>{}, AscendC::Std::Int<8>{}, AscendC::Std::Int<8>{});

    EXPECT_TRUE(Std::tuple_size_v<decltype(shape2D)> == 2);
    EXPECT_TRUE(Std::tuple_size_v<decltype(shape3D)> == 3);
}

TEST_F(Tensor_Api_Layout_Definition, MakeCoordWithInt)
{
    using namespace AscendC::Te;

    auto coord2D = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
    auto coord3D = MakeCoord(AscendC::Std::Int<1>{}, AscendC::Std::Int<2>{}, AscendC::Std::Int<3>{});

    EXPECT_TRUE(Std::tuple_size_v<decltype(coord2D)> == 2);
    EXPECT_TRUE(Std::tuple_size_v<decltype(coord3D)> == 3);
}
