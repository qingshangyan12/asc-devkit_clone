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
 * \file test_operator_mmad_pointer_args.cpp
 * \brief The operand addresses and descriptor Mmad's pointer form hands to MmadCal, against the
 * Tensor form's.
 *
 * The backend is mocked, so the comparison is on what each arm passes: destination, left matrix,
 * right matrix, and the descriptor fields that select the instruction.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "kernel_operator.h"
#include "stub_fun.h"

/* The template arguments carry commas, which the MOCKER macro would split; expand them first. */
#define MMAD_CAL_FLOAT32 MmadCal<float, float, float>

namespace AscendC {
namespace {
struct MmadCapture {
    uint64_t dst = 0;
    uint64_t fm = 0;
    uint64_t filter = 0;
    MmadParams params{};
    uint32_t hits = 0;
};

MmadCapture g_mmadCapture;

void CaptureMmad(float* dst, float* fm, float* filter, const MmadParams& params)
{
    g_mmadCapture.dst = reinterpret_cast<uint64_t>(dst);
    g_mmadCapture.fm = reinterpret_cast<uint64_t>(fm);
    g_mmadCapture.filter = reinterpret_cast<uint64_t>(filter);
    g_mmadCapture.params = params;
    ++g_mmadCapture.hits;
}

constexpr uint32_t dstOffset = 0x400;
constexpr uint32_t fmOffset = 0x200;
constexpr uint32_t filterOffset = 0x200;
constexpr uint32_t bufferSize = 256;
} // namespace

class TEST_MMAD_POINTER_ARGS : public testing::Test {
protected:
    void SetUp()
    {
        SetGCoreType(1);
        g_mmadCapture = MmadCapture{};
    }
    void TearDown()
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }
};

/* C = A * B: both arms reach MmadCal with the same three addresses and the same descriptor. */
TEST_F(TEST_MMAD_POINTER_ARGS, NoBiasOperandsAndDescriptor)
{
    LocalTensor<float> dst;
    TBuffAddr dstAddr;
    dstAddr.logicPos = static_cast<uint8_t>(TPosition::CO1);
    dst.SetAddr(dstAddr);
    dst.InitBuffer(dstOffset, bufferSize);

    LocalTensor<float> fm;
    TBuffAddr fmAddr;
    fmAddr.logicPos = static_cast<uint8_t>(TPosition::A2);
    fm.SetAddr(fmAddr);
    fm.InitBuffer(fmOffset, bufferSize);

    LocalTensor<float> filter;
    TBuffAddr filterAddr;
    filterAddr.logicPos = static_cast<uint8_t>(TPosition::B2);
    filter.SetAddr(filterAddr);
    filter.InitBuffer(filterOffset, bufferSize);

    MmadParams params;
    params.m = 16;
    params.n = 16;
    params.k = 16;
    params.unitFlag = 2;
    params.disableGemv = false;
    params.isBias = false;
    params.cmatrixInitVal = true;
    params.cmatrixSource = false;

    const uint64_t expectedDst = reinterpret_cast<uint64_t>(dst.GetPhyAddr());
    const uint64_t expectedFm = reinterpret_cast<uint64_t>(fm.GetPhyAddr());
    const uint64_t expectedFilter = reinterpret_cast<uint64_t>(filter.GetPhyAddr());

    MOCKER(MMAD_CAL_FLOAT32, void (*)(float*, float*, float*, const MmadParams&))
        .stubs()
        .will(invoke(CaptureMmad));

    Mmad(dst, fm, filter, params);
    ASSERT_EQ(g_mmadCapture.hits, 1U);
    EXPECT_EQ(g_mmadCapture.dst, expectedDst);
    EXPECT_EQ(g_mmadCapture.fm, expectedFm);
    EXPECT_EQ(g_mmadCapture.filter, expectedFilter);
    const MmadParams tensorParams = g_mmadCapture.params;

    g_mmadCapture = MmadCapture{};
    Mmad(reinterpret_cast<__cc__ float*>(expectedDst), reinterpret_cast<__ca__ float*>(expectedFm),
        reinterpret_cast<__cb__ float*>(expectedFilter), params);
    ASSERT_EQ(g_mmadCapture.hits, 1U);
    EXPECT_EQ(g_mmadCapture.dst, expectedDst);
    EXPECT_EQ(g_mmadCapture.fm, expectedFm);
    EXPECT_EQ(g_mmadCapture.filter, expectedFilter);
    EXPECT_EQ(g_mmadCapture.params.m, tensorParams.m);
    EXPECT_EQ(g_mmadCapture.params.n, tensorParams.n);
    EXPECT_EQ(g_mmadCapture.params.k, tensorParams.k);
    EXPECT_EQ(g_mmadCapture.params.unitFlag, tensorParams.unitFlag);
    EXPECT_EQ(g_mmadCapture.params.disableGemv, tensorParams.disableGemv);
    EXPECT_EQ(g_mmadCapture.params.isBias, tensorParams.isBias);
    EXPECT_EQ(g_mmadCapture.params.cmatrixInitVal, tensorParams.cmatrixInitVal);
    EXPECT_EQ(g_mmadCapture.params.cmatrixSource, tensorParams.cmatrixSource);
}
} // namespace AscendC
