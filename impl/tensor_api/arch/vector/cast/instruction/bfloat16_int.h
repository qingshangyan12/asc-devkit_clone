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
    "impl/tensor_api/arch/vector/cast/instruction/bfloat16_int.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file bfloat16_int.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_BFLOAT16_INT_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_BFLOAT16_INT_H

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

ASCENDC_CAST_INST(BF162Int32RD, asc_bfloat162int32_rd);
ASCENDC_CAST_INST(BF162Int32RDSat, asc_bfloat162int32_rd_sat);
ASCENDC_CAST_INST(BF162Int32RDP2, asc_bfloat162int32_rd_v2);
ASCENDC_CAST_INST(BF162Int32RDSatP2, asc_bfloat162int32_rd_sat_v2);
ASCENDC_CAST_INST(BF162Int32RN, asc_bfloat162int32_rn);
ASCENDC_CAST_INST(BF162Int32RNSat, asc_bfloat162int32_rn_sat);
ASCENDC_CAST_INST(BF162Int32RNP2, asc_bfloat162int32_rn_v2);
ASCENDC_CAST_INST(BF162Int32RNSatP2, asc_bfloat162int32_rn_sat_v2);
ASCENDC_CAST_INST(BF162Int32RNA, asc_bfloat162int32_rna);
ASCENDC_CAST_INST(BF162Int32RNASat, asc_bfloat162int32_rna_sat);
ASCENDC_CAST_INST(BF162Int32RNAP2, asc_bfloat162int32_rna_v2);
ASCENDC_CAST_INST(BF162Int32RNASatP2, asc_bfloat162int32_rna_sat_v2);
ASCENDC_CAST_INST(BF162Int32RU, asc_bfloat162int32_ru);
ASCENDC_CAST_INST(BF162Int32RUSat, asc_bfloat162int32_ru_sat);
ASCENDC_CAST_INST(BF162Int32RUP2, asc_bfloat162int32_ru_v2);
ASCENDC_CAST_INST(BF162Int32RUSatP2, asc_bfloat162int32_ru_sat_v2);
ASCENDC_CAST_INST(BF162Int32RZ, asc_bfloat162int32_rz);
ASCENDC_CAST_INST(BF162Int32RZSat, asc_bfloat162int32_rz_sat);
ASCENDC_CAST_INST(BF162Int32RZP2, asc_bfloat162int32_rz_v2);
ASCENDC_CAST_INST(BF162Int32RZSatP2, asc_bfloat162int32_rz_sat_v2);

ASCENDC_CAST_INST(Int4x22BF16, asc_int4x22bfloat16);
ASCENDC_CAST_INST(Int4x22BF16P2, asc_int4x22bfloat16_v2);
ASCENDC_CAST_INST(Int4x22BF16P3, asc_int4x22bfloat16_v3);
ASCENDC_CAST_INST(Int4x22BF16P4, asc_int4x22bfloat16_v4);

#undef ASCENDC_CAST_INST

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_BFLOAT16_INT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
