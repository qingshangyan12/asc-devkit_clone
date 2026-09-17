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
 * \file kernel_operator_data_copy_ptr_impl.h
 * \brief Pointer overloads for DMA DataCopy / DataCopyPad / DataCopyL1ToUB.
 */
#ifndef ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_IMPL_H

#include "../../include/basic_api/kernel_operator_ptr_utils.h"

#if defined(ASCENDC_ENABLE_HW_PTR_API)
namespace AscendC {
template <typename T>
__aicore__ inline uint8_t GetPtrCacheMode(__gm__ T* addr)
{
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    return ExtractCacheMode(addr);
#else
    (void)addr;
    return 0;
#endif
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const DataCopyParams& repeatParams)
{
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyGM2UBImpl(dst, src, repeatParams, GetPtrCacheMode(src));
#else
    DataCopyGM2UBImpl(dst, src, repeatParams);
#endif
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const DataCopyParams& repeatParams)
{
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyGM2L1Impl(dst, src, repeatParams, GetPtrCacheMode(src));
#else
    DataCopyGM2L1Impl(dst, src, repeatParams);
#endif
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyUB2GMImpl(dst, src, repeatParams, GetPtrCacheMode(dst));
#else
    DataCopyUB2GMImpl(dst, src, repeatParams);
#endif
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyL12GMImpl(dst, src, repeatParams, GetPtrCacheMode(dst));
#else
    DataCopyL12GMImpl(dst, src, repeatParams);
#endif
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
    DataCopyL12UBImpl(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    DataCopyL12BTImpl(reinterpret_cast<uint64_t>(dst), src, static_cast<uint16_t>(0), repeatParams);
}

#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3510) || \
    (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T>
__aicore__ inline void DataCopy(__fbuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    DataCopyL12FBImpl(dst, src, repeatParams);
}
#endif

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || \
    (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, bool enableSmallC0>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    CheckNd2NzParams(intriParams, "DataCopy with Nd2NzParams");
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
    DataCopyGM2L1ND2NZImpl<T, enableSmallC0>(dst, src, intriParams, GetPtrCacheMode(src));
#else
    DataCopyGM2L1ND2NZImpl(dst, src, intriParams);
    (void)enableSmallC0;
#endif
}

template <typename T, bool enableSmallC0>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    CheckNd2NzParams(intriParams, "DataCopy with Nd2NzParams");
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || \
    (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
    DataCopyGM2UBND2NZImpl(dst, src, intriParams, GetPtrCacheMode(src));
#else
    DataCopyGM2UBND2NZImpl(dst, src, intriParams);
#endif
    (void)enableSmallC0;
}
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    CheckNd2NzParams(intriParams, "DataCopy with Nd2NzParams");
    DataCopyGM2L1ND2NZImpl(dst, src, intriParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    CheckNd2NzParams(intriParams, "DataCopy with Nd2NzParams");
    DataCopyGM2UBND2NZImpl(dst, src, intriParams);
}
#endif

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const Nd2NzParams& intriParams)
{
    DataCopyUB2L1ND2NZImpl(dst, src, intriParams);
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, bool enableSmallC0>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams)
{
    DataCopyGM2L1DN2NZImpl(dst, src, intriParams, enableSmallC0, GetPtrCacheMode(src));
}
#endif

template <typename T>
__aicore__ inline void DataCopySliceFillShape(
    const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[], const uint32_t dimValue, uint32_t* dstShapeInfo,
    uint32_t* srcShapeInfo)
{
    for (uint32_t i = 0; i < dimValue; i++) {
        srcShapeInfo[i] = srcSliceInfo[i].shapeValue;
        dstShapeInfo[i] = dstSliceInfo[i].shapeValue;
    }
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, __gm__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue)
{
    uint32_t srcShapeInfo[K_MAX_SHAPE_DIM] = {0};
    uint32_t dstShapeInfo[K_MAX_SHAPE_DIM] = {0};
    DataCopySliceFillShape<T>(dstSliceInfo, srcSliceInfo, dimValue, dstShapeInfo, srcShapeInfo);
    uint32_t srcStartIndex = DataCopyGetPhyStartIndex(srcSliceInfo, srcShapeInfo, dimValue);
    uint32_t dstStartIndex = DataCopyGetPhyStartIndex(dstSliceInfo, dstShapeInfo, dimValue);
    uint32_t srcOffsetListSize = 0;
    uint32_t dstOffsetListSize = 0;
    uint32_t srcOffsetList[MAX_SLICE_SIZE] = {0};
    uint32_t dstOffsetList[MAX_SLICE_SIZE] = {0};
    DataCopyGetOffsetList(srcSliceInfo, srcShapeInfo, dimValue, &srcOffsetListSize, srcOffsetList);
    DataCopyGetOffsetList(dstSliceInfo, dstShapeInfo, dimValue, &dstOffsetListSize, dstOffsetList);
    DataCopyParams repeatParams;
    repeatParams.blockLen = srcSliceInfo[0].burstLen;
    uint32_t oneSliceLen = srcSliceInfo[0].burstLen * AscendCUtils::GetC0Count(sizeof(T)) + srcSliceInfo[0].stride;
    repeatParams.blockCount =
        (srcSliceInfo[0].endIndex - srcSliceInfo[0].startIndex + 1 + srcSliceInfo[0].stride) / oneSliceLen;
    repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
    if ((srcSliceInfo[0].stride * sizeof(T)) % AscendCUtils::GetC0Size() == 0) {
        repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
            DataCopy(dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams);
        }
    } else {
        repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T);
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
            DataCopySliceGm2UBImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams,
                GetPtrCacheMode(src));
#else
            DataCopySliceGm2UBImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams);
