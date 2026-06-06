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

ASCENDC_CAST_INST(BF162E1M2RDP0, asc_bfloat162e1m2x2_rd);
ASCENDC_CAST_INST(BF162E1M2RDP1, asc_bfloat162e1m2x2_rd_v2);
ASCENDC_CAST_INST(BF162E1M2RDP2, asc_bfloat162e1m2x2_rd_v3);
ASCENDC_CAST_INST(BF162E1M2RDP3, asc_bfloat162e1m2x2_rd_v4);
ASCENDC_CAST_INST(BF162E1M2RNP0, asc_bfloat162e1m2x2_rn);
ASCENDC_CAST_INST(BF162E1M2RNP1, asc_bfloat162e1m2x2_rn_v2);
ASCENDC_CAST_INST(BF162E1M2RNP2, asc_bfloat162e1m2x2_rn_v3);
ASCENDC_CAST_INST(BF162E1M2RNP3, asc_bfloat162e1m2x2_rn_v4);
ASCENDC_CAST_INST(BF162E1M2RNAP0, asc_bfloat162e1m2x2_rna);
ASCENDC_CAST_INST(BF162E1M2RNAP1, asc_bfloat162e1m2x2_rna_v2);
ASCENDC_CAST_INST(BF162E1M2RNAP2, asc_bfloat162e1m2x2_rna_v3);
ASCENDC_CAST_INST(BF162E1M2RNAP3, asc_bfloat162e1m2x2_rna_v4);
ASCENDC_CAST_INST(BF162E1M2RUP0, asc_bfloat162e1m2x2_ru);
ASCENDC_CAST_INST(BF162E1M2RUP1, asc_bfloat162e1m2x2_ru_v2);
ASCENDC_CAST_INST(BF162E1M2RUP2, asc_bfloat162e1m2x2_ru_v3);
ASCENDC_CAST_INST(BF162E1M2RUP3, asc_bfloat162e1m2x2_ru_v4);
ASCENDC_CAST_INST(BF162E1M2RZP0, asc_bfloat162e1m2x2_rz);
ASCENDC_CAST_INST(BF162E1M2RZP1, asc_bfloat162e1m2x2_rz_v2);
ASCENDC_CAST_INST(BF162E1M2RZP2, asc_bfloat162e1m2x2_rz_v3);
ASCENDC_CAST_INST(BF162E1M2RZP3, asc_bfloat162e1m2x2_rz_v4);

ASCENDC_CAST_INST(E1M22BF16P0, asc_e1m2x22bfloat16);
ASCENDC_CAST_INST(E1M22BF16P1, asc_e1m2x22bfloat16_v2);
ASCENDC_CAST_INST(E1M22BF16P2, asc_e1m2x22bfloat16_v3);
ASCENDC_CAST_INST(E1M22BF16P3, asc_e1m2x22bfloat16_v4);

ASCENDC_CAST_INST(BF162E2M1RDP0, asc_bfloat162e2m1x2_rd);
ASCENDC_CAST_INST(BF162E2M1RDP1, asc_bfloat162e2m1x2_rd_v2);
ASCENDC_CAST_INST(BF162E2M1RDP2, asc_bfloat162e2m1x2_rd_v3);
ASCENDC_CAST_INST(BF162E2M1RDP3, asc_bfloat162e2m1x2_rd_v4);
ASCENDC_CAST_INST(BF162E2M1RNP0, asc_bfloat162e2m1x2_rn);
ASCENDC_CAST_INST(BF162E2M1RNP1, asc_bfloat162e2m1x2_rn_v2);
ASCENDC_CAST_INST(BF162E2M1RNP2, asc_bfloat162e2m1x2_rn_v3);
ASCENDC_CAST_INST(BF162E2M1RNP3, asc_bfloat162e2m1x2_rn_v4);
ASCENDC_CAST_INST(BF162E2M1RNAP0, asc_bfloat162e2m1x2_rna);
ASCENDC_CAST_INST(BF162E2M1RNAP1, asc_bfloat162e2m1x2_rna_v2);
ASCENDC_CAST_INST(BF162E2M1RNAP2, asc_bfloat162e2m1x2_rna_v3);
ASCENDC_CAST_INST(BF162E2M1RNAP3, asc_bfloat162e2m1x2_rna_v4);
ASCENDC_CAST_INST(BF162E2M1RUP0, asc_bfloat162e2m1x2_ru);
ASCENDC_CAST_INST(BF162E2M1RUP1, asc_bfloat162e2m1x2_ru_v2);
ASCENDC_CAST_INST(BF162E2M1RUP2, asc_bfloat162e2m1x2_ru_v3);
ASCENDC_CAST_INST(BF162E2M1RUP3, asc_bfloat162e2m1x2_ru_v4);
ASCENDC_CAST_INST(BF162E2M1RZP0, asc_bfloat162e2m1x2_rz);
ASCENDC_CAST_INST(BF162E2M1RZP1, asc_bfloat162e2m1x2_rz_v2);
ASCENDC_CAST_INST(BF162E2M1RZP2, asc_bfloat162e2m1x2_rz_v3);
ASCENDC_CAST_INST(BF162E2M1RZP3, asc_bfloat162e2m1x2_rz_v4);

