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
 * \file set_store_atomic_config_impl.h
 * \brief
 */
#ifndef IMPL_C_API_INSTR_SIMD_ATOMIC_SET_ST_ATOMIC_CFG_ASC_2201_SET_STORE_ATOMIC_CONFIG_IMPL_H
#define IMPL_C_API_INSTR_SIMD_ATOMIC_SET_ST_ATOMIC_CFG_ASC_2201_SET_STORE_ATOMIC_CONFIG_IMPL_H
#include "c_api_utils_intf.h"

#define CAPI_ATOMIC_TYPE_NONE    0
#define CAPI_ATOMIC_TYPE_F32     1
#define CAPI_ATOMIC_TYPE_F16     2
#define CAPI_ATOMIC_TYPE_S16     3
#define CAPI_ATOMIC_TYPE_S32     4
#define CAPI_ATOMIC_TYPE_S8      5
#define CAPI_ATOMIC_TYPE_BF16    6
#define CAPI_ATOMIC_OP_SUM       0

namespace CApiInternal {

__aicore__ inline void set_store_atomic_config_none_sum_impl()
{
    set_st_atomic_cfg(static_cast<atomic_type_t>(CAPI_ATOMIC_TYPE_NONE), static_cast<atomic_op_t>(CAPI_ATOMIC_OP_SUM));
}

__aicore__ inline void set_store_atomic_config_f32_sum_impl()
{
    set_st_atomic_cfg(static_cast<atomic_type_t>(CAPI_ATOMIC_TYPE_F32), static_cast<atomic_op_t>(CAPI_ATOMIC_OP_SUM));
}

__aicore__ inline void set_store_atomic_config_f16_sum_impl()
{
    set_st_atomic_cfg(static_cast<atomic_type_t>(CAPI_ATOMIC_TYPE_F16), static_cast<atomic_op_t>(CAPI_ATOMIC_OP_SUM));
}

__aicore__ inline void set_store_atomic_config_s16_sum_impl()
{
    set_st_atomic_cfg(static_cast<atomic_type_t>(CAPI_ATOMIC_TYPE_S16), static_cast<atomic_op_t>(CAPI_ATOMIC_OP_SUM));
}

__aicore__ inline void set_store_atomic_config_s32_sum_impl()
{
    set_st_atomic_cfg(static_cast<atomic_type_t>(CAPI_ATOMIC_TYPE_S32), static_cast<atomic_op_t>(CAPI_ATOMIC_OP_SUM));
}

__aicore__ inline void set_store_atomic_config_s8_sum_impl()
{
    set_st_atomic_cfg(static_cast<atomic_type_t>(CAPI_ATOMIC_TYPE_S8), static_cast<atomic_op_t>(CAPI_ATOMIC_OP_SUM));
}

__aicore__ inline void set_store_atomic_config_bf16_sum_impl()
{
    set_st_atomic_cfg(static_cast<atomic_type_t>(CAPI_ATOMIC_TYPE_BF16), static_cast<atomic_op_t>(CAPI_ATOMIC_OP_SUM));
}
 
} // namespace CApiInternal
 
#endif