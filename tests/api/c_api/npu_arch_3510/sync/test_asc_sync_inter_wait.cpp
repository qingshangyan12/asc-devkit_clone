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
#include <mockcpp/mockcpp.hpp>
#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"

template <typename DTYPE>
__aicore__ inline void wait_flag_dev_stub(pipe_t pipe, DTYPE flag_id) {
    EXPECT_EQ(pipe, static_cast<pipe_t>(pipe_t::PIPE_S));
    DTYPE expected_config = (static_cast<DTYPE>(0) << 8) | (static_cast<DTYPE>(11));
    EXPECT_EQ(flag_id, expected_config);
}

class TEST_ASC_SYNC_INTER_WAIT : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

#define TEST_ASC_SYNC_INTER_WAIT(dtype)  \
                                                                                      \
TEST_F(TEST_ASC_SYNC_INTER_WAIT, TEST_ASC_SYNC_INTER_WAIT_##dtype)               \
{                                                                                     \
    MOCKER_CPP(wait_flag_dev, void(pipe_t, dtype))                    \
            .times(1)                                                                           \
            .will(invoke(wait_flag_dev_stub<dtype>));                               \
                                                                                                \
    pipe_t pipe = static_cast<pipe_t>(pipe_t::PIPE_S);                                              \
    dtype flag_id = static_cast<dtype>(11);                                             \
                                                                                                    \
    asc_sync_inter_wait(pipe, flag_id);         \
    GlobalMockObject::verify();                                                                 \
}

// ==========asc_sync_inter_wait==========
TEST_ASC_SYNC_INTER_WAIT(uint8_t);
TEST_ASC_SYNC_INTER_WAIT(uint64_t);