/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file asc_clock.h
 * \brief
 */
#ifndef INCLUDE_UTILS_DEBUG_ASC_CLOCK_H
#define INCLUDE_UTILS_DEBUG_ASC_CLOCK_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_COMPILER_INTERNAL_HEADERS_ASC_CLOCK_H__
#endif

#ifndef __SIMT_DEVICE_FUNCTIONS_DECL__
#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#define __SIMT_DEVICE_FUNCTIONS_DECL__ __aicore__
#else
#define __SIMT_DEVICE_FUNCTIONS_DECL__ __simt_callee__
#endif
#endif

namespace __asc_simt_vf {
__SIMT_DEVICE_FUNCTIONS_DECL__ inline uint64_t clock(void) noexcept;
} // namespace __asc_simt_vf

#include "impl/utils/debug/asc_clock_simt_impl.h"

#if defined(__UNDEF_ASCENDC_INCLUDE_COMPILER_INTERNAL_HEADERS_ASC_CLOCK_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_COMPILER_INTERNAL_HEADERS_ASC_CLOCK_H__
#endif

#endif
