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
 * \file kernel_operator_mm_check.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message("impl/basic_api/kernel_operator_mm_check.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_mm_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_CHECK_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_MM_CHECK_H
#define ASCENDC_MODULE_OPERATOR_MM_CHECK_H

#include "kernel_check.h"
#include "kernel_npu_debug.h"
#include "kernel_struct_mm.h"
#include "kernel_struct_fixpipe.h"

namespace AscendC {

template <typename T>
__aicore__ static inline bool ChannelSizeRemainder(const uint16_t channelSize, uint16_t remainder[], uint16_t size)
{
    uint16_t oneBlkNum = ONE_BLK_SIZE / sizeof(T);
    if constexpr (IsSameType<T, int4b_t>::value) {
        oneBlkNum = 64;  // 1 block = 64 int4b_t
    }
    for (uint16_t i = 0; i < size; i++) {
        if (channelSize % oneBlkNum == remainder[i]) {
            return true;
        }
    }
    return false;
}
// check fm, filter align
template <typename T, typename U, typename S>
__aicore__ static inline void CheckMmadAlign(const LocalTensor<T>& dst, const LocalTensor<U>& fm,
    const LocalTensor<S>& filter) {
    constexpr uint64_t align1024B = 1024;
    if constexpr ((IsSameType<PrimT<U>, half>::value) && (IsSameType<PrimT<S>, half>::value) &&
        (IsSameType<PrimT<T>, half>::value)) {
        CheckTensorAlign<T>(dst, VALUE_512, "dst", "Mmad");
    } else {
        CheckTensorAlign<T>(dst, align1024B, "dst", "Mmad");
    }
    CheckTensorAlign<U>(fm, VALUE_512, "fm", "Mmad");
    CheckTensorAlign<S>(filter, VALUE_512, "filter", "Mmad");
}

__aicore__ inline void CheckMmadParamsCommon(const MmadParams& mmadParams, const __gm__ char* apiName)
{
    CheckValueRange<uint16_t>(mmadParams.m, 0, UINT12_MAX, "m", apiName);
    CheckValueRange<uint16_t>(mmadParams.n, 0, UINT12_MAX, "n", apiName);
    CheckValueRange<uint16_t>(mmadParams.k, 0, UINT12_MAX, "k", apiName);
}

template <typename T, typename U, typename S>
__aicore__ inline void CheckMmadTensorCommon(const LocalTensor<T>& dst, const LocalTensor<U>& fm,
    const LocalTensor<S>& filter, const MmadParams& mmadParams, const __gm__ char* apiName)
{
    const uint64_t needElementL0c = static_cast<uint64_t>(mmadParams.m) * mmadParams.n * sizeof(PrimT<T>);
    const uint64_t needElementL0a = static_cast<uint64_t>(mmadParams.m) * mmadParams.k * sizeof(PrimT<U>);
    const uint64_t needElementL0b = static_cast<uint64_t>(mmadParams.n) * mmadParams.k * sizeof(PrimT<S>);
    constexpr uint32_t align1024B = 1024;
    const uint32_t dstAlign = (IsSameType<PrimT<T>, half>::value && IsSameType<PrimT<U>, half>::value &&
        IsSameType<PrimT<S>, half>::value) ? VALUE_512 : align1024B;

    CheckMmadParamsCommon(mmadParams, apiName);
    CheckTensorTPosition<TPosition::CO1>(dst, "dstLocal", "CO1", apiName);
    CheckTensorTPosition<TPosition::A2>(fm, "fmLocal", "A2", apiName);
    CheckTensorTPosition<TPosition::B2>(filter, "filterLocal", "B2", apiName);
    CheckTensorAlignment(dst, dstAlign, "dst", apiName);
    CheckTensorAlignment(fm, VALUE_512, "fm", apiName);
    CheckTensorAlignment(filter, VALUE_512, "filter", apiName);
    ASCENDC_DEBUG_ASSERT((needElementL0c <= TOTAL_L0C_SIZE), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check "
        "L0C capacity in %s, required size is %llu bytes, current L0C size is %u bytes.\n", apiName,
        static_cast<unsigned long long>(needElementL0c), TOTAL_L0C_SIZE));
    ASCENDC_DEBUG_ASSERT(((needElementL0a + needElementL0b) <= TOTAL_L1_SIZE), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
        "Failed to check L1 capacity in %s, required size is %llu bytes, current L1 size is %u bytes.\n", apiName,
        static_cast<unsigned long long>(needElementL0a + needElementL0b), TOTAL_L1_SIZE));
}

