/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_set_store_atomic_config_impl.h
 * \brief
 */
#ifndef IMPL_C_API_INSTR_SIMD_ATOMIC_SET_ST_ATOMIC_CFG_IMPL_NPU_ARCH_2201_ASC_SET_STORE_ATOMIC_CONFIG_IMPL_H
#define IMPL_C_API_INSTR_SIMD_ATOMIC_SET_ST_ATOMIC_CFG_IMPL_NPU_ARCH_2201_ASC_SET_STORE_ATOMIC_CONFIG_IMPL_H
#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_set_store_atomic_config_none_sum_impl()
{
    set_st_atomic_cfg(ATOMIC_NONE, ATOMIC_SUM);
}

__aicore__ inline void asc_set_store_atomic_config_f32_sum_impl()
{
    set_st_atomic_cfg(ATOMIC_F32, ATOMIC_SUM);
}

__aicore__ inline void asc_set_store_atomic_config_f16_sum_impl()
{
    set_st_atomic_cfg(ATOMIC_F16, ATOMIC_SUM);
}

__aicore__ inline void asc_set_store_atomic_config_s16_sum_impl()
{
    set_st_atomic_cfg(ATOMIC_S16, ATOMIC_SUM);
}

__aicore__ inline void asc_set_store_atomic_config_s32_sum_impl()
{
    set_st_atomic_cfg(ATOMIC_S32, ATOMIC_SUM);
}

__aicore__ inline void asc_set_store_atomic_config_s8_sum_impl()
{
    set_st_atomic_cfg(ATOMIC_S8, ATOMIC_SUM);
}

__aicore__ inline void asc_set_store_atomic_config_bf16_sum_impl()
{
    set_st_atomic_cfg(ATOMIC_BF16, ATOMIC_SUM);
}
 
#endif