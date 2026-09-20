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
 * \file kernel_operator_data_copy_mixed_impl.h
 * \brief Mixed Tensor/Pointer overloads for DMA DataCopy family.
 *        Thin adapters: wrap __gm__* as GlobalTensor, or GetPhyAddr to Pointer path.
 */
#ifndef ASCENDC_MODULE_OPERATOR_DATA_COPY_MIXED_IMPL_H
#define ASCENDC_MODULE_OPERATOR_DATA_COPY_MIXED_IMPL_H

#if defined(ASCENDC_ENABLE_HW_PTR_API)
namespace AscendC {

template <typename T>
__aicore__ inline GlobalTensor<T> AscendCMixedMakeGmTensor(__gm__ T* ptr)
{
    GlobalTensor<T> tensor;
    tensor.SetGlobalBuffer(ptr);
    return tensor;
}

template <typename T>
__aicore__ inline __gm__ T* AscendCMixedGmPtr(const GlobalTensor<T>& tensor)
{
    return (__gm__ T*)tensor.GetPhyAddr();
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(const LocalTensor<T>& dst, __gm__ T* src, const uint32_t count)
{
    GlobalTensor<T> srcTensor;
    srcTensor.SetGlobalBuffer(src);
    DataCopy(dst, srcTensor, count);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, const GlobalTensor<T>& src, const uint32_t count)
{
    DataCopy(dst, AscendCMixedGmPtr(src), count);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, const GlobalTensor<T>& src, const uint32_t count)
{
    DataCopy(dst, AscendCMixedGmPtr(src), count);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, const LocalTensor<T>& src, const uint32_t count)
{
    GlobalTensor<T> dstTensor;
    dstTensor.SetGlobalBuffer(dst);
    DataCopy(dstTensor, src, count);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(const GlobalTensor<T>& dst, __ubuf__ T* src, const uint32_t count)
{
    DataCopy(AscendCMixedGmPtr(dst), src, count);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(const GlobalTensor<T>& dst, __cbuf__ T* src, const uint32_t count)
{
    DataCopy(AscendCMixedGmPtr(dst), src, count);
}

template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, __ubuf__ T* src, const uint32_t count)
{
    using Prim = PrimT<T>;
    const Hardware dstHW = GetPhyType((TPosition)dst.GetPosition());
    if (dstHW == Hardware::UB) {
        DataCopy((__ubuf__ Prim*)dst.GetPhyAddr(), src, count);
    } else if (dstHW == Hardware::L1) {
        DataCopy((__cbuf__ Prim*)dst.GetPhyAddr(), src, count);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "dst", "UB(VECIN/VECOUT)/L1 Buffer(A1/B1)", "DataCopy Mixed LocalTensor <- __ubuf__",
            GetPositionDisplay(static_cast<TPosition>(dst.GetPosition())));
    }
}

template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, __cbuf__ T* src, const uint32_t count)
{
    using Prim = PrimT<T>;
    const Hardware dstHW = GetPhyType((TPosition)dst.GetPosition());
    if (dstHW == Hardware::UB) {
        DataCopy((__ubuf__ Prim*)dst.GetPhyAddr(), src, count);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "dst", "UB(VECIN/VECOUT)", "DataCopy Mixed LocalTensor <- __cbuf__",
            GetPositionDisplay(static_cast<TPosition>(dst.GetPosition())));
    }
}

