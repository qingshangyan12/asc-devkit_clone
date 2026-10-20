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
 * Included from kernel_operator_data_copy_intf_impl.h inside namespace AscendC.
 * Tensor overloads remain the primary match; both paths call the same *Impl.
 */

#if !defined(ASCENDC_CPU_DEBUG) || ASCENDC_CPU_DEBUG != 1

/* DataCopy: GM -> UB / L1, DataCopyParams, MTE2 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const DataCopyParams& repeatParams)
{
    using PrimType = PrimT<T>;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyGM2UBImpl((__ubuf__ PrimType*)dst, (__gm__ PrimType*)src, repeatParams, ExtractCacheMode(src));
#else
    DataCopyGM2UBImpl((__ubuf__ PrimType*)dst, (__gm__ PrimType*)src, repeatParams);
#endif
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const DataCopyParams& repeatParams)
{
    using PrimType = PrimT<T>;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyGM2L1Impl((__cbuf__ PrimType*)dst, (__gm__ PrimType*)src, repeatParams, ExtractCacheMode(src));
#else
    DataCopyGM2L1Impl((__cbuf__ PrimType*)dst, (__gm__ PrimType*)src, repeatParams);
#endif
}

/* DataCopy: UB / L1 -> GM, DataCopyParams, MTE3 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    using PrimType = PrimT<T>;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyUB2GMImpl((__gm__ PrimType*)dst, (__ubuf__ PrimType*)src, repeatParams, ExtractCacheMode(dst));
#else
    DataCopyUB2GMImpl((__gm__ PrimType*)dst, (__ubuf__ PrimType*)src, repeatParams);
#endif
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    using PrimType = PrimT<T>;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyL12GMImpl((__gm__ PrimType*)dst, (__cbuf__ PrimType*)src, repeatParams, ExtractCacheMode(dst));
#else
    DataCopyL12GMImpl((__gm__ PrimType*)dst, (__cbuf__ PrimType*)src, repeatParams);
#endif
}

/* DataCopy: local -> local, DataCopyParams */
template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    using PrimType = PrimT<T>;
    DataCopyUB2UBImpl((__ubuf__ PrimType*)dst, (__ubuf__ PrimType*)src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    using PrimType = PrimT<T>;
    DataCopyUB2L1Impl((__cbuf__ PrimType*)dst, (__ubuf__ PrimType*)src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    using PrimType = PrimT<T>;
    DataCopyL12UBImpl<PrimType, 0>((__ubuf__ PrimType*)dst, (__cbuf__ PrimType*)src, repeatParams);
}

/* DataCopy: L1 -> BiasTable, DataCopyParams, mixed types.
 * dst is the BiasTable address passed as __cbuf__* (no dedicated BT address space).
 * isEnableConv follows Tensor dtype rules via DataCopyL12BTByDtype. */
template <typename T, typename U>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __cbuf__ U* src, const DataCopyParams& repeatParams)
{
    DataCopyL12BTByDtype<T, U>((uint64_t)dst, src, repeatParams);
}

/* DataCopy count forms */
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
    using PrimType = PrimT<T>;
    DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(PrimType));
    DataCopy(dst, src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    using PrimType = PrimT<T>;
    DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(PrimType));
    DataCopy(dst, src, repeatParams);
}

/* Nd2Nz / Dn2Nz */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || \
    (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, bool enableSmallC0 = false>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    using PrimType = PrimT<T>;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
    const uint8_t cacheMode = ExtractCacheMode(src);
    DataCopyGM2L1ND2NZImpl<PrimType, enableSmallC0>(
        (__cbuf__ PrimType*)dst, (__gm__ PrimType*)src, intriParams, cacheMode);
#else
    DataCopyGM2L1ND2NZImpl((__cbuf__ PrimType*)dst, (__gm__ PrimType*)src, intriParams);
#endif
}

