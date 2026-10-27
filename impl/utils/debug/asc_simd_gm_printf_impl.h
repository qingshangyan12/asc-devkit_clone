/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_UTILS_DEBUG_ASC_SIMD_GM_PRINTF_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_SIMD_GM_PRINTF_IMPL_H
#include "utils/debug/asc_gm_debug_string.h"
#include "impl/utils/debug/asc_simd_gm_types.h"

namespace __asc_simd_vf {
#ifndef ASCENDC_CPU_DEBUG
__simd_callee__ inline bool valid_gm_string_handle(__asc_simd_vf::GmDebugString value)
{
    return (value.addressLow != 0 || value.addressHigh != 0) && value.byteLen != 0 && value.reserved == 0;
}

__simd_callee__ inline void store_gm_string_address(__ubuf__ uint64_t* dst, __asc_simd_vf::GmDebugString value)
{
    auto* words = reinterpret_cast<__ubuf__ uint32_t*>(dst);
    words[0] = value.addressLow;
    words[1] = value.addressHigh;
}

__simd_callee__ inline bool valid_gm_string_handle(__asc_simd_vf::GmCStringRef value)
{
    return value.addressLow != 0 || value.addressHigh != 0;
}
__simd_callee__ inline void store_gm_string_address(__ubuf__ uint64_t* dst, __asc_simd_vf::GmCStringRef value)
{
    auto* words = reinterpret_cast<__ubuf__ uint32_t*>(dst);
    words[0] = value.addressLow;
    words[1] = value.addressHigh;
}
__simd_callee__ inline uint32_t gm_format_length(__asc_simd_vf::GmDebugString value) { return value.byteLen; }
__simd_callee__ inline uint32_t gm_format_length(__asc_simd_vf::GmCStringRef) { return 0; }

// Bounded by available inline space. A bare UB pointer still requires a readable, terminated source.
__simd_callee__ inline bool gm_print_string_size(__ubuf__ const char* str, uint32_t limit, uint32_t& length)
{
    if (str == nullptr) { return false; }
    for (uint32_t i = 0; i < limit; ++i) {
        if (str[i] == '\0') { length = i + 1; return true; }
    }
    return false;
}

__simd_callee__ inline bool append_gm_print_arg(
    __ubuf__ uint8_t* base, uint32_t index, uint32_t& inlineEnd, uint32_t capacity, __asc_simd_vf::GmDebugString value)
{
    if (!valid_gm_string_handle(value)) { return false; }
    auto* arg = reinterpret_cast<__ubuf__ GmPrintArg*>(base + sizeof(GmPrintRecord)) + index;
    arg->kind = GmPrintArgKind::GM_STRING;
    arg->byteLen = value.byteLen;
    store_gm_string_address(&arg->value, value);
    return true;
}

__simd_callee__ inline bool append_gm_print_arg(
    __ubuf__ uint8_t* base, uint32_t index, uint32_t& inlineEnd, uint32_t capacity, __ubuf__ const char* value)
{
    uint32_t length = 0;
    if (!gm_print_string_size(value, capacity - inlineEnd, length)) { return false; }
    auto* arg = reinterpret_cast<__ubuf__ GmPrintArg*>(base + sizeof(GmPrintRecord)) + index;
    arg->kind = GmPrintArgKind::INLINE_STRING;
    arg->byteLen = length;
    arg->value = inlineEnd;
    for (uint32_t i = 0; i < length; ++i) { base[inlineEnd + i] = value[i]; }
    inlineEnd += length;
    return true;
}

__simd_callee__ inline bool append_gm_print_arg(
    __ubuf__ uint8_t* base, uint32_t index, uint32_t& inlineEnd, uint32_t capacity, __ubuf__ char* value)
{
    return append_gm_print_arg(base, index, inlineEnd, capacity, (__ubuf__ const char*)value);
}

__simd_callee__ inline bool append_gm_print_arg(
    __ubuf__ uint8_t* base, uint32_t index, uint32_t& inlineEnd, uint32_t capacity, __asc_simd_vf::GmCStringRef value)
{
    if (!valid_gm_string_handle(value)) { return false; }
    auto* arg = reinterpret_cast<__ubuf__ GmPrintArg*>(base + sizeof(GmPrintRecord)) + index;
    arg->kind = GmPrintArgKind::GM_CSTRING;
    arg->byteLen = 0;
    store_gm_string_address(&arg->value, value);
    return true;
}

template <typename T>
__simd_callee__ inline bool append_gm_print_arg(
    __ubuf__ uint8_t* base, uint32_t index, uint32_t& inlineEnd, uint32_t capacity, T value)
{
    auto* arg = reinterpret_cast<__ubuf__ GmPrintArg*>(base + sizeof(GmPrintRecord)) + index;
    arg->kind = GmPrintArgKind::SCALAR64;
    arg->byteLen = 0;
    set_scalar_param_vf_impl(reinterpret_cast<__ubuf__ uint8_t*>(&arg->value), 0, value);
    return true;
}

// Drain before encoding: dynamic UB strings are snapshotted once, never referenced after publication.
// One record per drain avoids an extra temporary buffer / second scan of mutable UB strings.
template <typename Format, typename... Args>
__simd_callee__ inline void gm_scalar_printf_impl(
    DumpType type, Format fmt, __asc_simd_vf::GmDebugString prefix, Args&&... args)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    auto* info = get_printf_ubuf_addr(0);
    if (info->flag != 0) { return; }
    constexpr uint32_t count = sizeof...(Args);
    constexpr uint64_t fixedBytes = sizeof(GmPrintRecord) + uint64_t(count) * sizeof(GmPrintArg);
    if (!valid_gm_string_handle(fmt) || (prefix.byteLen != 0 && !valid_gm_string_handle(prefix)) ||
        info->length > ASCENDC_SIMD_VF_PRINTF_UBUF_MAX_SIZE || fixedBytes > info->length) {
        info->flag = 1;
        return;
    }
    wait_vf_debug_buffer_drained_and_reset(info);
    if (info->flag != 0) { return; }
    auto* base = info->buffer;
    auto* rec = reinterpret_cast<__ubuf__ GmPrintRecord*>(base);
    uint32_t inlineEnd = static_cast<uint32_t>(fixedBytes);
    uint32_t index = 0;
    // Fold && short-circuits after a malformed/oversized argument.
    const bool valid = (append_gm_print_arg(base, index++, inlineEnd, info->length, args) && ...);
    const uint32_t bytes = (inlineEnd + 7U) & ~7U;
    if (!valid || bytes > info->length) { info->flag = 1; return; }
    for (uint32_t i = inlineEnd; i < bytes; ++i) { base[i] = 0; }
    rec->type = std::is_same<Format, __asc_simd_vf::GmCStringRef>::value ? GM_CSTRING_REFERENCE : GM_PRINT_REFERENCE;
    // Address-only arguments also require the scanning protocol with a sized format.
    if constexpr ((std::is_same<typename std::decay<Args>::type, __asc_simd_vf::GmCStringRef>::value || ...)) {
        rec->type = GM_CSTRING_REFERENCE;
    }
    rec->length = bytes - 8;
    rec->version = GM_PRINT_VERSION;
    rec->outputType = static_cast<uint16_t>(type);
    rec->blockIdx = info->blockIdx;
    rec->argCount = count;
    rec->inlineBytes = inlineEnd - static_cast<uint32_t>(fixedBytes);
    store_gm_string_address(&rec->fmtAddress, fmt);
    rec->fmtByteLen = gm_format_length(fmt);
    rec->prefixByteLen = prefix.byteLen == 0 ? 0 : prefix.byteLen - 1;
    store_gm_string_address(&rec->prefixAddress, prefix);
    info->writeLen = bytes;
#endif
}

