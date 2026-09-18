/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with
 * the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_data_copy_ptr_impl.h
 * \brief 搬运类接口指针化扩展实现：指针入参重载与既有 Tensor 入参重载复用同一套底层 *Impl，
 *        通过 GetUnderlyingPtr 统一萃取底层硬件指针后下发，保证两条路径语义与数值结果一致。
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_data_copy_ptr_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_data_copy_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_PTR_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_IMPL_H

#include "kernel_utils_underlying_ptr.h"
#include "kernel_operator_data_copy_intf_impl.h"

/*
 * 地址空间限定符仅在设备编译器（ccec，定义 __CCE__）下真实生效；宿主机侧这些宏展开为空，
 * 仅按地址空间区分的同形重载会塌缩为同一签名。与 ptr_intf.h 保持一致：
 * 宿主机侧每组同形重载仅保留一个代表（GM -> UB 搬入方向），其余方向为设备编译专用。
 */
#if defined(__CCE__)
#ifndef ASCENDC_PTR_AS_REAL
#define ASCENDC_PTR_AS_REAL 1
#endif
#endif

namespace AscendC {
/* **************************************************************************************************
 * DataCopy 指针化扩展（Pointer 输入）                                        *
 * ************************************************************************************************* */
/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（GM -> UB 指针入参版本）
 */
template <typename T>
__aicore__ inline void __inout_pipe__(MTE2) DataCopy(__ubuf__ T* dst, __gm__ T* src, const DataCopyParams& repeatParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    const uint8_t cacheMode = ExtractCacheMode(srcPtr);
    DataCopyGM2UBImpl(dstPtr, srcPtr, repeatParams, cacheMode);
#else
    DataCopyGM2UBImpl(dstPtr, srcPtr, repeatParams);
#endif
}

/*
 * @ingroup DataCopy Level 2
 * @brief datacopy from src to dst（GM -> UB 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const uint32_t count)
{
    struct DataCopyParams repeatParams;
    DataCopyCheck<T>(count, repeatParams);
    DataCopy(dst, src, repeatParams);
}

/*
 * @ingroup DataCopy Level 1
 * @brief datacopy from src to dst（GM -> UB SliceInfo 指针入参版本）
 * @par 说明：指针路径没有 Tensor 形状信息可回退，要求 srcSliceInfo[0].shapeValue 非 0。
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, __gm__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue)
{
    using PrimType = PrimT<T>;
    static_assert(Std::is_same<PrimType, T>::value, "TensorTrait is not supported by DataCopy with SliceInfo!");
    ASCENDC_ASSERT((srcSliceInfo[0].shapeValue != 0), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "DataCopy with SliceInfo pointer overload requires non-zero srcSliceInfo[0].shapeValue, because pointer "
            "input has no tensor shape to fall back on.");
    });
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
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
    struct DataCopyParams repeatParams;
    repeatParams.blockLen = srcSliceInfo[0].burstLen;
    uint32_t oneSliceLen = srcSliceInfo[0].burstLen * AscendCUtils::GetC0Count(sizeof(T)) + srcSliceInfo[0].stride;
    repeatParams.blockCount =
        (srcSliceInfo[0].endIndex - srcSliceInfo[0].startIndex + 1 + srcSliceInfo[0].stride) / oneSliceLen;
    repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    const uint8_t cacheMode = ExtractCacheMode(srcPtr);
#endif
    if ((srcSliceInfo[0].stride * sizeof(T)) % AscendCUtils::GetC0Size() == 0) {
        repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
            DataCopyGM2UBImpl(
                dstPtr + dstStartIndex + dstOffsetList[i], srcPtr + srcStartIndex + srcOffsetList[i], repeatParams,
                cacheMode);
#else
            DataCopyGM2UBImpl(
                dstPtr + dstStartIndex + dstOffsetList[i], srcPtr + srcStartIndex + srcOffsetList[i], repeatParams);
#endif
        }
    } else {
        repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T);
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
            DataCopySliceGm2UBImpl(
                dstPtr + dstStartIndex + dstOffsetList[i], srcPtr + srcStartIndex + srcOffsetList[i], repeatParams,
                cacheMode);
#else
            DataCopySliceGm2UBImpl(
                dstPtr + dstStartIndex + dstOffsetList[i], srcPtr + srcStartIndex + srcOffsetList[i], repeatParams);
#endif
        }
    }
}

/*
 * @ingroup DataCopy Level 0
 * @brief format transform(such as nd2nz) during data load from OUT to UB（GM -> UB 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || \
    (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    CheckNd2NzParams(intriParams, "DataCopy with Nd2NzParams");
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    const uint8_t cacheMode = ExtractCacheMode(srcPtr);
    DataCopyGM2UBND2NZImpl(dstPtr, srcPtr, intriParams, cacheMode);
}
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst with N-dimensional DMA（GM -> UB 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename T, uint8_t dim, const NdDmaConfig& config>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __gm__ T* src, const MultiCopyParams<T, dim>& params)
{
#if (__NPU_ARCH__ != 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    const uint8_t cacheMode = ExtractCacheMode(srcPtr);
    DataCopyWithNDDMAImpl<T, dim, config>(dstPtr, srcPtr, params.loopInfo, params.constantValue, cacheMode);
}
#endif

/*
 * @ingroup DataCopy
 * @brief datacopy from src to dst, applicable to cube data（GM -> UB 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopy(dst, src, intriParams);
}

#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（GM -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const DataCopyParams& repeatParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    const uint8_t cacheMode = ExtractCacheMode(srcPtr);
    DataCopyGM2L1Impl(dstPtr, srcPtr, repeatParams, cacheMode);
#else
    DataCopyGM2L1Impl(dstPtr, srcPtr, repeatParams);
#endif
}
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief format transform(such as nd2nz) during data load from OUT to L1（GM -> L1 指针入参版本）
 * @par 说明：与 GM -> UB Nd2Nz 版仅地址空间限定不同，宿主机侧塌缩为同签名且无法偏序，设备编译专用。
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || \
    (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
#if defined(ASCENDC_PTR_AS_REAL)
template <typename T, bool enableSmallC0>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    CheckNd2NzParams(intriParams, "DataCopy with Nd2NzParams");
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    const uint8_t cacheMode = ExtractCacheMode(srcPtr);
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
    DataCopyGM2L1ND2NZImpl<T, enableSmallC0>(dstPtr, srcPtr, intriParams, cacheMode);
#else
    static_assert(!enableSmallC0, "enableSmallC0 is not supported on current device.");
    DataCopyGM2L1ND2NZImpl(dstPtr, srcPtr, intriParams, cacheMode);
#endif
}
#endif
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief format transform(such as dn2nz) during data load from OUT to L1（GM -> L1 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, bool enableSmallC0>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    const uint8_t cacheMode = ExtractCacheMode(srcPtr);
    DataCopyGM2L1DN2NZImpl(dstPtr, srcPtr, intriParams, enableSmallC0, cacheMode);
}
#endif

#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopy
 * @brief datacopy from src to dst, applicable to cube data（GM -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopy(dst, src, intriParams);
}

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（UB -> GM 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    const uint8_t cacheMode = ExtractCacheMode(dstPtr);
    DataCopyUB2GMImpl(dstPtr, srcPtr, repeatParams, cacheMode);
#else
    DataCopyUB2GMImpl(dstPtr, srcPtr, repeatParams);
#endif
}

/*
 * @ingroup DataCopy Level 2
 * @brief datacopy from src to dst（UB -> GM 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    struct DataCopyParams repeatParams;
    DataCopyCheck<T>(count, repeatParams);
    DataCopy(dst, src, repeatParams);
}
#endif

#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopy Level 1
 * @brief datacopy from src to dst（UB -> GM SliceInfo 指针入参版本）
 * @par 说明：指针路径没有 Tensor 形状信息可回退，要求 srcSliceInfo[0].shapeValue 非 0。
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue)
{
    using PrimType = PrimT<T>;
    static_assert(Std::is_same<PrimType, T>::value, "TensorTrait is not supported by DataCopy with SliceInfo!");
    ASCENDC_ASSERT((srcSliceInfo[0].shapeValue != 0), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "DataCopy with SliceInfo pointer overload requires non-zero srcSliceInfo[0].shapeValue, because pointer "
            "input has no tensor shape to fall back on.");
    });
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
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

    struct DataCopyParams repeatParams;
    repeatParams.blockLen = srcSliceInfo[0].burstLen;
    uint32_t oneSliceLen = srcSliceInfo[0].burstLen * AscendCUtils::GetC0Count(sizeof(T)) + srcSliceInfo[0].stride;
    repeatParams.blockCount =
        (srcSliceInfo[0].endIndex - srcSliceInfo[0].startIndex + 1 + srcSliceInfo[0].stride) / oneSliceLen;
    repeatParams.srcStride = srcSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    const uint8_t cacheMode = ExtractCacheMode(dstPtr);
#endif
    if ((dstSliceInfo[0].stride * sizeof(T)) % AscendCUtils::GetC0Size() == 0) {
        repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T) / AscendCUtils::GetC0Size();
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
            DataCopyUB2GMImpl(
                dstPtr + dstStartIndex + dstOffsetList[i], srcPtr + srcStartIndex + srcOffsetList[i], repeatParams,
                cacheMode);
#else
            DataCopyUB2GMImpl(
                dstPtr + dstStartIndex + dstOffsetList[i], srcPtr + srcStartIndex + srcOffsetList[i], repeatParams);
#endif
        }
    } else {
        repeatParams.dstStride = dstSliceInfo[0].stride * sizeof(T);
        for (uint32_t i = 0; i < srcOffsetListSize; i++) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
            DataCopySliceUB2GMImpl(
                dstPtr + dstStartIndex + dstOffsetList[i], srcPtr + srcStartIndex + srcOffsetList[i], repeatParams,
                cacheMode);
#else
            DataCopySliceUB2GMImpl(
                dstPtr + dstStartIndex + dstOffsetList[i], srcPtr + srcStartIndex + srcOffsetList[i], repeatParams);
#endif
        }
    }
}
#endif

/*
 * @ingroup DataCopy Level 2
 * @brief datacopy from src to dst, nz2nd, applicable to simulated cube data（UB -> GM 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const Nz2NdParamsFull& intriParams)
{
    CheckNz2NdParams(intriParams);
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    const uint8_t cacheMode = ExtractCacheMode(dstPtr);
    DataCopyUB2GMNZ2NDImpl(dstPtr, srcPtr, intriParams, cacheMode);
#else
    DataCopyUB2GMNZ2NDImpl(dstPtr, srcPtr, intriParams);
#endif
}

#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopy
 * @brief datacopy from src to dst, applicable to cube data（UB -> GM 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    (void)enhancedParams;
    DataCopy(dst, src, intriParams);
}

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（L1 -> GM 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    const uint8_t cacheMode = ExtractCacheMode(dstPtr);
    DataCopyL12GMImpl(dstPtr, srcPtr, repeatParams, cacheMode);
#else
    DataCopyL12GMImpl(dstPtr, srcPtr, repeatParams);
#endif
}

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（UB -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    DataCopyUB2L1Impl(dstPtr, srcPtr, repeatParams);
}

/*
 * @ingroup DataCopy Level 2
 * @brief datacopy from src to dst（UB -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    ASCENDC_ASSERT((count % AscendCUtils::GetC0Count(sizeof(T)) == 0), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to "
            "check count value in DataCopy from UB to L1, count * sizeof(T) must be 32B align, "
            "current count value is %u. In NPU mode, no error is reported. The value is rounded down by 32B.",
            count);
    });
    struct DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(T));
    DataCopy(dst, src, repeatParams);
}

/*
 * @ingroup DataCopy Level 0
 * @brief format transform(such as nd2nz) during data load from UB to L1（UB -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const Nd2NzParams& intriParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    DataCopyUB2L1ND2NZImpl(dstPtr, srcPtr, intriParams);
}
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（UB -> UB 指针入参版本）
 */
#if defined(ASCENDC_PTR_AS_REAL)
template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    DataCopyUB2UBImpl(dstPtr, srcPtr, repeatParams);
}

