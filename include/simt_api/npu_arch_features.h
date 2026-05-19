/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef INCLUDE_SIMT_API_NPU_ARCH_FEATURES_H
#define INCLUDE_SIMT_API_NPU_ARCH_FEATURES_H

/*
 * Arch capability map for simt_api.
 *
 * When adding a new __NPU_ARCH__ value, consult the hardware spec and add it
 * to the appropriate capability macros below. Each macro's comment describes
 * the required hardware feature.
 *
 * Currently all SIMT capabilities share the same arch set (3510 || 5102).
 * To restrict a specific capability to a subset, extract it to its own #if block.
 *
 */

/* All SIMT capabilities currently supported on the same arch set.
 * Split into separate #if blocks when a new arch supports only a subset. */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#  define __NPU_HAS_FP16__          /* asc_fp16.h: half/half2 ops */
#  define __NPU_HAS_BF16__          /* asc_bf16.h: bfloat16/bfloat16x2 ops */
#  define __NPU_HAS_FP8__           /* asc_fp8.h: fp8 conversion ops */
#  define __NPU_HAS_ATOMIC__        /* device_atomic_functions.h: asc_atomic_* */
#  define __NPU_HAS_WARP__          /* device_warp_functions.h: asc_shfl/ballot/etc. */
#  define __NPU_HAS_VECTOR_TYPES__  /* vector_functions.h: make_short2/int4/etc. */
#  define __NPU_HAS_VF_CALL__       /* common_functions.h: asc_vf_call template */
#endif

/* DAV vector core with split-core scheduling support (arch part only).
 * Business condition defined(__DAV_VEC__) stays in common_functions.h. */
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)
#  define __NPU_DAV_VEC_ARCH__
#endif

/* Native async_invoke execution path (arch part only).
 * Combined with SPLIT_CORE_VEC / ASCENDC_CPU_DEBUG in common_functions.h. */
#if __NPU_ARCH__ == 5102
#  define __NPU_ASYNC_INVOKE_ARCH__
#endif

#endif  // INCLUDE_SIMT_API_NPU_ARCH_FEATURES_H
