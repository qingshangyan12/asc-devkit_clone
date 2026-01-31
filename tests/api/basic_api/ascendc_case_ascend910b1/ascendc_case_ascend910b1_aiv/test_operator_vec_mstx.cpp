/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#define __MSTX_DFX_REPORT__
#include <cstdint>
extern "C" void __mstx_dfx_report_stub(int type, uint8_t size, void* desc);
#include <gtest/gtest.h>

#include "kernel_log.h"
static uint8_t g_testRes = 1;      // 全局变量记录运行结果, 如果进入ASCENDC_ASSERT报错，会被置为0
// 重定义ASCENDC_ASSERT，不Abort，仅修改全局变量通知进入报错分支
#undef ASCENDC_ASSERT
#define ASCENDC_ASSERT(cond, behavior) \
    do {                               \
        if (!(cond)) {                 \
            g_testRes = 0;              \
            behavior;                  \
        }                              \
    } while (0)

#undef ASCENDC_REPORT_CHECK_ERROR
#define ASCENDC_REPORT_CHECK_ERROR(apiMsg, funcType)   \
    do {                                                \
        g_testRes = 0;                                   \
    } while (0)

#include "kernel_utils.h"
#include "kernel_operator.h"

using namespace std;
using namespace AscendC;

MstxTensor::MstxVecUnaryDesc g_vec_unary;
MstxTensor::MstxVecBinaryDesc g_vec_binary;
extern "C" void __mstx_dfx_report_stub(int type, uint8_t size, void* desc) {
    if (type == 3000){
        if (desc && size == sizeof(MstxTensor::MstxVecUnaryDesc)) {
            g_vec_unary = *static_cast<MstxTensor::MstxVecUnaryDesc*>(desc);
        }
    } else if  (type == 3001) {
        if (desc && size == sizeof(MstxTensor::MstxVecBinaryDesc)) {
            g_vec_binary = *static_cast<MstxTensor::MstxVecBinaryDesc*>(desc);
        }
    }
}

void CopyName(char b[64], const char* a)
{
    uint32_t i = 0;
    for (; i < 63; ++i){
        b[i] = a[i];
        if (a[i] == '\0') {
            break;
        }
    }
    b[i] = '\0';
}

void CheckResult(MstxTensor::MstxVecUnaryDesc& origin_desc, MstxTensor::MstxVecUnaryDesc& g_vec_unary)
{
    EXPECT_EQ(origin_desc.dst.addr, g_vec_unary.dst.addr);
    EXPECT_EQ(origin_desc.dst.size, g_vec_unary.dst.size);
    EXPECT_EQ(origin_desc.dst.dataBits, g_vec_unary.dst.dataBits);
    EXPECT_EQ(origin_desc.dst.addr, g_vec_unary.dst.addr);
    EXPECT_EQ(origin_desc.dst.addr, g_vec_unary.dst.addr);

    EXPECT_EQ(origin_desc.dst.addr, g_vec_unary.dst.addr);
    EXPECT_EQ(origin_desc.dst.addr, g_vec_unary.dst.addr);
    EXPECT_EQ(origin_desc.dst.addr, g_vec_unary.dst.addr);

}

