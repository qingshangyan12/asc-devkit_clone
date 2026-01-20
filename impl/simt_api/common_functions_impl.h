/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/


/* !
 * \file common_functions_impl.h
 * \brief
 */
#ifndef IMPL_SIMT_API_COMMON_FUNCTIONS_IMPL_H
#define IMPL_SIMT_API_COMMON_FUNCTIONS_IMPL_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_COMMON_FUNCTIONS_IMPL__
#warning "common_functions_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "common_functions.h" and use public functions or variables defined in interface header files."
#endif

__SIMT_DEVICE_FUNCTIONS_DECL__ inline int32_t asc_get_block_idx()
{
    return bisheng::cce::simt::get_block_idx();
}

__SIMT_DEVICE_FUNCTIONS_DECL__ inline int32_t asc_get_block_num()
{
    return bisheng::cce::simt::get_block_num();
}

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_COMMON_FUNCTIONS_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_COMMON_FUNCTIONS_IMPL__
#endif

#endif  // IMPL_SIMT_API_COMMON_FUNCTIONS_IMPL_H