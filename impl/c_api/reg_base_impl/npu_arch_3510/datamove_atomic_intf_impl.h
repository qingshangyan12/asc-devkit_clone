/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_DATAMOVE_ATOMIC_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_DATAMOVE_ATOMIC_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/npu_arch_3510/datamove_atomic_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_DATAMOVE_ATOMIC_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_impl.h"
#include "impl/c_api/reg_base_impl/datamove_atomic_intf_impl.h"

__aicore__ inline void asc_set_atomic_none_impl() { set_atomic_none(); }

__aicore__ inline void asc_set_atomic_add_int_impl()
{
    set_atomic_add();
    set_atomic_s32();
}

__aicore__ inline void asc_set_atomic_max_int_impl()
{
    set_atomic_max();
    set_atomic_s32();
}

__aicore__ inline void asc_set_atomic_min_int_impl()
{
    set_atomic_min();
    set_atomic_s32();
}

[[deprecated("NOTICE: asc_set_atomic_none is deprecated. Please use asc_disable_dma_atomic instead.")]]
__aicore__ inline void asc_set_atomic_none()
{
    asc_disable_dma_atomic();
}

/// \deprecated Use asc_set_atomic_add_int32 instead.
[[deprecated("NOTICE: asc_set_atomic_add_int is deprecated. Please use asc_set_atomic_add_int32 instead.")]]
__aicore__ inline void asc_set_atomic_add_int()
{
    asc_set_atomic_add_int_impl();
}

/// \deprecated Use asc_set_atomic_max_int32 instead.
[[deprecated("NOTICE: asc_set_atomic_max_int is deprecated. Please use asc_set_atomic_max_int32 instead.")]]
__aicore__ inline void asc_set_atomic_max_int()
{
    asc_set_atomic_max_int_impl();
}

/// \deprecated Use asc_set_atomic_min_int32 instead.
[[deprecated("NOTICE: asc_set_atomic_min_int is deprecated. Please use asc_set_atomic_min_int32 instead.")]]
__aicore__ inline void asc_set_atomic_min_int()
{
    asc_set_atomic_min_int_impl();
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_DATAMOVE_ATOMIC_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_DATAMOVE_ATOMIC_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_DATAMOVE_ATOMIC_INTF_IMPL_H
