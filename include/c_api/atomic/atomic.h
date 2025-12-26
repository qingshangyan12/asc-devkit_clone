/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef INCLUDE_C_API_ATOMIC_H
#define INCLUDE_C_API_ATOMIC_H
 
#include "c_api_instr_impl/atomic_c_api_impl/atomic_c_api_impl.h"

__aicore__ inline void asc_set_atomic_max();

__aicore__ inline void asc_set_atomic_s32();

__aicore__ inline void asc_set_atomic_add();

__aicore__ inline void asc_set_atomic_none();

__aicore__ inline void asc_set_atomic_s8();

__aicore__ inline void asc_set_atomic_bf16();

__aicore__ inline void asc_get_store_atomic_config(asc_store_atomic_config& config);
#endif