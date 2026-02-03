#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_sys_var_intf.h"
#endif

// __aicore__ inline void GetArchVersion(uint32_t& coreVersion);
extern "C" __global__ __aicore__ void KernelTestGetArchVersion1() {
    uint32_t coreVersion = 0;
    AscendC::GetArchVersion(coreVersion);
}

// __aicore__ inline void Trap();
extern "C" __global__ __aicore__ void KernelTestTrap1() {
    AscendC::Trap();
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102))
// template <SpecialPurposeReg spr>
// __aicore__ inline void ClearSpr();
extern "C" __global__ __aicore__ void KernelTestClearSpr1() {
    AscendC::ClearSpr<AscendC::SpecialPurposeReg::AR>();
}
#endif
