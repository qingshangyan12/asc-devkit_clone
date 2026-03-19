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
 * \file kernel_operator_data_copy_check.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message("impl/basic_api/kernel_operator_data_copy_check.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_data_copy_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_CHECK_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_DATA_COPY_CHECK_H
#define ASCENDC_MODULE_OPERATOR_DATA_COPY_CHECK_H

#include "kernel_npu_debug.h"

namespace AscendC {
/* **************************************************************************************************
 * DataCopy                                             *
 * ************************************************************************************************* */
__aicore__ inline void CheckDataCopyParamsCommon(const DataCopyParams& intriParams, const __gm__ char* apiName)
{
    CheckValueRange<uint16_t>(intriParams.blockCount, 1, UINT12_MAX, "blockCount", apiName); CheckValueRange<uint16_t>(
        intriParams.blockLen, 1, UINT16_MAX, "blockLen", apiName);
}

template <typename T>
__aicore__ inline void CheckDataCopyCountCommon(const uint32_t count, const __gm__ char* apiName)
{
    using PrimType = PrimT<T>;
    ASCENDC_DEBUG_ASSERT((count % AscendCUtils::GetC0Count(sizeof(PrimType)) == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check count value in %s, count * sizeof(T) must be 32B "
        "align, current count value is %u.\n", apiName, count));
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

__aicore__ inline void CheckRequiredBytes(const uint64_t requiredBytes, const uint64_t tensorSizeBytes,
    const __gm__ char* tensorName, const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((requiredBytes <= tensorSizeBytes), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check %s "
        "size in %s, tensor size needs to be at least %llu bytes, while current tensor size is only %llu bytes.\n", tensorName,
        apiName, static_cast<unsigned long long>(requiredBytes), static_cast<unsigned long long>(tensorSizeBytes)));
}

__aicore__ inline void CheckDataCopyLocalRoute(const Hardware srcHwPos, const Hardware dstHwPos, const TPosition srcPos,
    const TPosition dstPos, const __gm__ char* apiName)
{
    if (srcHwPos == Hardware::UB) {
        ASCENDC_DEBUG_ASSERT((dstHwPos == Hardware::UB || dstHwPos == Hardware::L1),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dst tensor route in %s, supported routes are UB -> UB "
            "/ L1, current dst TPosition is %s.\n", apiName, GetTPositionName(dstPos)));
        return;
    }
    if (srcHwPos == Hardware::L1) {
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3510) || \
    (__NPU_ARCH__ == 5102)
        const bool isValidDst = (dstHwPos == Hardware::UB || dstHwPos == Hardware::BIAS || dstHwPos == Hardware::FIXBUF);
#else
        const bool isValidDst = (dstHwPos == Hardware::UB || dstHwPos == Hardware::BIAS);
#endif
        ASCENDC_DEBUG_ASSERT((isValidDst), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dst tensor route in "
            "%s, current dst TPosition is %s.\n", apiName, GetTPositionName(dstPos)));
        return;
    }
    ASCENDC_DEBUG_ASSERT((false), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check src tensor route in %s, "
        "supported src TPositions are VECIN / VECCALC / VECOUT / A1 / B1 / C1 / TSCM, current src TPosition is "
        "%s.\n", apiName, GetTPositionName(srcPos)));
}

template <typename T>
__aicore__ inline void CheckDataCopyLocalAlignment(const LocalTensor<T>& dst, const LocalTensor<T>& src,
    const Hardware srcHwPos, const Hardware dstHwPos, const __gm__ char* apiName)
{
    CheckTensorAlignment(src, ONE_BLK_SIZE, "src", apiName);
    if (srcHwPos != Hardware::L1) {
        CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", apiName);
        return;
    }
    if (dstHwPos == Hardware::BIAS) {
        CheckTensorAlignment(dst, 64, "dst", apiName);
        return;
    }
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3510) || \
    (__NPU_ARCH__ == 5102)
    if (dstHwPos == Hardware::FIXBUF) {
        CheckTensorAlignment(dst, 128, "dst", apiName);
        return;
    }
