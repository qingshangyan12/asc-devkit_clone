/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <gtest/gtest.h>
#include <type_traits>
#include <random>
#include "simt_compiler_stub.h"
#include "kernel_operator.h"
#include "simt_fp16.h"

using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

#define THREAD_DIM 128
template <typename T>
class KernelFP16 {
    public:
        __aicore__ KernelFP16() {}
        __aicore__ inline void Process(__gm__ T* dst, int32_t mode);
};

template <typename T>
__simt_vf__ LAUNCH_BOUND(1024) inline __aicore__ void KernelFP16Compute(__gm__ T* dst, int32_t mode)
{
    for(int idx=GetThreadIdx<0>()+block_idx*GetThreadNum<0>();idx < 1; idx+=block_num*GetThreadNum<0>()) {
        if(mode == 0) {
            half a = 2.3;
            a++;
            dst[idx] = ++a;
        } else if(mode == 1) {
            half a = 2.1;
            a--;
            dst[idx] = --a;
        } else if(mode == 2) {
            half a = 2.1;
            half b = 0.0;
            dst[idx] = a && b;
        } else if(mode == 3) {
            half a = 2.1;
            half b = 0.0;
            dst[idx] = a || b;
        } else if(mode == 4) {
            half a = 2.1;
            dst[idx] = Floor(a);
        } else if(mode == 5) {
            half a = 2.1;
            dst[idx] = Rint(a);
        } else if(mode == 6) {
            half a = 2.1;
            dst[idx] = Ceil(a);
        }
    }
}

template <typename T>
__aicore__ inline void KernelFP16<T>::Process(__gm__ T* dst, int32_t mode)
{
    AscendC::Simt::VF_CALL<KernelFP16Compute<T>>(AscendC::Simt::Dim3(THREAD_DIM, 1, 1), dst, mode);
}



struct FP16Params {
    int32_t mode;
};

class FP16Testsuite : public testing::Test, public testing::WithParamInterface<FP16Params> {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(FP16Testsuite, MathApiTest)
{
    float x = static_cast<float>(rand()) / RAND_MAX;
    half x_h = half(x);

    half result = hcos(x_h);
    half expect = half(cosf(x_h.ToFloat()));
    EXPECT_EQ(expect, result);
    
    result = hsin(x_h);
    expect = half(sinf(x_h.ToFloat()));
    EXPECT_EQ(expect, result);

    result = htanh(x_h);
    expect = half(tanhf(x_h.ToFloat()));
    EXPECT_EQ(expect, result);

    result = hexp(x_h);
    expect = half(expf(x_h.ToFloat()));
    EXPECT_EQ(expect, result);

    result = hexp2(x_h);
    expect = half(exp2f(x_h.ToFloat()));
    EXPECT_EQ(expect, result);
    
    result = hexp10(x_h);
    expect = half(powf(10.0, x_h.ToFloat()));
    EXPECT_EQ(expect, result);

    result = hlog(x_h);
    expect = half(logf(x_h.ToFloat()));
    EXPECT_EQ(expect, result);

    result = hlog2(x_h);
    expect = half(log2f(x_h.ToFloat()));
    EXPECT_EQ(expect, result);

    result = hlog10(x_h);
    expect = half(log10f(x_h.ToFloat()));
    EXPECT_EQ(expect, result);

    result = hsqrt(x_h);
    float tmp = sqrtf(x_h.ToFloat());
    expect = half(tmp);
    EXPECT_EQ(expect, result);
    
    result = hrsqrt(x_h);
    expect = half(1.0f / sqrtf(x_h.ToFloat()));
    EXPECT_EQ(expect, result);

    result = hrcp(x_h);
    expect = half(1.0f / x_h.ToFloat());
    EXPECT_EQ(expect, result);
}