/*
 * @ingroup DataCopy Level 2
 * @brief datacopy from src to dst（UB -> UB 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    ASCENDC_ASSERT((count % AscendCUtils::GetC0Count(sizeof(T)) == 0), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to "
            "check count value in DataCopy from UB to UB, count * sizeof(T) must be 32B align, "
            "current count value is %u. In NPU mode, no error is reported. The value is rounded down by 32B.",
            count);
    });
    struct DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(T));
    DataCopy(dst, src, repeatParams);
}
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief fixpipe datacopy from L0C to GM（L0C -> GM 指针入参版本）
 */
template <typename T, typename U>
__aicore__ inline void DataCopy(__gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    const uint8_t cacheMode = ExtractCacheMode(dstPtr);
    DataCopyL0C2GMImpl(dstPtr, srcPtr, intriParams, cacheMode);
#else
    DataCopyL0C2GMImpl(dstPtr, srcPtr, intriParams);
#endif
}

#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from L0C to L1（L0C -> L1 指针入参版本）
 */
template <typename T, typename U>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    DataCopyL0C2L1Impl(dstPtr, srcPtr, intriParams);
}
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from L1 to bt, applicable to vector data（C1 -> C2 BiasTable 指针入参版本）
 * @par 说明：指针路径无 Tensor 位置信息，C1 -> C2 BiasTable 固定走 DataCopyL12BTImpl；
 *            C2PIPE2GM（Fixpipe Buffer）目的地址请使用 __fbuf__ 限定指针重载。
 */