#endif
    CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", apiName);
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
    CheckDataCopyLocalRoute(srcHwPos, dstHwPos, srcPos, dstPos, apiName);
    CheckDataCopyLocalAlignment(dst, src, srcHwPos, dstHwPos, apiName);
    CalculateDataCopyRequiredBytes<PrimType, PrimType>(intriParams, srcHwPos, dstHwPos, srcRequiredBytes,
        dstRequiredBytes);
    CheckRequiredBytes(srcRequiredBytes, srcTensorSizeBytes, "srcLocal", apiName);
    CheckRequiredBytes(dstRequiredBytes, dstTensorSizeBytes, "dstLocal", apiName);
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
    CheckRequiredBytes(srcRequiredBytes, srcTensorSizeBytes, "srcLocal", apiName);
    CheckRequiredBytes(dstRequiredBytes, dstTensorSizeBytes, "dstLocal", apiName);
}

__aicore__ inline void CheckNd2NzParamsCommon(const Nd2NzParams& intriParams, const __gm__ char* apiName)
{
    CheckValueRange<decltype(intriParams.ndNum)>(intriParams.ndNum, 0, UINT12_MAX, "ndNum", apiName);
    CheckValueRange<decltype(intriParams.nValue)>(intriParams.nValue, 0, 16384, "nValue", apiName); CheckValueRange<
        decltype(intriParams.dstNzC0Stride)>(intriParams.dstNzC0Stride, 0, 16384, "dstNzC0Stride", apiName);
    CheckValueRange<decltype(intriParams.dstNzNStride)>(intriParams.dstNzNStride, 0, 16384, "dstNzNStride", apiName);
}

__aicore__ inline void CheckNz2NdParamsCommon(const Nz2NdParamsFull& intriParams, const __gm__ char* apiName)
{
    constexpr uint16_t nz2NdLimit = 8192;
    CheckValueRange<decltype(intriParams.ndNum)>(intriParams.ndNum, 0, UINT12_MAX, "ndNum", apiName); CheckValueRange<
        decltype(intriParams.nValue)>(intriParams.nValue, 1, nz2NdLimit, "nValue", apiName);
    CheckValueRange<decltype(intriParams.dValue)>(intriParams.dValue, 1, nz2NdLimit, "dValue", apiName); CheckValueRange<
        decltype(intriParams.srcNdMatrixStride)>(intriParams.srcNdMatrixStride, 1, VALUE_512, "srcNdMatrixStride", apiName);
    CheckValueRange<decltype(intriParams.srcNStride)>(intriParams.srcNStride, 0, UINT12_MAX, "srcNStride", apiName);
}

template <typename T>
__aicore__ inline uint64_t CalculateNd2NzDstRequiredBytes(const Nd2NzParams& intriParams)
{
    if (intriParams.ndNum == 0 || intriParams.nValue == 0 || intriParams.dValue == 0) { return 0; }
    return static_cast<uint64_t>(intriParams.ndNum - 1) * intriParams.dstNzMatrixStride * sizeof(PrimT<T>) +
        static_cast<uint64_t>(intriParams.nValue - 1) * intriParams.dstNzNStride * DEFAULT_C0_SIZE +
        static_cast<uint64_t>(DivCeil(intriParams.dValue * sizeof(PrimT<T>), DEFAULT_C0_SIZE) - 1) *
        intriParams.dstNzC0Stride * DEFAULT_C0_SIZE + DEFAULT_C0_SIZE;
}

template <typename T>
__aicore__ inline uint64_t CalculateNd2NzSrcRequiredBytes(const Nd2NzParams& intriParams)
{
    if (intriParams.ndNum == 0 || intriParams.nValue == 0 || intriParams.dValue == 0) { return 0; }
    return (static_cast<uint64_t>(intriParams.ndNum - 1) * intriParams.srcNdMatrixStride +
        static_cast<uint64_t>(intriParams.nValue - 1) * intriParams.srcDValue + intriParams.dValue) *
        sizeof(PrimT<T>);
}

template <typename T>
__aicore__ inline uint64_t CalculateNz2NdSrcRequiredBytes(const Nz2NdParamsFull& intriParams)
{
    if (intriParams.ndNum == 0 || intriParams.nValue == 0 || intriParams.dValue == 0) { return 0; }
    const uint64_t nzWidth = 16;
    const uint64_t srcMatrixStrideUnit = 256;
    return static_cast<uint64_t>(intriParams.ndNum - 1) * intriParams.srcNdMatrixStride * srcMatrixStrideUnit *
        sizeof(PrimT<T>) + static_cast<uint64_t>(intriParams.dValue / nzWidth - 1) * intriParams.srcNStride *
        nzWidth * sizeof(PrimT<T>) + nzWidth * intriParams.nValue * sizeof(PrimT<T>);
}

