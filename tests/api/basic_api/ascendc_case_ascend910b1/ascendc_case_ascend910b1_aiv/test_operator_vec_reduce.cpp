/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <gtest/gtest.h>
#include "kernel_operator.h"
#include "mockcpp/mockcpp.hpp"
using namespace std;
using namespace AscendC;

template <typename T>
void MainVecReduce(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm,
    __gm__ int32_t srcDataSize, __gm__ int32_t dstDataSize)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ PrimT<T>*>(srcGm), srcDataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ PrimT<T>*>(dstGm), dstDataSize);

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, srcDataSize * sizeof(PrimT<T>));
    LocalTensor<T> inputLocal = tbuf1.Get<T>();

    TBuf<TPosition::VECCALC> tbuf2;
    tpipe.InitBuffer(tbuf2, dstDataSize * sizeof(PrimT<T>));
    LocalTensor<T> outputLocal = tbuf2.Get<T>();

    AscendCUtils::SetMask<uint8_t>(256);
    DataCopy(inputLocal, inputGlobal, srcDataSize);

    AscendCUtils::SetMask<uint8_t>(256);
    event_t eventIdMte2ToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_V));
    SetFlag<HardEvent::MTE2_V>(eventIdMte2ToV);
    WaitFlag<HardEvent::MTE2_V>(eventIdMte2ToV);

    BlockReduceMax<T>(outputLocal, inputLocal, 1, 64, 8, 1, 8);
    BlockReduceMin<T>(outputLocal, inputLocal, 1, 64, 8, 1, 8);
    BlockReduceSum<T>(outputLocal, inputLocal, 1, 64, 8, 1, 8);
    PairReduceSum<T>(outputLocal, inputLocal, 1, 64, 8, 1, 8);

    SetMaskCount();
    SetVectorMask<PrimT<T>, MaskMode::COUNTER>(0, 64);
    BlockReduceMax<T, false>(outputLocal, inputLocal, 1, 64, 8, 1, 8);
    BlockReduceMin<T, false>(outputLocal, inputLocal, 1, 64, 8, 1, 8);
    BlockReduceSum<T, false>(outputLocal, inputLocal, 1, 64, 8, 1, 8);
    PairReduceSum<T, false>(outputLocal, inputLocal, 1, 64, 8, 1, 8);
    SetMaskNorm();
    AscendCUtils::ResetMask();
    event_t eventIdVToMte3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
    SetFlag<HardEvent::V_MTE3>(eventIdVToMte3);
    WaitFlag<HardEvent::V_MTE3>(eventIdVToMte3);

    DataCopy(outputGlobal, outputLocal, dstDataSize);

    PipeBarrier<PIPE_ALL>();
}

class TEST_VEC_REDUCE : public testing::Test {
protected:
    void SetUp()
    {
        AscendC::SetGCoreType(2);
    }

    void TearDown()
    {
        AscendC::SetGCoreType(0);
        GlobalMockObject::verify();
    }
};

TEST_F(TEST_VEC_REDUCE, OperatorVecReduceFloatCase)
{
    const int32_t srcDataSize = 128;
    const int32_t dstDataSize = 64;

    uint8_t srcGm[srcDataSize * sizeof(float)];
    uint8_t dstGm[dstDataSize * sizeof(float)];

    MainVecReduce<float>(srcGm, dstGm, srcDataSize, dstDataSize);

    for (int32_t i = 0; i < dstDataSize; i++) {
        EXPECT_EQ(dstGm[i], 0x00);
    }
}

TEST_F(TEST_VEC_REDUCE, OperatorVecReduceHalfCase)
{
    const int32_t srcDataSize = 128;
    const int32_t dstDataSize = 64;

    uint8_t srcGm[srcDataSize * sizeof(half)];
    uint8_t dstGm[dstDataSize * sizeof(half)];

    MainVecReduce<half>(srcGm, dstGm, srcDataSize, dstDataSize);

    for (int32_t i = 0; i < dstDataSize; i++) {
        EXPECT_EQ(dstGm[i], 0x00);
    }
}

TEST_F(TEST_VEC_REDUCE, OperatorVecReduceTensorTraitFloatCase)
{
    const int32_t srcDataSize = 128;
    const int32_t dstDataSize = 64;

    uint8_t srcGm[srcDataSize * sizeof(float)];
    uint8_t dstGm[dstDataSize * sizeof(float)];

    MainVecReduce<TensorTrait<float>>(srcGm, dstGm, srcDataSize, dstDataSize);

    for (int32_t i = 0; i < dstDataSize; i++) {
        EXPECT_EQ(dstGm[i], 0x00);
    }
}

TEST_F(TEST_VEC_REDUCE, OperatorVecReduceTensorTraitHalfCase)
{
    const int32_t srcDataSize = 128;
    const int32_t dstDataSize = 64;

    uint8_t srcGm[srcDataSize * sizeof(half)];
    uint8_t dstGm[dstDataSize * sizeof(half)];

    MainVecReduce<TensorTrait<half>>(srcGm, dstGm, srcDataSize, dstDataSize);

    for (int32_t i = 0; i < dstDataSize; i++) {
        EXPECT_EQ(dstGm[i], 0x00);
    }
}

