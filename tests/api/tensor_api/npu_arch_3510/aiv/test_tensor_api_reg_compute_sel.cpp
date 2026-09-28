/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
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

template <typename SrcType, typename PackedType>
__simd_vf__ inline void compile_reg_data_pack_unpack(vector_bool mask)
{
    asc::te::experimental::reg_tensor<SrcType> src {mask};
    auto packed_default = asc::te::experimental::pack(src);
    auto packed_low = asc::te::experimental::pack<asc::te::experimental::high_low_part::low>(src);
    auto packed_high = asc::te::experimental::pack<asc::te::experimental::high_low_part::high>(src);
    static_assert(AscendC::Std::is_same_v<decltype(packed_default), asc::te::experimental::reg_tensor<PackedType>>);
    static_assert(AscendC::Std::is_same_v<decltype(packed_low), asc::te::experimental::reg_tensor<PackedType>>);
    static_assert(AscendC::Std::is_same_v<decltype(packed_high), asc::te::experimental::reg_tensor<PackedType>>);

    auto unpacked_default = asc::te::experimental::unpack(packed_default);
    auto unpacked_low = asc::te::experimental::unpack<asc::te::experimental::high_low_part::low>(packed_low);
    auto unpacked_high = asc::te::experimental::unpack<asc::te::experimental::high_low_part::high>(packed_high);
    static_assert(AscendC::Std::is_same_v<decltype(unpacked_default), asc::te::experimental::reg_tensor<SrcType>>);
    static_assert(AscendC::Std::is_same_v<decltype(unpacked_low), asc::te::experimental::reg_tensor<SrcType>>);
    static_assert(AscendC::Std::is_same_v<decltype(unpacked_high), asc::te::experimental::reg_tensor<SrcType>>);
}

template <typename SrcType, typename UnpackedType>
__simd_vf__ inline void compile_reg_data_unpack(vector_bool mask)
{
    asc::te::experimental::reg_tensor<SrcType> src {mask};
    auto unpacked_low = asc::te::experimental::unpack<asc::te::experimental::high_low_part::low>(src);
    auto unpacked_high = asc::te::experimental::unpack<asc::te::experimental::high_low_part::high>(src);
    static_assert(AscendC::Std::is_same_v<decltype(unpacked_low), asc::te::experimental::reg_tensor<UnpackedType>>);
    static_assert(AscendC::Std::is_same_v<decltype(unpacked_high), asc::te::experimental::reg_tensor<UnpackedType>>);
}

__simd_vf__ inline void compile_reg_mask_pack_unpack(vector_bool mask)
{
    asc::te::experimental::reg_tensor<bool> src {mask};
    auto packed_default = asc::te::experimental::pack(src);
    auto packed_low = asc::te::experimental::pack<asc::te::experimental::high_low_part::low>(src);
    auto packed_high = asc::te::experimental::pack<asc::te::experimental::high_low_part::high>(src);
    auto unpacked_default = asc::te::experimental::unpack(packed_default);
    auto unpacked_low = asc::te::experimental::unpack<asc::te::experimental::high_low_part::low>(packed_low);
    auto unpacked_high = asc::te::experimental::unpack<asc::te::experimental::high_low_part::high>(packed_high);
    static_assert(AscendC::Std::is_same_v<decltype(packed_default), asc::te::experimental::reg_tensor<bool>>);
    static_assert(AscendC::Std::is_same_v<decltype(packed_low), asc::te::experimental::reg_tensor<bool>>);
    static_assert(AscendC::Std::is_same_v<decltype(packed_high), asc::te::experimental::reg_tensor<bool>>);
    static_assert(AscendC::Std::is_same_v<decltype(unpacked_default), asc::te::experimental::reg_tensor<bool>>);
    static_assert(AscendC::Std::is_same_v<decltype(unpacked_low), asc::te::experimental::reg_tensor<bool>>);
    static_assert(AscendC::Std::is_same_v<decltype(unpacked_high), asc::te::experimental::reg_tensor<bool>>);
}

template <typename T>
__simd_vf__ inline void compile_reg_compress(vector_bool mask)
{
    asc::te::experimental::reg_tensor<T> src {mask};
    asc::te::experimental::reg_tensor<bool> select_mask {mask};
    auto compressed = asc::te::experimental::compress(src, select_mask);
    static_assert(AscendC::Std::is_same_v<decltype(compressed), asc::te::experimental::reg_tensor<T>>);
}

template <typename T>
__simd_vf__ inline void compile_reg_expand(vector_bool mask)
{
    asc::te::experimental::reg_tensor<bool> select_mask {mask};
    auto expanded = asc::te::experimental::expand<T>(select_mask);
    static_assert(AscendC::Std::is_same_v<decltype(expanded), asc::te::experimental::reg_tensor<T>>);
}

TEST(tensor_api_reg_compute_sel_3510, compiles_reg_compute_sel_interfaces)
{
    auto compile_data_pack_b16 = &compile_reg_data_pack_unpack<uint16_t, uint8_t>;
    auto compile_data_pack_b32 = &compile_reg_data_pack_unpack<uint32_t, uint16_t>;
    auto compile_data_unpack_s8 = &compile_reg_data_unpack<int8_t, int16_t>;
    auto compile_data_unpack_s16 = &compile_reg_data_unpack<int16_t, int32_t>;
    auto compile_mask_pack_unpack = &compile_reg_mask_pack_unpack;
    auto compile_compress_float = &compile_reg_compress<float>;
    auto compile_expand_s8 = &compile_reg_expand<int8_t>;
    auto compile_expand_u8 = &compile_reg_expand<uint8_t>;
    auto compile_expand_s16 = &compile_reg_expand<int16_t>;
    auto compile_expand_u16 = &compile_reg_expand<uint16_t>;
    auto compile_expand_s32 = &compile_reg_expand<int32_t>;
    auto compile_expand_u32 = &compile_reg_expand<uint32_t>;
    (void)compile_data_pack_b16;
    (void)compile_data_pack_b32;
    (void)compile_data_unpack_s8;
    (void)compile_data_unpack_s16;
    (void)compile_mask_pack_unpack;
    (void)compile_compress_float;
    (void)compile_expand_s8;
    (void)compile_expand_u8;
    (void)compile_expand_s16;
    (void)compile_expand_u16;
    (void)compile_expand_s32;
    (void)compile_expand_u32;
}

} // namespace
