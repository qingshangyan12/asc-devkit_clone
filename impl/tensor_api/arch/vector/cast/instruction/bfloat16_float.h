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
    "impl/tensor_api/arch/vector/cast/instruction/bfloat16_float.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file bfloat16_float.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_BFLOAT16_FLOAT_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_BFLOAT16_FLOAT_H

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

ASCENDC_CAST_INST(BF162Float, asc_bfloat162float);
ASCENDC_CAST_INST(BF162FloatP2, asc_bfloat162float_v2);

ASCENDC_CAST_INST(Float2BF16RD, asc_float2bfloat16_rd);
ASCENDC_CAST_INST(Float2BF16RDSat, asc_float2bfloat16_rd_sat);
ASCENDC_CAST_INST(Float2BF16RDP2, asc_float2bfloat16_rd_v2);
ASCENDC_CAST_INST(Float2BF16RDSatP2, asc_float2bfloat16_rd_sat_v2);
ASCENDC_CAST_INST(Float2BF16RN, asc_float2bfloat16_rn);
ASCENDC_CAST_INST(Float2BF16RNSat, asc_float2bfloat16_rn_sat);
ASCENDC_CAST_INST(Float2BF16RNP2, asc_float2bfloat16_rn_v2);
ASCENDC_CAST_INST(Float2BF16RNSatP2, asc_float2bfloat16_rn_sat_v2);
ASCENDC_CAST_INST(Float2BF16RNA, asc_float2bfloat16_rna);
ASCENDC_CAST_INST(Float2BF16RNASat, asc_float2bfloat16_rna_sat);
ASCENDC_CAST_INST(Float2BF16RNAP2, asc_float2bfloat16_rna_v2);
ASCENDC_CAST_INST(Float2BF16RNASatP2, asc_float2bfloat16_rna_sat_v2);
ASCENDC_CAST_INST(Float2BF16RU, asc_float2bfloat16_ru);
ASCENDC_CAST_INST(Float2BF16RUSat, asc_float2bfloat16_ru_sat);
ASCENDC_CAST_INST(Float2BF16RUP2, asc_float2bfloat16_ru_v2);
ASCENDC_CAST_INST(Float2BF16RUSatP2, asc_float2bfloat16_ru_sat_v2);
ASCENDC_CAST_INST(Float2BF16RZ, asc_float2bfloat16_rz);
ASCENDC_CAST_INST(Float2BF16RZSat, asc_float2bfloat16_rz_sat);
ASCENDC_CAST_INST(Float2BF16RZP2, asc_float2bfloat16_rz_v2);
ASCENDC_CAST_INST(Float2BF16RZSatP2, asc_float2bfloat16_rz_sat_v2);

#undef ASCENDC_CAST_INST

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_BFLOAT16_FLOAT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
