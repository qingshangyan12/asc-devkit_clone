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
 * \file test_operator_loaddatawithtranspose_pointer_args.cpp
 * \brief The operands and descriptor LoadDataWithTranspose's pointer form hands to the L1 -> L0B
 * backend, against the Tensor form's.
 *
 * The backend is mocked, so the comparison is on what each arm passes: destination, source and every
 * descriptor field the instruction reads.
 *
 * Scope -- the V2 descriptor only.  The V1 descriptor is not compared here, and the reason is in the
 * build rather than in the API: under ASCENDC_CPU_DEBUG the address-space qualifiers are empty
 * macros, so the single representative helper in kernel_operator_mm_intf_impl.h always takes the
 * L0A backend, and on this architecture that backend reports "LoadDataWithTranspose from L1
 * Buffer(A1) to L0A Buffer(A2) is not supported".  A pointer call with a V1 descriptor therefore
 * cannot be observed reaching the L0B backend in this build, while the Tensor call with an L0B
 * destination does.  The V2 descriptor has no such branch: both forms reach the L0B backend here.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "kernel_operator.h"
#include "stub_fun.h"

/* The template arguments carry commas, which the MOCKER macro would split; expand them first. */
#define TRANSPOSE_V2_CAL_FLOAT32 LoadData2DL12L0BTransposeCal<float>

namespace AscendC {
namespace {
struct TransposeCapture {
    uint64_t dst = 0;
    uint64_t src = 0;
    LoadData2dTransposeParamsV2 params{};
    uint32_t hits = 0;
};

TransposeCapture g_transposeCapture;

void CaptureTranspose(float* dst, float* src, const LoadData2dTransposeParamsV2& params)
{
    g_transposeCapture.dst = reinterpret_cast<uint64_t>(dst);
    g_transposeCapture.src = reinterpret_cast<uint64_t>(src);
    g_transposeCapture.params = params;
    ++g_transposeCapture.hits;
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

class TEST_LOADDATAWITHTRANSPOSE_POINTER_ARGS : public testing::Test {
protected:
    void SetUp()
    {
        SetGCoreType(1);
        g_transposeCapture = TransposeCapture{};
        MOCKER(raise, int32_t(*)(int32_t)).stubs().will(invoke(RaiseStub));
    }
    void TearDown()
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }

    static LoadData2dTransposeParamsV2 MakeParams()
    {
        LoadData2dTransposeParamsV2 params(0, 2, 1, 0, 0, 0, 0);
        return params;
    }
};

/* L1 -> L0B: both arms reach the backend with the same addresses and the same descriptor. */
TEST_F(TEST_LOADDATAWITHTRANSPOSE_POINTER_ARGS, OperandsAndDescriptor)
{
    LocalTensor<float> dst;
    TBuffAddr dstAddr;
    dstAddr.logicPos = static_cast<uint8_t>(TPosition::B2);
    dst.SetAddr(dstAddr);
    dst.InitBuffer(dstOffset, bufferSize);

    LocalTensor<float> src;
    TBuffAddr srcAddr;
    srcAddr.logicPos = static_cast<uint8_t>(TPosition::A1);
    src.SetAddr(srcAddr);
    src.InitBuffer(srcOffset, bufferSize);

    const LoadData2dTransposeParamsV2 params = MakeParams();
    const uint64_t expectedDst = reinterpret_cast<uint64_t>(dst.GetPhyAddr());
    const uint64_t expectedSrc = reinterpret_cast<uint64_t>(src.GetPhyAddr());

    MOCKER(TRANSPOSE_V2_CAL_FLOAT32, void (*)(float*, float*, const LoadData2dTransposeParamsV2&))
        .stubs()
        .will(invoke(CaptureTranspose));

    LoadDataWithTranspose(dst, src, params);
    ASSERT_EQ(g_transposeCapture.hits, 1U);
    EXPECT_EQ(g_transposeCapture.dst, expectedDst);
    EXPECT_EQ(g_transposeCapture.src, expectedSrc);
    const LoadData2dTransposeParamsV2 tensorParams = g_transposeCapture.params;

    g_transposeCapture = TransposeCapture{};
    LoadDataWithTranspose(
        reinterpret_cast<__cb__ float*>(expectedDst), reinterpret_cast<__cbuf__ float*>(expectedSrc), params);
    ASSERT_EQ(g_transposeCapture.hits, 1U);
    EXPECT_EQ(g_transposeCapture.dst, expectedDst);
    EXPECT_EQ(g_transposeCapture.src, expectedSrc);
    EXPECT_EQ(g_transposeCapture.params.startIndex, tensorParams.startIndex);
    EXPECT_EQ(g_transposeCapture.params.repeatTimes, tensorParams.repeatTimes);
    EXPECT_EQ(g_transposeCapture.params.srcStride, tensorParams.srcStride);
    EXPECT_EQ(g_transposeCapture.params.dstGap, tensorParams.dstGap);
    EXPECT_EQ(g_transposeCapture.params.dstFracGap, tensorParams.dstFracGap);
    EXPECT_EQ(g_transposeCapture.params.srcFracGap, tensorParams.srcFracGap);
    EXPECT_EQ(g_transposeCapture.params.addrMode, tensorParams.addrMode);
}
} // namespace AscendC
