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
#warning "c_api_instr_impl/vector_compute_c_api_impl/get_vms4_sr_impl/npu_arch_2201/asc_get_vms4_sr_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif    

#ifndef IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_GET_VMS4_SR_IMPL_ASC_2201_ASC_GET_VMS4_SR_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_GET_VMS4_SR_IMPL_ASC_2201_ASC_GET_VMS4_SR_IMPL_H

__aicore__ inline void asc_get_vms4_sr_impl(uint16_t sorted_num[4])
{
    if ASC_IS_AIV {
        uint8_t offset_step = 16;
        uint64_t vms4_sr = (uint64_t)get_vms4_sr();
        sorted_num[0] = 0xffff & vms4_sr;
        sorted_num[1] = 0xffff & (vms4_sr >> offset_step); // >> 16
        sorted_num[2] = 0xffff & (vms4_sr >> (offset_step * 2)); // >> 32
        sorted_num[3] = 0xffff & (vms4_sr >> (offset_step * 3)); // >> 48
    }
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC  
#endif  