template <typename T, typename U>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __cbuf__ U* src, const DataCopyParams& repeatParams)
{
    using PrimDstType = PrimT<T>;
    using PrimSrcType = PrimT<U>;
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    CheckAddrAlignment(
        reinterpret_cast<uint64_t>(dstPtr), Hardware::BIAS, 64, "dst",
        "DataCopy from L1 Buffer(C1) to BiasTable Buffer(C2)"); // 64B align
    CheckAddrAlignment(
        reinterpret_cast<uint64_t>(srcPtr), Hardware::L1, ONE_BLK_SIZE, "src",
        "DataCopy from L1 Buffer(C1) to BiasTable Buffer(C2)"); // 32B align
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5102)
    if constexpr (
        Std::is_same<PrimDstType, int32_t>::value &&
            (Std::is_same<PrimSrcType, half>::value || Std::is_same<PrimSrcType, bfloat16_t>::value ||
             Std::is_same<PrimSrcType, float>::value) ||
        Std::is_same<PrimSrcType, int16_t>::value) {
        DataCopyL12BTImpl((uint64_t)dstPtr, (__cbuf__ PrimSrcType*)srcPtr, (uint16_t)2, repeatParams);
        return;
    }
#endif
    if constexpr (Std::is_same<PrimSrcType, bfloat16_t>::value && (!Std::is_same<PrimDstType, float>::value)) {
        ASCENDC_ASSERT((false), {
            KERNEL_LOG(
                KERNEL_ERROR, "unsupported case where src dtype is bfloat16, dst dtype is not float on current device");
        });
    } else if constexpr (
        Std::is_same<PrimDstType, PrimSrcType>::value ||
        (Std::is_same<PrimSrcType, bfloat16_t>::value && Std::is_same<PrimDstType, float>::value)) {
#else
#if defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
    if constexpr (
        Std::is_same<PrimDstType, int32_t>::value &&
        (Std::is_same<PrimSrcType, half>::value || Std::is_same<PrimSrcType, float>::value ||
         Std::is_same<PrimSrcType, int32_t>::value)) {
        DataCopyL12BTImpl((uint64_t)dstPtr, (__cbuf__ PrimSrcType*)srcPtr, (uint16_t)2, repeatParams);
    } else if constexpr (Std::is_same<PrimDstType, PrimSrcType>::value) {
#else
    if constexpr (Std::is_same<PrimDstType, PrimSrcType>::value) {
#endif
#endif
        DataCopyL12BTImpl((uint64_t)dstPtr, (__cbuf__ PrimSrcType*)srcPtr, static_cast<uint16_t>(0), repeatParams);
    } else if constexpr (Std::is_same<PrimDstType, float>::value && Std::is_same<PrimSrcType, half>::value) {
        DataCopyL12BTImpl((uint64_t)dstPtr, (__cbuf__ half*)srcPtr, static_cast<uint16_t>(1), repeatParams);
    } else {
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(
                KERNEL_ERROR, "Failed to check dtype in DataCopy from L1 Buffer(C1) to BiasTable Buffer(C2), "
                              "current api support dtype combination is U = T or src: half, dst: float.");
        });
    }
}