#endif
        }
    }
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue)
{
    uint32_t srcShapeInfo[K_MAX_SHAPE_DIM] = {0};
    uint32_t dstShapeInfo[K_MAX_SHAPE_DIM] = {0};
    DataCopySliceFillShape<T>(dstSliceInfo, srcSliceInfo, dimValue, dstShapeInfo, srcShapeInfo);
    uint32_t srcStartIndex = DataCopyGetPhyStartIndex(srcSliceInfo, srcShapeInfo, dimValue);
    uint32_t dstStartIndex = DataCopyGetPhyStartIndex(dstSliceInfo, dstShapeInfo, dimValue);
    uint32_t srcOffsetListSize = 0;
    uint32_t dstOffsetListSize = 0;
    uint32_t srcOffsetList[MAX_SLICE_SIZE] = {0};
    uint32_t dstOffsetList[MAX_SLICE_SIZE] = {0};
    DataCopyGetOffsetList(srcSliceInfo, srcShapeInfo, dimValue, &srcOffsetListSize, srcOffsetList);
    DataCopyGetOffsetList(dstSliceInfo, dstShapeInfo, dimValue, &dstOffsetListSize, dstOffsetList);
    DataCopyParams repeatParams;
    repeatParams.blockLen = srcSliceInfo[0].burstLen;
    uint32_t oneSliceLen = srcSliceInfo[0].burstLen * AscendCUtils::GetC0Count(sizeof(T)) + srcSliceInfo[0].stride;
    repeatParams.blockCount =
        (srcSliceInfo[0].endIndex - srcSliceInfo[0].startIndex + 1 + srcSliceInfo[0].stride) / oneSliceLen;
    repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
    if ((dstSliceInfo[0].stride * sizeof(T)) % AscendCUtils::GetC0Size() == 0) {
        repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
            DataCopy(dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams);
        }
    } else {
        repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T);
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
            DataCopySliceUB2GMImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams,
                GetPtrCacheMode(dst));
#else
            DataCopySliceUB2GMImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams);
#endif
        }
    }
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
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(T));
    DataCopy(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(T));
    DataCopy(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __cbuf__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(T));
    DataCopy(dst, src, repeatParams);
}

template <typename T, typename U>
__aicore__ inline void DataCopy(__gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    DataCopyL0C2GMImpl(dst, src, intriParams, GetPtrCacheMode(dst));
#else
    DataCopyL0C2GMImpl(dst, src, intriParams);
#endif
}

template <typename T, typename U>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    DataCopyL0C2L1Impl(dst, src, intriParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyGM2UBImpl(dst, src, intriParams, GetPtrCacheMode(src));
#else
    DataCopyGM2UBImpl(dst, src, intriParams);
#endif
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyGM2L1Impl(dst, src, intriParams, GetPtrCacheMode(src));
#else
    DataCopyGM2L1Impl(dst, src, intriParams);
#endif
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyUB2GMImpl(dst, src, intriParams, GetPtrCacheMode(dst));
#else
    DataCopyUB2GMImpl(dst, src, intriParams);
#endif
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyL12GMImpl(dst, src, intriParams, GetPtrCacheMode(dst));
#else
    DataCopyL12GMImpl(dst, src, intriParams);
#endif
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

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    DataCopyPadGm2UBImpl<T, mode>(dst, src, dataCopyParams, padParams, GetPtrCacheMode(src));
}

