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
    "impl/tensor_api/arch/vector/cast/cast_vf.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file cast_vf.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_CAST_VF_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_CAST_VF_H
#include "impl/tensor_api/arch/vector/cast/cast_enums.h"
#include "impl/tensor_api/arch/vector/cast/cast_trait_map.h"
#include "impl/tensor_api/arch/vector/cast/instruction.h"
#include "impl/tensor_api/arch/vector/utils/mask_utils.h"

namespace asc {
namespace te {

struct cast_width_src_eq_dst {}; // 1:1: float<->int32, half<->int16
struct cast_width_dst2x_src {};  // 1:2: half->float, int16->int32
struct cast_width_src2x_dst {};  // 2:1: float->half, float->int16
struct cast_width_dst4x_src {};  // 1:4: fp8->float, int8->int32
struct cast_width_src4x_dst {};  // 4:1: float->fp8, int32->uint8

template <typename WidthCategory>
class cast_vf_loop;

//   T = dst element type, U = src element type, sizeof(T) == sizeof(U)
template <>
class cast_vf_loop<cast_width_src_eq_dst> {
public:
    template <typename CalcFunc, typename T, typename U, typename DstRegType, typename SrcRegType>
    __simd_callee__ inline static void run(__ubuf__ T* dst, __ubuf__ U* src, uint16_t repeat, uint32_t one_rep_size,
                                           uint32_t data_size, DstRegType& dst_reg, SrcRegType& src_reg,
                                           vector_bool& dst_mask, vector_bool full_mask)
    {
        for (uint16_t i = 0; i < repeat; i++) {
            dst_mask = inst::update_mask::template run<uint32_t>(data_size);

            asc_loadalign(src_reg, src + i * one_rep_size);
            CalcFunc::template run(dst_reg, src_reg, full_mask);
            asc_storealign(dst + i * one_rep_size, dst_reg, dst_mask);
        }
    }
};

//   T = dst element type, U = src element type, sizeof(T) == 2 * sizeof(U)
template <>
class cast_vf_loop<cast_width_dst2x_src> {
public:
    template <typename CalcFunc, typename T, typename U, typename DstRegType, typename SrcRegType>
    __simd_callee__ inline static void run(__ubuf__ T* dst, __ubuf__ U* src, uint16_t repeat, uint32_t one_rep_size,
                                           uint32_t data_size, DstRegType& dst_reg, SrcRegType& src_reg,
                                           vector_bool& dst_mask, vector_bool full_mask)
    {
        for (uint16_t i = 0; i < repeat; i++) {
            dst_mask = inst::update_mask::template run<uint32_t>(data_size);

            asc_loadalign_unpack_postupdate(src_reg, src, one_rep_size);
            CalcFunc::template run(dst_reg, src_reg, full_mask);
            asc_storealign(dst + i * one_rep_size, dst_reg, dst_mask);
        }
    }
};

//   T = dst element type, U = src element type, sizeof(U) == 2 * sizeof(T)
template <>
class cast_vf_loop<cast_width_src2x_dst> {
public:
    template <typename CalcFunc, typename T, typename U, typename DstRegType, typename SrcRegType>
    __simd_callee__ inline static void run(__ubuf__ T* dst, __ubuf__ U* src, uint16_t repeat, uint32_t one_rep_size,
                                           uint32_t data_size, DstRegType& dst_reg, SrcRegType& src_reg,
                                           vector_bool& dst_mask, vector_bool full_mask)
    {
        auto dst_cur = reinterpret_cast<__ubuf__ U*>(dst);
        for (uint16_t i = 0; i < repeat; i++) {
            dst_mask = inst::update_mask::template run<uint32_t>(data_size);

            asc_loadalign(src_reg, src + i * one_rep_size);
            CalcFunc::template run(dst_reg, src_reg, full_mask);
            asc_storealign_pack_postupdate(dst_cur, *reinterpret_cast<SrcRegType*>(&dst_reg), one_rep_size / 2,
                                           dst_mask);
        }
    }
};

//   T = dst element type, U = src element type, sizeof(T) == 4 * sizeof(U)
template <>
class cast_vf_loop<cast_width_dst4x_src> {
public:
    template <typename CalcFunc, typename T, typename U, typename DstRegType, typename SrcRegType>
    __simd_callee__ inline static void run(__ubuf__ T* dst, __ubuf__ U* src, uint16_t repeat, uint32_t one_rep_size,
                                           uint32_t data_size, DstRegType& dst_reg, SrcRegType& src_reg,
                                           vector_bool& dst_mask, vector_bool full_mask)
    {
        for (uint16_t i = 0; i < repeat; i++) {
            dst_mask = inst::update_mask::template run<uint32_t>(data_size);

            asc_loadalign_unpack_postupdate_v2(src_reg, src, one_rep_size);
            CalcFunc::template run(dst_reg, src_reg, full_mask);
            asc_storealign(dst + i * one_rep_size, dst_reg, dst_mask);
        }
    }
};

//   T = dst element type, U = src element type, sizeof(U) == 4 * sizeof(T)
template <>
class cast_vf_loop<cast_width_src4x_dst> {
public:
    template <typename CalcFunc, typename T, typename U, typename DstRegType, typename SrcRegType>
    __simd_callee__ inline static void run(__ubuf__ T* dst, __ubuf__ U* src, uint16_t repeat, uint32_t one_rep_size,
                                           uint32_t data_size, DstRegType& dst_reg, SrcRegType& src_reg,
                                           vector_bool& dst_mask, vector_bool full_mask)
    {
        auto dst_cur = reinterpret_cast<__ubuf__ U*>(dst);
        for (uint16_t i = 0; i < repeat; i++) {
            dst_mask = inst::update_mask::template run<uint32_t>(data_size);

            asc_loadalign(src_reg, src + i * one_rep_size);
            CalcFunc::template run(dst_reg, src_reg, full_mask);
            asc_storealign_pack_postupdate_v2(dst_cur, *reinterpret_cast<SrcRegType*>(&dst_reg), one_rep_size / 4,
                                              dst_mask);
        }
    }
};

// ==================== cast_vf ====================
template <typename CalcFunc, typename TraitType, typename WidthCategory>
class cast_vf {
public:
    template <typename DstRegType, typename SrcRegType, typename T, typename U>
    __simd_vf__ inline static void run(__ubuf__ T* dst, __ubuf__ U* src, const uint32_t data_size)
    {
        using greater_type = Std::conditional_t<(sizeof(T) > sizeof(U)), T, U>;
        constexpr uint32_t one_rep_size = static_cast<uint32_t>(asc_get_vf_len() / sizeof(greater_type));
        uint16_t repeat = static_cast<uint16_t>((data_size + one_rep_size - 1) / one_rep_size);

        DstRegType dst_reg;
        SrcRegType src_reg;

        vector_bool dst_mask;
        vector_bool full_mask = inst::create_mask::template run<uint32_t, decltype(PAT_ALL)>();
        cast_vf_loop<WidthCategory>::template run<CalcFunc, T, U>(dst, src, repeat, one_rep_size, data_size, dst_reg,
                                                                  src_reg, dst_mask, full_mask);
    }
};

// ==================== transform2_cast_vf ====================
template <typename CalcFunc, typename TraitType>
class transform2_cast_vf {
public:
    template <typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src)
    {
        using dstType = GetAttributeElementType<typename T::elementType*>;
        using srcType = GetAttributeElementType<typename U::elementType*>;
        using DstRegType = typename VectorTypeTransform::template Get<dstType>;
        using SrcRegType = typename VectorTypeTransform::template Get<srcType>;

        const uint32_t dst_size = dst.size();

        constexpr bool src_eq_dst = (sizeof(dstType) == sizeof(srcType));     // 1:1: float<->int32, half<->int16
        constexpr bool dst_2x_src = (sizeof(dstType) == 2 * sizeof(srcType)); // 1:2: half->float, int16->int32
        constexpr bool src_2x_dst = (sizeof(srcType) == 2 * sizeof(dstType)); // 2:1: float->half, float->int16
        constexpr bool dst_4x_src = (sizeof(dstType) == 4 * sizeof(srcType)); // 1:4: fp8->float, int8->int32
        constexpr bool src_4x_dst = (sizeof(srcType) == 4 * sizeof(dstType)); // 4:1: float->fp8, int32->uint8

        // select width category tag based on size relationship
        using WidthCategory = Std::conditional_t<
            src_eq_dst, cast_width_src_eq_dst,
            Std::conditional_t<
                dst_2x_src, cast_width_dst2x_src,
                Std::conditional_t<src_2x_dst, cast_width_src2x_dst,
                                   Std::conditional_t<dst_4x_src, cast_width_dst4x_src, cast_width_src4x_dst>>>>;

        cast_vf<CalcFunc, TraitType, WidthCategory>::template run<DstRegType, SrcRegType>(dst.data().get(),
                                                                                          src.data().get(), dst_size);
    }
};

// Two-level lookup to avoid template recursion depth overflow.
// First level: cast_sub_map<SrcType, DstType> -> sub-map.  Second level: (RoundMode, SatMode, IndexPos) -> Inst.
struct cast_trait2_cast_func {
private:
    template <typename SrcType, typename DstType, typename RoundMode, typename SatMode, typename IndexPos>
    struct impl {
        using SubMap = typename cast_sub_map<SrcType, DstType>::type;
        using type = typename SubMap::template Get<Std::tuple<RoundMode, SatMode, IndexPos>>;
    };

public:
    template <typename SrcType, typename DstType, typename RoundMode, typename SatMode, typename IndexPos>
    using Lookup = typename impl<SrcType, DstType, RoundMode, SatMode, IndexPos>::type;
};

template <typename CalcFunc, typename TraitType>
class transform2_cast_inst_match {
public:
    template <typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src)
    {
        using srcType = GetAttributeElementType<typename U::elementType*>;
        using dstType = GetAttributeElementType<typename T::elementType*>;
        using trait = Std::conditional_t<Std::is_same_v<TraitType, Std::ignore_t>, cast_trait_default, TraitType>;

        using roundMode = Std::conditional_t<Std::is_same_v<srcType, dstType>, CalcFunc, typename trait::roundMode>;
        using satMode = Std::conditional_t<Std::is_same_v<typename trait::satMode, cast_sat_mode::no_sat>,
                                           Std::ignore_t, typename trait::satMode>;
        using indexPos = Std::conditional_t<(Std::is_same_v<typename trait::indexPos, cast_index_pos::even>
                                             || Std::is_same_v<typename trait::indexPos, cast_index_pos::part_p0>),
                                            Std::ignore_t, typename trait::indexPos>;

        using func = cast_trait2_cast_func::template Lookup<srcType, dstType, roundMode, satMode, indexPos>;
        static_assert(!Std::is_same_v<func, Std::ignore_t>,
                      "Unsupported cast trait: no matching entry found in cast_trait2_cast_func.");
        transform2_cast_vf<func, Std::ignore_t>::template run(dst, src);
    }
};
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_CAST_VF_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
