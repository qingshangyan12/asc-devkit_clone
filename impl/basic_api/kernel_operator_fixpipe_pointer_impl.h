/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_FIXPIPE_POINTER_IMPL_H
#define ASCENDC_FIXPIPE_POINTER_IMPL_H
#include "kernel_cube_pointer_traits.h"

namespace AscendC {
namespace Impl {
template <typename P> struct CubeLegacyFixpipeParams { static constexpr bool value = false; };
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113 || \
    __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102
template <> struct CubeLegacyFixpipeParams<FixpipeParamsV220> { static constexpr bool value = true; };
#endif
#if __NPU_ARCH__ == 3002
template <> struct CubeLegacyFixpipeParams<FixpipeParamsM300> { static constexpr bool value = true; };
#elif __NPU_ARCH__ == 3102
template <> struct CubeLegacyFixpipeParams<FixpipeParamsM310> { static constexpr bool value = true; };
#endif
} // namespace Impl

template <typename D, typename S, const FixpipeConfig& config = CFG_ROW_MAJOR, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value &&
        Impl::CubeLegacyFixpipeParams<P>::value, bool>::type = true>
__aicore__ inline void Fixpipe(const D& dst, const S& src, const P& params)
{
    using T = Impl::CubeElement<D>;
    using U = typename Std::remove_const<Impl::CubeElement<S>>::type;
    static_assert(!Std::is_const<T>::value, "Fixpipe destination must be writable");
    Impl::CheckCubeSpace<Hardware::L0C>(src);
    if constexpr (Impl::CubeOperandTraits<D>::isGlobal) {
        FixpipeL0C2GMImpl<T, U, config>((__gm__ T*)Impl::GetUnderlyingPtr(dst),
            (__cc__ U*)Impl::GetUnderlyingPtr(src), params);
    } else {
        ASCENDC_ASSERT(Impl::GetCubeSpace(dst) == Hardware::L1 ||
            Impl::GetCubeSpace(dst) == Hardware::UB,
            { KERNEL_LOG(KERNEL_ERROR, "Fixpipe destination must be L1 or UB"); });
        if constexpr (Impl::CubeOperandTraits<D>::isPointer) {
            static_assert(Impl::CubeMayUseSpace<D, Hardware::L1>() ||
                Impl::CubeMayUseSpace<D, Hardware::UB>(), "Fixpipe destination must be L1 or UB");
        }
        if constexpr (Impl::CubeMayUseSpace<D, Hardware::L1>()) {
            if (Impl::GetCubeSpace(dst) == Hardware::L1) {
                FixpipeL0C2L1Impl<T, U, config>((__cbuf__ T*)(uint64_t)Impl::GetUnderlyingPtr(dst),
                    (__cc__ U*)Impl::GetUnderlyingPtr(src), params);
            }
        }
        if constexpr (Impl::CubeMayUseSpace<D, Hardware::UB>()) {
            if (Impl::GetCubeSpace(dst) == Hardware::UB) {
                FixpipeL0C2UBImpl<T, U, config>((__ubuf__ T*)(uint64_t)Impl::GetUnderlyingPtr(dst),
                    (__cc__ U*)Impl::GetUnderlyingPtr(src), params);
            }
        }
    }
}

template <typename D, typename S, const FixpipeConfig& config = CFG_ROW_MAJOR, typename W, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, S, W>::value &&
        Impl::CubeLegacyFixpipeParams<P>::value, bool>::type = true>
__aicore__ inline void Fixpipe(const D& dst, const S& src, const W& workspace, const P& params)
{
    using T = Impl::CubeElement<D>;
    using U = typename Std::remove_const<Impl::CubeElement<S>>::type;
    static_assert(!Std::is_const<T>::value, "Fixpipe destination must be writable");
    static_assert(Std::is_same<typename Std::remove_const<Impl::CubeElement<W>>::type, uint64_t>::value,
        "Fixpipe quantization workspace must contain uint64_t");
    Impl::CheckCubeSpace<Hardware::L0C>(src);
    Impl::CheckCubeSpace<Hardware::L1>(workspace);
    auto quant = (__cbuf__ uint64_t*)Impl::GetUnderlyingPtr(workspace);
    if constexpr (Impl::CubeOperandTraits<D>::isGlobal) {
        FixpipeL0C2GMImpl<T, U, config>((__gm__ T*)Impl::GetUnderlyingPtr(dst),
            (__cc__ U*)Impl::GetUnderlyingPtr(src), quant, params);
    } else {
        ASCENDC_ASSERT(Impl::GetCubeSpace(dst) == Hardware::L1 ||
            Impl::GetCubeSpace(dst) == Hardware::UB,
            { KERNEL_LOG(KERNEL_ERROR, "Fixpipe destination must be L1 or UB"); });
        if constexpr (Impl::CubeOperandTraits<D>::isPointer) {
            static_assert(Impl::CubeMayUseSpace<D, Hardware::L1>() ||
                Impl::CubeMayUseSpace<D, Hardware::UB>(), "Fixpipe destination must be L1 or UB");
        }
        if constexpr (Impl::CubeMayUseSpace<D, Hardware::L1>()) {
            if (Impl::GetCubeSpace(dst) == Hardware::L1) {
                FixpipeL0C2L1Impl<T, U, config>((__cbuf__ T*)(uint64_t)Impl::GetUnderlyingPtr(dst),
                    (__cc__ U*)Impl::GetUnderlyingPtr(src), quant, params);
            }
        }
        if constexpr (Impl::CubeMayUseSpace<D, Hardware::UB>()) {
            if (Impl::GetCubeSpace(dst) == Hardware::UB) {
                FixpipeL0C2UBImpl<T, U, config>((__ubuf__ T*)(uint64_t)Impl::GetUnderlyingPtr(dst),
                    (__cc__ U*)Impl::GetUnderlyingPtr(src), quant, params);
            }
        }
    }
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename D, typename S, const FixpipeConfig& config = CFG_ROW_MAJOR,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value, bool>::type = true>
__aicore__ inline void Fixpipe(const D& dst, const S& src,
    const FixpipeParamsArch3510<config.format>& params)
{
    using T = Impl::CubeElement<D>;
    using U = typename Std::remove_const<Impl::CubeElement<S>>::type;
    static_assert(!Std::is_const<T>::value, "Fixpipe destination must be writable");
    Impl::CheckCubeSpace<Hardware::L0C>(src);
    if constexpr (Impl::CubeOperandTraits<D>::isGlobal) {
        FixpipeL0C2GMImpl<T, U, config>((__gm__ T*)Impl::GetUnderlyingPtr(dst),
            (__cc__ U*)Impl::GetUnderlyingPtr(src), params, ExtractCacheMode(dst));
    } else if constexpr (config.isToUB) {
        Impl::CheckCubeSpace<Hardware::UB>(dst);
        FixpipeL0C2UBImpl<T, U, config>((__ubuf__ T*)Impl::GetUnderlyingPtr(dst),
            (__cc__ U*)Impl::GetUnderlyingPtr(src), params);
    } else {
        Impl::CheckCubeSpace<Hardware::L1>(dst);
        FixpipeL0C2L1Impl<T, U, config>((__cbuf__ T*)Impl::GetUnderlyingPtr(dst),
            (__cc__ U*)Impl::GetUnderlyingPtr(src), params);
    }
}

template <typename D, typename S, const FixpipeConfig& config = CFG_ROW_MAJOR, typename W,
    typename Std::enable_if<Impl::CubeHasPointer<D, S, W>::value, bool>::type = true>
__aicore__ inline void Fixpipe(const D& dst, const S& src, const W& workspace,
    const FixpipeParamsArch3510<config.format>& params)
{
    using T = Impl::CubeElement<D>;
    using U = typename Std::remove_const<Impl::CubeElement<S>>::type;
    static_assert(!Std::is_const<T>::value, "Fixpipe destination must be writable");
    static_assert(Std::is_same<typename Std::remove_const<Impl::CubeElement<W>>::type, uint64_t>::value,
        "Fixpipe quantization workspace must contain uint64_t");
    Impl::CheckCubeSpace<Hardware::L0C>(src);
    Impl::CheckCubeSpace<Hardware::L1>(workspace);
    if constexpr (Impl::CubeOperandTraits<D>::isGlobal) {
        FixpipeL0C2GMImpl<T, U, config>((__gm__ T*)Impl::GetUnderlyingPtr(dst),
            (__cc__ U*)Impl::GetUnderlyingPtr(src), (__cbuf__ uint64_t*)Impl::GetUnderlyingPtr(workspace),
            params, ExtractCacheMode(dst));
    } else if constexpr (config.isToUB) {
        Impl::CheckCubeSpace<Hardware::UB>(dst);
        FixpipeL0C2UBImpl<T, U, config>((__ubuf__ T*)Impl::GetUnderlyingPtr(dst),
            (__cc__ U*)Impl::GetUnderlyingPtr(src), (__cbuf__ uint64_t*)Impl::GetUnderlyingPtr(workspace), params);
    } else {
        Impl::CheckCubeSpace<Hardware::L1>(dst);
        FixpipeL0C2L1Impl<T, U, config>((__cbuf__ T*)Impl::GetUnderlyingPtr(dst),
            (__cc__ U*)Impl::GetUnderlyingPtr(src), (__cbuf__ uint64_t*)Impl::GetUnderlyingPtr(workspace), params);
    }
}

template <typename P, bool setRelu = false,
    typename Std::enable_if<Impl::CubeOperandTraits<P>::isPointer, bool>::type = true>
__aicore__ inline void SetFixPipeConfig(const P& preData, bool isUnitFlag = false)
{
    static_assert(Impl::CubeMayUseSpace<P, Hardware::FIXBUF>() ||
        Impl::CubeMayUseSpace<P, Hardware::L0C>(), "SetFixPipeConfig requires FIXBUF or L0C");
    if ASCEND_IS_AIC {
        uint64_t config = static_cast<uint64_t>(isUnitFlag) << 63;
        const uint64_t addr = (uint64_t)Impl::GetUnderlyingPtr(preData);
        if constexpr (setRelu) {
            config |= addr >> 6;
        } else {
            config |= (addr >> 7) << 8;
        }
        set_fpc(config);
    }
}

template <typename R, typename Q,
    typename Std::enable_if<Impl::CubeHasPointer<R, Q>::value, bool>::type = true>
__aicore__ inline void SetFixPipeConfig(const R& reluPre, const Q& quantPre, bool isUnitFlag = false)
{
    static_assert(Std::is_same<Impl::CubeElement<R>, Impl::CubeElement<Q>>::value,
        "SetFixPipeConfig input element types must match");
    Impl::CheckCubeSpace<Hardware::FIXBUF>(reluPre);
    Impl::CheckCubeSpace<Hardware::FIXBUF>(quantPre);
    if ASCEND_IS_AIC {
        const uint64_t config = ((uint64_t)Impl::GetUnderlyingPtr(reluPre) >> 6) |
            (((uint64_t)Impl::GetUnderlyingPtr(quantPre) >> 7) << 8) |
            (static_cast<uint64_t>(isUnitFlag) << 63);
        set_fpc(config);
    }
}
#endif
// Keep explicit element template arguments compatible with Tensor call sites.
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR,
    typename D, typename S, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value &&
        Std::is_same<T, Impl::CubeElement<D>>::value &&
        Std::is_same<U, typename Std::remove_const<Impl::CubeElement<S>>::type>::value, bool>::type = true>
__aicore__ inline void Fixpipe(const D& dst, const S& src, const P& params)
{
    Fixpipe<D, S, config>(dst, src, params);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR,
    typename WElement = uint64_t, typename D, typename S, typename W, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, S, W>::value &&
        Std::is_same<T, Impl::CubeElement<D>>::value &&
        Std::is_same<U, typename Std::remove_const<Impl::CubeElement<S>>::type>::value &&
        Std::is_same<WElement, typename Std::remove_const<Impl::CubeElement<W>>::type>::value,
        bool>::type = true>
__aicore__ inline void Fixpipe(const D& dst, const S& src, const W& workspace, const P& params)
{
    Fixpipe<D, S, config>(dst, src, workspace, params);
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename T, bool setRelu = false, typename P,
    typename Std::enable_if<Impl::CubeOperandTraits<P>::isPointer &&
        Std::is_same<T, Impl::CubeElement<P>>::value, bool>::type = true>
__aicore__ inline void SetFixPipeConfig(const P& preData, bool isUnitFlag = false)
{
    SetFixPipeConfig<P, setRelu>(preData, isUnitFlag);
}

template <typename T, typename R, typename Q,
    typename Std::enable_if<Impl::CubeHasPointer<R, Q>::value &&
        Std::is_same<T, Impl::CubeElement<R>>::value &&
        Std::is_same<T, Impl::CubeElement<Q>>::value, bool>::type = true>
__aicore__ inline void SetFixPipeConfig(const R& reluPre, const Q& quantPre, bool isUnitFlag = false)
{
    SetFixPipeConfig<R, Q>(reluPre, quantPre, isUnitFlag);
}
#endif
} // namespace AscendC
#endif
