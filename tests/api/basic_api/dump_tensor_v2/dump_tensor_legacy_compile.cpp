// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "kernel_operator.h"

extern "C" __global__ __aicore__ void dump_tensor_legacy_compile(__gm__ int32_t* source, uint32_t count)
{
    AscendC::GlobalTensor<int32_t> global;
    global.SetGlobalBuffer(source, count);
    uint32_t dimensions[] = {8, 256};
    AscendC::ShapeInfo shape(2, dimensions);
    AscendC::DumpTensor(global, 1, count);
    AscendC::DumpTensor(global, 2, count, shape);
}
