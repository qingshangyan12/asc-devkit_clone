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
    "impl/tensor_api/arch/vector/cast/instruction/float_fp8.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file float_fp8.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_FP8_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_FP8_H

#define ASCENDC_CAST_INST(CLASS_NAME, FUNC_NAME)                                                                     \
class CLASS_NAME {                                                                                                   \
public:                                                                                                              \
    template <typename T, typename U>                                                                                \
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask)                                         \
    {                                                                                                                \
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {                                                  \
            FUNC_NAME(dst, src, mask);                                                                               \
        }                                                                                                            \
    }                                                                                                                \
}

ASCENDC_CAST_INST(E4M32Float, asc_e4m32float);
ASCENDC_CAST_INST(E4M32FloatP2, asc_e4m32float_v2);
ASCENDC_CAST_INST(E4M32FloatP3, asc_e4m32float_v3);
ASCENDC_CAST_INST(E4M32FloatP4, asc_e4m32float_v4);
ASCENDC_CAST_INST(E5M22Float, asc_e5m22float);
ASCENDC_CAST_INST(E5M22FloatP2, asc_e5m22float_v2);
ASCENDC_CAST_INST(E5M22FloatP3, asc_e5m22float_v3);
ASCENDC_CAST_INST(E5M22FloatP4, asc_e5m22float_v4);

ASCENDC_CAST_INST(Float2E4M3RN, asc_float2e4m3_rn);
ASCENDC_CAST_INST(Float2E4M3RNSat, asc_float2e4m3_rn_sat);
ASCENDC_CAST_INST(Float2E4M3RNP2, asc_float2e4m3_rn_v2);
ASCENDC_CAST_INST(Float2E4M3RNSatP2, asc_float2e4m3_rn_sat_v2);
ASCENDC_CAST_INST(Float2E4M3RNP3, asc_float2e4m3_rn_v3);
ASCENDC_CAST_INST(Float2E4M3RNSatP3, asc_float2e4m3_rn_sat_v3);
ASCENDC_CAST_INST(Float2E4M3RNP4, asc_float2e4m3_rn_v4);
ASCENDC_CAST_INST(Float2E4M3RNSatP4, asc_float2e4m3_rn_sat_v4);

ASCENDC_CAST_INST(Float2E5M2RN, asc_float2e5m2_rn);
ASCENDC_CAST_INST(Float2E5M2RNSat, asc_float2e5m2_rn_sat);
ASCENDC_CAST_INST(Float2E5M2RNP2, asc_float2e5m2_rn_v2);
ASCENDC_CAST_INST(Float2E5M2RNSatP2, asc_float2e5m2_rn_sat_v2);
ASCENDC_CAST_INST(Float2E5M2RNP3, asc_float2e5m2_rn_v3);
ASCENDC_CAST_INST(Float2E5M2RNSatP3, asc_float2e5m2_rn_sat_v3);
ASCENDC_CAST_INST(Float2E5M2RNP4, asc_float2e5m2_rn_v4);
ASCENDC_CAST_INST(Float2E5M2RNSatP4, asc_float2e5m2_rn_sat_v4);

#undef ASCENDC_CAST_INST

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_FP8_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
