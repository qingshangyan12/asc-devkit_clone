/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express OR IMPLIED.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tensor_api/stub/cce_stub.h"
#include "tensor_api/tensor.h"

class tensor_api_new_interfaces_aiv : public testing::Test {
protected:
    void SetUp() override { AscendC::SetGCoreType(2); }
    void TearDown() override { AscendC::SetGCoreType(0); }
};

namespace {

int64_t g_tensor_api_sub_block_id = 0;
int64_t g_tensor_api_sub_block_num = 0;
uint64_t g_tensor_api_phy_buf_addr = 0;

int64_t get_subblockid_stub() { return g_tensor_api_sub_block_id; }

int64_t get_subblockdim_stub() { return g_tensor_api_sub_block_num; }

uint64_t get_imm_stub(uint64_t offset) { return offset + g_tensor_api_phy_buf_addr; }

} // namespace

TEST_F(tensor_api_new_interfaces_aiv, init_soc_state_forwards_to_asc_init)
{
    MOCKER_CPP(asc_init, void(void)).times(1).will(ignoreReturnValue());
    asc::te::init_soc_state();
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, get_sub_block_id_forwards_to_c_api)
{
    g_tensor_api_sub_block_id = 7;
    MOCKER_CPP(asc_get_sub_block_id, int64_t(void)).times(1).will(invoke(get_subblockid_stub));
    EXPECT_EQ(asc::te::get_sub_block_id(), 7);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, get_sub_block_num_forwards_to_c_api)
{
    g_tensor_api_sub_block_num = 4;
    MOCKER_CPP(asc_get_sub_block_num, int64_t(void)).times(1).will(invoke(get_subblockdim_stub));
    EXPECT_EQ(asc::te::get_sub_block_num(), 4);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, get_vf_len_forwards_to_c_api)
{
    MOCKER_CPP(asc_get_vf_len, int64_t(void)).times(1).will(returnValue(256));
    EXPECT_EQ(asc::te::get_vf_len(), 256);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, get_phy_buf_addr_forwards_offset_to_c_api)
{
    g_tensor_api_phy_buf_addr = 0x12340000U;
    MOCKER_CPP(asc_get_phy_buf_addr, uint64_t(uint64_t)).times(1).will(invoke(get_imm_stub));
    EXPECT_EQ(asc::te::get_phy_buf_addr(0x1000U), 0x12341000U);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, mutex_forwards_pipe_and_id_to_c_api)
{
    MOCKER_CPP(asc_lock, void(pipe_t, uint8_t)).times(1).will(ignoreReturnValue());
    MOCKER_CPP(asc_unlock, void(pipe_t, uint8_t)).times(1).will(ignoreReturnValue());

    asc::te::mutex<pipe_t::PIPE_MTE3> mutex(9);
    mutex.lock();
    mutex.unlock();
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, lock_guard_locks_and_unlocks_once)
{
    MOCKER_CPP(asc_lock, void(pipe_t, uint8_t)).times(1).will(ignoreReturnValue());
    MOCKER_CPP(asc_unlock, void(pipe_t, uint8_t)).times(1).will(ignoreReturnValue());

    {
        asc::te::mutex<pipe_t::PIPE_V> mutex(3);
        asc::te::lock_guard guard(mutex);
    }
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, block_and_intra_sync_forward_to_c_api)
{
    MOCKER_CPP(asc_sync_block_arrive, void(pipe_t, int64_t)).times(1).will(ignoreReturnValue());
    MOCKER_CPP(asc_sync_block_wait, void(pipe_t, int64_t)).times(1).will(ignoreReturnValue());
    MOCKER_CPP(asc_sync_intra_arrive, void(pipe_t, uint64_t)).times(1).will(ignoreReturnValue());
    MOCKER_CPP(asc_sync_intra_wait, void(pipe_t, uint64_t)).times(1).will(ignoreReturnValue());

    asc::te::sync_block_arrive(pipe_t::PIPE_MTE2, 5);
    asc::te::sync_block_wait(pipe_t::PIPE_MTE2, 6);
    asc::te::sync_intra_arrive(pipe_t::PIPE_V, 7);
    asc::te::sync_intra_wait(pipe_t::PIPE_V, 8);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, inter_and_subblock_sync_forward_to_c_api)
{
    MOCKER_CPP(asc_sync_inter_arrive, void(pipe_t, int64_t)).times(1).will(ignoreReturnValue());
    MOCKER_CPP(asc_sync_inter_wait, void(pipe_t, int64_t)).times(1).will(ignoreReturnValue());
    MOCKER_CPP(asc_sync_subblock_arrive, void(pipe_t, int64_t)).times(1).will(ignoreReturnValue());
    MOCKER_CPP(asc_sync_subblock_wait, void(pipe_t, int64_t)).times(1).will(ignoreReturnValue());

    asc::te::sync_inter_arrive(pipe_t::PIPE_MTE2, 9);
    asc::te::sync_inter_wait(pipe_t::PIPE_MTE2, 10);
    asc::te::sync_subblock_arrive(pipe_t::PIPE_V, 11);
    asc::te::sync_subblock_wait(pipe_t::PIPE_V, 12);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, sync_pipe_forwards_pipe_to_c_api)
{
    MOCKER_CPP(asc_sync_pipe, void(pipe_t)).times(4).will(ignoreReturnValue());

    asc::te::sync_pipe(pipe_t::PIPE_V);
    asc::te::sync_pipe(pipe_t::PIPE_M);
    asc::te::sync_pipe(pipe_t::PIPE_MTE2);
    asc::te::sync_pipe(pipe_t::PIPE_ALL);
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, dma_atomic_dispatches_all_supported_types)
{
    MOCKER(set_atomic_add, void(void)).times(6).will(ignoreReturnValue());
    MOCKER(set_atomic_max, void(void)).times(6).will(ignoreReturnValue());
    MOCKER(set_atomic_min, void(void)).times(6).will(ignoreReturnValue());
    MOCKER(set_atomic_s8, void(void)).times(3).will(ignoreReturnValue());
    MOCKER(set_atomic_s16, void(void)).times(3).will(ignoreReturnValue());
    MOCKER(set_atomic_s32, void(void)).times(3).will(ignoreReturnValue());
    MOCKER(set_atomic_f16, void(void)).times(3).will(ignoreReturnValue());
    MOCKER(set_atomic_bf16, void(void)).times(3).will(ignoreReturnValue());
    MOCKER(set_atomic_f32, void(void)).times(3).will(ignoreReturnValue());

    asc::te::set_atomic_add<int8_t>();
    asc::te::set_atomic_max<int8_t>();
    asc::te::set_atomic_min<int8_t>();
    asc::te::set_atomic_add<int16_t>();
    asc::te::set_atomic_max<int16_t>();
    asc::te::set_atomic_min<int16_t>();
    asc::te::set_atomic_add<int32_t>();
    asc::te::set_atomic_max<int32_t>();
    asc::te::set_atomic_min<int32_t>();
    asc::te::set_atomic_add<half>();
    asc::te::set_atomic_max<half>();
    asc::te::set_atomic_min<half>();
    asc::te::set_atomic_add<bfloat16_t>();
    asc::te::set_atomic_max<bfloat16_t>();
    asc::te::set_atomic_min<bfloat16_t>();
    asc::te::set_atomic_add<float>();
    asc::te::set_atomic_max<float>();
    asc::te::set_atomic_min<float>();

    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, disable_dma_atomic_forwards_to_c_api)
{
    MOCKER_CPP(asc_disable_dma_atomic, void(void)).times(1).will(ignoreReturnValue());
    asc::te::disable_dma_atomic();
    GlobalMockObject::verify();
}

