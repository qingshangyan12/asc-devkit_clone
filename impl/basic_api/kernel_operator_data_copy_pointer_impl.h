/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_KERNEL_OPERATOR_DATA_COPY_POINTER_IMPL_H
#define ASCENDC_KERNEL_OPERATOR_DATA_COPY_POINTER_IMPL_H

#include "utils/kernel_utils_dma_copy_traits.h"

#if !defined(ASCENDC_CPU_DEBUG) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 2201))
namespace AscendC {
namespace DmaInternal {

#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void CopyUbToUb(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& params)
{
    DataCopyUB2UBImpl(dst, src, params);
}
#pragma end_pipe

template <typename D, typename S>
__aicore__ inline bool TryCopyGmInput(D&& dst, S&& src, const DataCopyParams& params)
{
    using ST = DmaElem<S>;
    const Hardware dstPos = DmaPosition(dst);
    if constexpr (CopyRoute<D, S, Hardware::UB, Hardware::GM>::value) {
        if (dstPos == Hardware::UB) {
            // Existing DMA implementations take a mutable pointer for a read-only source.
            DataCopyGM2UBImpl(
                GetUnderlyingPtr<Hardware::UB>(dst), const_cast<__gm__ ST*>(GetUnderlyingPtr<Hardware::GM>(src)), params
#if __NPU_ARCH__ == 3510
                ,
                DmaCacheMode(src)
#endif
            );
            return true;
        }
    }
    if constexpr (CopyRoute<D, S, Hardware::L1, Hardware::GM>::value) {
        if (dstPos == Hardware::L1) {
            DataCopyGM2L1Impl(
                GetUnderlyingPtr<Hardware::L1>(dst), const_cast<__gm__ ST*>(GetUnderlyingPtr<Hardware::GM>(src)), params
#if __NPU_ARCH__ == 3510
                ,
                DmaCacheMode(src)
#endif
            );
            return true;
        }
    }
    return false;
}

template <typename D, typename S>
__aicore__ inline bool TryCopyGmOutput(D&& dst, S&& src, const DataCopyParams& params)
{
    using ST = DmaElem<S>;
    const Hardware srcPos = DmaPosition(src);
    if constexpr (CopyRoute<D, S, Hardware::GM, Hardware::UB>::value) {
        if (srcPos == Hardware::UB) {
            DataCopyUB2GMImpl(
                GetUnderlyingPtr<Hardware::GM>(dst), const_cast<__ubuf__ ST*>(GetUnderlyingPtr<Hardware::UB>(src)),
                params
#if __NPU_ARCH__ == 3510
                ,
                DmaCacheMode(dst)
#endif
            );
            return true;
        }
    }
    if constexpr (CopyRoute<D, S, Hardware::GM, Hardware::L1>::value) {
        if (srcPos == Hardware::L1) {
            DataCopyL12GMImpl(
                GetUnderlyingPtr<Hardware::GM>(dst), const_cast<__cbuf__ ST*>(GetUnderlyingPtr<Hardware::L1>(src)),
                params
#if __NPU_ARCH__ == 3510
                ,
                DmaCacheMode(dst)
#endif
            );
            return true;
        }
    }
    return false;
}

template <typename D, typename S>
__aicore__ inline bool TryCopyUbLocal(D&& dst, S&& src, const DataCopyParams& params)
{
    using DT = DmaElem<D>;
    using ST = DmaElem<S>;
    const Hardware dstPos = DmaPosition(dst);
    const Hardware srcPos = DmaPosition(src);
    if constexpr (Std::is_same<DT, ST>::value && CopyRoute<D, S, Hardware::UB, Hardware::UB>::value) {
        if (srcPos == Hardware::UB && dstPos == Hardware::UB) {
            CopyUbToUb(
                GetUnderlyingPtr<Hardware::UB>(dst), const_cast<__ubuf__ ST*>(GetUnderlyingPtr<Hardware::UB>(src)),
                params);
            return true;
        }
    }
    if constexpr (Std::is_same<DT, ST>::value && CopyRoute<D, S, Hardware::L1, Hardware::UB>::value) {
        if (srcPos == Hardware::UB && dstPos == Hardware::L1) {
            DataCopyUB2L1Impl(
                GetUnderlyingPtr<Hardware::L1>(dst), const_cast<__ubuf__ ST*>(GetUnderlyingPtr<Hardware::UB>(src)),
                params);
            return true;
        }
    }
    return false;
}

template <typename D, typename S>
__aicore__ inline void CopyBias(D&& dst, S&& src, const DataCopyParams& params)
{
    using DT = DmaElem<D>;
    using ST = DmaElem<S>;
    constexpr bool sameType = Std::is_same<DT, ST>::value;
#if __NPU_ARCH__ == 3510
    constexpr bool bf16ToFloat = Std::is_same<ST, bfloat16_t>::value && Std::is_same<DT, float>::value;
    constexpr bool sameBiasType = sameType && !Std::is_same<ST, bfloat16_t>::value;
#else
    constexpr bool bf16ToFloat = false;
    constexpr bool sameBiasType = sameType;
#endif
    if constexpr (sameBiasType || bf16ToFloat) {
        DataCopyL12BTImpl(
            reinterpret_cast<uint64_t>(GetUnderlyingPtr<Hardware::BIAS>(dst)),
            const_cast<__cbuf__ ST*>(GetUnderlyingPtr<Hardware::L1>(src)), uint16_t(0), params);
    } else if constexpr (Std::is_same<ST, half>::value && Std::is_same<DT, float>::value) {
        DataCopyL12BTImpl(
            reinterpret_cast<uint64_t>(GetUnderlyingPtr<Hardware::BIAS>(dst)),
            const_cast<__cbuf__ ST*>(GetUnderlyingPtr<Hardware::L1>(src)), uint16_t(1), params);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Unsupported DataCopy bias dtype combination"); });
    }
}

template <typename D, typename S>
__aicore__ inline bool TryCopyL1Local(D&& dst, S&& src, const DataCopyParams& params)
{
    using DT = DmaElem<D>;
    using ST = DmaElem<S>;
    const Hardware dstPos = DmaPosition(dst);
    const Hardware srcPos = DmaPosition(src);
    if constexpr (Std::is_same<DT, ST>::value && CopyRoute<D, S, Hardware::UB, Hardware::L1>::value) {
        if (srcPos == Hardware::L1 && dstPos == Hardware::UB) {
#if __NPU_ARCH__ == 3510
            // Ordinary DataCopy is unsupported here; the dedicated L1ToUB API is separate.
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopy L1 to UB is unsupported on this device"); });
#else
            DataCopyL12UBImpl(
                GetUnderlyingPtr<Hardware::UB>(dst), const_cast<__cbuf__ ST*>(GetUnderlyingPtr<Hardware::L1>(src)),
                params);
#endif
            return true;
        }
    }
    if constexpr (CopyRoute<D, S, Hardware::BIAS, Hardware::L1>::value) {
        if (srcPos == Hardware::L1 && dstPos == Hardware::BIAS) {
            CopyBias(dst, src, params);
            return true;
        }
    }
    if constexpr (Std::is_same<DT, ST>::value && CopyRoute<D, S, Hardware::FIXBUF, Hardware::L1>::value) {
        if (srcPos == Hardware::L1 && dstPos == Hardware::FIXBUF) {
            DataCopyL12FBImpl(
                GetUnderlyingPtr<Hardware::FIXBUF>(dst), const_cast<__cbuf__ ST*>(GetUnderlyingPtr<Hardware::L1>(src)),
                params);
            return true;
        }
    }
    return false;
}

template <typename D, typename S>
__aicore__ inline void CopyParams(D&& dst, S&& src, const DataCopyParams& params)
{
    if constexpr (DmaTraits<S>::isGlobal) {
        if (TryCopyGmInput(dst, src, params)) {
            return;
        }
    } else if constexpr (DmaTraits<D>::isGlobal) {
        if (TryCopyGmOutput(dst, src, params)) {
            return;
        }
    } else if (DmaPosition(src) == Hardware::UB) {
        if (TryCopyUbLocal(dst, src, params)) {
            return;
        }
    } else if (DmaPosition(src) == Hardware::L1) {
        if (TryCopyL1Local(dst, src, params)) {
            return;
        }
    }
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Unsupported DataCopy pointer route"); });
}

template <typename D, typename S>
__aicore__ inline void CopyCount(D&& dst, S&& src, uint32_t count)
{
    using T = DmaElem<D>;
    DataCopyParams params;
    if constexpr (DmaTraits<D>::isLocal && DmaTraits<S>::isLocal) {
        params.blockLen = count / AscendCUtils::GetC0Count(sizeof(T));
        if (DmaPosition(src) == Hardware::L1) {
            if (DmaPosition(dst) == Hardware::BIAS) {
#if __NPU_ARCH__ == 3510
                params.blockLen = count / (32 / sizeof(T));
#else
                params.blockLen = count / (64 / sizeof(T));
#endif
            } else if (DmaPosition(dst) == Hardware::FIXBUF) {
                params.blockLen = count / (128 / sizeof(T));
            }
        }
    } else {
        DataCopyCheck<T>(count, params);
    }
    CopyParams(dst, src, params);
}

} // namespace DmaInternal

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::gmIn, bool>::type>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(D&& dst, S&& src, const DataCopyParams& params)
{
    DmaInternal::CopyParams(dst, src, params);
}

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::gmOut, bool>::type>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(D&& dst, S&& src, const DataCopyParams& params)
{
    DmaInternal::CopyParams(dst, src, params);
}

template <
    typename T, typename U, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, U, D, S>::value && DmaInternal::BasicRoute<D, S>::local, bool>::type>
__aicore__ inline void DataCopy(D&& dst, S&& src, const DataCopyParams& params)
{
    DmaInternal::CopyParams(dst, src, params);
}

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::gmIn, bool>::type>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(D&& dst, S&& src, const uint32_t count)
{
    DmaInternal::CopyCount(dst, src, count);
}

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::gmOut, bool>::type>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(D&& dst, S&& src, const uint32_t count)
{
    DmaInternal::CopyCount(dst, src, count);
}

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::local &&
            DmaInternal::BasicRoute<D, S>::same,
        bool>::type>
__aicore__ inline void DataCopy(D&& dst, S&& src, const uint32_t count)
{
    DmaInternal::CopyCount(dst, src, count);
}

namespace DmaInternal {
template <bool enableSmallC0, typename D, typename S>
__aicore__ inline void CopyNd2Nz(D&& dst, S&& src, const Nd2NzParams& params)
{
    using T = DmaElem<D>;
    CheckNd2NzParams(params, "DataCopy pointer with Nd2NzParams");
    if constexpr (CopyRoute<D, S, Hardware::L1, Hardware::GM>::value) {
        if (DmaPosition(dst) == Hardware::L1) {
#if __NPU_ARCH__ == 3510
            DataCopyGM2L1ND2NZImpl<T, enableSmallC0>(
                GetUnderlyingPtr<Hardware::L1>(dst), const_cast<__gm__ T*>(GetUnderlyingPtr<Hardware::GM>(src)), params,
                DmaCacheMode(src));
#else
            DataCopyGM2L1ND2NZImpl(
                GetUnderlyingPtr<Hardware::L1>(dst), const_cast<__gm__ T*>(GetUnderlyingPtr<Hardware::GM>(src)),
                params);
#endif
            return;
        }
    }
    if constexpr (!enableSmallC0 && CopyRoute<D, S, Hardware::UB, Hardware::GM>::value) {
        if (DmaPosition(dst) == Hardware::UB) {
            DataCopyGM2UBND2NZImpl(
                GetUnderlyingPtr<Hardware::UB>(dst), const_cast<__gm__ T*>(GetUnderlyingPtr<Hardware::GM>(src)), params
#if __NPU_ARCH__ == 3510
                ,
                DmaCacheMode(src)
#endif
            );
            return;
        }
    }
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Unsupported ND2NZ pointer route"); });
}
} // namespace DmaInternal

#if __NPU_ARCH__ == 3510
template <
    typename T, bool enableSmallC0, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::gmIn &&
            (!enableSmallC0 || DmaInternal::CopyRoute<D, S, Hardware::L1, Hardware::GM>::value),
        bool>::type>
#else
template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::gmIn, bool>::type>
#endif
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(D&& dst, S&& src, const Nd2NzParams& params)
{
#if __NPU_ARCH__ == 3510
    DmaInternal::CopyNd2Nz<enableSmallC0>(dst, src, params);
#else
    DmaInternal::CopyNd2Nz<false>(dst, src, params);
#endif
}

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::same &&
            DmaInternal::CopyRoute<D, S, Hardware::L1, Hardware::UB>::value,
        bool>::type>
__aicore__ inline void DataCopy(D&& dst, S&& src, const Nd2NzParams& params)
{
    using E = DmaInternal::DmaElem<S>;
    if (DmaInternal::DmaPosition(dst) != Hardware::L1 || DmaInternal::DmaPosition(src) != Hardware::UB) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "ND2NZ local copy requires UB to L1"); });
        return;
    }
    CheckNd2NzParams(params, "DataCopy pointer with Nd2NzParams");
    DataCopyUB2L1ND2NZImpl(
        DmaInternal::GetUnderlyingPtr<Hardware::L1>(dst),
        const_cast<__ubuf__ E*>(DmaInternal::GetUnderlyingPtr<Hardware::UB>(src)), params);
}

