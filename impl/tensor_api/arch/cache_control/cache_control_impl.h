/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License"). You may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/arch/cache_control/cache_control_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"#include \"tensor_api/arch/cache_control/cache_control.h\" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file cache_control_impl.h
 * \brief Data cache control implementation.
 */
#ifndef IMPL_TENSOR_API_ARCH_CACHE_CONTROL_CACHE_CONTROL_IMPL_H
#define IMPL_TENSOR_API_ARCH_CACHE_CONTROL_CACHE_CONTROL_IMPL_H

#include "c_api/asc_simd.h"

namespace asc {
namespace te {

__aicore__ inline void datacache_clean_and_invalid(clean_target target)
{
    switch (target) {
        case clean_target::all:
            asc_dcci_entire_all();
            break;
        case clean_target::ub:
#if (ASC_DEVKIT_VERSION_NUM >= 90200000)
            asc_dcci_entire_ub();
#endif
            break;
        case clean_target::gm:
            asc_dcci_entire_out();
            break;
        case clean_target::atomic:
            asc_dcci_entire_atomic();
            break;
        default:
            break;
    }
}

__aicore__ inline void datacache_clean() { asc_dci(); }

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CACHE_CONTROL_CACHE_CONTROL_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
