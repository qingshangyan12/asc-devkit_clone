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


class Tensor_Api_Tensor : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Tensor, ViewEngineOperation)
{
    using namespace AscendC::Te;

    int data[4] = {1, 2, 3, 4};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeLayout(MakeShape(2, 2), MakeStride(2, 1));
    auto tensor = MakeTensor(ptr, layout);

    auto engine = tensor.Engine();
    EXPECT_EQ(engine.Begin(), ptr);
    EXPECT_EQ(engine.Begin()[0], 1);
    EXPECT_EQ(engine.Begin()[3], 4);

    const auto& constTensor = tensor;
    EXPECT_EQ(constTensor.Engine().Begin(), ptr);

    ViewEngine<decltype(ptr)> viewEngine(ptr);
    EXPECT_EQ(viewEngine.Begin(), ptr);
    EXPECT_EQ(viewEngine.Begin()[1], 2);

    ConstViewEngine<decltype(ptr)> constViewEngine(ptr);
    EXPECT_EQ(constViewEngine.Begin(), ptr);
    EXPECT_EQ(constViewEngine.Begin()[1], 2);

    ViewEngine<decltype(ptr)> defaultEngine;
    EXPECT_EQ(defaultEngine.Begin().Get(), nullptr);

    ConstViewEngine<decltype(ptr)> defaultConstEngine;
    EXPECT_EQ(defaultConstEngine.Begin().Get(), nullptr);
}

TEST_F(Tensor_Api_Tensor, IteratorMakeMemPtrOperation)
{
    using namespace AscendC::Te;

    int gmData[4] = {0};
    int ubData[4] = {0};
    int l1Data[4] = {0};
    int l0aData[4] = {0};
    int l0bData[4] = {0};
    int l0cData[4] = {0};
    int biasData[4] = {0};
    int fixData[4] = {0};

    auto gmPtr = MakeGMmemPtr(gmData);
    auto ubPtr = MakeUBmemPtr(ubData);
    auto l1Ptr = MakeL1memPtr(l1Data);
    auto l0aPtr = MakeL0AmemPtr(l0aData);
    auto l0bPtr = MakeL0BmemPtr(l0bData);
    auto l0cPtr = MakeL0CmemPtr(l0cData);
    auto biasPtr = MakeBiasmemPtr(biasData);
    auto fixPtr = MakeFixbufmemPtr(fixData);

    EXPECT_EQ(gmPtr.Get(), gmData);
    EXPECT_EQ(ubPtr.Get(), ubData);
    EXPECT_EQ(l1Ptr.Get(), l1Data);
    EXPECT_EQ(l0aPtr.Get(), l0aData);
    EXPECT_EQ(l0bPtr.Get(), l0bData);
    EXPECT_EQ(l0cPtr.Get(), l0cData);
    EXPECT_EQ(biasPtr.Get(), biasData);
    EXPECT_EQ(fixPtr.Get(), fixData);

    EXPECT_EQ(MakeGMmemPtr(gmPtr), gmPtr);
    EXPECT_EQ(MakeUBmemPtr(ubPtr), ubPtr);
    EXPECT_EQ(MakeL1memPtr(l1Ptr), l1Ptr);
    EXPECT_EQ(MakeL0AmemPtr(l0aPtr), l0aPtr);
    EXPECT_EQ(MakeL0BmemPtr(l0bPtr), l0bPtr);
    EXPECT_EQ(MakeL0CmemPtr(l0cPtr), l0cPtr);
    EXPECT_EQ(MakeBiasmemPtr(biasPtr), biasPtr);
    EXPECT_EQ(MakeFixbufmemPtr(fixPtr), fixPtr);
}

TEST_F(Tensor_Api_Tensor, IteratorGetOperation)
{
    using namespace AscendC::Te;

    int data[4] = {1, 2, 3, 4};
    auto ptr = MakeGMmemPtr(data);
    auto offsetPtr = ptr + 2;

    EXPECT_EQ(ptr.Get(), data);
    EXPECT_EQ(offsetPtr.Get(), data + 2);
}

TEST_F(Tensor_Api_Tensor, IteratorOperatorOperation)
{
    using namespace AscendC::Te;

    int data[4] = {1, 2, 3, 4};
    auto ptr = MakeGMmemPtr(data);
    auto next = ptr + 1;
    auto far = ptr + 3;

    EXPECT_EQ(*ptr, 1);
    EXPECT_EQ(ptr[2], 3);

    *ptr = 10;
    next[1] = 20;
    EXPECT_EQ(data[0], 10);
    EXPECT_EQ(data[2], 20);

    EXPECT_TRUE(ptr == ptr);
    EXPECT_TRUE(ptr != next);
    EXPECT_TRUE(ptr < next);
    EXPECT_TRUE(next > ptr);
    EXPECT_TRUE(ptr <= next);
    EXPECT_TRUE(far >= next);
}

TEST_F(Tensor_Api_Tensor, LocalTensorOperation)
{
    using namespace AscendC::Te;

    int data[6] = {0, 1, 2, 3, 4, 5};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeLayout(MakeShape(2, 3), MakeStride(3, 1));
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_EQ(tensor.Tensor().Data(), ptr);
    EXPECT_EQ(tensor.Engine().Begin(), ptr);
    EXPECT_EQ(tensor.Layout().Size(), 6);
    EXPECT_EQ(tensor.Data(), ptr);

    auto shape = tensor.Shape();
    EXPECT_EQ(AscendC::Std::get<0>(shape), 2);
    EXPECT_EQ(AscendC::Std::get<1>(shape), 3);

    auto stride = tensor.Stride();
    EXPECT_EQ(AscendC::Std::get<0>(stride), 3);
    EXPECT_EQ(AscendC::Std::get<1>(stride), 1);

    EXPECT_EQ(tensor.Size(), 6);
    EXPECT_EQ(tensor.Capacity(), 6);

    auto coord = MakeCoord(1, 2);
    tensor[coord] = 9;
    EXPECT_EQ(data[5], 9);
    EXPECT_EQ(tensor(coord), 9);
    EXPECT_EQ(tensor(1, 2), 9);
}

TEST_F(Tensor_Api_Tensor, LocalTensorConstOperation)
{
    using namespace AscendC::Te;

    int data[6] = {0, 1, 2, 3, 4, 5};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeLayout(MakeShape(2, 3), MakeStride(3, 1));
    const auto constTensor = MakeTensor(ptr, layout);

    EXPECT_EQ(constTensor.Tensor().Data(), ptr);
    EXPECT_EQ(constTensor.Engine().Begin(), ptr);
    EXPECT_EQ(constTensor.Layout().Size(), 6);
    EXPECT_EQ(constTensor.Data(), ptr);

    auto shape = constTensor.Shape();
    EXPECT_EQ(AscendC::Std::get<0>(shape), 2);
    EXPECT_EQ(AscendC::Std::get<1>(shape), 3);

    auto stride = constTensor.Stride();
    EXPECT_EQ(AscendC::Std::get<0>(stride), 3);
    EXPECT_EQ(AscendC::Std::get<1>(stride), 1);

    EXPECT_EQ(constTensor.Size(), 6);
    EXPECT_EQ(constTensor.Capacity(), 6);

    auto coord = MakeCoord(1, 2);
    EXPECT_EQ(constTensor[coord], 5);
    EXPECT_EQ(constTensor(coord), 5);
    EXPECT_EQ(constTensor(1, 2), 5);
}