template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, const LocalTensor<T>& src, const uint32_t count)
{
    using Prim = PrimT<T>;
    const Hardware srcHW = GetPhyType((TPosition)src.GetPosition());
    if (srcHW == Hardware::UB) {
        DataCopy(dst, (__ubuf__ Prim*)src.GetPhyAddr(), count);
    } else if (srcHW == Hardware::L1) {
        DataCopy(dst, (__cbuf__ Prim*)src.GetPhyAddr(), count);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "src", "UB/L1", "DataCopy Mixed __ubuf__ <- LocalTensor",
            GetPositionDisplay(static_cast<TPosition>(src.GetPosition())));
    }
}

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, const LocalTensor<T>& src, const uint32_t count)
{
    using Prim = PrimT<T>;
    const Hardware srcHW = GetPhyType((TPosition)src.GetPosition());
    if (srcHW == Hardware::UB) {
        DataCopy(dst, (__ubuf__ Prim*)src.GetPhyAddr(), count);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "src", "UB(VECIN/VECOUT)", "DataCopy Mixed __cbuf__ <- LocalTensor",
            GetPositionDisplay(static_cast<TPosition>(src.GetPosition())));
    }
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    const LocalTensor<T>& dst, __gm__ T* src, const DataCopyParams& repeatParams)
{
    GlobalTensor<T> srcTensor;
    srcTensor.SetGlobalBuffer(src);
    DataCopy(dst, srcTensor, repeatParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, const GlobalTensor<T>& src, const DataCopyParams& repeatParams)
{
    DataCopy(dst, AscendCMixedGmPtr(src), repeatParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __cbuf__ T* dst, const GlobalTensor<T>& src, const DataCopyParams& repeatParams)
{
    DataCopy(dst, AscendCMixedGmPtr(src), repeatParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
{
    GlobalTensor<T> dstTensor;
    dstTensor.SetGlobalBuffer(dst);
    DataCopy(dstTensor, src, repeatParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    const GlobalTensor<T>& dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    DataCopy(AscendCMixedGmPtr(dst), src, repeatParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    const GlobalTensor<T>& dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    DataCopy(AscendCMixedGmPtr(dst), src, repeatParams);
}

template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, __ubuf__ T* src, const DataCopyParams& repeatParams)
{
    using Prim = PrimT<T>;
    const Hardware dstHW = GetPhyType((TPosition)dst.GetPosition());
    if (dstHW == Hardware::UB) {
        DataCopy((__ubuf__ Prim*)dst.GetPhyAddr(), src, repeatParams);
    } else if (dstHW == Hardware::L1) {
        DataCopy((__cbuf__ Prim*)dst.GetPhyAddr(), src, repeatParams);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "dst", "UB/L1", "DataCopy Mixed Params LocalTensor <- __ubuf__",
            GetPositionDisplay(static_cast<TPosition>(dst.GetPosition())));
    }
}

template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    using Prim = PrimT<T>;
    const Hardware dstHW = GetPhyType((TPosition)dst.GetPosition());
    if (dstHW == Hardware::UB) {
        DataCopy((__ubuf__ Prim*)dst.GetPhyAddr(), src, repeatParams);
    } else if (dstHW == Hardware::L1) {
        DataCopy((__cbuf__ Prim*)dst.GetPhyAddr(), src, repeatParams);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "dst", "UB/L1", "DataCopy Mixed Params LocalTensor <- __cbuf__",
            GetPositionDisplay(static_cast<TPosition>(dst.GetPosition())));
    }
}

template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
{
    using Prim = PrimT<T>;
    const Hardware srcHW = GetPhyType((TPosition)src.GetPosition());
    if (srcHW == Hardware::UB) {
        DataCopy(dst, (__ubuf__ Prim*)src.GetPhyAddr(), repeatParams);
    } else if (srcHW == Hardware::L1) {
        DataCopy(dst, (__cbuf__ Prim*)src.GetPhyAddr(), repeatParams);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "src", "UB/L1", "DataCopy Mixed Params __ubuf__ <- LocalTensor",
            GetPositionDisplay(static_cast<TPosition>(src.GetPosition())));
    }
}

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
{
    using Prim = PrimT<T>;
    const Hardware srcHW = GetPhyType((TPosition)src.GetPosition());
    if (srcHW == Hardware::UB) {
        DataCopy(dst, (__ubuf__ Prim*)src.GetPhyAddr(), repeatParams);
    } else if (srcHW == Hardware::L1) {
        DataCopy(dst, (__cbuf__ Prim*)src.GetPhyAddr(), repeatParams);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "src", "UB/L1", "DataCopy Mixed Params __cbuf__ <- LocalTensor",
            GetPositionDisplay(static_cast<TPosition>(src.GetPosition())));
    }
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    const LocalTensor<T>& dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
{
    GlobalTensor<T> srcTensor;
    srcTensor.SetGlobalBuffer(src);
    DataCopyPad(dst, srcTensor, dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, const GlobalTensor<T>& src, const DataCopyParams& dataCopyParams,
    const DataCopyPadParams& padParams)
{
    DataCopyPad(dst, AscendCMixedGmPtr(src), dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, const GlobalTensor<T>& src, const DataCopyParams& dataCopyParams,
    const DataCopyPadParams& padParams)
{
    DataCopyPad(dst, AscendCMixedGmPtr(src), dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, const LocalTensor<T>& src, const DataCopyParams& dataCopyParams)
{
    GlobalTensor<T> dstTensor;
    dstTensor.SetGlobalBuffer(dst);
    DataCopyPad(dstTensor, src, dataCopyParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    const GlobalTensor<T>& dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams)
{
    DataCopyPad(AscendCMixedGmPtr(dst), src, dataCopyParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    const LocalTensor<T>& dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams,
    const DataCopyPadExtParams<T>& padParams)
{
    GlobalTensor<T> srcTensor;
    srcTensor.SetGlobalBuffer(src);
    DataCopyPad(dst, srcTensor, dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams,
    const DataCopyPadExtParams<T>& padParams)
{
    DataCopyPad(dst, AscendCMixedGmPtr(src), dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams,
    const DataCopyPadExtParams<T>& padParams)
{
    DataCopyPad(dst, AscendCMixedGmPtr(src), dataCopyParams, padParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams)
{
    GlobalTensor<T> dstTensor;
    dstTensor.SetGlobalBuffer(dst);
    DataCopyPad(dstTensor, src, dataCopyParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    const GlobalTensor<T>& dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams)
{
    DataCopyPad(AscendCMixedGmPtr(dst), src, dataCopyParams);
}

template <typename T, bool enableSmallC0 = false>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    const LocalTensor<T>& dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    GlobalTensor<T> srcTensor;
    srcTensor.SetGlobalBuffer(src);
    if constexpr (enableSmallC0) {
        DataCopy<T, true>(dst, srcTensor, intriParams);
    } else {
        DataCopy(dst, srcTensor, intriParams);
    }
}

template <typename T, bool enableSmallC0 = false>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __cbuf__ T* dst, const GlobalTensor<T>& src, const Nd2NzParams& intriParams)
{
    if constexpr (enableSmallC0) {
        DataCopy<T, true>(dst, AscendCMixedGmPtr(src), intriParams);
    } else {
        DataCopy(dst, AscendCMixedGmPtr(src), intriParams);
    }
}

template <typename T, bool enableSmallC0 = false>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, const GlobalTensor<T>& src, const Nd2NzParams& intriParams)
{
    if constexpr (enableSmallC0) {
        DataCopy<T, true>(dst, AscendCMixedGmPtr(src), intriParams);
    } else {
        DataCopy(dst, AscendCMixedGmPtr(src), intriParams);
    }
}

template <typename T, bool enableSmallC0 = false>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    const LocalTensor<T>& dst, __gm__ T* src, const Dn2NzParams& intriParams)
{
    GlobalTensor<T> srcTensor;
    srcTensor.SetGlobalBuffer(src);
    if constexpr (enableSmallC0) {
        DataCopy<T, true>(dst, srcTensor, intriParams);
    } else {
        DataCopy(dst, srcTensor, intriParams);
    }
}

template <typename T, bool enableSmallC0 = false>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __cbuf__ T* dst, const GlobalTensor<T>& src, const Dn2NzParams& intriParams)
{
    if constexpr (enableSmallC0) {
        DataCopy<T, true>(dst, AscendCMixedGmPtr(src), intriParams);
    } else {
        DataCopy(dst, AscendCMixedGmPtr(src), intriParams);
    }
}

template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, __ubuf__ T* src, const Nd2NzParams& intriParams)
{
    using Prim = PrimT<T>;
    const Hardware dstHW = GetPhyType((TPosition)dst.GetPosition());
    if (dstHW == Hardware::L1) {
        DataCopy((__cbuf__ Prim*)dst.GetPhyAddr(), src, intriParams);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "dst", "L1 Buffer(A1/B1)", "DataCopy Mixed Nd2Nz LocalTensor <- __ubuf__",
            GetPositionDisplay(static_cast<TPosition>(dst.GetPosition())));
    }
}

template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, const LocalTensor<T>& src, const Nd2NzParams& intriParams)
{
    using Prim = PrimT<T>;
    const Hardware srcHW = GetPhyType((TPosition)src.GetPosition());
    if (srcHW == Hardware::UB) {
        DataCopy(dst, (__ubuf__ Prim*)src.GetPhyAddr(), intriParams);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "src", "UB(VECIN/VECOUT)", "DataCopy Mixed Nd2Nz __cbuf__ <- LocalTensor",
            GetPositionDisplay(static_cast<TPosition>(src.GetPosition())));
    }
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    const LocalTensor<T>& dst, __gm__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue = 1)
{
    GlobalTensor<T> srcTensor;
    srcTensor.SetGlobalBuffer(src);
    DataCopy(dst, srcTensor, dstSliceInfo, srcSliceInfo, dimValue);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, const GlobalTensor<T>& src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue = 1)
{
    DataCopy(dst, AscendCMixedGmPtr(src), dstSliceInfo, srcSliceInfo, dimValue);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, const LocalTensor<T>& src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue = 1)
{
    GlobalTensor<T> dstTensor;
    dstTensor.SetGlobalBuffer(dst);
    DataCopy(dstTensor, src, dstSliceInfo, srcSliceInfo, dimValue);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    const GlobalTensor<T>& dst, __ubuf__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue = 1)
{
    DataCopy(AscendCMixedGmPtr(dst), src, dstSliceInfo, srcSliceInfo, dimValue);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, const LocalTensor<T>& src, const Nz2NdParamsFull& intriParams)
{
    GlobalTensor<T> dstTensor;
    dstTensor.SetGlobalBuffer(dst);
    DataCopy(dstTensor, src, intriParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    const GlobalTensor<T>& dst, __ubuf__ T* src, const Nz2NdParamsFull& intriParams)
{
    DataCopy(AscendCMixedGmPtr(dst), src, intriParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    const GlobalTensor<T>& dst, __cbuf__ T* src, const Nz2NdParamsFull& intriParams)
{
    DataCopy(AscendCMixedGmPtr(dst), src, intriParams);
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename T, uint8_t dim, const NdDmaConfig& config = kDefaultNdDmaConfig>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, __gm__ T* src, const MultiCopyParams<T, dim>& params)
{
    GlobalTensor<T> srcTensor;
    srcTensor.SetGlobalBuffer(src);
    DataCopy<T, dim, config>(dst, srcTensor, params);
}

template <typename T, uint8_t dim, const NdDmaConfig& config = kDefaultNdDmaConfig>
__aicore__ inline void DataCopy(__ubuf__ T* dst, const GlobalTensor<T>& src, const MultiCopyParams<T, dim>& params)
{
    DataCopy<T, dim, config>(dst, AscendCMixedGmPtr(src), params);
}
#endif

template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, __cbuf__ T* src, const uint32_t count)
{
    using Prim = PrimT<T>;
    DataCopyL1ToUB<Prim, subBlockId>((__ubuf__ Prim*)dst.GetPhyAddr(), src, count);
}

template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, const LocalTensor<T>& src, const uint32_t count)
{
    using Prim = PrimT<T>;
    DataCopyL1ToUB<Prim, subBlockId>(dst, (__cbuf__ Prim*)src.GetPhyAddr(), count);
}

template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, __cbuf__ T* src, const DataCopyParams& repeatParams)
{
    using Prim = PrimT<T>;
    DataCopyL1ToUB<Prim, subBlockId>((__ubuf__ Prim*)dst.GetPhyAddr(), src, repeatParams);
}

