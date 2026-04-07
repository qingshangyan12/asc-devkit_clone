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

class Tensor_Api_Aiv_Fixpipe : public testing::Test {
protected:
    void SetUp() { AscendC::SetGCoreType(0); }

    void TearDown() { AscendC::SetGCoreType(0); }
};

TEST_F(Tensor_Api_Aiv_Fixpipe, FixpipeL0c2outNz2Nz)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cc__ float srcData[TILE_LENGTH] = {0};
    __gm__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL0CmemPtr(srcData);
    auto dstPtr = MakeGMmemPtr(dstData);

    auto srcLayout = MakeNzLayout<float>(16, 8);
    auto dstLayout = MakeNzLayout<float>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    FixpipeParams params;
    params.nSize = 16;
    params.mSize = 8;
    params.srcStride = 16;
    params.dstStride = 16;
    params.cacheMode = 0;
    params.reluEn = false;
    params.unitFlag = 0;
    params.isChannelSplit = false;
    params.nz2ndEn = false;
    params.nz2dnEn = false;

    FixpipeL0c2out(dstTensor, srcTensor, params);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Fixpipe, FixpipeL0c2outNz2Nd)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cc__ float srcData[TILE_LENGTH] = {0};
    __gm__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL0CmemPtr(srcData);
    auto dstPtr = MakeGMmemPtr(dstData);

    auto srcLayout = MakeNzLayout<float>(16, 8);
    auto dstLayout = MakeNdLayout<float>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    FixpipeParams params;
    params.nSize = 16;
    params.mSize = 8;
    params.srcStride = 16;
    params.dstStride = 16;
    params.cacheMode = 0;
    params.reluEn = false;
    params.unitFlag = 0;
    params.isChannelSplit = false;
    params.nz2ndEn = true;
    params.nz2dnEn = false;

    FixpipeL0c2out(dstTensor, srcTensor, params);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Fixpipe, FixpipeL0c2outNz2Dn)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cc__ float srcData[TILE_LENGTH] = {0};
    __gm__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL0CmemPtr(srcData);
    auto dstPtr = MakeGMmemPtr(dstData);

    auto srcLayout = MakeNzLayout<float>(16, 8);
    auto dstLayout = MakeNdLayout<float>(8, 16);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    FixpipeParams params;
    params.nSize = 16;
    params.mSize = 8;
    params.srcStride = 16;
    params.dstStride = 8;
    params.cacheMode = 0;
    params.reluEn = false;
    params.unitFlag = 0;
    params.isChannelSplit = false;
    params.nz2ndEn = false;
    params.nz2dnEn = true;

    FixpipeL0c2out(dstTensor, srcTensor, params);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Fixpipe, FixpipeL0c2outWithRelu)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cc__ float srcData[TILE_LENGTH] = {0};
    __gm__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL0CmemPtr(srcData);
    auto dstPtr = MakeGMmemPtr(dstData);

    auto srcLayout = MakeNzLayout<float>(16, 8);
    auto dstLayout = MakeNzLayout<float>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    FixpipeParams params;
    params.nSize = 16;
    params.mSize = 8;
    params.srcStride = 16;
    params.dstStride = 16;
    params.cacheMode = 0;
    params.reluEn = true;
    params.unitFlag = 0;
    params.isChannelSplit = false;
    params.nz2ndEn = false;
    params.nz2dnEn = false;

    FixpipeL0c2out(dstTensor, srcTensor, params);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Fixpipe, FixpipeL0c2outWithHalfType)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cc__ half srcData[TILE_LENGTH] = {0};
    __gm__ half dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL0CmemPtr(srcData);
    auto dstPtr = MakeGMmemPtr(dstData);

    auto srcLayout = MakeNzLayout<half>(16, 8);
    auto dstLayout = MakeNzLayout<half>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    FixpipeParams params;
    params.nSize = 16;
    params.mSize = 8;
    params.srcStride = 16;
    params.dstStride = 16;
    params.cacheMode = 0;
    params.reluEn = false;
    params.unitFlag = 0;
    params.isChannelSplit = false;
    params.nz2ndEn = false;
    params.nz2dnEn = false;

    FixpipeL0c2out(dstTensor, srcTensor, params);

    EXPECT_EQ(dstData[0], 0);
}
