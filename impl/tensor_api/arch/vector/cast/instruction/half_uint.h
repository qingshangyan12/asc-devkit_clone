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

ASCENDC_CAST_INST(Half2UInt8RDNoSatEven, asc_half2uint8_rd);
ASCENDC_CAST_INST(Half2UInt8RDSatEven, asc_half2uint8_rd_sat);
ASCENDC_CAST_INST(Half2UInt8RDNoSatOdd, asc_half2uint8_rd_v2);
ASCENDC_CAST_INST(Half2UInt8RDSatOdd, asc_half2uint8_rd_sat_v2);
ASCENDC_CAST_INST(Half2UInt8RNNoSatEven, asc_half2uint8_rn);
ASCENDC_CAST_INST(Half2UInt8RNSatEven, asc_half2uint8_rn_sat);
ASCENDC_CAST_INST(Half2UInt8RNNoSatOdd, asc_half2uint8_rn_v2);
ASCENDC_CAST_INST(Half2UInt8RNSatOdd, asc_half2uint8_rn_sat_v2);
ASCENDC_CAST_INST(Half2UInt8RNANoSatEven, asc_half2uint8_rna);
ASCENDC_CAST_INST(Half2UInt8RNASatEven, asc_half2uint8_rna_sat);
ASCENDC_CAST_INST(Half2UInt8RNANoSatOdd, asc_half2uint8_rna_v2);
ASCENDC_CAST_INST(Half2UInt8RNASatOdd, asc_half2uint8_rna_sat_v2);
ASCENDC_CAST_INST(Half2UInt8RUNoSatEven, asc_half2uint8_ru);
ASCENDC_CAST_INST(Half2UInt8RUSatEven, asc_half2uint8_ru_sat);
ASCENDC_CAST_INST(Half2UInt8RUNoSatOdd, asc_half2uint8_ru_v2);
ASCENDC_CAST_INST(Half2UInt8RUSatOdd, asc_half2uint8_ru_sat_v2);
ASCENDC_CAST_INST(Half2UInt8RZNoSatEven, asc_half2uint8_rz);
ASCENDC_CAST_INST(Half2UInt8RZSatEven, asc_half2uint8_rz_sat);
ASCENDC_CAST_INST(Half2UInt8RZNoSatOdd, asc_half2uint8_rz_v2);
ASCENDC_CAST_INST(Half2UInt8RZSatOdd, asc_half2uint8_rz_sat_v2);

ASCENDC_CAST_INST(U82HalfNoSatEven, asc_uint82half);
ASCENDC_CAST_INST(U82HalfNoSatOdd, asc_uint82half_v2);


// --- Cast trait sub-maps ---
using CastSub_half_to_uint8_t = TupleMap<
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, Std::ignore_t>, Inst::Half2UInt8RDNoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RD, CastSatMode::Sat, Std::ignore_t>, Inst::Half2UInt8RDSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RD, Std::ignore_t, CastIndexPos::Odd>, Inst::Half2UInt8RDNoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RD, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Half2UInt8RDSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, Std::ignore_t>, Inst::Half2UInt8RNNoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RN, CastSatMode::Sat, Std::ignore_t>, Inst::Half2UInt8RNSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RN, Std::ignore_t, CastIndexPos::Odd>, Inst::Half2UInt8RNNoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RN, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Half2UInt8RNSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, Std::ignore_t>, Inst::Half2UInt8RNANoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, CastSatMode::Sat, Std::ignore_t>, Inst::Half2UInt8RNASatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, Std::ignore_t, CastIndexPos::Odd>, Inst::Half2UInt8RNANoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RNA, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Half2UInt8RNASatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, Std::ignore_t>, Inst::Half2UInt8RUNoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RU, CastSatMode::Sat, Std::ignore_t>, Inst::Half2UInt8RUSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RU, Std::ignore_t, CastIndexPos::Odd>, Inst::Half2UInt8RUNoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RU, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Half2UInt8RUSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, Std::ignore_t>, Inst::Half2UInt8RZNoSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, CastSatMode::Sat, Std::ignore_t>, Inst::Half2UInt8RZSatEven>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, Std::ignore_t, CastIndexPos::Odd>, Inst::Half2UInt8RZNoSatOdd>,
    Std::tuple<Std::tuple<CastRoundMode::RZ, CastSatMode::Sat, CastIndexPos::Odd>, Inst::Half2UInt8RZSatOdd>
>;

using CastSub_uint8_t_to_half = TupleMap<
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, Std::ignore_t>, Inst::U82HalfNoSatEven>,
    Std::tuple<Std::tuple<Std::ignore_t, Std::ignore_t, CastIndexPos::Odd>, Inst::U82HalfNoSatOdd>
>;

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_HALF_UINT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
