/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#pragma once

#include <cstdint>

#include "adaptive_max_pool3d_grad_common.h"
#include "c_api/asc_simd.h"
#include "kernel_operator.h"

namespace simd_impl {

using pool3d_common::min;
using pool3d_common::wait_mte2_to_scalar;
using pool3d_common::wait_mte3_to_v;
using pool3d_common::wait_scalar_to_v;
using pool3d_common::wait_v_to_mte3;
using pool3d_common::wait_v_to_scalar;

constexpr uint32_t NC_TILE = 64;
constexpr uint32_t GM_BLOCK_BYTES = 32;
constexpr uint32_t OUTPUT_W_TILE = 8;
constexpr uint32_t W_COMPARE_TILE = 32;
constexpr uint32_t B32_ELEM_COUNT = GM_BLOCK_BYTES / sizeof(float);

static_assert(sizeof(float) == sizeof(int32_t), "grad and argmax use the same 32B-aligned UB layout");
static_assert(OUTPUT_W_TILE <= B32_ELEM_COUNT, "grad_tile and argmax_tile only reserve one 32B slot per NC lane");

// SIMD实现使用的全局内存张量。
struct GlobalTensors {
    __gm__ float* grad;
    __gm__ int32_t* argmax;
    __gm__ float* grad_input;
};

// [NC, D, H, W]的运行时形状参数。
struct Pool3DShape {
    uint32_t nc;
    uint32_t input_d;
    uint32_t input_h;
    uint32_t input_w;
    uint32_t output_d;
    uint32_t output_h;
    uint32_t output_w;
};

// 全局核函数入口分配并传给各辅助函数使用的UB缓冲区。
struct UbBuffers {
    __ubuf__ float* grad_tile;
    __ubuf__ int32_t* argmax_tile;
    __ubuf__ float* grad;
    __ubuf__ int32_t* argmax;
    __ubuf__ uint32_t* scatter_offsets;
    __ubuf__ float* selected_tile;
    __ubuf__ float* expanded;
};

// 每个核的执行上下文，供标量控制代码和SIMD辅助函数共享。
struct SimdContext {
    GlobalTensors tensors;
    Pool3DShape shape;
    UbBuffers ub;
    uint32_t input_spatial;
    uint32_t output_spatial;
    uint32_t nc_tile_count;
    uint32_t output_w_tile_count;
    uint32_t output_tile_count;
    uint32_t nc_start;
    uint32_t nc_count;
};

// 针对一个输出元素，将argmax与一段输入W块比较，只保留命中的grad通道。
__simd_vf__ inline void select_w_tile(
    __ubuf__ float* grad, __ubuf__ int32_t* argmax, __ubuf__ float* selected_tile, uint32_t base_input_index,
    uint32_t tile_count, uint32_t nc_count)
{
    vector_bool active_mask = asc_update_mask_b32(nc_count);
    vector_bool cmp_mask;
    vector_float grad_reg;
    vector_float zero_reg;
    vector_float selected_reg;
    vector_int32_t argmax_reg;
    vector_int32_t input_index_reg;

    asc_duplicate_scalar(zero_reg, 0.0F, active_mask);
    asc_loadalign(grad_reg, grad);
    asc_loadalign(argmax_reg, argmax);

    for (uint32_t tile_index = 0; tile_index < tile_count; ++tile_index) {
        asc_duplicate_scalar(input_index_reg, static_cast<int32_t>(base_input_index + tile_index), active_mask);
        asc_eq(cmp_mask, argmax_reg, input_index_reg, active_mask);
        asc_select(selected_reg, grad_reg, zero_reg, cmp_mask);
        asc_storealign(selected_tile + tile_index * NC_TILE, selected_reg, active_mask);
    }
}

// 将NC连续的选中值散列成32B间隔的UB布局，供后续按步长写回GM。
__simd_vf__ inline void scatter_selected_w(
    __ubuf__ float* selected, __ubuf__ uint32_t* scatter_offsets, __ubuf__ float* expanded, uint32_t nc_count)
{
    vector_bool active_mask = asc_update_mask_b32(nc_count);
    vector_float selected_reg;
    vector_uint32_t scatter_index_reg;

    asc_loadalign(selected_reg, selected);
    asc_loadalign(scatter_index_reg, scatter_offsets);
    asc_scatter(expanded, selected_reg, scatter_index_reg, active_mask);
}

// 初始化派生维度和当前核函数需要复用的上下文信息。
__aicore__ inline void init_context(
    SimdContext& ctx, const GlobalTensors& tensors, const Pool3DShape& shape, const UbBuffers& ub)
{
    ctx.tensors = tensors;
    ctx.shape = shape;
    ctx.ub = ub;
    ctx.input_spatial = shape.input_d * shape.input_h * shape.input_w;
    ctx.output_spatial = shape.output_d * shape.output_h * shape.output_w;
    ctx.nc_tile_count = (shape.nc + NC_TILE - 1U) / NC_TILE;
    ctx.output_w_tile_count = (shape.output_w + OUTPUT_W_TILE - 1U) / OUTPUT_W_TILE;
    ctx.output_tile_count = shape.output_d * shape.output_h * ctx.output_w_tile_count;
}

// 准备散列偏移，使每个NC通道都落在一个32B UB槽位的起始位置。
__aicore__ inline void init_scatter_offsets(SimdContext& ctx)
{
    for (uint32_t i = 0; i < NC_TILE; ++i) {
        ctx.ub.scatter_offsets[i] = i * B32_ELEM_COUNT;
    }
    wait_scalar_to_v();
}

// 将一块输出W块的grad和argmax数据搬运到UB中，并按32B对齐。
__aicore__ inline void copy_in_grad_argmax(SimdContext& ctx, uint32_t output_base_index, uint32_t output_count)
{
    const uint64_t gm_offset = static_cast<uint64_t>(ctx.nc_start) * ctx.output_spatial + output_base_index;
    asc_set_ndim_loop0_stride(1, 1);
    // UB布局为[nc][B32_ELEM_COUNT]，每个NC通道从32B边界开始。
    asc_set_ndim_loop1_stride(B32_ELEM_COUNT, ctx.output_spatial);
    asc_set_ndim_loop2_stride(0, 0);
    asc_set_ndim_loop3_stride(0, 0);
    asc_set_ndim_loop4_stride(0, 0);
    asc_ndim_copy_gm2ub(
        ctx.ub.grad_tile, ctx.tensors.grad + gm_offset, output_count, ctx.nc_count, 1, 1, 1, 0, 0, false,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);
    asc_ndim_copy_gm2ub(
        ctx.ub.argmax_tile, ctx.tensors.argmax + gm_offset, output_count, ctx.nc_count, 1, 1, 1, 0, 0, false,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);
}

// 从已搬入UB的[NC, W]块中抽取一个W通道，整理成NC连续的grad和argmax。
__aicore__ inline void extract_output_lanes(SimdContext& ctx, uint32_t output_offset)
{
    for (uint32_t nc_offset = 0; nc_offset < ctx.nc_count; ++nc_offset) {
        const uint32_t tile_offset = nc_offset * B32_ELEM_COUNT + output_offset;
        ctx.ub.grad[nc_offset] = ctx.ub.grad_tile[tile_offset];
        ctx.ub.argmax[nc_offset] = ctx.ub.argmax_tile[tile_offset];
    }
}

// 将一个输入空间位置上选中的grad值写回按[NC, input_spatial]排布的GM。
__aicore__ inline void store_selected_input(SimdContext& ctx, uint32_t input_index, uint32_t tile_index)
{
    asc_vf_call<scatter_selected_w>(
        ctx.ub.selected_tile + tile_index * NC_TILE, ctx.ub.scatter_offsets, ctx.ub.expanded, ctx.nc_count);
    wait_v_to_mte3();
    const uint64_t grad_input_offset = static_cast<uint64_t>(ctx.nc_start) * ctx.input_spatial + input_index;
    // expanded每个32B槽位存一个选中值，用于匹配跨NC的步长写回。
    asc_copy_ub2gm_align(
        ctx.tensors.grad_input + grad_input_offset, ctx.ub.expanded, static_cast<uint16_t>(ctx.nc_count), sizeof(float),
        asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, static_cast<uint64_t>(ctx.input_spatial) * sizeof(float),
        GM_BLOCK_BYTES);
    wait_mte3_to_v();
}

// 处理当前梯度所占区域覆盖的一段output行。
__aicore__ inline void process_input_row(SimdContext& ctx, uint32_t d, uint32_t h, uint32_t w_start, uint32_t w_end)
{
    const uint32_t row_base_input_index = (d * ctx.shape.input_h + h) * ctx.shape.input_w;
    for (uint32_t w = w_start; w < w_end; w += W_COMPARE_TILE) {
        const uint32_t tile_count = min(W_COMPARE_TILE, w_end - w);
        // 计算出当前w_tile块的output数据放到ctx.ub.selected_tile中。
        asc_vf_call<select_w_tile>(
            ctx.ub.grad, ctx.ub.argmax, ctx.ub.selected_tile, row_base_input_index + w, tile_count, ctx.nc_count);

        for (uint32_t tile_index = 0; tile_index < tile_count; ++tile_index) {
            // 把selected_tile的数据从UB中搬运到GM的对应位置。
            store_selected_input(ctx, row_base_input_index + w + tile_index, tile_index);
        }
    }
}

// 将一个梯度坐标换算为它在output上占据的区域，并处理该区域。
__aicore__ inline void process_output_window(SimdContext& ctx, uint32_t od_index, uint32_t oh_index, uint32_t ow_index)
{
    const uint32_t d_start = od_index * ctx.shape.input_d / ctx.shape.output_d;
    const uint32_t d_end = ((od_index + 1U) * ctx.shape.input_d + ctx.shape.output_d - 1U) / ctx.shape.output_d;
    const uint32_t h_start = oh_index * ctx.shape.input_h / ctx.shape.output_h;
    const uint32_t h_end = ((oh_index + 1U) * ctx.shape.input_h + ctx.shape.output_h - 1U) / ctx.shape.output_h;
    const uint32_t w_start = ow_index * ctx.shape.input_w / ctx.shape.output_w;
    const uint32_t w_end = ((ow_index + 1U) * ctx.shape.input_w + ctx.shape.output_w - 1U) / ctx.shape.output_w;

    for (uint32_t d = d_start; d < d_end; ++d) {
        for (uint32_t h = h_start; h < h_end; ++h) {
            process_input_row(ctx, d, h, w_start, w_end);
        }
    }
}

__aicore__ inline void process_task(SimdContext& ctx, uint32_t output_tile_index)
{
    const uint32_t ow_tile_index = output_tile_index % ctx.output_w_tile_count;
    output_tile_index /= ctx.output_w_tile_count;
    const uint32_t oh_index = output_tile_index % ctx.shape.output_h;
    const uint32_t od_index = output_tile_index / ctx.shape.output_h;
    const uint32_t ow_start_tile = ow_tile_index * OUTPUT_W_TILE;
    const uint32_t output_count = min(OUTPUT_W_TILE, ctx.shape.output_w - ow_start_tile);
    const uint32_t output_base_index = (od_index * ctx.shape.output_h + oh_index) * ctx.shape.output_w + ow_start_tile;
    // 将当前块的grad和argmax搬运到UB中，提升访问速度。
    copy_in_grad_argmax(ctx, output_base_index, output_count);
    wait_mte2_to_scalar();

    for (uint32_t output_offset = 0; output_offset < output_count; ++output_offset) {
        wait_v_to_scalar();
        extract_output_lanes(ctx, output_offset);
        wait_scalar_to_v();
        const uint32_t ow_index = ow_start_tile + output_offset;
        process_output_window(ctx, od_index, oh_index, ow_index);
    }
}

__aicore__ inline void process(SimdContext& ctx)
{
    init_scatter_offsets(ctx);
    // 将NC分块处理，NC_TILE = 64。
    for (uint32_t nc_tile_index = block_idx; nc_tile_index < ctx.nc_tile_count; nc_tile_index += block_num) {
        ctx.nc_start = nc_tile_index * NC_TILE;
        ctx.nc_count = min(NC_TILE, ctx.shape.nc - ctx.nc_start);
        asc_dcci_entire_all();
        // 将每个NC块继续按照输出W维度分块处理，OUTPUT_W_TILE = 8。
        for (uint32_t output_tile_index = 0; output_tile_index < ctx.output_tile_count; ++output_tile_index) {
            process_task(ctx, output_tile_index);
        }
    }
    asc_dcci_entire_all();
}

} // namespace simd_impl

