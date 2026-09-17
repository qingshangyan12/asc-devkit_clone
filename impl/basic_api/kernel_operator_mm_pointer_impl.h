/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * Licensed under CANN Open Software License Agreement Version 2.0.
 */
#ifndef ASCENDC_MM_POINTER_IMPL_H
#define ASCENDC_MM_POINTER_IMPL_H
#include "kernel_cube_pointer_traits.h"
namespace AscendC {
#if __NPU_ARCH__ == 5102
template <typename D, typename S,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value, bool>::type = true>
__aicore__ inline __inout_pipe__(MTE2) void LoadData(const D& dst, const S& src,
    const LoadData2DParamsV2& params, const Nd2NzParamsV2& nd2nz)
{
    Impl::CheckCubeSpace<Hardware::L1>(dst);
    Impl::CheckCubeSpace<Hardware::GM>(src);
    static_assert(!Std::is_const<Impl::CubeElement<D>>::value, "CUBE destination must be writable");
    LoadData2DGM2L1Cal((__cbuf__ Impl::CubeValue<D>*)Impl::GetUnderlyingPtr(dst),
        (__gm__ Impl::CubeValue<S>*)Impl::GetUnderlyingPtr(src), params, nd2nz);
}
#endif
namespace Impl {
template <bool Mx, typename D, typename A, typename B, typename Bias, typename P>
__aicore__ inline void CubeMmadBias(const D& dst, const A& a, const B& b, const Bias& bias, const P& params)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    if constexpr (Std::is_same<P, MmadParams>::value) { CheckMmadParamsCommon(params, "Mmad"); }
#endif
    CheckCubeSpace<Hardware::L0C>(dst);
    CheckCubeSpace<Hardware::L0A>(a);
    CheckCubeSpace<Hardware::L0B>(b);
    static_assert(!Std::is_const<CubeElement<D>>::value, "CUBE destination must be writable");
    if constexpr (CubeOperandTraits<Bias>::isPointer) {
        static_assert(CubeOperandTraits<Bias>::space == Hardware::BIAS ||
            CubeOperandTraits<Bias>::space == Hardware::L0C, "Invalid bias address space");
    } else {
        ASCENDC_ASSERT(GetCubeSpace(bias) == Hardware::BIAS || GetCubeSpace(bias) == Hardware::L0C,
            { KERNEL_LOG(KERNEL_ERROR, "Mmad bias must be in BIAS or L0C"); });
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
        CheckTensorAlignment(bias, 128, "bias", "Mmad");
#endif
    }
    auto d = (__cc__ CubeElement<D>*)GetUnderlyingPtr(dst);
    auto x = (__ca__ CubeValue<A>*)GetUnderlyingPtr(a);
    auto y = (__cb__ CubeValue<B>*)GetUnderlyingPtr(b);
    const uint64_t biasAddr = (uint64_t)GetUnderlyingPtr(bias);
    if constexpr (Std::is_same<P, MmadParams>::value) {
        const bool biasTable = GetCubeSpace(bias) == Hardware::BIAS;
        if constexpr (Mx) { MmadMxCal(d, x, y, biasAddr, params, biasTable); }
        else { MmadCal(d, x, y, biasAddr, params, biasTable); }
    } else {
        auto mutableParams = params;
        if constexpr (Mx) { MmadMxCal(d, x, y, biasAddr, mutableParams); }
        else { MmadCal(d, x, y, biasAddr, mutableParams); }
    }
}
} // namespace Impl
template <typename D, typename A, typename B, typename Bias, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B, Bias>::value &&
        Std::is_same<P, MmadParams>::value, bool>::type = true>
__aicore__ inline void Mmad(const D& dst, const A& a, const B& b, const Bias& bias, const P& params)
{
    Impl::CubeMmadBias<false>(dst, a, b, bias, params);
}
#if __NPU_ARCH__ == 3510
template <typename D, typename A, typename B, typename Bias,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B, Bias>::value, bool>::type = true>
__aicore__ inline void Mmad(const D& dst, const A& a, const B& b, const Bias& bias, const MmadBitModeParams& params)
{
    Impl::CubeMmadBias<false>(dst, a, b, bias, params);
}
template <typename D, typename A, typename B, typename Bias, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B, Bias>::value &&
        (Std::is_same<P, MmadParams>::value || Std::is_same<P, MmadBitModeParams>::value), bool>::type = true>