template <typename T>
__global__ __aicore__ void MainAllReduceSimple(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm,
    __gm__ int32_t srcDataSize, __gm__ int32_t dstDataSize, __gm__ int32_t level, int32_t count)
{
    TPipe tpipe;
    int32_t mask = 64;
    uint64_t masks[2]{ FULL_MASK, 0 };
    if (sizeof(PrimT<T>) == sizeof(half)) {
        mask = 128;
        masks[0] = FULL_MASK;
        masks[1] = FULL_MASK;
    }
    int32_t repStride = 8;
    int32_t repeat = srcDataSize / mask;

    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ PrimT<T>*>(srcGm), srcDataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ PrimT<T>*>(dstGm), dstDataSize);

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, srcDataSize * sizeof(PrimT<T>));
    LocalTensor<T> inputLocal = tbuf1.Get<T>();

    TBuf<TPosition::VECCALC> tbuf2;
    tpipe.InitBuffer(tbuf2, dstDataSize * sizeof(PrimT<T>));
    LocalTensor<T> outputLocal = tbuf2.Get<T>();

    TBuf<TPosition::VECCALC> tbuf3;
    tpipe.InitBuffer(tbuf3, repeat * repStride);
    LocalTensor<T> workLocal = tbuf3.Get<T>();

    DataCopy(inputLocal, inputGlobal, srcDataSize);
    event_t eventIdMte2ToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_V));
    SetFlag<HardEvent::MTE2_V>(eventIdMte2ToV);
    WaitFlag<HardEvent::MTE2_V>(eventIdMte2ToV);

    switch (level) {
        case 0:
            ReduceMax<T>(outputLocal, inputLocal, workLocal, mask, repeat, repStride, false);
            ReduceMax<T>(outputLocal, inputLocal, workLocal, mask, repeat, repStride, true);
            ReduceMin<T>(outputLocal, inputLocal, workLocal, mask, repeat, repStride, false);
            ReduceMin<T>(outputLocal, inputLocal, workLocal, mask, repeat, repStride, true);
            ReduceSum<T>(outputLocal, inputLocal, workLocal, mask, repeat, repStride);

            ReduceMax<T>(outputLocal, inputLocal, workLocal, masks, repeat, repStride, false);
            ReduceMax<T>(outputLocal, inputLocal, workLocal, masks, repeat, repStride, true);
            ReduceMin<T>(outputLocal, inputLocal, workLocal, masks, repeat, repStride, false);
            ReduceMin<T>(outputLocal, inputLocal, workLocal, masks, repeat, repStride, true);
            ReduceSum<T>(outputLocal, inputLocal, workLocal, masks, repeat, repStride);
            break;
        case 2:
            ReduceMax<T>(outputLocal, inputLocal, workLocal, count, false);
            ReduceMax<T>(outputLocal, inputLocal, workLocal, count, true);
            ReduceMin<T>(outputLocal, inputLocal, workLocal, count, false);
            ReduceMin<T>(outputLocal, inputLocal, workLocal, count, true);
            ReduceSum<T>(outputLocal, inputLocal, workLocal, count);
            break;
        default:
            break;
    }
    event_t eventIdVToMte3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
    SetFlag<HardEvent::V_MTE3>(eventIdVToMte3);
    WaitFlag<HardEvent::V_MTE3>(eventIdVToMte3);

    DataCopy(outputGlobal, outputLocal, dstDataSize);

    PipeBarrier<PIPE_ALL>();
}

template <typename T>
__global__ __aicore__ void MainWholeReduceSimple(__gm__ uint8_t* __restrict__ srcGm,
    __gm__ uint8_t* __restrict__ dstGm, __gm__ int32_t srcDataSize, __gm__ int32_t dstDataSize,
    __gm__ int32_t level, int32_t count)
{
    TPipe tpipe;
    int32_t mask = 64;
    uint64_t masks[2]{ FULL_MASK, 0 };
    if (sizeof(PrimT<T>) == sizeof(half)) {
        mask = 128;
        masks[0] = FULL_MASK;
        masks[1] = FULL_MASK;
    }
    int32_t repStride = 8;
    int32_t repeat = srcDataSize / mask;

    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ PrimT<T>*>(srcGm), srcDataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ PrimT<T>*>(dstGm), dstDataSize);

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, srcDataSize * sizeof(PrimT<T>));
    LocalTensor<T> inputLocal = tbuf1.Get<T>();

    TBuf<TPosition::VECCALC> tbuf2;
    tpipe.InitBuffer(tbuf2, dstDataSize * sizeof(PrimT<T>));
    LocalTensor<T> outputLocal = tbuf2.Get<T>();

    TBuf<TPosition::VECCALC> tbuf3;
    tpipe.InitBuffer(tbuf3, repeat * repStride);
    LocalTensor<T> workLocal = tbuf3.Get<T>();

    DataCopy(inputLocal, inputGlobal, srcDataSize);
    event_t eventIdMte2ToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_V));
    SetFlag<HardEvent::MTE2_V>(eventIdMte2ToV);
    WaitFlag<HardEvent::MTE2_V>(eventIdMte2ToV);

    WholeReduceMax<T>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_VALUE_INDEX);
    WholeReduceMax<T>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_INDEX_VALUE);
    WholeReduceMax<T>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_VALUE);
    WholeReduceMax<T>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_INDEX);
    WholeReduceSum<T>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride);
    WholeReduceMin<T>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_VALUE_INDEX);
    WholeReduceMin<T>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_INDEX_VALUE);
    WholeReduceMin<T>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_VALUE);
    WholeReduceMin<T>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_INDEX);

    WholeReduceMax<T>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_VALUE_INDEX);
    WholeReduceMax<T>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_INDEX_VALUE);
    WholeReduceMax<T>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_VALUE);
    WholeReduceMax<T>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_INDEX);
    WholeReduceSum<T>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride);
    WholeReduceMin<T>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_VALUE_INDEX);
    WholeReduceMin<T>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_INDEX_VALUE);
    WholeReduceMin<T>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_VALUE);
    WholeReduceMin<T>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_INDEX);

    SetMaskCount();
    SetVectorMask<PrimT<T>, MaskMode::COUNTER>(0, 64);
    WholeReduceMax<T, false>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_VALUE_INDEX);
    WholeReduceMax<T, false>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_INDEX_VALUE);
    WholeReduceMax<T, false>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_VALUE);
    WholeReduceMax<T, false>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_INDEX);
    WholeReduceSum<T, false>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride);
    WholeReduceMin<T, false>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_VALUE_INDEX);
    WholeReduceMin<T, false>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_INDEX_VALUE);
    WholeReduceMin<T, false>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_VALUE);
    WholeReduceMin<T, false>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_INDEX);

    WholeReduceMax<T, false>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_VALUE_INDEX);
    WholeReduceMax<T, false>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_INDEX_VALUE);
    WholeReduceMax<T, false>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_VALUE);
    WholeReduceMax<T, false>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_INDEX);
    WholeReduceSum<T, false>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride);
    WholeReduceMin<T, false>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_VALUE_INDEX);
    WholeReduceMin<T, false>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_INDEX_VALUE);
    WholeReduceMin<T, false>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_VALUE);
    WholeReduceMin<T, false>(outputLocal, inputLocal, masks, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_INDEX);
    SetMaskNorm();
    AscendCUtils::ResetMask();
    event_t eventIdVToMte3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
    SetFlag<HardEvent::V_MTE3>(eventIdVToMte3);
    WaitFlag<HardEvent::V_MTE3>(eventIdVToMte3);

    DataCopy(outputGlobal, outputLocal, dstDataSize);

    PipeBarrier<PIPE_ALL>();
}
struct ReduceTestParams {
    void (*calFunc)(uint8_t*, uint8_t*, int32_t, int32_t, int32_t, int32_t);
    int32_t srcDataSize;
    int32_t dstDataSize;
    int32_t dtypeSize;
    int32_t level;
    int32_t count;
};

