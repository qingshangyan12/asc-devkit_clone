/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
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

#include "impl/utils/debug/asc_debug_utils.h"
#include "impl/utils/sys_macros.h"

namespace __asc_aicore {

template<typename T>
__aicore__ inline void mem_copy_cbuf_to_gm_impl(__gm__ T* dst, __cc__ T* src, const uint32_t& dumpSize)
{
    if ASCEND_IS_NOT_AIC {
        return;
    }
    constexpr int32_t blockCube = 16;
    constexpr int32_t defaultOneBlockSize = 256;
    constexpr int32_t srcBurstLenSizeEle = 16;
    constexpr uint16_t b32ByteSize = 4;
    
    uint16_t align = (dumpSize % defaultOneBlockSize == 0) ? 0 : 1;
    uint16_t countBlks = align + dumpSize / defaultOneBlockSize;
    uint16_t burstLen = static_cast<uint16_t>(srcBurstLenSizeEle * srcBurstLenSizeEle * sizeof(float) / ASC_ONE_DATABLOCK_SIZE);
    uint16_t n = countBlks * blockCube;
    uint16_t m = (burstLen * ASC_ONE_DATABLOCK_SIZE / b32ByteSize) / blockCube;
    bool nz2ndEn = true;

    copy_matrix_cc_to_gm((__gm__ float*)dst, (__cc__ float*)src, 0, n, m, m * blockCube, m, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), static_cast<uint8_t>(false), false, false, static_cast<uint64_t>(QuantMode_post::NoConv),
        0, false, false, 0, false, false, true, false, false, false);
}

// no such path
template<typename T>
__aicore__ inline void mem_copy_l1buf_to_gm_impl(__gm__ T* dst, __cbuf__ T* src, const uint32_t& len) {}

template<typename T>
__aicore__ inline void mem_copy_ub_to_gm_impl(__gm__ T* dst, __ubuf__ T* src, const uint32_t& len)
{
#if defined(__DAV_VEC__)
    constexpr uint8_t byte_32_align = 32;
    constexpr uint32_t blockCount = 1;
    uint32_t blockLen = len;
    constexpr uint32_t dstStride = 0;
    constexpr uint32_t srcStride = 0;

    uint32_t unitOfBytes = byte_32_align;
    uint32_t burstLen = blockLen * unitOfBytes;
    uint32_t srcStride1 = srcStride * byte_32_align + burstLen;
    srcStride1 = div_ceil(srcStride1, byte_32_align) * byte_32_align;
    uint64_t dstStride1 = dstStride * unitOfBytes + burstLen;
    copy_ubuf_to_gm_align_v2((__gm__ void*)dst, (__ubuf__ void*)src, 0, blockCount, burstLen, 0, dstStride1, srcStride1);
#endif
}

} // namespace __asc_aicore

