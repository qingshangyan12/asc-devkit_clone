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
 * \file kernel_operator_data_copy_ptr_intf_impl.h
 * \brief Pointer overloads for DMA basic APIs on DAV 3510.
 */
#ifndef ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_INTERFACE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_INTERFACE_IMPL_H

/* This file is included inside namespace AscendC by kernel_operator_data_copy_intf_impl.h. */

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const DataCopyParams& repeatParams)
{
    DataCopyGM2UBImpl(dst, src, repeatParams, 0);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const DataCopyParams& repeatParams)
{
    DataCopyGM2L1Impl(dst, src, repeatParams, 0);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    DataCopyUB2GMImpl(dst, src, repeatParams, 0);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    DataCopyL12GMImpl(dst, src, repeatParams, 0);
}

template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    DataCopyUB2UBImpl(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    DataCopyUB2L1Impl(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    DataCopyL12UBImpl<T, 0>(dst, src, repeatParams);
}

template <typename T, bool enableSmallC0>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    CheckNd2NzParams(intriParams, "DataCopy pointer overload with Nd2NzParams");
    DataCopyGM2L1ND2NZImpl<T, enableSmallC0>(dst, src, intriParams, 0);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    CheckNd2NzParams(intriParams, "DataCopy pointer overload with Nd2NzParams");
    DataCopyGM2UBND2NZImpl(dst, src, intriParams, 0);
}

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const Nd2NzParams& intriParams)
{
    CheckNd2NzParams(intriParams, "DataCopy pointer overload with Nd2NzParams");
    DataCopyUB2L1ND2NZImpl(dst, src, intriParams);
}

template <typename T, bool enableSmallC0>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams)
{
    DataCopyGM2L1DN2NZImpl(dst, src, intriParams, enableSmallC0, 0);
}

template <typename T>
__aicore__ inline bool CheckDataCopyPointerSliceInfo(
    const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[], const uint32_t dimValue)
{
    if (dimValue == 0 || dimValue > K_MAX_SHAPE_DIM) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "invalid DataCopy pointer slice dimension"); });
        return false;
    }
    uint64_t srcSliceCount = 1;
    uint64_t dstSliceCount = 1;
    uint64_t srcShapeElements = 1;
    uint64_t dstShapeElements = 1;
    for (uint32_t i = 0; i < dimValue; ++i) {
        const SliceInfo& srcInfo = srcSliceInfo[i];
        const SliceInfo& dstInfo = dstSliceInfo[i];
        if (srcInfo.shapeValue == 0 || dstInfo.shapeValue == 0 || srcInfo.startIndex > srcInfo.endIndex ||
            dstInfo.startIndex > dstInfo.endIndex || srcInfo.endIndex >= srcInfo.shapeValue ||
            dstInfo.endIndex >= dstInfo.shapeValue) {
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "invalid DataCopy pointer slice range or shape"); });
            return false;
        }
        const uint64_t srcSpan = static_cast<uint64_t>(srcInfo.endIndex) - srcInfo.startIndex + 1;
        const uint64_t dstSpan = static_cast<uint64_t>(dstInfo.endIndex) - dstInfo.startIndex + 1;
        if (srcSpan + srcInfo.stride > UINT32_MAX || dstSpan + dstInfo.stride > UINT32_MAX ||
            srcInfo.stride == UINT32_MAX || dstInfo.stride == UINT32_MAX) {
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "invalid DataCopy pointer slice stride"); });
            return false;
        }
        if (srcShapeElements > UINT32_MAX / srcInfo.shapeValue ||
            dstShapeElements > UINT32_MAX / dstInfo.shapeValue) {
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopy pointer slice shape exceeds index range"); });
            return false;
        }
        srcShapeElements *= srcInfo.shapeValue;
        dstShapeElements *= dstInfo.shapeValue;
        if (i != 0) {
            const uint64_t srcSliceFactor =
                (srcSpan + srcInfo.stride) / (static_cast<uint64_t>(srcInfo.stride) + 1);
            const uint64_t dstSliceFactor =
                (dstSpan + dstInfo.stride) / (static_cast<uint64_t>(dstInfo.stride) + 1);
            if (srcSliceFactor == 0 || dstSliceFactor == 0 ||
                srcSliceCount > MAX_SLICE_SIZE / srcSliceFactor ||
                dstSliceCount > MAX_SLICE_SIZE / dstSliceFactor) {
                ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopy pointer slice list exceeds capacity"); });
                return false;
            }
            srcSliceCount *= srcSliceFactor;
            dstSliceCount *= dstSliceFactor;
        }
    }
    const uint64_t oneSliceLen = static_cast<uint64_t>(srcSliceInfo[0].burstLen) *
        AscendCUtils::GetC0Count(sizeof(T)) + srcSliceInfo[0].stride;
    if (srcSliceInfo[0].burstLen == 0 || oneSliceLen > UINT32_MAX ||
        static_cast<uint64_t>(srcSliceInfo[0].stride) * sizeof(T) > UINT32_MAX ||
        static_cast<uint64_t>(dstSliceInfo[0].stride) * sizeof(T) > UINT32_MAX) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "invalid DataCopy pointer slice length"); });
        return false;
    }
    return true;
}

