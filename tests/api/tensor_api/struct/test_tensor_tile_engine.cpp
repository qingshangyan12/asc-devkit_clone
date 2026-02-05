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
#include "include/experimental/tensor_api/tensor/engine.h"

class Tensor_Api_Engine : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

// u8 f8 u32 bool float
TEST_F(Tensor_Api_Engine, ViewEngineOperation)
{
    using namespace AscendC;
    uint8_t aGM[16] = {0};

    auto gmA = reinterpret_cast<__gm__ A_T *>(aGM);
    auto gmIterator = MakeGMmemPtr(gmA);
    ViewEngine engineInit = ViewEngine<gmIterator>;

    EXPECT_EQ(viewEngine.Begin(), gmIterator);
}

TEST_F(Tensor_Api_Engine, ConstViewEngineBegin)
{
    using namespace AscendC;
    uint8_t aGM[M * K * sizeof(A_DType)] = {0};
    auto gmA = reinterpret_cast<__gm__ A_T *>(aGM);
    auto gmIterator = MakeGMmemPtr(gmA);
    ConstViewEngine constViewEngine = ConstViewEngine<gmIterator>;

    EXPECT_EQ(constViewEngine.Begin(), 0);
}