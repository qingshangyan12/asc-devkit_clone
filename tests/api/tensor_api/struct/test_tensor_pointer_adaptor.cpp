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

class Tensor_Api_Pointer_Adaptor : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Pointer_Adaptor, MakeGMmemPtr)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    EXPECT_EQ(ptr.Get(), data);
}

TEST_F(Tensor_Api_Pointer_Adaptor, MakeL1memPtr)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeL1memPtr(data);

    EXPECT_EQ(ptr.Get(), data);
}

TEST_F(Tensor_Api_Pointer_Adaptor, MakeL0AmemPtr)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeL0AmemPtr(data);

    EXPECT_EQ(ptr.Get(), data);
}

TEST_F(Tensor_Api_Pointer_Adaptor, MakeL0BmemPtr)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cb__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeL0BmemPtr(data);

    EXPECT_EQ(ptr.Get(), data);
}

TEST_F(Tensor_Api_Pointer_Adaptor, MakeL0CmemPtr)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cc__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeL0CmemPtr(data);

    EXPECT_EQ(ptr.Get(), data);
}

TEST_F(Tensor_Api_Pointer_Adaptor, MakeBiasmemPtr)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __biasbuf__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeBiasmemPtr(data);

    EXPECT_EQ(ptr.Get(), data);
}

TEST_F(Tensor_Api_Pointer_Adaptor, MakeFixbufmemPtr)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __fbuf__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeFixbufmemPtr(data);

    EXPECT_EQ(ptr.Get(), data);
}

TEST_F(Tensor_Api_Pointer_Adaptor, PointerArithmetic)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);

    auto next = ptr + 1;
    EXPECT_EQ(next.Get(), data + 1);

    auto prev = ptr - 1;
    EXPECT_EQ(prev.Get(), data - 1);
}

TEST_F(Tensor_Api_Pointer_Adaptor, PointerDereference)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {1, 2, 3, 4};
    auto ptr = MakeGMmemPtr(data);

    EXPECT_EQ(*ptr, 1);
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
    EXPECT_EQ(ptr[3], 4);
}

TEST_F(Tensor_Api_Pointer_Adaptor, PointerComparison)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto next = ptr + 1;

    EXPECT_TRUE(ptr == ptr);
    EXPECT_TRUE(ptr != next);
    EXPECT_TRUE(ptr < next);
    EXPECT_TRUE(next > ptr);
}
