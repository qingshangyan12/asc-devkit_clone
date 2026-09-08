/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/cube_datamove_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/common_impl.h"

__aicore__ inline void asc_set_l0c2gm_config_impl(uint64_t relu_pre, uint64_t quant_pre, bool enable_unit_flag)
{
    if ASC_IS_AIC {
        asc_capi_fpc_reg_config config;
        config.config = 0;
        config.relu_units = relu_pre;
        config.quant_units = quant_pre;
        config.unit_flag = enable_unit_flag;
        set_fpc(config.config);
    }
}

__aicore__ inline uint64_t asc_get_l0c2gm_relu_impl()
{
    if ASC_IS_AIC {
        asc_capi_fpc_reg_config config;
        config.config = get_fpc();
        return static_cast<uint64_t>(config.relu_units);
    }
    return 0;
}

__aicore__ inline uint64_t asc_get_l0c2gm_prequant_impl()
{
    if ASC_IS_AIC {
        asc_capi_fpc_reg_config config;
        config.config = get_fpc();
        return static_cast<uint64_t>(config.quant_units);
    }
    return 0;
}

__aicore__ inline uint64_t asc_get_l0c2gm_unitflag_impl()
{
    if ASC_IS_AIC {
        asc_capi_fpc_reg_config config;
        config.config = get_fpc();
        return static_cast<uint64_t>(config.unit_flag);
    }
    return 0;
}

__aicore__ inline void asc_copy_l12l0b_sparse_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat)
{
    if ASC_IS_AIC {
        asc_sparse_addr_config addr_config;
        addr_config.src = (uint64_t)src & 0xffffffff;
        addr_config.index = (uint64_t)index & 0xffffffff;
        __cbuf__ int8_t* src_addr = reinterpret_cast<__cbuf__ int8_t*>(addr_config.config);
        load_cbuf_to_cb_sp(dst, src_addr, start_index, repeat);
    }
}

