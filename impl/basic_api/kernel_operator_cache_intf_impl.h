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
 * \file kernel_operator_cache_intf_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_cache_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_cache_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CACHE_INTF_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_CACHE_INTERFACE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_CACHE_INTERFACE_IMPL_H

#include "../../include/basic_api/kernel_tensor.h"
#include "kernel_reg.h"
#include "utils/kernel_ptr_utils.h"

#if __NPU_ARCH__ == 1001
#include "dav_c100/kernel_operator_cache_impl.h"
#elif __NPU_ARCH__ == 2002
#include "dav_m200/kernel_operator_cache_impl.h"
#elif __NPU_ARCH__ == 2201
#include "dav_c220/kernel_operator_cache_impl.h"
#elif __NPU_ARCH__ == 3002
#include "dav_m300/kernel_operator_cache_impl.h"
#elif __NPU_ARCH__ == 3003
#include "dav_l300/kernel_operator_cache_impl.h"
#elif __NPU_ARCH__ == 3102
#include "dav_m310/kernel_operator_cache_impl.h"
#elif __NPU_ARCH__ == 3510
#include "dav_3510/kernel_operator_cache_impl.h"
#elif (__NPU_ARCH__ == 5102)
#include "dav_m510/kernel_operator_cache_impl.h"
#elif (__NPU_ARCH__ == 3113)
#include "dav_l311/kernel_operator_cache_impl.h"
#elif (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
#include "dav_5161/kernel_operator_cache_impl.h"
#endif

namespace AscendC {

template <typename T>
__aicore__ inline void DataCachePreload(const GlobalTensor<uint64_t>& src, const T cacheOffset)
{
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113)
    DataCachePreloadImpl((__gm__ uint64_t*)src.GetPhyAddr(), cacheOffset);
#else
    DataCachePreloadImpl(src, cacheOffset);
#endif
}

// Pointer form: the operand is already a GM pointer, so it is forwarded without any Tensor round trip. The
// address space cast is kept at the call site, matching the Tensor path above.
template <typename T>
__aicore__ inline void DataCachePreload(__gm__ uint64_t* src, const T cacheOffset)
{
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113)
    DataCachePreloadImpl((__gm__ uint64_t*)src, cacheOffset);
#else
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCachePreload with pointer operand");
#endif
}

#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || \
     (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
     (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst)
{
    DcciGMImpl<T, entireType, dcciDst>(const_cast<__gm__ T*>(dst.GetPhyAddr()));
}

template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DataCacheCleanAndInvalid(const LocalTensor<T>& dst)
{
    DcciUBImpl<T, entireType, dcciDst>(const_cast<__ubuf__ T*>(dst.GetPhyAddr()));
}

// Pointer form. T is the address-space qualified element type (or a bare element type where the address space
// is not recoverable, see the kUnknown fallback below), so the address space picks the GM or the UB
// implementation at compile time. The element type handed to the implementation is stripped of the address
// space, because Dcci*Impl adds the space itself in its own signature.
template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DataCacheCleanAndInvalid(const T* dst)
{
    using ElemT = DmaStripAddrSpaceT<T>;
    constexpr DmaAddrSpace space = DmaAddrSpaceOfV<const T*>;
    if constexpr (space == DmaAddrSpace::kGm || space == DmaAddrSpace::kUnknown) {
        DcciGMImpl<ElemT, entireType, dcciDst>(const_cast<__gm__ ElemT*>(dst));
    } else if constexpr (space == DmaAddrSpace::kUb) {
        DcciUBImpl<ElemT, entireType, dcciDst>(const_cast<__ubuf__ ElemT*>(dst));
    } else {
        static_assert(space == DmaAddrSpace::kGm, "DataCacheCleanAndInvalid pointer operand must be GM or UB");
    }
}
#endif

#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 2002) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3510) || \
     (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || \
     (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
template <typename T, CacheLine entireType>
__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst)
{
    DcciGMImpl<T, entireType>(const_cast<__gm__ T*>(dst.GetPhyAddr()));
}

// Pointer form of the "entireType" overload.
template <typename T, CacheLine entireType>
__aicore__ inline void DataCacheCleanAndInvalid(const T* dst)
{
    using ElemT = DmaStripAddrSpaceT<T>;
    constexpr DmaAddrSpace space = DmaAddrSpaceOfV<const T*>;
    if constexpr (space == DmaAddrSpace::kGm || space == DmaAddrSpace::kUnknown) {
        DcciGMImpl<ElemT, entireType>(const_cast<__gm__ ElemT*>(dst));
    } else if constexpr (space == DmaAddrSpace::kUb) {
        // The two-argument DcciUBImpl does not exist: the UB flavour of the "entireType" form is only
        // wired up on the architectures that also provide the three-argument Dcci*Impl family (the
        // Tensor-only counterpart only declares, and never defines, a LocalTensor operand there).
        static_assert(
            (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) ||
                (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) ||
                (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163),
            "DataCacheCleanAndInvalid with a UB pointer operand is not supported on this architecture");
    } else {
        static_assert(space == DmaAddrSpace::kGm, "DataCacheCleanAndInvalid pointer operand must be GM or UB");
    }
}
#endif

__aicore__ inline void ICachePreLoad(const int64_t preFetchLen) { PreLoad(preFetchLen); }

__aicore__ inline int64_t GetICachePreloadStatus() { return GetICachePreloadStatusImpl(); }

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_CACHE_INTERFACE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CACHE_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CACHE_INTF_IMPL_H__
#endif
