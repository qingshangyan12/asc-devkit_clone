/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0A_COPY_IMPL_IMG2COL_COMMON_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0A_COPY_IMPL_IMG2COL_COMMON_H

#include "impl/tensor_api/arch/cube/l1_to_l0a/copy_impl/instruction.h"

namespace asc {
namespace te {

template <typename DstTensor, typename SrcTensor, typename PaddingValue>
__aicore__ inline void emit_l1_to_l0a_img2col(
    const DstTensor& dst, const SrcTensor& src, const img2col_params<PaddingValue>& params, uint16_t l1_h,
    uint16_t l1_w, uint16_t k_start_pos, uint16_t m_start_pos, uint16_t channel_size)
{
    load_l1_to_l0a_img2col_instr::set_f_matrix(l1_h, l1_w, params.pad_list);
    load_l1_to_l0a_img2col_instr::set_padding(params.pad_value);
    load_l1_to_l0a_img2col_instr::set_repeat(
        static_cast<uint16_t>(Std::ceil_division(params.m_extension, fractal_fixed)));
    load_l1_to_l0a_img2col_instr::load_data(
        dst.data().get(), src.data().get(), params.k_extension, params.m_extension, k_start_pos, m_start_pos,
        params.stride_w, params.stride_h, params.filter_w, params.filter_h, params.dilation_filter_w,
        params.dilation_filter_h, params.enable_filter_w_extend, params.enable_filter_h_extend, params.enable_transpose,
        params.enable_f_matrix_ctrl, channel_size);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0A_COPY_IMPL_IMG2COL_COMMON_H
