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
 * \file asc_clock_simt_impl.h
 * \brief
 */

#ifndef IMPL_UTILS_DEBUG_ASC_CLOCK_SIMT_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_CLOCK_SIMT_IMPL_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_CLOCK_SIMT_IMPL__
#warning "asc_clock_simt_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "asc_clock.h" and use public functions or variables defined in interface header files."
#endif

namespace __asc_simt_vf {

__SIMT_DEVICE_FUNCTIONS_DECL__ inline uint64_t clock (void) noexcept
{
    return __cce_simt_get_CLOCK64();
}

} // namespace __asc_simt_vf

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_CLOCK_SIMT_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_CLOCK_SIMT_IMPL__
#endif

#endif