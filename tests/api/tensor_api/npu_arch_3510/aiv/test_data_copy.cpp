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

class Tensor_Api_Aiv_Data_Copy : public testing::Test {
protected:
    void SetUp() { AscendC::SetGCoreType(0); }

    void TearDown() { AscendC::SetGCoreType(0); }
};

TEST_F(Tensor_Api_Aiv_Data_Copy, DataCopyGmToL1_Nd2Nd)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float srcData[TILE_LENGTH] = {0};
    __cbuf__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeNdLayout<float>(16, 8);
    auto dstLayout = MakeNdLayout<float>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Data_Copy, DataCopyGmToL1_Nz2Nz)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float srcData[TILE_LENGTH] = {0};
    __cbuf__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeNzLayout<float>(16, 8);
    auto dstLayout = MakeNzLayout<float>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Data_Copy, DataCopyL1ToBias_Nd2Nd)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ float srcData[TILE_LENGTH] = {0};
    __biasbuf__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL1memPtr(srcData);
    auto dstPtr = MakeBiasmemPtr(dstData);

    auto srcLayout = MakeNdLayout<float>(1, 64);
    auto dstLayout = MakeNdLayout<float>(1, 64);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Data_Copy, DataCopyL1ToFixbuf_Nd2Nd)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __cbuf__ uint64_t srcData[TILE_LENGTH] = {0};
    __fbuf__ uint64_t dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeL1memPtr(srcData);
    auto dstPtr = MakeFixbufmemPtr(dstData);

    auto srcLayout = MakeNdLayout<uint64_t>(1, 64);
    auto dstLayout = MakeNdLayout<uint64_t>(1, 64);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Data_Copy, DataCopyWithCoord)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float srcData[2 * TILE_LENGTH] = {0};
    __cbuf__ float dstData[2 * TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeNdLayout<float>(32, 8);
    auto dstLayout = MakeNdLayout<float>(32, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    auto coord = MakeCoord(0, 0);
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(dstTensor, srcTensor, coord);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Data_Copy, DataCopyWithIntCoord)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float srcData[2 * TILE_LENGTH] = {0};
    __cbuf__ float dstData[2 * TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeNdLayout<float>(32, 8);
    auto dstLayout = MakeNdLayout<float>(32, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(dstTensor, srcTensor, coord);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Data_Copy, DataCopyWithHalfType)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ half srcData[TILE_LENGTH] = {0};
    __cbuf__ half dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeNzLayout<half>(16, 8);
    auto dstLayout = MakeNzLayout<half>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Data_Copy, DataCopyWithInt32Type)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ int32_t srcData[TILE_LENGTH] = {0};
    __cbuf__ int32_t dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeNzLayout<int32_t>(16, 8);
    auto dstLayout = MakeNzLayout<int32_t>(16, 8);

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}
