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
 * \file test_operator_readspmbuffer_pointer_args.cpp
 * \brief What TPipe::ReadSpmBuffer's pointer form hands to the GM -> UB copy, against the
 * LocalTensor form's.
 *
 * The spill buffer is backed by GM here, so both forms end in DataCopyGM2UBImpl with a destination
 * address, a source address inside the spill buffer, and a copy descriptor.  The backend is mocked
 * and the three are compared.
 *
 * The last case covers a divergence that is deliberate and documented at the overload: a raw pointer
 * carries no ShapeInfo, so the pointer form takes a readSize of 0 -- and a descriptor naming no
 * block -- literally and moves nothing, whereas the Tensor form would derive a length from that
 * ShapeInfo.  Each "no transfer" assertion is paired with a positive control in the same case (a
 * read that does reach the backend), which is what makes the zero an observation rather than a
 * mock that never fires.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "kernel_operator.h"
#include "stub_fun.h"

/* The template arguments carry commas, which the MOCKER macro would split; expand them first. */
#define GM2UB_IMPL_FLOAT32 DataCopyGM2UBImpl<float>

namespace AscendC {
namespace {
struct SpmCapture {
    uint64_t dst = 0;
    uint64_t src = 0;
    DataCopyParams params{};
    uint32_t hits = 0;
};

SpmCapture g_spmCapture;

void CaptureGm2Ub(float* dst, float* src, const DataCopyParams& params, uint8_t cacheMode)
{
    (void)cacheMode;
    g_spmCapture.dst = reinterpret_cast<uint64_t>(dst);
    g_spmCapture.src = reinterpret_cast<uint64_t>(src);
    g_spmCapture.params = params;
    ++g_spmCapture.hits;
}

constexpr uint64_t spmAddress = 0x10000;
constexpr uint32_t spmElements = 1024;
constexpr uint32_t localBytes = 1024;
constexpr int32_t readSize = 128;
constexpr int32_t readOffset = 32;

void MockGmToUb()
{
    MOCKER(GM2UB_IMPL_FLOAT32, void (*)(float*, float*, const DataCopyParams&, uint8_t))
        .stubs()
        .will(invoke(CaptureGm2Ub));
}
} // namespace

class TEST_READSPMBUFFER_POINTER_ARGS : public testing::Test {
protected:
    void SetUp()
    {
        SetGCoreType(1);
        g_spmCapture = SpmCapture{};
    }
    void TearDown()
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }

    static void MakeSpm(TPipe& tpipe, GlobalTensor<float>& spm, uint64_t& spmPhyAddr)
    {
        spm.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(spmAddress), spmElements);
        tpipe.InitSpmBuffer(spm, static_cast<int32_t>(spmElements * sizeof(float)));
        spmPhyAddr = reinterpret_cast<uint64_t>(spm.GetPhyAddr());
    }
};

/* Size form: both arms move the same span from the same spill-buffer offset into the same address. */
TEST_F(TEST_READSPMBUFFER_POINTER_ARGS, ReadSizeForm)
{
    TPipe tpipe;
    GlobalTensor<float> spm;
    uint64_t spmPhyAddr = 0;
    MakeSpm(tpipe, spm, spmPhyAddr);

    TBuf<TPosition::VECCALC> buf;
    tpipe.InitBuffer(buf, localBytes);
    LocalTensor<float> local = buf.Get<float>();

    const uint64_t expectedDst = reinterpret_cast<uint64_t>(local.GetPhyAddr());
    const uint64_t expectedSrc = spmPhyAddr + static_cast<uint64_t>(readOffset) * sizeof(float);
    MockGmToUb();

    tpipe.ReadSpmBuffer(local, readSize, readOffset);
    ASSERT_EQ(g_spmCapture.hits, 1U);
    EXPECT_EQ(g_spmCapture.dst, expectedDst);
    EXPECT_EQ(g_spmCapture.src, expectedSrc);
    const DataCopyParams tensorParams = g_spmCapture.params;

    g_spmCapture = SpmCapture{};
    tpipe.ReadSpmBuffer(reinterpret_cast<__ubuf__ float*>(expectedDst), readSize, readOffset);
    ASSERT_EQ(g_spmCapture.hits, 1U);
    EXPECT_EQ(g_spmCapture.dst, expectedDst);
    EXPECT_EQ(g_spmCapture.src, expectedSrc);
    EXPECT_EQ(g_spmCapture.params.blockCount, tensorParams.blockCount);
    EXPECT_EQ(g_spmCapture.params.blockLen, tensorParams.blockLen);
    EXPECT_EQ(g_spmCapture.params.srcStride, tensorParams.srcStride);
    EXPECT_EQ(g_spmCapture.params.dstStride, tensorParams.dstStride);
}

