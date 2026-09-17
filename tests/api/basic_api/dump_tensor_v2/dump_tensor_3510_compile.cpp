// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "kernel_operator.h"

extern "C" __global__ __aicore__ void dump_tensor_3510_compile(__gm__ float* source, uint32_t count)
{
    AscendC::GlobalTensor<float> global;
    global.SetGlobalBuffer(source, count);
    uint32_t dimensions[] = {8, 256};
    AscendC::ShapeInfo shape(2, dimensions);
    AscendC::DumpTensor(global, 1, count);
    AscendC::DumpTensor(global, 6, count, shape);

#if defined(__DAV_VEC__)
    AscendC::LocalTensor<float> ub(AscendC::TPosition::VECIN, 0, 2048);
    AscendC::DumpTensor(ub, 2, 64);
#elif defined(__DAV_CUBE__)
    AscendC::LocalTensor<float> l1(AscendC::TPosition::A1, 0, 2048);
    AscendC::LocalTensor<float> l0a(AscendC::TPosition::A2, 0, 2048);
    AscendC::LocalTensor<float> l0b(AscendC::TPosition::B2, 0, 2048);
    AscendC::LocalTensor<float> l0c(AscendC::TPosition::CO1, 0, 2048);
    AscendC::DumpTensor(l1, 2, 64);
    AscendC::DumpTensor(l0a, 3, 64);
    AscendC::DumpTensor(l0b, 4, 64);
    AscendC::DumpTensor(l0c, 5, 64);
    AscendC::DumpTensor(l1, 7, 64, shape);
    AscendC::DumpTensor(l0c, 8, 64, shape);
#endif
}
