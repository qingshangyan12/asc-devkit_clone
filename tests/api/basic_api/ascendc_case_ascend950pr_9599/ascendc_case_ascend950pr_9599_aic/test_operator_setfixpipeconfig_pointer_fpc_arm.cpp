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
 * \file test_operator_setfixpipeconfig_pointer_fpc_arm.cpp
 * \brief FPC config word of the raw-pointer SetFixPipeConfig overloads, against the Tensor overloads.
 *
 * SetFixPipeConfig writes the quantization/relu pre-tensor addresses into the fixpipe config
 * register, and that register is the whole of what a quantized Fixpipe reads to find its deq
 * tensor.  The pointer overloads re-express that write on the operand addresses instead of on
 * LocalTensor::GetPhyAddr(); the two expressions are different text, so the claim that they are
 * equivalent is a claim about a value, and has to be checked on the value.
 *
 * Each case below runs both overloads on the same address and compares the config word handed to
 * set_fpc(), and separately against the encoding the API documents (FPC[7:0] relu address, 64B
 * aligned; FPC[15:8] quant address, 128B aligned; FPC[63] unit flag).  A pointer overload that
 * skipped the write, wrote the address into the other field, or dropped the alignment shift fails
 * on the first of those comparisons, not just on the second.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "kernel_operator.h"
#include "stub_fun.h"

namespace AscendC {
namespace {
uint64_t g_fpcConfig = 0;
uint32_t g_fpcWriteTimes = 0;

void CaptureFpc(uint64_t config)
{
    g_fpcConfig = config;
    ++g_fpcWriteTimes;
}

constexpr uint32_t reluOffset = 0x400;
constexpr uint32_t quantOffset = 0x800;
constexpr uint32_t preTensorSize = 128;
} // namespace

class TEST_FIXPIPE_CONFIG_POINTER : public testing::Test {
protected:
    void SetUp()
    {
        SetGCoreType(1);
        g_fpcConfig = 0;
        g_fpcWriteTimes = 0;
    }
    void TearDown()
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }
};

/* FPC[15:8], QuantPreAddr, 128B aligned; FPC[63], UnitFlag. */
TEST_F(TEST_FIXPIPE_CONFIG_POINTER, SingleOperandQuantField)
{
    LocalTensor<uint64_t> quantPre;
    TBuffAddr tbufQuantPre;
    tbufQuantPre.logicPos = static_cast<uint8_t>(TPosition::C1);
    quantPre.SetAddr(tbufQuantPre);
    quantPre.InitBuffer(quantOffset, preTensorSize);
    const uint64_t quantAddr = reinterpret_cast<uint64_t>(quantPre.GetPhyAddr());
    const uint64_t expected = (quantAddr >> 7) << 8;

    MOCKER(set_fpc, void(uint64_t)).times(2).will(invoke(CaptureFpc));
    SetFixPipeConfig(quantPre, false);
    EXPECT_EQ(g_fpcWriteTimes, 1U);
    EXPECT_EQ(g_fpcConfig, expected);

    g_fpcConfig = 0;
    g_fpcWriteTimes = 0;
    SetFixPipeConfig(reinterpret_cast<__cbuf__ uint64_t*>(quantAddr), false);
    EXPECT_EQ(g_fpcWriteTimes, 1U);
    EXPECT_EQ(g_fpcConfig, expected);
}