template <typename T, bool enableSmallC0 = false>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    using PrimType = PrimT<T>;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || \
    (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
    const uint8_t cacheMode = ExtractCacheMode(src);
    DataCopyGM2UBND2NZImpl((__ubuf__ PrimType*)dst, (__gm__ PrimType*)src, intriParams, cacheMode);
#else
    DataCopyGM2UBND2NZImpl((__ubuf__ PrimType*)dst, (__gm__ PrimType*)src, intriParams);
#endif
}
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    using PrimType = PrimT<T>;
    DataCopyGM2L1ND2NZImpl((__cbuf__ PrimType*)dst, (__gm__ PrimType*)src, intriParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    using PrimType = PrimT<T>;
    DataCopyGM2UBND2NZImpl((__ubuf__ PrimType*)dst, (__gm__ PrimType*)src, intriParams);
}
#endif

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const Nd2NzParams& intriParams)
{
    using PrimType = PrimT<T>;
    DataCopyUB2L1ND2NZImpl((__cbuf__ PrimType*)dst, (__ubuf__ PrimType*)src, intriParams);
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, bool enableSmallC0 = false>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams)
{
    using PrimType = PrimT<T>;
    const uint8_t cacheMode = ExtractCacheMode(src);
    DataCopyGM2L1DN2NZImpl((__cbuf__ PrimType*)dst, (__gm__ PrimType*)src, intriParams, enableSmallC0, cacheMode);
}
#endif

/* L0C -> L1 / GM */
template <typename T, typename U>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    DataCopyL0C2L1Impl((__cbuf__ PrimT<T>*)dst, (__cc__ PrimT<U>*)src, intriParams);
}

template <typename T, typename U>
__aicore__ inline void DataCopy(__gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    DataCopyL0C2GMImpl((__gm__ PrimT<T>*)dst, (__cc__ PrimT<U>*)src, intriParams, ExtractCacheMode(dst));
#else
    DataCopyL0C2GMImpl((__gm__ PrimT<T>*)dst, (__cc__ PrimT<U>*)src, intriParams);
#endif
}

/* Enhanced GM <-> UB / L1 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopy(dst, src, intriParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopy(dst, src, intriParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopy(dst, src, intriParams);
}

template <typename T>
__aicore__ inline void DataCopy(
    __ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopy(dst, src, intriParams);
}

/* Enhanced SFINAE conversion pairs: L0C <-> UB */
#if (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201))
template <
    typename T, typename U,
    typename Std::enable_if<
        Std::is_same<PrimT<T>, bfloat16_t>::value && Std::is_same<PrimT<U>, float>::value, bool>::type = true>
__aicore__ inline void DataCopy(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL12L0CImpl((__cc__ PrimT<T>*)dst, (__cbuf__ PrimT<U>*)src, intriParams, enhancedParams);
}
#endif

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, half>::value && Std::is_same<PrimT<U>, float>::value, bool>::type =
        true>
