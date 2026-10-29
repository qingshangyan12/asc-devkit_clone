/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License"). You may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tensor_api/stub/cce_stub.h"
#include "tensor_api/tensor.h"

class tensor_api_config_and_cache_interfaces : public testing::Test {
protected:
    void SetUp() override { AscendC::SetGCoreType(2); }
    void TearDown() override { AscendC::SetGCoreType(0); }
};

namespace {

uint64_t g_tensor_api_pad_value = 0;

void set_mov_pad_val_stub(uint64_t config) { EXPECT_EQ(g_tensor_api_pad_value, config); }

void mock_set_copy_pad_val_once()
{
    MOCKER_CPP(set_mov_pad_val, void(uint64_t)).times(1).will(invoke(set_mov_pad_val_stub));
}

void datacache_clean_and_invalid_entire_all_stub(__gm__ void* dst, uint64_t entire, uint64_t target)
{
    EXPECT_EQ(nullptr, dst);
    EXPECT_EQ(cache_line_t::ENTIRE_DATA_CACHE, entire);
    EXPECT_EQ(dcci_dst_t::CACHELINE_ALL, target);
}

void datacache_clean_and_invalid_entire_ub_stub(__ubuf__ void* dst, uint64_t entire, uint64_t target)
{
    EXPECT_EQ(nullptr, dst);
    EXPECT_EQ(cache_line_t::ENTIRE_DATA_CACHE, entire);
    EXPECT_EQ(dcci_dst_t::CACHELINE_UB, target);
}

void datacache_clean_and_invalid_entire_gm_stub(__gm__ void* dst, uint64_t entire, uint64_t target)
{
    EXPECT_EQ(nullptr, dst);
    EXPECT_EQ(cache_line_t::ENTIRE_DATA_CACHE, entire);
    EXPECT_EQ(dcci_dst_t::CACHELINE_OUT, target);
}

void datacache_clean_and_invalid_entire_atomic_stub(__gm__ void* dst, uint64_t entire, uint64_t target)
{
    EXPECT_EQ(nullptr, dst);
    EXPECT_EQ(cache_line_t::ENTIRE_DATA_CACHE, entire);
    EXPECT_EQ(dcci_dst_t::CACHELINE_ATOMIC, target);
}

void datacache_clean_stub() {}

mem_dsb_t g_tensor_api_dsb_scope = mem_dsb_t::DSB_ALL;

void dsb_stub(mem_dsb_t scope) { EXPECT_EQ(g_tensor_api_dsb_scope, scope); }

} // namespace

TEST_F(tensor_api_config_and_cache_interfaces, set_copy_pad_val_int8_forwards_to_c_api)
{
    g_tensor_api_pad_value = 0x1;
    mock_set_copy_pad_val_once();
    asc::te::set_copy_pad_val<int8_t>(1);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, set_copy_pad_val_uint8_forwards_to_c_api)
{
    g_tensor_api_pad_value = 0x1;
    mock_set_copy_pad_val_once();
    asc::te::set_copy_pad_val<uint8_t>(1);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, set_copy_pad_val_int16_forwards_to_c_api)
{
    g_tensor_api_pad_value = 0x1;
    mock_set_copy_pad_val_once();
    asc::te::set_copy_pad_val<int16_t>(1);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, set_copy_pad_val_uint16_forwards_to_c_api)
{
    g_tensor_api_pad_value = 0x1;
    mock_set_copy_pad_val_once();
    asc::te::set_copy_pad_val<uint16_t>(1);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, set_copy_pad_val_half_forwards_to_c_api)
{
    g_tensor_api_pad_value = 0x3c00;
    mock_set_copy_pad_val_once();
    asc::te::set_copy_pad_val<half>(static_cast<half>(1));
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, set_copy_pad_val_bfloat16_forwards_to_c_api)
{
    g_tensor_api_pad_value = 0x3f80;
    mock_set_copy_pad_val_once();
    asc::te::set_copy_pad_val<bfloat16_t>(static_cast<bfloat16_t>(1));
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, set_copy_pad_val_int32_forwards_to_c_api)
{
    g_tensor_api_pad_value = 0x1;
    mock_set_copy_pad_val_once();
    asc::te::set_copy_pad_val<int32_t>(1);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, set_copy_pad_val_uint32_forwards_to_c_api)
{
    g_tensor_api_pad_value = 0x1;
    mock_set_copy_pad_val_once();
    asc::te::set_copy_pad_val<uint32_t>(1);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, set_copy_pad_val_float_forwards_to_c_api)
{
    g_tensor_api_pad_value = 0x3f800000;
    mock_set_copy_pad_val_once();
    asc::te::set_copy_pad_val<float>(1.0f);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, datacache_clean_and_invalid_all_forwards_to_c_api)
{
    MOCKER(dcci, void(__gm__ void*, uint64_t, uint64_t))
        .times(1)
        .will(invoke(datacache_clean_and_invalid_entire_all_stub));
    asc::te::datacache_clean_and_invalid(asc::te::clean_target::all);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, datacache_clean_and_invalid_ub_forwards_to_c_api)
{
    MOCKER(dcci, void(__ubuf__ void*, uint64_t, uint64_t))
        .times(1)
        .will(invoke(datacache_clean_and_invalid_entire_ub_stub));
    asc::te::datacache_clean_and_invalid(asc::te::clean_target::ub);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, datacache_clean_and_invalid_gm_forwards_to_c_api)
{
    MOCKER(dcci, void(__gm__ void*, uint64_t, uint64_t))
        .times(1)
        .will(invoke(datacache_clean_and_invalid_entire_gm_stub));
    asc::te::datacache_clean_and_invalid(asc::te::clean_target::gm);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, datacache_clean_and_invalid_atomic_forwards_to_c_api)
{
    MOCKER(dcci, void(__gm__ void*, uint64_t, uint64_t))
        .times(1)
        .will(invoke(datacache_clean_and_invalid_entire_atomic_stub));
    asc::te::datacache_clean_and_invalid(asc::te::clean_target::atomic);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, datacache_clean_forwards_to_c_api)
{
    MOCKER(dci, void()).times(1).will(invoke(datacache_clean_stub));
    asc::te::datacache_clean();
    GlobalMockObject::verify();
}

TEST_F(tensor_api_config_and_cache_interfaces, sync_data_barrier_forwards_to_c_api)
{
    MOCKER_CPP(dsb, void(mem_dsb_t)).times(3).will(invoke(dsb_stub));

    g_tensor_api_dsb_scope = mem_dsb_t::DSB_ALL;
    asc::te::sync_data_barrier(mem_dsb_t::DSB_ALL);
    g_tensor_api_dsb_scope = mem_dsb_t::DSB_DDR;
    asc::te::sync_data_barrier(mem_dsb_t::DSB_DDR);
    g_tensor_api_dsb_scope = mem_dsb_t::DSB_UB;
    asc::te::sync_data_barrier(mem_dsb_t::DSB_UB);

    GlobalMockObject::verify();
}
