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
#include "set_hf32_trans_mode_impl/asc_2201/asc_set_hf32_trans_mode_impl.h"

__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config)
{
    CApiInternal::asc_set_l0c_copy_prequant_impl(config);
}

__aicore__ inline void asc_set_hf32_trans_mode(bool hf32_transmode)
{
    CApiInternal::asc_set_hf32_trans_mode_impl(hf32_transmode);
}
 
#endif