template <typename T, typename U, typename S, typename V>
__aicore__ inline void CheckMmadTensorCommon(const LocalTensor<T>& dst, const LocalTensor<U>& fm,
    const LocalTensor<S>& filter, const LocalTensor<V>& bias, const MmadParams& mmadParams, const __gm__ char* apiName)
{
    CheckMmadTensorCommon(dst, fm, filter, mmadParams, apiName);
    CheckTensorAlignment(bias, 128, "bias", apiName);
    CheckTensorTPosition<TPosition::CO1, TPosition::C2>(bias, "bias", "CO1 / C2", apiName);
#if __NPU_ARCH__ == 2201
    ASCENDC_DEBUG_ASSERT((SupportType<Tuple<PrimT<T>, PrimT<U>, PrimT<S>, PrimT<V>>,
        Tuple<int32_t, int8_t, int8_t, int32_t>,
        Tuple<float, half, half, float>,
        Tuple<float, float, float, float>,
        Tuple<float, bfloat16_t, bfloat16_t, float>>()),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dtype in %s, current api support dtype combination is "
        "Dst: int32_t, src0: int8_t, src1: int8_t, Bias: int32_t; Dst: float, src0: half, src1: half, Bias: float; "
        "Dst: float, src0: float, src1: float, Bias: float; Dst: float, src0: bfloat16_t, src1: bfloat16_t, "
        "Bias: float.\n", apiName));
#endif
}

__aicore__ inline void CheckFixpipeQuantPreWithWorkspaceCommon(const QuantMode_t quantPre, const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((quantPre == QuantMode_t::VDEQF16 || quantPre == QuantMode_t::VQF322B8_PRE ||
        quantPre == QuantMode_t::VREQ8), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check quantPre value in %s, "
        "when cbufWorkspace is given, supported values are VDEQF16 / VQF322B8_PRE / VREQ8.\n", apiName));
}

template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void CheckFixpipeParamsV220Common(const FixpipeParamsV220& intriParams, const __gm__ char* apiName)
{
    if (intriParams.isChannelSplit) {
        ASCENDC_DEBUG_ASSERT((intriParams.nSize >= 1 && intriParams.nSize <= UINT12_MAX &&
            intriParams.nSize % 8 == 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check nSize value in %s, "
            "when isChannelSplit is true, its valid range is 1 ~ 4095 and must be divisible by 8, current value "
            "is %u.\n", apiName, intriParams.nSize));
    } else if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        CheckValueRange<uint16_t>(intriParams.nSize, 1, UINT12_MAX, "nSize", apiName);
    } else {
        ASCENDC_DEBUG_ASSERT((intriParams.nSize >= 1 && intriParams.nSize <= UINT12_MAX &&
            intriParams.nSize % 16 == 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check nSize value in %s, "
            "when isChannelSplit is false and format is NZ, its valid range is 1 ~ 4095 and must be divisible by 16, "
            "current value is %u.\n", apiName, intriParams.nSize));
    }

    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        CheckValueRange<uint16_t>(intriParams.mSize, 1, 8192, "mSize", apiName);
    } else {
        CheckValueRange<uint16_t>(intriParams.mSize, 1, UINT16_MAX, "mSize", apiName);
    }

    ASCENDC_DEBUG_ASSERT((intriParams.dstStride != 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
        "Failed to check dstStride value in %s, its valid range is 1 ~ 4294967295, current value is %u.\n", apiName,
        intriParams.dstStride));

    if (intriParams.ndNum > 1) {
        CheckValueRange<uint16_t>(intriParams.srcNdStride, 1, VALUE_512, "srcNdStride", apiName);
        CheckValueRange<uint16_t>(intriParams.dstNdStride, 1, UINT16_MAX, "dstNdStride", apiName);
    }

    if constexpr (IsSameType<U, float>::value && SupportType<T, int8_t, uint8_t>()) {
        ASCENDC_DEBUG_ASSERT((intriParams.quantPre == QuantMode_t::QF322B8_PRE ||
            intriParams.quantPre == QuantMode_t::VQF322B8_PRE), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check quantPre value in %s, when src is float and dst is int8_t / uint8_t, supported values "
            "are QF322B8_PRE and VQF322B8_PRE.\n", apiName));
    } else if constexpr (IsSameType<U, float>::value && IsSameType<T, half>::value) {
        ASCENDC_DEBUG_ASSERT((intriParams.quantPre == QuantMode_t::F322F16), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check quantPre value in %s, when src is float and dst is half, supported value is F322F16.\n",
            apiName));
    } else if constexpr (IsSameType<U, float>::value && IsSameType<T, bfloat16_t>::value) {
        ASCENDC_DEBUG_ASSERT((intriParams.quantPre == QuantMode_t::F322BF16), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check quantPre value in %s, when src is float and dst is bfloat16_t, supported value is "
            "F322BF16.\n", apiName));
    } else if constexpr (IsSameType<U, int32_t>::value && SupportType<T, int8_t, uint8_t>()) {
        ASCENDC_DEBUG_ASSERT((intriParams.quantPre == QuantMode_t::REQ8 ||
            intriParams.quantPre == QuantMode_t::VREQ8), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check quantPre value in %s, when src is int32_t and dst is int8_t / uint8_t, supported values "
            "are REQ8 and VREQ8.\n", apiName));
    } else if constexpr (IsSameType<U, int32_t>::value && IsSameType<T, half>::value) {
        ASCENDC_DEBUG_ASSERT((intriParams.quantPre == QuantMode_t::DEQF16 ||
            intriParams.quantPre == QuantMode_t::VDEQF16), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check quantPre value in %s, when src is int32_t and dst is half, supported values are DEQF16 "
            "and VDEQF16.\n", apiName));
    }
}

