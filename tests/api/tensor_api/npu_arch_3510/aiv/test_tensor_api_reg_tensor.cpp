/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <type_traits>

#include <gtest/gtest.h>

#include "c_api/stub/cce_stub.h"
#include "tensor_api/tensor.h"

namespace asc {
namespace te {

template <typename T, typename = void>
struct has_mask_member : std::false_type {};

template <typename T>
struct has_mask_member<T, std::void_t<decltype(&T::mask)>> : std::true_type {};

static_assert(std::is_same_v<typename type_get<uint8_t>::type, vector_uint8_t>);
static_assert(std::is_same_v<typename type_get<uint16_t>::type, vector_uint16_t>);
static_assert(std::is_same_v<typename type_get<uint32_t>::type, vector_uint32_t>);
static_assert(std::is_same_v<typename type_get<uint64_t>::type, vector_uint64_t>);
static_assert(std::is_same_v<typename type_get<int8_t>::type, vector_int8_t>);
static_assert(std::is_same_v<typename type_get<int16_t>::type, vector_int16_t>);
static_assert(std::is_same_v<typename type_get<int32_t>::type, vector_int32_t>);
static_assert(std::is_same_v<typename type_get<int64_t>::type, vector_int64_t>);
static_assert(std::is_same_v<typename type_get<half>::type, vector_half>);
static_assert(std::is_same_v<typename type_get<bfloat16_t>::type, vector_bfloat16_t>);
static_assert(std::is_same_v<typename type_get<float>::type, vector_float>);
static_assert(std::is_same_v<typename type_get<hifloat8_t>::type, vector_hifloat8_t>);
static_assert(std::is_same_v<typename type_get<fp8_e4m3fn_t>::type, vector_fp8_e4m3fn_t>);
static_assert(std::is_same_v<typename type_get<fp8_e5m2_t>::type, vector_fp8_e5m2_t>);
static_assert(std::is_same_v<typename type_get<fp8_e8m0_t>::type, vector_fp8_e8m0_t>);
static_assert(std::is_same_v<typename type_get<int4x2_t>::type, vector_int4x2_t>);
static_assert(std::is_same_v<typename type_get<fp4x2_e2m1_t>::type, vector_fp4x2_e2m1_t>);
static_assert(std::is_same_v<typename type_get<fp4x2_e1m2_t>::type, vector_fp4x2_e1m2_t>);
static_assert(std::is_same_v<typename type_get<bool>::type, vector_bool>);
static_assert(std::is_same_v<typename reg_tensor<float>::type, float>);
static_assert(std::is_same_v<typename reg_tensor<float>::reg_type, vector_float>);
static_assert(std::is_same_v<typename reg_tensor<bool>::reg_type, vector_bool>);
static_assert(has_mask_member<reg_tensor<float>>::value);
static_assert(!has_mask_member<reg_tensor<bool>>::value);
static_assert(std::is_same_v<decltype(ones<float>()), reg_tensor<bool>>);
static_assert(std::is_same_v<decltype(zeros<float>()), reg_tensor<bool>>);
static_assert(std::is_same_v<decltype(make_mask<mask_pattern::vl8, float>()), reg_tensor<bool>>);
static_assert(std::is_same_v<decltype(interleave<float>({}, {})), reg_pair<bool>>);
static_assert(std::is_same_v<decltype(deinterleave<float>({}, {})), reg_pair<bool>>);
static_assert(std::is_same_v<decltype(select({}, {}, {})), reg_tensor<bool>>);
static_assert(std::is_same_v<decltype(pack({})), reg_tensor<bool>>);
static_assert(std::is_same_v<decltype(unpack({})), reg_tensor<bool>>);

template <typename T>
__aicore__ inline void compile_mask_apis(uint32_t& remain)
{
    auto all = make_mask<mask_pattern::all, T>();
    auto vl1 = make_mask<mask_pattern::vl1, T>();
    auto vl2 = make_mask<mask_pattern::vl2, T>();
    auto vl3 = make_mask<mask_pattern::vl3, T>();
    auto vl4 = make_mask<mask_pattern::vl4, T>();
    auto vl8 = make_mask<mask_pattern::vl8, T>();
    auto vl16 = make_mask<mask_pattern::vl16, T>();
    auto vl32 = make_mask<mask_pattern::vl32, T>();
    auto vl64 = make_mask<mask_pattern::vl64, T>();
    auto vl128 = make_mask<mask_pattern::vl128, T>();
    auto every3 = make_mask<mask_pattern::every3, T>();
    auto every4 = make_mask<mask_pattern::every4, T>();
    auto half = make_mask<mask_pattern::half, T>();
    auto quarter = make_mask<mask_pattern::quarter, T>();
    auto none = make_mask<mask_pattern::none, T>();
    auto updated = update_mask<T>(remain);
    auto interleaved = interleave<T>(all, none);
    auto deinterleaved = deinterleave<T>(interleaved.first, interleaved.second);
    auto selected = select(updated, deinterleaved.first, deinterleaved.second);
    auto packed_low = pack(selected);
    auto packed_high = pack<mask_half::high>(selected);
    auto unpacked_low = unpack(packed_low);
    auto unpacked_high = unpack<mask_half::high>(packed_high);

    (void)vl1;
    (void)vl2;
    (void)vl3;
    (void)vl4;
    (void)vl8;
    (void)vl16;
    (void)vl32;
    (void)vl64;
    (void)vl128;
    (void)every3;
    (void)every4;
    (void)half;
    (void)quarter;
    (void)unpacked_low;
    (void)unpacked_high;
}

template <typename T, load_sideband_mode sideband_mode = load_sideband_mode::direct>
__aicore__ inline void compile_tensor_reg_load_store(__ubuf__ T* dst, __ubuf__ T* src, vector_bool mask)
{
    auto layout = make_layout(make_shape(64));
    auto dst_tensor = make_tensor(make_mem_ptr<location::ub>(dst), layout);
    auto src_tensor = make_tensor(make_mem_ptr<location::ub>(src), layout);
    reg_tensor<bool> mask_reg{mask};
    auto value = src_tensor.template load<sideband_mode>(make_coord(0)).with_mask(mask_reg);
    static_assert(std::is_same_v<typename decltype(value)::type, T>);
    dst_tensor.store(make_coord(0), value);

    auto full_value = src_tensor.load(make_coord(0)).with_mask(ones<T>());
    static_assert(std::is_same_v<typename decltype(full_value)::type, T>);
}

TEST(test_tensor_api_reg_tensor, holds_register_and_mask)
{
    reg_tensor<float> value{};
    (void)value.reg;
    (void)value.mask;

    auto compile_load_store = &compile_tensor_reg_load_store<float>;
    auto compile_mask8 = &compile_mask_apis<uint8_t>;
    auto compile_mask16 = &compile_mask_apis<half>;
    auto compile_mask32 = &compile_mask_apis<float>;
    (void)compile_load_store;
    (void)compile_mask8;
    (void)compile_mask16;
    (void)compile_mask32;
}

} // namespace te
} // namespace asc
