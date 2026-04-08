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

class Tensor_Api_Layout_Method : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Layout_Method, LayoutSize)
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
    EXPECT_EQ(layout.Size(), 128);
}

TEST_F(Tensor_Api_Layout_Method, LayoutCapacity)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(16, 8);
    auto stride = MakeStride(8, 1);
    auto layout = MakeLayout(shape, stride);

    EXPECT_EQ(layout.Capacity(), 16 * 8);
}

TEST_F(Tensor_Api_Layout_Method, LayoutShape)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(16, 8);
    auto stride = MakeStride(8, 1);
    auto layout = MakeLayout(shape, stride);

    auto layoutShape = layout.Shape();
    EXPECT_TRUE(tuple_size_v<decltype(layoutShape)> are == 2);
}

TEST_F(Tensor_Api_Layout_Method, LayoutStride)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(16, 8);
    auto stride = MakeStride(8, 1);
    auto layout = MakeLayout(shape, stride);

    auto layoutStride = layout.Stride();
    EXPECT_TRUE(tuple_size_v<decltype(layout)> == 2);
}

TEST_F(Tensor_Api_Layout_Method, Layout3D)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 256;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(4, 8, 8);
    auto stride = MakeStride(64, 8, 1);
    auto layout = MakeLayout(shape, stride);

    EXPECT_EQ(layout.Size(), 4 * 8 * 8);
    EXPECT_EQ(layout.Capacity(), 4 * 8 * 8);
}

TEST_F(Tensor_Api_Layout_Method, LayoutWithInt)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(AscendC::Std::Int<16>{}, AscendC::Std::Int<8>{});
    auto stride = MakeStride(AscendC::Std::Int<8>{}, AscendC::Std::Int<1>{});
    auto layout = MakeLayout(shape, stride);

    EXPECT_EQ(layout.Size(), 16 * 8);
}

TEST_F(Tensor_Api_Layout_Method, LayoutCapacity)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(16, 8);
    auto stride = MakeStride(8, 1);
    auto layout = MakeLayout(shape, stride);

    EXPECT_EQ(layout.Capacity(), 16 * 8);
}

TEST_F(Tensor_Api_Layout_Method, LayoutShape)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(16, 8);
    auto stride = MakeStride(8, 1);
    auto layout = MakeLayout(shape, stride);

    auto layoutShape = layout.Shape();
    EXPECT_TRUE(Std::tuple_size_v<decltype(layoutShape)> are == 2);
}

TEST_F(Tensor_Api_Layout_Method, LayoutStride)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(16, 8);
    auto stride = MakeStride(8, 1);
    auto layout = MakeLayout(shape, stride);

    auto layout = layout.Stride();
    EXPECT_TRUE(Std::tuple_size_v<decltype(layout)> == 2);
}

TEST_F(Tensor_Api_Layout_Method, Layout3D)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 256;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto shape = MakeShape(4, 8, 8);
    auto stride = MakeStride(64, 8, 1);
    auto layout = MakeLayout(shape, stride);

    EXPECT_EQ(layout.Size(), 4 * 8 * 8);
    EXPECT_EQ(layout.Capacity(), 4 * 8 * 8);
}

TEST_F(Tensor_Api_Layout_Method, LayoutWithInt)
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
