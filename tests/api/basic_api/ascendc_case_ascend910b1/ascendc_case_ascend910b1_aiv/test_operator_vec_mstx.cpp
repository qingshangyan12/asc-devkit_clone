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
#include <string.h>
 
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

 template <typename T>
MstxTensor::MstxTensorAddressSpace GetTensorSpace(const LocalTensor<T>& dst)
{
    Hardware dstHWPos = GetPhyType((TPosition)dst.GetPosition());
    if (dstHWPos == Hardware::GM) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_GM;
    } else if (dstHWPos == Hardware::UB) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_UB;
    } else if (dstHWPos == Hardware::L1) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_L1;
    } else if (dstHWPos == Hardware::L0A) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_L0A;
    } else if (dstHWPos == Hardware::L0B) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_L0B;
    } else if (dstHWPos == Hardware::L0C) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_L0C;
    }
}

void CheckResult(MstxTensor::MstxVecUnaryDesc& expect_desc, MstxTensor::MstxVecUnaryDesc& g_vec_unary)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_unary.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_unary.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_unary.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_unary.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_vec_unary.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_vec_unary.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_vec_unary.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_vec_unary.src.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_unary.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_unary.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_unary.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_unary.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_unary.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.blockNum, g_vec_unary.blockNum);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_unary.repeatTimes);
    EXPECT_EQ(expect_desc.dstBlockStride, g_vec_unary.dstBlockStride);
    EXPECT_EQ(expect_desc.srcBlockStride, g_vec_unary.srcBlockStride);
    EXPECT_EQ(expect_desc.dstRepeatStride, g_vec_unary.dstRepeatStride);
    EXPECT_EQ(expect_desc.srcRepeatStride, g_vec_unary.srcRepeatStride);
    EXPECT_EQ(expect_desc.name[0], g_vec_unary.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_unary.name[1]);
}

