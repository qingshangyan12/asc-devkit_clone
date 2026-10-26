/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdint>
#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "c_api/utils/sys_init.h"

namespace {
constexpr uint64_t CTRL_PRESERVE_BIT = 0x1000000000000ULL;
constexpr uint64_t CTRL_INIT_VALUE = 0x1000000000000008ULL;
constexpr uint64_t EXPECTED_CTRL = CTRL_INIT_VALUE | CTRL_PRESERVE_BIT;
constexpr uint64_t LOOP_SIZE_PARA = (1ULL << 21) | 1ULL;

int64_t get_ctrl_stub() { return static_cast<int64_t>(CTRL_PRESERVE_BIT); }

void set_ctrl_stub(uint64_t value) { EXPECT_EQ(EXPECTED_CTRL, value); }

void set_vector_mask_stub(uint64_t mask_low, uint64_t mask_high)
{
    EXPECT_EQ(UINT64_MAX, mask_low);
    EXPECT_EQ(UINT64_MAX, mask_high);
}

void set_loop_size_stub(uint64_t value) { EXPECT_EQ(LOOP_SIZE_PARA, value); }

void set_padding_stub(uint64_t value) { EXPECT_EQ(0ULL, value); }
} // namespace

class TestInitInstr9201CApi : public testing::Test {
protected:
    void SetUp() override
    {
#if defined(__DAV_VEC__)
        g_coreType = AscendC::AIV;
#elif defined(__DAV_CUBE__)
        g_coreType = AscendC::AIC;
#endif
    }

    void TearDown() override { g_coreType = AscendC::AIV; }
};

TEST_F(TestInitInstr9201CApi, c_api_asc_init_Succ)
{
    MOCKER_CPP(set_atomic_none).times(1);
    MOCKER_CPP(set_mask_norm, void(void)).times(1);
    MOCKER_CPP(get_ctrl, int64_t(void)).times(1).will(invoke(get_ctrl_stub));
    MOCKER_CPP(set_ctrl, void(uint64_t)).times(1).will(invoke(set_ctrl_stub));
    // 9201 does not support set_st_atomic_cfg, asc_init must not emit it.
    MOCKER_CPP(set_st_atomic_cfg, void(uint64_t)).times(0);

#if defined(__DAV_VEC__)
    MOCKER_CPP(set_vector_mask, void(uint64_t, uint64_t)).times(1).will(invoke(set_vector_mask_stub));
    MOCKER_CPP(set_loop_size_ubtoout, void(uint64_t)).times(1).will(invoke(set_loop_size_stub));
    MOCKER_CPP(set_loop_size_outtoub, void(uint64_t)).times(1).will(invoke(set_loop_size_stub));
    MOCKER_CPP(set_padding, void(uint64_t)).times(0);
    MOCKER_CPP(set_loop_size_outtol1, void(uint64_t)).times(0);
#elif defined(__DAV_CUBE__)
    MOCKER_CPP(set_padding, void(uint64_t)).times(1).will(invoke(set_padding_stub));
    MOCKER_CPP(set_loop_size_outtol1, void(uint64_t)).times(1).will(invoke(set_loop_size_stub));
    MOCKER_CPP(set_vector_mask, void(uint64_t, uint64_t)).times(0);
    MOCKER_CPP(set_loop_size_ubtoout, void(uint64_t)).times(0);
    MOCKER_CPP(set_loop_size_outtoub, void(uint64_t)).times(0);
#endif

    asc_init();
    GlobalMockObject::verify();
}
