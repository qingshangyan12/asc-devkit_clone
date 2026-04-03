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
#warning \
    "impl/tensor_api/arch/utils/check_data_type_3510.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file check_data_type_3510.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_UTILS_CHECK_DATA_TYPE_3510_H
#define IMPL_TENSOR_API_ARCH_UTILS_CHECK_DATA_TYPE_3510_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"
#include "impl/experimental/tensor_api/arch/utils/is_format.h"

namespace AscendC {
namespace Te {

class CheckDataTypeFor3510 {
public:
    template <typename T, typename U, typename S>
    __aicore__ inline static constexpr void CheckMxMmadDataType()
    {
        using dstDataType = typename T::elementType;
        using fmDataType = typename U::elementType;
        using filterDataType = typename S::elementType;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            is_one_of_attr_v<
                Std::tuple<dstDataType, fmDataType, filterDataType>,
                Std::tuple<float, fp4x2_e2m1_t, fp4x2_e2m1_t>,
                Std::tuple<float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
                Std::tuple<float, fp4x2_e1m2_t, fp4x2_e2m1_t>,
                Std::tuple<float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
                Std::tuple<float, fp8_e4m3fn_t, fp8_e4m3fn_t>,
                Std::tuple<float, fp8_e4m3fn_t, fp8_e5m2_t>,
                Std::tuple<float, fp8_e5m2_t, fp8_e4m3fn_t>,
                Std::tuple<float, fp8_e5m2_t, fp8_e5m2_t>>,
            "The data type is not supported for L0C position.");
#endif
    }

    template <typename T, typename U, typename S, typename V>
    __aicore__ inline static constexpr void CheckMxMmadBiasDataType()
    {
        using dstDataType = typename T::elementType;
        using biasDataType = typename V::elementType;
        using fmDataType = typename U::elementType;
        using filterDataType = typename S::elementType;
        constexpr auto biasPos = GetHardPos<V>();
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        if constexpr (biasPos == Hardware::BIAS) {
            static_assert(
                is_one_of_attr_v<
                    Std::tuple<biasDataType, dstDataType, fmDataType, filterDataType>,
                    Std::tuple<float, float, fp4x2_e2m1_t, fp4x2_e2m1_t>,
                    Std::tuple<float, float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
                    Std::tuple<float, float, fp4x2_e1m2_t, fp4x2_e2m1_t>,
                    Std::tuple<float, float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
                    Std::tuple<float, float, fp8_e4m3fn_t, fp8_e4m3fn_t>,
                    Std::tuple<float, float, fp8_e4m3fn_t, fp8_e5m2_t>,
                    Std::tuple<float, float, fp8_e5m2_t, fp8_e4m3fn_t>,
                    Std::tuple<float, float, fp8_e5m2_t, fp8_e5m2_t>>,
                "The data type is not supported for BIAS position.");
        } else if constexpr (biasPos == Hardware::L0C) {
            static_assert(
                is_one_of_attr_v<
                    Std::tuple<biasDataType, dstDataType, fmDataType, filterDataType>,
                    Std::tuple<float, float, fp4x2_e2m1_t, fp4x2_e2m1_t>,
                    Std::tuple<float, float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
                    Std::tuple<float, float, fp4x2_e1m2_t, fp4x2_e2m1_t>,
                    Std::tuple<float, float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
                    Std::tuple<float, float, fp8_e4m3fn_t, fp8_e4m3fn_t>,
                    Std::tuple<float, float, fp8_e4m3fn_t, fp8_e5m2_t>,
                    Std::tuple<float, float, fp8_e5m2_t, fp8_e4m3fn_t>,
                    Std::tuple<float, float, fp8_e5m2_t, fp8_e5m2_t>>,
                "The data type is not supported for L0C position.");
        }
#endif
    }

