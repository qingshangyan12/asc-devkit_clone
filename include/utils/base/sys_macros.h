/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file sys_macros.h
 * \brief
 */
#ifndef INCLUDE_UTILS_BASE_SYS_MACROS_H
#define INCLUDE_UTILS_BASE_SYS_MACROS_H

#include "impl/utils/sys_macros.h"

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

#define PRELOAD(len) ASCENDC_PRELOAD_IMPL(len)

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
#define KERNEL_TASK_TYPE_DEFAULT(value)
#else
#ifndef __PLUGIN__KERNEL_TASK_TYPE_DEFAULT_DEFINED__
#define __PLUGIN__KERNEL_TASK_TYPE_DEFAULT_DEFINED__
#define KERNEL_TASK_TYPE_DEFAULT(value) ENABLE_FEATURE_FOR_COMPILE(default, value)
#endif
#endif

#define REGISTER_TILING_DEFAULT(tiling_struct) ENABLE_FEATURE_FOR_TILING(default, tiling_struct)

#define TILING_KEY_IS(k) (TILING_KEY_VAR == (k))

#endif
