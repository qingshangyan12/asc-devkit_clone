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
    "impl/tensor_api/arch/vector/cast/instruction/rounding_ops.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file rounding_ops.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_ROUNDING_OPS_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_ROUNDING_OPS_H

ASCENDC_ROUNDING_INST(Rint, asc_rint);
ASCENDC_ROUNDING_INST(Round, asc_round);
ASCENDC_ROUNDING_INST(Floor, asc_floor);
ASCENDC_ROUNDING_INST(Ceil, asc_ceil);
ASCENDC_ROUNDING_INST(Trunc, asc_trunc);


// --- Cast trait sub-maps ---
using CastSub_bfloat16_t_to_bfloat16_t = TupleMap<
    Std::tuple<Std::tuple<Inst::Rint, Std::ignore_t, Std::ignore_t>, Inst::Rint>,
    Std::tuple<Std::tuple<Inst::Round, Std::ignore_t, Std::ignore_t>, Inst::Round>,
    Std::tuple<Std::tuple<Inst::Floor, Std::ignore_t, Std::ignore_t>, Inst::Floor>,
    Std::tuple<Std::tuple<Inst::Ceil, Std::ignore_t, Std::ignore_t>, Inst::Ceil>,
    Std::tuple<Std::tuple<Inst::Trunc, Std::ignore_t, Std::ignore_t>, Inst::Trunc>
>;

using CastSub_float_to_float = TupleMap<
    Std::tuple<Std::tuple<Inst::Rint, Std::ignore_t, Std::ignore_t>, Inst::Rint>,
    Std::tuple<Std::tuple<Inst::Round, Std::ignore_t, Std::ignore_t>, Inst::Round>,
    Std::tuple<Std::tuple<Inst::Floor, Std::ignore_t, Std::ignore_t>, Inst::Floor>,
    Std::tuple<Std::tuple<Inst::Ceil, Std::ignore_t, Std::ignore_t>, Inst::Ceil>,
    Std::tuple<Std::tuple<Inst::Trunc, Std::ignore_t, Std::ignore_t>, Inst::Trunc>
>;

using CastSub_half_to_half = TupleMap<
    Std::tuple<Std::tuple<Inst::Rint, Std::ignore_t, Std::ignore_t>, Inst::Rint>,
    Std::tuple<Std::tuple<Inst::Round, Std::ignore_t, Std::ignore_t>, Inst::Round>,
    Std::tuple<Std::tuple<Inst::Floor, Std::ignore_t, Std::ignore_t>, Inst::Floor>,
    Std::tuple<Std::tuple<Inst::Ceil, Std::ignore_t, Std::ignore_t>, Inst::Ceil>,
    Std::tuple<Std::tuple<Inst::Trunc, Std::ignore_t, Std::ignore_t>, Inst::Trunc>
>;

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_ROUNDING_OPS_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