#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3510) || \
     (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5161) || \
     (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from L1 to Fixpipe Buffer C2PIPE2GM（C1 -> C2PIPE2GM 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopy(__fbuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    CheckAddrAlignment(
        reinterpret_cast<uint64_t>(dstPtr), Hardware::FIXBUF, 128, "dst",
        "DataCopy from L1 Buffer(A1/B1/C1) to Fixpipe Buffer(C2PIPE2GM)"); // 128B align
    CheckAddrAlignment(
        reinterpret_cast<uint64_t>(srcPtr), Hardware::L1, ONE_BLK_SIZE, "src",
        "DataCopy from L1 Buffer(A1/B1/C1) to Fixpipe Buffer(C2PIPE2GM)"); // 32B align
    DataCopyL12FBImpl((__fbuf__ PrimT<T>*)dstPtr, (__cbuf__ PrimT<T>*)srcPtr, repeatParams);
}
#endif
#endif

/* **************************************************************************************************
 * DataCopyPad 指针化扩展（Pointer 输入）                                       *
 * ************************************************************************************************* */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
/*
 * @ingroup DataCopyPad
 * @brief datacopy with padding from src to dst（GM -> UB 指针入参版本）
 */
template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    const uint8_t cacheMode = ExtractCacheMode(srcPtr);
    DataCopyPadGm2UBImpl<T, mode>(dstPtr, srcPtr, dataCopyParams, padParams, cacheMode);
}

