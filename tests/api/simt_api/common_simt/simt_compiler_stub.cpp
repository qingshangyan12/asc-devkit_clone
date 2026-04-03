/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "simt_compiler_stub.h"
#include "kernel_process_lock.h"
#include "kernel_utils.h"
#include "kernel_simt_cpu.h"
#include "stub_def.h"
#include <cmath>
int64_t block_idx = 0;
int64_t block_num = 8;
int32_t asc_get_block_idx()
{
    return 0;
}

int32_t asc_get_block_num()
{
    return 1;
}

int64_t clz(uint64_t x)
{
    return 0;
}
int64_t bcnt1(uint64_t x)
{
    return 0;
}
namespace AscendC {
namespace check {
struct VecBinaryScalarApiParams {
};

bool CheckFunVecBinaryScalarImplForMaskArray(VecBinaryScalarApiParams &chkParams,
                                             const uint64_t mask[],
                                             const char *intriName) { return true; }
bool CheckFunVecBinaryScalarImpl(VecBinaryScalarApiParams &chkParams, const uint64_t mask, const char *intriName) {
    return true;
}
bool CheckFunVecBinaryScalarImpl(VecBinaryScalarApiParams &chkParams, const char *intriName) { return true; }
}
}

uint64_t atomicAdd(uint64_t *addr, uint64_t val) {
    return 0;
}