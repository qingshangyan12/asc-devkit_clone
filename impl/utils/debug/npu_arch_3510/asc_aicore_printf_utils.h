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
 * \file asc_aicore_printf_utils.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_NPU_ARCH_3510_ASC_AICORE_PRINTF_UTILS_H
#define IMPL_UTILS_DEBUG_NPU_ARCH_3510_ASC_AICORE_PRINTF_UTILS_H

#include "impl/utils/debug/asc_debug_utils.h"
#include "impl/utils/debug/npu_arch_3510/asc_type_conversion_utils.h"
namespace __asc_aicore {

template <typename T>
__aicore__ inline void set_scalar_param_impl(__gm__ uint8_t* paramAddr, uint32_t paramIdx, T scalar)
{
    __gm__ uint64_t *scalarAddr = (__gm__ uint64_t *)paramAddr + paramIdx;
    *scalarAddr = 0;

    if constexpr (is_same_in_list<T, half, float>()) {
        *((__gm__ float *)scalarAddr) = static_cast<float>(scalar);
    } else if constexpr (is_same_in_list<T, double>()) {
        *((__gm__ double *)scalarAddr) = static_cast<double>(scalar);
    } else if constexpr (std::is_signed<T>::value) {
        *((__gm__ int64_t *)scalarAddr) = static_cast<int64_t>(scalar);
    } else if constexpr(std::is_unsigned<T>::value) {
        *((__gm__ uint64_t *)scalarAddr) = static_cast<uint64_t>(scalar);
    } else if constexpr(is_same_in_list<T, bfloat16_t, float8_e5m2_t, float8_e8m0_t, float8_e4m3_t, hifloat8_t>()) {
        *((__gm__ float *)scalarAddr) = to_float(scalar);
    } else if constexpr(std::is_pointer<T>::value) {
        *((__gm__ uint64_t *)scalarAddr) = (uintptr_t)scalar;
    } else if constexpr(std::is_enum<T>::value) {
        *((__gm__ uint64_t *)scalarAddr) = static_cast<uint64_t>(scalar);
    }
    asc_entire_dcci((__gm__ uint64_t*)scalarAddr);
}

} // namespace __asc_aicore

namespace __asc_simd_vf {
__simd_callee__ constexpr uint32_t align_print_tlv_len(const uint32_t dataLen)
{
    constexpr uint32_t alignBytes = 8;
    return ((dataLen + (alignBytes - 1)) & ~(alignBytes - 1)) + alignBytes;
}

template <typename T>
__simd_callee__ inline void set_scalar_param_vf(__ubuf__ uint8_t* paramAddr, uint32_t paramIdx, T scalar)
{
    set_scalar_param_vf_impl(paramAddr, paramIdx, scalar);
}

__simd_callee__ inline void set_string_param_vf(
    __ubuf__ uint8_t* paramAddr, uint32_t paramIdx, __ubuf__ const char* s, uint32_t& offset)
{
    __ubuf__ uint64_t* stringAddr = reinterpret_cast<__ubuf__ uint64_t*>(paramAddr) + paramIdx;
    __ubuf__ uint8_t* dstStrAddr = paramAddr + offset;

    // write string value offset
    *stringAddr = static_cast<uint64_t>(offset - sizeof(uint64_t) * paramIdx);

    // write string content: GM -> UBuf
    uint32_t strLen = get_cstring_len_vf(s);
    for (uint32_t i = 0; i < strLen; i++) {
        *(dstStrAddr + i) = *(s + i);
    }
    offset += strLen;
}

__simd_callee__ inline void set_param_vf(__ubuf__ uint8_t* paramAddr, uint32_t paramIdx, uint32_t& offset)
{
    (void)paramAddr;
    (void)paramIdx;
    (void)offset;
    return;
}

template <typename... Args>
__simd_callee__ inline void set_param_vf(
    __ubuf__ uint8_t* paramAddr, uint32_t paramIdx, uint32_t& offset, Args&&... args);

template <typename... Args>
__simd_callee__ inline void set_param_vf_impl(
    __ubuf__ uint8_t* paramAddr, uint32_t paramIdx, uint32_t& offset, __ubuf__ const char* s, Args&&... args)
{
    set_string_param_vf(paramAddr, paramIdx, s, offset);
    set_param_vf(paramAddr, paramIdx + 1, offset, args...);
}

template <typename T, typename... Args>
__simd_callee__ inline void set_param_vf_impl(
    __ubuf__ uint8_t* paramAddr, uint32_t paramIdx, uint32_t& offset, T scalar, Args&&... args)
{
    set_scalar_param_vf(paramAddr, paramIdx, scalar);
    set_param_vf(paramAddr, paramIdx + 1, offset, args...);
}

template <typename... Args>
__simd_callee__ inline void set_param_vf(
    __ubuf__ uint8_t* paramAddr, uint32_t paramIdx, uint32_t& offset, Args&&... args)
{
    set_param_vf_impl(paramAddr, paramIdx, offset, args...);
}

__simd_callee__ inline uint32_t get_args_len_vf(uint32_t& argsNum)
{
    (void)argsNum;
    return 0;
}

template <typename... Args>
__simd_callee__ inline uint32_t get_args_len_vf(uint32_t& argsNum, Args&&... args);

template <typename... Args>
__simd_callee__ inline uint32_t get_args_len_vf_impl(uint32_t& argsNum, __ubuf__ const char* s, Args&&... args)
{
    constexpr uint32_t paramSize = sizeof(uint64_t);
    const uint32_t strLen = get_cstring_len_vf(s);
    argsNum += 1;
    return paramSize + strLen + get_args_len_vf(argsNum, args...);
}

template <typename T, typename... Args>
__simd_callee__ inline uint32_t get_args_len_vf_impl(uint32_t& argsNum, T scalar, Args&&... args)
{
    constexpr uint32_t paramSize = sizeof(uint64_t);
    argsNum += 1;
    return paramSize + get_args_len_vf(argsNum, args...);
}

template <typename... Args>
__simd_callee__ inline uint32_t get_args_len_vf(uint32_t& argsNum, Args&&... args)
{
    return get_args_len_vf_impl(argsNum, args...);
}

template <typename... Args>
__simd_callee__ inline uint32_t get_print_tlv_len_simd(uint32_t& argsNum, __ubuf__ const char* fmt, Args&&... args)
{
    constexpr uint32_t printInfoLen = sizeof(PrintTlv);
    const uint32_t argsLen = get_args_len_vf(argsNum, args...);
    const uint32_t fmtLen = get_cstring_len_vf(fmt);
    return align_print_tlv_len(printInfoLen + argsLen + fmtLen);
}

__simd_callee__ inline void set_print_tlv_info_vf(
    DumpType debugType, __ubuf__ PrintTlv* printTlv, const uint32_t& tlvLen, const uint32_t& argsNum, uint16_t blockIdx)
{
    printTlv->type = static_cast<uint32_t>(debugType);
    printTlv->length = tlvLen - sizeof(uint32_t[2]); // exclude type and length
    printTlv->blockIdx = blockIdx;  // set in aicore
    printTlv->resv = static_cast<uint32_t>(0U);
    printTlv->fmtOffset = (argsNum + 1) * sizeof(uint64_t); // include fmt offset
}

__simd_callee__ inline void copy_fmt_to_ubuf(__ubuf__ uint8_t* dst, __ubuf__ const char* src, uint32_t len)
{
    // Workaround for -O2 optimization issue: copy byte-by-byte to avoid miscompilation
    for (uint32_t i = 0; i < len; i += 2) {
        dst[i] = src[i];
        dst[i + 1] = src[i + 1];
    }
}

template <typename... Args>
__simd_callee__ inline void set_print_tlv_data_vf(
    __ubuf__ PrintTlv* printTlv, __ubuf__ const char* fmt, Args&&... args)
{
    const uint32_t strLen = get_cstring_len_vf(fmt);
    __ubuf__ uint8_t* paramAddr = reinterpret_cast<__ubuf__ uint8_t*>(printTlv + 1);
    __ubuf__ uint8_t* fmtAddr = paramAddr + printTlv->fmtOffset - sizeof(uint64_t);

    copy_fmt_to_ubuf(fmtAddr, fmt, strLen);

    uint32_t strParamOffset = printTlv->fmtOffset + strLen;
    set_param_vf(paramAddr, 0, strParamOffset, args...);
}

template <class... Args>
__simd_callee__ inline void scalar_printf_impl(DumpType debugType, __ubuf__ const char* fmt, Args&&... args)
{
    __ubuf__ BlockVFBufInfo* blockInfo = get_printf_ubuf_addr(0);

    uint32_t argsNum = 0;
    const uint32_t tlvLen = get_print_tlv_len_simd(argsNum, fmt, args...);

    // construct PrintTlv TLV in BlockVFBufInfo.buffer (UBuf)
    __ubuf__ PrintTlv* printTlv =
        reinterpret_cast<__ubuf__ PrintTlv*>((__ubuf__ uint8_t*)(blockInfo->buffer) + blockInfo->writeLen);
    set_print_tlv_info_vf(debugType, printTlv, tlvLen, argsNum, blockInfo->blockIdx);
    set_print_tlv_data_vf(printTlv, fmt, args...);

    blockInfo->magic = SIMD_VF_MAGIC_NUMBER;
    blockInfo->writeLen += tlvLen;
    blockInfo->pidx += 1;
}

template <class... Args>
__simd_callee__ inline void printf_impl(__ubuf__ const char* fmt, Args&&... args)
{
    enable_asc_diagnostics();
    scalar_printf_impl(DumpType::DUMP_SCALAR, fmt, args...);
}
} // namespace __asc_simd_vf


#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_PRINTF_UTILS__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_PRINTF_UTILS__
#endif

#endif // IMPL_UTILS_DEBUG_NPU_ARCH_3510_ASC_AICORE_PRINTF_UTILS_H