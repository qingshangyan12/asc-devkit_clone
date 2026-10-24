/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

 #ifndef TEST_REG_COMPAT_H
 #define TEST_REG_COMPAT_H

#include "stub_fun.h"
#include "basic_api/reg_compute/kernel_reg_compute_utils.h"
#include "basic_api/reg_compute/kernel_reg_compute_common_intf.h"

namespace AscendC {
namespace Reg {
// kernel_reg_compute_maskreg_intf_impl.h
template <typename T>
__simd_callee__ inline MaskReg MoveMaskImpl()
{
    return MaskReg{};
}

// kernel_reg_compute_vec_unary_intf_impl.h
template <typename T, auto mode, typename U>
__simd_callee__ inline void Log2Impl(U&, U&, MaskReg&)
{}

template <typename T, auto mode, typename U>
__simd_callee__ inline void Log10Impl(U&, U&, MaskReg&)
{}

template <typename T, MaskMergeMode mode, typename U>
__simd_callee__ inline void NotImpl(U&, U&, MaskReg&)
{}

// kernel_reg_compute_vec_binary_intf_impl.h
template <typename T, MaskMergeMode mode, typename U>
__simd_callee__ inline void AndImpl(U&, U&, U&, MaskReg&)
{}

template <typename T, MaskMergeMode mode, typename U>
__simd_callee__ inline void XorImpl(U&, U&, U&, MaskReg&)
{}

template <typename T, typename U, MaskMergeMode mode, typename S, typename V>
__simd_callee__ inline void ShiftLeftImpl(S&, S&, V&, MaskReg&)
{}

template <typename T, typename U, MaskMergeMode mode, typename S, typename V>
__simd_callee__ inline void ShiftRightImpl(S&, S&, V&, MaskReg&)
{}

template <typename T, typename U>
__simd_callee__ inline void MullImpl(U&, U&, U&, U&, MaskReg&)
{}

template <typename T, typename U>
__simd_callee__ inline void AddCarryOutImpl(MaskReg&, U&, U&, U&, MaskReg&)
{}

template <typename T, typename U>
__simd_callee__ inline void SubCarryOutImpl(MaskReg&, U&, U&, U&, MaskReg&)
{}

template <typename T, typename U>
__simd_callee__ inline void AddCarryOutsImpl(MaskReg&, U&, U&, U&, MaskReg&, MaskReg&)
{}

template <typename T, typename U>
__simd_callee__ inline void SubCarryOutsImpl(MaskReg&, U&, U&, U&, MaskReg&, MaskReg&)
{}

// kernel_reg_compute_vec_binary_scalar_intf_impl.h
template <typename T, typename U, MaskMergeMode mode, typename S>
__simd_callee__ inline void ShiftLeftsImpl(S&, S&, U, MaskReg&)
{}

template <typename T, typename U, MaskMergeMode mode, typename S>
__simd_callee__ inline void ShiftRightsImpl(S&, S&, U, MaskReg&)
{}

// kernel_reg_compute_datacopy_intf_impl.h
template <typename T, typename U>
__simd_callee__ inline void LoadImpl(U&, __ubuf__ T*)
{}

template <typename T, typename U>
__simd_callee__ inline void StoreImpl(__ubuf__ T*, U&)
{}

template <typename T, typename U>
__simd_callee__ inline void StoreImpl(__ubuf__ T*, U&, uint32_t)
{}

// kernel_reg_compute_vec_fused_intf_impl.h
template <typename T, MaskMergeMode mode, typename U>
__simd_callee__ inline void FusedAbsSubImpl(U&, U&, U&, MaskReg&)
{}

template <typename T, MaskMergeMode mode, typename U>
__simd_callee__ inline void FusedMulDstAddImpl(U&, U&, U&, MaskReg&)
{}
} // namespace Reg
} // namespace AscendC

#endif // TEST_REG_COMPAT_H
