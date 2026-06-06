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
    "impl/tensor_api/arch/vector/cast/instruction/float_half.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file float_half.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_HALF_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_HALF_H

ASCENDC_CAST_INST(Float2HalfRDNoSatEven, asc_float2half_rd);
ASCENDC_CAST_INST(Float2HalfRDSatEven, asc_float2half_rd_sat);
ASCENDC_CAST_INST(Float2HalfRDNoSatOdd, asc_float2half_rd_v2);
ASCENDC_CAST_INST(Float2HalfRDSatOdd, asc_float2half_rd_sat_v2);
ASCENDC_CAST_INST(Float2HalfRNNoSatEven, asc_float2half_rn);
ASCENDC_CAST_INST(Float2HalfRNSatEven, asc_float2half_rn_sat);
ASCENDC_CAST_INST(Float2HalfRNNoSatOdd, asc_float2half_rn_v2);
ASCENDC_CAST_INST(Float2HalfRNSatOdd, asc_float2half_rn_sat_v2);
ASCENDC_CAST_INST(Float2HalfRNANoSatEven, asc_float2half_rna);
ASCENDC_CAST_INST(Float2HalfRNASatEven, asc_float2half_rna_sat);
ASCENDC_CAST_INST(Float2HalfRNANoSatOdd, asc_float2half_rna_v2);
ASCENDC_CAST_INST(Float2HalfRNASatOdd, asc_float2half_rna_sat_v2);
ASCENDC_CAST_INST(Float2HalfRONoSatEven, asc_float2half_ro);
ASCENDC_CAST_INST(Float2HalfROSatEven, asc_float2half_ro_sat);
ASCENDC_CAST_INST(Float2HalfRONoSatOdd, asc_float2half_ro_v2);
ASCENDC_CAST_INST(Float2HalfROSatOdd, asc_float2half_ro_sat_v2);
ASCENDC_CAST_INST(Float2HalfRUNoSatEven, asc_float2half_ru);
ASCENDC_CAST_INST(Float2HalfRUSatEven, asc_float2half_ru_sat);
ASCENDC_CAST_INST(Float2HalfRUNoSatOdd, asc_float2half_ru_v2);
ASCENDC_CAST_INST(Float2HalfRUSatOdd, asc_float2half_ru_sat_v2);
ASCENDC_CAST_INST(Float2HalfRZNoSatEven, asc_float2half_rz);
ASCENDC_CAST_INST(Float2HalfRZSatEven, asc_float2half_rz_sat);
ASCENDC_CAST_INST(Float2HalfRZNoSatOdd, asc_float2half_rz_v2);
ASCENDC_CAST_INST(Float2HalfRZSatOdd, asc_float2half_rz_sat_v2);

ASCENDC_CAST_INST(Half2FloatEven, asc_half2float);
ASCENDC_CAST_INST(Half2FloatOdd, asc_half2float_v2);


// --- Cast trait sub-maps ---
using CastSub_float_to_half = TupleMap<
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, Std::ignore_t>, Inst::Float2HalfRDNoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RD, CastSatMode::Sat, Std::ignore_t>, Inst::Float2HalfRDSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, CastIndexPos::Odd>, Inst::Float2HalfRDNoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RD, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Float2HalfRDSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, Std::ignore_t>, Inst::Float2HalfRNNoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RN, CastSatMode::Sat, Std::ignore_t>, Inst::Float2HalfRNSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, CastIndexPos::Odd>, Inst::Float2HalfRNNoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RN, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Float2HalfRNSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, Std::ignore_t>, Inst::Float2HalfRNANoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, CastSatMode::Sat, Std::ignore_t>, Inst::Float2HalfRNASatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, CastIndexPos::Odd>, Inst::Float2HalfRNANoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Float2HalfRNASatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RO, Std::ignore_t, Std::ignore_t>, Inst::Float2HalfRONoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RO, CastSatMode::Sat, Std::ignore_t>, Inst::Float2HalfROSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RO, Std::ignore_t, CastIndexPos::Odd>, Inst::Float2HalfRONoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RO, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Float2HalfROSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, Std::ignore_t>, Inst::Float2HalfRUNoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RU, CastSatMode::Sat, Std::ignore_t>, Inst::Float2HalfRUSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, CastIndexPos::Odd>, Inst::Float2HalfRUNoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RU, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Float2HalfRUSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, Std::ignore_t>, Inst::Float2HalfRZNoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, CastSatMode::Sat, Std::ignore_t>, Inst::Float2HalfRZSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, CastIndexPos::Odd>, Inst::Float2HalfRZNoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Float2HalfRZSatOdd>
>;

using CastSub_half_to_float = TupleMap<
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, Std::ignore_t>, Inst::Half2FloatEven>,
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, CastIndexPos::Odd>, Inst::Half2FloatOdd>
>;

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_FLOAT_HALF_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
