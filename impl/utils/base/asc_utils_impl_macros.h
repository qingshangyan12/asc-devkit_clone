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
 * \file asc_debug_impl_def.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_DEBUG_IMPL_DEF_H
#define IMPL_UTILS_DEBUG_ASC_DEBUG_IMPL_DEF_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_IMPL_DEF__
#warning "asc_debug_impl_def.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future."
#endif

#ifndef __aicore__
#define __aicore__ [aicore]
#endif // __aicore__

#ifndef __host_aicore__
#define __host_aicore__ [host, aicore]
#endif // __host_aicore__

#if (_ASCENDC_HAS_BISHENG_COMPILER)
#define ASCENDC_HOST __host__
#define ASCENDC_AICORE __aicore__
#define ASCENDC_HOST_AICORE __host_aicore__
#else
#define ASCENDC_HOST
#define ASCENDC_AICORE
#define ASCENDC_HOST_AICORE
#endif





#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_IMPL_DEF__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_IMPL_DEF__
#endif

#endif // IMPL_UTILS_DEBUG_ASC_DEBUG_IMPL_DEF_H