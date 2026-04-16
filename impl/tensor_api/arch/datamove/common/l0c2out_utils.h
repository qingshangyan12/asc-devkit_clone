/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/arch/datamove/common/l0c2out_utils.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file utils.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_DATAMOVE_COMMON_L0C2OUT_UTILS_H
#define IMPL_TENSOR_API_ARCH_DATAMOVE_COMMON_L0C2OUT_UTILS_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/arch/datamove/common/instruction.h"

namespace AscendC {
namespace Te {

constexpr uint32_t MAIN_LOOP_N_SIZE_3510 = 512;
constexpr uint32_t CBURST_NUM_3510 = MAIN_LOOP_N_SIZE_3510 / BLOCK_CUBE;

constexpr FixpipeParams DEFAULT_FIXPIPE_PARAMS = FixpipeParams{};

template <RoundMode mode, typename Dst, typename Src>
struct VectorQuantMapper {
    static constexpr auto value = []() {
        static_assert(mode != mode, "VectorQuantMapper: not supported combination of RoundMode/Dst/Src");
        return QuantMode_t::NoQuant;
    }();
};

template <>
struct VectorQuantMapper<RoundMode::DEFAULT, half, int32_t> {
    static constexpr auto value = QuantMode_t::VDEQF16;
};
template <>
struct VectorQuantMapper<RoundMode::DEFAULT, uint8_t, float> {
    static constexpr auto value = QuantMode_t::VQF322B8_PRE;
};
template <>
struct VectorQuantMapper<RoundMode::DEFAULT, int8_t, float> {
    static constexpr auto value = QuantMode_t::VQF322B8_PRE;
};
template <>
struct VectorQuantMapper<RoundMode::DEFAULT, uint8_t, int32_t> {
    static constexpr auto value = QuantMode_t::VREQ8;
};
template <>
struct VectorQuantMapper<RoundMode::DEFAULT, int8_t, int32_t> {
    static constexpr auto value = QuantMode_t::VREQ8;
};
template <>
struct VectorQuantMapper<RoundMode::DEFAULT, fp8_e4m3fn_t, float> {
    static constexpr auto value = QuantMode_t::VQF322FP8_PRE;
};
template <>
struct VectorQuantMapper<RoundMode::DEFAULT, hifloat8_t, float> {
    static constexpr auto value = QuantMode_t::VQF322HIF8_PRE;
};
template <>
struct VectorQuantMapper<RoundMode::HYBRID, hifloat8_t, float> {
    static constexpr auto value = QuantMode_t::VQF322HIF8_PRE_HYBRID;
};
template <>
struct VectorQuantMapper<RoundMode::DEFAULT, bfloat16_t, int32_t> {
    static constexpr auto value = QuantMode_t::VQS322BF16_PRE;
};
template <>
struct VectorQuantMapper<RoundMode::DEFAULT, half, float> {
    static constexpr auto value = QuantMode_t::VQF322F16_PRE;
};
template <>
struct VectorQuantMapper<RoundMode::DEFAULT, bfloat16_t, float> {
    static constexpr auto value = QuantMode_t::VQF322BF16_PRE;
};
template <>
struct VectorQuantMapper<RoundMode::DEFAULT, float, float> {
    static constexpr auto value = QuantMode_t::VQF322F32_PRE;
};

template <RoundMode mode, typename Dst, typename Src>
struct ScalarQuantMapper {
    static constexpr auto value = []() {
        static_assert(mode != mode, "ScalarQuantMapper: not supported combination of RoundMode/Dst/Src");
        return QuantMode_t::NoQuant;
    }();
};

template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, half, int32_t> {
    static constexpr auto value = QuantMode_t::DEQF16;
};
template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, uint8_t, float> {
    static constexpr auto value = QuantMode_t::QF322B8_PRE;
};
template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, int8_t, float> {
    static constexpr auto value = QuantMode_t::QF322B8_PRE;
};
template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, uint8_t, int32_t> {
    static constexpr auto value = QuantMode_t::REQ8;
};
template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, int8_t, int32_t> {
    static constexpr auto value = QuantMode_t::REQ8;
};
template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, fp8_e4m3fn_t, float> {
    static constexpr auto value = QuantMode_t::QF322FP8_PRE;
};
template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, hifloat8_t, float> {
    static constexpr auto value = QuantMode_t::QF322HIF8_PRE;
};
template <>
struct ScalarQuantMapper<RoundMode::HYBRID, hifloat8_t, float> {
    static constexpr auto value = QuantMode_t::QF322HIF8_PRE_HYBRID;
};
template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, bfloat16_t, int32_t> {
    static constexpr auto value = QuantMode_t::QS322BF16_PRE;
};
template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, half, float> {
    static constexpr auto value = QuantMode_t::QF322F16_PRE;
};
template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, bfloat16_t, float> {
    static constexpr auto value = QuantMode_t::QF322BF16_PRE;
};
template <>
struct ScalarQuantMapper<RoundMode::DEFAULT, float, float> {
    static constexpr auto value = QuantMode_t::QF322F32_PRE;
};

template <RoundMode mode, typename Dst, typename Src>
struct CastQuantMapper {
private:
    static constexpr auto SameTypeImpl()
    { return QuantMode_t::NoQuant; }

