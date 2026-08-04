namespace hybrid_impl {

constexpr uint32_t THREAD_COUNT = 512;
constexpr uint32_t ZERO_TILE_COUNT = 4096;

template <typename T>
__aicore__ inline T min(T a, T b)
{
    return a < b ? a : b;
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
__simt_vf__ __launch_bounds__(THREAD_COUNT) inline void scatter_grad(const __gm__ T* grad,
                                                                     const __gm__ IndexT* argmax,
                                                                     __gm__ T* grad_input,
                                                                     uint32_t block_nc_count,
                                                                     uint32_t input_spatial,
                                                                     uint32_t output_spatial)
{
    const uint64_t block_grad_count = static_cast<uint64_t>(block_nc_count) * output_spatial;
    for (uint64_t local_grad_index = static_cast<uint64_t>(threadIdx.x); local_grad_index < block_grad_count;
         local_grad_index += static_cast<uint64_t>(blockDim.x)) {
        const uint64_t local_nc_index = static_cast<uint32_t>(local_grad_index) / output_spatial;
        const uint64_t input_index =
            local_nc_index * input_spatial + static_cast<uint64_t>(argmax[local_grad_index]);
        grad_input[input_index] = grad[local_grad_index];
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

__aicore__ inline void zero_input(__gm__ float* input_data, uint64_t count)
{
    __ubuf__ float zero_ub[ZERO_TILE_COUNT];
    asc_vf_call<fill_float_ub>(zero_ub, 0.0F, ZERO_TILE_COUNT);
    wait_v_to_mte3();

    for (uint64_t offset = 0; offset < count; offset += ZERO_TILE_COUNT) {
        const uint32_t copy_count = static_cast<uint32_t>(min<uint64_t>(ZERO_TILE_COUNT, count - offset));
        const uint32_t copy_bytes = copy_count * sizeof(float);
        asc_copy_ub2gm(input_data + offset, zero_ub, copy_bytes);
    }
    wait_mte3_to_scalar();
}

} // namespace hybrid_impl

__global__ __vector__ void adaptive_max_pool3d_grad_hybrid_kernel(__gm__ uint8_t* grad, __gm__ uint8_t* argmax,
                                                                  __gm__ uint8_t* grad_input, uint32_t nc,
                                                                  uint32_t input_spatial, uint32_t output_spatial)
{
    asc_init();
    
    __gm__ float* grad_data = reinterpret_cast<__gm__ float*>(grad);
    __gm__ int32_t* argmax_data = reinterpret_cast<__gm__ int32_t*>(argmax);
    __gm__ float* input_data = reinterpret_cast<__gm__ float*>(grad_input);

    const uint32_t nc_start = static_cast<uint32_t>((static_cast<uint64_t>(block_idx) * nc) / block_num);
    const uint32_t nc_end = static_cast<uint32_t>((static_cast<uint64_t>(block_idx + 1U) * nc) / block_num);
    const uint32_t block_nc_count = nc_end - nc_start;

    __gm__ float* grad_slice = grad_data + static_cast<uint64_t>(nc_start) * output_spatial;
    __gm__ int32_t* argmax_slice = argmax_data + static_cast<uint64_t>(nc_start) * output_spatial;
    __gm__ float* input_slice = input_data + static_cast<uint64_t>(nc_start) * input_spatial;

    hybrid_impl::zero_input(input_slice, static_cast<uint64_t>(block_nc_count) * input_spatial);
    asc_vf_call<hybrid_impl::scatter_grad<float, int32_t>>(
        dim3(hybrid_impl::THREAD_COUNT), grad_slice, argmax_slice, input_slice, block_nc_count, input_spatial, output_spatial);
}
