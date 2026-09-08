/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_ATOMIC_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_ATOMIC_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/scalar_atomic_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_ATOMIC_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/common_impl.h"

__aicore__ inline void asc_get_store_atomic_config_impl(asc_store_atomic_config& config)
{
    config.config = get_st_atomic_cfg();
}

__aicore__ inline void asc_set_store_atomic_config_v1_impl(uint16_t type, uint16_t op)
{
    constexpr uint64_t type_mask = 0x7;
    constexpr uint64_t op_mask = 0x3;
    constexpr uint64_t op_bit = 3;
    uint64_t config = (type & type_mask) | ((op & op_mask) << op_bit);
    set_st_atomic_cfg(config);
}

__aicore__ inline void asc_get_store_atomic_config(asc_store_atomic_config& config)
{
    asc_get_store_atomic_config_impl(config);
}

__aicore__ inline void asc_set_store_atomic_config_v1(uint16_t type, uint16_t op)
{
    asc_set_store_atomic_config_v1_impl(type, op);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_ATOMIC_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_ATOMIC_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_SCALAR_ATOMIC_INTF_IMPL_H