__aicore__ inline void MmadMx(const D& dst, const A& a, const B& b, const Bias& bias, const P& params)
{
    Impl::CubeMmadBias<true>(dst, a, b, bias, params);
}
template <typename D, typename A, typename B, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B>::value &&
        (Std::is_same<P, MmadParams>::value || Std::is_same<P, MmadBitModeParams>::value), bool>::type = true>
__aicore__ inline void MmadMx(const D& dst, const A& a, const B& b, const P& params)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    if constexpr (Std::is_same<P, MmadParams>::value) { CheckMmadParamsCommon(params, "MmadMx"); }
#endif
    Impl::CheckCubeSpace<Hardware::L0C>(dst);
    Impl::CheckCubeSpace<Hardware::L0A>(a);
    Impl::CheckCubeSpace<Hardware::L0B>(b);
    static_assert(!Std::is_const<Impl::CubeElement<D>>::value, "CUBE destination must be writable");
    auto mutableParams = params;
    MmadMxCal((__cc__ Impl::CubeElement<D>*)Impl::GetUnderlyingPtr(dst),
        (__ca__ Impl::CubeValue<A>*)Impl::GetUnderlyingPtr(a),
        (__cb__ Impl::CubeValue<B>*)Impl::GetUnderlyingPtr(b), mutableParams);
}
template <typename D, typename A, typename B,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B>::value, bool>::type = true>
__aicore__ inline void Mmad(const D& dst, const A& a, const B& b, const MmadBitModeParams& params)
{
    Impl::CheckCubeSpace<Hardware::L0C>(dst);
    Impl::CheckCubeSpace<Hardware::L0A>(a);
    Impl::CheckCubeSpace<Hardware::L0B>(b);
    static_assert(!Std::is_const<Impl::CubeElement<D>>::value, "CUBE destination must be writable");
    auto mutableParams = params;
    MmadCal((__cc__ Impl::CubeElement<D>*)Impl::GetUnderlyingPtr(dst),
        (__ca__ Impl::CubeValue<A>*)Impl::GetUnderlyingPtr(a),
        (__cb__ Impl::CubeValue<B>*)Impl::GetUnderlyingPtr(b), mutableParams);
}
#endif
template <typename D, typename A, typename B,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B>::value, bool>::type = true>
__aicore__ inline void Mmad(const D& dst, const A& a, const B& b, const MmadParams& params)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckMmadParamsCommon(params, "Mmad");
#endif
    Impl::CheckCubeSpace<Hardware::L0C>(dst);
    Impl::CheckCubeSpace<Hardware::L0A>(a);
    Impl::CheckCubeSpace<Hardware::L0B>(b);
    static_assert(!Std::is_const<Impl::CubeElement<D>>::value, "CUBE destination must be writable");
    MmadCal((__cc__ Impl::CubeElement<D>*)Impl::GetUnderlyingPtr(dst),
        (__ca__ Impl::CubeValue<A>*)Impl::GetUnderlyingPtr(a),
        (__cb__ Impl::CubeValue<B>*)Impl::GetUnderlyingPtr(b), params);
}

template <typename D, typename S, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value &&
        (Std::is_same<P, LoadData2DParams>::value || Std::is_same<P, LoadData2DParamsV2>::value), bool>::type = true>
