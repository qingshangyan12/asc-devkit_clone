/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/c_api/reg_base_impl/npu_arch_9201/reg_reduce_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_REG_REDUCE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_REG_REDUCE_INTF_IMPL_H

#include "impl/c_api/reg_base_impl/utils_impl.h"

__simd_callee__ inline void asc_datablock_reduce_max_stride(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmaxv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_max_stride(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmaxv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_max_stride(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmaxv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_max_stride(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmaxv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_max_stride(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmaxv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_max_stride(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmaxv2(dst, src, mask);
    }
}

__simd_callee__ inline vector_half asc_datablock_reduce_max_stride(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_datablock_reduce_max_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_datablock_reduce_max_stride(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_datablock_reduce_max_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_datablock_reduce_max_stride(vector_uint16_t src, vector_bool mask)
{
    vector_uint16_t dst;
    asc_datablock_reduce_max_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_datablock_reduce_max_stride(vector_int16_t src, vector_bool mask)
{
    vector_int16_t dst;
    asc_datablock_reduce_max_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_datablock_reduce_max_stride(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_datablock_reduce_max_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_datablock_reduce_max_stride(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_datablock_reduce_max_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_datablock_reduce_min_stride(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgminv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_min_stride(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgminv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_min_stride(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgminv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_min_stride(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgminv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_min_stride(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgminv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_min_stride(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgminv2(dst, src, mask);
    }
}

__simd_callee__ inline vector_half asc_datablock_reduce_min_stride(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_datablock_reduce_min_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_datablock_reduce_min_stride(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_datablock_reduce_min_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_datablock_reduce_min_stride(vector_uint16_t src, vector_bool mask)
{
    vector_uint16_t dst;
    asc_datablock_reduce_min_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_datablock_reduce_min_stride(vector_int16_t src, vector_bool mask)
{
    vector_int16_t dst;
    asc_datablock_reduce_min_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_datablock_reduce_min_stride(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_datablock_reduce_min_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_datablock_reduce_min_stride(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_datablock_reduce_min_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_datablock_reduce_sum_stride(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgaddv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_sum_stride(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgaddv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_sum_stride(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgaddv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_sum_stride(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgaddv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_sum_stride(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgaddv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_datablock_reduce_sum_stride(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgaddv2(dst, src, mask);
    }
}

__simd_callee__ inline vector_half asc_datablock_reduce_sum_stride(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_datablock_reduce_sum_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_datablock_reduce_sum_stride(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_datablock_reduce_sum_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_datablock_reduce_sum_stride(vector_uint16_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_datablock_reduce_sum_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_datablock_reduce_sum_stride(vector_int16_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_datablock_reduce_sum_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_datablock_reduce_sum_stride(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_datablock_reduce_sum_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_datablock_reduce_sum_stride(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_datablock_reduce_sum_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_pair_reduce_sum_stride(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcpaddv2(dst, src, mask);
    }
}

__simd_callee__ inline void asc_pair_reduce_sum_stride(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcpaddv2(dst, src, mask);
    }
}

__simd_callee__ inline vector_half asc_pair_reduce_sum_stride(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_pair_reduce_sum_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_pair_reduce_sum_stride(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_pair_reduce_sum_stride(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_reduce_max_last_elem(vector_half& dst, vector_half src)
{
    if ASC_IS_AIV {
        vcmaxv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_max_last_elem(vector_float& dst, vector_float src)
{
    if ASC_IS_AIV {
        vcmaxv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_max_last_elem(vector_uint16_t& dst, vector_uint16_t src)
{
    if ASC_IS_AIV {
        vcmaxv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_max_last_elem(vector_int16_t& dst, vector_int16_t src)
{
    if ASC_IS_AIV {
        vcmaxv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_max_last_elem(vector_uint32_t& dst, vector_uint32_t src)
{
    if ASC_IS_AIV {
        vcmaxv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_max_last_elem(vector_int32_t& dst, vector_int32_t src)
{
    if ASC_IS_AIV {
        vcmaxv2(dst, src);
    }
}

__simd_callee__ inline vector_half asc_reduce_max_last_elem(vector_half src)
{
    vector_half dst;
    asc_reduce_max_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_float asc_reduce_max_last_elem(vector_float src)
{
    vector_float dst;
    asc_reduce_max_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_reduce_max_last_elem(vector_uint16_t src)
{
    vector_uint16_t dst;
    asc_reduce_max_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_reduce_max_last_elem(vector_int16_t src)
{
    vector_int16_t dst;
    asc_reduce_max_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_max_last_elem(vector_uint32_t src)
{
    vector_uint32_t dst;
    asc_reduce_max_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_reduce_max_last_elem(vector_int32_t src)
{
    vector_int32_t dst;
    asc_reduce_max_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline void asc_reduce_min_last_elem(vector_half& dst, vector_half src)
{
    if ASC_IS_AIV {
        vcminv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_min_last_elem(vector_float& dst, vector_float src)
{
    if ASC_IS_AIV {
        vcminv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_min_last_elem(vector_uint16_t& dst, vector_uint16_t src)
{
    if ASC_IS_AIV {
        vcminv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_min_last_elem(vector_int16_t& dst, vector_int16_t src)
{
    if ASC_IS_AIV {
        vcminv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_min_last_elem(vector_uint32_t& dst, vector_uint32_t src)
{
    if ASC_IS_AIV {
        vcminv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_min_last_elem(vector_int32_t& dst, vector_int32_t src)
{
    if ASC_IS_AIV {
        vcminv2(dst, src);
    }
}

__simd_callee__ inline vector_half asc_reduce_min_last_elem(vector_half src)
{
    vector_half dst;
    asc_reduce_min_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_float asc_reduce_min_last_elem(vector_float src)
{
    vector_float dst;
    asc_reduce_min_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_reduce_min_last_elem(vector_uint16_t src)
{
    vector_uint16_t dst;
    asc_reduce_min_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_reduce_min_last_elem(vector_int16_t src)
{
    vector_int16_t dst;
    asc_reduce_min_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_min_last_elem(vector_uint32_t src)
{
    vector_uint32_t dst;
    asc_reduce_min_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_reduce_min_last_elem(vector_int32_t src)
{
    vector_int32_t dst;
    asc_reduce_min_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline void asc_reduce_sum_last_elem(vector_int32_t& dst, vector_int32_t src)
{
    if ASC_IS_AIV {
        vcaddv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_sum_last_elem(vector_uint32_t& dst, vector_uint32_t src)
{
    if ASC_IS_AIV {
        vcaddv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_sum_last_elem(vector_half& dst, vector_half src)
{
    if ASC_IS_AIV {
        vcaddv2(dst, src);
    }
}

__simd_callee__ inline void asc_reduce_sum_last_elem(vector_float& dst, vector_float src)
{
    if ASC_IS_AIV {
        vcaddv2(dst, src);
    }
}

__simd_callee__ inline vector_int32_t asc_reduce_sum_last_elem(vector_int32_t src)
{
    vector_int32_t dst;
    asc_reduce_sum_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_sum_last_elem(vector_uint32_t src)
{
    vector_uint32_t dst;
    asc_reduce_sum_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_half asc_reduce_sum_last_elem(vector_half src)
{
    vector_half dst;
    asc_reduce_sum_last_elem(dst, src);
    return dst;
}

__simd_callee__ inline vector_float asc_reduce_sum_last_elem(vector_float src)
{
    vector_float dst;
    asc_reduce_sum_last_elem(dst, src);
    return dst;
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