class ReduceSimpleTestsuite : public testing::Test, public testing::WithParamInterface<ReduceTestParams> {
protected:
    void SetUp()
    {
        AscendC::SetGCoreType(2);
    }
    void TearDown()
    {
        AscendC::SetGCoreType(0);
        GlobalMockObject::verify();
    }
};

INSTANTIATE_TEST_CASE_P(TEST_REDUCE_SIMPLE, ReduceSimpleTestsuite,
    ::testing::Values(ReduceTestParams{ MainAllReduceSimple<half>, 128, 16, 2, 0, 128 },
    ReduceTestParams{ MainAllReduceSimple<float>, 64, 16, 4, 0, 64 },
    ReduceTestParams{ MainAllReduceSimple<float>, 64, 16, 4, 2, 64 },
    ReduceTestParams{ MainAllReduceSimple<float>, 64, 16, 4, 2, 63 },
    ReduceTestParams{ MainAllReduceSimple<float>, 16648, 16, 4, 0, 16648 },
    ReduceTestParams{ MainAllReduceSimple<float>, 16648, 16, 4, 2, 16648 },
    ReduceTestParams{ MainWholeReduceSimple<float>, 64, 16, 4, 0, 64},
    ReduceTestParams{ MainAllReduceSimple<half>, 128, 16, 2, 0, 128 },
    ReduceTestParams{ MainAllReduceSimple<half>, 128, 16, 2, 2, 128 },
    ReduceTestParams{ MainAllReduceSimple<half>, 32656, 16, 2, 0, 32656 },
    ReduceTestParams{ MainAllReduceSimple<half>, 32656, 16, 2, 2, 32656 },
    ReduceTestParams{ MainWholeReduceSimple<half>, 128, 16, 2, 0, 128 },
    // TensorTrait Case
    ReduceTestParams{ MainAllReduceSimple<TensorTrait<float>>, 16648, 16, 4, 2, 16648 },
    ReduceTestParams{ MainAllReduceSimple<TensorTrait<half>>, 32656, 16, 2, 2, 32656 },
    ReduceTestParams{ MainWholeReduceSimple<TensorTrait<float>>, 64, 16, 4, 0, 64 },
    ReduceTestParams{ MainWholeReduceSimple<TensorTrait<half>>, 128, 16, 2, 0, 128 }
    ));

