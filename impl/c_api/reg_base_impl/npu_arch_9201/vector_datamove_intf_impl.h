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
    "impl/c_api/reg_base_impl/npu_arch_9201/vector_datamove_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_VECTOR_DATAMOVE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_VECTOR_DATAMOVE_INTF_IMPL_H

// ==========asc_scatter_ub2gm===========
__aicore__ inline void asc_scatter_ub2gm(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint8_t offset_type, __ubuf__ void* offset_src,
    asc_store_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num)
{
    if ASC_IS_AIV {
        scatter_ubuf_to_gm(
            dst, src, 0, offset_type, offset_src, static_cast<uint8_t>(l2_cache_mode), burst_len, src_stride,
            offset_num);
    }
}

__aicore__ inline void asc_scatter_ub2gm(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t offset_type, __ubuf__ void* offset_src,
    asc_store_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num)
{
    if ASC_IS_AIV {
        scatter_ubuf_to_gm(
            dst, src, 0, offset_type, offset_src, static_cast<uint8_t>(l2_cache_mode), burst_len, src_stride,
            offset_num);
    }
}

__aicore__ inline void asc_scatter_ub2gm(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint8_t offset_type, __ubuf__ void* offset_src,
    asc_store_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num)
{
    if ASC_IS_AIV {
        scatter_ubuf_to_gm(
            dst, src, 0, offset_type, offset_src, static_cast<uint8_t>(l2_cache_mode), burst_len, src_stride,
            offset_num);
    }
}

__aicore__ inline void asc_scatter_ub2gm(
    __gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint8_t offset_type, __ubuf__ void* offset_src,
    asc_store_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num)
{
    if ASC_IS_AIV {
        scatter_ubuf_to_gm(
            dst, src, 0, offset_type, offset_src, static_cast<uint8_t>(l2_cache_mode), burst_len, src_stride,
            offset_num);
    }
}

// ==========asc_gather_gm2ub===========
__aicore__ inline void asc_gather_gm2ub(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offset_src, asc_load_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t dst_stride,
    uint32_t offset_num)
{
    if ASC_IS_AIV {
        gather_gm_to_ubuf(
            dst, src, 0, offset_type, constant_padding_ctl, offset_src, static_cast<uint8_t>(l2_cache_mode), burst_len,
            dst_stride, offset_num);
    }
}

__aicore__ inline void asc_gather_gm2ub(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offset_src, asc_load_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t dst_stride,
    uint32_t offset_num)
{
    if ASC_IS_AIV {
        gather_gm_to_ubuf(
            dst, src, 0, offset_type, constant_padding_ctl, offset_src, static_cast<uint8_t>(l2_cache_mode), burst_len,
            dst_stride, offset_num);
    }
}

__aicore__ inline void asc_gather_gm2ub(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offset_src, asc_load_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t dst_stride,
    uint32_t offset_num)
{
    if ASC_IS_AIV {
        gather_gm_to_ubuf(
            dst, src, 0, offset_type, constant_padding_ctl, offset_src, static_cast<uint8_t>(l2_cache_mode), burst_len,
            dst_stride, offset_num);
    }
}

__aicore__ inline void asc_gather_gm2ub(
    __ubuf__ uint64_t* dst, __gm__ uint64_t* src, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offset_src, asc_load_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t dst_stride,
    uint32_t offset_num)
{
    if ASC_IS_AIV {
        gather_gm_to_ubuf(
            dst, src, 0, offset_type, constant_padding_ctl, offset_src, static_cast<uint8_t>(l2_cache_mode), burst_len,
            dst_stride, offset_num);
    }
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
