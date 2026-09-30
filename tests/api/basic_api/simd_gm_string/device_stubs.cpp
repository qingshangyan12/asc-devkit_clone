/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
// Only intrinsics reachable from the FIFO publisher are needed. Cache operations and
// core selection are intercepted by mockcpp; no simulator or NPU runtime is loaded.
#include <cstdint>

int32_t g_coreType = 0;
int64_t get_coreid() { return 0; }
void dcci(void*, uint64_t, uint64_t) {}
uint64_t ld_dev(uint64_t* address, short) { return *address; }
uint64_t get_sys_cnt()
{
    // Expire a real FIFO wait promptly if a test accidentally reaches its retry loop.
    static uint64_t clock = 0;
    return clock += 100000000000ULL;
}

uint64_t get_shmem_sz() { return 0; }
