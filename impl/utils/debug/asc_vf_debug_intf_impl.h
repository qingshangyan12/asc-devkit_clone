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
 * \file asc_vf_debug_intf_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message("impl/utils/debug/asc_vf_debug_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"utils/debug/asc_printf.h\"\" or \"#include \"utils/debug/asc_dump.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_VF_DEBUG_INTF_IMPL_H__
#endif

#ifndef IMPL_UTILS_DEBUG_ASC_VF_DEBUG_INTF_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_VF_DEBUG_INTF_IMPL_H

#include "impl/utils/sys_macros.h"
#include "impl/utils/debug/asc_debug_utils.h"

namespace AscendC {
namespace detail {
struct AscVFDebugDisabled {};

template <auto funcPtr>
struct AscVFDebugTag {};

template <typename Tag>
struct AscVFDebugState {
    using Type = AscVFDebugDisabled;
};

template <typename State>
struct AscVFDebugRunner {
    template <auto funcPtr, typename... Args>
    __aicore__ static inline void Run(Args&&... args)
    {
        AscVFDebugInitUb();
        funcPtr(args...);
        AscVFDebugTransferUb();
    }
};
} // namespace detail
} // namespace AscendC

#endif // IMPL_UTILS_DEBUG_ASC_VF_DEBUG_INTF_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_VF_DEBUG_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_VF_DEBUG_INTF_IMPL_H__
#endif
