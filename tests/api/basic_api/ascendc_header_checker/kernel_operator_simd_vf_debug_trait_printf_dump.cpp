/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_CPU_DEBUG)
using __cce_scalar::get_shmem_sz;
#endif

#include "kernel_operator.h"
#include "utils/debug/asc_printf.h"
#include "utils/debug/asc_dump.h"

static_assert(SimdVfTraits<SimdVfDebugTag>::enabled,
    "Including asc_printf.h and asc_dump.h together must enable SIMD VF debug exactly once.");

__aicore__ static void SimdVfDebugTraitTarget() {}

__aicore__ inline void InstantiateEnabledSimdVfDebugTrait()
{
    AscendC::AscVFCallImpl<SimdVfDebugTraitTarget>();
}