template <typename T>
__aicore__ inline void CheckNd2NzRoute(const Hardware srcHwPos, const Hardware dstHwPos, const TPosition srcPos,
    const TPosition dstPos, const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((srcHwPos == Hardware::UB), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check src tensor "
        "position in %s, supported positions are VECIN / VECCALC / VECOUT, current position is %s.\n", apiName,
        GetTPositionName(srcPos)));
    ASCENDC_DEBUG_ASSERT((dstHwPos == Hardware::L1), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dst tensor "
        "position in %s, supported positions are TSCM, current position is %s.\n", apiName,
        GetTPositionName(dstPos)));
}

template <typename T>
__aicore__ inline void CheckNd2NzFormat(const Nd2NzParams& intriParams, const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((intriParams.ndNum == 1), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
        "Failed to check ndNum value in %s, current value is %llu, valid value is 1.\n", apiName,
        static_cast<unsigned long long>(intriParams.ndNum)));
    ASCENDC_DEBUG_ASSERT((intriParams.dValue * sizeof(PrimT<T>) % ONE_BLK_SIZE == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dValue in %s, dValue * sizeof(T) must be 32B aligned.\n",
        apiName));
    ASCENDC_DEBUG_ASSERT((intriParams.srcDValue * sizeof(PrimT<T>) % ONE_BLK_SIZE == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR,
        "Failed to check srcDValue in %s, srcDValue * sizeof(T) must be 32B aligned.\n", apiName));
}

__aicore__ inline void CheckUbToL1Route(const Hardware srcHwPos, const Hardware dstHwPos, const TPosition srcPos,
    const TPosition dstPos, const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((srcHwPos == Hardware::UB), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check src tensor "
        "route in %s, supported route is UB -> L1, current src TPosition is %s.\n", apiName,
        GetTPositionName(srcPos)));
    ASCENDC_DEBUG_ASSERT((dstHwPos == Hardware::L1), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dst tensor "
        "route in %s, supported route is UB -> L1, current dst TPosition is %s.\n", apiName,
        GetTPositionName(dstPos)));
}

template <typename T>
__aicore__ inline void CheckDataCopyTensor(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
    const Nd2NzParams& intriParams, const __gm__ char* apiName)
{
    (void)src;
    const TPosition dstPos = static_cast<TPosition>(dst.GetPosition());
    const Hardware dstHwPos = GetPhyType(dstPos);
    const uint64_t dstTensorSizeBytes = static_cast<uint64_t>(dst.GetSize()) * sizeof(PrimT<T>);
    const uint64_t dstRequiredBytes = CalculateNd2NzDstRequiredBytes<T>(intriParams);

    CheckNd2NzParamsCommon(intriParams, apiName);
    ASCENDC_DEBUG_ASSERT((dstHwPos == Hardware::L1 || dstHwPos == Hardware::UB),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dst tensor position in %s, supported positions are "
        "A1 / B1 / VECIN, current position is %s.\n", apiName, GetTPositionName(dstPos)));
    CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", apiName);
    CheckRequiredBytes(dstRequiredBytes, dstTensorSizeBytes, "dstLocal", apiName);
}