template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void CheckFixpipeTensor(const LocalTensor<T>& dst, const LocalTensor<U>& src,
    const FixpipeParamsV220& intriParams, const __gm__ char* apiName)
{
    CheckFixpipeParamsV220Common<T, U, config>(intriParams, apiName);
    CheckTensorTPosition<TPosition::CO1>(src, "src", "CO1", apiName);
    const uint32_t L0C_SRC_ALIGN = 16 * sizeof(float);
    CheckTensorAlignment(src, L0C_SRC_ALIGN, "src", apiName);
    CheckTensorTPosition<TPosition::A1, TPosition::B1, TPosition::C1, TPosition::VECIN, TPosition::VECOUT,
        TPosition::VECCALC, TPosition::TSCM>(dst, "dst", "A1 / B1 / C1 / VECIN / VECOUT / VECCALC / TSCM", apiName);
}

template <typename T, typename U, const FixpipeConfig& config, typename S>
__aicore__ inline void CheckFixpipeTensor(const LocalTensor<T>& dst, const LocalTensor<U>& src,
    const LocalTensor<S>& cbufWorkspace, const FixpipeParamsV220& intriParams, const __gm__ char* apiName)
{
    CheckFixpipeTensor<T, U, config>(dst, src, intriParams, apiName);
    CheckTensorTPosition<TPosition::C1>(cbufWorkspace, "cbufWorkspace",
        "C1", apiName);
    CheckFixpipeQuantPreWithWorkspaceCommon(intriParams.quantPre, apiName);
}

template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void CheckFixpipeTensor(const GlobalTensor<T>& dst, const LocalTensor<U>& src,
    const FixpipeParamsV220& intriParams, const __gm__ char* apiName)
{
    (void)dst;
    CheckFixpipeParamsV220Common<T, U, config>(intriParams, apiName);
    CheckTensorTPosition<TPosition::CO1>(src, "src", "CO1", apiName);
}

template <typename T, typename U, const FixpipeConfig& config, typename S>
__aicore__ inline void CheckFixpipeTensor(const GlobalTensor<T>& dst, const LocalTensor<U>& src,
    const LocalTensor<S>& cbufWorkspace, const FixpipeParamsV220& intriParams, const __gm__ char* apiName)
{
    CheckFixpipeTensor<T, U, config>(dst, src, intriParams, apiName);
    CheckTensorTPosition<TPosition::C1>(cbufWorkspace, "cbufWorkspace",
        "C1", apiName);
    CheckFixpipeQuantPreWithWorkspaceCommon(intriParams.quantPre, apiName);
}

// check LoadData2D datatype
template <typename T>
__aicore__ static inline void CheckLoadData2dDatatype()
{
#if __NPU_ARCH__ == 2002
    ASCENDC_ASSERT((SupportType<PrimT<T>, uint8_t, int8_t, uint16_t, int16_t, half, int4b_t>()),
        {KERNEL_LOG(KERNEL_ERROR, "Failed to "
        "check dtype in LoadData with LoadData2DParams, current api support dtype combination is src and dst both: "
        "uint8_t / int8_t / uint16_t / int16_t / half / int4b_t.");});
#elif __NPU_ARCH__ == 2201
    ASCENDC_ASSERT((SupportType<PrimT<T>, uint8_t, int8_t, uint16_t, int16_t, half, bfloat16_t, uint32_t, int32_t,
        float, int4b_t>()),
        {KERNEL_LOG(KERNEL_ERROR, "Failed to check dtype in LoadData with LoadData2DParams, current api "
        "support dtype combination is src and dst both uint8_t / int8_t / uint16_t / int16_t / half / bfloat16_t / "
        "uint32_t / int32_t / float / int4b_t.");});
#elif __NPU_ARCH__ == 3102
    ASCENDC_ASSERT((SupportType<PrimT<T>, uint8_t, int8_t, half, uint16_t, int16_t, int4b_t>()),
        {KERNEL_LOG(KERNEL_ERROR,
        "Failed to check dtype in LoadData with LoadData2DParamsV2, current api support dtype combination is src and "
        "dst both: uint8_t / int8_t / half / uint16_t / int16_t / int4b_t.");});
#endif
}