#if __NPU_ARCH__ == 3510
template <
    typename T, bool enableSmallC0, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::same &&
            DmaInternal::CopyRoute<D, S, Hardware::L1, Hardware::GM>::value,
        bool>::type>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(D&& dst, S&& src, const Dn2NzParams& params)
{
    using E = DmaInternal::DmaElem<S>;
    if (DmaInternal::DmaPosition(dst) != Hardware::L1) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DN2NZ requires GM to L1"); });
        return;
    }
    DataCopyGM2L1DN2NZImpl(
        DmaInternal::GetUnderlyingPtr<Hardware::L1>(dst),
        const_cast<__gm__ E*>(DmaInternal::GetUnderlyingPtr<Hardware::GM>(src)), params, enableSmallC0,
        DmaInternal::DmaCacheMode(src));
}

template <
    typename T, uint8_t dim, const NdDmaConfig& config, typename Std::enable_if<(dim >= 1 && dim <= 5), bool>::type,
    typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::same &&
            DmaInternal::CopyRoute<D, S, Hardware::UB, Hardware::GM>::value,
        bool>::type>
__aicore__ inline void DataCopy(D&& dst, S&& src, const MultiCopyParams<T, dim>& params)
{
    if ASCEND_IS_AIC {
        return;
    }
    if (DmaInternal::DmaPosition(dst) != Hardware::UB) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "NDDMA requires GM to UB"); });
        return;
    }
    DataCopyWithNDDMAImpl<T, dim, config>(
        DmaInternal::GetUnderlyingPtr<Hardware::UB>(dst),
        const_cast<__gm__ PrimT<T>*>(DmaInternal::GetUnderlyingPtr<Hardware::GM>(src)), params.loopInfo,
        params.constantValue, DmaInternal::DmaCacheMode(src));
}

