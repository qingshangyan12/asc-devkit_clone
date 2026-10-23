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
 * \file kernel_operator_vec_duplicate_intf_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_vec_duplicate_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_vec_duplicate_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_INTF_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_INTERFACE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_INTERFACE_IMPL_H
#include "../../include/basic_api/kernel_tensor.h"
#include "kernel_check.h"
#include "kernel_npu_debug.h"
#include "mstx_local_tensor_info.h"

#if __NPU_ARCH__ == 1001
#include "dav_c100/kernel_operator_vec_duplicate_impl.h"
#elif __NPU_ARCH__ == 2002
#include "dav_m200/kernel_operator_vec_duplicate_impl.h"
#elif __NPU_ARCH__ == 2201
#include "dav_c220/kernel_operator_vec_duplicate_impl.h"
#elif __NPU_ARCH__ == 3002
#include "dav_m300/kernel_operator_vec_duplicate_impl.h"
#elif __NPU_ARCH__ == 3102
#include "dav_m310/kernel_operator_vec_duplicate_impl.h"
#elif __NPU_ARCH__ == 3510
#include "../../include/basic_api/reg_compute/kernel_reg_compute_intf.h"
#include "dav_3510/kernel_operator_vec_duplicate_impl.h"
#elif __NPU_ARCH__ == 9201
#include "../../include/basic_api/reg_compute/kernel_reg_compute_intf.h"
#include "dav_9201/kernel_operator_vec_duplicate_impl.h"
#elif __NPU_ARCH__ == 9202
#include "../../include/basic_api/reg_compute/kernel_reg_compute_intf.h"
#include "dav_9202/kernel_operator_vec_duplicate_impl.h"
#elif (__NPU_ARCH__ == 5102)
#include "../../include/basic_api/reg_compute/kernel_reg_compute_intf.h"
#include "dav_m510/kernel_operator_vec_duplicate_impl.h"
#elif (__NPU_ARCH__ == 5162)
#include "../../include/basic_api/reg_compute/kernel_reg_compute_intf.h"
#include "dav_5162/kernel_operator_vec_duplicate_impl.h"
#elif __NPU_ARCH__ == 3003
#include "dav_l300/kernel_operator_vec_duplicate_impl.h"
#elif __NPU_ARCH__ == 3113
#include "dav_l311/kernel_operator_vec_duplicate_impl.h"
#endif

#pragma begin_pipe(V)
namespace AscendC {
/* **************************************************************************************************
 * Duplicate                                            *
 * ************************************************************************************************* */
/*
 * @ingroup Duplicate Level 0
 * @brief dst[i] = scalar
 * @param [out] dst output LocalTensor
 * @param [in] scalar input scalar number
 * @param [in] mask[]/mask mask array/count
 * @param [in] repeatTime repeat times
 * @param [in] dstBlockStride dst block stride
 * @param [in] dstRepeatStride dst repeat stride
 */
template <typename T, bool isSetMask>
__aicore__ inline void Duplicate(
    const LocalTensor<T>& dst, const T& scalarValue, uint64_t mask, const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckVectorTensor("Duplicate", NamedTensor(dst, "dst"));
    CheckMaskRepeat<T, isSetMask>(mask, repeatTime, "Duplicate");
#endif
#ifdef __MSTX_DFX_REPORT__
    MstxTensor::GetMstxVecDupInfo(dst, mask, repeatTime, dstBlockStride, dstRepeatStride, isSetMask, "Duplicate");
#endif
    CheckDuplicateSupportedType<T>();
#if ASCENDC_CPU_DEBUG
    MaskSetter::Instance().SetMask(isSetMask);
    if (!CheckFunDup(dst, mask, repeatTime, dstBlockStride, dstRepeatStride, "Duplicate")) {
        ASCENDC_REPORT_CHECK_ERROR("Duplicate", KernelFuncType::MASK_COUNT_MODE);
    }
#endif
    DuplicateImpl<T, isSetMask>(
        (__ubuf__ T*)dst.GetPhyAddr(), scalarValue, mask, repeatTime, dstBlockStride, dstRepeatStride);
}

template <
    typename T, bool isSetMask, typename Operand0,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>(),
        int>::type>
__aicore__ inline void Duplicate(
    Operand0 dst, const T& scalarValue, uint64_t mask, const uint8_t repeatTime, const uint16_t dstBlockStride,
    const uint8_t dstRepeatStride)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG) || defined(__MSTX_DFX_REPORT__)
    auto debugDst = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(dst);
    return AscendC::Duplicate<PrimT<VectorOperand::ElementT<T, Operand0>>, isSetMask>(
        debugDst, scalarValue, mask, repeatTime, dstBlockStride, dstRepeatStride);
