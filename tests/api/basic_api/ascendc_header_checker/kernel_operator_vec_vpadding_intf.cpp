#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_vec_vpadding_intf.h"
#endif

// __aicore__ inline void VectorPadding(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint8_t padMode, const bool padSide, const uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
extern "C" __global__ __aicore__ void KernelTestVectorPadding1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    uint8_t padMode = 1;
    bool padSide = true;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 1;
    AscendC::UnaryRepeatParams repeatParams;
    AscendC::VectorPadding(dst, src, padMode, padSide, mask, repeatTime, repeatParams);
}

// __aicore__ inline void VectorPadding(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint8_t padMode, const bool padSide, const uint64_t mask[], const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
extern "C" __global__ __aicore__ void KernelTestVectorPadding2() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    uint8_t padMode = 1;
    bool padSide = true;
    uint64_t mask[2] = {0};
    uint8_t repeatTime = 1;
    AscendC::UnaryRepeatParams repeatParams;
    AscendC::VectorPadding(dst, src, padMode, padSide, mask, repeatTime, repeatParams);
}

// __aicore__ inline void VectorPadding(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint8_t padMode, const bool padSide, const uint32_t count);
extern "C" __global__ __aicore__ void KernelTestVectorPadding3() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    uint8_t padMode = 1;
    bool padSide = true;
    uint32_t count = 0;
    AscendC::VectorPadding(dst, src, padMode, padSide, count);
}