__aicore__ inline void DataCopy(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL12L0CImpl((__cc__ half*)dst, (__cbuf__ float*)src, intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, half>::value && Std::is_same<PrimT<U>, float>::value, bool>::type =
        true>
__aicore__ inline void DataCopy(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL0C2UBImpl((__ubuf__ half*)dst, (__cc__ float*)src, intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, half>::value && Std::is_same<PrimT<U>, int32_t>::value, bool>::type =
        true>
__aicore__ inline __inout_pipe__(V) void DataCopy(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL0C2UBImpl((__ubuf__ half*)dst, (__cc__ int32_t*)src, intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<
        Std::is_same<PrimT<T>, int16_t>::value && Std::is_same<PrimT<U>, int32_t>::value, bool>::type = true>
__aicore__ inline __inout_pipe__(V) void DataCopy(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL0C2UBImpl((__ubuf__ int16_t*)dst, (__cc__ int32_t*)src, intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<
        Std::is_same<PrimT<T>, int8_t>::value && Std::is_same<PrimT<U>, int32_t>::value, bool>::type = true>
__aicore__ inline __inout_pipe__(V) void DataCopy(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL0C2UBImpl((__ubuf__ int8_t*)dst, (__cc__ int32_t*)src, intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<
        Std::is_same<PrimT<T>, uint8_t>::value && Std::is_same<PrimT<U>, int32_t>::value, bool>::type = true>
__aicore__ inline __inout_pipe__(V) void DataCopy(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL0C2UBImpl((__ubuf__ uint8_t*)dst, (__cc__ int32_t*)src, intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, float>::value && Std::is_same<PrimT<U>, half>::value, bool>::type =
        true>
__aicore__ inline __inout_pipe__(V) void DataCopy(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl((__cc__ float*)dst, (__ubuf__ half*)src, intriParams, enhancedParams);
}

/* Nz2Nd */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const Nz2NdParamsFull& intriParams)
{
    using PrimType = PrimT<T>;
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || (__NPU_ARCH__ == 9201) || \
    (__NPU_ARCH__ == 9202)
    DataCopyUB2GMNZ2NDImpl((__gm__ PrimType*)dst, (__ubuf__ PrimType*)src, intriParams, ExtractCacheMode(dst));
#else
    DataCopyUB2GMNZ2NDImpl((__gm__ PrimType*)dst, (__ubuf__ PrimType*)src, intriParams);
#endif
}

#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
     (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163) || (__NPU_ARCH__ == 5162))
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __cbuf__ T* src, const Nz2NdParamsFull& intriParams)
{
    DataCopyL12GMNZ2NDImpl((__gm__ T*)dst, (__cbuf__ T*)src, intriParams);
}
#endif

/* Slice: require SliceInfo.shapeValue; do not read Tensor shape. */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, __gm__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue = 1)
{
    uint32_t srcStartIndex = 0;
    uint32_t dstStartIndex = 0;
    uint32_t srcOffsetListSize = 0;
    uint32_t dstOffsetListSize = 0;
    uint32_t srcShapeInfo[K_MAX_SHAPE_DIM];
    uint32_t dstShapeInfo[K_MAX_SHAPE_DIM];
    for (uint32_t i = 0; i < dimValue; i++) {
        srcShapeInfo[i] = srcSliceInfo[i].shapeValue;
        dstShapeInfo[i] = dstSliceInfo[i].shapeValue;
    }
    srcStartIndex = DataCopyGetPhyStartIndex(srcSliceInfo, srcShapeInfo, dimValue);
    dstStartIndex = DataCopyGetPhyStartIndex(dstSliceInfo, dstShapeInfo, dimValue);
    uint32_t srcOffsetList[MAX_SLICE_SIZE];
    uint32_t dstOffsetList[MAX_SLICE_SIZE];
    DataCopyGetOffsetList(srcSliceInfo, srcShapeInfo, dimValue, &srcOffsetListSize, srcOffsetList);
    DataCopyGetOffsetList(dstSliceInfo, dstShapeInfo, dimValue, &dstOffsetListSize, dstOffsetList);
    DataCopyParams repeatParams;
    repeatParams.blockLen = srcSliceInfo[0].burstLen;
    uint32_t oneSliceLen = srcSliceInfo[0].burstLen * AscendCUtils::GetC0Count(sizeof(T)) + srcSliceInfo[0].stride;
    repeatParams.blockCount =
        (srcSliceInfo[0].endIndex - srcSliceInfo[0].startIndex + 1 + srcSliceInfo[0].stride) / oneSliceLen;
    repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    const uint8_t cacheMode = ExtractCacheMode(src);
#endif
    if ((srcSliceInfo[0].stride * sizeof(T)) % AscendCUtils::GetC0Size() == 0) {
        repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
            DataCopyGM2UBImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams,
                cacheMode);
#else
            DataCopyGM2UBImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams);
#endif
        }
    } else {
        repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T);
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
            DataCopySliceGm2UBImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams,
                cacheMode);
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
    const uint32_t dimValue = 1)
{
    uint32_t srcStartIndex = 0;
    uint32_t dstStartIndex = 0;
    uint32_t srcOffsetListSize = 0;
    uint32_t dstOffsetListSize = 0;
    uint32_t srcShapeInfo[K_MAX_SHAPE_DIM];
    uint32_t dstShapeInfo[K_MAX_SHAPE_DIM];
    for (uint32_t i = 0; i < dimValue; i++) {
        srcShapeInfo[i] = srcSliceInfo[i].shapeValue;
        dstShapeInfo[i] = dstSliceInfo[i].shapeValue;
    }
    srcStartIndex = DataCopyGetPhyStartIndex(srcSliceInfo, srcShapeInfo, dimValue);
    dstStartIndex = DataCopyGetPhyStartIndex(dstSliceInfo, dstShapeInfo, dimValue);
    uint32_t dstOffsetList[MAX_SLICE_SIZE];
    uint32_t srcOffsetList[MAX_SLICE_SIZE];
    DataCopyGetOffsetList(srcSliceInfo, srcShapeInfo, dimValue, &srcOffsetListSize, srcOffsetList);
    DataCopyGetOffsetList(dstSliceInfo, dstShapeInfo, dimValue, &dstOffsetListSize, dstOffsetList);
    DataCopyParams repeatParams;
    repeatParams.blockLen = srcSliceInfo[0].burstLen;
    uint32_t oneSliceLen = srcSliceInfo[0].burstLen * AscendCUtils::GetC0Count(sizeof(T)) + srcSliceInfo[0].stride;
    repeatParams.blockCount =
        (srcSliceInfo[0].endIndex - srcSliceInfo[0].startIndex + 1 + srcSliceInfo[0].stride) / oneSliceLen;
    repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    const uint8_t cacheMode = ExtractCacheMode(dst);
#endif
    if ((dstSliceInfo[0].stride * sizeof(T)) % AscendCUtils::GetC0Size() == 0) {
        repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
            DataCopyUB2GMImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams,
                cacheMode);
#else
            DataCopyUB2GMImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams);
#endif
        }
    } else {
        repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T);
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
            DataCopySliceUB2GMImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams,
                cacheMode);
#else
            DataCopySliceUB2GMImpl(
                dst + dstStartIndex + dstOffsetList[i], src + srcStartIndex + srcOffsetList[i], repeatParams);
#endif
        }
    }
}

/* DataCopyL1ToUB */
template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
#if (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510))
    using PrimType = PrimT<T>;
    DataCopyL12UBImpl<PrimType, subBlockId>((__ubuf__ PrimType*)dst, (__cbuf__ PrimType*)src, repeatParams);
#else
    (void)dst;
    (void)src;
    (void)repeatParams;
#endif
}

template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const uint32_t count)
{
#if (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510))
    using PrimType = PrimT<T>;
    DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(PrimType));
    DataCopyL1ToUB<T, subBlockId>(dst, src, repeatParams);
#else
    (void)dst;
    (void)src;
    (void)count;
#endif
}

/* DataCopyPad */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    using PrimType = PrimT<T>;
    DataCopyPadGm2UBImpl<PrimType, mode>(
        (__ubuf__ PrimType*)dst, (__gm__ PrimType*)src, dataCopyParams, padParams, ExtractCacheMode(src));
}

template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    using PrimType = PrimT<T>;
    DataCopyPadGm2L1Impl<PrimType, mode>(
        (__cbuf__ PrimType*)dst, (__gm__ PrimType*)src, dataCopyParams, padParams, ExtractCacheMode(src));
}

template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams)
{
    using PrimType = PrimT<T>;
    DataCopyPadUB2GMImpl<T, mode>(
        (__gm__ PrimType*)dst, (__ubuf__ PrimType*)src, dataCopyParams, ExtractCacheMode(dst));
}
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    using PrimType = PrimT<T>;
    DataCopyPadGm2UBImpl((__ubuf__ PrimType*)dst, (__gm__ PrimType*)src, dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams)
{
    using PrimType = PrimT<T>;
    DataCopyPadUB2GMImpl((__gm__ PrimType*)dst, (__ubuf__ PrimType*)src, dataCopyParams);
}
#endif

template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    using PrimType = PrimT<T>;
    DataCopyPadUB2L1Impl((__cbuf__ PrimType*)dst, (__ubuf__ PrimType*)src, dataCopyParams, nd2nzParams);
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
{
    DataCopyPadGm2UBImpl<T, mode>(dst, src, dataCopyParams, padParams, ExtractCacheMode(src));
}

template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
{
    DataCopyPadGm2L1Impl<T, mode>(dst, src, dataCopyParams, padParams, ExtractCacheMode(src));
}

template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams)
{
    using PrimType = PrimT<T>;
    DataCopyPadUB2GMImpl<T, mode>(
        (__gm__ PrimType*)dst, (__ubuf__ PrimType*)src, dataCopyParams, ExtractCacheMode(dst));
}
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
    using PrimType = PrimT<T>;
    DataCopyPadUB2GMImpl((__gm__ PrimType*)dst, (__ubuf__ PrimType*)src, dataCopyParams);
}
#endif

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, U>::value && (!Std::is_same<T, U>::value), bool>::type = true>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<U>& padParams)
{
    using PrimType = PrimT<T>;
    DataCopyPadGm2UBImpl((__ubuf__ PrimType*)dst, (__gm__ PrimType*)src, dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    using PrimType = PrimT<T>;
    DataCopyPadUB2L1Impl((__cbuf__ PrimType*)dst, (__ubuf__ PrimType*)src, dataCopyParams, nd2nzParams);
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename T, uint8_t dim, const NdDmaConfig& config = kDefaultNdDmaConfig>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __gm__ T* src, const MultiCopyParams<T, dim>& params)
{
    DataCopyWithNDDMAImpl<T, dim, config>(
        (__ubuf__ PrimT<T>*)dst, (__gm__ PrimT<T>*)src, params.loopInfo, params.constantValue, ExtractCacheMode(src));
}
#endif

#endif // !ASCENDC_CPU_DEBUG
