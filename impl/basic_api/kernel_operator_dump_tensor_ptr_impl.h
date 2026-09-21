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
 * \file kernel_operator_dump_tensor_ptr_impl.h
 * \brief Pointer overloads for DumpTensor / DumpAccChkPoint.
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_dump_tensor_ptr_impl.h is an internal header file and must not be used directly.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_PTR_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_PTR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_PTR_IMPL_H

namespace AscendC {

#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0) || defined(ASCENDC_ACC_DUMP)
namespace CubePtrDetail {
template <typename T>
__aicore__ inline GlobalTensor<T> BindGm(__gm__ T* ptr, uint32_t dumpSize)
{
    GlobalTensor<T> tensor;
    tensor.SetGlobalBuffer(ptr, dumpSize);
    return tensor;
}

template <typename T>
__aicore__ inline LocalTensor<T> BindLocal(TPosition pos, uint32_t addr, uint32_t dumpSize)
{
    // Address-space pointers (__ubuf__/__cbuf__/...) cannot reinterpret_cast to T*;
    // LocalTensor only needs the numeric local address.
    return LocalTensor<T>(pos, addr, dumpSize);
}

template <typename PtrT>
__aicore__ inline uint32_t LocalAddr(PtrT ptr)
{
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ptr));
}

template <AscendC::Hardware hard, typename T, typename U>
__aicore__ inline void DumpHw(U src, uint32_t desc, uint32_t dumpSize, const uint32_t* shape = nullptr,
    uint32_t shapeDim = 0)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    __gm__ __asc_aicore::DebugBlockHeadInfo* blockInfo = __asc_aicore::get_block_info();
    if (dumpSize == 0 || blockInfo == nullptr) {
        return;
    }
    constexpr uint32_t dataBlockSize = 32;
    uint32_t alignDumpLen = __asc_aicore::align_up(static_cast<uint32_t>(dumpSize * sizeof(T)), dataBlockSize);
    uint32_t tlvLen = static_cast<uint32_t>(sizeof(__asc_aicore::DumpTensorTlv)) + alignDumpLen;
    if (!__asc_aicore::check_ringbuf_space(blockInfo, tlvLen)) {
        return;
    }
    __gm__ __asc_aicore::DumpTensorTlv* dumpTlv = reinterpret_cast<__gm__ __asc_aicore::DumpTensorTlv*>(
        __asc_aicore::get_ringbuf_tlv_addr(blockInfo));
    __asc_aicore::set_dump_tlv_info<hard, T>(src, dumpTlv, alignDumpLen, desc, dumpSize);
    if (shape != nullptr && shapeDim > 0) {
        __asc_aicore::set_dump_shape_info(dumpTlv, shapeDim, shape);
    }
    uint32_t copyRet = __asc_aicore::debug_bus_copy_local_to_gm_impl<hard>(
        reinterpret_cast<__gm__ T*>(dumpTlv + 1), src, alignDumpLen, __asc_aicore::get_debug_bus_addr_impl());
    if (copyRet != 0) {
        return;
    }
    __asc_aicore::update_write_info(__asc_aicore::get_block_write_info(blockInfo), tlvLen);
#else
    (void)src;
    (void)desc;
    (void)dumpSize;
    (void)shape;
    (void)shapeDim;
#endif
}
} // namespace CubePtrDetail

template <typename T>
__aicore__ inline void DumpTensor(__gm__ T* tensor, uint32_t desc, uint32_t dumpSize)
{
    auto tmp = CubePtrDetail::BindGm(tensor, dumpSize);
    DumpTensorGM2GMImpl(tmp, desc, dumpSize);
}

template <typename T>
__aicore__ inline void DumpTensor(__ubuf__ T* tensor, uint32_t desc, uint32_t dumpSize)
{
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::VECCALC, CubePtrDetail::LocalAddr(tensor), dumpSize);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
}

template <typename T>
__aicore__ inline void DumpTensor(__cbuf__ T* tensor, uint32_t desc, uint32_t dumpSize)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    CubePtrDetail::DumpHw<Hardware::L1, T>(tensor, desc, dumpSize);
#else
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::A1, CubePtrDetail::LocalAddr(tensor), dumpSize);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#endif
}

template <typename T>
__aicore__ inline void DumpTensor(__cc__ T* tensor, uint32_t desc, uint32_t dumpSize)
{
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::CO1, CubePtrDetail::LocalAddr(tensor), dumpSize);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
}

template <typename T>
__aicore__ inline void DumpTensor(__ca__ T* tensor, uint32_t desc, uint32_t dumpSize)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    CubePtrDetail::DumpHw<Hardware::L0A, T>(tensor, desc, dumpSize);
#else
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::A2, CubePtrDetail::LocalAddr(tensor), dumpSize);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#endif
}

template <typename T>
__aicore__ inline void DumpTensor(__cb__ T* tensor, uint32_t desc, uint32_t dumpSize)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    CubePtrDetail::DumpHw<Hardware::L0B, T>(tensor, desc, dumpSize);
#else
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::B2, CubePtrDetail::LocalAddr(tensor), dumpSize);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#endif
}

template <typename T>
__aicore__ inline void DumpTensor(__gm__ T* tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
    auto tmp = CubePtrDetail::BindGm(tensor, dumpSize);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorGM2GMImpl(tmp, desc, dumpSize);
#else
    DumpTensorGM2GMImpl(tmp, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#endif
}

template <typename T>
__aicore__ inline void DumpTensor(__ubuf__ T* tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::VECCALC, CubePtrDetail::LocalAddr(tensor), dumpSize);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#else
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#endif
}

template <typename T>
__aicore__ inline void DumpTensor(__cbuf__ T* tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    CubePtrDetail::DumpHw<Hardware::L1, T>(tensor, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#else
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::A1, CubePtrDetail::LocalAddr(tensor), dumpSize);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#else
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#endif
#endif
}

template <typename T>
__aicore__ inline void DumpTensor(__cc__ T* tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::CO1, CubePtrDetail::LocalAddr(tensor), dumpSize);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#else
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#endif
}

template <typename T>
__aicore__ inline void DumpTensor(__ca__ T* tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    CubePtrDetail::DumpHw<Hardware::L0A, T>(tensor, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#else
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::A2, CubePtrDetail::LocalAddr(tensor), dumpSize);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#else
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#endif
#endif
}

template <typename T>
__aicore__ inline void DumpTensor(__cb__ T* tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    CubePtrDetail::DumpHw<Hardware::L0B, T>(tensor, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#else
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::B2, CubePtrDetail::LocalAddr(tensor), dumpSize);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#else
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#endif
#endif
}

template <typename T>
__aicore__ inline void DumpAccChkPoint(__gm__ T* tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
    DumpTensor(tensor + countOff, index, dumpSize);
}

template <typename T>
__aicore__ inline void DumpAccChkPoint(__ubuf__ T* tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
    DumpTensor(tensor + countOff, index, dumpSize);
}

template <typename T>
__aicore__ inline void DumpAccChkPoint(__cbuf__ T* tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
    DumpTensor(tensor + countOff, index, dumpSize);
}

template <typename T>
__aicore__ inline void DumpAccChkPoint(__cc__ T* tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
    DumpTensor(tensor + countOff, index, dumpSize);
}

template <typename T>
__aicore__ inline void DumpAccChkPoint(__ca__ T* tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
    DumpTensor(tensor + countOff, index, dumpSize);
}

template <typename T>
__aicore__ inline void DumpAccChkPoint(__cb__ T* tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
    DumpTensor(tensor + countOff, index, dumpSize);
}
#endif

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_PTR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_PTR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_PTR_IMPL_H__
#endif
