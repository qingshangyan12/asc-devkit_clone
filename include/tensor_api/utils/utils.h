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
 * \file utils.h
 * \brief Defines utility types, enums, and parameter structures.
 * \brief Provides common types and configuration for tensor operations.
 * \brief Includes cache modes, MMAD types, and operation parameter structures.
 * 
 */
#ifndef INCLUDE_TENSOR_API_UTILS_UTILS_H
#define INCLUDE_TENSOR_API_UTILS_UTILS_H

namespace AscendC { 
namespace Te {

/*
 * @brief Cache mode enumeration for memory operations.
 * @brief Defines cache behavior for data access operations.
 */
enum class CacheMode : uint8_t {
    CACHE_MODE_NORMAL = 0,	 
    CACHE_MODE_DISABLE = 4,	    
    CACHE_MODE_LAST, 
    CACHE_MODE_PERSISTENT
};

/*
 * @brief MMAD type enumeration for matrix operations.
 * @brief Defines matrix multiply-add operation variants.
 */
enum class MmadType : uint8_t { NORMAL = 0, MX = 1};

/*
 * @brief MMAD trait structure for operation configuration.
 * @brief Defines MMAD operation traits and behavior parameters.
 * @brief Controls FM offset, K-direction alignment, C-matrix source, etc.
 */
struct MmadTrait {
    __aicore__ constexpr MmadTrait() {};

    __aicore__ constexpr MmadTrait(int32_t fmOffsetIn, bool kDirectionAlignIn, bool cmatrixSourceIn,
            bool disableGemvIn, MmadType mmadTypeIn) {
        fmOffset = fmOffsetIn;
        kDirectionAlign = kDirectionAlignIn;
        cmatrixSource = cmatrixSourceIn;
        disableGemv = disableGemvIn;
        mmadType = mmadTypeIn;
    };
    
    int32_t fmOffset = 0;
    bool kDirectionAlign = false;
    bool cmatrixSource = false;
    bool disableGemv = true;
    MmadType mmadType = MmadType::NORMAL; 
};

/*
 * @brief MMAD parameters structure for matrix dimensions.
 * @brief Defines matrix dimensions (M, N, K) for MMAD operations.
 * @brief Parameters control matrix multiply-add execution.
 * @param [in] m Matrix M dimension (rows of A matrix)
 * @param [in] n Matrix N dimension (columns of B matrix)
 * @param [in] k Matrix K dimension (columns of A / rows of B)
 * @param [in] unitFlag Unit flag for operation mode
 * @param [in] cmatrixInitVal Initial value for C matrix accumulation
 */
struct MmadParams {
    __aicore__ constexpr MmadParams() {};

    __aicore__ constexpr MmadParams(uint16_t mIn, uint16_t nIn, uint16_t kIn, uint8_t unitFlagIn, bool cmatrixInitValIn) : 
        m(mIn), n(nIn), k(kIn), unitFlag(unitFlagIn), cmatrixInitVal(cmatrixInitValIn){};
    
    uint16_t m = 0;
    uint16_t n = 0;
    uint16_t k = 0;
    uint8_t unitFlag = 0;
    bool cmatrixInitVal = false;
};

struct DataCopyTrait {};

enum class RoundMode : uint8_t {DEFAULT = 0, HYBRID};


enum DualDstMode : uint8_t {
    DUAL_DST_DISABLE = 0,
    DUAL_DST_SPLIT_M,
    DUAL_DST_SPLIT_N
};

struct FixpipeTrait {
    __aicore__ constexpr FixpipeTrait() {}

    __aicore__ constexpr FixpipeTrait(RoundMode roundModeIn, bool enableReluIn, bool enableChannelSplitIn, DualDstMode dualDstCtlIn) :
        roundMode(roundModeIn), enableRelu(enableReluIn), enableChannelSplit(enableChannelSplitIn), dualDstCtl(dualDstCtlIn) {}

    RoundMode roundMode = RoundMode::DEFAULT;
    bool enableRelu = false;
    bool enableChannelSplit = false;
    DualDstMode dualDstCtl = DUAL_DST_DISABLE;
};

struct FixpipeParams {
   __aicore__ constexpr FixpipeParams() {};

   __aicore__ constexpr FixpipeParams(uint8_t unitFlagIn) : unitFlag(unitFlagIn) {};
    
    uint8_t unitFlag = 0;
};

struct LoadDataTrait {
    __aicore__ constexpr LoadDataTrait() {}

    __aicore__ constexpr LoadDataTrait(bool transposedIn) : transposed(transposedIn) {}

    __aicore__ constexpr LoadDataTrait(const LoadDataTrait& trait, bool transposedIn) : transposed(transposedIn) {}

    bool transposed = false;
};

} // namespace Te 
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_UTILS_UTILS_H
