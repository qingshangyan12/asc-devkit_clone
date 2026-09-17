// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "kernel_operator.h"

extern "C" __global__ __aicore__ void dump_tensor_2002_compile(__gm__ int32_t* source, uint32_t count)
{
    AscendC::GlobalTensor<int32_t> global;
    global.SetGlobalBuffer(source, count);
    uint32_t dimensions[] = {8, 256};
    AscendC::ShapeInfo shape(2, dimensions);

    AscendC::DumpTensor(global, 1, count);
    AscendC::DumpTensor(global, 2, count, shape);

    AscendC::TPipe pipe;
    AscendC::TBuf<AscendC::TPosition::VECIN> ub_buffer;
    pipe.InitBuffer(ub_buffer, 2048 * sizeof(int32_t));
    auto local = ub_buffer.Get<int32_t>();
    AscendC::DumpTensor(local, 3, 64);
    AscendC::DumpTensor(local, 4, 64, shape);

    AscendC::TBuf<AscendC::TPosition::A1> l1_buffer;
    pipe.InitBuffer(l1_buffer, 2048 * sizeof(int32_t));
    auto l1 = l1_buffer.Get<int32_t>();
    AscendC::DumpTensor(l1, 5, 64);
    AscendC::DumpTensor(l1, 6, 64, shape);

    AscendC::TBuf<AscendC::TPosition::CO1> l0c_buffer;
    pipe.InitBuffer(l0c_buffer, 2048 * sizeof(int32_t));
    auto l0c = l0c_buffer.Get<int32_t>();
    AscendC::DumpTensor(l0c, 7, 64);
    AscendC::DumpTensor(l0c, 8, 64, shape);
}