/* The same operand through setRelu=true goes to FPC[7:0], 64B aligned, instead. */
TEST_F(TEST_FIXPIPE_CONFIG_POINTER, SingleOperandReluField)
{
    LocalTensor<uint64_t> reluPre;
    TBuffAddr tbufReluPre;
    tbufReluPre.logicPos = static_cast<uint8_t>(TPosition::C1);
    reluPre.SetAddr(tbufReluPre);
    reluPre.InitBuffer(reluOffset, preTensorSize);
    const uint64_t reluAddr = reinterpret_cast<uint64_t>(reluPre.GetPhyAddr());
    const uint64_t expected = reluAddr >> 6;

    MOCKER(set_fpc, void(uint64_t)).times(2).will(invoke(CaptureFpc));
    SetFixPipeConfig<uint64_t, true>(reluPre, false);
    EXPECT_EQ(g_fpcWriteTimes, 1U);
    EXPECT_EQ(g_fpcConfig, expected);

    g_fpcConfig = 0;
    g_fpcWriteTimes = 0;
    SetFixPipeConfig<true>(reinterpret_cast<__cbuf__ uint64_t*>(reluAddr), false);
    EXPECT_EQ(g_fpcWriteTimes, 1U);
    EXPECT_EQ(g_fpcConfig, expected);
}

/* Unit flag reaches FPC[63] on both paths, and does not disturb the address field. */
TEST_F(TEST_FIXPIPE_CONFIG_POINTER, SingleOperandUnitFlag)
{
    LocalTensor<uint64_t> quantPre;
    TBuffAddr tbufQuantPre;
    tbufQuantPre.logicPos = static_cast<uint8_t>(TPosition::C1);
    quantPre.SetAddr(tbufQuantPre);
    quantPre.InitBuffer(quantOffset, preTensorSize);
    const uint64_t quantAddr = reinterpret_cast<uint64_t>(quantPre.GetPhyAddr());
    const uint64_t expected = ((quantAddr >> 7) << 8) | (static_cast<uint64_t>(1) << 63);

    MOCKER(set_fpc, void(uint64_t)).times(2).will(invoke(CaptureFpc));
    SetFixPipeConfig(quantPre, true);
    EXPECT_EQ(g_fpcWriteTimes, 1U);
    EXPECT_EQ(g_fpcConfig, expected);

    g_fpcConfig = 0;
    g_fpcWriteTimes = 0;
    SetFixPipeConfig(reinterpret_cast<__cbuf__ uint64_t*>(quantAddr), true);
    EXPECT_EQ(g_fpcWriteTimes, 1U);
    EXPECT_EQ(g_fpcConfig, expected);
}

/* Both addresses at once: the two fields must not collapse into one another. */
TEST_F(TEST_FIXPIPE_CONFIG_POINTER, TwoOperandReluAndQuantFields)
{
    LocalTensor<uint64_t> reluPre;
    TBuffAddr tbufReluPre;
    tbufReluPre.logicPos = static_cast<uint8_t>(TPosition::C1);
    reluPre.SetAddr(tbufReluPre);
    reluPre.InitBuffer(reluOffset, preTensorSize);
    LocalTensor<uint64_t> quantPre;
    TBuffAddr tbufQuantPre;
    tbufQuantPre.logicPos = static_cast<uint8_t>(TPosition::C1);
    quantPre.SetAddr(tbufQuantPre);
    quantPre.InitBuffer(quantOffset, preTensorSize);
    const uint64_t reluAddr = reinterpret_cast<uint64_t>(reluPre.GetPhyAddr());
    const uint64_t quantAddr = reinterpret_cast<uint64_t>(quantPre.GetPhyAddr());
    const uint64_t expected = (reluAddr >> 6) | (((quantAddr >> 7) << 8)) | (static_cast<uint64_t>(1) << 63);

    MOCKER(set_fpc, void(uint64_t)).times(2).will(invoke(CaptureFpc));
    SetFixPipeConfig(reluPre, quantPre, true);
    EXPECT_EQ(g_fpcWriteTimes, 1U);
    EXPECT_EQ(g_fpcConfig, expected);

    g_fpcConfig = 0;
    g_fpcWriteTimes = 0;
    SetFixPipeConfig(
        reinterpret_cast<__cbuf__ uint64_t*>(reluAddr), reinterpret_cast<__cbuf__ uint64_t*>(quantAddr), true);
    EXPECT_EQ(g_fpcWriteTimes, 1U);
    EXPECT_EQ(g_fpcConfig, expected);
}
} // namespace AscendC
