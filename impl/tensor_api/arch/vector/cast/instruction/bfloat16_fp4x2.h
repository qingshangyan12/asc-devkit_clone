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
    "impl/tensor_api/arch/vector/cast/instruction/bfloat16_fp4x2.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file bfloat16_fp4x2.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_BFLOAT16_FP4X2_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_BFLOAT16_FP4X2_H

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

ASCENDC_CAST_INST(BF162E1M2RD, asc_bfloat162e1m2x2_rd);
ASCENDC_CAST_INST(BF162E1M2RDP2, asc_bfloat162e1m2x2_rd_v2);
ASCENDC_CAST_INST(BF162E1M2RDP3, asc_bfloat162e1m2x2_rd_v3);
ASCENDC_CAST_INST(BF162E1M2RDP4, asc_bfloat162e1m2x2_rd_v4);
ASCENDC_CAST_INST(BF162E1M2RN, asc_bfloat162e1m2x2_rn);
ASCENDC_CAST_INST(BF162E1M2RNP2, asc_bfloat162e1m2x2_rn_v2);
ASCENDC_CAST_INST(BF162E1M2RNP3, asc_bfloat162e1m2x2_rn_v3);
ASCENDC_CAST_INST(BF162E1M2RNP4, asc_bfloat162e1m2x2_rn_v4);
ASCENDC_CAST_INST(BF162E1M2RNA, asc_bfloat162e1m2x2_rna);
ASCENDC_CAST_INST(BF162E1M2RNAP2, asc_bfloat162e1m2x2_rna_v2);
ASCENDC_CAST_INST(BF162E1M2RNAP3, asc_bfloat162e1m2x2_rna_v3);
ASCENDC_CAST_INST(BF162E1M2RNAP4, asc_bfloat162e1m2x2_rna_v4);
ASCENDC_CAST_INST(BF162E1M2RU, asc_bfloat162e1m2x2_ru);
ASCENDC_CAST_INST(BF162E1M2RUP2, asc_bfloat162e1m2x2_ru_v2);
ASCENDC_CAST_INST(BF162E1M2RUP3, asc_bfloat162e1m2x2_ru_v3);
ASCENDC_CAST_INST(BF162E1M2RUP4, asc_bfloat162e1m2x2_ru_v4);
ASCENDC_CAST_INST(BF162E1M2RZ, asc_bfloat162e1m2x2_rz);
ASCENDC_CAST_INST(BF162E1M2RZP2, asc_bfloat162e1m2x2_rz_v2);
ASCENDC_CAST_INST(BF162E1M2RZP3, asc_bfloat162e1m2x2_rz_v3);
ASCENDC_CAST_INST(BF162E1M2RZP4, asc_bfloat162e1m2x2_rz_v4);

ASCENDC_CAST_INST(E1M22BF16, asc_e1m2x22bfloat16);
ASCENDC_CAST_INST(E1M22BF16P2, asc_e1m2x22bfloat16_v2);
ASCENDC_CAST_INST(E1M22BF16P3, asc_e1m2x22bfloat16_v3);
ASCENDC_CAST_INST(E1M22BF16P4, asc_e1m2x22bfloat16_v4);

ASCENDC_CAST_INST(BF162E2M1RD, asc_bfloat162e2m1x2_rd);
ASCENDC_CAST_INST(BF162E2M1RDP2, asc_bfloat162e2m1x2_rd_v2);
ASCENDC_CAST_INST(BF162E2M1RDP3, asc_bfloat162e2m1x2_rd_v3);
ASCENDC_CAST_INST(BF162E2M1RDP4, asc_bfloat162e2m1x2_rd_v4);
ASCENDC_CAST_INST(BF162E2M1RN, asc_bfloat162e2m1x2_rn);
ASCENDC_CAST_INST(BF162E2M1RNP2, asc_bfloat162e2m1x2_rn_v2);
ASCENDC_CAST_INST(BF162E2M1RNP3, asc_bfloat162e2m1x2_rn_v3);
ASCENDC_CAST_INST(BF162E2M1RNP4, asc_bfloat162e2m1x2_rn_v4);
ASCENDC_CAST_INST(BF162E2M1RNA, asc_bfloat162e2m1x2_rna);
ASCENDC_CAST_INST(BF162E2M1RNAP2, asc_bfloat162e2m1x2_rna_v2);
ASCENDC_CAST_INST(BF162E2M1RNAP3, asc_bfloat162e2m1x2_rna_v3);
ASCENDC_CAST_INST(BF162E2M1RNAP4, asc_bfloat162e2m1x2_rna_v4);
ASCENDC_CAST_INST(BF162E2M1RU, asc_bfloat162e2m1x2_ru);
ASCENDC_CAST_INST(BF162E2M1RUP2, asc_bfloat162e2m1x2_ru_v2);
ASCENDC_CAST_INST(BF162E2M1RUP3, asc_bfloat162e2m1x2_ru_v3);
ASCENDC_CAST_INST(BF162E2M1RUP4, asc_bfloat162e2m1x2_ru_v4);
ASCENDC_CAST_INST(BF162E2M1RZ, asc_bfloat162e2m1x2_rz);
ASCENDC_CAST_INST(BF162E2M1RZP2, asc_bfloat162e2m1x2_rz_v2);
ASCENDC_CAST_INST(BF162E2M1RZP3, asc_bfloat162e2m1x2_rz_v3);
ASCENDC_CAST_INST(BF162E2M1RZP4, asc_bfloat162e2m1x2_rz_v4);

ASCENDC_CAST_INST(E2M12BF16, asc_e2m1x22bfloat16);
ASCENDC_CAST_INST(E2M12BF16P2, asc_e2m1x22bfloat16_v2);
ASCENDC_CAST_INST(E2M12BF16P3, asc_e2m1x22bfloat16_v3);
ASCENDC_CAST_INST(E2M12BF16P4, asc_e2m1x22bfloat16_v4);

#undef ASCENDC_CAST_INST

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_BFLOAT16_FP4X2_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