TEST_P(ReduceSimpleTestsuite, ReduceSimpleTestCase)
{
    auto param = GetParam();
    uint8_t srcGm[param.srcDataSize * param.dtypeSize];
    uint8_t dstGm[param.dstDataSize * param.dtypeSize];

    param.calFunc(srcGm, dstGm, param.srcDataSize, param.dstDataSize, param.level, param.count);
    for (int32_t i = 0; i < param.dstDataSize; i++) {
        EXPECT_EQ(dstGm[i], 0x00);
    }
}
TEST_F(ReduceSimpleTestsuite, GetMaxMinCntCase)
{
    int32_t srcDataSize = 64;
    int32_t dstDataSize = 16;
    int32_t dtypeSize = 4;

    uint8_t srcGm[srcDataSize * dtypeSize];
    uint8_t dstGm[dstDataSize * dtypeSize];
    TPipe tpipe;
    int32_t mask = 64;
    int32_t repStride = 8;
    int32_t repeat = srcDataSize / mask;

    GlobalTensor<float> inputGlobal;
    GlobalTensor<float> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(srcGm), srcDataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(dstGm), dstDataSize);

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, srcDataSize * sizeof(float));
    LocalTensor<float> inputLocal = tbuf1.Get<float>();

    TBuf<TPosition::VECCALC> tbuf2;
    tpipe.InitBuffer(tbuf2, dstDataSize * sizeof(float));
    LocalTensor<float> outputLocal = tbuf2.Get<float>();

    TBuf<TPosition::VECCALC> tbuf3;
    tpipe.InitBuffer(tbuf3, repeat * repStride);
    LocalTensor<float> workLocal = tbuf3.Get<float>();

    DataCopy(inputLocal, inputGlobal, srcDataSize);
    event_t eventIdMte2ToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_V));
    SetFlag<HardEvent::MTE2_V>(eventIdMte2ToV);
    WaitFlag<HardEvent::MTE2_V>(eventIdMte2ToV);

    WholeReduceMax<float>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_VALUE_INDEX);

    event_t eventIdVToS = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_S));
    SetFlag<HardEvent::V_S>(eventIdVToS);
    WaitFlag<HardEvent::V_S>(eventIdVToS);

    uint32_t maxMinVal = 0;
    uint32_t maxMinIdx = 0;
    GetReduceMaxMinCount<float>(maxMinVal, maxMinIdx);
    EXPECT_EQ(maxMinVal, 0x0);
    EXPECT_EQ(maxMinIdx, 0x0);

    float maxMinFp32Val = 0;
    float maxMinFp32Idx = 0;
    GetReduceMaxMinCount<float>(maxMinFp32Val, maxMinFp32Idx);
    EXPECT_EQ(maxMinFp32Val, 0x0);
    EXPECT_EQ(maxMinFp32Idx, 0x0);

    event_t eventIdSToMte3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_MTE3));
    SetFlag<HardEvent::S_MTE3>(eventIdSToMte3);
    WaitFlag<HardEvent::S_MTE3>(eventIdSToMte3);

    DataCopy(outputGlobal, outputLocal, dstDataSize);

    PipeBarrier<PIPE_ALL>();
}
TEST_F(ReduceSimpleTestsuite, GetReduceRepeatMaxMinSprCase)
{
    int32_t srcDataSize = 64;
    int32_t dstDataSize = 16;
    int32_t dtypeSize = 4;

    uint8_t srcGm[srcDataSize * dtypeSize];
    uint8_t dstGm[dstDataSize * dtypeSize];
    TPipe tpipe;
    int32_t mask = 64;
    int32_t repStride = 8;
    int32_t repeat = srcDataSize / mask;

    GlobalTensor<float> inputGlobal;
    GlobalTensor<float> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(srcGm), srcDataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(dstGm), dstDataSize);

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, srcDataSize * sizeof(float));
    LocalTensor<float> inputLocal = tbuf1.Get<float>();

    TBuf<TPosition::VECCALC> tbuf2;
    tpipe.InitBuffer(tbuf2, dstDataSize * sizeof(float));
    LocalTensor<float> outputLocal = tbuf2.Get<float>();

    TBuf<TPosition::VECCALC> tbuf3;
    tpipe.InitBuffer(tbuf3, repeat * repStride);
    LocalTensor<float> workLocal = tbuf3.Get<float>();

    DataCopy(inputLocal, inputGlobal, srcDataSize);
    event_t eventIdMte2ToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_V));
    SetFlag<HardEvent::MTE2_V>(eventIdMte2ToV);
    WaitFlag<HardEvent::MTE2_V>(eventIdMte2ToV);

    WholeReduceMax<float>(outputLocal, inputLocal, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_VALUE_INDEX);

    event_t eventIdVToS = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_S));
    SetFlag<HardEvent::V_S>(eventIdVToS);
    WaitFlag<HardEvent::V_S>(eventIdVToS);

    uint32_t maxMinVal = 0;
    uint32_t maxMinIdx = 0;
    GetReduceRepeatMaxMinSpr<float>(maxMinVal, maxMinIdx);
    EXPECT_EQ(maxMinVal, 0x0);
    EXPECT_EQ(maxMinIdx, 0x0);

    float maxMinFp32Val = 0;
    float maxMinFp32Idx = 0;
    GetReduceRepeatMaxMinSpr<float>(maxMinFp32Val, maxMinFp32Idx);
    EXPECT_EQ(maxMinFp32Val, 0x0);
    EXPECT_EQ(maxMinFp32Idx, 0x0);

    event_t eventIdSToMte3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_MTE3));
    SetFlag<HardEvent::S_MTE3>(eventIdSToMte3);
    WaitFlag<HardEvent::S_MTE3>(eventIdSToMte3);

    DataCopy(outputGlobal, outputLocal, dstDataSize);

    PipeBarrier<PIPE_ALL>();
}

template <typename T>
__global__ __aicore__ void MainRepeatReduceSum(__gm__ uint8_t* __restrict__ dstGm, __gm__ uint8_t* __restrict__ srcGm,
    const int32_t repeat, const int32_t elemsInOneRepeat, const int32_t dstBlkStride, const int32_t srcBlkStride,
    const int32_t dstRepStride, const int32_t srcRepStride, const int32_t dataSize1, const int32_t dataSize2)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize1);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize2);

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize1 * sizeof(T));
    LocalTensor<T> inputLocal = tbuf1.Get<T>();

    TBuf<TPosition::VECCALC> tbuf2;
    tpipe.InitBuffer(tbuf2, dataSize2 * sizeof(T));
    LocalTensor<T> outputLocal = tbuf2.Get<T>();

    RepeatReduceSum(outputLocal, inputLocal, repeat, elemsInOneRepeat, dstBlkStride, srcBlkStride, dstRepStride,
        srcRepStride);
    event_t eventIdVToMte3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
    SetFlag<HardEvent::V_MTE3>(eventIdVToMte3);
    WaitFlag<HardEvent::V_MTE3>(eventIdVToMte3);

    PipeBarrier<PIPE_ALL>();
}

struct RepeatReduceSumTestParams {
    const int32_t repeatIn;
    const int32_t elemsInOneRepeatIn;
    const int32_t dstBlkStrideIn;
    const int32_t srcBlkStrideIn;
    const int32_t dstRepStrideIn;
    const int32_t srcRepStrideIn;
    void (*CalFunc)(uint8_t*, uint8_t*, const int32_t, const int32_t, const int32_t, const int32_t, const int32_t,
        const int32_t, const int32_t, const int32_t);
    const int32_t dataSize1;
    const int32_t dataSize2;
    const int8_t typeByte;
    const bool expectRes;
};

class RepeatReduceSumTestsuite : public testing::Test, public testing::WithParamInterface<RepeatReduceSumTestParams> {
protected:
    void SetUp() { AscendC::SetGCoreType(2); }
    void TearDown()
    {
        AscendC::SetGCoreType(0);
        GlobalMockObject::verify();
    }
};

