/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_UTILS_DEBUG_NPU_ARCH_3510_ASC_SIMD_GM_PUBLISH_IMPL_H
#define IMPL_UTILS_DEBUG_NPU_ARCH_3510_ASC_SIMD_GM_PUBLISH_IMPL_H
#include "impl/utils/debug/asc_simd_gm_types.h"

namespace __asc_aicore {
__aicore__ inline bool valid_gm_debug_range(uint64_t address, uint32_t bytes)
{
    return address != 0 && bytes != 0 && address <= UINT64_MAX - bytes;
}

// Sources are trusted readable C strings. The FIFO budget bounds scanning, not address validity.
__aicore__ inline bool measure_gm_cstring(uint64_t address, uint32_t budget, uint32_t& bytes)
{
    if (address == 0 || budget == 0) { return false; }
    auto* text = reinterpret_cast<__gm__ const uint8_t*>(address);
    for (uint32_t i = 0; i < budget; ++i) {
        if (address > UINT64_MAX - i) { return false; }
        if (text[i] == 0) { bytes = i + 1; return true; }
    }
    return false;
}

// Validate all descriptors before GM reads. The producer cannot reuse this UB until readLen advances.
// On success cache resolved lengths in the owned record and change its tag LAST. Space retries
// subsequently use the existing sized path without rescanning. Failure stops this lifecycle.
__aicore__ inline bool resolve_gm_cstring_record(__ubuf__ uint8_t* src, uint32_t bytes, uint32_t capacity)
{
    using namespace __asc_simd_vf;
    if (bytes < sizeof(GmPrintRecord) || capacity < 8) { return false; }
    auto* rec = reinterpret_cast<__ubuf__ GmPrintRecord*>(src);
    if (rec->type != GM_CSTRING_REFERENCE || rec->version != GM_PRINT_VERSION ||
        rec->outputType != static_cast<uint16_t>(DumpType::DUMP_SCALAR) ||
        rec->fmtAddress == 0 || rec->prefixByteLen != 0 || rec->prefixAddress != 0 ||
        rec->argCount > (bytes - sizeof(GmPrintRecord)) / sizeof(GmPrintArg)) { return false; }
    const uint32_t inlineStart = sizeof(GmPrintRecord) + rec->argCount * sizeof(GmPrintArg);
    if (rec->inlineBytes > bytes - inlineStart ||
        ((uint64_t(inlineStart) + rec->inlineBytes + 7) & ~uint64_t(7)) != bytes) { return false; }
    const uint32_t inlineEnd = inlineStart + rec->inlineBytes;
    // align_up(raw, 8) + 8 <= capacity, including the legacy eight-byte string gap.
    const uint32_t maxRaw = (capacity & ~7U) - 8;
    uint64_t raw = sizeof(PrintTlv) + uint64_t(rec->argCount) * sizeof(uint64_t) + rec->fmtByteLen;
    if (rec->fmtByteLen != 0 && !valid_gm_debug_range(rec->fmtAddress, rec->fmtByteLen)) { return false; }
    auto* args = reinterpret_cast<__ubuf__ GmPrintArg*>(src + sizeof(GmPrintRecord));
    for (uint32_t i = 0; i < rec->argCount; ++i) {
        const auto kind = args[i].kind;
        const uint32_t len = args[i].byteLen;
        const uint64_t value = args[i].value;
        if (kind == GmPrintArgKind::SCALAR64) {
            if (len != 0) { return false; }
        } else if (kind == GmPrintArgKind::GM_CSTRING) {
            if (len != 0 || value == 0) { return false; }
        } else if (kind == GmPrintArgKind::GM_STRING) {
            if (!valid_gm_debug_range(value, len)) { return false; }
            raw += len;
        } else if (kind == GmPrintArgKind::INLINE_STRING) {
            if (len == 0 || value < inlineStart || value > inlineEnd || len > inlineEnd - value ||
                src[value + len - 1] != 0) { return false; }
            raw += len;
        } else { return false; }
    }
    if (raw > maxRaw) { return false; }
    if (rec->fmtByteLen == 0) {
        uint32_t measured = 0;
        if (!measure_gm_cstring(rec->fmtAddress, maxRaw - static_cast<uint32_t>(raw), measured)) { return false; }
        rec->fmtByteLen = measured;
        raw += measured;
    }
    for (uint32_t i = 0; i < rec->argCount; ++i) {
        if (args[i].kind != GmPrintArgKind::GM_CSTRING) { continue; }
        uint32_t measured = 0;
        if (!measure_gm_cstring(args[i].value, maxRaw - static_cast<uint32_t>(raw), measured)) { return false; }
        args[i].byteLen = measured;
        args[i].kind = GmPrintArgKind::GM_STRING;
        raw += measured;
    }
    rec->type = GM_PRINT_REFERENCE;
    return true;
}

// Validate the entire descriptor without dereferencing GM. Bound the expansion before any GM read.
// GM handles come from the trusted compiler/AICore producer; the consumer uses
// byteLen and does not probe an arbitrary GM address for a terminating NUL.
__aicore__ inline bool size_gm_print_record(
    __ubuf__ const uint8_t* src, uint32_t bytes, uint32_t& outputBytes)
{
    using namespace __asc_simd_vf;
    if (bytes < sizeof(GmPrintRecord)) {
        return false;
    }
    auto* rec = reinterpret_cast<__ubuf__ const GmPrintRecord*>(src);
    if (rec->version != GM_PRINT_VERSION ||
        (rec->outputType != static_cast<uint16_t>(DumpType::DUMP_SCALAR) &&
         rec->outputType != static_cast<uint16_t>(DumpType::DUMP_ASSERT)) ||
        !valid_gm_debug_range(rec->fmtAddress, rec->fmtByteLen) ||
        (rec->prefixByteLen != 0 && !valid_gm_debug_range(rec->prefixAddress, rec->prefixByteLen))) {
        return false;
    }
    if (rec->argCount > (bytes - sizeof(GmPrintRecord)) / sizeof(GmPrintArg)) {
        return false;
    }
    const uint32_t inlineStart = sizeof(GmPrintRecord) + rec->argCount * sizeof(GmPrintArg);
    if (rec->inlineBytes > bytes - inlineStart || ((inlineStart + rec->inlineBytes + 7U) & ~7U) != bytes) {
        return false;
    }
    const uint32_t inlineEnd = inlineStart + rec->inlineBytes;
    uint64_t size = sizeof(PrintTlv) + uint64_t(rec->argCount) * sizeof(uint64_t) +
        rec->fmtByteLen + uint64_t(rec->prefixByteLen);
    auto* args = reinterpret_cast<__ubuf__ const GmPrintArg*>(src + sizeof(GmPrintRecord));
    for (uint32_t i = 0; i < rec->argCount; ++i) {
        const auto kind = args[i].kind;
        const uint32_t len = args[i].byteLen;
        const uint64_t value = args[i].value;
        if (kind == GmPrintArgKind::SCALAR64) {
            if (len != 0) { return false; }
        } else if (kind == GmPrintArgKind::GM_STRING) {
            if (!valid_gm_debug_range(value, len)) { return false; }
            size += len;
        } else if (kind == GmPrintArgKind::INLINE_STRING) {
            if (len == 0 || value < inlineStart || value > inlineEnd || len > inlineEnd - value ||
                src[value + len - 1] != 0) { return false; }
            size += len;
        } else {
            return false;
        }
    }
    size = ((size + 7) & ~uint64_t(7)) + 8;
    if (size > UINT32_MAX) { return false; }
    outputBytes = static_cast<uint32_t>(size);
    return true;
}

// Caller owns FIFO space and commits/flushed writeInfo only after all bytes have been written.
__aicore__ inline void write_gm_print_record(
    __gm__ uint8_t* dst, __ubuf__ const uint8_t* src, uint32_t outputBytes)
{
    using namespace __asc_simd_vf;
    auto* rec = reinterpret_cast<__ubuf__ const GmPrintRecord*>(src);
    auto* args = reinterpret_cast<__ubuf__ const GmPrintArg*>(src + sizeof(GmPrintRecord));
    for (uint32_t i = 0; i < outputBytes; ++i) { dst[i] = 0; }
    auto* output = reinterpret_cast<__gm__ PrintTlv*>(dst);
    output->type = rec->outputType;
    output->length = outputBytes - 8;
    output->blockIdx = rec->blockIdx;
    output->resv = 0;
    output->fmtOffset = (uint64_t(rec->argCount) + 1) * sizeof(uint64_t);
    __gm__ uint8_t* params = dst + sizeof(PrintTlv);
    __gm__ uint8_t* fmt = params + rec->argCount * sizeof(uint64_t);
    for (uint32_t i = 0; i < rec->prefixByteLen; ++i) {
        fmt[i] = reinterpret_cast<__gm__ const uint8_t*>(rec->prefixAddress)[i];
    }
    for (uint32_t i = 0; i < rec->fmtByteLen; ++i) {
        fmt[rec->prefixByteLen + i] = reinterpret_cast<__gm__ const uint8_t*>(rec->fmtAddress)[i];
    }
    // Keep the existing eight-byte gap before string arguments (relative to paramAddr).
    uint32_t stringOffset = static_cast<uint32_t>(output->fmtOffset) + rec->prefixByteLen + rec->fmtByteLen;
    for (uint32_t i = 0; i < rec->argCount; ++i) {
        auto* slot = reinterpret_cast<__gm__ uint64_t*>(params) + i;
        if (args[i].kind == GmPrintArgKind::SCALAR64) {
            *slot = args[i].value;
            continue;
        }
        *slot = stringOffset - i * sizeof(uint64_t);
        for (uint32_t j = 0; j < args[i].byteLen; ++j) {
            params[stringOffset + j] = args[i].kind == GmPrintArgKind::GM_STRING ?
                reinterpret_cast<__gm__ const uint8_t*>(args[i].value)[j] : src[args[i].value + j];
        }
        stringOffset += args[i].byteLen;
    }
}
} // namespace __asc_aicore
#endif
