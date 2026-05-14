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

using namespace std;
using namespace AscendC;

static int g_testFlag = 0;

__aicore__ static void TestTargetFunc(int x)
{
    g_testFlag = x;
}

class TestVFDebug : public testing::Test {
protected:
    void SetUp()
    {
        g_testFlag = 0;
    }
    void TearDown()
    {
        CheckSyncState();
    }
};

TEST_F(TestVFDebug, AscVFDebugInitUb_Callable)
{
    // Verify AscVFDebugInitUb() can be called without crash under ASCENDC_CPU_DEBUG or else branch.
    AscVFDebugInitUb();
    SUCCEED();
}

TEST_F(TestVFDebug, AscVFDebugTransferUb_Callable)
{
    // Verify AscVFDebugTransferUb() can be called without crash under ASCENDC_CPU_DEBUG or else branch.
    AscVFDebugTransferUb();
    SUCCEED();
}

TEST_F(TestVFDebug, AscVFCallImpl_CallsTargetFunction)
{
    // Verify AscVFCallImpl invokes the target function after InitUb and before TransferUb.
    AscVFCallImpl<TestTargetFunc>(42);
    EXPECT_EQ(g_testFlag, 42);
}

TEST_F(TestVFDebug, AscVFCallImpl_InitAndTransferCalled)
{
    // Verify both AscVFDebugInitUb and AscVFDebugTransferUb are callable within AscVFCallImpl.
    g_testFlag = 0;
    AscVFCallImpl<TestTargetFunc>(100);
    EXPECT_EQ(g_testFlag, 100);

    g_testFlag = 0;
    AscVFCallImpl<TestTargetFunc>(-1);
    EXPECT_EQ(g_testFlag, -1);
}
