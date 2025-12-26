/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef IMPL_C_API_C_API_INSTR_IMPL_SYS_VAR_C_API_IMPL_SYS_VAR_C_API_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_SYS_VAR_C_API_IMPL_SYS_VAR_C_API_IMPL_H
 
#include "get_core_id_impl/asc_2201/asc_get_core_id_impl.h"
#include "get_ctrl_impl/asc_2201/asc_get_ctrl_impl.h"
#include "get_imm_impl/npu_arch_2201/asc_get_phy_buf_addr_impl.h"
#include "get_overflow_status_impl/asc_2201/asc_get_overflow_status_impl.h"
#include "get_sub_block_id_impl/npu_arch_2201/asc_get_sub_block_id_impl.h"
#include "get_sub_block_num_impl/npu_arch_2201/asc_get_sub_block_num_impl.h"
#include "get_sys_cnt_impl/npu_arch_2201/asc_get_sys_cnt_impl.h"
#include "set_ctrl_impl/asc_2201/asc_set_ctrl_impl.h"
#include "get_block_idx_impl/npu_arch_2201/asc_get_block_idx_impl.h"
#include "get_block_num_impl/npu_arch_2201/asc_get_block_num_impl.h"
#include "get_datablock_size/npu_arch_2201/asc_get_datablock_size_impl.h"
#include "get_arch_ver_impl/npu_arch_2201/asc_get_arch_ver_impl.h"
#include "get_program_counter/npu_arch_2201/get_program_counter_impl.h"

__aicore__ inline int64_t asc_get_core_id()
{
    return asc_get_core_id_impl();
}

__aicore__ inline int64_t asc_get_block_idx()
{
    return asc_get_block_idx_impl();
}

__aicore__ inline int64_t asc_get_block_num()
{
    return asc_get_block_num_impl();
}

__aicore__ inline int64_t asc_get_ctrl()
{
    return asc_get_ctrl_impl();
}

__aicore__ inline uint64_t asc_get_phy_buf_addr(uint64_t offset)
{
    return asc_get_phy_buf_addr_impl(offset);
}

__aicore__ inline uint64_t asc_get_overflow_status()
{
    return asc_get_overflow_status_impl();
}

__aicore__ inline int64_t asc_get_sub_block_id()
{
    return asc_get_sub_block_id_impl();
}

__aicore__ inline int64_t asc_get_sub_block_num()
{
    return asc_get_sub_block_num_impl();
}

__aicore__ inline int64_t asc_get_system_cycle()
{
    return asc_get_system_cycle_impl();
}

__aicore__ inline void asc_set_ctrl(uint64_t config)
{
    asc_set_ctrl_impl(config);
}

__aicore__ inline uint16_t asc_get_datablock_size()
{
    return asc_get_datablock_size_impl();
}

__aicore__ inline void asc_get_arch_ver(uint32_t& coreVersion)
{
    asc_get_arch_ver_impl(coreVersion);
}

__aicore__ inline int64_t asc_get_program_counter()
{
    return asc_get_program_counter_impl();
}

#endif