template <typename T>
__aicore__ inline void DataCopyPtrSliceGm2Ub(
    __ubuf__ T* dst, __gm__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue)
{
    if (!CheckDataCopyPointerSliceInfo<T>(dstSliceInfo, srcSliceInfo, dimValue)) {
        return;
    }
    uint32_t srcOffsetListSize = 0;
    uint32_t dstOffsetListSize = 0;
    uint32_t srcShapeInfo[K_MAX_SHAPE_DIM];
    uint32_t dstShapeInfo[K_MAX_SHAPE_DIM];
    for (uint32_t i = 0; i < dimValue; ++i) {
        srcShapeInfo[i] = srcSliceInfo[i].shapeValue;
        dstShapeInfo[i] = dstSliceInfo[i].shapeValue;
    }
    const uint32_t srcStartIndex = DataCopyGetPhyStartIndex(srcSliceInfo, srcShapeInfo, dimValue);
    const uint32_t dstStartIndex = DataCopyGetPhyStartIndex(dstSliceInfo, dstShapeInfo, dimValue);
    uint32_t srcOffsetList[MAX_SLICE_SIZE];
    uint32_t dstOffsetList[MAX_SLICE_SIZE];
    DataCopyGetOffsetList(srcSliceInfo, srcShapeInfo, dimValue, &srcOffsetListSize, srcOffsetList);
    DataCopyGetOffsetList(dstSliceInfo, dstShapeInfo, dimValue, &dstOffsetListSize, dstOffsetList);
    if (srcOffsetListSize != dstOffsetListSize) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "source and destination slice counts must be equal"); });
        return;
    }

    DataCopyParams repeatParams;
    repeatParams.blockLen = srcSliceInfo[0].burstLen;
    const uint32_t oneSliceLen =
        srcSliceInfo[0].burstLen * AscendCUtils::GetC0Count(sizeof(T)) + srcSliceInfo[0].stride;
    repeatParams.blockCount =
        (srcSliceInfo[0].endIndex - srcSliceInfo[0].startIndex + 1 + srcSliceInfo[0].stride) / oneSliceLen;
    repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
    if ((srcSliceInfo[0].stride * sizeof(T)) % AscendCUtils::GetC0Size() == 0) {
        repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
        for (uint32_t i = 0; i < srcOffsetListSize; ++i) {
            DataCopyGM2UBImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams, 0);
        }
    } else {
        repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T);
        for (uint32_t i = 0; i < srcOffsetListSize; ++i) {
            DataCopySliceGm2UBImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams, 0);
        }
    }
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, __gm__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue)
{
    DataCopyPtrSliceGm2Ub(dst, src, dstSliceInfo, srcSliceInfo, dimValue);
}