__aicore__ inline void asc_copy_l12bt_impl(
    uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt_impl(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

__aicore__ inline void asc_set_l13d_rpt_impl(asc_load3d_v2_config& config)
{
    if ASC_IS_AIC {
        set_l3d_rpt(config.config);
    }
}

__aicore__ inline void asc_set_l13d_padding_impl(uint64_t config)
{
    if ASC_IS_AIC {
        uint64_t paddingValue = 0;
        uint64_t padValueShiftBit = 8;
        paddingValue = (config << padValueShiftBit) | (config & 0xFF);
        set_padding(paddingValue);
    }
}

__aicore__ inline void asc_set_l13d_padding_impl(half config)
{
    if ASC_IS_AIC {
        asc_scalar_bitcode bitcode;
        bitcode.input_half = config;
        set_padding(bitcode.output);
    }
}

__aicore__ inline void asc_set_l13d_padding_impl(int16_t config)
{
    if ASC_IS_AIC {
        asc_scalar_bitcode bitcode;
        bitcode.input_int16_t = config;
        set_padding(bitcode.output);
    }
}

__aicore__ inline void asc_set_l13d_padding_impl(uint16_t config)
{
    if ASC_IS_AIC {
        asc_scalar_bitcode bitcode;
        bitcode.input_uint16_t = config;
        set_padding(bitcode.output);
    }
}

__aicore__ inline void set_l0c_copy_params_impl(uint16_t nd_num, uint16_t src_nd_stride, uint16_t dst_nd_stride)
{
    if ASC_IS_AIC {
        asc_set_l0c_copy_params_config config;
        config.nd_num = nd_num;
        config.src_nd_stride = src_nd_stride;
        config.dst_nd_stride = dst_nd_stride;
        set_nd_para(config.config);
    }
}

__aicore__ inline void asc_set_l0c_copy_prequant_impl(uint64_t config)
{
    if ASC_IS_AIC {
        set_quant_pre(config);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0a_trans_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0a_trans_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0a_trans_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0a_trans_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0a_trans_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0a_trans_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0a_trans_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_transpose(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
    }
}

__aicore__ inline void asc_set_l13d_size_impl(uint64_t value)
{
    if ASC_IS_AIC {
        set_l1_3d_size(value);
    }
}

// asc_load_image_to_cbuf half
__aicore__ inline void asc_load_image_to_cbuf_impl(
    __cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size)
{
    if ASC_IS_AIC {
        load_image_to_cbuf(
            dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_size, top_pad_size, bot_pad_size,
            left_pad_size, right_pad_size, 0);
    }
}

// asc_load_image_to_cbuf int8_t
__aicore__ inline void asc_load_image_to_cbuf_impl(
    __cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size)
{
    if ASC_IS_AIC {
        load_image_to_cbuf(
            dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_size, top_pad_size, bot_pad_size,
            left_pad_size, right_pad_size, 0);
    }
}

// asc_fill_l0a half
__aicore__ inline void asc_fill_l0a_impl(__ca__ half* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_impl(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

// asc_fill_l0a float
__aicore__ inline void asc_fill_l0a_impl(__ca__ float* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_impl(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

// asc_fill_l0a int16_t
__aicore__ inline void asc_fill_l0a_impl(__ca__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_impl(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

// asc_fill_l0a int32_t
__aicore__ inline void asc_fill_l0a_impl(__ca__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_impl(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

// asc_fill_l0a uint16_t
__aicore__ inline void asc_fill_l0a_impl(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_impl(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

// asc_fill_l0a uint32_t
__aicore__ inline void asc_fill_l0a_impl(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_impl(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

// asc_fill_l0a bfloat16_t
__aicore__ inline void asc_fill_l0a_impl(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix_bf16(dst, config.config, value);
    }
}

// asc_fill_l0a bfloat16_t-half
__aicore__ inline void asc_fill_l0a_impl(__ca__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix_h(dst, config.config, value);
    }
}

// asc_fill_l0a bfloat16_t-uint32_t
__aicore__ inline void asc_fill_l0a_impl(__ca__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix_ui(dst, config.config, value);
    }
}

// asc_fill_l0b_value half
__aicore__ inline void asc_fill_l0b_impl(__cb__ half* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0b_impl(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

// asc_fill_l0b_value float
__aicore__ inline void asc_fill_l0b_impl(__cb__ float* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0b_impl(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

// asc_fill_l0b_value int16_t
__aicore__ inline void asc_fill_l0b_impl(__cb__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0b_impl(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

// asc_fill_l0b_value int32_t
__aicore__ inline void asc_fill_l0b_impl(__cb__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0b_impl(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

// asc_fill_l0b_value uint16_t
__aicore__ inline void asc_fill_l0b_impl(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0b_impl(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

// asc_fill_l0b_value uint32_t
__aicore__ inline void asc_fill_l0b_impl(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0b_impl(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix(dst, config.config, value);
    }
}

// asc_fill_l0b_value bfloat16_t
__aicore__ inline void asc_fill_l0b_impl(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix_bf16(dst, config.config, value);
    }
}

// asc_fill_l0b bfloat16_t-half
__aicore__ inline void asc_fill_l0b_impl(__cb__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix_h(dst, config.config, value);
    }
}

// asc_fill_l0b bfloat16_t-uint32_t
__aicore__ inline void asc_fill_l0b_impl(__cb__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cb_matrix_ui(dst, config.config, value);
    }
}

// asc_fill_l1_value half
__aicore__ inline void asc_fill_l1_impl(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value float
__aicore__ inline void asc_fill_l1_impl(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value int16_t
__aicore__ inline void asc_fill_l1_impl(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value int32_t
__aicore__ inline void asc_fill_l1_impl(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value uint16_t
__aicore__ inline void asc_fill_l1_impl(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value uint32_t
__aicore__ inline void asc_fill_l1_impl(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value bfloat16_t
__aicore__ inline void asc_fill_l1_impl(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_bf16(dst, config.config, value);
    }
}

// create_cbuf_matrix_h
__aicore__ inline void asc_fill_l1_impl(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_h(dst, config.config, value);
    }
}

// create_cbuf_matrix_ui
__aicore__ inline void asc_fill_l1_impl(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_ui(dst, config.config, value);
    }
}

__aicore__ inline void asc_copy_l12fb_impl(
    __fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap_size,
    uint16_t dst_gap_size)
{
    if ASC_IS_AIC {
        copy_cbuf_to_fbuf(dst, src, n_burst, len_burst, src_gap_size, dst_gap_size);
    }
}

__aicore__ inline void asc_copy_l12fb_impl(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// PAD_NONE
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf(dst, src, 0, n_burst, len_burst, src_gap, dst_gap, PAD_NONE);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// PAD_MODE1
__aicore__ inline void asc_copy_gm2l1_pad1_impl(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf(dst, src, 0, n_burst, len_burst, src_gap, dst_gap, PAD_MODE1);
    }
}

__aicore__ inline void asc_copy_gm2l1_pad1_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad1_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// PAD_MODE2
__aicore__ inline void asc_copy_gm2l1_pad2_impl(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf(dst, src, 0, n_burst, len_burst, src_gap, dst_gap, PAD_MODE2);
    }
}

__aicore__ inline void asc_copy_gm2l1_pad2_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad2_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// PAD_MODE3
__aicore__ inline void asc_copy_gm2l1_pad3_impl(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf(dst, src, 0, n_burst, len_burst, src_gap, dst_gap, PAD_MODE3);
    }
}

__aicore__ inline void asc_copy_gm2l1_pad3_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad3_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// PAD_MODE4
__aicore__ inline void asc_copy_gm2l1_pad4_impl(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf(dst, src, 0, n_burst, len_burst, src_gap, dst_gap, PAD_MODE4);
    }
}

__aicore__ inline void asc_copy_gm2l1_pad4_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad4_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// PAD_MODE5
__aicore__ inline void asc_copy_gm2l1_pad5_impl(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf(dst, src, 0, n_burst, len_burst, src_gap, dst_gap, PAD_MODE5);
    }
}

__aicore__ inline void asc_copy_gm2l1_pad5_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad5_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// PAD_MODE6
__aicore__ inline void asc_copy_gm2l1_pad6_impl(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf(dst, src, 0, n_burst, len_burst, src_gap, dst_gap, PAD_MODE6);
    }
}

__aicore__ inline void asc_copy_gm2l1_pad6_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad6_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// PAD_MODE7
__aicore__ inline void asc_copy_gm2l1_pad7_impl(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf(dst, src, 0, n_burst, len_burst, src_gap, dst_gap, PAD_MODE7);
    }
}

__aicore__ inline void asc_copy_gm2l1_pad7_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad7_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// PAD_MODE8
__aicore__ inline void asc_copy_gm2l1_pad8_impl(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf(dst, src, 0, n_burst, len_burst, src_gap, dst_gap, PAD_MODE8);
    }
}

__aicore__ inline void asc_copy_gm2l1_pad8_impl(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad8_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf(dst, src, base_idx, repeat, src_stride, dst_gap, static_cast<uint8_t>(0), addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf(dst, src, base_idx, repeat, src_stride, dst_gap, static_cast<uint8_t>(0), addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf(dst, src, base_idx, repeat, src_stride, dst_gap, static_cast<uint8_t>(0), addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf(dst, src, base_idx, repeat, src_stride, dst_gap, static_cast<uint8_t>(0), addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf(dst, src, base_idx, repeat, src_stride, dst_gap, static_cast<uint8_t>(0), addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf(dst, src, base_idx, repeat, src_stride, dst_gap, static_cast<uint8_t>(0), addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf(dst, src, base_idx, repeat, src_stride, dst_gap, static_cast<uint8_t>(0), addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l0a_impl(
    __ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_ca(dst, src, base_idx, repeat, src_stride, dst_gap, 0, inc);
    }
}

__aicore__ inline void asc_copy_gm2l0a_impl(
    __ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_ca(dst, src, base_idx, repeat, src_stride, dst_gap, 0, inc);
    }
}

__aicore__ inline void asc_copy_gm2l0a_impl(
    __ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_ca(dst, src, base_idx, repeat, src_stride, dst_gap, 0, inc);
    }
}

__aicore__ inline void asc_copy_gm2l0a_impl(
    __ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_ca(dst, src, base_idx, repeat, src_stride, dst_gap, 0, inc);
    }
}

__aicore__ inline void asc_copy_gm2l0a_impl(
    __ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_ca(dst, src, base_idx, repeat, src_stride, dst_gap, 0, inc);
    }
}

__aicore__ inline void asc_copy_gm2l0a_impl(
    __ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_ca(dst, src, base_idx, repeat, src_stride, dst_gap, 0, inc);
    }
}

__aicore__ inline void asc_copy_gm2l0a_impl(
    __ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_ca(dst, src, base_idx, repeat, src_stride, dst_gap, 0, inc);
    }
}

__aicore__ inline void asc_copy_gm2l0a_impl(
    __ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_ca_s4((__ca__ void*)dst, (__gm__ void*)src, base_idx, repeat, src_stride, dst_gap, 0, inc);
    }
}

// copy_gm2l1_nd2nz_b8 int8_t
__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz_b8(
            dst, src, 0, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
            dst_nz_matrix_stride);
    }
}

// copy_gm2l1_nd2nz_b8 uint8_t
__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz_b8(
            dst, src, 0, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
            dst_nz_matrix_stride);
    }
}

// copy_gm2l1_nd2nz_b16 bfloat16
__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz_b16(
            dst, src, 0, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
            dst_nz_matrix_stride);
    }
}

// copy_gm2l1_nd2nz_b16 half
__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ half* dst, __gm__ half* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz_b16(
            dst, src, 0, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
            dst_nz_matrix_stride);
    }
}

// copy_gm2l1_nd2nz_b16 int16_t
__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz_b16(
            dst, src, 0, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
            dst_nz_matrix_stride);
    }
}

// copy_gm2l1_nd2nz_b32s float
__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ float* dst, __gm__ float* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz_b32s(
            dst, src, 0, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
            dst_nz_matrix_stride);
    }
}

// copy_gm2l1_nd2nz_b32s int32_t
__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz_b32s(
            dst, src, 0, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
            dst_nz_matrix_stride);
    }
}

// copy_gm2l1_nd2nz_b32s uint32_t
__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz_b32s(
            dst, src, 0, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
            dst_nz_matrix_stride);
    }
}

