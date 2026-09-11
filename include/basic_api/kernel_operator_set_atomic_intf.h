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
 * \file kernel_operator_set_atomic_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_set_atomic_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SET_ATOMIC_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_SET_ATOMIC_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_SET_ATOMIC_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include "stub_def.h"
#endif

namespace AscendC {
template <typename T>
/**
 * @ingroup simd_api.basic_api.atomic_operations
 * @override_id SetAtomicType__z
 * @archs 3510 2201 3002 2002
 * @brief 通过设置模板参数来设定原子操作不同的数据类型。
 */
__aicore__ inline void SetAtomicType();

template <typename T>
/**
 * @ingroup simd_api.basic_api.atomic_operations
 * @override_id SetAtomicAdd__z
 * @archs 3510 2201 3002 2002 1001
 * @brief 对后续目的地址为GM的数据搬运开启原子累加。原子累加过程：将待拷贝的内容和GM已有内容进行求和，然后将求和结果写入GM。SetAtomicAdd接口可通过模板参数设定不同的累加数据类型。
 */
__aicore__ inline void SetAtomicAdd();

/**
 * @ingroup simd_api.basic_api.atomic_operations
 * @override_id DisableDmaAtomic__z
 * @archs 3510 2201 3002 2002 1001
 * @brief 关闭数据搬运随路原子操作功能，后续执行数据搬运时，GM中原始数据将被新搬运数据完全覆盖。
 */
__aicore__ inline void DisableDmaAtomic();

// SetAtomicNone has been updated, please use DisableDmaAtomic instead.
__aicore__ inline void SetAtomicNone();

template <typename T>
/**
 * @ingroup simd_api.basic_api.atomic_operations
 * @override_id SetAtomicMax__z
 * @archs 3510 2201
 * @brief 设置后续搬运到GM的数据是否执行原子比较：将待拷贝的内容和GM已有内容进行比较，然后将最大值写入GM。SetAtomicMax接口可通过设置模板参数来设定不同的数据类型。
 */
__aicore__ inline void SetAtomicMax();

template <typename T>
/**
 * @ingroup simd_api.basic_api.atomic_operations
 * @override_id SetAtomicMin__z
 * @archs 3510 2201
 * @brief 设置后续搬运到GM的数据是否执行原子比较：将待拷贝的内容和GM已有内容进行比较，然后将最小值写入GM。SetAtomicMin接口可通过设置模板参数来设定不同的数据类型。
 */
__aicore__ inline void SetAtomicMin();
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_set_atomic_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_SET_ATOMIC_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SET_ATOMIC_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SET_ATOMIC_INTF_H__
#endif
