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
#include "include/experimental/tensor_api/tensor.h"


class Tensor_Api_Layout : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

template <size_t v>
using Int = AscendC::Std::integral_constant<size_t, v>; 

TEST_F(Tensor_Api_Layout, ShapeAndStrideOperation)
{
    using namespace AscendC::Te;

    Shape<int, int, int> shapeInit{11, 22, 33};
    Stride<int, int, int> strideInit{44, 55, 66};

    Shape<int, int, int> shapeMake = MakeShape(10, 20, 30);
    Stride<int, int, int> strideMake = MakeStride(40, 50, 60);

    auto shapeMakeInt = MakeShape(Int<111>{}, Int<222>{}, Int<333>{});
    auto strideMakeInt = MakeStride(Int<444>{}, Int<555>{}, Int<666>{});

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

TEST_F(Tensor_Api_Layout, IsTupleOperation)
{
    using namespace AscendC::Te;

    Shape<int, int, int> shapeInit{11, 22, 33};
    Stride<int, int, int> strideInit{44, 55, 66};

    Shape<int, int, int> shapeMake = MakeShape(10, 20, 30);
    Stride<int, int, int> strideMake = MakeStride(40, 50, 60);

    auto shapeMakeInt = MakeShape(Int<111>{}, Int<222>{}, Int<333>{});
    auto strideMakeInt = MakeStride(Int<444>{}, Int<555>{}, Int<666>{});

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

TEST_F(Tensor_Api_Layout, LayoutOperation)
{
    using namespace AscendC::Te;

    Shape<int,int,int> shape = MakeShape(10, 20, 30);
    Stride<int,int,int> stride = MakeStride(1, 100, 200);

    auto layoutMake = MakeLayout(shape, stride);

    Layout<Shape<int, int, int>, Stride<int, int, int>> layoutInit(shape, stride);

    EXPECT_EQ(AscendC::Std::get<0>(layoutMake.Shape()), 10);
    EXPECT_EQ(AscendC::Std::get<1>(layoutMake.Shape()), 20);
    EXPECT_EQ(AscendC::Std::get<2>(layoutMake.Shape()), 30);

    EXPECT_EQ(AscendC::Std::get<0>(layoutMake.Stride()), 1);
    EXPECT_EQ(AscendC::Std::get<1>(layoutMake.Stride()), 100);
    EXPECT_EQ(AscendC::Std::get<2>(layoutMake.Stride()), 200);


    EXPECT_EQ(AscendC::Std::get<0>(layoutInit.Shape()), 10);
    EXPECT_EQ(AscendC::Std::get<1>(layoutInit.Shape()), 20);
    EXPECT_EQ(AscendC::Std::get<2>(layoutInit.Shape()), 30);

    EXPECT_EQ(AscendC::Std::get<0>(layoutInit.Stride()), 1);
    EXPECT_EQ(AscendC::Std::get<1>(layoutInit.Stride()), 100);
    EXPECT_EQ(AscendC::Std::get<2>(layoutInit.Stride()), 200);

    EXPECT_EQ(layoutMake.Rank(), 3);
    EXPECT_EQ(layoutMake.Rank<0>(), 1);
    EXPECT_EQ(Rank(layoutMake), 3);

    auto shapeTuple = GetShape(Select<1,2>(layoutMake));
    EXPECT_EQ(AscendC::Std::get<0>(shapeTuple), 20);
    EXPECT_EQ(Size(layoutMake), 6000);
    EXPECT_EQ(layoutMake.Size(), 6000);
    EXPECT_EQ(layoutMake.Size(), 6000);
    EXPECT_EQ(Coshape(layoutMake), 7710);
    EXPECT_EQ(Cosize(layoutMake), 7710);
}

TEST_F(Tensor_Api_Layout, IsLayoutOperation)
{
    using namespace AscendC::Te;

    Shape<int,int,int> shape = MakeShape(10, 20, 30);
    Stride<int,int,int> stride = MakeStride(1, 100, 200);

    auto layoutMake = MakeLayout(shape, stride);

    Layout<Shape<int, int, int>, Stride<int, int, int>> layoutInit(shape, stride);

    EXPECT_EQ(is_layout<decltype(shape)>::value, false);
    EXPECT_EQ(is_layout<decltype(stride)>::value, false);
    EXPECT_EQ(is_layout<decltype(layoutMake)>::value, true);
    EXPECT_EQ(is_layout<decltype(layoutInit)>::value, true);

    EXPECT_EQ(is_layout_v<decltype(shape)>, false);
    EXPECT_EQ(is_layout_v<decltype(stride)>, false);
    EXPECT_EQ(is_layout_v<decltype(layoutMake)>, true);
    EXPECT_EQ(is_layout_v<decltype(layoutInit)>, true);
}

TEST_F(Tensor_Api_Layout, MakeLayoutByShapeOperation)
{
    using namespace AscendC::Te;

    Shape<int,int,int> shape = MakeShape(2, 3, 4);
    Stride<int,int,int> stride = MakeStride(1, 2, 6);

    auto layoutMake1 = MakeLayout(shape, stride);
    auto layoutMake2 = MakeLayout(shape);

    EXPECT_EQ(AscendC::Std::get<1>(layoutMake1.Stride()), 2);
    EXPECT_EQ(AscendC::Std::get<1>(layoutMake2.Stride()), 4);
    EXPECT_EQ(Size(layoutMake1), 24);
    EXPECT_EQ(Size(layoutMake2), 24);
    EXPECT_EQ(layoutMake1.Size(), 24);
    EXPECT_EQ(layoutMake2.Size(), 24);
    EXPECT_EQ(layoutMake2.Capacity(), 24);
}

TEST_F(Tensor_Api_Layout, LayoutSizeOperation)
{
    using namespace AscendC::Te;

    using shape = Shape<Int<16>, Int<16>>;
    using stride = Stride<Int<1>, Int<16>>;
    Layout<shape, stride> layoutMake;
    Tile<int, int> tile = MakeTile(1,2);
    Coord<int, int> coord = MakeCoord(1,2);

    EXPECT_EQ(layoutMake.size, 256);
    EXPECT_EQ(AscendC::Std::get<0>(layoutMake.Shape()), 16);
    EXPECT_EQ(AscendC::Std::get<0>(GetShape(layoutMake)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(layoutMake.Stride()), 1);
    EXPECT_EQ(AscendC::Std::get<0>(GetStride(layoutMake)), 1);
    EXPECT_EQ(layoutMake.Size(), 256);
}

TEST_F(Tensor_Api_Layout, StaticLayoutOperation)
{
    using namespace AscendC::Te;
    
    using TwoDimT = AscendC::Std::tuple<AscendC::Std::Int<3>, AscendC::Std::Int<4>>;
    using TwoDimU = AscendC::Std::tuple<AscendC::Std::Int<2>, AscendC::Std::Int<1>>;
    EXPECT_EQ((StaticLayoutSize<TwoDimT, TwoDimU>::size), 6);

    using FourDimT = AscendC::Std::tuple<AscendC::Std::tuple<AscendC::Std::Int<3>, AscendC::Std::Int<4>>,
                                        AscendC::Std::tuple<AscendC::Std::Int<1>, AscendC::Std::Int<2>>>;
    using FourDimU = AscendC::Std::tuple<AscendC::Std::tuple<AscendC::Std::Int<4>, AscendC::Std::Int<5>>,
                                        AscendC::Std::tuple<AscendC::Std::Int<2>, AscendC::Std::Int<3>>>;
    EXPECT_EQ((StaticLayoutSize<FourDimT, FourDimU>::size), 20);
}

TEST_F(Tensor_Api_Layout, GetOperation)
{
    using namespace AscendC::Te;

    Shape<int, int, int> shape = MakeShape(10, 20, 30);
    Stride<int, int, int> stride = MakeStride(1, 100, 200);
    auto layout = MakeLayout(shape, stride);

    auto getAll = Get(layout);
    auto getShapeAll = GetShape(getAll);
    auto getStrideAll = GetStride(getAll);
    EXPECT_EQ(AscendC::Std::get<0>(getShapeAll), 10);
    EXPECT_EQ(AscendC::Std::get<0>(getStrideAll), 1);

    auto get1 = Get<1>(layout);
    auto get1Shape = GetShape(get1);
    auto get1Stride = GetStride(get1);
    EXPECT_EQ(AscendC::Std::get<0>(get1Shape), 20);
    EXPECT_EQ(AscendC::Std::get<0>(get1Stride), 100);

    auto nestedShape = MakeShape(MakeShape(2, 3), MakeShape(4, 5));
    auto nestedStride = MakeStride(MakeStride(1, 2), MakeStride(3, 4));
    auto nestedLayout = MakeLayout(nestedShape, nestedStride);

    auto nestedGetAll = Get(nestedLayout);
    auto nestedGetShape = GetShape(nestedGetAll);
    auto nestedGetStride = GetStride(nestedGetAll);
    auto nestedGetShape0 = AscendC::Std::get<0>(nestedGetShape);
    auto nestedGetStride1 = AscendC::Std::get<1>(nestedGetStride);
    EXPECT_EQ(AscendC::Std::get<0>(nestedGetShape0), 2);
    EXPECT_EQ(AscendC::Std::get<1>(nestedGetShape0), 3);
    EXPECT_EQ(AscendC::Std::get<0>(nestedGetStride1), 3);
    EXPECT_EQ(AscendC::Std::get<1>(nestedGetStride1), 4);

    auto nestedGet1 = Get<1>(nestedLayout);
    auto nestedGet1Shape = GetShape(nestedGet1);
    auto nestedGet1Stride = GetStride(nestedGet1);
    auto nestedGet1Shape0 = AscendC::Std::get<0>(nestedGet1Shape);
    auto nestedGet1Stride0 = AscendC::Std::get<0>(nestedGet1Stride);
    EXPECT_EQ(AscendC::Std::get<0>(nestedGet1Shape0), 4);
    EXPECT_EQ(AscendC::Std::get<0>(nestedGet1Stride0), 3);
}

TEST_F(Tensor_Api_Layout, SelectOperation)
{
    using namespace AscendC::Te;

    Shape<int,int,int> shape = MakeShape(10, 20, 30);
    Stride<int,int,int> stride = MakeStride(1, 100, 200);
    auto layout = MakeLayout(shape, stride);

    auto selectAll = Select(layout);
    auto selectAllShape = GetShape(selectAll);
    auto selectAllStride = GetStride(selectAll);
    EXPECT_EQ(AscendC::Std::get<0>(selectAllShape), 10);
    EXPECT_EQ(AscendC::Std::get<2>(selectAllStride), 200);

    auto select1 = Select<1>(layout);
    auto select1Shape = GetShape(select1);
    auto select1Stride = GetStride(select1);
    EXPECT_EQ(AscendC::Std::get<0>(select1Shape), 20);
    EXPECT_EQ(AscendC::Std::get<0>(select1Stride), 100);

    auto select12 = Select<1, 2>(layout);
    auto select12Shape = GetShape(select12);
    auto select12Stride = GetStride(select12);
    EXPECT_EQ(AscendC::Std::get<1>(select12Shape), 30);
    EXPECT_EQ(AscendC::Std::get<1>(select12Stride), 200);

    auto nestedShape = MakeShape(MakeShape(2, 3), MakeShape(4, 5));
    auto nestedStride = MakeStride(MakeStride(1, 2), MakeStride(3, 4));
    auto nestedLayout = MakeLayout(nestedShape, nestedStride);

    auto nestedSelectAll = Select(nestedLayout);
    auto nestedSelectAllShape = GetShape(nestedSelectAll);
    auto nestedSelectAllStride = GetStride(nestedSelectAll);
    auto nestedSelectAllShape0 = AscendC::Std::get<0>(nestedSelectAllShape);
    auto nestedSelectAllShape1 = AscendC::Std::get<1>(nestedSelectAllShape);
    auto nestedSelectAllStride0 = AscendC::Std::get<0>(nestedSelectAllStride);
    auto nestedSelectAllStride1 = AscendC::Std::get<1>(nestedSelectAllStride);

    EXPECT_EQ(AscendC::Std::get<0>(nestedSelectAllShape0), 2);
    EXPECT_EQ(AscendC::Std::get<0>(nestedSelectAllShape1), 4);
    EXPECT_EQ(AscendC::Std::get<1>(nestedSelectAllStride0), 2);
    EXPECT_EQ(AscendC::Std::get<1>(nestedSelectAllStride1), 4);

    auto nestedSelect0 = Select<0>(nestedLayout);
    auto nestedSelect0Shape = GetShape(nestedSelect0);
    auto nestedSelect0Stride = GetStride(nestedSelect0);
    EXPECT_EQ(AscendC::Std::get<0>(nestedSelect0Shape), 2);
    EXPECT_EQ(AscendC::Std::get<1>(nestedSelect0Stride), 2);

    auto nestedSelect01 = Select<0, 1>(nestedLayout);
    auto nestedSelect01Shape = GetShape(nestedSelect01);
    auto nestedSelect01Stride = GetStride(nestedSelect01);
    auto nestedSelect01Shape0 = AscendC::Std::get<0>(nestedSelect01Shape);
    auto nestedSelect01Shape1 = AscendC::Std::get<1>(nestedSelect01Shape);
    auto nestedSelect01Stride0 = AscendC::Std::get<0>(nestedSelect01Stride);
    auto nestedSelect01Stride1 = AscendC::Std::get<1>(nestedSelect01Stride);
    EXPECT_EQ(AscendC::Std::get<0>(nestedSelect01Shape0), 2);
    EXPECT_EQ(AscendC::Std::get<0>(nestedSelect01Shape1), 4);
    EXPECT_EQ(AscendC::Std::get<1>(nestedSelect01Stride0), 2);
    EXPECT_EQ(AscendC::Std::get<1>(nestedSelect01Stride1), 4);
}
