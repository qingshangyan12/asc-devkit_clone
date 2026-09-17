/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_MM_STRIDE_POINTER_IMPL_H
#define ASCENDC_MM_STRIDE_POINTER_IMPL_H
#include "kernel_cube_pointer_traits.h"
#if __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102
namespace AscendC {
template <typename T = void, const IsResetLoad3dConfig& config = IS_RESER_LOAD3D_DEFAULT_CONFIG,
    typename D, typename S, typename U,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value, bool>::type = true>
__aicore__ inline void LoadDataWithStride(const D& dst, const S& src, const LoadData3DParamsV2<U>& params)
{
    using Element = Impl::CubeValue<D>;
    static_assert(!Std::is_const<Impl::CubeElement<D>>::value, "Destination must be writable");
    static_assert(Std::is_same<Element, U>::value && Std::is_same<Element, Impl::CubeValue<S>>::value,
        "LoadDataWithStride element types must match");
    static_assert(Std::is_same<T, void>::value || Std::is_same<PrimT<T>, Element>::value,
        "LoadDataWithStride explicit type mismatch");
    Impl::CheckCubeSpace<Hardware::L1>(src);
    if constexpr (config.isSetFMatrix) { Load3DSetFMatrixCal(params.l1H, params.l1W, params.padList); }
    if constexpr (config.isSetPadding) { Load3DSetPaddingCal(params.padValue); }
    ASCENDC_ASSERT(params.kExtension * sizeof(Element) % ONE_BLK_SIZE == 0 &&
        params.kStartPt * sizeof(Element) % ONE_BLK_SIZE == 0 &&
        params.mExtension % 16 == 0 && params.mStartPt % 16 == 0,
        { KERNEL_LOG(KERNEL_ERROR, "Invalid LoadDataWithStride matrix alignment"); });
    if constexpr (Impl::CubeOperandTraits<D>::isPointer) {
        static_assert(Impl::CubeOperandTraits<D>::space == Hardware::L0A ||
            Impl::CubeOperandTraits<D>::space == Hardware::L0B, "Invalid LoadDataWithStride destination");
    }
    ASCENDC_ASSERT(Impl::GetCubeSpace(dst) == Hardware::L0A || Impl::GetCubeSpace(dst) == Hardware::L0B,
        { KERNEL_LOG(KERNEL_ERROR, "Invalid LoadDataWithStride destination position"); });
    auto source = (__cbuf__ Element*)Impl::GetUnderlyingPtr(src);
    if constexpr (Impl::CubeMayUseSpace<D, Hardware::L0A>()) {
        if (Impl::GetCubeSpace(dst) == Hardware::L0A) {
            Impl::CheckCubeSpace<Hardware::L0A>(dst);
            LoadData3DV2L12L0AWithStrideCal((__ca__ Element*)Impl::GetUnderlyingPtr(dst), source, params);
        }
    }
    if constexpr (Impl::CubeMayUseSpace<D, Hardware::L0B>()) {
        if (Impl::GetCubeSpace(dst) == Hardware::L0B) {
            Impl::CheckCubeSpace<Hardware::L0B>(dst);
            LoadData3DV2L12L0BWithStrideCal((__cb__ Element*)Impl::GetUnderlyingPtr(dst), source, params);
        }
    }
}
} // namespace AscendC
#endif
#endif
