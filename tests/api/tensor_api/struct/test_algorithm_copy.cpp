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

class Tensor_Api_Algorithm_Copy : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Algorithm_Copy, CopyWithDefaultTrait)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float srcData[TILE_LENGTH] = {0};
    __cbuf__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    auto copyAtom = MakeCopy(DataCopyOperation{});
    Copy(copyAtom, dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Algorithm_Copy, CopyWithCustomTrait)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float srcData[TILE_LENGTH] = {0};
    __cbuf__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    auto copyAtom = MakeCopy(DataCopyOperation{}, DataCopyTrait{});
    Copy(copyAtom, dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Algorithm_Copy, CopyWithParams)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float srcData[TILE_LENGTH] = {0};
    __cbuf__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    auto copyAtom = MakeCopy(DataCopyOperation{});
    Copy(copyAtom, dstTensor, srcTensor, 0, 0);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Algorithm_Copy, MakeCopyOperation)
{
    using namespace AscendC::Te;

    auto copyAtom = MakeCopy(DataCopyOperation{});
    auto copyAtomWithTrait = MakeCopy(DataCopyOperation{}, DataCopyTrait{});

    EXPECT_EQ(&copyAtom, &copyAtom);
    EXPECT_EQ(&copyAtomWithTrait, &copyAtomWithTrait);
}

TEST_F(Tensor_Api_Algorithm_Copy, CopyWithCoord)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float srcData[TILE_LENGTH] = {0};
    __cbuf__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    auto coord = MakeCoord(0, 0);
    auto copyAtom = MakeCopy(DataCopyOperation{});
    Copy(copyAtom, dstTensor, srcTensor, coord);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Algorithm_Copy, CopyWithIntCoord)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __gm__ float srcData[TILE_LENGTH] = {0};
    __cbuf__ float dstData[TILE_LENGTH] = {0};

    auto srcPtr = MakeGMmemPtr(srcData);
    auto dstPtr = MakeL1memPtr(dstData);

    auto srcLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));

    auto srcTensor = MakeTensor(srcPtr, srcLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
    auto copyAtom = MakeCopy(DataCopyOperation{});
    Copy(copyAtom, dstTensor, srcTensor, coord);

    EXPECT_EQ(dstData[0], 0);
}