template <typename T>
__aicore__ inline void DataCopyPtrSliceUb2Gm(
    __gm__ T* dst, __ubuf__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue)
{
    if (!CheckDataCopyPointerSliceInfo<T>(dstSliceInfo, srcSliceInfo, dimValue)) {
        return;
    }
    uint32_t srcOffsetListSize = 0;
    uint32_t dstOffsetListSize = 0;
    uint32_t srcShapeInfo[K_MAX_SHAPE_DIM];
    uint32_t dstShapeInfo[K_MAX_SHAPE_DIM];
    for (uint32_t i = 0; i < dimValue; ++i) {
        srcShapeInfo[i] = srcSliceInfo[i].shapeValue;
        dstShapeInfo[i] = dstSliceInfo[i].shapeValue;
    }
    const uint32_t srcStartIndex = DataCopyGetPhyStartIndex(srcSliceInfo, srcShapeInfo, dimValue);
    const uint32_t dstStartIndex = DataCopyGetPhyStartIndex(dstSliceInfo, dstShapeInfo, dimValue);
    uint32_t srcOffsetList[MAX_SLICE_SIZE];
    uint32_t dstOffsetList[MAX_SLICE_SIZE];
    DataCopyGetOffsetList(srcSliceInfo, srcShapeInfo, dimValue, &srcOffsetListSize, srcOffsetList);
    DataCopyGetOffsetList(dstSliceInfo, dstShapeInfo, dimValue, &dstOffsetListSize, dstOffsetList);
    if (srcOffsetListSize != dstOffsetListSize) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "source and destination slice counts must be equal"); });
        return;
    }

    DataCopyParams repeatParams;
    repeatParams.blockLen = srcSliceInfo[0].burstLen;
    const uint32_t oneSliceLen =
        srcSliceInfo[0].burstLen * AscendCUtils::GetC0Count(sizeof(T)) + srcSliceInfo[0].stride;
    repeatParams.blockCount =
        (srcSliceInfo[0].endIndex - srcSliceInfo[0].startIndex + 1 + srcSliceInfo[0].stride) / oneSliceLen;
    repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
    if ((dstSliceInfo[0].stride * sizeof(T)) % AscendCUtils::GetC0Size() == 0) {
        repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
        for (uint32_t i = 0; i < srcOffsetListSize; ++i) {
            DataCopyUB2GMImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams, 0);
        }
    } else {
        repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T);
        for (uint32_t i = 0; i < srcOffsetListSize; ++i) {
            DataCopySliceUB2GMImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams, 0);
        }
    }
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue)
{
    DataCopyPtrSliceUb2Gm(dst, src, dstSliceInfo, srcSliceInfo, dimValue);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    DataCopyCheck<T>(count, repeatParams);
    DataCopy(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    DataCopyCheck<T>(count, repeatParams);
    DataCopy(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    DataCopyCheck<T>(count, repeatParams);
    DataCopy(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __cbuf__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    DataCopyCheck<T>(count, repeatParams);
    DataCopy(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    DataCopyCheck<T>(count, repeatParams);
    DataCopy(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    DataCopyCheck<T>(count, repeatParams);
    DataCopy(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __cbuf__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    DataCopyCheck<T>(count, repeatParams);
    DataCopy(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const Nz2NdParamsFull& intriParams)
{
    CheckNz2NdParams(intriParams);
    DataCopyUB2GMNZ2NDImpl(dst, src, intriParams, 0);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopyGM2UBImpl(dst, src, intriParams, 0);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopyGM2L1Impl(dst, src, intriParams, 0);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopyUB2GMImpl(dst, src, intriParams, 0);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopyL12GMImpl(dst, src, intriParams, 0);
}

template <typename T>
__aicore__ inline void DataCopy(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopyUB2L1Impl(dst, src, intriParams);
}

template <typename T>
__aicore__ inline void DataCopy(
    __ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopyUB2UBImpl(dst, src, intriParams);
}

template <typename T>
__aicore__ inline void DataCopy(
    __ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopyL12UBImpl<T, 0>(dst, src, intriParams);
}

template <typename T, typename U>
__aicore__ inline void DataCopy(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL12L0CImpl(dst, src, intriParams, enhancedParams);
}

template <typename T, typename U>
__aicore__ inline void DataCopy(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL0C2UBImpl(dst, src, intriParams, enhancedParams);
}

template <typename T, typename U>
__aicore__ inline void DataCopy(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(dst, src, intriParams, enhancedParams);
}

template <typename T, typename U>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    DataCopyL0C2L1Impl(dst, src, intriParams);
}

template <typename T, typename U>
__aicore__ inline void DataCopy(__gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    DataCopyL0C2GMImpl(dst, src, intriParams, 0);
}

template <typename T, uint8_t subBlockId>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    DataCopyL12UBImpl<T, subBlockId>(dst, src, repeatParams);
}

template <typename T, uint8_t subBlockId>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    DataCopyCheck<T>(count, repeatParams);
    DataCopyL1ToUB<T, subBlockId>(dst, src, repeatParams);
}

template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    DataCopyPadGm2UBImpl<T, mode>(dst, src, dataCopyParams, padParams, 0);
}

template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    DataCopyPadGm2L1Impl<T, mode>(dst, src, dataCopyParams, padParams, 0);
}

template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams)
{
    if ASCEND_IS_AIC {
        return;
    }
    DataCopyPadUB2GMImpl<T, mode>(dst, src, dataCopyParams, 0);
}

template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    DataCopyPadUB2L1Impl(dst, src, dataCopyParams, nd2nzParams, 0);
}

template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
{
    DataCopyPadGm2UBImpl<T, mode>(dst, src, dataCopyParams, padParams, 0);
}

template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
{
    DataCopyPadGm2L1Impl<T, mode>(dst, src, dataCopyParams, padParams, 0);
}

template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams)
{
    if ASCEND_IS_AIC {
        return;
    }
    DataCopyPadUB2GMImpl<T, mode>(dst, src, dataCopyParams, 0);
}

template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    DataCopyPadUB2L1Impl(dst, src, dataCopyParams, nd2nzParams, 0);
}

template <typename T, uint8_t dim, const NdDmaConfig& config>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __gm__ T* src, const MultiCopyParams<T, dim>& params)
{
    if ASCEND_IS_AIC {
        return;
    }
    DataCopyWithNDDMAImpl<T, dim, config>(dst, src, params.loopInfo, params.constantValue, 0);
}

#endif // ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_INTERFACE_IMPL_H
