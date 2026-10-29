/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)

#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file sys_var_impl.h
 * \brief Device state and system variable implementation.
 */
#ifndef IMPL_TENSOR_API_UTILS_SYS_VAR_IMPL_H
#define IMPL_TENSOR_API_UTILS_SYS_VAR_IMPL_H

#include "c_api/asc_simd.h"

namespace asc {
namespace te {

__aicore__ inline void init_soc_state() { asc_init(); }

__aicore__ inline int64_t get_sub_block_id() { return asc_get_sub_block_id(); }

__aicore__ inline int64_t get_sub_block_num() { return asc_get_sub_block_num(); }

__aicore__ inline constexpr int64_t get_vf_len() { return asc_get_vf_len(); }

__aicore__ inline uint64_t get_phy_buf_addr(uint64_t offset) { return asc_get_phy_buf_addr(offset); }

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_UTILS_SYS_VAR_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