INSTANTIATE_TEST_CASE_P(TEST_OPEARATION_REPEATREDUCESUM, RepeatReduceSumTestsuite,
    ::testing::Values(
    // 1 repeatTimes
    RepeatReduceSumTestParams{1, 64, 1, 1, 8, 8, MainRepeatReduceSum<half>,  64, 1,   2, true},   // src test
    // 2 repeatTimes
    RepeatReduceSumTestParams{ 2, 32, 1, 1, 1, 8, MainRepeatReduceSum<float>, 96, 2, 4, true}, // src + dst test
    RepeatReduceSumTestParams{ 2, 32, 1, 1, 8, 16, MainRepeatReduceSum<float>, 160, 9, 4, true}, // src + dst test
    RepeatReduceSumTestParams{ 2, 32, 1, 1, 8, 8, MainRepeatReduceSum<float>, 96, 8, 4, false}  // dst false
));

TEST_P(RepeatReduceSumTestsuite, RepeatReduceSumTestCase)
{
    auto param = GetParam();
    uint8_t dstGm[param.dataSize2 * param.typeByte] = {0};
    uint8_t srcGm[param.dataSize1 * param.typeByte] = {0};
    if (!param.expectRes) {
        MOCKER(raise, int(*)(int)).times(1).will(returnValue(0));
    }
    param.CalFunc(dstGm, srcGm, param.repeatIn, param.elemsInOneRepeatIn, param.dstBlkStrideIn, param.srcBlkStrideIn,
        param.dstRepStrideIn, param.srcRepStrideIn, param.dataSize1, param.dataSize2);

    for (int i = 1; i < param.dataSize2; i++) {
        EXPECT_EQ(dstGm[i], 0x00);
    }
    if (param.typeByte == B32_BYTE_SIZE) {
        float valRes = 0;
        EXPECT_EQ(GetAccVal<float>(), valRes);
    } else {
        half valRes = 0;
        EXPECT_EQ(GetAccVal<half>(), valRes);
    }
}

// ============================================================
// Reduce API NPU Debug validation - parameterized negative tests
// Cover: Tensor position, mask, parameter range for all Reduce APIs
// ============================================================

namespace {
int32_t RaiseStubForNpuDebug(int32_t i)
{
    return 0;
}

enum class ReduceApiType : int32_t {
    WHOLE_REDUCE_MAX,
    WHOLE_REDUCE_MIN,
    WHOLE_REDUCE_SUM,
    BLOCK_REDUCE_MAX,
    BLOCK_REDUCE_MIN,
    BLOCK_REDUCE_SUM,
    PAIR_REDUCE_SUM,
    REPEAT_REDUCE_SUM,
};

enum class NegCaseType : int32_t {
    DST_NOT_UB,
    SRC_NOT_UB,
    MASK_COUNT_ZERO,
    MASK_COUNT_EXCEED_HALF,
    MASK_COUNT_EXCEED_FLOAT,
    REPEAT_TIME_OVERFLOW,
    ORDER_OVERFLOW,
    DST_NOT_UB_MASK_BIT,
};

struct ReduceNpuDebugParams {
    ReduceApiType apiType;
    NegCaseType negType;
    int32_t dtypeSize; // 2=half, 4=float
    int32_t expectedRaiseCount;
    const char* caseName;
};
} // namespace

class TestReduceNpuDebug : public testing::Test,
                           public testing::WithParamInterface<ReduceNpuDebugParams> {
protected:
    void SetUp()
    {
        AscendC::SetGCoreType(2);
        AscendC::KernelRaise::GetInstance().SetRaiseMode(false);
    }
    void TearDown()
    {
        AscendC::KernelRaise::GetInstance().SetRaiseMode(true);
        AscendC::SetGCoreType(0);
        block_idx = 0;
        block_num = 8;
        g_coreType = 0;
        g_taskRation = 2;
        sub_block_idx = 0;
        g_kernelMode = KernelMode::MIX_MODE;
        AscendC::CheckSyncState();
        GlobalMockObject::verify();
    }
};