__aicore__ inline void LoadData(const D& dst, const S& src, const P& params)
{
    using T = Impl::CubeElement<D>;
    static_assert(!Std::is_const<T>::value, "CUBE destination must be writable");
    static_assert(Std::is_same<T, typename Std::remove_const<Impl::CubeElement<S>>::type>::value,
        "LoadData operand element types must match");
    CheckLoadData2dDatatype<T>();
    if constexpr (Impl::CubeOperandTraits<D>::isPointer) {
        constexpr Hardware dstSpace = Impl::CubeOperandTraits<D>::space;
        static_assert(dstSpace == Hardware::L0A || dstSpace == Hardware::L0B ||
            (dstSpace == Hardware::L1 && Impl::CubeOperandTraits<S>::isGlobal),
            "Invalid LoadData destination address space");
#if __NPU_ARCH__ == 3510
        static_assert(!Impl::CubeOperandTraits<S>::isGlobal || dstSpace == Hardware::L1,
            "Ascend 950 does not support GM to L0A/L0B LoadData");
#endif
    }
    const uint64_t dstAddr = (uint64_t)Impl::GetUnderlyingPtr(dst);
    const Hardware space = Impl::GetCubeSpace(dst);
    ASCENDC_ASSERT(space == Hardware::L0A || space == Hardware::L0B ||
        (space == Hardware::L1 && Impl::CubeOperandTraits<S>::isGlobal),
        { KERNEL_LOG(KERNEL_ERROR, "Invalid LoadData destination position"); });
    if constexpr (Impl::CubeOperandTraits<S>::isGlobal) {
        if constexpr (Impl::CubeMayUseSpace<D, Hardware::L0A>()) { if (space == Hardware::L0A) {
            LoadData2DGM2L0ACal((__ca__ T*)dstAddr,
                (__gm__ T*)Impl::GetUnderlyingPtr(src), params);
        }}
        if constexpr (Impl::CubeMayUseSpace<D, Hardware::L0B>()) { if (space == Hardware::L0B) {
            LoadData2DGM2L0BCal((__cb__ T*)dstAddr,
                (__gm__ T*)Impl::GetUnderlyingPtr(src), params);
        }}
        if constexpr (Impl::CubeMayUseSpace<D, Hardware::L1>()) { if (space == Hardware::L1) {
#if __NPU_ARCH__ == 3510
            LoadData2DGM2L1Cal((__cbuf__ T*)dstAddr,
                (__gm__ T*)Impl::GetUnderlyingPtr(src), params, ExtractCacheMode(src));
#else
            LoadData2DGM2L1Cal((__cbuf__ T*)dstAddr,
                (__gm__ T*)Impl::GetUnderlyingPtr(src), params);
#endif
        }}
    } else {
        Impl::CheckCubeSpace<Hardware::L1>(src);
        if constexpr (Impl::CubeMayUseSpace<D, Hardware::L0A>()) { if (space == Hardware::L0A) {
            LoadData2DL12L0ACal((__ca__ T*)dstAddr,
                (__cbuf__ T*)Impl::GetUnderlyingPtr(src), params);
        }}
        if constexpr (Impl::CubeMayUseSpace<D, Hardware::L0B>()) { if (space == Hardware::L0B) {
            LoadData2DL12L0BCal((__cb__ T*)dstAddr,
                (__cbuf__ T*)Impl::GetUnderlyingPtr(src), params);
        }}
    }
}

#if __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102
template <TPosition Dst, TPosition Src, typename D, typename S, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value &&
        (Std::is_same<P, Load2DBitModeParam>::value || Std::is_same<P, Load3DBitModeParam>::value), bool>::type = true>
__aicore__ inline void LoadData(const D& dst, const S& src, const P& params)
{
    using T = Impl::CubeElement<D>;
    static_assert(Src == TPosition::A1 || Src == TPosition::B1, "LoadData source must be L1");
    static_assert(Std::is_same<T, Impl::CubeValue<S>>::value, "LoadData bit-mode element types must match");
    static_assert(Dst == TPosition::A2 || Dst == TPosition::B2, "LoadData destination must be L0A or L0B");
    static_assert(!Std::is_const<T>::value, "CUBE destination must be writable");
    Impl::CheckCubeSpace<Hardware::L1>(src);
    auto source = (__cbuf__ T*)Impl::GetUnderlyingPtr(src);
    if constexpr (Dst == TPosition::A2) {
        Impl::CheckCubeSpace<Hardware::L0A>(dst);
        auto destination = (__ca__ T*)Impl::GetUnderlyingPtr(dst);
        if constexpr (Std::is_same<P, Load2DBitModeParam>::value) {
            LoadData2DL12L0ACal(destination, source, params);
        } else { LoadData3DV2L12L0ACal(destination, source, params); }
    } else {
        Impl::CheckCubeSpace<Hardware::L0B>(dst);
        auto destination = (__cb__ T*)Impl::GetUnderlyingPtr(dst);
        if constexpr (Std::is_same<P, Load2DBitModeParam>::value) {
            LoadData2DL12L0BCal(destination, source, params);
        } else { LoadData3DV2L12L0BCal(destination, source, params); }
    }
}
template <typename D, typename S, typename Scale,
    typename Std::enable_if<Impl::CubeHasPointer<D, S, Scale>::value, bool>::type = true>
