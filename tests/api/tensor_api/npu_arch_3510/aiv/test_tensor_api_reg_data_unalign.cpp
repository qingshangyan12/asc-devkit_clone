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
#include "tensor_api/experimental/vector_ops.h"
#include "tensor_api/tensor.h"

namespace {

template <typename T, typename RegDataType = T>
__simd_vf__ inline void run_unalign_load_store(__ubuf__ T* dst, __ubuf__ T* src)
{
    constexpr uint32_t packing_factor = asc::te::is_b4_type<T> ? 2 : 1;
    constexpr uint32_t elements_per_register = packing_factor * asc::te::get_vf_len() / sizeof(RegDataType);
    auto layout = asc::te::make_layout(asc::te::make_shape(elements_per_register * 2));
    auto dst_tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(dst), layout);
    auto src_tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(src), layout);
    static_assert(AscendC::Std::is_same_v<typename decltype(src_tensor)::data_type, T>);

    asc::te::experimental::unalign_load loader;
    asc::te::experimental::unalign_store storer;
    loader.prepare(src_tensor);

    auto first_src = src_tensor.slice(asc::te::make_coord(0), asc::te::make_shape(elements_per_register));
    auto first = loader.load(first_src);
    auto second = loader.load(src_tensor, asc::te::make_coord(elements_per_register));
    static_assert(AscendC::Std::is_same_v<decltype(first), asc::te::experimental::reg_tensor<RegDataType>>);
    static_assert(AscendC::Std::is_same_v<decltype(second), asc::te::experimental::reg_tensor<RegDataType>>);

    storer.store(
        dst_tensor.slice(asc::te::make_coord(0), asc::te::make_shape(elements_per_register)), first,
        elements_per_register);
    storer.store(dst_tensor, asc::te::make_coord(elements_per_register), second, elements_per_register);
    constexpr int32_t total_elements = static_cast<int32_t>(elements_per_register * 2);
    static_assert(AscendC::Std::is_same_v<decltype(storer.flush(dst_tensor, total_elements)), void>);
    storer.flush(dst_tensor, total_elements);
    EXPECT_EQ(src_tensor.engine().begin().get(), src);
    EXPECT_EQ(dst_tensor.engine().begin().get(), dst);
}

template <typename T>
class tensor_api_reg_data_unalign_3510 : public testing::Test {};

using UnalignDataTypes = testing::Types<
    int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float, int64_t, hifloat8_t, fp8_e8m0_t,
    fp8_e5m2_t, fp8_e4m3fn_t, fp4x2_e2m1_t, fp4x2_e1m2_t>;
TYPED_TEST_SUITE(tensor_api_reg_data_unalign_3510, UnalignDataTypes);

TYPED_TEST(tensor_api_reg_data_unalign_3510, full_register_sessions)
{
    constexpr uint32_t capacity = 2 * asc::te::get_vf_len() / sizeof(TypeParam) + 32;
    alignas(32) TypeParam src[capacity]{};
    alignas(32) TypeParam dst[capacity]{};
    run_unalign_load_store(dst, src);
    run_unalign_load_store(dst + 2, src + 1);
}

TEST(tensor_api_reg_data_unalign_3510_int4, full_register_session)
{
    constexpr uint32_t capacity = 2 * asc::te::get_vf_len() / sizeof(int4x2_t) + 32;
    alignas(32) int4b_t src[capacity]{};
    alignas(32) int4b_t dst[capacity]{};
    run_unalign_load_store<int4b_t, int4x2_t>(dst, src);
    run_unalign_load_store<int4b_t, int4x2_t>(dst + 2, src + 2);
}

TYPED_TEST(tensor_api_reg_data_unalign_3510, partial_stores_from_nonzero_coordinate)
{
    constexpr uint32_t packing_factor = asc::te::is_b4_type<TypeParam> ? 2 : 1;
    constexpr uint32_t elements_per_register = packing_factor * asc::te::get_vf_len() / sizeof(TypeParam);
    constexpr uint32_t columns = 2 * elements_per_register + 32;
    alignas(32) TypeParam src[2 * columns]{};
    alignas(32) TypeParam dst[2 * columns]{};
    auto layout = asc::te::make_layout(asc::te::make_shape(2, columns), asc::te::make_stride(columns, 1));
    auto src_tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(src), layout);
    auto dst_tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(dst), layout);
    auto start = asc::te::make_coord(1, 2);
    auto shape = asc::te::make_shape(1, 2 * elements_per_register);
    auto src_base = src_tensor.slice(start, shape);
    auto dst_base = dst_tensor.slice(start, shape);
    EXPECT_EQ(src_base.engine().begin().get(), src + (columns + 2) / packing_factor);
    EXPECT_EQ(dst_base.engine().begin().get(), dst + (columns + 2) / packing_factor);

    for (uint32_t count : {packing_factor, elements_per_register - packing_factor, elements_per_register}) {
        asc::te::experimental::unalign_load loader;
        asc::te::experimental::unalign_store storer;
        loader.prepare(src_base);
        auto value = loader.load(src_tensor, start);
        storer.store(dst_tensor, start, value, count);
        storer.flush(dst_base, static_cast<int32_t>(count));
    }
    EXPECT_EQ(src_tensor.engine().begin().get(), src);
    EXPECT_EQ(dst_tensor.engine().begin().get(), dst);
}

TYPED_TEST(tensor_api_reg_data_unalign_3510, continuous_partial_stores)
{
    constexpr uint32_t packing_factor = asc::te::is_b4_type<TypeParam> ? 2 : 1;
    constexpr uint32_t elements_per_register = packing_factor * asc::te::get_vf_len() / sizeof(TypeParam);
    constexpr uint32_t first_count = elements_per_register - 2 * packing_factor;
    constexpr uint32_t last_count = packing_factor;
    constexpr uint32_t total_count = first_count + last_count;
    constexpr uint32_t capacity = 2 * asc::te::get_vf_len() / sizeof(TypeParam) + 32;
    alignas(32) TypeParam src[capacity]{};
    alignas(32) TypeParam dst[capacity]{};
    auto layout = asc::te::make_layout(asc::te::make_shape(2 * elements_per_register));
    auto src_tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(src + 1), layout);
    auto dst_tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(dst + 2), layout);
    auto tail_coord = asc::te::make_coord(first_count);
    auto dst_tail = dst_tensor.slice(tail_coord, asc::te::make_shape(last_count));
    EXPECT_EQ(dst_tail.engine().begin().get(), dst + 2 + first_count / packing_factor);

    asc::te::experimental::unalign_load loader;
    asc::te::experimental::unalign_store storer;
    loader.prepare(src_tensor);
    auto first = loader.load(src_tensor);
    auto last = loader.load(src_tensor, asc::te::make_coord(elements_per_register));
    storer.store(dst_tensor, first, first_count);
    storer.store(dst_tensor, tail_coord, last, last_count);
    storer.flush(dst_tensor, static_cast<int32_t>(total_count));

    EXPECT_EQ(src_tensor.engine().begin().get(), src + 1);
    EXPECT_EQ(dst_tensor.engine().begin().get(), dst + 2);
}

} // namespace
