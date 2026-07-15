/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef INCLUDE_C_API_UTILS_C_API_ENUM_H
#define INCLUDE_C_API_UTILS_C_API_ENUM_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#include <cstdint>

enum acs_load_l2_cache_type : uint8_t {
    ASC_NORMAL_FIRST_VICTIM = 0,
    ASC_NORMAL_LAST_VICTIM = 1,
    ASC_NORMAL_PERSISTENT = 2,
    ASC_NOTALLOC_KEEP = 4,
    ASC_NOTALLOC_CLEAN = 5,
    ASC_NOTALLOC_DROP = 6
};

// enum acs_store_l2_cache_type : uint8_t {
//     ASC_NORMAL_FIRST_VICTIM = 0,
//     ASC_NORMAL_LAST_VICTIM = 1,
//     ASC_NORMAL_PERSISTENT = 2,
//     ASC_NOTALLPC_CLEAN = 4
// };

#endif
