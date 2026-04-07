/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file cube_common.h
 * \brief
 */

#include "kernel_operator.h"

constexpr uint32_t CUBE_BLOCK = 16;
constexpr uint32_t C0_SIZE = 32;
constexpr uint32_t CUBE_BLOCK_SIZE_B16 = 16 * 16;
constexpr uint32_t CUBE_BLOCK_SIZE_B8 = 16 * 32;
constexpr uint32_t CUBE_BLOCK_NUM = 2;

constexpr static uint16_t LIMIT_MNSIZE = 10;
constexpr static uint16_t ALIGN_NUM = 16;

__aicore__ inline uint16_t CeilDivision(uint16_t numerator, uint16_t denominator) 
{
    return (numerator + denominator - 1) / denominator;
}

__aicore__ inline uint16_t CeilAlign(uint16_t numerator, uint16_t denominator) 
{
    return (numerator + denominator - 1) / denominator * denominator;
}

template <class inputType>
__aicore__ inline void CopyIn(
    AscendC::GlobalTensor<inputType>& aGM,
    AscendC::GlobalTensor<inputType>& bGM,
    AscendC::TQue<AscendC::TPosition::A1, 1>& inQueueA1,
    AscendC::TQue<AscendC::TPosition::B1, 1>& inQueueB1,
    uint32_t kIndex,
    uint16_t m, uint16_t k, uint16_t n,
    uint16_t aSingleSize, uint16_t bSingleSize)
{
    AscendC::LocalTensor<inputType> a1Local = inQueueA1.AllocTensor<inputType>();
    AscendC::LocalTensor<inputType> b1Local = inQueueB1.AllocTensor<inputType>();
    
    AscendC::Nd2NzParams nd2nzA1Params;
    nd2nzA1Params.ndNum = 1;
    nd2nzA1Params.nValue = m;
    nd2nzA1Params.dValue = k;
    nd2nzA1Params.srcNdMatrixStride = 0;
    nd2nzA1Params.srcDValue = k;
    nd2nzA1Params.dstNzC0Stride = CeilAlign(m, CUBE_BLOCK);
    nd2nzA1Params.dstNzNStride = 1;
    nd2nzA1Params.dstNzMatrixStride = 0;
    AscendC::DataCopy(a1Local, aGM[kIndex * aSingleSize], nd2nzA1Params);

    AscendC::Nd2NzParams nd2nzB1Params;
    nd2nzB1Params.ndNum = 1;
    nd2nzB1Params.nValue = k;
    nd2nzB1Params.dValue = n;
    nd2nzB1Params.srcNdMatrixStride = 0;
    nd2nzB1Params.srcDValue = n;
    if constexpr (AscendC::IsSameType<inputType, int8_t>::value) {
        nd2nzB1Params.dstNzC0Stride = CeilAlign(k, C0_SIZE);
    } else {
        nd2nzB1Params.dstNzC0Stride = CeilAlign(k, CUBE_BLOCK);
    }
    nd2nzB1Params.dstNzNStride = 1;
    nd2nzB1Params.dstNzMatrixStride = 0;
    AscendC::DataCopy(b1Local, bGM[kIndex * bSingleSize], nd2nzB1Params);

    inQueueA1.EnQue(a1Local);
    inQueueB1.EnQue(b1Local);
}

template <class inputType>
__aicore__ inline void SplitA(
    AscendC::TQue<AscendC::TPosition::A1, 1>& inQueueA1,
    AscendC::TQue<AscendC::TPosition::A2, 1>& inQueueA2,
    uint16_t m, uint16_t k)
{
    AscendC::LocalTensor<inputType> a1Local = inQueueA1.DeQue<inputType>();
    AscendC::LocalTensor<inputType> a2Local = inQueueA2.AllocTensor<inputType>();
    uint32_t dstOffset = 0;
    uint32_t srcOffset = 0;
    if constexpr (AscendC::IsSameType<inputType, int8_t>::value) {
        dstOffset = CeilDivision(k, C0_SIZE) * CUBE_BLOCK_SIZE_B8;
        srcOffset = CUBE_BLOCK_SIZE_B8;
    } else {
        dstOffset = CeilDivision(k, CUBE_BLOCK) * CUBE_BLOCK_SIZE_B16;
        srcOffset = CUBE_BLOCK_SIZE_B16;
    }
    
    AscendC::LoadData2DParams loadDataParams;
    if constexpr (AscendC::IsSameType<inputType, int8_t>::value) {
        loadDataParams.repeatTimes = CeilDivision(k, C0_SIZE);
    } else {
        loadDataParams.repeatTimes = CeilDivision(k, CUBE_BLOCK);
    }
    loadDataParams.srcStride = CeilDivision(m, CUBE_BLOCK);
    loadDataParams.dstGap = 0;
    loadDataParams.ifTranspose = false;
    
    for (int i = 0; i < CeilDivision(m, CUBE_BLOCK); ++i) {
        AscendC::LoadData(a2Local[i * dstOffset], a1Local[i * srcOffset], loadDataParams);
    }
    inQueueA2.EnQue<inputType>(a2Local);
    inQueueA1.FreeTensor(a1Local);
}

template <class inputType>
__aicore__ inline void SplitBB16(
    AscendC::TQue<AscendC::TPosition::B1, 1>& inQueueB1,
    AscendC::TQue<AscendC::TPosition::B2, 1>& inQueueB2,
    uint16_t k, uint16_t n)
{
    AscendC::LocalTensor<inputType> b1Local = inQueueB1.DeQue<inputType>();
    AscendC::LocalTensor<inputType> b2Local = inQueueB2.AllocTensor<inputType>();

    uint32_t dstOffset = CeilDivision(n, CUBE_BLOCK) * CUBE_BLOCK_SIZE_B16;
    uint32_t srcOffset = CUBE_BLOCK_SIZE_B16;
    
    AscendC::LoadData2DParams loadDataParams;
    loadDataParams.startIndex = 0;
    loadDataParams.repeatTimes = CeilDivision(n, CUBE_BLOCK);
    loadDataParams.srcStride = CeilDivision(k, CUBE_BLOCK);
    loadDataParams.dstGap = 0;
    loadDataParams.ifTranspose = true;
    
    for (int i = 0; i < CeilDivision(k, CUBE_BLOCK); ++i) {
        AscendC::LoadData(b2Local[i * dstOffset], b1Local[i * srcOffset], loadDataParams);
    }
    inQueueB2.EnQue<inputType>(b2Local);
    inQueueB1.FreeTensor(b1Local);
}

template <class inputType>
__aicore__ inline void SplitBB8(
    AscendC::TQue<AscendC::TPosition::B1, 1>& inQueueB1,
    AscendC::TQue<AscendC::TPosition::B2, 1>& inQueueB2,
    uint16_t k, uint16_t n)
{
    AscendC::LocalTensor<inputType> b1Local = inQueueB1.DeQue<inputType>();
    AscendC::LocalTensor<inputType> b2Local = inQueueB2.AllocTensor<inputType>();

    uint32_t dstOffset = CeilDivision(n, C0_SIZE) * CUBE_BLOCK_SIZE_B8 * CUBE_BLOCK_NUM;
    uint32_t srcOffset = CUBE_BLOCK_SIZE_B8 * CUBE_BLOCK_NUM;
    
    AscendC::LoadData2dTransposeParams loadDataParams;
    loadDataParams.startIndex = 0;
    loadDataParams.repeatTimes = CeilDivision(n, C0_SIZE);
    loadDataParams.srcStride = CeilDivision(k, C0_SIZE);
    loadDataParams.dstGap = 1;
    loadDataParams.dstFracGap = 0;
    
    for (int i = 0; i < CeilDivision(k, C0_SIZE); ++i) {
        AscendC::LoadDataWithTranspose(b2Local[i * dstOffset], b1Local[i * srcOffset], loadDataParams);
    }
    inQueueB2.EnQue<inputType>(b2Local);
    inQueueB1.FreeTensor(b1Local);
}

template <class inputType>
__aicore__ inline void SplitB(
    AscendC::TQue<AscendC::TPosition::B1, 1>& inQueueB1,
    AscendC::TQue<AscendC::TPosition::B2, 1>& inQueueB2,
    uint16_t k, uint16_t n)
{
    if constexpr (AscendC::IsSameType<inputType, half>::value) {
        SplitBB16<inputType>(inQueueB1, inQueueB2, k, n);
    } else {
        SplitBB8<inputType>(inQueueB1, inQueueB2, k, n);
    }
}

template <class inputType, class l0cType>
__aicore__ inline void Compute(
    AscendC::TQue<AscendC::TPosition::A2, 1>& inQueueA2,
    AscendC::TQue<AscendC::TPosition::B2, 1>& inQueueB2,
    AscendC::TQue<AscendC::TPosition::CO1, 1>& outQueueCO1,
    AscendC::LocalTensor<l0cType>& c,
    uint32_t kIndex, uint32_t K_ROUND,
    uint16_t m, uint16_t n, uint16_t k,
    bool enUnitFlag)
{
    AscendC::LocalTensor<inputType> a = inQueueA2.DeQue<inputType>();
    AscendC::LocalTensor<inputType> b = inQueueB2.DeQue<inputType>();
    
    AscendC::MmadParams mmadParams;
    mmadParams.m = m;
    mmadParams.n = CeilAlign(n, C0_SIZE);
    mmadParams.k = k;
    
    if (kIndex == 0) {
        mmadParams.cmatrixInitVal = true;
    } else {
        mmadParams.cmatrixInitVal = false;
    }

    if (enUnitFlag) {
        if (kIndex != K_ROUND - 1) {
            mmadParams.unitFlag = 2;
        } else {
            mmadParams.unitFlag = 3;
        }
    }
    
    AscendC::Mmad(c, a, b, mmadParams);
    
    if ((m / ALIGN_NUM) * (n / ALIGN_NUM) < LIMIT_MNSIZE) {
        AscendC::PipeBarrier<PIPE_M>();
    }
    
    if (!enUnitFlag) {
        if (kIndex == K_ROUND - 1) {
            outQueueCO1.EnQue<l0cType>(c);
        }
    }

    inQueueA2.FreeTensor(a);
    inQueueB2.FreeTensor(b);
}
