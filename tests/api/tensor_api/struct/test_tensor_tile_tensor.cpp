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


class Tensor_Api_Tensor : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Tensor, TensorOperation)
{
    auto iterator = AscendC::MakeL1memPtr(reinterpret_cast<__cbuf__ half*>(0));
    auto layout = AscendC::MakeNZLayout<half>(16, 16);
    auto tensor = AscendC::MakeTensor(iterator, layout);
    
    auto retTensor = tensor.Tensor();
    auto retEngine = tensor.Engine();
    auto retLayout = tensor.Layout();
    EXPECT_EQ(iterator.Get(), retEngine.Begin().Get());
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(layout.GetShape())), AscendC::Std::get<0>(AscendC::Std::get<0>(retLayout.GetShape())));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(layout.GetShape())), AscendC::Std::get<1>(AscendC::Std::get<0>(retLayout.GetShape())));
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(layout.GetShape())), AscendC::Std::get<0>(AscendC::Std::get<1>(retLayout.GetShape())));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(layout.GetShape())), AscendC::Std::get<1>(AscendC::Std::get<1>(retLayout.GetShape())));
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(layout.GetStride())), AscendC::Std::get<0>(AscendC::Std::get<0>(retLayout.GetStride())));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(layout.GetStride())), AscendC::Std::get<1>(AscendC::Std::get<0>(retLayout.GetStride())));
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(layout.GetStride())), AscendC::Std::get<0>(AscendC::Std::get<1>(retLayout.GetStride())));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(layout.GetStride())), AscendC::Std::get<1>(AscendC::Std::get<1>(retLayout.GetStride())));
}

TEST_F(Tensor_Api_Tensor, EngineOperation)
{
    auto iterator = AscendC::MakeL1memPtr(reinterpret_cast<__cbuf__ half*>(0));
    auto layout = AscendC::MakeNZLayout<half>(16, 16);
    auto tensor = AscendC::MakeTensor(iterator, layout);
    
    auto retEngine = tensor.Engine();
    EXPECT_EQ(iterator.Get(), retEngine.Begin().Get());
}

TEST_F(Tensor_Api_Tensor, LyaoutOperation)
{
    auto iterator = AscendC::MakeL1memPtr(reinterpret_cast<__cbuf__ half*>(0));
    auto layout = AscendC::MakeNZLayout<half>(16, 16);
    auto tensor = AscendC::MakeTensor(iterator, layout);
    
    auto retLayout = tensor.Layout();
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(layout.GetShape())), AscendC::Std::get<0>(AscendC::Std::get<0>(retLayout.GetShape())));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(layout.GetShape())), AscendC::Std::get<1>(AscendC::Std::get<0>(retLayout.GetShape())));
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(layout.GetShape())), AscendC::Std::get<0>(AscendC::Std::get<1>(retLayout.GetShape())));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(layout.GetShape())), AscendC::Std::get<1>(AscendC::Std::get<1>(retLayout.GetShape())));
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(layout.GetStride())), AscendC::Std::get<0>(AscendC::Std::get<0>(retLayout.GetStride())));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(layout.GetStride())), AscendC::Std::get<1>(AscendC::Std::get<0>(retLayout.GetStride())));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(layout.GetStride())), AscendC::Std::get<1>(AscendC::Std::get<1>(retLayout.GetStride())));
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(layout.GetStride())), AscendC::Std::get<0>(AscendC::Std::get<1>(retLayout.GetStride())));

}

TEST_F(Tensor_Api_Tensor, DataOperation)
{
    auto iterator = AscendC::MakeL1memPtr(reinterpret_cast<__cbuf__ half*>(0));
    auto layout = AscendC::MakeNZLayout<half>(16, 16);
    auto tensor = AscendC::MakeTensor(iterator, layout);
    
    auto retData = tensor.Data();
    EXPECT_EQ(iterator.Get(), retData.Get());
}

TEST_F(Tensor_Api_Tensor, ShapeOperation)
{
    auto iterator = AscendC::MakeL1memPtr(reinterpret_cast<__cbuf__ half*>(0));
    auto layout = AscendC::MakeNZLayout<half>(16, 16);
    auto tensor = AscendC::MakeTensor(iterator, layout);
    
    auto retShape = tensor.Shape();
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(layout.GetShape())), AscendC::Std::get<0>(AscendC::Std::get<0>(retShape)));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(layout.GetShape())), AscendC::Std::get<1>(AscendC::Std::get<0>(retShape)));
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(layout.GetShape())), AscendC::Std::get<0>(AscendC::Std::get<1>(retShape)));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(layout.GetShape())), AscendC::Std::get<1>(AscendC::Std::get<1>(retShape)));
}

TEST_F(Tensor_Api_Tensor, StrideOperation)
{
    auto iterator = AscendC::MakeL1memPtr(reinterpret_cast<__cbuf__ half*>(0));
    auto layout = AscendC::MakeNZLayout<half>(16, 16);
    auto tensor = AscendC::MakeTensor(iterator, layout);
    
    auto retStride = tensor.Stride();
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(layout.GetStride())), AscendC::Std::get<0>(AscendC::Std::get<0>(retStride)));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(layout.GetStride())), AscendC::Std::get<1>(AscendC::Std::get<0>(retStride)));
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(layout.GetStride())), AscendC::Std::get<1>(AscendC::Std::get<1>(retStride)));
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(layout.GetStride())), AscendC::Std::get<0>(AscendC::Std::get<1>(retStride)));
}

// TEST_F(Tensor_Api_Tensor, SizeOperation)
// {
//     auto iterator = AscendC::MakeL1memPtr(reinterpret_cast<__cbuf__ half*>(0));
//     auto layout = AscendC::MakeNZLayout<half>(16, 16);
//     auto tensor = AscendC::MakeTensor(iterator, layout);
    
//     auto size = tensor.Size();
//     EXPECT_EQ(Layout.Size(), size);
// }

// TEST_F(Tensor_Api_Tensor, CapacotyOperation)
// {
//     auto iterator = AscendC::MakeL1memPtr(reinterpret_cast<__cbuf__ half*>(0));
//     auto layout = AscendC::MakeNZLayout<half>(16, 16);
//     auto tensor = AscendC::MakeTensor(iterator, layout);
    
//     auto capacity = tensor.Capacity();
//     EXPECT_EQ(Layout.Capacity(), capacity);
// }

// TEST_F(Tensor_Api_Tensor, SquareBracketsOperation)
// {
//     auto iterator = AscendC::MakeL1memPtr(reinterpret_cast<__cbuf__ half*>(0));
//     auto layout = AscendC::MakeNZLayout<half>(16, 16);
//     auto tensor = AscendC::MakeTensor(iterator, layout);
//     auto coord = AscendC::MakeCoord(MakeCoord(0, 0), MakeCoord(0, 0));
//     auto ret = tensor[coord];
//     EXPECT_EQ(tensor.Data().Get(), ret.Get());
// }

// TEST_F(Tensor_Api_Tensor, ParenthesesOperation)
// {
//     auto iterator = AscendC::MakeL1memPtr(reinterpret_cast<__cbuf__ half*>(0));
//     auto layout = AscendC::MakeNZLayout<half>(16, 16);
//     auto tensor = AscendC::MakeTensor(iterator, layout);
//     auto coord = AscendC::MakeCoord(MakeCoord(0, 0), MakeCoord(0, 0));
//     auto ret = tensor(coord);
//     EXPECT_EQ(tensor.Data().Get(), ret.Get());
// }