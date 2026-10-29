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
 * \file test_operator_loaddata_pointer_args.cpp
 * \brief The argument the LoadData pointer form hands to the backend, against the Tensor form's.
 *
 * Both forms are supposed to reach LoadData2DL12L0{A,B}Cal with the same destination address,
 * source address and descriptor.  That claim is observable in the CPU-debug build even though the
 * backend computes nothing: mock the backend, run each form once, and compare what each passed.
 *
 * The expected values are constructed here (buffer addresses from InitBuffer, descriptor fields
 * set below), not recomputed from the code under test, so the assertion can disagree with the
 * implementation.
 *
 * Scope: under ASCENDC_CPU_DEBUG the address-space qualifiers are empty macros, so __ca__ and
 * __cb__ destinations select the same overload here; this file checks the backend call that
 * survives that collapse (L1 -> L0A) and does NOT check the address-space dispatch itself, which
 * only the device build can exercise.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "kernel_operator.h"
#include "stub_fun.h"

namespace AscendC {
namespace {
struct LoadDataCapture {
    uint64_t dst = 0;
    uint64_t src = 0;
    LoadData2DParams params{};
    uint32_t hits = 0;
};

LoadDataCapture g_capture;

void CaptureLoadData2D(float* dst, float* src, const LoadData2DParams& params)
{
    g_capture.dst = reinterpret_cast<uint64_t>(dst);
    g_capture.src = reinterpret_cast<uint64_t>(src);
    g_capture.params = params;
    ++g_capture.hits;
}

constexpr uint32_t dstOffset = 0x400;
constexpr uint32_t srcOffset = 0x800;
constexpr uint32_t bufferSize = 256;
} // namespace

class TEST_LOADDATA_POINTER_ARGS : public testing::Test {
protected:
    void SetUp()
    {
        SetGCoreType(1);
        g_capture = LoadDataCapture{};
    }
    void TearDown()
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }
};

/* L1 -> L0A, 2D descriptor: address and every descriptor field arrive unchanged. */
TEST_F(TEST_LOADDATA_POINTER_ARGS, TwoDimensionalL0A)
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

    LoadData2DParams params;
    params.startIndex = 0;
    params.repeatTimes = 1;
    params.srcStride = 1;
    params.sid = 0;
    params.dstGap = 0;
    params.ifTranspose = false;

    const uint64_t expectedDst = reinterpret_cast<uint64_t>(dst.GetPhyAddr());
    const uint64_t expectedSrc = reinterpret_cast<uint64_t>(src.GetPhyAddr());

    MOCKER(LoadData2DL12L0ACal<float>, void (*)(float*, float*, const LoadData2DParams&))
        .stubs()
        .will(invoke(CaptureLoadData2D));

    LoadData(dst, src, params);
    ASSERT_EQ(g_capture.hits, 1U);
    EXPECT_EQ(g_capture.dst, expectedDst);
    EXPECT_EQ(g_capture.src, expectedSrc);
    const LoadData2DParams tensorParams = g_capture.params;

    g_capture = LoadDataCapture{};
    LoadData(reinterpret_cast<__ca__ float*>(expectedDst), reinterpret_cast<__cbuf__ float*>(expectedSrc), params);
    ASSERT_EQ(g_capture.hits, 1U);
    EXPECT_EQ(g_capture.dst, expectedDst);
    EXPECT_EQ(g_capture.src, expectedSrc);
    EXPECT_EQ(g_capture.params.startIndex, tensorParams.startIndex);
    EXPECT_EQ(g_capture.params.repeatTimes, tensorParams.repeatTimes);
    EXPECT_EQ(g_capture.params.srcStride, tensorParams.srcStride);
    EXPECT_EQ(g_capture.params.sid, tensorParams.sid);
    EXPECT_EQ(g_capture.params.dstGap, tensorParams.dstGap);
    EXPECT_EQ(g_capture.params.ifTranspose, tensorParams.ifTranspose);
}

} // namespace AscendC