#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopyPad
 * @brief datacopy with padding from src to dst（GM -> L1 指针入参版本）
 */
template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    const uint8_t cacheMode = ExtractCacheMode(srcPtr);
    DataCopyPadGm2L1Impl<T, mode>(dstPtr, srcPtr, dataCopyParams, padParams, cacheMode);
}
#endif

/*
 * @ingroup DataCopyPad
 * @brief datacopy with padding from src to dst（UB -> GM 指针入参版本）
 */
template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams)
{
#if (__NPU_ARCH__ != 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    const uint8_t cacheMode = ExtractCacheMode(dstPtr);
    DataCopyPadUB2GMImpl<T, mode>(dstPtr, srcPtr, dataCopyParams, cacheMode);
}
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    DataCopyPadGm2UBImpl(dstPtr, srcPtr, dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams)
{
    if ASCEND_IS_AIC {
        return;
    }
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    DataCopyPadUB2GMImpl(dstPtr, srcPtr, dataCopyParams);
}
#endif

// override DataCopyPad, use new param DataCopyExtParams
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || \
                              (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
/*
 * @ingroup DataCopyPad
 * @brief override DataCopyPad, use new param DataCopyExtParams（GM -> UB 指针入参版本）
 */
template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    const uint8_t cacheMode = ExtractCacheMode(srcPtr);
    DataCopyPadGm2UBImpl<T, mode>(dstPtr, srcPtr, dataCopyParams, padParams, cacheMode);
}

/*
 * @ingroup DataCopyPad
 * @brief override DataCopyPad, use new param DataCopyExtParams（UB -> GM 指针入参版本）
 */
