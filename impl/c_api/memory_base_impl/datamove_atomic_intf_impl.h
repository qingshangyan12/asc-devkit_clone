/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/c_api/memory_base_impl/datamove_atomic_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_DATAMOVE_ATOMIC_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_DATAMOVE_ATOMIC_INTF_IMPL_H

__aicore__ inline void asc_set_atomic_none_impl() { set_atomic_none(); }

__aicore__ inline void asc_disable_dma_atomic() { set_atomic_none(); }

[[deprecated("NOTICE: asc_set_atomic_none is deprecated. Please use asc_disable_dma_atomic instead.")]]
__aicore__ inline void asc_set_atomic_none()
{
    asc_disable_dma_atomic();
}

__aicore__ inline void asc_set_atomic_add_bfloat()
{
    set_atomic_add();
    set_atomic_bf16();
}

__aicore__ inline void asc_set_atomic_add_float()
{
    set_atomic_add();
    set_atomic_f32();
}

__aicore__ inline void asc_set_atomic_add_float16()
{
    set_atomic_add();
    set_atomic_f16();
}

[[deprecated("NOTICE: asc_set_atomic_add_int is deprecated. Please use asc_set_atomic_add_int32 instead.")]]
__aicore__ inline void asc_set_atomic_add_int()
{
    set_atomic_add();
    set_atomic_s32();
}

__aicore__ inline void asc_set_atomic_add_int8()
{
    set_atomic_add();
    set_atomic_s8();
}

__aicore__ inline void asc_set_atomic_add_int16()
{
    set_atomic_add();
    set_atomic_s16();
}

__aicore__ inline void asc_set_atomic_add_int32()
{
    set_atomic_add();
    set_atomic_s32();
}

__aicore__ inline void asc_set_atomic_max_bfloat()
{
    set_atomic_max();
    set_atomic_bf16();
}

__aicore__ inline void asc_set_atomic_max_float()
{
    set_atomic_max();
    set_atomic_f32();
}

__aicore__ inline void asc_set_atomic_max_float16()
{
    set_atomic_max();
    set_atomic_f16();
}

[[deprecated("NOTICE: asc_set_atomic_max_int is deprecated. Please use asc_set_atomic_max_int32 instead.")]]
__aicore__ inline void asc_set_atomic_max_int()
{
    set_atomic_max();
    set_atomic_s32();
}

__aicore__ inline void asc_set_atomic_max_int8()
{
    set_atomic_max();
    set_atomic_s8();
}

__aicore__ inline void asc_set_atomic_max_int16()
{
    set_atomic_max();
    set_atomic_s16();
}

__aicore__ inline void asc_set_atomic_max_int32()
{
    set_atomic_max();
    set_atomic_s32();
}

__aicore__ inline void asc_set_atomic_min_bfloat()
{
    set_atomic_min();
    set_atomic_bf16();
}

__aicore__ inline void asc_set_atomic_min_float()
{
    set_atomic_min();
    set_atomic_f32();
}

__aicore__ inline void asc_set_atomic_min_float16()
{
    set_atomic_min();
    set_atomic_f16();
}

[[deprecated("NOTICE: asc_set_atomic_min_int is deprecated. Please use asc_set_atomic_min_int32 instead.")]]
__aicore__ inline void asc_set_atomic_min_int()
{
    set_atomic_min();
    set_atomic_s32();
}

__aicore__ inline void asc_set_atomic_min_int8()
{
    set_atomic_min();
    set_atomic_s8();
}

__aicore__ inline void asc_set_atomic_min_int16()
{
    set_atomic_min();
    set_atomic_s16();
}

__aicore__ inline void asc_set_atomic_min_int32()
{
    set_atomic_min();
    set_atomic_s32();
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
