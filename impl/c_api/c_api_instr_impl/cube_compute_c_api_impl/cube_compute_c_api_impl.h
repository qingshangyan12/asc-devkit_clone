/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef IMPL_C_API_C_API_INSTR_IMPL_CUBE_COMPUTE_C_API_IMPL_CUBE_COMPUTE_C_API_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_CUBE_COMPUTE_C_API_IMPL_CUBE_COMPUTE_C_API_IMPL_H

#include "set_l0a_2d_impl/asc_2201/set_l0a_2d_impl.h"
#include "set_l0b_2d_impl/asc_2201/set_l0b_2d_impl.h"
#include "set_l0c_copy_prequant_impl/npu_arch_2201/set_l0c_copy_prequant_impl.h"
#include "set_l1_2d_impl/asc_2201/set_l1_2d_impl.h"
#include "set_hf32_trans2even_impl/npu_arch_2201/asc_set_hf32_trans2even_impl.h"
#include "set_hf32_trans2zero_impl/npu_arch_2201/asc_set_hf32_trans2zero_impl.h"
#include "clz_impl/npu_arch_2201/asc_clz_impl.h"
#include "mad_sp_impl/npu_arch_2201/asc_mmad_sparse_impl.h"
#include "set_hf32_mode_impl/npu_arch_2201/asc_set_hf32_mode_impl.h"
#include "set_fp32_mode_impl/npu_arch_2201/asc_set_fp32_mode_impl.h"

__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config)
{
    asc_set_l0c_copy_prequant_impl(config);
}

__aicore__ inline void asc_set_hf32_trans2even()
{
    asc_set_hf32_trans2even_impl();
}

__aicore__ inline void asc_set_hf32_trans2zero()
{
    asc_set_hf32_trans2zero_impl();
}

__aicore__ inline int64_t asc_clz(uint64_t value_in)
{
    return asc_clz_impl(value_in);
}

__aicore__ inline void asc_mmad_sparse(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m, uint16_t k, uint16_t n, uint8_t unitFlag, bool cmatrixSource, bool cmatrixInitVal)
{
    asc_mmad_sparse_impl(c, a, b, m, k, n, unitFlag, cmatrixSource, cmatrixInitVal);
}

__aicore__ inline void asc_mmad_sparse_sync(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m, uint16_t k, uint16_t n, uint8_t unitFlag, bool cmatrixSource, bool cmatrixInitVal)
{
    asc_mmad_sparse_sync_impl(c, a, b, m, k, n, unitFlag, cmatrixSource, cmatrixInitVal);
}

__aicore__ inline void asc_set_hf32_mode()
{
    asc_set_hf32_mode_impl();
}

__aicore__ inline void asc_set_fp32_mode()
{
    asc_set_fp32_mode_impl();
}

#endif