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
// WholeReduceMax NPU Debug validation - negative test cases
// Cover: Tensor position, Tensor alignment, mask, parameter range
// ============================================================

namespace {
int32_t RaiseStubForNpuDebug(int32_t i)
{
    return 0;
}
}

class TestWholeReduceMaxNpuDebug : public testing::Test {
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

// --- 1. Tensor physical position negative cases ---

// dst on L0A (not UB) - mask count mode
TEST_F(TestWholeReduceMaxNpuDebug, DstPositionNotUbMaskCount)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::A2> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMax<half>(dstLocal, srcLocal, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
}

// dst on L0A (not UB) - mask bit mode
TEST_F(TestWholeReduceMaxNpuDebug, DstPositionNotUbMaskBit)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::A2> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    uint64_t masks[2] = { FULL_MASK, FULL_MASK };
    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMax<half>(dstLocal, srcLocal, masks, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
}

// src on L1 (not UB) - mask count mode
TEST_F(TestWholeReduceMaxNpuDebug, SrcPositionNotUbMaskCount)
{
    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMax<half>(dstLocal, srcLocal, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
}

// --- 2. Mask negative cases ---

// mask count mode, mask=0 (reduce API requires mask >= 1)
TEST_F(TestWholeReduceMaxNpuDebug, MaskCountZero)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMax<half>(dstLocal, srcLocal, 0, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
}

// mask count mode, mask=129 (exceeds half upper bound 128)
TEST_F(TestWholeReduceMaxNpuDebug, MaskCountExceedUpperHalf)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 256 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMax<half>(dstLocal, srcLocal, 129, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
}

// mask count mode, mask=65 (exceeds float upper bound 64)
TEST_F(TestWholeReduceMaxNpuDebug, MaskCountExceedUpperFloat)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(float));
    LocalTensor<float> srcLocal = srcBuf.Get<float>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(float));
    LocalTensor<float> dstLocal = dstBuf.Get<float>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMax<float>(dstLocal, srcLocal, 65, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
}

// --- 3. Parameter range negative cases ---

// repeatTime=256 (exceeds [0, 255])
TEST_F(TestWholeReduceMaxNpuDebug, RepeatTimeOverflow)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMax<half>(dstLocal, srcLocal, 128, 256, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
}

// order=4 (exceeds [0, 3])
TEST_F(TestWholeReduceMaxNpuDebug, OrderOverflow)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMax<half>(dstLocal, srcLocal, 128, 1, 1, 1, 8, static_cast<ReduceOrder>(4));
}

// ============================================================
// WholeReduceMin NPU Debug validation - negative test cases
// Same checks as WholeReduceMax: position, alignment, mask, order
// ============================================================

class TestWholeReduceMinNpuDebug : public testing::Test {
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

TEST_F(TestWholeReduceMinNpuDebug, DstPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::A2> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMin<half>(dstLocal, srcLocal, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
}

TEST_F(TestWholeReduceMinNpuDebug, SrcPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMin<half>(dstLocal, srcLocal, 128, 1, 1, 1, 8, ReduceOrder::ORDER_VALUE_INDEX);
}

TEST_F(TestWholeReduceMinNpuDebug, MaskCountZero)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMin<half>(dstLocal, srcLocal, 0, 1, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
}

TEST_F(TestWholeReduceMinNpuDebug, RepeatTimeOverflow)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMin<half>(dstLocal, srcLocal, 128, 256, 1, 1, 8, ReduceOrder::ORDER_ONLY_VALUE);
}

TEST_F(TestWholeReduceMinNpuDebug, OrderOverflow)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceMin<half>(dstLocal, srcLocal, 128, 1, 1, 1, 8, static_cast<ReduceOrder>(4));
}

// ============================================================
// WholeReduceSum NPU Debug validation - negative test cases
// Same as Max/Min but no order parameter
// ============================================================

class TestWholeReduceSumNpuDebug : public testing::Test {
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

TEST_F(TestWholeReduceSumNpuDebug, DstPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::A2> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceSum<half>(dstLocal, srcLocal, 128, 1, 1, 1, 8);
}

TEST_F(TestWholeReduceSumNpuDebug, SrcPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceSum<half>(dstLocal, srcLocal, 128, 1, 1, 1, 8);
}

TEST_F(TestWholeReduceSumNpuDebug, MaskCountZero)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceSum<half>(dstLocal, srcLocal, 0, 1, 1, 1, 8);
}

