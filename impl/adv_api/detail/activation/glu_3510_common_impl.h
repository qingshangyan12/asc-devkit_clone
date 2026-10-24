/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_ACTIVATION_GLU_3510_COMMON_IMPL_H
#define IMPL_ACTIVATION_GLU_3510_COMMON_IMPL_H

namespace AscendC {
namespace Internal {
template <typename T>
__simd_callee__ inline void LoadGluOperands(
    Reg::RegTensor<float>& srcVreg0, Reg::RegTensor<float>& srcVreg1, Reg::RegTensor<half>& srcOrigin0,
    Reg::RegTensor<half>& srcOrigin1, __ubuf__ T* src0, __ubuf__ T* src1, const uint32_t offset, Reg::MaskReg& mask)
{
    if constexpr (sizeof(T) == sizeof(half)) {
        Reg::LoadAlign<half, Reg::LoadDist::DIST_UNPACK_B16>(srcOrigin0, src0 + offset);
        Reg::Cast<float, half, castTraitB16ToB32>(srcVreg0, srcOrigin0, mask);
        Reg::LoadAlign<half, Reg::LoadDist::DIST_UNPACK_B16>(srcOrigin1, src1 + offset);
        Reg::Cast<float, half, castTraitB16ToB32>(srcVreg1, srcOrigin1, mask);
    } else {
        Reg::LoadAlign(srcVreg0, src0 + offset);
        Reg::LoadAlign(srcVreg1, src1 + offset);
    }
}

template <typename T>
__simd_callee__ inline void FinishGlu(
    Reg::RegTensor<float>& tmpReg1, Reg::RegTensor<float>& dstVreg, Reg::RegTensor<float>& tmpReg0,
    Reg::RegTensor<float>& srcVreg0, Reg::RegTensor<float>& srcVreg1, Reg::MaskReg& mask, __ubuf__ T* dst,
    const uint32_t offset)
{
    Reg::Exp(tmpReg1, tmpReg0, mask);
    Reg::Adds(tmpReg1, tmpReg1, 1.0f, mask);
    Reg::Div(tmpReg1, srcVreg1, tmpReg1, mask);
    Reg::Mul(dstVreg, srcVreg0, tmpReg1, mask);
    if constexpr (sizeof(T) == sizeof(half)) {
        Reg::Cast<half, float, castTraitB32ToB16>((Reg::RegTensor<half>&)dstVreg, dstVreg, mask);
        Reg::StoreAlign<half, Reg::StoreDist::DIST_PACK_B32>(dst + offset, (Reg::RegTensor<half>&)dstVreg, mask);
    } else {
        Reg::StoreAlign(dst + offset, dstVreg, mask);
    }
}
} // namespace Internal
} // namespace AscendC

#endif // IMPL_ACTIVATION_GLU_3510_COMMON_IMPL_H
