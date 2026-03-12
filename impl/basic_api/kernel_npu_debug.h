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
 * \file kernel_npu_debug.h
 * \brief
 */
#ifndef ASCENDC_MODULE_OPERATOR_KERNEL_NPU_DEBUG_H
#define ASCENDC_MODULE_OPERATOR_KERNEL_NPU_DEBUG_H

#include "kernel_log.h"
#include "utils/kernel_utils_ceil_oom_que.h"
#include "utils/kernel_utils_constants.h"
#include "kernel_tensor.h"

namespace AscendC {
template <typename T>
struct NamedTensor {
    __aicore__ NamedTensor(const LocalTensor<T>& tensorIn, const __gm__ char* tensorNameIn)
        : tensor(tensorIn), tensorName(tensorNameIn) {}

    LocalTensor<T> tensor;
    const __gm__ char* tensorName;
};

__aicore__ inline __gm__ const char* GetTPositionName(TPosition pos)
{
    switch (pos) {
        case TPosition::GM:
            return "GM";
        case TPosition::A1:
            return "A1";
        case TPosition::B1:
            return "B1";
        case TPosition::C1:
            return "C1";
        case TPosition::A2:
            return "A2";
        case TPosition::B2:
            return "B2";
        case TPosition::C2:
            return "C2";
        case TPosition::CO1:
            return "CO1";
        case TPosition::CO2:
            return "CO2";
        case TPosition::VECIN:
            return "VECIN";
        case TPosition::VECOUT:
            return "VECOUT";
        case TPosition::VECCALC:
            return "VECCALC / LCM";
        case TPosition::SPM:
            return "SPM / SHM";
        case TPosition::TSCM:
            return "TSCM";
        case TPosition::C2PIPE2GM:
            return "C2PIPE2GM";
        case TPosition::C2PIPE2LOCAL:
            return "C2PIPE2LOCAL";
        case TPosition::MAX:
            return "MAX";
        default:
            return "UNKNOWN";
    }
}

__aicore__ inline void ReportNotSupport(bool isSupported, const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((isSupported), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "%s is not supported on current device.\n",
        apiName));
}

template <TPosition... expectedPos, typename T>
__aicore__ inline void CheckTensorTPosition(const LocalTensor<T>& inputTensor, const __gm__ char* tensorName,
    const __gm__ char* posName, const __gm__ char* apiName)
{
    TPosition currentPos = static_cast<TPosition>(inputTensor.GetPosition());
    bool tPosValid = ((currentPos == expectedPos) || ...);
    ASCENDC_DEBUG_ASSERT((tPosValid), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check %s tensor TPosition in %s, "
        "supported TPositions are %s, current TPosition is %s.\n", tensorName, apiName, posName,
        GetTPositionName(currentPos)));
}

template <typename T>
__aicore__ inline void CheckTensorAlignment(const LocalTensor<T>& inputTensor, uint32_t alignBytes,
    const __gm__ char* tensorName,  const __gm__ char* apiName)
{
    uint64_t tensorAddr = reinterpret_cast<uint64_t>(inputTensor.GetPhyAddr());
#ifdef ASCENDC_CPU_DEBUG
    auto positionHardMap = ConstDefiner::Instance().positionHardMap;
    TPosition tensorPos = static_cast<TPosition>(inputTensor.GetPosition());
    tensorAddr = tensorAddr -
        reinterpret_cast<uint64_t>(ConstDefiner::Instance().hardwareCpuBufferMap.at(positionHardMap.at(tensorPos)));
#endif
    ASCENDC_DEBUG_ASSERT((tensorAddr % alignBytes == 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check %s start "
        "address alignment in %s, its start address must align with %u bytes, current start address is %llu.\n",
        tensorName, apiName, alignBytes, static_cast<unsigned long long>(tensorAddr)));
}