TEST_P(TestReduceNpuDebug, NegativeCase)
{
    auto p = GetParam();
    TPipe tpipe;
    MOCKER(raise, int32_t (*)(int32_t)).times(p.expectedRaiseCount).will(invoke(RaiseStubForNpuDebug));

    bool isFloat = (p.dtypeSize == 4);
    bool srcOnA1 = (p.negType == NegCaseType::SRC_NOT_UB);
    bool dstOnA2 = (p.negType == NegCaseType::DST_NOT_UB || p.negType == NegCaseType::DST_NOT_UB_MASK_BIT);
    int32_t srcSize = isFloat ? 128 : 128;
    if (p.negType == NegCaseType::MASK_COUNT_EXCEED_HALF && !isFloat) {
        srcSize = 256;
    }
    int32_t dstSize = 16;

    // Declare TBuf objects with full TEST_P scope lifetime
    TBuf<TPosition::VECCALC> srcBufUb;   // src on UB (VECCALC)
    TBuf<TPosition::A1>      srcBufA1;   // src on L1 (A1)
    TBuf<TPosition::VECCALC> dstBufUb;   // dst on UB (VECCALC)
    TBuf<TPosition::A2>      dstBufA2;   // dst on L0A (A2)

    LocalTensor<half> srcHalf;
    LocalTensor<float> srcFloat;
    LocalTensor<half> dstHalf;
    LocalTensor<float> dstFloat;

    // Init src buffer and get tensor
    if (srcOnA1) {
        tpipe.InitBuffer(srcBufA1, srcSize * (isFloat ? sizeof(float) : sizeof(half)));
        if (isFloat) { srcFloat = srcBufA1.Get<float>(); } else { srcHalf = srcBufA1.Get<half>(); }
    } else {
        tpipe.InitBuffer(srcBufUb, srcSize * (isFloat ? sizeof(float) : sizeof(half)));
        if (isFloat) { srcFloat = srcBufUb.Get<float>(); } else { srcHalf = srcBufUb.Get<half>(); }
    }

    // Init dst buffer and get tensor
    if (dstOnA2) {
        tpipe.InitBuffer(dstBufA2, dstSize * (isFloat ? sizeof(float) : sizeof(half)));
        if (isFloat) { dstFloat = dstBufA2.Get<float>(); } else { dstHalf = dstBufA2.Get<half>(); }
    } else {
        tpipe.InitBuffer(dstBufUb, dstSize * (isFloat ? sizeof(float) : sizeof(half)));
        if (isFloat) { dstFloat = dstBufUb.Get<float>(); } else { dstHalf = dstBufUb.Get<half>(); }
    }

    // Invoke the corresponding Reduce API with illegal params
    switch (p.apiType) {
        case ReduceApiType::WHOLE_REDUCE_MAX:
            if (isFloat) {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        WholeReduceMax<float>(dstFloat, srcFloat, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        WholeReduceMax<float>(dstFloat, srcFloat, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        WholeReduceMax<float>(dstFloat, srcFloat, 0, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::MASK_COUNT_EXCEED_FLOAT:
                        WholeReduceMax<float>(dstFloat, srcFloat, 65, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        WholeReduceMax<float>(dstFloat, srcFloat, 128, 256, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::ORDER_OVERFLOW:
                        WholeReduceMax<float>(dstFloat, srcFloat, 128, 1, 1, 1, 8, static_cast<ReduceOrder>(4));
                        break;
                    default:
                        break;
                }
            } else {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        WholeReduceMax<half>(dstHalf, srcHalf, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
                        break;
                    case NegCaseType::DST_NOT_UB_MASK_BIT: {
                        uint64_t masks[2] = { FULL_MASK, FULL_MASK };
                        WholeReduceMax<half>(dstHalf, srcHalf, masks, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
                        break;
                    }
                    case NegCaseType::SRC_NOT_UB:
                        WholeReduceMax<half>(dstHalf, srcHalf, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        WholeReduceMax<half>(dstHalf, srcHalf, 0, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::MASK_COUNT_EXCEED_HALF:
                        WholeReduceMax<half>(dstHalf, srcHalf, 129, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        WholeReduceMax<half>(dstHalf, srcHalf, 128, 256, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::ORDER_OVERFLOW:
                        WholeReduceMax<half>(dstHalf, srcHalf, 128, 1, 1, 1, 8, static_cast<ReduceOrder>(4));
                        break;
                    default:
                        break;
                }
            }
            break;
        case ReduceApiType::WHOLE_REDUCE_MIN:
            if (isFloat) {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        WholeReduceMin<float>(dstFloat, srcFloat, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        WholeReduceMin<float>(dstFloat, srcFloat, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        WholeReduceMin<float>(dstFloat, srcFloat, 0, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::MASK_COUNT_EXCEED_FLOAT:
                        WholeReduceMin<float>(dstFloat, srcFloat, 65, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        WholeReduceMin<float>(dstFloat, srcFloat, 128, 256, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::ORDER_OVERFLOW:
                        WholeReduceMin<float>(dstFloat, srcFloat, 128, 1, 1, 1, 8, static_cast<ReduceOrder>(4));
                        break;
                    default:
                        break;
                }
            } else {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        WholeReduceMin<half>(dstHalf, srcHalf, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        WholeReduceMin<half>(dstHalf, srcHalf, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        WholeReduceMin<half>(dstHalf, srcHalf, 0, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::MASK_COUNT_EXCEED_HALF:
                        WholeReduceMin<half>(dstHalf, srcHalf, 129, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        WholeReduceMin<half>(dstHalf, srcHalf, 128, 256, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
                        break;
                    case NegCaseType::ORDER_OVERFLOW:
                        WholeReduceMin<half>(dstHalf, srcHalf, 128, 1, 1, 1, 8, static_cast<ReduceOrder>(4));
                        break;
                    default:
                        break;
                }
            }
            break;
        case ReduceApiType::WHOLE_REDUCE_SUM:
            if (isFloat) {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        WholeReduceSum<float>(dstFloat, srcFloat, 128, 1, 1, 1, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        WholeReduceSum<float>(dstFloat, srcFloat, 128, 1, 1, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        WholeReduceSum<float>(dstFloat, srcFloat, 0, 1, 1, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_EXCEED_FLOAT:
                        WholeReduceSum<float>(dstFloat, srcFloat, 65, 1, 1, 1, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        WholeReduceSum<float>(dstFloat, srcFloat, 128, 256, 1, 1, 8);
                        break;
                    default:
                        break;
                }
            } else {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        WholeReduceSum<half>(dstHalf, srcHalf, 128, 1, 1, 1, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        WholeReduceSum<half>(dstHalf, srcHalf, 128, 1, 1, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        WholeReduceSum<half>(dstHalf, srcHalf, 0, 1, 1, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_EXCEED_HALF:
                        WholeReduceSum<half>(dstHalf, srcHalf, 129, 1, 1, 1, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        WholeReduceSum<half>(dstHalf, srcHalf, 128, 256, 1, 1, 8);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ReduceApiType::BLOCK_REDUCE_MAX:
            if (isFloat) {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        BlockReduceMax<float>(dstFloat, srcFloat, 64, 1, 8, 1, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        BlockReduceMax<float>(dstFloat, srcFloat, 64, 1, 8, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        BlockReduceMax<float>(dstFloat, srcFloat, 0, 1, 8, 1, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        BlockReduceMax<float>(dstFloat, srcFloat, 64, 256, 8, 1, 8);
                        break;
                    default:
                        break;
                }
            } else {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        BlockReduceMax<half>(dstHalf, srcHalf, 128, 1, 8, 1, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        BlockReduceMax<half>(dstHalf, srcHalf, 128, 1, 8, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        BlockReduceMax<half>(dstHalf, srcHalf, 0, 1, 8, 1, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        BlockReduceMax<half>(dstHalf, srcHalf, 128, 256, 8, 1, 8);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ReduceApiType::BLOCK_REDUCE_MIN:
            if (isFloat) {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        BlockReduceMin<float>(dstFloat, srcFloat, 64, 1, 8, 1, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        BlockReduceMin<float>(dstFloat, srcFloat, 64, 1, 8, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        BlockReduceMin<float>(dstFloat, srcFloat, 0, 1, 8, 1, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        BlockReduceMin<float>(dstFloat, srcFloat, 64, 256, 8, 1, 8);
                        break;
                    default:
                        break;
                }
            } else {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        BlockReduceMin<half>(dstHalf, srcHalf, 128, 1, 8, 1, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        BlockReduceMin<half>(dstHalf, srcHalf, 128, 1, 8, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        BlockReduceMin<half>(dstHalf, srcHalf, 0, 1, 8, 1, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        BlockReduceMin<half>(dstHalf, srcHalf, 128, 256, 8, 1, 8);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ReduceApiType::BLOCK_REDUCE_SUM:
            if (isFloat) {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        BlockReduceSum<float>(dstFloat, srcFloat, 64, 1, 8, 1, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        BlockReduceSum<float>(dstFloat, srcFloat, 64, 1, 8, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        BlockReduceSum<float>(dstFloat, srcFloat, 0, 1, 8, 1, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        BlockReduceSum<float>(dstFloat, srcFloat, 64, 256, 8, 1, 8);
                        break;
                    default:
                        break;
                }
            } else {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        BlockReduceSum<half>(dstHalf, srcHalf, 128, 1, 8, 1, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        BlockReduceSum<half>(dstHalf, srcHalf, 128, 1, 8, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        BlockReduceSum<half>(dstHalf, srcHalf, 0, 1, 8, 1, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        BlockReduceSum<half>(dstHalf, srcHalf, 128, 256, 8, 1, 8);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ReduceApiType::PAIR_REDUCE_SUM:
            if (isFloat) {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        PairReduceSum<float>(dstFloat, srcFloat, 1, 64, 8, 1, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        PairReduceSum<float>(dstFloat, srcFloat, 1, 64, 8, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        PairReduceSum<float>(dstFloat, srcFloat, 1, 0, 8, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_EXCEED_FLOAT:
                        PairReduceSum<float>(dstFloat, srcFloat, 1, 65, 8, 1, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        PairReduceSum<float>(dstFloat, srcFloat, 256, 64, 8, 1, 8);
                        break;
                    default:
                        break;
                }
            } else {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        PairReduceSum<half>(dstHalf, srcHalf, 1, 128, 8, 1, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        PairReduceSum<half>(dstHalf, srcHalf, 1, 128, 8, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        PairReduceSum<half>(dstHalf, srcHalf, 1, 0, 8, 1, 8);
                        break;
                    case NegCaseType::MASK_COUNT_EXCEED_HALF:
                        PairReduceSum<half>(dstHalf, srcHalf, 1, 129, 8, 1, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        PairReduceSum<half>(dstHalf, srcHalf, 256, 128, 8, 1, 8);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ReduceApiType::REPEAT_REDUCE_SUM:
            if (isFloat) {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        RepeatReduceSum<float>(dstFloat, srcFloat, 1, 64, 1, 1, 8, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        RepeatReduceSum<float>(dstFloat, srcFloat, 1, 64, 1, 1, 8, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        RepeatReduceSum<float>(dstFloat, srcFloat, 1, 0, 1, 1, 8, 8);
                        break;
                    case NegCaseType::MASK_COUNT_EXCEED_FLOAT:
                        RepeatReduceSum<float>(dstFloat, srcFloat, 1, 65, 1, 1, 8, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        RepeatReduceSum<float>(dstFloat, srcFloat, 256, 64, 1, 1, 8, 8);
                        break;
                    default:
                        break;
                }
            } else {
                switch (p.negType) {
                    case NegCaseType::DST_NOT_UB:
                        RepeatReduceSum<half>(dstHalf, srcHalf, 1, 64, 1, 1, 8, 8);
                        break;
                    case NegCaseType::SRC_NOT_UB:
                        RepeatReduceSum<half>(dstHalf, srcHalf, 1, 64, 1, 1, 8, 8);
                        break;
                    case NegCaseType::MASK_COUNT_ZERO:
                        RepeatReduceSum<half>(dstHalf, srcHalf, 1, 0, 1, 1, 8, 8);
                        break;
                    case NegCaseType::REPEAT_TIME_OVERFLOW:
                        RepeatReduceSum<half>(dstHalf, srcHalf, 256, 64, 1, 1, 8, 8);
                        break;
                    default:
                        break;
                }
            }
            break;
    }
}

INSTANTIATE_TEST_CASE_P(TEST_REDUCE_NPU_DEBUG, TestReduceNpuDebug,
    ::testing::Values(
        // WholeReduceMax
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MAX, NegCaseType::DST_NOT_UB, 2, 1, "WholeReduceMax_DstNotUb"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MAX, NegCaseType::DST_NOT_UB_MASK_BIT, 2, 1, "WholeReduceMax_DstNotUbMaskBit"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MAX, NegCaseType::SRC_NOT_UB, 2, 1, "WholeReduceMax_SrcNotUb"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MAX, NegCaseType::MASK_COUNT_ZERO, 2, 0, "WholeReduceMax_MaskZero"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MAX, NegCaseType::MASK_COUNT_EXCEED_HALF, 2, 0, "WholeReduceMax_MaskExceedHalf"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MAX, NegCaseType::MASK_COUNT_EXCEED_FLOAT, 4, 0, "WholeReduceMax_MaskExceedFloat"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MAX, NegCaseType::REPEAT_TIME_OVERFLOW, 2, 1, "WholeReduceMax_RepeatOverflow"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MAX, NegCaseType::ORDER_OVERFLOW, 2, 1, "WholeReduceMax_OrderOverflow"},

        // WholeReduceMin
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MIN, NegCaseType::DST_NOT_UB, 2, 1, "WholeReduceMin_DstNotUb"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MIN, NegCaseType::SRC_NOT_UB, 2, 1, "WholeReduceMin_SrcNotUb"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MIN, NegCaseType::MASK_COUNT_ZERO, 2, 0, "WholeReduceMin_MaskZero"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MIN, NegCaseType::REPEAT_TIME_OVERFLOW, 2, 1, "WholeReduceMin_RepeatOverflow"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_MIN, NegCaseType::ORDER_OVERFLOW, 2, 1, "WholeReduceMin_OrderOverflow"},

        // WholeReduceSum
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_SUM, NegCaseType::DST_NOT_UB, 2, 1, "WholeReduceSum_DstNotUb"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_SUM, NegCaseType::SRC_NOT_UB, 2, 1, "WholeReduceSum_SrcNotUb"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_SUM, NegCaseType::MASK_COUNT_ZERO, 2, 0, "WholeReduceSum_MaskZero"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_SUM, NegCaseType::MASK_COUNT_EXCEED_FLOAT, 4, 0, "WholeReduceSum_MaskExceedFloat"},
        ReduceNpuDebugParams{ReduceApiType::WHOLE_REDUCE_SUM, NegCaseType::REPEAT_TIME_OVERFLOW, 2, 1, "WholeReduceSum_RepeatOverflow"},

        // BlockReduceMax
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_MAX, NegCaseType::DST_NOT_UB, 2, 1, "BlockReduceMax_DstNotUb"},
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_MAX, NegCaseType::SRC_NOT_UB, 2, 1, "BlockReduceMax_SrcNotUb"},
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_MAX, NegCaseType::MASK_COUNT_ZERO, 2, 0, "BlockReduceMax_MaskZero"},
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_MAX, NegCaseType::REPEAT_TIME_OVERFLOW, 2, 1, "BlockReduceMax_RepeatOverflow"},

        // BlockReduceMin
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_MIN, NegCaseType::DST_NOT_UB, 2, 1, "BlockReduceMin_DstNotUb"},
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_MIN, NegCaseType::SRC_NOT_UB, 2, 1, "BlockReduceMin_SrcNotUb"},
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_MIN, NegCaseType::MASK_COUNT_ZERO, 2, 0, "BlockReduceMin_MaskZero"},
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_MIN, NegCaseType::REPEAT_TIME_OVERFLOW, 2, 1, "BlockReduceMin_RepeatOverflow"},

        // BlockReduceSum
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_SUM, NegCaseType::DST_NOT_UB, 2, 1, "BlockReduceSum_DstNotUb"},
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_SUM, NegCaseType::SRC_NOT_UB, 2, 1, "BlockReduceSum_SrcNotUb"},
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_SUM, NegCaseType::MASK_COUNT_ZERO, 2, 0, "BlockReduceSum_MaskZero"},
        ReduceNpuDebugParams{ReduceApiType::BLOCK_REDUCE_SUM, NegCaseType::REPEAT_TIME_OVERFLOW, 2, 1, "BlockReduceSum_RepeatOverflow"},

        // PairReduceSum
        ReduceNpuDebugParams{ReduceApiType::PAIR_REDUCE_SUM, NegCaseType::DST_NOT_UB, 2, 1, "PairReduceSum_DstNotUb"},
        ReduceNpuDebugParams{ReduceApiType::PAIR_REDUCE_SUM, NegCaseType::SRC_NOT_UB, 2, 1, "PairReduceSum_SrcNotUb"},
        ReduceNpuDebugParams{ReduceApiType::PAIR_REDUCE_SUM, NegCaseType::MASK_COUNT_ZERO, 2, 0, "PairReduceSum_MaskZero"},
        ReduceNpuDebugParams{ReduceApiType::PAIR_REDUCE_SUM, NegCaseType::MASK_COUNT_EXCEED_HALF, 2, 1, "PairReduceSum_MaskExceedHalf"},
        ReduceNpuDebugParams{ReduceApiType::PAIR_REDUCE_SUM, NegCaseType::MASK_COUNT_EXCEED_FLOAT, 4, 1, "PairReduceSum_MaskExceedFloat"},
        ReduceNpuDebugParams{ReduceApiType::PAIR_REDUCE_SUM, NegCaseType::REPEAT_TIME_OVERFLOW, 2, 2, "PairReduceSum_RepeatOverflow"},

        // RepeatReduceSum
        ReduceNpuDebugParams{ReduceApiType::REPEAT_REDUCE_SUM, NegCaseType::DST_NOT_UB, 2, 1, "RepeatReduceSum_DstNotUb"},
        ReduceNpuDebugParams{ReduceApiType::REPEAT_REDUCE_SUM, NegCaseType::SRC_NOT_UB, 2, 1, "RepeatReduceSum_SrcNotUb"},
        ReduceNpuDebugParams{ReduceApiType::REPEAT_REDUCE_SUM, NegCaseType::MASK_COUNT_ZERO, 2, 0, "RepeatReduceSum_MaskZero"},
        ReduceNpuDebugParams{ReduceApiType::REPEAT_REDUCE_SUM, NegCaseType::MASK_COUNT_EXCEED_FLOAT, 4, 0, "RepeatReduceSum_MaskExceedFloat"},
        ReduceNpuDebugParams{ReduceApiType::REPEAT_REDUCE_SUM, NegCaseType::REPEAT_TIME_OVERFLOW, 2, 2, "RepeatReduceSum_RepeatOverflow"}
    ));