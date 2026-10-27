/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_UTILS_DEBUG_ASC_SIMD_GM_TYPES_H
#define IMPL_UTILS_DEBUG_ASC_SIMD_GM_TYPES_H
#include <cstddef>
#include <cstdint>

namespace __asc_simd_vf {
// Internal UB protocol only. Never expose this tag to the runtime FIFO.
constexpr uint32_t GM_PRINT_REFERENCE = 0x80000001U;
// Same layout: fmtByteLen=0 requests scanning; GM_CSTRING arguments have byteLen=0.
// Consumer resolves lengths before converting this private record to GM_PRINT_REFERENCE.
constexpr uint32_t GM_CSTRING_REFERENCE = 0x80000002U;
constexpr uint16_t GM_PRINT_VERSION = 1;
enum class GmPrintArgKind : uint32_t { SCALAR64 = 0, GM_STRING = 1, INLINE_STRING = 2, GM_CSTRING = 3 };
struct GmPrintRecord {
    uint32_t type;
    uint32_t length;
    uint16_t version;
    uint16_t outputType;
    uint32_t blockIdx;
    uint32_t argCount;
    uint32_t inlineBytes;
    uint64_t fmtAddress;
    uint32_t fmtByteLen;
    uint32_t prefixByteLen;
    uint64_t prefixAddress;
};
struct GmPrintArg {
    GmPrintArgKind kind;
    uint32_t byteLen;
    uint64_t value;
};
static_assert(sizeof(GmPrintRecord) == 48 && alignof(GmPrintRecord) == 8, "GM record ABI");
static_assert(offsetof(GmPrintRecord, version) == 8 && offsetof(GmPrintRecord, argCount) == 16 &&
    offsetof(GmPrintRecord, fmtAddress) == 24 && offsetof(GmPrintRecord, fmtByteLen) == 32 &&
    offsetof(GmPrintRecord, prefixAddress) == 40, "GM record offsets");
static_assert(sizeof(GmPrintArg) == 16 && offsetof(GmPrintArg, value) == 8, "GM argument ABI");
} // namespace __asc_simd_vf
#endif
