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

class Tensor_Api_Pointer_Mem : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Pointer_Mem, GMmemPtrOperations)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    EXPECT_EQ(ptr.Get(), data);
    EXPECT_EQ(ptr[0], 0);
    EXPECT_EQ(ptr[TILE_LENGTH - 1], 0);

    ptr[0] = 1.0f;
    EXPECT_EQ(data[0], 1.0f);
}

TEST_F(Tensor_Api_Pointer_Mem, L1memPtrOperations)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeL1memPtr(data);

    EXPECT_EQ(ptr.Get(), data);
    EXPECT_EQ(ptr[0], 0);
    EXPECT_EQ(ptr[TILE_LENGTH - 1], 0);

    ptr[0] = 1.0f;
    EXPECT_EQ(data[0], 1.0f);
}

TEST_F(Tensor_Api_Pointer_Mem, L0memPtrOperations)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ float dataA[TILE_LENGTH] = {0};
    __cb__ float dataB[TILE_LENGTH] = {0};
    __cc__ float dataC[TILE_LENGTH] = {0};

    auto ptrA = MakeL0AmemPtr(dataA);
    auto ptrB = MakeL0BmemPtr(dataB);
    auto ptrC = MakeL0CmemPtr(dataC);

    EXPECT_EQ(ptrA.Get(), dataA);
    EXPECT_EQ(ptrB.Get(), dataB);
    EXPECT_EQ(ptrC.Get(), dataC);

    ptrA[0] = 1.0f;
    ptrB[0] = 2.0f;
    ptrC[0] = 3.0f;

    EXPECT_EQ(dataA[0], 1.0f);
    EXPECT_EQ(dataB[0], 2.0f);
    EXPECT_EQ(dataC[0], 3.0f);
}

TEST_F(Tensor_Api_Pointer_Mem, BiasmemPtrOperations)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __biasbuf__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeBiasmemPtr(data);

    EXPECT_EQ(ptr.Get(), data);
    EXPECT_EQ(ptr[0], 0);
    EXPECT_EQ(ptr[TILE_LENGTH - 1], 0);

    ptr[0] = 1.0f;
    EXPECT_EQ(data[0], 1.0f);
}

TEST_F(Tensor_Api_Pointer_Mem, FixbufmemPtrOperations)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __fbuf__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeFixbufmemPtr(data);

    EXPECT_EQ(ptr.Get(), data);
    EXPECT_EQ(ptr[0], 0);
    EXPECT_EQ(ptr[TILE_LENGTH - 1], 0);

    ptr[0] = 1.0f;
    EXPECT_EQ(data[0], 1.0f);
}

TEST_F(Tensor_Api_Pointer_Mem, PointerArithmetic)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto ptr1 = ptr + 1;
    auto ptr2 = ptr + 10;
    auto ptr3 = ptr - 1;

    EXPECT_EQ(ptr1.Get(), data + 1);
    EXPECT_EQ(ptr2.Get(), data + 10);
    EXPECT_EQ(ptr3.Get(), data - 1);
}

TEST_F(Tensor_Api_Pointer_Mem, PointerWithDifferentTypes)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ half halfData[TILE_LENGTH] = {0};
    __gm__ int32_t intData[TILE_LENGTH] = {0};
    __gm__ uint8_t byteData[TILE_LENGTH] = {0};

    auto halfPtr = MakeGMmemPtr(halfData);
    auto intPtr = MakeGMmemPtr(intData);
    auto bytePtr = MakeGMmemPtr(byteData);

    EXPECT_EQ(halfPtr.Get(), halfData);
    EXPECT_EQ(intPtr.Get(), intData);
    EXPECT_EQ(bytePtr.Get(), byteData);
}