template <typename T>
__aicore__ inline void CheckValueRange(T value, T valueLow, T valueHigh, const __gm__ char* paramName,
    const __gm__ char* apiName)
{
    if constexpr (SupportType<T, int8_t, int16_t, int32_t, int64_t>()) {
        int64_t castValue = static_cast<int64_t>(value);
        int64_t castLow = static_cast<int64_t>(valueLow);
        int64_t castHigh = static_cast<int64_t>(valueHigh);
        ASCENDC_DEBUG_ASSERT((value <= valueHigh && value >= valueLow), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
            "check %s value in %s, its valid range is [%lld, %lld], current value is %lld.\n", paramName, apiName,
            castLow, castHigh, castValue));
    } else if constexpr (SupportType<T, uint8_t, uint16_t, uint32_t, uint64_t>()) {
        uint64_t castValue = static_cast<uint64_t>(value);
        uint64_t castLow = static_cast<uint64_t>(valueLow);
        uint64_t castHigh = static_cast<uint64_t>(valueHigh);
        ASCENDC_DEBUG_ASSERT((value <= valueHigh && value >= valueLow), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
            "check %s value in %s, its valid range is [%llu, %llu], current value is %llu.\n", paramName, apiName,
            castLow, castHigh, castValue));
    } else {
        ASCENDC_DEBUG_ASSERT((false), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Current dtype is not supported for check "
            "function CheckValueRange.\n"));
    }
}

template <typename T>
__aicore__ inline void CheckMaskArray(const uint64_t mask[], const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((mask != nullptr), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check mask[] in %s, "
        "it is nullptr.\n", apiName));
    if constexpr (sizeof(T) == 2) {
        ASCENDC_DEBUG_ASSERT(!(mask[0] == 0 && mask[1] == 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check "
            "mask[] value in %s, when sizeof(T) == 2, mask[0] and mask[1] cannot be both 0.\n", apiName));
    } else if constexpr (sizeof(T) == 4) {
        ASCENDC_DEBUG_ASSERT((mask[0] != 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check mask[] value in %s, "
            "when sizeof(T) == 4, mask[0] cannot be 0.\n", apiName));
    } else if constexpr (sizeof(T) == 8) {
        ASCENDC_DEBUG_ASSERT((mask[0] != 0 && mask[0] <= UINT32_MAX), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
            "check mask[] value in %s, when sizeof(T) == 8, mask[0] should be in range (0, UINT32_MAX], current value "
            "is %llu.\n", apiName, mask[0]));
    }
}

template <typename T>
__aicore__ inline void CheckMaskValue(const uint64_t mask, const __gm__ char* apiName)
{
    if constexpr (sizeof(T) == 2) {
        CheckValueRange<uint64_t>(mask, 1, ONE_REPEAT_BYTE_SIZE / sizeof(T), "mask when sizeof(T) == 2" , apiName);
    } else if constexpr (sizeof(T) == 4) {
        CheckValueRange<uint64_t>(mask, 1, ONE_REPEAT_BYTE_SIZE / sizeof(T), "mask when sizeof(T) == 4" , apiName);
    } else if constexpr (sizeof(T) == 8) {
        CheckValueRange<uint64_t>(mask, 1, ONE_REPEAT_BYTE_SIZE / sizeof(T), "mask when sizeof(T) == 8" , apiName);
    }
}

template <typename... Ts>
__aicore__ inline void CheckVectorTensor(const __gm__ char* apiName, const Ts& ...tensors)
{
    (CheckTensorTPosition<TPosition::VECIN, TPosition::VECCALC, TPosition::VECOUT>(tensors.tensor, tensors.tensorName,
        "VECIN / VECCALC / VECOUT", apiName), ...);
    (CheckTensorAlignment(tensors.tensor, ONE_BLK_SIZE, tensors.tensorName, apiName), ...);
}


/* **************************************************************************************************
 * DataCopy                                             *
 * ************************************************************************************************* */
__aicore__ inline void CheckDataCopyParamsCommon(const DataCopyParams& intriParams, const __gm__ char* apiName)
{
    CheckValueRange<uint16_t>(intriParams.blockCount, 1, UINT12_MAX, "blockCount", apiName);
    CheckValueRange<uint16_t>(intriParams.blockLen, 1, UINT16_MAX, "blockLen", apiName);
}