__aicore__ inline void LoadData(const D& dst, const S& src, const Scale& scale,
    const LoadData2DParamsV2& params, const LoadData2DMxParams& mxParams)
{
    Impl::CheckCubeSpace<Hardware::L1>(src);
    Impl::CheckCubeSpace<Hardware::L1>(scale);
    using T = Impl::CubeElement<D>;
    static_assert(Std::is_same<Impl::CubeValue<Scale>, fp8_e8m0_t>::value,
        "MX LoadData scale must be fp8_e8m0_t");
    if constexpr (Impl::CubeOperandTraits<D>::isPointer) {
        static_assert(Impl::CubeOperandTraits<D>::space == Hardware::L0A ||
            Impl::CubeOperandTraits<D>::space == Hardware::L0B, "Invalid MX LoadData destination");
    }
    ASCENDC_ASSERT(Impl::GetCubeSpace(dst) == Hardware::L0A || Impl::GetCubeSpace(dst) == Hardware::L0B,
        { KERNEL_LOG(KERNEL_ERROR, "Invalid MX LoadData destination position"); });
    static_assert(!Std::is_const<T>::value, "CUBE destination must be writable");
    auto source = (__cbuf__ Impl::CubeElement<S>*)Impl::GetUnderlyingPtr(src);
    auto scales = (__cbuf__ Impl::CubeElement<Scale>*)Impl::GetUnderlyingPtr(scale);
    if constexpr (Impl::CubeMayUseSpace<D, Hardware::L0A>()) {
        if (Impl::GetCubeSpace(dst) == Hardware::L0A) {
            Impl::CheckCubeSpace<Hardware::L0A>(dst);
            LoadData2DL12L0ACal((__ca__ T*)Impl::GetUnderlyingPtr(dst), source, scales, params, mxParams);
        }
    }
    if constexpr (Impl::CubeMayUseSpace<D, Hardware::L0B>()) {
        if (Impl::GetCubeSpace(dst) == Hardware::L0B) {
            Impl::CheckCubeSpace<Hardware::L0B>(dst);
            LoadData2DL12L0BCal((__cb__ T*)Impl::GetUnderlyingPtr(dst), source, scales, params, mxParams);
        }
    }
}
#endif

namespace Impl {
template <bool V1, typename D, typename S, typename P>
__aicore__ inline void CubeLoad3D(const D& dst, const S& src, const P& params)
{
    using T = CubeElement<D>;
    static_assert(!Std::is_const<T>::value, "CUBE destination must be writable");
    static_assert(Std::is_same<T, typename Std::remove_const<CubeElement<S>>::type>::value,
        "LoadData element types must match");
    CheckCubeSpace<Hardware::L1>(src);
    if constexpr (CubeOperandTraits<D>::isPointer) {
        static_assert(CubeOperandTraits<D>::space == Hardware::L0A ||
            CubeOperandTraits<D>::space == Hardware::L0B || CubeOperandTraits<D>::space == Hardware::UB,
            "Invalid LoadData 3D destination");
    }
    ASCENDC_ASSERT(GetCubeSpace(dst) == Hardware::L0A || GetCubeSpace(dst) == Hardware::L0B ||
        GetCubeSpace(dst) == Hardware::UB,
        { KERNEL_LOG(KERNEL_ERROR, "Invalid LoadData 3D destination position"); });
    auto source = (__cbuf__ T*)GetUnderlyingPtr(src);
    if constexpr (CubeMayUseSpace<D, Hardware::L0A>()) {
        if (GetCubeSpace(dst) == Hardware::L0A) {
            auto destination = (__ca__ T*)GetUnderlyingPtr(dst);
            if constexpr (V1) { LoadData3DV1L12L0ACal(destination, source, params); }
            else { LoadData3DV2L12L0ACal(destination, source, params); }
        }
    }
    if constexpr (CubeMayUseSpace<D, Hardware::L0B>()) {
        if (GetCubeSpace(dst) == Hardware::L0B) {
            auto destination = (__cb__ T*)GetUnderlyingPtr(dst);
            if constexpr (V1) { LoadData3DV1L12L0BCal(destination, source, params); }
            else { LoadData3DV2L12L0BCal(destination, source, params); }
        }
    }
    if constexpr (CubeMayUseSpace<D, Hardware::UB>()) {
        if (GetCubeSpace(dst) == Hardware::UB) {
            auto destination = (__ubuf__ T*)GetUnderlyingPtr(dst);
            if constexpr (V1) { LoadData3DV1L12UBCal(destination, source, params); }
            else { LoadData3DV2L12UBCal(destination, source, params); }
        }
    }
}
} // namespace Impl
template <typename T = void, const IsResetLoad3dConfig& config = IS_RESER_LOAD3D_DEFAULT_CONFIG,
    typename D, typename S, typename U,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value, bool>::type = true>
