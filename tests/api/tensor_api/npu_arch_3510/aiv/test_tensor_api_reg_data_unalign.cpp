/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the LICENSE file.
 */
#include <gtest/gtest.h>

#include "tensor_api/stub/cce_stub.h"
#include "tensor_api/experimental/vector_ops.h"
#include "tensor_api/tensor.h"

namespace {

template <typename T, typename RegDataType = T>
__simd_vf__ inline void compile_unalign_load_store(__ubuf__ T* dst, __ubuf__ T* src)
{
    constexpr uint32_t elements_per_register = static_cast<uint32_t>(asc_get_vf_len()) / sizeof(RegDataType);
    auto layout = asc::te::make_layout(asc::te::make_shape(elements_per_register * 2));
    auto dst_tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(dst), layout);
    auto src_tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(src), layout);
    static_assert(AscendC::Std::is_same_v<typename decltype(src_tensor)::data_type, T>);

    asc::te::experimental::unalign_load loader;
    asc::te::experimental::unalign_store storer;
    loader.prepare(src_tensor);

    auto first_src =
        src_tensor.slice(asc::te::make_coord(0), asc::te::make_shape(elements_per_register));
    auto first = loader.load(first_src);
    auto second_src = src_tensor.slice(
        asc::te::make_coord(elements_per_register), asc::te::make_shape(elements_per_register));
    loader.prepare(second_src);
    auto second = loader.load(second_src, asc::te::make_coord(0));
    static_assert(AscendC::Std::is_same_v<decltype(first), asc::te::experimental::reg_tensor<RegDataType>>);
    static_assert(AscendC::Std::is_same_v<decltype(second), asc::te::experimental::reg_tensor<RegDataType>>);

    storer.store(
        dst_tensor.slice(asc::te::make_coord(0), asc::te::make_shape(elements_per_register)), first,
        elements_per_register);
    storer.store(
        dst_tensor, asc::te::make_coord(elements_per_register), second, elements_per_register);
    constexpr int32_t total_elements = static_cast<int32_t>(elements_per_register * 2);
    static_assert(AscendC::Std::is_same_v<decltype(storer.flush(dst_tensor, total_elements)), void>);
    storer.flush(dst_tensor, total_elements);
}

TEST(tensor_api_reg_data_unalign_3510, compiles_full_register_sessions)
{
    auto compile_b8 = &compile_unalign_load_store<uint8_t>;
    auto compile_b16 = &compile_unalign_load_store<half>;
    auto compile_b32 = &compile_unalign_load_store<float>;
    auto compile_b64 = &compile_unalign_load_store<int64_t>;
    auto compile_int4 = &compile_unalign_load_store<int4b_t, int4x2_t>;
    (void)compile_b8;
    (void)compile_b16;
    (void)compile_b32;
    (void)compile_b64;
    (void)compile_int4;
}

} // namespace
