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
 * \file asc_aicore_dump_routing_impl.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_ROUTING_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_ROUTING_IMPL_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_ROUTING_IMPL__
#warning "asc_aicore_dump_routing_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future."
#endif

#include "impl/utils/debug/asc_aicore_dump_impl.h"

namespace __asc_aicore {
template<typename T>
__aicore__ inline void asc_dump_gm(__gm__ T* input, uint32_t desc, uint32_t dumpSize)
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    if (g_sysPrintFifoSpace != nullptr) {
        asc_dump_impl<Hardware::GM, T>(input, desc, dumpSize);
    }
    set_ctrl(ctrlValue);
}

template<typename T>
__aicore__ inline void asc_dump_ubuf(__ubuf__ T* input, uint32_t desc, uint32_t dumpSize);
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    if (g_sysPrintFifoSpace != nullptr) {
        asc_dump_impl<Hardware::UB, T>(input, desc, dumpSize);
    }
    set_ctrl(ctrlValue);
}

template<typename T>
__aicore__ inline void asc_dump_cbuf(__cc__ T* input, uint32_t desc, uint32_t dumpSize)
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    if (g_sysPrintFifoSpace != nullptr) {
        asc_dump_impl<Hardware::L0C, T>(input, desc, dumpSize);
    }
    set_ctrl(ctrlValue);
}

template<typename T>
__aicore__ inline void asc_dump_l1buf(__cbuf__ T* input, uint32_t desc, uint32_t dumpSize)
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    if (g_sysPrintFifoSpace != nullptr) {
        asc_dump_impl<Hardware::L1, T>(input, desc, dumpSize);
    }
    set_ctrl(ctrlValue);
}
}

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_ROUTING_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_ROUTING_IMPL__
#endif

#endif // IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_IMPL_H