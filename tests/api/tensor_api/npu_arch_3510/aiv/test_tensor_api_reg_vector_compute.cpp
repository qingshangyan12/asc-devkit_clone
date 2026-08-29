/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

#include "tensor_api/stub/cce_stub.h"
#include "tensor_api/experimental/vector_compute.h"
#include "tensor_api/tensor.h"

namespace {

template <typename T>
using reg_type = asc::te::experimental::reg_tensor<T>;

template <typename T>
__simd_vf__ inline void compile_log(reg_type<T> src)
{
    auto dst = asc::te::experimental::log(src);
    static_assert(AscendC::Std::is_same_v<decltype(dst), reg_type<T>>);
}

template <typename T>
__simd_vf__ inline void compile_add(reg_type<T> src0, reg_type<T> src1, T scalar)
{
    auto register_result = src0 + src1;
    auto right_scalar_result = src0 + scalar;
    auto left_scalar_result = scalar + src0;
    static_assert(AscendC::Std::is_same_v<decltype(register_result), reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<decltype(right_scalar_result), reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<decltype(left_scalar_result), reg_type<T>>);
}

template <typename T>
__simd_vf__ inline void compile_sub(reg_type<T> src0, reg_type<T> src1, T scalar)
{
    auto register_result = src0 - src1;
    auto right_scalar_result = src0 - scalar;
    auto left_scalar_result = scalar - src0;
    static_assert(AscendC::Std::is_same_v<decltype(register_result), reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<decltype(right_scalar_result), reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<decltype(left_scalar_result), reg_type<T>>);
}

__simd_vf__ inline void compile_carry_arithmetic(
    reg_type<uint32_t> src0, reg_type<uint32_t> src1, reg_type<bool> carry_src)
{
    reg_type<bool> add_carry;
    auto add_result = asc::te::experimental::addc(add_carry, src0, src1);
    reg_type<bool> addc_carry;
    auto addc_result = asc::te::experimental::addc(addc_carry, src0, src1, carry_src);
    reg_type<bool> sub_carry;
    auto sub_result = asc::te::experimental::subc(sub_carry, src0, src1);
    reg_type<bool> subc_carry;
    auto subc_result = asc::te::experimental::subc(subc_carry, src0, src1, carry_src);
    static_assert(AscendC::Std::is_same_v<decltype(add_result), reg_type<uint32_t>>);
    static_assert(AscendC::Std::is_same_v<decltype(addc_result), reg_type<uint32_t>>);
    static_assert(AscendC::Std::is_same_v<decltype(sub_result), reg_type<uint32_t>>);
    static_assert(AscendC::Std::is_same_v<decltype(subc_result), reg_type<uint32_t>>);
}

template <typename T>
__simd_vf__ inline void compile_muls(reg_type<T> src, T scalar)
{
    auto right_scalar_result = src * scalar;
    auto left_scalar_result = scalar * src;
    static_assert(AscendC::Std::is_same_v<decltype(right_scalar_result), reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<decltype(left_scalar_result), reg_type<T>>);
}

template <typename T>
__simd_vf__ inline void compile_max(reg_type<T> src0, reg_type<T> src1, T scalar)
{
    auto register_result = asc::te::experimental::max(src0, src1);
    auto right_scalar_result = asc::te::experimental::max(src0, scalar);
    auto left_scalar_result = asc::te::experimental::max(scalar, src0);
    static_assert(AscendC::Std::is_same_v<decltype(register_result), reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<decltype(right_scalar_result), reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<decltype(left_scalar_result), reg_type<T>>);
}

template <typename T>
__simd_vf__ inline void compile_or(reg_type<T> src0, reg_type<T> src1, T scalar)
{
    auto register_result = src0 | src1;
    auto right_scalar_result = src0 | scalar;
    auto left_scalar_result = scalar | src0;
    static_assert(AscendC::Std::is_same_v<decltype(register_result), reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<decltype(right_scalar_result), reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<decltype(left_scalar_result), reg_type<T>>);
}

__simd_vf__ inline void compile_bool_or(reg_type<bool> src0, reg_type<bool> src1)
{
    auto result = src0 | src1;
    static_assert(AscendC::Std::is_same_v<decltype(result), reg_type<bool>>);
}

TEST(tensor_api_reg_vector_compute_3510, log_compiles_for_supported_types)
{
    auto log_half = &compile_log<half>;
    auto log_float = &compile_log<float>;
    (void)log_half;
    (void)log_float;
}

TEST(tensor_api_reg_vector_compute_3510, add_and_sub_compile_for_supported_types)
{
    auto add_int8 = &compile_add<int8_t>;
    auto add_uint8 = &compile_add<uint8_t>;
    auto add_int16 = &compile_add<int16_t>;
    auto add_uint16 = &compile_add<uint16_t>;
    auto add_half = &compile_add<half>;
    auto add_bfloat16 = &compile_add<bfloat16_t>;
    auto add_int32 = &compile_add<int32_t>;
    auto add_uint32 = &compile_add<uint32_t>;
    auto add_float = &compile_add<float>;
    auto sub_int8 = &compile_sub<int8_t>;
    auto sub_uint8 = &compile_sub<uint8_t>;
    auto sub_int16 = &compile_sub<int16_t>;
    auto sub_uint16 = &compile_sub<uint16_t>;
    auto sub_half = &compile_sub<half>;
    auto sub_bfloat16 = &compile_sub<bfloat16_t>;
    auto sub_int32 = &compile_sub<int32_t>;
    auto sub_uint32 = &compile_sub<uint32_t>;
    auto sub_float = &compile_sub<float>;
    (void)add_int8;
    (void)add_uint8;
    (void)add_int16;
    (void)add_uint16;
    (void)add_half;
    (void)add_bfloat16;
    (void)add_int32;
    (void)add_uint32;
    (void)add_float;
    (void)sub_int8;
    (void)sub_uint8;
    (void)sub_int16;
    (void)sub_uint16;
    (void)sub_half;
    (void)sub_bfloat16;
    (void)sub_int32;
    (void)sub_uint32;
    (void)sub_float;
}

TEST(tensor_api_reg_vector_compute_3510, muls_compiles_for_supported_types)
{
    auto muls_int16 = &compile_muls<int16_t>;
    auto muls_uint16 = &compile_muls<uint16_t>;
    auto muls_half = &compile_muls<half>;
    auto muls_int32 = &compile_muls<int32_t>;
    auto muls_uint32 = &compile_muls<uint32_t>;
    auto muls_float = &compile_muls<float>;
    (void)muls_int16;
    (void)muls_uint16;
    (void)muls_half;
    (void)muls_int32;
    (void)muls_uint32;
    (void)muls_float;
}

TEST(tensor_api_reg_vector_compute_3510, carry_arithmetic_compiles_for_uint32)
{
    auto carry_arithmetic = &compile_carry_arithmetic;
    (void)carry_arithmetic;
}

TEST(tensor_api_reg_vector_compute_3510, max_compiles_for_supported_types)
{
    auto max_int8 = &compile_max<int8_t>;
    auto max_uint8 = &compile_max<uint8_t>;
    auto max_int16 = &compile_max<int16_t>;
    auto max_uint16 = &compile_max<uint16_t>;
    auto max_half = &compile_max<half>;
    auto max_bfloat16 = &compile_max<bfloat16_t>;
    auto max_int32 = &compile_max<int32_t>;
    auto max_uint32 = &compile_max<uint32_t>;
    auto max_float = &compile_max<float>;
    (void)max_int8;
    (void)max_uint8;
    (void)max_int16;
    (void)max_uint16;
    (void)max_half;
    (void)max_bfloat16;
    (void)max_int32;
    (void)max_uint32;
    (void)max_float;
}

TEST(tensor_api_reg_vector_compute_3510, or_compiles_for_supported_types)
{
    auto or_bool = &compile_bool_or;
    auto or_int8 = &compile_or<int8_t>;
    auto or_uint8 = &compile_or<uint8_t>;
    auto or_int16 = &compile_or<int16_t>;
    auto or_uint16 = &compile_or<uint16_t>;
    auto or_half = &compile_or<half>;
    auto or_int32 = &compile_or<int32_t>;
    auto or_uint32 = &compile_or<uint32_t>;
    auto or_float = &compile_or<float>;
    (void)or_bool;
    (void)or_int8;
    (void)or_uint8;
    (void)or_int16;
    (void)or_uint16;
    (void)or_half;
    (void)or_int32;
    (void)or_uint32;
    (void)or_float;
}

} // namespace