template <typename T, PaddingMode mode>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams)
{
#if (__NPU_ARCH__ != 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    const uint8_t cacheMode = ExtractCacheMode(dstPtr);
    DataCopyPadUB2GMImpl<T, mode>(dstPtr, srcPtr, dataCopyParams, cacheMode);
}
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
{
    if ASCEND_IS_AIC {
        return;
    }
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    DataCopyPadGm2UBImpl(dstPtr, srcPtr, dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams)
{
    if ASCEND_IS_AIC {
        return;
    }
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    DataCopyPadUB2GMImpl(dstPtr, srcPtr, dataCopyParams);
}
#endif

/*
 * @ingroup DataCopyPad
 * @brief datacopy with padding and format transform(such as nd2nz) from UB to L1（UB -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    CheckNd2NzParams(nd2nzParams, "DataCopyPad with Nd2NzParams");
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    DataCopyPadUB2L1Impl(dstPtr, srcPtr, dataCopyParams, nd2nzParams);
}

/*
 * @ingroup DataCopyPad
 * @brief override DataCopyPad with Nd2NzParams, use new param DataCopyExtParams（UB -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    CheckNd2NzParams(nd2nzParams, "DataCopyPad with Nd2NzParams");
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    DataCopyPadUB2L1Impl(dstPtr, srcPtr, dataCopyParams, nd2nzParams);
}

/* **************************************************************************************************
 * DataCopyL1ToUB 指针化扩展（Pointer 输入）                                    *
 * ************************************************************************************************* */
template <typename T, uint8_t subBlockId>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
#if (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510))
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    ASCENDC_ASSERT(((reinterpret_cast<uint64_t>(dstPtr) % ONE_BLK_SIZE) == 0), {
        KERNEL_LOG(KERNEL_ERROR, "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT) requires 32B align dst address");
    });
    ASCENDC_ASSERT(((reinterpret_cast<uint64_t>(srcPtr) % ONE_BLK_SIZE) == 0), {
        KERNEL_LOG(KERNEL_ERROR, "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT) requires 32B align src address");
    });
    DataCopyL12UBImpl<T, subBlockId>(dstPtr, srcPtr, repeatParams);
#endif
}

template <typename T, uint8_t subBlockId>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const uint32_t count)
{
#if (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510))
    ASCENDC_ASSERT((count % AscendCUtils::GetC0Count(sizeof(T)) == 0), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to "
            "check count value in DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT), count * sizeof(T) must be 32B "
            "align, current count value is %u. In NPU mode, no error is reported. The value is rounded down by 32B.",
            count);
    });
    struct DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(T));
    DataCopyL1ToUB<T, subBlockId>(dst, src, repeatParams);
#endif
}

/* **************************************************************************************************
 * Copy 指针化扩展（Pointer 输入）                                          *
 * ************************************************************************************************* */
// Copy::Level 0 - mask bit mode
template <typename T, bool isSetMask>
__aicore__ inline __inout_pipe__(V) void Copy(
    __ubuf__ T* dst, __ubuf__ T* src, const uint64_t mask[], const uint8_t repeatTime,
    const CopyRepeatParams& repeatParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    CopyImpl<T, isSetMask>(dstPtr, srcPtr, mask, repeatTime, repeatParams);
}

// Copy::Level 0 - mask count mode
template <typename T, bool isSetMask>
__aicore__ inline __inout_pipe__(V) void Copy(
    __ubuf__ T* dst, __ubuf__ T* src, const uint64_t mask, const uint8_t repeatTime,
    const CopyRepeatParams& repeatParams)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    CopyImpl<T, isSetMask>(dstPtr, srcPtr, mask, repeatTime, repeatParams);
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
// Copy::Level 2 - count mode
template <typename T, bool isSetMask>
__aicore__ inline __inout_pipe__(V) void Copy(__ubuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    auto dstPtr = GetUnderlyingPtr(dst);
    auto srcPtr = GetUnderlyingPtr(src);
    CopyImpl<T>(dstPtr, srcPtr, count);
}
#endif
} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_PTR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_PTR_IMPL_H__
#endif