__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, unit_flag_mode, static_cast<QuantMode_t>(quant_pre),
            relu_pre, enable_channel_split, enable_nz2nd);
    }
}

__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, unit_flag_mode, static_cast<QuantMode_t>(quant_pre),
            relu_pre, enable_channel_split, enable_nz2nd);
    }
}

__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, unit_flag_mode, static_cast<QuantMode_t>(quant_pre),
            relu_pre, enable_channel_split, enable_nz2nd);
    }
}

__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, unit_flag_mode, static_cast<QuantMode_t>(quant_pre),
            relu_pre, enable_channel_split, enable_nz2nd);
    }
}

__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, unit_flag_mode, static_cast<QuantMode_t>(quant_pre),
            relu_pre, enable_channel_split, enable_nz2nd);
    }
}

__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, unit_flag_mode, static_cast<QuantMode_t>(quant_pre),
            relu_pre, enable_channel_split, enable_nz2nd);
    }
}

__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, unit_flag_mode, static_cast<QuantMode_t>(quant_pre),
            relu_pre, enable_channel_split, enable_nz2nd);
    }
}

__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_b4(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, unit_flag_mode, static_cast<QuantMode_t>(quant_pre),
            relu_pre, enable_channel_split, enable_nz2nd);
    }
}

__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_b4(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, unit_flag_mode, static_cast<QuantMode_t>(quant_pre),
            relu_pre, enable_channel_split, enable_nz2nd);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca_s4(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap, 0, false, inc);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre), relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre), relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre), relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre), relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre), relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre), relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre), relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre), relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre), relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_set_l13d_fmatrix_impl(asc_l13d_fmatrix_config& config)
{
    if ASC_IS_AIC {
        set_fmatrix(config.config);
    }
}

