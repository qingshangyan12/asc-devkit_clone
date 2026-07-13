/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file l2_cache_optimizer.h
 */

#ifndef ADD_L2CACHE_L2_CACHE_OPTIMIZER_H
#define ADD_L2CACHE_L2_CACHE_OPTIMIZER_H

namespace CustomAddL2Cache {

constexpr int64_t L2_TILE_THRESHOLD = 100 * 1024 * 1024;

class L2CacheOptimizer {
public:
    __aicore__ inline L2CacheOptimizer(int64_t totalLen, int64_t blockNum) : totalLen_(totalLen), blockNum_(blockNum)
    {
        int64_t totalByteSize = totalLen * sizeof(half) * 3;
        if (totalByteSize <= L2_TILE_THRESHOLD) {
            tileNum_ = blockNum_;
            baseTileLen_ = AscendC::Ceil(totalLen_, blockNum_);
            tailTileLen_ = totalLen_ % baseTileLen_;
            if (tailTileLen_ == 0 && totalLen_ > 0) {
                tailTileLen_ = baseTileLen_;
            }
            return;
        }
        baseTileLen_ = L2_TILE_THRESHOLD / (3 * sizeof(half));
        tileNum_ = AscendC::Ceil(totalLen_, baseTileLen_);
        tailTileLen_ = totalLen_ % baseTileLen_;
        if (tailTileLen_ == 0 && totalLen_ > 0) {
            tailTileLen_ = baseTileLen_;
        }
    }

    __aicore__ inline int64_t GetTileNum() { return tileNum_; }

    __aicore__ inline int64_t GetBlockLen(int64_t tileIdx)
    {
        return (tileIdx == tileNum_ - 1) ? tailTileLen_ : baseTileLen_;
    }

    __aicore__ inline int64_t GetBlockOffset(int64_t tileIdx) { return tileIdx * baseTileLen_; }

private:
    int64_t totalLen_;
    int64_t blockNum_;
    int64_t tileNum_;
    int64_t baseTileLen_;
    int64_t tailTileLen_;
};

} // namespace CustomAddL2Cache
#endif
