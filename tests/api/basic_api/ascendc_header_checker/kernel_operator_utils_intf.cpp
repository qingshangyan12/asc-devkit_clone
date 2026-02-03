#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_utils_intf.h"
#endif

#if (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102)
// __aicore__ inline void Nop()
extern "C" __global__ __aicore__ void KernelTestNop1() {
    AscendC::Nop();
}
#endif
