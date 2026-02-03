#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_conv2d_intf.h"
#endif

// __aicore__ inline __in_pipe__(MTE2)__out_pipe__(MTE3) void Conv2D(const LocalTensor<T> &dst, const LocalTensor<T> &bias, const LocalTensor<U> &featureMap, const LocalTensor<U> &weight, Conv2dParams &conv2dParams, Conv2dTilling &tilling);
extern "C" __global__ __aicore__ void KernelTestConv2D1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> bias;
    AscendC::LocalTensor<half> featureMap;
    AscendC::LocalTensor<half> weight;
    AscendC::Conv2dParams conv2dParams;
    AscendC::Conv2dTilling tilling;
    AscendC::Conv2D(dst, bias, featureMap, weight, conv2dParams, tilling);
}
