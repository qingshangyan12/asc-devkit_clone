/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file kernel_reg_compute_te_intf.h
 * \brief TensorAPI-style register compute layer (Te) + thin Reg:: compatibility adapters.
 *
 * New code:
 *   using namespace AscendC::Te;
 *   auto a = MakeRegTensor<half>();
 *   auto m = MakeMaskReg<half>(elemCount);
 *   Compute(MakeVec(vec::Add{}), dst, src0, src1, m);
 *   Load(MakeLoad(load::UbAlign{}), a, ubPtr);
 *
 * Legacy-shaped wrappers (same names as old Reg:: API for common ops):
 *   Reg::Add(dst, src0, src1, mask);
 *   Reg::DataCopy(dst, ubPtr);
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_TE_INTF_H__
#endif

#ifndef ASCENDC_MODULE_REG_COMPUTE_TE_INTERFACE_H
#define ASCENDC_MODULE_REG_COMPUTE_TE_INTERFACE_H

#include "kernel_reg_compute_struct_intf.h"
#include "kernel_reg_compute_common_intf.h"

#include "tensor_api/algorithm/vector_compute.h"
#include "tensor_api/algorithm/reg_load.h"
#include "tensor_api/algorithm/mask_ops.h"
#include "tensor_api/reg/reg_tensor.h"

namespace AscendC {
namespace Reg {

using MaskReg = vector_bool;

// -------- TensorAPI verbs + register factories re-exported at Reg boundary --------
using Te::Compute;
using Te::MakeVec;
using Te::Load;
using Te::Store;
using Te::MakeLoad;
using Te::MakeStore;
using Te::VecTrait;
using Te::MakeRegTensor;
using Te::MakeRegTrait;
using Te::MakeMaskReg;

namespace vec = Te::vec;
namespace load = Te::load;
namespace mask = Te::mask;

// -------- Legacy adapters: old Reg:: call shape → Te implementation --------

template <typename Dst, typename Src0, typename Src1, typename MaskT>
__simd_callee__ inline void Add(Dst& dst, Src0& src0, Src1& src1, MaskT& mask)
{
    Te::Compute(Te::MakeVec(Te::vec::Add{}), dst, src0, src1, mask);
}

template <typename Dst, typename Src0, typename Src1, typename MaskT>
__simd_callee__ inline void Sub(Dst& dst, Src0& src0, Src1& src1, MaskT& mask)
{
    Te::Compute(Te::MakeVec(Te::vec::Sub{}), dst, src0, src1, mask);
}

template <typename Dst, typename Src0, typename Src1, typename MaskT>
__simd_callee__ inline void Mul(Dst& dst, Src0& src0, Src1& src1, MaskT& mask)
{
    Te::Compute(Te::MakeVec(Te::vec::Mul{}), dst, src0, src1, mask);
}

template <typename Dst, typename Src0, typename Src1, typename MaskT>
__simd_callee__ inline void Div(Dst& dst, Src0& src0, Src1& src1, MaskT& mask)
{
    Te::Compute(Te::MakeVec(Te::vec::Div{}), dst, src0, src1, mask);
}

template <typename Dst, typename Src0, typename Src1, typename MaskT>
__simd_callee__ inline void Max(Dst& dst, Src0& src0, Src1& src1, MaskT& mask)
{
    Te::Compute(Te::MakeVec(Te::vec::Max{}), dst, src0, src1, mask);
}

template <typename Dst, typename Src0, typename Src1, typename MaskT>
__simd_callee__ inline void Min(Dst& dst, Src0& src0, Src1& src1, MaskT& mask)
{
    Te::Compute(Te::MakeVec(Te::vec::Min{}), dst, src0, src1, mask);
}

template <typename Dst, typename Src, typename MaskT>
__simd_callee__ inline void Abs(Dst& dst, Src& src, MaskT& mask)
{
    Te::Compute(Te::MakeVec(Te::vec::Abs{}), dst, src, mask);
}

template <typename Dst, typename Src, typename MaskT>
__simd_callee__ inline void Exp(Dst& dst, Src& src, MaskT& mask)
{
    Te::Compute(Te::MakeVec(Te::vec::Exp{}), dst, src, mask);
}

/// Aligned UB → RegTensor load (public name matches test usage; implemented via Te::Load).
template <typename Dst, typename T>
__simd_callee__ inline void DataCopy(Dst& dst, __ubuf__ T* src)
{
    Te::Load(Te::MakeLoad(Te::load::UbAlign{}), dst, src);
}

/// Aligned RegTensor → UB store with mask.
template <typename T, typename Src, typename MaskT>
__simd_callee__ inline void DataCopy(__ubuf__ T* dst, Src& src, MaskT& mask)
{
    Te::Store(Te::MakeStore(Te::load::UbStoreAlign{}), dst, src, mask);
}

template <typename T, typename U>
__simd_callee__ inline MaskReg UpdateMask(U& scalarValue)
{
    return Te::mask::UpdateMask<T>(scalarValue);
}

} // namespace Reg
} // namespace AscendC

#endif // ASCENDC_MODULE_REG_COMPUTE_TE_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_TE_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_TE_INTF_H__
#endif