template <typename T>
__aicore__ inline void CheckDataCopyTensor(const LocalTensor<T>& dst, const LocalTensor<T>& src,
    const Nd2NzParams& intriParams, const __gm__ char* apiName)
{
    const TPosition dstPos = static_cast<TPosition>(dst.GetPosition());
    const TPosition srcPos = static_cast<TPosition>(src.GetPosition());
    const Hardware dstHwPos = GetPhyType(dstPos);
    const Hardware srcHwPos = GetPhyType(srcPos);
    const uint64_t srcTensorSizeBytes = static_cast<uint64_t>(src.GetSize()) * sizeof(PrimT<T>);
    const uint64_t dstTensorSizeBytes = static_cast<uint64_t>(dst.GetSize()) * sizeof(PrimT<T>);
    const uint64_t srcRequiredBytes = CalculateNd2NzSrcRequiredBytes<T>(intriParams);
    const uint64_t dstRequiredBytes = CalculateNd2NzDstRequiredBytes<T>(intriParams);

    CheckNd2NzParamsCommon(intriParams, apiName);
    CheckNd2NzRoute<T>(srcHwPos, dstHwPos, srcPos, dstPos, apiName);
    CheckTensorAlignment(src, ONE_BLK_SIZE, "src", apiName);
    CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", apiName);
    CheckNd2NzFormat<T>(intriParams, apiName);
    CheckRequiredBytes(srcRequiredBytes, srcTensorSizeBytes, "srcLocal", apiName);
    CheckRequiredBytes(dstRequiredBytes, dstTensorSizeBytes, "dstLocal", apiName);
}

template <typename T>
__aicore__ inline void CheckDataCopyTensor(const GlobalTensor<T>& dst, const LocalTensor<T>& src,
    const Nz2NdParamsFull& intriParams, const __gm__ char* apiName)
{
    (void)dst;
    const TPosition srcPos = static_cast<TPosition>(src.GetPosition());
    const Hardware srcHwPos = GetPhyType(srcPos);
    const uint64_t srcTensorSizeBytes = static_cast<uint64_t>(src.GetSize()) * sizeof(PrimT<T>);
    const uint64_t srcRequiredBytes = CalculateNz2NdSrcRequiredBytes<T>(intriParams);

    CheckNz2NdParamsCommon(intriParams, apiName);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
    const bool isValidSrc = (srcHwPos == Hardware::UB || srcHwPos == Hardware::L1);
    const __gm__ char* supportedPos = "VECOUT / A1 / B1";
#else
    const bool isValidSrc = (srcHwPos == Hardware::UB);
    const __gm__ char* supportedPos = "VECOUT";
#endif
    ASCENDC_DEBUG_ASSERT((isValidSrc), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check src tensor route in %s, "
        "supported src TPositions are %s, current src TPosition is %s.\n", apiName, supportedPos,
        GetTPositionName(srcPos)));
    ASCENDC_DEBUG_ASSERT((intriParams.dValue % BLOCK_CUBE == 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
        "check dValue value in %s, dValue must be divisible by %u, current value is %u.\n", apiName, BLOCK_CUBE,
        intriParams.dValue));
    CheckTensorAlignment(src, ONE_BLK_SIZE, "src", apiName);
    CheckRequiredBytes(srcRequiredBytes, srcTensorSizeBytes, "srcLocal", apiName);
}

template <typename T>
__aicore__ inline void CheckDataCopyPadParamsCommon(const DataCopyParams& dataCopyParams, const bool isGmToUb,
    const __gm__ char* apiName)
{
    constexpr uint32_t dataCopyPadBlkLenLimit = 2097151;
    CheckValueRange<uint16_t>(dataCopyParams.blockCount, 0, UINT12_MAX, "blockCount", apiName);
    CheckValueRange<uint32_t>(static_cast<uint32_t>(dataCopyParams.blockLen), 0, dataCopyPadBlkLenLimit, "blockLen",
        apiName);
    if (isGmToUb) {
        ASCENDC_DEBUG_ASSERT((dataCopyParams.blockLen % sizeof(PrimT<T>) == 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check blockLen value in %s, it must be divisible by %zu, current value is %u.\n", apiName,
            sizeof(PrimT<T>), dataCopyParams.blockLen));
    }
}

template <typename T>
__aicore__ inline void CheckDataCopyPadParamsCommon(const DataCopyExtParams& dataCopyParams, const bool isGmToUb,
    const __gm__ char* apiName)
{
    constexpr uint32_t dataCopyPadBlkLenLimit = 2097151;
    CheckValueRange<uint16_t>(dataCopyParams.blockCount, 0, UINT12_MAX, "blockCount", apiName);
    CheckValueRange<uint32_t>(dataCopyParams.blockLen, 0, dataCopyPadBlkLenLimit, "blockLen", apiName);
    if (isGmToUb) {
        ASCENDC_DEBUG_ASSERT((dataCopyParams.blockLen % sizeof(PrimT<T>) == 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check blockLen value in %s, it must be divisible by %zu, current value is %u.\n", apiName,
            sizeof(PrimT<T>), dataCopyParams.blockLen));
    }
}