template <typename T, bool USE_CAST_DEQ>
void MainVecUnaryMstx01(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, __gm__ int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::VECCALC> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> inputLocal = tbuf.Get<T>();

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize * sizeof(int16_t));
    LocalTensor<int16_t> inputLocal2 = tbuf1.Get<int16_t>();

    TBuf<TPosition::VECCALC> tbuf2;
    tpipe.InitBuffer(tbuf2, dataSize * sizeof(T));
    LocalTensor<T> outputLocal = tbuf2.Get<T>();

    TBuf<TPosition::VECCALC> tbuf3;
    tpipe.InitBuffer(tbuf3, dataSize * 2 * sizeof(int8_t));
    LocalTensor<int8_t> outputLocal2 = tbuf3.Get<int8_t>();

    TBuf<TPosition::VECCALC> tbuf4;
    tpipe.InitBuffer(tbuf4, dataSize * 2 * sizeof(uint8_t));
    LocalTensor<uint8_t> outputLocal3 = tbuf4.Get<uint8_t>();

    AscendCUtils::SetMask<uint8_t>(256);

    DataCopy(inputLocal, inputGlobal, dataSize);

    set_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);
    AscendCUtils::SetMask<uint8_t>(128);
    wait_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);

    uint64_t maskBit[2] = {0xff, 0x00};
    uint64_t maskCounter = 123;
    uint8_t repeatTimes = 8;
    UnaryRepeatParams repeatParams = {1, 1, 8, 8};
    int32_t count = 456;
    MstxTensor::MstxVecUnaryDesc origin_desc;

    origin_desc.dst.addr = reinterpret_cast<uint64_t>(outputLocal.GetPhyAddr());
    origin_desc.dst.size = outputLocal.GetSize();
    origin_desc.dst.dataBits = static_cast<uint8_t>(sizeof(T));
    origin_desc.src.addr = reinterpret_cast<uint64_t>(inputLocal.GetPhyAddr());
    origin_desc.src.size = inputLocal.GetSize();
    origin_desc.src.dataBits = static_cast<uint8_t>(sizeof(T));
    origin_desc.wapper.maskMode = MstxTensor::MSTX_MASK_FROM_REG;
    origin_desc.wapper.mask.mask0 = maskCounter;
    origin_desc.wapper.mask.mask1 = 0;
    origin_desc.wapper.useMask = true;
    origin_desc.blockNum = repeatParams.blockNumber;
    origin_desc.repeatTimes = repeatTimes;
    origin_desc.dstBlockStride = repeatParams.dstBlkStride;
    origin_desc.srcBlockStride = repeatParams.srcBlkStride;
    origin_desc.dstRepeatStride = repeatParams.dstRepStride;
    origin_desc.srcRepeatStride = repeatParams.srcRepStride;
    CopyName(origin_desc.name, "Relu");

    Relu(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);



    Exp(outputLocal, inputLocal, dataSize);
    Ln(outputLocal, inputLocal, dataSize);
    Abs(outputLocal, inputLocal, dataSize);
    Reciprocal(outputLocal, inputLocal, dataSize);
    Rsqrt(outputLocal, inputLocal, dataSize);
    Sqrt(outputLocal, inputLocal, dataSize);
    if constexpr (USE_CAST_DEQ){
        CastDeq<int8_t, int16_t, false, false>(outputLocal2, inputLocal2, dataSize);
        CastDeq<uint8_t, int16_t, false, true>(outputLocal3, inputLocal2, dataSize);
        CastDeq<int8_t, int16_t, true, false>(outputLocal2, inputLocal2, dataSize);
        CastDeq<uint8_t, int16_t, true, true>(outputLocal3, inputLocal2, dataSize);
        CastDeq<uint8_t, int16_t, true, true>(outputLocal3, inputLocal2, 1024);
        int maskCount = 32;
        UnaryRepeatParams repeatParams;
        CastDeq<int8_t, int16_t, true, false, false>(outputLocal2, inputLocal2, maskCount, 2, repeatParams);
        CastDeq<uint8_t, int16_t, true, false, true>(outputLocal3, inputLocal2, maskCount, 2, repeatParams);
        CastDeq<int8_t, int16_t, true, true, false>(outputLocal2, inputLocal2, maskCount, 2, repeatParams);
        CastDeq<uint8_t, int16_t, true, true, true>(outputLocal3, inputLocal2, maskCount, 2, repeatParams);
        uint64_t maskVec[2] = {0xff, 0};
        CastDeq<int8_t, int16_t, true, false, false>(outputLocal2, inputLocal2, maskVec, 2, repeatParams);
        CastDeq<uint8_t, int16_t, true, false, true>(outputLocal3, inputLocal2, maskVec, 2, repeatParams);
        CastDeq<int8_t, int16_t, true, true, false>(outputLocal2, inputLocal2, maskVec, 2, repeatParams);
        CastDeq<uint8_t, int16_t, true, true, true>(outputLocal3, inputLocal2, maskVec, 2, repeatParams);
    }else{
        CastDequant<int8_t, int16_t, false, false>(outputLocal2, inputLocal2, dataSize);
        CastDequant<uint8_t, int16_t, false, true>(outputLocal3, inputLocal2, dataSize);
        CastDequant<int8_t, int16_t, true, false>(outputLocal2, inputLocal2, dataSize);
        CastDequant<uint8_t, int16_t, true, true>(outputLocal3, inputLocal2, dataSize);
        CastDequant<uint8_t, int16_t, true, true>(outputLocal3, inputLocal2, 1024);
        int maskCount = 32;
        UnaryRepeatParams repeatParams;
        CastDequant<int8_t, int16_t, true, false, false>(outputLocal2, inputLocal2, maskCount, 2, repeatParams);
        CastDequant<uint8_t, int16_t, true, false, true>(outputLocal3, inputLocal2, maskCount, 2, repeatParams);
        CastDequant<int8_t, int16_t, true, true, false>(outputLocal2, inputLocal2, maskCount, 2, repeatParams);
        CastDequant<uint8_t, int16_t, true, true, true>(outputLocal3, inputLocal2, maskCount, 2, repeatParams);
        uint64_t maskVec[2] = {0xff, 0};
        CastDequant<int8_t, int16_t, true, false, false>(outputLocal2, inputLocal2, maskVec, 2, repeatParams);
        CastDequant<uint8_t, int16_t, true, false, true>(outputLocal3, inputLocal2, maskVec, 2, repeatParams);
        CastDequant<int8_t, int16_t, true, true, false>(outputLocal2, inputLocal2, maskVec, 2, repeatParams);
        CastDequant<uint8_t, int16_t, true, true, true>(outputLocal3, inputLocal2, maskVec, 2, repeatParams);
    }


    set_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);
    wait_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dataSize);
    pipe_barrier(PIPE_ALL);
}

