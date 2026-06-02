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
    "impl/tensor_api/arch/vector/cast/instruction/half_int.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file half_int.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_HALF_INT_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_HALF_INT_H

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

ASCENDC_CAST_INST(Half2IntRD, asc_half2int8_rd);
ASCENDC_CAST_INST(Half2IntRDSat, asc_half2int8_rd_sat);
ASCENDC_CAST_INST(Half2IntRDOdd, asc_half2int8_rd_v2);
ASCENDC_CAST_INST(Half2IntRDSatOdd, asc_half2int8_rd_sat_v2);
ASCENDC_CAST_INST(Half2IntRN, asc_half2int8_rn);
ASCENDC_CAST_INST(Half2IntRNSat, asc_half2int8_rn_sat);
ASCENDC_CAST_INST(Half2IntRNOdd, asc_half2int8_rn_v2);
ASCENDC_CAST_INST(Half2IntRNSatOdd, asc_half2int8_rn_sat_v2);
ASCENDC_CAST_INST(Half2IntRNA, asc_half2int8_rna);
ASCENDC_CAST_INST(Half2IntRNASat, asc_half2int8_rna_sat);
ASCENDC_CAST_INST(Half2IntRNAOdd, asc_half2int8_rna_v2);
ASCENDC_CAST_INST(Half2IntRNASatOdd, asc_half2int8_rna_sat_v2);
ASCENDC_CAST_INST(Half2IntRU, asc_half2int8_ru);
ASCENDC_CAST_INST(Half2IntRUSat, asc_half2int8_ru_sat);
ASCENDC_CAST_INST(Half2IntRUOdd, asc_half2int8_ru_v2);
ASCENDC_CAST_INST(Half2IntRUSatOdd, asc_half2int8_ru_sat_v2);
ASCENDC_CAST_INST(Half2IntRZ, asc_half2int8_rz);
ASCENDC_CAST_INST(Half2IntRZSat, asc_half2int8_rz_sat);
ASCENDC_CAST_INST(Half2IntRZOdd, asc_half2int8_rz_v2);
ASCENDC_CAST_INST(Half2IntRZSatOdd, asc_half2int8_rz_sat_v2);

ASCENDC_CAST_INST(Half2Int16RD, asc_half2int16_rd);
ASCENDC_CAST_INST(Half2Int16RDSat, asc_half2int16_rd_sat);
ASCENDC_CAST_INST(Half2Int16RN, asc_half2int16_rn);
ASCENDC_CAST_INST(Half2Int16RNSat, asc_half2int16_rn_sat);
ASCENDC_CAST_INST(Half2Int16RNA, asc_half2int16_rna);
ASCENDC_CAST_INST(Half2Int16RNASat, asc_half2int16_rna_sat);
ASCENDC_CAST_INST(Half2Int16RU, asc_half2int16_ru);
ASCENDC_CAST_INST(Half2Int16RUSat, asc_half2int16_ru_sat);
ASCENDC_CAST_INST(Half2Int16RZ, asc_half2int16_rz);
ASCENDC_CAST_INST(Half2Int16RZSat, asc_half2int16_rz_sat);

ASCENDC_CAST_INST(Half2Int32RD, asc_half2int32_rd);
ASCENDC_CAST_INST(Half2Int32RDOdd, asc_half2int32_rd_v2);
ASCENDC_CAST_INST(Half2Int32RN, asc_half2int32_rn);
ASCENDC_CAST_INST(Half2Int32RNOdd, asc_half2int32_rn_v2);
ASCENDC_CAST_INST(Half2Int32RNA, asc_half2int32_rna);
ASCENDC_CAST_INST(Half2Int32RNAOdd, asc_half2int32_rna_v2);
ASCENDC_CAST_INST(Half2Int32RU, asc_half2int32_ru);
ASCENDC_CAST_INST(Half2Int32RUOdd, asc_half2int32_ru_v2);
ASCENDC_CAST_INST(Half2Int32RZ, asc_half2int32_rz);
ASCENDC_CAST_INST(Half2Int32RZOdd, asc_half2int32_rz_v2);

