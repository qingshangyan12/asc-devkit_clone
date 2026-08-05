/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file asc_create_mask_impl.h
 * \brief
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_create_mask_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_CREATE_MASK_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_CREATE_MASK_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/utils_impl.h"

__aicore__ inline void dispatch_pat_mode(Pat pat_mode)
{
    if (pat_mode == PAT_ALL) {
        pset_b8(PAT_ALL);
    } else if(pat_mode == PAT_VL1) {
        pset_b8(PAT_VL1);
    } else if(pat_mode == PAT_VL2) {
        pset_b8(PAT_VL2);
    } else if(pat_mode == PAT_VL3) {
        pset_b8(PAT_VL3);
    } else if(pat_mode == PAT_VL4) {
        pset_b8(PAT_VL4);
    } else if(pat_mode == PAT_VL8) {
        pset_b8(PAT_VL8);
    } else if(pat_mode == PAT_VL16) {
        pset_b8(PAT_VL16);
    } else if(pat_mode == PAT_VL32) {
        pset_b8(PAT_VL32);
    } else if(pat_mode == PAT_VL64) {
        pset_b8(PAT_VL64);
    } else if(pat_mode == PAT_VL128) {
        pset_b8(PAT_VL128);
    } else if(pat_mode == PAT_M3) {
        pset_b8(PAT_M3);
    } else if(pat_mode == PAT_M4) {
        pset_b8(PAT_M4);
    } else if(pat_mode == PAT_H) {
        pset_b8(PAT_H);
    } else if(pat_mode == PAT_Q) {
        pset_b8(PAT_Q);
    } else if(pat_mode == PAT_ALLF) {
        pset_b8(PAT_ALLF);
    } else {}
}

__simd_callee__ inline void asc_create_mask_b8_impl(Pat pat_mode)
{
    if ASC_IS_AIV
    {
        dispatch_pat_mode(pat_mode);
    }
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
