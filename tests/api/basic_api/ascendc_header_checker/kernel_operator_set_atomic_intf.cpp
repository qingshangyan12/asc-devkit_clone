#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_set_atomic_intf.h"
#endif

// __aicore__ inline void SetAtomicType();
extern "C" __global__ __aicore__ void KernelTestSetAtomicType1() {
    AscendC::SetAtomicType<int32_t>();
}

// __aicore__ inline void SetAtomicAdd();
extern "C" __global__ __aicore__ void KernelTestSetAtomicAdd1() {
    AscendC::SetAtomicAdd<int32_t>();
}

// __aicore__ inline void SetAtomicNone();
extern "C" __global__ __aicore__ void KernelTestSetAtomicNone1() {
    AscendC::SetAtomicNone();
}

// __aicore__ inline void SetAtomicMax();
extern "C" __global__ __aicore__ void KernelTestSetAtomicMax1() {
    AscendC::SetAtomicMax<int32_t>();
}

// __aicore__ inline void SetAtomicMin();
extern "C" __global__ __aicore__ void KernelTestSetAtomicMin1() {
    AscendC::SetAtomicMin<int32_t>();
}
