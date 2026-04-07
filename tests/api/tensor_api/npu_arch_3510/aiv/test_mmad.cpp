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

class Tensor_Api_Aiv_Mmad : public testing::Test {
protected:
    void SetUp() { AscendC::SetGCoreType(0); }

    void TearDown() { AscendC::SetGCoreType(0); }
};

TEST_F(Tensor_Api_Aiv_Mmad, MmadFloatHalf)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ half fmData[TILE_LENGTH] = {0};
    __cb__ half filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeNzLayout<half>(16, 16);
    auto filterLayout = MakeZnLayout<half>(16, 16);
    auto dstLayout = MakeNzLayout<AscendC::Std::ignore_t>(16, 16);

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadParams params;
    params.m = 16;
    params.n = 16;
    params.k = 16;
    params.unitFlag = 0;
    params.cmatrixInitVal = true;

    Mmad(dstTensor, fmTensor, filterTensor, params);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Mmad, MmadFloatFloat)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ float fmData[TILE_LENGTH] = {0};
    __cb__ float filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeNzLayout<float>(16, 16);
    auto filterLayout = MakeZnLayout<float>(16, 16);
    auto dstLayout = MakeNzLayout<AscendC::Std::ignore_t>(16, 16);

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadParams params;
    params.m = 16;
    params.n = 16;
    params.k = 16;
    params.unitFlag = 0;
    params.cmatrixInitVal = true;

    Mmad(dstTensor, fmTensor, filterTensor, params);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Mmad, MmadFloatBfloat16)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ bfloat16_t fmData[TILE_LENGTH] = {0};
    __cb__ bfloat16_t filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeNzLayout<bfloat16_t>(16, 16);
    auto filterLayout = MakeZnLayout<bfloat16_t>(16, 16);
    auto dstLayout = MakeNzLayout<AscendC::Std::ignore_t>(16, 16);

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadParams params;
    params.m = 16;
    params.n = 16;
    params.k = 16;
    params.unitFlag = 0;
    params.cmatrixInitVal = true;

    Mmad(dstTensor, fmTensor, filterTensor, params);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Mmad, MmadWithDifferentSizes)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 256;
    __ca__ float fmData[TILE_LENGTH] = {0};
    __cb__ float filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeNzLayout<float>(32, 16);
    auto filterLayout = MakeZnLayout<float>(16, 32);
    auto dstLayout = MakeNzLayout<AscendC::Std::ignore_t>(32, 32);

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadParams params;
    params.m = 32;
    params.n = 32;
    params.k = 16;
    params.unitFlag = 0;
    params.cmatrixInitVal = true;

    Mmad(dstTensor, fmTensor, filterTensor, params);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Mmad, MmadWithCustomParams)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ float fmData[TILE_LENGTH] = {0};
    __cb__ float filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeNzLayout<float>(16, 16);
    auto filterLayout = MakeZnLayout<float>(16, 16);
    auto dstLayout = MakeNzLayout<AscendC::Std::ignore_t>(16, 16);

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadParams params;
    params.m = 16;
    params.n = 16;
    params.k = 16;
    params.unitFlag = 1;
    params.cmatrixInitVal = false;

    Mmad(dstTensor, fmTensor, filterTensor, params);

    EXPECT_EQ(dstData[0], 0);
}
