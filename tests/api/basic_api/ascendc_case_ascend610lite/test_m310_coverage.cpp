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

using namespace AscendC;

namespace {
constexpr uint32_t UB_BYTES = 1024;
constexpr uint32_t GM_BYTES = 256;

struct VectorCtx {
    TPipe pipe;
    TBuf<TPosition::VECCALC> src0Buf;
    TBuf<TPosition::VECCALC> src1Buf;
    TBuf<TPosition::VECCALC> dstBuf;
    TBuf<TPosition::VECCALC> castDstBuf;
    TBuf<TPosition::VECCALC> selMaskBuf;
    TBuf<TPosition::VECCALC> gatherIndexBuf;
    TBuf<TPosition::VECCALC> reduceWorkBuf;

    VectorCtx()
    {
        pipe.InitBuffer(src0Buf, UB_BYTES);
        pipe.InitBuffer(src1Buf, UB_BYTES);
        pipe.InitBuffer(dstBuf, UB_BYTES);
        pipe.InitBuffer(castDstBuf, UB_BYTES);
        pipe.InitBuffer(selMaskBuf, UB_BYTES);
        pipe.InitBuffer(gatherIndexBuf, UB_BYTES);
        pipe.InitBuffer(reduceWorkBuf, UB_BYTES);
    }

    LocalTensor<half> Src0()
    {
        return src0Buf.Get<half>();
    }

    LocalTensor<half> Src1()
    {
        return src1Buf.Get<half>();
    }

    LocalTensor<half> Dst()
    {
        return dstBuf.Get<half>();
    }

    LocalTensor<float> CastDst()
    {
        return castDstBuf.Get<float>();
    }

    LocalTensor<uint8_t> SelMask()
    {
        return selMaskBuf.Get<uint8_t>();
    }

    LocalTensor<uint16_t> GatherIndex()
    {
        return gatherIndexBuf.Get<uint16_t>();
    }

    LocalTensor<half> ReduceWork()
    {
        return reduceWorkBuf.Get<half>();
    }
};
} // namespace

class M310CoverageTest : public testing::Test {
protected:
    void SetUp() override
    {
        SetGCoreType(AIV_TYPE);
    }

    void TearDown() override
    {
        CheckSyncState();
        SetGCoreType(MIX_TYPE);
    }
};

TEST_F(M310CoverageTest, CoversVecBinaryEventFetchPath)
{
    VectorCtx ctx;
    uint64_t mask[2] = {0, 0};
    BinaryRepeatParams binaryParams {1, 1, 1, 8, 8, 8};

    Add(ctx.Dst(), ctx.Src0(), ctx.Src1(), mask, 0, binaryParams);
    SUCCEED();
}

TEST_F(M310CoverageTest, CoversVecBinaryScalarEventFetchPath)
{
    VectorCtx ctx;
    uint64_t mask[2] = {0, 0};
    UnaryRepeatParams unaryParams {1, 1, 8, 8};

    Adds(ctx.Dst(), ctx.Src0(), static_cast<half>(1.0F), mask, 0, unaryParams);
    SUCCEED();
}

TEST_F(M310CoverageTest, CoversVecUnaryEventFetchPath)
{
    VectorCtx ctx;
    uint64_t mask[2] = {0, 0};
    UnaryRepeatParams unaryParams {1, 1, 8, 8};

    Abs(ctx.Dst(), ctx.Src0(), mask, 0, unaryParams);
    SUCCEED();
}

TEST_F(M310CoverageTest, CoversVecDuplicateEventFetchPath)
{
    VectorCtx ctx;
    uint64_t mask[2] = {0, 0};

    Duplicate(ctx.Dst(), static_cast<half>(1.0F), mask, 0, 1, 8);
    SUCCEED();
}

