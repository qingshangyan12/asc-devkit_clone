#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_vec_ternary_scalar_intf.h"
#endif

// __aicore__ inline void Axpy(const LocalTensor<T>& dst, const LocalTensor<U>& src, const U& scalarValue, uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
extern "C" __global__ __aicore__ void KernelTestAxpy1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    float scalarValue = 0.0f;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 1;
    AscendC::UnaryRepeatParams repeatParams;
    AscendC::Axpy(dst, src, scalarValue, mask, repeatTime, repeatParams);
}

// __aicore__ inline void Axpy(const LocalTensor<T>& dst, const LocalTensor<U>& src, const U& scalarValue, uint64_t mask[], const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
extern "C" __global__ __aicore__ void KernelTestAxpy2() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    float scalarValue = 0.0f;
    uint64_t mask[2] = {0};
    uint8_t repeatTime = 1;
    AscendC::UnaryRepeatParams repeatParams;
    AscendC::Axpy(dst, src, scalarValue, mask, repeatTime, repeatParams);
}

// __aicore__ inline void Axpy(const LocalTensor<T>& dst, const LocalTensor<U>& src, const U& scalarValue, const int32_t& count);
extern "C" __global__ __aicore__ void KernelTestAxpy3() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    float scalarValue = 0.0f;
    int32_t count = 0;
    AscendC::Axpy(dst, src, scalarValue, count);
}