#else
    using VectorElementT = VectorOperand::ElementT<T, Operand0>;
    CheckDuplicateSupportedType<VectorElementT>();
    DuplicateImpl<VectorElementT, isSetMask>(
        (__ubuf__ VectorElementT*)GetUnderlyingPtr(dst), scalarValue, mask, repeatTime, dstBlockStride,
        dstRepeatStride);
#endif
}

template <typename T, bool isSetMask>
__aicore__ inline void Duplicate(
    const LocalTensor<T>& dst, const T& scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckVectorTensor("Duplicate", NamedTensor(dst, "dst"));
    CheckMaskRepeat<T, isSetMask>(mask, repeatTime, "Duplicate");
#endif
#ifdef __MSTX_DFX_REPORT__
    MstxTensor::GetMstxVecDupInfo(
        dst, mask[0], mask[1], repeatTime, dstBlockStride, dstRepeatStride, isSetMask, "Duplicate");
#endif
    CheckDuplicateSupportedType<T>();
#if ASCENDC_CPU_DEBUG
    MaskSetter::Instance().SetMask(isSetMask);
    if (!CheckFunDup(dst, mask, repeatTime, dstBlockStride, dstRepeatStride, "Duplicate")) {
        ASCENDC_REPORT_CHECK_ERROR("Duplicate", KernelFuncType::MASK_BIT_MODE);
    }
#endif
    DuplicateImpl<T, isSetMask>(
        (__ubuf__ T*)dst.GetPhyAddr(), scalarValue, mask, repeatTime, dstBlockStride, dstRepeatStride);
}

template <
    typename T, bool isSetMask, typename Operand0,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>(),
        int>::type>
__aicore__ inline void Duplicate(
    Operand0 dst, const T& scalarValue, uint64_t mask[], const uint8_t repeatTime, const uint16_t dstBlockStride,
    const uint8_t dstRepeatStride)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG) || defined(__MSTX_DFX_REPORT__)
    auto debugDst = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(dst);
    return AscendC::Duplicate<PrimT<VectorOperand::ElementT<T, Operand0>>, isSetMask>(
        debugDst, scalarValue, mask, repeatTime, dstBlockStride, dstRepeatStride);
#else
    using VectorElementT = VectorOperand::ElementT<T, Operand0>;
    CheckDuplicateSupportedType<VectorElementT>();
    DuplicateImpl<VectorElementT, isSetMask>(
        (__ubuf__ VectorElementT*)GetUnderlyingPtr(dst), scalarValue, mask, repeatTime, dstBlockStride,
        dstRepeatStride);
#endif
}

/*
 * @ingroup Duplicate Level 2
 * @brief dst = dst[i] = scalar
 * @param [out] dst output LocalTensor
 * @param [in] scalar input scalar number
 * @param [in] count number Number of data involved in calculation
 */
template <typename T>
__aicore__ inline void Duplicate(const LocalTensor<T>& dst, const T& scalarValue, const int32_t& count)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckVectorTensor("Duplicate", NamedTensor(dst, "dst"));
    CheckCalcount(count, "count", "Duplicate");
#endif
#ifdef __MSTX_DFX_REPORT__
    MstxTensor::GetMstxVecDupInfo(dst, count, "Duplicate");
#endif
    CheckDuplicateSupportedType<T>();
#if ASCENDC_CPU_DEBUG
    if (!CheckFunDup(dst, count, "Duplicate")) {
        ASCENDC_REPORT_CHECK_ERROR("Duplicate", KernelFuncType::CALCOUNT_MODE);
    }
#endif
    DuplicateImpl<T>((__ubuf__ T*)dst.GetPhyAddr(), scalarValue, count);
}

template <
    typename T, typename Operand0,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>(),
        int>::type>
__aicore__ inline void Duplicate(Operand0 dst, const T& scalarValue, const int32_t& count)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG) || defined(__MSTX_DFX_REPORT__)
    auto debugDst = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(dst);
    return AscendC::Duplicate<PrimT<VectorOperand::ElementT<T, Operand0>>>(debugDst, scalarValue, count);
#else
    using VectorElementT = VectorOperand::ElementT<T, Operand0>;
    CheckDuplicateSupportedType<VectorElementT>();
    DuplicateImpl<VectorElementT>((__ubuf__ VectorElementT*)GetUnderlyingPtr(dst), scalarValue, count);