TEST_F(TestWholeReduceSumNpuDebug, MaskCountExceedUpperFloat)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(float));
    LocalTensor<float> srcLocal = srcBuf.Get<float>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(float));
    LocalTensor<float> dstLocal = dstBuf.Get<float>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceSum<float>(dstLocal, srcLocal, 65, 1, 1, 1, 8);
}

TEST_F(TestWholeReduceSumNpuDebug, RepeatTimeOverflow)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    WholeReduceSum<half>(dstLocal, srcLocal, 128, 256, 1, 1, 8);
}

// ============================================================
// BlockReduceMax NPU Debug validation - negative test cases
// BlockReduce dst alignment: half=16B, float=32B
// BlockReduce only has mask count mode (no mask bit overload for C220)
// ============================================================

class TestBlockReduceMaxNpuDebug : public testing::Test {
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

TEST_F(TestBlockReduceMaxNpuDebug, DstPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::A2> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceMax<half>(dstLocal, srcLocal, 128, 1, 8, 1, 8);
}

TEST_F(TestBlockReduceMaxNpuDebug, SrcPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceMax<half>(dstLocal, srcLocal, 128, 1, 8, 1, 8);
}

TEST_F(TestBlockReduceMaxNpuDebug, MaskCountZero)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceMax<half>(dstLocal, srcLocal, 0, 1, 8, 1, 8);
}

TEST_F(TestBlockReduceMaxNpuDebug, RepeatTimeOverflow)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceMax<half>(dstLocal, srcLocal, 128, 256, 8, 1, 8);
}

// ============================================================
// BlockReduceMin NPU Debug validation - negative test cases
// Same checks as BlockReduceMax
// ============================================================

class TestBlockReduceMinNpuDebug : public testing::Test {
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

TEST_F(TestBlockReduceMinNpuDebug, DstPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::A2> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceMin<half>(dstLocal, srcLocal, 128, 1, 8, 1, 8);
}

TEST_F(TestBlockReduceMinNpuDebug, SrcPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceMin<half>(dstLocal, srcLocal, 128, 1, 8, 1, 8);
}

TEST_F(TestBlockReduceMinNpuDebug, MaskCountZero)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceMin<half>(dstLocal, srcLocal, 0, 1, 8, 1, 8);
}

TEST_F(TestBlockReduceMinNpuDebug, RepeatTimeOverflow)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceMin<half>(dstLocal, srcLocal, 128, 256, 8, 1, 8);
}

// ============================================================
// BlockReduceSum NPU Debug validation - negative test cases
// Same checks as BlockReduceMax
// ============================================================

class TestBlockReduceSumNpuDebug : public testing::Test {
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

TEST_F(TestBlockReduceSumNpuDebug, DstPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::A2> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceSum<half>(dstLocal, srcLocal, 128, 1, 8, 1, 8);
}

TEST_F(TestBlockReduceSumNpuDebug, SrcPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceSum<half>(dstLocal, srcLocal, 128, 1, 8, 1, 8);
}

TEST_F(TestBlockReduceSumNpuDebug, MaskCountZero)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceSum<half>(dstLocal, srcLocal, 0, 1, 8, 1, 8);
}

TEST_F(TestBlockReduceSumNpuDebug, RepeatTimeOverflow)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    BlockReduceSum<half>(dstLocal, srcLocal, 128, 256, 8, 1, 8);
}

// ============================================================
// PairReduceSum NPU Debug validation - negative test cases
// PairReduceSum signature (mask count): (dst, src, repeatTime, mask, dstRepStride, srcBlkStride, srcRepStride)
// PairReduceSum signature (mask bit):   (dst, src, repeatTime, mask[], dstRepStride, srcBlkStride, srcRepStride)
// Validation: position (UB), alignment (dst/src 32B), mask, repeatTime
// Note: mask bit nullptr test omitted - ASCENDC_DEBUG_ASSERT does not terminate in UT,
//       causing subsequent code to dereference null pointer (same as WholeReduceMax MaskBitNullPtr).
// ============================================================