template <typename T>
void MainVecUnaryMstx02(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, __gm__ int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::VECCALC> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> inputLocal = tbuf.Get<T>();

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize * sizeof(T));
    LocalTensor<T> outputLocal = tbuf1.Get<T>();

    AscendCUtils::SetMask<uint8_t>(256);

    DataCopy(inputLocal, inputGlobal, dataSize);

    set_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);
    AscendCUtils::SetMask<uint8_t>(128);
    wait_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);

    Relu(outputLocal, inputLocal, dataSize);

    set_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);
    wait_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dataSize);
    pipe_barrier(PIPE_ALL);
}

template <typename T>
void MainVecUnaryMstx03(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, __gm__ int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::VECCALC> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> inputLocal = tbuf.Get<T>();

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize * sizeof(T));
    LocalTensor<T> outputLocal = tbuf1.Get<T>();

    AscendCUtils::SetMask<uint8_t>(256);

    DataCopy(inputLocal, inputGlobal, dataSize);

    set_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);
    AscendCUtils::SetMask<uint8_t>(128);
    wait_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);

    Not(outputLocal, inputLocal, dataSize);

    set_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);
    wait_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dataSize);
    pipe_barrier(PIPE_ALL);
}

template <typename T>
void MainVecUnaryMstx04(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, __gm__ int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::VECCALC> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> inputLocal = tbuf.Get<T>();

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize * sizeof(T));
    LocalTensor<T> outputLocal = tbuf1.Get<T>();

    AscendCUtils::SetMask<uint8_t>(256);

    DataCopy(inputLocal, inputGlobal, dataSize);

    set_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);
    AscendCUtils::SetMask<uint8_t>(128);
    wait_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);

    Abs(outputLocal, inputLocal, dataSize);

    set_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);
    wait_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dataSize);
    pipe_barrier(PIPE_ALL);
}

struct UnaryTestParamsMstx {
    int32_t dataSize;
    int32_t dataBitSize;
    void (*CalFunc)(uint8_t*, uint8_t*, int32_t);
};

class UnarySimpleMstxTestsuite : public testing::Test, public testing::WithParamInterface<UnaryTestParamsMstx> {
protected:
    void SetUp()
    {
        AscendC::SetGCoreType(2);
    }
    void TearDown()
    {
        AscendC::SetGCoreType(0);
    }
};

INSTANTIATE_TEST_CASE_P(TEST_UNARY_SIMPLE_MSTX, UnarySimpleMstxTestsuite,
    ::testing::Values(UnaryTestParamsMstx { 208, 2, MainVecUnaryMstx01<half, true> },
    UnaryTestParamsMstx { 208, 4, MainVecUnaryMstx01<float, true> }, UnaryTestParamsMstx { 208, 2, MainVecUnaryMstx01<half, false> }, 
    UnaryTestParamsMstx { 208, 4, MainVecUnaryMstx01<float, false> }, UnaryTestParamsMstx { 208, 4, MainVecUnaryMstx02<int32_t> }, 
    UnaryTestParamsMstx { 208, 4, MainVecUnaryMstx02<half> }, UnaryTestParamsMstx { 208, 4, MainVecUnaryMstx02<float> },
    UnaryTestParamsMstx { 208, 2, MainVecUnaryMstx03<int16_t> }, UnaryTestParamsMstx { 208, 2, MainVecUnaryMstx03<uint16_t> },
    UnaryTestParamsMstx { 208, 2, MainVecUnaryMstx04<int16_t> }));

TEST_P(UnarySimpleMstxTestsuite, UnarySimpleMstxTestCase)
{
    auto param = GetParam();
    uint8_t srcGm[param.dataSize * param.dataBitSize];
    uint8_t dstGm[param.dataSize * param.dataBitSize];

    param.CalFunc(srcGm, dstGm, param.dataSize);
    for (int32_t i = 0; i < param.dataSize; i++) {
        EXPECT_EQ(dstGm[i], 0x00);
    }
}