#endif
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || (__NPU_ARCH__ == 3003) || \
    (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202)
/*
 * @ingroup Duplicate lowest position of src
 * @brief dst = dst[i] = src[0]
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] count number Number of data involved in calculation
 */
template <typename T>
__aicore__ inline void Duplicate(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)
{
#ifdef __MSTX_DFX_REPORT__
    MstxTensor::GetMstxVecDupInfo(dst, count, "Duplicate");
#endif
    using PrimType = PrimT<T>;
    CheckDuplicateSupportedType<PrimType>();
#if ASCENDC_CPU_DEBUG
    if (!CheckFunDup(dst, count, "Duplicate")) {
        ASCENDC_REPORT_CHECK_ERROR("Duplicate", KernelFuncType::CALCOUNT_MODE);
    }
#endif
    DuplicateImpl<PrimType>((__ubuf__ PrimType*)dst.GetPhyAddr(), (__ubuf__ PrimType*)src.GetPhyAddr(), count);
}

template <
    typename T, typename Operand0, typename Operand1,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0, Operand1>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand1>(),
        int>::type>
__aicore__ inline void Duplicate(Operand0 dst, Operand1 src, const int32_t& count)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG) || defined(__MSTX_DFX_REPORT__)
    auto debugDst = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(dst);
    auto debugSrc = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(src);
    return AscendC::Duplicate<PrimT<VectorOperand::ElementT<T, Operand0>>>(debugDst, debugSrc, count);
#else
    using VectorElementT = VectorOperand::ElementT<T, Operand0>;
    using PrimType = PrimT<VectorElementT>;
    CheckDuplicateSupportedType<PrimType>();
    DuplicateImpl<PrimType>(
        (__ubuf__ PrimType*)GetUnderlyingPtr(dst), (__ubuf__ PrimType*)GetUnderlyingPtr(src), count);
#endif
}

/* **************************************************************************************************
 * Interleave                                            *
 * ************************************************************************************************* */
/*
 * @ingroup Interleave Level 2
 * @brief Interleave src0 and src1 to dst0 and dst1
 * @param [out] dst0 output0 LocalTensor
 * @param [out] dst1 output1 LocalTensor
 * @param [in] src0 input0 LocalTensor
 * @param [in] src1 input1 LocalTensor
 * @param [count] count number of data calculation, must be even number
 */
template <typename T>
__aicore__ inline void Interleave(
    const LocalTensor<T>& dst0, const LocalTensor<T>& dst1, const LocalTensor<T>& src0, const LocalTensor<T>& src1,
    const int32_t count)
{
    using PrimType = PrimT<T>;
    InterleaveImpl<PrimType>(
        (__ubuf__ PrimType*)dst0.GetPhyAddr(), (__ubuf__ PrimType*)dst1.GetPhyAddr(),
        (__ubuf__ PrimType*)src0.GetPhyAddr(), (__ubuf__ PrimType*)src1.GetPhyAddr(), count);
}

template <
    typename T, typename Operand0, typename Operand1, typename Operand2, typename Operand3,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0, Operand1, Operand2, Operand3>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand1>() &&
            VectorOperand::IsWritable<Operand1>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand2>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand3>(),
        int>::type>
__aicore__ inline void Interleave(Operand0 dst0, Operand1 dst1, Operand2 src0, Operand3 src1, const int32_t count)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG) || defined(__MSTX_DFX_REPORT__)
    auto debugDst0 = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(dst0);
    auto debugDst1 = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(dst1);
    auto debugSrc0 = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(src0);
    auto debugSrc1 = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(src1);
    return AscendC::Interleave<PrimT<VectorOperand::ElementT<T, Operand0>>>(
        debugDst0, debugDst1, debugSrc0, debugSrc1, count);
#else
    using VectorElementT = VectorOperand::ElementT<T, Operand0>;
    using PrimType = PrimT<VectorElementT>;
    InterleaveImpl<PrimType>(
        (__ubuf__ PrimType*)GetUnderlyingPtr(dst0), (__ubuf__ PrimType*)GetUnderlyingPtr(dst1),
        (__ubuf__ PrimType*)GetUnderlyingPtr(src0), (__ubuf__ PrimType*)GetUnderlyingPtr(src1), count);
#endif
}

/* **************************************************************************************************
 * DeInterleave                                            *
 * ************************************************************************************************* */