// SIMD全局核函数入口：分配UB缓冲区，初始化上下文，并执行每个核的任务。
__global__ __vector__ void adaptive_max_pool3d_grad_simd_kernel(
    __gm__ uint8_t* grad, __gm__ uint8_t* argmax, __gm__ uint8_t* grad_input, uint32_t nc, uint32_t input_d,
    uint32_t input_h, uint32_t input_w, uint32_t output_d, uint32_t output_h, uint32_t output_w)
{
    asc_init();

    __gm__ float* grad_data = reinterpret_cast<__gm__ float*>(grad);
    __gm__ int32_t* argmax_data = reinterpret_cast<__gm__ int32_t*>(argmax);
    __gm__ float* input_data = reinterpret_cast<__gm__ float*>(grad_input);
    const simd_impl::GlobalTensors tensors{grad_data, argmax_data, input_data};
    const simd_impl::Pool3DShape shape{nc, input_d, input_h, input_w, output_d, output_h, output_w};

    __ubuf__ float grad_tile[simd_impl::NC_TILE * simd_impl::B32_ELEM_COUNT];
    __ubuf__ int32_t argmax_tile[simd_impl::NC_TILE * simd_impl::B32_ELEM_COUNT];
    __ubuf__ float grad_ub[simd_impl::NC_TILE];
    __ubuf__ int32_t argmax_ub[simd_impl::NC_TILE];
    __ubuf__ uint32_t scatter_offsets[simd_impl::NC_TILE];
    __ubuf__ float selected_tile[simd_impl::W_COMPARE_TILE * simd_impl::NC_TILE];
    __ubuf__ float expanded[simd_impl::NC_TILE * simd_impl::B32_ELEM_COUNT];
    const simd_impl::UbBuffers ub{grad_tile, argmax_tile, grad_ub, argmax_ub, scatter_offsets, selected_tile, expanded};

    simd_impl::SimdContext ctx;
    simd_impl::init_context(ctx, tensors, shape, ub);
    simd_impl::process(ctx);
}
