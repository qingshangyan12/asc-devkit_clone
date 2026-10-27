/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef INCLUDE_UTILS_DEBUG_ASC_GM_DEBUG_STRING_H
#define INCLUDE_UTILS_DEBUG_ASC_GM_DEBUG_STRING_H
#include <cstdint>
#include "impl/utils/sys_macros_impl.h"

namespace __asc_simd_vf {
// Address-only reference to a valid, immutable, NUL-terminated GM string.
// Compiler lowering may construct these words directly; VF never dereferences them.
struct GmCStringRef {
    uint32_t addressLow;
    uint32_t addressHigh;
};
static_assert(sizeof(GmCStringRef) == 8, "GM C string reference ABI");

__aicore__ inline GmCStringRef make_gm_cstring_ref(__gm__ const char* data)
{
    const uint64_t address = reinterpret_cast<uint64_t>(data);
    return {static_cast<uint32_t>(address), static_cast<uint32_t>(address >> 32)};
}

// A non-owning reference to immutable GM bytes, including the terminating NUL.
// Split address words allow VF to transport it without GM access or 64-bit arithmetic.
struct GmDebugString {
    uint32_t addressLow;
    uint32_t addressHigh;
    uint32_t byteLen;
    uint32_t reserved;
};
static_assert(sizeof(GmDebugString) == 16, "GM debug string handle ABI");

// Call on AICore (or CPU debug), never cast a UB literal to GM.
// The source must remain valid and immutable until the VF call has drained.
__aicore__ inline GmDebugString make_gm_debug_string(__gm__ const char* data, uint32_t byteLen)
{
    const uint64_t address = reinterpret_cast<uint64_t>(data);
    return {static_cast<uint32_t>(address), static_cast<uint32_t>(address >> 32), byteLen, 0};
}

#if (__NPU_ARCH__ == 3510) || defined(ASCENDC_CPU_DEBUG)
// The AICore caller may stage a small table of handles in UB; string bytes stay in GM.
__simd_callee__ inline GmDebugString load_gm_debug_string(__ubuf__ const GmDebugString* ref)
{
    return {ref->addressLow, ref->addressHigh, ref->byteLen, ref->reserved};
}
#endif
#if (__NPU_ARCH__ == 3510) || defined(ASCENDC_CPU_DEBUG)
__simd_callee__ inline GmCStringRef load_gm_cstring_ref(__ubuf__ const GmCStringRef* ref)
{
    return {ref->addressLow, ref->addressHigh};
}
#endif
} // namespace __asc_simd_vf
#endif