/*
 * @ingroup DeInterleave Level 2
 * @brief DeInterleave src0 and src1 to dst0 and dst1
 * @param [out] dst0 output0 LocalTensor
 * @param [out] dst1 output1 LocalTensor
 * @param [in] src0 input0 LocalTensor
 * @param [in] src1 input1 LocalTensor
 * @param [count] count number of data calculation, must be even number
 */
template <typename T>
__aicore__ inline void DeInterleave(
    const LocalTensor<T>& dst0, const LocalTensor<T>& dst1, const LocalTensor<T>& src0, const LocalTensor<T>& src1,
    const int32_t count)
{
    using PrimType = PrimT<T>;
    DeInterleaveImpl<PrimType>(
        (__ubuf__ PrimType*)dst0.GetPhyAddr(), (__ubuf__ PrimType*)dst1.GetPhyAddr(),
        (__ubuf__ PrimType*)src0.GetPhyAddr(), (__ubuf__ PrimType*)src1.GetPhyAddr(), count);
}

template <
    typename T, typename Operand0, typename Operand1, typename Operand2, typename Operand3,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0, Operand1, Operand2, Operand3>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand1>() &&
            VectorOperand::IsWritable<Operand1>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand2>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand3>(),
        int>::type>
__aicore__ inline void DeInterleave(Operand0 dst0, Operand1 dst1, Operand2 src0, Operand3 src1, const int32_t count)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG) || defined(__MSTX_DFX_REPORT__)
    auto debugDst0 = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(dst0);
    auto debugDst1 = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(dst1);
    auto debugSrc0 = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(src0);
    auto debugSrc1 = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(src1);
    return AscendC::DeInterleave<PrimT<VectorOperand::ElementT<T, Operand0>>>(
        debugDst0, debugDst1, debugSrc0, debugSrc1, count);
#else
    using VectorElementT = VectorOperand::ElementT<T, Operand0>;
    using PrimType = PrimT<VectorElementT>;
    DeInterleaveImpl<PrimType>(
        (__ubuf__ PrimType*)GetUnderlyingPtr(dst0), (__ubuf__ PrimType*)GetUnderlyingPtr(dst1),
        (__ubuf__ PrimType*)GetUnderlyingPtr(src0), (__ubuf__ PrimType*)GetUnderlyingPtr(src1), count);
#endif
}

/*
 * @ingroup DeInterleave Level 2
 * @brief DeInterleave src to dst0 and dst1
 * @param [out] dst0 output0 LocalTensor
 * @param [out] dst1 output1 LocalTensor
 * @param [in] src input LocalTensor
 * @param [srcCount] srcCount number of data calculation, must be even number
 */
template <typename T>
__aicore__ inline void DeInterleave(
    const LocalTensor<T>& dst0, const LocalTensor<T>& dst1, const LocalTensor<T>& src, const int32_t srcCount)
{
    using PrimType = PrimT<T>;
    DeInterleaveImpl<PrimType>(
        (__ubuf__ PrimType*)dst0.GetPhyAddr(), (__ubuf__ PrimType*)dst1.GetPhyAddr(),
        (__ubuf__ PrimType*)src.GetPhyAddr(), srcCount);
}

template <
    typename T, typename Operand0, typename Operand1, typename Operand2,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0, Operand1, Operand2>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand1>() &&
            VectorOperand::IsWritable<Operand1>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand2>(),
        int>::type>
__aicore__ inline void DeInterleave(Operand0 dst0, Operand1 dst1, Operand2 src, const int32_t srcCount)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG) || defined(__MSTX_DFX_REPORT__)
    auto debugDst0 = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(dst0);
    auto debugDst1 = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(dst1);
    auto debugSrc = VectorOperand::ToDebugTensor<PrimT<VectorOperand::ElementT<T, Operand0>>>(src);
    return AscendC::DeInterleave<PrimT<VectorOperand::ElementT<T, Operand0>>>(debugDst0, debugDst1, debugSrc, srcCount);
#else
    using VectorElementT = VectorOperand::ElementT<T, Operand0>;
    using PrimType = PrimT<VectorElementT>;
    DeInterleaveImpl<PrimType>(
        (__ubuf__ PrimType*)GetUnderlyingPtr(dst0), (__ubuf__ PrimType*)GetUnderlyingPtr(dst1),
        (__ubuf__ PrimType*)GetUnderlyingPtr(src), srcCount);
#endif
}
#endif
} // namespace AscendC
#pragma end_pipe
#endif // ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_INTERFACE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_INTF_IMPL_H__
#endif
