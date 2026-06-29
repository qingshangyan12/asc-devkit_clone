/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file asc_printf_impl.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_PRINTF_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_PRINTF_IMPL_H

#ifndef __CHECK_FEATURE_AT_PRECOMPILE
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#include "impl/utils/debug/asc_printf_simt_impl.h"
#endif
#endif

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#ifndef __CHECK_FEATURE_AT_PRECOMPILE
#if (__NPU_ARCH__ == 2002) || (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#include "impl/utils/debug/asc_aicore_printf_impl.h"
namespace __asc_aicore {
template <class... Args>
__aicore__ inline void printf(__gm__ const char* fmt, Args&&... args)
{
    printf_impl(fmt, args...);
}

template <class... Args>
__aicore__ inline void PRINTF(__gm__ const char* fmt, Args&&... args)
{
    printf_impl(fmt, args...);
}
} // namespace __asc_aicore
#else
namespace __asc_aicore {
template <class... Args>
__aicore__ inline void printf(__gm__ const char* fmt, Args&&... args) {}
template <class... Args>
__aicore__ inline void PRINTF(__gm__ const char* fmt, Args&&... args) {}
} // namespace __asc_aicore
#endif // (__NPU_ARCH__ == 2002) || (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)
#endif

namespace __asc_simd_vf {
template <class... Args>
__ASC_USE_RESERVED_UBUF__(3510,
    "printf is forbidden when compile option --cce-disable-asc-reserved-ubuf is enabled")
__simd_callee__ inline void printf(__ubuf__ const char* fmt, Args&&... args)
{
    printf_impl(fmt, args...);
}
}
#endif

#endif // IMPL_UTILS_DEBUG_ASC_PRINTF_IMPL_H
