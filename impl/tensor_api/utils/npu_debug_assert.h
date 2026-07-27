/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/utils/npu_debug_assert.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file npu_debug_assert.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_UTILS_NPU_DEBUG_ASSERT_H
#define IMPL_TENSOR_API_UTILS_NPU_DEBUG_ASSERT_H

#ifdef ASCENDC_DEBUG
#include "utils/debug/asc_printf.h"

namespace AscendC {
namespace Te {

struct tensor_api_assert_context {
    __gm__ const char* file;
    uint32_t line;
    __gm__ const char* function;
};

// Keep the trap intrinsic outside constexpr Tensor API functions while preserving one assert record.
template <typename... arg_types>
__aicore__ inline void tensor_api_debug_assert_fail(__gm__ const char* full_format, arg_types&&... args)
{
    __asc_aicore::printf_impl_assert(full_format, args...);
    trap();
}

} // namespace Te
} // namespace AscendC

#define TENSOR_API_DETAIL_VA_ARGS_IS_EMPTY(...) (sizeof(#__VA_ARGS__) == 1)

#define TENSOR_API_DETAIL_ASSERT_FAIL(context, format, ...)                                                         \
    do {                                                                                                            \
        ::AscendC::Te::tensor_api_debug_assert_fail(                                                                \
            "[ASSERT] %s:%u: %s: " format "\n", (context).file, (context).line, (context).function, ##__VA_ARGS__); \
    } while (0)

#define TENSOR_API_DETAIL_DEBUG_ASSERT(expr, ...)                                                 \
    do {                                                                                          \
        if (!(expr)) {                                                                            \
            const ::AscendC::Te::tensor_api_assert_context tensor_api_assert_context_instance = { \
                __FILE__, static_cast<uint32_t>(__LINE__), __FUNCTION__};                         \
            if (TENSOR_API_DETAIL_VA_ARGS_IS_EMPTY(__VA_ARGS__)) {                                \
                TENSOR_API_DETAIL_ASSERT_FAIL(tensor_api_assert_context_instance, "");            \
            } else {                                                                              \
                __VA_ARGS__;                                                                      \
            }                                                                                     \
        }                                                                                         \
    } while (0)

#define TENSOR_API_LOG_INTERNAL(format, ...) \
    TENSOR_API_DETAIL_ASSERT_FAIL(tensor_api_assert_context_instance, format, ##__VA_ARGS__)
#define TENSOR_API_REPORT_INTERNAL(reporter, ...) reporter(tensor_api_assert_context_instance, ##__VA_ARGS__)
#define TENSOR_API_DEBUG_ASSERT(...) TENSOR_API_DETAIL_DEBUG_ASSERT(__VA_ARGS__)
#else
#define TENSOR_API_LOG_INTERNAL(format, ...)
#define TENSOR_API_REPORT_INTERNAL(reporter, ...)
#define TENSOR_API_DEBUG_ASSERT(...)
#endif

#endif // IMPL_TENSOR_API_UTILS_NPU_DEBUG_ASSERT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
