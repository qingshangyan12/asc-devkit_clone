/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if __NPU_ARCH__ == 2201 && !defined(ASCENDC_CPU_DEBUG)
#include "utils/debug/asc_dump.h"

extern "C" __global__ __aicore__ void CheckDumpCopy2201(__gm__ uint8_t* fifo, __gm__ float* input)
{
    g_sysPrintFifoSpace = fifo;
#if defined(__DAV_CUBE__)
    __asc_aicore::asc_dump_impl<AscendC::Hardware::L0C, float>((__cc__ float*)0, 0, 257, nullptr, 0);
    __asc_aicore::asc_dump_impl<AscendC::Hardware::L1, float>((__cbuf__ float*)0, 0, 33, nullptr, 0);
#else
    __asc_aicore::asc_dump_impl<AscendC::Hardware::UB, float>((__ubuf__ float*)0, 0, 33, nullptr, 0);
#endif
    __asc_aicore::asc_dump_impl<AscendC::Hardware::GM, float>(input, 0, 33, nullptr, 0);
}
#endif
