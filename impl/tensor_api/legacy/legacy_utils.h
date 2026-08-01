/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/legacy/legacy_utils.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LEGACY_UTILS_H
#endif

#ifndef IMPL_TENSOR_API_LEGACY_LEGACY_UTILS_H
#define IMPL_TENSOR_API_LEGACY_LEGACY_UTILS_H

namespace asc {
namespace te {

enum class CacheMode : uint8_t {
    CACHE_MODE_NORMAL = 0,
    CACHE_MODE_DISABLE = 4,
    CACHE_MODE_LAST = 5,
    CACHE_MODE_PERSISTENT = 6
};
enum class MmadType : uint8_t { NORMAL = 0, MX = 1 };
enum class RoundMode : uint8_t { DEFAULT = 0, HYBRID = 1 };

enum DualDstMode : uint8_t { DUAL_DST_DISABLE = 0, DUAL_DST_SPLIT_M = 1, DUAL_DST_SPLIT_N = 2 };

struct CopyL0C2GMTrait {
    __aicore__ constexpr CopyL0C2GMTrait() {}

    __aicore__ constexpr CopyL0C2GMTrait(RoundMode roundModeIn, bool enableReluIn, bool enableChannelSplitIn) :
        roundMode(roundModeIn), enableRelu(enableReluIn), enableChannelSplit(enableChannelSplitIn)
    {}

    RoundMode roundMode = RoundMode::DEFAULT;
    bool enableRelu = false;
    bool enableChannelSplit = false;
};

struct CopyL0C2UBTrait {
    __aicore__ constexpr CopyL0C2UBTrait() {}

    __aicore__ constexpr CopyL0C2UBTrait(RoundMode roundModeIn, bool enableReluIn, bool enableChannelSplitIn,
                                         DualDstMode dualDstCtlIn) :
        roundMode(roundModeIn), enableRelu(enableReluIn), enableChannelSplit(enableChannelSplitIn),
        dualDstCtl(dualDstCtlIn)
    {}

    RoundMode roundMode = RoundMode::DEFAULT;
    bool enableRelu = false;
    bool enableChannelSplit = false;
    DualDstMode dualDstCtl = DUAL_DST_DISABLE;
};

struct CopyL0C2L1Trait {
    __aicore__ constexpr CopyL0C2L1Trait() {}

    __aicore__ constexpr CopyL0C2L1Trait(RoundMode roundModeIn, bool enableReluIn, bool enableChannelSplitIn) :
        roundMode(roundModeIn), enableRelu(enableReluIn), enableChannelSplit(enableChannelSplitIn)
    {}

    RoundMode roundMode = RoundMode::DEFAULT;
    bool enableRelu = false;
    bool enableChannelSplit = false;
};

struct MmadTrait {
    __aicore__ constexpr MmadTrait(){};

    __aicore__ constexpr MmadTrait(int32_t fmOffsetIn, bool kDirectionAlignIn, bool cmatrixSourceIn, bool disableGemvIn,
                                   MmadType mmadTypeIn) :
        fmOffset(fmOffsetIn), kDirectionAlign(kDirectionAlignIn), cmatrixSource(cmatrixSourceIn),
        disableGemv(disableGemvIn), mmadType(mmadTypeIn)
    {}

    int32_t fmOffset = 0;
    bool kDirectionAlign = false;
    bool cmatrixSource = false;
    bool disableGemv = true;
    MmadType mmadType = MmadType::NORMAL;
};

struct MmadParams {
    __aicore__ constexpr MmadParams(){};

    __aicore__ constexpr MmadParams(uint16_t mIn, uint16_t nIn, uint16_t kIn, uint8_t unitFlagIn,
                                    bool cmatrixInitValIn) :
        m(mIn), n(nIn), k(kIn), unitFlag(unitFlagIn), cmatrixInitVal(cmatrixInitValIn)
    {}

    uint16_t m = 0;
    uint16_t n = 0;
    uint16_t k = 0;
    uint8_t unitFlag = 0;
    bool cmatrixInitVal = false;
};

struct FixpipeParams {
    __aicore__ constexpr FixpipeParams(){};

    __aicore__ constexpr FixpipeParams(uint8_t unitFlagIn, bool subBlockIdIn = false) :
        unitFlag(unitFlagIn), subBlockId(subBlockIdIn)
    {}

    uint8_t unitFlag = 0;
    bool subBlockId = false;
};

struct CopyGM2UBParams {
    __aicore__ constexpr CopyGM2UBParams() {}

    __aicore__ constexpr CopyGM2UBParams(uint8_t leftPaddingCountIn, uint8_t rightPaddingCountIn,
                                         bool enableConstantPadIn = true) :
        leftPaddingCount(leftPaddingCountIn), rightPaddingCount(rightPaddingCountIn),
        enableConstantPad(enableConstantPadIn)
    {}

    uint8_t leftPaddingCount = 0;
    uint8_t rightPaddingCount = 0;
    bool enableConstantPad = true;
};

template <typename T>
struct Img2ColParams {
    uint16_t mExtension = 0;
    uint16_t kExtension = 0;
    uint16_t mStartPt = 0;
    uint16_t kStartPt = 0;
    uint8_t padList[4] = {0, 0, 0, 0};
    uint8_t strideW = 1;
    uint8_t strideH = 1;
    uint8_t filterW = 1;
    uint8_t filterH = 1;
    uint8_t dilationFilterW = 1;
    uint8_t dilationFilterH = 1;
    bool filterSizeW = false;
    bool filterSizeH = false;
    bool transpose = false;
    bool fMatrixCtrl = false;
    T padValue = 0;
};

} // namespace te
} // namespace asc

namespace AscendC {
namespace Te = asc::te;
}

#endif // IMPL_TENSOR_API_LEGACY_LEGACY_UTILS_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LEGACY_UTILS_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_LEGACY_UTILS_H
#endif