template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
{
    using Prim = PrimT<T>;
    DataCopyL1ToUB<Prim, subBlockId>(dst, (__cbuf__ Prim*)src.GetPhyAddr(), repeatParams);
}

template <typename T>
__aicore__ inline void DataCopyPad(
    const LocalTensor<T>& dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    using Prim = PrimT<T>;
    DataCopyPad((__cbuf__ Prim*)dst.GetPhyAddr(), src, dataCopyParams, nd2nzParams);
}

template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, const LocalTensor<T>& src, const DataCopyParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    using Prim = PrimT<T>;
    DataCopyPad(dst, (__ubuf__ Prim*)src.GetPhyAddr(), dataCopyParams, nd2nzParams);
}

template <typename T>
__aicore__ inline void DataCopyPad(
    const LocalTensor<T>& dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    using Prim = PrimT<T>;
    DataCopyPad((__cbuf__ Prim*)dst.GetPhyAddr(), src, dataCopyParams, nd2nzParams);
}

template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
{
    using Prim = PrimT<T>;
    DataCopyPad(dst, (__ubuf__ Prim*)src.GetPhyAddr(), dataCopyParams, nd2nzParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    const LocalTensor<T>& dst, __gm__ T* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    GlobalTensor<T> srcTensor;
    srcTensor.SetGlobalBuffer(src);
    DataCopy(dst, srcTensor, intriParams, enhancedParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, const GlobalTensor<T>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy(dst, AscendCMixedGmPtr(src), intriParams, enhancedParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __cbuf__ T* dst, const GlobalTensor<T>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy(dst, AscendCMixedGmPtr(src), intriParams, enhancedParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, const LocalTensor<T>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    GlobalTensor<T> dstTensor;
    dstTensor.SetGlobalBuffer(dst);
    DataCopy(dstTensor, src, intriParams, enhancedParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    const GlobalTensor<T>& dst, __ubuf__ T* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy(AscendCMixedGmPtr(dst), src, intriParams, enhancedParams);
}

template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    const GlobalTensor<T>& dst, __cbuf__ T* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy(AscendCMixedGmPtr(dst), src, intriParams, enhancedParams);
}

template <typename T>
__aicore__ inline void DataCopy(
    const LocalTensor<T>& dst, __ubuf__ T* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    using Prim = PrimT<T>;
    const Hardware dstHW = GetPhyType((TPosition)dst.GetPosition());
    if (dstHW == Hardware::UB) {
        DataCopy((__ubuf__ Prim*)dst.GetPhyAddr(), src, intriParams, enhancedParams);
    } else if (dstHW == Hardware::L1) {
        DataCopy((__cbuf__ Prim*)dst.GetPhyAddr(), src, intriParams, enhancedParams);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "dst", "UB/L1", "DataCopy Mixed Enhanced LocalTensor <- __ubuf__",
            GetPositionDisplay(static_cast<TPosition>(dst.GetPosition())));
    }
}

template <typename T>
__aicore__ inline void DataCopy(
    __ubuf__ T* dst, const LocalTensor<T>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    using Prim = PrimT<T>;
    const Hardware srcHW = GetPhyType((TPosition)src.GetPosition());
    if (srcHW == Hardware::UB) {
        DataCopy(dst, (__ubuf__ Prim*)src.GetPhyAddr(), intriParams, enhancedParams);
    } else {
        ASCENDC_CHECK_TPOSITION(
            false, "src", "UB", "DataCopy Mixed Enhanced __ubuf__ <- LocalTensor",
            GetPositionDisplay(static_cast<TPosition>(src.GetPosition())));
    }
}

template <typename T, typename U>
__aicore__ inline void DataCopy(__gm__ T* dst, const LocalTensor<U>& src, const DataCopyCO12DstParams& intriParams)
{
    GlobalTensor<T> dstTensor;
    dstTensor.SetGlobalBuffer(dst);
    DataCopy(dstTensor, src, intriParams);
}

template <typename T, typename U>
__aicore__ inline void DataCopy(const GlobalTensor<T>& dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    DataCopy(AscendCMixedGmPtr(dst), src, intriParams);
}

template <typename T, typename U>
__aicore__ inline void DataCopy(__cbuf__ T* dst, const LocalTensor<U>& src, const DataCopyCO12DstParams& intriParams)
{
    DataCopy(dst, (__cc__ U*)src.GetPhyAddr(), intriParams);
}

template <typename T, typename U>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    using Prim = PrimT<T>;
    DataCopy((__cbuf__ Prim*)dst.GetPhyAddr(), src, intriParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, half>::value && Std::is_same<PrimT<U>, float>::value, bool>::type =
        true>
__aicore__ inline void DataCopy(
    const LocalTensor<T>& dst, __cbuf__ U* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy((__cc__ PrimT<T>*)dst.GetPhyAddr(), src, intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, half>::value && Std::is_same<PrimT<U>, float>::value, bool>::type =
        true>
__aicore__ inline void DataCopy(
    const LocalTensor<T>& dst, __cc__ U* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), src, intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, half>::value && Std::is_same<PrimT<U>, float>::value, bool>::type =
        true>
__aicore__ inline void DataCopy(
    __cc__ T* dst, const LocalTensor<U>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy(dst, (__cbuf__ PrimT<U>*)src.GetPhyAddr(), intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, half>::value && Std::is_same<PrimT<U>, float>::value, bool>::type =
        true>
__aicore__ inline void DataCopy(
    __ubuf__ T* dst, const LocalTensor<U>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy(dst, (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams, enhancedParams);
}

#define ASCENDC_MIXED_ENH_L0C2UB(dstElem, srcElem)                                                                \
    template <                                                                                                    \
        typename T, typename U,                                                                                   \
        typename Std::enable_if<                                                                                  \
            Std::is_same<PrimT<T>, dstElem>::value && Std::is_same<PrimT<U>, srcElem>::value, bool>::type = true> \
    __aicore__ inline __inout_pipe__(V) void DataCopy(                                                            \
        const LocalTensor<T>& dst, __cc__ U* src, const DataCopyParams& intriParams,                              \
        const DataCopyEnhancedParams& enhancedParams)                                                             \
    {                                                                                                             \
        DataCopy((__ubuf__ dstElem*)dst.GetPhyAddr(), src, intriParams, enhancedParams);                          \
    }                                                                                                             \
    template <                                                                                                    \
        typename T, typename U,                                                                                   \
        typename Std::enable_if<                                                                                  \
            Std::is_same<PrimT<T>, dstElem>::value && Std::is_same<PrimT<U>, srcElem>::value, bool>::type = true> \
    __aicore__ inline __inout_pipe__(V) void DataCopy(                                                            \
        __ubuf__ T* dst, const LocalTensor<U>& src, const DataCopyParams& intriParams,                            \
        const DataCopyEnhancedParams& enhancedParams)                                                             \
    {                                                                                                             \
        DataCopy(dst, (__cc__ srcElem*)src.GetPhyAddr(), intriParams, enhancedParams);                            \
    }

ASCENDC_MIXED_ENH_L0C2UB(int16_t, int32_t)
ASCENDC_MIXED_ENH_L0C2UB(int8_t, int32_t)
ASCENDC_MIXED_ENH_L0C2UB(uint8_t, int32_t)
ASCENDC_MIXED_ENH_L0C2UB(half, int32_t)
#undef ASCENDC_MIXED_ENH_L0C2UB

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, float>::value && Std::is_same<PrimT<U>, half>::value, bool>::type =
        true>
__aicore__ inline __inout_pipe__(V) void DataCopy(
    const LocalTensor<T>& dst, __ubuf__ U* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy((__cc__ float*)dst.GetPhyAddr(), src, intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<Std::is_same<PrimT<T>, float>::value && Std::is_same<PrimT<U>, half>::value, bool>::type =
        true>
__aicore__ inline __inout_pipe__(V) void DataCopy(
    __cc__ T* dst, const LocalTensor<U>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy(dst, (__ubuf__ half*)src.GetPhyAddr(), intriParams, enhancedParams);
}

#if (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201))
template <
    typename T, typename U,
    typename Std::enable_if<
        Std::is_same<PrimT<T>, bfloat16_t>::value && Std::is_same<PrimT<U>, float>::value, bool>::type = true>
__aicore__ inline void DataCopy(
    const LocalTensor<T>& dst, __cbuf__ U* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy((__cc__ PrimT<T>*)dst.GetPhyAddr(), src, intriParams, enhancedParams);
}

template <
    typename T, typename U,
    typename Std::enable_if<
        Std::is_same<PrimT<T>, bfloat16_t>::value && Std::is_same<PrimT<U>, float>::value, bool>::type = true>
__aicore__ inline void DataCopy(
    __cc__ T* dst, const LocalTensor<U>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopy(dst, (__cbuf__ PrimT<U>*)src.GetPhyAddr(), intriParams, enhancedParams);
}
#endif

} // namespace AscendC
#endif // ASCENDC_ENABLE_HW_PTR_API
#endif // ASCENDC_MODULE_OPERATOR_DATA_COPY_MIXED_IMPL_H
