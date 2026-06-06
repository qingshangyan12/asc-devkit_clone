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
    "impl/tensor_api/arch/vector/cast/cast_trait_map.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file cast_trait_map.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_CAST_TRAIT_MAP_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_CAST_TRAIT_MAP_H

#include "impl/tensor_api/arch/vector/cast/instruction.h"

namespace AscendC {
namespace Te {

namespace CastRoundMode {
struct RD {};
struct RN {};
struct RNA {};
struct RH {};
struct RO {};
struct RU {};
struct RZ {};
}

namespace CastSatMode {
struct Sat {};
struct NoSat {};
}

namespace CastIndexPos {
struct Even {};
struct Odd {};
struct PartP0 {};
struct PartP1 {};
struct PartP2 {};
struct PartP3 {};
}

struct CastTraitDefault {
    using roundMode = Std::ignore_t;
    using satMode = Std::ignore_t;
    using indexPos = Std::ignore_t;
};


using CastBucketMap = TupleMap<
    Std::tuple<Std::tuple<bfloat16_t, bfloat16_t>, CastSub_bfloat16_t_to_bfloat16_t>,
    Std::tuple<Std::tuple<bfloat16_t, float>, CastSub_bfloat16_t_to_float>,
    Std::tuple<Std::tuple<bfloat16_t, fp4x2_e1m2_t>, CastSub_bfloat16_t_to_fp4x2_e1m2_t>,
    Std::tuple<Std::tuple<bfloat16_t, fp4x2_e2m1_t>, CastSub_bfloat16_t_to_fp4x2_e2m1_t>,
    Std::tuple<Std::tuple<bfloat16_t, half>, CastSub_bfloat16_t_to_half>,
    Std::tuple<Std::tuple<bfloat16_t, int32_t>, CastSub_bfloat16_t_to_int32_t>,
    Std::tuple<Std::tuple<float, bfloat16_t>, CastSub_float_to_bfloat16_t>,
    Std::tuple<Std::tuple<float, float>, CastSub_float_to_float>,
    Std::tuple<Std::tuple<float, fp8_e4m3fn_t>, CastSub_float_to_fp8_e4m3fn_t>,
    Std::tuple<Std::tuple<float, fp8_e5m2_t>, CastSub_float_to_fp8_e5m2_t>,
    Std::tuple<Std::tuple<float, half>, CastSub_float_to_half>,
    Std::tuple<Std::tuple<float, hifloat8_t>, CastSub_float_to_hifloat8_t>,
    Std::tuple<Std::tuple<float, int16_t>, CastSub_float_to_int16_t>,
    Std::tuple<Std::tuple<float, int32_t>, CastSub_float_to_int32_t>,
    Std::tuple<Std::tuple<float, int64_t>, CastSub_float_to_int64_t>,
    Std::tuple<Std::tuple<fp4x2_e1m2_t, bfloat16_t>, CastSub_fp4x2_e1m2_t_to_bfloat16_t>,
    Std::tuple<Std::tuple<fp4x2_e2m1_t, bfloat16_t>, CastSub_fp4x2_e2m1_t_to_bfloat16_t>,
    Std::tuple<Std::tuple<fp8_e4m3fn_t, float>, CastSub_fp8_e4m3fn_t_to_float>,
    Std::tuple<Std::tuple<fp8_e5m2_t, float>, CastSub_fp8_e5m2_t_to_float>,
    Std::tuple<Std::tuple<half, bfloat16_t>, CastSub_half_to_bfloat16_t>,
    Std::tuple<Std::tuple<half, float>, CastSub_half_to_float>,
    Std::tuple<Std::tuple<half, half>, CastSub_half_to_half>,
    Std::tuple<Std::tuple<half, hifloat8_t>, CastSub_half_to_hifloat8_t>,
    Std::tuple<Std::tuple<half, int16_t>, CastSub_half_to_int16_t>,
    Std::tuple<Std::tuple<half, int32_t>, CastSub_half_to_int32_t>,
    Std::tuple<Std::tuple<half, int4x2_t>, CastSub_half_to_int4x2_t>,
    Std::tuple<Std::tuple<half, int8_t>, CastSub_half_to_int8_t>,
    Std::tuple<Std::tuple<half, uint8_t>, CastSub_half_to_uint8_t>,
    Std::tuple<Std::tuple<hifloat8_t, float>, CastSub_hifloat8_t_to_float>,
    Std::tuple<Std::tuple<hifloat8_t, half>, CastSub_hifloat8_t_to_half>,
    Std::tuple<Std::tuple<int16_t, float>, CastSub_int16_t_to_float>,
    Std::tuple<Std::tuple<int16_t, half>, CastSub_int16_t_to_half>,
    Std::tuple<Std::tuple<int16_t, int32_t>, CastSub_int16_t_to_int32_t>,
    Std::tuple<Std::tuple<int16_t, uint32_t>, CastSub_int16_t_to_uint32_t>,
    Std::tuple<Std::tuple<int16_t, uint8_t>, CastSub_int16_t_to_uint8_t>,
    Std::tuple<Std::tuple<int32_t, float>, CastSub_int32_t_to_float>,
    Std::tuple<Std::tuple<int32_t, int16_t>, CastSub_int32_t_to_int16_t>,
    Std::tuple<Std::tuple<int32_t, int64_t>, CastSub_int32_t_to_int64_t>,
    Std::tuple<Std::tuple<int32_t, uint16_t>, CastSub_int32_t_to_uint16_t>,
    Std::tuple<Std::tuple<int32_t, uint8_t>, CastSub_int32_t_to_uint8_t>,
    Std::tuple<Std::tuple<int4x2_t, bfloat16_t>, CastSub_int4x2_t_to_bfloat16_t>,
    Std::tuple<Std::tuple<int4x2_t, half>, CastSub_int4x2_t_to_half>,
    Std::tuple<Std::tuple<int4x2_t, int16_t>, CastSub_int4x2_t_to_int16_t>,
    Std::tuple<Std::tuple<int64_t, float>, CastSub_int64_t_to_float>,
    Std::tuple<Std::tuple<int64_t, int32_t>, CastSub_int64_t_to_int32_t>,
    Std::tuple<Std::tuple<int8_t, half>, CastSub_int8_t_to_half>,
    Std::tuple<Std::tuple<int8_t, int16_t>, CastSub_int8_t_to_int16_t>,
    Std::tuple<Std::tuple<int8_t, int32_t>, CastSub_int8_t_to_int32_t>,
    Std::tuple<Std::tuple<uint16_t, uint32_t>, CastSub_uint16_t_to_uint32_t>,
    Std::tuple<Std::tuple<uint16_t, uint8_t>, CastSub_uint16_t_to_uint8_t>,
    Std::tuple<Std::tuple<uint32_t, int16_t>, CastSub_uint32_t_to_int16_t>,
    Std::tuple<Std::tuple<uint32_t, uint16_t>, CastSub_uint32_t_to_uint16_t>,
    Std::tuple<Std::tuple<uint32_t, uint8_t>, CastSub_uint32_t_to_uint8_t>,
    Std::tuple<Std::tuple<uint8_t, half>, CastSub_uint8_t_to_half>,
    Std::tuple<Std::tuple<uint8_t, uint16_t>, CastSub_uint8_t_to_uint16_t>,
    Std::tuple<Std::tuple<uint8_t, uint32_t>, CastSub_uint8_t_to_uint32_t>
>;

// Two-level lookup to avoid template recursion depth overflow.
// First level: (SrcType, DstType) -> sub-map.  Second level: (RoundMode, SatMode, IndexPos) -> Inst.
struct CastTrait2CastFunc {
private:
    template <typename SrcType, typename DstType, typename RoundMode, typename SatMode, typename IndexPos>
    struct Impl {
        using SubMap = typename CastBucketMap::template Get<Std::tuple<SrcType, DstType>>;
        using type = typename SubMap::template Get<Std::tuple<RoundMode, SatMode, IndexPos>>;
    };
public:
    template <typename SrcType, typename DstType, typename RoundMode, typename SatMode, typename IndexPos>
    using Lookup = typename Impl<SrcType, DstType, RoundMode, SatMode, IndexPos>::type;
};


} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_CAST_TRAIT_MAP_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
