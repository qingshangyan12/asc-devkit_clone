/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_CUBE_DATAMOVE_C_API_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_CUBE_DATAMOVE_C_API_IMPL_H
 
#include "get_fpc_impl/asc_2201/get_fpc_impl.h"
#include "set_fmatrix_impl/asc_2201/set_fmatrix_impl.h"
#include "set_fmatrix_b_impl/asc_2201/set_fmatrix_b_impl.h"
#include "set_fpc_impl/asc_2201/set_fpc_impl.h"
#include "set_l1_3d_size_impl/asc_2201/set_l1_3d_size_impl.h"
#include "set_l3d_rpt_impl/asc_2201/set_l3d_rpt_impl.h"
#include "set_nd_para_impl/asc_2201/set_nd_para_impl.h"
#include "set_padding_impl/asc_2201/set_padding_impl.h"
#include "set_quant_pre_impl/asc_2201/set_quant_pre_impl.h"
#include "set_mm_layout_transform_impl/asc_2201/set_mm_layout_transform_impl.h"
#include "set_l3d_rpt_impl/asc_2201/set_l3d_rpt_impl.h"

__aicore__ inline void asc_set_mm_layout_transform(bool mmLayoutMode)
{
    CApiInternal::set_mm_layout_transform_impl(mmLayoutMode);
}

__aicore__ inline void asc_set_l1_3d_rpt(uint64_t value)
{
    CApiInternal::set_l1_3d_rpt_impl(value);
}

#endif