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
 * \file asc_printf_simd_impl.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_PRINTF_SIMD_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_PRINTF_SIMD_IMPL_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_PRINTF_SIMD_IMPL__
#warning "asc_printf_simd_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future."
#endif

#include "impl/utils/debug/asc_debug_utils.h"
#include "impl/utils/debug/asc_debug_types.h"

template <class... Args>
__aicore__ inline void printf_impl(__gm__ const char* fmt, Args&&... args)
{
    __gm__ DebugBlockHeadInfo* blockRingBufInfo = GetBlockRingBufInfo();
    if (blockRingBufInfo == nullptr) {
        return;
    }
    uint32_t argsNum = 0;
    const uint32_t& tlvLen = GetPrintTlvLen(argsNum, fmt, args...);
    if (!CheckAndWaitRingBufSpace(blockRingBufInfo, tlvLen)) {
        return;
    }

    __gm__ PrintTlvInfoHead* printTlv = reinterpret_cast<__gm__ PrintTlvInfoHead*>(GetRingBufTlv(blockRingBufInfo));

    WriteRingBufTlvHead(printType, printTlv, tlvLen, argsNum);
    WriteRingBufTlvData(printTlv, fmt, args...);

    __gm__ RingBufWriteInfo* writeInfo = GetRingBufWriteInfo(blockRingBufInfo);

    UpdateWriteInfo(writeInfo, tlvLen);
}


namespace __asc_aicore {
template <class... Args>
__aicore__ inline void printf(__gm__ const char* fmt, Args&&... args)
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    printf_impl(__gm__ const char* fmt, Args&&... args);
    set_ctrl(ctrlValue);
}
}

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_PRINTF_SIMD_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_PRINTF_SIMD_IMPL__
#endif

#endif