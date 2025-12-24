/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_copy_l12l0b_trans_impl.h
 * \brief
 */

#ifndef IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_COPY_L12L0B_TRANS_IMPL_NPU_ARCH_2201_ASC_COPY_L12L0B_TRANS_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_COPY_L12L0B_TRANS_IMPL_NPU_ARCH_2201_ASC_COPY_L12L0B_TRANS_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void copy_l12l0b_trans_impl(__cb__ half* dst, __cbuf__ half* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t frac_stride)
{   
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_stride, addrmode, frac_stride);
    }
}

__aicore__ inline void copy_l12l0b_trans_impl(__cb__ float* dst, __cbuf__ float* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint64_t frac_stride)
{   
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_stride, addrmode, frac_stride);
    }
}

__aicore__ inline void copy_l12l0b_trans_impl(__cb__ int32_t* dst, __cbuf__ int32_t* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint64_t frac_stride)
{   
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_stride, addrmode, frac_stride);
    }
}

__aicore__ inline void copy_l12l0b_trans_impl(__cb__ int8_t* dst, __cbuf__ int8_t* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint64_t frac_stride)
{   
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_stride, addrmode, frac_stride);
    }
}

__aicore__ inline void copy_l12l0b_trans_impl(__cb__ uint32_t* dst, __cbuf__ uint32_t* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint64_t frac_stride)
{   
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_stride, addrmode, frac_stride);
    }
}

__aicore__ inline void copy_l12l0b_trans_impl(__cb__ uint8_t* dst, __cbuf__ uint8_t* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint64_t frac_stride)
{   
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_stride, addrmode, frac_stride);
    }
}

#endif