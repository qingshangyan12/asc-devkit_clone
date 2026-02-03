#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_vec_brcb_intf.h"
#endif

#if __NPU_ARCH__ != 3102
// __aicore__ inline void Brcb(const LocalTensor<T>& dst, const LocalTensor<T>& src0, const uint8_t repeatTime, const BrcbRepeatParams& repeatParams);
extern "C" __global__ __aicore__ void KernelTestBrcb1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    uint8_t repeatTime = 1;
    AscendC::BrcbRepeatParams repeatParams;
    AscendC::Brcb(dst, src0, repeatTime, repeatParams);
}
#endif
