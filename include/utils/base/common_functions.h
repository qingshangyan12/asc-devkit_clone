/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef INCLUDE_UTILS_BASE_COMMON_FUNCTIONS_H
#define INCLUDE_UTILS_BASE_COMMON_FUNCTIONS_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_UTILS_BASE_COMMON_FUNCTIONS_H__
#endif

namespace AscendC {
template <auto funcPtr, typename... Args> __aicore__ inline void AscVFCallImpl(Args &&... args)
{
    AscVFDebugInitUb();
    funcPtr(args...);
    AscVFDebugTransferUb();
}
}

template <auto funcPtr, typename... Args> __aicore__ inline void asc_vf_call(Args &&... args)
{
    if ASCEND_IS_AIV {
        AscendC::AscVFCallImpl<funcPtr>(args...);
    }
}

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_UTILS_BASE_COMMON_FUNCTIONS_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_UTILS_BASE_COMMON_FUNCTIONS_H__
#endif
#endif  // INCLUDE_UTILS_BASE_COMMON_FUNCTIONS_H
