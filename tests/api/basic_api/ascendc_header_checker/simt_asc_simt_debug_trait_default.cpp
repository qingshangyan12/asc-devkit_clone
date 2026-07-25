/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(ASCENDC_CPU_DEBUG)
#include "impl/utils/debug/asc_simd_vf_debug_traits.h"

static_assert(!SimdVfTraits<SimdVfDebugTag>::enabled,
    "SIMD VF debug must be disabled by default.");
#else
using __cce_scalar::get_shmem_sz;

#include "simt_api/asc_simt.h"
#include "impl/utils/debug/asc_simd_vf_debug_traits.h"

static_assert(!SimdVfTraits<SimdVfDebugTag>::enabled,
    "Including simt_api/asc_simt.h must not enable SIMD VF debug.");

__simt_vf__ __launch_bounds__(1) inline void KernelTestAscSimtPrintf(__gm__ const char* fmt)
{
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    printf(fmt);
    __asc_simt_vf::printf(fmt);
#endif
}
#endif
