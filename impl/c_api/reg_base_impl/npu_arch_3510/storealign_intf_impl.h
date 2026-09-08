/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREALIGN_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREALIGN_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/npu_arch_3510/storealign_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREALIGN_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_impl.h"
#include "impl/c_api/reg_base_impl/storealign_intf_impl.h"

// ==========asc_storealign_pack_v2(float/u32/s32)=========
[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ int32_t* dst_align32b, vector_int32_t src, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ float* dst_align32b, vector_float src, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, mask);
}

// ==========asc_storealign_pack_v2(float/u32/s32)=========
[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ int32_t* dst_align32b, vector_int32_t src, addr_reg offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, addr_reg offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ float* dst_align32b, vector_float src, addr_reg offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

// storealign_pack_v2
[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ int32_t* dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ float* dst_align32b, vector_float src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

// storealign_pack_v2
[[deprecated("NOTICE: asc_storealign_pack_postupdate_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter_postupdate instead.")]] __simd_callee__ inline void
asc_storealign_pack_postupdate_v2(__ubuf__ int32_t*& dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_postupdate_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_postupdate_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter_postupdate instead.")]] __simd_callee__ inline void
asc_storealign_pack_postupdate_v2(
    __ubuf__ uint32_t*& dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_postupdate_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_postupdate_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter_postupdate instead.")]] __simd_callee__ inline void
asc_storealign_pack_postupdate_v2(__ubuf__ float*& dst_align32b, vector_float src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_postupdate_v2_impl(dst_align32b, src, offset, mask);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREALIGN_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREALIGN_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_STOREALIGN_INTF_IMPL_H
