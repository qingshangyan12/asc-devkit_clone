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
#include <cstring>
#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"

class TestSetDeqScale : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {

void set_deqscale_half_Stub(half scale_value)
{
    half val = 11;
    EXPECT_EQ(scale_value, val);
}

void set_deqscale_asc_deq_scale_config_Stub(uint64_t config)
{
    uint64_t offset_value = (config >> 5) & 0X1FF;
    uint64_t sign_mode_value = (config >> 14) & 0X1;
    uint32_t scale_bits = static_cast<uint32_t>(config >> 32);
    float scale_value;
    std::memcpy(&scale_value, &scale_bits, sizeof(scale_value));
    
    EXPECT_EQ(offset_value, static_cast<uint64_t>(10));
    EXPECT_EQ(sign_mode_value, static_cast<uint64_t>(1));
    EXPECT_EQ(scale_value, static_cast<float>(0));
}

void set_deqscale_ubuf_uint64_t_Stub(uint64_t config)
{
    constexpr uint64_t deq_addr = 5;
    EXPECT_EQ(config, (uint64_t)(reinterpret_cast<__ubuf__ uint64_t*>(55)) >> deq_addr);
}
}

TEST_F(TestSetDeqScale, set_deqscale_asc_deq_scale_config_Succ)
{
    asc_deq_scale_config config;
    config.reserved1 = 10;
    config.reserved2 = 10;
    config.offset = 10;
    config.sign_mode = true;
    config.scale = 0;

    MOCKER_CPP(set_deqscale, void(uint64_t))
        .times(1)
        .will(invoke(set_deqscale_asc_deq_scale_config_Stub));
    
    asc_set_deq_scale(config);
    GlobalMockObject::verify();
}

TEST_F(TestSetDeqScale, set_deqscale_ubuf_uint64_t_Succ)
{
    __ubuf__ uint64_t* config = reinterpret_cast<__ubuf__ uint64_t*>(55);

    MOCKER_CPP(set_deqscale, void(uint64_t))
        .times(1)
        .will(invoke(set_deqscale_ubuf_uint64_t_Stub));

    asc_set_deq_scale(config);
    GlobalMockObject::verify();
}