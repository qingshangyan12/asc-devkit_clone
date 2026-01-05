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
#warning "impl/c_api/c_api_instr_impl/atomic_c_api_impl/atomic_c_api_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif    

#ifndef IMPL_C_API_C_API_INSTR_IMPL_ATOMIC_C_API_IMPL_ATOMIC_C_API_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_ATOMIC_C_API_IMPL_ATOMIC_C_API_IMPL_H

#include "set_atomic_max_impl/npu_arch_2201/asc_set_atomic_max_impl.h"
#include "set_atomic_add_impl/npu_arch_2201/asc_set_atomic_add_impl.h"
#include "set_atomic_none_impl/npu_arch_2201/asc_set_atomic_none_impl.h"
#include "set_atomic_min_impl/npu_arch_2201/asc_set_atomic_min_impl.h"
#include "set_atomic_s8_impl/npu_arch_2201/set_atomic_s8_impl.h"
#include "set_atomic_type_c_api_impl/set_atomic_bf16_impl/npu_arch_2201/asc_set_atomic_bf16_impl.h"
#include "get_st_atomic_cfg_impl/npu_arch_2201/asc_get_store_atomic_config_impl.h"
#include "set_atomic_s16_impl/npu_arch_2201/asc_set_atomic_s16_impl.h"
#include "set_atomic_type_c_api_impl/set_atomic_f16_impl/npu_arch_2201/asc_set_atomic_f16_impl.h"
#include "set_st_atomic_cfg_impl/npu_arch_2201/asc_set_store_atomic_config_impl.h"
#include "set_atomic_s32_impl/npu_arch_2201/asc_set_atomic_s32_impl.h"
#include "set_atomic_f32_impl/npu_arch_2201/asc_set_atomic_f32_impl.h"

__aicore__ inline void asc_set_atomic_add()
{
    asc_set_atomic_add_impl();
}

__aicore__ inline void asc_set_atomic_none()
{
    asc_set_atomic_none_impl();
}

__aicore__ inline void asc_set_atomic_min()
{
    asc_set_atomic_min_impl();
}

__aicore__ inline void asc_set_atomic_max()
{
    asc_set_atomic_max_impl();
}

__aicore__ inline void asc_set_atomic_s8()
{
    asc_set_atomic_s8_impl();
}

__aicore__ inline void asc_set_atomic_s32()
{
    asc_set_atomic_s32_impl();
}

__aicore__ inline void asc_set_atomic_bf16()
{
    asc_set_atomic_bf16_impl();
}

__aicore__ inline void asc_get_store_atomic_config(asc_store_atomic_config& config)
{
    asc_get_store_atomic_config_impl(config);
}

__aicore__ inline void asc_set_atomic_s16()
{
    asc_set_atomic_s16_impl();
}

__aicore__ inline void asc_set_atomic_f16()
{
    asc_set_atomic_f16_impl();
}

__aicore__ inline void asc_set_store_atomic_config(asc_store_atomic_config& config)
{
    asc_set_store_atomic_config_impl(config);
}

__aicore__ inline void asc_set_atomic_f32()
{
    asc_set_atomic_f32_impl();
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif  