void CheckResult(MstxTensor::MstxVecBinaryDesc& expect_desc, MstxTensor::MstxVecBinaryDesc& g_vec_unary)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_unary.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_unary.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_unary.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_unary.dst.dataBits);
    EXPECT_EQ(expect_desc.src0.space, g_vec_unary.src0.space);
    EXPECT_EQ(expect_desc.src0.addr, g_vec_unary.src0.addr);
    EXPECT_EQ(expect_desc.src0.size, g_vec_unary.src0.size);
    EXPECT_EQ(expect_desc.src0.dataBits, g_vec_unary.src0.dataBits);
    EXPECT_EQ(expect_desc.src1.space, g_vec_unary.src1.space);
    EXPECT_EQ(expect_desc.src1.addr, g_vec_unary.src1.addr);
    EXPECT_EQ(expect_desc.src1.size, g_vec_unary.src1.size);
    EXPECT_EQ(expect_desc.src1.dataBits, g_vec_unary.src1.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_unary.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_unary.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_unary.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_unary.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_unary.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.blockNum, g_vec_unary.blockNum);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_unary.repeatTimes);
    EXPECT_EQ(expect_desc.dstBlockStride, g_vec_unary.dstBlockStride);
    EXPECT_EQ(expect_desc.src0BlockStride, g_vec_unary.src0BlockStride);
    EXPECT_EQ(expect_desc.src1BlockStride, g_vec_unary.src1BlockStride);
    EXPECT_EQ(expect_desc.dstRepeatStride, g_vec_unary.dstRepeatStride);
    EXPECT_EQ(expect_desc.src0RepeatStride, g_vec_unary.src0RepeatStride);
    EXPECT_EQ(expect_desc.src1RepeatStride, g_vec_unary.src1RepeatStride);
    EXPECT_EQ(expect_desc.name[0], g_vec_unary.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_unary.name[1]);
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

    uint64_t maskBit[2] = {0x77, 0x88};
    uint64_t maskCounter = 123;//mask相等时，mask0是18446744073709551615，mask1是576460752303423487
    uint8_t repeatTimes = 5;
    UnaryRepeatParams repeatParams = {2, 2, 6, 6};
    int32_t count = 456;
    T scalar = 2;
    RoundMode roundMode = RoundMode::CAST_ROUND;
    MstxTensor::MstxVecUnaryDesc expect_desc;

    expect_desc.dst.space = GetTensorSpace(outputLocal);
    expect_desc.dst.addr = reinterpret_cast<uint64_t>(outputLocal.GetPhyAddr());
    expect_desc.dst.size = outputLocal.GetSize();
    expect_desc.dst.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));
    expect_desc.src.space = GetTensorSpace(inputLocal);
    expect_desc.src.addr = reinterpret_cast<uint64_t>(inputLocal.GetPhyAddr());
    expect_desc.src.size = inputLocal.GetSize();
    expect_desc.src.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));
    expect_desc.wapper.maskMode = MstxTensor::MSTX_MASK_FROM_REG;
    expect_desc.wapper.useMask = true;
    expect_desc.wapper.reserveBufSize = 0;
    expect_desc.blockNum = repeatParams.blockNumber;
    expect_desc.repeatTimes = repeatTimes;
    expect_desc.dstBlockStride = repeatParams.dstBlkStride;
    expect_desc.srcBlockStride = repeatParams.srcBlkStride;
    expect_desc.dstRepeatStride = repeatParams.dstRepStride;
    expect_desc.srcRepeatStride = repeatParams.srcRepStride;
    
    //mask是uint64
    expect_desc.wapper.mask.mask0 = 18446744073709551615;
    expect_desc.wapper.mask.mask1 = 576460752303423487;
    CopyName(expect_desc.name, "Relu");
    Relu(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Exp");
    Exp(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Ln");
    Ln(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Abs");
    Abs(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Reciprocal");
    Reciprocal(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Sqrt");
    Sqrt(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Rsqrt");
    Rsqrt(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Not");
    Not(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Adds");
    Adds(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Muls");
    Muls(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Maxs");
    Maxs(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Mins");
    Mins(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftLeft");
    ShiftLeft(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftRight");
    ShiftRight(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "LeakyRelu");
    LeakyRelu(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Axpy");
    Axpy(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Cast");
    Cast(outputLocal, inputLocal, roundMode, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);


    //mask是[]
    expect_desc.wapper.mask.mask0 = maskBit[0];
    expect_desc.wapper.mask.mask1 = maskBit[1];
    CopyName(expect_desc.name, "Relu");
    Relu(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Exp");
    Exp(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Ln");
    Ln(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Abs");
    Abs(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Reciprocal");
    Reciprocal(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Sqrt");
    Sqrt(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Rsqrt");
    Rsqrt(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Not");
    Not(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Adds");
    Adds(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Muls");
    Muls(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Maxs");
    Maxs(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Mins");
    Mins(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftLeft");
    ShiftLeft(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftRight");
    ShiftRight(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "LeakyRelu");
    LeakyRelu(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Axpy");
    Axpy(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Cast");
    Cast(outputLocal, inputLocal, roundMode,  maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

    //count
    expect_desc.wapper.mask.mask0 = count;
    expect_desc.wapper.mask.mask1 = 0;
    expect_desc.blockNum = 8;
    expect_desc.repeatTimes = 1;
    expect_desc.dstBlockStride = 1;
    expect_desc.srcBlockStride = 1;
    expect_desc.dstRepeatStride = 8;
    expect_desc.srcRepeatStride = 8;
    expect_desc.wapper.maskMode = MstxTensor::MSTX_MASK_COUNT;
    CopyName(expect_desc.name, "Relu");
    Relu(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Exp");
    Exp(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Ln");
    Ln(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Abs");
    Abs(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Reciprocal");
    Reciprocal(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Sqrt");
    Sqrt(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Rsqrt");
    Rsqrt(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Not");
    Not(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Adds");
    Adds(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Muls");
    Muls(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Maxs");
    Maxs(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Mins");
    Mins(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftLeft");
    ShiftLeft(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftRight");
    ShiftRight(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "LeakyRelu");
    LeakyRelu(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Axpy");
    Axpy(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Cast");
    Cast(outputLocal, inputLocal, roundMode, dataSize);
    CheckResult(expect_desc, g_vec_unary);



    Exp(outputLocal, inputLocal, dataSize);


    set_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);
    wait_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dataSize);
    pipe_barrier(PIPE_ALL);
}

template <typename T>
void MainVecBinaryMstx02(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, __gm__ int32_t dataSize)
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

    MstxVecBinaryDesc repeatParams = { 2, 2, 2, 6, 6, 6 };
    uint64_t maskBit[2] = {0x77, 0x88};
    uint64_t maskCounter = 123;//mask相等时，mask0是18446744073709551615，mask1是576460752303423487
    uint8_t repeatTimes = 5;
    int32_t count = 456;
    MstxTensor::MstxVecBinaryDesc expect_desc;

    expect_desc.dst.space = GetTensorSpace(outputLocal);
    expect_desc.dst.addr = reinterpret_cast<uint64_t>(outputLocal.GetPhyAddr());
    expect_desc.dst.size = outputLocal.GetSize();
    expect_desc.dst.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));
    expect_desc.src0.space = GetTensorSpace(inputLocal);
    expect_desc.src0.addr = reinterpret_cast<uint64_t>(inputLocal.GetPhyAddr());
    expect_desc.src0.size = inputLocal.GetSize();
    expect_desc.src0.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));
    expect_desc.src1 = expect_desc.src0;
    expect_desc.wapper.maskMode = MstxTensor::MSTX_MASK_FROM_REG;
    expect_desc.wapper.useMask = true;
    expect_desc.wapper.reserveBufSize = 0;
    expect_desc.blockNum = repeatParams.blockNumber;
    expect_desc.repeatTimes = repeatTimes;
    expect_desc.dstBlockStride = repeatParams.dstBlkStride;
    expect_desc.src0BlockStride = repeatParams.src0BlkStride;
    expect_desc.src1BlockStride = repeatParams.src1BlkStride;
    expect_desc.dstRepeatStride = repeatParams.dstRepStride;
    expect_desc.src0RepeatStride = repeatParams.src0RepStride;
    expect_desc.src1RepeatStride = repeatParams.src1RepStride;

    //mask是uint64
    expect_desc.wapper.mask.mask0 = 18446744073709551615;
    expect_desc.wapper.mask.mask1 = 576460752303423487;
    CopyName(expect_desc.name, "And");
    And(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Or");
    Or(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Add");
    Add(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Sub");
    Sub(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Mul");
    Mul(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Max");
    Max(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Min");
    Min(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Div");
    Div(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "AddRelu");
    AddRelu(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "SubRelu");
    SubRelu(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "AddReluCast");
    AddReluCast(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "SubReluCast");
    SubReluCast(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

 

    //mask是[]
    expect_desc.wapper.mask.mask0 = maskBit[0];
    expect_desc.wapper.mask.mask1 = maskBit[1];
    CopyName(expect_desc.name, "And");
    And(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Or");
    Or(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Add");
    Add(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Sub");
    Sub(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Mul");
    Mul(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Max");
    Max(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Min");
    Min(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Div");
    Div(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "AddRelu");
    AddRelu(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "SubRelu");
    SubRelu(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "AddReluCast");
    AddReluCast(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "SubReluCast");
    SubReluCast(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);


    //count
    expect_desc.wapper.mask.mask0 = count;
    expect_desc.wapper.mask.mask1 = 0;
    expect_desc.blockNum = 8;
    expect_desc.repeatTimes = 1;
    expect_desc.dstBlockStride = 1;
    expect_desc.srcBlockStride = 1;
    expect_desc.dstRepeatStride = 8;
    expect_desc.srcRepeatStride = 8;
    expect_desc.wapper.maskMode = MstxTensor::MSTX_MASK_COUNT;
    CopyName(expect_desc.name, "And");
    And(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Or");
    Or(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Add");
    Add(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Sub");
    Sub(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Mul");
    Mul(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Max");
    Max(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Min");
    Min(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Div");
    Div(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "AddRelu");
    AddRelu(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "SubRelu");
    SubRelu(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "AddReluCast");
    AddReluCast(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "SubReluCast");
    SubReluCast(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);

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
    UnaryTestParamsMstx { 208, 4, MainVecUnaryMstx01<float, false> }, UnaryTestParamsMstx { 208, 4, MainVecBinaryMstx02<int32_t> }, 
    UnaryTestParamsMstx { 208, 4, MainVecBinaryMstx02<half> }, UnaryTestParamsMstx { 208, 4, MainVecBinaryMstx02<float> },
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