__aicore__ inline void LoadData(const D& dst, const S& src, const LoadData3DParamsV1<U>& params)
{
#if __NPU_ARCH__ == 3510
    static_assert(!Impl::CubeHasPointer<D, S>::value, "Ascend 950 does not support LoadData3DParamsV1");
#endif
    if constexpr (config.isSetFMatrix) { Load3DSetFMatrixCal(params.l1H, params.l1W, params.padList); }
    if constexpr (config.isSetPadding) { Load3DSetPaddingCal(params.padValue); }
    Impl::CubeLoad3D<true>(dst, src, params);
}
template <typename T = void, const IsResetLoad3dConfig& config = IS_RESER_LOAD3D_DEFAULT_CONFIG,
    typename D, typename S, typename U,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value, bool>::type = true>
__aicore__ inline void LoadData(const D& dst, const S& src, const LoadData3DParamsV2<U>& params)
{
    static_assert(Std::is_same<T, void>::value || Std::is_same<PrimT<T>, Impl::CubeValue<D>>::value,
        "LoadData explicit type mismatch");
    static_assert(Std::is_same<U, Impl::CubeValue<D>>::value, "LoadData parameter type mismatch");
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckLoadData3dv2Params<U>(params);
    CheckLoadData3dv2ChannelSize<U>(params.channelSize);
    CheckLoadData3dParams(params.l1H, params.l1W, params.strideW, params.strideH);
    CheckLoadData3dv2MatrixParams<U>(params.kExtension, params.mExtension, params.kStartPt, params.mStartPt);
#endif
#if __NPU_ARCH__ == 3510
    ASCENDC_ASSERT(params.kExtension * sizeof(U) % ONE_BLK_SIZE == 0 &&
        params.kStartPt * sizeof(U) % ONE_BLK_SIZE == 0 &&
        params.mExtension % 16 == 0 && params.mStartPt % 16 == 0,
        { KERNEL_LOG(KERNEL_ERROR, "Invalid LoadData 3D matrix alignment"); });
#endif
    if constexpr (config.isSetFMatrix) { Load3DSetFMatrixCal(params.l1H, params.l1W, params.padList); }
    if constexpr (config.isSetPadding) { Load3DSetPaddingCal(params.padValue); }
    Impl::CubeLoad3D<false>(dst, src, params);
}
template <typename D, typename S,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value, bool>::type = true>
__aicore__ inline void LoadData(const D& dst, const S& src, const LoadData3DParamsV2Pro& params)
{
    Impl::CubeLoad3D<false>(dst, src, params);
}

template <typename D, typename U,
    typename Std::enable_if<Impl::CubeOperandTraits<D>::isPointer, bool>::type = true>
__aicore__ inline void InitConstValue(const D& dst, const InitConstValueParams<U>& params)
{
    using T = Impl::CubeElement<D>;
    static_assert(Std::is_same<T, U>::value, "InitConstValue element type must match");
    constexpr Hardware space = Impl::CubeOperandTraits<D>::space;
    static_assert(space == Hardware::L1 || space == Hardware::L0A || space == Hardware::L0B,
        "InitConstValue destination must be L1, L0A or L0B");
#if __NPU_ARCH__ == 3510
    static_assert(space == Hardware::L1, "Ascend 950 only supports InitConstValue in L1");
#endif
    if constexpr (space == Hardware::L0A) {
        InitL0ANzMatrixCal(Impl::GetUnderlyingPtr(dst), params);
    } else if constexpr (space == Hardware::L0B) {
        InitL0BNzMatrixCal(Impl::GetUnderlyingPtr(dst), params);
    } else {
        InitL1BufferCal(Impl::GetUnderlyingPtr(dst), params);
    }
}

