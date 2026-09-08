/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_MASK_CONFIG_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_MASK_CONFIG_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/vector_mask_config_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_MASK_CONFIG_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/common_impl.h"

__aicore__ inline void asc_set_mask_count_impl()
{
    if ASC_IS_AIV {
        set_mask_count();
    }
}

__aicore__ inline void asc_set_mask_norm_impl()
{
    if ASC_IS_AIV {
        set_mask_norm();
    }
}

__aicore__ inline void asc_set_vector_mask_impl(uint64_t mask_high, uint64_t mask_low)
{
    if ASC_IS_AIV {
        set_vector_mask(mask_high, mask_low);
    }
}

__aicore__ inline void asc_set_mask_count() { asc_set_mask_count_impl(); }

__aicore__ inline void asc_set_mask_norm() { asc_set_mask_norm_impl(); }

__aicore__ inline void asc_set_vector_mask(uint64_t mask_high, uint64_t mask_low)
{
    asc_set_vector_mask_impl(mask_high, mask_low);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_MASK_CONFIG_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_MASK_CONFIG_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_MASK_CONFIG_INTF_IMPL_H
