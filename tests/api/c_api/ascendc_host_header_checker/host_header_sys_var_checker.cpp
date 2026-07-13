/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "c_api/sys_var/sys_var.h"

static void test_host_c_api_sys_var_0()
{
    (void)static_cast<int64_t(*)()>(&asc_get_ar_spr);
    (void)static_cast<void(*)(uint32_t&)>(&asc_get_arch_ver);
    (void)static_cast<int64_t(*)()>(&asc_get_block_idx);
    (void)static_cast<int64_t(*)()>(&asc_get_block_num);
    (void)static_cast<int64_t(*)()>(&asc_get_core_id);
    (void)static_cast<int64_t(*)()>(&asc_get_ctrl);
    (void)static_cast<int64_t(*)()>(&asc_get_ffts_base_addr);
    (void)static_cast<uint64_t(*)()>(&asc_get_overflow_status);
    (void)static_cast<uint64_t(*)(uint64_t)>(&asc_get_phy_buf_addr);
    (void)static_cast<int64_t(*)()>(&asc_get_phy_stack_base);
    (void)static_cast<int64_t(*)()>(&asc_get_program_counter);
    (void)static_cast<int64_t(*)()>(&asc_get_smmu_tag_version);
    (void)static_cast<int64_t(*)()>(&asc_get_squeeze_status);
    (void)static_cast<int64_t(*)()>(&asc_get_status);
    (void)static_cast<int64_t(*)()>(&asc_get_sub_block_id);
    (void)static_cast<int64_t(*)()>(&asc_get_sub_block_num);
    (void)static_cast<int64_t(*)()>(&asc_get_sys_virtual_base);
    (void)static_cast<int64_t(*)()>(&asc_get_system_cycle);
    (void)static_cast<int64_t(*)()>(&asc_get_vf_len);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_ctrl);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_ffts_base_addr);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_gm2l1_nz_para);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_l0c2gm_channel_para);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_l0c2gm_quant_post);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_l0c2gm_relu_alpha);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_l12l0_padding_val);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_l13d_padding);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_l3d_rpt_b);
}

