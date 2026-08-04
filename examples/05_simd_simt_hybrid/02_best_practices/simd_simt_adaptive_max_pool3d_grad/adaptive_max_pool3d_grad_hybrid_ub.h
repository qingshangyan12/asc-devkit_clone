/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

namespace hybrid_ub_impl {

constexpr uint32_t THREAD_COUNT = 512;
constexpr uint32_t INPUT_UB_SIZE = 32U * 32U * 32U;
constexpr uint32_t GRAD_UB_SIZE = 16U * 16U * 16U;

__aicore__ inline uint32_t align_up8(uint32_t value) { return (value + 7U) / 8U * 8U; }

__aicore__ inline void wait_mte2_to_v()
{
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
}

__aicore__ inline void wait_v_to_mte3()
{
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
}

__aicore__ inline void wait_mte3_to_scalar()
{
    asc_sync_notify(PIPE_MTE3, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_MTE3, PIPE_S, EVENT_ID0);
}

template <typename T, typename IndexT>
__simt_vf__ __launch_bounds__(THREAD_COUNT) inline void scatter_grad(
    const __ubuf__ T* grad, const __ubuf__ IndexT* argmax, __ubuf__ T* grad_input, uint32_t input_spatial,
    uint32_t output_spatial)
{
    for (uint32_t local_grad_index = threadIdx.x; local_grad_index < output_spatial; local_grad_index += blockDim.x) {
        const uint32_t output_index = static_cast<uint32_t>(argmax[local_grad_index]);
        if (output_index < input_spatial) {
            grad_input[output_index] = grad[local_grad_index];
        }
    }
}

__simd_vf__ inline void fill_float_ub(__ubuf__ float* dst, float value, uint32_t count)
{
    constexpr uint32_t one_repeat_size = asc_get_vf_len() / sizeof(float);
    vector_float value_reg;
    vector_bool mask;
    for (uint32_t offset = 0; offset < count; offset += one_repeat_size) {
        uint32_t active_count = count - offset < one_repeat_size ? count - offset : one_repeat_size;
        mask = asc_update_mask_b32(active_count);
        asc_duplicate_scalar(value_reg, value, mask);
        asc_storealign(dst + offset, value_reg, mask);
    }
}

} // namespace hybrid_ub_impl

__global__ __vector__ void adaptive_max_pool3d_grad_hybrid_ub_kernel(
    __gm__ uint8_t* grad, __gm__ uint8_t* argmax, __gm__ uint8_t* grad_input, uint32_t nc, uint32_t input_spatial,
    uint32_t output_spatial)
{
    asc_init();

    __gm__ float* grad_data = reinterpret_cast<__gm__ float*>(grad);
    __gm__ int32_t* argmax_data = reinterpret_cast<__gm__ int32_t*>(argmax);
    __gm__ float* input_data = reinterpret_cast<__gm__ float*>(grad_input);

    const uint32_t nc_start = static_cast<uint32_t>((static_cast<uint64_t>(block_idx) * nc) / block_num);
    const uint32_t nc_end = static_cast<uint32_t>((static_cast<uint64_t>(block_idx + 1U) * nc) / block_num);
    const uint32_t block_nc_count = nc_end - nc_start;

    __ubuf__ float grad_input_ub[hybrid_ub_impl::INPUT_UB_SIZE];
    __ubuf__ float grad_ub[hybrid_ub_impl::GRAD_UB_SIZE];
    __ubuf__ int32_t argmax_ub[hybrid_ub_impl::GRAD_UB_SIZE];

    for (uint32_t nc_index = nc_start; nc_index < nc_end; ++nc_index) {
        const uint32_t active_grad_count = output_spatial;
        const uint8_t grad_right_padding =
            static_cast<uint8_t>(hybrid_ub_impl::align_up8(active_grad_count) - active_grad_count);

        asc_vf_call<hybrid_ub_impl::fill_float_ub>(grad_input_ub, 0.0F, input_spatial);
        asc_sync_vec();

        const uint32_t active_grad_bytes = active_grad_count * sizeof(float);
        const uint64_t grad_base_offset = static_cast<uint64_t>(nc_index) * output_spatial;
        if (grad_right_padding == 0 && grad_base_offset % 8U == 0) {
            asc_copy_gm2ub(grad_ub, grad_data + grad_base_offset, active_grad_bytes);
            asc_copy_gm2ub(argmax_ub, argmax_data + grad_base_offset, active_grad_bytes);
        } else {
            asc_set_copy_pad_val(0.0F);
            asc_copy_gm2ub_align(
                grad_ub, grad_data + grad_base_offset, 1, active_grad_bytes, 0, grad_right_padding, true,
                asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, 0, 0);
            asc_set_copy_pad_val(static_cast<int32_t>(0));
            asc_copy_gm2ub_align(
                argmax_ub, argmax_data + grad_base_offset, 1, active_grad_bytes, 0, grad_right_padding, true,
                asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, 0, 0);
        }
        hybrid_ub_impl::wait_mte2_to_v();

        asc_vf_call<hybrid_ub_impl::scatter_grad<float, int32_t>>(
            dim3(hybrid_ub_impl::THREAD_COUNT), grad_ub, argmax_ub, grad_input_ub, input_spatial, output_spatial);
        asc_sync_data_barrier(mem_dsb_t::DSB_UB);
        hybrid_ub_impl::wait_v_to_mte3();

        const uint32_t active_input_bytes = input_spatial * sizeof(float);
        const uint64_t input_base_offset = static_cast<uint64_t>(nc_index) * input_spatial;
        asc_copy_ub2gm_align(input_data + input_base_offset, grad_input_ub, active_input_bytes);
        hybrid_ub_impl::wait_mte3_to_scalar();
    }
}
