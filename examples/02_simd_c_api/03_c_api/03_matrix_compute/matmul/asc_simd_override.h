/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#pragma once

// The 9.0 compiler pre-includes its compatibility C API headers.  Remove
// their include guards so this checkout's dav-3510 declarations are used.
#undef INCLUDE_C_API_ASC_SIMD_H
#undef INCLUDE_C_API_CUBE_COMPUTE_CUBE_COMPUTE_H
#undef INCLUDE_C_API_CUBE_DATAMOVE_CUBE_DATAMOVE_H
#undef INCLUDE_C_API_SYS_VAR_SYS_VAR_H
#undef INCLUDE_C_API_SYNC_SYNC_H
#undef INCLUDE_C_API_MISC_H
#include "../../../../../impl/utils/common_types.h"
#include "../../../../../include/c_api/utils/enum.h"
#include "../../../../../include/c_api/cube_compute/cube_compute.h"
#include "../../../../../include/c_api/cube_datamove/cube_datamove.h"
#include "../../../../../include/c_api/misc/misc.h"
#include "../../../../../include/c_api/sync/sync.h"
#include "../../../../../include/c_api/sys_var/sys_var.h"