// check LoadData3D params
__aicore__ static inline void CheckLoadData3dParams(const uint16_t srcHeight, const uint16_t srcWidth,
    const uint8_t srcWStride, const uint8_t srcHStride)
{
    ASCENDC_CHECK_VALUE_RANGE(srcHeight, MIN_LOAD3D_L1, MAX_LOAD3D_L1, "l1H", "LoadData with LoadData3DParams");
    ASCENDC_CHECK_VALUE_RANGE(srcWidth, MIN_LOAD3D_L1, MAX_LOAD3D_L1, "l1W", "LoadData with LoadData3DParams");
    ASCENDC_CHECK_VALUE_RANGE(srcWStride, MIN_LOAD3D_STRIDE, MAX_LOAD3D_STRIDE, "strideW",
        "LoadData with LoadData3DParams");
    ASCENDC_CHECK_VALUE_RANGE(srcHStride, MIN_LOAD3D_STRIDE, MAX_LOAD3D_STRIDE, "strideH",
        "LoadData with LoadData3DParams");
}

// check Load3dv2 ChannelSize
template <typename T>
__aicore__ static inline void CheckLoadData3dv2ChannelSize(const uint16_t channelSize)
{
#if __NPU_ARCH__ == 2002
    if constexpr (IsSameType<PrimT<T>, half>::value) {
        uint16_t remainderList[] = {4, 8};
        ASCENDC_ASSERT((ChannelSizeRemainder<PrimT<T>>(channelSize, remainderList, 2) || channelSize == 16),
            {KERNEL_LOG(KERNEL_ERROR, "Failed to check param channelSize value in LoadData with LoadData3DParamsV2 "
            "with dtype half, it should be: 16 or channelSize % 16 = 4 / 8, current value is %u", channelSize);});
    } else if constexpr(SupportType<PrimT<T>, int8_t, uint8_t>()) {
        uint16_t remainderList[] = {4, 8, 16};
        ASCENDC_ASSERT((ChannelSizeRemainder<PrimT<T>>(channelSize, remainderList, 3) || channelSize == 32),
            {KERNEL_LOG(KERNEL_ERROR, "Failed to check param channelSize value in LoadData with LoadData3DParamsV2 "
            "with dtype int8_t / uint8_t, it should be: 32 or channelSize % 32 = 4 / 8 / 16, current value is %u",
            channelSize);});
    } else if constexpr (IsSameType<PrimT<T>, int4b_t>::value) {
        uint16_t remainderList[] = {8, 16, 32};
        ASCENDC_ASSERT((ChannelSizeRemainder<PrimT<T>>(channelSize, remainderList, 3) || channelSize == 64),
            {KERNEL_LOG(KERNEL_ERROR, "Failed to check param channelSize value in LoadData with LoadData3DParamsV2 "
            "with dtype int4b_t, it should be: 64 or channelSize % 64 = 8 / 16 / 32, current value is %u",
            channelSize);});
    }
#elif defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) ||                     \
      (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) ||                     \
      (__NPU_ARCH__ == 3510))
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3102 || (__NPU_ARCH__ == 3003) ||  \
    (__NPU_ARCH__ == 3113))
    if constexpr (IsSameType<PrimT<T>, half>::value) {
        uint16_t remainderList[] = {0, 4, 8};
        ASCENDC_ASSERT((ChannelSizeRemainder<PrimT<T>>(channelSize, remainderList, 3)),
            {KERNEL_LOG(KERNEL_ERROR, "Failed to "
            "check param channelSize value in LoadData with LoadData3DParamsV2 with dtype half, it should be: "
            "channelSize % 16 = 0 / 4 / 8, current value is %u", channelSize);});
    }