    template <typename T, typename U, typename S>
    __aicore__ inline static constexpr void CheckMmadDataType()
    {
        using dstDataType = typename T::elementType;
        using fmDataType = typename U::elementType;
        using filterDataType = typename S::elementType;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            is_one_of_attr_v<
                Std::tuple<dstDataType, fmDataType, filterDataType>,
                Std::tuple<int32_t, int8_t, int8_t>,
                Std::tuple<float, half, half>,
                Std::tuple<float, float, float>,
                Std::tuple<float, bfloat16_t, bfloat16_t>,
                Std::tuple<float, fp8_e4m3fn_t, fp8_e4m3fn_t>,
                Std::tuple<float, fp8_e4m3fn_t, fp8_e5m2_t>,
                Std::tuple<float, fp8_e5m2_t, fp8_e4m3fn_t>,
                Std::tuple<float, fp8_e5m2_t, fp8_e5m2_t>,
                Std::tuple<float, hifloat8_t, hifloat8_t>>,
            "The data type is not supported for L0C position.");
#endif
    }

    template <typename T, typename U, typename S, typename V>
    __aicore__ inline static constexpr void CheckMmadBiasDataType()
    {
        using dstDataType = typename T::elementType;
        using fmDataType = typename U::elementType;
        using filterDataType = typename S::elementType;
        using biasDataType = typename V::elementType;
        constexpr auto biasPos = GetHardPos<V>();

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        if constexpr (biasPos == Hardware::BIAS) {
            static_assert(
                is_one_of_attr_v<
                    Std::tuple<biasDataType, dstDataType, fmDataType, filterDataType>,
                    Std::tuple<int32_t, int32_t, int8_t, int8_t>,
                    Std::tuple<float, float, half, half>,
                    Std::tuple<float, float, float, float>,
                    Std::tuple<float, float, bfloat16_t, bfloat16_t>,
                    Std::tuple<float, float, fp8_e4m3fn_t, fp8_e4m3fn_t>,
                    Std::tuple<float, float, fp8_e4m3fn_t, fp8_e5m2_t>,
                    Std::tuple<float, float, fp8_e5m2_t, fp8_e4m3fn_t>,
                    Std::tuple<float, float, fp8_e5m2_t, fp8_e5m2_t>,
                    Std::tuple<float, float, hifloat8_t, hifloat8_t>>,
                "The data type is not supported for BIAS position.");
        } else if constexpr (biasPos == Hardware::L0C) {
            static_assert(
                is_one_of_attr_v<
                    Std::tuple<biasDataType, dstDataType, fmDataType, filterDataType>,
                    Std::tuple<int32_t, int32_t, int8_t, int8_t>,
                    Std::tuple<float, float, half, half>,
                    Std::tuple<float, float, float, float>,
                    Std::tuple<float, float, bfloat16_t, bfloat16_t>,
                    Std::tuple<float, float, fp8_e4m3fn_t, fp8_e4m3fn_t>,
                    Std::tuple<float, float, fp8_e4m3fn_t, fp8_e5m2_t>,
                    Std::tuple<float, float, fp8_e5m2_t, fp8_e4m3fn_t>,
                    Std::tuple<float, float, fp8_e5m2_t, fp8_e5m2_t>,
                    Std::tuple<float, float, hifloat8_t, hifloat8_t>>,
                "The data type is not supported for L0C position.");
        }
#endif
    }

