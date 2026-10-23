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
 * \file kernel_operator_dump_tensor_intf_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_dump_tensor_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_dump_tensor_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_INTF_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_INTERFACE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_INTERFACE_IMPL_H
#include "../../include/basic_api/kernel_tensor.h"
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3002 || \
    __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 5162 || __NPU_ARCH__ == 9201 || __NPU_ARCH__ == 9202
#include "../utils/debug/asc_debug_utils.h"
#include "../utils/debug/asc_aicore_printf_impl.h"
#endif

#if __NPU_ARCH__ == 1001
#include "dav_c100/kernel_operator_dump_tensor_impl.h"
#elif __NPU_ARCH__ == 2002
#include "dav_m200/kernel_operator_dump_tensor_impl.h"
#elif __NPU_ARCH__ == 2201
#include "dav_c220/kernel_operator_dump_tensor_impl.h"
#elif __NPU_ARCH__ == 3002
#include "dav_m300/kernel_operator_dump_tensor_impl.h"
#elif __NPU_ARCH__ == 3510
#include "dav_3510/kernel_operator_dump_tensor_impl.h"
#elif __NPU_ARCH__ == 9201
#include "dav_9201/kernel_operator_dump_tensor_impl.h"
#elif __NPU_ARCH__ == 9202
#include "dav_9202/kernel_operator_dump_tensor_impl.h"
#elif (__NPU_ARCH__ == 5102)
#include "dav_m510/kernel_operator_dump_tensor_impl.h"
#elif (__NPU_ARCH__ == 5162)
#include "dav_5162/kernel_operator_dump_tensor_impl.h"
#elif (__NPU_ARCH__ == 3003)
#include "dav_l300/kernel_operator_dump_tensor_impl.h"
#elif (__NPU_ARCH__ == 3113)
#include "dav_l311/kernel_operator_dump_tensor_impl.h"
#elif (__NPU_ARCH__ == 3102)
#include "dav_m310/kernel_operator_dump_tensor_impl.h"
#endif

#ifdef ASCENDC_CPU_DEBUG
#include <cstdio>
#include <utility>
#endif

namespace AscendC {
__aicore__ inline void PrintTimeStamp(uint32_t descId)
{
#ifdef ASCENDC_TIME_STAMP_ON // 打点开关宏
    DumpTimeStampImpl(descId);
#endif
}

#if defined(__NPU_ARCH__)
template <typename T>
__aicore__ inline void DumpTensor(const LocalTensor<T>& input, uint32_t desc, uint32_t dumpSize)
{
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
#else
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "DumpTensor is not supported in cpu mode."); });
#endif
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    DumpTensorLocal2GMImpl(input, desc, dumpSize);
#endif
    return;
}
template <typename T>
__aicore__ inline void DumpTensor(const GlobalTensor<T>& input, uint32_t desc, uint32_t dumpSize)
{
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
#else
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "DumpTensor is not supported in cpu mode."); });
#endif
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    DumpTensorGM2GMImpl(input, desc, dumpSize);
#endif
    return;
}
template <typename T>
__aicore__ inline void DumpTensor(
    const GlobalTensor<T>& input, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
#else
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "DumpTensor is not supported in cpu mode."); });
#endif
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorGM2GMImpl(input, desc, dumpSize);
#else
    DumpTensorGM2GMImpl(input, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#endif
#endif
    return;
}
template <typename T>
__aicore__ inline void DumpTensor(
    const LocalTensor<T>& input, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
#else
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "DumpTensor is not supported in cpu mode."); });
#endif
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3003) || \
                              (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3102))
    DumpShapeImpl(shapeInfo);
    DumpTensorLocal2GMImpl(input, desc, dumpSize);
#else
    DumpTensorLocal2GMImpl(input, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#endif
#endif
    return;
}

template <typename T>
__aicore__ inline void DumpAccChkPoint(
    const LocalTensor<T>& input, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
#else
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "DumpAccChkPoint is not supported in cpu mode."); });
#endif
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0) || defined(ASCENDC_ACC_DUMP)
    if (countOff > input.GetSize()) {
        ASCENDC_ASSERT(
            (false), { KERNEL_LOG(KERNEL_ERROR, "tensor offset [%d] exceeds limit [%d]", countOff, input.GetSize()); });
        return;
    }
    LocalTensor<T> tmp = input[countOff];
    DumpTensorLocal2GMImpl(tmp, index, dumpSize);
#endif
    return;
}
template <typename T>
__aicore__ inline void DumpAccChkPoint(
    const GlobalTensor<T>& input, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
#else
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "DumpAccChkPoint is not supported in cpu mode."); });
#endif
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0) || defined(ASCENDC_ACC_DUMP)
    if (countOff > input.GetSize()) {
        ASCENDC_ASSERT(
            (false), { KERNEL_LOG(KERNEL_ERROR, "tensor offset [%d] exceeds limit [%d]", countOff, input.GetSize()); });
        return;
    }
    GlobalTensor<T> tmp = input[countOff];
    DumpTensorGM2GMImpl(tmp, index, dumpSize);
#endif
    return;
}

/*
 * Definitions of the raw-pointer GM dump overloads declared in the interface header.  The GM
 * address is re-wrapped as the GlobalTensor view it denotes and handed to the Tensor overload
 * above, so the dump FIFO and the entity selection are exactly the Tensor path's.
 */
template <typename T>
__aicore__ inline void DumpTensor(__gm__ T* tensor, uint32_t desc, uint32_t dumpSize)
{
    GlobalTensor<T> tmp;
    tmp.SetGlobalBuffer(tensor);
    DumpTensor<T>(tmp, desc, dumpSize);
}