TEST_F(tensor_api_new_interfaces_aiv, copy_gm_to_ub_does_not_set_padding_value)
{
    using namespace asc::te;

    constexpr uint32_t m = 2;
    constexpr uint32_t n = 16;
    __gm__ int8_t src[m * n] = {0};
    __ubuf__ int8_t dst[m * n] = {0};

    auto layout = make_frame_layout<nd_ext_layout_ptn, layout_trait_default<int8_t>>(m, n);
    auto src_tensor = make_tensor(make_mem_ptr<location::gm>(src), layout);
    auto dst_tensor = make_tensor(make_mem_ptr<location::ub>(dst), layout);

    MOCKER_CPP(
        copy_gm_to_ubuf_align_v2, void(
                                      __ubuf__ uint8_t*, __gm__ uint8_t*, uint8_t, uint32_t, uint32_t, uint8_t, uint8_t,
                                      bool, uint8_t, uint64_t, uint32_t))
        .times(2)
        .will(ignoreReturnValue());
    MOCKER_CPP(set_mov_pad_val, void(uint64_t)).times(0);

    constexpr gm_to_ub_params disabled_params{0, 0, false};
    copy(make_copy(copy_gm_to_ub{}).with(disabled_params), dst_tensor, src_tensor);

    constexpr gm_to_ub_params enabled_params{0, 0, true};
    copy(make_copy(copy_gm_to_ub{}).with(enabled_params), dst_tensor, src_tensor);

    GlobalMockObject::verify();
}
