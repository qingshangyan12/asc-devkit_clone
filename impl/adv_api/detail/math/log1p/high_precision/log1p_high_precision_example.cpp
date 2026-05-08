/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file log1p_high_precision_example.cpp
 * \brief Example usage of high-precision log1p implementation
 */

#include "kernel_operator.h"

using namespace AscendC;

constexpr int32_t BUFFER_SIZE = 256;

class Log1pHighPrecisionKernel {
public:
    __aicore__ Log1pHighPrecisionKernel() {}
    __aicore__ inline void Process(const LocalTensor<float>& dst, const LocalTensor<float>& src, uint32_t count)
    {
        Log1pHighPrecisionImpl<float>(dst, src, count);
    }
};

class Log1pHighPrecisionKernelHalf {
public:
    __aicore__ Log1pHighPrecisionKernelHalf() {}
    __aicore__ inline void Process(const LocalTensor<half>& dst, const LocalTensor<half>& src, uint32_t count)
    {
        Log1pHighPrecisionImpl<half>(dst, src, count);
    }
};

extern "C" __global__ __aicore__ void log1p_high_precision_kernel_float(
    __gm__ float* dst, __gm__ float* src, uint32_t count)
{
    GM_ADDR dstGM = (__gm__ GM_ADDR)dst;
    GM_ADDR srcGM = (__gm__ GM_ADDR)src;
    
    TPipe pipe;
    TQue<QuePosition::VECIN, 1> srcQueue;
    TQue<QuePosition::VECOUT, 1> dstQueue;
    
    pipe.InitBuffer(srcQueue, 1, BUFFER_SIZE * sizeof(float));
    pipe.InitBuffer(dstQueue, 1, BUFFER_SIZE * sizeof(float));
    
    Log1pHighPrecisionKernel kernel;
    
    uint32_t offset = 0;
    while (offset < count) {
        uint32_t processCount = (count - offset) > BUFFER_SIZE ? BUFFER_SIZE : (count - offset);
        
        LocalTensor<float> srcLocal = srcQueue.AllocTensor<float>();
        DataCopy(srcLocal, srcGM[offset], processCount);
        srcQueue.EnQue(srcLocal);
        
        LocalTensor<float> srcTensor = srcQueue.DeQue<float>();
        LocalTensor<float> dstLocal = dstQueue.AllocTensor<float>();
        
        kernel.Process(dstLocal, srcTensor, processCount);
        
        dstQueue.EnQue(dstLocal);
        LocalTensor<float> dstTensor = dstQueue.DeQue<float>();
        DataCopy(dstGM[offset], dstTensor, processCount);
        
        srcQueue.FreeTensor(srcTensor);
        dstQueue.FreeTensor(dstTensor);
        
        offset += processCount;
    }
}

extern "C" __global__ __aicore__ void log1p_high_precision_kernel_half(
    __gm__ half* dst, __gm__ half* src, uint32_t count)
{
    GM_ADDR dstGM = (__gm__ GM_ADDR)dst;
    GM_ADDR srcGM = (__gm__ GM_ADDR)src;
    
    TPipe pipe;
    TQue<QuePosition::VECIN, 1> srcQueue;
    TQue<QuePosition::VECOUT, 1> dstQueue;
    
    pipe.InitBuffer(srcQueue, 1, BUFFER_SIZE * sizeof(half));
    pipe.InitBuffer(dstQueue, 1, BUFFER_SIZE * sizeof(half));
    
    Log1pHighPrecisionKernelHalf kernel;
    
    uint32_t offset = 0;
    while (offset < count) {
        uint32_t processCount = (count - offset) > BUFFER_SIZE ? BUFFER_SIZE : (count - offset);
        
        LocalTensor<half> srcLocal = srcQueue.AllocTensor<half>();
        DataCopy(srcLocal, srcGM[offset], processCount);
        srcQueue.EnQue(srcLocal);
        
        LocalTensor<half> srcTensor = srcQueue.DeQue<half>();
        LocalTensor<half> dstLocal = dstQueue.AllocTensor<half>();
        
        kernel.Process(dstLocal, srcTensor, processCount);
        
        dstQueue.EnQue(dstLocal);
        LocalTensor<half> dstTensor = dstQueue.DeQue<half>();
        DataCopy(dstGM[offset], dstTensor, processCount);
        
        srcQueue.FreeTensor(srcTensor);
        dstQueue.FreeTensor(dstTensor);
        
        offset += processCount;
    }
}

#ifndef __CCE_KT_TEST__
void main()
{
    constexpr uint32_t count = 1024;
    
    __gm__ float* srcGM = (__gm__ float*)0x10000000;
    __gm__ float* dstGM = (__gm__ float*)0x20000000;
    
    log1p_high_precision_kernel_float(dstGM, srcGM, count);
}
#endif