/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to License for details. You may not use this file except in compliance with License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"

class Tensor_Api_Layout_Fractal : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Layout_Fractal, MakeNzLayout)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeNzLayout<float>(16, 8);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_EQ(tensor.Size(), 16 * 8);
    EXPECT_EQ(tensor.Layout().Size(), 16 * 8);
}

TEST_F(Tensor_Api_Layout_Fractal, MakeZnLayout)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeZnLayout<float>(8, 16);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_EQ(tensor.Size(), 8 * 16);
    EXPECT_EQ(tensor.Layout().Size(), 8 * 16);
}

TEST_F(Tensor_Api_Layout_Fractal, MakeNdLayout)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeNDLayout<float>(16, 8);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_EQ(tensor.Size(), 16 * 8);
    EXPECT_EQ(tensor.Layout().Size(), 16 * 8);
}

TEST_F(Tensor_Api_Layout_Fractal, MakeZzLayout)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeZzLayout<float>(8, 8);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_EQ(tensor.Size(), 8 * 8);
    EXPECT_EQ(tensor.Layout().Size(), 8 * 8);
}

TEST_F(Tensor_Api_Layout_Fractal, MakeNnLayout)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeNnLayout<float>(16, 8);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_EQ(tensor.Size(), 16 * 8);
    EXPECT_EQ(tensor.Layout().Size(), 16 * 8);
}

TEST_F(Tensor_Api_Layout_Fractal, LayoutWithDifferentTypes)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ half halfData[TILE_LENGTH] = {0};
    __gm__ int32_t intData[TILE_LENGTH] = {0};
    __gm__ uint8_t byteData[TILE_LENGTH] = {0};

    auto halfPtr = MakeGMmemPtr(halfData);
    auto intPtr = MakeGMmemPtr(intData);
    auto bytePtr = MakeGMmemPtr(byteData);

    auto halfLayout = MakeNzLayout<half>(16, 8);
    auto intLayout = MakeNDLayout<int32_t>(16, 8);
    auto byteLayout = MakeZnLayout<uint8_t>(8, 16);

    auto halfTensor = MakeTensor(halfPtr, halfLayout);
    auto intTensor = MakeTensor(intPtr, intLayout);
    auto byteTensor = MakeTensor(bytePtr, byteLayout);

    EXPECT_EQ(halfTensor.Size(), 16 * 8);
    EXPECT_EQ(intTensor.Size(), 16 * 8);
    EXPECT_EQ(byteTensor.Size(), 8 * 16);
}

TEST_F(Tensor_Api_Layout_Fractal, LayoutWithIntShape)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeNzLayout<float>(Int<16>{}, Int<8>{});
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_EQ(tensor.Layout().Size(), 16 * 8);
}

TEST_F(Tensor_Api_Layout_Fractal, Layout3D)
{
    using namespace AscendC::Te;
    using namespace AscendC::Std;

    constexpr uint32_t TILE_LENGTH = 256;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeLayout(MakeShape(4, 8, 8), MakeStride(64, 8, 1));
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_EQ(tensor.Size(), 4 * 8 * 8);
    EXPECT_EQ(tensor.Layout().Size(), 4 * 8 * 8);
}