__aicore__ inline void asc_copy_l12gm_impl(
    __gm__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_gm(dst, src, 0, n_burst, len_burst, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12l0c_impl(
    __cc__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_matrix_cbuf_to_cc(dst, src, n_burst, len_burst, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12l0c_impl(
    __cc__ half* dst, __cbuf__ half* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_matrix_cbuf_to_cc(dst, src, n_burst, len_burst, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12l0c_impl(
    __cc__ half* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_matrix_cbuf_to_cc(dst, src, n_burst, len_burst, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12l0c_impl(
    __cc__ bfloat16_t* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_matrix_cbuf_to_cc(dst, src, n_burst, len_burst, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12l0c_impl(
    __cc__ float* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_matrix_cbuf_to_cc(dst, src, n_burst, len_burst, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12l0c_impl(
    __cc__ int32_t* dst, __cbuf__ int32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_matrix_cbuf_to_cc(dst, src, n_burst, len_burst, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12l0c_impl(
    __cc__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_matrix_cbuf_to_cc(dst, src, n_burst, len_burst, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_gm2l0b_impl(
    __cb__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cb(dst, src, base_idx, repeat, src_stride, dst_gap, 0, addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l0b_impl(
    __cb__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cb(dst, src, base_idx, repeat, src_stride, dst_gap, 0, addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l0b_impl(
    __cb__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cb(dst, src, base_idx, repeat, src_stride, dst_gap, 0, addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l0b_impl(
    __cb__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cb(dst, src, base_idx, repeat, src_stride, dst_gap, 0, addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l0b_impl(
    __cb__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cb(dst, src, base_idx, repeat, src_stride, dst_gap, 0, addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l0b_impl(
    __cb__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cb(dst, src, base_idx, repeat, src_stride, dst_gap, 0, addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l0b_impl(
    __cb__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cb(dst, src, base_idx, repeat, src_stride, dst_gap, 0, addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_copy_gm2l0b_impl(
    __cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        load_gm_to_cb_s4(dst, src, base_idx, repeat, src_stride, dst_gap, 0, addr_cal_mode_t::inc);
    }
}

__aicore__ inline void asc_set_l13d_fmatrix_b_impl(asc_l13d_fmatrix_config& config)
{
    if ASC_IS_AIC {
        set_fmatrix_b(config.config);
    }
}

__aicore__ inline void asc_set_l0c2gm_lrelu_alpha_impl(half& config)
{
    if ASC_IS_AIC {
        set_lrelu_alpha(config);
    }
}

__aicore__ inline void asc_set_l0c2gm_lrelu_alpha_impl(float& config)
{
    if ASC_IS_AIC {
        set_lrelu_alpha(config);
    }
}

__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre, uint64_t quant_pre, bool enable_unit_flag)
{
    asc_set_l0c2gm_config_impl(relu_pre, quant_pre, enable_unit_flag);
}

__aicore__ inline uint64_t asc_get_l0c2gm_relu() { return asc_get_l0c2gm_relu_impl(); }

__aicore__ inline uint64_t asc_get_l0c2gm_unitflag() { return asc_get_l0c2gm_unitflag_impl(); }

__aicore__ inline uint64_t asc_get_l0c2gm_prequant() { return asc_get_l0c2gm_prequant_impl(); }

__aicore__ inline void asc_copy_l12l0b_sparse(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat)
{
    asc_copy_l12l0b_sparse_impl(dst, src, index, start_index, repeat);
}

__aicore__ inline void asc_set_l13d_rpt(asc_load3d_v2_config& config) { asc_set_l13d_rpt_impl(config); }

// ==========asc_copy_l12l0b, 2D, int4b_t/int8_t/uint8_t/half/bfloat16_t/int32_t/uint32_t/float==========
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

// ==========asc_copy_l12l0b, 3D, half/bfloat16_t/int32_t/uint32_t/float==========
__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

// ==========asc_copy_l12l0a, 2D, int4b_t/int8_t/uint8_t/half/bfloat16_t/int32_t/uint32_t/float==========
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_l12l0a_impl(dst, src, start_index, repeat, src_stride, dst_gap);
}

// ==========asc_copy_l12l0a, 3D, int4b_t*/int8_t/uint8_t/half/bfloat16_t/int32_t/uint32_t/float==========
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

// ==========asc_l0c_copy_params()==========
__aicore__ inline void asc_set_l0c_copy_params(uint16_t nd_num, uint16_t src_nd_stride, uint16_t dst_nd_stride)
{
    set_l0c_copy_params_impl(nd_num, src_nd_stride, dst_nd_stride);
}

__aicore__ inline void asc_set_l13d_size(uint64_t value) { asc_set_l13d_size_impl(value); }

// ==========asc_load_image_to_cbuf(half/int8_t)==========
__aicore__ inline void asc_load_image_to_cbuf(
    __cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size)
{
    asc_load_image_to_cbuf_impl(
        dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_size, top_pad_size, bot_pad_size, left_pad_size,
        right_pad_size);
}

__aicore__ inline void asc_load_image_to_cbuf(
    __cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size)
{
    asc_load_image_to_cbuf_impl(
        dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_size, top_pad_size, bot_pad_size, left_pad_size,
        right_pad_size);
}

// ==========asc_fill_l0a(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l0a(__ca__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a(__ca__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_impl(dst, value, config);
}

// ==========asc_fill_l0b(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l0b(__cb__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_impl(dst, value, config);
}

// ==========asc_fill_l1(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l1(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

// ==========asc_copy_l12l0a_trans=========
__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0a_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

// ==========asc_copy_l12l0b_trans=========
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
}

// ==========asc_copy_gm2l1==========
// PAD_NONE
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_impl(dst, src, size);
}

// PAD_MODE1
__aicore__ inline void asc_copy_gm2l1_pad1(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_gm2l1_pad1_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1_pad1(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad1_impl(dst, src, size);
}

// PAD_MODE2
__aicore__ inline void asc_copy_gm2l1_pad2(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_gm2l1_pad2_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1_pad2(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad2_impl(dst, src, size);
}

// PAD_MODE3
__aicore__ inline void asc_copy_gm2l1_pad3(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_gm2l1_pad3_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1_pad3(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad3_impl(dst, src, size);
}

// PAD_MODE4
__aicore__ inline void asc_copy_gm2l1_pad4(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_gm2l1_pad4_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1_pad4(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad4_impl(dst, src, size);
}

// PAD_MODE5
__aicore__ inline void asc_copy_gm2l1_pad5(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_gm2l1_pad5_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1_pad5(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad5_impl(dst, src, size);
}

// PAD_MODE6
__aicore__ inline void asc_copy_gm2l1_pad6(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_gm2l1_pad6_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1_pad6(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad6_impl(dst, src, size);
}

// PAD_MODE7
__aicore__ inline void asc_copy_gm2l1_pad7(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_gm2l1_pad7_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1_pad7(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad7_impl(dst, src, size);
}

// PAD_MODE8
__aicore__ inline void asc_copy_gm2l1_pad8(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_gm2l1_pad8_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1_pad8(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2l1_pad8_impl(dst, src, size);
}

// ==========asc_copy_gm2l1_nd2nz b8(int8_t/uint8_t)==========
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

// ==========asc_copy_gm2l1_nd2nz b16(bfloat16_t/half/int16_t)==========
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

// ==========asc_copy_gm2l1_nd2nz b32s(float/int32_t/uint32_t)==========
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride,
        dst_nz_matrix_stride);
}

//=============asc_copy_l12bt===============
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

//=============asc_copy_l12fb===============
__aicore__ inline void asc_copy_l12fb(
    __fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap_size,
    uint16_t dst_gap_size)
{
    asc_copy_l12fb_impl(dst, src, n_burst, len_burst, src_gap_size, dst_gap_size);
}

__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_impl(dst, src, size);
}

__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config) { asc_set_l0c_copy_prequant_impl(config); }

__aicore__ inline void asc_set_l13d_padding(uint64_t config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l13d_padding(half config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l13d_padding(int16_t config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l13d_padding(uint16_t config) { asc_set_l13d_padding_impl(config); }

// ==========asc_copy_l0c2gm==========
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride_dst_d, src_stride, unit_flag_mode, quant_pre, relu_pre, channel_split,
        nz2nd_en);
}

// ==========asc_copy_l0c2l1==========
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split,
        enable_nz2nd);
}

__aicore__ inline void asc_set_l13d_fmatrix(asc_l13d_fmatrix_config& config) { asc_set_l13d_fmatrix_impl(config); }

// ==========asc_copy_gm2l0a  bfloat16_t/half/float/int32_t/uint8_t/int8_t/uint32_t/===========
__aicore__ inline void asc_copy_gm2l0a(
    __ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0a(
    __ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0a(
    __ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0a(
    __ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0a(
    __ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0a(
    __ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0a(
    __ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0a(
    __ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0a_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

// ==========asc_copy_l12gm==========
__aicore__ inline void asc_copy_l12gm(
    __gm__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12gm_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

// ==========asc_copy_l12l0c==========
__aicore__ inline void asc_copy_l12l0c(
    __cc__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12l0c(
    __cc__ half* dst, __cbuf__ half* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12l0c(
    __cc__ half* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12l0c(
    __cc__ bfloat16_t* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12l0c(
    __cc__ float* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12l0c(
    __cc__ int32_t* dst, __cbuf__ int32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12l0c(
    __cc__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_l12l0c_impl(dst, src, n_burst, len_burst, src_gap, dst_gap);
}

// ==========asc_copy_gm2l0b==========
__aicore__ inline void asc_copy_gm2l0b(
    __cb__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0b(
    __cb__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0b(
    __cb__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0b(
    __cb__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0b(
    __cb__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0b(
    __cb__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0b(
    __cb__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l0b(
    __cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l0b_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

// ==========asc_copy_gm2l1==========
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap)
{
    asc_copy_gm2l1_impl(dst, src, base_idx, repeat, src_stride, dst_gap);
}

__aicore__ inline void asc_set_l13d_fmatrix_b(asc_l13d_fmatrix_config& config) { asc_set_l13d_fmatrix_b_impl(config); }

__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(half& config) { asc_set_l0c2gm_lrelu_alpha_impl(config); }

__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(float& config) { asc_set_l0c2gm_lrelu_alpha_impl(config); }

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H
