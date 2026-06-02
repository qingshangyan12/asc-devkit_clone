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
    "impl/tensor_api/arch/vector/cast/instruction/float_hif8.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file float_hif8.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_HIF8_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_HIF8_H

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

ASCENDC_CAST_INST(Hif82Float, asc_hif82float);
ASCENDC_CAST_INST(Hif82FloatP1, asc_hif82float_v2);
ASCENDC_CAST_INST(Hif82FloatP2, asc_hif82float_v3);
ASCENDC_CAST_INST(Hif82FloatP3, asc_hif82float_v4);

ASCENDC_CAST_INST(Float2Hif8RH, asc_float2hif8_rh);
ASCENDC_CAST_INST(Float2Hif8RHSat, asc_float2hif8_rh_sat);
ASCENDC_CAST_INST(Float2Hif8RHP1, asc_float2hif8_rh_v2);
ASCENDC_CAST_INST(Float2Hif8RHSatP1, asc_float2hif8_rh_sat_v2);
ASCENDC_CAST_INST(Float2Hif8RHP2, asc_float2hif8_rh_v3);
ASCENDC_CAST_INST(Float2Hif8RHSatP2, asc_float2hif8_rh_sat_v3);
ASCENDC_CAST_INST(Float2Hif8RHP3, asc_float2hif8_rh_v4);
ASCENDC_CAST_INST(Float2Hif8RHSatP3, asc_float2hif8_rh_sat_v4);
ASCENDC_CAST_INST(Float2Hif8RNA, asc_float2hif8_rna);
ASCENDC_CAST_INST(Float2Hif8RNASat, asc_float2hif8_rna_sat);
ASCENDC_CAST_INST(Float2Hif8RNAP1, asc_float2hif8_rna_v2);
ASCENDC_CAST_INST(Float2Hif8RNASatP1, asc_float2hif8_rna_sat_v2);
ASCENDC_CAST_INST(Float2Hif8RNAP2, asc_float2hif8_rna_v3);
ASCENDC_CAST_INST(Float2Hif8RNASatP2, asc_float2hif8_rna_sat_v3);
ASCENDC_CAST_INST(Float2Hif8RNAP3, asc_float2hif8_rna_v4);
ASCENDC_CAST_INST(Float2Hif8RNASatP3, asc_float2hif8_rna_sat_v4);

#undef ASCENDC_CAST_INST

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_HIF8_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
