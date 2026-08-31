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
    "impl/c_api/utils/helpers_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/utils/helpers.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_HELPERS_IMPL_H
#endif

#ifndef IMPL_C_API_UTILS_HELPERS_IMPL_H
#define IMPL_C_API_UTILS_HELPERS_IMPL_H

#include "impl/utils/debug/asc_debug_utils.h"

namespace asc {
namespace c_api {
namespace details {

template <auto funcPtr, typename DebugTag = AscendC::Internal::SimdVfDebugTag, typename... Args>
__aicore__ static inline void asc_vf_call_impl(Args&&... args)
{
    if constexpr (AscendC::Internal::SimdVfDebugTraits<DebugTag>::enabled) {
        AscVFDebugInitUb();
    }

    funcPtr(args...);

    if constexpr (AscendC::Internal::SimdVfDebugTraits<DebugTag>::enabled) {
        const bool hasOverflow = AscVFDebugTransferUb();
#ifdef ASCENDC_INTERNAL_SIMD_VF_OVERFLOW_WARNING_ENABLED
        if (hasOverflow) {
            __asc_aicore::printf_impl(
                "[WARNING]: SIMD VF debug buffer overflow (max limit is 2KB), output was truncated.\n");
        }
#else
        (void)hasOverflow;
#endif
    }
}

} // namespace details

template <auto funcPtr, typename... Args>
__aicore__ inline void asc_vf_call(Args&&... args)
{
    if ASCEND_IS_AIV {
        details::asc_vf_call_impl<funcPtr>(args...);
    }
}

} // namespace c_api
} // namespace asc

#endif // IMPL_C_API_UTILS_HELPERS_IMPL_H

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_HELPERS_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_HELPERS_IMPL_H
#endif
