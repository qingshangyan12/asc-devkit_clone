/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_COPY_L12L0B_IMPL_SP_NPU_ARCH_2201_COPY_L12L0B_SP_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_COPY_L12L0B_IMPL_SP_NPU_ARCH_2201_COPY_L12L0B_SP_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_copy_l12l0b_sparse_impl(__cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat)
{
    if ASC_IS_AIC {
        asc_sparse_addr_config addr_config;
        addr_config.src = (uint64_t)src & 0xffffffff;
        addr_config.index = (uint64_t)index & 0xffffffff;
        __cbuf__ int8_t *src_addr = reinterpret_cast<__cbuf__ int8_t *>(addr_config.config);
        load_cbuf_to_cb_sp(dst, src_addr, start_index, repeat);
    }
}

__aicore__ inline void asc_copy_l12l0b_sparse_sync_impl(__cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat)
{
    asc_copy_l12l0b_sparse_impl(dst, src, index, start_index, repeat);
    asc_sync_post_process();
}

#endif