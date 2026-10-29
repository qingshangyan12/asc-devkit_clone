/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file test_operator_loaddatawithstride_pointer_args.cpp
 * \brief The operands and descriptor LoadDataWithStride's pointer form hands to the L1 -> L0A backend,
 * against the Tensor form's.
 *
 * The backend is mocked, so the comparison is on what each arm passes: destination, source and every
 * descriptor field the instruction reads.  The expected values come from the tensors this test built
 * and the descriptor it set, not from the code under test.
 *
 * Scope: under ASCENDC_CPU_DEBUG the address-space qualifiers are empty macros, so the L0B and UB
 * backends collapse into the L0A one; this file covers the single backend the collapsed build
 * reaches, not the address-space dispatch, which only the device build exercises.
 *
 * The Tensor form runs its descriptor checks first; the pointer form does not (a raw pointer carries
 * no position or alignment to check).  That asymmetry is out of scope here, so raise is stubbed to
 * keep a rejected descriptor from ending the process -- the comparison below is about the arguments
 * that reach the backend, which is reached either way.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "kernel_operator.h"
#include "stub_fun.h"

/* The template arguments carry commas, which the MOCKER macro would split; expand them first. */
#define STRIDE_CAL_FLOAT32 LoadData3DV2L12L0AWithStrideCal<float>

namespace AscendC {
namespace {
struct StrideCapture {
    uint64_t dst = 0;
    uint64_t src = 0;
    LoadData3DParamsV2<float> params{};
    uint32_t hits = 0;
};

StrideCapture g_strideCapture;

void CaptureStride(float* dst, float* src, const LoadData3DParamsV2<float>& params)
{
    g_strideCapture.dst = reinterpret_cast<uint64_t>(dst);
    g_strideCapture.src = reinterpret_cast<uint64_t>(src);
    g_strideCapture.params = params;
    ++g_strideCapture.hits;
}

int32_t RaiseStub(int32_t signalNumber)
{
    (void)signalNumber;
    return 0;
}

constexpr uint32_t dstOffset = 0x400;
constexpr uint32_t srcOffset = 0x800;
constexpr uint32_t bufferSize = 512;
} // namespace

class TEST_LOADDATAWITHSTRIDE_POINTER_ARGS : public testing::Test {
protected:
    void SetUp()
    {
        SetGCoreType(1);
        g_strideCapture = StrideCapture{};
        MOCKER(raise, int32_t(*)(int32_t)).stubs().will(invoke(RaiseStub));
    }
    void TearDown()
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }

    static LoadData3DParamsV2<float> MakeParams()
    {
        LoadData3DParamsV2<float> params;
        params.l1H = 2;
        params.l1W = 8;
        params.channelSize = 8;
        params.kExtension = 8;
        params.mExtension = 16;
        params.kStartPt = 0;
        params.mStartPt = 0;
        params.strideW = 1;
        params.strideH = 1;
        params.filterW = 1;
        params.filterH = 1;
        params.dilationFilterW = 1;
        params.dilationFilterH = 1;
        params.enTranspose = false;
        params.enSmallK = false;
        params.padValue = 7.0f;
        params.filterSizeW = 0;
        params.filterSizeH = 0;
        params.fMatrixCtrl = false;
        uint8_t padList[PAD_SIZE] = {0, 0, 0, 0};
        for (uint32_t i = 0; i < PAD_SIZE; ++i) {
            params.padList[i] = padList[i];
        }
        return params;
    }
};

/* L1 -> L0A: both arms reach the backend with the same addresses and the same descriptor. */
TEST_F(TEST_LOADDATAWITHSTRIDE_POINTER_ARGS, OperandsAndDescriptor)
{
    LocalTensor<float> dst;
    TBuffAddr dstAddr;
    dstAddr.logicPos = static_cast<uint8_t>(TPosition::A2);
    dst.SetAddr(dstAddr);
    dst.InitBuffer(dstOffset, bufferSize);

    LocalTensor<float> src;
    TBuffAddr srcAddr;
    srcAddr.logicPos = static_cast<uint8_t>(TPosition::A1);
    src.SetAddr(srcAddr);
    src.InitBuffer(srcOffset, bufferSize);

    const LoadData3DParamsV2<float> params = MakeParams();
    const uint64_t expectedDst = reinterpret_cast<uint64_t>(dst.GetPhyAddr());
    const uint64_t expectedSrc = reinterpret_cast<uint64_t>(src.GetPhyAddr());

    MOCKER(STRIDE_CAL_FLOAT32, void (*)(float*, float*, const LoadData3DParamsV2<float>&))
        .stubs()
        .will(invoke(CaptureStride));

    LoadDataWithStride(dst, src, params);
    ASSERT_EQ(g_strideCapture.hits, 1U);
    EXPECT_EQ(g_strideCapture.dst, expectedDst);
    EXPECT_EQ(g_strideCapture.src, expectedSrc);
    const LoadData3DParamsV2<float> tensorParams = g_strideCapture.params;

    g_strideCapture = StrideCapture{};
    LoadDataWithStride(
        reinterpret_cast<__ca__ float*>(expectedDst), reinterpret_cast<__cbuf__ float*>(expectedSrc), params);
    ASSERT_EQ(g_strideCapture.hits, 1U);
    EXPECT_EQ(g_strideCapture.dst, expectedDst);
    EXPECT_EQ(g_strideCapture.src, expectedSrc);
    EXPECT_EQ(g_strideCapture.params.l1H, tensorParams.l1H);
    EXPECT_EQ(g_strideCapture.params.l1W, tensorParams.l1W);
    EXPECT_EQ(g_strideCapture.params.channelSize, tensorParams.channelSize);
    EXPECT_EQ(g_strideCapture.params.kExtension, tensorParams.kExtension);
    EXPECT_EQ(g_strideCapture.params.mExtension, tensorParams.mExtension);
    EXPECT_EQ(g_strideCapture.params.kStartPt, tensorParams.kStartPt);
    EXPECT_EQ(g_strideCapture.params.mStartPt, tensorParams.mStartPt);
    EXPECT_EQ(g_strideCapture.params.strideW, tensorParams.strideW);
    EXPECT_EQ(g_strideCapture.params.strideH, tensorParams.strideH);
    EXPECT_EQ(g_strideCapture.params.enTranspose, tensorParams.enTranspose);
    EXPECT_EQ(g_strideCapture.params.padValue, tensorParams.padValue);
}
} // namespace AscendC