ASCENDC_CAST_INST(Half2Int4x2RD, asc_half2int4x2_rd);
ASCENDC_CAST_INST(Half2Int4x2RDSat, asc_half2int4x2_rd_sat);
ASCENDC_CAST_INST(Half2Int4x2RDP1, asc_half2int4x2_rd_v2);
ASCENDC_CAST_INST(Half2Int4x2RDSatP1, asc_half2int4x2_rd_sat_v2);
ASCENDC_CAST_INST(Half2Int4x2RDP2, asc_half2int4x2_rd_v3);
ASCENDC_CAST_INST(Half2Int4x2RDSatP2, asc_half2int4x2_rd_sat_v3);
ASCENDC_CAST_INST(Half2Int4x2RDP3, asc_half2int4x2_rd_v4);
ASCENDC_CAST_INST(Half2Int4x2RDSatP3, asc_half2int4x2_rd_sat_v4);
ASCENDC_CAST_INST(Half2Int4x2RN, asc_half2int4x2_rn);
ASCENDC_CAST_INST(Half2Int4x2RNSat, asc_half2int4x2_rn_sat);
ASCENDC_CAST_INST(Half2Int4x2RNP1, asc_half2int4x2_rn_v2);
ASCENDC_CAST_INST(Half2Int4x2RNSatP1, asc_half2int4x2_rn_sat_v2);
ASCENDC_CAST_INST(Half2Int4x2RNP2, asc_half2int4x2_rn_v3);
ASCENDC_CAST_INST(Half2Int4x2RNSatP2, asc_half2int4x2_rn_sat_v3);
ASCENDC_CAST_INST(Half2Int4x2RNP3, asc_half2int4x2_rn_v4);
ASCENDC_CAST_INST(Half2Int4x2RNSatP3, asc_half2int4x2_rn_sat_v4);
ASCENDC_CAST_INST(Half2Int4x2RNA, asc_half2int4x2_rna);
ASCENDC_CAST_INST(Half2Int4x2RNASat, asc_half2int4x2_rna_sat);
ASCENDC_CAST_INST(Half2Int4x2RNAP1, asc_half2int4x2_rna_v2);
ASCENDC_CAST_INST(Half2Int4x2RNASatP1, asc_half2int4x2_rna_sat_v2);
ASCENDC_CAST_INST(Half2Int4x2RNAP2, asc_half2int4x2_rna_v3);
ASCENDC_CAST_INST(Half2Int4x2RNASatP2, asc_half2int4x2_rna_sat_v3);
ASCENDC_CAST_INST(Half2Int4x2RNAP3, asc_half2int4x2_rna_v4);
ASCENDC_CAST_INST(Half2Int4x2RNASatP3, asc_half2int4x2_rna_sat_v4);
ASCENDC_CAST_INST(Half2Int4x2RU, asc_half2int4x2_ru);
ASCENDC_CAST_INST(Half2Int4x2RUSat, asc_half2int4x2_ru_sat);
ASCENDC_CAST_INST(Half2Int4x2RUP1, asc_half2int4x2_ru_v2);
ASCENDC_CAST_INST(Half2Int4x2RUSatP1, asc_half2int4x2_ru_sat_v2);
ASCENDC_CAST_INST(Half2Int4x2RUP2, asc_half2int4x2_ru_v3);
ASCENDC_CAST_INST(Half2Int4x2RUSatP2, asc_half2int4x2_ru_sat_v3);
ASCENDC_CAST_INST(Half2Int4x2RUP3, asc_half2int4x2_ru_v4);
ASCENDC_CAST_INST(Half2Int4x2RUSatP3, asc_half2int4x2_ru_sat_v4);
ASCENDC_CAST_INST(Half2Int4x2RZ, asc_half2int4x2_rz);
ASCENDC_CAST_INST(Half2Int4x2RZSat, asc_half2int4x2_rz_sat);
ASCENDC_CAST_INST(Half2Int4x2RZP1, asc_half2int4x2_rz_v2);
ASCENDC_CAST_INST(Half2Int4x2RZSatP1, asc_half2int4x2_rz_sat_v2);
ASCENDC_CAST_INST(Half2Int4x2RZP2, asc_half2int4x2_rz_v3);
ASCENDC_CAST_INST(Half2Int4x2RZSatP2, asc_half2int4x2_rz_sat_v3);
ASCENDC_CAST_INST(Half2Int4x2RZP3, asc_half2int4x2_rz_v4);
ASCENDC_CAST_INST(Half2Int4x2RZSatP3, asc_half2int4x2_rz_sat_v4);

ASCENDC_CAST_INST(Int162HalfRD, asc_int162half_rd);
ASCENDC_CAST_INST(Int162HalfRN, asc_int162half_rn);
ASCENDC_CAST_INST(Int162HalfRNA, asc_int162half_rna);
ASCENDC_CAST_INST(Int162HalfRU, asc_int162half_ru);
ASCENDC_CAST_INST(Int162HalfRZ, asc_int162half_rz);

ASCENDC_CAST_INST(Int4x22Half, asc_int4x22half);
ASCENDC_CAST_INST(Int4x22HalfP1, asc_int4x22half_v2);
ASCENDC_CAST_INST(Int4x22HalfP2, asc_int4x22half_v3);
ASCENDC_CAST_INST(Int4x22HalfP3, asc_int4x22half_v4);

ASCENDC_CAST_INST(Int82Half, asc_int82half);
ASCENDC_CAST_INST(Int82HalfOdd, asc_int82half_v2);

#undef ASCENDC_CAST_INST

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_HALF_INT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
