#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_scalar_intf.h"
#endif

#if (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102)
// __aicore__ inline void GetUintDivMagicAndShift(T& magic, T& shift, T divisor);
extern "C" __global__ __aicore__ void KernelTestGetUintDivMagicAndShift1() {
    uint32_t magic = 0;
    uint32_t shift = 0;
    uint32_t divisor = 0;
    AscendC::GetUintDivMagicAndShift(magic, shift, divisor);
}
#endif

#if __NPU_ARCH__ == 2201 || (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102)
// __aicore__ inline void WriteGmByPassDCache(__gm__ T* addr, T value);
extern "C" __global__ __aicore__ void KernelTestWriteGmByPassDCache1() {
    __gm__ int32_t* addr;
    int32_t value = 0;
    AscendC::WriteGmByPassDCache(addr, value);
}
#endif
