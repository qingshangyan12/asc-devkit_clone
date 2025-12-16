/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_C_API_INSTR_IMPL_ATOMIC_C_API_IMPL_ATOMIC_C_API_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_ATOMIC_C_API_IMPL_ATOMIC_C_API_IMPL_H

#include "set_atomic_min_impl/asc_2201/asc_set_atomic_min_impl.h"
#include "set_atomic_s8_impl/npu_arch_2201/set_atomic_s8_impl.h"
#include "set_atomic_type_c_api_impl/set_atomic_bf16_impl/npu_arch_2201/asc_set_atomic_bf16_impl.h"
#include "get_st_atomic_cfg_impl/npu_arch_2201/asc_get_store_atomic_config_impl.h"
#include "set_atomic_s16_impl/asc_2201/asc_set_atomic_s16_impl.h"
#include "set_atomic_f16_impl/asc_2201/set_atomic_f16_impl.h"
#include "set_st_atomic_cfg_impl/asc_2201/set_store_atomic_config_impl.h"

__aicore__ inline void asc_set_atomic_min()
{
    CApiInternal::asc_set_atomic_min_impl();
}

__aicore__ inline void asc_set_atomic_s8()
{
    CApiInternal::asc_set_atomic_s8_impl();
}

__aicore__ inline void asc_set_atomic_bf16()
{
    CApiInternal::asc_set_atomic_bf16_impl();
}

__aicore__ inline int64_t asc_get_store_atomic_config()
{
    return CApiInternal::asc_get_store_atomic_config_impl();
}

__aicore__ inline void asc_set_atomic_s16()
{
    CApiInternal::asc_set_atomic_s16_impl();
}
__aicore__ inline void asc_set_atomic_f16()
{
    CApiInternal::set_atomic_f16_impl();
}

__aicore__ inline void asc_set_store_atomic_config_none_sum()
{
    CApiInternal::set_store_atomic_config_none_sum_impl();
}

__aicore__ inline void asc_set_store_atomic_config_f32_sum()
{
    CApiInternal::set_store_atomic_config_f32_sum_impl();
}

__aicore__ inline void asc_set_store_atomic_config_f16_sum()
{
    CApiInternal::set_store_atomic_config_f16_sum_impl();
}

__aicore__ inline void asc_set_store_atomic_config_s16_sum()
{
    CApiInternal::set_store_atomic_config_s16_sum_impl();
}

__aicore__ inline void asc_set_store_atomic_config_s32_sum()
{
    CApiInternal::set_store_atomic_config_s32_sum_impl();
}

__aicore__ inline void asc_set_store_atomic_config_s8_sum()
{
    CApiInternal::set_store_atomic_config_s8_sum_impl();
}

__aicore__ inline void asc_set_store_atomic_config_bf16_sum()
{
    CApiInternal::set_store_atomic_config_bf16_sum_impl();
}

#endif