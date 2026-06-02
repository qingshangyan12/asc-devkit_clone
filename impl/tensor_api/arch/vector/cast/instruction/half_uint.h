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
    "impl/tensor_api/arch/vector/cast/instruction/half_uint.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file half_uint.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_HALF_UINT_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_HALF_UINT_H

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

ASCENDC_CAST_INST(Half2UIntRD, asc_half2uint8_rd);
ASCENDC_CAST_INST(Half2UIntRDSat, asc_half2uint8_rd_sat);
ASCENDC_CAST_INST(Half2UIntRDOdd, asc_half2uint8_rd_v2);
ASCENDC_CAST_INST(Half2UIntRDSatOdd, asc_half2uint8_rd_sat_v2);
ASCENDC_CAST_INST(Half2UIntRN, asc_half2uint8_rn);
ASCENDC_CAST_INST(Half2UIntRNSat, asc_half2uint8_rn_sat);
ASCENDC_CAST_INST(Half2UIntRNOdd, asc_half2uint8_rn_v2);
ASCENDC_CAST_INST(Half2UIntRNSatOdd, asc_half2uint8_rn_sat_v2);
ASCENDC_CAST_INST(Half2UIntRNA, asc_half2uint8_rna);
ASCENDC_CAST_INST(Half2UIntRNASat, asc_half2uint8_rna_sat);
ASCENDC_CAST_INST(Half2UIntRNAOdd, asc_half2uint8_rna_v2);
ASCENDC_CAST_INST(Half2UIntRNASatOdd, asc_half2uint8_rna_sat_v2);
ASCENDC_CAST_INST(Half2UIntRU, asc_half2uint8_ru);
ASCENDC_CAST_INST(Half2UIntRUSat, asc_half2uint8_ru_sat);
ASCENDC_CAST_INST(Half2UIntRUOdd, asc_half2uint8_ru_v2);
ASCENDC_CAST_INST(Half2UIntRUSatOdd, asc_half2uint8_ru_sat_v2);
ASCENDC_CAST_INST(Half2UIntRZ, asc_half2uint8_rz);
ASCENDC_CAST_INST(Half2UIntRZSat, asc_half2uint8_rz_sat);
ASCENDC_CAST_INST(Half2UIntRZOdd, asc_half2uint8_rz_v2);
ASCENDC_CAST_INST(Half2UIntRZSatOdd, asc_half2uint8_rz_sat_v2);

ASCENDC_CAST_INST(U82Half, asc_uint82half);
ASCENDC_CAST_INST(U82HalfOdd, asc_uint82half_v2);

#undef ASCENDC_CAST_INST

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_HALF_UINT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
