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
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PHYSICAL_INT64_H
#endif

/*!
 * \file physical_int64.h
 * \brief Physical B64 element tag for single hardware vector_int64 register path.
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_PHYSICAL_INT64_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_PHYSICAL_INT64_H

namespace asc {
namespace te {
namespace experimental {

/**
 * @brief Physical B64 element tag for a single hardware @c vector_int64 register path.
 *
 * Do NOT typedef this to @c int64_t. Logical @c int64_t selects the soft {lo,hi}
 * @c reg_tensor specialization; @c physical_int64_t selects the single-reg path.
 * Mixing the two representations is unsupported.
 */
struct physical_int64_t {};

} // namespace experimental
} // namespace te
} // namespace asc

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_PHYSICAL_INT64_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PHYSICAL_INT64_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PHYSICAL_INT64_H
#endif
