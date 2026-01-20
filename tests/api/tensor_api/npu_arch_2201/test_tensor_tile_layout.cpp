/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "tensor_api_impl.h"


class TensorTileLayoutTest : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

template <size_t v>
using Int = AscendC::Std::integral_constant<size_t, v>; 

TEST_F(TensorTileLayoutTest, ShapeAndStrideOperation)
{
    AscendC::Shape<int, int, int> shapeInit{11, 22, 33};
    AscendC::Stride<int, int, int> strideInit{44, 55, 66};

    AscendC::Shape<int, int, int> shapeMake = AscendC::MakeShape(10, 20, 30);
    AscendC::Stride<int, int, int> strideMake = AscendC::MakeStride(40, 50, 60);

    auto shapeMakeInt = AscendC::MakeShape(Int<111>{}, Int<222>{}, Int<333>{});
    auto strideMakeInt = AscendC::MakeStride(Int<444>{}, Int<555>{}, Int<666>{});

    EXPECT_EQ(AscendC::Std::get<0>(shapeInit), 11);
    EXPECT_EQ(AscendC::Std::get<1>(shapeInit), 22);
    EXPECT_EQ(AscendC::Std::get<2>(shapeInit), 33);

    EXPECT_EQ(AscendC::Std::get<0>(strideInit), 44);
    EXPECT_EQ(AscendC::Std::get<1>(strideInit), 55);
    EXPECT_EQ(AscendC::Std::get<2>(strideInit), 66);

    EXPECT_EQ(AscendC::Std::get<0>(shapeMake), 10);
    EXPECT_EQ(AscendC::Std::get<1>(shapeMake), 20);
    EXPECT_EQ(AscendC::Std::get<2>(shapeMake), 30);

    EXPECT_EQ(AscendC::Std::get<0>(strideMake), 40);
    EXPECT_EQ(AscendC::Std::get<1>(strideMake), 50);
    EXPECT_EQ(AscendC::Std::get<2>(strideMake), 60);

    EXPECT_EQ(AscendC::Std::get<0>(shapeMakeInt).value, 111);
    EXPECT_EQ(AscendC::Std::get<1>(shapeMakeInt).value, 222);
    EXPECT_EQ(AscendC::Std::get<2>(shapeMakeInt).value, 333);

    EXPECT_EQ(AscendC::Std::get<0>(strideMakeInt).value, 444);
    EXPECT_EQ(AscendC::Std::get<1>(strideMakeInt).value, 555);
    EXPECT_EQ(AscendC::Std::get<2>(strideMakeInt).value, 666);
}

TEST_F(TensorTileLayoutTest, IsTupleOperation)
{
    AscendC::Shape<int, int, int> shapeInit{11, 22, 33};
    AscendC::Stride<int, int, int> strideInit{44, 55, 66};

    AscendC::Shape<int, int, int> shapeMake = AscendC::MakeShape(10, 20, 30);
    AscendC::Stride<int, int, int> strideMake = AscendC::MakeStride(40, 50, 60);

    auto shapeMakeInt = AscendC::MakeShape(Int<111>{}, Int<222>{}, Int<333>{});
    auto strideMakeInt = AscendC::MakeStride(Int<444>{}, Int<555>{}, Int<666>{});

    EXPECT_EQ(AscendC::Std::is_tuple<decltype(shapeInit)>::value, true);
    EXPECT_EQ(AscendC::Std::is_tuple<decltype(strideInit)>::value, true);
    EXPECT_EQ(AscendC::Std::is_tuple<decltype(shapeMake)>::value, true);
    EXPECT_EQ(AscendC::Std::is_tuple<decltype(strideMake)>::value, true);
    EXPECT_EQ(AscendC::Std::is_tuple<decltype(shapeMakeInt)>::value, true);
    EXPECT_EQ(AscendC::Std::is_tuple<decltype(strideMakeInt)>::value, true);

    EXPECT_EQ(AscendC::Std::is_tuple_v<decltype(shapeInit)>, true);
    EXPECT_EQ(AscendC::Std::is_tuple_v<decltype(strideInit)>, true);
    EXPECT_EQ(AscendC::Std::is_tuple_v<decltype(shapeMake)>, true);
    EXPECT_EQ(AscendC::Std::is_tuple_v<decltype(strideMake)>, true);
    EXPECT_EQ(AscendC::Std::is_tuple_v<decltype(shapeMakeInt)>, true);
    EXPECT_EQ(AscendC::Std::is_tuple_v<decltype(strideMakeInt)>, true);
}

TEST_F(TensorTileLayoutTest, LayoutOperation)
{
    AscendC::Shape<int,int,int> shape = AscendC::MakeShape(10, 20, 30);
    AscendC::Stride<int,int,int> stride = AscendC::MakeStride(1, 100, 200);

    auto layoutMake = AscendC::MakeLayout(shape, stride);

    AscendC::Layout<AscendC::Shape<int, int, int>, AscendC::Stride<int, int, int>> layoutInit(shape, stride);

    EXPECT_EQ(AscendC::Std::get<0>(layoutMake.GetShape()), 10);
    EXPECT_EQ(AscendC::Std::get<1>(layoutMake.GetShape()), 20);
    EXPECT_EQ(AscendC::Std::get<2>(layoutMake.GetShape()), 30);

    EXPECT_EQ(AscendC::Std::get<0>(layoutMake.GetStride()), 1);
    EXPECT_EQ(AscendC::Std::get<1>(layoutMake.GetStride()), 100);
    EXPECT_EQ(AscendC::Std::get<2>(layoutMake.GetStride()), 200);


    EXPECT_EQ(AscendC::Std::get<0>(layoutInit.GetShape()), 10);
    EXPECT_EQ(AscendC::Std::get<1>(layoutInit.GetShape()), 20);
    EXPECT_EQ(AscendC::Std::get<2>(layoutInit.GetShape()), 30);

    EXPECT_EQ(AscendC::Std::get<0>(layoutInit.GetStride()), 1);
    EXPECT_EQ(AscendC::Std::get<1>(layoutInit.GetStride()), 100);
    EXPECT_EQ(AscendC::Std::get<2>(layoutInit.GetStride()), 200);
}

TEST_F(TensorTileLayoutTest, IsLayoutOperation)
{
    AscendC::Shape<int,int,int> shape = AscendC::MakeShape(10, 20, 30);
    AscendC::Stride<int,int,int> stride = AscendC::MakeStride(1, 100, 200);

    auto layoutMake = AscendC::MakeLayout(shape, stride);

    AscendC::Layout<AscendC::Shape<int, int, int>, AscendC::Stride<int, int, int>> layoutInit(shape, stride);

    EXPECT_EQ(AscendC::is_layout<decltype(shape)>::value, false);
    EXPECT_EQ(AscendC::is_layout<decltype(stride)>::value, false);
    EXPECT_EQ(AscendC::is_layout<decltype(layoutMake)>::value, true);
    EXPECT_EQ(AscendC::is_layout<decltype(layoutInit)>::value, true);

    EXPECT_EQ(AscendC::is_layout_v<decltype(shape)>, false);
    EXPECT_EQ(AscendC::is_layout_v<decltype(stride)>, false);
    EXPECT_EQ(AscendC::is_layout_v<decltype(layoutMake)>, true);
    EXPECT_EQ(AscendC::is_layout_v<decltype(layoutInit)>, true);
}

