/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_TENSOR_API_ARCH_CUBE_UTILS_L0C_VECTOR_QUANT_UTILS_H
#define IMPL_TENSOR_API_ARCH_CUBE_UTILS_L0C_VECTOR_QUANT_UTILS_H

#include "impl/tensor_api/arch/cube/l1_to_fb/l1_to_fb_copy.h"
#include "impl/tensor_api/arch/cube/utils/l0c2out_utils.h"

namespace asc {
namespace te {

template <typename SrcTensor>
__aicore__ inline static void copy_l0c_vector_quant_to_fixbuf(
    const SrcTensor& src, uint16_t cal_n_size, uint16_t n_iter_index)
{
    auto dst_addr = reinterpret_cast<__fbuf__ uint64_t*>(alloc_fixbuf_temp_buf(cal_n_size));
    auto dst = make_tensor(make_mem_ptr<location::fixbuf>(dst_addr), src.layout());
    auto coord = make_coord(_0{}, n_iter_index * main_loop_n_size);
    auto shape = make_shape(_1{}, cal_n_size);
    auto tile_src = src.slice(coord, shape);
    copy_l1_to_fixbuf_nd::run<l1_to_fixbuf_trait_default::value>(dst, tile_src);
    set_qua(dst_addr);
}

template <typename DstTensor>
__aicore__ inline static constexpr auto make_l0c_vector_quant_dst_coord(uint32_t n_offset)
{
    using layout_type = typename DstTensor::layout_type;
    if constexpr (layout_type::depth == five_dim_data) {
        return make_coord(_0{}, make_coord(make_coord(0, 0), make_coord(0, n_offset)));
    } else if constexpr (layout_type::depth == four_dim_data) {
        return make_coord(make_coord(0, 0), make_coord(0, n_offset));
    } else if constexpr (layout_type::depth == three_dim_data) {
        return make_coord(_0{}, make_coord(0, n_offset));
    } else {
        static_assert(layout_type::depth == two_dim_data, "Only support two-dim or four-dim dst tensor.");
        return make_coord(0, n_offset);
    }
}

template <typename SrcTensor>
__aicore__ inline static constexpr auto make_l0c_vector_quant_src_coord(uint32_t n_offset)
{
    using layout_type = typename SrcTensor::layout_type;
    if constexpr (layout_type::depth == five_dim_data) {
        return make_coord(_0{}, make_coord(make_coord(0, 0), make_coord(0, n_offset)));
    } else {
        static_assert(layout_type::depth == four_dim_data, "Only support four-dim or five-dim src tensor.");
        return make_coord(make_coord(0, 0), make_coord(0, n_offset));
    }
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_UTILS_L0C_VECTOR_QUANT_UTILS_H
