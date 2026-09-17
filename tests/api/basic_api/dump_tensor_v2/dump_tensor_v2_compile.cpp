// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "kernel_operator.h"

extern "C" __global__ __aicore__ void dump_tensor_v2_compile(__gm__ float* source, uint32_t count)
{
    AscendC::GlobalTensor<float> global;
    global.SetGlobalBuffer(source, count);
    uint32_t dimensions[] = {8, 256};
    AscendC::ShapeInfo shape(2, dimensions);

    AscendC::DumpTensor(global, 1, count);
    AscendC::DumpTensor(global, 2, count, shape);

#if defined(__DAV_VEC__) || __NPU_ARCH__ == 5102
    AscendC::LocalTensor<float> local(AscendC::TPosition::VECIN, 0, 2048);
#elif defined(__DAV_CUBE__)
    AscendC::LocalTensor<float> local(AscendC::TPosition::CO1, 0, 2048);
#endif
#if defined(__DAV_VEC__) || defined(__DAV_CUBE__) || __NPU_ARCH__ == 5102
    AscendC::DumpTensor(local, 3, 64);
    AscendC::DumpTensor(local, 4, 64, shape);
#endif
#if defined(__DAV_CUBE__) && __NPU_ARCH__ == 2201
    AscendC::LocalTensor<float> l1(AscendC::TPosition::A1, 0, 2048);
    AscendC::DumpTensor(l1, 5, 64);
    AscendC::DumpTensor(l1, 6, 64, shape);
#endif
}
