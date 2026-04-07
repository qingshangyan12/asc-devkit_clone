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

class Tensor_Api_Mad_Atom : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Mad_Atom, MadAtomDefaultTrait)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ float fmData[TILE_LENGTH] = {0};
    __cb__ float filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto filterLayout = MakeLayout(MakeShape(8, 16), MakeStride(16, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 16), MakeStride(16, 1));

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadAtom<MmadOperation> madAtom;
    madAtom.Call(dstTensor, fmTensor, filterTensor, defaultMmadmadParams);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Mad_Atom, MadAtomWithTraits)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ float fmData[TILE_LENGTH] = {0};
    __cb__ float filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto filterLayout = MakeLayout(MakeShape(8, 16), MakeStride(16, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 16), MakeStride(16, 1));

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>> madAtom;
    madAtom.Call(dstTensor, fmTensor, filterTensor, defaultMmadmadParams);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Mad_Atom, MadAtomWithBias)
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

    auto fmLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto filterLayout = MakeLayout(MakeShape(8, 16), MakeStride(16, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 16), MakeStride(16, 1));
    auto biasLayout = MakeLayout(MakeShape(1, 16), MakeStride(0, 1));

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);
    auto biasTensor = MakeTensor(biasPtr, biasLayout);

    MmadAtom<MmadOperation> madAtom;
    madAtom.Call(dstTensor, fmTensor, filterTensor, biasTensor, defaultMmadWithBiasParams);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Mad_Atom, MadAtomWithCustomParams)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ float fmData[TILE_LENGTH] = {0};
    __cb__ float filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto filterLayout = MakeLayout(MakeShape(8, 16), MakeStride(16, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 16), MakeStride(16, 1));

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadParams params;
    params.m = 16;
    params.n = 16;
    params.k = 8;
    params.unitFlag = 0;
    params.cmatrixInitVal = true;

    MmadAtom<MmadOperation> madAtom;
    madAtom.Call(dstTensor, fmTensor, filterTensor, params);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Mad_Atom, MadAtomWithMethod)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ float fmData[TILE_LENGTH] = {0};
    __cb__ float filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto filterLayout = MakeLayout(MakeShape(8, 16), MakeStride(16, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 16), MakeStride(16, 1));

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadAtom<MmadOperation> madAtom;
    auto newMadAtom = madAtom.with(MmadTraitDefault{});
    newMadAtom.Call(dstTensor, fmTensor, filterTensor, defaultMmadmadParams);

    EXPECT_EQ(dstData[0], 0);
}

TEST_F(Tensor_Api_Mad_Atom, MadAtomWithHalfType)
{
    using namespace AscendC::Te;

    constexpr uint32_t TILE_LENGTH = 128;
    __ca__ half fmData[TILE_LENGTH] = {0};
    __cb__ half filterData[TILE_LENGTH] = {0};
    __cc__ float dstData[TILE_LENGTH] = {0};

    auto fmPtr = MakeL0AmemPtr(fmData);
    auto filterPtr = MakeL0BmemPtr(filterData);
    auto dstPtr = MakeL0CmemPtr(dstData);

    auto fmLayout = MakeLayout(MakeShape(16, 8), MakeStride(8, 1));
    auto filterLayout = MakeLayout(MakeShape(8, 16), MakeStride(16, 1));
    auto dstLayout = MakeLayout(MakeShape(16, 16), MakeStride(16, 1));

    auto fmTensor = MakeTensor(fmPtr, fmLayout);
    auto filterTensor = MakeTensor(filterPtr, filterLayout);
    auto dstTensor = MakeTensor(dstPtr, dstLayout);

    MmadAtom<MmadOperation> madAtom;
    madAtom.Call(dstTensor, fmTensor, filterTensor, defaultMmadmadParams);

    EXPECT_EQ(dstData[0], 0);
}