template <typename T>
__aicore__ inline void DumpTensor(__gm__ T* tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
    GlobalTensor<T> tmp;
    tmp.SetGlobalBuffer(tensor);
    DumpTensor<T>(tmp, desc, dumpSize, shapeInfo);
}

template <typename T>
__aicore__ inline void DumpAccChkPoint(__gm__ T* tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
    GlobalTensor<T> tmp;
    // The Tensor path first rejects a countOff beyond the tensor extent.  A raw pointer carries no
    // extent, so that one runtime check cannot be evaluated here and is omitted -- the same omission
    // the design records for the pointer path's position check.  An unbounded extent omits exactly
    // that check and leaves the compile-time guards and the assert inside the Tensor overload intact.
    constexpr uint64_t unknownExtent = ~static_cast<uint64_t>(0);
    tmp.SetGlobalBuffer(tensor, unknownExtent);
    DumpAccChkPoint<T>(tmp, index, countOff, dumpSize);
}

/* ---- DumpTensor / DumpAccChkPoint with a raw on-chip address -----------------
 *
 * DumpTensorLocal2GMImpl selects the entity from the operand position, branching on
 * GetPhyType(GetPosition()) to asc_dump_ubuf / asc_dump_l1buf / asc_dump_cbuf / asc_dump(BIAS) /
 * asc_dump(FIXBUF) (dav_3510/kernel_operator_dump_tensor_impl.h:48-71).  The pointer overloads
 * below take that position as a template argument, build the LocalTensor view the impl needs, and
 * forward to the same Tensor overload the LocalTensor form reaches.  No address is recomputed and
 * no parameter is reinterpreted.
 *
 * The view is built through TBuffAddr rather than LocalTensor(TPosition, offset, length): on the
 * device that constructor takes a buffer offset, which a raw GetPhyAddr() is, but under
 * ASCENDC_CPU_DEBUG GetPhyAddr() returns an absolute host address and the constructor would add the
 * platform base a second time.  Each build is given the one field its own GetPhyAddr() reads --
 * bufferAddr on device, absAddr under ASCENDC_CPU_DEBUG -- the same construction
 * CubePtrOverload::LocalTensorFromPtr / ::UbTensorOver already use for ReadSpmBuffer and the
 * synchronisation APIs.
 *
 * The extent question, which is why DumpAccChkPoint and DumpTensor differ below: a raw pointer
 * carries no element count, and only DumpAccChkPoint reads one (the Tensor body rejects a countOff
 * beyond input.GetSize()).  DumpTensor reads only the address, so its view is declared with the
 * dumpSize the caller already gives; DumpAccChkPoint gets an unbounded declared extent, the same
 * decision the raw GM form above records, which drops exactly that one runtime check and leaves the
 * compile-time guards intact.
 */
namespace DumpPtrOverload {
template <TPosition position, typename T, typename Ptr>
__aicore__ inline LocalTensor<T> LocalViewOver(Ptr ptr, uint32_t lenHintBytes)
{
    TBuffAddr addr;
    addr.logicPos = static_cast<uint8_t>(position);
    addr.bufferAddr = static_cast<uint32_t>(reinterpret_cast<uint64_t>(ptr));
    addr.bufferHandle = nullptr;
    addr.dataLen = lenHintBytes;
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    // GetPhyAddr() reads absAddr on the host and bufferAddr on the device.  The two sibling
    // helpers named above fill whichever one applies; this view has to do the same, or the
    // host build would carry a view whose address is not the one GetPhyAddr() reads.
    addr.absAddr = reinterpret_cast<uint8_t*>(ptr);
#endif
    LocalTensor<T> tensor;
    tensor.SetAddr(addr);
    return tensor;
}

// The declared byte extent an on-chip pointer call can claim: none is knowable, so it claims the
// largest representable one, which makes the countOff check unfalsifiable rather than falsely
// tripping on a zero-length view.
constexpr uint32_t UnknownLocalExtentBytes = ~static_cast<uint32_t>(0);
} // namespace DumpPtrOverload

template <
    TPosition position, typename T, typename DstPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type>
__aicore__ inline void DumpTensor(DstPtr tensor, uint32_t desc, uint32_t dumpSize)
{
    DumpTensor<T>(DumpPtrOverload::LocalViewOver<position, T>(tensor, dumpSize * sizeof(PrimT<T>)), desc, dumpSize);
}

template <
    TPosition position, typename T, typename DstPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type>
__aicore__ inline void DumpTensor(DstPtr tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
    DumpTensor<T>(
        DumpPtrOverload::LocalViewOver<position, T>(tensor, dumpSize * sizeof(PrimT<T>)), desc, dumpSize, shapeInfo);
}

template <
    TPosition position, typename T, typename DstPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type>
__aicore__ inline void DumpAccChkPoint(DstPtr tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
    DumpAccChkPoint<T>(
        DumpPtrOverload::LocalViewOver<position, T>(tensor, DumpPtrOverload::UnknownLocalExtentBytes), index, countOff,
        dumpSize);
}

// only 51, 71, 82 supports printf/assert. the other code is left to avoid errors in test cases.
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3002 || \
    __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 5162 || __NPU_ARCH__ == 9201 || __NPU_ARCH__ == 9202
template <class... Args>
__aicore__ inline void AssertImpl(__gm__ const char* fmt, Args&&... args)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    __asc_aicore::printf_impl_assert(fmt, args...);
#else
    return;
#endif
}
#else
template <class... Args>
__aicore__ inline void AssertImpl(__gm__ const char* fmt, Args&&... args)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    PrintfImpl(DumpType::DUMP_ASSERT, fmt, args...);
#else
    return;
#endif
}
#endif // __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3002

#endif // defined(__NPU_ARCH__)
} // namespace AscendC
#endif // END OF ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_INTERFACE_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_INTF_IMPL_H__
#endif
