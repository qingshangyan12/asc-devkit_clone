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
 * \file test_operator_syncall_pointer_args.cpp
 * \brief The workspace addresses and core count SyncAll's pointer form hands to SoftSyncAllImpl,
 * against the Tensor form's.
 *
 * In the CPU-debug build both forms call SoftSyncAllImpl<false>, so mocking that function and
 * comparing the arguments is a direct check of the equivalence claim.  The expected addresses come
 * from the tensors the test built, not from the code under test.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "kernel_operator.h"
#include "stub_fun.h"

/* A macro keeps the template argument out of the MOCKER call site. */
#define SOFT_SYNC_ALL_FALSE SoftSyncAllImpl<false>

namespace AscendC {
namespace {
struct SyncAllCapture {
    uint64_t gm = 0;
    uint64_t ub = 0;
    int32_t usedCores = -1;
    uint32_t hits = 0;
};

SyncAllCapture g_syncCapture;

void CaptureSoftSyncAll(int32_t* gmWorkspace, int32_t* ubWorkspace, const int usedCores)
{
    g_syncCapture.gm = reinterpret_cast<uint64_t>(gmWorkspace);
    g_syncCapture.ub = reinterpret_cast<uint64_t>(ubWorkspace);
    g_syncCapture.usedCores = usedCores;
    ++g_syncCapture.hits;
}

constexpr uint32_t gmAddress = 0x1000;
constexpr uint32_t ubOffset = 0x400;
constexpr uint32_t bufferSize = 256;
constexpr int32_t usedCores = 3;
} // namespace

class TEST_SYNCALL_POINTER_ARGS : public testing::Test {
protected:
    void SetUp()
    {
        SetGCoreType(1);
        g_syncCapture = SyncAllCapture{};
    }
    void TearDown()
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }
};

/* Both arms reach SoftSyncAllImpl with the same two workspace addresses and the same core count. */
TEST_F(TEST_SYNCALL_POINTER_ARGS, WorkspaceAndCoreCount)
{
    GlobalTensor<int32_t> gm;
    gm.SetGlobalBuffer(reinterpret_cast<__gm__ int32_t*>(gmAddress), bufferSize);

    LocalTensor<int32_t> ub;
    TBuffAddr ubAddr;
    ubAddr.logicPos = static_cast<uint8_t>(TPosition::VECCALC);
    ub.SetAddr(ubAddr);
    ub.InitBuffer(ubOffset, bufferSize);

    const uint64_t expectedGm = reinterpret_cast<uint64_t>(gm.GetPhyAddr());
    const uint64_t expectedUb = reinterpret_cast<uint64_t>(ub.GetPhyAddr());

    MOCKER(SOFT_SYNC_ALL_FALSE, void (*)(int32_t*, int32_t*, const int)).stubs().will(invoke(CaptureSoftSyncAll));

    SyncAll<false>(gm, ub, usedCores);
    ASSERT_EQ(g_syncCapture.hits, 1U);
    EXPECT_EQ(g_syncCapture.gm, expectedGm);
    EXPECT_EQ(g_syncCapture.ub, expectedUb);
    EXPECT_EQ(g_syncCapture.usedCores, usedCores);
    const int32_t tensorUsedCores = g_syncCapture.usedCores;

    g_syncCapture = SyncAllCapture{};
    SyncAll<false>(reinterpret_cast<__gm__ int32_t*>(expectedGm), reinterpret_cast<__ubuf__ int32_t*>(expectedUb), usedCores);
    ASSERT_EQ(g_syncCapture.hits, 1U);
    EXPECT_EQ(g_syncCapture.gm, expectedGm);
    EXPECT_EQ(g_syncCapture.ub, expectedUb);
    EXPECT_EQ(g_syncCapture.usedCores, tensorUsedCores);
}
} // namespace AscendC