template <typename T>
__aicore__ inline void CheckDataCopyCountCommon(const uint32_t count, const __gm__ char* apiName)
{
    using PrimType = PrimT<T>;
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5102)
    if constexpr(Std::is_same<PrimType, int2b_t>::value) {
        ASCENDC_DEBUG_ASSERT((count % ConstantsInternal::ONE_BLK_B2_NUM == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check count value in %s, count * sizeof(T) must be 32B "
            "align, current count value is %u.\n", apiName, count));
    } else if constexpr(Std::is_same<PrimType, uint1b_t>::value) {
        ASCENDC_DEBUG_ASSERT((count % ConstantsInternal::ONE_BLK_B1_NUM == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check count value in %s, count * sizeof(T) must be 32B "
            "align, current count value is %u.\n", apiName, count));
    } else if constexpr (Std::is_same<PrimType, fp4x2_e2m1_t>::value ||
        Std::is_same<PrimType, fp4x2_e1m2_t>::value || Std::is_same<PrimType, int4b_t>::value) {
        ASCENDC_DEBUG_ASSERT((count % ConstantsInternal::ONE_BLK_FP4_NUM == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check count value in %s, count * sizeof(T) must be 32B "
            "align, current count value is %u.\n", apiName, count));
    } else
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    if constexpr (Std::is_same<PrimType, fp4x2_e2m1_t>::value || Std::is_same<PrimType, fp4x2_e1m2_t>::value) {
        ASCENDC_DEBUG_ASSERT((count % ConstantsInternal::ONE_BLK_FP4_NUM == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check count value in %s, count * sizeof(T) must be 32B "
            "align, current count value is %u.\n", apiName, count));
    } else
#endif
    {
        ASCENDC_DEBUG_ASSERT((count % AscendCUtils::GetC0Count(sizeof(PrimType)) == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check count value in %s, count * sizeof(T) must be 32B "
            "align, current count value is %u.\n", apiName, count));
    }
}

__aicore__ inline void GetDataCopyUnitsByRoute(const Hardware srcHwPos, const Hardware dstHwPos,
    uint16_t& srcBurstUnit, uint16_t& dstBurstUnit, uint16_t& srcStrideUnit, uint16_t& dstStrideUnit)
{
    srcBurstUnit = ONE_BLK_SIZE;
    dstBurstUnit = ONE_BLK_SIZE;
    srcStrideUnit = ONE_BLK_SIZE;
    dstStrideUnit = ONE_BLK_SIZE;

    if (srcHwPos == Hardware::L1 && dstHwPos == Hardware::BIAS) {
        dstBurstUnit = 64;
        srcStrideUnit = ONE_BLK_SIZE;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
        srcBurstUnit = ONE_BLK_SIZE;
        dstStrideUnit = ONE_BLK_SIZE;
#else
        srcBurstUnit = 64;
        dstStrideUnit = 64;
#endif
    } else if (srcHwPos == Hardware::L1 && dstHwPos == Hardware::FIXBUF) {
        dstBurstUnit = 128;
        srcStrideUnit = ONE_BLK_SIZE;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
        srcBurstUnit = 64;
        dstStrideUnit = 64;
#else
        srcBurstUnit = 128;
        dstStrideUnit = 128;
#endif
    }
}

template <typename SrcT, typename DstT>
__aicore__ inline void CalculateDataCopyRequiredBytes(const DataCopyParams& intriParams, const Hardware srcHwPos,
    const Hardware dstHwPos, uint64_t& srcRequiredBytes, uint64_t& dstRequiredBytes)
{
    uint16_t srcBurstUnit = ONE_BLK_SIZE;
    uint16_t dstBurstUnit = ONE_BLK_SIZE;
    uint16_t srcStrideUnit = ONE_BLK_SIZE;
    uint16_t dstStrideUnit = ONE_BLK_SIZE;
    GetDataCopyUnitsByRoute(srcHwPos, dstHwPos, srcBurstUnit, dstBurstUnit, srcStrideUnit, dstStrideUnit);
    if (srcHwPos == Hardware::L1 && dstHwPos == Hardware::BIAS && sizeof(SrcT) != sizeof(DstT)) {
        dstBurstUnit = static_cast<uint16_t>(dstBurstUnit * sizeof(DstT) / sizeof(SrcT));
    }

    srcRequiredBytes = static_cast<uint64_t>(intriParams.blockCount) * intriParams.blockLen * srcBurstUnit +
        static_cast<uint64_t>(intriParams.blockCount - 1) * intriParams.srcStride * srcStrideUnit;
    dstRequiredBytes = static_cast<uint64_t>(intriParams.blockCount) * intriParams.blockLen * dstBurstUnit +
        static_cast<uint64_t>(intriParams.blockCount - 1) * intriParams.dstStride * dstStrideUnit;
}

