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
 * \file asc_vf_debug_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message("impl/utils/debug/asc_vf_debug_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"utils/debug/asc_printf.h\"\" or \"#include \"utils/debug/asc_dump.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_VF_DEBUG_IMPL_H__
#endif

#ifndef IMPL_UTILS_DEBUG_ASC_VF_DEBUG_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_VF_DEBUG_IMPL_H

#include "impl/utils/debug/asc_vf_debug_intf_impl.h"

#if !defined(ASCENDC_CPU_DEBUG) && defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510) && \
    !defined(__ASC_DISABLE_RESERVED_UBUF__) && defined(ASCENDC_SIMD_VF_DEBUG)
namespace __asc_simd_vf {
namespace detail {
struct AscVFUbufEnabled {};

template <int32_t id>
struct AscVFUbufState<AscVFUbufTag<id>> {
    using Type = AscVFUbufEnabled;
};

template <>
struct AscVFUbufGetter<AscVFUbufEnabled> {
    __simd_callee__ __ubuf__ static inline BlockVFBufInfo* Get(uint64_t addr, uint16_t blockIdx)
    {
        constexpr uint32_t safeStaticLen = 32;
        static __ubuf__ uint64_t info[safeStaticLen];
        if (addr != 0) {
            info[0] = addr;
            auto* bufInfo = reinterpret_cast<__ubuf__ BlockVFBufInfo*>(addr);
            bufInfo->writeLen = 0;
            bufInfo->pidx = 0;
            bufInfo->blockIdx = blockIdx;
        }
        return reinterpret_cast<__ubuf__ BlockVFBufInfo*>(info[0]);
    }
};
} // namespace detail
} // namespace __asc_simd_vf

namespace AscendC {
namespace detail {
struct AscVFDebugEnabled {};

template <auto funcPtr>
struct AscVFDebugState<AscVFDebugTag<funcPtr>> {
    using Type = AscVFDebugEnabled;
};

__aicore__ static __attribute__((noinline)) void AscVFDebugInitUbImpl()
{
    if (g_sysPrintFifoSpace != nullptr) {
        constexpr uint32_t RESERVED_UB_SIZE = 8 * 1024;
        uint64_t ascReservedAddr = get_shmem_sz() - RESERVED_UB_SIZE;
#if defined(__ASC_DISABLE_VF_STACK_RESERVED_UBUF__)
        constexpr uint32_t RESERVED_ASC_UB_SIZE = 2 * 1024;
        ascReservedAddr = get_shmem_sz() - RESERVED_ASC_UB_SIZE;
#endif
        uint16_t blockIdx = __asc_aicore::asc_debug_get_block_idx();
        __asc_aicore::get_printf_ubuf_addr_aicore(ascReservedAddr, blockIdx);
    }
}

__aicore__ static __attribute__((noinline)) void AscVFDebugTransferUbImpl()
{
    if (g_sysPrintFifoSpace != nullptr) {
        pipe_barrier(PIPE_ALL);
        __asc_aicore::asc_vf_debug_ub2gm();
    }
}

template <>
struct AscVFDebugRunner<AscVFDebugEnabled> {
    template <auto funcPtr, typename... Args>
    __aicore__ static inline void Run(Args&&... args)
    {
        AscVFDebugInitUbImpl();
        funcPtr(args...);
        AscVFDebugTransferUbImpl();
    }
};
} // namespace detail
} // namespace AscendC
#endif

#endif // IMPL_UTILS_DEBUG_ASC_VF_DEBUG_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_VF_DEBUG_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_VF_DEBUG_IMPL_H__
#endif
