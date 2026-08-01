/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_UTILS_UTILS_H
#define IMPL_TENSOR_API_LEGACY_UTILS_UTILS_H

namespace asc {
namespace te {

using CacheMode = cache_mode;
using MmadType = mmad_type;
using RoundMode = round_mode;
using DualDstMode = dual_dst_mode;

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

#endif // IMPL_TENSOR_API_LEGACY_UTILS_UTILS_H