template <typename T>
__aicore__ inline void CheckDataCopyTensor(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
    const DataCopyParams& intriParams, const __gm__ char* apiName)
{
    (void)src;
    using PrimType = PrimT<T>;
    const TPosition dstPos = static_cast<TPosition>(dst.GetPosition());
    const Hardware dstHwPos = GetPhyType(dstPos);
    const uint64_t dstTensorSizeBytes = static_cast<uint64_t>(dst.GetSize()) * sizeof(PrimType);
    uint64_t srcRequiredBytes = 0;
    uint64_t dstRequiredBytes = 0;
    CalculateDataCopyRequiredBytes<PrimType, PrimType>(intriParams, Hardware::GM, dstHwPos, srcRequiredBytes,
        dstRequiredBytes);

    CheckDataCopyParamsCommon(intriParams, apiName);
    ASCENDC_DEBUG_ASSERT((dstHwPos == Hardware::UB || dstHwPos == Hardware::L1),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dst tensor route in %s, supported routes are GM -> UB / "
        "L1, current dst TPosition is %s.\n", apiName, GetTPositionName(dstPos)));
    CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", apiName);
    ASCENDC_DEBUG_ASSERT((dstRequiredBytes <= dstTensorSizeBytes), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
        "check dstLocal size in %s, tensor size needs to be at least %llu bytes, while current tensor size is only "
        "%llu bytes.\n", apiName, static_cast<unsigned long long>(dstRequiredBytes),
        static_cast<unsigned long long>(dstTensorSizeBytes)));
}

template <typename T>
__aicore__ inline void CheckDataCopyTensor(const GlobalTensor<T>& dst, const LocalTensor<T>& src,
    const DataCopyParams& intriParams, const __gm__ char* apiName)
{
    (void)dst;
    using PrimType = PrimT<T>;
    const uint64_t srcTensorSizeBytes = static_cast<uint64_t>(src.GetSize()) * sizeof(PrimType);
    uint64_t srcRequiredBytes = 0;
    uint64_t dstRequiredBytes = 0;
    CheckDataCopyParamsCommon(intriParams, apiName);
    CheckTensorAlignment(src, ONE_BLK_SIZE, "src", apiName);
    CalculateDataCopyRequiredBytes<PrimType, PrimType>(intriParams, GetPhyType(static_cast<TPosition>(src.GetPosition())),
        Hardware::GM, srcRequiredBytes, dstRequiredBytes);

    ASCENDC_DEBUG_ASSERT((srcRequiredBytes <= srcTensorSizeBytes), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
        "check srcLocal size in %s, tensor size needs to be at least %llu bytes, while current tensor size is only "
        "%llu bytes.\n", apiName, static_cast<unsigned long long>(srcRequiredBytes),
        static_cast<unsigned long long>(srcTensorSizeBytes)));
}

template <typename T>
__aicore__ inline void CheckDataCopyTensor(const LocalTensor<T>& dst, const LocalTensor<T>& src,
    const DataCopyParams& intriParams, const __gm__ char* apiName)
{
    using PrimType = PrimT<T>;
    const TPosition dstPos = static_cast<TPosition>(dst.GetPosition());
    const TPosition srcPos = static_cast<TPosition>(src.GetPosition());
    const Hardware dstHwPos = GetPhyType(dstPos);
    const Hardware srcHwPos = GetPhyType(srcPos);
    const uint64_t srcTensorSizeBytes = static_cast<uint64_t>(src.GetSize()) * sizeof(PrimType);
    const uint64_t dstTensorSizeBytes = static_cast<uint64_t>(dst.GetSize()) * sizeof(PrimType);
    uint64_t srcRequiredBytes = 0;
    uint64_t dstRequiredBytes = 0;

    CheckDataCopyParamsCommon(intriParams, apiName);
    if (srcHwPos == Hardware::UB) {
        ASCENDC_DEBUG_ASSERT((dstHwPos == Hardware::UB || dstHwPos == Hardware::L1),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dst tensor route in %s, supported routes are UB -> UB "
            "/ L1, current dst TPosition is %s.\n", apiName, GetTPositionName(dstPos)));
        CheckTensorAlignment(src, ONE_BLK_SIZE, "src", apiName);
        CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", apiName);
    } else if (srcHwPos == Hardware::L1) {
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3510) || \
    (__NPU_ARCH__ == 5102)
        const bool isValidDst = (dstHwPos == Hardware::UB || dstHwPos == Hardware::BIAS || dstHwPos == Hardware::FIXBUF);
#else
        const bool isValidDst = (dstHwPos == Hardware::UB || dstHwPos == Hardware::BIAS);
