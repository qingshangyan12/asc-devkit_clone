/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with
 * the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_utils_underlying_ptr.h
 * \brief 统一萃取底层硬件指针（Pointer / Tensor 双重输入支持，DMA 与 VECTOR 分册共用）
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_utils_underlying_ptr.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_data_copy_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_UNDERLYING_PTR_H__
#endif

#ifndef ASCENDC_MODULE_KERNEL_UTILS_UNDERLYING_PTR_H
#define ASCENDC_MODULE_KERNEL_UTILS_UNDERLYING_PTR_H

#include "../../include/basic_api/kernel_tensor.h"

namespace AscendC {

/*
 * @ingroup GetUnderlyingPtr
 * @brief 萃取入参的底层硬件指针：裸指针（含 __ubuf__ / __gm__ / __cbuf__ / __cc__ 等地址空间限定指针）
 *        原样返回；GlobalTensor 萃取其 __gm__ 数据指针。
 * @par 说明：
 *       1. 指针化扩展（Pointer / Tensor 双重输入）的统一萃取入口，DMA 与 VECTOR 分册共用；
 *       2. LocalTensor 在 NPU 模式下 GetPhyAddr() 返回 uint64_t 偏移，目标地址空间由调用侧类型
 *          （如 __ubuf__ T*）静态给出，由各指针重载自行完成强转，故本工具不提供 LocalTensor 版本。
 * @param [in] val 裸硬件指针
 * @return 裸硬件指针本身
 */
template <typename T>
__aicore__ inline T* GetUnderlyingPtr(T* val)
{
    return val;
}

/*
 * @ingroup GetUnderlyingPtr
 * @brief 萃取 GlobalTensor 的底层硬件指针（__gm__ T*）
 * @param [in] val GlobalTensor 入参
 * @return 其底层 __gm__ 数据指针
 */
template <typename T>
__aicore__ inline __gm__ typename GlobalTensor<T>::PrimType* GetUnderlyingPtr(const GlobalTensor<T>& val)
{
    return const_cast<__gm__ typename GlobalTensor<T>::PrimType*>(val.GetPhyAddr());
}

} // namespace AscendC

#endif // ASCENDC_MODULE_KERNEL_UTILS_UNDERLYING_PTR_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_UNDERLYING_PTR_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_UNDERLYING_PTR_H__
#endif
