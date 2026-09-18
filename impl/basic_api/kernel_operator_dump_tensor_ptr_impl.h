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
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::A1, CubePtrDetail::LocalAddr(tensor), dumpSize);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
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
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::A2, CubePtrDetail::LocalAddr(tensor), dumpSize);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
}

template <typename T>
__aicore__ inline void DumpTensor(__cb__ T* tensor, uint32_t desc, uint32_t dumpSize)
{
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::B2, CubePtrDetail::LocalAddr(tensor), dumpSize);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
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
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::A1, CubePtrDetail::LocalAddr(tensor), dumpSize);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#else
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
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
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::A2, CubePtrDetail::LocalAddr(tensor), dumpSize);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#else
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#endif
}

template <typename T>
__aicore__ inline void DumpTensor(__cb__ T* tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
    auto tmp = CubePtrDetail::BindLocal<T>(TPosition::B2, CubePtrDetail::LocalAddr(tensor), dumpSize);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize);
#else
    DumpTensorLocal2GMImpl(tmp, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
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
