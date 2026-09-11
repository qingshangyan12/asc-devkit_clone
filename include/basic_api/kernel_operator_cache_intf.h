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
 * \file kernel_operator_cache_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_cache_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CACHE_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_CACHE_INTF_H
#define ASCENDC_MODULE_OPERATOR_CACHE_INTF_H

#include "../../impl/basic_api/kernel_macros.h"
#include "../../impl/basic_api/kernel_reg.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
template <typename T>
class GlobalTensor;

template <typename T>
class LocalTensor;

template <typename T>
/**
 * @ingroup simd_api.basic_api.cache_control
 * @override_id DataCachePreload__RCgtg1x3_u64_pT
 * @archs 3510 2201 3002 2002
 * @brief 从源地址所在的特定GM地址预加载数据到DCache中，每次调用只能预加载一个Cache Line大小的数据。
 * @param [in] src 源操作数，类型为[GlobalTensor](../data_structures/GlobalTensor/GlobalTensor_intro.md)。支持的数据类型为：uint64_t。
 * @param [in] cacheOffset 在源操作数上偏移cacheOffset大小开始加载数据，单位为byte，支持的数据类型为：int16_t、int64_t。
 */
__aicore__ inline void DataCachePreload(const GlobalTensor<uint64_t>& src, const T cacheOffset);

template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst);

template <typename T, CacheLine entireType, DcciDst dcciDst>
/**
 * @ingroup simd_api.basic_api.cache_control
 * @override_id DataCacheCleanAndInvalid__RCltg1x2_pT
 * @archs 3510 2201 3002 2002 x90 9030
 * @brief 如图1所示，在AI Core内部。
 * @param [in] dst 需要刷新Cache的Tensor。
 */
__aicore__ inline void DataCacheCleanAndInvalid(const LocalTensor<T>& dst);

template <typename T, CacheLine entireType>
__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst);

/**
 * @ingroup simd_api.basic_api.cache_control
 * @override_id ICachePreLoad__i64
 * @archs 3510 2201 2002
 * @brief 开发者手动调用ICachePreLoad接口，能够从指令所在GM地址预加载指令到ICache中。
 * @param [in] preFetchLen 预取长度。
 */
__aicore__ inline void ICachePreLoad(const int64_t preFetchLen);

/**
 * @ingroup simd_api.basic_api.cache_control
 * @override_id GetICachePreloadStatus__z
 * @archs 3510 2201
 * @brief GetICachePreloadStatus为调试接口，在ICachePreLoad后调用，用于获取ICache的PreLoad的状态：当返回值为0时，说明ICache的PreLoad已完成；当返回值为1时，说明ICache的PreLoad未完成。
 */
__aicore__ inline int64_t GetICachePreloadStatus();

} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_cache_intf_impl.h"
#endif
#endif // KERNEL_CACHE_INTF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CACHE_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CACHE_INTF_H__
#endif