template <typename D, typename S, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value &&
        (Std::is_same<P, LoadData2dTransposeParams>::value ||
         Std::is_same<P, LoadData2dTransposeParamsV2>::value), bool>::type = true>
__aicore__ inline void LoadDataWithTranspose(const D& dst, const S& src, const P& params)
{
    using T = Impl::CubeElement<D>;
    static_assert(!Std::is_const<T>::value, "CUBE destination must be writable");
    static_assert(Std::is_same<T, Impl::CubeValue<S>>::value, "Transpose element types must match");
    ASCENDC_ASSERT(Impl::GetCubeSpace(dst) == Hardware::L0B ||
        (Impl::GetCubeSpace(dst) == Hardware::L0A && Std::is_same<P, LoadData2dTransposeParams>::value),
        { KERNEL_LOG(KERNEL_ERROR, "Invalid transpose destination position"); });
    Impl::CheckCubeSpace<Hardware::L1>(src);
    if constexpr (Impl::CubeOperandTraits<D>::isPointer) {
        static_assert(Impl::CubeOperandTraits<D>::space == Hardware::L0B ||
            (Impl::CubeOperandTraits<D>::space == Hardware::L0A &&
             Std::is_same<P, LoadData2dTransposeParams>::value), "Invalid transpose destination");
    }
    const uint64_t dstAddr = (uint64_t)Impl::GetUnderlyingPtr(dst);
    if constexpr (Impl::CubeMayUseSpace<D, Hardware::L0A>() &&
        Std::is_same<P, LoadData2dTransposeParams>::value) {
        if (Impl::GetCubeSpace(dst) == Hardware::L0A) {
            LoadData2DL12L0ATransposeCal((__ca__ T*)dstAddr,
                (__cbuf__ T*)Impl::GetUnderlyingPtr(src), params);
        }
    }
    if constexpr (Impl::CubeMayUseSpace<D, Hardware::L0B>()) {
        if (Impl::GetCubeSpace(dst) == Hardware::L0B) {
            LoadData2DL12L0BTransposeCal((__cb__ T*)dstAddr,
                (__cbuf__ T*)Impl::GetUnderlyingPtr(src), params);
        }
    }
}

