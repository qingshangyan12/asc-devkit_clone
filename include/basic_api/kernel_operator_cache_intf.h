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
#include "../../impl/basic_api/utils/kernel_ptr_utils.h"

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
__aicore__ inline void DataCachePreload(const GlobalTensor<uint64_t>& src, const T cacheOffset);

/*!
 * \brief Pointer overload of DataCachePreload. data cache preload only ever addresses GM, so a raw
 *        __gm__ uint64_t* is the equivalent pointer form of GlobalTensor<uint64_t>. Keeping it as a separate
 *        overload (instead of generically replacing the Tensor parameter) leaves every existing explicit
 *        template argument usage resolving exactly as before.
 */
template <typename T>
__aicore__ inline void DataCachePreload(__gm__ uint64_t* src, const T cacheOffset);

template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst);

template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DataCacheCleanAndInvalid(const LocalTensor<T>& dst);

/*!
 * \brief Pointer overload of DataCacheCleanAndInvalid, "entireType + dcciDst" form. GM or UB.
 *
 *        Both spaces are accepted because both have a three-argument implementation to dispatch to
 *        (`DcciGMImpl` / `DcciUBImpl`): this is the overload that carries the `dcciDst` choice, and UB is one of
 *        its documented values (`CACHELINE_UB`). The operand must therefore be an explicitly qualified pointer.
 *
 *        There is deliberately no fallback for an operand whose address space cannot be determined: on this
 *        toolchain `__gm__` is the *default* address space, so a bare `T*` is indistinguishable from
 *        `__gm__ T*`, and accepting it would silently assume GM. Such an operand is rejected by SFINAE.
 */
template <
    typename T, CacheLine entireType, DcciDst dcciDst,
    typename Std::enable_if<
        DmaAddrSpaceOfV<const T*> == DmaAddrSpace::kGm || DmaAddrSpaceOfV<const T*> == DmaAddrSpace::kUb, bool>::type =
        true>
__aicore__ inline void DataCacheCleanAndInvalid(const T* dst);

template <typename T, CacheLine entireType>
__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst);

/*!
 * \brief Pointer overload of DataCacheCleanAndInvalid, "entireType" form. GM ONLY -- see the overload above.
 *        The two-argument UB call does not exist, so a UB operand is rejected by SFINAE rather than turned into
 *        a silent no-op.
 */
template <
    typename T, CacheLine entireType,
    typename Std::enable_if<DmaAddrSpaceOfV<const T*> == DmaAddrSpace::kGm, bool>::type = true>
__aicore__ inline void DataCacheCleanAndInvalid(const T* dst);

__aicore__ inline void ICachePreLoad(const int64_t preFetchLen);

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
