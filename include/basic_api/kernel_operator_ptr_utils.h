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
 * \file kernel_operator_ptr_utils.h
 * \brief Extract hardware pointers from Tensor or pass through raw pointers.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_ptr_utils.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PTR_UTILS_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_PTR_UTILS_H
#define ASCENDC_MODULE_OPERATOR_PTR_UTILS_H

#include "../../impl/basic_api/kernel_macros.h"
#include "../utils/std/type_traits.h"

namespace AscendC {
/*
 * GetUnderlyingPtr
 * Pointer arguments are returned as-is. Tensor arguments use GetPhyAddr().
 * Shared by DMA / VECTOR pointer-extension tasks.
 */
template <typename U>
__aicore__ inline auto GetUnderlyingPtr(const U& val)
{
    if constexpr (Std::is_pointer_v<U>) {
        return val;
    } else {
        return val.GetPhyAddr();
    }
}

template <typename T>
__aicore__ inline T* GetUnderlyingPtr(T* val)
{
    return val;
}

#if !defined(ASCENDC_CPU_DEBUG) || ASCENDC_CPU_DEBUG != 1
// __gm__ 重载仅在编译器将 __gm__（cce_global）视为独立地址空间类型时启用。
// 现有 ccec（clang 15.0.5，CANN 9.1.0 / 9.2.0-beta.2）在重载决议中将其塌缩为 T*，
// 与上方 T* 重载构成重复定义；其透传语义已由 T* 重载覆盖，行为等价。
// 待编译器支持地址空间指针类型后，由构建系统定义 ASCENDC_ADDRESS_SPACE_TYPES=1 启用。
#if defined(ASCENDC_ADDRESS_SPACE_TYPES)
template <typename T>
__aicore__ inline __gm__ T* GetUnderlyingPtr(__gm__ T* val)
{
    return val;
}
#endif

template <typename T>
__aicore__ inline __ubuf__ T* GetUnderlyingPtr(__ubuf__ T* val)
{
    return val;
}

template <typename T>
__aicore__ inline __cbuf__ T* GetUnderlyingPtr(__cbuf__ T* val)
{
    return val;
}

template <typename T>
__aicore__ inline __cc__ T* GetUnderlyingPtr(__cc__ T* val)
{
    return val;
}

// dav-c100 / dav-m200 不允许 CCE 内存属性 __fbuf__，且没有 Fixpipe Buffer。
// 这些架构上的指针透传由上方 T* 重载覆盖。
#if !defined(__DAV_C100__) && !defined(__DAV_M200__) && \
    (!defined(__NPU_ARCH__) || (__NPU_ARCH__ != 1001 && __NPU_ARCH__ != 2002))
template <typename T>
__aicore__ inline __fbuf__ T* GetUnderlyingPtr(__fbuf__ T* val)
{
    return val;
}
#endif
#endif
} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_PTR_UTILS_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PTR_UTILS_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PTR_UTILS_H__
#endif
