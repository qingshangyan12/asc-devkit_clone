/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file asc_aicore_dump_utils.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_NPU_ARCH_3510_ASC_AICORE_DUMP_UTILS_H
#define IMPL_UTILS_DEBUG_NPU_ARCH_3510_ASC_AICORE_DUMP_UTILS_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_UTILS__
#warning "asc_aicore_dump_utils.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future."
#endif

#include "impl/utils/debug/asc_debug_utils.h"
namespace __asc_aicore {

template <typename T>
__aicore__ constexpr inline DumpTensorDataType GetTensorDataType()
{
    if constexpr (AscendC::Std::is_same<T, bool>::value) {
        return DumpTensorDataType::ACL_BOOL;
    } else if (AscendC::Std::is_same<T, uint8_t>::value) {
        return DumpTensorDataType::ACL_UINT8;
    } else if (AscendC::Std::is_same<T, int8_t>::value) {
        return DumpTensorDataType::ACL_INT8;
    } else if (AscendC::Std::is_same<T, int16_t>::value) {
        return DumpTensorDataType::ACL_INT16;
    } else if (AscendC::Std::is_same<T, uint16_t>::value) {
        return DumpTensorDataType::ACL_UINT16;
    } else if (AscendC::Std::is_same<T, int32_t>::value) {
        return DumpTensorDataType::ACL_INT32;
    } else if (AscendC::Std::is_same<T, uint32_t>::value) {
        return DumpTensorDataType::ACL_UINT32;
    } else if (AscendC::Std::is_same<T, uint64_t>::value) {
        return DumpTensorDataType::ACL_UINT64;
    } else if (AscendC::Std::is_same<T, int64_t>::value) {
        return DumpTensorDataType::ACL_INT64;
    } else if (AscendC::Std::is_same<T, float>::value) {
        return DumpTensorDataType::ACL_FLOAT;
    } else if (AscendC::Std::is_same<T, half>::value) {
        return DumpTensorDataType::ACL_FLOAT16;
    } else if (AscendC::Std::is_same<T, bfloat16_t>::value) {
        return DumpTensorDataType::ACL_BF16;
    } else if (AscendC::Std::is_same<T, hifloat8_t>::value) {
        return DumpTensorDataType::ACL_HIFLOAT8;
    } else if (AscendC::Std::is_same<T, fp8_e5m2_t>::value) {
        return DumpTensorDataType::ACL_FLOAT8_E5M2;
    } else if (AscendC::Std::is_same<T, fp8_e4m3fn_t>::value) {
        return DumpTensorDataType::ACL_FLOAT8_E4M3FN;
    } else if (AscendC::Std::is_same<T, fp8_e8m0_t>::value) {
        return DumpTensorDataType::ACL_FLOAT8_E8M0;
    } else if (AscendC::Std::is_same<T, fp4x2_e2m1_t>::value) {
        return DumpTensorDataType::ACL_FLOAT4_E2M1;
    } else if (AscendC::Std::is_same<T, fp4x2_e1m2_t>::value) {
        return DumpTensorDataType::ACL_FLOAT4_E1M2;
    } else {
        return DumpTensorDataType::ACL_MAX;
    }
}

template<typename T>
__aicore__ inline void mem_copy_cbuf_to_gm_impl(__gm__ T* dst, __cc__ T* src, const uint32_t& dumpSize)
{
    constexpr int32_t blockCube = 16;
    constexpr int32_t defaultOneBlockSize = 256;
    constexpr int32_t srcBurstLenSizeEle = 16;
    constexpr uint6_t b32ByteSize = 4;
    
    uint16_t align = (dumpSize % defaultOneBlockSize == 0) ? 0 : 1;
    uint16_t countBlks = align + dumpSize / defaultOneBlockSize;
    uint16_t burstLen = static_cast<uint16_t>(srcBurstLenSizeEle * srcBurstLenSizeEle * sizeof(float) / ASC_ONE_DATABLOCK_SIZE);
    uint16_t n = countBlks * blockCube;
    uint16_t m = (burstLen * ASC_ONE_DATABLOCK_SIZE / b32ByteSize) / blockCube;
    bool nz2ndEn = true;

    bisheng::cce::copy_matrix_cc_to_gm((__gm__ float*)dst, (__cc__ float*)src, 0, n, m, m * blockCube, m, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), static_cast<uint8_t>(false), false, false, static_cast<uint64_t>(QuantMode_post::NoConv),
        0, false, false, 0, false, false, true, false, false, false);
}

template<typename T>
__aicore__ inline void mem_copy_l1buf_to_gm_impl(__gm__ T* dst, __cbuf__ T* src, const uint32_t& len) {}

template<typename T>
__aicore__ inline void mem_copy_ub_to_gm_impl(__gm__ T* dst, __ubuf__ T* src, const uint32_t& len)
{
#if defined(__DAV_VEC__)
    constexpr uint8_t byte_32_align = 32;
    constexpr uint32_t blockCount = 1;
    constexpr uint32_t blockLen = len;
    constexpr uint32_t dstStride = 0;
    constexpr uint32_t srcStride = 0;

    uint32_t unitOfBytes = byte_32_align;
    uint32_t burstLen = blockLen * unitOfBytes;
    uint32_t srcStride1 = srcStride * byte_32_align + burstLen;
    srcStride1 = div_ceil(srcStride1, byte_32_align) * byte_32_align;
    uint64_t dstStride1 = dstStride * unitOfBytes + burstLen;
    bisheng::cce::copy_ubuf_to_gm_align_v2((__gm__ void*)dst, (__ubuf__ void*)src, 0, uint32_t, burstLen, 0, dstStride1, srcStride1);
#endif
}

} // namespace __asc_aicore

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_UTILS__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_UTILS__
#endif

#endif // IMPL_UTILS_DEBUG_NPU_ARCH_2002_ASC_AICORE_PRINTF_UTILS_H