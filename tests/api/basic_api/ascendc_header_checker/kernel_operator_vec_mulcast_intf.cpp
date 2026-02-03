#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_vec_mulcast_intf.h"
#endif

// __aicore__ inline void MulCast(const LocalTensor<T> &dst, const LocalTensor<U> &src0, const LocalTensor<U> &src1, uint64_t mask, const uint8_t repeatTime, const BinaryRepeatParams &repeatParams);
extern "C" __global__ __aicore__ void KernelTestMulCast1() {
    AscendC::LocalTensor<int8_t> dst;
    AscendC::LocalTensor<half> src0;
    AscendC::LocalTensor<half> src1;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 1;
    AscendC::BinaryRepeatParams repeatParams;
    AscendC::MulCast(dst, src0, src1, mask, repeatTime, repeatParams);
}

// __aicore__ inline void MulCast(const LocalTensor<T> &dst, const LocalTensor<U> &src0, const LocalTensor<U> &src1, uint64_t mask[], const uint8_t repeatTime, const BinaryRepeatParams &repeatParams);
extern "C" __global__ __aicore__ void KernelTestMulCast2() {
    AscendC::LocalTensor<int8_t> dst;
    AscendC::LocalTensor<half> src0;
    AscendC::LocalTensor<half> src1;
    uint64_t mask[2] = {0};
    uint8_t repeatTime = 1;
    AscendC::BinaryRepeatParams repeatParams;
    AscendC::MulCast(dst, src0, src1, mask, repeatTime, repeatParams);
}

// __aicore__ inline void MulCast(const LocalTensor<T> &dst, const LocalTensor<U> &src0, const LocalTensor<U> &src1, uint32_t count);
extern "C" __global__ __aicore__ void KernelTestMulCast3() {
    AscendC::LocalTensor<int8_t> dst;
    AscendC::LocalTensor<half> src0;
    AscendC::LocalTensor<half> src1;
    uint32_t count = 0;
    AscendC::MulCast(dst, src0, src1, count);
}
