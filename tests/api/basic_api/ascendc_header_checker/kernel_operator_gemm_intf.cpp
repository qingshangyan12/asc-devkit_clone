#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_gemm_intf.h"
#endif

// __aicore__ inline __inout_pipe__(V) void Gemm(const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<S>& src1, const uint32_t m, const uint32_t k, const uint32_t n, GemmTiling tilling, bool partialsum = true, int32_t initValue = 0);
extern "C" __global__ __aicore__ void KernelTestGemm1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<half> src0;
    AscendC::LocalTensor<half> src1;
    uint32_t m = 0;
    uint32_t k = 0;
    uint32_t n = 0;
    AscendC::GemmTiling tilling;
    bool partialsum = true ;
    int32_t initValue = 0;
    AscendC::Gemm(dst, src0, src1, m, k, n, tilling, true, 0);
}
