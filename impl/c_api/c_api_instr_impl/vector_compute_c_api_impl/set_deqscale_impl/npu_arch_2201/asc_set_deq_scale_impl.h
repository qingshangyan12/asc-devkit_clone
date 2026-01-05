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
#warning "c_api_instr_impl/vector_compute_c_api_impl/set_deqscale_impl/npu_arch_2201/asc_set_deq_scale_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif    

#ifndef IMPL_C_API_INSTR_VECTOR_COMPUTE_SET_DEQSCALE_NPU_ARCH_2201_ASC_SET_DEQSCALE_IMPL_H
#define IMPL_C_API_INSTR_VECTOR_COMPUTE_SET_DEQSCALE_NPU_ARCH_2201_ASC_SET_DEQSCALE_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_set_deq_scale_impl(const asc_deq_scale_config config)
{
    if ASC_IS_AIV {
        set_deqscale(config.val);
    }
}

__aicore__ inline void asc_set_deq_scale_impl(__ubuf__ uint64_t* config)
{
    if ASC_IS_AIV {
        constexpr uint64_t deq_addr = 5;
        set_deqscale((uint64_t)config >> deq_addr);
    }
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif  