    template <typename T, typename U>
    __aicore__ inline static constexpr void CheckGm2L1DataType()
    {
        using dstDataType = typename T::elementType;
        using srcDataType = typename U::elementType;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            is_one_of_attr_v<
                Std::tuple<dstDataType, srcDataType>, Std::tuple<half, half>,
                Std::tuple<bfloat16_t, bfloat16_t>, Std::tuple<float, float>,
                Std::tuple<int8_t, int8_t>, Std::tuple<uint8_t, uint8_t>,
                Std::tuple<int16_t, int16_t>, Std::tuple<uint16_t, uint16_t>,
                Std::tuple<int32_t, int32_t>, Std::tuple<uint32_t, uint32_t>,
                Std::tuple<fp8_e5m2_t, fp8_e5m2_t>,
                Std::tuple<fp8_e4m3fn_t, fp8_e4m3fn_t>,
                Std::tuple<hifloat8_t, hifloat8_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename T, typename U>
    __aicore__ inline static constexpr void CheckGm2L1Fp4DataType()
    {
        using srcDataType = typename U::elementType;
        using dstDataType = typename T::elementType;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            is_one_of_attr_v<
                Std::tuple<dstDataType, srcDataType>, Std::tuple<half, half>,
                Std::tuple<bfloat16_t, bfloat16_t>, Std::tuple<float, float>,
                Std::tuple<int8_t, int8_t>, Std::tuple<uint8_t, uint8_t>,
                Std::tuple<int16_t, int16_t>, Std::tuple<uint16_t, uint16_t>,
                Std::tuple<int32_t, int32_t>, Std::tuple<uint32_t, uint32_t>,
                Std::tuple<fp4x2_e1m2_t, fp4x2_e1m2_t>,
                Std::tuple<fp4x2_e2m1_t, fp4x2_e2m1_t>,
                Std::tuple<fp8_e5m2_t, fp8_e5m2_t>,
                Std::tuple<fp8_e4m3fn_t, fp8_e4m3fn_t>,
                Std::tuple<hifloat8_t, hifloat8_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename T, typename U>
    __aicore__ inline static constexpr void CheckGm2L1ScaleDataType()
    {
        using srcDataType = typename U::elementType;
        using dstDataType = typename T::elementType;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            is_one_of_attr_v<Std::tuple<dstDataType, srcDataType>, Std::tuple<fp8_e8m0_t, fp8_e8m0_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename T, typename U>
    __aicore__ inline static constexpr void CheckGm2L1AlignV2NDDataType()
    {
        using srcDataType = typename U::elementType;
        using dstDataType = typename T::elementType;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            is_one_of_attr_v<
                Std::tuple<dstDataType, srcDataType>, Std::tuple<half, half>,
                Std::tuple<bfloat16_t, bfloat16_t>, Std::tuple<float, float>,
                Std::tuple<int8_t, int8_t>, Std::tuple<uint8_t, uint8_t>,
                Std::tuple<int16_t, int16_t>, Std::tuple<uint16_t, uint16_t>,
                Std::tuple<int32_t, int32_t>, Std::tuple<uint32_t, uint32_t>,
                Std::tuple<int64_t, int64_t>, Std::tuple<uint64_t, uint64_t>,
                Std::tuple<fp8_e5m2_t, fp8_e5m2_t>,
                Std::tuple<fp8_e4m3fn_t, fp8_e4m3fn_t>,
                Std::tuple<hifloat8_t, hifloat8_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename U>
    __aicore__ inline static constexpr void CheckGm2L1ND2NDSrcOneDim()
    {
        using ShapeRow1 = typename GetFourDimType<U, AttrInfo::SHAPE, AttrInfo::ROW, 1>::type;
        using ShapeCol1 = typename GetFourDimType<U, AttrInfo::SHAPE, AttrInfo::COLUMN, 1>::type;
        static_assert(
            Std::is_constant<1, ShapeRow1>::value || Std::is_constant<1, ShapeCol1>::value,
            "The src only support 1D tensor");
    }

    template <typename T, typename U>
    __aicore__ inline static constexpr void CheckL12BtDataType()
    {
        using srcDataType = typename U::elementType;
        using dstDataType = typename T::elementType;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            is_one_of_attr_v<
                Std::tuple<dstDataType, srcDataType>, Std::tuple<float, bfloat16_t>,
                Std::tuple<float, half>, Std::tuple<float, float>,
                Std::tuple<int32_t, int32_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename T, typename U>
    __aicore__ inline static constexpr void CheckL12FbDataType()
    {
        using srcDataType = typename U::elementType;
        using dstDataType = typename T::elementType;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            is_one_of_attr_v<Std::tuple<dstDataType, srcDataType>, Std::tuple<uint64_t, uint64_t>>,
            "The data type is not supported.");
#endif
    }

    template <QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static constexpr void CheckL0C2GmDataType()
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            (quantPre == QuantMode_t::NoQuant &&
             is_one_of_attr_v<
                 Std::tuple<dstType, srcType>, Std::tuple<float, float>,
                 Std::tuple<int32_t, int32_t>>) ||
                (quantPre == QuantMode_t::F322F16 &&
                 is_one_of_attr_v<Std::tuple<dstType, srcType>, Std::tuple<half, float>>) ||
                (quantPre == QuantMode_t::F322BF16 &&
                 is_one_of_attr_v<Std::tuple<dstType, srcType>, Std::tuple<bfloat16_t, float>>),
            "The data type is not supported.");
#endif
    }

    template <QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static constexpr void CheckL0C2UbDataType()
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            (quantPre == QuantMode_t::NoQuant &&
             is_one_of_attr_v<
                 Std::tuple<dstType, srcType>, Std::tuple<float, float>,
                 Std::tuple<int32_t, int32_t>>) ||
                (quantPre == QuantMode_t::F322F16 &&
                 is_one_of_attr_v<Std::tuple<dstType, srcType>, Std::tuple<half, float>>) ||
                (quantPre == QuantMode_t::F322BF16 &&
                 is_one_of_attr_v<Std::tuple<dstType, srcType>, Std::tuple<bfloat16_t, float>>),
            "The data type is not supported.");
#endif
    }

