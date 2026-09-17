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
 * \file kernel_operator_mm_ptr_impl.h
 * \brief Pointer-path entry points for the CUBE (matrix / ISASI) Basic APIs.
 *
 *        These overloads are strictly additive: they take bare hardware pointers instead of
 *        LocalTensor / GlobalTensor and forward to exactly the same low level *Cal implementation that the
 *        Tensor path uses. The Tensor overloads are left untouched, so the Tensor path is bit-for-bit
 *        unchanged and no existing call site can change meaning.
 *
 *        The address space of the destination operand selects the hardware target in exactly the way
 *        LocalTensor::GetPosition() does on the Tensor path:
 *            __ca__   -> L0A Buffer (A2)      __cb__   -> L0B Buffer (B2)
 *            __cc__   -> L0C Buffer (CO1)     __cbuf__ -> L1 Buffer (A1/B1/C1)
 *            __ubuf__ -> Unified Buffer       __gm__   -> Global Memory
 *
 *        All operands are normalized by GetUnderlyingPtr() so that a raw pointer is passed through and a
 *        Tensor operand is resolved through GetPhyAddr().
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_mm_ptr_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_mm_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_PTR_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_MM_PTR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_MM_PTR_IMPL_H

#include "utils/kernel_utils_underlying_ptr.h"
#include "../../include/basic_api/kernel_struct_mm.h"