ASCENDC_CAST_INST(E2M12BF16P0, asc_e2m1x22bfloat16);
ASCENDC_CAST_INST(E2M12BF16P1, asc_e2m1x22bfloat16_v2);
ASCENDC_CAST_INST(E2M12BF16P2, asc_e2m1x22bfloat16_v3);
ASCENDC_CAST_INST(E2M12BF16P3, asc_e2m1x22bfloat16_v4);


// --- Cast trait sub-maps ---
using CastSub_bfloat16_t_to_fp4x2_e1m2_t = TupleMap<
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, Std::ignore_t>, Inst::BF162E1M2RDP0>,
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, CastIndexPos::PartP1>, Inst::BF162E1M2RDP1>,
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, CastIndexPos::PartP2>, Inst::BF162E1M2RDP2>,
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, CastIndexPos::PartP3>, Inst::BF162E1M2RDP3>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, Std::ignore_t>, Inst::BF162E1M2RNP0>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, CastIndexPos::PartP1>, Inst::BF162E1M2RNP1>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, CastIndexPos::PartP2>, Inst::BF162E1M2RNP2>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, CastIndexPos::PartP3>, Inst::BF162E1M2RNP3>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, Std::ignore_t>, Inst::BF162E1M2RNAP0>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, CastIndexPos::PartP1>, Inst::BF162E1M2RNAP1>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, CastIndexPos::PartP2>, Inst::BF162E1M2RNAP2>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, CastIndexPos::PartP3>, Inst::BF162E1M2RNAP3>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, Std::ignore_t>, Inst::BF162E1M2RUP0>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, CastIndexPos::PartP1>, Inst::BF162E1M2RUP1>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, CastIndexPos::PartP2>, Inst::BF162E1M2RUP2>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, CastIndexPos::PartP3>, Inst::BF162E1M2RUP3>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, Std::ignore_t>, Inst::BF162E1M2RZP0>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, CastIndexPos::PartP1>, Inst::BF162E1M2RZP1>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, CastIndexPos::PartP2>, Inst::BF162E1M2RZP2>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, CastIndexPos::PartP3>, Inst::BF162E1M2RZP3>
>;

using CastSub_bfloat16_t_to_fp4x2_e2m1_t = TupleMap<
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, Std::ignore_t>, Inst::BF162E2M1RDP0>,
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, CastIndexPos::PartP1>, Inst::BF162E2M1RDP1>,
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, CastIndexPos::PartP2>, Inst::BF162E2M1RDP2>,
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, CastIndexPos::PartP3>, Inst::BF162E2M1RDP3>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, Std::ignore_t>, Inst::BF162E2M1RNP0>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, CastIndexPos::PartP1>, Inst::BF162E2M1RNP1>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, CastIndexPos::PartP2>, Inst::BF162E2M1RNP2>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, CastIndexPos::PartP3>, Inst::BF162E2M1RNP3>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, Std::ignore_t>, Inst::BF162E2M1RNAP0>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, CastIndexPos::PartP1>, Inst::BF162E2M1RNAP1>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, CastIndexPos::PartP2>, Inst::BF162E2M1RNAP2>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, CastIndexPos::PartP3>, Inst::BF162E2M1RNAP3>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, Std::ignore_t>, Inst::BF162E2M1RUP0>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, CastIndexPos::PartP1>, Inst::BF162E2M1RUP1>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, CastIndexPos::PartP2>, Inst::BF162E2M1RUP2>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, CastIndexPos::PartP3>, Inst::BF162E2M1RUP3>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, Std::ignore_t>, Inst::BF162E2M1RZP0>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, CastIndexPos::PartP1>, Inst::BF162E2M1RZP1>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, CastIndexPos::PartP2>, Inst::BF162E2M1RZP2>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, CastIndexPos::PartP3>, Inst::BF162E2M1RZP3>
>;

using CastSub_fp4x2_e1m2_t_to_bfloat16_t = TupleMap<
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, Std::ignore_t>, Inst::E1M22BF16P0>,
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, CastIndexPos::PartP1>, Inst::E1M22BF16P1>,
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, CastIndexPos::PartP2>, Inst::E1M22BF16P2>,
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, CastIndexPos::PartP3>, Inst::E1M22BF16P3>
>;

using CastSub_fp4x2_e2m1_t_to_bfloat16_t = TupleMap<
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, Std::ignore_t>, Inst::E2M12BF16P0>,
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, CastIndexPos::PartP1>, Inst::E2M12BF16P1>,
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, CastIndexPos::PartP2>, Inst::E2M12BF16P2>,
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, CastIndexPos::PartP3>, Inst::E2M12BF16P3>
>;

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_BFLOAT16_FP4X2_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