template <typename D, typename Std::enable_if<Impl::CubeOperandTraits<D>::isPointer, bool>::type = true>
__aicore__ inline __inout_pipe__(MTE2) void LoadImageToLocal(const D& dst, const LoadImageToLocalParams& params)
{
    Impl::CheckCubeSpace<Hardware::L1>(dst);
    static_assert(!Std::is_const<Impl::CubeElement<D>>::value, "CUBE destination must be writable");
    ASCENDC_CHECK_VALUE_RANGE(params.horizSize, 2, UINT12_MAX, "horizSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(params.vertSize, 2, UINT12_MAX, "vertSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(params.horizStartPos, 0, UINT12_MAX, "horizStartPos", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(params.vertStartPos, 0, UINT12_MAX, "vertStartPos", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(params.srcHorizSize, 2, UINT12_MAX, "srcHorizSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(params.topPadSize, 0, 32, "topPadSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(params.botPadSize, 0, 32, "botPadSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(params.leftPadSize, 0, 32, "leftPadSize", "LoadImageToLocal");
    ASCENDC_CHECK_VALUE_RANGE(params.rightPadSize, 0, 32, "rightPadSize", "LoadImageToLocal");
    LoadImageToLocalCal(Impl::GetUnderlyingPtr(dst), params);
}
// Preserve the element-template spelling used by existing Tensor call sites.
template <typename T, typename D, typename S, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value &&
        (Std::is_same<P, LoadData2DParams>::value || Std::is_same<P, LoadData2DParamsV2>::value ||
         Std::is_same<P, LoadData3DParamsV2Pro>::value), bool>::type = true>
__aicore__ inline void LoadData(const D& dst, const S& src, const P& params)
{
    static_assert(Std::is_same<PrimT<T>, Impl::CubeValue<D>>::value, "LoadData explicit element type mismatch");
    LoadData(dst, src, params);
}
template <typename T, typename U, typename S, typename D, typename A, typename B, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B>::value, bool>::type = true>
__aicore__ inline void Mmad(const D& dst, const A& a, const B& b, const P& params)
{
    static_assert(Std::is_same<PrimT<T>, Impl::CubeValue<D>>::value &&
        Std::is_same<PrimT<U>, Impl::CubeValue<A>>::value && Std::is_same<PrimT<S>, Impl::CubeValue<B>>::value,
        "Mmad explicit element type mismatch");
    Mmad(dst, a, b, params);
}
#if __NPU_ARCH__ == 3510
template <typename T, typename U, typename S, typename D, typename A, typename B, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B>::value, bool>::type = true>
__aicore__ inline void MmadMx(const D& dst, const A& a, const B& b, const P& params)
{
    static_assert(Std::is_same<PrimT<T>, Impl::CubeValue<D>>::value &&
        Std::is_same<PrimT<U>, Impl::CubeValue<A>>::value && Std::is_same<PrimT<S>, Impl::CubeValue<B>>::value,
        "MmadMx explicit element type mismatch");
    MmadMx(dst, a, b, params);
}
#endif
template <typename T, typename D, typename S, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value, bool>::type = true>
__aicore__ inline void LoadDataWithTranspose(const D& dst, const S& src, const P& params)
{
    static_assert(Std::is_same<PrimT<T>, Impl::CubeValue<D>>::value, "Transpose explicit element type mismatch");
    LoadDataWithTranspose(dst, src, params);
}
template <typename T, typename U = PrimT<T>, typename D,
    typename Std::enable_if<Impl::CubeOperandTraits<D>::isPointer, bool>::type = true>
__aicore__ inline void InitConstValue(const D& dst, const InitConstValueParams<U>& params)
{
    static_assert(Std::is_same<PrimT<T>, Impl::CubeValue<D>>::value, "InitConstValue explicit type mismatch");
    InitConstValue(dst, params);
}
template <typename T, typename D,
    typename Std::enable_if<Impl::CubeOperandTraits<D>::isPointer, bool>::type = true>
__aicore__ inline __inout_pipe__(MTE2) void LoadImageToLocal(const D& dst, const LoadImageToLocalParams& params)
{
    static_assert(Std::is_same<PrimT<T>, Impl::CubeValue<D>>::value, "LoadImageToLocal explicit type mismatch");
    LoadImageToLocal(dst, params);
}
template <typename T, typename U, typename S, typename V, typename D, typename A, typename B, typename Bias, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B, Bias>::value, bool>::type = true>
__aicore__ inline void Mmad(const D& dst, const A& a, const B& b, const Bias& bias, const P& params)
{
    static_assert(Std::is_same<PrimT<T>, Impl::CubeValue<D>>::value &&
        Std::is_same<PrimT<U>, Impl::CubeValue<A>>::value && Std::is_same<PrimT<S>, Impl::CubeValue<B>>::value &&
        Std::is_same<PrimT<V>, Impl::CubeValue<Bias>>::value, "Explicit CUBE element type mismatch");
    Mmad(dst, a, b, bias, params);
}
#if __NPU_ARCH__ == 3510
template <typename T, typename U, typename S, typename V, typename D, typename A, typename B, typename Bias, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B, Bias>::value, bool>::type = true>
__aicore__ inline void MmadMx(const D& dst, const A& a, const B& b, const Bias& bias, const P& params)
{
    static_assert(Std::is_same<PrimT<T>, Impl::CubeValue<D>>::value &&
        Std::is_same<PrimT<U>, Impl::CubeValue<A>>::value && Std::is_same<PrimT<S>, Impl::CubeValue<B>>::value &&
        Std::is_same<PrimT<V>, Impl::CubeValue<Bias>>::value, "Explicit CUBE element type mismatch");
    MmadMx(dst, a, b, bias, params);
}
#endif
#if __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102
 template <TPosition Dst, TPosition Src, typename T, typename D, typename S, typename P,
    typename Std::enable_if<Impl::CubeHasPointer<D, S>::value, bool>::type = true>
__aicore__ inline void LoadData(const D& dst, const S& src, const P& params)
{
    static_assert(Std::is_same<PrimT<T>, Impl::CubeValue<D>>::value, "LoadData explicit type mismatch");
    LoadData<Dst, Src>(dst, src, params);
}
template <typename T, typename U, typename V = fp8_e8m0_t, typename D, typename S, typename Scale,
    typename Std::enable_if<Impl::CubeHasPointer<D, S, Scale>::value, bool>::type = true>
__aicore__ inline void LoadData(const D& dst, const S& src, const Scale& scale,
    const LoadData2DParamsV2& params, const LoadData2DMxParams& mx)
{
    static_assert(Std::is_same<PrimT<T>, Impl::CubeValue<S>>::value &&
        Std::is_same<PrimT<U>, Impl::CubeValue<D>>::value && Std::is_same<PrimT<V>, Impl::CubeValue<Scale>>::value,
        "MX LoadData explicit type mismatch");
    LoadData(dst, src, scale, params, mx);
}
#endif
#if __NPU_ARCH__ == 2201
template <typename D, typename S, typename I,
    typename Std::enable_if<Impl::CubeHasPointer<D, S, I>::value, bool>::type = true>
__aicore__ inline void LoadDataWithSparse(const D& dst, const S& src, const I& idx,
    const LoadData2dParams& loadDataParam)
{
    static_assert(!Std::is_const<Impl::CubeElement<D>>::value, "CUBE destination must be writable");
    static_assert(Std::is_same<Impl::CubeValue<D>, int8_t>::value &&
        Std::is_same<Impl::CubeValue<S>, int8_t>::value &&
        Std::is_same<Impl::CubeValue<I>, uint8_t>::value,
        "LoadDataWithSparse types must be int8_t/int8_t/uint8_t");
    Impl::CheckCubeSpace<Hardware::L0B>(dst);
    Impl::CheckCubeSpace<Hardware::L1>(src);
    Impl::CheckCubeSpace<Hardware::L1>(idx);
    if ASCEND_IS_AIC {
#ifdef ASCENDC_CPU_DEBUG
        SetIndexMatrix((void*)Impl::GetUnderlyingPtr(idx));
        auto srcAddr = (__cbuf__ int8_t*)Impl::GetUnderlyingPtr(src);
#else
        const uint64_t src0tmp = reinterpret_cast<uint64_t>(Impl::GetUnderlyingPtr(src));
        const uint64_t src1tmp = reinterpret_cast<uint64_t>(Impl::GetUnderlyingPtr(idx));
        const uint64_t srctmp = (src0tmp & 0xffffffff) | ((src1tmp & 0xffffffff) << 32);
        auto srcAddr = reinterpret_cast<__cbuf__ int8_t*>(srctmp);
#endif
        load_cbuf_to_cb_sp((__cb__ int8_t*)Impl::GetUnderlyingPtr(dst), srcAddr,
            loadDataParam.startIndex, loadDataParam.repeatTimes);
    }
}
template <typename D, typename A, typename B,
    typename Std::enable_if<Impl::CubeHasPointer<D, A, B>::value, bool>::type = true>
__aicore__ inline void MmadWithSparse(const D& dst, const A& a, const B& b, const MmadParams& mmadParams)
{
    static_assert(!Std::is_const<Impl::CubeElement<D>>::value, "CUBE destination must be writable");
    static_assert(Std::is_same<Impl::CubeValue<D>, int32_t>::value &&
        Std::is_same<Impl::CubeValue<A>, int8_t>::value &&
        Std::is_same<Impl::CubeValue<B>, int8_t>::value,
        "MmadWithSparse types must be int32_t/int8_t/int8_t");
    Impl::CheckCubeSpace<Hardware::L0C>(dst);
    Impl::CheckCubeSpace<Hardware::L0A>(a);
    Impl::CheckCubeSpace<Hardware::L0B>(b);
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckMmadParamsCommon(mmadParams, "MmadWithSparse with MmadParams");
#endif
    MmadSpCal((__cc__ int32_t*)Impl::GetUnderlyingPtr(dst),
        (__ca__ int8_t*)Impl::GetUnderlyingPtr(a),
        (__cb__ int8_t*)Impl::GetUnderlyingPtr(b), mmadParams);
}
#endif
} // namespace AscendC
#endif