template <
    typename T, uint8_t subBlockId, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::same &&
            DmaInternal::CopyRoute<D, S, Hardware::UB, Hardware::L1>::value,
        bool>::type>
__aicore__ inline void DataCopyL1ToUB(D&& dst, S&& src, const DataCopyParams& params)
{
    using E = DmaInternal::DmaElem<S>;
    if (DmaInternal::DmaPosition(dst) != Hardware::UB || DmaInternal::DmaPosition(src) != Hardware::L1) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopyL1ToUB requires L1 to UB"); });
        return;
    }
    DataCopyL12UBImpl<E, subBlockId>(
        DmaInternal::GetUnderlyingPtr<Hardware::UB>(dst),
        const_cast<__cbuf__ E*>(DmaInternal::GetUnderlyingPtr<Hardware::L1>(src)), params);
}

template <
    typename T, uint8_t subBlockId, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::same &&
            DmaInternal::CopyRoute<D, S, Hardware::UB, Hardware::L1>::value,
        bool>::type>
__aicore__ inline void DataCopyL1ToUB(D&& dst, S&& src, const uint32_t count)
{
    DataCopyParams params;
    params.blockLen = count / AscendCUtils::GetC0Count(sizeof(DmaInternal::DmaElem<D>));
    DataCopyL1ToUB<DmaInternal::DmaElem<D>, subBlockId>(dst, src, params);
}
#endif

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::same &&
            DmaInternal::CopyRoute<D, S, Hardware::GM, Hardware::UB>::value,
        bool>::type>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(D&& dst, S&& src, const Nz2NdParamsFull& params)
{
    using E = DmaInternal::DmaElem<S>;
    CheckNz2NdParams(params);
    if (DmaInternal::DmaPosition(src) != Hardware::UB) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "NZ2ND requires UB to GM"); });
        return;
    }
    DataCopyUB2GMNZ2NDImpl(
        DmaInternal::GetUnderlyingPtr<Hardware::GM>(dst),
        const_cast<__ubuf__ E*>(DmaInternal::GetUnderlyingPtr<Hardware::UB>(src)), params
#if __NPU_ARCH__ == 3510
        ,
        DmaInternal::DmaCacheMode(dst)
#endif
    );
}

template <
    typename T, typename U, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, U, D, S>::value && (DmaInternal::CopyRoute<D, S, Hardware::GM, Hardware::L0C>::value ||
                                                     DmaInternal::CopyRoute<D, S, Hardware::L1, Hardware::L0C>::value),
        bool>::type>
__aicore__ inline void DataCopy(D&& dst, S&& src, const DataCopyCO12DstParams& params)
{
    using E = DmaInternal::DmaElem<S>;
    if (DmaInternal::DmaPosition(src) != Hardware::L0C) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "CO12 DataCopy requires L0C source"); });
        return;
    }
    if constexpr (DmaInternal::DmaTraits<D>::isGlobal) {
        DataCopyL0C2GMImpl(
            DmaInternal::GetUnderlyingPtr<Hardware::GM>(dst),
            const_cast<__cc__ E*>(DmaInternal::GetUnderlyingPtr<Hardware::L0C>(src)), params
#if __NPU_ARCH__ == 3510
            ,
            DmaInternal::DmaCacheMode(dst)
#endif
        );
    } else {
        if (DmaInternal::DmaPosition(dst) != Hardware::L1) {
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "CO12 local DataCopy requires L1 destination"); });
            return;
        }
        DataCopyL0C2L1Impl(
            DmaInternal::GetUnderlyingPtr<Hardware::L1>(dst),
            const_cast<__cc__ E*>(DmaInternal::GetUnderlyingPtr<Hardware::L0C>(src)), params);
    }
}

namespace DmaInternal {

template <typename D, typename S>
__aicore__ inline bool TryCopyEnhancedFromL0C(
    D&& dst, S&& src, const DataCopyParams& params, const DataCopyEnhancedParams& enhanced)
{
    using ST = DmaElem<S>;
    using R = EnhancedRoute<D, S>;
    const Hardware dstPos = DmaPosition(dst);
    const Hardware srcPos = DmaPosition(src);
    if constexpr ((R::same || R::int32ToUb || R::floatToHalf) && CopyRoute<D, S, Hardware::UB, Hardware::L0C>::value) {
        if (srcPos == Hardware::L0C && dstPos == Hardware::UB) {
            DataCopyL0C2UBImpl(
                GetUnderlyingPtr<Hardware::UB>(dst), const_cast<__cc__ ST*>(GetUnderlyingPtr<Hardware::L0C>(src)),
                params, enhanced);
            return true;
        }
    }
    return false;
}

template <typename D, typename S>
__aicore__ inline bool TryCopyEnhancedToL0C(
    D&& dst, S&& src, const DataCopyParams& params, const DataCopyEnhancedParams& enhanced)
{
    using ST = DmaElem<S>;
    using R = EnhancedRoute<D, S>;
    const Hardware dstPos = DmaPosition(dst);
    const Hardware srcPos = DmaPosition(src);
    if constexpr ((R::same || R::halfToFloat) && CopyRoute<D, S, Hardware::L0C, Hardware::UB>::value) {
        if (srcPos == Hardware::UB && dstPos == Hardware::L0C) {
            DataCopyUB2L0CImpl(
                GetUnderlyingPtr<Hardware::L0C>(dst), const_cast<__ubuf__ ST*>(GetUnderlyingPtr<Hardware::UB>(src)),
                params, enhanced);
            return true;
        }
    }
    if constexpr (
        (R::same || R::floatToHalf || R::floatToBf16) && CopyRoute<D, S, Hardware::L0C, Hardware::L1>::value) {
        if (srcPos == Hardware::L1 && dstPos == Hardware::L0C) {
            DataCopyL12L0CImpl(
                GetUnderlyingPtr<Hardware::L0C>(dst), const_cast<__cbuf__ ST*>(GetUnderlyingPtr<Hardware::L1>(src)),
                params, enhanced);
            return true;
        }
    }
    return false;
}

template <typename D, typename S>
__aicore__ inline void CopyEnhanced(
    D&& dst, S&& src, const DataCopyParams& params, const DataCopyEnhancedParams& enhanced)
{
    if (TryCopyEnhancedFromL0C(dst, src, params, enhanced) || TryCopyEnhancedToL0C(dst, src, params, enhanced)) {
        return;
    }
    using R = EnhancedRoute<D, S>;
    const Hardware dstPos = DmaPosition(dst);
    const Hardware srcPos = DmaPosition(src);
    if constexpr (R::same) {
        if ((srcPos == Hardware::UB && (dstPos == Hardware::UB || dstPos == Hardware::L1)) ||
            (srcPos == Hardware::L1 && dstPos == Hardware::UB)) {
            CopyParams(dst, src, params);
            return;
        }
    }
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Unsupported Enhanced DataCopy pointer route"); });
}
} // namespace DmaInternal

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::gmIn, bool>::type>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    D&& dst, S&& src, const DataCopyParams& params, const DataCopyEnhancedParams& enhanced)
{
    DmaInternal::CopyParams(dst, src, params);
}

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::gmOut, bool>::type>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    D&& dst, S&& src, const DataCopyParams& params, const DataCopyEnhancedParams& enhanced)
{
    DmaInternal::CopyParams(dst, src, params);
}

template <
    typename T, typename U, bool enabled, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, U, D, S>::value && DmaInternal::EnhancedRoute<D, S>::local &&
            !DmaInternal::EnhancedRoute<D, S>::vector,
        bool>::type>
__aicore__ inline void DataCopy(D&& dst, S&& src, const DataCopyParams& params, const DataCopyEnhancedParams& enhanced)
{
    DmaInternal::CopyEnhanced(dst, src, params, enhanced);
}

template <
    typename T, typename U, bool enabled, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, U, D, S>::value && DmaInternal::EnhancedRoute<D, S>::local &&
            DmaInternal::EnhancedRoute<D, S>::vector,
        bool>::type>
__aicore__ inline __inout_pipe__(V) void DataCopy(
    D&& dst, S&& src, const DataCopyParams& params, const DataCopyEnhancedParams& enhanced)
{
    DmaInternal::CopyEnhanced(dst, src, params, enhanced);
}

namespace DmaInternal {
template <typename O>
__aicore__ inline bool CopySliceShape(const O& operand, const SliceInfo* slice, uint32_t dim, uint32_t* shape)
{
    uint64_t volume = 1;
    for (uint32_t i = 0; i < dim; ++i) {
        if (slice[0].shapeValue != 0) {
            shape[i] = slice[i].shapeValue;
        } else if constexpr (DmaTraits<O>::isTensor) {
            if (i >= operand.GetShapeInfo().shapeDim) {
                return false;
            }
            shape[i] = operand.GetShapeInfo().shape[i];
        } else {
            return false;
        }
        if (shape[i] == 0 || slice[i].startIndex > slice[i].endIndex || slice[i].endIndex >= shape[i]) {
            return false;
        }
        // The existing offset-list helpers use uint32_t arithmetic.
        if (volume > uint64_t(UINT32_MAX) / shape[i] || slice[i].stride == UINT32_MAX ||
            uint64_t(slice[i].endIndex) - slice[i].startIndex + 1 + slice[i].stride > UINT32_MAX) {
            return false;
        }
        volume *= shape[i];
    }
    return true;
}

__aicore__ inline bool CopySliceCounts(const SliceInfo* dstSlice, const SliceInfo* srcSlice, uint32_t dim)
{
    uint64_t srcSlices = 1;
    uint64_t dstSlices = 1;
    for (uint32_t i = 0; i < dim; ++i) {
        if (srcSlice[i].burstLen != dstSlice[i].burstLen || srcSlice[i].burstLen == 0) {
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopy Slice burst lengths must match"); });
            return false;
        }
        if (i != 0) {
            srcSlices *= (uint64_t(srcSlice[i].endIndex) - srcSlice[i].startIndex + 1 + srcSlice[i].stride) /
                         (uint64_t(1) + srcSlice[i].stride);
            dstSlices *= (uint64_t(dstSlice[i].endIndex) - dstSlice[i].startIndex + 1 + dstSlice[i].stride) /
                         (uint64_t(1) + dstSlice[i].stride);
        }
        if (srcSlices > MAX_SLICE_SIZE || dstSlices > MAX_SLICE_SIZE) {
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopy Slice exceeds offset list capacity"); });
            return false;
        }
    }
    if (srcSlices != dstSlices) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopy Slice counts must match"); });
        return false;
    }
    return true;
}

template <typename T, bool gmIn>
__aicore__ inline bool CopySliceParams(
    const SliceInfo* dstSlice, const SliceInfo* srcSlice, DataCopyParams& params, bool& alignedGm)
{
    const uint64_t burstElements = uint64_t(srcSlice[0].burstLen) * AscendCUtils::GetC0Count(sizeof(T));
    const uint64_t srcSliceLen = burstElements + srcSlice[0].stride;
    const uint64_t dstSliceLen = burstElements + dstSlice[0].stride;
    const uint64_t srcBlocks =
        (uint64_t(srcSlice[0].endIndex) - srcSlice[0].startIndex + 1 + srcSlice[0].stride) / srcSliceLen;
    const uint64_t dstBlocks =
        (uint64_t(dstSlice[0].endIndex) - dstSlice[0].startIndex + 1 + dstSlice[0].stride) / dstSliceLen;
    const uint64_t srcStrideBytes = uint64_t(srcSlice[0].stride) * sizeof(T);
    const uint64_t dstStrideBytes = uint64_t(dstSlice[0].stride) * sizeof(T);
    alignedGm = (gmIn ? srcStrideBytes : dstStrideBytes) % ONE_BLK_SIZE == 0;
    const uint64_t localStride = (gmIn ? dstStrideBytes : srcStrideBytes) / ONE_BLK_SIZE;
    const uint64_t gmStride = (gmIn ? srcStrideBytes : dstStrideBytes) / (alignedGm ? ONE_BLK_SIZE : 1);
    if (srcBlocks != dstBlocks || srcBlocks > UINT12_MAX || srcSlice[0].burstLen > UINT16_MAX ||
        localStride > UINT16_MAX || gmStride > UINT16_MAX ||
        (gmIn ? dstStrideBytes : srcStrideBytes) % ONE_BLK_SIZE != 0) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Invalid DataCopy Slice block count or stride"); });
        return false;
    }
    params.blockLen = srcSlice[0].burstLen;
    params.blockCount = static_cast<uint16_t>(srcBlocks);
    params.srcStride = static_cast<uint16_t>(gmIn ? gmStride : localStride);
    params.dstStride = static_cast<uint16_t>(gmIn ? localStride : gmStride);
    return true;
}

struct CopySlicePlan {
    DataCopyParams params;
    bool alignedGm;
    uint32_t srcOffsets[MAX_SLICE_SIZE];
    uint32_t dstOffsets[MAX_SLICE_SIZE];
    uint32_t srcStart;
    uint32_t dstStart;
    uint32_t count;
};

template <typename D, typename S>
__aicore__ inline void CopySliceGmInput(D&& dst, S&& src, const CopySlicePlan& plan)
{
    using T = DmaElem<D>;
    for (uint32_t i = 0; i < plan.count; ++i) {
        auto dstPtr = GetUnderlyingPtr<Hardware::UB>(dst) + plan.dstStart + plan.dstOffsets[i];
        auto srcPtr = const_cast<__gm__ T*>(GetUnderlyingPtr<Hardware::GM>(src)) + plan.srcStart + plan.srcOffsets[i];
        if (plan.alignedGm) {
            DataCopyGM2UBImpl(
                dstPtr, srcPtr, plan.params
#if __NPU_ARCH__ == 3510
                ,
                DmaCacheMode(src)
#endif
            );
        } else {
            DataCopySliceGm2UBImpl(
                dstPtr, srcPtr, plan.params
#if __NPU_ARCH__ == 3510
                ,
                DmaCacheMode(src)
#endif
            );
        }
    }
}

