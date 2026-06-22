/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <type_traits>

#include "kernel_operator.h"

#ifdef ASCENDC_SIMD_VF_DEBUG
#error "kernel_operator.h must not define ASCENDC_SIMD_VF_DEBUG"
#endif

#include "utils/debug/asc_dump.h"

#ifndef ASCENDC_SIMD_VF_DEBUG
#error "utils/debug/asc_dump.h must define ASCENDC_SIMD_VF_DEBUG"
#endif

#if !defined(ASCENDC_CPU_DEBUG) && defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
__simd_vf__ inline void AscDumpVfCall()
{
    AscendC::Reg::RegTensor<float> input;
    AscendC::Reg::Duplicate<float>(input, static_cast<float>(1));
    asc_dump<float>(input, 1, 1);
}

__aicore__ inline void KernelOperatorAscDumpVfCall()
{
    asc_vf_call<AscDumpVfCall>();
}

static_assert(std::is_same<typename AscendC::detail::AscVFDebugState<
    AscendC::detail::AscVFDebugTag<AscDumpVfCall>>::Type, AscendC::detail::AscVFDebugEnabled>::value);
static_assert(std::is_same<typename __asc_simd_vf::detail::AscVFUbufState<
    __asc_simd_vf::detail::AscVFUbufTag<0>>::Type, __asc_simd_vf::detail::AscVFUbufEnabled>::value);
#endif
