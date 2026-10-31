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
#include "kernel_operator.h"

using namespace AscendC;

class VectorPointerTestsuite : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

TEST_F(VectorPointerTestsuite, AddPreservesMixedTensorBounds)
{
    TPipe pipe;
    TBuf<TPosition::VECCALC> dstBuffer, srcBuffer;
    pipe.InitBuffer(dstBuffer, 256);
    pipe.InitBuffer(srcBuffer, 32);
    auto dst = dstBuffer.Get<float>();
    auto src = srcBuffer.Get<float>();
    auto dstPtr = reinterpret_cast<float*>(dst.GetPhyAddr());
    EXPECT_DEATH(Add(dstPtr, src, src, int32_t(64)), "");
}

TEST_F(VectorPointerTestsuite, AddRejectsInvalidMask)
{
    TPipe pipe;
    TBuf<TPosition::VECCALC> dstBuffer, srcBuffer;
    pipe.InitBuffer(dstBuffer, 512);
    pipe.InitBuffer(srcBuffer, 512);
    auto dst = dstBuffer.Get<float>();
    auto src = srcBuffer.Get<float>();
    auto dstPtr = reinterpret_cast<float*>(dst.GetPhyAddr());
    auto srcPtr = reinterpret_cast<float*>(src.GetPhyAddr());
    BinaryRepeatParams params{1, 1, 1, 8, 8, 8};
    EXPECT_DEATH(Add(dstPtr, srcPtr, srcPtr, uint64_t(65), uint8_t(1), params), "");
}

TEST_F(VectorPointerTestsuite, AddRejectsMisalignedPointer)
{
    TPipe pipe;
    TBuf<TPosition::VECCALC> dstBuffer, srcBuffer;
    pipe.InitBuffer(dstBuffer, 512);
    pipe.InitBuffer(srcBuffer, 512);
    auto dst = dstBuffer.Get<float>();
    auto src = srcBuffer.Get<float>();
    auto dstPtr = reinterpret_cast<float*>(dst.GetPhyAddr());
    EXPECT_DEATH(Add(dstPtr + 1, src, src, int32_t(64)), "");
}

TEST_F(VectorPointerTestsuite, AddAcceptsValidMixedOperands)
{
    TPipe pipe;
    TBuf<TPosition::VECCALC> dstBuffer, srcBuffer;
    pipe.InitBuffer(dstBuffer, 256);
    pipe.InitBuffer(srcBuffer, 256);
    auto dst = dstBuffer.Get<float>();
    auto src = srcBuffer.Get<float>();
    for (uint32_t i = 0; i < 64; ++i) {
        src.SetValue(i, static_cast<float>(i));
    }
    auto dstPtr = reinterpret_cast<float*>(dst.GetPhyAddr());
    auto srcPtr = reinterpret_cast<float*>(src.GetPhyAddr());
    Add(dstPtr, srcPtr, src, int32_t(64));
    BinaryRepeatParams params{1, 1, 1, 8, 8, 8};
    Add(dstPtr, srcPtr, src, uint64_t(64), uint8_t(1), params);
}

TEST_F(VectorPointerTestsuite, ComparesAcceptsValidMixedOperands)
{
    TPipe pipe;
    TBuf<TPosition::VECCALC> dstBuffer, srcBuffer;
    pipe.InitBuffer(dstBuffer, 32);
    pipe.InitBuffer(srcBuffer, 256);
    auto dst = dstBuffer.Get<uint8_t>();
    auto src = srcBuffer.Get<float>();
    auto dstPtr = reinterpret_cast<uint8_t*>(dst.GetPhyAddr());
    UnaryRepeatParams params{1, 1, 8, 8};
    Compares(dstPtr, src, 1.0f, CMPMODE::EQ, uint64_t(64), uint8_t(1), params);
    Compares(dstPtr, src, 1.0f, CMPMODE::EQ, uint32_t(64));
}

TEST_F(VectorPointerTestsuite, TransDataTo5HDLeavesCpuAddressTablesUnchanged)
{
    TPipe pipe;
    TBuf<TPosition::VECCALC> srcBuffer, dstBuffer, srcTableBuffer, dstTableBuffer;
    pipe.InitBuffer(srcBuffer, 512);
    pipe.InitBuffer(dstBuffer, 512);
    pipe.InitBuffer(srcTableBuffer, 128);
    pipe.InitBuffer(dstTableBuffer, 128);
    auto src = srcBuffer.Get<uint16_t>();
    auto dst = dstBuffer.Get<uint16_t>();
    auto srcTable = srcTableBuffer.Get<uint64_t>();
    auto dstTable = dstTableBuffer.Get<uint64_t>();
    uint64_t savedSrc[16], savedDst[16];
    for (uint32_t i = 0; i < 16; ++i) {
        savedSrc[i] = reinterpret_cast<uint64_t>(src[i * 16].GetPhyAddr());
        savedDst[i] = reinterpret_cast<uint64_t>(dst[i * 16].GetPhyAddr());
        srcTable.SetValue(i, savedSrc[i]);
        dstTable.SetValue(i, savedDst[i]);
    }
    auto srcPtr = reinterpret_cast<uint64_t*>(srcTable.GetPhyAddr());
    auto dstPtr = reinterpret_cast<uint64_t*>(dstTable.GetPhyAddr());
    TransDataTo5HDParams params{false, false, 1, 16, 16};
    TransDataTo5HD<uint16_t>(dstPtr, srcPtr, params);
    for (uint32_t i = 0; i < 16; ++i) {
        EXPECT_EQ(srcPtr[i], savedSrc[i]);
        EXPECT_EQ(dstPtr[i], savedDst[i]);
    }
    TransDataTo5HD<uint16_t>(dstTable, srcPtr, params);
    for (uint32_t i = 0; i < 16; ++i) {
        EXPECT_EQ(srcPtr[i], savedSrc[i]);
        EXPECT_EQ(dstPtr[i], savedDst[i]);
    }
}

TEST_F(VectorPointerTestsuite, GetAbsAddrPointerMatchesTensorOffsets)
{
    TPipe pipe;
    TBuf<TPosition::VECCALC> ubBuffer;
    TBuf<TPosition::A1> l1Buffer;
    pipe.InitBuffer(ubBuffer, 256);
    pipe.InitBuffer(l1Buffer, 256);
    auto ub = ubBuffer.Get<float>();
    auto l1 = l1Buffer.Get<half>();
    for (uint32_t offset : {0, 1, 7, 31}) {
        auto ubView = ub[offset];
        auto l1View = l1[offset];
        auto ubPtr = reinterpret_cast<float*>(ubView.GetPhyAddr());
        auto l1Ptr = reinterpret_cast<half*>(l1View.GetPhyAddr());
        EXPECT_EQ(pipe.GetAbsAddr(ubPtr), pipe.GetAbsAddr(ubView));
        EXPECT_EQ(GetAbsAddr(&pipe, ubPtr), GetAbsAddr(&pipe, ubView));
        EXPECT_EQ((pipe.GetAbsAddr<half, Hardware::L1>(l1Ptr)), pipe.GetAbsAddr(l1View));
        EXPECT_EQ((GetAbsAddr<half, Hardware::L1>(&pipe, l1Ptr)), GetAbsAddr(&pipe, l1View));
    }
}
