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
    "impl/tensor_api/arch/vector/cast/cast_enums.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file cast_enums.h
 * \brief cast trait enums — rounding mode, saturation mode, index position, and default trait.
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_CAST_ENUMS_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_CAST_ENUMS_H

namespace asc {
namespace te {

namespace cast_round_mode {
struct rd {};
struct rn {};
struct rna {};
struct rh {};
struct ro {};
struct ru {};
struct rz {};
using RD = rd;
using RN = rn;
using RNA = rna;
using RH = rh;
using RO = ro;
using RU = ru;
using RZ = rz;
} // namespace cast_round_mode
namespace CastRoundMode = cast_round_mode;

namespace cast_sat_mode {
struct sat {};
struct no_sat {};
using Sat = sat;
using NoSat = no_sat;
} // namespace cast_sat_mode
namespace CastSatMode = cast_sat_mode;

namespace cast_index_pos {
struct even {};
struct odd {};
struct part_p0 {};
struct part_p1 {};
struct part_p2 {};
struct part_p3 {};
using Even = even;
using Odd = odd;
using PartP0 = part_p0;
using PartP1 = part_p1;
using PartP2 = part_p2;
using PartP3 = part_p3;
} // namespace cast_index_pos
namespace CastIndexPos = cast_index_pos;

struct cast_trait_default {
    using roundMode = Std::ignore_t;
    using satMode = Std::ignore_t;
    using indexPos = Std::ignore_t;
};

using CastTraitDefault = cast_trait_default;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_CAST_ENUMS_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
