/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under
 * the terms and conditions of CANN Open Software License Agreement Version 2.0
 * (the "License"). Please refer to the License for details. You may not use
 * this file except in compliance with the License. THIS SOFTWARE IS PROVIDED ON
 * AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS
 * FOR A PARTICULAR PURPOSE. See LICENSE in the root of the software repository
 * for the full text of the License.
 */

/* !
 * \file adds_custom_v3.h
 * \brief
 */

#ifndef ADDS_CUSTOM_V3_H
#define ADDS_CUSTOM_V3_H
#include "kernel_operator.h"
#include "adds_custom_tiling.h"

using AscendC::TPosition;
class KernelAddsV3 {
public:
    __aicore__ inline KernelAddsV3() {}
    __aicore__ inline void Init(GM_ADDR x, GM_ADDR z, AddsCustomTilingData *tilingPtr)
    {
        tiling = tilingPtr;
        // change the tile method from column split to row split
        xGm.SetGlobalBuffer((__gm__ float *)x + AscendC::GetBlockIdx() * tiling->tileM * tiling->n);
        zGm.SetGlobalBuffer((__gm__ float *)z + AscendC::GetBlockIdx() * tiling->tileM * tiling->n);
        // the gm address conflict happens when multi cores visit the same addr range(512Bytes)
        // we disable the L2 cache mode to highlight the influence of the gm address conflict
        xGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        zGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        pipe.InitBuffer(inQueueX, BUFFER_NUM, tiling->tileM * tiling->tileN * sizeof(float));
        pipe.InitBuffer(outQueueZ, BUFFER_NUM, tiling->tileM * tiling->tileN * sizeof(float));
    }
    __aicore__ inline void Process()
    {
        for (int32_t i = 0; i < tiling->loopOneCore; i++) {
            // the following two SyncAll in this case are unnecessary actually,
            // we just used them to highlight the influence of gm address conflict in each loop
            AscendC::SyncAll();
            CopyIn(i);
            Compute();
            AscendC::SyncAll();
            CopyOut(i);
        }
    }

private:
    __aicore__ inline void CopyIn(int32_t progress)
    {
        AscendC::LocalTensor<float> xLocal = inQueueX.AllocTensor<float>();
        AscendC::DataCopyParams params;
        params.blockCount = tiling->tileM;
        params.blockLen = tiling->tileN * sizeof(float) / BLOCK_SIZE;
        params.srcStride = (tiling->n - tiling->tileN) * sizeof(float) / BLOCK_SIZE;
        params.dstStride = 0;
        AscendC::DataCopy(xLocal, xGm[progress * tiling->tileN], params);
        inQueueX.EnQue(xLocal);
    }
    __aicore__ inline void Compute()
    {
        AscendC::LocalTensor<float> xLocal = inQueueX.DeQue<float>();
        AscendC::LocalTensor<float> zLocal = outQueueZ.AllocTensor<float>();
        constexpr float scale = 2.0;
        AscendC::Adds(zLocal, xLocal, scale, tiling->tileM * tiling->tileN);
        outQueueZ.EnQue<float>(zLocal);
        inQueueX.FreeTensor(xLocal);
    }
    __aicore__ inline void CopyOut(int32_t progress)
    {
        AscendC::LocalTensor<float> zLocal = outQueueZ.DeQue<float>();
        AscendC::DataCopyParams params;
        params.blockCount = tiling->tileM;
        params.blockLen = tiling->tileN * sizeof(float) / BLOCK_SIZE;
        params.srcStride = 0;
        params.dstStride = (tiling->n - tiling->tileN) * sizeof(float) / BLOCK_SIZE;
        AscendC::DataCopy(zGm[progress * tiling->tileN], zLocal, params);
        outQueueZ.FreeTensor(zLocal);
    }

private:
    static constexpr int32_t BUFFER_NUM = 2;
    static constexpr int32_t BLOCK_SIZE = 32;

    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> inQueueX;
    AscendC::TQue<AscendC::TPosition::VECOUT, BUFFER_NUM> outQueueZ;
    AscendC::GlobalTensor<float> xGm;
    AscendC::GlobalTensor<float> zGm;
    AddsCustomTilingData *tiling;
};
#endif // ADDS_CUSTOM_V3_H