TEST_F(M310CoverageTest, CoversVecCastEventFetchPath)
{
    VectorCtx ctx;
    uint64_t mask[2] = {0, 0};
    UnaryRepeatParams unaryParams {1, 1, 8, 8};

    Cast(ctx.CastDst(), ctx.Src0(), RoundMode::CAST_NONE, mask, 0, unaryParams);
    SUCCEED();
}

TEST_F(M310CoverageTest, CoversVecCompareSelectEventFetchPath)
{
    VectorCtx ctx;
    uint64_t mask[2] = {0, 0};
    BinaryRepeatParams binaryParams {1, 1, 1, 8, 8, 8};

    Compare(ctx.SelMask(), ctx.Src0(), ctx.Src1(), CMPMODE::LT, mask, 0, {0, 1, 1, 0, 8, 8});
    Select(ctx.Dst(), ctx.SelMask(), ctx.Src0(), ctx.Src1(), SELMODE::VSEL_TENSOR_TENSOR_MODE, mask, 0, binaryParams);
    SUCCEED();
}

TEST_F(M310CoverageTest, CoversVecTernaryScalarEventFetchPath)
{
    VectorCtx ctx;
    uint64_t mask[2] = {0, 0};
    UnaryRepeatParams unaryParams {1, 1, 8, 8};

    Axpy(ctx.Dst(), ctx.Src0(), static_cast<half>(1.0F), mask, 0, unaryParams);
    SUCCEED();
}

TEST_F(M310CoverageTest, CoversVecReduceEventFetchPath)
{
    VectorCtx ctx;
    uint64_t mask[2] = {0, 0};

    WholeReduceSum(ctx.Dst(), ctx.Src0(), mask, 0, 1, 1, 8);
    ReduceMax(ctx.Dst(), ctx.Src0(), ctx.ReduceWork(), mask, 0, 1, false);
    SUCCEED();
}

TEST_F(M310CoverageTest, CoversVecGatherMaskEventFetchPath)
{
    VectorCtx ctx;

    uint64_t rsvdCnt = 0;
    GatherMask(ctx.Dst(), ctx.Src0(), ctx.GatherIndex(), false, 0, {1, 1, 0, 0}, rsvdCnt);

    EXPECT_EQ(rsvdCnt, 0U);
}

TEST_F(M310CoverageTest, CoversDataCopyPadUbToGmBaseAddrPaths)
{
    TPipe pipe;
    TBuf<TPosition::VECIN> srcBuf;
    pipe.InitBuffer(srcBuf, GM_BYTES);
    LocalTensor<uint8_t> srcLocal = srcBuf.Get<uint8_t>();

    uint8_t gm[GM_BYTES] = {};
    GlobalTensor<uint8_t> dstGlobal;
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ uint8_t*>(gm), GM_BYTES);

    DataCopyPad(dstGlobal, srcLocal, DataCopyParams {1, 1, 0, 0});
    DataCopyPad(dstGlobal, srcLocal, DataCopyExtParams {1, 32, 0, 0, 0});

    EXPECT_EQ(gm[0], 0U);
}

TEST_F(M310CoverageTest, CoversFixpipeL0CToL1BaseAddrPath)
{
    TPipe pipe;
    TBuf<TPosition::A1> dstBuf;
    TBuf<TPosition::CO1> srcBuf;
    pipe.InitBuffer(dstBuf, UB_BYTES);
    pipe.InitBuffer(srcBuf, UB_BYTES);

    LocalTensor<half> dst = dstBuf.Get<half>();
    LocalTensor<float> src = srcBuf.Get<float>();
    FixpipeParamsM310 params;
    params.nSize = 16;
    params.mSize = 16;
    params.srcStride = 16;
    params.dstStride = 16;
    params.quantPre = QuantMode_t::F322F16;

    CheckFixpipeL0C2L1Param<half, float, CFG_ROW_MAJOR>(
        reinterpret_cast<__cbuf__ half*>(dst.GetPhyAddr()), reinterpret_cast<__cc__ float*>(src.GetPhyAddr()), params);

    SUCCEED();
}