template <typename ParamsT>
__aicore__ inline uint64_t CalculateDataCopyPadLinearBytes(const ParamsT& dataCopyParams)
{
    if (dataCopyParams.blockCount == 0 || dataCopyParams.blockLen == 0) { return 0; }
    return static_cast<uint64_t>(dataCopyParams.blockCount) * AlignUp(dataCopyParams.blockLen, DEFAULT_C0_SIZE) +
        static_cast<uint64_t>(dataCopyParams.blockCount - 1) * dataCopyParams.srcStride * DEFAULT_C0_SIZE;
}

template <typename T, typename ParamsT, typename PadParamsT>
__aicore__ inline uint64_t CalculateDataCopyPadDstBytes(const ParamsT& dataCopyParams, const PadParamsT& padParams)
{
    if (dataCopyParams.blockCount == 0 || dataCopyParams.blockLen == 0) { return 0; }
    const uint64_t paddingSize = static_cast<uint64_t>(padParams.leftPadding + padParams.rightPadding) *
        sizeof(PrimT<T>);
    return static_cast<uint64_t>(dataCopyParams.blockCount) *
        AlignUp(dataCopyParams.blockLen + paddingSize, DEFAULT_C0_SIZE) +
        static_cast<uint64_t>(dataCopyParams.blockCount - 1) * dataCopyParams.dstStride * DEFAULT_C0_SIZE;
}

template <typename T, typename ParamsT, typename PadParamsT>
__aicore__ inline void CheckGmToLocalPadTensor(const LocalTensor<T>& dst, const ParamsT& dataCopyParams,
    const PadParamsT& padParams, const bool isGmToUb, const bool isValidDst, const __gm__ char* apiName)
{
    const uint64_t dstRequiredBytes = CalculateDataCopyPadDstBytes<T>(dataCopyParams, padParams);
    const uint64_t dstTensorSizeBytes = static_cast<uint64_t>(dst.GetSize()) * sizeof(PrimT<T>);
    CheckDataCopyPadParamsCommon<T>(dataCopyParams, isGmToUb, apiName);
    ASCENDC_DEBUG_ASSERT((isValidDst), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check dst tensor route in %s, "
        "current dst TPosition is %s.\n", apiName, GetTPositionName(static_cast<TPosition>(dst.GetPosition()))));
    CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", apiName);
    CheckRequiredBytes(dstRequiredBytes, dstTensorSizeBytes, "dstLocal", apiName);
}

template <typename T, typename ParamsT>
__aicore__ inline void CheckLocalToGmPadTensor(const LocalTensor<T>& src, const ParamsT& dataCopyParams,
    const bool isValidSrc, const __gm__ char* apiName)
{
    const uint64_t srcRequiredBytes = CalculateDataCopyPadLinearBytes(dataCopyParams);
    const uint64_t srcTensorSizeBytes = static_cast<uint64_t>(src.GetSize()) * sizeof(PrimT<T>);
    CheckDataCopyPadParamsCommon<T>(dataCopyParams, false, apiName);
    ASCENDC_DEBUG_ASSERT((isValidSrc), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check src tensor route in %s, "
        "current src TPosition is %s.\n", apiName, GetTPositionName(static_cast<TPosition>(src.GetPosition()))));
    CheckTensorAlignment(src, ONE_BLK_SIZE, "src", apiName);
    CheckRequiredBytes(srcRequiredBytes, srcTensorSizeBytes, "srcLocal", apiName);
}

