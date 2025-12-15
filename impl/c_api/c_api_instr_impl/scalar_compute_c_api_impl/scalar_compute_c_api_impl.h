
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef IMPL_C_API_C_API_INSTR_IMPL_SCALAR_COMPUTE_C_API_IMPL_SCALAR_COMPUTE_C_API_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_SCALAR_COMPUTE_C_API_IMPL_SCALAR_COMPUTE_C_API_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"
#include "ffs_impl/npu_arch_2201/ffs_impl.h"
#include "sflbits_impl/npu_arch_2201/asc_sflbits_impl.h"

__aicore__ inline int64_t asc_ffs(uint64_t in)
{
    return CApiInternal::asc_ffs_impl(in);
}

__aicore__ inline int64_t asc_sflbits(int64_t value)
{
    return CApiInternal::asc_sflbits_impl(value);
}

#endif