template <typename... Args>
__ASC_USE_RESERVED_UBUF__(3510, "printf is forbidden when compile option --cce-disable-asc-reserved-ubuf is enabled")
__simd_callee__ inline void printf(__asc_simd_vf::GmDebugString fmt, Args&&... args)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    enable_asc_diagnostics();
    gm_scalar_printf_impl(DumpType::DUMP_SCALAR, fmt, __asc_simd_vf::GmDebugString{}, args...);
#endif
}
template <typename... Args>
__ASC_USE_RESERVED_UBUF__(3510, "printf_gm requires reserved UB")
__simd_callee__ inline void printf_gm(__asc_simd_vf::GmCStringRef fmt, Args&&... args)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    enable_asc_diagnostics();
    gm_scalar_printf_impl(DumpType::DUMP_SCALAR, fmt, __asc_simd_vf::GmDebugString{}, args...);
#endif
}
#else
__simd_callee__ inline const char* gm_printf_cpu_arg(__asc_simd_vf::GmCStringRef value)
{
    return reinterpret_cast<const char*>(uint64_t(value.addressLow) | (uint64_t(value.addressHigh) << 32));
}
__simd_callee__ inline const char* gm_printf_cpu_arg(__asc_simd_vf::GmDebugString value)
{
    return reinterpret_cast<const char*>(uint64_t(value.addressLow) | (uint64_t(value.addressHigh) << 32));
}
template <typename T>
__simd_callee__ inline T gm_printf_cpu_arg(T value) { return value; }

template <typename... Args>
__simd_callee__ inline void printf(__asc_simd_vf::GmDebugString fmt, Args&&... args)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    std::printf(gm_printf_cpu_arg(fmt), gm_printf_cpu_arg(args)...);
#endif
}
template <typename... Args>
__simd_callee__ inline void printf_gm(__asc_simd_vf::GmCStringRef fmt, Args&&... args)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    std::printf(gm_printf_cpu_arg(fmt), gm_printf_cpu_arg(args)...);
#endif
}
#endif
} // namespace __asc_simd_vf
#endif
