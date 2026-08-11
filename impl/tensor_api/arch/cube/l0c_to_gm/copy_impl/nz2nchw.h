/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/arch/cube/l0c_to_gm/copy_impl/nz2nchw.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file nz2nchw.h
 * \brief l0c(nz) -> gm(NCHW) copy via the nz2dn fixpipe path (no quant).
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_IMPL_NZ2NCHW_H
#define IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_IMPL_NZ2NCHW_H

#include "impl/tensor_api/arch/cube/l0c_to_gm/copy_impl/instruction.h"

namespace asc {
namespace te {

class data_copy_l0c_to_gm_nz2nchw {
public:
    template <const copy_l0c_to_gm_trait& trait, typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src, const fixpipe_params& params)
    {
        constexpr QuantMode_t quant_pre = get_quant_mode<trait.round_mode, T, U>();
        check_data_type::check_l0c_to_gm_data_type<quant_pre, T, U>();

        auto dst_layout = dst.layout();
        auto src_layout = src.layout();

        // nz2dn needs the channel register (src nz c0_value stride) set, mirroring the dn path's
        // emit_set_register non-nd branch. The generic set_register_impl would treat NCHW as neither
        // nd nor dn and emit set_register(1,0,0) without set_channel_para, so set it explicitly here.
        set_register_instr::set_register(1u, 0u, 0u, get_element<attr_info::stride, attr_info::column, 0>(src_layout));

        // dst NCHW (n_value, channel, height, width), n_value == 1. NCHW memory equals dn_ext (height*width, channel) column-major, so this
        // mirrors the validated dn path: n_size = channel = shape_type[1] (column count), m_size = height*width =
        // shape_type[2]*shape_type[3] (row count), dst_stride = height*width (the dn column stride = row count = stride[1]).
        uint32_t n_size = get<1>(dst_layout.shape());
        uint32_t m_size = get<2>(dst_layout.shape()) * get<3>(dst_layout.shape());
        uint32_t src_stride = get_element<attr_info::stride, attr_info::column, 1>(src_layout) / FRACTAL_FIXED;
        uint32_t dst_stride = get<1>(dst_layout.stride());

        uint8_t cache_mode = dst.engine().get_cache_mode();
        bool relu_en = trait.enable_relu;
        uint8_t unit_flag = params.unit_flag;
        bool is_channel_split = trait.enable_channel_split;

        copy_l0c_to_gm_instr::data_copy_tensor<quant_pre, T, U>(
            dst, src, n_size, m_size, src_stride, dst_stride, cache_mode, relu_en, unit_flag, is_channel_split, false, true);
    }

    template <const copy_l0c_to_gm_trait& trait, typename T, typename U, typename coord_dst_type, typename coord_src_type,
        typename shape_type>
    __aicore__ inline static void run(const T& dst, const U& src, const coord_dst_type& coord_dst,
        const coord_src_type& coord_src, const shape_type& shape, const fixpipe_params& params)
    {
        constexpr QuantMode_t quant_pre = get_quant_mode<trait.round_mode, T, U>();
        check_data_type::check_l0c_to_gm_data_type<quant_pre, T, U>();
        auto dst_shape = make_slice_shape(coord_dst, dst.layout(), shape);
        auto region_shape = dst_shape;
        auto dst_offset = dst.layout()(coord_dst);
        auto src_offset = src.layout()(coord_src);
        auto src_layout = src.layout();
        set_register_instr::set_register(
            1u, 0u, 0u, get_element<attr_info::stride, attr_info::column, 0>(src_layout));
        uint32_t n_size = get<1>(region_shape);
        uint32_t m_size = get<2>(region_shape) * get<3>(region_shape);
        uint32_t src_stride = get_element<attr_info::stride, attr_info::column, 1>(src_layout) / FRACTAL_FIXED;
        uint32_t dst_stride = get<1>(dst.layout().stride());
        copy_l0c_to_gm_instr::data_copy_region<quant_pre>(dst, src, dst_offset, src_offset, n_size, m_size, src_stride,
            dst_stride, dst.engine().get_cache_mode(), trait.enable_relu, params.unit_flag,
            trait.enable_channel_split, false, true);
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_IMPL_NZ2NCHW_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
