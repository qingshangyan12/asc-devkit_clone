// Compile the public DumpTensor overloads without unrelated high-level API headers.
#include "basic_api/kernel_operator_dump_tensor_intf.h"
extern "C" __global__ __aicore__ void compile_super_tensor(__gm__ float* source, uint32_t count)
{
    AscendC::GlobalTensor<float> global;
    global.SetGlobalBuffer(source, count);
    uint32_t dims[] = {2, 4};
    AscendC::ShapeInfo shape(2, dims);
    AscendC::DumpTensor(global, 1, count);
    AscendC::DumpTensor(global, 2, count, shape);
#if defined(__DAV_VEC__) || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3002
    AscendC::LocalTensor<float> ub(AscendC::TPosition::VECIN, 0, 4096);
    AscendC::DumpTensor(ub, 3, 4096, shape);
#endif
#if defined(__DAV_CUBE__) || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3002
    AscendC::LocalTensor<float> l1(AscendC::TPosition::A1, 0, 4096);
    AscendC::LocalTensor<float> l0c(AscendC::TPosition::CO1, 0, 4096);
    AscendC::DumpTensor(l1, 4, 4096);
    AscendC::DumpTensor(l0c, 5, 4096, shape);
#endif
}
