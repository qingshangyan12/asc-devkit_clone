/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
/*!
 * \file check_data_type_3510.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_ARCH_UTILS_CHECK_DATA_TYPE_3510_H
#define IMPL_TENSOR_API_ARCH_ARCH_UTILS_CHECK_DATA_TYPE_3510_H

namespace AscendC {
namespace Te {

class CheckDataTypeFor3510 {
public:
    template <Hardware biasPos, typename dstDataType, typename fmDataType, typename filterDataType,
              typename biasDataType>
    __aicore__ inline static constexpr void CheckMxMmadDataType()
    {
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        if constexpr (biasPos == Hardware::BIAS) {
            static_assert(
                Std::is_one_of_v<Std::tuple<biasDataType, dstDataType, fmDataType, filterDataType>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp4x2_e2m1_t, __cb__ fp4x2_e2m1_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp4x2_e2m1_t, __cb__ fp4x2_e1m2_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp4x2_e1m2_t, __cb__ fp4x2_e2m1_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp4x2_e1m2_t, __cb__ fp4x2_e1m2_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp8_e4m3fn_t, __cb__ fp8_e4m3fn_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp8_e4m3fn_t, __cb__ fp8_e5m2_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp8_e5m2_t, __cb__ fp8_e4m3fn_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp8_e5m2_t, __cb__ fp8_e5m2_t>>,
                "The data type is not supported for BIAS position.");
        } else if constexpr (biasPos == Hardware::L0C) {
            static_assert(
                Std::is_one_of_v<Std::tuple<biasDataType, dstDataType, fmDataType, filterDataType>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp4x2_e2m1_t, __cb__ fp4x2_e2m1_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp4x2_e2m1_t, __cb__ fp4x2_e1m2_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp4x2_e1m2_t, __cb__ fp4x2_e2m1_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp4x2_e1m2_t, __cb__ fp4x2_e1m2_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp8_e4m3fn_t, __cb__ fp8_e4m3fn_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp8_e4m3fn_t, __cb__ fp8_e5m2_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp8_e5m2_t, __cb__ fp8_e4m3fn_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp8_e5m2_t, __cb__ fp8_e5m2_t>>,
                "The data type is not supported for L0C position.");
        }
#endif
    }
    template <Hardware biasPos, typename dstDataType, typename fmDataType, typename filterDataType,
              typename biasDataType>
    __aicore__ inline static constexpr void CheckMmadDataType()
    {
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        if constexpr (biasPos == Hardware::BIAS) {
            static_assert(
                Std::is_one_of_v<Std::tuple<biasDataType, dstDataType, fmDataType, filterDataType>,
                                 Std::tuple<__biasbuf__ int32_t, __cc__ int32_t, __ca__ int8_t, __cb__ int8_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ half, __cb__ half>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ float, __cb__ float>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ bfloat16_t, __cb__ bfloat16_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp8_e4m3fn_t, __cb__ fp8_e4m3fn_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp8_e4m3fn_t, __cb__ fp8_e5m2_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp8_e5m2_t, __cb__ fp8_e4m3fn_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ fp8_e5m2_t, __cb__ fp8_e5m2_t>,
                                 Std::tuple<__biasbuf__ float, __cc__ float, __ca__ hifloat8_t, __cb__ hifloat8_t>>,
                "The data type is not supported for BIAS position.");
        } else if constexpr (biasPos == Hardware::L0C) {
            static_assert(
                Std::is_one_of_v<Std::tuple<biasDataType, dstDataType, fmDataType, filterDataType>,
                                 Std::tuple<__cc__ int32_t, __cc__ int32_t, __ca__ int8_t, __cb__ int8_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ half, __cb__ half>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ float, __cb__ float>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ bfloat16_t, __cb__ bfloat16_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp8_e4m3fn_t, __cb__ fp8_e4m3fn_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp8_e4m3fn_t, __cb__ fp8_e5m2_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp8_e5m2_t, __cb__ fp8_e4m3fn_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ fp8_e5m2_t, __cb__ fp8_e5m2_t>,
                                 Std::tuple<__cc__ float, __cc__ float, __ca__ hifloat8_t, __cb__ hifloat8_t>>,
                "The data type is not supported for L0C position.");
        }
#endif
    }

    template <typename dstDataType, typename srcDataType>
    __aicore__ inline static constexpr void CheckGm2L1DataType()
    {
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            Std::is_one_of_v<Std::tuple<dstDataType, srcDataType>, Std::tuple<__cbuf__ bfloat16_t, __gm__ bfloat16_t>,
                             Std::tuple<__cbuf__ half, __gm__ half>, Std::tuple<__cbuf__ float, __gm__ float>,
                             Std::tuple<__cbuf__ int16_t, __gm__ int16_t>, Std::tuple<__cbuf__ int32_t, __gm__ int32_t>,
                             Std::tuple<__cbuf__ int8_t, __gm__ int8_t>, Std::tuple<__cbuf__ uint16_t, __gm__ uint16_t>,
                             Std::tuple<__cbuf__ uint32_t, __gm__ uint32_t>,
                             Std::tuple<__cbuf__ uint8_t, __gm__ uint8_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename dstDataType, typename srcDataType>
    __aicore__ inline static constexpr void CheckGm2L1Fp4DataType()
    {
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            Std::is_one_of_v<Std::tuple<dstDataType, srcDataType>, Std::tuple<__cbuf__ bfloat16_t, __gm__ bfloat16_t>,
                             Std::tuple<__cbuf__ half, __gm__ half>, Std::tuple<__cbuf__ float, __gm__ float>,
                             Std::tuple<__cbuf__ int16_t, __gm__ int16_t>, Std::tuple<__cbuf__ int32_t, __gm__ int32_t>,
                             Std::tuple<__cbuf__ int8_t, __gm__ int8_t>, Std::tuple<__cbuf__ uint16_t, __gm__ uint16_t>,
                             Std::tuple<__cbuf__ uint32_t, __gm__ uint32_t>,
                             Std::tuple<__cbuf__ uint8_t, __gm__ uint8_t>,
                             Std::tuple<__cbuf__ fp4x2_e1m2_t, __gm__ fp4x2_e1m2_t>,
                             Std::tuple<__cbuf__ fp4x2_e2m1_t, __gm__ fp4x2_e2m1_t>,
                             Std::tuple<__cbuf__ fp8_e5m2_t, __gm__ fp8_e5m2_t>,
                             Std::tuple<__cbuf__ fp8_e4m3fn_t, __gm__ fp8_e4m3fn_t>,
                             Std::tuple<__cbuf__ fp8_e8m0_t, __gm__ fp8_e8m0_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename dstDataType, typename srcDataType>
    __aicore__ inline static constexpr void CheckGm2L1AlignV2NDDataType()
    {
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            Std::is_one_of_v<Std::tuple<dstDataType, srcDataType>, Std::tuple<__cbuf__ bfloat16_t, __gm__ bfloat16_t>,
                             Std::tuple<__cbuf__ half, __gm__ half>, Std::tuple<__cbuf__ float, __gm__ float>,
                             Std::tuple<__cbuf__ int32_t, __gm__ int32_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename dstDataType, typename srcDataType>
    __aicore__ inline static constexpr void CheckL12BtDataType()
    {
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            Std::is_one_of_v<Std::tuple<dstDataType, srcDataType>, Std::tuple<__biasbuf__ float, __cbuf__ bfloat16_t>,
                             Std::tuple<__biasbuf__ float, __cbuf__ half>,
                             Std::tuple<__biasbuf__ float, __cbuf__ float>,
                             Std::tuple<__biasbuf__ int32_t, __cbuf__ int32_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename dstDataType, typename srcDataType>
    __aicore__ inline static constexpr void CheckL12FbDataType()
    {
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            Std::is_one_of_v<
                Std::tuple<dstDataType, srcDataType>, Std::tuple<__fbuf__ bool, __cbuf__ bool>,
                Std::tuple<__fbuf__ int8_t, __cbuf__ int8_t>, Std::tuple<__fbuf__ uint8_t, __cbuf__ uint8_t>,
                Std::tuple<__fbuf__ hifloat8_t, __cbuf__ hifloat8_t>,
                Std::tuple<__fbuf__ fp8_e5m2_t, __cbuf__ fp8_e5m2_t>,
                Std::tuple<__fbuf__ fp8_e4m3fn_t, __cbuf__ fp8_e4m3fn_t>,
                Std::tuple<__fbuf__ fp8_e8m0_t, __cbuf__ fp8_e8m0_t>, Std::tuple<__fbuf__ int16_t, __cbuf__ int16_t>,
                Std::tuple<__fbuf__ uint16_t, __cbuf__ uint16_t>, Std::tuple<__fbuf__ half, __cbuf__ half>,
                Std::tuple<__fbuf__ float, __cbuf__ float>, Std::tuple<__fbuf__ bfloat16_t, __cbuf__ bfloat16_t>,
                Std::tuple<__fbuf__ int32_t, __cbuf__ int32_t>, Std::tuple<__fbuf__ uint32_t, __cbuf__ uint32_t>,
                Std::tuple<__fbuf__ int64_t, __cbuf__ int64_t>, Std::tuple<__fbuf__ uint64_t, __cbuf__ uint64_t>,
                Std::tuple<__fbuf__ double, __cbuf__ double>>,
            "The data type is not supported.");
#endif
    }

    template <typename dstDataType, typename srcDataType>
    __aicore__ inline static constexpr void CheckFixPipeDataType()
    {
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(Std::is_one_of_v<Std::tuple<dstDataType, srcDataType>, Std::tuple<__gm__ float, __cc__ float>,
                                       Std::tuple<__gm__ int32_t, __cc__ int32_t>>,
                      "The data type is not supported.");
#endif
    }

    template <const FixpipeTrait& trait, typename T, typename U>
    __aicore__ inline static constexpr void CheckFixpipeQuantDataType()
    {
        using srcDataType = typename U::elementType;
        using dstDataType = typename T::elementType;
        using currentType = Std::tuple<srcDataType, dstDataType>;
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        if constexpr (trait.quantPre == QuantMode_t::NoQuant) {
            using quantDataType1 = Std::tuple<__cc__ float, __gm__ float>;
            using quantDataType2 = Std::tuple<__cc__ int32_t, __gm__ int32_t>;
            static_assert((Std::is_one_of_v<currentType, quantDataType1, quantDataType2>),
                          "Failed to check quantPre value in Fixpipe");
        } else if constexpr (trait.quantPre == QuantMode_t::F322F16 || trait.quantPre == QuantMode_t::QF322F16_PRE
                             || trait.quantPre == QuantMode_t::VQF322F16_PRE) {
            using quantDataType = Std::tuple<__cc__ float, __gm__ half>;
            static_assert((Std::is_one_of_v<currentType, quantDataType>), "Failed to check quantPre value in Fixpipe");
        } else if constexpr (trait.quantPre == QuantMode_t::F322BF16 || trait.quantPre == QuantMode_t::QF322BF16_PRE
                             || trait.quantPre == QuantMode_t::VQF322BF16_PRE) {
            using quantDataType = Std::tuple<__cc__ float, __gm__ bfloat16_t>;
            static_assert((Std::is_one_of_v<currentType, quantDataType>), "Failed to check quantPre value in Fixpipe");
        } else if constexpr (trait.quantPre == QuantMode_t::DEQF16 || trait.quantPre == QuantMode_t::VDEQF16) {
            using quantDataType = Std::tuple<__cc__ int32_t, __gm__ half>;
            static_assert((Std::is_one_of_v<currentType, quantDataType>), "Failed to check quantPre value in Fixpipe");
        } else if constexpr (trait.quantPre == QuantMode_t::QF322B8_PRE
                             || trait.quantPre == QuantMode_t::VQF322B8_PRE) {
            using quantDataType1 = Std::tuple<__cc__ float, __gm__ int8_t>;
            using quantDataType2 = Std::tuple<__cc__ float, __gm__ uint8_t>;
            static_assert((Std::is_one_of_v<currentType, quantDataType1, quantDataType2>),
                          "Failed to check quantPre value in Fixpipe");
        } else if constexpr (trait.quantPre == QuantMode_t::REQ8 || trait.quantPre == QuantMode_t::VREQ8) {
            using quantDataType1 = Std::tuple<__cc__ int32_t, __gm__ int8_t>;
            using quantDataType2 = Std::tuple<__cc__ int32_t, __gm__ uint8_t>;
            static_assert((Std::is_one_of_v<currentType, quantDataType1, quantDataType2>),
                          "Failed to check quantPre value in Fixpipe");
        } else if constexpr (trait.quantPre == QuantMode_t::QF322FP8_PRE
                             || trait.quantPre == QuantMode_t::VQF322FP8_PRE) {
            using quantDataType = Std::tuple<__cc__ float, __gm__ fp8_e4m3fn_t>;
            static_assert((Std::is_one_of_v<currentType, quantDataType>), "Failed to check quantPre value in Fixpipe");
        } else if constexpr (trait.quantPre == QuantMode_t::QF322HIF8_PRE
                             || trait.quantPre == QuantMode_t::VQF322HIF8_PRE
                             || trait.quantPre == QuantMode_t::QF322HIF8_PRE_HYBRID
                             || trait.quantPre == QuantMode_t::VQF322HIF8_PRE_HYBRID) {
            using quantDataType = Std::tuple<__cc__ float, __gm__ hifloat8_t>;
            static_assert((Std::is_one_of_v<currentType, quantDataType>), "Failed to check quantPre value in Fixpipe");
        } else if constexpr (trait.quantPre == QuantMode_t::QS322BF16_PRE
                             || trait.quantPre == QuantMode_t::VQS322BF16_PRE) {
            using quantDataType = Std::tuple<__cc__ int32_t, __gm__ bfloat16_t>;
            static_assert((Std::is_one_of_v<currentType, quantDataType>), "Failed to check quantPre value in Fixpipe");
        } else if constexpr (trait.quantPre == QuantMode_t::QF322F32_PRE
                             || trait.quantPre == QuantMode_t::VQF322F32_PRE) {
            using quantDataType = Std::tuple<__cc__ float, __gm__ float>;
            static_assert((Std::is_one_of_v<currentType, quantDataType>), "Failed to check quantPre value in Fixpipe");
        }

#endif
    }

    template <typename dstDataType, typename srcDataType>
    __aicore__ inline static constexpr void CheckL12L0ADataType()
    {
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            Std::is_one_of_v<
                Std::tuple<dstDataType, srcDataType>, Std::tuple<__ca__ half, __cbuf__ half>,
                Std::tuple<__ca__ int16_t, __cbuf__ int16_t>, Std::tuple<__ca__ uint16_t, __cbuf__ uint16_t>,
                Std::tuple<__ca__ bfloat16_t, __cbuf__ bfloat16_t>, Std::tuple<__ca__ uint32_t, __cbuf__ uint32_t>,
                Std::tuple<__ca__ int32_t, __cbuf__ int32_t>, Std::tuple<__ca__ float, __cbuf__ float>,
                Std::tuple<__ca__ uint8_t, __cbuf__ uint8_t>, Std::tuple<__ca__ int8_t, __cbuf__ int8_t>,
                Std::tuple<__ca__ fp8_e4m3fn_t, __cbuf__ fp8_e4m3fn_t>,
                Std::tuple<__ca__ fp8_e5m2_t, __cbuf__ fp8_e5m2_t>,
                Std::tuple<__ca__ fp4x2_e2m1_t, __cbuf__ fp4x2_e2m1_t>,
                Std::tuple<__ca__ fp4x2_e1m2_t, __cbuf__ fp4x2_e1m2_t>,
                Std::tuple<__ca__ hifloat8_t, __cbuf__ hifloat8_t>>,
            "The data type is not supported.");
#endif
    }

    template <typename dstDataType, typename srcDataType>
    __aicore__ inline static constexpr void CheckL12L0BDataType()
    {
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
        static_assert(
            Std::is_one_of_v<
                Std::tuple<dstDataType, srcDataType>, Std::tuple<__cb__ half, __cbuf__ half>,
                Std::tuple<__cb__ int16_t, __cbuf__ int16_t>, Std::tuple<__cb__ uint16_t, __cbuf__ uint16_t>,
                Std::tuple<__cb__ bfloat16_t, __cbuf__ bfloat16_t>, Std::tuple<__cb__ uint32_t, __cbuf__ uint32_t>,
                Std::tuple<__cb__ int32_t, __cbuf__ int32_t>, Std::tuple<__cb__ float, __cbuf__ float>,
                Std::tuple<__cb__ uint8_t, __cbuf__ uint8_t>, Std::tuple<__cb__ int8_t, __cbuf__ int8_t>,
                Std::tuple<__cb__ fp8_e4m3fn_t, __cbuf__ fp8_e4m3fn_t>,
                Std::tuple<__cb__ fp8_e5m2_t, __cbuf__ fp8_e5m2_t>,
                Std::tuple<__cb__ fp4x2_e2m1_t, __cbuf__ fp4x2_e2m1_t>,
                Std::tuple<__cb__ fp4x2_e1m2_t, __cbuf__ fp4x2_e1m2_t>,
                Std::tuple<__cb__ hifloat8_t, __cbuf__ hifloat8_t>>,
            "The data type is not supported.");
#endif
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_ARCH_UTILS_CHECK_DATA_TYPE_3510_H