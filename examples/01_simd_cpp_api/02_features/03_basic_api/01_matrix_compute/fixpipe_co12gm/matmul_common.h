/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef MATMUL_COMMON_H
#define MATMUL_COMMON_H

#include "kernel_operator.h"

// 根据输入类型确定C0_SIZE（每个C0方向的元素数）
#ifdef INPUT_TYPE_IS_HALF
constexpr uint32_t C0_SIZE = 16;      // half: 16个元素 = 32B
#else
constexpr uint32_t C0_SIZE = 32;      // int8: 32个元素 = 32B
#endif

constexpr uint32_t CUBE_BLOCK = 16;            // Cube块大小16x16
constexpr uint32_t CUBE_BLOCK_SIZE = CUBE_BLOCK * CUBE_BLOCK; // 16x16个元素
#ifdef INPUT_TYPE_IS_HALF
constexpr uint32_t CUBE_BLOCK_NUM = 1;         // half: 一个块
#else
constexpr uint32_t CUBE_BLOCK_NUM = 2;         // int8: 因为C0_SIZE=32，每个Cube块占2个C0块
#endif

// 矩阵固定大小
constexpr uint32_t M = 128;
constexpr uint32_t K = 128;
constexpr uint32_t N = 256;
constexpr uint32_t kRound = 2;                 // K轴切分次数

template <typename InputT, typename OutputT, typename L0cT>
class MatmulCore {
public:
    __aicore__ inline MatmulCore() {
        m = M;
        k = K / kRound;
        n = N;
        aSingleSize = m * k;
        bSingleSize = k * n;
        cSingleSize = m * n;

        // 对齐后大小
        aSizeAlignL1 = CeilAlign(m, CUBE_BLOCK) * CeilAlign(k, C0_SIZE);
        aSizeAlignL0 = CeilAlign(m, CUBE_BLOCK) * CeilAlign(k, C0_SIZE);
        bSizeAlignL1 = CeilAlign(k, C0_SIZE) * CeilAlign(n, C0_SIZE);
        bSizeAlignL0 = CeilAlign(k, C0_SIZE) * CeilAlign(n, C0_SIZE);
        cSizeAlignL0 = CeilAlign(m, CUBE_BLOCK) * CeilAlign(n, CUBE_BLOCK);
    }

    __aicore__ inline void Init(GM_ADDR a, GM_ADDR b, AscendC::TPipe* pipeIn) {
        pipe = pipeIn;
        aGM.SetGlobalBuffer((__gm__ InputT*)a);
        bGM.SetGlobalBuffer((__gm__ InputT*)b);
        pipe->InitBuffer(inQueueA1, 1, aSizeAlignL1 * sizeof(InputT));
        pipe->InitBuffer(inQueueA2, 1, aSizeAlignL0 * sizeof(InputT));
        pipe->InitBuffer(inQueueB1, 1, bSizeAlignL1 * sizeof(InputT));
        pipe->InitBuffer(inQueueB2, 1, bSizeAlignL0 * sizeof(InputT));
    }

    __aicore__ inline void Process() {
        for (uint32_t kIdx = 0; kIdx < kRound; ++kIdx) {
            CopyIn(kIdx);
            SplitA(kIdx);
            SplitB(kIdx);
            Compute(kIdx);
        }
    }

protected:
    __aicore__ inline uint16_t CeilDivision(uint16_t numerator, uint16_t denominator) {
        return (numerator + denominator - 1) / denominator;
    }

    __aicore__ inline uint16_t CeilAlign(uint16_t numerator, uint16_t denominator) {
        return (numerator + denominator - 1) / denominator * denominator;
    }

    __aicore__ inline void CopyIn(uint32_t kIdx) {
        AscendC::LocalTensor<InputT> a1Local = inQueueA1.AllocTensor<InputT>();
        AscendC::LocalTensor<InputT> b1Local = inQueueB1.AllocTensor<InputT>();

        // A: ND -> NZ
        AscendC::Nd2NzParams nd2nzA;
        nd2nzA.ndNum = 1;
        nd2nzA.nValue = m;
        nd2nzA.dValue = k;
        nd2nzA.srcNdMatrixStride = 0;
        nd2nzA.srcDValue = k;
        nd2nzA.dstNzC0Stride = CeilAlign(m, CUBE_BLOCK);
        nd2nzA.dstNzNStride = 1;
        nd2nzA.dstNzMatrixStride = 0;
        AscendC::DataCopy(a1Local, aGM[kIdx * aSingleSize], nd2nzA);

        // B: ND -> NZ
        AscendC::Nd2NzParams nd2nzB;
        nd2nzB.ndNum = 1;
        nd2nzB.nValue = k;
        nd2nzB.dValue = n;
        nd2nzB.srcNdMatrixStride = 0;
        nd2nzB.srcDValue = n;
        nd2nzB.dstNzC0Stride = CeilAlign(k, CUBE_BLOCK);
        nd2nzB.dstNzNStride = 1;
        nd2nzB.dstNzMatrixStride = 0;
        AscendC::DataCopy(b1Local, bGM[kIdx * bSingleSize], nd2nzB);

        inQueueA1.EnQue(a1Local);
        inQueueB1.EnQue(b1Local);
    }

    __aicore__ inline void SplitA(uint32_t kIdx) {
        AscendC::LocalTensor<InputT> a1Local = inQueueA1.DeQue<InputT>();
        AscendC::LocalTensor<InputT> a2Local = inQueueA2.AllocTensor<InputT>();

        uint32_t dstOffset = CeilDivision(k, CUBE_BLOCK) * CUBE_BLOCK_SIZE;
        uint32_t srcOffset = CUBE_BLOCK_SIZE;

        AscendC::LoadData2DParams loadParams;
        loadParams.repeatTimes = CeilDivision(k, CUBE_BLOCK);
        loadParams.srcStride = CeilDivision(m, CUBE_BLOCK);
        loadParams.dstGap = 0;
        loadParams.ifTranspose = false;

        for (int i = 0; i < CeilDivision(m, CUBE_BLOCK); ++i) {
            AscendC::LoadData(a2Local[i * dstOffset], a1Local[i * srcOffset], loadParams);
        }

        inQueueA2.EnQue<InputT>(a2Local);
        inQueueA1.FreeTensor(a1Local);
    }

    __aicore__ inline void SplitB(uint32_t kIdx) {
        AscendC::LocalTensor<InputT> b1Local = inQueueB1.DeQue<InputT>();
        AscendC::LocalTensor<InputT> b2Local = inQueueB2.AllocTensor<InputT>();

#ifdef INPUT_TYPE_IS_HALF
        // half: 使用普通LoadData，转置后每个分形大小CUBE_BLOCK_SIZE
        uint32_t dstOffset = CeilDivision(n, CUBE_BLOCK) * CUBE_BLOCK_SIZE;
        uint32_t srcOffset = CUBE_BLOCK_SIZE;
        AscendC::LoadData2DParams loadParams;
        loadParams.repeatTimes = CeilDivision(n, CUBE_BLOCK);
        loadParams.srcStride = CeilDivision(k, CUBE_BLOCK);
        loadParams.dstGap = 0;
        loadParams.ifTranspose = true;
        for (int i = 0; i < CeilDivision(k, CUBE_BLOCK); ++i) {
            AscendC::LoadData(b2Local[i * dstOffset], b1Local[i * srcOffset], loadParams);
        }
#else
        // int8: 使用LoadDataWithTranspose，分形大小为CUBE_BLOCK_SIZE * CUBE_BLOCK_NUM
        uint32_t dstOffset = CeilDivision(n, CUBE_BLOCK) * CUBE_BLOCK_SIZE * CUBE_BLOCK_NUM;
        uint32_t srcOffset = CUBE_BLOCK_SIZE * CUBE_BLOCK_NUM;
        AscendC::LoadData2dTransposeParams loadParams;
        loadParams.startIndex = 0;
        loadParams.repeatTimes = CeilDivision(n, CUBE_BLOCK);
        loadParams.srcStride = CeilDivision(k, CUBE_BLOCK);
        loadParams.dstGap = 1;
        loadParams.dstFracGap = 0;
        for (int i = 0; i < CeilDivision(k, CUBE_BLOCK); ++i) {
            AscendC::LoadDataWithTranspose(b2Local[i * dstOffset], b1Local[i * srcOffset], loadParams);
        }
#endif
        inQueueB2.EnQue<InputT>(b2Local);
        inQueueB1.FreeTensor(b1Local);
    }

    __aicore__ inline void Compute(uint32_t kIdx) {
        AscendC::LocalTensor<InputT> a = inQueueA2.DeQue<InputT>();
        AscendC::LocalTensor<InputT> b = inQueueB2.DeQue<InputT>();

        AscendC::MmadParams mmadParams;
        mmadParams.m = m;
        // mmad默认n向16对齐，但实际需要向C0_SIZE对齐，以便包含有效数据
        mmadParams.n = CeilAlign(n, CUBE_BLOCK);
        mmadParams.k = k;
        if (kIdx == 0) {
            mmadParams.cmatrixInitVal = true;
        } else {
            mmadParams.cmatrixInitVal = false;
        }

#ifdef EN_UNIT_FLAG
        if (kIdx != kRound - 1) {
            mmadParams.unitFlag = 2;
        } else {
            mmadParams.unitFlag = 3;
        }
#endif
        AscendC::Mmad(c, a, b, mmadParams);

        if ((m / CUBE_BLOCK) * (n / CUBE_BLOCK) < 10) {
            AscendC::PipeBarrier<PIPE_M>();
        }

        if constexpr (!defined(EN_UNIT_FLAG)) {
            if (kIdx == kRound - 1) {
                outQueueCO1.EnQue<L0cT>(c);
            }
        }

        inQueueA2.FreeTensor(a);
        inQueueB2.FreeTensor(b);
    }

protected:
    AscendC::TPipe* pipe;
    AscendC::TQue<AscendC::TPosition::A1, 1> inQueueA1;
    AscendC::TQue<AscendC::TPosition::A2, 1> inQueueA2;
    AscendC::TQue<AscendC::TPosition::B1, 1> inQueueB1;
    AscendC::TQue<AscendC::TPosition::B2, 1> inQueueB2;
    AscendC::TQue<AscendC::TPosition::CO1, 1> outQueueCO1;

    AscendC::GlobalTensor<InputT> aGM;
    AscendC::GlobalTensor<InputT> bGM;

    // 注意：cGM、c、quantAlpha等由派生类管理

    uint16_t m, k, n;
    uint16_t aSingleSize, bSingleSize, cSingleSize;
    uint16_t aSizeAlignL1, bSizeAlignL1;
    uint16_t aSizeAlignL0, bSizeAlignL0, cSizeAlignL0;
};

#endif // MATMUL_COMMON_H