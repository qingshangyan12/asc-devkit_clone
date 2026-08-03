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
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"

namespace {
constexpr uint8_t EXPECTED_IDLE_SLOT_COUNT = 2;
pipe_t expectedPipe;

int64_t get_iqent_stub(pipe_t pipe)
{
    EXPECT_EQ(expectedPipe, pipe);
    return EXPECTED_IDLE_SLOT_COUNT;
}

using GetIdleSlotCountFunc = uint8_t (*)();

struct GetIdleSlotCountParams {
    GetIdleSlotCountFunc func;
    pipe_t pipe;
};
} // namespace

class TestSysVarAscGetPipeIdleSlotCountCApi : public testing::TestWithParam<GetIdleSlotCountParams> {};

TEST_P(TestSysVarAscGetPipeIdleSlotCountCApi, GetIdleSlotCountSucc)
{
    GetIdleSlotCountParams params = GetParam();
    expectedPipe = params.pipe;
    MOCKER_CPP(get_iqent, int64_t(pipe_t)).times(1).will(invoke(get_iqent_stub));

    uint8_t actual = params.func();

    EXPECT_EQ(EXPECTED_IDLE_SLOT_COUNT, actual);
    GlobalMockObject::verify();
}

INSTANTIATE_TEST_SUITE_P(
    TestPipe, TestSysVarAscGetPipeIdleSlotCountCApi,
#if defined(__DAV_VEC__)
    testing::Values(
        GetIdleSlotCountParams{asc_get_pipe_v_idle_slot_count, PIPE_V},
        GetIdleSlotCountParams{asc_get_pipe_mte2_idle_slot_count, PIPE_MTE2},
        GetIdleSlotCountParams{asc_get_pipe_mte3_idle_slot_count, PIPE_MTE3})
#elif defined(__DAV_CUBE__)
    testing::Values(
        GetIdleSlotCountParams{asc_get_pipe_m_idle_slot_count, PIPE_M},
        GetIdleSlotCountParams{asc_get_pipe_mte1_idle_slot_count, PIPE_MTE1},
        GetIdleSlotCountParams{asc_get_pipe_mte2_idle_slot_count, PIPE_MTE2},
        GetIdleSlotCountParams{asc_get_pipe_mte3_idle_slot_count, PIPE_MTE3},
        GetIdleSlotCountParams{asc_get_pipe_fix_idle_slot_count, PIPE_FIX})
#endif
);