template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    DataCopyPadGm2L1Impl<T, mode>(dst, src, dataCopyParams, padParams, GetPtrCacheMode(src));
}

template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams)
{
#if (__NPU_ARCH__ != 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    DataCopyPadUB2GMImpl<T, mode>(dst, src, dataCopyParams, GetPtrCacheMode(dst));
}
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    DataCopyPadGm2UBImpl(dst, src, dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
    DataCopyPadGm2L1Impl(dst, src, dataCopyParams, padParams);
#else
    DataCopyPadGM2L1Impl(dst, src, dataCopyParams, padParams);
#endif
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams)
{
    if ASCEND_IS_AIC {
        return;
    }
    DataCopyPadUB2GMImpl(dst, src, dataCopyParams);
}
#endif

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
{
    DataCopyPadGm2UBImpl<T, mode>(dst, src, dataCopyParams, padParams, GetPtrCacheMode(src));
}

template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
{
    DataCopyPadGm2L1Impl<T, mode>(dst, src, dataCopyParams, padParams, GetPtrCacheMode(src));
}

template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams)
{
#if (__NPU_ARCH__ != 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    DataCopyPadUB2GMImpl<T, mode>(dst, src, dataCopyParams, GetPtrCacheMode(dst));
}

#if (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __cbuf__ T* src, const DataCopyExtParams& dataCopyParams)
{
    DataCopyPadL12GMImpl<T, mode>(dst, src, dataCopyParams);
}
#endif
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
{
    DataCopyPadGm2UBImpl(dst, src, dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams)
{
    if ASCEND_IS_AIC {
        return;
    }
    DataCopyPadUB2GMImpl(dst, src, dataCopyParams);
}

#if (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102)
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __cbuf__ T* src, const DataCopyExtParams& dataCopyParams)
{
    DataCopyPadL12GMImpl(dst, src, dataCopyParams);
}
#endif
#endif

template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    CheckNd2NzParams(nd2nzParams, "DataCopyPad with Nd2NzParams");
    DataCopyPadUB2L1Impl(dst, src, dataCopyParams, nd2nzParams);
}

template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    CheckNd2NzParams(nd2nzParams, "DataCopyPad with Nd2NzParams");
    DataCopyPadUB2L1Impl(dst, src, dataCopyParams, nd2nzParams);
}

template <typename T, uint8_t subBlockId>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
#if (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510))
    DataCopyL12UBImpl<T, subBlockId>(dst, src, repeatParams);
#else
    (void)dst;
    (void)src;
    (void)repeatParams;
#endif
}

template <typename T, uint8_t subBlockId>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const uint32_t count)
{
    DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(T));
    DataCopyL1ToUB<T, subBlockId>(dst, src, repeatParams);
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename T, uint8_t dim, const NdDmaConfig& config>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __gm__ T* src, const MultiCopyParams<T, dim>& params)
{
#if (__NPU_ARCH__ != 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    DataCopyWithNDDMAImpl<T, dim, config>(dst, src, params.loopInfo, params.constantValue, GetPtrCacheMode(src));
}
#endif

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const Nz2NdParamsFull& intriParams)
{
    CheckNz2NdParams(intriParams);
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyUB2GMNZ2NDImpl(dst, src, intriParams, GetPtrCacheMode(dst));
#else
    DataCopyUB2GMNZ2NDImpl(dst, src, intriParams);
#endif
}

#if defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __cbuf__ T* src, const Nz2NdParamsFull& intriParams)
{
    CheckNz2NdParams(intriParams);
    DataCopyL12GMNZ2NDImpl(dst, src, intriParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __cbuf__ T* src, const Nz2DnParamsFull& intriParams)
{
    DataCopyL12GMNZ2DNImpl(dst, src, intriParams);
}
#endif
} // namespace AscendC
#endif // ASCENDC_ENABLE_HW_PTR_API

#endif // ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_IMPL_H
