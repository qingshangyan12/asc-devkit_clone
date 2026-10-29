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
 * \file test_operator_mmad_bias_source_args.cpp
 * \brief Pins the cmatrixSource value each Mmad form sends when a bias operand is present.
 *
 * The Tensor form derives cmatrixSource from the bias tensor's position: BIAS (C2) -> true,
 * L0C (CO1) -> false.  A raw pointer carries no position, so the pointer form fixes the value at
 * true.  This file records that difference as behaviour, not as a defect report: the three cases
 * below assert what each form sends, so a future change to the fixed value turns red here.
 *
 * The backend is mocked; the captured cmatrixSource is the value MmadCal receives.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "kernel_operator.h"
#include "stub_fun.h"

/* The template arguments carry commas, which the MOCKER macro would split; expand them first. */
#define MMAD_CAL_BIAS_FLOAT32 MmadCal<float, float, float>

namespace AscendC {
namespace {
struct MmadBiasCapture {
    uint64_t bias = 0;
    bool cmatrixSource = false;
    uint32_t hits = 0;
};

MmadBiasCapture g_biasCapture;

void CaptureMmadBias(float* dst, float* fm, float* filter, uint64_t bias, const MmadParams& params, bool source)
{
    g_biasCapture.bias = bias;
    g_biasCapture.cmatrixSource = source;
    ++g_biasCapture.hits;
}

constexpr uint32_t dstOffset = 0x400;
constexpr uint32_t fmOffset = 0x200;
constexpr uint32_t filterOffset = 0x200;
constexpr uint32_t biasOffset = 0x200;
constexpr uint32_t bufferSize = 256;
} // namespace

class TEST_MMAD_BIAS_SOURCE : public testing::Test {
protected:
    void SetUp()
    {
        SetGCoreType(1);
        g_biasCapture = MmadBiasCapture{};
    }
    void TearDown()
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }

    static void MakeOperands(LocalTensor<float>& dst, LocalTensor<float>& fm, LocalTensor<float>& filter)
    {
        TBuffAddr dstAddr;
        dstAddr.logicPos = static_cast<uint8_t>(TPosition::CO1);
        dst.SetAddr(dstAddr);
        dst.InitBuffer(dstOffset, bufferSize);

        TBuffAddr fmAddr;
        fmAddr.logicPos = static_cast<uint8_t>(TPosition::A2);
        fm.SetAddr(fmAddr);
        fm.InitBuffer(fmOffset, bufferSize);

        TBuffAddr filterAddr;
        filterAddr.logicPos = static_cast<uint8_t>(TPosition::B2);
        filter.SetAddr(filterAddr);
        filter.InitBuffer(filterOffset, bufferSize);
    }

    static MmadParams MakeParams()
    {
        MmadParams params;
        params.m = 16;
        params.n = 16;
        params.k = 16;
        params.unitFlag = 2;
        params.disableGemv = false;
        params.isBias = true;
        params.cmatrixInitVal = false;
        params.cmatrixSource = false;
        return params;
    }
};

/* Bias in the bias table (C2): the Tensor form sends cmatrixSource = true. */
TEST_F(TEST_MMAD_BIAS_SOURCE, TensorBiasInC2SendsTrue)
{
    LocalTensor<float> dst, fm, filter;
    MakeOperands(dst, fm, filter);

    LocalTensor<float> bias;
    TBuffAddr biasAddr;
    biasAddr.logicPos = static_cast<uint8_t>(TPosition::C2);
    bias.SetAddr(biasAddr);
    bias.InitBuffer(biasOffset, bufferSize);

    MOCKER(MMAD_CAL_BIAS_FLOAT32, void (*)(float*, float*, float*, uint64_t, const MmadParams&, bool))
        .stubs()
        .will(invoke(CaptureMmadBias));

    Mmad(dst, fm, filter, bias, MakeParams());
    ASSERT_EQ(g_biasCapture.hits, 1U);
    EXPECT_EQ(g_biasCapture.bias, reinterpret_cast<uint64_t>(bias.GetPhyAddr()));
    EXPECT_TRUE(g_biasCapture.cmatrixSource);
}

/* Bias in L0C (CO1): the Tensor form sends cmatrixSource = false. */
TEST_F(TEST_MMAD_BIAS_SOURCE, TensorBiasInCo1SendsFalse)
{
    LocalTensor<float> dst, fm, filter;
    MakeOperands(dst, fm, filter);

    LocalTensor<float> bias;
    TBuffAddr biasAddr;
    biasAddr.logicPos = static_cast<uint8_t>(TPosition::CO1);
    bias.SetAddr(biasAddr);
    bias.InitBuffer(biasOffset, bufferSize);

    MOCKER(MMAD_CAL_BIAS_FLOAT32, void (*)(float*, float*, float*, uint64_t, const MmadParams&, bool))
        .stubs()
        .will(invoke(CaptureMmadBias));

    Mmad(dst, fm, filter, bias, MakeParams());
    ASSERT_EQ(g_biasCapture.hits, 1U);
    EXPECT_EQ(g_biasCapture.bias, reinterpret_cast<uint64_t>(bias.GetPhyAddr()));
    EXPECT_FALSE(g_biasCapture.cmatrixSource);
}

/* The pointer form fixes cmatrixSource at true: it matches the C2 case and cannot express CO1. */
TEST_F(TEST_MMAD_BIAS_SOURCE, PointerBiasAlwaysSendsTrue)
{
    LocalTensor<float> dst, fm, filter;
    MakeOperands(dst, fm, filter);

    const uint64_t dstPtr = reinterpret_cast<uint64_t>(dst.GetPhyAddr());
    const uint64_t fmPtr = reinterpret_cast<uint64_t>(fm.GetPhyAddr());
    const uint64_t filterPtr = reinterpret_cast<uint64_t>(filter.GetPhyAddr());
    const uint64_t biasPtr = static_cast<uint64_t>(biasOffset);

    MOCKER(MMAD_CAL_BIAS_FLOAT32, void (*)(float*, float*, float*, uint64_t, const MmadParams&, bool))
        .stubs()
        .will(invoke(CaptureMmadBias));

    Mmad(
        reinterpret_cast<__cc__ float*>(dstPtr), reinterpret_cast<__ca__ float*>(fmPtr),
        reinterpret_cast<__cb__ float*>(filterPtr), reinterpret_cast<__cc__ float*>(biasPtr), MakeParams());
    ASSERT_EQ(g_biasCapture.hits, 1U);
    EXPECT_EQ(g_biasCapture.bias, biasPtr);
    EXPECT_TRUE(g_biasCapture.cmatrixSource);
}
} // namespace AscendC