    static constexpr auto DiffTypeImpl()
    {
        static_assert(mode != mode, "CastQuantMapper: not supported combination of RoundMode/Dst/Src");
        return QuantMode_t::NoQuant;
    }

public:
    static constexpr auto value = Std::is_same_v<Src, Dst> ? SameTypeImpl() : DiffTypeImpl();
};

template <>
struct CastQuantMapper<RoundMode::DEFAULT, half, float> {
    static constexpr auto value = QuantMode_t::F322F16;
};
template <>
struct CastQuantMapper<RoundMode::DEFAULT, bfloat16_t, float> {
    static constexpr auto value = QuantMode_t::F322BF16;
};

template <RoundMode roundMode, typename dstType, typename srcType>
__aicore__ inline constexpr QuantMode_t GetVectorQuantMode()
{ return VectorQuantMapper<roundMode, dstType, srcType>::value; }

template <RoundMode roundMode, typename dstType, typename srcType>
__aicore__ inline constexpr QuantMode_t GetScalarQuantMode()
{ return ScalarQuantMapper<roundMode, dstType, srcType>::value; }

template <RoundMode roundMode, typename dstType, typename srcType>
__aicore__ inline constexpr QuantMode_t GetCastQuantMode()
{ return CastQuantMapper<roundMode, dstType, srcType>::value; }

template <RoundMode roundMode, typename T, typename U, typename S = void>
__aicore__ inline constexpr QuantMode_t GetQuantMode()
{
    using srcType = typename U::elementType;
    using dstType = typename T::elementType;
    constexpr bool isTensor = IsTileTensorV<S>;
    constexpr bool isScalar = Std::is_same_v<S, uint64_t>;

    if constexpr (isTensor) {
        return GetVectorQuantMode<roundMode, dstType, srcType>();
    } else if constexpr (isScalar) {
        return GetScalarQuantMode<roundMode, dstType, srcType>();
    } else {
        return GetCastQuantMode<roundMode, dstType, srcType>();
    }
}

template <typename LayoutType>
__aicore__ inline static constexpr uint32_t GetToalColumnShape(const LayoutType& layout)
{
    if constexpr (IsNDLayout<LayoutType>() || IsDNLayout<LayoutType>()) {
        return Get<1>(layout.Shape());
    } else {
        return Get<1, 0>(layout.Shape()) * Get<1, 1>(layout.Shape());
    }
}

template <typename LayoutType>
__aicore__ inline static constexpr uint32_t GetTotalRowShape(const LayoutType& layout)
{
    if constexpr (IsNDLayout<LayoutType>() || IsDNLayout<LayoutType>()) {
        return Get<0>(layout.Shape());
    } else {
        return Get<0, 0>(layout.Shape()) * Get<0, 1>(layout.Shape());
    }
}

template <size_t index = 1, typename LayoutType>
__aicore__ inline static constexpr uint32_t GetColumnStride(const LayoutType& layout)
{
    if constexpr (IsNDLayout<LayoutType>() || IsDNLayout<LayoutType>()) {
        return Get<1>(layout.Stride());
    } else {
        static_assert(index < 2, "For non-ND/DN layout, only index 0 and 1 are valid for GetColumnStride");
        return Get<1, index>(layout.Stride());
    }
}

template <size_t index = 0, typename LayoutType>
__aicore__ inline static constexpr uint32_t GetRowStride(const LayoutType& layout)
{
    if constexpr (IsNDLayout<LayoutType>() || IsDNLayout<LayoutType>()) {
        return Get<0>(layout.Stride());
    } else {
        static_assert(index < 2, "For non-ND/DN layout, only index 0 and 1 are valid for GetRowStride");
        return Get<0, index>(layout.Stride());
    }
}

template <size_t index = 1, typename LayoutType>
__aicore__ inline static constexpr uint32_t GetColumnShape(const LayoutType& layout)
{
    if constexpr (IsNDLayout<LayoutType>() || IsDNLayout<LayoutType>()) {
        return Get<1>(layout.Shape());
    } else {
        static_assert(index < 2, "For non-ND/DN layout, only index 0 and 1 are valid for GetColumnShape");
        return Get<1, index>(layout.Shape());
    }
}

template <size_t index = 0, typename LayoutType>
__aicore__ inline static constexpr uint32_t GetRowShape(const LayoutType& layout)
{
    if constexpr (IsNDLayout<LayoutType>() || IsDNLayout<LayoutType>()) {
        return Get<0>(layout.Shape());
    } else {
        static_assert(index < 2, "For non-ND/DN layout, only index 0 and 1 are valid for GetRowShape");
        return Get<0, index>(layout.Shape());
    }
}

template <typename T, typename U>
__aicore__ inline static void SetRegisterImpl(const T& /*dst*/, const U& /*src*/)
{
    if constexpr (IsNDLayout<T>()) {
        constexpr uint32_t ndNum = 1;
        constexpr uint32_t srcNdStride = 0;
        constexpr uint32_t dstNdStride = 0;
        SetRegister::SetRegister(ndNum, dstNdStride, srcNdStride);
    } else if constexpr (IsDNLayout<T>()) {
        constexpr uint32_t dnNum = 1;
        constexpr uint32_t dstDnMatrixStride = 0;
        constexpr uint32_t srcNzMatrixStride = 0;
        constexpr uint32_t srcNzC0Stride = 1;
        SetRegister::SetRegister(dnNum, dstDnMatrixStride, srcNzMatrixStride, srcNzC0Stride);
    }
}

template <typename T, typename U>
__aicore__ inline static void SetRegisterImpl(const T& /*dst*/, const U& /*src*/, uint64_t quant)
{
    if constexpr (IsNDLayout<T>()) {
        constexpr uint32_t ndNum = 1;
        constexpr uint32_t srcNdStride = 0;
        constexpr uint32_t dstNdStride = 0;
        SetRegister::SetRegister(quant, ndNum, dstNdStride, srcNdStride);
    } else if constexpr (IsDNLayout<T>()) {
        constexpr uint32_t dnNum = 1;
        constexpr uint32_t dstDnMatrixStride = 0;
        constexpr uint32_t srcNzMatrixStride = 0;
        constexpr uint32_t srcNzC0Stride = 1;
        SetRegister::SetRegister(quant, dnNum, dstDnMatrixStride, srcNzMatrixStride, srcNzC0Stride);
    }
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_DATAMOVE_COMMON_L0C2OUT_UTILS_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif