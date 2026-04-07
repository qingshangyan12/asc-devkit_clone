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

class Tensor_Api_Aiv_Tensor_Tile_Atom : public testing::Test {
protected:
    void SetUp() { AscendC::SetGCoreType(0); }

    void TearDown() { AscendC::SetGCoreType(0); }
};

TEST_F(Tensor_Api_Aiv_Tensor_Tile_Atom, CopyAtomL1ToL0A)
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

    CopyAtom<DataCopyOperation> copyAtom;
    copyAtom.Call(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Tensor_Tile_Atom, CopyAtomL1ToL0B)
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

    CopyAtom<DataCopyOperation> copyAtom;
    copyAtom.Call(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Tensor_Tile_Atom, MadAtomL0C)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ float fmData[TILE_LENGTH] = {0};
    __cb__ float filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeNzLayout<float>(16, 8);
);
auto filterLayout = MakeZnLayout<float>(8, 16);
auto dstLayout = MakeNzLayout<AscendC::Std::ignore_t>(16, 16);

auto fmTensor = MakeTensor(fmPtr, fmLayout);
auto filterTensor = MakeTensor(filterPtr, filterLayout);
auto dstTensor = MakeTensor(dstPtr, dstLayout);

MmadAtom<MmadOperation> madAtom;
madAtom.Call(dstTensor, fmTensor, filterTensor, defaultMmadParams);

EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Tensor_Tile_Atom, CopyAtomWithCoord)
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
    CopyAtom<DataCopyOperation> copyAtom;
    copyAtom.Call(dstTensor, srcTensor, coord);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Tensor_Tile_Atom, MadAtomWithBias)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ float fmData[TILE_LENGTH] = {0};
    __cb__ float filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};
    __biasbuf__ float biasData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);
    auto biasPtr = MakeBiasmemPtr(biasData);

    auto fmLayout = MakeNzLayout<float>(16, 8);
    auto filterLayout = MakeZnLayout<float>(8, 16);
    auto dstLayout = MakeNzLayout<AscendC::Std::ignore_t>(16, 16);
    auto biasLayout = MakeLayout(MakeShape(1, 16), MakeStride(0, 1));

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);
    auto biasTensor = MakeTensor(biasPtr, biasLayout);

    MmadAtom<MmadOperation> madAtom;
    madAtom.Call(dstTensor, fmTensor, filterTensor, biasTensor, defaultMmadWithBiasParams);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Tensor_Tile_Atom, CopyAtomWithHalfType)
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

    CopyAtom<DataCopyOperation> copyAtom;
    copyAtom.Call(dstTensor, srcTensor);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Aiv_Tensor_Tile_Atom, MadAtomWithHalfType)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ half fmData[TILE_LENGTH] = {0};
    __cb__ half filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeNzLayout<half>(16, 8);
    auto filterLayout = MakeZnLayout<half>(8, 16);
    auto dstLayout = MakeNzLayout<AscendC::Std::ignore_t>(16, 16);

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadAtom<MmadOperation> madAtom;
    madAtom.Call(dstTensor, fmTensor, filterTensor, defaultMmadParams);

    EXPECT_EQ(dstData[0], 0);
}