namespace AscendC {

/* **************************************************************************************************
 * LoadData 2d, pointer path: L1 (__cbuf__) -> L0A (__ca__) / L0B (__cb__)
 * ************************************************************************************************* */
template <typename Tp>
__aicore__ inline void LoadData(__ca__ Tp* dst, __cbuf__ Tp* src, const LoadData2DParams& loadDataParams)
{
    LoadData2DL12L0ACal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadData(__cb__ Tp* dst, __cbuf__ Tp* src, const LoadData2DParams& loadDataParams)
{
    LoadData2DL12L0BCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

/* **************************************************************************************************
 * LoadData 2dV2, pointer path: L1 (__cbuf__) -> L0A (__ca__) / L0B (__cb__)
 * ************************************************************************************************* */
template <typename Tp>
__aicore__ inline void LoadData(__ca__ Tp* dst, __cbuf__ Tp* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DL12L0ACal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadData(__cb__ Tp* dst, __cbuf__ Tp* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DL12L0BCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

/* **************************************************************************************************
 * LoadData 2dV2, pointer path, MX (microscaling) form: A/B plus their scale operand
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || \
                              (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
template <typename Tp, typename Up, typename Vp>
__aicore__ inline void LoadData(
    __ca__ Up* dst, __cbuf__ Tp* src, __cbuf__ Vp* srcMx, const LoadData2DParamsV2& loadDataParams,
    const LoadData2DMxParams& loadMxDataParams)
{
    LoadData2DL12L0ACal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), GetUnderlyingPtr(srcMx), loadDataParams, loadMxDataParams);
}

template <typename Tp, typename Up, typename Vp>
__aicore__ inline void LoadData(
    __cb__ Up* dst, __cbuf__ Tp* src, __cbuf__ Vp* srcMx, const LoadData2DParamsV2& loadDataParams,
    const LoadData2DMxParams& loadMxDataParams)
{
    LoadData2DL12L0BCal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), GetUnderlyingPtr(srcMx), loadDataParams, loadMxDataParams);
}
#endif

/* **************************************************************************************************
 * LoadData 3dv1, pointer path
 * ************************************************************************************************* */
template <typename Tp, typename Up>
__aicore__ inline void LoadData(__ca__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV1<Up>& loadDataParams)
{
    LoadData3DV1L12L0ACal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp, typename Up>
__aicore__ inline void LoadData(__cb__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV1<Up>& loadDataParams)
{
    LoadData3DV1L12L0BCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp, typename Up>
__aicore__ inline void LoadData(__ubuf__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV1<Up>& loadDataParams)
{
    LoadData3DV1L12UBCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

/* **************************************************************************************************
 * LoadData 3dv2, pointer path
 * ************************************************************************************************* */
template <typename Tp, typename Up>
__aicore__ inline void LoadData(__ca__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV2<Up>& loadDataParams)
{
    LoadData3DV2L12L0ACal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp, typename Up>
__aicore__ inline void LoadData(__cb__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV2<Up>& loadDataParams)
{
    LoadData3DV2L12L0BCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp, typename Up>
__aicore__ inline void LoadData(__ubuf__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV2<Up>& loadDataParams)
{
    LoadData3DV2L12UBCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

/* **************************************************************************************************
 * LoadData 3dv2Pro and load3d bit mode form, pointer path (only where the low level form exists)
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
template <typename Tp>
__aicore__ inline void LoadData(__ca__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadData3DV2L12L0ACal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadData(__cb__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV2Pro& loadDataParams)
{
    LoadData3DV2L12L0BCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadData(__ca__ Tp* dst, __cbuf__ Tp* src, const Load3DBitModeParam& loadDataParams)
{
    LoadData3DV2L12L0ACal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadData(__cb__ Tp* dst, __cbuf__ Tp* src, const Load3DBitModeParam& loadDataParams)
{
    LoadData3DV2L12L0BCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadData(__ca__ Tp* dst, __cbuf__ Tp* src, const Load2DBitModeParam& loadDataParams)
{
    LoadData2DL12L0ACal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadData(__cb__ Tp* dst, __cbuf__ Tp* src, const Load2DBitModeParam& loadDataParams)
{
    LoadData2DL12L0BCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}
#endif

/* **************************************************************************************************
 * LoadData, pointer path: GM (__gm__) -> L1 (__cbuf__) / L0A (__ca__) / L0B (__cb__)
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
template <typename Tp>
__aicore__ inline void LoadData(__cbuf__ Tp* dst, __gm__ Tp* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DGM2L1Cal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams, ExtractCacheMode(GetUnderlyingPtr(src)));
}

template <typename Tp>
__aicore__ inline void LoadData(__ca__ Tp* dst, __gm__ Tp* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DGM2L0ACal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams, ExtractCacheMode(GetUnderlyingPtr(src)));
}

template <typename Tp>
__aicore__ inline void LoadData(__cb__ Tp* dst, __gm__ Tp* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DGM2L0BCal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams, ExtractCacheMode(GetUnderlyingPtr(src)));
}
#else
template <typename Tp>
__aicore__ inline void LoadData(__cbuf__ Tp* dst, __gm__ Tp* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DGM2L1Cal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadData(__ca__ Tp* dst, __gm__ Tp* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DGM2L0ACal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadData(__cb__ Tp* dst, __gm__ Tp* src, const LoadData2DParamsV2& loadDataParams)
{
    LoadData2DGM2L0BCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}
#endif

/* **************************************************************************************************
 * LoadDataWithStride, pointer path (load3d v2 with the stride repeat mode)
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
template <typename Tp, typename Up>
__aicore__ inline void LoadDataWithStride(
    __ca__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV2<Up>& loadDataParams)
{
    LoadData3DV2L12L0AWithStrideCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp, typename Up>
__aicore__ inline void LoadDataWithStride(
    __cb__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV2<Up>& loadDataParams)
{
    LoadData3DV2L12L0BWithStrideCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp, typename Up>
__aicore__ inline void LoadDataWithStride(
    __ubuf__ Tp* dst, __cbuf__ Tp* src, const LoadData3DParamsV2<Up>& loadDataParams)
{
    LoadData3DV2L12UBWithStrideCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}
#endif

/* **************************************************************************************************
 * LoadDataWithTranspose, pointer path
 * ************************************************************************************************* */
template <typename Tp>
__aicore__ inline void LoadDataWithTranspose(
    __ca__ Tp* dst, __cbuf__ Tp* src, const LoadData2dTransposeParams& loadDataParams)
{
    LoadData2DL12L0ATransposeCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadDataWithTranspose(
    __cb__ Tp* dst, __cbuf__ Tp* src, const LoadData2dTransposeParams& loadDataParams)
{
    LoadData2DL12L0BTransposeCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

template <typename Tp>
__aicore__ inline void LoadDataWithTranspose(
    __cb__ Tp* dst, __cbuf__ Tp* src, const LoadData2dTransposeParamsV2& loadDataParams)
{
    LoadData2DL12L0BTransposeCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), loadDataParams);
}

/* **************************************************************************************************
 * LoadDataWithSparse, pointer path
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
template <typename Tp, typename Up>
__aicore__ inline void LoadDataWithSparse(
    __cb__ Tp* dst, __cbuf__ Tp* src, __cbuf__ Up* idx, const LoadData2dParams& loadDataParam)
{
    LoadDataWithSparseCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), GetUnderlyingPtr(idx), loadDataParam);
}
#endif

/* **************************************************************************************************
 * Mmad, pointer path
 * ************************************************************************************************* */
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void Mmad(__cc__ DstT* dst, __ca__ Src0T* fm, __cb__ Src1T* filter, const MmadParams& mmadParams)
{
    MmadCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter), mmadParams);
}

// A bare hardware pointer carries no TPosition, so the address space is what selects the meaning of the
// bias operand, exactly like GetPosition() does on the Tensor path:
//     __biasbuf__ -> the operand lives in the BiasTable buffer, i.e. the C matrix initial value is read
//                    from BT (cmatrixSource = true)
//     __cc__      -> the operand lives in L0C (cmatrixSource = false)
// On the Tensor path the same decision is made by GetPhyType(bias.GetPosition()) in MmadImpl.
template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void Mmad(
    __cc__ DstT* dst, __ca__ Src0T* fm, __cb__ Src1T* filter, __biasbuf__ BiasT* bias, const MmadParams& mmadParams)
{
    constexpr bool cmatrixSource = true;
    MmadCal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter),
        static_cast<uint64_t>(reinterpret_cast<uint64_t>(GetUnderlyingPtr(bias))), mmadParams, cmatrixSource);
}

template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void Mmad(
    __cc__ DstT* dst, __ca__ Src0T* fm, __cb__ Src1T* filter, __cc__ BiasT* bias, const MmadParams& mmadParams)
{
    constexpr bool cmatrixSource = false;
    MmadCal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter),
        static_cast<uint64_t>(reinterpret_cast<uint64_t>(GetUnderlyingPtr(bias))), mmadParams, cmatrixSource);
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void Mmad(
    __cc__ DstT* dst, __ca__ Src0T* fm, __cb__ Src1T* filter, const MmadBitModeParams& mmadParams)
{
    MmadCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter), mmadParams);
}

template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void Mmad(
    __cc__ DstT* dst, __ca__ Src0T* fm, __cb__ Src1T* filter, __cc__ BiasT* bias, MmadBitModeParams& mmadParams)
{
    MmadCal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter),
        static_cast<uint64_t>(reinterpret_cast<uint64_t>(GetUnderlyingPtr(bias))), mmadParams);
}
#endif

/* **************************************************************************************************
 * MmadMx, pointer path
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadMx(
    __cc__ DstT* dst, __ca__ Src0T* fm, __cb__ Src1T* filter, const MmadParams& mmadParams)
{
    MmadMxCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter), mmadParams);
}

template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void MmadMx(
    __cc__ DstT* dst, __ca__ Src0T* fm, __cb__ Src1T* filter, __biasbuf__ BiasT* bias, const MmadParams& mmadParams)
{
    constexpr bool cmatrixSource = true;
    MmadMxCal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter),
        static_cast<uint64_t>(reinterpret_cast<uint64_t>(GetUnderlyingPtr(bias))), mmadParams, cmatrixSource);
}

template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void MmadMx(
    __cc__ DstT* dst, __ca__ Src0T* fm, __cb__ Src1T* filter, __cc__ BiasT* bias, const MmadParams& mmadParams)
{
    constexpr bool cmatrixSource = false;
    MmadMxCal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter),
        static_cast<uint64_t>(reinterpret_cast<uint64_t>(GetUnderlyingPtr(bias))), mmadParams, cmatrixSource);
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadMx(
    __cc__ DstT* dst, __ca__ Src0T* fm, __cb__ Src1T* filter, const MmadBitModeParams& mmadParams)
{
    MmadMxCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter), mmadParams);
}

template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void MmadMx(
    __cc__ DstT* dst, __ca__ Src0T* fm, __cb__ Src1T* filter, __cc__ BiasT* bias, MmadBitModeParams& mmadParams)
{
    MmadMxCal(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter),
        static_cast<uint64_t>(reinterpret_cast<uint64_t>(GetUnderlyingPtr(bias))), mmadParams);
}
#endif

/* **************************************************************************************************
 * MmadWithSparse, pointer path
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
template <typename DstT, typename SrcT>
__aicore__ inline void MmadWithSparse(
    __cc__ DstT* dst, __ca__ SrcT* fm, __cb__ SrcT* filter, const MmadParams& mmadParams)
{
    MmadSpCal(GetUnderlyingPtr(dst), GetUnderlyingPtr(fm), GetUnderlyingPtr(filter), mmadParams);
}
#endif

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_MM_PTR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_PTR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_PTR_IMPL_H__
#endif