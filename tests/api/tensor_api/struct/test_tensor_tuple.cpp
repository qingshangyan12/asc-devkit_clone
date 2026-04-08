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

class Tensor_Api_Tuple : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Tuple, MakeTuple)
{
    using namespace AscendC::Te;

    auto tuple2D = MakeTuple(1, 2);
    auto tuple3D = MakeTuple(1, 2, 3);
    auto tuple4D = MakeTuple(1, 2, 3, 4);

    EXPECT_TRUE(Std::tuple_size_v<decltype(tuple2D)> == 2);
    EXPECT_TRUE(Std::tuple_size_v<decltype(tuple3D)> == 3);
    EXPECT_TRUE(Std::tuple_size_v<decltype(tuple4D)> == 4);
}

TEST_F(Tensor_Api_Tuple, TupleGet)
{
    using namespace AscendC::Te;

    auto tuple = MakeTuple(1, 2, 3, 4);

    EXPECT_EQ(Std::get<0>(tuple), 1);
    EXPECT_EQ(Std::get<1>(tuple), 2);
    EXPECT_EQ(Std::get<2>(tuple), 3);
    EXPECT_EQ(Std::get<3>(tuple), 4);
}

TEST_F(Tensor_Api_Tuple, TupleWithInt)
{
    using namespace AscendC::Te;

    auto tuple = MakeTuple(AscendC::Std::Int<1>{}, AscendC::Std::Int<2>{}, AscendC::Std::Int<3>{});

    EXPECT_TRUE(Std::tuple_size_v<decltype(tuple)> == 3);
    EXPECT_EQ(Std::get<0>(tuple).value, 1);
    EXPECT_EQ(Std::get<1>(tuple).value, 2);
    EXPECT_EQ(Std::get<2>(tuple).value, 3);
}

TEST_F(Tensor_Api_Tuple, TupleComparison)
{
    using namespace AscendC::Te;

    auto tuple1 = MakeTuple(1, 2, 3);
    auto tuple2 = MakeTuple(1, 2, 3);
    auto tuple3 = MakeTuple(1, 2, 4);

    EXPECT_TRUE(tuple1 == tuple2);
    EXPECT_TRUE(tuple1 != tuple3);
}

TEST_F(Tensor_Api_Tuple, TupleCopy)
{
    using namespace AscendC::Te;

    auto tuple1 = MakeTuple(1, 2, 3);
    auto tuple2 = tuple1;

    EXPECT_TRUE(tuple1 == tuple2);
}

TEST_F(Tensor_Api_Tuple, TupleAssign)
{
    using namespace AscendC::Te;

    auto tuple1 = MakeTuple(1, 2, 3);
    auto tuple2 = MakeTuple(4, 5, 6);

    tuple2 = tuple1;

    EXPECT_TRUE(tuple1 == tuple2);
}

TEST_F(Tensor_Api_Tuple, TupleWithDifferentTypes)
{
    using namespace AscendC::Te;

    auto tuple = MakeTuple(1, 2.0f, 3.0);

    EXPECT_TRUE(Std::tuple_size_v<decltype(tuple)> == 3);
    EXPECT_EQ(Std::get<0>(tuple), 1);
    EXPECT_FLOAT_EQ(Std::get<1>(tuple), 2.0f);
    EXPECT_FLOAT_EQ(Std::get<2>(tuple), 3.0);
}