class TestPairReduceSumNpuDebug : public testing::Test {
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

// dst on L0A (not UB) - mask count mode
TEST_F(TestPairReduceSumNpuDebug, DstPositionNotUbMaskCount)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::A2> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    PairReduceSum<half>(dstLocal, srcLocal, 1, 128, 8, 1, 8);
}

// src on L1 (not UB) - mask count mode
TEST_F(TestPairReduceSumNpuDebug, SrcPositionNotUbMaskCount)
{
    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    PairReduceSum<half>(dstLocal, srcLocal, 1, 128, 8, 1, 8);
}

// mask count mode, mask=0 (reduce API requires mask >= 1)
TEST_F(TestPairReduceSumNpuDebug, MaskCountZero)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    PairReduceSum<half>(dstLocal, srcLocal, 1, 0, 8, 1, 8);
}

// mask count mode, mask=129 (exceeds half upper bound 128)
TEST_F(TestPairReduceSumNpuDebug, MaskCountExceedUpperHalf)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 256 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    PairReduceSum<half>(dstLocal, srcLocal, 1, 129, 8, 1, 8);
}

// mask count mode, mask=65 (exceeds float upper bound 64)
TEST_F(TestPairReduceSumNpuDebug, MaskCountExceedUpperFloat)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(float));
    LocalTensor<float> srcLocal = srcBuf.Get<float>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(float));
    LocalTensor<float> dstLocal = dstBuf.Get<float>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    PairReduceSum<float>(dstLocal, srcLocal, 1, 65, 8, 1, 8);
}

// repeatTime=256 (exceeds [0, 255])
TEST_F(TestPairReduceSumNpuDebug, RepeatTimeOverflow)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    PairReduceSum<half>(dstLocal, srcLocal, 256, 128, 8, 1, 8);
}

// ============================================================
// RepeatReduceSum NPU Debug validation - negative test cases
// RepeatReduceSum signature (mask count only): (dst, src, repeatTime, mask, dstBlkStride, srcBlkStride, dstRepStride, srcRepStride)
// Validation: position (dst/src UB), alignment (src 32B, dst half=2B/float=4B), mask count, repeatTime
// Note: Only mask count mode exists for C220.
// ============================================================

class TestRepeatReduceSumNpuDebug : public testing::Test {
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

// dst on L0A (not UB)
TEST_F(TestRepeatReduceSumNpuDebug, DstPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::A2> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    RepeatReduceSum<half>(dstLocal, srcLocal, 1, 64, 1, 1, 8, 8);
}

// src on L1 (not UB)
TEST_F(TestRepeatReduceSumNpuDebug, SrcPositionNotUb)
{
    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    RepeatReduceSum<half>(dstLocal, srcLocal, 1, 64, 1, 1, 8, 8);
}

// mask count mode, mask=0 (reduce API requires mask >= 1)
TEST_F(TestRepeatReduceSumNpuDebug, MaskCountZero)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    RepeatReduceSum<half>(dstLocal, srcLocal, 1, 0, 1, 1, 8, 8);
}

// mask count mode, mask=65 (exceeds float upper bound 64)
TEST_F(TestRepeatReduceSumNpuDebug, MaskCountExceedUpperFloat)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(float));
    LocalTensor<float> srcLocal = srcBuf.Get<float>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(float));
    LocalTensor<float> dstLocal = dstBuf.Get<float>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    RepeatReduceSum<float>(dstLocal, srcLocal, 1, 65, 1, 1, 8, 8);
}

// repeatTime=256 (exceeds [0, 255])
TEST_F(TestRepeatReduceSumNpuDebug, RepeatTimeOverflow)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> srcBuf;
    tpipe.InitBuffer(srcBuf, 128 * sizeof(half));
    LocalTensor<half> srcLocal = srcBuf.Get<half>();

    TBuf<TPosition::VECCALC> dstBuf;
    tpipe.InitBuffer(dstBuf, 16 * sizeof(half));
    LocalTensor<half> dstLocal = dstBuf.Get<half>();

    MOCKER(raise, int32_t (*)(int32_t)).stubs().will(invoke(RaiseStubForNpuDebug));
    RepeatReduceSum<half>(dstLocal, srcLocal, 256, 64, 1, 1, 8, 8);
}