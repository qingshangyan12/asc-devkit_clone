/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_SYS_VAR_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_SYS_VAR_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_ar_spr_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_program_counter_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_ctrl_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_block_idx_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_block_num_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_system_cycle_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_ctrl_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_phy_buf_addr_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_vf_len_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_gm2l1_nz_para_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l0c2gm_quant_post_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l0c2gm_relu_alpha_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l12l0_padding_val_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l13d_padding_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_arch_ver_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_core_id_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l0c2gm_channel_para_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l3d_rpt_b_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_smmu_tag_version_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_phy_stack_base_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_status_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_sub_block_id_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_sub_block_num_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_sys_virtual_base_impl.h"
#include "utils/base/sys_enums.h"

[[deprecated("NOTICE: asc_get_ar_spr is deprecated. "
             "Please use asc_get_squeeze_status instead.")]] __aicore__ inline int64_t
asc_get_ar_spr()
{
    return asc_get_ar_spr_impl();
}

__aicore__ inline int64_t asc_get_squeeze_status() { return asc_get_squeeze_status_impl(); }

__aicore__ inline int64_t asc_get_program_counter() { return asc_get_program_counter_impl(); }

__aicore__ inline int64_t asc_get_block_idx() { return asc_get_block_idx_impl(); }

__aicore__ inline int64_t asc_get_sub_block_num() { return asc_get_sub_block_num_impl(); }

__aicore__ inline int64_t asc_get_sub_block_id() { return asc_get_sub_block_id_impl(); }

__aicore__ inline int64_t asc_get_block_num() { return asc_get_block_num_impl(); }

__aicore__ inline int64_t asc_get_system_cycle() { return asc_get_system_cycle_impl(); }

__aicore__ inline void asc_set_ctrl(uint64_t config) { asc_set_ctrl_impl(config); }

__aicore__ inline int64_t asc_get_ctrl() { return asc_get_ctrl_impl(); }

__aicore__ inline constexpr int64_t asc_get_vf_len() { return asc_get_vf_len_impl(); }

__aicore__ inline uint64_t asc_get_phy_buf_addr(uint64_t offset) { return asc_get_phy_buf_addr_impl(offset); }

__aicore__ inline void asc_get_arch_ver(uint32_t& core_version) { asc_get_arch_ver_impl(core_version); }

__aicore__ inline int64_t asc_get_core_id() { return asc_get_core_id_impl(); }

__aicore__ inline void asc_set_l0c2gm_channel_para(uint64_t config) { asc_set_l0c2gm_channel_para_impl(config); }

__aicore__ inline void asc_set_l3d_rpt_b(uint64_t config) { asc_set_l3d_rpt_b_impl(config); }

__aicore__ inline void asc_set_gm2l1_nz_para(uint64_t config) { asc_set_gm2l1_nz_para_impl(config); }

__aicore__ inline void asc_set_l13d_padding(uint64_t config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l13d_padding(half config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l13d_padding(int16_t config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l13d_padding(uint16_t config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l12l0_padding_val(uint64_t config) { asc_set_l12l0_padding_val_impl(config); }

__aicore__ inline void asc_set_l0c2gm_quant_post(uint64_t config) { asc_set_l0c2gm_quant_post_impl(config); }

__aicore__ inline void asc_set_l0c2gm_relu_alpha(uint64_t config) { asc_set_l0c2gm_relu_alpha_impl(config); }

__aicore__ inline int64_t asc_get_smmu_tag_version() { return asc_get_smmu_tag_version_impl(); }

__aicore__ inline int64_t asc_get_phy_stack_base() { return asc_get_phy_stack_base_impl(); }

__aicore__ inline int64_t asc_get_status() { return asc_get_status_impl(); }

__aicore__ inline int64_t asc_get_sys_virtual_base() { return asc_get_sys_virtual_base_impl(); }

__aicore__ inline void asc_set_saturation_flag(asc_saturation_mode saturation_mode, bool enable_sat)
{
    int8_t sprBit = 0;
    bool invertLogic = false;
    switch (saturation_mode) {
        case asc_saturation_mode::FLOAT:
            sprBit = 48; // 0 => sat, 1 => no sat
            break;
        case asc_saturation_mode::FLOAT8:
            sprBit = 50; // 0 => sat, 1 => no sat
            break;
        case asc_saturation_mode::INT:
            sprBit = 53; // 0 => truncation, 1 => saturation
            invertLogic = true;
            break;
        default:         // asc_saturation_mode::CAST
            sprBit = 59; // 0 => saturation, 1 => truncation
            break;
    }
    int64_t ctrlValue = get_ctrl();
    uint64_t value;
    if (invertLogic) {
        value = enable_sat ? sbitset1(ctrlValue, sprBit) : sbitset0(ctrlValue, sprBit);
    } else {
        value = enable_sat ? sbitset0(ctrlValue, sprBit) : sbitset1(ctrlValue, sprBit);
    }
    set_ctrl(value);
}

__aicore__ inline bool asc_get_saturation_flag(asc_saturation_mode saturation_mode)
{
    int8_t sprBit = 0;
    bool invertLogic = false;
    switch (saturation_mode) {
        case asc_saturation_mode::FLOAT:
            sprBit = 48;
            break;
        case asc_saturation_mode::FLOAT8:
            sprBit = 50;
            break;
        case asc_saturation_mode::INT:
            sprBit = 53;
            invertLogic = true;
            break;
        default: // asc_saturation_mode::CAST
            sprBit = 59;
            break;
    }
    int64_t value = (get_ctrl() >> sprBit) & 1;
    if (invertLogic) {
        return value != 0;
    }
    return value == 0;
}

__aicore__ inline void asc_set_saturation_strategy(asc_override_strategy strategy)
{
    int64_t ctrlValue = get_ctrl();
    if (strategy == asc_override_strategy::USE_API) {
        set_ctrl(sbitset0(ctrlValue, 60));
    } else {
        set_ctrl(sbitset1(ctrlValue, 60));
    }
}

__aicore__ inline asc_override_strategy asc_get_saturation_strategy()
{
    int64_t value = (get_ctrl() >> 60) & 1;
    return value == 0 ? asc_override_strategy::USE_API : asc_override_strategy::USE_GLOBAL;
}

#endif
