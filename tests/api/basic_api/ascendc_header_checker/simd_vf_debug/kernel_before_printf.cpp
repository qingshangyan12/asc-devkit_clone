/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "simd_vf_debug_test_prelude.h"
#include "kernel_operator.h"
#include "utils/debug/asc_printf.h"

static_assert(AscendC::Internal::SimdVfDebugTraits<AscendC::Internal::SimdVfDebugTag>::enabled);

#if (__NPU_ARCH__ == 3510) && !defined(ASCENDC_CPU_DEBUG)
__simd_vf__ static void InstantiateSimdVfPrintf(__ubuf__ const char* fmt) { __asc_simd_vf::printf(fmt, 1U); }
#endif

#include "simd_vf_debug_test_common.h"
