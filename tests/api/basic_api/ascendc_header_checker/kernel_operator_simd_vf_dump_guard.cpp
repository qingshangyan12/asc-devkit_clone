/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifdef ASCENDC_DUMP
#undef ASCENDC_DUMP
#endif
#define ASCENDC_DUMP 0

#if !defined(ASCENDC_CPU_DEBUG)
using __cce_scalar::get_shmem_sz;
#endif

#include "kernel_operator.h"
#include "utils/debug/asc_dump.h"

struct IncompleteDumpType;

extern "C" __global__ __aicore__ void KernelTestSimdVfDumpOff()
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510) && defined(__DAV_C310_VEC__)
    __ubuf__ IncompleteDumpType* ubInput = (__ubuf__ IncompleteDumpType*)0;
    IncompleteDumpType* regInput = (IncompleteDumpType*)0;

    __asc_simd_vf::asc_dump_ubuf<IncompleteDumpType>(ubInput, 0, 1);
    __asc_simd_vf::asc_dump<IncompleteDumpType>(ubInput, 0, 1);
    __asc_simd_vf::asc_dump_reg<IncompleteDumpType>(*regInput, 0, 1);
    __asc_simd_vf::asc_dump<IncompleteDumpType>(*regInput, 0, 1);
#endif
}
