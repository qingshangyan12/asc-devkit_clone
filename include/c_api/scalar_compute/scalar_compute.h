/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef INCLUDE_C_API_SCALAR_COMPUTE_H
#define INCLUDE_C_API_SCALAR_COMPUTE_H
 
#include "c_api_instr_impl/scalar_compute_c_api_impl/scalar_compute_c_api_impl.h"

__aicore__ inline int64_t asc_ffs(uint64_t value);

__aicore__ inline int64_t asc_clz(uint64_t value_in);

__aicore__ inline int64_t asc_sflbits(int64_t value);

__aicore__ inline int64_t asc_ffz(uint64_t value);

__aicore__ inline int64_t asc_zero_bits_cnt(uint64_t value);

__aicore__ inline uint64_t asc_set_nthbit(uint64_t bits, int64_t idx);
 
#endif