namespace __asc_simd_vf {
template <AscendC::Hardware hardware, typename T, typename U>
__simd_callee__ inline void set_dump_tlv_info_vf(U& src, __ubuf__ DumpTensorTlv* dumpTlv,
    uint32_t alignDumpLen, uint32_t desc, uint32_t dumpSize, uint16_t blockIdx)
{
    dumpTlv->type = static_cast<uint32_t>(DumpType::DUMP_TENSOR);
    dumpTlv->length = sizeof(DumpTensorTlv) - sizeof(uint32_t[2]) + alignDumpLen;
    dumpTlv->tensorAddr = 0U; // set in aicore
    dumpTlv->dataType = static_cast<uint32_t>(get_dump_datatype<T>());
    dumpTlv->desc = desc;
    dumpTlv->blockIdx = blockIdx;  // set in aicore
    dumpTlv->bufferId = static_cast<uint32_t>(0U);
    dumpTlv->position = static_cast<uint16_t>(hardware);
    dumpTlv->dim = static_cast<uint32_t>(0U);
    for (uint32_t i = 0; i < 8; ++i) {
        dumpTlv->shape[i] = static_cast<uint32_t>(0U);
    }
    dumpTlv->resv1 = static_cast<uint32_t>(0U);
    dumpTlv->dumpSize = dumpSize * sizeof(T);
}

template <AscendC::Hardware hardware, typename T, typename U>
__simd_callee__ inline void set_dump_tlv_data_vf(U& src, __ubuf__ DumpTensorTlv* dumpTlv,
    uint32_t alignDumpLen, uint32_t dumpSize)
{
    __ubuf__ T* dumpDstAddr = reinterpret_cast<__ubuf__ T*>(dumpTlv + 1);

    // Scalar copy; performance optimization deferred
    for (uint32_t i = 0; i < dumpSize; i++) {
        dumpDstAddr[i] = src[i];
    }
}

template <AscendC::Hardware hardware, typename T, typename U>
__simd_callee__ inline void set_dump_tlv_data_reg(U& src, __ubuf__ DumpTensorTlv* dumpTlv,
    uint32_t alignDumpLen, uint32_t dumpSize)
{
    __ubuf__ T* dumpDstAddr = reinterpret_cast<__ubuf__ T*>(dumpTlv + 1);

    // Store interface simulation, supports unaligned transfer. dumpDstAddr is likely unaligned.
    uint32_t count = dumpTlv->dumpSize / sizeof(T);
    vector_align ureg;
    constexpr auto postValue = 
        std::integral_constant<::Post, static_cast<::Post>(AscendC::Reg::PostLiteral::POST_MODE_UPDATE)>();
    vstus(ureg, count * 2, (vector_u32&)src, (__ubuf__ uint32_t*&)dumpDstAddr, postValue);
    vstas(ureg, (__ubuf__ uint32_t*&)dumpDstAddr, count * 2, POST_UPDATE);
}

template <AscendC::Hardware hardware, typename T, typename U>
__simd_callee__ inline void asc_dump_impl_reg(U& src, uint32_t desc, uint32_t dumpSize)
{
    __ubuf__ BlockVFBufInfo *blockInfo = get_printf_ubuf_addr(0);
    
    constexpr uint16_t dataBlockSize = 32;
    uint32_t alignDumpLen = align_up(dumpSize * sizeof(T), dataBlockSize);
    uint32_t tlvLen = sizeof(DumpTensorTlv) + alignDumpLen;

    __ubuf__ DumpTensorTlv* dumpTlv =
        (__ubuf__ DumpTensorTlv*)((__ubuf__ uint8_t*)(blockInfo->buffer) + blockInfo->writeLen);
    set_dump_tlv_info_vf<hardware, T>(src, dumpTlv, alignDumpLen, desc, dumpSize, blockInfo->blockIdx);
    set_dump_tlv_data_reg<hardware, T>(src, dumpTlv, alignDumpLen, dumpSize);

    blockInfo->magic = 0xF0A00B0F;
    blockInfo->writeLen += tlvLen;
    blockInfo->pidx += 1;
}

template <AscendC::Hardware hardware, typename T, typename U>
__simd_callee__ inline void asc_dump_impl(U& src, uint32_t desc, uint32_t dumpSize)
{
    __ubuf__ BlockVFBufInfo *blockInfo = get_printf_ubuf_addr(0);
    
    constexpr uint16_t dataBlockSize = 32;
    uint32_t alignDumpLen = align_up(dumpSize * sizeof(T), dataBlockSize);
    uint32_t tlvLen = sizeof(DumpTensorTlv) + alignDumpLen;

    __ubuf__ DumpTensorTlv* dumpTlv =
        (__ubuf__ DumpTensorTlv*)((__ubuf__ uint8_t*)(blockInfo->buffer) + blockInfo->writeLen);
    set_dump_tlv_info_vf<hardware, T>(src, dumpTlv, alignDumpLen, desc, dumpSize, blockInfo->blockIdx);
    set_dump_tlv_data_vf<hardware, T>(src, dumpTlv, alignDumpLen, dumpSize);

    blockInfo->magic = 0xF0A00B0F;
    blockInfo->writeLen += tlvLen;
    blockInfo->pidx += 1;
}

template <typename T, typename U>
__simd_callee__ inline void asc_dump_reg(U& input, uint32_t desc, uint32_t dumpSize)
{
    enable_asc_diagnostics();
    asc_dump_impl_reg<AscendC::Hardware::UB, T>(input, desc, dumpSize);
}

template <typename T>
__simd_callee__ inline void asc_dump_ubuf(__ubuf__ T* input, uint32_t desc, uint32_t dumpSize) {
    enable_asc_diagnostics();
    asc_dump_impl<AscendC::Hardware::UB, T>(input, desc, dumpSize);
}

template <typename T, typename U>
__simd_callee__ inline void asc_dump(U& input, uint32_t desc, uint32_t dumpSize)
{
    enable_asc_diagnostics();
    asc_dump_impl_reg<AscendC::Hardware::UB, T>(input, desc, dumpSize);
}

template <typename T>
__simd_callee__ inline void asc_dump(__ubuf__ T* input, uint32_t desc, uint32_t dumpSize)
{
    enable_asc_diagnostics();
    asc_dump_impl<AscendC::Hardware::UB, T>(input, desc, dumpSize);
}
} // namespace __asc_simd_vf

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_UTILS__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_UTILS__
#endif

#endif // IMPL_UTILS_DEBUG_NPU_ARCH_2002_ASC_AICORE_PRINTF_UTILS_H