/* Descriptor form: the caller's DataCopyParams reach the backend unchanged in both arms. */
TEST_F(TEST_READSPMBUFFER_POINTER_ARGS, CopyParamsForm)
{
    TPipe tpipe;
    GlobalTensor<float> spm;
    uint64_t spmPhyAddr = 0;
    MakeSpm(tpipe, spm, spmPhyAddr);

    TBuf<TPosition::VECCALC> buf;
    tpipe.InitBuffer(buf, localBytes);
    LocalTensor<float> local = buf.Get<float>();

    DataCopyParams copyParams;
    copyParams.blockCount = 1;
    copyParams.blockLen = 8;
    copyParams.srcStride = 0;
    copyParams.dstStride = 0;

    const uint64_t expectedDst = reinterpret_cast<uint64_t>(local.GetPhyAddr());
    const uint64_t expectedSrc = spmPhyAddr + static_cast<uint64_t>(readOffset) * sizeof(float);
    MockGmToUb();

    tpipe.ReadSpmBuffer(local, copyParams, readOffset);
    ASSERT_EQ(g_spmCapture.hits, 1U);
    EXPECT_EQ(g_spmCapture.dst, expectedDst);
    EXPECT_EQ(g_spmCapture.src, expectedSrc);
    EXPECT_EQ(g_spmCapture.params.blockCount, copyParams.blockCount);
    EXPECT_EQ(g_spmCapture.params.blockLen, copyParams.blockLen);

    g_spmCapture = SpmCapture{};
    tpipe.ReadSpmBuffer(reinterpret_cast<__ubuf__ float*>(expectedDst), copyParams, readOffset);
    ASSERT_EQ(g_spmCapture.hits, 1U);
    EXPECT_EQ(g_spmCapture.dst, expectedDst);
    EXPECT_EQ(g_spmCapture.src, expectedSrc);
    EXPECT_EQ(g_spmCapture.params.blockCount, copyParams.blockCount);
    EXPECT_EQ(g_spmCapture.params.blockLen, copyParams.blockLen);
}

/* A read that moves nothing does no transfer: size 0, or a descriptor with no block. */
TEST_F(TEST_READSPMBUFFER_POINTER_ARGS, NothingToMoveDoesNoTransfer)
{
    TPipe tpipe;
    GlobalTensor<float> spm;
    uint64_t spmPhyAddr = 0;
    MakeSpm(tpipe, spm, spmPhyAddr);

    TBuf<TPosition::VECCALC> buf;
    tpipe.InitBuffer(buf, localBytes);
    LocalTensor<float> local = buf.Get<float>();
    __ubuf__ float* dst = reinterpret_cast<__ubuf__ float*>(reinterpret_cast<uint64_t>(local.GetPhyAddr()));
    MockGmToUb();

    /* Positive control: with a size, the transfer does reach the backend. */
    tpipe.ReadSpmBuffer(dst, readSize, readOffset);
    ASSERT_EQ(g_spmCapture.hits, 1U);

    g_spmCapture = SpmCapture{};
    tpipe.ReadSpmBuffer(dst, 0, readOffset);
    EXPECT_EQ(g_spmCapture.hits, 0U);

    /* Same for a descriptor that names no block. */
    DataCopyParams copyParams;
    copyParams.blockCount = 1;
    copyParams.blockLen = 8;
    copyParams.srcStride = 0;
    copyParams.dstStride = 0;
    tpipe.ReadSpmBuffer(dst, copyParams, readOffset);
    ASSERT_EQ(g_spmCapture.hits, 1U);

    g_spmCapture = SpmCapture{};
    copyParams.blockCount = 0;
    tpipe.ReadSpmBuffer(dst, copyParams, readOffset);
    EXPECT_EQ(g_spmCapture.hits, 0U);
}

} // namespace AscendC