template <typename T, typename ParamsT>
__aicore__ inline void CheckPadUbToL1Tensor(const LocalTensor<T>& dst, const LocalTensor<T>& src,
    const ParamsT& dataCopyParams, const Nd2NzParams& nd2nzParams, const __gm__ char* apiName)
{
    const TPosition dstPos = static_cast<TPosition>(dst.GetPosition());
    const TPosition srcPos = static_cast<TPosition>(src.GetPosition());
    const Hardware dstHwPos = GetPhyType(dstPos);
    const Hardware srcHwPos = GetPhyType(srcPos);
    const uint64_t srcRequiredBytes = CalculateDataCopyPadLinearBytes(dataCopyParams);
    const uint64_t dstRequiredBytes = CalculateNd2NzDstRequiredBytes<T>(nd2nzParams);
    const uint64_t srcTensorSizeBytes = static_cast<uint64_t>(src.GetSize()) * sizeof(PrimT<T>);
    const uint64_t dstTensorSizeBytes = static_cast<uint64_t>(dst.GetSize()) * sizeof(PrimT<T>);

    CheckDataCopyPadParamsCommon<T>(dataCopyParams, false, apiName);
    CheckNd2NzParamsCommon(nd2nzParams, apiName);
    CheckUbToL1Route(srcHwPos, dstHwPos, srcPos, dstPos, apiName);
    CheckTensorAlignment(src, ONE_BLK_SIZE, "src", apiName);
    CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", apiName);
    CheckRequiredBytes(srcRequiredBytes, srcTensorSizeBytes, "srcLocal", apiName);
    CheckRequiredBytes(dstRequiredBytes, dstTensorSizeBytes, "dstLocal", apiName);
}

template <typename T>
__aicore__ inline void CheckDataCopyPadTensor(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
    const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams, const __gm__ char* apiName)
{
    (void)src;
    const Hardware dstHwPos = GetPhyType(static_cast<TPosition>(dst.GetPosition()));
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || \
    (__NPU_ARCH__ == 3113))
    const bool isValidDst = (dstHwPos == Hardware::UB || dstHwPos == Hardware::L1);
    const bool isGmToUb = (dstHwPos == Hardware::UB);
#else
    const bool isValidDst = (dstHwPos == Hardware::UB);
    const bool isGmToUb = true;
#endif
    CheckGmToLocalPadTensor(dst, dataCopyParams, padParams, isGmToUb, isValidDst, apiName);
}

template <typename T>
__aicore__ inline void CheckDataCopyPadTensor(const GlobalTensor<T>& dst, const LocalTensor<T>& src,
    const DataCopyParams& dataCopyParams, const __gm__ char* apiName)
{ (void)dst; CheckDataCopyPadLocalToGlobalTensor(src, dataCopyParams, apiName); }

template <typename T, typename U>
__aicore__ inline void CheckDataCopyPadTensor(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
    const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<U>& padParams, const __gm__ char* apiName)
{ (void)src; CheckGmToLocalPadTensor(dst, dataCopyParams, padParams,
    true, GetPhyType(static_cast<TPosition>(dst.GetPosition())) == Hardware::UB, apiName); }

template <typename T, typename CopyParamsT>
__aicore__ inline void CheckDataCopyPadLocalToGlobalTensor(const LocalTensor<T>& src, const CopyParamsT& dataCopyParams,
    const __gm__ char* apiName)
{
    const Hardware srcHwPos = GetPhyType(static_cast<TPosition>(src.GetPosition()));
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
    const bool isValidSrc = (srcHwPos == Hardware::UB || srcHwPos == Hardware::L1);
#else
    const bool isValidSrc = (srcHwPos == Hardware::UB);
#endif
    CheckLocalToGmPadTensor(src, dataCopyParams, isValidSrc, apiName);
}

template <typename T>
__aicore__ inline void CheckDataCopyPadTensor(const GlobalTensor<T>& dst, const LocalTensor<T>& src,
    const DataCopyExtParams& dataCopyParams, const __gm__ char* apiName)
{ (void)dst; CheckDataCopyPadLocalToGlobalTensor(src, dataCopyParams, apiName); }

template <typename T>
__aicore__ inline void CheckDataCopyPadTensor(const LocalTensor<T>& dst, const LocalTensor<T>& src,
    const DataCopyParams& dataCopyParams, const Nd2NzParams& nd2nzParams, const __gm__ char* apiName)
{ CheckPadUbToL1Tensor(dst, src, dataCopyParams, nd2nzParams, apiName); }

template <typename T>
__aicore__ inline void CheckDataCopyPadTensor(const LocalTensor<T>& dst, const LocalTensor<T>& src,
    const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams, const __gm__ char* apiName)
{ CheckPadUbToL1Tensor(dst, src, dataCopyParams, nd2nzParams, apiName); }

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_DATA_COPY_CHECK_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_CHECK_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_CHECK_H__
#endif
