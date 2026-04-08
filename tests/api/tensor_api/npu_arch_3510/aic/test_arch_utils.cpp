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

class Tensor_Api_Arch_Utils : public testing::Test {
protected:
    void SetUp() { AscendC::SetGCoreType(1); }

    void TearDown() { AscendC::SetGCoreType(0); }
};

TEST_F(Tensor_Api_Arch_Utils, CheckFormatArch3510)
{
    using namespace AscendC::Te;

    EXPECT_TRUE(CheckFormatArch3510<float>());
    EXPECT_TRUE(CheckFormatArch3510<half>());
    EXPECT_TRUE(CheckFormatArch3510<int32_t>());
}

TEST_F(Tensor_Api_Arch_Utils, CheckFormatArch3510WithLayout)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeNzLayout<float>(16, 8);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_TRUE(CheckFormatArch3510(tensor.Layout()));
}

TEST_F(Tensor_Api_Arch_Utils, IsFormatNz)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeNzLayout<float>(16, 8);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_TRUE(IsFormatNz(tensor.Layout()));
}

TEST_F(Tensor_Api_Arch_Utils, IsFormatZn)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeZnLayout<float>(8, 16);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_TRUE(IsFormatZn(tensor.Layout()));
}

TEST_F(Tensor_Api_Arch_Utils, IsFormatNd)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeNdLayout<float>(16, 8);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_TRUE(IsFormatNd(tensor.Layout()));
}

TEST_F(Tensor_Api_Arch_Utils, IsFormatZz)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeZzLayout<float>(8, 8);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_TRUE(IsFormatZz(tensor.Layout()));
}

TEST_F(Tensor_Api_Arch_Utils, IsFormatNn)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeNnLayout<float>(16, 8);
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_TRUE(IsFormatNn(tensor.Layout()));
}

TEST_F(Tensor_Api_Arch_Utils, IsFormatDn)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float data[TILE_LENGTH] = {0};
    auto ptr = MakeGMmemPtr(data);
    auto layout = MakeLayout(MakeShape(8, 16), MakeStride(16, 1));
    auto tensor = MakeTensor(ptr, layout);

    EXPECT_TRUE(IsFormatDn(tensor.Layout()));
}