#endif
        ASCENDC_DEBUG_ASSERT((isValidDst), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dst tensor route in "
            "%s, current dst TPosition is %s.\n", apiName, GetTPositionName(dstPos)));
        CheckTensorAlignment(src, ONE_BLK_SIZE, "src", apiName);
        if (dstHwPos == Hardware::BIAS) {
            CheckTensorAlignment(dst, 64, "dst", apiName);
        }
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3510) || \
    (__NPU_ARCH__ == 5102)
        else if (dstHwPos == Hardware::FIXBUF) {
            CheckTensorAlignment(dst, 128, "dst", apiName);
        }
#endif
        else {
            CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", apiName);
        }
    } else {
        ASCENDC_DEBUG_ASSERT((false), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check src tensor route in %s, "
            "supported src TPositions are VECIN / VECCALC / VECOUT / A1 / B1 / C1 / TSCM, current src TPosition is "
            "%s.\n", apiName, GetTPositionName(srcPos)));
    }
    CalculateDataCopyRequiredBytes<PrimType, PrimType>(intriParams, srcHwPos, dstHwPos, srcRequiredBytes,
        dstRequiredBytes);
    ASCENDC_DEBUG_ASSERT((srcRequiredBytes <= srcTensorSizeBytes), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
        "check srcLocal size in %s, tensor size needs to be at least %llu bytes, while current tensor size is only "
        "%llu bytes.\n", apiName, static_cast<unsigned long long>(srcRequiredBytes),
        static_cast<unsigned long long>(srcTensorSizeBytes)));
    ASCENDC_DEBUG_ASSERT((dstRequiredBytes <= dstTensorSizeBytes), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
        "check dstLocal size in %s, tensor size needs to be at least %llu bytes, while current tensor size is only "
        "%llu bytes.\n", apiName, static_cast<unsigned long long>(dstRequiredBytes),
        static_cast<unsigned long long>(dstTensorSizeBytes)));
}

template <typename T, typename U>
__aicore__ inline void CheckDataCopyTensor(const LocalTensor<T>& dst, const LocalTensor<U>& src,
    const DataCopyParams& intriParams, const __gm__ char* apiName)
{
    using PrimDstType = PrimT<T>;
    using PrimSrcType = PrimT<U>;
    const TPosition dstPos = static_cast<TPosition>(dst.GetPosition());
    const TPosition srcPos = static_cast<TPosition>(src.GetPosition());
    const Hardware dstHwPos = GetPhyType(dstPos);
    const Hardware srcHwPos = GetPhyType(srcPos);
    const uint64_t srcTensorSizeBytes = static_cast<uint64_t>(src.GetSize()) * sizeof(PrimSrcType);
    const uint64_t dstTensorSizeBytes = static_cast<uint64_t>(dst.GetSize()) * sizeof(PrimDstType);
    uint64_t srcRequiredBytes = 0;
    uint64_t dstRequiredBytes = 0;

    CheckDataCopyParamsCommon(intriParams, apiName);
    ASCENDC_DEBUG_ASSERT((srcHwPos == Hardware::L1), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check src tensor "
        "route in %s, supported route is L1 -> BIAS, current src TPosition is %s.\n", apiName,
        GetTPositionName(srcPos)));
    ASCENDC_DEBUG_ASSERT((dstHwPos == Hardware::BIAS), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dst tensor "
        "route in %s, supported route is L1 -> BIAS, current dst TPosition is %s.\n", apiName,
        GetTPositionName(dstPos)));
    CheckTensorAlignment(src, ONE_BLK_SIZE, "src", apiName);
    CheckTensorAlignment(dst, 64, "dst", apiName);
    CalculateDataCopyRequiredBytes<PrimSrcType, PrimDstType>(intriParams, srcHwPos, dstHwPos, srcRequiredBytes,
        dstRequiredBytes);
    ASCENDC_DEBUG_ASSERT((srcRequiredBytes <= srcTensorSizeBytes), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
        "check srcLocal size in %s, tensor size needs to be at least %llu bytes, while current tensor size is only "
        "%llu bytes.\n", apiName, static_cast<unsigned long long>(srcRequiredBytes),
        static_cast<unsigned long long>(srcTensorSizeBytes)));
    ASCENDC_DEBUG_ASSERT((dstRequiredBytes <= dstTensorSizeBytes), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
        "check dstLocal size in %s, tensor size needs to be at least %llu bytes, while current tensor size is only "
        "%llu bytes.\n", apiName, static_cast<unsigned long long>(dstRequiredBytes),
        static_cast<unsigned long long>(dstTensorSizeBytes)));
}

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_KERNEL_NPU_DEBUG_H