    template <typename T, typename U>
    __aicore__ inline static constexpr void CheckL12L0ADataType()
    {
        using srcDataType = typename U::elementType;
        using dstDataType = typename T::elementType;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            is_one_of_attr_v<
                Std::tuple<dstDataType, srcDataType>, Std::tuple<half, half>,
                Std::tuple<int16_t, int16_t>, Std::tuple<uint16_t, uint16_t>,
                Std::tuple<bfloat16_t, bfloat16_t>, Std::tuple<uint32_t, uint32_t>,
                Std::tuple<int32_t, int32_t>, Std::tuple<float, float>,
                Std::tuple<uint8_t, uint8_t>, Std::tuple<int8_t, int8_t>,
                Std::tuple<fp8_e4m3fn_t, fp8_e4m3fn_t>,
                Std::tuple<fp8_e5m2_t, fp8_e5m2_t>,
                Std::tuple<fp4x2_e2m1_t, fp4x2_e2m1_t>,
                Std::tuple<fp4x2_e1m2_t, fp4x2_e1m2_t>,
                Std::tuple<hifloat8_t, hifloat8_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename T, typename U>
    __aicore__ inline static constexpr void CheckL12L0BDataType()
    {
        using srcDataType = typename U::elementType;
        using dstDataType = typename T::elementType;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            is_one_of_attr_v<
                Std::tuple<dstDataType, srcDataType>, Std::tuple<half, half>,
                Std::tuple<int16_t, int16_t>, Std::tuple<uint16_t, uint16_t>,
                Std::tuple<bfloat16_t, bfloat16_t>, Std::tuple<uint32_t, uint32_t>,
                Std::tuple<int32_t, int32_t>, Std::tuple<float, float>,
                Std::tuple<uint8_t, uint8_t>, Std::tuple<int8_t, int8_t>,
                Std::tuple<fp8_e4m3fn_t, fp8_e4m3fn_t>,
                Std::tuple<fp8_e5m2_t, fp8_e5m2_t>,
                Std::tuple<fp4x2_e2m1_t, fp4x2_e2m1_t>,
                Std::tuple<fp4x2_e1m2_t, fp4x2_e1m2_t>,
                Std::tuple<hifloat8_t, hifloat8_t>>,
            "The data type is not supported.");
#endif
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_UTILS_CHECK_DATA_TYPE_3510_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif