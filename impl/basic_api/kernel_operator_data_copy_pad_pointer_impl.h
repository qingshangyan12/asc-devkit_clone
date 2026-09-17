/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026. All rights reserved.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
/*!
 * \file kernel_operator_data_copy_pad_pointer_impl.h
 * \brief Pointer and mixed-operand DataCopyPad adapters.
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message("impl/basic_api/kernel_operator_data_copy_pad_pointer_impl.h is an internal header file. " \
                "Please include basic_api/kernel_operator_data_copy_intf.h instead.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_PAD_POINTER_IMPL_H__
#endif

#ifndef ASCENDC_KERNEL_OPERATOR_DATA_COPY_PAD_POINTER_IMPL_H
#define ASCENDC_KERNEL_OPERATOR_DATA_COPY_PAD_POINTER_IMPL_H

#include "utils/kernel_utils_dma_pad_traits.h"

#if !defined(ASCENDC_CPU_DEBUG) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 2201))
namespace AscendC {
namespace DmaInternal {

// Source casts are confined to read-only Impl boundaries, whose legacy signatures are non-const.
template <PaddingMode mode, bool hasMode, typename D, typename S, typename P, typename Pad>
__aicore__ inline void PadFromGm(D&& dst, S&& src, const P& params, const Pad& pad)
{
    using E = DmaElem<D>;
#ifdef ASCENDC_TIME_STAMP_ON
    PrintTimeStamp(static_cast<uint32_t>(TimeStampId::TIME_STAMP_MTE2_DATACOPY));
#endif
    if constexpr (!hasMode) {
        if ASCEND_IS_AIC {
            return;
        }
    }
    const Hardware position = DmaPosition(dst);
    if constexpr (DmaCanUse<D, Hardware::UB>::value) {
        if (position == Hardware::UB) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
            if constexpr (hasMode) {
                DataCopyPadGm2UBImpl<E, mode>(
                    GetUnderlyingPtr<Hardware::UB>(dst), const_cast<__gm__ E*>(GetUnderlyingPtr<Hardware::GM>(src)),
                    params, pad, DmaCacheMode(src));
            } else
#endif
            {
                DataCopyPadGm2UBImpl(
                    GetUnderlyingPtr<Hardware::UB>(dst), const_cast<__gm__ E*>(GetUnderlyingPtr<Hardware::GM>(src)),
                    params, pad);
            }
            return;
        }
    }
    if constexpr (DmaCanUse<D, Hardware::L1>::value) {
        if (position == Hardware::L1) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
            if constexpr (hasMode) {
                DataCopyPadGm2L1Impl<E, mode>(
                    GetUnderlyingPtr<Hardware::L1>(dst), const_cast<__gm__ E*>(GetUnderlyingPtr<Hardware::GM>(src)),
                    params, pad, DmaCacheMode(src));
            } else
#endif
            {
#if (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113)
                DataCopyPadGm2L1Impl(
                    GetUnderlyingPtr<Hardware::L1>(dst), const_cast<__gm__ E*>(GetUnderlyingPtr<Hardware::GM>(src)),
                    params, pad);
#else
                DataCopyPadGM2L1Impl(
                    GetUnderlyingPtr<Hardware::L1>(dst), const_cast<__gm__ E*>(GetUnderlyingPtr<Hardware::GM>(src)),
                    params, pad);
#endif
            }
            return;
        }
    }
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopyPad destination must be UB or L1"); });
}

template <PaddingMode mode, bool hasMode, typename D, typename S, typename P>
__aicore__ inline void PadToGm(D&& dst, S&& src, const P& params)
{
    using E = DmaElem<D>;
#if (__NPU_ARCH__ != 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    const Hardware position = DmaPosition(src);
    if constexpr (DmaCanUse<S, Hardware::UB>::value) {
        if (position == Hardware::UB) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
            if constexpr (hasMode) {
                DataCopyPadUB2GMImpl<E, mode>(
                    GetUnderlyingPtr<Hardware::GM>(dst), const_cast<__ubuf__ E*>(GetUnderlyingPtr<Hardware::UB>(src)),
                    params, DmaCacheMode(dst));
            } else
#endif
            {
                DataCopyPadUB2GMImpl(
                    GetUnderlyingPtr<Hardware::GM>(dst), const_cast<__ubuf__ E*>(GetUnderlyingPtr<Hardware::UB>(src)),
                    params);
            }
            return;
        }
    }
    if constexpr (DmaCanUse<S, Hardware::L1>::value) {
        if (position == Hardware::L1) {
            if constexpr (!hasMode) {
                DataCopyPadL12GMImpl(
                    GetUnderlyingPtr<Hardware::GM>(dst), const_cast<__cbuf__ E*>(GetUnderlyingPtr<Hardware::L1>(src)),
                    params);
#if (__NPU_ARCH__ == 5101)
            } else if constexpr (Std::is_same<P, DataCopyExtParams>::value) {
                DataCopyPadL12GMImpl<E, mode>(
                    GetUnderlyingPtr<Hardware::GM>(dst), const_cast<__cbuf__ E*>(GetUnderlyingPtr<Hardware::L1>(src)),
                    params);
#endif
            } else {
                ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopyPad source must be UB"); });
            }
            return;
        }
    }
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopyPad source must be UB or L1"); });
}

template <typename D, typename S, typename P>
__aicore__ inline void PadFromUbToL1(D&& dst, S&& src, const P& params, const Nd2NzParams& nd2nzParams)
{
    using E = DmaElem<D>;
    CheckNd2NzParams(nd2nzParams, "DataCopyPad with Nd2NzParams");
    if (DmaPosition(dst) != Hardware::L1 || DmaPosition(src) != Hardware::UB) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopyPad with Nd2NzParams requires UB to L1"); });
        return;
    }
    DataCopyPadUB2L1Impl(
        GetUnderlyingPtr<Hardware::L1>(dst), const_cast<__ubuf__ E*>(GetUnderlyingPtr<Hardware::UB>(src)), params,
        nd2nzParams);
}

template <typename D, typename S, typename U>
__aicore__ inline void PadTraitFromGm(
    D&& dst, S&& src, const DataCopyExtParams& params, const DataCopyPadExtParams<U>& pad)
{
    if ASCEND_IS_AIC {
        return;
    }
    if (DmaPosition(dst) != Hardware::UB) {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "DataCopyPad with TensorTrait requires UB destination"); });
        return;
    }
    DataCopyPadGm2UBImpl(
        GetUnderlyingPtr<Hardware::UB>(dst), const_cast<__gm__ U*>(GetUnderlyingPtr<Hardware::GM>(src)), params, pad);
}
} // namespace DmaInternal

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <
    typename T, PaddingMode mode, typename D, typename S,
    typename Std::enable_if<DmaInternal::PadGmToLocal<T, D, S>::value, bool>::type>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    D&& dst, S&& src, const DataCopyParams& params, const DataCopyPadParams& pad)
{
    DmaInternal::PadFromGm<mode, true>(dst, src, params, pad);
}

template <
    typename T, PaddingMode mode, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::PadLocalToGm<T, D, S>::value && DmaInternal::DmaCanUse<S, Hardware::UB>::value, bool>::type>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(D&& dst, S&& src, const DataCopyParams& params)
{
    DmaInternal::PadToGm<mode, true>(dst, src, params);
}
#else
template <
    typename T, typename D, typename S, typename Std::enable_if<DmaInternal::PadGmToLocal<T, D, S>::value, bool>::type>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    D&& dst, S&& src, const DataCopyParams& params, const DataCopyPadParams& pad)
{
    DmaInternal::PadFromGm<PaddingMode::Normal, false>(dst, src, params, pad);
}

template <
    typename T, typename D, typename S, typename Std::enable_if<DmaInternal::PadLocalToGm<T, D, S>::value, bool>::type>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(D&& dst, S&& src, const DataCopyParams& params)
{
    DmaInternal::PadToGm<PaddingMode::Normal, false>(dst, src, params);
}
#endif

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <
    typename T, PaddingMode mode, typename D, typename S, typename U,
    typename Std::enable_if<
        DmaInternal::PadGmToLocal<T, D, S>::value && Std::is_same<DmaInternal::DmaElem<D>, U>::value &&
            !DmaInternal::PadUsesTrait<T, D, S>::value,
        bool>::type>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    D&& dst, S&& src, const DataCopyExtParams& params, const DataCopyPadExtParams<U>& pad)
{
    DmaInternal::PadFromGm<mode, true>(dst, src, params, pad);
}

template <
    typename T, PaddingMode mode, typename D, typename S,
    typename Std::enable_if<
        DmaInternal::PadLocalToGm<T, D, S>::value &&
#if (__NPU_ARCH__ == 5101)
            (DmaInternal::DmaCanUse<S, Hardware::UB>::value || DmaInternal::DmaCanUse<S, Hardware::L1>::value),
#else
            DmaInternal::DmaCanUse<S, Hardware::UB>::value,
#endif
        bool>::type>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(D&& dst, S&& src, const DataCopyExtParams& params)
{
    DmaInternal::PadToGm<mode, true>(dst, src, params);
}
#else
template <
    typename T, typename D, typename S, typename U,
    typename Std::enable_if<
        DmaInternal::PadGmToLocal<T, D, S>::value && Std::is_same<DmaInternal::DmaElem<D>, U>::value &&
            !DmaInternal::PadUsesTrait<T, D, S>::value,
        bool>::type>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    D&& dst, S&& src, const DataCopyExtParams& params, const DataCopyPadExtParams<U>& pad)
{
    DmaInternal::PadFromGm<PaddingMode::Normal, false>(dst, src, params, pad);
}

template <
    typename T, typename D, typename S, typename Std::enable_if<DmaInternal::PadLocalToGm<T, D, S>::value, bool>::type>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(D&& dst, S&& src, const DataCopyExtParams& params)
{
    DmaInternal::PadToGm<PaddingMode::Normal, false>(dst, src, params);
}
#endif

template <
    typename T, typename U,
    typename Std::enable_if<
        Std::is_same<T, void>::value || (Std::is_same<PrimT<T>, U>::value && !Std::is_same<T, U>::value), bool>::type,
    typename D, typename S,
    typename Std::enable_if<
        DmaInternal::PadGmToLocal<T, D, S>::value && DmaInternal::PadUsesTrait<T, D, S>::value &&
            Std::is_same<DmaInternal::DmaElem<D>, U>::value && DmaInternal::DmaCanUse<D, Hardware::UB>::value,
        bool>::type>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    D&& dst, S&& src, const DataCopyExtParams& params, const DataCopyPadExtParams<U>& pad)
{
    DmaInternal::PadTraitFromGm(dst, src, params, pad);
}

template <
    typename T, typename D, typename S, typename Std::enable_if<DmaInternal::PadUbToL1<T, D, S>::value, bool>::type>
__aicore__ inline void DataCopyPad(D&& dst, S&& src, const DataCopyParams& params, const Nd2NzParams& nd2nzParams)
{
    DmaInternal::PadFromUbToL1(dst, src, params, nd2nzParams);
}

template <
    typename T, typename D, typename S, typename Std::enable_if<DmaInternal::PadUbToL1<T, D, S>::value, bool>::type>
__aicore__ inline void DataCopyPad(D&& dst, S&& src, const DataCopyExtParams& params, const Nd2NzParams& nd2nzParams)
{
    DmaInternal::PadFromUbToL1(dst, src, params, nd2nzParams);
}

} // namespace AscendC
#endif
#endif // ASCENDC_KERNEL_OPERATOR_DATA_COPY_PAD_POINTER_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_PAD_POINTER_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_PAD_POINTER_IMPL_H__
#endif
