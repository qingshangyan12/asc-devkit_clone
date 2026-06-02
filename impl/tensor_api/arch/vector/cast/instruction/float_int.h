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
    "impl/tensor_api/arch/vector/cast/instruction/float_int.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file float_int.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_INT_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_INT_H

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

ASCENDC_CAST_INST(Float2Int16RD, asc_float2int16_rd);
ASCENDC_CAST_INST(Float2Int16RDSat, asc_float2int16_rd_sat);
ASCENDC_CAST_INST(Float2Int16RDOdd, asc_float2int16_rd_v2);
ASCENDC_CAST_INST(Float2Int16RDSatOdd, asc_float2int16_rd_sat_v2);
ASCENDC_CAST_INST(Float2Int16RN, asc_float2int16_rn);
ASCENDC_CAST_INST(Float2Int16RNSat, asc_float2int16_rn_sat);
ASCENDC_CAST_INST(Float2Int16RNOdd, asc_float2int16_rn_v2);
ASCENDC_CAST_INST(Float2Int16RNSatOdd, asc_float2int16_rn_sat_v2);
ASCENDC_CAST_INST(Float2Int16RNA, asc_float2int16_rna);
ASCENDC_CAST_INST(Float2Int16RNASat, asc_float2int16_rna_sat);
ASCENDC_CAST_INST(Float2Int16RNAOdd, asc_float2int16_rna_v2);
ASCENDC_CAST_INST(Float2Int16RNASatOdd, asc_float2int16_rna_sat_v2);
ASCENDC_CAST_INST(Float2Int16RU, asc_float2int16_ru);
ASCENDC_CAST_INST(Float2Int16RUSat, asc_float2int16_ru_sat);
ASCENDC_CAST_INST(Float2Int16RUOdd, asc_float2int16_ru_v2);
ASCENDC_CAST_INST(Float2Int16RUSatOdd, asc_float2int16_ru_sat_v2);
ASCENDC_CAST_INST(Float2Int16RZ, asc_float2int16_rz);
ASCENDC_CAST_INST(Float2Int16RZSat, asc_float2int16_rz_sat);
ASCENDC_CAST_INST(Float2Int16RZOdd, asc_float2int16_rz_v2);
ASCENDC_CAST_INST(Float2Int16RZSatOdd, asc_float2int16_rz_sat_v2);

ASCENDC_CAST_INST(Float2Int32RD, asc_float2int32_rd);
ASCENDC_CAST_INST(Float2Int32RDSat, asc_float2int32_rd_sat);
ASCENDC_CAST_INST(Float2Int32RN, asc_float2int32_rn);
ASCENDC_CAST_INST(Float2Int32RNSat, asc_float2int32_rn_sat);
ASCENDC_CAST_INST(Float2Int32RNA, asc_float2int32_rna);
ASCENDC_CAST_INST(Float2Int32RNASat, asc_float2int32_rna_sat);
ASCENDC_CAST_INST(Float2Int32RU, asc_float2int32_ru);
ASCENDC_CAST_INST(Float2Int32RUSat, asc_float2int32_ru_sat);
ASCENDC_CAST_INST(Float2Int32RZ, asc_float2int32_rz);
ASCENDC_CAST_INST(Float2Int32RZSat, asc_float2int32_rz_sat);

ASCENDC_CAST_INST(Float2Int64RD, asc_float2int64_rd);
ASCENDC_CAST_INST(Float2Int64RDSat, asc_float2int64_rd_sat);
ASCENDC_CAST_INST(Float2Int64RDOdd, asc_float2int64_rd_v2);
ASCENDC_CAST_INST(Float2Int64RDSatOdd, asc_float2int64_rd_sat_v2);
ASCENDC_CAST_INST(Float2Int64RN, asc_float2int64_rn);
ASCENDC_CAST_INST(Float2Int64RNSat, asc_float2int64_rn_sat);
ASCENDC_CAST_INST(Float2Int64RNOdd, asc_float2int64_rn_v2);
ASCENDC_CAST_INST(Float2Int64RNSatOdd, asc_float2int64_rn_sat_v2);
ASCENDC_CAST_INST(Float2Int64RNA, asc_float2int64_rna);
ASCENDC_CAST_INST(Float2Int64RNASat, asc_float2int64_rna_sat);
ASCENDC_CAST_INST(Float2Int64RNAOdd, asc_float2int64_rna_v2);
ASCENDC_CAST_INST(Float2Int64RNASatOdd, asc_float2int64_rna_sat_v2);
ASCENDC_CAST_INST(Float2Int64RU, asc_float2int64_ru);
ASCENDC_CAST_INST(Float2Int64RUSat, asc_float2int64_ru_sat);
ASCENDC_CAST_INST(Float2Int64RUOdd, asc_float2int64_ru_v2);
ASCENDC_CAST_INST(Float2Int64RUSatOdd, asc_float2int64_ru_sat_v2);
ASCENDC_CAST_INST(Float2Int64RZ, asc_float2int64_rz);
ASCENDC_CAST_INST(Float2Int64RZSat, asc_float2int64_rz_sat);
ASCENDC_CAST_INST(Float2Int64RZOdd, asc_float2int64_rz_v2);
ASCENDC_CAST_INST(Float2Int64RZSatOdd, asc_float2int64_rz_sat_v2);

ASCENDC_CAST_INST(Int162Float, asc_int162float);
ASCENDC_CAST_INST(Int162FloatOdd, asc_int162float_v2);
ASCENDC_CAST_INST(Int322FloatRD, asc_int322float_rd);
ASCENDC_CAST_INST(Int322FloatRN, asc_int322float_rn);
ASCENDC_CAST_INST(Int322FloatRNA, asc_int322float_rna);
ASCENDC_CAST_INST(Int322FloatRU, asc_int322float_ru);
ASCENDC_CAST_INST(Int322FloatRZ, asc_int322float_rz);
ASCENDC_CAST_INST(Int642FloatRD, asc_int642float_rd);
ASCENDC_CAST_INST(Int642FloatRDOdd, asc_int642float_rd_v2);
ASCENDC_CAST_INST(Int642FloatRN, asc_int642float_rn);
ASCENDC_CAST_INST(Int642FloatRNOdd, asc_int642float_rn_v2);
ASCENDC_CAST_INST(Int642FloatRNA, asc_int642float_rna);
ASCENDC_CAST_INST(Int642FloatRNAOdd, asc_int642float_rna_v2);
ASCENDC_CAST_INST(Int642FloatRU, asc_int642float_ru);
ASCENDC_CAST_INST(Int642FloatRUOdd, asc_int642float_ru_v2);
ASCENDC_CAST_INST(Int642FloatRZ, asc_int642float_rz);
ASCENDC_CAST_INST(Int642FloatRZOdd, asc_int642float_rz_v2);

#undef ASCENDC_CAST_INST

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_INT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
