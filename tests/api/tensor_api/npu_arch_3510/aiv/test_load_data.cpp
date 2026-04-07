/**
 * Copyright (c) 202. Huawei Technologies Co., Ltd.
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

class Tensor_Api_Aiv_Load_Data : public testing::Test {
protected:
    void SetUp() { AscendC::SetGCoreType(1); }

    void TearDown() { AscendC::SetGCoreType(0); }
};

TEST_F(Tensor_Api_Aiv_Load_Data, LoadDataL1ToL0A_Nz2Nz)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ float srcData[TILE_LENGTH] = {0};
    __ca__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL1memPtr(srcData);
    auto dstPtr = MakeL0AmemPtr(dstData);

    auto srcLayout = MakeNzLayout<float>(16, 8);
    auto dstLayout = MakeNzLayout<float>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    LoadData<LoadDataTrait{false}>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Load_Data, LoadDataL1ToL0A_Zn2Nz)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ float srcData[TILE_LENGTH] = {0};
    __ca__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL1memPtr(srcData);
    auto dstPtr = MakeL0AmemPtr(dstData);

    auto srcLayout = MakeZnLayout<float>(8, 16);
);
auto dstLayout = MakeNzLayout<float>(16, 8);

auto srcTensor = MakeTensor(srcPtr, srcLayout);
auto dstTensor = MakeTensor(dstPtr, dstLayout);

LoadData<LoadDataTrait{true}>(dstTensor, srcTensor);

EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Load_Data, LoadDataL1ToL0B_Zn2Zn)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ float srcData[TILE_LENGTH] = {0};
    __cb__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL1memPtr(srcData);
    auto dstPtr = MakeL0BmemPtr(dstData);

    auto srcLayout = MakeZnLayout<float>(8, 16);
    auto dstLayout = MakeZnLayout<float>(8, 16);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    LoadData<LoadDataTrait{false}>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Load_Data, LoadDataL1ToL0B_Nz2Zn)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ float srcData[TILE_LENGTH] = {0};
    __cb__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL1memPtr(srcData);
    auto dstPtr = MakeL0BmemPtr(dstData);

    auto srcLayout = MakeNzLayout<float>(16, 8);
    auto dstLayout = MakeZnLayout<float>(8, 16);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    LoadData<LoadDataTrait{true}>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Load_Data, LoadDataWithCoord)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ float srcData[TILE_LENGTH] = {0};
    __ca__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL1memPtr(srcData);
    auto dstPtr = MakeL0AmemPtr(dstData);

    auto srcLayout = MakeNzLayout<float>(16, 8);
    auto dstLayout = MakeNzLayout<float>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    auto coord = MakeCoord(0, 0);
    LoadData<LoadDataTrait{false}>(dstTensor, srcTensor, coord);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Load_Data, LoadDataWithIntCoord)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ float srcData[TILE_LENGTH] = {0};
    __ca__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL1memPtr(srcData);
    auto dstPtr = MakeL0AmemPtr(dstData);

    auto srcLayout = MakeNzLayout<float>(16, 8);
    auto dstLayout = MakeNzLayout<float>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
    LoadData<LoadDataTrait{false}>(dstTensor, srcTensor, coord);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Load_Data, LoadDataWithHalfType)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ half srcData[TILE_LENGTH] = {0};
    __ca__ half dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL1memPtr(srcData);
    auto dstPtr = MakeL0AmemPtr(dstData);

    auto srcLayout = MakeNzLayout<half>(16, 8);
    auto dstLayout = MakeNzLayout<half>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    LoadData<LoadDataTrait{false}>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Load_Data, LoadDataWithInt32Type)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ int32_t srcData[TILE_LENGTH] = {0};
    __ca__ int32_t dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL1memPtr(srcData);
    auto dstPtr = MakeL0AmemPtr(dstData);

    auto srcLayout = MakeNzLayout<int32_t>(16, 8);
    auto dstLayout = MakeNzLayout<int32_t>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    LoadData<LoadDataTrait{false}>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}