template <typename D, typename S>
__aicore__ inline void CopySliceGmOutput(D&& dst, S&& src, const CopySlicePlan& plan)
{
    using T = DmaElem<D>;
    for (uint32_t i = 0; i < plan.count; ++i) {
        auto dstPtr = GetUnderlyingPtr<Hardware::GM>(dst) + plan.dstStart + plan.dstOffsets[i];
        auto srcPtr = const_cast<__ubuf__ T*>(GetUnderlyingPtr<Hardware::UB>(src)) + plan.srcStart + plan.srcOffsets[i];
        if (plan.alignedGm) {
            DataCopyUB2GMImpl(
                dstPtr, srcPtr, plan.params
#if __NPU_ARCH__ == 3510
                ,
                DmaCacheMode(dst)
#endif
            );
        } else {
            DataCopySliceUB2GMImpl(
                dstPtr, srcPtr, plan.params
#if __NPU_ARCH__ == 3510
                ,
                DmaCacheMode(dst)
#endif
            );
        }
    }
}
template <typename D, typename S>
__aicore__ inline void CopySlice(D&& dst, S&& src, const SliceInfo* dstSlice, const SliceInfo* srcSlice, uint32_t dim)
{
    using T = DmaElem<D>;
    constexpr bool gmIn = DmaTraits<S>::isGlobal;
    if (dim == 0 || dim > K_MAX_SHAPE_DIM ||
        (gmIn ? DmaPosition(dst) != Hardware::UB : DmaPosition(src) != Hardware::UB)) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Invalid DataCopy Slice dimension or route"); });
        return;
    }
    uint32_t srcShape[K_MAX_SHAPE_DIM];
    uint32_t dstShape[K_MAX_SHAPE_DIM];
    if (!CopySliceShape(src, srcSlice, dim, srcShape) || !CopySliceShape(dst, dstSlice, dim, dstShape)) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Pointer Slice requires complete valid shape values"); });
        return;
    }
    if (!CopySliceCounts(dstSlice, srcSlice, dim)) {
        return;
    }
    CopySlicePlan plan;
    if (!CopySliceParams<T, gmIn>(dstSlice, srcSlice, plan.params, plan.alignedGm)) {
        return;
    }
    uint32_t dstCount = 0;
    DataCopyGetOffsetList(srcSlice, srcShape, dim, &plan.count, plan.srcOffsets);
    DataCopyGetOffsetList(dstSlice, dstShape, dim, &dstCount, plan.dstOffsets);
    plan.srcStart = DataCopyGetPhyStartIndex(srcSlice, srcShape, dim);
    plan.dstStart = DataCopyGetPhyStartIndex(dstSlice, dstShape, dim);
    if constexpr (gmIn) {
        CopySliceGmInput(dst, src, plan);
    } else {
        CopySliceGmOutput(dst, src, plan);
    }
}
} // namespace DmaInternal

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::same &&
            DmaInternal::CopyRoute<D, S, Hardware::UB, Hardware::GM>::value,
        bool>::type>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    D&& dst, S&& src, const SliceInfo dstSlice[], const SliceInfo srcSlice[], const uint32_t dim)
{
    DmaInternal::CopySlice(dst, src, dstSlice, srcSlice, dim);
}

template <
    typename T, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::CopyPair<T, T, D, S>::value && DmaInternal::BasicRoute<D, S>::same &&
            DmaInternal::CopyRoute<D, S, Hardware::GM, Hardware::UB>::value,
        bool>::type>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    D&& dst, S&& src, const SliceInfo dstSlice[], const SliceInfo srcSlice[], const uint32_t dim)
{
    DmaInternal::CopySlice(dst, src, dstSlice, srcSlice, dim);
}

} // namespace AscendC
#endif
#endif // ASCENDC_KERNEL_OPERATOR_DATA_COPY_POINTER_IMPL_H