#else
    if constexpr (SupportType<PrimT<T>, half, bfloat16_t>()) {
        uint16_t remainderList[] = {0, 4, 8};
        ASCENDC_ASSERT((ChannelSizeRemainder<PrimT<T>>(channelSize, remainderList, 3)),
            {KERNEL_LOG(KERNEL_ERROR, "Failed to "
            "check param channelSize value in LoadData with LoadData3DParamsV2 with dtype half / bfloat16_t, it should "
            "be: channelSize % 16 = 0 / 4 / 8, current value is %u", channelSize);});
    }
#endif
    if constexpr (SupportType<PrimT<T>, float, int32_t, uint32_t>()) {
        uint16_t remainderList[] = {0, 4};
        ASCENDC_ASSERT((ChannelSizeRemainder<PrimT<T>>(channelSize, remainderList, 2)),
            {KERNEL_LOG(KERNEL_ERROR, "Failed to "
            "check param channelSize value in LoadData with LoadData3DParamsV2 with dtype float / int32_t / uint32_t, "
            "it should be: channelSize % 8 = 0 / 4, current value is %u", channelSize);});
    } else if constexpr (SupportType<PrimT<T>, int8_t, uint8_t>()) {
        uint16_t remainderList[] = {0, 4, 8, 16};
        ASCENDC_ASSERT((ChannelSizeRemainder<PrimT<T>>(channelSize, remainderList, 4)),
            {KERNEL_LOG(KERNEL_ERROR, "Failed to "
            "check param channelSize value in LoadData with LoadData3DParamsV2 with dtype int8_t / uint8_t, it should "
            "be: channelSize % 32 = 0 / 4 / 8 / 16, current value is %u", channelSize);});
    } else if constexpr (IsSameType<PrimT<T>, int4b_t>::value) {
        uint16_t remainderList[] = {0, 8, 16, 32};
        ASCENDC_ASSERT((ChannelSizeRemainder<PrimT<T>>(channelSize, remainderList, 4)),
            {KERNEL_LOG(KERNEL_ERROR, "Failed to "
            "check param channelSize value in LoadData with LoadData3DParamsV2 with dtype int4b_t, it should be: "
            "channelSize % 64 = 0 / 8 / 16 / 32, current value is %u", channelSize);});
    }
#endif
}

// check LoadData3dv2 matrix params
template <typename T>
__aicore__ static inline void CheckLoadData3dv2MatrixParams(const uint16_t kExtension, const uint16_t mExtension,
    const uint16_t kStartPt, const uint16_t mStartPt) {
    constexpr uint16_t base16 = 16;
    if constexpr (SupportType<PrimT<T>, half, int8_t, int4b_t>()) {
        ASCENDC_ASSERT((mExtension % base16 == 0), { KERNEL_LOG(KERNEL_ERROR, "Failed to check mExtension value in "
            "LoadData with LoadData3DParamsV2 when dtype is half / int8_t / int4b_t, it should be divisible by 16, "
            "current value is %u", mExtension);});
    }
    uint16_t kExtBase = (SupportType<PrimT<T>, int4b_t>()) ? 64 : ONE_BLK_SIZE / sizeof(PrimT<T>);
    if constexpr (SupportType<PrimT<T>, half, int8_t, int4b_t, int32_t, uint32_t, float>()) {
        ASCENDC_ASSERT((kExtension % kExtBase == 0), { KERNEL_LOG(KERNEL_ERROR, "Failed to check kExtension value in "
            "LoadData with LoadData3DParamsV2 when dtype is half / int8_t / int4b_t / int32_t / uint32_t / float, it "
            "should be divisible by %u, current value is %u", kExtBase, kExtension);});
        ASCENDC_ASSERT((kStartPt % kExtBase == 0), { KERNEL_LOG(KERNEL_ERROR, "Failed to check kStartPt value in "
            "LoadData with LoadData3DParamsV2 when dtype is half / int8_t / int4b_t / int32_t / uint32_t / float, it "
            "should be divisible by %u, current value is %u", kExtBase, kStartPt);});
    }
#if __NPU_ARCH__ == 2002
    if constexpr (SupportType<PrimT<T>, half, int8_t, int4b_t>()) {
        ASCENDC_ASSERT((mStartPt % base16 == 0), { KERNEL_LOG(KERNEL_ERROR, "Failed to check mStartPt value in "
            "LoadData with LoadData3DParamsV2 when dtype is half / int8_t / int4b_t, it should be divisible by 16, "
            "current value is %u", mStartPt);});
    }
#elif __NPU_ARCH__ == 2201
    ASCENDC_CHECK_VALUE_RANGE(mStartPt, 0, UINT15_MAX, "mStartPt", "LoadData with LoadData3DParamsV2");
#endif
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_MM_CHECK_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_CHECK_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_CHECK_H__
#endif
